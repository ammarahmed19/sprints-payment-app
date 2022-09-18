#include "lib.h"


void cpy_char_to_uint8(uint8_t *dest, char *src, int len) {
    for (int i = 0; i < len; i++) {
        *dest = (uint8_t)src[i];
        dest++;
    }
}

void number_cpy_char_to_uint8(uint8_t *dest, char *src, int len) {
    for (int i = 0; i < len; i++) {
        *dest = src[i] - '0';
        dest++;
    }
}

int inputString(uint8_t *arr, char maxlen) {
    char temp[maxlen];
    scanf("%[^\n]%*c", temp);
    char templen = strlen(temp);
    cpy_char_to_uint8(arr, temp, templen);
    return templen;
}

int inputNumericString(uint8_t *arr, char maxlen) {
    char temp[maxlen];
    scanf("%[^\n]%*c", temp);
    char templen = strlen(temp);
    number_cpy_char_to_uint8(arr, temp, templen);
    return templen;
}

int firstDigit(int n) {
    while(n >= 10)
    {
        n = n / 10;
    }
    return n;
}

int lastDigit(int n) {
    return n % 10;
}

uint8_t u8_arrEqual(uint8_t *arr1, uint8_t *arr2, int len) {
    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

uint8_t u8_arrEqual_pan(uint8_t *arrdb, uint8_t *arr, int len) {
    for (int i = 0; i < len; i++) {
        if (arrdb[i] == NULL_PAN || arrdb[i] > 9) continue;
        if (arrdb[i] != arr[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

void u8_debug_printArr(uint8_t *arr, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d", *arr);
        arr++;
    }
    printf("\n");
}

void u8_debug_printArr_str(uint8_t *arr, int len) {
    for (int i = 0; i < len; i++) {
        printf("%c", *arr);
        arr++;
    }
    printf("\n");
}