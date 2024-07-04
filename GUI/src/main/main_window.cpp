#include "main_window.h"
#include <login_window.h>
#include <client_main_window.h>

main_window::main_window(sql::Connection *db_connection, QWidget *parent)
    : QMainWindow(parent), connection(db_connection)
{
    setWindowTitle("CROSS CONTINENTAL TREASURY BANK");
    resize(200, 200);

    setStyleSheet("font-family: Herculanum;"
                  "font: bold italic 14px;"
                  "font-size: 20;"
                  "background-color: black;");

    _stack = new QStackedWidget(this);
    setCentralWidget(_stack);

    QWidget *central_widget = new QWidget(this);

    QLabel *name = new QLabel("CROSS-CONTINENTAL TREASUTY BANK", this);
    name->setStyleSheet("font-family: Zapfino;"
                        "font: bold italic 20px;"
                        "font-size: 20;");

    QLabel *welcome = new QLabel("Welcome to our Bank", this);

    QPushButton *admin = new QPushButton("1. Administrator", this);
    admin->setStyleSheet("color: black;"
                         "background-color: beige;");
    connect(admin, &QPushButton::clicked, this, &main_window::adm_button_clicked);

    QPushButton *client = new QPushButton("2. Regular Client", this);
    client->setStyleSheet("color: black;"
                          "background-color: beige;");
    connect(client, &QPushButton::clicked, this, &main_window::client_button_clicked);

    QLabel *image_label = new QLabel(this);
    QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/globe_transfer.gif");
    image_label->setPixmap(image.scaled(80, 80, Qt::KeepAspectRatio));
    image_label->setScaledContents(true);

    QVBoxLayout *vbox = new QVBoxLayout(central_widget);
    vbox->addWidget(name, 2, Qt ::AlignCenter);
    vbox->addWidget(image_label, 2, Qt ::AlignCenter);
    vbox->addWidget(welcome, 2, Qt ::AlignCenter);
    vbox->addWidget(admin, Qt ::AlignLeft);
    vbox->addWidget(client, Qt ::AlignLeft);

    _stack->addWidget(central_widget);
}

void main_window::client_button_clicked()
{
    client_main_window *client_window = new client_main_window(connection, _stack, this);

    _stack->addWidget(client_window);

    _stack->setCurrentWidget(client_window);
}

void main_window::adm_button_clicked()
{
    login_window *login = new login_window(connection, _stack, "adm", this);

    _stack->addWidget(login);

    _stack->setCurrentWidget(login);
}
