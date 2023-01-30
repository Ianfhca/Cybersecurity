#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FIRST_LOW 65
#define LAST_LOW 90
#define FIRST_UP 97
#define LAST_UP 122
#define ALPHABET 25
#define ASCII 127

void print_string (unsigned char a[], int len);
void unencrypt(unsigned char a[], int len, int displacement);
void cal_frecuence(unsigned char a[], int len, int displacement, int f[]);
int most_frecuent(int f[ALPHABET][ASCII]);