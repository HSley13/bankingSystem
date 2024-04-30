#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <database.h> // library that I have created which is used for both the Banking System main.cpp and the GUI Implementation. THIS LIBRARY IS LOCATED IN GUI/src/database

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

int main(int argc, const char **argv)
{
    try
    {
        if (argc < 5)
        {
            std::cerr << " ERROR !!!!The Information required to connect to the Database should be entered as command line arguments : Server-- > argv[1], Port-- > argv[2], UserName-- > argv[3], Schema-- > argv[4], Password-- > argv[5] " << std::endl;

            return 1;
        }

        connection_details ID;
        ID.server = argv[1];
        ID.port = std::stoi(argv[2]);
        ID.user = argv[3];
        ID.schema = argv[4];
        ID.password = argv[5];

        sql::Connection *connection = connection_setup(&ID);
        if (!connection)
        {
            std::cerr << "Failed to establish the Database connection." << std::endl;

            return 1;
        }

        int adm_options, options, options1, options2, options3, options4;

        std::string first_name, last_name, new_first_name, new_first_name_confirmation, date_birth, email, new_email, new_email_confirmation, national_ID, address, new_address, new_address_confirmation, password, password_confirmation, new_password, new_password_confirmation, hash_password, new_hash_password, initial_timestamp, date, confirmation, confirmation_answer, question, answer, confirm_answer;

        int phone_number, new_phone_number, new_phone_number_confirmation, account_number, account_number1, account_number2, k = 3, choice;

        double balance, amount_to_deposit, amount_to_withdraw, amount_to_transfer, interest_rate, amount_to_borrow, amount_to_return, borrowal_interest_rate;

        std::stack<int> main_menu;

        std::stack<int> sub_menu;

        do
        {
            std::cout << "---------- ********** ---------- ********** ---------- ********** WELCOME TO THE CROSS-CONTINENTAL TREASURY BANK ********** ---------- ********** ---------- ********** ----------" << std::endl;

            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;

            std::cout << "                                                 *********** Among the List below, choose what best suits You **********" << std::endl;
            std::cout << std::endl;

            std::cout << "1. Administrator" << std::endl;

            std::cout << "2. Regular Client" << std::endl;

            std::cout << "0. QUIT" << std::endl;

            std::cout << std::endl;

            std::cin >> options;
            std::cout << std::endl;

            if (options)
                sub_menu.push(options);

            switch (options)
            {
            case 1: // Administrator
                if (!BANK::adm_authentification_message(connection, account_number, hash_password))
                    break;

                do
                {
                    std::cout << "What is your Password: " << std::endl;
                    std::cout << "You have 3 Chances" << std::endl;

                    std::cin >> password;
                    std::cout << std::endl;

                    if (BANK::verifying_password(password, hash_password))
                    {
                        do
                        {
                            std::cout << "1. Create an Administrator" << std::endl;

                            std::cout << "2. Display The Accounts Table" << std::endl;

                            std::cout << "3. Display Specific Account through the Account_number" << std::endl;

                            std::cout << "4. Display People in Debt " << std::endl;

                            std::cout << "5. Display Specific Person in Debt through the Account_number" << std::endl;

                            std::cout << "6. Display all Transactions History for an Account_number" << std::endl;

                            std::cout << "7. Display All Transaction History Relative to a Specific Date for an Account_Number" << std::endl;

                            std::cout << "8. Remove Accounts through the Account_number" << std::endl;

                            std::cout << "0. Return to the Previous Menu" << std::endl;
                            std::cout << std::endl;

                            std::cin >> adm_options;
                            std::cout << std::endl;

                            if (adm_options)
                                sub_menu.push(adm_options);

                            if (!adm_options && sub_menu.empty())
                            {
                                adm_options = sub_menu.top();
                                sub_menu.pop();

                                break;
                            }

                            switch (adm_options)
                            {
                            case 1: // Create an Administrator
                                std::cout << "Enter your Desired Account Number: ";
                                std::cin >> account_number;
                                std::cout << std::endl;

                                std::cout << "Enter Your Password: ";
                                std::cin >> password;
                                std::cout << std::endl;

                                do
                                {
                                    std::cout << "Password Confirmation: ";
                                    std::cin >> password_confirmation;
                                    std::cout << std::endl;

                                } while (password.compare(password_confirmation));

                                hash_password = BANK::hashing_password(password);

                                BANK::create_adm(connection, account_number, hash_password);

                                std::cout << "ADM account " << account_number << " created successfully" << std::endl;
                                std::cout << std::endl;

                                password.clear();
                                password_confirmation.clear();

                                break;

                            case 2: // Display Accounts Table

                                BANK::display_accounts_table(connection);

                                break;

                            case 3: // Display Specific Account
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                BANK::display_specific_accounts(connection, account_number);

                                break;

                            case 4: // Display People in Debt
                                BANK::display_people_in_debt(connection);

                                break;

                            case 5: // Display Specific Accounts in Debt
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                BANK::display_specific_accounts_in_debt(connection, account_number);

                                break;

                            case 6: // Display Transactions History
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                BANK::display_transactions_history(connection, account_number);

                                break;

                            case 7: // Display Transactions History Relative to s specific date
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                std::cout << "Enter the relative Date ( 2024-01-31 ): " << std::endl;
                                std::cin >> date;
                                std::cout << std::endl;

                                std::cout << "0. Display All Transactions occurred BEFORE the selected Date" << std::endl;
                                std::cout << "1. Display All Transactions occurred AFTER the selected Date" << std::endl;
                                std::cout << "2. Display All Transactions occurred ONLY ON the selected Date" << std::endl;
                                std::cout << std::endl;

                                std::cin >> choice;
                                std::cout << std::endl;

                                if (choice != 0 && choice != 1 && choice != 2)
                                {
                                    std::cout << "INVALID ENTER OPTION, PLEASE CHOOSE BETWEEN 1, 2 AND 3" << std::endl;

                                    break;
                                }

                                BANK::display_specific_transactions_history(connection, account_number, date, choice);

                                break;

                            case 8: // Remove Account
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                BANK::remove_accounts(connection, account_number);

                                break;
                            }

                        } while (adm_options);

                        break;
                    }

                    password.clear();

                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                    k--;

                } while (k);

                k = 3;

                break;

            case 2: // Regular client
                do
                {
                    std::cout << "1. New to our Bank and Would like to Create an Account" << std::endl;

                    std::cout << "2. Already possess an Account and would like to process some inquiries relative to it" << std::endl;

                    std::cout << "3. Information on our Bank" << std::endl;

                    std::cout << "0. Return to the Previous Menu" << std::endl;

                    std::cout << std::endl;

                    std::cin >> options1;

                    if (options1)
                        sub_menu.push(options1);

                    if (!options1 && !sub_menu.empty())
                    {
                        options1 = sub_menu.top();
                        sub_menu.pop();

                        break;
                    }

                    switch (options1)
                    {
                    case 1: // Create the Account and the table History along the way

                        std::cout << "Please Provide Us with the following Information in order to create your account. Make sure You enter the Correct Information: " << std::endl;
                        std::cout << std::endl;

                        std::cout << "National ID with at least a letter within it: ";
                        std::cin >> national_ID;
                        std::cout << std::endl;

                        std::cout << "First Name: ";
                        std::cin >> first_name;
                        std::cout << std::endl;

                        std::cout << "Last Name: ";
                        std::cin >> last_name;
                        std::cout << std::endl;

                        std::cout << "Date of Birth ( 2024-01-31 ) : ";
                        std::cin >> date_birth;
                        std::cout << std::endl;

                        std::cout << "Phone Number: ";
                        std::cin >> phone_number;
                        std::cout << std::endl;

                        std::cout << "Email: ";
                        std::cin >> email;
                        std::cout << std::endl;

                        std::cout << "Address ( Taiwan-Taipei_City-Datong_District-Zhongshan_Road-001 ): ";
                        std::cin >> address;
                        std::cout << std::endl;

                        do
                        {
                            std::cout << "Your Account should have at least 100 when creating it, so Please enter those 100 dollars and not less: " << std::endl;
                            std::cout << std::endl;

                            std::cout << "Interest Rate Scale according to your First Deposit which can't be changed: " << std::endl;
                            std::cout << std::endl;

                            std::cout << "1. Balance = 100 ---> Interest Rate = 0" << std::endl;

                            std::cout << "2. 500 > Balance > 100 ---> Interest Rate = 2%" << std::endl;

                            std::cout << "3. 1000 > Balance >= 500 ---> Interest Rate = 5%" << std::endl;

                            std::cout << "4. Balance > 1000 ---> Interest Rate = 7%" << std::endl;

                            std::cin >> balance;

                        } while (balance < 100);

                        if (balance == 100)
                            interest_rate = 0;

                        else if (balance > 100 && balance < 500)
                            interest_rate = 0.02;

                        else if (balance < 1000 && balance >= 500)
                            interest_rate = 0.05;

                        else
                            interest_rate = 0.07;

                        std::cout << "Password: ";
                        std::cin >> password;
                        std::cout << std::endl;

                        do
                        {
                            std::cout << "Password Confirmation: ";
                            std::cin >> password_confirmation;
                            std::cout << std::endl;

                        } while (password.compare(password_confirmation));

                        std::cin.ignore();

                        std::cout << "Ask Yourself question which is gonna be used for Password Recovery: ";
                        std::getline(std::cin, question);
                        std::cout << std::endl;

                        std::cout << "Answer to the Question: ";
                        std::getline(std::cin, answer);
                        std::cout << std::endl;

                        do
                        {
                            std::cout << "Confirm Answer: ";
                            std::getline(std::cin, confirm_answer);
                            std::cout << std::endl;

                        } while (answer.compare(confirm_answer));

                        std::cout << "National ID: " << national_ID << std::endl;
                        std::cout << "First Name: " << first_name << std::endl;
                        std::cout << "Last Name: " << last_name << std::endl;
                        std::cout << "Date Of Birth: " << date_birth << std::endl;
                        std::cout << "Phone Number: " << phone_number << std::endl;
                        std::cout << "Email: " << email << std::endl;
                        std::cout << "Address: " << address << std::endl;
                        std::cout << "Balance: " << balance << std::endl;
                        std::cout << "Question: " << question << std::endl;
                        std::cout << "Answer: " << answer << std::endl;
                        std::cout << std::endl;

                        std::cout << "Type YES to confirm and NO to cancel: ";
                        std::cin >> confirmation_answer;

                        if (confirmation_answer != "YES")
                        {
                            std::cout << "Cancelling..." << std::endl;
                            std::cout << std::endl;

                            break;
                        }

                        hash_password = BANK::hashing_password(password);
                        Account::create_account(connection, account_number, national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, interest_rate, hash_password, question, answer);

                        password.clear();
                        password_confirmation.clear();

                        break;

                    case 2: // Perform Inquiry Relative to an Existing Account
                        do
                        {
                            std::cout << "Choose among the Options below, what best suits You" << std::endl;
                            std::cout << std::endl;

                            std::cout << "1. Check your Balance" << std::endl;

                            std::cout << "2. Deposit" << std::endl;

                            std::cout << "3. Money Withdrawal " << std::endl;

                            std::cout << "4. Money Transfer" << std::endl;

                            std::cout << "5. Borrow Money" << std::endl;

                            std::cout << "6. Return Borrowed Money" << std::endl;

                            std::cout << "7. Edit Account Information / Forget Password" << std::endl;

                            std::cout << "8. Transaction History" << std::endl;

                            std::cout << "9. Specific Transaction History" << std::endl;

                            std::cout << "10. Delete Account" << std::endl;

                            std::cout << "0. Return to the Previous Menu" << std::endl;

                            std::cout << std::endl;

                            std::cin >> options2;
                            std::cout << std::endl;

                            if (options2)
                                main_menu.push(options2);

                            if (!options2 && !sub_menu.empty())
                            {
                                options2 = sub_menu.top();
                                sub_menu.pop();

                                break;
                            }

                            switch (options2)
                            {
                            case 1: // Balance Check
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        BANK::apply_interest_rate_to_balance(connection, account_number);

                                        std::cout << "Your Current Balance is: " << check_balance(connection, account_number) << std::endl;
                                        std::cout << std::endl;

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 2: // Deposit
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                std::cout << "Enter Amount to Deposit: ";
                                std::cin >> amount_to_deposit;
                                std::cout << std::endl;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        BANK::apply_interest_rate_to_balance(connection, account_number);

                                        Transactions::deposit(connection, amount_to_deposit, account_number);

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 3: // Withdraw
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                std::cout << "Enter Amount to Withdraw: ";
                                std::cin >> amount_to_withdraw;
                                std::cout << std::endl;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        BANK::apply_interest_rate_to_balance(connection, account_number);

                                        balance = check_balance(connection, account_number);

                                        while (amount_to_withdraw > balance)
                                        {
                                            std::cout << "Balance is: " << balance << " which is less than the Amount to Withdraw" << std::endl;

                                            std::cout << "So Please Enter a reasonable Amount: ";
                                            std::cin >> amount_to_withdraw;
                                            std::cout << std::endl;
                                        }

                                        Transactions::withdrawal(connection, amount_to_withdraw, account_number);

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 4: // Transfer
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                std::cout << "Enter Amount to Transfer: ";
                                std::cin >> amount_to_transfer;
                                std::cout << std::endl;

                                std::cout << "Enter the Account Number to receive the Money: ";
                                std::cin >> account_number2;
                                std::cout << std::endl;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        BANK::apply_interest_rate_to_balance(connection, account_number1);

                                        balance = check_balance(connection, account_number1);

                                        while (amount_to_transfer > balance)
                                        {
                                            std::cout << "Your balance is: " << balance << " which is less than the Amount to Transfer" << std::endl;

                                            std::cout << "So Please enter a reasonable Amount: ";
                                            std::cin >> amount_to_transfer;
                                            std::cout << std::endl;
                                        }

                                        BANK::apply_interest_rate_to_balance(connection, account_number2);

                                        Transactions::transfer(connection, amount_to_transfer, account_number1, account_number2);

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 5: // Borrow Money
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                std::cout << "No one is allowed to borrow Money if currently owes the Bank any amount otherwise She/He will be logged out of the System Completely. Please go to the Previous Menu and Pay what is owed before asking for any New Borrowal" << std::endl;
                                std::cout << std::endl;

                                std::cout << "Interest Rate Scale on Borrowed Amount " << std::endl;
                                std::cout << std::endl;

                                std::cout << "1. Borrowed Amount = 100 ---> Interest Rate = 0.1%. PS: TO BE RETURN WITHIN 1 DAY OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << std::endl;

                                std::cout << "2. 500 > Borrowed Amount > 100 ---> Interest Rate = 5% PS: TO BE RETURN WITHIN 2 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << std::endl;

                                std::cout << "3. 1000 > Borrowed Amount >= 500 ---> Interest Rate = 7% PS: TO BE RETURN WITHIN 3 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << std::endl;

                                std::cout << "4. Borrowed Amount > 1000 ---> Interest Rate = 10% PS: TO BE RETURN WITHIN 4 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << std::endl;
                                std::cout << std::endl;

                                std::cout << "Enter Amount to Borrow: " << std::endl;
                                std::cin >> amount_to_borrow;
                                std::cout << std::endl;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {

                                        if (amount_to_borrow == 100)
                                            borrowal_interest_rate = 0.001;

                                        else if (amount_to_borrow > 100 && amount_to_borrow < 500)
                                            borrowal_interest_rate = 0.05;

                                        else if (amount_to_borrow < 1000 && amount_to_borrow >= 500)
                                            borrowal_interest_rate = 0.07;

                                        else
                                            borrowal_interest_rate = 0.1;

                                        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
                                        prep_statement->setInt(1, account_number);

                                        std::unique_ptr<sql::ResultSet> result(prep_statement->executeQuery());

                                        if (result->next())
                                        {
                                            std::cout << "Aren't allowed to borrow Cause this account owes the Bank. First Pay the Debt and then the  borrowal will be possible" << std::endl;

                                            break;
                                        }

                                        Transactions::insert_borrowal(connection, account_number, amount_to_borrow, borrowal_interest_rate);

                                        prep_statement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement("INSERT INTO event_schedule (account_number, scheduled_time) VALUES (?, CURRENT_TIMESTAMP + INTERVAL 96 HOUR);"));
                                        prep_statement->setInt(1, account_number);

                                        prep_statement->executeUpdate();

                                        Transactions::borrow(connection, amount_to_borrow, account_number);

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 6: // Return Borrowed Money
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        std::unique_ptr<sql::PreparedStatement> prep_statement_call_update(connection->prepareStatement("CALL update_borrowed_money(?);"));
                                        prep_statement_call_update->setInt(1, account_number);

                                        prep_statement_call_update->executeUpdate();

                                        std::cout << "The Amount ought to be returned is: ";
                                        std::unique_ptr<sql::PreparedStatement> prep_statement_select_borrowal(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
                                        prep_statement_select_borrowal->setInt(1, account_number);

                                        std::unique_ptr<sql::ResultSet> result(prep_statement_select_borrowal->executeQuery());

                                        double due_returned;
                                        if (result->next())
                                        {
                                            due_returned = result->getDouble("borrowed_amount");
                                            std::cout << due_returned << std::endl;
                                        }

                                        std::cout << "Enter Exact Amount to be returned and not less" << std::endl;
                                        std::cin >> amount_to_return;
                                        std::cout << std::endl;

                                        while (due_returned != amount_to_return)
                                        {
                                            std::cout << "Entered Amount is less than the one to be returned, Enter it again" << std::endl;
                                            std::cin >> amount_to_return;
                                            std::cout << std::endl;
                                        }

                                        std::cout << "Thanks, You have officially paid your debt and are now allowed to make another one" << std::endl;
                                        std::cout << std::endl;

                                        std::unique_ptr<sql::PreparedStatement> prep_statement_delete_borrowal(connection->prepareStatement("DELETE FROM borrowal_record WHERE account_number = ?;"));
                                        prep_statement_delete_borrowal->setInt(1, account_number);

                                        prep_statement_delete_borrowal->executeUpdate();

                                        std::unique_ptr<sql::PreparedStatement> prep_statement_delete_event(connection->prepareStatement("DELETE FROM event_schedule WHERE account_number = ?;"));
                                        prep_statement_delete_event->setInt(1, account_number);

                                        prep_statement_delete_event->executeUpdate();

                                        Transactions::insert_transactions(connection, account_number, "New Money Returned, Sum of ", amount_to_return);

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 7: // Edit Account Information
                                do
                                {
                                    std::cout << "Choose among the options below, what best suits your requirements" << std::endl;
                                    std::cout << std::endl;

                                    std::cout << "1. Edit Personal Information" << std::endl;

                                    std::cout << "2. Change Password" << std::endl;

                                    std::cout << "3. Forget Password" << std::endl;

                                    std::cout << "0. Return to the Previous Menu" << std::endl;

                                    std::cout << std::endl;

                                    std::cin >> options3;
                                    std::cout << std::endl;

                                    if (options3)
                                        main_menu.push(options3);

                                    if (!options3 && !sub_menu.empty())
                                    {
                                        options3 = sub_menu.top();
                                        sub_menu.pop();

                                        break;
                                    }

                                    switch (options3)
                                    {
                                    case 1: // Edit personal Information
                                        do
                                        {
                                            std::cout << "Choose among the options below, what best suits your requirements: PS: The Date of Birth and National ID number can't be edited" << std::endl;
                                            std::cout << std::endl;

                                            std::cout << "1. Edit Name" << std::endl;

                                            std::cout << "2. Edit email" << std::endl;

                                            std::cout << "3. Edit address" << std::endl;

                                            std::cout << "4. Edit Phone Number" << std::endl;

                                            std::cout << "0. Return to the Previous Menu" << std::endl;

                                            std::cout << std::endl;

                                            std::cin >> options4;
                                            std::cout << std::endl;

                                            if (options4)
                                                main_menu.push(options4);

                                            if (!options4 && !sub_menu.empty())
                                            {
                                                options4 = sub_menu.top();
                                                sub_menu.pop();

                                                break;
                                            }

                                            switch (options4)
                                            {
                                            case 1: // Edit Name
                                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                                    break;

                                                do
                                                {
                                                    std::cout << "What is your Password: " << std::endl;
                                                    std::cout << "You have 3 Chances" << std::endl;

                                                    std::cin >> password;
                                                    std::cout << std::endl;

                                                    if (BANK::verifying_password(password, hash_password))
                                                    {
                                                        std::cout << "Enter the New First Name. PS: Last Name can't be changed: ";
                                                        std::cin >> new_first_name;
                                                        std::cout << std::endl;

                                                        while (new_first_name.empty())
                                                        {
                                                            std::cout << "Name left Blank, Enter a valid One" << std::endl;
                                                            std::cin >> new_first_name;

                                                            std::cout << std::endl;
                                                        }

                                                        std::cout << "Enter the New First Name Confirmation: ";
                                                        std::cin >> new_first_name_confirmation;
                                                        std::cout << std::endl;

                                                        while (new_first_name.compare(new_first_name_confirmation))
                                                        {
                                                            std::cout << "New first Name Confirmation Incorrect" << std::endl;
                                                            std::cin >> new_first_name_confirmation;

                                                            std::cout << std::endl;
                                                        }

                                                        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_name(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setString(2, new_first_name);

                                                        prep_statement->executeUpdate();

                                                        std::cout << "Name changed Successfully" << std::endl;
                                                        std::cout << std::endl;

                                                        password.clear();

                                                        break;
                                                    }

                                                    password.clear();

                                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;

                                            case 2: // Edit Email
                                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                                    break;

                                                do
                                                {
                                                    std::cout << "What is your Password: " << std::endl;
                                                    std::cout << "You have 3 Chances" << std::endl;

                                                    std::cin >> password;
                                                    std::cout << std::endl;

                                                    if (BANK::verifying_password(password, hash_password))
                                                    {
                                                        std::cout << "Enter the New Mail: ";
                                                        std::cin >> new_email;

                                                        while (new_email.empty())
                                                        {
                                                            std::cout << "Email left Blank, Enter a valid One" << std::endl;
                                                            std::cin >> new_email;

                                                            std::cout << std::endl;
                                                        }

                                                        std::cout << "Enter the New Email Confirmation: ";
                                                        std::cin >> new_email_confirmation;
                                                        std::cout << std::endl;

                                                        while (new_email.compare(new_email_confirmation))
                                                        {
                                                            std::cout << "New Email Confirmation Incorrect" << std::endl;
                                                            std::cin >> new_email_confirmation;

                                                            std::cout << std::endl;
                                                        }

                                                        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_email(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setString(2, new_email);

                                                        prep_statement->executeUpdate();

                                                        std::cout << "Email changed Successfully" << std::endl;
                                                        std::cout << std::endl;

                                                        password.clear();

                                                        break;
                                                    }

                                                    password.clear();

                                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;

                                            case 3: // Edit address
                                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                                    break;

                                                do
                                                {
                                                    std::cout << "What is your Password: " << std::endl;
                                                    std::cout << "You have 3 Chances" << std::endl;

                                                    std::cin >> password;
                                                    std::cout << std::endl;

                                                    if (BANK::verifying_password(password, hash_password))
                                                    {
                                                        std::cout << "Enter the New Address: ";
                                                        std::cin >> new_address;
                                                        std::cout << std::endl;

                                                        while (new_address.empty())
                                                        {
                                                            std::cout << "Address left Blank, Enter a valid One" << std::endl;
                                                            std::cin >> new_address;

                                                            std::cout << std::endl;
                                                        }

                                                        std::cout << "Enter the New Address Confirmation: ";
                                                        std::cin >> new_address_confirmation;
                                                        std::cout << std::endl;

                                                        while (new_address.compare(new_address_confirmation))
                                                        {
                                                            std::cout << "New Address Confirmation Incorrect" << std::endl;
                                                            std::cin >> new_address_confirmation;

                                                            std::cout << std::endl;
                                                        }

                                                        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_address(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setString(2, new_address);

                                                        prep_statement->executeUpdate();

                                                        password.clear();

                                                        std::cout << "Address changed Successfully" << std::endl;
                                                        std::cout << std::endl;

                                                        break;
                                                    }

                                                    password.clear();

                                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;

                                            case 4: // Edit Phone Number
                                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                                    break;

                                                do
                                                {
                                                    std::cout << "What is your Password: " << std::endl;
                                                    std::cout << "You have 3 Chances" << std::endl;

                                                    std::cin >> password;
                                                    std::cout << std::endl;

                                                    if (BANK::verifying_password(password, hash_password))
                                                    {
                                                        std::cout << "Enter the New Phone Number: ";
                                                        std::cin >> new_phone_number;
                                                        std::cout << std::endl;

                                                        while (Account::are_all_same(new_phone_number))
                                                        {
                                                            std::cerr << "Enter a valid Phone number where all the digits aren't the same" << std::endl;
                                                            std::cin >> new_phone_number;

                                                            std::cout << std::endl;
                                                        }

                                                        std::cout << "Enter the New Phone Number Confirmation: ";
                                                        std::cin >> new_phone_number_confirmation;
                                                        std::cout << std::endl;

                                                        while (new_phone_number != new_phone_number_confirmation)
                                                        {
                                                            std::cout << "New Address Confirmation Incorrect" << std::endl;
                                                            std::cin >> new_phone_number_confirmation;

                                                            std::cout << std::endl;
                                                        }

                                                        std::unique_ptr<sql::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_phone_number(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setInt(2, new_phone_number);

                                                        prep_statement->executeUpdate();

                                                        std::cout << "Phone Number changed Successfully" << std::endl;
                                                        std::cout << std::endl;

                                                        password.clear();

                                                        break;
                                                    }

                                                    password.clear();

                                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;
                                            }

                                        } while (options4);

                                        break;

                                    case 2: // Change Password
                                        if (!BANK::authentification_message(connection, account_number, hash_password))
                                            break;

                                        do
                                        {
                                            std::cout << "What is your Password: " << std::endl;
                                            std::cout << "You have 3 Chances" << std::endl;

                                            std::cin >> password;
                                            std::cout << std::endl;

                                            if (BANK::verifying_password(password, hash_password))
                                            {
                                                std::cout << "Enter the New Password: ";
                                                std::cin >> new_password;
                                                std::cout << std::endl;

                                                do
                                                {
                                                    std::cout << "Enter the New Password Confirmation: ";
                                                    std::cin >> new_password_confirmation;
                                                    std::cout << std::endl;

                                                } while (new_password.compare(new_password_confirmation));

                                                new_hash_password = BANK::hashing_password(new_password);

                                                call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

                                                std::cout << "Password changed Successfully" << std::endl;
                                                std::cout << std::endl;

                                                password.clear();
                                                new_password.clear();

                                                break;
                                            }

                                            password.clear();

                                            std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                            k--;

                                        } while (k);

                                        k = 3;

                                        break;

                                    case 3: // Forget Password
                                        std::cout << "Enter Account Number: " << std::endl;
                                        std::cin >> account_number;
                                        std::cout << std::endl;

                                        std::cout << "In order to Change the password, We shall make sure that One is Who is proclaims to be, so Please Provide us the answer of the following question for the Authentification Process" << std::endl;
                                        std::cout << std::endl;

                                        if (BANK::authentification_check(connection, account_number, question, answer))
                                        {
                                            std::cout << "Enter New Password: ";
                                            std::cin >> new_password;
                                            std::cout << std::endl;

                                            do
                                            {
                                                std::cout << "Enter New Password Confirmation: ";
                                                std::cin >> new_password_confirmation;
                                                std::cout << std::endl;

                                            } while (new_password.compare(new_password_confirmation));

                                            new_hash_password = BANK::hashing_password(new_password);

                                            call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

                                            std::cout << "Password changed Successfully" << std::endl;
                                            std::cout << std::endl;

                                            new_password.clear();
                                            new_password_confirmation.clear();
                                        }

                                        else
                                            std::cout << "The provided Answer is Incorrect" << std::endl;

                                        break;
                                    }

                                } while (options3);

                                break;

                            case 8: // Transaction History
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        Transactions::display_transactions_history(connection, account_number);

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 9: // Relative Transaction History
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        std::cout << "Enter the relative Date ( 2024-01-31 ): " << std::endl;
                                        std::cin >> date;
                                        std::cout << std::endl;

                                        std::cout << "0. Display All Transactions occurred BEFORE the selected Date" << std::endl;
                                        std::cout << "1. Display All Transactions occurred AFTER the selected Date" << std::endl;
                                        std::cout << "2. Display All Transactions occurred ONLY ON the selected Date" << std::endl;
                                        std::cout << std::endl;

                                        std::cin >> choice;
                                        std::cout << std::endl;

                                        if (choice != 0 && choice != 1 && choice != 2)
                                        {
                                            std::cout << "INVALID ENTER OPTION, PLEASE CHOOSE BETWEEN 1, 2 AND 3" << std::endl;

                                            break;
                                        }

                                        Transactions::display_specific_transactions_history(connection, account_number, date, choice);

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 10: // Delete an Account
                                if (!BANK::authentification_message(connection, account_number, hash_password))
                                    break;

                                do
                                {
                                    std::cout << "What is your Password: " << std::endl;
                                    std::cout << "You have 3 Chances" << std::endl;

                                    std::cin >> password;
                                    std::cout << std::endl;

                                    if (BANK::verifying_password(password, hash_password))
                                    {
                                        Account::remove_accounts(connection, account_number);

                                        password.clear();

                                        break;
                                    }

                                    password.clear();

                                    std::cout << "Incorrect password. Chances left: " << k - 1 << std::endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;
                            }

                        } while (options2);

                        break;

                    case 3: // Information on the Bank
                        std::cout << "Title The CROSS_CONTINENTAL TREASURY BANK: A Compendium of Financial Excellence" << std::endl;

                        std::cout << "CHAPTER 1: INTRODUCTION TO THE CROSS-CONTINENTAL TREASURY BANK" << std::endl;
                        std::cout << "SECTION 1.1: Our Vision and Mission" << std::endl;
                        std::cout << std::endl;
                        std::cout << "The CROSS-CONTINENTAL TREASURY BANK envisions a world where financial services transcend borders, empower individuals, and foster economic growth." << std::endl;
                        std::cout << "Our mission is to provide cutting-edge banking solutions with a commitment to integrity, innovation, and client satisfaction." << std::endl;
                        std::cout << "SECTION 1.2: Core Values" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Integrity: Upholding the highest ethical standards in all interactions." << std::endl;
                        std::cout << "Innovation: Pioneering financial solutions that adapt to the dynamic global landscape." << std::endl;
                        std::cout << "Client Satisfaction: Place our clients at the center and ensuring their success is our success." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        std::cout << "CHAPTER 2: THE PILLARS OF FINANCIAL SECURITY" << std::endl;
                        std::cout << std::endl;
                        std::cout << "SECTION 2.1: Robust Security Protocols" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Implementing state-of-the-art encryption and cybersecurity measures to safeguard client information." << std::endl;
                        std::cout << "Regular audits and assessments to fortify our defenses against emerging threats." << std::endl;
                        std::cout << "SECTION 2.2: Compliance and Regulatory Adherence" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Strict adherence to international banking regulations and compliance standards." << std::endl;
                        std::cout << "Continuous training and development for staff to stay abreast of regulatory changes." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        std::cout << "CHAPTER 3: SERVICES TAILORED TO YOUR AMBITIONS" << std::endl;
                        std::cout << std::endl;
                        std::cout << "SECTION 3.1: Personalized Banking Solutions" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Tailoring financial services to meet the unique needs and aspirations of our diverse clientele." << std::endl;
                        std::cout << "Offering a wide array of accounts, investment options, and lending services." << std::endl;
                        std::cout << "SECTION 3.2: Global Accessibility" << std::endl;
                        std::cout << std::endl;
                        std::cout << "A network of branches and ATMs spanning continents, ensuring clients can access their funds whenever and wherever they need." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        std::cout << "CHAPTER 4: EMPOWERING FINANCIAL LITERACY" << std::endl;
                        std::cout << std::endl;
                        std::cout << "SECTION 4.1: Educational Initiatives" << std::endl;
                        std::cout << std::endl;
                        std::cout << "The CROSS-CONTINENTAL TREASURY BANK commits to promoting financial literacy through workshops, seminars, and accessible online resources." << std::endl;
                        std::cout << "Encouraging informed decision-making and responsible financial behavior." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        std::cout << "CHAPTER 5: ENVIRONMENTAL AND SOCIAL RESPONSIBILITY" << std::endl;
                        std::cout << std::endl;
                        std::cout << "SECTION 5.1: Sustainable Banking Practices" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Integrating environmental and social responsibility into our business model." << std::endl;
                        std::cout << "Investing in eco-friendly initiatives and supporting community development projects." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        std::cout << "CHAPTER 6: THE FUTURE OF BANKING" << std::endl;
                        std::cout << std::endl;
                        std::cout << "SECTION 6.1: Technological Advancements" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Embrace cutting-edge technology to enhance efficiency, security, and user experience." << std::endl;
                        std::cout << "Exploring blockchain, artificial intelligence, and other innovations to shape the future of banking." << std::endl;
                        std::cout << "SECTION 6.2: Continuous Improvement" << std::endl;
                        std::cout << std::endl;
                        std::cout << "A commitment to continuous improvement through client feedback, market analysis, and strategic partnerships." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        std::cout << "CHAPTER 7: CLIENT-CENTRIC APPROACH" << std::endl;
                        std::cout << std::endl;
                        std::cout << "SECTION 7.1: Personalized Customer Service" << std::endl;
                        std::cout << std::endl;
                        std::cout << "A dedicated customer support team ensuring a seamless and delightful banking experience." << std::endl;
                        std::cout << "Proactive resolution of issues and a commitment to exceeding client expectations." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        std::cout << "CHAPTER 8: THANKS FOR CHOOSING OUR BANK" << std::endl;
                        std::cout << std::endl;
                        std::cout << "In the ever-evolving world of finance, The CROSS-CONTINENTAL TREASURY BANK stands as a beacon of reliability, innovation, and commitment." << std::endl;
                        std::cout << " This compendium serves as a testament to our dedication to providing unparalleled financial services on a global scale. " << std::endl;
                        std::cout << " Join us in the pursuit of financial excellence, where your aspirations find a home in The CROSS-CONTINENTAL TREASURY BANK." << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;
                        std::cout << std::endl;

                        break;
                    }

                } while (options1);

                break;

            case 0: // Exit
                std::cout << "Thanks for having choose CROSS-CONTINENTAL TREASURY BANK, Have a Good Day" << std::endl;

                exit(1);

                break;
            }

        } while (options);

        connection->close();
        delete connection;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "C++ ERROR: " << e.what() << std::endl;
    }
}