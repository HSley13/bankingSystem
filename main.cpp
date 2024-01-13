#include <iostream>
#include </usr/local/mysql-connector-c++-8.2.0/include/jdbc/mysql_driver.h>
#include </usr/local/mysql-connector-c++-8.2.0/include/jdbc/mysql_connection.h>
#include </usr/local/mysql-connector-c++-8.2.0/include/jdbc/cppconn/prepared_statement.h>
#include <string>
using namespace std;

// g++ -std=c++11 -o main main.cpp -L/usr/local/mysql-connector-c++-8.2.0/lib64 -lmysqlcppconn -lssl -lcrypto -rpath /usr/local/mysql-connector-c++-8.2.0/lib64   

// THIS PORGRAM IS BANKING SYSTEM WHICH USES C++ OOP AND MYSQL DATABASE FOR THE BACKEND.
// IT OPEREATES LIKE A REAL BANKING SYSTEM WHICH HAS ALL THE FEATURES THAT YOU COULD IMAGINED.
// I  ALSO IMPLEMENT INTEREST RATES (COUMPOUND EFFECT) THAT WILL INCREASE ACCORGING TO WHEN THE MONEY HAS BEEN PLACED
// USERS ARE ABLE TO BORROW MONEY FROM THE BANK WITH AN INTEREST RATE
// I USE PREPARED STATEMENTS TO PREVENT SQL INJECTIONS AND PROTECT THE DATABASE ALONG WITH THE INFORMATION/MONEY STORED WITHIN IT
//
// 


// FOR THIS PROGRAM, THERE ARE SOME TRIGGERS THAT I HAVE CREATED ON THE COMPUTER AND ARE NOT PART OF THIS CODE.
// I HAVE PROCEEDED THAT WAY BECAUSE TRIGGERS CAN ONLY BE CREATED ONCE AND BECAUSE THAT PROGRAM WILL BE RUN MORE THAN ONCE, IT IS SAFER NOT IMBEDDING THE TRIGGERS CODE IN THE MAIN FUNCTION CAUSE IT WOULD
// TRIED TO CREATE THEM AS MANY TIME AS THE PROGRAM IS RUN AND AFTER THE FIRST TIME, YOU WOULD ALWAYS GET AN SQL ERROR SAYING THE TRIGGERS YOU ARE TRYING TO CREATE ALREADY EXIST

// THOSE TRIGGERS MUST BE ON THE COMPUTER FOR THE PROFRAM TO RUN AS EXPECTED.
// PS: WRITE ME ON A MAIL IF YOU WANT TO HAVE THEM (sleyhortes13@gamil.com) OR COMMENT IT ON THAT GIT REPOSITORY

// TODO LIST
// Passoword Hahing for a better security and Authentification for DEPOSIT, WITHDRAW AND TRANSFER
// Finish the Bank Class which will be responsible to manage the createD accounts
// Check and resolve the memory leaks problem
// Reduce code redundancy into functions
// Review the code and make it the simplest way it can be
// Interest Rate Class which will increase the balance according to when the money has been placed
// Borrow / Lend Class which will increase the borrowed / lent money to pay the Bank according the duration
// Create the GUI for a more user friendly

class connection_details
{
    public :
    string server, user, password, database;
};

sql :: Connection *connection_setup(connection_details *ID)
{
    try
    {
        sql :: mysql :: MySQL_Driver *driver;
        sql :: Connection *connection;

        driver = sql :: mysql :: get_driver_instance();
        connection = driver->connect(ID->server, ID->user, ID->password);

        connection->setSchema("bankingSystemDatabase");

        return connection;
    }
    catch (sql :: SQLException *e)
    {
        cerr << "Connection Error: " << e->what() << "Code causing Error: " << e->getErrorCode() << endl;
        return NULL;
    }
}

class Transactions
{
    public:

    void deposit(sql :: Connection *connection, double amount_to_deposit, int account_number);

    void withdrawal( sql :: Connection *connection, double sum_to_withdraw, int account_number);

    void transfer(sql :: Connection *connection, double amount_to_send, int account_number1, int account_number2);

    void transactions_history(sql :: Connection *connection, int account_number); 
};

void Transactions :: deposit (sql :: Connection *connection, double amount_to_deposit, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("UPDATE transactions SET deposit = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_deposit);
    prep_statement->setInt(2, account_number);

    prep_statement->executeQuery();

    cout << "You have deposit " << amount_to_deposit << " dollars and your new Balance is: ";

    prep_statement = connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if (result->next()) cout << result->getDouble("balance") << endl;

    prep_statement = connection->prepareStatement("INSERT INTO CONCAT(NO, ?) VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setInt(1, account_number);
    prep_statement->setString(2, "New deposit of ");
    prep_statement->setDouble(3, amount_to_deposit);

    prep_statement->executeUpdate();
    
    delete prep_statement;
    delete result;
}

void Transactions :: withdrawal(sql :: Connection *connection, double amount_to_withdraw, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("UPDATE transactions SET withdraw = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_withdraw);
    prep_statement->setInt(2, account_number);

    prep_statement->executeQuery();

    cout << "You have withdrawn " << amount_to_withdraw << " dollars and your new Balance is: ";

    prep_statement = connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if (result->next()) cout << result->getDouble("balance") << endl;

    prep_statement = connection->prepareStatement("INSERT INTO CONCAT(NO, ?) VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setInt(1, account_number);
    prep_statement->setString(2, "New Withdrawal of ");
    prep_statement->setDouble(3, amount_to_withdraw);

    prep_statement->executeUpdate();
    
    delete prep_statement;
    delete result;
}

void Transactions :: transfer(sql :: Connection *connection, double amount_to_transfer,int account_number1, int account_number2)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("UPDATE transactions SET transfer = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_transfer);
    prep_statement->setInt(2, account_number1);

    prep_statement->executeUpdate();

    cout << "You have sent " << amount_to_transfer << " dollars to the account_number :" << account_number2 << " and Your new balance is: ";

    prep_statement = connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number1);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if (result->next()) cout << result->getDouble("balance") << endl;

    prep_statement = connection->prepareStatement("UPDATE transactions SET receive = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_transfer);
    prep_statement->setInt(2, account_number2);

    prep_statement->executeUpdate();

    prep_statement = connection->prepareStatement("INSERT INTO CONCAT(NO, ?) VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setString(1, "New Transfer of ");
    prep_statement->setDouble(2, amount_to_transfer);

    prep_statement->executeUpdate();

    string new_receive= "New Receive of ";

    prep_statement = connection->prepareStatement("INSERT INTO CONCAT(NO, ?) VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setString(1, "New Receive of ");
    prep_statement->setDouble(2, amount_to_transfer);

    delete prep_statement;
    delete result;
}

void Transactions :: transactions_history(sql :: Connection *connection, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT * FROM CONCAT(NO, ?);");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();

    while (result->next())
    {
        cout << "transaction_details: " << result->getString("transaction_details") << "  Date & Time: " << result->getString("date_time") << endl;
    }
    
   delete prep_statement;
   delete result;
}

class Account : public Transactions
{
    public:

    string first_name, last_name, date_birth, email, password, national_ID, address;
    int phone_number;

    void create_account(int account_number, string national_ID, string first_name, string last_name, string date_birth, int phone_number, string email, string address, double balance, string password, sql :: Connection *connection);

    void check_balance(sql :: Connection *connection, int account_number);

    void remove_accounts(sql :: Connection *connection, int account_number);
};

void Account :: create_account(int account_number, string national_ID, string first_name, string last_name, string date_birth, int phone_number, string email, string address, double balance, string password, sql :: Connection *connection)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("INSERT INTO accounts (national_ID, first_name, last_name, date_birth, phone_number, email, address, balance) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    prep_statement->setString(1, national_ID);
    prep_statement->setString(2, first_name);
    prep_statement->setString(3, last_name);
    prep_statement->setString(4, date_birth);
    prep_statement->setInt(5, phone_number);
    prep_statement->setString(6, email);
    prep_statement->setString(7, address);
    prep_statement->setDouble(8, balance);

    prep_statement->executeUpdate();

    cout << "This is Your Account Number, remember it cause you will need it to gain access to everything you want to do in the future:  ";

    prep_statement = connection->prepareStatement("SELECT account_number FROM accounts WHERE national_ID = ?");
    prep_statement->setString(1, national_ID);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if(result->next())
    {
        account_number = result->getInt("account_number");

        cout << account_number << endl;

        string table_name = "NO";
        table_name.append(to_string(account_number));

        prep_statement = connection->prepareStatement("CREATE TABLE "+table_name+" (transaction_details VARCHAR(100), date_time DATETIME DEFAULT NOW() );");

        prep_statement->executeUpdate();

        prep_statement = connection->prepareStatement("INSERT INTO "+table_name+" (transaction_details) VALUES (?);");
        prep_statement->setString(1, "Account Created");

        prep_statement->executeUpdate();
    }

    delete prep_statement;
    delete result;
}

void Account :: check_balance(sql :: Connection *connection, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if (result->next()) cout << "Your Current Balance is: " << result->getDouble("balance") << endl;

    delete prep_statement;
    delete result;
}

void Account :: remove_accounts(sql :: Connection *connection, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("DELETE FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    prep_statement->executeUpdate();

    delete prep_statement;
}

class BANK : public Account
{   
    public :
    vector <Account> created_accounts;

};

int main(void)
{
    try
    {
        connection_details ID;
        ID.server= "localhost";
        ID.user = "root";
        ID.password = "sleyHortes1312";

        sql :: Connection *connection = connection_setup(&ID);
    
        sql :: PreparedStatement *pre_statement;
        sql :: ResultSet *result;

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
    
        string first_name, last_name, date_birth, email, national_ID, address, password, password_confirmation;

        int phone_number, account_number;

        double balance, amount_to_deposit, amount_to_withdraw, amount_to_transfer;

        Account accounts;

        cin >> options;
        switch(options)
        {
            case 1: // Create the Account and the table History along the way

                cout << "Please Provide Us with the following Information in order to create your account, Make sure that you enter the correct Information: " << endl;
                cout << endl;
                
                cout << "National ID with at least a letter: ";
                cin >> national_ID;

                cout << "First Name: ";
                cin >> first_name;

                cout << "Last Name: ";
                cin >> last_name;

                cout << "Date of Birth with format 2024-01-31 : ";
                cin >> date_birth;

                cout << "Phone Number: ";
                cin >> phone_number;
               
                cout << "Email: ";
                cin >> email;

                cout << "address with format Taiwan-shoufeng-zhixue-Dong_Hwa_University-Dormitory_4: ";
                cin >> address;

                do
                {
                    cout << "100 dollars FEE in order to create the account. PS: This amount won't appeat in your balance: ";
                    cin >> balance;
                } while (balance < 100);
                
                cout << "Password: ";
                cin >> password;

                do
                {
                    cout << "Passord Confirmation: ";
                    cin >> password_confirmation;

                } while (password.compare(password_confirmation));

                // Use Argon2_ID to hash the passwrord and store the hashed result into a table where only the BANK class will have access to it

                accounts.create_account(account_number, national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, password, connection);

                password.clear();
                password_confirmation.clear();

            break;

            case 2: // Perform Inquiry Relative to an Existing Account 

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
                    case 1: // Balance Check
                        cout << "What is yout Account Number: " << endl;
                        cin >> account_number;

                        cout << "What is your Password: " << endl;
                        cin >> password;

                        // I should inplement using ARGON2_ID the code which will make sure that the user passwors authentification is correct before procceding to the balance chack query

                        accounts.check_balance(connection, account_number);

                    break;

                    case 2: // Deposit
                        cout << "What is yout Account Number: " << endl;
                        cin >> account_number;

                        cout << "What is your Password: " << endl;
                        cin >> password;

                        cout << "What is the Amount you would like to deposit: " << endl;
                        cin >> amount_to_deposit;
                    
                        // I should inplement using ARGON2_ID the code which will make sure that the user passwors authentification is correct before procceding to the balance chack query

                        accounts.deposit(connection, amount_to_deposit, account_number);

                    break;

                    case 3: // Withdraw
                        cout << "What is yout Account Number: " << endl;
                        cin >> account_number;

                        cout << "What is your Password: " << endl;
                        cin >> password;

                        cout << "What is the Amount you would like to Withdraw: " << endl;
                        cin >> amount_to_withdraw;
                    
                        // I should inplement using ARGON2_ID the code which will make sure that the user passwors authentification is correct before procceding to the balance chack query

                        // Also Check if the user's Balance >= to the amounts he/she wants to Withdraw

                        // I should also return the exact Balance so that the user might be aware of it and enter a reasonnable amount

                        accounts.withdrawal(connection, amount_to_withdraw, account_number);

                    break;

                    case 4: // Transfer
                        cout << "What is yout Account Number: " << endl;
                        cin >> account_number;

                        cout << "What is your Password: " << endl;
                        cin >> password;

                        cout << "What is the Amount you would like to Transfer: " << endl;
                        cin >> amount_to_deposit;
                    
                        // I should inplement using ARGON2_ID the code which will make sure that the user passwors authentification is correct before procceding to the balance chack query

                        // Also Check if the user's Balance >= to the amounts he/she wants to Transfer

                        // I should also return the exact Balance so that the user might be aware of it and enter a reasonnable amount

                        accounts.deposit(connection, amount_to_deposit, account_number);

                    break;

                    case 5: // Edit Account Information
                        cout << "Choose among the options below, what best suits your requirements" << endl;
                        cout << endl;

                        cout << "1. Edit Personal Information" << endl;

                        cout << "2. Change Password" << endl;

                    cin >> options3;
                    switch(options3) 
                    {
                        case 1: // Edit personal Information
                            cout << "Choose among the options below, what best suits your requirements: PS: The Date of Birth and National ID number can't be edited" << endl;
                            cout << endl;

                            cout << "1. Edit Name" << endl;

                            cout << "2. Edit email" << endl;

                            cout << "3. Edit Phone Number" << endl;

                            cin >> options4;
                            switch(options4)
                            {
                                case 1: // Edit Name
                                // on hold

                                break;

                                case 2: // Edit Email
                                // on hold

                                break;

                                case 3: // Edit Number
                                // on hold

                                break;
                            }

                        break;

                        case 2: // Change Password
                            // on hold

                        break;
                    }

                    break;
                }   


            break;

            case 3: // Information on the Bank
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

        connection->close();
        delete connection;
        delete pre_statement;
        delete result;
    }

    catch (sql :: SQLException *e)
    {
        cerr << "Connection Error: " << e->what() << "Code causing Error: " << e->getErrorCode() << endl;
    }

    catch (const std :: exception *e)
    {
            cout << "Error: " << e->what() << endl;
    }
}