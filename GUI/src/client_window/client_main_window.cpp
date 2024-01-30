#include "client_main_window.h"
#include <info_main_window.h>
#include <database.h>
#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

client_main_window ::client_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Administrator");
    resize(600, 600);

    central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QStackedWidget *window_stack = new QStackedWidget();

    create_account = new QPushButton("1. New to our Bank and Would like to Create an Account", this);
    connect(create_account, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(1); });

    account_inquiry = new QPushButton("2. Already possess an Account and would like to process some inquiries relative to it", this);
    connect(account_inquiry, &QPushButton::clicked, this, &client_main_window::account_inquiry_func);

    bank_info = new QPushButton("3. Information on our Bank", this);
    connect(bank_info, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(2); });

    vbox = new QVBoxLayout(central_widget);
    vbox->addWidget(create_account, Qt::AlignCenter);
    vbox->addWidget(account_inquiry, Qt::AlignCenter);
    vbox->addWidget(bank_info, Qt::AlignCenter);
    vbox->setAlignment(Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    create_account_widget = new QWidget();
    create_account_widget->setWindowTitle("Create Account");

    message = new QLabel("Please Provide Us with the following Information in order to create your account. Make sure You enter the Correct Information", this);

    national_ID = new QLabel("National ID with at least a letter within it: ", this);
    insert_national_ID = new QLineEdit(this);
    hbox1 = new QHBoxLayout(this);
    hbox1->addWidget(national_ID);
    hbox1->addWidget(insert_national_ID);

    first_name = new QLabel("First Name: ", this);
    insert_first_name = new QLineEdit(this);
    hbox2 = new QHBoxLayout(this);
    hbox2->addWidget(first_name);
    hbox2->addWidget(insert_first_name);

    last_name = new QLabel("Last Name: ", this);
    insert_last_name = new QLineEdit(this);
    hbox3 = new QHBoxLayout(this);
    hbox3->addWidget(last_name);
    hbox3->addWidget(insert_last_name);

    date_birth = new QLabel("Date of Birth ( 2024-01-31 ): ", this);
    insert_date_birth = new QLineEdit(this);
    hbox4 = new QHBoxLayout(this);
    hbox4->addWidget(date_birth);
    hbox4->addWidget(insert_date_birth);

    phone_number = new QLabel("Phone Number: ", this);
    insert_phone_number = new QLineEdit(this);
    hbox5 = new QHBoxLayout(this);
    hbox5->addWidget(phone_number);
    hbox5->addWidget(insert_phone_number);

    email = new QLabel("Email: ", this);
    insert_email = new QLineEdit(this);
    hbox6 = new QHBoxLayout(this);
    hbox6->addWidget(email);
    hbox6->addWidget(insert_email);

    address = new QLabel("Address ( Taiwan-Taipei_City-Datong_District-Zhongshan_Road-001 ): ", this);
    insert_address = new QLineEdit(this);
    hbox7 = new QHBoxLayout(this);
    hbox7->addWidget(address);
    hbox7->addWidget(insert_address);

    balance = new QLabel("Your Account should have at least 100 when creating it, so Please enter those 100 dollars and not less \n\n"
                         "Interest Rate Scale according to your First Deposit which cant be changed\n\n"
                         "1. Balance = 100 ---> Interest Rate = 0\n\n"
                         "2. 500 > Balance > 100 ---> Interest Rate = 2%\n\n"
                         "3. 1000 > Balance >= 500 ---> Interest Rate = 5%\n\n"
                         "4. Balance > 1000 ---> Interest Rate = 7%\n\n",
                         this);
    insert_balance = new QLineEdit(this);
    QPushButton *confirm_balance = new QPushButton("Confirm", this);
    connect(confirm_balance, &QPushButton::clicked, this, &client_main_window::confirm_balance_func);
    hbox8->addWidget(balance);
    hbox8->addWidget(insert_balance);
    hbox8->addWidget(confirm_balance);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    window_stack->addWidget(central_widget);
    window_stack->addWidget(create_account_widget);
    window_stack->addWidget(bank_info_widget);
}

void client_main_window::confirm_balance_func()
{
    double balance = insert_balance->text().toDouble();

    while (balance < 100)
    {
        message2 = new QMessageBox(this);
        message2->warning(this, "UnSufficient amount", "Amount Entered is less than 100, Enter an reasonnable amount");

        insert_balance->clear();
        return;
    }
}
void client_main_window::account_inquiry_func()
{
    connection_details ID;
    ID.server = "localhost";
    ID.user = "root";
    ID.password = "sleyHortes1312";

    sql ::Connection *connection = connection_setup(&ID);
}
