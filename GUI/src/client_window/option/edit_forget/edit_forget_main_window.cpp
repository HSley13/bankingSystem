#include "edit_forget_main_window.h"
#include <database.h>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTableWidgetItem>
#include <QInputDialog>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

edit_forget_main_window::edit_forget_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    window_stack = new QStackedWidget();
    setCentralWidget(window_stack);

    setWindowTitle("Edit Personal Information/ change Password");
    resize(600, 600);

    central_widget = new QWidget(this);

    edit_perso = new QPushButton("1. Edit Personnal Information", this);
    connect(edit_perso, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(1); });

    change_password = new QPushButton("2. Change Password", this);
    connect(change_password, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(2); });

    forget_password = new QPushButton("3. Forget Password", this);
    connect(forget_password, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(3); });

    VBOX = new QVBoxLayout(central_widget);
    VBOX->addWidget(edit_perso, Qt::AlignCenter);
    VBOX->addWidget(change_password, Qt::AlignCenter);
    VBOX->addWidget(forget_password, Qt::AlignCenter);
    VBOX->setAlignment(Qt::AlignCenter);

    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    edit_perso_widget = new QWidget();

    name_button = new QPushButton("1. Edit First Name", this);
    connect(name_button, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(4); });

    email_button = new QPushButton("2. Edit Email", this);
    connect(email_button, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(5); });

    address_button = new QPushButton("3. Edit Email", this);
    connect(address_button, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(6); });

    phone_number_button = new QPushButton("4. Edit Email", this);
    connect(phone_number_button, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(7); });

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func);

    vbox_1 = new QVBoxLayout();
    vbox_1->addWidget(name_button, Qt::AlignCenter);
    vbox_1->addWidget(email_button, Qt::AlignCenter);
    vbox_1->addWidget(address_button, Qt::AlignCenter);
    vbox_1->addWidget(phone_number_button, Qt::AlignCenter);
    vbox_1->addWidget(back_button, Qt::AlignCenter);
    vbox_1->setAlignment(Qt::AlignCenter);

    edit_perso_widget->setLayout(vbox_1);

    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_first_name_widget = new QWidget();
    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_email_widget = new QWidget();
    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_address_widget = new QWidget();
    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_phone_number_widget = new QWidget();
    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    change_password_widget = new QWidget();

    account_number_change = new QLabel("Enter Account Number", this);
    insert_account_number_change = new QLineEdit(this);
    change_hbox_1 = new QHBoxLayout();
    change_hbox_1->addWidget(account_number_change, Qt::AlignCenter);
    change_hbox_1->addWidget(insert_account_number_change, Qt::AlignCenter);

    password_change = new QLabel("Enter Password", this);
    insert_password_change = new QLineEdit(this);
    insert_password_change->setEchoMode(QLineEdit::Password);
    change_hbox_2 = new QHBoxLayout();
    change_hbox_2->addWidget(password_change, Qt::AlignCenter);
    change_hbox_2->addWidget(insert_password_change, Qt::AlignCenter);

    new_password_change = new QLabel("Enter new  Password", this);
    insert_new_password_change = new QLineEdit(this);
    change_hbox_3 = new QHBoxLayout();
    change_hbox_3->addWidget(new_password_change, Qt::AlignCenter);
    change_hbox_3->addWidget(insert_new_password_change, Qt::AlignCenter);

    new_password_confirmation_change = new QLabel("Enter new Password Confirmation", this);
    insert_new_password_confirmation_change = new QLineEdit(this);
    insert_new_password_confirmation_change->setEchoMode(QLineEdit::Password);
    change_hbox_4 = new QHBoxLayout();
    change_hbox_4->addWidget(new_password_confirmation_change, Qt::AlignCenter);
    change_hbox_4->addWidget(insert_new_password_confirmation_change, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &edit_forget_main_window::confirm_change_func);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func);

    vbox_2 = new QVBoxLayout();
    vbox_2->addLayout(change_hbox_1, Qt::AlignCenter);
    vbox_2->addLayout(change_hbox_2, Qt::AlignCenter);
    vbox_2->addLayout(change_hbox_3, Qt::AlignCenter);
    vbox_2->addLayout(change_hbox_4, Qt::AlignCenter);
    vbox_2->addWidget(confirm_button, Qt::AlignCenter);
    vbox_2->addWidget(back_button, Qt::AlignCenter);
    vbox_2->setAlignment(Qt::AlignCenter);

    change_password_widget->setLayout(vbox_2);

    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    forget_password_widget = new QWidget();

    account_number_forget = new QLabel("Enter Account Number", this);
    insert_account_number_forget = new QLineEdit(this);
    forget_hbox_1 = new QHBoxLayout();
    forget_hbox_1->addWidget(account_number_forget, Qt::AlignCenter);
    forget_hbox_1->addWidget(insert_account_number_forget, Qt::AlignCenter);

    password_forget = new QLabel("Enter Password", this);
    insert_password_forget = new QLineEdit(this);
    insert_password_forget->setEchoMode(QLineEdit::Password);
    forget_hbox_2 = new QHBoxLayout();
    forget_hbox_2->addWidget(password_forget, Qt::AlignCenter);
    forget_hbox_2->addWidget(insert_password_forget, Qt::AlignCenter);

    national_ID_forget = new QLabel("Enter Your National ID", this);
    insert_national_ID_forget = new QLineEdit(this);
    forget_hbox_3 = new QHBoxLayout();
    forget_hbox_3->addWidget(national_ID_forget, Qt::AlignCenter);
    forget_hbox_3->addWidget(insert_national_ID_forget, Qt::AlignCenter);

    date_birth_forget = new QLabel("Enter Date of Birth", this);
    insert_date_birth_forget = new QLineEdit(this);
    forget_hbox_4 = new QHBoxLayout();
    forget_hbox_4->addWidget(date_birth_forget, Qt::AlignCenter);
    forget_hbox_4->addWidget(insert_date_birth_forget, Qt::AlignCenter);

    new_password_forget = new QLabel("Enter new  Password", this);
    insert_new_password_forget = new QLineEdit(this);
    forget_hbox_5 = new QHBoxLayout();
    forget_hbox_5->addWidget(new_password_forget, Qt::AlignCenter);
    forget_hbox_5->addWidget(insert_new_password_forget, Qt::AlignCenter);

    new_password_confirmation_forget = new QLabel("Enter new Password Confirmation", this);
    insert_new_password_confirmation_forget = new QLineEdit(this);
    insert_new_password_confirmation_forget->setEchoMode(QLineEdit::Password);
    forget_hbox_6 = new QHBoxLayout();
    forget_hbox_6->addWidget(new_password_confirmation_forget, Qt::AlignCenter);
    forget_hbox_6->addWidget(insert_new_password_confirmation_forget, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &edit_forget_main_window::confirm_forget_func);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func);

    vbox_3 = new QVBoxLayout();
    vbox_3->addLayout(forget_hbox_1, Qt::AlignCenter);
    vbox_3->addLayout(forget_hbox_2, Qt::AlignCenter);
    vbox_3->addLayout(forget_hbox_3, Qt::AlignCenter);
    vbox_3->addLayout(forget_hbox_4, Qt::AlignCenter);
    vbox_3->addLayout(forget_hbox_5, Qt::AlignCenter);
    vbox_3->addLayout(forget_hbox_6, Qt::AlignCenter);
    vbox_3->addWidget(confirm_button, Qt::AlignCenter);
    vbox_3->addWidget(back_button, Qt::AlignCenter);
    vbox_3->setAlignment(Qt::AlignCenter);

    forget_password_widget->setLayout(vbox_3);

    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    window_stack->addWidget(central_widget);
    window_stack->addWidget(edit_perso_widget);
    window_stack->addWidget(change_password_widget);
    window_stack->addWidget(forget_password_widget);
    window_stack->addWidget(edit_first_name_widget);
    window_stack->addWidget(edit_email_widget);
    window_stack->addWidget(edit_address_widget);
    window_stack->addWidget(edit_phone_number_widget);
}

void edit_forget_main_window::back_button_func()
{
    int current_index = window_stack->currentIndex();

    if (current_index)
        window_stack->setCurrentIndex(0);
}

void edit_forget_main_window::confirm_change_func()
{
    int account_number = insert_account_number_change->text().toInt();
    std ::string password = insert_password_change->text().toStdString();

    std ::string new_password = insert_new_password_change->text().toStdString();
    std ::string new_password_confirmation = insert_new_password_confirmation_change->text().toStdString();

    connection_details ID;
    ID.server = "localhost";
    ID.user = "root";
    ID.password = "sleyHortes1312";

    sql ::Connection *connection = connection_setup(&ID);

    std ::string hashed_password = BANK ::Qt_retrieve_hashed_password(connection, account_number);

    if (hashed_password == "")
        return;

    if (!BANK ::verifying_password(password, hashed_password))
    {
        QMessageBox::warning(nullptr, "Balance Check", "Password Incorrect");

        return;
    }

    if (new_password.compare(new_password_confirmation))
    {
        QMessageBox::warning(this, "Confirmation Incorrect", "New password Confirmation Incorrect");

        return;
    }

    std ::string new_hash_password = BANK ::hashing_password(new_password);

    call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

    new_password.clear();
    new_password_confirmation.clear();

    QMessageBox::information(this, "OK", "Account Password Changed Successfully");
}

void edit_forget_main_window::confirm_forget_func()
{
    int account_number = insert_account_number_forget->text().toInt();
    std ::string password = insert_password_forget->text().toStdString();

    std ::string new_password = insert_new_password_forget->text().toStdString();
    std ::string new_password_confirmation = insert_new_password_confirmation_forget->text().toStdString();

    std ::string national_ID = insert_national_ID_forget->text().toStdString();
    std ::string date_birth = insert_date_birth_forget->text().toStdString();

    connection_details ID;
    ID.server = "localhost";
    ID.user = "root";
    ID.password = "sleyHortes1312";

    sql ::Connection *connection = connection_setup(&ID);

    std ::string hashed_password = BANK ::Qt_retrieve_hashed_password(connection, account_number);

    if (hashed_password == "")
        return;

    if (!BANK ::verifying_password(password, hashed_password))
    {
        QMessageBox::warning(nullptr, "Balance Check", "Password Incorrect");

        return;
    }

    if (!BANK ::authentification_check(connection, account_number, national_ID, date_birth))
    {
        QMessageBox::warning(this, "Information Incorrect", "The provided Information are Incorrect, check the National ID and the Date of Birth");

        return;
    }

    if (new_password.compare(new_password_confirmation))
    {
        QMessageBox::warning(this, "Confirmation Incorrect", "New password Confirmation Incorrect");

        return;
    }

    std ::string new_hash_password = BANK ::hashing_password(new_password);

    call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

    new_password.clear();
    new_password_confirmation.clear();

    QMessageBox::information(this, "OK", "Account Password Changed Successfully");
}
