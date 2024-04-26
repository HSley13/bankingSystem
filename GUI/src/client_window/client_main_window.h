#pragma once

#include <database.h>
#include <QtCore>
#include <QtWidgets>

class client_main_window : public QMainWindow
{
    Q_OBJECT

public:
    client_main_window(sql::Connection *db_connection, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *window_stack;

    QPushButton *confirm_button;
    QPushButton *back_button;

    QLineEdit *insert_national_ID;
    QLineEdit *insert_first_name;
    QLineEdit *insert_last_name;
    QLineEdit *insert_phone_number;
    QLineEdit *insert_email;
    QLineEdit *insert_address;
    QLineEdit *insert_balance;
    QLineEdit *insert_password;
    QLineEdit *insert_password_confirmation;
    QLineEdit *insert_question;
    QLineEdit *insert_answer;
    QLineEdit *insert_confirm_answer;

    QVBoxLayout *vbox;

    QDateEdit *calendar;
    QDate selected_date;

private slots:
    void back_button_func();
    void confirm_button_func();
    void account_inquiry_func();
};
