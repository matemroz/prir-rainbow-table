/*rainbow_cracker.c*/

#define MAX_TAB_SIZE 1024
#define MAX_PASS_SIZE 10
 

/* 
 * Funkcja łamiąca zaszyfrowane algorytmem DES hasło. 
 * Zwraca odszyfrowane podane w argumencie hasło.
 */
void crackPassword(char *str, char ***rainbowTable, int tabSize,int depth){
	int l = 0;//początek tablicy
	int p = tabSize-1;//koniec tablicy
	int s = 0;//środek tablicy
	int hashFound = 0;//czy znaleziono hash
	int i;
	char *hashTmp;
	char *passTmp;



	for (i = 0; i < tabSize; i++) {
		printf("%s -> %s\n", rainbowTable[i][0], rainbowTable[i][1]);
	}

	i = depth;
	hashTmp = str;
	while ((i > 0) && (hashFound == 0)) {
		//wyszukanie hasha w tabeli
		printf("%d\n",i);
		while ((l < p) && (hashFound == 0)) {
			s = ( l + p )/2;
			printf("%d,%d,%d\n",l,s,p);
			if (strcmp(rainbowTable[s][1],hashTmp) > 0) {
				printf("%s ? %s = %d\n",rainbowTable[s][1],hashTmp,strcmp(rainbowTable[s][1],hashTmp));
				p = s;
			}
			else if (strcmp(rainbowTable[s][1],hashTmp) < 0) {
				printf("%s ? %s = %d\n",rainbowTable[s][1],hashTmp,strcmp(rainbowTable[s][1],hashTmp));
				l = s;
			}
			else {
				printf("Znaleziono hash: %s\n", hashTmp);
				hashFound = 1;
			}
		}
		i--;

		if (hashFound == 0) {
			/*recukcja i zahashowanie szukanego stringa*/
			printf("%s->",hashTmp);
			hashTmp = reduce(hashTmp);
			printf("%s->",hashTmp);
			hashTmp = hash(hashTmp);
			printf("%s\n",hashTmp);
			/*ponowne ustawienie poczatku i konca tablicy*/
			l = 0;
			p = tabSize - 1;
		}
	}

	if (hashFound == 0) {
		printf("Hasla nie ma w talibcy tęczowej!\n");
		return;
	}


	printf("Szukany hash: %s\n",str);
	passTmp = rainbowTable[s][0];
	while (depth > 0) {
		hashTmp = hash(passTmp);
		printf("[Wyszukiwanie w łancuchu:] %s -> %s \n", passTmp, hashTmp);
			if (strcmp(hashTmp,str) == 0) {
				printf("Haslo to: %s\n",passTmp);
				return;
			}
		passTmp = reduce(hashTmp);
		depth--;
	}

	printf("Nastapil blad podczas odzyskiwania hasla z lancucha");

}
