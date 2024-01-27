#pragma once

#include <iostream>
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);

private:
    QLabel *name;
    QLabel *welcome;
    QPushButton *admin;
    QPushButton *client;
    QPushButton *information;
    QVBoxLayout *vbox;
    QHBoxLayout *hbox;

private slots:
    void adm_button_clicked();
    void client_button_clicked();
    void bank_info_button_clicked();
};