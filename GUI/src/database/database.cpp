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

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

sql ::Connection *connection_setup(connection_details *ID)
{
    try
    {
        sql ::mysql ::MySQL_Driver *driver;
        sql ::Connection *connection;

        driver = sql ::mysql ::get_driver_instance();
        connection = driver->connect(ID->server, ID->user, ID->password);

        connection->setSchema("bankingSystemDatabase");

        return connection;
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
        return NULL;
    }
}

double check_balance(sql ::Connection *connection, int account_number)
{
    try
    {
        double balance;

        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->next())
        {
            balance = result->getDouble("balance");
            return balance;
        }
        else
        {
            std ::cout << "Account Not Found, Verify the Number again before trying" << std ::endl;
            return 0.0;
        }
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
        return 0.0;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return 0.0;
    }
}

void call_insert_or_update_hashed_password(sql ::Connection *connection, int account_number, const std ::string hash_password)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("CALL insert_or_update_hashed_password(?, ?);"));
        prep_statement->setInt(1, account_number);
        prep_statement->setString(2, hash_password);

        prep_statement->executeUpdate();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

void Transactions ::insert_transactions(sql ::Connection *connection, int account_number, std ::string details, double amount)
{
    try
    {
        std ::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std ::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO " + table_name + " VALUES ( CONCAT(?, ?), NOW() );"));
        prep_statement->setString(1, details);
        prep_statement->setDouble(2, amount);

        prep_statement->executeUpdate();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Transactions ::deposit(sql ::Connection *connection, const double amount_to_deposit, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("UPDATE transactions SET deposit = ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_deposit);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        std ::cout << "You have deposited: $" << amount_to_deposit << ", and your new Balance is: $" << check_balance(connection, account_number) << std ::endl;
        std ::cout << std ::endl;

        insert_transactions(connection, account_number, "New Money Deposited, Sum of ", amount_to_deposit);
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Transactions ::withdrawal(sql ::Connection *connection, const double amount_to_withdraw, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("UPDATE transactions SET withdrawal = ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_withdraw);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        std ::cout << "You have withdrawn: $" << amount_to_withdraw << ", and your new Balance is: $" << check_balance(connection, account_number) << std ::endl;
        std ::cout << std ::endl;

        insert_transactions(connection, account_number, "New Money Withdrawn, Sum of: $", amount_to_withdraw);
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Transactions ::transfer(sql ::Connection *connection, const double amount_to_transfer, int account_number1, int account_number2)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement_sender(connection->prepareStatement("UPDATE transactions SET transfer = ? WHERE account_number = ?;"));
        prep_statement_sender->setDouble(1, amount_to_transfer);
        prep_statement_sender->setInt(2, account_number1);
        prep_statement_sender->executeUpdate();

        insert_transactions(connection, account_number1, "Money Transferred to " + std::to_string(account_number2) + ", Amount of: $", amount_to_transfer);

        std ::unique_ptr<sql ::PreparedStatement> prep_statement_receiver(connection->prepareStatement("UPDATE transactions SET receive = ? WHERE account_number = ?;"));
        prep_statement_receiver->setDouble(1, amount_to_transfer);
        prep_statement_receiver->setInt(2, account_number2);
        prep_statement_receiver->executeUpdate();

        insert_transactions(connection, account_number2, "Money Received from " + std::to_string(account_number1) + ", Amount of: $", amount_to_transfer);

        std ::cout << "You have sent: $" << amount_to_transfer << " to the account number: " << account_number2 << " and Your new balance is: $" << check_balance(connection, account_number1) << std ::endl;
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Transactions ::borrow(sql ::Connection *connection, const double amount_to_borrow, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("Update accounts set balance = balance + ? WHERE account_number = ?;"));
        prep_statement->setDouble(1, amount_to_borrow);
        prep_statement->setInt(2, account_number);

        prep_statement->executeUpdate();

        std ::cout << "You have borrowed: $" << amount_to_borrow << ", and your new Balance is: $" << check_balance(connection, account_number) << std ::endl;
        std ::cout << std ::endl;

        insert_transactions(connection, account_number, "New Money Borrowed, Sum of ", amount_to_borrow);
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Transactions ::display_transactions_history(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM " + table_name + ";"));

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (!result)
        {
            std ::cout << "The Account Number entered is not registered in out Database" << std ::endl;
            return;
        }

        if (result->isBeforeFirst())
        {
            while (result->next())
            {
                std ::cout << "Transaction_details: " << result->getString("transaction_details") << " on " << result->getString("date_time") << std ::endl;
                std ::cout << std ::endl;
            }

            std ::cout << std ::endl;
            std ::cout << std ::endl;
        }
        else
            std ::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std ::endl;
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Transactions ::Qt_display_transactions_history(sql ::Connection *connection, int account_number)
{
    try
    {
        QTableWidget *table = new QTableWidget();
        table->setRowCount(0);
        table->setColumnCount(2);

        table->setHorizontalHeaderLabels(QStringList() << "Transaction Details"
                                                       << "Date & Time");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std ::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM " + table_name + ";"));

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        if (!result)
        {
            QMessageBox *message = new QMessageBox();
            message->warning(nullptr, "Warning!", "The Account you enter doesn't exist in our data, Check and try again");
            message->show();

            delete message;
            return;
        }

        while (result->next())
        {
            items << new QTableWidgetItem(QString::fromStdString(result->getString("transaction_details")))
                  << new QTableWidgetItem(QString::fromStdString(result->getString("date_time")));

            table->insertRow(row);
            table->setItem(row, 0, items[0]);
            table->setItem(row, 1, items[1]);

            row++;

            items.clear();
        }

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(400, 400);
        table->show();
    }
    catch (const sql ::SQLException &e)
    {
        QMessageBox *message = new QMessageBox();
        message->warning(nullptr, "Warning!", "The Account you entered doesn't exist in our data. Check and try again");
        message->show();

        delete message;
        return;

        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Transactions ::insert_borrowal(sql ::Connection *connection, int account_number, const double amount_to_borrow, const double borrowal_interest_rate)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO borrowal_record (account_number, borrowed_amount, interest_rate, initial_timestamp) VALUES (?, ?, ?, CURRENT_TIMESTAMP);"));
        prep_statement->setInt(1, account_number);
        prep_statement->setDouble(2, amount_to_borrow);
        prep_statement->setDouble(3, borrowal_interest_rate);

        prep_statement->executeUpdate();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
void Account::create_account(sql ::Connection *connection, int account_number, std ::string national_ID, std ::string first_name, std ::string last_name, std ::string date_birth, int phone_number, std ::string email, std ::string address, const double balance, const double interest_rate, std ::string password, std ::string hash_password)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO accounts (national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, interest_rate) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"));
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

        std ::cout << "This is Your Account Number, remember it because you will need it to gain access to everything you want to do in the future:  ";

        prep_statement = std ::unique_ptr<sql ::PreparedStatement>(connection->prepareStatement("SELECT account_number FROM accounts WHERE national_ID = ?;"));
        prep_statement->setString(1, national_ID);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->next())
        {
            account_number = result->getInt("account_number");

            std ::cout << account_number << std ::endl;

            std ::string table_name = "NO";
            table_name.append(std::to_string(account_number));

            prep_statement = std ::unique_ptr<sql ::PreparedStatement>(connection->prepareStatement("CREATE TABLE " + table_name + " (transaction_details VARCHAR(100), date_time DATETIME DEFAULT NOW() );"));
            prep_statement->executeUpdate();

            prep_statement = std ::unique_ptr<sql ::PreparedStatement>(connection->prepareStatement("INSERT INTO " + table_name + " VALUES (?, NOW() );"));
            prep_statement->setString(1, "Account Created");
            prep_statement->executeUpdate();
        }

        call_insert_or_update_hashed_password(connection, account_number, hash_password);
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void Account ::remove_accounts(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->isBeforeFirst())
        {
            std ::cout << "Aren't allowed to Delete this Account Cause it owes the Bank. First Pay the Debt and then the Deletion will be possible" << std ::endl;

            QMessageBox *message = new QMessageBox();
            message->warning(nullptr, "Aren't allowed to Delete this Account Cause it owes the Bank. First Pay the Debt and then the Deletion will be possible", "Aren't allowed to Delete this Account Cause it owes the Bank. First Pay the Debt and then the Deletion will be possible");

            delete message;

            return;
        }

        prep_statement = std ::unique_ptr<sql ::PreparedStatement>(connection->prepareStatement("DELETE FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        prep_statement = std ::unique_ptr<sql ::PreparedStatement>(connection->prepareStatement("DELETE FROM password_security WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        prep_statement = std ::unique_ptr<sql ::PreparedStatement>(connection->prepareStatement("DELETE FROM transactions WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);
        prep_statement->executeUpdate();

        std ::string table_name = "NO";
        table_name.append(std::to_string(account_number));

        prep_statement = std ::unique_ptr<sql ::PreparedStatement>(connection->prepareStatement("INSERT INTO " + table_name + " VALUES (?, NOW() );"));
        prep_statement->setString(1, "Account Deleted");
        prep_statement->executeUpdate();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
std ::string BANK ::generate_random_salt(std ::size_t len)
{
    try
    {
        std ::string valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

        std ::random_device rd;
        std ::mt19937 generator(rd());

        std ::uniform_int_distribution<> distribution(0, valid_chars.size() - 1);

        std ::string salt;
        for (size_t i = 0; i < len; i++)
            salt.push_back(valid_chars[distribution(generator)]);

        return salt;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return "";
    }
}

std ::string BANK ::hashing_password(std ::string &password)
{
    try
    {
        const size_t SALT_LENGTH = 32;

        std ::string salt = generate_random_salt(SALT_LENGTH);

        const uint32_t t_cost = 2;
        const uint32_t m_cost = 32;
        const uint32_t parallelism = 1;
        const uint32_t hash_length = 32;

        std ::string hash;
        hash.resize(hash_length);

        int result = argon2_hash(t_cost, m_cost, parallelism, password.c_str(), password.length(), salt.c_str(), salt.length(), &hash[0], hash.length(), NULL, 0, Argon2_id, ARGON2_VERSION_NUMBER);

        if (result != ARGON2_OK)
        {
            std ::cout << "Error Hashing Password" << std ::endl;
            return "";
        }

        std ::string hashed_password = salt + hash;

        return hashed_password;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return "";
    }
}

std ::string BANK ::retrieve_hashed_password(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT hashed_password FROM password_security WHERE account_number = ?"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        std ::string hashed_password;

        if (result->next())
        {
            hashed_password = result->getString("hashed_password");
            return hashed_password;
        }
        else
        {
            std ::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std ::endl;
            return "";
        }
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
        return "";
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return "";
    }
}

std ::string BANK ::retrieve_adm_hashed_password(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT hashed_password FROM adm_password_security WHERE account_number = ?"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        std ::string hashed_password;

        if (result->next())
        {
            hashed_password = result->getString("hashed_password");
            return hashed_password;
        }
        else
        {
            std ::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std ::endl;
            return "";
        }
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
        return "";
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return "";
    }
}

bool BANK ::verifying_password(std ::string password, std ::string &hashed_password)
{
    try
    {
        const uint32_t t_cost = 2;
        const uint32_t m_cost = 32;
        const uint32_t parallelism = 1;
        const uint32_t hash_length = 32;

        const size_t SALT_LENGTH = hashed_password.length() - hash_length;

        std ::string hash;
        hash.resize(hash_length);

        int result = argon2_hash(t_cost, m_cost, parallelism, password.c_str(), password.length(), hashed_password.c_str(), SALT_LENGTH, &hash[0], hash.length(), NULL, 0, Argon2_id, ARGON2_VERSION_NUMBER);

        if (result != ARGON2_OK)
        {
            std ::cout << "Error Verifying Password" << std ::endl;
            return false;
        }

        return !hashed_password.substr(SALT_LENGTH, hash_length).compare(hash);
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return false;
    }
}

std ::string BANK ::retrieve_interest_rate_initial_timestamp(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT initial_timestamp FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        std::string initial_timestamp;

        if (result->next())
        {
            initial_timestamp = result->getString("initial_timestamp");
            return initial_timestamp;
        }
        else
        {
            std ::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std ::endl;
            return "";
        }
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
        return "";
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return "";
    }
}

int BANK ::calculate_interest_rate_time_elapsed(sql ::Connection *connection, const std ::string initial_timestamp)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement_now(connection->prepareStatement("SELECT NOW() AS time_now;"));
        std ::unique_ptr<sql ::ResultSet> result_now(prep_statement_now->executeQuery());

        std ::string current_time;
        if (result_now->next())
            current_time = result_now->getString("time_now");

        std ::unique_ptr<sql ::PreparedStatement> prep_statement_diff(connection->prepareStatement("SELECT TIMESTAMPDIFF(DAY, ?, ?) AS time_elapsed;"));
        prep_statement_diff->setString(1, initial_timestamp);
        prep_statement_diff->setString(2, current_time);

        std ::unique_ptr<sql ::ResultSet> result_diff(prep_statement_diff->executeQuery());

        int time_elapsed;

        if (result_diff->next())
            time_elapsed = result_diff->getInt("time_elapsed");

        return time_elapsed;
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
        return 1;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << e.what() << std ::endl;
        return 1;
    }
}

void BANK ::apply_interest_rate_to_balance(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement_interest(connection->prepareStatement("SELECT interest_rate FROM accounts WHERE account_number = ?;"));
        prep_statement_interest->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result_interest(prep_statement_interest->executeQuery());

        if (!result_interest->next())
        {
            std ::cout << "Error retrieving interest rate for account number: " << account_number << "Cause it not Found" << std ::endl;
            return;
        }

        double interest_rate = result_interest->getDouble("interest_rate");

        int time_elapsed = calculate_interest_rate_time_elapsed(connection, retrieve_interest_rate_initial_timestamp(connection, account_number));

        std ::unique_ptr<sql ::PreparedStatement> prep_statement_balance(connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;"));
        prep_statement_balance->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result_balance(prep_statement_balance->executeQuery());

        if (!result_balance->next())
        {
            std ::cout << "Error retrieving balance for account number: " << account_number << "Cause it not Found" << std ::endl;
            return;
        }

        double balance = result_balance->getDouble("balance");

        double interest_rate_plus_balance = (interest_rate * balance * time_elapsed) + balance;

        std ::unique_ptr<sql ::PreparedStatement> prep_statement_update(connection->prepareStatement("UPDATE accounts SET balance = ? WHERE account_number = ?;"));
        prep_statement_update->setDouble(1, interest_rate_plus_balance);
        prep_statement_update->setInt(2, account_number);

        prep_statement_update->executeUpdate();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

bool BANK ::authentification_check(sql ::Connection *connection, int account_number, std ::string national_ID, std ::string date_birth)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT national_ID, date_birth FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        std ::string national_ID1, date_birth1;

        if (result->next())
        {
            national_ID1 = result->getString("national_ID");
            date_birth1 = result->getString("date_birth");
        }

        if (national_ID1.compare(national_ID) && date_birth1.compare(date_birth))
            return false;

        else
            return true;
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
        return false;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
        return false;
    }
}

void BANK ::authentification_message(sql ::Connection *connection, int &account_number, std ::string &hash_password)
{
    std ::cout << "Enter Account Number: ";
    std ::cin >> account_number;
    std ::cout << std ::endl;

    hash_password = BANK ::retrieve_hashed_password(connection, account_number);

    std ::cout << "What is your Password: " << std ::endl;
    std ::cout << "You have 3 Chances" << std ::endl;
}

void BANK ::create_adm(sql ::Connection *connection, int account_number, std ::string hash_password)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO adm_password_security VALUES (?, ?) "));
        prep_statement->setInt(1, account_number);
        prep_statement->setString(2, hash_password);

        prep_statement->executeUpdate();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::display_accounts_table(sql ::Connection *connection)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts;"));

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->isBeforeFirst())
        {
            while (result->next())
            {
                std ::cout << "National ID: " << result->getString("national_ID") << " | Account Number: " << result->getInt("account_number") << " | First Name: " << result->getString("first_name");

                std ::cout << " | Last Nmae: " << result->getString("last_name") << " | Date of Birth: " << result->getString("date_birth") << " | Phone Number: " << result->getInt("phone_number");

                std ::cout << " | Email: " << result->getString("email") << " | Address: " << result->getString("address") << " | Balance: " << result->getDouble("balance") << " | Interest Rate: " << result->getDouble("interest_rate");

                std ::cout << " | Initial Timestamp: " << result->getString("initial_timestamp") << std ::endl;
                std ::cout << std ::endl;
                std ::cout << std ::endl;
            }
        }
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::Qt_display_accounts_table(sql ::Connection *connection)
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
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts;"));

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->isBeforeFirst())
        {
            int row = table->rowCount();

            while (result->next())
            {
                QList<QTableWidgetItem *> items;
                items << new QTableWidgetItem(QString::fromStdString(result->getString("national_ID")))
                      << new QTableWidgetItem(QString::number(result->getInt("account_number")))
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
        }

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(400, 400);
        table->show();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::display_specific_accounts(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->next())
        {
            std ::cout << "National ID: " << result->getString("national_ID") << " | First Name: " << result->getString("first_name");

            std ::cout << " | Last Nmae: " << result->getString("last_name") << " | Date of Birth: " << result->getString("date_birth") << " | Phone Number: " << result->getInt("phone_number");

            std ::cout << " | Email: " << result->getString("email") << " | Address: " << result->getString("address") << " | Balance: " << result->getDouble("balance") << " | Interest Rate: " << result->getDouble("interest_rate");

            std ::cout << " | Initial Timestamp: " << result->getString("initial_timestamp") << std ::endl;
            std ::cout << std ::endl;
        }
        else
            std ::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std ::endl;
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::Qt_display_specific_accounts(sql ::Connection *connection, int account_number)
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
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT * FROM accounts WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        if (!result->next())
        {
            QMessageBox *message = new QMessageBox();
            message->warning(nullptr, "Warning!", "The Account you enter doesn't exist in our data, Check and try again");
            message->show();

            delete message;
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
        table->resize(400, 400);
        table->show();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::display_people_in_debt(sql ::Connection *connection)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT accounts.account_number AS A, national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number;"));

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->isBeforeFirst())
        {
            while (result->next())
            {
                std ::cout << "Account Number: " << result->getInt("A") << " | National ID: " << result->getString("national_ID") << " | First Name: " << result->getString("first_name") << " | Last Name: " << result->getString("last_name") << " | Balance: " << result->getDouble("balance");

                std ::cout << " | Account Interest Rate: " << result->getDouble("B") << " | Borrowed Amount: " << result->getDouble("borrowed_amount") << " | Borrowed Amount Interest Rate: " << result->getDouble("C");

                std ::cout << " | Borrowed Amount Initial timestamp: " << result->getString("D") << " | Scheduled Time: " << result->getString("scheduled_time") << std ::endl;
                std ::cout << std ::endl;
                std ::cout << std ::endl;
            }
        }
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::Qt_display_people_in_debt(sql ::Connection *connection)
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
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT accounts.account_number AS A, national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number;"));

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        if (result->isBeforeFirst())
        {
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
        }

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->resize(400, 400);
        table->show();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::display_specific_accounts_in_debt(sql ::Connection *connection, int account_number)
{
    try
    {
        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number WHERE accounts.account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        if (result->next())
        {
            std ::cout << "National ID: " << result->getString("national_ID") << " | First Name: " << result->getString("first_name") << " | Last Name: " << result->getString("last_name") << " | Balance: " << result->getDouble("balance");

            std ::cout << " | Account Interest Rate: " << result->getDouble("B") << " | Borrowed Amount: " << result->getDouble("borrowed_amount") << " | Borrowed Amount Interest Rate: " << result->getDouble("C");

            std ::cout << " | Borrowed Amount Initial timestamp: " << result->getString("D") << " | Scheduled Time: " << result->getString("scheduled_time") << std ::endl;
            std ::cout << std ::endl;
            std ::cout << std ::endl;
        }
        else
            std ::cout << "Account " << account_number << " Not Found, Verify the Number and try again" << std ::endl;
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}

void BANK ::Qt_display_specific_accounts_in_debt(sql ::Connection *connection, int account_number)
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
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(true);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std ::unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("SELECT national_ID, first_name, last_name, balance, accounts.interest_rate AS B, borrowed_amount, borrowal_record.interest_rate AS C, borrowal_record.initial_timestamp AS D, scheduled_time FROM accounts INNER JOIN borrowal_record ON accounts.account_number = borrowal_record.account_number INNER JOIN event_schedule ON accounts.account_number = event_schedule.account_number WHERE accounts.account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std ::unique_ptr<sql ::ResultSet> result(prep_statement->executeQuery());

        QList<QTableWidgetItem *> items;
        int row = table->rowCount();

        if (!result->next())
        {
            QMessageBox *message = new QMessageBox();
            message->warning(nullptr, "Warning!", "The Account you enter either doesn't exist or doesn't owe the Bank, Check and try again");

            delete message;
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
        table->resize(400, 400);
        table->show();
    }
    catch (const sql ::SQLException &e)
    {
        std ::cerr << "SQL ERROR: " << e.what() << std ::endl;
    }
    catch (const std ::exception &e)
    {
        std ::cerr << "C++ ERROR: " << e.what() << std ::endl;
    }
}