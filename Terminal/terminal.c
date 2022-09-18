#include "terminal.h"


/* HELPER FUNCTIONS */

char checkLuhn(uint8_t* cardNo)
{
    int nDigits = sizeof(cardNo);
 
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
    int tdlen = inputString(termData->transactionDate, 6);
    if (tdlen == 10) {
        return OK_terminal;
    }
    else return WRONG_DATE;
}
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
    int cardMonth, cardYear, termDay, termMonth, termYear;
    sscanf((void*)termData.transactionDate, "%d/%d/%d", &termDay, &termMonth, &termYear);
    sscanf((void*)cardData.cardExpirationDate, "%d/%d", &cardMonth, &cardYear);
    cardYear += 2000; // because cardYear is in YY format
    if (cardYear >= termYear) {
        if (cardYear > termYear) {
            return OK_terminal;
        }
        else {
            if (cardMonth >= termMonth) {
                if (cardYear > termMonth) return OK_terminal;
        }
        }
    }
    return EXPIRED_CARD;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) {
    if (checkLuhn(cardData->primaryAccountNumber)) return OK_terminal;
    else return WRONG_PAN;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
    float val;
    printf("Enter Transaction Amount: ");
    scanf("%f", &val);
    termData->transAmount = val;
    if (termData->maxTransAmount > 0) return OK_terminal;
    else return INVALID_AMOUNT;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
    if (termData->transAmount < termData->maxTransAmount) return OK_terminal;
    else return EXCEED_MAX_AMOUNT;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData) {
    float val;
    printf("Enter Max Transaction Amount: ");
    scanf("%f", &val);
    termData->maxTransAmount = val;
    if (termData->maxTransAmount > 0) return OK_terminal;
    else return INVALID_MAX_AMOUNT;
}