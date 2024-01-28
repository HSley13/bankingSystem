#include "adm_main_window.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

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

    vbox = new QVBoxLayout(central_widget);
    vbox->setAlignment(Qt::AlignCenter);

    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addWidget(button);
}

// void adm_main_window::confirmed_login()
// {

// }