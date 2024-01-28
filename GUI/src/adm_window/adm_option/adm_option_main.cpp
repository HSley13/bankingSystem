#include "adm_option_main_window.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    adm_option_main_window *Adm_option_main_window = new adm_option_main_window;
    Adm_option_main_window->show();

    app.exec();
}