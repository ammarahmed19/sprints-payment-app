#ifndef SERVER_H
#define SERVER_H

#pragma once

#include "../Terminal/terminal.h"

#define ACCOUNTS_DB_FILE "./Server/Accounts_DB.txt"
#define TRANSACTIONS_DB_FILE "./Server/Transactions_DB.txt"

#define DB_SIZE 255

typedef enum EN_transState_t
{
APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;


typedef struct ST_transaction_t
{
ST_cardData_t cardHolderData;
ST_terminalData_t terminalData;
EN_transState_t transState;
uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum EN_serverError_t
{
OK_server, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t ;

typedef struct ST_accountsDB_t
{
float balance;
uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

typedef enum EN_dbError_t
{
    OK_db, LOAD_FAILED, PARSE_ERROR
}EN_dbError_t;

ST_accountsDB_t accounts[DB_SIZE];
ST_transaction_t transactions[DB_SIZE];

ST_transaction_t* transactions_stack;

// DB HELPER VARS
char f_line[MAX_LINE_LENGTH];
char db_accounts_raw[255][MAX_LINE_LENGTH];
char db_transactions_raw[255][MAX_LINE_LENGTH];

EN_dbError_t loadServer();
EN_dbError_t loadDB(char* fileName, char rawLines[255][MAX_LINE_LENGTH]);
EN_dbError_t updateTransactionDB();
EN_dbError_t parseAccounts(ST_accountsDB_t* accountsdb, char rawLines[255][MAX_LINE_LENGTH]);
EN_dbError_t parseTransactions(ST_transaction_t* transactionsdb, char rawlines[255][MAX_LINE_LENGTH]);
EN_transState_t recieveTransactionData(ST_transaction_t *transData);
EN_serverError_t isValidAccount(ST_cardData_t *cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, uint8_t pan[20]);
EN_serverError_t saveTransaction(ST_transaction_t *transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData);


#endif