#include "option_main_window.h"
#include <edit_forget_main_window.h>

option_main_window::option_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack, QWidget *parent)
    : QMainWindow(parent), connection(db_connection), _previous_stack(previous_stack)
{
        _stack = new QStackedWidget(this);
        setCentralWidget(_stack);
        setStyleSheet("color: beige;"
                      "font-family: Arial Black;"
                      "font-size: 20;"
                      "font: bold italic 14px;"
                      "background-color: black;");
        setWindowTitle("Client Inquiry Window");
        resize(200, 200);

        QWidget *option = new QWidget(this);

        QPushButton *balance = new QPushButton("1. Check Balance", this);
        connect(balance, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(1); });

        QPushButton *deposit = new QPushButton("2. Deposit", this);
        connect(deposit, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(2); });

        QPushButton *withdrawal = new QPushButton("3. Money Withdrawal", this);
        connect(withdrawal, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(3); });

        QPushButton *transfer = new QPushButton("4. Transfer Money", this);
        connect(transfer, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(4); });

        QPushButton *borrowal = new QPushButton("5. Borrowed Money", this);
        connect(borrowal, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(5); });

        QPushButton *return_borrowal = new QPushButton("6. Return Borrowed Money", this);
        connect(return_borrowal, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(6); });

        QPushButton *edit_and_forget = new QPushButton("7. Edit Account Information", this);
        connect(edit_and_forget, &QPushButton::clicked, this, &option_main_window::confirm_button_edit_perso);

        QPushButton *transaction_history = new QPushButton("8. Transaction History", this);
        connect(transaction_history, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(7); });

        QPushButton *specific_transaction_history = new QPushButton("9. Transaction History according Specific Date", this);
        connect(specific_transaction_history, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(8); });

        QPushButton *delete_account = new QPushButton("10. Delete Account", this);
        connect(delete_account, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(9); });

        QLabel *image_label = new QLabel(this);
        QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/client_option.jpeg");
        image_label->setPixmap(image.scaled(80, 80, Qt::KeepAspectRatio));
        image_label->setScaledContents(true);

        _back_button = new QPushButton("Previous Menu", this);
        _back_button->setStyleSheet("color: beige;"
                                    "font-family: Arial Black;"
                                    "font-size: 20;"
                                    "font: bold italic 14px;"
                                    "background-color: black;");
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *formLayout1 = new QFormLayout();
        formLayout1->addRow(balance);
        formLayout1->addRow(deposit);
        formLayout1->addRow(withdrawal);
        formLayout1->addRow(transfer);
        formLayout1->addRow(borrowal);

        QFormLayout *formLayout2 = new QFormLayout();
        formLayout2->addRow(return_borrowal);
        formLayout2->addRow(edit_and_forget);
        formLayout2->addRow(transaction_history);
        formLayout2->addRow(specific_transaction_history);
        formLayout2->addRow(delete_account);

        QGridLayout *gridLayout = new QGridLayout();
        gridLayout->setSpacing(5);
        gridLayout->addWidget(image_label, 0, 0, 1, 2, Qt::AlignCenter);
        gridLayout->addLayout(formLayout1, 1, 0);
        gridLayout->addLayout(formLayout2, 1, 1);
        gridLayout->addWidget(_back_button, 2, 0, 1, 2, Qt::AlignCenter);

        QVBoxLayout *mainLayout = new QVBoxLayout(option);
        mainLayout->addWidget(image_label, 0, Qt::AlignCenter);
        mainLayout->addLayout(gridLayout);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *balance_widget = new QWidget(this);
        balance_widget->setWindowTitle("Check Balance");

        _account_number_ba = new QLineEdit(this);

        _password_ba = new QLineEdit(this);
        _password_ba->setEchoMode(QLineEdit::Password);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_balance);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *balance_layout = new QFormLayout();
        balance_layout->addRow("Enter Account Number", _account_number_ba);
        balance_layout->addRow("Enter Password", _password_ba);
        balance_layout->addWidget(_confirm_button);
        balance_layout->addWidget(_back_button);

        QVBoxLayout *vbox1 = new QVBoxLayout(balance_widget);
        vbox1->addLayout(balance_layout);
        vbox1->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *deposit_widget = new QWidget(this);
        deposit_widget->setWindowTitle("Deposit Money");

        _account_number_de = new QLineEdit(this);
        _password_de = new QLineEdit(this);
        _password_de->setEchoMode(QLineEdit::Password);

        _amount_de = new QLineEdit(this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_deposit);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *deposit_layout = new QFormLayout();
        deposit_layout->addRow("Enter Account Number", _account_number_de);
        deposit_layout->addRow("Enter Password", _password_de);
        deposit_layout->addRow("Enter Amount to Deposit", _amount_de);
        deposit_layout->addWidget(_confirm_button);
        deposit_layout->addWidget(_back_button);

        QVBoxLayout *vbox2 = new QVBoxLayout(deposit_widget);
        vbox2->addLayout(deposit_layout);
        vbox2->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *withdrawal_widget = new QWidget(this);
        withdrawal_widget->setWindowTitle("Withdraw Money");

        _account_number_with = new QLineEdit(this);

        _password_with = new QLineEdit(this);
        _password_with->setEchoMode(QLineEdit::Password);

        _amount_with = new QLineEdit(this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_withdrawal);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *withdrawal_layout = new QFormLayout();
        withdrawal_layout->addRow("Enter Account Number", _account_number_with);
        withdrawal_layout->addRow("Enter Password", _password_with);
        withdrawal_layout->addRow("Enter Amount to Withdraw", _amount_with);
        withdrawal_layout->addWidget(_confirm_button);
        withdrawal_layout->addWidget(_back_button);

        QVBoxLayout *vbox3 = new QVBoxLayout(withdrawal_widget);
        vbox3->addLayout(withdrawal_layout);
        vbox3->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *transfer_widget = new QWidget(this);
        transfer_widget->setWindowTitle("Transfer Money");

        _account_number1_tran = new QLineEdit(this);

        _password_tran = new QLineEdit(this);
        _password_tran->setEchoMode(QLineEdit::Password);

        _account_number2_tran = new QLineEdit(this);

        _amount_tran = new QLineEdit(this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_transfer);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *transfer_layout = new QFormLayout();
        transfer_layout->addRow("Enter Account Number which will receive the Money", _account_number2_tran);
        transfer_layout->addRow("Enter Amount to Transfer", _amount_tran);
        transfer_layout->addRow("Enter Account Number", _account_number1_tran);
        transfer_layout->addRow("Enter Password", _password_tran);
        transfer_layout->addWidget(_confirm_button);
        transfer_layout->addWidget(_back_button);

        QVBoxLayout *vbox4 = new QVBoxLayout(transfer_widget);
        vbox4->addLayout(transfer_layout);
        vbox4->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *borrowal_widget = new QWidget(this);
        borrowal_widget->setWindowTitle("Borrow Money");

        _account_number_borr = new QLineEdit(this);

        _password_borr = new QLineEdit(this);
        _password_borr->setEchoMode(QLineEdit::Password);

        QString info_text = "Read the text below carefully and choose what best suits your needs.\n\n"
                            "Interest Rate Scale according to the borrowed Amount, which can't be changed.\n\n"
                            "1. Borrowed Amount = 100 ---> Interest Rate = 0.1%. PS: TO BE RETURN WITHIN 1 DAY OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                            "2. 500 > Borrowed Amount > 100 ---> Interest Rate = 5% PS: TO BE RETURN WITHIN 2 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                            "3. 1000 > Borrowed Amount >= 500 ---> Interest Rate = 7% PS: TO BE RETURN WITHIN 3 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                            "4. Borrowed Amount > 1000 ---> Interest Rate = 10% PS: TO BE RETURN WITHIN 4 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE";

        QTextEdit *interest_rate_info = new QTextEdit(this);
        interest_rate_info->setPlainText(info_text);
        interest_rate_info->setReadOnly(true);
        _amount_borr = new QLineEdit(this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_borrowal);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *borrowal_layout = new QFormLayout();
        borrowal_layout->addRow("Enter Account Number", _account_number_borr);
        borrowal_layout->addRow(interest_rate_info, _amount_borr);
        borrowal_layout->addRow("Enter Password", _password_borr);
        borrowal_layout->addWidget(_confirm_button);
        borrowal_layout->addWidget(_back_button);

        QVBoxLayout *vbox5 = new QVBoxLayout(borrowal_widget);
        vbox5->addLayout(borrowal_layout);
        vbox5->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *return_borrowal_widget = new QWidget(this);
        return_borrowal_widget->setWindowTitle("Return Borrowed Money");

        _account_number_ret = new QLineEdit(this);
        _password_ret = new QLineEdit(this);
        _password_ret->setEchoMode(QLineEdit::Password);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_return_borrowal);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *return_borrowal_layout = new QFormLayout();
        return_borrowal_layout->addRow("Enter Account Number", _account_number_ret);
        return_borrowal_layout->addRow("Enter Password", _password_ret);
        return_borrowal_layout->addWidget(_confirm_button);
        return_borrowal_layout->addWidget(_back_button);

        QVBoxLayout *vbox6 = new QVBoxLayout(return_borrowal_widget);
        vbox6->addLayout(return_borrowal_layout);
        vbox6->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *transaction_history_widget = new QWidget(this);
        transaction_history_widget->setWindowTitle("Transaction History");

        _account_number_transac = new QLineEdit(this);

        _password_transac = new QLineEdit(this);
        _password_transac->setEchoMode(QLineEdit::Password);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_transaction_history);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *transaction_history_layout = new QFormLayout();
        transaction_history_layout->addRow("Enter Account Number", _account_number_transac);
        transaction_history_layout->addWidget(_confirm_button);
        transaction_history_layout->addWidget(_back_button);

        QVBoxLayout *vbox7 = new QVBoxLayout(transaction_history_widget);
        vbox7->addLayout(transaction_history_layout);
        vbox7->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *specific_transaction_history_widget = new QWidget(this);
        specific_transaction_history_widget->setWindowTitle("Specific Transaction History");

        _specific_account_number_transac = new QLineEdit(this);

        _specific_password_transac = new QLineEdit(this);
        _specific_password_transac->setEchoMode(QLineEdit::Password);

        _calendar = new QDateEdit(this);
        _calendar->setCalendarPopup(true);
        _calendar->setDate(QDate::currentDate());
        connect(_calendar, &QDateEdit::dateChanged, this, [=]()
                { _selected_date = _calendar->date(); });

        _choice = new QComboBox(this);
        _choice->addItem("Display All Transactions occurred BEFORE the selected Date");
        _choice->addItem("Display All Transactions occurred AFTER the selected Date");
        _choice->addItem("Display All Transactions occurred ONLY on the selected Date");
        _choice->setStyleSheet("color: red;");

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_specific_transaction_history);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *specific_transaction_history_layout = new QFormLayout();
        specific_transaction_history_layout->addRow("Enter Account Number", _specific_account_number_transac);
        specific_transaction_history_layout->addRow("Enter Password", _specific_password_transac);
        specific_transaction_history_layout->addWidget(_calendar);
        specific_transaction_history_layout->addWidget(_choice);
        specific_transaction_history_layout->addWidget(_confirm_button);
        specific_transaction_history_layout->addWidget(_back_button);

        QVBoxLayout *vbox8 = new QVBoxLayout(specific_transaction_history_widget);
        vbox8->addLayout(specific_transaction_history_layout);
        vbox8->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *delete_account_widget = new QWidget(this);
        delete_account_widget->setWindowTitle("Delete Account");

        _account_number_dele = new QLineEdit(this);

        _password_dele = new QLineEdit(this);
        _password_dele->setEchoMode(QLineEdit::Password);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_delete_account);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QFormLayout *delete_account_layout = new QFormLayout();
        delete_account_layout->addRow("Enter Account Number", _account_number_dele);
        delete_account_layout->addRow("Enter Password", _password_dele);
        delete_account_layout->addWidget(_confirm_button);
        delete_account_layout->addWidget(_back_button);

        QVBoxLayout *vbox9 = new QVBoxLayout(delete_account_widget);
        vbox9->addLayout(delete_account_layout);
        vbox9->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        _stack->addWidget(option);
        _stack->addWidget(balance_widget);
        _stack->addWidget(deposit_widget);
        _stack->addWidget(withdrawal_widget);
        _stack->addWidget(transfer_widget);
        _stack->addWidget(borrowal_widget);
        _stack->addWidget(return_borrowal_widget);
        _stack->addWidget(transaction_history_widget);
        _stack->addWidget(specific_transaction_history_widget);
        _stack->addWidget(delete_account_widget);
}

void option_main_window::back_button_func()
{
        int current_index = _stack->currentIndex();

        (current_index != 0) ? _stack->setCurrentIndex(0) : _previous_stack->setCurrentIndex(0);
}

void option_main_window::confirm_button_balance()
{
        int account_number = _account_number_ba->text().toInt();
        std::string password = _password_ba->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _account_number_ba->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number_ba->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_ba->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Balance Check", "Password Incorrect");

                return;
        }

        _password_ba->setStyleSheet("border: 1px solid gray");

        BANK::apply_interest_rate_to_balance(connection, account_number);

        Qt_display_balance(connection, account_number);

        hashed_password.clear();
        password.clear();

        _account_number_ba->clear();
        _password_ba->clear();
}

void option_main_window::confirm_button_deposit()
{
        int account_number = _account_number_de->text().toInt();
        std::string password = _password_de->text().toStdString();
        double amount_to_deposit = _amount_de->text().toDouble();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _account_number_de->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number_de->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_de->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Deposit", "Password Incorrect");

                return;
        }

        _password_de->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_deposit(connection, amount_to_deposit, account_number);

        password.clear();
        hashed_password.clear();

        _account_number_de->clear();
        _password_de->clear();
        _amount_de->clear();
}

void option_main_window::confirm_button_withdrawal()
{
        int account_number = _account_number_with->text().toInt();
        std::string password = _password_with->text().toStdString();
        double amount_to_withdraw = _amount_with->text().toDouble();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _account_number_with->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number_with->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_with->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Withdrawal", "Password Incorrect");

                return;
        }

        _password_with->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_withdrawal(connection, amount_to_withdraw, account_number);

        password.clear();
        hashed_password.clear();

        _account_number_with->clear();
        _password_with->clear();
        _amount_with->clear();
}
void option_main_window::confirm_button_transfer()
{
        int account_number1 = _account_number1_tran->text().toInt();
        int account_number2 = _account_number2_tran->text().toInt();
        std::string password = _password_tran->text().toStdString();
        double amount_to_transfer = _amount_tran->text().toDouble();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number1);

        if (hashed_password.empty())
        {
                _account_number1_tran->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number1_tran->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_tran->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Transfer", "Password Incorrect");

                return;
        }

        _password_tran->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_transfer(connection, amount_to_transfer, account_number1, account_number2);

        password.clear();
        hashed_password.clear();

        _account_number1_tran->clear();
        _account_number2_tran->clear();
        _password_tran->clear();
        _amount_tran->clear();
}

void option_main_window::confirm_button_borrowal()
{
        int account_number = _account_number_borr->text().toInt();
        std::string password = _password_borr->text().toStdString();
        double amount_to_borrow = _amount_borr->text().toDouble();

        double borrowal_interest_rate;

        if (amount_to_borrow == 100)
                borrowal_interest_rate = 0.001;

        else if (amount_to_borrow > 100 && amount_to_borrow < 500)
                borrowal_interest_rate = 0.05;

        else if (amount_to_borrow < 1000 && amount_to_borrow >= 500)
                borrowal_interest_rate = 0.07;

        else
                borrowal_interest_rate = 0.1;

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _account_number_borr->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number_borr->setStyleSheet("border: 1px solid gray");

        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
        prep_statement->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

        if (result->next())
        {
                QMessageBox::warning(nullptr, "Error", "Aren't allowed to borrow Cause this account owes the Bank. First Pay the Debt and then the  borrowal will be possible");

                return;
        }

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_borr->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Borrow", "Password Incorrect");

                return;
        }

        _password_borr->setStyleSheet("border: 1px solid gray");

        Transactions::insert_borrowal(connection, account_number, amount_to_borrow, borrowal_interest_rate);

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO event_schedule (account_number, scheduled_time) VALUES (?, CURRENT_TIMESTAMP + INTERVAL 96 HOUR);"));
        prep_statement->setInt(1, account_number);

        prep_statement->executeUpdate();

        Transactions::Qt_borrow(connection, amount_to_borrow, account_number);

        password.clear();
        hashed_password.clear();

        _account_number_borr->clear();
        _password_borr->clear();
        _amount_borr->clear();
}

void option_main_window::confirm_button_return_borrowal()
{
        int account_number = _account_number_ret->text().toInt();
        std::string password = _password_ret->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _account_number_ret->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number_ret->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_ret->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Borrowal Return", "Password Incorrect");

                return;
        }

        _password_ret->setStyleSheet("border: 1px solid gray");

        std::unique_ptr<sql::PreparedStatement> prep_statement_call_update(connection->prepareStatement("CALL update_borrowed_money(?);"));
        prep_statement_call_update->setInt(1, account_number);

        prep_statement_call_update->executeUpdate();

        std::unique_ptr<sql::PreparedStatement> prep_statement_select_borrowal(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
        prep_statement_select_borrowal->setInt(1, account_number);

        std::unique_ptr<sql::ResultSet> result(prep_statement_select_borrowal->executeQuery());

        double due_returned;

        if (result->next())
                due_returned = result->getDouble("borrowed_amount");

        bool OK = true;
        QString info = "The amount to be returned is: $" + QString::number(static_cast<double>(due_returned)) + ". Enter the exact amount and not less";

        QString input = QInputDialog::getText(nullptr, "Due Returned", info, QLineEdit::Normal, "", &OK);

        if (input.isEmpty())
        {
                QMessageBox::warning(this, "void", "Input Empty");

                return;
        }

        if (OK && !input.isEmpty())
        {
                if (input.toDouble() < due_returned)
                {
                        QMessageBox::warning(this, "Unsufficient amount", "Unsufficient amount");

                        return;
                }
        }

        std::unique_ptr<sql::PreparedStatement> prep_statement_delete_borrowal(connection->prepareStatement("DELETE FROM borrowal_record WHERE account_number = ?;"));
        prep_statement_delete_borrowal->setInt(1, account_number);

        prep_statement_delete_borrowal->executeUpdate();

        std::unique_ptr<sql::PreparedStatement> prep_statement_delete_event(connection->prepareStatement("DELETE FROM event_schedule WHERE account_number = ?;"));
        prep_statement_delete_event->setInt(1, account_number);

        prep_statement_delete_event->executeUpdate();

        Transactions::insert_transactions(connection, account_number, "New Money Returned, Sum of ", due_returned);

        QMessageBox::information(this, "Debt paid", "Thanks, You have officially paid your debt and are now allowed to make another one");

        _account_number_ret->clear();
        _password_ret->clear();

        password.clear();
        hashed_password.clear();
}

void option_main_window::confirm_button_edit_perso()
{
        QMessageBox::information(this, "Redirecting...", "You are about to be redirected to the account editing official webpage");

        edit_forget_main_window *edit_forget_window = new edit_forget_main_window(connection, _stack, this);

        _stack->addWidget(edit_forget_window);

        _stack->setCurrentWidget(edit_forget_window);
}

void option_main_window::confirm_button_transaction_history()
{
        int account_number = _account_number_transac->text().toInt();
        std::string password = _password_transac->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _account_number_transac->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number_transac->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_transac->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Transac History", "Password Incorrect");

                return;
        }

        _password_transac->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_display_transactions_history(connection, account_number);

        _account_number_transac->clear();
        _password_transac->clear();

        password.clear();
        hashed_password.clear();
}

void option_main_window::confirm_button_specific_transaction_history()
{
        int account_number = _specific_account_number_transac->text().toInt();
        std::string password = _specific_password_transac->text().toStdString();
        QString date = _selected_date.toString(Qt::ISODate);
        int selected__choice = _choice->currentIndex();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _specific_account_number_transac->setStyleSheet("border: 1px solid red");

                return;
        }

        _specific_account_number_transac->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _specific_password_transac->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Transac History", "Password Incorrect");

                return;
        }

        _specific_password_transac->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_display_specific_transactions_history(connection, account_number, date.toStdString(), selected__choice);

        _specific_account_number_transac->clear();
        _specific_password_transac->clear();

        password.clear();
        hashed_password.clear();
}

void option_main_window::confirm_button_delete_account()
{
        int account_number = _account_number_dele->text().toInt();
        std::string password = _password_dele->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password.empty())
        {
                _account_number_dele->setStyleSheet("border: 1px solid red");

                return;
        }

        _account_number_dele->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                _password_dele->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Delete Account", "Password Incorrect");

                return;
        }

        _password_dele->setStyleSheet("border: 1px solid gray");

        Account::Qt_remove_accounts(connection, account_number);

        _account_number_dele->clear();
        _password_dele->clear();

        password.clear();
        hashed_password.clear();
}