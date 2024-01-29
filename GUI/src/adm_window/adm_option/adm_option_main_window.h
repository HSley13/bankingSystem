#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLineEdit>

class adm_option_main_window : public QMainWindow
{
    Q_OBJECT

public:
    adm_option_main_window(QWidget *parent = nullptr);

private:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;
    QPushButton *button7;

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

    QLineEdit *new_adm_account_number;
    QLineEdit *new_adm_password;
    QLineEdit *new_adm_password_confirmation;

private slots:
    void back_button();
    void wid_1_conf();
    void display_accounts_table();
};