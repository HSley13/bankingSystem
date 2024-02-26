#include "adm_option_main_window.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

adm_option_main_window::adm_option_main_window(sql::Connection *db_connection, QWidget *parent)
    : QMainWindow(parent), connection(db_connection)
{
        window_stack = new QStackedWidget();
        setWindowTitle("ADM window Management");
        setCentralWidget(window_stack);
        setStyleSheet("font-family: Arial Black;"
                      "font-size: 20;"
                      "font: bold italic 14px;"
                      "color: beige;"
                      "background-color: black;");
        resize(600, 300);

        central_widget = new QWidget(this);
        central_widget->resize(600, 300);

        image_label = new QLabel(this);
        QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/adm_option.jpeg");
        image_label->setPixmap(image.scaled(500, 500, Qt::KeepAspectRatio));
        image_label->setScaledContents(true);

        button1 = new QPushButton("1. Create Administrator", this);
        connect(button1, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(1); });

        button2 = new QPushButton("2. Display The Accounts Table", this);
        connect(button2, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(2); });

        button3 = new QPushButton("3. Display Specific Account through the Account number", this);
        connect(button3, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(3); });

        button4 = new QPushButton("4. Display People in Debt", this);
        connect(button4, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(4); });

        button5 = new QPushButton("5. Display Specific Person in Debt through the Account_number", this);
        connect(button5, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(5); });

        button6 = new QPushButton("6. Display all Transactions History for an Account_number", this);
        connect(button6, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(6); });

        button7 = new QPushButton("7. Display All Transaction History Relative to a Specific Date for an Account_Number", this);
        connect(button7, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(7); });

        button8 = new QPushButton("8. Remove Accounts through the Account_number", this);
        connect(button8, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(8); });

        vbox = new QVBoxLayout();

        vbox->addWidget(image_label);
        vbox->addWidget(button1);
        vbox->addWidget(button2);
        vbox->addWidget(button3);
        vbox->addWidget(button4);
        vbox->addWidget(button5);
        vbox->addWidget(button6);
        vbox->addWidget(button7);
        vbox->addWidget(button8);
        vbox->setAlignment(Qt::AlignCenter);

        central_widget->setLayout(vbox);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        wid_1 = new QWidget();
        wid_1->setWindowTitle("Create Administrator");

        QLabel *message1 = new QLabel("Enter your Desired Account Number", this);
        new_adm_account_number = new QLineEdit(this);

        QHBoxLayout *wid_1_hbox1 = new QHBoxLayout();
        wid_1_hbox1->addWidget(message1, Qt::AlignCenter);
        wid_1_hbox1->addWidget(new_adm_account_number, Qt::AlignCenter);

        QLabel *message2 = new QLabel("Enter your Desired Password", this);
        new_adm_password = new QLineEdit(this);

        QHBoxLayout *wid_1_hbox2 = new QHBoxLayout();
        wid_1_hbox2->addWidget(message2, Qt::AlignCenter);
        wid_1_hbox2->addWidget(new_adm_password, Qt::AlignCenter);

        QLabel *message3 = new QLabel("Confirmed Password", this);
        new_adm_password_confirmation = new QLineEdit(this);
        new_adm_password_confirmation->setEchoMode(QLineEdit::Password);

        QHBoxLayout *wid_1_hbox3 = new QHBoxLayout();
        wid_1_hbox3->addWidget(message3, Qt::AlignCenter);
        wid_1_hbox3->addWidget(new_adm_password_confirmation, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::wid_1_config);

        back_button = new QPushButton("Previous Menu");
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        wid_1_vbox = new QVBoxLayout();
        wid_1_vbox->addLayout(wid_1_hbox1);
        wid_1_vbox->addLayout(wid_1_hbox2);
        wid_1_vbox->addLayout(wid_1_hbox3);
        wid_1_vbox->addWidget(confirm_button);
        wid_1_vbox->addWidget(back_button);

        box_1 = new QGroupBox();
        box_1->setLayout(wid_1_vbox);
        box_1->setFixedSize(500, 300);

        HBOX_1 = new QHBoxLayout();
        HBOX_1->addWidget(box_1);

        wid_1->setLayout(HBOX_1);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_2 = new QWidget();
        wid_2->setWindowTitle("Display The Accounts Table");

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_accounts_table);

        back_button = new QPushButton("Previous Menu");
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_2 = new QLabel("Are sure you want to display The Accounts Table?", this);

        wid_2_vbox = new QVBoxLayout();
        wid_2_vbox->addWidget(message_2);
        wid_2_vbox->addWidget(confirm_button);
        wid_2_vbox->addWidget(back_button);

        box_2 = new QGroupBox();
        box_2->setLayout(wid_2_vbox);
        box_2->setFixedSize(500, 300);

        HBOX_2 = new QHBoxLayout();
        HBOX_2->addWidget(box_2);

        wid_2->setLayout(HBOX_2);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_3 = new QWidget();
        wid_3->setWindowTitle("Display Specific Account through the Account number");
        wid_3->resize(600, 300);

        QLabel *wid_3_label = new QLabel("DISPLAY SPECIFIC ACCOUNT INFO", this);
        wid_3_account_number = new QLineEdit(this);

        QHBoxLayout *wid_3_hbox = new QHBoxLayout();
        wid_3_hbox->addWidget(wid_3_label, Qt::AlignCenter);
        wid_3_hbox->addWidget(wid_3_account_number, Qt::AlignCenter);

        QLabel *message_3 = new QLabel("Are sure you want to display the entered Account?", this);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_specific_accounts_table);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        wid_3_vbox = new QVBoxLayout();
        wid_3_vbox->addLayout(wid_3_hbox);
        wid_3_vbox->addWidget(message_3);
        wid_3_vbox->addWidget(confirm_button);
        wid_3_vbox->addWidget(back_button);

        box_3 = new QGroupBox();
        box_3->setLayout(wid_3_vbox);
        box_3->setFixedSize(500, 300);

        HBOX_3 = new QHBoxLayout();
        HBOX_3->addWidget(box_3);

        wid_3->setLayout(HBOX_3);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_4 = new QWidget();
        wid_4->setWindowTitle("Display People in Debt");
        wid_4->resize(600, 300);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_people_in_debt);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_4 = new QLabel("Are sure you want to display all People in bebt??", this);

        wid_4_vbox = new QVBoxLayout();
        wid_4_vbox->addWidget(message_4, Qt::AlignCenter);
        wid_4_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_4_vbox->addWidget(back_button, Qt::AlignCenter | Qt::AlignBottom);

        box_4 = new QGroupBox();
        box_4->setLayout(wid_4_vbox);
        box_4->setFixedSize(500, 300);

        HBOX_4 = new QHBoxLayout();
        HBOX_4->addWidget(box_4);

        wid_4->setLayout(HBOX_4);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_5 = new QWidget();
        wid_5->setWindowTitle("Display Specific Person in Debt through the Account_number");
        wid_5->resize(600, 300);

        QLabel *wid_5_label = new QLabel("DISPLAY SPECIFIC ACCOUNT IN DEBT", this);
        wid_5_account_number = new QLineEdit(this);

        QHBoxLayout *wid_5_hbox = new QHBoxLayout();
        wid_5_hbox->addWidget(wid_5_label, Qt::AlignCenter);
        wid_5_hbox->addWidget(wid_5_account_number, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_specific_accounts_in_debt);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_5 = new QLabel("Are sure you want to display the entered Account?", this);

        wid_5_vbox = new QVBoxLayout();
        wid_5_vbox->addLayout(wid_5_hbox, Qt::AlignCenter);
        wid_5_vbox->addWidget(message_5, Qt::AlignCenter);
        wid_5_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_5_vbox->addWidget(back_button, Qt::AlignCenter | Qt::AlignBottom);

        box_5 = new QGroupBox();
        box_5->setLayout(wid_5_vbox);
        box_5->setFixedSize(500, 300);

        HBOX_5 = new QHBoxLayout();
        HBOX_5->addWidget(box_5);

        wid_5->setLayout(HBOX_5);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_6 = new QWidget();
        wid_6->setWindowTitle("Display all Transactions History for an Account_number");
        wid_6->resize(600, 300);

        QLabel *wid_6_label = new QLabel("DISPLAY TRANSACTION HISTORY", this);
        wid_6_account_number = new QLineEdit(this);
        QHBoxLayout *wid_6_hbox = new QHBoxLayout();
        wid_6_hbox->addWidget(wid_6_label, Qt::AlignCenter);
        wid_6_hbox->addWidget(wid_6_account_number, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_transactions_history);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_6 = new QLabel("Are sure you want to display the Transaction History?", this);

        wid_6_vbox = new QVBoxLayout();
        wid_6_vbox->addLayout(wid_6_hbox, Qt::AlignCenter);
        wid_6_vbox->addWidget(message_6, Qt::AlignCenter);
        wid_6_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_6_vbox->addWidget(back_button, Qt::AlignCenter | Qt::AlignBottom);

        box_6 = new QGroupBox();
        box_6->setLayout(wid_6_vbox);
        box_6->setFixedSize(500, 300);

        HBOX_6 = new QHBoxLayout();
        HBOX_6->addWidget(box_6);

        wid_6->setLayout(HBOX_6);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_7 = new QWidget();
        wid_7->setWindowTitle("Display Account Relative  Transaction History");
        wid_7->resize(600, 300);

        specific_transaction_history_message1 = new QLabel("Enter Account Number", this);
        specific_account_number_transac = new QLineEdit(this);
        specific_transac_hbox1 = new QHBoxLayout();
        specific_transac_hbox1->addWidget(specific_transaction_history_message1, Qt::AlignCenter);
        specific_transac_hbox1->addWidget(specific_account_number_transac, Qt::AlignCenter);

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
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_relative_transactions_history);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        wid_7_vbox = new QVBoxLayout();
        wid_7_vbox->addLayout(specific_transac_hbox1, Qt::AlignCenter);
        wid_7_vbox->addWidget(calendar, Qt::AlignCenter);
        wid_7_vbox->addWidget(choice, Qt::AlignCenter);
        wid_7_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_7_vbox->addWidget(back_button, Qt::AlignCenter);
        wid_7_vbox->setAlignment(Qt::AlignCenter);

        box_7 = new QGroupBox();
        box_7->setLayout(wid_7_vbox);
        box_7->setFixedSize(500, 300);

        HBOX_7 = new QHBoxLayout();
        HBOX_7->addWidget(box_7);

        wid_7->setLayout(HBOX_7);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_8 = new QWidget();
        wid_8->setWindowTitle("Remove Accounts through the Account_number");
        wid_8->resize(600, 300);

        QLabel *wid_8_label = new QLabel("DELETE ACCOUNT", this);
        wid_8_account_number = new QLineEdit(this);
        QHBoxLayout *wid_8_hbox = new QHBoxLayout();
        wid_8_hbox->addWidget(wid_8_label, Qt::AlignCenter);
        wid_8_hbox->addWidget(wid_8_account_number, Qt::AlignCenter);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::delete_accounts);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_8 = new QLabel("Are sure you want to delete the entered Account?", this);

        wid_8_vbox = new QVBoxLayout();
        wid_8_vbox->addLayout(wid_8_hbox, Qt::AlignCenter);
        wid_8_vbox->addWidget(message_8, Qt::AlignCenter);
        wid_8_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_8_vbox->addWidget(back_button, Qt::AlignCenter | Qt::AlignBottom);

        box_8 = new QGroupBox();
        box_8->setLayout(wid_8_vbox);
        box_8->setFixedSize(500, 300);

        HBOX_8 = new QHBoxLayout();
        HBOX_8->addWidget(box_8);

        wid_8->setLayout(HBOX_8);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        window_stack->addWidget(central_widget);
        window_stack->addWidget(wid_1);
        window_stack->addWidget(wid_2);
        window_stack->addWidget(wid_3);
        window_stack->addWidget(wid_4);
        window_stack->addWidget(wid_5);
        window_stack->addWidget(wid_6);
        window_stack->addWidget(wid_7);
        window_stack->addWidget(wid_8);
}

void adm_option_main_window::back_button_func()
{
        int current_index = window_stack->currentIndex();

        if (current_index)
                window_stack->setCurrentIndex(0);
}

void adm_option_main_window::wid_1_config()
{
        int account_number = new_adm_account_number->text().toInt();
        std::string password = new_adm_password->text().toStdString();
        std::string password_confirmation = new_adm_password_confirmation->text().toStdString();

        if (password.compare(password_confirmation))
        {
                new_adm_password_confirmation->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(this, "Warning!", "Password Confirmation Incorrect, try again");

                return;
        }

        new_adm_password_confirmation->setStyleSheet("border: 1px solid gray");

        std::string hash_password = BANK::hashing_password(password);

        BANK::create_adm(connection, account_number, hash_password);

        QMessageBox::information(this, "Password Confirmation Correct, Account created successfully", "Password Confirmation Correct, Account created successfully");

        new_adm_account_number->clear();
        new_adm_password->clear();
        new_adm_password_confirmation->clear();

        hash_password.clear();
        password.clear();
        password_confirmation.clear();
}

void adm_option_main_window::display_accounts_table()
{
        BANK::Qt_display_accounts_table(connection);
}

void adm_option_main_window::display_specific_accounts_table()
{
        int account_number = wid_3_account_number->text().toInt();

        BANK::Qt_display_specific_accounts(connection, account_number);

        wid_3_account_number->clear();
}

void adm_option_main_window::display_people_in_debt()
{
        BANK::Qt_display_people_in_debt(connection);
}

void adm_option_main_window::display_specific_accounts_in_debt()
{
        int account_number = wid_5_account_number->text().toInt();

        BANK::Qt_display_specific_accounts_in_debt(connection, account_number);
}

void adm_option_main_window::display_transactions_history()
{
        int account_number = wid_6_account_number->text().toInt();

        Transactions::Qt_display_transactions_history(connection, account_number);

        wid_6_account_number->clear();
}

void adm_option_main_window::display_relative_transactions_history()
{
        int account_number = specific_account_number_transac->text().toInt();
        QString date = selected_date.toString(Qt::ISODate);
        int selected_choice = choice->currentIndex();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

        Transactions::Qt_display_specific_transactions_history(connection, account_number, date.toStdString(), selected_choice);

        specific_account_number_transac->clear();
}

void adm_option_main_window::delete_accounts()
{
        int account_number = wid_7_account_number->text().toInt();

        Account::Qt_remove_accounts(connection, account_number);

        wid_7_account_number->clear();
}