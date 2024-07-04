#include "client_main_window.h"
#include <login_window.h>

client_main_window::client_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack, QWidget *parent)
    : QMainWindow(parent), connection(db_connection), _previous_stack(previous_stack)
{
    _stack = new QStackedWidget(this);
    _stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(_stack);
    setStyleSheet("color: beige;"
                  "font-family: Arial Black;"
                  "font-size: 20;"
                  "font: bold italic 14px;"
                  "background-color: black;");
    setWindowTitle("Client");
    resize(200, 200);

    QWidget *client_option = new QWidget(this);

    QPushButton *create_account = new QPushButton("1. Create New Account", this);
    connect(create_account, &QPushButton::clicked, this, [=]()
            { _stack->setCurrentIndex(1); });

    QPushButton *account_inquiry = new QPushButton("2. Already possess an Account", this);
    connect(account_inquiry, &QPushButton::clicked, this, &client_main_window::account_inquiry_func);

    QPushButton *bank_info = new QPushButton("3. Information on our Bank", this);
    connect(bank_info, &QPushButton::clicked, this, [=]()
            { _stack->setCurrentIndex(2); });

    QLabel *image_label = new QLabel(this);
    QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/client.jpeg");
    image_label->setPixmap(image.scaled(80, 80, Qt::KeepAspectRatio));
    image_label->setScaledContents(true);

    _back_button = new QPushButton("Previous Menu", this);
    connect(_back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    _VBOX = new QVBoxLayout(client_option);
    _VBOX->addWidget(image_label, 2, Qt::AlignCenter);
    _VBOX->addWidget(create_account, Qt::AlignCenter);
    _VBOX->addWidget(account_inquiry, Qt::AlignCenter);
    _VBOX->addWidget(bank_info, Qt::AlignCenter);
    _VBOX->addWidget(_back_button, Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *create_account_widget = new QWidget(this);
    create_account_widget->setWindowTitle("Create Account");

    QLabel *message = new QLabel("Please Provide Us with the following Information to create your Account", this);

    _national_ID = new QLineEdit(this);
    _first_name = new QLineEdit(this);
    _last_name = new QLineEdit(this);

    _calendar = new QDateEdit(this);
    _calendar->setCalendarPopup(true);
    _calendar->setDate(QDate::currentDate());
    connect(_calendar, &QDateEdit::dateChanged, this, [=]()
            { _selected_date = _calendar->date(); });

    _phone_number = new QLineEdit(this);
    _email = new QLineEdit(this);
    _address = new QLineEdit(this);

    QString info_text = "Minimum Balance is $100\n\n"
                        "Definitive Interest Rate Scale according to your First Deposit.\n\n"
                        "1. Balance = $100 ---> Interest Rate = 0%\n"
                        "2. $500 > Balance > $100 ---> Interest Rate = 2%\n"
                        "3. $1000 > Balance >= $500 ---> Interest Rate = 5%\n"
                        "4. Balance > $1000 ---> Interest Rate = 7%";

    QTextEdit *interest_rate_info = new QTextEdit(this);
    interest_rate_info->setPlainText(info_text);
    interest_rate_info->setReadOnly(true);

    _balance = new QLineEdit(this);
    _password = new QLineEdit(this);
    _password_confirmation = new QLineEdit(this);
    _password_confirmation->setEchoMode(QLineEdit::Password);

    _question = new QLineEdit(this);
    _answer = new QLineEdit(this);
    _confirm_answer = new QLineEdit(this);

    QPushButton *confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black; background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &client_main_window::confirm_button_func);

    _back_button = new QPushButton("Previous Menu", this);
    _back_button->setStyleSheet("color: beige;"
                                "font-family: Arial Black;"
                                "font-size: 20;"
                                "font: bold italic 14px;"
                                "background-color: black;");
    connect(_back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    QGridLayout *gridLayout = new QGridLayout(create_account_widget);

    QFormLayout *formLayout1 = new QFormLayout();
    formLayout1->addRow("National ID: ", _national_ID);
    formLayout1->addRow("First Name: ", _first_name);
    formLayout1->addRow("Last Name: ", _last_name);
    formLayout1->addRow("Date of Birth: ", _calendar);
    formLayout1->addRow("Phone Number: ", _phone_number);
    formLayout1->addRow("Email: ", _email);
    formLayout1->addRow("Address: ", _address);

    QFormLayout *formLayout2 = new QFormLayout();
    formLayout2->addRow(interest_rate_info, _balance);
    formLayout2->addRow("Password: ", _password);
    formLayout2->addRow("Password Confirmation: ", _password_confirmation);
    formLayout2->addRow("Secret Question", _question);
    formLayout2->addRow("Secret Answer", _answer);
    formLayout2->addRow("Secret Answer Confirmation", _confirm_answer);

    gridLayout->addWidget(message, 0, 0, 1, 2, Qt::AlignCenter);
    gridLayout->addLayout(formLayout1, 1, 0);
    gridLayout->addLayout(formLayout2, 1, 1);
    gridLayout->addWidget(confirm_button, 2, 0, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(_back_button, 3, 0, 1, 2, Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *bank_info_widget = new QWidget(this);
    bank_info_widget->setWindowTitle("Bank Info");

    QTextEdit *information = new QTextEdit(this);
    information->setPlainText(
        "Title: The CROSS_CONTINENTAL TREASURY BANK: A Compendium of Financial Excellence\n\n"
        "CHAPTER 1: INTRODUCTION TO THE CROSS-CONTINENTAL TREASURY BANK\n"
        "SECTION 1.1: Our Vision and Mission\n"
        "\n"
        "The CROSS-CONTINENTAL TREASURY BANK envisions a world where financial services transcend borders, empower individuals, and foster economic growth.\n"
        "Our mission is to provide cutting-edge banking solutions with a commitment to integrity, innovation, and client satisfaction.\n"
        "SECTION 1.2: Core Values\n"
        "\n"
        "Integrity: Upholding the highest ethical standards in all interactions.\n"
        "Innovation: Pioneering financial solutions that adapt to the dynamic global landscape.\n"
        "Client Satisfaction: Placing our clients at the center and ensuring their success is our success.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 2: THE PILLARS OF FINANCIAL SECURITY\n"
        "\n"
        "SECTION 2.1: Robust Security Protocols\n"
        "\n"
        "Implementing state-of-the-art encryption and cybersecurity measures to safeguard client information.\n"
        "Regular audits and assessments to fortify our defenses against emerging threats.\n"
        "SECTION 2.2: Compliance and Regulatory Adherence\n"
        "\n"
        "Strict adherence to international banking regulations and compliance standards.\n"
        "Continuous training and development for staff to stay abreast of regulatory changes.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 3: SERVICES TAILORED TO YOUR AMBITIONS\n"
        "\n"
        "SECTION 3.1: Personalized Banking Solutions\n"
        "\n"
        "Tailoring financial services to meet the unique needs and aspirations of our diverse clientele.\n"
        "Offering a wide array of accounts, investment options, and lending services.\n"
        "SECTION 3.2: Global Accessibility\n"
        "\n"
        "A network of branches and ATMs spanning continents, ensuring clients can access their funds whenever and wherever they need.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 4: EMPOWERING FINANCIAL LITERACY\n"
        "\n"
        "SECTION 4.1: Educational Initiatives\n"
        "\n"
        "The CROSS-CONTINENTAL TREASURY BANK commits to promoting financial literacy through workshops, seminars, and accessible online resources.\n"
        "Encouraging informed decision-making and responsible financial behavior.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 5: ENVIRONMENTAL AND SOCIAL RESPONSIBILITY\n"
        "\n"
        "SECTION 5.1: Sustainable Banking Practices\n"
        "\n"
        "Integrating environmental and social responsibility into our business model.\n"
        "Investing in eco-friendly initiatives and supporting community development projects.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 6: THE FUTURE OF BANKING\n"
        "\n"
        "SECTION 6.1: Technological Advancements\n"
        "\n"
        "Embracing cutting-edge technology to enhance efficiency, security, and user experience.\n"
        "Exploring blockchain, artificial intelligence, and other innovations to shape the future of banking.\n"
        "SECTION 6.2: Continuous Improvement\n"
        "\n"
        "A commitment to continuous improvement through client feedback, market analysis, and strategic partnerships.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 7: CLIENT-CENTRIC APPROACH\n"
        "\n"
        "SECTION 7.1: Personalized Customer Service\n"
        "\n"
        "A dedicated customer support team ensuring a seamless and delightful banking experience.\n"
        "Proactive resolution of issues and a commitment to exceeding client expectations.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 8: THANKS FOR CHOOSING OUR BANK\n"
        "\n"
        "In the ever-evolving world of finance, The CROSS-CONTINENTAL TREASURY BANK stands as a beacon of reliability, innovation, and commitment.\n"
        " This compendium serves as a testament to our dedication to providing unparalleled financial services on a global scale. \n"
        " Join us in the pursuit of financial excellence, where your aspirations find a home in The CROSS-CONTINENTAL TREASURY BANK.\n");

    information->setReadOnly(true);
    information->setLineWrapMode(QTextEdit::WidgetWidth);
    information->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    information->setStyleSheet("color: white;");

    _back_button = new QPushButton("Previous Menu", this);
    _back_button->setStyleSheet("color: beige;"
                                "font-family: Arial Black;"
                                "font-size: 20;"
                                "font: bold italic 14px;"
                                "background-color: black;");
    connect(_back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    _VBOX = new QVBoxLayout(bank_info_widget);
    _VBOX->addWidget(information, Qt::AlignCenter);
    _VBOX->addWidget(_back_button, Qt::AlignCenter);
    _VBOX->setAlignment(Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    _stack->addWidget(client_option);
    _stack->addWidget(create_account_widget);
    _stack->addWidget(bank_info_widget);
}

void client_main_window::back_button_func()
{
    int current_index = _stack->currentIndex();

    (current_index != 0) ? _stack->setCurrentIndex(0) : _previous_stack->setCurrentIndex(0);
}

void client_main_window::confirm_button_func()
{
    int account_number;

    if (_balance->text().toDouble() < 100 || _password->text().compare(_password_confirmation->text()))
    {
        if (_balance->text().toDouble() < 100)
        {
            _balance->setStyleSheet("border: 1px solid red");
            QMessageBox::warning(this, "Balance XXX", "Insufficient Balance, Enter an amount >= 100");

            return;
        }
        else
        {
            _password_confirmation->setStyleSheet("border: 1px solid red");
            QMessageBox::warning(this, "Password XXX", "Password Confirmation Incorrect, check and try again");

            return;
        }
    }

    _balance->setStyleSheet("border: 1px solid gray");
    _password_confirmation->setStyleSheet("border: 1px solid gray");

    double interest_rate = 0;
    if (_balance->text().toDouble() > 100 && _balance->text().toDouble() < 500)
        interest_rate = 0.02;
    else if (_balance->text().toDouble() < 1000 && _balance->text().toDouble() >= 500)
        interest_rate = 0.05;
    else if (_balance->text().toDouble() >= 1000)
        interest_rate = 0.07;

    if (_question->text().isEmpty())
    {
        QMessageBox::warning(this, "void!!!", "Question left empty!!!");
        _question->setStyleSheet("border: 1px solid red;");

        return;
    }

    _question->setStyleSheet("border: 1px solid gray;");

    if (_answer->text().isEmpty())
    {
        QMessageBox::warning(this, "void!!!", "Answer left empty!!!");
        _answer->setStyleSheet("border: 1px solid red;");

        return;
    }

    _answer->setStyleSheet("border: 1px solid gray;");

    if (_answer->text().compare(_confirm_answer->text()))
    {
        QMessageBox::warning(this, "Confirmation Incorrect!!", "The Answer's Confirmation is Incorrect!!");
        _confirm_answer->setStyleSheet("border: 1px solid red;");

        return;
    }

    _confirm_answer->setStyleSheet("border: 1px solid gray;");

    bool OK;

    QString info = QString("National ID: %1\n First Name: %2\n Last Name: %3\n Date of Birth: %4\n Phone Number: %5\n Phone Number: %6\n Email: %7\n Address: %8\n Balance: %9\n Question: %10\n Answer: %11\n\n  Type YES to confirm and NO to cancel")
                       .arg(_national_ID->text())
                       .arg(_first_name->text())
                       .arg(_last_name->text())
                       .arg(_selected_date.toString(Qt::ISODate))
                       .arg(_phone_number->text())
                       .arg(_email->text())
                       .arg(_address->text())
                       .arg(_balance->text())
                       .arg(_question->text())
                       .arg(_answer->text());

    QMessageBox review;
    review.setWindowTitle("Information Review");
    review.setText(info);
    review.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int result = review.exec();

    if (result == QMessageBox::Cancel)
        return;

    std::string password = _password->text().toStdString();

    std::string hash_password = BANK::hashing_password(password);

    Account::Qt_create_account(connection, account_number, _national_ID->text().toStdString(), _first_name->text().toStdString(), _last_name->text().toStdString(), _selected_date.toString(Qt::ISODate).toStdString(), _phone_number->text().toInt(), _email->text().toStdString(), _address->text().toStdString(), _balance->text().toDouble(), interest_rate, hash_password, _question->text().toStdString(), _answer->text().toStdString());
}

void client_main_window::account_inquiry_func()
{
    login_window *client_login = new login_window(connection, _stack, "client", this);

    _stack->addWidget(client_login);

    _stack->setCurrentWidget(client_login);
}
