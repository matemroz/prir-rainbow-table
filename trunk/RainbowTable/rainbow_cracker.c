/*rainbow_cracker.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rainbow_cracker.h"


/* 
 * Funkcja łamiąca zaszyfrowane algorytmem DES hasło. 
 * Zwraca odszyfrowane podane w argumencie hasło.
 */
char* crackPassword(char *str, char ***rainbowTable, int tabSize, int depth, int passSize, int passw_type) {
    int l; /*początek tablicy*/
    int p = tabSize - 1; /*koniec tablicy*/
    int s = 0; /*środek tablicy*/
    int hashFound = 0; /*czy znaleziono hash*/
    int i; /*zmienna pomocnicza - iterator*/
    char *hashTmp; /*zmienna pomocnicza przechowujaca hash*/
    char *passTmp; /*zmienna pomocnicza przechowujaca haslo*/

    if (passSize < MIN_PASS_SIZE || passSize > MAX_PASS_SIZE) {
            fprintf(stderr, "Haslo powinno miec co najmniej 3 znaki i nie więcej niż 10 znakow");
            return;
    }

    l = 0;

    /* Wyszukiwanie zadanego hasha w tablicy teczowej */
    i = depth;

    hashTmp = (char *) malloc(strlen(str) * sizeof (char));

    strcpy(hashTmp, str);
    while ((i > 0) && (hashFound == 0)) {
        while ((l <= p) && (hashFound == 0)) {
            s = (l + p) / 2;
            //printf("%d %d %d\n",l,s,p);
            if (strcmp(rainbowTable[s][1], hashTmp) > 0) {
                //printf("Porownuje: %s ? %s = %d\n",rainbowTable[s][1],hashTmp,strcmp(rainbowTable[s][1],hashTmp));
                p = s - 1;
            } else if (strcmp(rainbowTable[s][1], hashTmp) < 0) {
                //printf("Porownuje: %s ? %s = %d\n",rainbowTable[s][1],hashTmp,strcmp(rainbowTable[s][1],hashTmp));
                l = s + 1;
            } else {
                printf("Znaleziono hash: %s w %d wierszu\n", hashTmp, s);
                hashFound = 1;
            }
        }
        i--;

        if (hashFound == 0) {
            /*recukcja i zahashowanie szukanego stringa*/
            //printf("Przeksztalcam: %s->",hashTmp);
            strcpy(hashTmp, (char *) reduce(hashTmp, depth, passSize, passw_type));
            //printf("%s->",hashTmp);
            strcpy(hashTmp, (char *) hash(hashTmp));
            //printf("%s\n",hashTmp);

            /*ponowne ustawienie poczatku i konca tablicy*/
            l = 0;
            p = tabSize - 1;
        }
    }

    if (hashFound == 0) {
        printf("Hasla nie ma w tablicy teczowej!\n");
        free(hashTmp);
        return;
    }

    /* Wyszukiwanie zadanego hasha w lancuchu */
    printf("Szukany hash: %s\n", str);
    passTmp = (char *) malloc(passSize * sizeof (char));
    strcpy(passTmp, rainbowTable[s][0]);
    i = 0;
    while (i < depth) {
        //hashTmp = (char *)hash(passTmp);
        strcpy(hashTmp, (char *) hash(passTmp));
        printf("[Wyszukiwanie w lancuchu:] %s -> %s \n", passTmp, hashTmp);
        if (strcmp(hashTmp, str) == 0) {
            printf("-----------------------------\nHaslo to: %s\n-----------------------------\n", passTmp);
            free(hashTmp);
            return passTmp;
        }
        //passTmp =(char *)reduce(hashTmp,depth,10,passw_type);
        strcpy(passTmp, (char *) reduce(hashTmp, depth, passSize, passw_type));
        i++;
    }

    printf("Nie udalo sie odnalesc hasla w lancuchu!\n");
    free(hashTmp);
    free(passTmp);

}
