#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
    printf("Please enter the cardholder name: ");
    gets(cardData->cardHolderName);
    char namelen = strlen(cardData->cardHolderName);
    if (cardData->cardHolderName == NULL || namelen > 24 || namelen < 20) return WRONG_NAME;
    else return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
    printf("Please enter the expiry date (MM/YY): ");
    gets(cardData->cardExpirationDate);
    char datelen = strlen(cardData->cardExpirationDate);
    if(datelen == 5 && cardData->cardExpirationDate[2] == '/') return OK;
    else return WRONG_EXP_DATE;
}
EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
    printf("Please enter the card PAN: ");
    gets(cardData->primaryAccountNumber);
    char panlen = strlen(cardData->primaryAccountNumber);
    if (cardData->cardHolderName == NULL || panlen > 19 || panlen < 16) return WRONG_NAME;
    else return OK;
}