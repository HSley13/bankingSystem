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

        central_widget = new QWidget(this);
        setCentralWidget(window_stack);
        setWindowTitle("ADM window Management");

        resize(600, 600);

        button1 = new QPushButton("1. Create Administrator", this);
        connect(button1, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(1); });

        button2 = new QPushButton("2. Display The Accounts Table", this);
        connect(button2, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(2); });

        button3 = new QPushButton("3. Create Administrator", this);
        connect(button3, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(3); });

        button4 = new QPushButton("4. Create Administrator", this);
        connect(button4, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(4); });

        button5 = new QPushButton("5. Create Administrator", this);
        connect(button5, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(5); });

        button6 = new QPushButton("6. Create Administrator", this);
        connect(button6, &QPushButton::clicked, this, [=]()
                { window_stack->setCurrentIndex(6); });

        button7 = new QPushButton("7. Create Administrator", this);
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
        wid_1 = new QWidget(this);
        wid_1->setWindowTitle("Create Administrator");

        QLabel *message1 = new QLabel("Enter your Desired Account Number", this);
        new_adm_account_number = new QLineEdit(this);
        QHBoxLayout *wid_1_hbox1 = new QHBoxLayout();
        wid_1_hbox1->addWidget(message1);
        wid_1_hbox1->addWidget(new_adm_account_number);

        QLabel *message2 = new QLabel("Enter your Desired Password", this);
        new_adm_password = new QLineEdit(this);
        QHBoxLayout *wid_1_hbox2 = new QHBoxLayout();
        wid_1_hbox2->addWidget(message2);
        wid_1_hbox2->addWidget(new_adm_password);

        QLabel *message3 = new QLabel("Confirmed Password", this);
        new_adm_password_confirmation = new QLineEdit(this);
        new_adm_password_confirmation->setEchoMode(QLineEdit::Password);
        QHBoxLayout *wid_1_hbox3 = new QHBoxLayout();
        wid_1_hbox3->addWidget(message3);
        wid_1_hbox3->addWidget(new_adm_password_confirmation);

        QPushButton *wid_1_confirm_button = new QPushButton("Confirm", this);
        connect(wid_1_confirm_button, &QPushButton::clicked, this, &adm_option_main_window::wid_1_conf);

        QPushButton *back = new QPushButton("Previous Menu", this);
        connect(back, &QPushButton::clicked, this, &adm_option_main_window::back_button);

        QVBoxLayout *wid1_vbox = new QVBoxLayout();
        wid1_vbox->addLayout(wid_1_hbox1, Qt::AlignCenter);
        wid1_vbox->addLayout(wid_1_hbox2, Qt::AlignCenter);
        wid1_vbox->addLayout(wid_1_hbox3, Qt::AlignCenter);

        wid_1->setLayout(wid1_vbox);
        wid_1->layout()->addWidget(wid_1_confirm_button);
        wid_1->layout()->addWidget(back);
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_2 = new QWidget(this);
        wid_2->setWindowTitle("Display The Accounts Table");

        QPushButton *wid_2_button = new QPushButton("Click on this Button to display The Account Table", this);
        connect(wid_2_button, &QPushButton::clicked, this, &adm_option_main_window::display_accounts_table);

        QVBoxLayout *wid_2_vbox = new QVBoxLayout(this);
        wid_2_vbox->addWidget(wid_2_button, Qt::AlignCenter | Qt::AlignTop);

        wid_2->setLayout(wid_2_vbox);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        wid_3 = new QWidget(this);
        wid_3->setWindowTitle("Display Specific Account through the Account_number");

        wid_4 = new QWidget(this);
        wid_4->setWindowTitle("Display People in Debt");

        wid_5 = new QWidget(this);
        wid_5->setWindowTitle("Display Specific Person in Debt through the Account_number");

        wid_6 = new QWidget(this);
        wid_6->setWindowTitle("Display all Transactions History for an Account_number ");

        wid_7 = new QWidget(this);
        wid_7->setWindowTitle("Remove Accounts through the Account_number");

        window_stack->addWidget(central_widget);
        window_stack->addWidget(wid_1);
        window_stack->addWidget(wid_2);
        window_stack->addWidget(wid_3);
        window_stack->addWidget(wid_4);
        window_stack->addWidget(wid_5);
        window_stack->addWidget(wid_6);
        window_stack->addWidget(wid_7);
}

void adm_option_main_window::wid_1_conf()
{
        int account_number = new_adm_account_number->text().toInt();
        std ::string password = new_adm_password->text().toStdString();
        std ::string password_confirmation = new_adm_password_confirmation->text().toStdString();

        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql ::Connection *connection = connection_setup(&ID);

        if (password.compare(password_confirmation))
        {
                QMessageBox *message = new QMessageBox(this);
                message->warning(this, "Password Confirmation Incorrect, try again", "Password Confirmation Incorrect, try again");
                return;
        }

        std ::string hash_password = BANK ::hashing_password(password);

        BANK ::create_adm(connection, account_number, hash_password);

        QMessageBox *message = new QMessageBox(this);
        message->information(this, "Password Confirmation Correct, Account created successfully", "Password Confirmation Correct, Account created successfully");

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

void adm_option_main_window::back_button()
{
        int current_index = window_stack->currentIndex();

        if (current_index)
                window_stack->setCurrentIndex(0);
}