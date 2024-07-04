#pragma once

#include <database.h>
class client_main_window : public QMainWindow
{
    Q_OBJECT

public:
    client_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack = nullptr, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *_stack;

    QStackedWidget *_previous_stack;

    QPushButton *_confirm_button;
    QPushButton *_back_button;

    QLineEdit *_national_ID;
    QLineEdit *_first_name;
    QLineEdit *_last_name;
    QLineEdit *_phone_number;
    QLineEdit *_email;
    QLineEdit *_address;
    QLineEdit *_balance;
    QLineEdit *_password;
    QLineEdit *_password_confirmation;
    QLineEdit *_question;
    QLineEdit *_answer;
    QLineEdit *_confirm_answer;

    QVBoxLayout *_VBOX;

    QDateEdit *_calendar;
    QDate _selected_date;

private slots:
    void back_button_func();
    void confirm_button_func();
    void account_inquiry_func();
};
