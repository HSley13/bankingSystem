CREATE TRIGGER new_receive AFTER UPDATE ON transactions FOR EACH ROW
BEGIN 
        IF 
            OLD.receive <> NEW.receive THEN 
            UPDATE accounts SET balance = balance + NEW.receive
            WHERE account_number = NEW.account_number;
        END IF;
END;