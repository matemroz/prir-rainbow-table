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
 * password - łańcuch na podstawie, którego będzie generowany hash
 */
char *hash(char *password) {

    char salt[3] = "AB";

    if (strlen(password) < MIN_PASS_SIZE || strlen(password) > MAX_PASS_SIZE) {
        fprintf(stderr, "Haslo powinno miec co najmniej 3 znaki i nie więcej niż 10 znakow");
        return NULL;
    }

    if (strcmp(password, "") == 0) {
        fprintf(stderr, "Nie podano lancucha, ktory ma byc poddany haszowaniu!");
        return NULL;
    }

    char *pass = malloc(15);
    strcpy(pass, crypt(password, salt));

    return pass;
}

/*
 * Funkcja redukcji w tablicy teczowej. Jako argumenty przyjmuje kolejno:
 * hash - zostanie poddany redukcji,
 * depth - glebokosc tablicy teczowej,
 * passSize - prawdopodobny rozmiar zaszyfrowanego hasla.
 * passType - rodzaj znakow na podstawie, ktorego bedzie generowana tablica.
 */
char* red(char* h, int depth, int passSize, int passType) {

    int i;
    int index;
    int chars_size;
    char* reduction;
    char *charset;

    if (passType == 0) {
        charset = (char *) alphanum;
        chars_size = sizeof (alphanum);
    }

    if (passType == 1) {
        charset = (char *) bigalphanum;
        chars_size = sizeof (bigalphanum);
    }

    if (passType == 2) {
        charset = (char *) smallalphanum;
        chars_size = sizeof (smallalphanum);
    }

    if (passType == 3) {
        charset = (char *) alpha;
        chars_size = sizeof (alpha);
    }

    if (passType == 4) {
        charset = (char *) smallalpha;
        chars_size = sizeof (smallalpha);
    }

    if (passType == 5) {
        charset = (char *) bigalpha;
        chars_size = sizeof (bigalpha);
    }

    if (passType == 6) {
        charset = (char *) num;
        chars_size = sizeof (num);
    }

    if (passType == 7) {
        charset = (char *) ext_alphanum;
        chars_size = sizeof (ext_alphanum);
    }

    reduction = malloc(passSize * sizeof (char));
    for (i = 0; i < passSize - 1; i++) {
        index = (h[i]*11 + h[i + 2]*13 + h[i + 4]*17 + h[13]*19 + h[11]*23) % chars_size;
        reduction[i] = charset[index % 60];
    }

    reduction[passSize] = '\0';
    return reduction;
}

/*
 * Funkcja redukcji w tablicy teczowej. Jako argumenty przyjmuje kolejno:
 * wordstab - tablicę wyrazów na podstawie, których ma być tworzona tablica tęczowa,
 * depth - glebokosc tablicy teczowej,
 * n - ilość wyrazów na podstawie, których będzie tworzona tablica tęczowa,
 * passSize - prawdopodobny rozmiar zaszyfrowanego hasla.
 * passType - rodzaj znakow na podstawie, ktorego bedzie generowana tablica.
 */
char*** createRainbowTable(char **wordstab, int depth, int n, int passSize, int passType) {
    int i;
    int j;
    char *h;
    char *r;
    char ***rainbowtab = (char ***) malloc(n * sizeof (char **));

    for (i = 0; i < n; i++) {
        rainbowtab[i] = (char **) malloc(2 * sizeof (char *));
        if (rainbowtab[i] == NULL) {
            fprintf(stderr, "Nie mozna przydzielic pamieci!");
            free(rainbowtab);
            return NULL;
        }
    }

    if (rainbowtab == NULL) {
        fprintf(stderr, "Nie mozna przydzielic pamieci!");
        free(rainbowtab);
        return NULL;
    }

    for (j = 0; j < n; j++) {
        for (i = 0; i < depth; i++) {
            if (i == 0)
                h = (char *) hash(*(wordstab + j));

            if (i > 0 && i % 2 == 0)
                h = (char *) hash(r);

            if (i % 2 != 0)
                r = (char *) red(h, i, passSize, passType);

            if (i == depth - 1)
                rainbowtab[j][1] = h;
        }
        rainbowtab[j][0] = *(wordstab + j);
    }
    return rainbowtab;
}
