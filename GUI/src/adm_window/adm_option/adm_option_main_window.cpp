#include "adm_option_main_window.h"
#include <QMainWindow>
#include <QWidget>

adm_option_main_window::adm_option_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    resize(600, 600);
}