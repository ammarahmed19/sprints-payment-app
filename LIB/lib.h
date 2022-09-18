#ifndef PAYMENT_LIB_H
#define PAYMENT_LIB_H

#pragma once

#define MAX_LINE_LENGTH 1000
#define NULL_PAN 255
#define TRUE 1 
#define FALSE 0
#define NULL_CHAR '\0'

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void cpy_char_to_uint8(uint8_t *dest, char *src, int len);
void number_cpy_char_to_uint8(uint8_t *dest, char *src, int len);
int inputString(uint8_t *arr, char maxlen);
int inputNumericString(uint8_t *arr, char maxlen);
int firstDigit(int n);
int lastDigit(int n);
uint8_t u8_arrEqual(uint8_t *arr1, uint8_t *arr2, int len);
uint8_t u8_arrEqual_pan(uint8_t *arrdb, uint8_t *arr, int len);
void u8_debug_printArr(uint8_t *arr, int len);
void u8_debug_printArr_str(uint8_t *arr, int len);

#endif