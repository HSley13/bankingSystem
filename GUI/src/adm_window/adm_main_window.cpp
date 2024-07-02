#include "adm_main_window.h"
#include <adm_option_main_window.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

adm_main_window::adm_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack, QWidget *parent)
    : QMainWindow(parent), connection(db_connection), _previous_stack(previous_stack)
{
    setWindowTitle("Administrator");
    setStyleSheet("font-family: Arial Black;"
                  "font-size: 20;"
                  "font: bold italic 14px;"
                  "background-color: black;");

    resize(200, 200);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    QWidget *adm_widget = new QWidget(this);

    insert_adm_account_number = new QLineEdit(this);

    insert_adm_password = new QLineEdit(this);
    insert_adm_password->setEchoMode(QLineEdit::Password);

    QPushButton *confirm_login = new QPushButton("Confirm", this);
    confirm_login->setStyleSheet("color: black;"
                                 "background-color: beige;");
    connect(confirm_login, &QPushButton::clicked, this, &adm_main_window::confirm_login_func);

    QLabel *image_label4 = new QLabel(this);
    QPixmap image4("/Users/test/Documents/banking_system/GUI/src/ressources/adm.jpeg");
    image_label4->setPixmap(image4.scaled(80, 80, Qt::KeepAspectRatio));
    image_label4->setScaledContents(true);

    QFormLayout *adm_layout = new QFormLayout();
    adm_layout->addRow("Enter ADM Account Number", insert_adm_account_number);
    adm_layout->addRow("Enter ADM Password: ", insert_adm_password);
    adm_layout->addWidget(confirm_login);

    QPushButton *back_button = new QPushButton("Previous Menu", this);
    back_button->setStyleSheet("color: beige;"
                               "font-family: Arial Black;"
                               "font-size: 20;"
                               "font: bold italic 14px;"
                               "background-color: black;");
    connect(back_button, &QPushButton::clicked, this, [=]()
            { _previous_stack->setCurrentIndex(0); });

    QHBoxLayout *hbox = new QHBoxLayout(adm_widget);
    hbox->addWidget(image_label4);
    hbox->addLayout(adm_layout);
    hbox->addWidget(back_button);

    stack->addWidget(adm_widget);
}

void adm_main_window::confirm_login_func()
{
    int account_number = insert_adm_account_number->text().toInt();

    std::string password = insert_adm_password->text().toStdString();

    std::string hashed_password = BANK::Qt_retrieve_adm_hashed_password(connection, account_number);

    if (hashed_password.empty())
    {
        insert_adm_account_number->setStyleSheet("border: 1px solid red");

        return;
    }

    insert_adm_account_number->setStyleSheet("border: 1px solid gray");

    if (!BANK::verifying_password(password, hashed_password))
    {
        insert_adm_password->setStyleSheet("border: 1px solid red");

        QMessageBox::warning(nullptr, "!!!", "Password Incorrect");

        return;
    }

    insert_adm_password->setStyleSheet("border: 1px solid gray");

    QMessageBox::information(this, "Redirecting...", "You are about to be redirected to the Administrator's Official Page");

    adm_option_main_window *new_window = new adm_option_main_window(connection, stack, this);

    stack->addWidget(new_window);

    stack->setCurrentWidget(new_window);
}