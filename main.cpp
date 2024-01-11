#include <iostream>
#include </usr/local/mysql/include/mysql.h>
#include <string>
#include <vector>
using namespace std;

//  g++ -std=c++11 -g -o main main.cpp -L/usr/local/mysql-8.2.0-macos13-arm64/lib -lmysqlclient -rpath /usr/local/mysql-8.2.0-macos13-arm64/lib

// FOR THIS PROGRAM, THERE ARE SOME TRIGGERS THAT I HAVE CREATED ON THE COMPUTER AMD ARE NOT PART OF THIS CODE.
// I HAVE PROCEEDED THAT WAY BECAUSE TRIGGERS CAN ONLY BE CREATED ONCE AND BECAUSE THAT PROGRAM WILL BE RUN MORE THAN ONCE, IT IS SAFER NOT IMBEDDING THE TRIGGERS CODE IN THE MAIN FUNCTION CAUSE IT WOULD
// TRIED TO CREATE THEM AS MANY TIME AS THE PROGRAM IS RUN AND AFTER THE FIRST TIME, YOU WOULD ALWAYS GET AN SQL ERROR EVERYTIME SAYING THE TRIGGERS YOU ARE TRYING TO CREATE ALREADY EXIST

// THOSE TRIGGERS MUST BE ON THE COMPUTER IN ORDER FOR THE PROFRAM TO RUN AS EXPECTED.
// PS: WRITE ME ON YOUR MAIL IF YOU WANT TO HAVE THEM (sleyhortes13@gamil.com) OR COMMENT IT ON THAT GIT REPOSITORY



// TO do
// Passoword Hahing for a better security
// Finish the Bank Class which will be responsible to manage the create accounts
// check and resolve the memory leaks problem
// reduce code redundancy into functions
// review the code and make it the simplest way it can be
// Prevent direct SGL injection in the code by changing the queries methods
// create the GUI for a more user friendly

struct connection_details
{
    string server, user, password, database;
};

MYSQL *mysql_connection_setup(connection_details *mysql_ID)
{
    MYSQL *connection = mysql_init(NULL);
    if (!connection)
    {
        cerr << "Database Initialiation Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    if (!mysql_real_connect(connection, mysql_ID->server.c_str(), mysql_ID->user.c_str(), mysql_ID->password.c_str(), mysql_ID->database.c_str(), 0, NULL, 0))
    {
        cerr << "Error Connection to the database: " << mysql_error(connection) << endl;
        exit(1);
    }

    return connection;
}

MYSQL_RES *mysql_perform_query(MYSQL *connection, const char *sql_query)
{
    if (mysql_query(connection, sql_query))
    {
        cerr << "Query Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    return mysql_use_result(connection);
}

class Bank
{
    public :

    vector <string> password;
    
};

class Account
{
    public:

    string first_name, last_name, date_birth, email, password, national_ID, address;
    int phone_number;

    void create_account(int account_number, string national_ID, string first_name, string last_name, string date_birth, int phone_number, string email, string address, double balance, string password, MYSQL *connection);

    void check_balance(const Account &obj, MYSQL *connection, double account_number);

    void remove_accounts(const Account &obj, MYSQL *connection, double account_number);
};

void Account :: create_account(int account_number, string national_ID, string first_name, string last_name, string date_birth, int phone_number, string email, string address, double balance, string password, MYSQL *connection)
{
    string account_number_str;
    string balance_str = to_string(balance);
    string phone_number_str = to_string(phone_number);

    string create_account = "INSERT INTO accounts (national_ID, first_name, last_name, date_birth, phone_number, email, address, balance) VALUES ("+national_ID+" , "+first_name+", "+last_name+", "+date_birth+" , "+phone_number_str+" , "+email+" , "+address+" , "+balance_str+");";

    MYSQL_RES *result = mysql_perform_query(connection, create_account.c_str());

    cout << "This is Your Account Number, remember it cause you will need it to gain access to everything you want to do in the future:  ";

    string assign_account_number = "SELECT account_number FROM accounts WHERE national_ID = "+national_ID+"; ";

    result = mysql_perform_query(connection, assign_account_number.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        
        if (row)
        {
            cout << row[0] << endl;

            account_number_str.assign(row[0]);
            string table_history = "CREATE TABLE NO"+account_number_str+" (transaction_details VARCHAR(100), date_time DATIME DEFAULT NOW()); ";
            result = mysql_perform_query(connection, table_history.c_str());
        }   
    }
}

void Account :: check_balance(const Account &obj, MYSQL *connection, double account_number)
{
    string account_number_str = to_string(account_number);

    string check_balance0 = "SELECT balance FROM accounts WHERE account_number = "+account_number_str+"; ";

    MYSQL_RES *result = mysql_perform_query(connection, check_balance0.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);

        if (row)
        {
            cout << "Your Current Balance is: ";

            cout << row[mysql_num_fields(result) - 1];
        }

        mysql_free_result(result);
    }
}

void Account :: remove_accounts(const Account &obj, MYSQL *connection, double account_number)
{
    string account_number_str = to_string(account_number);

    string remove_account0 = "DELETE FROM accounts WHERE account_number = "+account_number_str+"; ";
}

class Transactions : public Account
{
    public:

    void deposit(MYSQL *connection, double amount_to_deposit, double account_number);

    void withdrawal( MYSQL *connection, double sum_to_withdraw, double account_number);

    void transfer(MYSQL *connection, double amount_to_send, double account_number1, double account_number2);

    void transactions_history(MYSQL *connection, double account_number); 
};

void Transactions :: deposit (MYSQL *connection, double amount_to_deposit, double account_number)
{
     MYSQL_RES *result;
    string account_number_str = to_string(account_number);
    string amount_to_deposit_str = to_string(amount_to_deposit);

    string deposit0 = "UPDATE transactions SET withdraw = "+amount_to_deposit_str+" WHERE account_number = "+account_number_str+"; ";
    result = mysql_perform_query(connection, deposit0.c_str());

    cout << "You have withdrawed " << amount_to_deposit << " dollars and your new Balance is: ";

    string new_balance = "SELECT balance FROM accounts WHERE account_number = "+account_number_str+"; ";
    result = mysql_perform_query(connection, new_balance.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);

        if (row) cout << row[0] << endl;

        mysql_free_result(result);
    }

    string new_deposit = "New Deposit of ";

    string log_deposit0 = "INSERT INTO NO"+account_number_str+" VALUES (CONCAT("+new_deposit+", "+amount_to_deposit_str+"), NOW()) ";
    result = mysql_perform_query(connection, log_deposit0.c_str());
}

void Transactions :: withdrawal(MYSQL *connection, double amount_to_withdraw, double account_number)
{
    MYSQL_RES *result;
    string account_number_str = to_string(account_number);
    string amount_to_withdraw_str = to_string(amount_to_withdraw);

    string withdrawal0 = "UPDATE transactions SET withdraw = "+amount_to_withdraw_str+" WHERE account_number = "+account_number_str+"; ";
    result = mysql_perform_query(connection, withdrawal0.c_str());

    cout << "You have withdrawed " << amount_to_withdraw << " dollars and your new Balance is: ";

    string new_balance = "SELECT balance FROM accounts WHERE account_number = "+account_number_str+"; ";
    result = mysql_perform_query(connection, new_balance.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);

        if (row) cout << row[0] << endl;

        mysql_free_result(result);
    }

    string new_withdraw = "New Withdraw of ";

    string log_withdrawal0 = "INSERT INTO NO"+account_number_str+" VALUES (CONCAT("+new_withdraw+", "+amount_to_withdraw_str+"), NOW()) ";
    result = mysql_perform_query(connection, log_withdrawal0.c_str());
}

void Transactions :: transfer(MYSQL *connection, double amount_to_transfer, double account_number1, double account_number2)
{
    MYSQL_RES *result;
    string account_number1_str = to_string(account_number1);
    string account_number2_str = to_string(account_number2);
    string amount_to_transfer_str = to_string(amount_to_transfer);

    string transfer0 = "UPDATE transactions SET transfer = "+amount_to_transfer_str+" WHERE account_number = "+account_number1_str+"; ";
    result = mysql_perform_query(connection, transfer0.c_str());

    string receive0 = "UPDATE TABLE transactions SET receive = "+amount_to_transfer_str+" WHERE account_number = "+account_number1_str+"; ";
    result = mysql_perform_query(connection, receive0.c_str());

    cout << "You have sent " << amount_to_transfer << " dollars and your new Balance is: ";

    string new_balance = "SELECT balance FROM accounts WHERE account_number = "+account_number1_str+"; ";
    result = mysql_perform_query(connection, new_balance.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);

        if (row) cout << row[0] << endl;

        mysql_free_result(result);
    }

    string new_transfer= "New Transfer of ";

    string log_transfer0 = "INSERT INTO NO"+account_number1_str+" VALUES (CONCAT("+new_transfer+", "+amount_to_transfer_str+"), NOW()) ";
    result = mysql_perform_query(connection, log_transfer0.c_str());

    string new_receive= "New Receive of ";

    string log_receive0 = "INSERT INTO NO"+account_number2_str+" VALUES (CONCAT("+new_receive+", "+amount_to_transfer_str+"), NOW()) ";
    result = mysql_perform_query(connection, log_receive0.c_str());
}

void Transactions :: transactions_history(MYSQL *connection, double account_number)
{
    MYSQL_RES *result;
    string account_number_str = to_string(account_number);

    string transaction_history0 = "SELECT * FROM NO"+account_number_str+"; ";
    
    result = mysql_perform_query(connection, transaction_history0.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row)
        {
            for (int i = 0; i < mysql_num_fields(result); i++) cout << row[i] << endl;  
        }

        mysql_free_result(result);
    }
}

int main(void)
{
    Bank CROSS_CONTINENTAL_TREASUTY_BANK;
    CROSS_CONTINENTAL_TREASUTY_BANK.password.reserve(1000000);  // reserving memory to hold the passwods;

    MYSQL *connection;
    MYSQL_RES *result;

    connection_details mysql_ID;
    mysql_ID.server = "localhost";
    mysql_ID.user = "root";
    mysql_ID.password = "sleyHortes1312";
    mysql_ID.database = "bankingSystemDatabase";

    connection = mysql_connection_setup(&mysql_ID);

    cout << "---------- ********** ---------- ********** ---------- ********** WELCOME TO THE CROSS-CONTINENTAL TREASURY BANK ********** ---------- ********** ---------- ********** ----------" << endl;

    cout << endl;
    cout << endl;
    cout << endl;

    cout << "                                                 *********** Among the List below, choose what best suits your Status **********" << endl;
    cout << endl;

    cout << "1. You are New to our Bank and Would like to Create an Account" << endl;

    cout << "2. You already possess an Account and would like to process some inquiries relative to it" << endl;

    cout << "3. Information on our Bank" << endl;
    
    cout << endl;

    int options, options2, options3, options4, options5;
   

    string first_name, last_name, date_of_birth, email, password, password_confirmation;
    int national_ID, phone_number, account_number;

    string create_account, transfer, check_balance, account_history, money_withdrawal;

    vector <Account> accounts;
    accounts.reserve(1000);

    cin >> options;
    switch(options)
    {
        case 1:

            cout << "Please Provide Us with the following Information in order to create your account, Make sure that you enter the correct Information: " << endl;
            cout << endl;

            cout << "First Name: ";
            cin >> first_name;

            cout << "Last Name: ";
            cin >> last_name;

            cout << "Date of Birth: ";
            cin >> date_of_birth;

            cout << "National ID: ";
            cin >> national_ID;

            cout << "Email: ";
            cin >> email;

            cout << "Phone Number: ";
            cin >> phone_number;

            cout << "Password: ";
            cin >> password;

            first_name.append(last_name);

            do
            {
                cout << "Passord Confirmation: ";
                cin >> password_confirmation;

            } while (password.compare(password_confirmation));
            
            // accounts[1].create_account(1000, first_name, last_name, results, date_birth, email, password, national_ID, address, phone_number, connection); this is commented because I don't want to execute the query yet

            // there will be much more account vectors to hold the created ones;

        break;

        case 2:

        cout << "Choose among the options below, what best suits your requirements" << endl;
        cout << endl;

        cout << "1. Checking your Balance" << endl;

        cout << "2. Deposit" << endl;

        cout << "3. Money Withdrawal " << endl;

        cout << "4. Money Transfer" << endl;

        cout << "5. Edit Account Information" << endl;

        
        cin >> options2;
        switch(options2)
        {
            case 1:
                cout << "What is yout Account Number: " << endl;
                cin >> account_number;

                cout << "What is your Password: " << endl;
                cin >> password;

                // on hold


            break;

            case 2:
            // on hold

            break;

            case 3:
            // on hold

            break;

            case 4:
            // on hold

            break;

            case 5:

            cout << "Choose among the options below, what best suits your requirements" << endl;
            cout << endl;

            cout << "1. Edit Personal Information" << endl;

            cout << "2. Change Password" << endl;

            cin >> options3;
            switch(options3)
            {
                case 1:
                cout << "Choose among the options below, what best suits your requirements: PS: The Date of Birth and National ID number can't be edited" << endl;
                cout << endl;

                cout << "1. Edit Name" << endl;

                cout << "2. Edit email" << endl;

                cout << "3. Edit Phone Number" << endl;

                cin >> options4;
                switch(options4)
                {
                    case 1:
                    // on hold

                    break;

                    case 2:
                    // on hold

                    break;

                    case 3:
                    // on hold
                    break;
                }

                break;

                case 2:
                    // on hold

                break;
            }
            break;

        }


        break;

        case 3:
            cout <<  "Title The CROSS_CONTINENTAL TREASURY BANK: A Compendium of Financial Excellence"<< endl;
    
    cout << "CHAPTER 1: INTRODUCTION TO THE CROSS-CONTINENTAL TREASURY BANK"<< endl;
    cout << "SECTION 1.1: Our Vision and Mission"<< endl;
    cout << endl;
    cout << "The CROSS-CONTINENTAL TREASURY BANK envisions a world where financial services transcend borders, empower individuals, and foster economic growth."<< endl;
    cout << "Our mission is to provide cutting-edge banking solutions with a commitment to integrity, innovation, and client satisfaction."<< endl;
    cout << "SECTION 1.2: Core Values"<< endl;
    cout << endl;
    cout << "Integrity: Upholding the highest ethical standards in all interactions."<< endl;
    cout << "Innovation: Pioneering financial solutions that adapt to the dynamic global landscape."<< endl;
    cout << "Client Satisfaction: Placing our clients at the center and ensuring their success is our success."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 2: THE PILLARS OF FINANCIAL SECURITY" << endl;
    cout << endl;
    cout << "SECTION 2.1: Robust Security Protocols"<< endl;
    cout << endl;
    cout << "Implementing state-of-the-art encryption and cybersecurity measures to safeguard client information."<< endl;
    cout << "Regular audits and assessments to fortify our defenses against emerging threats."<< endl;
    cout << "SECTION 2.2: Compliance and Regulatory Adherence"<< endl;
    cout << endl;
    cout << "Strict adherence to international banking regulations and compliance standards."<< endl;
    cout << "Continuous training and development for staff to stay abreast of regulatory changes."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 3: SERVICES TAILORED TO YOUR AMBITIONS"<< endl;
    cout << endl;
    cout << "SECTION 3.1: Personalized Banking Solutions"<< endl;
    cout << endl;
    cout << "Tailoring financial services to meet the unique needs and aspirations of our diverse clientele."<< endl;
    cout << "Offering a wide array of accounts, investment options, and lending services."<< endl;
    cout << "SECTION 3.2: Global Accessibility"<< endl;
    cout << endl;
    cout << "A network of branches and ATMs spanning continents, ensuring clients can access their funds whenever and wherever they need."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 4: EMPOWERING FINANCIAL LITERACY"<< endl;
    cout << endl;
    cout << "SECTION 4.1: Educational Initiatives"<< endl;
    cout << endl;
    cout << "The CROSS-CONTINENTAL TREASURY BANK commits to promoting financial literacy through workshops, seminars, and accessible online resources."<< endl;
    cout << "Encouraging informed decision-making and responsible financial behavior."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 5: ENVIRONMENTAL AND SOCIAL RESPONSABILITY"<< endl;
    cout << endl;
    cout << "SECTION 5.1: Sustainable Banking Practices"<< endl;
    cout << endl;
    cout << "Integrating environmental and social responsibility into our business model."<< endl;
    cout << "Investing in eco-friendly initiatives and supporting community development projects."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 6: THE FUTURE OF BANKING"<< endl;
    cout << endl;
    cout << "SECTION 6.1: Technological Advancements"<< endl;
    cout << endl;
    cout << "Embracing cutting-edge technology to enhance efficiency, security, and user experience."<< endl;
    cout << "Exploring blockchain, artificial intelligence, and other innovations to shape the future of banking."<< endl;
    cout << "SECTION 6.2: Continuous Improvement"<< endl;
    cout << endl;
    cout << "A commitment to continuous improvement through client feedback, market analysis, and strategic partnerships."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 7: CLIENT-CENTRIC APPROACH"<< endl;
    cout << endl;
    cout << "SECTION 7.1: Personalized Customer Service"<< endl;
    cout << endl;
    cout << "A dedicated customer support team ensuring a seamless and delightful banking experience."<< endl;
    cout << "Proactive resolution of issues and a commitment to exceeding client expectations."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 8: THANKS FOR CHOOSING OUR BANK"<< endl;
    cout <<  endl;
    cout << "In the ever-evolving world of finance, The CROSS-CONTINENTAL TREASURY BANK stands as a beacon of reliability, innovation, and commitment." << endl;
    cout << " This compendium serves as a testament to our dedication to providing unparalleled financial services on a global scale. " << endl;
    cout << " Join us in the pursuit of financial excellence, where your aspirations find a home in The CROSS-CONTINENTAL TREASURY BANK." << endl; 
    break;
    }
    
    

}


