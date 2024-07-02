#include "client_main_window.h"
#include <option_main_window.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

client_main_window::client_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack, QWidget *parent)
    : QMainWindow(parent), connection(db_connection), _previous_stack(previous_stack)
{
    stack = new QStackedWidget(this);
    stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(stack);
    setStyleSheet("color: beige;"
                  "font-family: Arial Black;"
                  "font-size: 20;"
                  "font: bold italic 14px;"
                  "background-color: black;");
    setWindowTitle("Client");
    resize(200, 200);

    QWidget *client_option = new QWidget();

    QPushButton *create_account = new QPushButton("1. Create New Account", this);
    connect(create_account, &QPushButton::clicked, this, [=]()
            { stack->setCurrentIndex(1); });

    QPushButton *account_inquiry = new QPushButton("2. Already possess an Account", this);
    connect(account_inquiry, &QPushButton::clicked, this, &client_main_window::account_inquiry_func);

    QPushButton *bank_info = new QPushButton("3. Information on our Bank", this);
    connect(bank_info, &QPushButton::clicked, this, [=]()
            { stack->setCurrentIndex(2); });

    QLabel *image_label = new QLabel(this);
    QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/client.jpeg");
    image_label->setPixmap(image.scaled(80, 80, Qt::KeepAspectRatio));
    image_label->setScaledContents(true);

    back_button = new QPushButton("Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    vbox = new QVBoxLayout(client_option);
    vbox->addWidget(image_label, 2, Qt::AlignCenter);
    vbox->addWidget(create_account, Qt::AlignCenter);
    vbox->addWidget(account_inquiry, Qt::AlignCenter);
    vbox->addWidget(bank_info, Qt::AlignCenter);
    vbox->addWidget(back_button, Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *create_account_widget = new QWidget();
    create_account_widget->setWindowTitle("Create Account");

    QLabel *message = new QLabel("Please Provide Us with the following Information to create your Account", this);

    QLineEdit *insert_national_ID = new QLineEdit(this);
    QLineEdit *insert_first_name = new QLineEdit(this);
    QLineEdit *insert_last_name = new QLineEdit(this);

    QDateEdit *calendar = new QDateEdit(this);
    calendar->setCalendarPopup(true);
    calendar->setDate(QDate::currentDate());
    connect(calendar, &QDateEdit::dateChanged, this, [=]()
            { selected_date = calendar->date(); });

    QLineEdit *insert_phone_number = new QLineEdit(this);
    QLineEdit *insert_email = new QLineEdit(this);
    QLineEdit *insert_address = new QLineEdit(this);

    QString info_text = "Minimum Balance is $100\n\n"
                        "Definitive Interest Rate Scale according to your First Deposit.\n\n"
                        "1. Balance = $100 ---> Interest Rate = 0%\n"
                        "2. $500 > Balance > $100 ---> Interest Rate = 2%\n"
                        "3. $1000 > Balance >= $500 ---> Interest Rate = 5%\n"
                        "4. Balance > $1000 ---> Interest Rate = 7%";

    QTextEdit *interest_rate_info = new QTextEdit(this);
    interest_rate_info->setPlainText(info_text);
    interest_rate_info->setReadOnly(true);

    QLineEdit *insert_balance = new QLineEdit(this);
    QLineEdit *insert_password = new QLineEdit(this);
    QLineEdit *insert_password_confirmation = new QLineEdit(this);
    insert_password_confirmation->setEchoMode(QLineEdit::Password);

    QLineEdit *insert_question = new QLineEdit(this);
    QLineEdit *insert_answer = new QLineEdit(this);
    QLineEdit *insert_confirm_answer = new QLineEdit(this);

    QPushButton *confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black; background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &client_main_window::confirm_button_func);

    back_button = new QPushButton("Previous Menu", this);
    back_button->setStyleSheet("color: beige;"
                               "font-family: Arial Black;"
                               "font-size: 20;"
                               "font: bold italic 14px;"
                               "background-color: black;");
    connect(back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    QGridLayout *gridLayout = new QGridLayout(create_account_widget);

    QFormLayout *formLayout1 = new QFormLayout();
    formLayout1->addRow("National ID: ", insert_national_ID);
    formLayout1->addRow("First Name: ", insert_first_name);
    formLayout1->addRow("Last Name: ", insert_last_name);
    formLayout1->addRow("Date of Birth: ", calendar);
    formLayout1->addRow("Phone Number: ", insert_phone_number);
    formLayout1->addRow("Email: ", insert_email);
    formLayout1->addRow("Address: ", insert_address);

    QFormLayout *formLayout2 = new QFormLayout();
    formLayout2->addRow(interest_rate_info, insert_balance);
    formLayout2->addRow("Password: ", insert_password);
    formLayout2->addRow("Password Confirmation: ", insert_password_confirmation);
    formLayout2->addRow("Secret Question", insert_question);
    formLayout2->addRow("Secret Answer", insert_answer);
    formLayout2->addRow("Secret Answer Confirmation", insert_confirm_answer);

    gridLayout->addWidget(message, 0, 0, 1, 2, Qt::AlignCenter);
    gridLayout->addLayout(formLayout1, 1, 0);
    gridLayout->addLayout(formLayout2, 1, 1);
    gridLayout->addWidget(confirm_button, 2, 0, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(back_button, 3, 0, 1, 2, Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *bank_info_widget = new QWidget();
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

    back_button = new QPushButton("Previous Menu", this);
    back_button->setStyleSheet("color: beige;"
                               "font-family: Arial Black;"
                               "font-size: 20;"
                               "font: bold italic 14px;"
                               "background-color: black;");
    connect(back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    vbox = new QVBoxLayout(bank_info_widget);
    vbox->addWidget(information, Qt::AlignCenter);
    vbox->addWidget(back_button, Qt::AlignCenter);
    vbox->setAlignment(Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
    stack->addWidget(client_option);
    stack->addWidget(create_account_widget);
    stack->addWidget(bank_info_widget);
}

void client_main_window::back_button_func()
{
    int current_index = stack->currentIndex();

    (current_index != 0) ? stack->setCurrentIndex(0) : _previous_stack->setCurrentIndex(0);
}

void client_main_window::confirm_button_func()
{
    int account_number;

    if (insert_balance->text().toDouble() < 100 || insert_password->text().compare(insert_password_confirmation->text()))
    {
        if (insert_balance->text().toDouble() < 100)
        {
            insert_balance->setStyleSheet("border: 1px solid red");
            QMessageBox::warning(this, "Balance XXX", "Insufficient Balance, Enter an amount >= 100");

            return;
        }
        else
        {
            insert_password_confirmation->setStyleSheet("border: 1px solid red");
            QMessageBox::warning(this, "Password XXX", "Password Confirmation Incorrect, check and try again");

            return;
        }
    }

    insert_balance->setStyleSheet("border: 1px solid gray");
    insert_password_confirmation->setStyleSheet("border: 1px solid gray");

    double interest_rate = 0;
    if (insert_balance->text().toDouble() > 100 && insert_balance->text().toDouble() < 500)
        interest_rate = 0.02;
    else if (insert_balance->text().toDouble() < 1000 && insert_balance->text().toDouble() >= 500)
        interest_rate = 0.05;
    else if (insert_balance->text().toDouble() >= 1000)
        interest_rate = 0.07;

    if (insert_question->text().isEmpty())
    {
        QMessageBox::warning(this, "void!!!", "Question left empty!!!");
        insert_question->setStyleSheet("border: 1px solid red;");

        return;
    }

    insert_question->setStyleSheet("border: 1px solid gray;");

    if (insert_answer->text().isEmpty())
    {
        QMessageBox::warning(this, "void!!!", "Answer left empty!!!");
        insert_answer->setStyleSheet("border: 1px solid red;");

        return;
    }

    insert_answer->setStyleSheet("border: 1px solid gray;");

    if (insert_answer->text().compare(insert_confirm_answer->text()))
    {
        QMessageBox::warning(this, "Confirmation Incorrect!!", "The Answer's Confirmation is Incorrect!!");
        insert_confirm_answer->setStyleSheet("border: 1px solid red;");

        return;
    }

    insert_confirm_answer->setStyleSheet("border: 1px solid gray;");

    bool OK;

    QString info = QString("National ID: %1\n First Name: %2\n Last Name: %3\n Date of Birth: %4\n Phone Number: %5\n Phone Number: %6\n Email: %7\n Address: %8\n Balance: %9\n Question: %10\n Answer: %11\n\n  Type YES to confirm and NO to cancel")
                       .arg(insert_national_ID->text())
                       .arg(insert_first_name->text())
                       .arg(insert_last_name->text())
                       .arg(selected_date.toString(Qt::ISODate))
                       .arg(insert_phone_number->text())
                       .arg(insert_email->text())
                       .arg(insert_address->text())
                       .arg(insert_balance->text())
                       .arg(insert_question->text())
                       .arg(insert_answer->text());

    QMessageBox review;
    review.setWindowTitle("Information Review");
    review.setText(info);
    review.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int result = review.exec();

    if (result == QMessageBox::Cancel)
        return;

    std::string password = insert_password->text().toStdString();

    std::string hash_password = BANK::hashing_password(password);

    Account::Qt_create_account(connection, account_number, insert_national_ID->text().toStdString(), insert_first_name->text().toStdString(), insert_last_name->text().toStdString(), selected_date.toString(Qt::ISODate).toStdString(), insert_phone_number->text().toInt(), insert_email->text().toStdString(), insert_address->text().toStdString(), insert_balance->text().toDouble(), interest_rate, hash_password, insert_question->text().toStdString(), insert_answer->text().toStdString());
}

void client_main_window::account_inquiry_func()
{
    QMessageBox::information(this, "Redirecting...", "You are about to be redirected to the Client's Official Page");

    option_main_window *option_window = new option_main_window(connection, stack, this);

    stack->addWidget(option_window);

    stack->setCurrentWidget(option_window);
}