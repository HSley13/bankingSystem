#include "option_main_window.h"
#include <database.h>
#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTableWidgetItem>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

option_main_window::option_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Client Inquiry Window");
    resize(600, 600);

    central_widget = new QWidget(this);
    setCentralWidget(central_widget);
}