#include "application.h"


/* INITIALIZE */
struct ST_cardData_t User = {
    .cardHolderName = {NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN}, 
    .primaryAccountNumber = {NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN}, 
    .cardExpirationDate = {NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN}
};

struct ST_terminalData_t Terminal = {
    .maxTransAmount = 0,
    .transactionDate = {NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN, NULL_PAN , NULL_PAN},
    .transAmount = 0
};

struct ST_transaction_t Transaction = {
    .cardHolderData = 0,
    .terminalData = 0,
    .transactionSequenceNumber = 0,
    .transState = 0
};
/**********************/

EN_cardError_t cardStatus;
EN_terminalError_t terminalStatus;
EN_transState_t transStatus;
EN_serverError_t serverStatus;
EN_dbError_t dbStatus;

uint8_t userStory() {
    // Get Card Holder Name
    cardStatus = getCardHolderName(&User);
    if (cardStatus == WRONG_NAME) {
        printf("ERROR: WRONG NAME\n"); 
        return WRONG_NAME;
    }

    // Get Card Expiry Name
    cardStatus = getCardExpiryDate(&User);
    if (cardStatus == WRONG_EXP_DATE) {
        printf("ERROR: WRONG_EXP_DATE\n");
        return WRONG_EXP_DATE;
    }

    cardStatus = getCardPAN(&User);
    if (cardStatus == WRONG_PAN) {
        printf("ERROR: WRONG PAN\n");
        return WRONG_PAN;
    }

    terminalStatus = getTransactionDate(&Terminal);
    if (terminalStatus == WRONG_DATE) {
        printf("ERROR: WRONG DATE\n");
        return WRONG_DATE;
    }

    terminalStatus = isCardExpired(User, Terminal);
    if (terminalStatus == EXPIRED_CARD) {
        printf("ERROR: EXPIRED CARD\n");
        return EXPIRED_CARD;
    }

    terminalStatus = setMaxAmount(&Terminal);
    if (terminalStatus == INVALID_MAX_AMOUNT)
    {
        printf("ERROR: INVALID MAX AMOUNT\n");
        return INVALID_MAX_AMOUNT;
    }

    terminalStatus = getTransactionAmount(&Terminal);
    if (terminalStatus == INVALID_AMOUNT) {
        printf("ERROR: INVALID AMOUNT\n");
        return INVALID_AMOUNT;
    }

    terminalStatus = isBelowMaxAmount(&Terminal);
    if (terminalStatus == EXCEED_MAX_AMOUNT) {
        printf("ERROR: EXCEED MAX AMOUNT\n");
        return EXCEED_MAX_AMOUNT;
    }

    // Push Data to Server
    Transaction.cardHolderData = User;
    Transaction.terminalData = Terminal;

    transStatus = recieveTransactionData(&Transaction);
    if (transStatus == DECLINED_STOLEN_CARD) {
        printf("ERROR: DECLINED STOLEN CARD\n");
        return DECLINED_STOLEN_CARD;
    }
    else if (transStatus == DECLINED_INSUFFECIENT_FUND) {
        printf("ERROR: DECLIDED INSUFFECIENT FUND\n");
        return DECLINED_INSUFFECIENT_FUND;
    }
    else if (transStatus == INTERNAL_SERVER_ERROR) {
        printf("ERROR: INTERNAL SERVER ERROR\n");
        return INTERNAL_SERVER_ERROR;
    }

    printf("TRANSACTION SAVED SUCCESSFULLY\n");
    return OK_APP;
}

void appStart() {
    dbStatus = loadServer();
    if (dbStatus == PARSE_ERROR) {
        printf("ERROR: PARSE ERROR\n");
    }
    
    uint8_t status = userStory();
    if (status != OK_APP) printf("THE PROCESS WAS ABORTED DUE TO AN ERROR\n");
}

