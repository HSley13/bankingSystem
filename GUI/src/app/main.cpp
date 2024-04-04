#include "main_window.h"
#include <database.h>
#include <QApplication>
#include <QMessageBox>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (argc < 4)
    {
        QMessageBox::warning(nullptr, "ERROR!!!!!", "ERROR!!!! The Password to connect to the database should be entered as the second command line argument ( argv[1] )");

        exit(1);
    }

    connection_details ID;
    ID.server = argv[1];
    ID.port = 3306;
    ID.user = argv[2];
    ID.password = argv[3];

    sql::Connection *db_connection = connection_setup(&ID);

    main_window *Main_window = new main_window(db_connection);
    Main_window->show();

    return app.exec();
}