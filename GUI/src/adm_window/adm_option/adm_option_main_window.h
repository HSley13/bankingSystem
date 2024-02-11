#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QDateEdit>
#include <QComboBox>

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
    QLineEdit *wid_8_account_number;

    QLineEdit *specific_account_number_transac;

    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;
    QPushButton *button7;
    QPushButton *button8;

    QPushButton *back_button;
    QPushButton *confirm_button;

    QStackedWidget *window_stack;
    QWidget *central_widget;
    QWidget *wid_1;
    QWidget *wid_2;
    QWidget *wid_3;
    QWidget *wid_4;
    QWidget *wid_5;
    QWidget *wid_6;
    QWidget *wid_7;
    QWidget *wid_8;

    QGroupBox *box_1;
    QGroupBox *box_2;
    QGroupBox *box_3;
    QGroupBox *box_4;
    QGroupBox *box_5;
    QGroupBox *box_6;
    QGroupBox *box_7;
    QGroupBox *box_8;

    QHBoxLayout *HBOX_1;
    QHBoxLayout *HBOX_2;
    QHBoxLayout *HBOX_3;
    QHBoxLayout *HBOX_4;
    QHBoxLayout *HBOX_5;
    QHBoxLayout *HBOX_6;
    QHBoxLayout *HBOX_7;
    QHBoxLayout *HBOX_8;

    QHBoxLayout *specific_transac_hbox1;

    QVBoxLayout *vbox;

    QLabel *image_label;
    QLabel *specific_transaction_history_message1;

    QVBoxLayout *wid_1_vbox;
    QVBoxLayout *wid_2_vbox;
    QVBoxLayout *wid_3_vbox;
    QVBoxLayout *wid_4_vbox;
    QVBoxLayout *wid_5_vbox;
    QVBoxLayout *wid_6_vbox;
    QVBoxLayout *wid_7_vbox;
    QVBoxLayout *wid_8_vbox;

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