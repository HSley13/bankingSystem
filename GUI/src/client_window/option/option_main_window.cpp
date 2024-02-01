#include "option_main_window.h"
#include <database.h>
#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTableWidgetItem>
#include <QGroupBox>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

option_main_window::option_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    window_stack = new QStackedWidget();
    setCentralWidget(window_stack);

    setWindowTitle("Client Inquiry Window");
    resize(600, 600);

    central_widget = new QWidget(this);

    balance = new QPushButton("1. Check Balance", this);
    connect(balance, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(1); });

    deposit = new QPushButton("2. Deposit", this);
    connect(deposit, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(2); });

    withdrawal = new QPushButton("3. Money Withdrawal", this);
    connect(withdrawal, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(3); });

    transfer = new QPushButton("4. Transfer Money", this);
    connect(transfer, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(4); });

    borrowal = new QPushButton("5. Borrowed Money", this);
    connect(borrowal, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(5); });

    return_borrowal = new QPushButton("6. Return Borrowed Money", this);
    connect(return_borrowal, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(6); });

    edit_and_forget = new QPushButton("7. Edit Account Information", this);
    connect(edit_and_forget, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(7); });

    transaction_history = new QPushButton("8. Transaction History", this);
    connect(transaction_history, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(8); });

    delete_account = new QPushButton("9. Delete Account", this);
    connect(delete_account, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(9); });

    VBOX = new QVBoxLayout(central_widget);
    VBOX->setAlignment(Qt::AlignCenter);
    VBOX->addWidget(balance);
    VBOX->addWidget(deposit);
    VBOX->addWidget(withdrawal);
    VBOX->addWidget(transfer);
    VBOX->addWidget(borrowal);
    VBOX->addWidget(return_borrowal);
    VBOX->addWidget(edit_and_forget);
    VBOX->addWidget(transaction_history);
    VBOX->addWidget(delete_account);
    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    balance_widget = new QWidget();
    balance_widget->setWindowTitle("Check Balance");

    balance_message1 = new QLabel("Enter Account Number", this);
    account_number_ba = new QLineEdit(this);
    ba_hbox1 = new QHBoxLayout();
    ba_hbox1->addWidget(balance_message1, Qt::AlignCenter);
    ba_hbox1->addWidget(account_number_ba, Qt::AlignCenter);

    balance_message2 = new QLabel("Enter Password", this);
    password_ba = new QLineEdit(this);
    password_ba->setEchoMode(QLineEdit::Password);
    ba_hbox2 = new QHBoxLayout();
    ba_hbox2->addWidget(balance_message2, Qt::AlignCenter);
    ba_hbox2->addWidget(password_ba, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func1);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox1 = new QVBoxLayout();
    vbox1->addLayout(ba_hbox1, Qt::AlignCenter);
    vbox1->addLayout(ba_hbox2, Qt::AlignCenter);
    vbox1->addWidget(confirm_button, Qt::AlignCenter);
    vbox1->addWidget(back_button, Qt::AlignCenter);
    vbox1->setAlignment(Qt::AlignCenter);

    balance_widget->setLayout(vbox1);
    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    deposit_widget = new QWidget();
    deposit_widget->setWindowTitle("Deposit Money");

    deposit_message1 = new QLabel("Enter Account Number", this);
    account_number_de = new QLineEdit(this);
    de_hbox1 = new QHBoxLayout();
    de_hbox1->addWidget(deposit_message1, Qt::AlignCenter);
    de_hbox1->addWidget(account_number_de, Qt::AlignCenter);

    deposit_message2 = new QLabel("Enter Password", this);
    password_de = new QLineEdit();
    password_de->setEchoMode(QLineEdit::Password);
    de_hbox2 = new QHBoxLayout();
    de_hbox2->addWidget(deposit_message2, Qt::AlignCenter);
    de_hbox2->addWidget(password_de, Qt::AlignCenter);

    deposit_message3 = new QLabel("Enter Amount to Deposit", this);
    amount_de = new QLineEdit(this);
    de_hbox3 = new QHBoxLayout();
    de_hbox3->addWidget(deposit_message3, Qt::AlignCenter);
    de_hbox3->addWidget(amount_de, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func2);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox2 = new QVBoxLayout();
    vbox2->addLayout(de_hbox1, Qt::AlignCenter);
    vbox2->addLayout(de_hbox2, Qt::AlignCenter);
    vbox2->addLayout(de_hbox3, Qt::AlignCenter);
    vbox2->addWidget(confirm_button, Qt::AlignCenter);
    vbox2->addWidget(back_button, Qt::AlignCenter);
    vbox2->setAlignment(Qt::AlignCenter);

    deposit_widget->setLayout(vbox2);
    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    withdrawal_widget = new QWidget();
    withdrawal_widget->setWindowTitle("Withdraw Money");

    withdrawal_message1 = new QLabel("Enter Account Number", this);
    account_number_with = new QLineEdit(this);
    with_hbox1 = new QHBoxLayout();
    with_hbox1->addWidget(withdrawal_message1, Qt::AlignCenter);
    with_hbox1->addWidget(account_number_with, Qt::AlignCenter);

    withdrawal_message2 = new QLabel("Enter Password", this);
    password_with = new QLineEdit();
    password_with->setEchoMode(QLineEdit::Password);
    with_hbox2 = new QHBoxLayout();
    with_hbox2->addWidget(withdrawal_message2, Qt::AlignCenter);
    with_hbox2->addWidget(password_with, Qt::AlignCenter);

    withdrawal_message3 = new QLabel("Enter Amount to Withdraw", this);
    amount_with = new QLineEdit(this);
    with_hbox3 = new QHBoxLayout();
    with_hbox3->addWidget(withdrawal_message3, Qt::AlignCenter);
    with_hbox3->addWidget(amount_with, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func3);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox3 = new QVBoxLayout();
    vbox3->addLayout(with_hbox1, Qt::AlignCenter);
    vbox3->addLayout(with_hbox2, Qt::AlignCenter);
    vbox3->addLayout(with_hbox3, Qt::AlignCenter);
    vbox3->addWidget(confirm_button, Qt::AlignCenter);
    vbox3->addWidget(back_button, Qt::AlignCenter);
    vbox3->setAlignment(Qt::AlignCenter);

    withdrawal_widget->setLayout(vbox3);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    transfer_widget = new QWidget();
    transfer_widget->setWindowTitle("Transfer Money");

    transfer_message1 = new QLabel("Enter Account Number", this);
    account_number1_tran = new QLineEdit(this);
    tran_hbox1 = new QHBoxLayout();
    tran_hbox1->addWidget(transfer_message1, Qt::AlignCenter);
    tran_hbox1->addWidget(account_number1_tran, Qt::AlignCenter);

    transfer_message2 = new QLabel("Enter Password", this);
    password_tran = new QLineEdit();
    password_tran->setEchoMode(QLineEdit::Password);
    tran_hbox2 = new QHBoxLayout();
    tran_hbox2->addWidget(transfer_message2, Qt::AlignCenter);
    tran_hbox2->addWidget(password_tran, Qt::AlignCenter);

    transfer_message3 = new QLabel("Enter Account Number which will receive the Money", this);
    account_number2_tran = new QLineEdit(this);
    tran_hbox3 = new QHBoxLayout();
    tran_hbox3->addWidget(transfer_message3, Qt::AlignCenter);
    tran_hbox3->addWidget(account_number2_tran, Qt::AlignCenter);

    transfer_message4 = new QLabel("Enter Amount to Transfer", this);
    amount_tran = new QLineEdit(this);
    tran_hbox4 = new QHBoxLayout();
    tran_hbox4->addWidget(transfer_message4, Qt::AlignCenter);
    tran_hbox4->addWidget(amount_tran, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func5);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox4 = new QVBoxLayout();
    vbox4->addLayout(tran_hbox1, Qt::AlignCenter);
    vbox4->addLayout(tran_hbox2, Qt::AlignCenter);
    vbox4->addLayout(tran_hbox3, Qt::AlignCenter);
    vbox4->addLayout(tran_hbox4, Qt::AlignCenter);
    vbox4->addWidget(confirm_button, Qt::AlignCenter);
    vbox4->addWidget(back_button, Qt::AlignCenter);
    vbox4->setAlignment(Qt::AlignCenter);

    transfer_widget->setLayout(vbox4);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    borrowal_widget = new QWidget();
    borrowal_widget->setWindowTitle("Borrow Money");

    borrowal_widget = new QWidget();
    borrowal_widget->setWindowTitle("Deposit Money");

    borrowal_message1 = new QLabel("Enter Account Number", this);
    account_number_borr = new QLineEdit(this);
    borr_hbox1 = new QHBoxLayout();
    borr_hbox1->addWidget(borrowal_message1, Qt::AlignCenter);
    borr_hbox1->addWidget(account_number_borr, Qt::AlignCenter);

    borrowal_message2 = new QLabel("Enter Password", this);
    password_borr = new QLineEdit();
    password_borr->setEchoMode(QLineEdit::Password);
    borr_hbox2 = new QHBoxLayout();
    borr_hbox2->addWidget(borrowal_message2, Qt::AlignCenter);
    borr_hbox2->addWidget(password_borr, Qt::AlignCenter);

    QString info_text = "Read the text below carefully and choose what best suits your neeeds.\n\n"
                        "Interest Rate Scale according to the borrowed Amount, which can't be changed.\n\n"
                        "1. Borrowed Amount = 100 ---> Interest Rate = 0.1%. PS: TO BE RETURN WITHIN 1 DAY OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                        "2. 500 > Borrowed Amount > 100 ---> Interest Rate = 5% PS: TO BE RETURN WITHIN 2 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                        "3. 1000 > Borrowed Amount >= 500 ---> Interest Rate = 7% PS: TO BE RETURN WITHIN 3 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE\n\n"
                        "4. Borrowed Amount > 1000 ---> Interest Rate = 10% PS: TO BE RETURN WITHIN 4 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE";

    QTextEdit *interest_rate_info = new QTextEdit(this);
    interest_rate_info->setPlainText(info_text);
    interest_rate_info->setReadOnly(true);
    amount_borr = new QLineEdit(this);

    borr_hbox3 = new QHBoxLayout();
    borr_hbox3->addWidget(interest_rate_info, Qt::AlignCenter);
    borr_hbox3->addWidget(amount_borr, Qt::AlignCenter);
    QGroupBox *grp_balance = new QGroupBox();
    grp_balance->setLayout(borr_hbox3);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func6);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox5 = new QVBoxLayout();
    vbox5->addLayout(borr_hbox1, Qt::AlignCenter);
    vbox5->addLayout(borr_hbox2, Qt::AlignCenter);
    vbox5->addWidget(grp_balance, Qt::AlignCenter);
    vbox5->addWidget(confirm_button, Qt::AlignCenter);
    vbox5->addWidget(back_button, Qt::AlignCenter);
    vbox5->setAlignment(Qt::AlignCenter);

    borrowal_widget->setLayout(vbox5);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    return_borrowal_widget = new QWidget();
    return_borrowal_widget->setWindowTitle("Return Borrowed Money");

    return_borrowal_message1 = new QLabel("Enter Account Number", this);
    account_number_ret = new QLineEdit(this);
    ret_hbox1 = new QHBoxLayout();
    ret_hbox1->addWidget(return_borrowal_message1, Qt::AlignCenter);
    ret_hbox1->addWidget(account_number_ret, Qt::AlignCenter);

    return_borrowal_message2 = new QLabel("Enter Password", this);
    password_ret = new QLineEdit(this);
    password_ret->setEchoMode(QLineEdit::Password);
    ret_hbox2 = new QHBoxLayout();
    ret_hbox2->addWidget(return_borrowal_message2, Qt::AlignCenter);
    ret_hbox2->addWidget(password_ret, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func7);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox6 = new QVBoxLayout();
    vbox6->addLayout(ret_hbox1, Qt::AlignCenter);
    vbox6->addLayout(ret_hbox2, Qt::AlignCenter);
    vbox6->addWidget(confirm_button, Qt::AlignCenter);
    vbox6->addWidget(back_button, Qt::AlignCenter);
    vbox6->setAlignment(Qt::AlignCenter);

    return_borrowal_widget->setLayout(vbox6);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    edit_and_forget_widget = new QWidget();
    edit_and_forget_widget->setWindowTitle("Edit Personal Information/ Forget Password");

    edit_perso = new QPushButton("1. Edit Personnal Information", this);
    // connect(edit_perso, &QPushButton::clicked, this, &option_main_window::);

    forget_password = new QPushButton("2. Forget Password", this);
    // connect(forget_password, &QPushButton::clicked, this, &option_main_window::);

    change_password = new QPushButton("3. Change Password", this);
    // connect(change_password, &QPushButton::clicked, this, &option_main_window::);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox7 = new QVBoxLayout();
    vbox7->addWidget(edit_perso, Qt::AlignCenter);
    vbox7->addWidget(forget_password, Qt::AlignCenter);
    vbox7->addWidget(change_password, Qt::AlignCenter);
    vbox7->addWidget(back_button, Qt::AlignCenter);
    vbox7->setAlignment(Qt::AlignCenter);

    edit_and_forget_widget->setLayout(vbox7);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    transaction_history_widget = new QWidget();
    transaction_history_widget->setWindowTitle("Transaction History");

    transaction_history_message1 = new QLabel("Enter Account Number", this);
    account_number_transac = new QLineEdit(this);
    transac_hbox1 = new QHBoxLayout();
    transac_hbox1->addWidget(transaction_history_message1, Qt::AlignCenter);
    transac_hbox1->addWidget(account_number_transac, Qt::AlignCenter);

    transaction_history_message2 = new QLabel("Enter Password", this);
    password_transac = new QLineEdit(this);
    password_transac->setEchoMode(QLineEdit::Password);
    transac_hbox2 = new QHBoxLayout();
    transac_hbox2->addWidget(transaction_history_message2, Qt::AlignCenter);
    transac_hbox2->addWidget(password_transac, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func7);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox8 = new QVBoxLayout();
    vbox8->addLayout(transac_hbox1, Qt::AlignCenter);
    vbox8->addLayout(transac_hbox2, Qt::AlignCenter);
    vbox8->addWidget(confirm_button, Qt::AlignCenter);
    vbox8->addWidget(back_button, Qt::AlignCenter);
    vbox8->setAlignment(Qt::AlignCenter);

    transaction_history_widget->setLayout(vbox8);
    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    delete_account_widget = new QWidget();
    delete_account_widget->setWindowTitle("Delete Account");

    delete_account_message1 = new QLabel("Enter Account Number", this);
    account_number_dele = new QLineEdit(this);
    dele_hbox1 = new QHBoxLayout();
    dele_hbox1->addWidget(delete_account_message1, Qt::AlignCenter);
    dele_hbox1->addWidget(account_number_dele, Qt::AlignCenter);

    delete_account_message2 = new QLabel("Enter Password", this);
    password_dele = new QLineEdit(this);
    password_dele->setEchoMode(QLineEdit::Password);
    dele_hbox2 = new QHBoxLayout();
    dele_hbox2->addWidget(delete_account_message2, Qt::AlignCenter);
    dele_hbox2->addWidget(password_dele, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    connect(confirm_button, &QPushButton::clicked, this, &option_main_window::confirm_button_func9);

    back_button = new QPushButton("Return to the Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &option_main_window::back_button_func);

    vbox9 = new QVBoxLayout();
    vbox9->addLayout(dele_hbox1, Qt::AlignCenter);
    vbox9->addLayout(dele_hbox2, Qt::AlignCenter);
    vbox9->addWidget(confirm_button, Qt::AlignCenter);
    vbox9->addWidget(back_button, Qt::AlignCenter);
    vbox9->setAlignment(Qt::AlignCenter);

    delete_account_widget->setLayout(vbox9);
    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    window_stack->addWidget(central_widget);
    window_stack->addWidget(balance_widget);
    window_stack->addWidget(deposit_widget);
    window_stack->addWidget(withdrawal_widget);
    window_stack->addWidget(transfer_widget);
    window_stack->addWidget(borrowal_widget);
    window_stack->addWidget(return_borrowal_widget);
    window_stack->addWidget(edit_and_forget_widget);
    window_stack->addWidget(transaction_history_widget);
    window_stack->addWidget(delete_account_widget);
}

void option_main_window::back_button_func()
{
    int current_index = window_stack->currentIndex();

    if (current_index)
        window_stack->setCurrentIndex(0);
}

void option_main_window::confirm_button_func1()
{
}

void option_main_window::confirm_button_func2()
{
}

void option_main_window::confirm_button_func3()
{
}
void option_main_window::confirm_button_func4()
{
}
void option_main_window::confirm_button_func5()
{
}
void option_main_window::confirm_button_func6()
{
}
void option_main_window::confirm_button_func7()
{
}
void option_main_window::confirm_button_func8()
{
}
void option_main_window::confirm_button_func9()
{
}