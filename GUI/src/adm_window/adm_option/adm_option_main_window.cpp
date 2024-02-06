#include "adm_option_main_window.h"
#include <database.h>
#include <QMainWindow>
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

adm_option_main_window::adm_option_main_window(QWidget *parent)
    : QMainWindow(parent)
{
        window_stack = new QStackedWidget();
        setCentralWidget(window_stack);
        setStyleSheet("font-family: Herculanum; font-size: 20; font: bold italic 14px;");

        central_widget = new QWidget(this);

        setWindowTitle("ADM window Management");

        resize(600, 600);

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

        button7 = new QPushButton("7. Remove Accounts through the Account_number", this);
        connect(button7, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(7); });

        vbox = new QVBoxLayout(central_widget);
        vbox->setAlignment(Qt::AlignCenter);

        vbox->addWidget(button1);
        vbox->addWidget(button2);
        vbox->addWidget(button3);
        vbox->addWidget(button4);
        vbox->addWidget(button5);
        vbox->addWidget(button6);
        vbox->addWidget(button7);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        wid_1 = new QWidget();
        wid_1->setWindowTitle("Create Administrator");

        QLabel *message1 = new QLabel("Enter your Desired Account Number", this);
        new_adm_account_number = new QLineEdit(this);
        QHBoxLayout *wid_1_hbox1 = new QHBoxLayout();
        wid_1_hbox1->addWidget(message1, Qt::AlignCenter);
        wid_1_hbox1->addWidget(new_adm_account_number, Qt::AlignCenter);
        wid_1_hbox1->setAlignment(Qt::AlignCenter);

        QLabel *message2 = new QLabel("Enter your Desired Password", this);
        new_adm_password = new QLineEdit(this);
        QHBoxLayout *wid_1_hbox2 = new QHBoxLayout();
        wid_1_hbox2->addWidget(message2, Qt::AlignCenter);
        wid_1_hbox2->addWidget(new_adm_password, Qt::AlignCenter);
        wid_1_hbox2->setAlignment(Qt::AlignCenter);

        QLabel *message3 = new QLabel("Confirmed Password", this);
        new_adm_password_confirmation = new QLineEdit(this);
        new_adm_password_confirmation->setEchoMode(QLineEdit::Password);
        QHBoxLayout *wid_1_hbox3 = new QHBoxLayout();
        wid_1_hbox3->addWidget(message3, Qt::AlignCenter);
        wid_1_hbox3->addWidget(new_adm_password_confirmation, Qt::AlignCenter);
        wid_1_hbox3->setAlignment(Qt::AlignCenter);

        QPushButton *wid_1_confirm_button = new QPushButton("Confirm", this);
        connect(wid_1_confirm_button, &QPushButton::clicked, this, &adm_option_main_window::wid_1_config);

        back_button1 = new QPushButton("Previous Menu");
        connect(back_button1, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_1_vbox = new QVBoxLayout();
        wid_1_vbox->addLayout(wid_1_hbox1);
        wid_1_vbox->addLayout(wid_1_hbox2);
        wid_1_vbox->addLayout(wid_1_hbox3);
        wid_1_vbox->addWidget(wid_1_confirm_button, Qt::AlignCenter);
        wid_1_vbox->addWidget(back_button1, Qt::AlignCenter, Qt::AlignBottom);
        wid_1_vbox->setAlignment(Qt::AlignCenter);

        wid_1->setLayout(wid_1_vbox);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_2 = new QWidget();
        wid_2->setWindowTitle("Display The Accounts Table");

        QPushButton *wid_2_button = new QPushButton("Confirm", this);
        connect(wid_2_button, &QPushButton::clicked, this, &adm_option_main_window::display_accounts_table);

        back_button2 = new QPushButton("Previous Menu");
        connect(back_button2, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_2_vbox = new QVBoxLayout();
        wid_2_vbox->addWidget(wid_2_button, Qt::AlignCenter);
        wid_2_vbox->addWidget(back_button2, Qt::AlignCenter | Qt::AlignBottom);
        wid_2_vbox->setAlignment(Qt::AlignCenter);

        wid_2->setLayout(wid_2_vbox);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_3 = new QWidget();
        wid_3->setWindowTitle("Display Specific Account through the Account number");

        QLabel *wid_3_label = new QLabel("Enter the Account You want to display", this);
        wid_3_account_number = new QLineEdit(this);
        QHBoxLayout *wid_3_hbox = new QHBoxLayout();
        wid_3_hbox->addWidget(wid_3_label, Qt::AlignCenter);
        wid_3_hbox->addWidget(wid_3_account_number, Qt::AlignCenter);
        wid_3_hbox->setAlignment(Qt::AlignCenter);

        QPushButton *wid_3_button = new QPushButton("Confirm", this);
        connect(wid_3_button, &QPushButton::clicked, this, &adm_option_main_window::display_specific_accounts_table);

        back_button3 = new QPushButton("Previous Menu", this);
        connect(back_button3, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_3_vbox = new QVBoxLayout();
        wid_3_vbox->addLayout(wid_3_hbox);
        wid_3_vbox->addWidget(wid_3_button, Qt::AlignCenter);
        wid_3_vbox->addWidget(back_button3, Qt::AlignCenter | Qt::AlignBottom);
        wid_3_vbox->setAlignment(Qt::AlignCenter);

        wid_3->setLayout(wid_3_vbox);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_4 = new QWidget();
        wid_4->setWindowTitle("Display People in Debt");

        QPushButton *wid_4_button = new QPushButton("Confirm", this);
        connect(wid_4_button, &QPushButton::clicked, this, &adm_option_main_window::display_people_in_debt);

        back_button4 = new QPushButton("Previous Menu", this);
        connect(back_button4, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_4_vbox = new QVBoxLayout();
        wid_4_vbox->addWidget(wid_4_button, Qt::AlignCenter);
        wid_4_vbox->addWidget(back_button4, Qt::AlignCenter | Qt::AlignBottom);
        wid_4_vbox->setAlignment(Qt::AlignCenter);

        wid_4->setLayout(wid_4_vbox);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_5 = new QWidget();
        wid_5->setWindowTitle("Display Specific Person in Debt through the Account_number");

        QLabel *wid_5_label = new QLabel("Enter the Account You want to display to see their debt", this);
        wid_5_account_number = new QLineEdit(this);
        QHBoxLayout *wid_5_hbox = new QHBoxLayout();
        wid_5_hbox->addWidget(wid_5_label, Qt::AlignCenter);
        wid_5_hbox->addWidget(wid_5_account_number, Qt::AlignCenter);
        wid_5_hbox->setAlignment(Qt::AlignCenter);

        QPushButton *wid_5_button = new QPushButton("Confirm", this);
        connect(wid_5_button, &QPushButton::clicked, this, &adm_option_main_window::display_specific_accounts_in_debt);

        back_button5 = new QPushButton("Previous Menu", this);
        connect(back_button5, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_5_vbox = new QVBoxLayout();
        wid_5_vbox->addLayout(wid_5_hbox);
        wid_5_vbox->addWidget(wid_5_button, Qt::AlignCenter);
        wid_5_vbox->addWidget(back_button5, Qt::AlignCenter | Qt::AlignBottom);
        wid_5_vbox->setAlignment(Qt::AlignCenter);

        wid_5->setLayout(wid_5_vbox);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_6 = new QWidget();
        wid_6->setWindowTitle("Display all Transactions History for an Account_number");

        QLabel *wid_6_label = new QLabel("Enter the Account You want to Display all Transactions History for", this);
        wid_6_account_number = new QLineEdit(this);
        QHBoxLayout *wid_6_hbox = new QHBoxLayout();
        wid_6_hbox->addWidget(wid_6_label, Qt::AlignCenter);
        wid_6_hbox->addWidget(wid_6_account_number, Qt::AlignCenter);
        wid_6_hbox->setAlignment(Qt::AlignCenter);

        QPushButton *wid_6_button = new QPushButton("Confirm", this);
        connect(wid_6_button, &QPushButton::clicked, this, &adm_option_main_window::display_transactions_history);

        back_button6 = new QPushButton("Previous Menu", this);
        connect(back_button6, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_6_vbox = new QVBoxLayout();
        wid_6_vbox->addLayout(wid_6_hbox);
        wid_6_vbox->addWidget(wid_6_button, Qt::AlignCenter);
        wid_6_vbox->addWidget(back_button6, Qt::AlignCenter | Qt::AlignBottom);
        wid_6_vbox->setAlignment(Qt::AlignCenter);

        wid_6->setLayout(wid_6_vbox);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_7 = new QWidget();
        wid_7->setWindowTitle("Remove Accounts through the Account_number");

        QLabel *wid_7_label = new QLabel("Enter the Account You want to delete", this);
        wid_7_account_number = new QLineEdit(this);
        QHBoxLayout *wid_7_hbox = new QHBoxLayout();
        wid_7_hbox->addWidget(wid_7_label, Qt::AlignCenter);
        wid_7_hbox->addWidget(wid_7_account_number, Qt::AlignCenter);
        wid_7_hbox->setAlignment(Qt::AlignCenter);

        QPushButton *wid_7_button = new QPushButton("Confirm", this);
        connect(wid_7_button, &QPushButton::clicked, this, &adm_option_main_window::delete_accounts);

        back_button7 = new QPushButton("Previous Menu", this);
        connect(back_button7, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_7_vbox = new QVBoxLayout();
        wid_7_vbox->addLayout(wid_7_hbox);
        wid_7_vbox->addWidget(wid_7_button, Qt::AlignCenter);
        wid_7_vbox->addWidget(back_button7, Qt::AlignCenter | Qt::AlignBottom);
        wid_7_vbox->setAlignment(Qt::AlignCenter);

        wid_7->setLayout(wid_7_vbox);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        window_stack->addWidget(central_widget);
        window_stack->addWidget(wid_1);
        window_stack->addWidget(wid_2);
        window_stack->addWidget(wid_3);
        window_stack->addWidget(wid_4);
        window_stack->addWidget(wid_5);
        window_stack->addWidget(wid_6);
        window_stack->addWidget(wid_7);
}

void adm_option_main_window::back_button_func()
{
        int current_index = window_stack->currentIndex();

        if (current_index)
                window_stack->setCurrentIndex(0);
}

void adm_option_main_window::wid_1_config()
{
        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        int account_number = new_adm_account_number->text().toInt();
        std ::string password = new_adm_password->text().toStdString();
        std ::string password_confirmation = new_adm_password_confirmation->text().toStdString();

        if (password.compare(password_confirmation))
        {
                new_adm_password_confirmation->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(this, "Warning!", "Password Confirmation Incorrect, try again");

                return;
        }

        new_adm_password_confirmation->setStyleSheet("border: 1px solid gray");

        std ::string hash_password = BANK ::hashing_password(password);

        BANK ::create_adm(connection, account_number, hash_password);

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
        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        BANK ::Qt_display_accounts_table(connection);
}

void adm_option_main_window::display_specific_accounts_table()
{
        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        int account_number = wid_3_account_number->text().toInt();

        BANK ::Qt_display_specific_accounts(connection, account_number);

        wid_3_account_number->clear();
}

void adm_option_main_window::display_people_in_debt()
{
        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        BANK ::Qt_display_people_in_debt(connection);
}

void adm_option_main_window::display_specific_accounts_in_debt()
{
        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        int account_number = wid_5_account_number->text().toInt();

        BANK ::Qt_display_specific_accounts_in_debt(connection, account_number);
}

void adm_option_main_window::display_transactions_history()
{
        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        int account_number = wid_6_account_number->text().toInt();

        Transactions ::Qt_display_transactions_history(connection, account_number);

        wid_6_account_number->clear();
}

void adm_option_main_window::delete_accounts()
{
        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        int account_number = wid_7_account_number->text().toInt();

        Account::Qt_remove_accounts(connection, account_number);

        wid_7_account_number->clear();
}