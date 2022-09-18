#include "server.h"


EN_dbError_t loadServer() {
    printf("Initializing Server...\n");
    EN_dbError_t dbstatus;
    // LOAD ACCOUNTS DB
    dbstatus = loadDB(ACCOUNTS_DB_FILE, db_accounts_raw);
    if (dbstatus == LOAD_FAILED) { 
        printf("Failed to load Accounts database...\n");
        return dbstatus;
    }

    dbstatus = loadDB(TRANSACTIONS_DB_FILE, db_transactions_raw);
    if (dbstatus == LOAD_FAILED) 
    {   
        printf("Failed to load Transactions database...\n");
        return dbstatus;
    }

    if (parseAccounts(accounts, db_accounts_raw) == PARSE_ERROR) {
        printf("Accoutns Parsing Error...\n");
        return PARSE_ERROR;
    }

    if (parseTransactions(transactions, db_transactions_raw) == PARSE_ERROR) {
        printf("Transactions Parsing Error...\n");
        return PARSE_ERROR;
    }


    return OK_db;
}

EN_dbError_t loadDB(char* fileName, char rawLines[DB_SIZE][MAX_LINE_LENGTH]) {
    FILE *F_DB = fopen(fileName, "r");
    printf("\nLoading %s database\n", fileName);
    if (F_DB == NULL) return LOAD_FAILED;
    int i = 0; // iterator
    char line[MAX_LINE_LENGTH];
    while(fgets(line, MAX_LINE_LENGTH, F_DB)) {
        i++; 
        strcpy(rawLines[i], line);
        }
    fclose(F_DB);


    return OK_db;
}

EN_dbError_t parseAccounts(ST_accountsDB_t* accountsdb, char rawLines[DB_SIZE][MAX_LINE_LENGTH]) {
    int total_read; // helper
    char panreader[PAN_LENGTH]; // helper
    int balance;
    printf("Parsing accounts...\n");

    for (int i = 0; i < DB_SIZE; i++) {
        if(rawLines[i] != NULL) {
            
           char line[MAX_LINE_LENGTH];
           strcpy(line, rawLines[i]);
           total_read = sscanf(line, "%d %[^,]\n" , &balance, panreader);
           if (total_read == -1) continue;
           if (total_read == 0) return PARSE_ERROR; // if total number of items in line is 0 then parsing failed
           accountsdb[i].balance = (float) balance;
           // parse pan
            for (int j = 0; j < PAN_LENGTH; j++) {
                accountsdb[i].primaryAccountNumber[j] = panreader[j] - '0';

                // if pan is less than 20 characters, fill the remaining characters with NULL
                if (panreader[j] == NULL_CHAR) {
                    for (int k = j; k < PAN_LENGTH; k++) accountsdb[i].primaryAccountNumber[k] = NULL_PAN;
                    break;
                }

            }
           accountsdb++;
        }
        else {
            accounts[i].primaryAccountNumber[0] = NULL_PAN;
        }
    }


    return OK_db;
}
EN_dbError_t parseTransactions(ST_transaction_t* transactionsdb, char rawLines[255][MAX_LINE_LENGTH]) {
    char panreader[PAN_LENGTH]; // helper

    int total_read; // helper

    int exp_m, exp_y, t_d, t_m, t_y; // helper
    int transAmount, maxTransAmount, transState, tsn;

    printf("Parsing Transactions...\n");
    
    for (int i = 0; i < DB_SIZE; i++) {
        if(rawLines[i] != NULL) {
            total_read = sscanf(rawLines[i], "%[^\t\n,],%[^,],%d/%d,%d,%d,%d/%d/%d,%d,%d", 
            transactionsdb->cardHolderData.cardHolderName, panreader, &exp_m, &exp_y, 
            &transAmount, &maxTransAmount,
            &t_d, &t_m, &t_y, &transState, &tsn);

           if (total_read == -1) continue;
           if (total_read == 0) return PARSE_ERROR; // if total number of items in line is 0 then parsing failed
            // parse pan
            for (int j = 0; i < PAN_LENGTH; j++) {
                transactionsdb->cardHolderData.primaryAccountNumber[j] = panreader[j] - '0';

                // if pan is less than 20 characters, fill the remaining characters with NULL
                if (panreader[j] == NULL_CHAR) {
                    for (int k = j; k < PAN_LENGTH; k++) transactionsdb->cardHolderData.primaryAccountNumber[k] = NULL_PAN;
                    break;
                }
            }

            //format expiration date
            transactionsdb->cardHolderData.cardExpirationDate[2] = '/';
            if(exp_m < 10) {
                transactionsdb->cardHolderData.cardExpirationDate[0] = (uint8_t) 0;
                transactionsdb->cardHolderData.cardExpirationDate[1] = (uint8_t) exp_m;
            }
            else {
                transactionsdb->cardHolderData.cardExpirationDate[0] = (uint8_t) firstDigit(exp_m);
                transactionsdb->cardHolderData.cardExpirationDate[1] = (uint8_t) lastDigit(exp_m);
            }
            if(exp_y < 10) {
                transactionsdb->cardHolderData.cardExpirationDate[3] = (uint8_t) 0;
                transactionsdb->cardHolderData.cardExpirationDate[4] = (uint8_t) exp_m;
            }
            else {
                transactionsdb->cardHolderData.cardExpirationDate[3] = (uint8_t) firstDigit(exp_y);
                transactionsdb->cardHolderData.cardExpirationDate[4] = (uint8_t) lastDigit(exp_y);
            }

            // parse transAmount, maxTransAmount, transState, and tsn
            transactionsdb->terminalData.transAmount = transAmount;
            transactionsdb->terminalData.maxTransAmount = maxTransAmount;
            transactionsdb->transState = (EN_transState_t) transState;
            transactionsdb->transactionSequenceNumber = (uint32_t) tsn;

            // format transaction date
            transactionsdb->terminalData.transactionDate[2] = '/';
            transactionsdb->terminalData.transactionDate[4] = '/';

            // MOVE TO NEXT ELEMENT
            transactionsdb++;
        }
    }

    return OK_db;
}

EN_dbError_t updateTransactionDB() {
    
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData) { // Check if card holder data is in accounts database
    EN_serverError_t check;
    check = isValidAccount(&transData->cardHolderData);
    if (check == ACCOUNT_NOT_FOUND) return DECLINED_STOLEN_CARD;
    check = isAmountAvailable(&transData->terminalData, transData->cardHolderData.primaryAccountNumber);
    if (check == LOW_BALANCE) return DECLINED_INSUFFECIENT_FUND;
    check = saveTransaction(transData);
    if (check == SAVING_FAILED) return INTERNAL_SERVER_ERROR;
    return APPROVED;

}
EN_serverError_t isValidAccount(ST_cardData_t *cardData) {
    for (int i = 0; i < DB_SIZE; i++) {
        if (accounts[i].primaryAccountNumber[0] == NULL_PAN) continue;
        if (u8_arrEqual_pan(cardData->primaryAccountNumber, accounts[i].primaryAccountNumber, PAN_LENGTH)) return OK_server;
    }
    return ACCOUNT_NOT_FOUND;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, uint8_t pan[20]) {
    for (int i = 0; i < DB_SIZE; i++) {
        if (accounts[i].primaryAccountNumber[0] == NULL_PAN) continue;
        if (u8_arrEqual_pan(pan, accounts[i].primaryAccountNumber, PAN_LENGTH)) {
            if (accounts[i].balance > termData->transAmount) return OK_server;
            else return LOW_BALANCE;
        }
    }
    return ACCOUNT_NOT_FOUND;
}
EN_serverError_t saveTransaction(ST_transaction_t *transData) {
    if (transactions_stack == NULL) {
        transData->transactionSequenceNumber = 0;
        transactions[0] = *transData;
        transactions_stack = &transactions[0];
    }
    else {
        uint32_t transactionIndex = transactions_stack->transactionSequenceNumber + 1;
        transData->transactionSequenceNumber = transactionIndex;
        transactions[transactionIndex] = *transData;
        transactions_stack = &transactions[transactionIndex];
    }
    return OK_server;
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData) {
    for (int i = 0; i < DB_SIZE; i++) {
        if (transactionSequenceNumber == transactions[i].transactionSequenceNumber) {
            *transData = transactions[i];
            return OK_server;
        }
    }
    return TRANSACTION_NOT_FOUND;
}