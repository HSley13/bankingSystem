#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

class adm_main_window : public QMainWindow
{
    Q_OBJECT

public:
    adm_main_window(QWidget *parent = nullptr);

private:
    QLabel *adm_account_number;
    QLabel *adm_password;
    QLineEdit *insert_adm_account_number;
    QLineEdit *insert_adm_password;
    QHBoxLayout *hbox1;
    QHBoxLayout *hbox2;
    QPushButton *button;
    QVBoxLayout *vbox;

private slots:
    void confirm_login();
};
