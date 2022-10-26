#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once

#define OK_APP 0

#include "../Server/server.h"


void TEST_getCardHolderName();
void TEST_getCardExpiryDate();
void TEST_getCardPAN();

void TEST_getTransactionDate();
void TEST_isCardExpired();
void TEST_isValidCardPAN();
void TEST_getTransactionAmount();
void TEST_isBelowMaxAmount();
//void TEST_setMaxAmount();

void TEST_recieveTransactionData();
void TEST_setMaxAmount();
void TEST_isValidAccount();
void TEST_isAmountAvailable();
void TEST_saveTransaction();
void TEST_getTransaction();

void testingModule(void (*f)());

void userStory1();
void appStart(void);

#endif