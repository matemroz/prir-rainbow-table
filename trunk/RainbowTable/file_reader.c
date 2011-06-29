/*file_reader.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"

#define DEFAULT_PASS_NUM 1024
#define LINE_LENGTH 64

char** readPassFile(char* filename, int maxCharacters) {
    FILE *fp;
    int i;
    int passCount = 0;
    char line[LINE_LENGTH];
    int n;
    char **tab;
    n = DEFAULT_PASS_NUM;
    tab = (char **) malloc(n * sizeof (char *));
    for (i = 0; i < n; i++) {
        tab[i] = (char *) malloc((maxCharacters + 1) * sizeof (char));
    }

    if (filename == NULL) {
        return 0;
    }

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Nie mozna otworzyc pliku z haslami!\n");
        return 0;
    }

    while (feof(fp) == 0) {// && passCount < DEFAULT_PASS_NUM
        if (passCount >= n) {

            char **tmpTab = (char **) realloc(tab, n * 2 * sizeof (char *));

            if (tmpTab == NULL) {
                fprintf(stderr, "Nie mozna przydzielic pamieci 10");
                free(tab);
                return NULL;
            }
            tab = tmpTab;

            for (i = n; i < n * 2; i++) {
                tab[i] = (char *) malloc((maxCharacters + 1) * sizeof (char));
            }

            n = n * 2;
        }
        fgets(line, sizeof (line), fp);
        line[strlen(line) - 1] = 0;

        line[maxCharacters] = 0;
        strncpy(tab[passCount], line, maxCharacters);
        tab[passCount][maxCharacters] = 0;
        printf("tab[%d]: %s\n", passCount, tab[passCount]);
        passCount++;
    }

    tab[passCount] = 0; //ostatnia komorka zakonczona nullem, aby w main mozna bylo zliczyc hasla

    fclose(fp);

    return tab;
}

char ***getRTabFromFile(char *filename) {
    int n = DEFAULT_PASS_NUM;
    int counter = 0;
    int i;
    int j;
    int flag = 0;
    char reduce[LINE_LENGTH / 2];
    char hash[LINE_LENGTH / 2];
    FILE *fp;

    char ***rainbowTab = (char ***) malloc(n * sizeof (char **));

    if (rainbowTab == NULL) {
        fprintf(stderr, "Nie mozna przydzielic pamieci 03");
        return NULL;
    }

    for (i = 0; i < n; i++) {
        *(rainbowTab + i) = (char **) malloc(LINE_LENGTH * sizeof (char *));
        if (rainbowTab == NULL) {
            fprintf(stderr, "Nie mozna przydzielic pamieci 04");
            free(rainbowTab);
            return NULL;
        }
    }
    char line[LINE_LENGTH];

    if (filename == NULL) {
        fprintf(stderr, "There is no filename given.\n");
        free(rainbowTab);
        return NULL;
    }

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open a file which storing rainbow table data!\n");
        free(rainbowTab);
        return NULL;
    }

    j = 0;

    while (feof(fp) == 0) {

        if (counter >= n) {
            printf("Przydzielanie pamieci: %d\n", counter);

            char ***tmpRainbowTab = (char ***) realloc(rainbowTab, n * 2 * sizeof (char **));

            if (tmpRainbowTab == NULL) {
                fprintf(stderr, "Nie mozna przydzielic pamieci 08");
                return NULL;
            }

            rainbowTab = tmpRainbowTab;

            for (i = n; i < n * 2; i++) {
                rainbowTab[i] = (char **) malloc(LINE_LENGTH * sizeof (char *));
                if (rainbowTab == NULL) {
                    fprintf(stderr, "Nie mozna przydzielic pamieci 09");
                    return NULL;
                }
            }

            n = n * 2;

        }

        fgets(line, sizeof (line), fp);

        for (i = 0; i < strlen(line) - 1; i++) {
            if (line[i] == ':') {
                flag = 1;
                reduce[i] = '\0';
                i++;
            }
            if (flag == 0) {
                reduce[i] = line[i];
            }
            if (flag == 1) {
                hash[j] = line[i];
                j++;
            }
        }

        hash[j] = '\0';

        flag = 0;
        j = 0;
        rainbowTab[counter][0] = reduce;
        rainbowTab[counter++][1] = hash;

        printf("Wczytano dane rainbowTab[%d]: %s:%s\n", counter - 1, rainbowTab[counter - 1][0], rainbowTab[counter - 1][1]);
    }
    fclose(fp);

    return rainbowTab;
}

