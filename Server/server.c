#include "server.h"

EN_transState_t recieveTransactionData(ST_transaction_t *transData) { // Check if card holder data is in accounts database
    EN_serverError_t check;
    check = isValidAccount(&transData->cardHolderData);
    if (check == ACCOUNT_NOT_FOUND) return DECLINED_STOLEN_CARD;
    check = isAmountAvailable(&transData->terminalData);
    if (check == LOW_BALANCE) return DECLINED_INSUFFECIENT_FUND;
    check = saveTransaction(transData);
    if (check == SAVING_FAILED) return INTERNAL_SERVER_ERROR;
    return APPROVED;

}
EN_serverError_t isValidAccount(ST_cardData_t *cardData) {
    for (int i = 0; i < 255; i++) {
        if (cardData->primaryAccountNumber == accounts[i].primaryAccountNumber) {
            return OK;
        }
    }
    return ACCOUNT_NOT_FOUND;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData) {
    for (int i = 0;i < 255; i++) {
        if (termData->transactionDate == transactions[i].terminalData.transactionDate && termData->transAmount == transactions[i].terminalData.transAmount && termData->maxTransAmount == transactions[i].terminalData.maxTransAmount) {
            for (int j = 0; j < 255; j++) {
                if (transactions[i].cardHolderData.primaryAccountNumber == accounts[j].primaryAccountNumber) {
                    if (termData->transAmount > accounts[i].balance) {
                        return OK;
                    }
                    return LOW_BALANCE;
                }
            }
            return ACCOUNT_NOT_FOUND;
        }
    }
    return TRANSACTION_NOT_FOUND;
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
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData) {
    for (int i = 0; i < 255; i++) {
        if (transactionSequenceNumber == transactions[i].transactionSequenceNumber) {
            *transData = transactions[i];
            return OK;
        }
    }
    return TRANSACTION_NOT_FOUND;
}