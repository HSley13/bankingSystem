#include "database.h"
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <vector>

#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDate>
#include <QInputDialog>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

sql::Connection *connection_setup(connection_details *ID)
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *connection;

        driver = sql::mysql::get_driver_instance();
        connection = driver->connect(ID->server, ID->user, ID->password);

        connection->setSchema("bankingSystemDatabase");

        return connection;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return NULL;
    }
}

double check_balance(sql::Connection *connection, int account_number)
{
    try
    {
        double balance;

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
            return 0.0;

        balance = result->getDouble("balance");

        return balance;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return 0.0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return 0.0;
    }
}

void Qt_display_balance(sql::Connection *connection, int account_number)
{
    try
    {

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
            return;

        QString info_text = "Your Balance is: " + QString::number(static_cast<double>(result->getDouble("balance")));

        QMessageBox::information(nullptr, "Balance Check", info_text);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void call_insert_or_update_hashed_password(sql::Connection *connection, int account_number, const std::string hash_password)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL insert_or_update_hashed_password(?, ?);"));
        prep_statement->setInt(1, account_number);
        prep_statement->setString(2, hash_password);

        prep_statement->executeUpdate();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

void Transactions::insert_transactions(sql::Connection *connection, int account_number, std::string details, double amount)
{
    try
    {
        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO " + table_name + " VALUES ( CONCAT(?, ?), CURDATE(), CURTIME());"));
        prep_statement->setString(1, details);
        prep_statement->setDouble(2, amount);

        prep_statement->executeUpdate();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::deposit(sql::Connection *connection, const double amount_to_deposit, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("UPDATE transactions SET deposit = ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_deposit);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        insert_transactions(connection, account_number, "New Money Deposited, Sum of ", amount_to_deposit);

        std::cout << "You have deposited: $" << amount_to_deposit << " and your new Balance is: $" << check_balance(connection, account_number) << std::endl;
        std::cout << std::endl;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::Qt_deposit(sql::Connection *connection, const double amount_to_deposit, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("UPDATE transactions SET deposit = ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_deposit);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        BANK::apply_interest_rate_to_balance(connection, account_number);

        insert_transactions(connection, account_number, "New Money Deposited, Sum of ", amount_to_deposit);

        QString info = "You have deposited: $" + QString::number(static_cast<double>(amount_to_deposit)) + " and Your new balance is: $" + QString::number(static_cast<double>(check_balance(connection, account_number))) + " ";

        QMessageBox::information(nullptr, "New Deposit", info);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::withdrawal(sql::Connection *connection, const double amount_to_withdraw, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("UPDATE transactions SET withdrawal = ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_withdraw);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        std::cout << "You have withdrawn: $" << amount_to_withdraw << ", and your new Balance is: $" << check_balance(connection, account_number) << std::endl;
        std::cout << std::endl;

        insert_transactions(connection, account_number, "New Money Withdrawn, Sum of: $", amount_to_withdraw);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::Qt_withdrawal(sql::Connection *connection, const double amount_to_withdraw, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("UPDATE transactions SET withdrawal = ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_withdraw);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        insert_transactions(connection, account_number, "New Money Withdrawn, Sum of: $", amount_to_withdraw);

        QString info = "You have withdrawn: $" + QString::number(static_cast<double>(amount_to_withdraw)) + " and Your new balance is: $" + QString::number(static_cast<double>(check_balance(connection, account_number))) + " ";

        QMessageBox::information(nullptr, "New Withdrawal", info);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::transfer(sql::Connection *connection, const double amount_to_transfer, int account_number1, int account_number2)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * from accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number2);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
        {
            std::cout << "Acount Number which will reiceive the money is not found in our database, check and try again" << std::endl;

            return;
        }

        std::unique_ptr<sql::PreparedStatement> prep_statement_sender(connection->prepareStatement("UPDATE transactions SET transfer = ? WHERE account_number = ?;"));
        prep_statement_sender->setDouble(1, amount_to_transfer);
        prep_statement_sender->setInt(2, account_number1);
        prep_statement_sender->executeUpdate();

        insert_transactions(connection, account_number1, "Money Transferred to " + std::to_string(account_number2) + ", Amount of: $", amount_to_transfer);

        std::unique_ptr<sql::PreparedStatement> prep_statement_receiver(connection->prepareStatement("UPDATE transactions SET receive = ? WHERE account_number = ?;"));
        prep_statement_receiver->setDouble(1, amount_to_transfer);
        prep_statement_receiver->setInt(2, account_number2);
        prep_statement_receiver->executeUpdate();

        insert_transactions(connection, account_number2, "Money Received from " + std::to_string(account_number1) + ", Amount of: $", amount_to_transfer);

        std::cout << "You have sent: $" << amount_to_transfer << " to the account number: " << account_number2 << " and Your new balance is: $" << check_balance(connection, account_number1) << std::endl;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::Qt_transfer(sql::Connection *connection, const double amount_to_transfer, int account_number1, int account_number2)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * from accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number2);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
        {
            QMessageBox::warning(nullptr, "Error", "Acount Number which will reiceive the money is not found in our database, check and try again");

            return;
        }

        std::unique_ptr<sql::PreparedStatement> prep_statement_sender(connection->prepareStatement("UPDATE transactions SET transfer = ? WHERE account_number = ?;"));
        prep_statement_sender->setDouble(1, amount_to_transfer);
        prep_statement_sender->setInt(2, account_number1);
        prep_statement_sender->executeUpdate();

        insert_transactions(connection, account_number1, "Money Transferred to " + std::to_string(account_number2) + ", Amount of: $", amount_to_transfer);

        std::unique_ptr<sql::PreparedStatement> prep_statement_receiver(connection->prepareStatement("UPDATE transactions SET receive = ? WHERE account_number = ?;"));
        prep_statement_receiver->setDouble(1, amount_to_transfer);
        prep_statement_receiver->setInt(2, account_number2);
        prep_statement_receiver->executeUpdate();

        insert_transactions(connection, account_number2, "Money Received from " + std::to_string(account_number1) + ", Amount of: $", amount_to_transfer);

        QString info = "You have sent $" + QString::number(static_cast<double>(amount_to_transfer)) + " to the account number " + QString::number(account_number2) + " and your new balance is: $" + QString::number(static_cast<double>(check_balance(connection, account_number1))) + " ";

        QMessageBox::information(nullptr, "New Withdrawal", info);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::borrow(sql::Connection *connection, const double amount_to_borrow, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("Update accounts set balance = balance + ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_borrow);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        std::cout << "You have borrowed: $" << amount_to_borrow << ", and your new Balance is: $" << check_balance(connection, account_number) << std::endl;
        std::cout << std::endl;

        insert_transactions(connection, account_number, "New Money Borrowed, Sum of ", amount_to_borrow);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::Qt_borrow(sql::Connection *connection, const double amount_to_borrow, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("Update accounts set balance = balance + ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_borrow);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        insert_transactions(connection, account_number, "New Money Borrowed, Sum of ", amount_to_borrow);

        QString info = "You have Borrowed: $" + QString::number(static_cast<double>(amount_to_borrow)) + " and Your new balance is: $" + QString::number(static_cast<double>(check_balance(connection, account_number))) + " ";

        QMessageBox::information(nullptr, "New Borrowal", info);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::display_transactions_history(sql::Connection *connection, int account_number)
{
    try
    {
        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM " + table_name + ";"));

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        while (result->next())
        {
            std::cout << "Transaction_details: " << result->getString("transaction_details") << " on " << result->getString("date") << " at " << result->getString("time") << std::endl;
            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << std::endl;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::Qt_display_transactions_history(sql::Connection *connection, int account_number)
{
    try
    {
        QTableWidget *table = new QTableWidget();
        table->setRowCount(0);
        table->setColumnCount(3);

        table->setHorizontalHeaderLabels(QStringList() << "Transaction Details"
                                                       << "Date"
                                                       << "Time");
        table->horizontalHeader()->setStyleSheet("color: black;"
                                                 "background-color: beige;");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM " + table_name + ";"));

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        while (result->next())
        {
            items << new QTableWidgetItem(QString::fromStdString(result->getString("transaction_details")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("date")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("time")));

            table->insertRow(row);
            table->setItem(row, 0, items[0]);
            table->setItem(row, 1, items[1]);
            table->setItem(row, 2, items[2]);

            row++;

            items.clear();
        }

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(500, 700);
        table->show();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::display_specific_transactions_history(sql::Connection *connection, int account_number, std::string date, int choice)
{
    try
    {
        std::string current_date = QDate::currentDate().toString(Qt::ISODate).toStdString();

        if (current_date < date)
        {
            std::cerr << "The Entered date can't be Greater than the Current Date!!!!" << std::endl;

            return;
        }

        std::string sign;

        if (choice == 0)
            sign = "<";
        else if (choice == 1)
            sign = ">";
        else
            sign = "=";

        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM " + table_name + " WHERE date " + sign + " ?;"));
        prep_statement->setString(1, date);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        while (result->next())
        {
            std::cout << "Transaction_details: " << result->getString("transaction_details") << " on " << result->getString("date") << " at " << result->getString("time") << std::endl;
            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << std::endl;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::Qt_display_specific_transactions_history(sql::Connection *connection, int account_number, std::string date, int choice)
{
    try
    {
        std::string current_date = QDate::currentDate().toString(Qt::ISODate).toStdString();

        if (current_date < date)
        {
            QMessageBox::warning(nullptr, "!!!!", "The Entered date can't be Greater than the Current Date !!!!");

            return;
        }

        std::string sign;

        if (choice == 0)
            sign = "<";
        else if (choice == 1)
            sign = ">";
        else
            sign = "=";

        QTableWidget *table = new QTableWidget();
        table->setRowCount(0);
        table->setColumnCount(3);

        table->setHorizontalHeaderLabels(QStringList() << "Transaction Details"
                                                       << "Date"
                                                       << "Time");
        table->horizontalHeader()->setStyleSheet("color: black;"
                                                 "background-color: beige;");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM " + table_name + " WHERE date " + sign + " ?;"));
        prep_statement->setString(1, date);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        while (result->next())
        {
            items << new QTableWidgetItem(QString::fromStdString(result->getString("transaction_details")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("date")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("time")));

            table->insertRow(row);
            table->setItem(row, 0, items[0]);
            table->setItem(row, 1, items[1]);
            table->setItem(row, 2, items[2]);

            row++;

            items.clear();
        }

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(500, 700);
        table->show();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Transactions::insert_borrowal(sql::Connection *connection, int account_number, const double amount_to_borrow, const double borrowal_interest_rate)
{
    try
    {

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO borrowal_record (account_number, borrowed_amount, interest_rate, initial_timestamp) VALUES (?, ?, ?, CURRENT_TIMESTAMP);"));
        prep_statement->setInt(1, account_number);
        prep_statement->setDouble(2, amount_to_borrow);
        prep_statement->setDouble(3, borrowal_interest_rate);

        prep_statement->executeUpdate();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR_1: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
bool Account::are_all_same(int phone_number)
{

    std::string phone_number_str = std::to_string(phone_number);

    for (size_t i = 1; i < phone_number_str.length(); i++)
    {
        if (phone_number_str[i] != phone_number_str[0])
            return false;
    }

    return true;
}

void Account::create_account(sql::Connection *connection, int account_number, std::string national_ID, std::string first_name, std::string last_name, std::string date_birth, int phone_number, std::string email, std::string address, const double balance, const double interest_rate, std::string hash_password, std::string question, std::string answer)
{
    try
    {
        QDate birth_date = QDate::fromString(QString::fromStdString(date_birth), Qt::ISODate);
        QDate current_date = QDate::currentDate();

        if ((birth_date > current_date) || (birth_date.addYears(18) > current_date))
        {
            std::cout << "You are Less than 18 years old , You aren't allowed to create an account on your own or Invalid Birth Date" << std::endl;

            return;
        }

        if (Account::are_all_same(phone_number))
        {
            std::cerr << "Enter a valid Phone number where all the digits aren't the same" << std::endl;

            return;
        }

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO accounts (national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, interest_rate) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"));
        prep_statement->setString(1, national_ID);
        prep_statement->setString(2, first_name);
        prep_statement->setString(3, last_name);
        prep_statement->setString(4, date_birth);
        prep_statement->setInt(5, phone_number);
        prep_statement->setString(6, email);
        prep_statement->setString(7, address);
        prep_statement->setDouble(8, balance);
        prep_statement->setDouble(9, interest_rate);

        prep_statement->executeUpdate();

        std::cout << "This is Your Account Number, remember it because you will need it to gain access to everything you want to do in the future:  ";

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("SELECT account_number FROM accounts WHERE national_ID = ?;"));
        prep_statement->setString(1, national_ID);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
            return;

        account_number = result->getInt("account_number");

        std::cout << account_number << std::endl;

        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("CREATE TABLE " + table_name + " (transaction_details VARCHAR(100), date DATE, time TIME);"));
        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO " + table_name + " VALUES (?, CURDATE(), CURTIME() );"));
        prep_statement->setString(1, "Account Created");

        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO password_recovery VALUES (?, ?, ?);"));
        prep_statement->setInt(1, account_number);
        prep_statement->setString(2, question);
        prep_statement->setString(3, answer);

        prep_statement->executeUpdate();

        call_insert_or_update_hashed_password(connection, account_number, hash_password);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Account::Qt_create_account(sql::Connection *connection, int account_number, std::string national_ID, std::string first_name, std::string last_name, std::string date_birth, int phone_number, std::string email, std::string address, const double balance, const double interest_rate, std::string hash_password, std::string question, std::string answer)
{
    try
    {
        QDate birth_date = QDate::fromString(QString::fromStdString(date_birth), Qt::ISODate);
        QDate current_date = QDate::currentDate();

        if ((birth_date > current_date) || (birth_date.addYears(18) > current_date))
        {
            QMessageBox::warning(nullptr, "!!!!!", "You are Less than 18 years old, You aren't allowed to create an account on your own or Invalid Birth Date");

            return;
        }

        if (Account::are_all_same(phone_number))
        {
            QMessageBox::warning(nullptr, "Phone Number Error", "Enter a valid Phone number where all the digits aren't the same");

            return;
        }

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO accounts (national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, interest_rate) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"));
        prep_statement->setString(1, national_ID);
        prep_statement->setString(2, first_name);
        prep_statement->setString(3, last_name);
        prep_statement->setString(4, date_birth);
        prep_statement->setInt(5, phone_number);
        prep_statement->setString(6, email);
        prep_statement->setString(7, address);
        prep_statement->setDouble(8, balance);
        prep_statement->setDouble(9, interest_rate);

        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("SELECT account_number FROM accounts WHERE national_ID = ?;"));
        prep_statement->setString(1, national_ID);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
            return;

        account_number = result->getInt("account_number");

        QString info = " " + QString::number(account_number) + " is Your Account Number, remember it because you will need it to gain access to everything you want to do in the future";
        QMessageBox::information(nullptr, "Account Number", info);

        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("CREATE TABLE " + table_name + " (transaction_details VARCHAR(100), date DATE, time TIME );"));
        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO " + table_name + " VALUES (?, CURDATE(), CURTIME() );"));
        prep_statement->setString(1, "Account Created");

        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO password_recovery VALUES (?, ?, ?);"));
        prep_statement->setInt(1, account_number);
        prep_statement->setString(2, question);
        prep_statement->setString(3, answer);

        prep_statement->executeUpdate();

        call_insert_or_update_hashed_password(connection, account_number, hash_password);
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Account::remove_accounts(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * from accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
        {
            std::cerr << "Acount Number not found in our database, check and try again" << std::endl;

            return;
        }

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        result = std::unique_ptr<sql::ResultSet>(prep_statement->executeQuery());

        if (result->isBeforeFirst())
        {
            std::cerr << "Aren't allowed to Delete this Account Cause it owes the Bank. First Pay the Debt and then the Deletion will be possible" << std::endl;

            return;
        }

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("DELETE FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("DELETE FROM password_security WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("DELETE FROM transactions WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO " + table_name + " VALUES (?, CURDATE(), CURTIME() );"));
        prep_statement->setString(1, "Account Deleted");
        prep_statement->executeUpdate();

        std::cout << "Account number: " << account_number << " Deleted successfully" << std::endl;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void Account::Qt_remove_accounts(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * from accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
        {
            QMessageBox::warning(nullptr, "Error", "Acount Number not found in our database, check and try again");

            return;
        }

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        result = std::unique_ptr<sql::ResultSet>(prep_statement->executeQuery());

        if (result->next())
        {
            QMessageBox::warning(nullptr, "Error", "Aren't allowed to Delete this Account Cause it owes the Bank. First Pay the Debt and then the Deletion will be possible");

            return;
        }

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("DELETE FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("DELETE FROM password_security WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("DELETE FROM transactions WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        std::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO " + table_name + " VALUES (?, CURDATE(), CURTIME() );"));
        prep_statement->setString(1, "Account Deleted");
        prep_statement->executeUpdate();

        QMessageBox::information(nullptr, "OK", "Account deleted Successfully");
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string BANK::generate_random_salt(std::size_t len)
{
    try
    {
        std::string valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

        std::random_device rd;
        std::mt19937 generator(rd());

        std::uniform_int_distribution<> distribution(0, valid_chars.size() - 1);

        std::string salt;
        for (size_t i = 0; i < len; i++)
            salt.push_back(valid_chars[distribution(generator)]);

        return salt;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return "";
    }
}

std::string BANK::hashing_password(std::string &password)
{
    try
    {
        const size_t SALT_LENGTH = 32;

        std::string salt = generate_random_salt(SALT_LENGTH);

        const uint32_t t_cost = 2;
        const uint32_t m_cost = 32;
        const uint32_t parallelism = 1;
        const uint32_t hash_length = 32;

        std::string hash;
        hash.resize(hash_length);

        int result = argon2_hash(t_cost, m_cost, parallelism, password.c_str(), password.length(), salt.c_str(), salt.length(), &hash[0], hash.length(), NULL, 0, Argon2_id, ARGON2_VERSION_NUMBER);

        if (result != ARGON2_OK)
        {
            std::cout << "Error Hashing Password" << std::endl;

            return "";
        }

        std::string hashed_password = salt + hash;

        return hashed_password;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return "";
    }
}

std::string BANK::retrieve_hashed_password(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT hashed_password FROM password_security WHERE account_number = ?"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        std::string hashed_password;

        if (!result->next())
        {
            std::cout << "Error retriving Hash Password! The Account " << account_number << " entered doesn't exist in our database, Check and try again" << std::endl;
            std::cout << std::endl;

            return "";
        }

        hashed_password = result->getString("hashed_password");

        return hashed_password;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return "";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return "";
    }
}

std::string BANK::Qt_retrieve_hashed_password(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT hashed_password FROM password_security WHERE account_number = ?"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        std::string hashed_password;

        if (!result->next())
        {
            QMessageBox::warning(nullptr, "Warning!", "Error Retriving hash password! The Account entered doesn't exist in our database, Check and try again");

            return "";
        }

        hashed_password = result->getString("hashed_password");

        return hashed_password;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return "";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return "";
    }
}

std::string BANK::retrieve_adm_hashed_password(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT hashed_password FROM adm_password_security WHERE account_number = ?"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        std::string hashed_password;

        if (!result->next())
        {
            std::cout << "Error retriving Hash Password! The Account " << account_number << " entered doesn't exist in our database, Check and try again" << std::endl;

            return "";
        }

        hashed_password = result->getString("hashed_password");

        return hashed_password;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return "";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return "";
    }
}

std::string BANK::Qt_retrieve_adm_hashed_password(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT hashed_password FROM adm_password_security WHERE account_number = ?"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        std::string hashed_password;

        if (!result->next())
        {
            QMessageBox::warning(nullptr, "Warning!", "Error Retriving hash password! The Account entered doesn't exist in our database, Check and try again");

            return "";
        }

        hashed_password = result->getString("hashed_password");

        return hashed_password;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return "";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return "";
    }
}

bool BANK::verifying_password(std::string password, std::string &hashed_password)
{
    try
    {
        const uint32_t t_cost = 2;
        const uint32_t m_cost = 32;
        const uint32_t parallelism = 1;
        const uint32_t hash_length = 32;

        const size_t SALT_LENGTH = hashed_password.length() - hash_length;

        std::string hash;
        hash.resize(hash_length);

        int result = argon2_hash(t_cost, m_cost, parallelism, password.c_str(), password.length(), hashed_password.c_str(), SALT_LENGTH, &hash[0], hash.length(), NULL, 0, Argon2_id, ARGON2_VERSION_NUMBER);

        if (result != ARGON2_OK)
        {
            std::cout << "Error Verifying Password" << std::endl;

            return false;
        }

        return !hashed_password.substr(SALT_LENGTH, hash_length).compare(hash);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return false;
    }
}

std::string BANK::retrieve_interest_rate_initial_timestamp(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT initial_timestamp FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        std::string initial_timestamp;

        if (!result->next())
        {
            std::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std::endl;

            return "";
        }

        initial_timestamp = result->getString("initial_timestamp");

        return initial_timestamp;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return "";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return "";
    }
}

int BANK::calculate_interest_rate_time_elapsed(sql::Connection *connection, const std::string initial_timestamp)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement_now(connection->prepareStatement("SELECT NOW() AS time_now;"));
        std::unique_ptr<sql::ResultSet> result_now(prep_statement_now->executeQuery());

        std::string current_time;
        if (result_now->next())
            current_time = result_now->getString("time_now");

        std::unique_ptr<sql::PreparedStatement> prep_statement_diff(connection->prepareStatement("SELECT TIMESTAMPDIFF(DAY, ?, ?) AS time_elapsed;"));
        prep_statement_diff->setString(1, initial_timestamp);
        prep_statement_diff->setString(2, current_time);

        std::unique_ptr<sql::ResultSet> result_diff(prep_statement_diff->executeQuery());

        int time_elapsed;

        if (result_diff->next())
            time_elapsed = result_diff->getInt("time_elapsed");

        return time_elapsed;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return 1;
    }
}

void BANK::apply_interest_rate_to_balance(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement_interest(connection->prepareStatement("SELECT interest_rate FROM accounts WHERE account_number = ?;"));
        prep_statement_interest->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result_interest(prep_statement_interest->executeQuery());

        if (!result_interest->next())
        {
            std::cout << "Error retrieving interest rate for account number: " << account_number << "Cause it not Found" << std::endl;

            return;
        }

        double interest_rate = result_interest->getDouble("interest_rate");

        int time_elapsed = calculate_interest_rate_time_elapsed(connection, retrieve_interest_rate_initial_timestamp(connection, account_number));

        std::unique_ptr<sql::PreparedStatement> prep_statement_balance(connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;"));
        prep_statement_balance->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result_balance(prep_statement_balance->executeQuery());

        if (!result_balance->next())
        {
            std::cout << "Error retrieving balance for account number: " << account_number << "Cause it not Found" << std::endl;

            return;
        }

        double balance = result_balance->getDouble("balance");

        double interest_rate_plus_balance = (interest_rate * balance * time_elapsed) + balance;

        std::unique_ptr<sql::PreparedStatement> prep_statement_update(connection->prepareStatement("UPDATE accounts SET balance = ? WHERE account_number = ?;"));
        prep_statement_update->setDouble(1, interest_rate_plus_balance);
        prep_statement_update->setInt(2, account_number);

        prep_statement_update->executeUpdate();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

bool BANK::authentification_check(sql::Connection *connection, int account_number, std::string question, std::string answer)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT question, answer FROM password_recovery WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        std::string answer1;

        if (result->next())
        {
            question = result->getString("question");
            answer1 = result->getString("answer");
        }

        std::cout << question << std::endl;
        std::cin >> answer;

        return (!answer1.compare(answer));
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;

        return false;
    }
}

bool BANK::Qt_authentification_check(sql::Connection *connection, int account_number, std::string question, std::string answer)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT question, answer FROM password_recovery WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (result->next())
        {
            question = result->getString("question");
            answer = result->getString("answer");
        }

        QString info = QString::fromStdString(question);

        bool OK;
        QString input = QInputDialog::getText(nullptr, "Confirmation", info, QLineEdit::Normal, "", &OK);

        if (input.isEmpty())
        {
            QMessageBox::warning(nullptr, "void", "Input Empty");

            return false;
        }

        return (!answer.compare(input.toStdString()));
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;

        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;

        return false;
    }
}

bool BANK::authentification_message(sql::Connection *connection, int &account_number, std::string &hash_password)
{
    std::cout << "Enter Account Number: ";
    std::cin >> account_number;
    std::cout << std::endl;

    hash_password = BANK::retrieve_hashed_password(connection, account_number);

    if (hash_password == "")
        return false;

    return true;
}

bool BANK::adm_authentification_message(sql::Connection *connection, int &account_number, std::string &hash_password)
{
    std::cout << "Enter Account Number: ";
    std::cin >> account_number;
    std::cout << std::endl;

    hash_password = BANK::retrieve_adm_hashed_password(connection, account_number);

    if (hash_password == "")
        return false;

    return true;
}

void BANK::create_adm(sql::Connection *connection, int account_number, std::string hash_password)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO adm_password_security VALUES (?, ?) "));
        prep_statement->setInt(1, account_number);
        prep_statement->setString(2, hash_password);

        prep_statement->executeUpdate();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::display_accounts_table(sql::Connection *connection)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts;"));

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (result->isBeforeFirst())
        {
            while (result->next())
            {
                std::cout << "National ID: " << result->getString("national_ID") << " | Account Number: " << result->getInt("account_number") << " | First Name: " << result->getString("first_name");

                std::cout << " | Last Nmae: " << result->getString("last_name") << " | Date of Birth: " << result->getString("date_birth") << " | Phone Number: " << result->getInt("phone_number");

                std::cout << " | Email: " << result->getString("email") << " | Address: " << result->getString("address") << " | Balance: " << result->getDouble("balance") << " | Interest Rate: " << result->getDouble("interest_rate");

                std::cout << " | Initial Timestamp: " << result->getString("initial_timestamp") << std::endl;
                std::cout << std::endl;
                std::cout << std::endl;
            }
        }
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::Qt_display_accounts_table(sql::Connection *connection)
{
    try
    {
        QTableWidget *table = new QTableWidget();

        table->setRowCount(0);
        table->setColumnCount(11);
        table->setHorizontalHeaderLabels(QStringList() << "Account Number"
                                                       << "National ID"
                                                       << "First Name"
                                                       << "Last Name"
                                                       << "Date of Birth"
                                                       << "Phone Number"
                                                       << "Email"
                                                       << "Adress"
                                                       << "Balance"
                                                       << "Interes Rate"
                                                       << "Itinial Timestamp");
        table->horizontalHeader()->setStyleSheet("color: black;"
                                                 "background-color: beige;");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts;"));

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        int row = table->rowCount();

        while (result->next())
        {
            QList<QTableWidgetItem *> items;

            items << new QTableWidgetItem(QString::number(result->getInt("account_number")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("national_ID")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("first_name")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("last_name")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("date_birth")))
                  << new QTableWidgetItem(QString::number(result->getInt("phone_number")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("email")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("address")))
                  << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("balance"))))
                  << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("interest_rate"))))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("initial_timestamp")));

            table->insertRow(row);

            for (int i = 0; i < items.size(); i++)
                table->setItem(row, i, items[i]);

            items.clear();

            row++;
        }

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(900, 500);
        table->show();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::display_specific_accounts(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
        {
            std::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std::endl;

            return;
        }

        std::cout << "National ID: " << result->getString("national_ID") << " | First Name: " << result->getString("first_name");

        std::cout << " | Last Nmae: " << result->getString("last_name") << " | Date of Birth: " << result->getString("date_birth") << " | Phone Number: " << result->getInt("phone_number");

        std::cout << " | Email: " << result->getString("email") << " | Address: " << result->getString("address") << " | Balance: " << result->getDouble("balance") << " | Interest Rate: " << result->getDouble("interest_rate");

        std::cout << " | Initial Timestamp: " << result->getString("initial_timestamp") << std::endl;
        std::cout << std::endl;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::Qt_display_specific_accounts(sql::Connection *connection, int account_number)
{
    try
    {
        QTableWidget *table = new QTableWidget();

        table->setRowCount(0);
        table->setColumnCount(10);
        table->setHorizontalHeaderLabels(QStringList() << "National ID"
                                                       << "First Name"
                                                       << "Last Name"
                                                       << "Date of Birth"
                                                       << "Phone Number"
                                                       << "Email"
                                                       << "Adress"
                                                       << "Balance"
                                                       << "Interes Rate"
                                                       << "Itinial Timestamp");
        table->horizontalHeader()->setStyleSheet("color: black;"
                                                 "background-color: beige;");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        if (!result->next())
        {
            QMessageBox::warning(nullptr, "Warning!", "The Account you enter doesn't exist in our data, Check and try again");

            return;
        }

        items << new QTableWidgetItem(QString::fromStdString(result->getString("national_ID")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("first_name")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("last_name")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("date_birth")))
              << new QTableWidgetItem(QString::number(result->getInt("phone_number")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("email")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("address")))
              << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("balance"))))
              << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("interest_rate"))))
              << new QTableWidgetItem(QString::fromStdString(result->getString("initial_timestamp")));

        table->insertRow(row);

        for (int i = 0; i < items.size(); i++)
            table->setItem(row, i, items[i]);

        items.clear();

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(900, 500);
        table->show();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::display_people_in_debt(sql::Connection *connection)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT accounts.account_number AS A, national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number;"));

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
        {
            std::cout << "There is no People in Debt" << std::endl;
            std::cout << std::endl;

            return;
        }

        while (result->next())
        {
            std::cout << "Account Number: " << result->getInt("A") << " | National ID: " << result->getString("national_ID") << " | First Name: " << result->getString("first_name") << " | Last Name: " << result->getString("last_name") << " | Balance: " << result->getDouble("balance");
            std::cout << " | Account Interest Rate: " << result->getDouble("B") << " | Borrowed Amount: " << result->getDouble("borrowed_amount") << " | Borrowed Amount Interest Rate: " << result->getDouble("C");
            std::cout << " | Borrowed Amount Initial timestamp: " << result->getString("D") << " | Scheduled Time: " << result->getString("scheduled_time") << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::Qt_display_people_in_debt(sql::Connection *connection)
{
    try
    {
        QTableWidget *table = new QTableWidget();
        table->setRowCount(0);
        table->setColumnCount(9);

        table->setHorizontalHeaderLabels(QStringList() << "Account Number"
                                                       << "National ID"
                                                       << "First Name"
                                                       << "Last Name"
                                                       << "Balance"
                                                       << "Account Inerest Rate"
                                                       << "Borrowed Amount"
                                                       << "Borrowed Amount Interest Rate"
                                                       << "Borrowed Amount Initial Timestamp"
                                                       << "Schedule Time");
        table->horizontalHeader()->setStyleSheet("color: black;"
                                                 "background-color: beige;");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT accounts.account_number AS A, national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number;"));

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        if (!result->next())
        {
            QMessageBox::information(nullptr, "!!!!!", "There is no People in Debt");

            return;
        }

        while (result->next())
        {
            items << new QTableWidgetItem(QString::number(result->getInt("A")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("national_ID")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("first_name")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("last_name")))
                  << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("balance"))))
                  << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("B"))))
                  << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("borrowed_amount"))))
                  << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("C"))))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("D")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("scheduled_time")));

            table->insertRow(row);

            for (int i = 0; i < items.size(); i++)
                table->setItem(row, i, items[i]);

            items.clear();

            row++;
        }

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(900, 500);
        table->show();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::display_specific_accounts_in_debt(sql::Connection *connection, int account_number)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number WHERE accounts.account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (!result->next())
        {
            std::cout << "Account " << account_number << " Not in Debt, Verify the Number and try again" << std::endl;

            return;
        }

        std::cout << "National ID: " << result->getString("national_ID") << " | First Name: " << result->getString("first_name") << " | Last Name: " << result->getString("last_name") << " | Balance: " << result->getDouble("balance");

        std::cout << " | Account Interest Rate: " << result->getDouble("B") << " | Borrowed Amount: " << result->getDouble("borrowed_amount") << " | Borrowed Amount Interest Rate: " << result->getDouble("C");

        std::cout << " | Borrowed Amount Initial timestamp: " << result->getString("D") << " | Scheduled Time: " << result->getString("scheduled_time") << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}

void BANK::Qt_display_specific_accounts_in_debt(sql::Connection *connection, int account_number)
{
    try
    {
        QTableWidget *table = new QTableWidget();
        table->setRowCount(0);
        table->setColumnCount(9);

        table->setHorizontalHeaderLabels(QStringList() << "National ID"
                                                       << "First Name"
                                                       << "Last Name"
                                                       << "Balance"
                                                       << "Account Inerest Rate"
                                                       << "Borrowed Amount"
                                                       << "Borrowed Amount Interest Rate"
                                                       << "Borrowed Amount Initial Timestamp"
                                                       << "Schedule Time");
        table->horizontalHeader()->setStyleSheet("color: black;"
                                                 "background-color: beige;");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number WHERE accounts.account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        if (!result->next())
        {
            QMessageBox::warning(nullptr, "Warning!", "The Account you enter either doesn't exist or doesn't owe the Bank, Check and try again");

            return;
        }

        items << new QTableWidgetItem(QString::fromStdString(result->getString("national_ID")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("first_name")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("last_name")))
              << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("balance"))))
              << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("B"))))
              << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("borrowed_amount"))))
              << new QTableWidgetItem(QString::number(static_cast<double>(result->getDouble("C"))))
              << new QTableWidgetItem(QString::fromStdString(result->getString("D")))
              << new QTableWidgetItem(QString::fromStdString(result->getString("scheduled_time")));

        table->insertRow(row);

        for (int i = 0; i < items.size(); i++)
            table->setItem(row, i, items[i]);

        items.clear();

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(900, 500);
        table->show();
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "C++ ERROR: " << e.what() << std::endl;
    }
}