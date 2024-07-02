#pragma once

#include <database.h>
#include <QtCore>
#include <QtWidgets>

class adm_main_window : public QMainWindow
{
    Q_OBJECT

public:
    adm_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack = nullptr, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *stack;

    QStackedWidget *_previous_stack;

    QLineEdit *insert_adm_account_number;
    QLineEdit *insert_adm_password;

private slots:
    void confirm_login_func();
};
