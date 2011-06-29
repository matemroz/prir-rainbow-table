/*file_reader.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"

#define DEFAULT_PASS_NUM 1024

char** readPassFile(char* filename, int maxCharacters) {
    FILE *fp;
    int i;
    int passCount = 0;
    char line[maxCharacters + 1];

    char **tab;

    tab = (char **) malloc(DEFAULT_PASS_NUM * sizeof (char *));
    for (i = 0; i < DEFAULT_PASS_NUM; i++) {
        tab[i] = (char *) malloc((maxCharacters + 1) * sizeof (char));
    }

    if (filename == NULL) {
        return 0;
    }

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Cannot open a file containing passwords!\n");
        return 0;
    }

    while (feof(fp) == 0) {
        fgets(line, sizeof (line), fp);
        line[strlen(line) - 1] = 0;
        printf("Zczytano haslo: %s\n", line);
        /* Obsluga przepelnienia tablicy */
        if (passCount + 1 >= DEFAULT_PASS_NUM * (passCount / DEFAULT_PASS_NUM + 1)) {
            //TODO realloc
        }
        strcpy(tab[passCount++], line);
    }

    tab[passCount] = 0; //ostatnia komorka zakonczona nullem, aby w main mozna bylo zliczyc hasla

    fclose(fp);

    return tab;
}

char ***getRTabFromFile(char *filename) {
    int n = 4096;
    int counter = 0;
    int i;
    int j;
    int flag = 0;
    char reduce[32];
    char hash[32];
    FILE *fp;

    char ***rainbowTab = (char ***) malloc(n * sizeof (char **));

    for (i = 0; i < n; i++) {
        *(rainbowTab + i) = (char **) malloc(64 * sizeof (char *));
    }
    char line[64];

    if (filename == NULL) {
        fprintf(stderr, "There is no filename given.\n");
        return NULL;
    }

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open a file which storing rainbow table data!\n");
        return NULL;
    }

    j = 0;

    while (feof(fp) == 0) {
        fgets(line, sizeof (line), fp);

        for (i = 0; i < strlen(line) - 1; i++) {
            if (line[i] == ':') {
                flag = 1;
                reduce[i] = '\0';
            }
            if (flag == 0){
                reduce[i] = line[i];
            }
            if (flag == 1){
                hash[j] = line[i];
                j++;
            }
        }

        hash[j] = '\0';

        flag = 0;
        j = 0;
        rainbowTab[counter][0] = reduce;
        rainbowTab[counter++][1] = hash;
        
        printf("Wczytano dane rainbowTab: %s:%s\n", rainbowTab[counter - 1][0], rainbowTab[counter - 1][1]);
    }
    fclose(fp);

    return rainbowTab;
}

