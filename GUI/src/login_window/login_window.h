#pragma once

#include <database.h>

class login_window : public QMainWindow
{
    Q_OBJECT

public:
    login_window(sql::Connection *db_connection, QStackedWidget *previous_stack = nullptr, QString choice = QString(), QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *_stack;

    QString _choice = QString();

    QStackedWidget *_previous_stack;

    QLineEdit *_account_number;
    QLineEdit *_password;

private slots:
    void confirm_login_func();
};
