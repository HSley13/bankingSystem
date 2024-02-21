#include "edit_forget_main_window.h"
#include <database.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    connection_details ID;
    ID.server = "134.208.2.14";
    ID.port = 3307;
    ID.user = "bankingSystem";
    ID.password = argv[1];

    sql::Connection *db_connection = connection_setup(&ID);

    edit_forget_main_window *Edit_forget_main_window = new edit_forget_main_window(db_connection);
    Edit_forget_main_window->show();

    app.exec();
}