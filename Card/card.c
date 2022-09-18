#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
    printf("Please enter the cardholder name: ");
    int namelen = inputString(cardData->cardHolderName, 25);
    if (cardData->cardHolderName == NULL || namelen > 24 || namelen < 20) return WRONG_NAME;
    else return OK_card;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
    printf("Please enter the expiry date (MM/YY): ");
    int datelen = inputString(cardData->cardExpirationDate, 6);
    if(datelen == 5 && cardData->cardExpirationDate[2] == '/') return OK_card;
    else return WRONG_EXP_DATE;
}
EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
    printf("Please enter the card PAN: ");
    int panlen = inputNumericString(cardData->primaryAccountNumber, PAN_LENGTH);
    if (cardData->cardHolderName == NULL || panlen > 19 || panlen < 16) return WRONG_NAME;
    else return OK_card;
}