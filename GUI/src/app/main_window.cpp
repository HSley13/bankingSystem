#include "main_window.h"
#include <adm_main_window.h>
#include <client_main_window.h>
#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>

main_window::main_window(const std::string &db_password, QWidget *parent)
    : QMainWindow(parent), database_password(db_password)
{
    setWindowTitle("CROSS CONTINENTAL TREASURY BANK");
    resize(600, 600);

    setStyleSheet("font-family: Herculanum;"
                  "font: bold italic 14px;"
                  "font-size: 40;"
                  "background-color: black;");

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    name = new QLabel("CROSS-CONTINENTAL TREASUTY BANK", this);
    name->setStyleSheet("font-family: Zapfino;"
                        "font: bold italic 20px;"
                        "font-size: 20;");

    welcome = new QLabel("Welcome to our Bank, Choose among the Options below what best suits your Status", this);

    admin = new QPushButton("1. Administrator", this);
    admin->setStyleSheet("color: black;"
                         "background-color: beige;");
    connect(admin, &QPushButton::clicked, this, &main_window::adm_button_clicked);

    client = new QPushButton("2. Regular Client", this);
    client->setStyleSheet("color: black;"
                          "background-color: beige;");
    connect(client, &QPushButton::clicked, this, &main_window::client_button_clicked);

    QLabel *image_label = new QLabel(this);
    QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/globe_transfer.gif");
    image_label->setPixmap(image.scaled(500, 500, Qt::KeepAspectRatio));
    image_label->setScaledContents(true);

    vbox = new QVBoxLayout(central_widget);
    vbox->addWidget(name, 2, Qt ::AlignCenter);
    vbox->addWidget(image_label, 2, Qt ::AlignCenter);
    vbox->addWidget(welcome, 2, Qt ::AlignCenter);
    vbox->addWidget(admin, Qt ::AlignLeft);
    vbox->addWidget(client, Qt ::AlignLeft);
}

void main_window::client_button_clicked()
{
    client_main_window *new_window = new client_main_window(database_password);

    new_window->show();
}

void main_window::adm_button_clicked()
{
    adm_main_window *new_window = new adm_main_window(database_password);

    new_window->show();
}
