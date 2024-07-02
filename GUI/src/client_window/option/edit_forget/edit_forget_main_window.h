#pragma once

#include <database.h>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

class edit_forget_main_window : public QMainWindow
{
    Q_OBJECT

public:
    edit_forget_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack = nullptr, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *stack;

    QStackedWidget *_previous_stack;

    QWidget *edit_forget_widget;

    QWidget *edit_perso_widget;
    QWidget *edit_first_name_widget;
    QWidget *edit_email_widget;
    QWidget *edit_address_widget;
    QWidget *edit_phone_number_widget;

    QWidget *change_password_widget;
    QWidget *forget_password_widget;

    QPushButton *edit_perso;
    QPushButton *change_password;
    QPushButton *forget_password;
    QPushButton *back_button;
    QPushButton *confirm_button;

    QVBoxLayout *VBOX;

    QLabel *account_number_change;
    QLabel *password_change;
    QLabel *new_password_change;
    QLabel *new_password_confirmation_change;

    QLineEdit *insert_account_number_change;
    QLineEdit *insert_password_change;
    QLineEdit *insert_new_password_change;
    QLineEdit *insert_new_password_confirmation_change;

    QHBoxLayout *change_hbox_1;
    QHBoxLayout *change_hbox_2;
    QHBoxLayout *change_hbox_3;
    QHBoxLayout *change_hbox_4;

    QLabel *account_number_forget;
    QLabel *password_forget;
    QLabel *new_password_forget;
    QLabel *new_password_confirmation_forget;
    QLabel *national_ID_forget;
    QLabel *date_birth_forget;

    QLineEdit *insert_account_number_forget;
    QLineEdit *insert_password_forget;
    QLineEdit *insert_new_password_forget;
    QLineEdit *insert_new_password_confirmation_forget;
    QLineEdit *insert_national_ID_forget;
    QLineEdit *insert_date_birth_forget;

    QHBoxLayout *forget_hbox_1;
    QHBoxLayout *forget_hbox_2;
    QHBoxLayout *forget_hbox_3;
    QHBoxLayout *forget_hbox_4;
    QHBoxLayout *forget_hbox_5;
    QHBoxLayout *forget_hbox_6;

    QPushButton *name_button;
    QPushButton *email_button;
    QPushButton *address_button;
    QPushButton *phone_number_button;

    QLabel *edit_name;
    QLabel *edit_name_confirmation;
    QLabel *edit_email;
    QLabel *edit_email_confirmation;
    QLabel *edit_address;
    QLabel *edit_address_confirmation;
    QLabel *edit_phone_number;
    QLabel *edit_phone_number_confirmation;

    QLineEdit *insert_name;
    QLineEdit *insert_name_confirmation;
    QLineEdit *insert_email;
    QLineEdit *insert_email_confirmation;
    QLineEdit *insert_address;
    QLineEdit *insert_address_confirmation;
    QLineEdit *insert_phone_number;
    QLineEdit *insert_phone_number_confirmation;

    QHBoxLayout *edit_hbox_0;
    QHBoxLayout *edit_hbox_1;
    QHBoxLayout *edit_hbox_2;
    QHBoxLayout *edit_hbox_3;

    QHBoxLayout *edit_email_hbox_0;
    QHBoxLayout *edit_email_hbox_1;
    QHBoxLayout *edit_email_hbox_2;
    QHBoxLayout *edit_email_hbox_3;

    QHBoxLayout *edit_address_hbox_0;
    QHBoxLayout *edit_address_hbox_1;
    QHBoxLayout *edit_address_hbox_2;
    QHBoxLayout *edit_address_hbox_3;

    QHBoxLayout *edit_phone_number_hbox_0;
    QHBoxLayout *edit_phone_number_hbox_1;
    QHBoxLayout *edit_phone_number_hbox_2;
    QHBoxLayout *edit_phone_number_hbox_3;

    QLabel *account_number_1;
    QLabel *account_number_2;
    QLabel *account_number_3;
    QLabel *account_number_4;

    QLineEdit *insert_account_number_1;
    QLineEdit *insert_account_number_2;
    QLineEdit *insert_account_number_3;
    QLineEdit *insert_account_number_4;

    QLabel *password_1;
    QLabel *password_2;
    QLabel *password_3;
    QLabel *password_4;

    QLineEdit *insert_password_1;
    QLineEdit *insert_password_2;
    QLineEdit *insert_password_3;
    QLineEdit *insert_password_4;

    QVBoxLayout *vbox_1;
    QVBoxLayout *vbox_2;
    QVBoxLayout *vbox_3;
    QVBoxLayout *vbox_4;
    QVBoxLayout *vbox_5;
    QVBoxLayout *vbox_6;
    QVBoxLayout *vbox_7;

private slots:
    void back_button_func();
    void back_button_func2();
    void confirm_change_func();
    void confirm_forget_func();

    void confirm_edit_name_func();
    void confirm_edit_email_func();
    void confirm_edit_address_func();
    void confirm_edit_phone_number_func();
};