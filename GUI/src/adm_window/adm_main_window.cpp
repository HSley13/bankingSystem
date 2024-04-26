#include "adm_main_window.h"
#include <adm_option_main_window.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

adm_main_window::adm_main_window(sql::Connection *db_connection, QWidget *parent)
    : QMainWindow(parent), connection(db_connection)
{
    setWindowTitle("Administrator");
    setStyleSheet("font-family: Arial Black;"
                  "font-size: 20;"
                  "font: bold italic 14px;"
                  "background-color: black;");

    resize(600, 300);

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QLabel *adm_account_number = new QLabel("Enter ADM Account Number to log in?", this);
    insert_adm_account_number = new QLineEdit(this);

    QHBoxLayout *hbox1 = new QHBoxLayout();
    hbox1->addWidget(adm_account_number, Qt::AlignCenter);
    hbox1->addWidget(insert_adm_account_number, Qt::AlignCenter);

    QLabel *adm_password = new QLabel("Enter ADM Password: ");
    insert_adm_password = new QLineEdit(this);
    insert_adm_password->setEchoMode(QLineEdit::Password);

    QHBoxLayout *hbox2 = new QHBoxLayout();
    hbox2->addWidget(adm_password, Qt::AlignCenter);
    hbox2->addWidget(insert_adm_password, Qt::AlignCenter);

    QPushButton *confirm_login = new QPushButton("Confirm", this);
    confirm_login->setStyleSheet("color: black;"
                                 "background-color: beige;");
    connect(confirm_login, &QPushButton::clicked, this, &adm_main_window::confirm_login_func);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addWidget(confirm_login);

    QGroupBox *box = new QGroupBox();
    box->setLayout(vbox);
    box->setFixedSize(500, 300);

    QLabel *image_label = new QLabel(this);
    QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/adm.jpeg");
    image_label->setPixmap(image.scaled(300, 300, Qt::KeepAspectRatio));
    image_label->setScaledContents(true);

    QHBoxLayout *hbox = new QHBoxLayout(central_widget);
    hbox->addWidget(image_label);
    hbox->addWidget(box);
}

void adm_main_window::confirm_login_func()
{
    int account_number = insert_adm_account_number->text().toInt();

    std::string password = insert_adm_password->text().toStdString();

    std::string hashed_password = BANK::Qt_retrieve_adm_hashed_password(connection, account_number);

    if (hashed_password == "")
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
    hide();

    adm_option_main_window *new_window = new adm_option_main_window(connection);

    new_window->show();

    insert_adm_account_number->clear();
    insert_adm_password->clear();

    hashed_password.clear();
    password.clear();
}