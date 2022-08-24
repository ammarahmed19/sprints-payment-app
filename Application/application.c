#include "application.h"

void appStart() {

    // get card info
    struct ST_cardData_t *User;
    EN_cardError_t cardStatus;

    // Get Card Holder Name
    cardStatus = getCardHolderName(User);
    if (cardStatus == WRONG_NAME) {
        printf("ERROR: WRONG NAME"); 
        exit(1);
    }
    // Get Card Expiry Name
    cardStatus = getCardExpiryDate(User);
    if (cardStatus == WRONG_EXP_DATE) {
        printf("ERROR: WRONG_EXP_DATE");
        exit(1);
    }

    cardStatus = getCardPAN(User);
    if (cardStatus == WRONG_PAN) {
        printf("ERROR: WRONG PAN");
        exit(1);
    }

    
}

int main(void){
    appStart();
    return 0;
}