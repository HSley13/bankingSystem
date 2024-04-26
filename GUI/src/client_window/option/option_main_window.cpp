#include "option_main_window.h"
#include <edit_forget_main_window.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

option_main_window::option_main_window(sql::Connection *db_connection, QWidget *parent)
    : QMainWindow(parent), connection(db_connection)
{
        window_stack = new QStackedWidget();
        setCentralWidget(window_stack);
        setStyleSheet("color: beige;"
                      "font-family: Arial Black;"
                      "font-size: 20;"
                      "font: bold italic 14px;"
                      "background-color: black;");
        setWindowTitle("Client Inquiry Window");
        resize(500, 500);

        QWidget *central_widget = new QWidget(this);

        QPushButton *balance = new QPushButton("1. Check Balance", this);
        connect(balance, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(1); });

        QPushButton *deposit = new QPushButton("2. Deposit", this);
        connect(deposit, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(2); });

        QPushButton *withdrawal = new QPushButton("3. Money Withdrawal", this);
        connect(withdrawal, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(3); });

        QPushButton *transfer = new QPushButton("4. Transfer Money", this);
        connect(transfer, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(4); });

        QPushButton *borrowal = new QPushButton("5. Borrowed Money", this);
        connect(borrowal, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(5); });

        QPushButton *return_borrowal = new QPushButton("6. Return Borrowed Money", this);
        connect(return_borrowal, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(6); });

        QPushButton *edit_and_forget = new QPushButton("7. Edit Account Information", this);
        connect(edit_and_forget, &QPushButton::clicked, this, &option_main_window::confirm_button_edit_perso);

        QPushButton *transaction_history = new QPushButton("8. All Transaction History", this);
        connect(transaction_history, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(7); });

        QPushButton *specific_transaction_history = new QPushButton("9. All Transaction History Relative to a Specific Date", this);
        connect(specific_transaction_history, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(8); });

        QPushButton *delete_account = new QPushButton("10. Delete Account", this);
        connect(delete_account, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(9); });

        QLabel *image_label = new QLabel(this);
        QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/client_option.jpeg");
        image_label->setPixmap(image.scaled(500, 400, Qt::KeepAspectRatio));
        image_label->setScaledContents(true);

        QVBoxLayout *VBOX = new QVBoxLayout(central_widget);
        VBOX->addWidget(image_label, 2, Qt::AlignCenter);
        VBOX->addWidget(balance);
        VBOX->addWidget(deposit);
        VBOX->addWidget(withdrawal);
        VBOX->addWidget(transfer);
        VBOX->addWidget(borrowal);
        VBOX->addWidget(return_borrowal);
        VBOX->addWidget(edit_and_forget);
        VBOX->addWidget(transaction_history);
        VBOX->addWidget(specific_transaction_history);
        VBOX->addWidget(delete_account);
        VBOX->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *balance_widget = new QWidget();
        balance_widget->setWindowTitle("Check Balance");

        QLabel *balance_message1 = new QLabel("Enter Account Number", this);
        account_number_ba = new QLineEdit(this);
        QHBoxLayout *ba_hbox1 = new QHBoxLayout();
        ba_hbox1->addWidget(balance_message1, Qt::AlignCenter);
        ba_hbox1->addWidget(account_number_ba, Qt::AlignCenter);

        QLabel *balance_message2 = new QLabel("Enter Password", this);
        password_ba = new QLineEdit(this);
        password_ba->setEchoMode(QLineEdit::Password);
        QHBoxLayout *ba_hbox2 = new QHBoxLayout();
        ba_hbox2->addWidget(balance_message2, Qt::AlignCenter);
        ba_hbox2->addWidget(password_ba, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_balance);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox1 = new QVBoxLayout(balance_widget);
        vbox1->addLayout(ba_hbox1, Qt::AlignCenter);
        vbox1->addLayout(ba_hbox2, Qt::AlignCenter);
        vbox1->addWidget(confirm_button, Qt::AlignCenter);
        vbox1->addWidget(back_button, Qt::AlignCenter);
        vbox1->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *deposit_widget = new QWidget();
        deposit_widget->setWindowTitle("Deposit Money");

        QLabel *deposit_message1 = new QLabel("Enter Account Number", this);
        account_number_de = new QLineEdit(this);
        QHBoxLayout *de_hbox1 = new QHBoxLayout();
        de_hbox1->addWidget(deposit_message1, Qt::AlignCenter);
        de_hbox1->addWidget(account_number_de, Qt::AlignCenter);

        QLabel *deposit_message2 = new QLabel("Enter Password", this);
        password_de = new QLineEdit();
        password_de->setEchoMode(QLineEdit::Password);
        QHBoxLayout *de_hbox2 = new QHBoxLayout();
        de_hbox2->addWidget(deposit_message2, Qt::AlignCenter);
        de_hbox2->addWidget(password_de, Qt::AlignCenter);

        QLabel *deposit_message3 = new QLabel("Enter Amount to Deposit", this);
        amount_de = new QLineEdit(this);
        QHBoxLayout *de_hbox3 = new QHBoxLayout();
        de_hbox3->addWidget(deposit_message3, Qt::AlignCenter);
        de_hbox3->addWidget(amount_de, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_deposit);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox2 = new QVBoxLayout(deposit_widget);
        vbox2->addLayout(de_hbox1, Qt::AlignCenter);
        vbox2->addLayout(de_hbox2, Qt::AlignCenter);
        vbox2->addLayout(de_hbox3, Qt::AlignCenter);
        vbox2->addWidget(confirm_button, Qt::AlignCenter);
        vbox2->addWidget(back_button, Qt::AlignCenter);
        vbox2->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *withdrawal_widget = new QWidget();
        withdrawal_widget->setWindowTitle("Withdraw Money");

        QLabel *withdrawal_message1 = new QLabel("Enter Account Number", this);
        account_number_with = new QLineEdit(this);
        QHBoxLayout *with_hbox1 = new QHBoxLayout();
        with_hbox1->addWidget(withdrawal_message1, Qt::AlignCenter);
        with_hbox1->addWidget(account_number_with, Qt::AlignCenter);

        QLabel *withdrawal_message2 = new QLabel("Enter Password", this);
        password_with = new QLineEdit();
        password_with->setEchoMode(QLineEdit::Password);
        QHBoxLayout *with_hbox2 = new QHBoxLayout();
        with_hbox2->addWidget(withdrawal_message2, Qt::AlignCenter);
        with_hbox2->addWidget(password_with, Qt::AlignCenter);

        QLabel *withdrawal_message3 = new QLabel("Enter Amount to Withdraw", this);
        amount_with = new QLineEdit(this);
        QHBoxLayout *with_hbox3 = new QHBoxLayout();
        with_hbox3->addWidget(withdrawal_message3, Qt::AlignCenter);
        with_hbox3->addWidget(amount_with, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_withdrawal);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox3 = new QVBoxLayout(withdrawal_widget);
        vbox3->addLayout(with_hbox1, Qt::AlignCenter);
        vbox3->addLayout(with_hbox2, Qt::AlignCenter);
        vbox3->addLayout(with_hbox3, Qt::AlignCenter);
        vbox3->addWidget(confirm_button, Qt::AlignCenter);
        vbox3->addWidget(back_button, Qt::AlignCenter);
        vbox3->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *transfer_widget = new QWidget();
        transfer_widget->setWindowTitle("Transfer Money");

        QLabel *transfer_message1 = new QLabel("Enter Account Number", this);
        account_number1_tran = new QLineEdit(this);
        QHBoxLayout *tran_hbox1 = new QHBoxLayout();
        tran_hbox1->addWidget(transfer_message1, Qt::AlignCenter);
        tran_hbox1->addWidget(account_number1_tran, Qt::AlignCenter);

        QLabel *transfer_message2 = new QLabel("Enter Password", this);
        password_tran = new QLineEdit();
        password_tran->setEchoMode(QLineEdit::Password);
        QHBoxLayout *tran_hbox2 = new QHBoxLayout();
        tran_hbox2->addWidget(transfer_message2, Qt::AlignCenter);
        tran_hbox2->addWidget(password_tran, Qt::AlignCenter);

        QLabel *transfer_message3 = new QLabel("Enter Account Number which will receive the Money", this);
        account_number2_tran = new QLineEdit(this);
        QHBoxLayout *tran_hbox3 = new QHBoxLayout();
        tran_hbox3->addWidget(transfer_message3, Qt::AlignCenter);
        tran_hbox3->addWidget(account_number2_tran, Qt::AlignCenter);

        QLabel *transfer_message4 = new QLabel("Enter Amount to Transfer", this);
        amount_tran = new QLineEdit(this);
        QHBoxLayout *tran_hbox4 = new QHBoxLayout();
        tran_hbox4->addWidget(transfer_message4, Qt::AlignCenter);
        tran_hbox4->addWidget(amount_tran, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_transfer);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox4 = new QVBoxLayout(transfer_widget);
        vbox4->addLayout(tran_hbox1, Qt::AlignCenter);
        vbox4->addLayout(tran_hbox2, Qt::AlignCenter);
        vbox4->addLayout(tran_hbox3, Qt::AlignCenter);
        vbox4->addLayout(tran_hbox4, Qt::AlignCenter);
        vbox4->addWidget(confirm_button, Qt::AlignCenter);
        vbox4->addWidget(back_button, Qt::AlignCenter);
        vbox4->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *borrowal_widget = new QWidget();
        borrowal_widget->setWindowTitle("Borrow Money");

        borrowal_widget = new QWidget();
        borrowal_widget->setWindowTitle("Deposit Money");

        QLabel *borrowal_message1 = new QLabel("Enter Account Number", this);
        account_number_borr = new QLineEdit(this);
        QHBoxLayout *borr_hbox1 = new QHBoxLayout();
        borr_hbox1->addWidget(borrowal_message1, Qt::AlignCenter);
        borr_hbox1->addWidget(account_number_borr, Qt::AlignCenter);

        QLabel *borrowal_message2 = new QLabel("Enter Password", this);
        password_borr = new QLineEdit();
        password_borr->setEchoMode(QLineEdit::Password);
        QHBoxLayout *borr_hbox2 = new QHBoxLayout();
        borr_hbox2->addWidget(borrowal_message2, Qt::AlignCenter);
        borr_hbox2->addWidget(password_borr, Qt::AlignCenter);

        QString info_text = "Read the text below carefully and choose what best suits your neeeds.\n\n"
                            "Interest Rate Scale according to the borrowed Amount, which can't be changed.\n\n"
                            "1. Borrowed Amount = 100 ---> Interest Rate = 0.1%. PS: TO BE RETURN WITHIN 1 DAY OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                            "2. 500 > Borrowed Amount > 100 ---> Interest Rate = 5% PS: TO BE RETURN WITHIN 2 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                            "3. 1000 > Borrowed Amount >= 500 ---> Interest Rate = 7% PS: TO BE RETURN WITHIN 3 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                            "4. Borrowed Amount > 1000 ---> Interest Rate = 10% PS: TO BE RETURN WITHIN 4 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE";

        QTextEdit *interest_rate_info = new QTextEdit(this);
        interest_rate_info->setPlainText(info_text);
        interest_rate_info->setReadOnly(true);
        amount_borr = new QLineEdit(this);

        QHBoxLayout *borr_hbox3 = new QHBoxLayout();
        borr_hbox3->addWidget(interest_rate_info, Qt::AlignCenter);
        borr_hbox3->addWidget(amount_borr, Qt::AlignCenter);
        QGroupBox *grp_balance = new QGroupBox();
        grp_balance->setLayout(borr_hbox3);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_borrowal);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox5 = new QVBoxLayout(borrowal_widget);
        vbox5->addLayout(borr_hbox1, Qt::AlignCenter);
        vbox5->addLayout(borr_hbox2, Qt::AlignCenter);
        vbox5->addWidget(grp_balance, Qt::AlignCenter);
        vbox5->addWidget(confirm_button, Qt::AlignCenter);
        vbox5->addWidget(back_button, Qt::AlignCenter);
        vbox5->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *return_borrowal_widget = new QWidget();
        return_borrowal_widget->setWindowTitle("Return Borrowed Money");

        QLabel *return_borrowal_message1 = new QLabel("Enter Account Number", this);
        account_number_ret = new QLineEdit(this);
        QHBoxLayout *ret_hbox1 = new QHBoxLayout();
        ret_hbox1->addWidget(return_borrowal_message1, Qt::AlignCenter);
        ret_hbox1->addWidget(account_number_ret, Qt::AlignCenter);

        QLabel *return_borrowal_message2 = new QLabel("Enter Password", this);
        password_ret = new QLineEdit(this);
        password_ret->setEchoMode(QLineEdit::Password);
        QHBoxLayout *ret_hbox2 = new QHBoxLayout();
        ret_hbox2->addWidget(return_borrowal_message2, Qt::AlignCenter);
        ret_hbox2->addWidget(password_ret, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_return_borrowal);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox6 = new QVBoxLayout(return_borrowal_widget);
        vbox6->addLayout(ret_hbox1, Qt::AlignCenter);
        vbox6->addLayout(ret_hbox2, Qt::AlignCenter);
        vbox6->addWidget(confirm_button, Qt::AlignCenter);
        vbox6->addWidget(back_button, Qt::AlignCenter);
        vbox6->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *transaction_history_widget = new QWidget();
        transaction_history_widget->setWindowTitle("Transaction History");

        QLabel *transaction_history_message1 = new QLabel("Enter Account Number", this);
        account_number_transac = new QLineEdit(this);
        QHBoxLayout *transac_hbox1 = new QHBoxLayout();
        transac_hbox1->addWidget(transaction_history_message1, Qt::AlignCenter);
        transac_hbox1->addWidget(account_number_transac, Qt::AlignCenter);

        QLabel *transaction_history_message2 = new QLabel("Enter Password", this);
        password_transac = new QLineEdit(this);
        password_transac->setEchoMode(QLineEdit::Password);
        QHBoxLayout *transac_hbox2 = new QHBoxLayout();
        transac_hbox2->addWidget(transaction_history_message2, Qt::AlignCenter);
        transac_hbox2->addWidget(password_transac, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_transaction_history);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox7 = new QVBoxLayout(transaction_history_widget);
        vbox7->addLayout(transac_hbox1, Qt::AlignCenter);
        vbox7->addLayout(transac_hbox2, Qt::AlignCenter);
        vbox7->addWidget(confirm_button, Qt::AlignCenter);
        vbox7->addWidget(back_button, Qt::AlignCenter);
        vbox7->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *specific_transaction_history_widget = new QWidget();
        specific_transaction_history_widget->setWindowTitle("Specific Transaction History");

        QLabel *specific_transaction_history_message1 = new QLabel("Enter Account Number", this);
        specific_account_number_transac = new QLineEdit(this);
        QHBoxLayout *specific_transac_hbox1 = new QHBoxLayout();
        specific_transac_hbox1->addWidget(specific_transaction_history_message1, Qt::AlignCenter);
        specific_transac_hbox1->addWidget(specific_account_number_transac, Qt::AlignCenter);

        QLabel *specific_transaction_history_message2 = new QLabel("Enter Password", this);
        specific_password_transac = new QLineEdit(this);
        specific_password_transac->setEchoMode(QLineEdit::Password);
        QHBoxLayout *specific_transac_hbox2 = new QHBoxLayout();
        specific_transac_hbox2->addWidget(specific_transaction_history_message2, Qt::AlignCenter);
        specific_transac_hbox2->addWidget(specific_password_transac, Qt::AlignCenter);

        calendar = new QDateEdit(this);
        calendar->setCalendarPopup(true);
        calendar->setDate(QDate::currentDate());
        connect(calendar, &QDateEdit::dateChanged, this, [=]()
                { selected_date = calendar->date(); });

        choice = new QComboBox(this);
        choice->addItem("Display All Transactions occured BEFORE the selected Date");
        choice->addItem("Display All Transactions occured AFTER the selected Date");
        choice->addItem("Display All Transactions occured ONLY on the selected Date");
        choice->setStyleSheet("color: red;");

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_specific_transaction_history);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox8 = new QVBoxLayout(specific_transaction_history_widget);
        vbox8->addLayout(specific_transac_hbox1, Qt::AlignCenter);
        vbox8->addLayout(specific_transac_hbox2, Qt::AlignCenter);
        vbox8->addWidget(calendar, Qt::AlignCenter);
        vbox8->addWidget(choice, Qt::AlignCenter);
        vbox8->addWidget(confirm_button, Qt::AlignCenter);
        vbox8->addWidget(back_button, Qt::AlignCenter);
        vbox8->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *delete_account_widget = new QWidget();
        delete_account_widget->setWindowTitle("Delete Account");

        QLabel *delete_account_message1 = new QLabel("Enter Account Number", this);
        account_number_dele = new QLineEdit(this);
        QHBoxLayout *dele_hbox1 = new QHBoxLayout();
        dele_hbox1->addWidget(delete_account_message1, Qt::AlignCenter);
        dele_hbox1->addWidget(account_number_dele, Qt::AlignCenter);

        QLabel *delete_account_message2 = new QLabel("Enter Password", this);
        password_dele = new QLineEdit(this);
        password_dele->setEchoMode(QLineEdit::Password);
        QHBoxLayout *dele_hbox2 = new QHBoxLayout();
        dele_hbox2->addWidget(delete_account_message2, Qt::AlignCenter);
        dele_hbox2->addWidget(password_dele, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_delete_account);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

        QVBoxLayout *vbox9 = new QVBoxLayout(delete_account_widget);
        vbox9->addLayout(dele_hbox1, Qt::AlignCenter);
        vbox9->addLayout(dele_hbox2, Qt::AlignCenter);
        vbox9->addWidget(confirm_button, Qt::AlignCenter);
        vbox9->addWidget(back_button, Qt::AlignCenter);
        vbox9->setAlignment(Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        window_stack->addWidget(central_widget);
        window_stack->addWidget(balance_widget);
        window_stack->addWidget(deposit_widget);
        window_stack->addWidget(withdrawal_widget);
        window_stack->addWidget(transfer_widget);
        window_stack->addWidget(borrowal_widget);
        window_stack->addWidget(return_borrowal_widget);
        window_stack->addWidget(transaction_history_widget);
        window_stack->addWidget(specific_transaction_history_widget);
        window_stack->addWidget(delete_account_widget);
}

void option_main_window::back_button_func()
{
        int current_index = window_stack->currentIndex();

        if (current_index)
                window_stack->setCurrentIndex(0);
}

void option_main_window::confirm_button_balance()
{
        int account_number = account_number_ba->text().toInt();
        std::string password = password_ba->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password == "")
        {
                account_number_ba->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number_ba->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                password_ba->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Balance Check", "Password Incorrect");

                return;
        }

        password_ba->setStyleSheet("border: 1px solid gray");

        BANK::apply_interest_rate_to_balance(connection, account_number);

        Qt_display_balance(connection, account_number);

        hashed_password.clear();
        password.clear();

        account_number_ba->clear();
        password_ba->clear();
}

void option_main_window::confirm_button_deposit()
{
        int account_number = account_number_de->text().toInt();
        std::string password = password_de->text().toStdString();
        double amount_to_deposit = amount_de->text().toDouble();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password == "")
        {
                account_number_de->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number_de->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                password_de->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Deposit", "Password Incorrect");

                return;
        }

        password_de->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_deposit(connection, amount_to_deposit, account_number);

        password.clear();
        hashed_password.clear();

        account_number_de->clear();
        password_de->clear();
        amount_de->clear();
}

void option_main_window::confirm_button_withdrawal()
{
        int account_number = account_number_with->text().toInt();
        std::string password = password_with->text().toStdString();
        double amount_to_withdraw = amount_with->text().toDouble();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password == "")
        {
                account_number_with->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number_with->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                password_with->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Withdrawal", "Password Incorrect");

                return;
        }

        password_with->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_withdrawal(connection, amount_to_withdraw, account_number);

        password.clear();
        hashed_password.clear();

        account_number_with->clear();
        password_with->clear();
        amount_with->clear();
}
void option_main_window::confirm_button_transfer()
{
        int account_number1 = account_number1_tran->text().toInt();
        int account_number2 = account_number2_tran->text().toInt();
        std::string password = password_tran->text().toStdString();
        double amount_to_transfer = amount_tran->text().toDouble();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number1);

        if (hashed_password == "")
        {
                account_number1_tran->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number1_tran->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                password_tran->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Transfer", "Password Incorrect");

                return;
        }

        password_tran->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_transfer(connection, amount_to_transfer, account_number1, account_number2);

        password.clear();
        hashed_password.clear();

        account_number1_tran->clear();
        account_number2_tran->clear();
        password_tran->clear();
        amount_tran->clear();
}

void option_main_window::confirm_button_borrowal()
{
        int account_number = account_number_borr->text().toInt();
        std::string password = password_borr->text().toStdString();
        double amount_to_borrow = amount_borr->text().toDouble();

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

        if (hashed_password == "")
        {
                account_number_borr->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number_borr->setStyleSheet("border: 1px solid gray");

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
                password_borr->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Borrow", "Password Incorrect");

                return;
        }

        password_borr->setStyleSheet("border: 1px solid gray");

        Transactions::insert_borrowal(connection, account_number, amount_to_borrow, borrowal_interest_rate);

        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO event_schedule (account_number, scheduled_time) VALUES (?, CURRENT_TIMESTAMP + INTERVAL 96 HOUR);"));
        prep_statement->setInt(1, account_number);

        prep_statement->executeUpdate();

        Transactions::Qt_borrow(connection, amount_to_borrow, account_number);

        password.clear();
        hashed_password.clear();

        account_number_borr->clear();
        password_borr->clear();
        amount_borr->clear();
}

void option_main_window::confirm_button_return_borrowal()
{
        int account_number = account_number_ret->text().toInt();
        std::string password = password_ret->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password == "")
        {
                account_number_ret->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number_ret->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                password_ret->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Borrowal Return", "Password Incorrect");

                return;
        }

        password_ret->setStyleSheet("border: 1px solid gray");

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

        account_number_ret->clear();
        password_ret->clear();

        password.clear();
        hashed_password.clear();
}

void option_main_window::confirm_button_edit_perso()
{
        QMessageBox::information(this, "Redirecting...", "You are about to be redirected to the account editing official webpage");

        edit_forget_main_window *new_window = new edit_forget_main_window(connection);

        new_window->show();
}

void option_main_window::confirm_button_transaction_history()
{
        int account_number = account_number_transac->text().toInt();
        std::string password = password_transac->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password == "")
        {
                account_number_transac->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number_transac->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                password_transac->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Transac History", "Password Incorrect");

                return;
        }

        password_transac->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_display_transactions_history(connection, account_number);

        account_number_transac->clear();
        password_transac->clear();

        password.clear();
        hashed_password.clear();
}

void option_main_window::confirm_button_specific_transaction_history()
{
        int account_number = specific_account_number_transac->text().toInt();
        std::string password = specific_password_transac->text().toStdString();
        QString date = selected_date.toString(Qt::ISODate);
        int selected_choice = choice->currentIndex();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password == "")
        {
                specific_account_number_transac->setStyleSheet("border: 1px solid red");

                return;
        }

        specific_account_number_transac->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                specific_password_transac->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Transac History", "Password Incorrect");

                return;
        }

        specific_password_transac->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_display_specific_transactions_history(connection, account_number, date.toStdString(), selected_choice);

        specific_account_number_transac->clear();
        specific_password_transac->clear();

        password.clear();
        hashed_password.clear();
}

void option_main_window::confirm_button_delete_account()
{
        int account_number = account_number_dele->text().toInt();
        std::string password = password_dele->text().toStdString();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        if (hashed_password == "")
        {
                account_number_dele->setStyleSheet("border: 1px solid red");

                return;
        }

        account_number_dele->setStyleSheet("border: 1px solid gray");

        if (!BANK::verifying_password(password, hashed_password))
        {
                password_dele->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(nullptr, "Delete Account", "Password Incorrect");

                return;
        }

        password_dele->setStyleSheet("border: 1px solid gray");

        Account::Qt_remove_accounts(connection, account_number);

        account_number_dele->clear();
        password_dele->clear();

        password.clear();
        hashed_password.clear();
}