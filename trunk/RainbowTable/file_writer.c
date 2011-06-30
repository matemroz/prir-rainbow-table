/*file_writer.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_writer.h"

/*Funkcja zapisująca tablice teczowa do pliku. Zwraca -1 w przypadku niepowodzenia, a
 0 w wypadku poprawnego działania.*/
int saveRTabToFile(char *filename, char ***rainbowTab, int tabSize, int depth, int  passSize, int passType) {
    FILE *fp;
    int i;

    if (tabSize <= 0) {
        fprintf(stderr, "Musisz podac rozmiar tablicy.\n");
        return -1;
    }

    if (filename == NULL) {
        fprintf(stderr, "Nie podano nazwy pliku.\n");
        return -1;
    }

    if ((fp = fopen(filename, "a+")) == NULL) {
        fprintf(stderr, "Nie mozna otworzyc pliku dla przechowywania tablicy!\n");
        return -1;
    }

    fprintf(fp, "%d %d %d %d\n", tabSize, depth, passSize, passType);

    for (i = 0; i < tabSize; i++) {
        fprintf(fp, "%s:%s\n", rainbowTab[i][0], rainbowTab[i][1]);
    }

    fclose(fp);

    return 0;
}
