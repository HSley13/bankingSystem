#include "edit_forget_main_window.h"
#include <QMessageBox>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

edit_forget_main_window::edit_forget_main_window(sql::Connection *db_connection, QWidget *parent)
    : QMainWindow(parent), connection(db_connection)
{
    window_stack = new QStackedWidget();
    setCentralWidget(window_stack);
    setWindowTitle("Edit Personal Information/ change Password");
    setStyleSheet("color: beige;"
                  "font-family: Arial Black;"
                  "font-size: 20;"
                  "font: bold italic 14px;"
                  "background-color: black;");
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

    QLabel *image_label = new QLabel(this);
    QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/password.jpeg");
    image_label->setPixmap(image.scaled(500, 300, Qt::KeepAspectRatio));
    image_label->setScaledContents(true);

    VBOX = new QVBoxLayout(central_widget);
    VBOX->addWidget(image_label, Qt::AlignCenter);
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

    address_button = new QPushButton("3. Edit Address", this);
    connect(address_button, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(6); });

    phone_number_button = new QPushButton("4. Edit Phone Number", this);
    connect(phone_number_button, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(7); });

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func);

    QLabel *image_label_1 = new QLabel(this);
    QPixmap image_1("/Users/test/Documents/banking_system/GUI/src/ressources/edit.jpeg");
    image_label_1->setPixmap(image_1.scaled(500, 300, Qt::KeepAspectRatio));
    image_label_1->setScaledContents(true);

    vbox_1 = new QVBoxLayout();
    vbox_1->addWidget(image_label_1, Qt::AlignCenter);
    vbox_1->addWidget(name_button, Qt::AlignCenter);
    vbox_1->addWidget(email_button, Qt::AlignCenter);
    vbox_1->addWidget(address_button, Qt::AlignCenter);
    vbox_1->addWidget(phone_number_button, Qt::AlignCenter);
    vbox_1->addWidget(back_button, Qt::AlignCenter);
    vbox_1->setAlignment(Qt::AlignCenter);

    edit_perso_widget->setLayout(vbox_1);

    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_first_name_widget = new QWidget();

    account_number_1 = new QLabel("Enter Account Number", this);
    insert_account_number_1 = new QLineEdit(this);
    edit_hbox_0 = new QHBoxLayout();
    edit_hbox_0->addWidget(account_number_1, Qt::AlignCenter);
    edit_hbox_0->addWidget(insert_account_number_1, Qt::AlignCenter);

    password_1 = new QLabel("Enter Password", this);
    insert_password_1 = new QLineEdit(this);
    insert_password_1->setEchoMode(QLineEdit::Password);
    edit_hbox_1 = new QHBoxLayout();
    edit_hbox_1->addWidget(password_1, Qt::AlignCenter);
    edit_hbox_1->addWidget(insert_password_1, Qt::AlignCenter);

    edit_name = new QLabel("Enter new First Name", this);
    insert_name = new QLineEdit(this);
    edit_hbox_2 = new QHBoxLayout();
    edit_hbox_2->addWidget(edit_name, Qt::AlignCenter);
    edit_hbox_2->addWidget(insert_name, Qt::AlignCenter);

    edit_name_confirmation = new QLabel("Enter new First Name Confirmation", this);
    insert_name_confirmation = new QLineEdit(this);
    edit_hbox_3 = new QHBoxLayout();
    edit_hbox_3->addWidget(edit_name_confirmation, Qt::AlignCenter);
    edit_hbox_3->addWidget(insert_name_confirmation, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black;"
                                  "background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &edit_forget_main_window::confirm_edit_name_func);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func2);

    vbox_4 = new QVBoxLayout();
    vbox_4->addLayout(edit_hbox_0, Qt::AlignCenter);
    vbox_4->addLayout(edit_hbox_1, Qt::AlignCenter);
    vbox_4->addLayout(edit_hbox_2, Qt::AlignCenter);
    vbox_4->addLayout(edit_hbox_3, Qt::AlignCenter);
    vbox_4->addWidget(confirm_button, Qt::AlignCenter);
    vbox_4->addWidget(back_button, Qt::AlignCenter);
    vbox_4->setAlignment(Qt::AlignCenter);

    edit_first_name_widget->setLayout(vbox_4);
    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_email_widget = new QWidget();

    account_number_2 = new QLabel("Enter Account Number", this);
    insert_account_number_2 = new QLineEdit(this);
    edit_email_hbox_0 = new QHBoxLayout();
    edit_email_hbox_0->addWidget(account_number_2, Qt::AlignCenter);
    edit_email_hbox_0->addWidget(insert_account_number_2, Qt::AlignCenter);

    password_2 = new QLabel("Enter Password", this);
    insert_password_2 = new QLineEdit(this);
    insert_password_2->setEchoMode(QLineEdit::Password);
    edit_email_hbox_1 = new QHBoxLayout();
    edit_email_hbox_1->addWidget(password_2, Qt::AlignCenter);
    edit_email_hbox_1->addWidget(insert_password_2, Qt::AlignCenter);

    edit_email = new QLabel("Enter new Email", this);
    insert_email = new QLineEdit(this);
    edit_email_hbox_2 = new QHBoxLayout();
    edit_email_hbox_2->addWidget(edit_email, Qt::AlignCenter);
    edit_email_hbox_2->addWidget(insert_email, Qt::AlignCenter);

    edit_email_confirmation = new QLabel("Enter new Email Confirmation", this);
    insert_email_confirmation = new QLineEdit(this);
    edit_email_hbox_3 = new QHBoxLayout();
    edit_email_hbox_3->addWidget(edit_email_confirmation, Qt::AlignCenter);
    edit_email_hbox_3->addWidget(insert_email_confirmation, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black;"
                                  "background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &edit_forget_main_window::confirm_edit_email_func);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func2);

    vbox_5 = new QVBoxLayout();
    vbox_5->addLayout(edit_email_hbox_0, Qt::AlignCenter);
    vbox_5->addLayout(edit_email_hbox_1, Qt::AlignCenter);
    vbox_5->addLayout(edit_email_hbox_2, Qt::AlignCenter);
    vbox_5->addLayout(edit_email_hbox_3, Qt::AlignCenter);
    vbox_5->addWidget(confirm_button, Qt::AlignCenter);
    vbox_5->addWidget(back_button, Qt::AlignCenter);
    vbox_5->setAlignment(Qt::AlignCenter);

    edit_email_widget->setLayout(vbox_5);
    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_address_widget = new QWidget();

    account_number_3 = new QLabel("Enter Account Number", this);
    insert_account_number_3 = new QLineEdit(this);
    edit_address_hbox_0 = new QHBoxLayout();
    edit_address_hbox_0->addWidget(account_number_3, Qt::AlignCenter);
    edit_address_hbox_0->addWidget(insert_account_number_3, Qt::AlignCenter);

    password_3 = new QLabel("Enter Password", this);
    insert_password_3 = new QLineEdit(this);
    insert_password_3->setEchoMode(QLineEdit::Password);
    edit_address_hbox_1 = new QHBoxLayout();
    edit_address_hbox_1->addWidget(password_3, Qt::AlignCenter);
    edit_address_hbox_1->addWidget(insert_password_3, Qt::AlignCenter);

    edit_address = new QLabel("Enter new Adress", this);
    insert_address = new QLineEdit(this);
    edit_address_hbox_2 = new QHBoxLayout();
    edit_address_hbox_2->addWidget(edit_address, Qt::AlignCenter);
    edit_address_hbox_2->addWidget(insert_address, Qt::AlignCenter);

    edit_address_confirmation = new QLabel("Enter new Address Confirmation", this);
    insert_address_confirmation = new QLineEdit(this);
    edit_address_hbox_3 = new QHBoxLayout();
    edit_address_hbox_3->addWidget(edit_address_confirmation, Qt::AlignCenter);
    edit_address_hbox_3->addWidget(insert_address_confirmation, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black;"
                                  "background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &edit_forget_main_window::confirm_edit_address_func);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func2);

    vbox_6 = new QVBoxLayout();
    vbox_6->addLayout(edit_address_hbox_0, Qt::AlignCenter);
    vbox_6->addLayout(edit_address_hbox_1, Qt::AlignCenter);
    vbox_6->addLayout(edit_address_hbox_2, Qt::AlignCenter);
    vbox_6->addLayout(edit_address_hbox_3, Qt::AlignCenter);
    vbox_6->addWidget(confirm_button, Qt::AlignCenter);
    vbox_6->addWidget(back_button, Qt::AlignCenter);
    vbox_6->setAlignment(Qt::AlignCenter);

    edit_address_widget->setLayout(vbox_6);
    /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_phone_number_widget = new QWidget();

    account_number_4 = new QLabel("Enter Account Number", this);
    insert_account_number_4 = new QLineEdit(this);
    edit_phone_number_hbox_0 = new QHBoxLayout();
    edit_phone_number_hbox_0->addWidget(account_number_4, Qt::AlignCenter);
    edit_phone_number_hbox_0->addWidget(insert_account_number_4, Qt::AlignCenter);

    password_4 = new QLabel("Enter Password", this);
    insert_password_4 = new QLineEdit(this);
    insert_password_4->setEchoMode(QLineEdit::Password);
    edit_phone_number_hbox_1 = new QHBoxLayout();
    edit_phone_number_hbox_1->addWidget(password_4, Qt::AlignCenter);
    edit_phone_number_hbox_1->addWidget(insert_password_4, Qt::AlignCenter);

    edit_phone_number = new QLabel("Enter new Phone Number", this);
    insert_phone_number = new QLineEdit(this);
    edit_phone_number_hbox_2 = new QHBoxLayout();
    edit_phone_number_hbox_2->addWidget(edit_phone_number, Qt::AlignCenter);
    edit_phone_number_hbox_2->addWidget(insert_phone_number, Qt::AlignCenter);

    edit_phone_number_confirmation = new QLabel("Enter new Phone Number Confirmation", this);
    insert_phone_number_confirmation = new QLineEdit(this);
    edit_phone_number_hbox_3 = new QHBoxLayout();
    edit_phone_number_hbox_3->addWidget(edit_phone_number_confirmation, Qt::AlignCenter);
    edit_phone_number_hbox_3->addWidget(insert_phone_number_confirmation, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black;"
                                  "background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &edit_forget_main_window::confirm_edit_phone_number_func);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func2);

    vbox_7 = new QVBoxLayout();
    vbox_7->addLayout(edit_phone_number_hbox_0, Qt::AlignCenter);
    vbox_7->addLayout(edit_phone_number_hbox_1, Qt::AlignCenter);
    vbox_7->addLayout(edit_phone_number_hbox_2, Qt::AlignCenter);
    vbox_7->addLayout(edit_phone_number_hbox_3, Qt::AlignCenter);
    vbox_7->addWidget(confirm_button, Qt::AlignCenter);
    vbox_7->addWidget(back_button, Qt::AlignCenter);
    vbox_7->setAlignment(Qt::AlignCenter);

    edit_phone_number_widget->setLayout(vbox_7);
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
    confirm_button->setStyleSheet("color: black;"
                                  "background-color: beige;");
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

    new_password_forget = new QLabel("Enter new  Password", this);
    insert_new_password_forget = new QLineEdit(this);
    forget_hbox_2 = new QHBoxLayout();
    forget_hbox_2->addWidget(new_password_forget, Qt::AlignCenter);
    forget_hbox_2->addWidget(insert_new_password_forget, Qt::AlignCenter);

    new_password_confirmation_forget = new QLabel("Enter new Password Confirmation", this);
    insert_new_password_confirmation_forget = new QLineEdit(this);
    insert_new_password_confirmation_forget->setEchoMode(QLineEdit::Password);
    forget_hbox_3 = new QHBoxLayout();
    forget_hbox_3->addWidget(new_password_confirmation_forget, Qt::AlignCenter);
    forget_hbox_3->addWidget(insert_new_password_confirmation_forget, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black;"
                                  "background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &edit_forget_main_window::confirm_forget_func);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &edit_forget_main_window::back_button_func);

    vbox_3 = new QVBoxLayout();
    vbox_3->addLayout(forget_hbox_1, Qt::AlignCenter);
    vbox_3->addLayout(forget_hbox_2, Qt::AlignCenter);
    vbox_3->addLayout(forget_hbox_3, Qt::AlignCenter);
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

void edit_forget_main_window::back_button_func2()
{
    int current_index = window_stack->currentIndex();

    if (current_index)
        window_stack->setCurrentIndex(1);
}

void edit_forget_main_window::confirm_change_func()
{
    int account_number = insert_account_number_change->text().toInt();
    std::string password = insert_password_change->text().toStdString();

    std::string new_password = insert_new_password_change->text().toStdString();
    std::string new_password_confirmation = insert_new_password_confirmation_change->text().toStdString();

    std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);

    if (hashed_password == "")
    {
        insert_account_number_change->setStyleSheet("border: 1px solid red");

        return;
    }

    insert_account_number_change->setStyleSheet("border: 1px solid gray");

    if (!BANK::verifying_password(password, hashed_password))
    {
        insert_password_change->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(nullptr, "!!!", "Password Incorrect");

        return;
    }

    insert_password_change->setStyleSheet("border: 1px solid gray");

    if (new_password.compare(new_password_confirmation))
    {
        insert_new_password_confirmation_change->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "Confirmation Incorrect", "New password Confirmation Incorrect");

        return;
    }

    insert_new_password_confirmation_change->setStyleSheet("border: 1px solid gray");

    std::string new_hash_password = BANK::hashing_password(new_password);

    call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

    QMessageBox::information(this, "OK", "Account Password Changed Successfully");

    insert_account_number_change->clear();
    insert_password_change->clear();
    insert_new_password_change->clear();
    insert_new_password_confirmation_change->clear();

    password.clear();
    new_password.clear();
    new_password_confirmation.clear();
    hashed_password.clear();
}

void edit_forget_main_window::confirm_forget_func()
{
    int account_number = insert_account_number_forget->text().toInt();

    std::string new_password = insert_new_password_forget->text().toStdString();
    std::string new_password_confirmation = insert_new_password_confirmation_forget->text().toStdString();

    std::string question;
    std::string answer;
    if (!BANK::Qt_authentification_check(connection, account_number, question, answer))
    {
        QMessageBox::warning(this, "Information Incorrect", "The provided Answer is Incorrect");

        return;
    }

    if (new_password.compare(new_password_confirmation))
    {
        insert_new_password_confirmation_forget->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "Confirmation Incorrect", "New password Confirmation Incorrect");

        return;
    }

    insert_new_password_confirmation_forget->setStyleSheet("border: 1px solid gray");

    std::string new_hash_password = BANK::hashing_password(new_password);

    call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

    QMessageBox::information(this, "OK", "Account Password Changed Successfully");

    insert_account_number_forget->clear();
    insert_new_password_forget->clear();
    insert_new_password_confirmation_forget->clear();

    new_password.clear();
    new_password_confirmation.clear();
    new_hash_password.clear();
}

void edit_forget_main_window::confirm_edit_name_func()
{
    int account_number = insert_account_number_1->text().toInt();
    std::string new_first_name = insert_name->text().toStdString();
    std::string new_first_name_confirmation = insert_name_confirmation->text().toStdString();
    std::string password = insert_password_1->text().toStdString();

    std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);
    if (hashed_password == "")
    {
        insert_account_number_1->setStyleSheet("border: 1px solid red");

        return;
    }

    insert_account_number_1->setStyleSheet("border: 1px solid gray");

    if (!BANK::verifying_password(password, hashed_password))
    {
        insert_password_1->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(nullptr, "!!!", "Password Incorrect");

        return;
    }

    insert_password_1->setStyleSheet("border: 1px solid gray");

    if (new_first_name.empty())
    {
        insert_name->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "void", "Name left Blank, Enter a valid One");

        return;
    }

    insert_name->setStyleSheet("border: 1px solid gray");

    if (new_first_name.compare(new_first_name_confirmation))
    {
        insert_name_confirmation->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "Confirmation Incorrect", "New Name Confirmation Incorrect");

        return;
    }

    insert_name_confirmation->setStyleSheet("border: 1px solid gray");

    std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_name(?,?);"));
    prep_statement->setInt(1, account_number);
    prep_statement->setString(2, new_first_name);

    prep_statement->executeUpdate();

    QMessageBox::information(this, "OK", "Name Changed Successfully");

    insert_account_number_1->clear();
    insert_name->clear();
    insert_name_confirmation->clear();
    insert_password_1->clear();

    new_first_name.clear();
    new_first_name_confirmation.clear();
    password.clear();
    hashed_password.clear();
}

void edit_forget_main_window::confirm_edit_email_func()
{
    int account_number = insert_account_number_2->text().toInt();
    std::string new_email = insert_email->text().toStdString();
    std::string new_email_confirmation = insert_email_confirmation->text().toStdString();
    std::string password = insert_password_2->text().toStdString();

    std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);
    if (hashed_password == "")
    {
        insert_account_number_2->setStyleSheet("border: 1px solid red");

        return;
    }

    insert_account_number_2->setStyleSheet("border: 1px solid gray");

    if (!BANK::verifying_password(password, hashed_password))
    {
        insert_password_2->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(nullptr, "!!!", "Password Incorrect");

        return;
    }

    insert_password_2->setStyleSheet("border: 1px solid gray");

    if (new_email.empty())
    {
        insert_email->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "void", "Email left Blank, Enter a valid One");

        return;
    }

    insert_email->setStyleSheet("border: 1px solid gray");

    if (new_email.compare(new_email_confirmation))
    {
        insert_email_confirmation->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "Confirmation Incorrect", "New Email Confirmation Incorrect");

        return;
    }

    insert_email_confirmation->setStyleSheet("border: 1px solid gray");

    std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_email(?,?);"));
    prep_statement->setInt(1, account_number);
    prep_statement->setString(2, new_email);

    prep_statement->executeUpdate();

    QMessageBox::information(this, "OK", "Email Changed Successfully");
}

void edit_forget_main_window::confirm_edit_address_func()
{
    int account_number = insert_account_number_3->text().toInt();
    std::string new_address = insert_address->text().toStdString();
    std::string new_address_confirmation = insert_address_confirmation->text().toStdString();
    std::string password = insert_password_3->text().toStdString();

    std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);
    if (hashed_password == "")
    {
        insert_account_number_3->setStyleSheet("border: 1px solid red");

        return;
    }

    insert_account_number_3->setStyleSheet("border: 1px solid gray");

    if (!BANK::verifying_password(password, hashed_password))
    {
        insert_password_3->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(nullptr, "!!!", "Password Incorrect");

        return;
    }

    insert_password_3->setStyleSheet("border: 1px solid gray");

    if (new_address.empty())
    {
        insert_address->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "void", "Address left Blank, Enter a valid One");

        return;
    }

    insert_address->setStyleSheet("border: 1px solid gray");

    if (new_address.compare(new_address_confirmation))
    {
        insert_address_confirmation->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "Confirmation Incorrect", "New Address Confirmation Incorrect");

        return;
    }

    insert_address_confirmation->setStyleSheet("border: 1px solid gray");

    std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_address(?,?);"));
    prep_statement->setInt(1, account_number);
    prep_statement->setString(2, new_address);

    prep_statement->executeUpdate();

    QMessageBox::information(this, "OK", "Address Changed Successfully");
}

void edit_forget_main_window::confirm_edit_phone_number_func()
{
    int account_number = insert_account_number_4->text().toInt();
    std::string new_phone_number = insert_phone_number->text().toStdString();
    std::string new_phone_number_confirmation = insert_phone_number_confirmation->text().toStdString();
    std::string password = insert_password_4->text().toStdString();

    std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);
    if (hashed_password == "")
    {
        insert_account_number_4->setStyleSheet("border: 1px solid red");

        return;
    }

    insert_account_number_4->setStyleSheet("border: 1px solid gray");

    if (!BANK::verifying_password(password, hashed_password))
    {
        insert_password_4->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(nullptr, "!!!", "Password Incorrect");

        return;
    }

    insert_password_4->setStyleSheet("border: 1px solid gray");

    if (new_phone_number.empty())
    {
        insert_phone_number->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "void", "Phone Number left Blank, Enter a valid One");

        return;
    }

    insert_phone_number->setStyleSheet("border: 1px solid gray");

    if (new_phone_number.compare(new_phone_number_confirmation))
    {
        insert_phone_number_confirmation->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(this, "Confirmation Incorrect", "New Phone Number Confirmation Incorrect");

        return;
    }

    insert_phone_number_confirmation->setStyleSheet("border: 1px solid gray");

    std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_phone_number(?,?);"));
    prep_statement->setInt(1, account_number);
    prep_statement->setString(2, new_phone_number);

    prep_statement->executeUpdate();

    QMessageBox::information(this, "OK", "Phone Number Changed Successfully");

    insert_account_number_4->clear();
    insert_phone_number->clear();
    insert_phone_number_confirmation->clear();
    insert_password_4->clear();

    new_phone_number.clear();
    new_phone_number_confirmation.clear();
    password.clear();
    hashed_password.clear();
}