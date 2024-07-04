#include "login_window.h"
#include <adm_main_window.h>
#include <option_main_window.h>

login_window::login_window(sql::Connection *db_connection, QStackedWidget *previous_stack, QString choice, QWidget *parent)
    : QMainWindow(parent), connection(db_connection), _previous_stack(previous_stack), _choice(choice)
{
    setWindowTitle("Administrator");
    setStyleSheet("font-family: Arial Black;"
                  "font-size: 20;"
                  "font: bold italic 14px;"
                  "background-color: black;");

    resize(200, 200);

    _stack = new QStackedWidget(this);
    setCentralWidget(_stack);

    QWidget *login_widget = new QWidget(this);

    _account_number = new QLineEdit(this);

    _password = new QLineEdit(this);
    _password->setEchoMode(QLineEdit::Password);

    QPushButton *confirm_login = new QPushButton("Confirm", this);
    confirm_login->setStyleSheet("color: black;"
                                 "background-color: beige;");
    connect(confirm_login, &QPushButton::clicked, this, &login_window::confirm_login_func);

    QLabel *image_label4 = new QLabel(this);
    QPixmap image4("/Users/test/Documents/banking_system/GUI/src/ressources/adm.jpeg");
    image_label4->setPixmap(image4.scaled(80, 80, Qt::KeepAspectRatio));
    image_label4->setScaledContents(true);

    QFormLayout *adm_layout = new QFormLayout();
    adm_layout->addRow("Enter Account Number", _account_number);
    adm_layout->addRow("Enter Password: ", _password);
    adm_layout->addWidget(confirm_login);

    QPushButton *back_button = new QPushButton("Previous Menu", this);
    back_button->setStyleSheet("color: beige;"
                               "font-family: Arial Black;"
                               "font-size: 20;"
                               "font: bold italic 14px;"
                               "background-color: black;");
    connect(back_button, &QPushButton::clicked, this, [=]()
            { _previous_stack->setCurrentIndex(0); });

    QHBoxLayout *hbox = new QHBoxLayout(login_widget);
    hbox->addWidget(image_label4);
    hbox->addLayout(adm_layout);
    hbox->addWidget(back_button);

    _stack->addWidget(login_widget);
}

void login_window::confirm_login_func()
{
    int account_number = _account_number->text().toInt();

    std::string password = _password->text().toStdString();

    std::string hashed_password = std::string();

    (!_choice.compare("client")) ? hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number) : hashed_password = BANK::Qt_retrieve_adm_hashed_password(connection, account_number);

    if (hashed_password.empty())
    {
        _account_number->setStyleSheet("border: 1px solid red");

        return;
    }

    _account_number->setStyleSheet("border: 1px solid gray");

    if (!BANK::verifying_password(password, hashed_password))
    {
        _password->setStyleSheet("border: 1px solid red");

        return;
    }

    _password->setStyleSheet("border: 1px solid gray");

    if (!_choice.compare("client"))
    {
        option_main_window *client_window = new option_main_window(connection, _stack, this);

        _stack->addWidget(client_window);

        _stack->setCurrentWidget(client_window);
    }
    else
    {
        adm_main_window *new_window = new adm_main_window(connection, _stack, this);

        _stack->addWidget(new_window);

        _stack->setCurrentWidget(new_window);
    }

    _account_number->clear();
    _password->clear();
}