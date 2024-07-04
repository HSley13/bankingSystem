#pragma once

#include <database.h>
class adm_main_window : public QMainWindow
{
    Q_OBJECT

public:
    adm_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack = nullptr, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QLineEdit *_new_adm_account_number;
    QLineEdit *_new_adm_password;
    QLineEdit *_new_adm_password_confirmation;

    QLineEdit *wid_3_account_number;
    QLineEdit *wid_5_account_number;
    QLineEdit *wid_6_account_number;
    QLineEdit *wid_7_account_number;

    QLineEdit *_specific_account_number_transac;

    QPushButton *_back_button;
    QPushButton *_confirm_button;

    QStackedWidget *_stack;
    QStackedWidget *_previous_stack;

    QComboBox *_choice;
    QDateEdit *_calendar;
    QDate _selected_date;

private slots:
    void back_button_func();
    void wid_1_config();
    void display_accounts_table();
    void display_specific_accounts_table();
    void display_people_in_debt();
    void display_specific_accounts_in_debt();
    void display_transactions_history();
    void display_relative_transactions_history();
    void delete_accounts();
};