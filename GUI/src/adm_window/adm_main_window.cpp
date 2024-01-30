#include "adm_main_window.h"
#include <adm_option_main_window.h>
#include <database.h>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

adm_main_window ::adm_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Administrator");
    resize(600, 600);

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    adm_account_number = new QLabel("Enter ADM Account Number to log in?", this);
    insert_adm_account_number = new QLineEdit(this);

    hbox1 = new QHBoxLayout();
    hbox1->addWidget(adm_account_number);
    hbox1->addWidget(insert_adm_account_number);

    adm_password = new QLabel("Enter ADM Password?");
    insert_adm_password = new QLineEdit(this);
    insert_adm_password->setEchoMode(QLineEdit::Password);

    hbox2 = new QHBoxLayout();
    hbox2->addWidget(adm_password);
    hbox2->addWidget(insert_adm_password);

    button = new QPushButton("Confirm", this);
    connect(button, &QPushButton::clicked, this, &adm_main_window::confirm_login);

    vbox = new QVBoxLayout(central_widget);
    vbox->setAlignment(Qt::AlignCenter);

    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addWidget(button);
}

void adm_main_window::confirm_login()
{
    int account_number = insert_adm_account_number->text().toInt();

    std ::string password = insert_adm_password->text().toStdString();

    connection_details ID;
    ID.server = "localhost";
    ID.user = "root";
    ID.password = "sleyHortes1312";

    sql ::Connection *connection = connection_setup(&ID);

    std ::string hashed_password = BANK ::retrieve_adm_hashed_password(connection, account_number);

    if (BANK ::verifying_password(password, hashed_password))
    {
        QMessageBox *message1 = new QMessageBox(this);
        message1->information(this, "Information", "Login succeed");
        hide();

        adm_option_main_window *new_window = new adm_option_main_window;

        new_window->show();
    }
    else
    {
        QString message1 = "Login Failed! The Account number: " + QString::number(account_number) + " is not Found, correct it and try again";

        QMessageBox *message = new QMessageBox(this);
        message->warning(this, "Warning!", message1);
    }
}