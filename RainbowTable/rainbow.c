/*rainbow.c*/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <stdlib.h>

#include "rainbow.h"

/*
 * Funkcja haszująca. Zwraca wygenerowany hasz na podstawie podanego
 * łańcucha. Wykorzystuje algorytm DES.
 */
char *hash(char *password) {

    char salt[3] = "AB";
    int a;
    int b;

    if (strcmp(password, "") == 0) {
        fprintf(stderr, "Nie podano hasza, ktore ma byc poddane haszowaniu!");
        return "";
    }

    password = crypt(password, salt);

    return password;
}

/*
 * Funkcja redukująca. Zwraca łańcuch po przeprowadzeniu redukcji.
 */
char *reduce(char *hash) {
    char *red_hash;
    int red = 5;
    int i, j;

    if ((strcmp(hash, "") == 0) || (strlen(hash) < red + 1)) {
        fprintf(stderr, "Nie podano hasza, ktory ma byc poddany redukcji lub jest zbyt krótki!");
        return "";
    }

    i = 0;
    j = 0;

    red_hash = (char *) malloc(strlen(hash) * sizeof (char));

    while (*(hash + i) != '\0') {
        if ((i >= red) && (*(hash + i) != '.') && (*(hash + i) != '/')) {
            *(red_hash + j++) = *(hash + i);
        }
        i++;
    }

    return red_hash;
}

/*
 * Funkcja zwracająca dwuwymiarową tablicę tęczową, o danej głębokości i rozmiarze. Jako
 * argument otrzymuje również listę wyrazów, z których będą generowane ciągi.
 */
char*** createRainbowTable(char *wordstab[], int deep, int n) {

    char *k;
    char *h;
    char *r;
    char *p;
    int i;
    int j;

    /*Alokacja pamięci dla tablicy*/
    char ***rainbowtab = (char ***) malloc(n * sizeof (char **));
    for (i = 0; i < n; i++) {
        rainbowtab[i] = (char **) malloc(deep * sizeof (char *));
        for (j = 0; j < deep; j++)
            rainbowtab[i][j] = (char *) malloc(DES_CHARS_NUM * sizeof (char));
    }

    for (j = 0; j < n; j++) {
        for (i = 0; i < deep; i++) {

            if (i == 0) {
                h = (char *) malloc(DES_CHARS_NUM * sizeof (char));
                h = hash(wordstab[j]); \
	printf("rainbow[%d][%d]=%s\n", j, i, h);
            }

            if (i > 0 && i % 2 == 0) {
                h = (char *) malloc(DES_CHARS_NUM * sizeof (char));
                h = hash(r);
                printf("rainbow[%d][%d]=%s\n", j, i, h);
            }

            if (i % 2 != 0) {
                r = (char *) malloc(RED_CHARS_NUM * sizeof (char));
                r = reduce(h);
                printf("rainbow[%d][%d]=%s\n", j, i, r);
            }

            if (i == deep - 1) {
                k = (char *) malloc(DES_CHARS_NUM * sizeof (char));
                k = hash(r);
            }
        }

        rainbowtab[j][0] = wordstab[j];
        rainbowtab[j][1] = k;

        printf("%s\n", rainbowtab[j][0]);
        printf("%s\n", rainbowtab[j][1]);
    }

    return rainbowtab;
}