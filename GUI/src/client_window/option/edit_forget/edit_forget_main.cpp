#include "edit_forget_main_window.h"
#include <database.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    connection_details ID;

    sql::Connection *db_connection = connection_setup(&ID);

    edit_forget_main_window *Edit_forget_main_window = new edit_forget_main_window(db_connection);
    Edit_forget_main_window->show();

    app.exec();
}