#ifndef CARD_H
#define CARD_H

#pragma once

#include "../LIB/lib.h"

#define PAN_LENGTH 20
#define MIN_PAN_LENGTH 15
#define CARD_HOLDERNAME_LENGTH 25
#define CARD_EXPIRATIONDATE_LENGTH 5

typedef struct ST_cardData_t
{
uint8_t cardHolderName[CARD_HOLDERNAME_LENGTH];
uint8_t primaryAccountNumber[PAN_LENGTH];
uint8_t cardExpirationDate[CARD_EXPIRATIONDATE_LENGTH];
} ST_cardData_t;

typedef enum EN_cardError_t
{
OK_card=0, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);

#endif