#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QLabel *name;
    QLabel *welcome;
    QPushButton *admin;
    QPushButton *client;
    QPushButton *information;
    QVBoxLayout *vbox;
    QHBoxLayout *hbox;

private slots:
};

#endif
