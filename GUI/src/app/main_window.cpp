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
#include <QFont>
#include <QPixmap>
#include <QPalette>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("CROSS CONTINENTAL TREASURY BANK");
    resize(600, 600);

    setStyleSheet("font-family: Herculanum; font-size: 20; font: bold italic 14px;");

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QLabel *image_label = new QLabel(this);
    QPixmap image(":/images/bank.png");
    if (image.isNull())
    {
        qDebug() << "Failed to load image.";
    }

    image_label->setPixmap(image);
    image_label->setScaledContents(true);

    name = new QLabel("CROSS-CONTINENTAL TREASUTY BANK", this);
    name->setStyleSheet("font-family: Zapfino; font-size: 20; font: bold italic 20px;");

    welcome = new QLabel("Welcome to our Bank, Choose among the Options below what best suits your Status", this);

    admin = new QPushButton("1. Administrator", this);
    connect(admin, &QPushButton::clicked, this, &main_window::adm_button_clicked);

    client = new QPushButton("2. Regular Client", this);
    connect(client, &QPushButton::clicked, this, &main_window::client_button_clicked);

    vbox = new QVBoxLayout(central_widget);
    vbox->addWidget(name, 2, Qt ::AlignCenter);
    vbox->addWidget(image_label, 2, Qt ::AlignCenter);
    vbox->addWidget(welcome, 2, Qt ::AlignCenter);
    vbox->addWidget(admin, Qt ::AlignLeft);
    vbox->addWidget(client, Qt ::AlignLeft);
}

void main_window::client_button_clicked()
{
    client_main_window *new_window = new client_main_window;

    new_window->show();
}

void main_window::adm_button_clicked()
{
    adm_main_window *new_window = new adm_main_window;

    new_window->show();
}
