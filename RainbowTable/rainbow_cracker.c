/*rainbow_cracker.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAB_SIZE 1024
#define MAX_PASS_SIZE 10
 

/* 
 * Funkcja łamiąca zaszyfrowane algorytmem DES hasło. 
 * Zwraca odszyfrowane podane w argumencie hasło.
 */
void crackPassword(char *str, char ***rainbowTable, int tabSize,int depth,int passw_type){
	int l = 0;//początek tablicy
	int p = tabSize-1;//koniec tablicy
	int s = 0;//środek tablicy
	int hashFound = 0;//czy znaleziono hash
	int i;//zmienna pomocnicza - iterator
	char *hashTmp;//zmienna pomocnicza przechowujaca hash
	char *passTmp;//zmienna pomocnicza przechowujaca haslo


	//wypisanie tablicy przekazanej do funkcji
	printf("Do funkcji crackPassword przekazano nastepujaca tablice:\n");
	for (i = 0; i < tabSize; i++) {
		printf("%s -> %s\n", rainbowTable[i][0], rainbowTable[i][1]);
	}
	printf("---\n");


	/* Wyszukiwanie zadanego hasha w tablicy teczowej */
	i = depth;
	hashTmp = str;
	while ((i > 0) && (hashFound == 0)) {
		while ((l <= p) && (hashFound == 0)) {
			s = ( l + p )/2;
			//printf("%d %d %d\n",l,s,p);
			if (strcmp(rainbowTable[s][1],hashTmp) > 0) {
				printf("Porownuje: %s ? %s = %d\n",rainbowTable[s][1],hashTmp,strcmp(rainbowTable[s][1],hashTmp));
				p = s - 1;
			}
			else if (strcmp(rainbowTable[s][1],hashTmp) < 0) {
				printf("Porownuje: %s ? %s = %d\n",rainbowTable[s][1],hashTmp,strcmp(rainbowTable[s][1],hashTmp));
				l = s + 1;
			}
			else {
				printf("Znaleziono hash: %s w %d wierszu\n", hashTmp,s);
				hashFound = 1;
			}
		}
		i--;

		if (hashFound == 0) {
			/*recukcja i zahashowanie szukanego stringa*/
			printf("Przeksztalcam: %s->",hashTmp);
			hashTmp = (char *)reduce(hashTmp,i,strlen(hashTmp),passw_type);
			printf("%s->",hashTmp);
			hashTmp = (char *)hash(hashTmp);
			printf("%s\n",hashTmp);
			/*ponowne ustawienie poczatku i konca tablicy*/
			l = 0;
			p = tabSize - 1;
		}
	}

	if (hashFound == 0) {
		printf("Hasla nie ma w tablicy teczowej!\n");
		return;
	}

	/* Wyszukiwanie zadanego hasha w lancuchu */
	printf("Szukany hash: %s\n",str);
	passTmp = rainbowTable[s][0];
	i=0;
	while (i < depth) {
		hashTmp = (char *)hash(passTmp);
		printf("[Wyszukiwanie w lancuchu:] %s -> %s \n", passTmp, hashTmp);
			if (strcmp(hashTmp,str) == 0) {
				printf("-----------------------------\nHaslo to: %s\n-----------------------------\n",passTmp);
				return;
			}
		passTmp =(char *)reduce(hashTmp,depth,10,passw_type);
		i++;
	}

	printf("Nastapil blad podczas odzyskiwania hasla z lancucha!\n");

}
