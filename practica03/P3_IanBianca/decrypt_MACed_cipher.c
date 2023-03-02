#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>
#include "aes.h"
#include "sha256.h"


// Enable CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
#define CBC 1

#define IPAD 0x36
#define OPAD 0x5C
#define SHA256_INPUT_SIZE 64

#define MAXLENGTH 200

//Declare all functions
int test_encrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv);
int test_decrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv);
void phex(uint8_t* str, int len); //printea hexadecimales
void write_hex_file(FILE* file, uint8_t* in, int size);
int read_hex_file(FILE* file, uint8_t* out);
int hex_to_int(char c);
int hex_to_ascii(char c, char d);
uint32_t hexdigit_value(uint8_t c);
void parse(uint32_t length, uint8_t *in, uint8_t *out);
void HMAC_SHA256(uint8_t* key, int size_k, uint8_t* m, int size_m, uint8_t* HMAC);


int main(int argc, char *argv[])
{
    //Initialize variables
    uint8_t* totalfile = malloc(MAXLENGTH * sizeof(uint8_t));
    uint8_t key[AES_KEYLEN];
    uint8_t* iv=malloc(AES_BLOCKLEN * sizeof(uint8_t));
	uint8_t* c = malloc(MAXLENGTH * sizeof(uint8_t));
	uint8_t* HMAC = malloc(SHA256_BLOCK_SIZE * sizeof(uint8_t));
	int total_size_m;
	int size_c;
	uint8_t* m = malloc(MAXLENGTH * sizeof(uint8_t));
	int size_m;
	uint8_t* HMAC2 = malloc(SHA256_BLOCK_SIZE * sizeof(uint8_t));
    FILE *fd_in;
	    	
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s source_file key(hex)\n", argv[0]);
		return(0);
	}

	//Read input file .txt file and add padding
	if ((fd_in = fopen(argv[1],"r"))== 0)
	{
		fprintf(stderr, "Error opening output file");
		return(0);
	}
	total_size_m = read_hex_file(fd_in, totalfile);
	fclose(fd_in);
	//Read hex key input
	parse(AES_KEYLEN, argv[2], key);

	//Interpret source file iv + c + tag 
	size_c = total_size_m - AES_BLOCKLEN - SHA256_BLOCK_SIZE; 
	memcpy(iv,totalfile, AES_BLOCKLEN); // desglosa el totalfile para sacar iv
	memcpy(c, totalfile + AES_BLOCKLEN, size_c); // desglosa el totalfile para sacar c
	memcpy(HMAC, totalfile + AES_BLOCKLEN+size_c, SHA256_BLOCK_SIZE); // desglosa el totalfile para sacar HMAC
	printf("Received iv: ");  phex(iv, AES_BLOCKLEN);
	printf("Received Cipher: "); phex(c, size_c);

	//Decrypt
	memcpy(m, c, size_c);
	size_m = test_decrypt_cbc(key, m, size_c,iv); // se guarda en "m"

	//Calculate MAC of decrypted message
	HMAC_SHA256(key, AES_KEYLEN, m, size_m, HMAC2);
	printf("Received HMAC: "); phex(HMAC, SHA256_BLOCK_SIZE);
	printf("Calculated HMAC: "); phex(HMAC2, SHA256_BLOCK_SIZE);
	if (memcmp(HMAC, HMAC2, SHA256_BLOCK_SIZE) != 0)
		printf("Careful! Message has been modified.\n");
	else
	{
		printf("Message is correct!\nDecryption key is:\n");
		phex(m, size_m);
	}
}

void HMAC_SHA256(uint8_t* key, int size_k, uint8_t* m, int size_m, uint8_t* HMAC)
{
    int i=0;
	//primero declaramos ipad y opad
    unsigned char ipad = 0x36;
    unsigned char opad = 0x5c;

    //hacemos el padding
    unsigned char padding1[SHA256_INPUT_SIZE];
    unsigned char padding2[SHA256_INPUT_SIZE];
    for(i=0; i< SHA256_INPUT_SIZE; i++){
        //primero lo ponemos a cero los dos
        padding1[i] = 0;
    }
    for(i = 0; i< SHA256_INPUT_SIZE; i++){
        padding2[i] = 0;
    }

    //copiamos lo que hay en key a paddin1 y padding2
    memcpy(padding1, key, size_k);
    memcpy(padding2, key, size_k);
    //ahora tenemos la clave key guardada en los paddings
    //ahora calculamos ipad y opad
    for(i=0; i< SHA256_INPUT_SIZE; i++){
        padding1[i] = padding1[i]^ipad;
        
    }
    for(i=0; i< SHA256_INPUT_SIZE; i++){
        padding2[i] = padding2[i]^opad;
    }

    //ahora tenemos que sacar hash1
    //necesitamos hash1 para sacar hash2, es de 32bytes
    //el hash1 = 64 bytes + tamaño del mensaje
    //guardamos espacio de memoria para el hash:
    unsigned char hash1[SHA256_BLOCK_SIZE];
    int tamano_hash1 = SHA256_INPUT_SIZE + size_m;
    unsigned char *h1 = malloc(tamano_hash1);
    

    //tamaño del hash2 = 64 bytes + h1(que es igual a block size)
   //volvemos a guardar espacio en la memoria
    int tamano_hash2 = SHA256_INPUT_SIZE + SHA256_BLOCK_SIZE;
    
    unsigned char *h2 = malloc(tamano_hash2);

    //creamos la estructura del hash1
    //hash1 tiene el ipad y luego el mensaje
    memcpy(h1, padding1, SHA256_INPUT_SIZE);
    memcpy(&h1[SHA256_INPUT_SIZE], m, size_m);

    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, h1, tamano_hash1);
    sha256_final(&ctx, hash1);

    //ahora hacemos lo mismo para hash2
    memcpy(h2, padding2, SHA256_INPUT_SIZE);
    memcpy(&h2[SHA256_INPUT_SIZE], hash1, tamano_hash1);

    sha256_init(&ctx);
    sha256_update(&ctx, h2, tamano_hash2);
    sha256_final(&ctx, HMAC);

}


int test_decrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv)
{
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, buf, size_buf);
    size_buf -= buf[size_buf-1];
    return size_buf;
}

int test_encrypt_cbc(uint8_t* key, uint8_t* buf, int size_buf, uint8_t* iv)
{
    uint8_t l_lastblock = size_buf % AES_BLOCKLEN;
    struct AES_ctx ctx;
    if (l_lastblock < AES_BLOCKLEN) //add padding
    {
        memset(&buf[size_buf + l_lastblock], AES_BLOCKLEN - l_lastblock, AES_BLOCKLEN - l_lastblock);
    }
    size_buf += AES_BLOCKLEN;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, buf, size_buf);
    return size_buf;
}

void phex(uint8_t* str, int len)
{
    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}


void write_hex_file(FILE* file, uint8_t* in, int size)
{
    if (!feof(file))
    {
        for (int i = 0; i < size; i++)
        {
            fprintf(file, "%.2x", in[i]);
        }
    }
}

int read_hex_file(FILE* file, uint8_t* out)
{
    char ch0, ch1;
    int size = 0;
    ch0 = fgetc(file);
    ch1 = fgetc(file);
    while (ch0 != EOF)
    {
        out[size] = hex_to_ascii(ch0, ch1);
        size++;
        ch0 = fgetc(file);
        ch1 = fgetc(file);
    }
    return size;
}

//devuelve el valor entero del hexadecimal
int hex_to_int(char c)
{
    if (c >= 48 && c <= 57) //48=0 57=9
        return c - 48; //devuelve el numero
    else if (c >= 97 && c <= 102) //97=a 102=f
        return c - 87;
    else
        return c - 55;
}

//con la fórmula pasa de hexadecimal a ascii
int hex_to_ascii(char c, char d)
{
    int high = hex_to_int(c) * 16;
    int low = hex_to_int(d);
    return high + low;
}

uint32_t hexdigit_value(uint8_t c)
{
    int nibble = -1;
    if(('0' <= c) && (c <= '9')) 
        nibble = c-'0';
    if(('a' <= c) && (c <= 'f'))
        nibble = c-'a' + 10;
    if(('A' <= c) && (c <= 'F'))
        nibble = c-'A' + 10;
    return nibble;
}

//este método mira la longitud de la clave y hace padded_key dependiendo de su longitud
void parse(uint32_t length, uint8_t *in, uint8_t *out)
{
    uint32_t i, shift, idx;
    uint8_t nibble, c;
    uint32_t len = strlen(in);
    if(length >(len/2))
        length = (len/2);
    memset(out, 0, length);
    for(i = 0;i < length * 2;i++)
    {
        shift = 4 - 4 * (i & 1);
        idx = i;//len-1-i;
        c = in[idx];
        nibble = hexdigit_value(c);
        out[i/2] |= nibble << shift;
    }
}
