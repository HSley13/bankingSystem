CREATE EVENT apply_interest_rate_to_balance
ON EVERY 1 DAY
STARTS CURRENT_TIMESTAMP
DO
BEGIN
    UPDATE accounts SET balance = (balance * interest_rate) + balance;
END;