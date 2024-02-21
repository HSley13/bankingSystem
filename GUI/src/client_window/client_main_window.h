#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTextEdit>
#include <QDateEdit>
#include <QString>

class client_main_window : public QMainWindow
{
    Q_OBJECT

public:
    client_main_window(const std::string &db_password, QWidget *parent = nullptr);

private:
    std::string database_password;

    QStackedWidget *window_stack;

    QWidget *central_widget;
    QWidget *create_account_widget;
    QWidget *bank_info_widget;

    QPushButton *create_account;
    QPushButton *account_inquiry;
    QPushButton *bank_info;
    QPushButton *confirm_button;
    QPushButton *back_button;

    QLabel *message;
    QLabel *national_ID;
    QLabel *first_name;
    QLabel *last_name;
    QLabel *date_birth;
    QLabel *phone_number;
    QLabel *email;
    QLabel *address;
    QLabel *password;
    QLabel *password_confirmation;
    QLabel *question;
    QLabel *answer;
    QLabel *confirm_answer;

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

    QHBoxLayout *hbox1;
    QHBoxLayout *hbox2;
    QHBoxLayout *hbox3;
    QHBoxLayout *hbox4;
    QHBoxLayout *hbox5;
    QHBoxLayout *hbox6;
    QHBoxLayout *hbox7;
    QHBoxLayout *hbox8;
    QHBoxLayout *hbox9;
    QHBoxLayout *hbox10;
    QHBoxLayout *hbox11;
    QHBoxLayout *hbox12;
    QHBoxLayout *hbox13;

    QVBoxLayout *vbox;

    QDateEdit *calendar;
    QDate selected_date;

private slots:
    void back_button_func();
    void confirm_button_func();
    void account_inquiry_func();
};
