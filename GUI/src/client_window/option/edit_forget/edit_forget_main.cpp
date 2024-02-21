#include "edit_forget_main_window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    edit_forget_main_window *Edit_forget_main_window = new edit_forget_main_window(argv[1]);
    Edit_forget_main_window->show();

    app.exec();
}