#include "application.h"

void appStart() {
    // get card info
    struct ST_cardData_t *User;
    struct ST_terminalData_t *Terminal;
    struct ST_transaction_t *Transaction;

    EN_cardError_t cardStatus;
    EN_terminalError_t terminalStatus;
    EN_transState_t transStatus;
    EN_serverError_t serverStatus;
    

    // Get Card Holder Name
    cardStatus = getCardHolderName(User);
    if (cardStatus == WRONG_NAME) {
        printf("ERROR: WRONG NAME"); 
        exit(WRONG_NAME);
    }

    // Get Card Expiry Name
    cardStatus = getCardExpiryDate(User);
    if (cardStatus == WRONG_EXP_DATE) {
        printf("ERROR: WRONG_EXP_DATE");
        exit(WRONG_EXP_DATE);
    }

    cardStatus = getCardPAN(User);
    if (cardStatus == WRONG_PAN) {
        printf("ERROR: WRONG PAN");
        exit(WRONG_PAN);
    }

    terminalStatus = getTransactionDate(Terminal);
    if (terminalStatus == WRONG_DATE) {
        printf("ERROR: WRONG DATE");
        exit(WRONG_DATE);
    }

    terminalStatus = isCardExpired(*User, *Terminal);
    if (terminalStatus == EXPIRED_CARD) {
        printf("ERROR: EXPIRED CARD");
        exit(EXPIRED_CARD);
    }

    terminalStatus = setMaxAmount(Terminal);
    if (terminalStatus == INVALID_MAX_AMOUNT);
    {
        printf("ERROR: INVALID MAX AMOUNT");
    }

    terminalStatus = getTransactionAmount(Terminal);
    if (terminalStatus == INVALID_AMOUNT) {
        printf("ERROR: INVALID AMOUNT");
        exit(INVALID_AMOUNT);
    }

    terminalStatus = isBelowMaxAmount(Terminal);
    if (terminalStatus == EXCEED_MAX_AMOUNT) {
        printf("ERROR: EXCEED MAX AMOUNT");
        exit(EXCEED_MAX_AMOUNT);
    }

    // Push Data to Server
    Transaction->cardHolderData = *User;
    Transaction->terminalData = *Terminal;

    transStatus = recieveTransactionData(Transaction);
    if (transStatus == DECLINED_STOLEN_CARD) {
        printf("ERROR: DECLINED STOLEN CARD");
        exit(DECLINED_STOLEN_CARD);
    }
    else if (transStatus == DECLINED_INSUFFECIENT_FUND) {
        printf("ERROR: DECLIDED INSUFFECIENT FUND");
        exit(DECLINED_INSUFFECIENT_FUND);
    }
    else if (transStatus == INTERNAL_SERVER_ERROR) {
        printf("ERROR: INTERNAL SERVER ERROR");
        exit(INTERNAL_SERVER_ERROR);
    }
}

