

CREATE TRIGGER log_deposit AFTER UPDATE ON transactions FOR EACH ROW
BEGIN
    DECLARE new_line VARCHAR(255);
    
    IF NEW.deposit <> OLD.deposit THEN
        SET new_line = CONCAT('THERE IS A NEW DEPOSIT OF ', NEW.deposit);
        
        IF EXISTS (SELECT 1 FROM transactions_history WHERE account_number = NEW.account_number) THEN
            UPDATE transactions_history
            SET
                transaction_details = CONCAT(transaction_details, '\n', new_line),
                date_time = NOW()
            WHERE account_number = NEW.account_number;
        ELSE
            INSERT INTO transactions_history (account_number, transaction_details, date_time)
            VALUES (NEW.account_number, new_line, NOW());
        END IF;
    END IF;
END;