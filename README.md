// BELOW ARE ALL THE REQUIREMENTS AND PREPARATIONS FOR THE DATABASE IN ORDER TO USE THIS PROJECT'S CODE. MAKE SURE THAT YOU HAVE MYSQL INSTALLED ON YOUR PC. YOU CAN USE EITHER VSCODE OR MYSQL WORKBENCH TO RUN THESE FOLLOWING QUERIES

/*
        ----- I use CMake to link all the executables and libraries. The Database_library is also link there. Check all the CMake files to understand how the System is built
        ----- Download Argon 2 in order to use its library for the password hashing process
        ----- Change the Include Directories Path to find all the preinstalled libraries required to run this project on your PC
        ----- When Running Your Program, Pass your Database's Password as the Second Argument
        ----- The Interest Rate works in a way that it updates the Balance everytime there is a change in the latter; By Change I mean New Deposit, New Withdrawal, New Transfer and New Money Recceived from another Account
        ----- I have also set the Interest rate according to how many days have passed since the last modification occured in your balance.
        ----- If You want to apply the Interest Rate Daily or Monthly for all Accounts, I will leave an SQL Event which will help you doing so. Event Name----> apply_interest_rate_to_balance
        ----- By appliying the Interest Rate Event to your database remember deleting or commenting the //BANK :: apply_interest_rate_to_balance()// function, everywhere I have used it throughout the program
        -----
        -----
        -----
        -----

        *************** ALL THE TABLES ***************
            -- -- -- -accounts
            CREATE TABLE
            accounts(
                account_number INT AUTO_INCREMENT PRIMARY KEY,
                national_ID VARCHAR(255) NOT NULL UNIQUE,
                first_name VARCHAR(255),
                last_name VARCHAR(1000),
                date_birth VARCHAR(255),
                phone_number INT,
                email VARCHAR(255),
                address VARCHAR(255),
                balance DECIMAL(20, 5),
                interest_rate DECIMAL(5, 2),
                initial_timestamp DATETIME,
                borrowed_money DECIMAL(20, 5) DEFAULT 0,
                initial_borrowed_money_timestamp DATETIME) AUTO_INCREMENT = 1000000000;

            -- -- -- -transactions
            CREATE TABLE
            transactions(
                account_number INT PRIMARY KEY AUTO_INCREMENT,
                deposit DECIMAL(20, 5) DEFAULT 0,
                withdrawal DECIMAL(20, 5) DEFAULT 0,
                transfer DECIMAL(20, 5) DEFAULT 0,
                receive DECIMAL(20, 5) DEFAULT 0) AUTO_INCREMENT = 1000000000;

            -- -- -- -password_security
            CREATE TABLE
            password_security(
                account_number INT PRIMARY KEY,
                hashed_password VARBINARY(500));

            -- -- -- -adm_password_security
            CREATE TABLE
            adm_password_security(
                account_number INT PRIMARY KEY,
                hashed_password VARBINARY(500));

            -- -- -- -password_recovery
            CREATE TABLE
            password_recovery(
                account_number INT PRIMARY KEY,
                question VARCHAR(255),
                answer VARCHAR(255));

            -- -- -- -borrowal_record
            CREATE TABLE
            borrowal_record(
                account_number INT PRIMARY KEY,
                borrowed_amount DECIMAL(20, 5),
                interest_rate DECIMAL(5, 3),
                initial_timestamp TIMESTAMP,
                paid_timestamp TIMESTAMP);

            -------event_schedule
            CREATE TABLE
            event_schedule(
                account_number INT PRIMARY KEY,
                scheduled_time TIMESTAMP,
                triggered INT DEFAULT 0);


        *************** ALL THE TRIGGERS  ***************
        ------- insert account
        CREATE TRIGGER insert_account AFTER INSERT ON accounts FOR EACH ROW
            BEGIN
                INSERT INTO transactions (deposit, withdrawal, transfer, receive)
                VALUES (0, 0, 0, 0);
            END;

        ------- new_deposit
        CREATE TRIGGER new_deposit AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.deposit <> NEW.deposit THEN
                    UPDATE accounts SET balance = balance + NEW.deposit, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;

        ------- new_receive
        CREATE TRIGGER new_receive AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.receive <> NEW.receive THEN
                    UPDATE accounts SET balance = balance + NEW.receive, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;

        ------- new_withdrawal
        CREATE TRIGGER new_withdrawal AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.withdrawal <> NEW.withdrawal THEN
                    UPDATE accounts SET balance = balance - NEW.withdrawal, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;

        ------- new_transfer
        CREATE TRIGGER new_transfer AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.transfer <> NEW.transfer THEN
                    UPDATE accounts SET balance = balance - NEW.transfer, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;

        ------- new_borrowal
        CREATE TRIGGER new_borrowal AFTER INSERT ON borrowal_record FOR EACH ROW
            BEGIN
                    UPDATE accounts SET balance = balance + NEW.borrowed_amount, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
            END;


        *************** ALL THE PROCEDURES ***************
        ------- update_and_log_name
        CREATE PROCEDURE update_and_log_name (IN account_number1 INT, IN new_first_name VARCHAR(255))
            BEGIN
                DECLARE old_first_name VARCHAR(255);

                SELECT first_name INTO old_first_name FROM accounts WHERE account_number = account_number1;

                IF old_first_name <> new_first_name THEN
                    UPDATE accounts SET first_name = new_first_name
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Name changed to ', new_first_name);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, CURDATE(), CURTIME());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- update_and_log_email
        CREATE PROCEDURE update_and_log_email (IN account_number1 INT, IN new_email VARCHAR(255))
            BEGIN
                DECLARE old_email VARCHAR(255);

                SELECT email INTO old_email FROM accounts WHERE account_number = account_number1;

                IF old_email <> new_email THEN
                    UPDATE accounts SET email = new_email
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Email changed to ', new_email);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, CURDATE(), CURTIME());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- update_and_log_address
        CREATE PROCEDURE update_and_log_address (IN account_number1 INT, IN new_address VARCHAR(255))
            BEGIN
                DECLARE old_address VARCHAR(255);

                SELECT address INTO old_address FROM accounts WHERE account_number = account_number1;

                IF old_address <> new_address THEN
                    UPDATE accounts SET address = new_address
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Address changed to ', new_address);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, CURDATE(), CURTIME());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- update_and_log_phone_number
        CREATE PROCEDURE update_and_log_phone_number (IN account_number1 INT, IN new_phone_number VARCHAR(255))
            BEGIN
                DECLARE old_phone_number INT;

                SELECT phone_number INTO old_phone_number FROM accounts WHERE account_number = account_number1;

                IF old_phone_number <> new_phone_number THEN
                    UPDATE accounts SET phone_number = new_phone_number
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Phone Number changed to ', new_phone_number);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, CURDATE(), CURTIME());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- insert_or_update_hashed_password
        CREATE PROCEDURE insert_or_update_hashed_password (IN account_number1 INT, IN hashed_password1 VARBINARY(1000))
            BEGIN
            INSERT INTO password_security
            VALUES (account_number1, hashed_password1)
            ON DUPLICATE KEY UPDATE hashed_password = hashed_password1;
        END;

        ------- update_borrowed_money
        CREATE PROCEDURE update_borrowed_money(IN account_number1 INT)
        BEGIN
            DECLARE borrowed_money_interest_rate DECIMAL (20,5);
            DECLARE borrowed_money DECIMAL (20,5);
            DECLARE time_elapsed INT;

            SELECT interest_rate INTO borrowed_money_interest_rate FROM borrowal_record
            WHERE account_number = account_number1;

            SET time_elapsed = borrowed_money_interest_rate(retrieve_borrowal_money_initial_timestamp(account_number1));

            SELECT borrowed_amount INTO borrowed_money FROM borrowal_record
            WHERE account_number = account_number1;

            SET borrowed_money = (borrowed_money_interest_rate * borrowed_money * time_elapsed) + borrowed_money;

            UPDATE borrowal_record
            SET borrowed_amount = borrowed_money
            WHERE account_number = account_number1;
        END;

        ------- deduce_borrowed_money
        CREATE PROCEDURE deduce_borrowed_money(IN account_number1 INT)
        BEGIN
            DECLARE updated_borrowed_money DECIMAL (20,5);

            SELECT borrowed_amount INTO updated_borrowed_money FROM borrowal_record
            WHERE account_number = account_number1;

            UPDATE accounts SET balance = balance - updated_borrowed_money;
        END;


        *************** ALL THE FUNCTIONS ***************
        ------- retrieve_borrowal_money_initial_timestamp (USED IN update_borrowed_money PROCEDURE)
        CREATE FUNCTION retrieve_borrowal_money_initial_timestamp (account_number1 INT) RETURNS TIMESTAMP
        READS SQL DATA
            BEGIN
                DECLARE init_timestamp TIMESTAMP;

                SELECT initial_timestamp INTO init_timestamp FROM borrowal_record
                WHERE account_number = account_number1;

                RETURN init_timestamp;
            END;

        ------- borrowed_money_interest_rate (USED IN update_borrowed_money PROCEDURE)
        CREATE FUNCTION borrowed_money_interest_rate(initial_timestamp TIMESTAMP) RETURNS INT
        NO SQL
            BEGIN
                DECLARE time_elapsed INT;

                SET time_elapsed = TIMESTAMPDIFF(HOUR, initial_timestamp, NOW());

                RETURN time_elapsed;
            END;


        *************** ALL THE EVENTS  ***************
        ------- check_events
        CREATE EVENT check_events
        ON SCHEDULE EVERY 1 HOUR
        STARTS CURRENT_TIMESTAMP + INTERVAL 5 HOUR
        DO
            BEGIN
                DECLARE account_number1 INT;
                DECLARE invalid_case INT DEFAULT FALSE;

                DECLARE cursor_acc CURSOR FOR
                SELECT account_number FROM event_schedule
                WHERE scheduled_time <= CURRENT_TIMESTAMP AND triggered = 0;

                DECLARE CONTINUE HANDLER FOR NOT FOUND SET invalid_case = TRUE;

                OPEN cursor_acc;
                    read_loop : LOOP

                        FETCH cursor_acc INTO account_number1;

                        IF invalid_case THEN
                            LEAVE read_loop;
                        END IF;

                        UPDATE event_schedule_table SET triggered = 1
                        WHERE account_number = account_number1;

                        CALL update_borrowed_money(account_number1);
                        CALL deduce_borrowed_money(account_number1);

                    END LOOP read_loop;
                CLOSE cursor_acc;
            END;

        ------- apply_interest_rate_to_balance
        CREATE EVENT apply_interest_rate_to_balance
        ON SCHEDULE EVERY 1 DAY | Month
        STARTS CURRENT_TIMESTAMP
        DO
        UPDATE accounts SET balance = (balance * interest_rate) + balance;
*/
