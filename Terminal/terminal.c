#include "terminal.h"


/* HELPER FUNCTIONS */
char checkLuhn(char* cardNo)
{
    int nDigits = strlen(cardNo);
 
    int nSum = 0, isSecond = 0;
    for (int i = nDigits - 1; i >= 0; i--) {
 
        int d = cardNo[i] - '0';
 
        if (isSecond == 1)
            d = d * 2;

        nSum += d / 10;
        nSum += d % 10;
 
        isSecond = !isSecond;
    }
    return (nSum % 10 == 0);
}
 ///////////////////////////

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
    printf("Enter transaction date (DD/MM/YYYY): ");
    gets(termData->transactionDate);
    int tdlen = strlen(termData->transactionDate);
    if (tdlen == 10) return OK;
    else return WRONG_DATE;
}
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
    int cardDay, cardMonth, cardYear, termDay, termMonth, termYear;
    sscanf(termData.transactionDate, "%d/%d/%d", &termDay, &termMonth, &termYear);
    sscanf(cardData.cardExpirationDate, "%d/%d/%d", &cardDay, &cardMonth, &cardYear);
    if (cardYear >= termYear) {
        if (cardYear > termYear) {
            return OK;
        }
        else {
            if (cardMonth >= termMonth) {
                if (cardYear > termMonth) return OK;
                else if (cardDay >= termDay) {
                    return OK;
                }
        }
        }
    }
    return EXPIRED_CARD;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) {
    if (checkLuhn(cardData->primaryAccountNumber)) return OK;
    else return WRONG_PAN;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
    printf("Enter Transaction Amount: ");
    scanf("%f", termData->maxTransAmount);
    if (termData->maxTransAmount > 0) return OK;
    else return INVALID_AMOUNT;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
    if (termData->transAmount < termData->maxTransAmount) return OK;
    else return EXCEED_MAX_AMOUNT;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData) {
    printf("Enter Max Transaction Amount: ");
    scanf("%f", termData->maxTransAmount);
    if (termData->maxTransAmount > 0) return OK;
    else return INVALID_MAX_AMOUNT;
}