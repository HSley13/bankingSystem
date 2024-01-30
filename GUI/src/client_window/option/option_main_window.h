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

class option_main_window : public QMainWindow
{
    Q_OBJECT

public:
    option_main_window(QWidget *parent = nullptr);

private:
    QWidget *central_widget;
private slots:
};