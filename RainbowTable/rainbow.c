/*rainbow.c*/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "rainbow.h"
#include "formatter.h"

/*
 * Funkcja haszująca. Zwraca wygenerowany hasz na podstawie podanego
 * łańcucha. Wykorzystuje algorytm DES.
 */
char *hash(char *password) {

    char salt[3] = "AB";

    if (strlen(password) < MIN_PASS_SIZE || strlen(password) > MAX_PASS_SIZE) {
        fprintf(stderr, "Haslo powinno miec co najmniej 3 znaki i nie więcej niż 10 znakow");
        return NULL;
    }

    if (strcmp(password, "") == 0) {
        fprintf(stderr, "Nie podano hasza, ktore ma byc poddane haszowaniu!");
        return NULL;
    }

    char *pass = malloc(15);
    strcpy(pass,crypt(password, salt));

    return pass;
}

/*Funkcja redukcji w tablicy teczowej. Jako argumenty przyjmuje kolejno:
 hash - zostanie poddany redukcji,
 depth - glebokosc tablicy teczowej,
 passw_size - prawdopodobny rozmiar zaszyfrowanego hasla.
 passw_type - rodzaj znakow na podstawie, ktorego bedzie generowana tablica.
 */
char *reduce(char *hash, int depth, int passw_size, int passw_type) {
    char red_hash[passw_size + 1];
    int hash_int[2];
    int reduce_tab[2];
    char hash_left_half[DES_CHARS_NUM / 2];
    char hash_right_half[DES_CHARS_NUM / 2 + 1];
    int i;
    int j;
    int chars_size = 0;
    char *charset;

    if (passw_size < MIN_PASS_SIZE || passw_size > MAX_PASS_SIZE) {
        fprintf(stderr, "Podano zbyt mala lub zbyt tuza liczbe znakow prawdopodobnego hasla");
        return NULL;
    }

    switch (passw_type) {

        case 0:
            charset = (char *) alphanum;
            chars_size = sizeof (alphanum);
            break;

        case 1:
            charset = (char *) bigalphanum;
            chars_size = sizeof (bigalphanum);
            break;

        case 2:
            charset = (char *) smallalphanum;
            chars_size = sizeof (smallalphanum);
            break;

        case 3:
            charset = (char *) alpha;
            chars_size = sizeof (alpha);
            break;

        case 4:
            charset = (char *) smallalpha;
            chars_size = sizeof (smallalpha);
            break;

        case 5:
            charset = (char *) bigalpha;
            chars_size = sizeof (bigalpha);
            break;

        case 6:
            charset = (char *) num;
            chars_size = sizeof (num);
            break;

        case 7:
            charset = (char *) ext_alphanum;
            chars_size = sizeof (ext_alphanum);
            break;

        default:
            break;
    }

    j = 0;
    for (i = 0; i < DES_CHARS_NUM / 2 - 1; i++) {
        hash_left_half[j++] = *(hash + i);
    }

    j = 0;
    for (i = DES_CHARS_NUM / 2 - 1; i < DES_CHARS_NUM - 1; i++) {
        hash_right_half[j++] = *(hash + i);
    }

    hash_int[0] = str_to_int(hash_left_half, DES_CHARS_NUM / 2 - 1);
    hash_int[1] = str_to_int(hash_right_half, DES_CHARS_NUM / 2);

    reduce_tab[0] = hash_int[0] ^ depth;
    reduce_tab[1] = hash_int[1] ^ depth;

    for (i = 0; i < passw_size / 2; i++) {
        int index = (reduce_tab[0] ^ ((i * i) ^ (str_to_int(hash, DES_CHARS_NUM) * (i + 1)))) % chars_size;
        red_hash[i] = charset[index];
    }

    for (i = passw_size / 2; i < passw_size; i++) {
        int index = (reduce_tab[1] ^ ((i * i) ^ (str_to_int(hash, DES_CHARS_NUM) * (i + 1)))) % chars_size;
        red_hash[i] = charset[index];
    }

    red_hash[passw_size] = '\0';

    return red_hash;
}

char* red(char* h,int depth,int passSize,int passType){
	char* reduction;
	int i,index;

	reduction = malloc(passSize*sizeof(char));
	for (i = 0; i < passSize-1; i++){
		index = (h[i]*11 + h[i+2]*13 + h[i+4]*17 + h[13]*19 + h[11]*23)%64;
		reduction[i] = alphanum[index%60];
	}
	reduction[passSize] = '\0';
	return reduction;
}

/*
 * Funkcja zwracająca dwuwymiarową tablicę tęczową, o danej głębokości i rozmiarze. Jako
 * argument otrzymuje również listę wyrazów, z których będą generowane ciągi.
 */
char*** createRainbowTable(char **wordstab, int depth, int n, int passw_size, int passw_type) {
    int i;
    int j;
    char *h;
    char *r;

    char ***rainbowtab = (char ***) malloc(n * sizeof (char **));

    for (i = 0; i < n; i++) {
        rainbowtab[i] = (char **) malloc(2 * sizeof (char *));
        if (rainbowtab[i] == NULL) {
            fprintf(stderr, "Nie mozna przydzielic pamieci 06");
            free(rainbowtab);
            return NULL;
        }
        //rainbowtab[i][0] = (char *)malloc((passw_size)*sizeof(char));
        //rainbowtab[i][1] = (char *)malloc((DES_CHARS_NUM)*sizeof(char));
        /*if (rainbowtab[i][0] == NULL || rainbowtab[i][1] == NULL) {
             fprintf(stderr, "Nie mozna przydzielic pamieci 06");
             free(rainbowtab);
             return NULL;
        }*/
    }

    if (rainbowtab == NULL) {
        fprintf(stderr, "Nie mozna przydzielic pamieci 02");
        free(rainbowtab);
        return NULL;
    }

    for (j = 0; j < n; j++) {
        //printf("lancuch[%d]:\n", j);

        for (i = 0; i < depth; i++) {

            if (i == 0) {
                printf("rainbowtab[%d][0]=%s\n", j, wordstab[j]);
                h = (char *) hash(*(wordstab + j));
                printf("rainbowtab1[%d][%d]=%s\n", j, i + 1, h);
            }

            if (i > 0 && i % 2 == 0) {
                h = (char *) hash(r);
                printf("rainbowtab2[%d][%d]=%s\n", j, i + 1, h);
            }


            if (i % 2 != 0) {
                r = (char *) red(h, i, passw_size, passw_type);
                printf("rainbowtab3[%d][%d]=%s\n", j, i + 1, r);
            }

            if (i == depth - 1) {
                rainbowtab[j][1] = h;
            	//strcpy(rainbowtab[j][1],h);
            }
        }

        //strcpy(rainbowtab[j][1],*(wordstab + j));
        rainbowtab[j][0] = *(wordstab + j);
    }

    //quicksort(rainbowtab, 0, n - 1);

    for (i = 0; i < n; i++) {
        //printf("tab[%d]: %s %s\n", i,rainbowtab[i][0], rainbowtab[i][1]);
    }

    return rainbowtab;
}
