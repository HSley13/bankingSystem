#pragma once

#include <database.h>
class option_main_window : public QMainWindow
{
    Q_OBJECT

public:
    option_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack = nullptr, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *_stack;
    QStackedWidget *_previous_stack;

    QPushButton *_back_button;
    QPushButton *_confirm_button;

    QLineEdit *_account_number_ba;
    QLineEdit *_password_ba;

    QLineEdit *_account_number_de;
    QLineEdit *_password_de;
    QLineEdit *_amount_de;

    QLineEdit *_account_number_with;
    QLineEdit *_password_with;
    QLineEdit *_amount_with;

    QLineEdit *_account_number1_tran;
    QLineEdit *_account_number2_tran;
    QLineEdit *_password_tran;
    QLineEdit *_amount_tran;

    QLineEdit *_account_number_borr;
    QLineEdit *_password_borr;
    QLineEdit *_amount_borr;

    QLineEdit *_account_number_ret;
    QLineEdit *_password_ret;
    QLineEdit *_amount_ret;

    QLineEdit *_account_number_transac;
    QLineEdit *_password_transac;

    QLineEdit *_specific_account_number_transac;
    QLineEdit *_specific_password_transac;

    QLineEdit *_account_number_edit;
    QLineEdit *_password_edit;

    QLineEdit *_account_number_dele;
    QLineEdit *_password_dele;

    QComboBox *_choice;
    QDateEdit *_calendar;
    QDate _selected_date;

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