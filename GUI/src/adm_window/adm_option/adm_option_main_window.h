#pragma once

#include <database.h>
#include <QtCore>
#include <QtWidgets>

class adm_option_main_window : public QMainWindow
{
    Q_OBJECT

public:
    adm_option_main_window(sql::Connection *db_connection, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QLineEdit *new_adm_account_number;
    QLineEdit *new_adm_password;
    QLineEdit *new_adm_password_confirmation;

    QLineEdit *wid_3_account_number;
    QLineEdit *wid_5_account_number;
    QLineEdit *wid_6_account_number;
    QLineEdit *wid_7_account_number;

    QLineEdit *specific_account_number_transac;

    QPushButton *back_button;
    QPushButton *confirm_button;

    QStackedWidget *window_stack;

    QComboBox *choice;
    QDateEdit *calendar;
    QDate selected_date;

private slots:
    void
    back_button_func();
    void wid_1_config();
    void display_accounts_table();
    void display_specific_accounts_table();
    void display_people_in_debt();
    void display_specific_accounts_in_debt();
    void display_transactions_history();
    void display_relative_transactions_history();
    void delete_accounts();
};