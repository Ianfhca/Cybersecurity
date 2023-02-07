#include "include.h"
#include <ctype.h>
#include <string.h>

void print_cmsgs(unsigned char c[MSGS][LEN]) {
    int i, j;

    for (i = 0; i < MSGS; i++) {
        printf("C%d: ", i);
        for (j = 0; j < LEN; j++) {
            printf("%c",  c[i][j]);
        }
        printf("\n");
    }
}

void print_hex(unsigned char c[LEN], int len) {
    int j;

    for (j = 0; j < len; j++) {
        printf("%x",  c[j]);
    }
}

void hex_to_ascii(unsigned char c[LEN], unsigned char aux[LEN], int len) {
    int i, j;
    unsigned char b1, b2;
    // a = 87 y 0 = 48/
    i = 0;
    for (j = 0; j < len; j += 2) {
        b1 = c[j];
        b2 = c[j+1];
        if (b1 >= 48 && b1 <= 57) {
            b1 = b1-48;
        } else {
            b1 = b1-87;
        }
        if (b2 >= 48 && b2 <= 57) {
            b2 = b2-48;
        } else {
            b2 = b2-87;
        }
        aux[i] = b1*16+b2;
        i++;
        printf("%02x", b1*16+b2);
    }
    aux[i] = '\00';
}

//mira si hay espacio entre los mensajes
void tabla_mensajes(unsigned char m1[10], unsigned char m2[10], int len){
    char b;
    char v_aux[LEN]; //CREAMOS el vector con las uniones de m1 y m2
    for(int i=0; i<len; i++){
        //for(int ji=0; j<m2; j++){
            if(m1[i] == 32 || m2[i]!= 32){ //el primero es espacio y el segundo no
                //miramos si es minuscula o mayuscula
                if(m2[i]>=65 || m2[i]<=90){//MAYÚSCULA
                    b = toupper(m2[i]);
                    v_aux[i] = b;
                }else if(m2[i]>=97 || m2[i]<=122){ //es minúscula
                    b = tolower(m2[i]);
                }
            }else if(m1[i]!= 32 || m2[i] == 32){ //el segundo es espacio y el primero no
                if(m1[i]>=65 || m1[i]<=90){//MAYÚSCULA
                    b = toupper(m1[i]);
                    v_aux[i] = b;
                }else if(m1[i]>=97 || m1[i]<=122){ //es minúscula
                    b = tolower(m1[i]);
                }
            }else{ //ninguno de los dos es espacio
                b = "NA";
                v_aux[i] = b;
            }
        }
        printf(v_aux);

    }
//}

int main() {

    int i, j;
    unsigned char mykey[10] = "abcdefghij"; 

    unsigned char mifrase1[10] = "kaixo libe";  
    unsigned char mifrase2[10] = "aB cd ef g";

    unsigned char c[MSGS][LEN] = {"1a1617451a411517490b061b0f08535404044e17450c1c45326222420a00340006544816170b54030b55020d530046",
        "184f184f0a081a000016071a00010017090b00100416010054530e060c52301b0c000a131304430e0a0640",
        "09001a5248041b04490a4f060b07550601115953150c010007000604134f2b4f01481a0417115348",
        "7926114506151f1159461b1d0b025454010850120617014542104b08104c35061a4e48201b00520f0c1400170e",
        "0c3c5547071713174e0b0a1b1a445018090b5353110c5216044505015904685a5542010d1a0c4f084f1a0044430d0a005200000007171d54124b",
        "1a001c4e480c1f0b490808551e1645070d0b5400450252071d474b020b4f2e1b1d00010f560659040a070d1649190d4b",
        "0d0710003d32560c5346211a4e550000091747161143140a06001f040b473c1b1044480002114105040640",
        "1c02054c0718130000080a12020d47110606455306021c45174f06150b4f34060645480f131157091d1e4e1745171d1749071c4d",
        "103b55530d02031749121655070a541104094914000d1100544918450c4e3d0a07551c081a0c5a030b5b",
        "120a1050010f1145410a03551d0b46001f0452164516020115540e0159492a4f14441e0805004448",
        "793b024f48151f0845461f140a444907480b4f074510170601520e4959552a0a5541480f1312000d0a0c4e0141170045541a08065c",
        "163b55530d020317491216551e1645070d0b54004502520b11574b161c54790013000b0917094c0301120b170e",
        "100021000c04000c43031c550d054e54010b43010002010054530e060c52301b0c001e141a0b45140e17070849000100535d",
        "00000000000000000000000000000000000000000000000000000000000000000000"};
        // "793b1d451a04560c53460e550d1d42111a4553160616000c00594b161249350306001b0919175407081040"};
    unsigned char aux[MSGS][LEN];
    int len;
    

    print_cmsgs(c);
    for (i = 0; i < MSGS; i++) {
        printf("\nM%d: ", i);
        len = strlen(c[i]);
        hex_to_ascii(c[i], aux[i], len);
    }
    printf("\n");
    for (i = 0; i < MSGS; i++) {
        printf("C%d: ", i);
        len = strlen(c[i]);
        print_hex(aux[i], len);
        printf("\n");
    }
    
    printf("\n");
    printf(mifrase1);
    printf("\n");
    printf(mifrase2);
    tabla_mensajes(mifrase1, mifrase2, 10);

    return 0;
}
