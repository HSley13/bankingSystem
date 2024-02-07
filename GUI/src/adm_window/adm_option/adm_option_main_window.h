#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>

class adm_option_main_window : public QMainWindow
{
    Q_OBJECT

public:
    adm_option_main_window(QWidget *parent = nullptr);

private:
    QLineEdit *new_adm_account_number;
    QLineEdit *new_adm_password;
    QLineEdit *new_adm_password_confirmation;

    QLineEdit *wid_3_account_number;
    QLineEdit *wid_5_account_number;
    QLineEdit *wid_6_account_number;
    QLineEdit *wid_7_account_number;

    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;
    QPushButton *button7;

    QPushButton *back_button1;
    QPushButton *back_button2;
    QPushButton *back_button3;
    QPushButton *back_button4;
    QPushButton *back_button5;
    QPushButton *back_button6;
    QPushButton *back_button7;

    QWidget *central_widget;
    QWidget *wid_1;
    QWidget *wid_2;
    QWidget *wid_3;
    QWidget *wid_4;
    QWidget *wid_5;
    QWidget *wid_6;
    QWidget *wid_7;

    QVBoxLayout *vbox;

    QStackedWidget *window_stack;

    QLabel *image_label;

private slots:
    void back_button_func();
    void wid_1_config();
    void display_accounts_table();
    void display_specific_accounts_table();
    void display_people_in_debt();
    void display_specific_accounts_in_debt();
    void display_transactions_history();
    void delete_accounts();
};