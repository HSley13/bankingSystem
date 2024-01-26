#include "mainwindow.h"
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("My Bank");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    name = new QLabel("CROSS-CONTINENTAL TREASUTY BANK", this);

    welcome = new QLabel("Welcome to our Bank, Choose among the Options below what best suits your Status", this);

    admin = new QPushButton("1. Administrator", this);
    client = new QPushButton("2. Regular Client", this);
    information = new QPushButton("Information on our Bank", this);

    vbox = new QVBoxLayout(centralWidget);
    vbox->addWidget(name, 0, Qt ::AlignCenter);
    vbox->addWidget(welcome, 0, Qt ::AlignCenter);
    vbox->addWidget(admin, Qt ::AlignLeft);
    vbox->addWidget(client, Qt ::AlignLeft);
    vbox->addWidget(information, Qt ::AlignLeft);
}
