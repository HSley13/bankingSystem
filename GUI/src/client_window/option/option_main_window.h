#pragma once

#include <database.h>
#include <QtCore>
#include <QtWidgets>

class option_main_window : public QMainWindow
{
    Q_OBJECT

public:
    option_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack = nullptr, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *stack;
    QStackedWidget *_previous_stack;

    QPushButton *back_button;
    QPushButton *confirm_button;

    QLineEdit *account_number_ba;
    QLineEdit *password_ba;

    QLineEdit *account_number_de;
    QLineEdit *password_de;
    QLineEdit *amount_de;

    QLineEdit *account_number_with;
    QLineEdit *password_with;
    QLineEdit *amount_with;

    QLineEdit *account_number1_tran;
    QLineEdit *account_number2_tran;
    QLineEdit *password_tran;
    QLineEdit *amount_tran;

    QLineEdit *account_number_borr;
    QLineEdit *password_borr;
    QLineEdit *amount_borr;

    QLineEdit *account_number_ret;
    QLineEdit *password_ret;
    QLineEdit *amount_ret;

    QLineEdit *account_number_transac;
    QLineEdit *password_transac;

    QLineEdit *specific_account_number_transac;
    QLineEdit *specific_password_transac;

    QLineEdit *account_number_edit;
    QLineEdit *password_edit;

    QLineEdit *account_number_dele;
    QLineEdit *password_dele;

    QComboBox *choice;
    QDateEdit *calendar;
    QDate selected_date;

private slots:
    void back_button_func();
    void confirm_button_balance();
    void confirm_button_deposit();
    void confirm_button_withdrawal();
    void confirm_button_transfer();
    void confirm_button_borrowal();
    void confirm_button_return_borrowal();
    void confirm_button_edit_perso();
    void confirm_button_transaction_history();
    void confirm_button_specific_transaction_history();
    void confirm_button_delete_account();
};