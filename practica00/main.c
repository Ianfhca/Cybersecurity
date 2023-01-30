#include "main.h"

int main() {
 	unsigned char cipher[1000]="Pda Ywaown yeldan ywj xa awoehu xnkgaj araj ej w yeldanpatp-kjhu oyajwnek. Psk oepqwpekjo ywj xa ykjoezanaz: 1) wj wppwygan gjkso (kn cqaooao) pdwp okia oknp kb oeilha oqxopepqpekj yeldan dwo xaaj qoaz, xqp jkp olayebeywhhu pdwp ep eo w Ywaown oydaia; wjz 2) wj wppwygan gjkso pdwp w Ywaown yeldan eo ej qoa, xqp zkao jkp gjks pda odebp rwhqa. Ej pda benop ywoa, pda yeldan ywj xa xnkgaj qoejc pda owia paydjemqao wo bkn w cajanwh oeilha oqxopepqpekj yeldan, oqyd wo bnamqajyu wjwhuoeo kn lwppanj sknzo. Sdeha okhrejc, ep eo hegahu pdwp wj wppwygan sehh mqeyghu jkpeya pda nacqhwnepu ej pda okhqpekj wjz zazqya pdwp w Ywaown yeldan eo pda olayebey whcknepdi ailhkuaz.Ej pda oaykjz ejopwjya, xnawgejc pda oydaia eo araj ikna opnwecdpbknswnz. Oejya pdana wna kjhu w heiepaz jqixan kb lkooexha odebpo (26 ej Ajcheod), pdau ywj awyd xa paopaz ej pqnj ej w xnqpa bknya wppwyg. ";
    int len=strlen(cipher);
    int frecuence[ALPHABET][ASCII];
    int key;
	print_string(cipher, len);

	// TODO
    for (int i = 0; i < ALPHABET; i++) {
        cal_frecuence(cipher, len, i, frecuence[i]);
    }

    key = most_frecuent(frecuence);
    
    printf("\nTHE KEY IS %d\n", key);

    unencrypt(cipher, len, key);

   	return 0;
}

int most_frecuent(int f[ALPHABET][ASCII]) {
    int i, j;
    int max = 0, aux = 0;

    for (i = 0; i < ALPHABET; i++) {
        // printf("%d\n", f[i][101]);
        if (f[i][101] > max) {
            max = f[i][101];
            aux = i;
        }
    }
    return aux;
}

void cal_frecuence(unsigned char a[], int len, int displacement, int f[]) {
    int i, current;

    for (i = 0; i <= ASCII; i++) {
        f[i] = 0;
    }

    for (i = 0; i < len; i++) {
        if ((a[i] <= LAST_LOW ) && (a[i] >= FIRST_LOW)) {
            current = a[i] + displacement;
            if (current > LAST_LOW) {
                current = (FIRST_LOW + (displacement - (LAST_LOW+1 - a[i])));
            }
            f[current]++;
        } else if ((a[i] <= LAST_UP) && (a[i] >= FIRST_UP)) {
            current = a[i] + displacement;
            if (current > LAST_UP) {
                current = (FIRST_UP + (displacement - (LAST_UP+1 - a[i])));
            }
            f[current]++;
        } 
    }
}

void unencrypt (unsigned char a[], int len, int displacement) {
    unsigned char uncrypted[1000];

    for (int i = 0; i < len; i++) {
        if ((a[i] <= LAST_LOW ) && (a[i] >= FIRST_LOW)){
            uncrypted[i] = a[i] + displacement;
            if (uncrypted[i] > LAST_LOW) {
                uncrypted[i] = (FIRST_LOW + (displacement - (LAST_LOW+1 - a[i])));
            }
        } else if ((a[i] <= LAST_UP) && (a[i] >= FIRST_UP)) {
            uncrypted[i] = a[i] + displacement;
            if (uncrypted[i] > LAST_UP) {
                uncrypted[i] = (FIRST_UP + (displacement - (LAST_UP+1 - a[i])));
            }
        } else {
            uncrypted[i] = a[i];
        }
    }
    print_string(uncrypted, len);
}

void print_string (unsigned char a[], int len)
{
	for (int i = 0; i < len; i++) {
		printf("%c", a[i]);
	}
	printf("\n");
}