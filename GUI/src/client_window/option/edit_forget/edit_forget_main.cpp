#include "edit_forget_main_window.h"
#include <database.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    sql::Connection *db_connection;

    edit_forget_main_window *Edit_forget_main_window = new edit_forget_main_window(db_connection);
    Edit_forget_main_window->show();

    app.exec();
}