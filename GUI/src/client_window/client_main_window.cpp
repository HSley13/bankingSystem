#include "client_main_window.h"
#include "client_database.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <argon2.h>
#include <iostream>
#include <QWidget>
#include <QMainWindow>

client_main_window ::client_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Administrator");
    resize(600, 600);

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);
}