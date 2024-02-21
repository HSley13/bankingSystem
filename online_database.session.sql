CREATE EVENT check_events ON SCHEDULE EVERY 1 HOUR STARTS CURRENT_TIMESTAMP + INTERVAL 5 HOUR DO BEGIN
DECLARE account_number1 INT;
DECLARE invalid_case INT DEFAULT FALSE;
DECLARE cursor_acc CURSOR FOR
SELECT account_number
FROM event_schedule
WHERE scheduled_time <= CURRENT_TIMESTAMP
    AND triggered = 0;
DECLARE CONTINUE HANDLER FOR NOT FOUND
SET invalid_case = TRUE;
OPEN cursor_acc;
read_loop: LOOP FETCH cursor_acc INTO account_number1;
IF invalid_case THEN LEAVE read_loop;
END IF;
UPDATE event_schedule_table
SET triggered = 1
WHERE account_number = account_number1;
CALL update_borrowed_money(account_number1);
CALL deduce_borrowed_money(account_number1);
END LOOP read_loop;
CLOSE cursor_acc;
END;