#pragma once

#include <database.h>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class adm_main_window : public QMainWindow
{
    Q_OBJECT

public:
    adm_main_window(sql::Connection *db_connection, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QLabel *adm_account_number;
    QLabel *adm_password;
    QLabel *image_label;

    QLineEdit *insert_adm_account_number;
    QLineEdit *insert_adm_password;

    QPushButton *confirm_login;

    QHBoxLayout *hbox1;
    QHBoxLayout *hbox2;
    QHBoxLayout *hbox;

    QVBoxLayout *vbox;

    QGroupBox *box;

private slots:
    void confirm_login_func();
};
