#ifndef TERMINAL_H
#define TERMINAL_H

#pragma once

#include "../Card/card.h"

#define TRANSACTION_DATE_LENGTH 11

typedef struct ST_terminalData_t
{
float transAmount;
float maxTransAmount;
uint8_t transactionDate[TRANSACTION_DATE_LENGTH];
} ST_terminalData_t;

typedef enum EN_terminalError_t
{
OK_terminal=0, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
} EN_terminalError_t ;



char checkLuhn(uint8_t* cardNo);


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData);

#endif