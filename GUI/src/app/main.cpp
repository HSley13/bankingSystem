#include "main_window.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (argc < 2)
    {
        QMessageBox::warning(nullptr, "ERROR!!!!!", "ERROR!!!! The Password to connect to the database should be entered as the second command line argument ( argv[1] )");

        exit(1);
    }

    main_window *Main_window = new main_window(argv[1]);
    Main_window->show();

    return app.exec();
}