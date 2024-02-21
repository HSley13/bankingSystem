#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTableWidgetItem>
#include <QDateEdit>
#include <QDate>
#include <QComboBox>

class option_main_window : public QMainWindow
{
    Q_OBJECT

public:
    option_main_window(const std::string &db_password, QWidget *parent = nullptr);

private:
    std::string database_password;

    QStackedWidget *window_stack;

    QWidget *central_widget;
    QWidget *balance_widget;
    QWidget *deposit_widget;
    QWidget *withdrawal_widget;
    QWidget *transfer_widget;
    QWidget *borrowal_widget;
    QWidget *return_borrowal_widget;
    QWidget *edit_and_forget_widget;
    QWidget *transaction_history_widget;
    QWidget *specific_transaction_history_widget;
    QWidget *delete_account_widget;

    QPushButton *balance;
    QPushButton *deposit;
    QPushButton *withdrawal;
    QPushButton *transfer;
    QPushButton *borrowal;
    QPushButton *return_borrowal;
    QPushButton *edit_and_forget;
    QPushButton *transaction_history;
    QPushButton *specific_transaction_history;
    QPushButton *delete_account;
    QPushButton *back_button;

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

    QPushButton *confirm_button;

    QLabel *balance_message1;
    QLabel *balance_message2;

    QLabel *deposit_message1;
    QLabel *deposit_message2;
    QLabel *deposit_message3;

    QLabel *withdrawal_message1;
    QLabel *withdrawal_message2;
    QLabel *withdrawal_message3;

    QLabel *transfer_message1;
    QLabel *transfer_message2;
    QLabel *transfer_message3;
    QLabel *transfer_message4;

    QLabel *borrowal_message1;
    QLabel *borrowal_message2;
    QLabel *borrowal_message3;

    QLabel *return_borrowal_message1;
    QLabel *return_borrowal_message2;
    QLabel *return_borrowal_message3;

    QLabel *edit_and_forget_message1;
    QLabel *edit_and_forget_message2;

    QLabel *transaction_history_message1;
    QLabel *transaction_history_message2;

    QLabel *specific_transaction_history_message1;
    QLabel *specific_transaction_history_message2;

    QLabel *delete_account_message1;
    QLabel *delete_account_message2;

    QVBoxLayout *VBOX;
    QVBoxLayout *vbox1;
    QVBoxLayout *vbox2;
    QVBoxLayout *vbox3;
    QVBoxLayout *vbox4;
    QVBoxLayout *vbox5;
    QVBoxLayout *vbox6;
    QVBoxLayout *vbox7;
    QVBoxLayout *vbox8;
    QVBoxLayout *vbox9;
    QVBoxLayout *vbox10;

    QHBoxLayout *ba_hbox1;
    QHBoxLayout *ba_hbox2;

    QHBoxLayout *de_hbox1;
    QHBoxLayout *de_hbox2;
    QHBoxLayout *de_hbox3;

    QHBoxLayout *with_hbox1;
    QHBoxLayout *with_hbox2;
    QHBoxLayout *with_hbox3;

    QHBoxLayout *tran_hbox1;
    QHBoxLayout *tran_hbox2;
    QHBoxLayout *tran_hbox3;
    QHBoxLayout *tran_hbox4;

    QHBoxLayout *borr_hbox1;
    QHBoxLayout *borr_hbox2;
    QHBoxLayout *borr_hbox3;

    QHBoxLayout *ret_hbox1;
    QHBoxLayout *ret_hbox2;
    QHBoxLayout *ret_hbox3;

    QHBoxLayout *edit_hbox1;
    QHBoxLayout *edit_hbox2;

    QHBoxLayout *transac_hbox1;
    QHBoxLayout *transac_hbox2;

    QHBoxLayout *specific_transac_hbox1;
    QHBoxLayout *specific_transac_hbox2;

    QHBoxLayout *dele_hbox1;
    QHBoxLayout *dele_hbox2;

    QComboBox *choice;
    QDateEdit *calendar;
    QDate selected_date;

private slots:
    void
    back_button_func();
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