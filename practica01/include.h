#include <stdio.h>
#include <string.h>

#define MSGS 14
#define LEN 1000

void hex_to_ascii(unsigned char c[LEN], unsigned char aux[LEN], int len);
void print_cmsgs(unsigned char c[MSGS][LEN]);
void print_hex(unsigned char c[LEN], int len);