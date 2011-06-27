#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorter.h"
#include "rainbow.h"

void quicksort(char*** tab, int p, int k) {
    if (p < k) {
        int q = merge(tab, p, k);
        quicksort(tab, p, q - 1);
        quicksort(tab, q + 1, k);
    }
}

int merge(char*** tab, int p, int k) {
    int i;
    int j;
    
    char *x = (char *) malloc(DES_CHARS_NUM * sizeof (char));
    x = (char *) tab[k][1];
    i = p - 1;

    for (j = p; j < k; j++) {
        if(strcmp(tab[j][1], x) <= 0)
        {
            i++;
            swap(&tab[i][0], &tab[j][0]);
            swap(&tab[i][1], &tab[j][1]);
        }
    }
    swap(&tab[i + 1][0], &tab[k][0]);
    swap(&tab[i + 1][1], &tab[k][1]);
    
    return i + 1;
}

void swap(char **str1, char **str2) {
    char *temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}
/*
void quicksort(char*** rainbowTable, int l, int r) {


    int i = l;
    int j = r;

    char *m = (char *) malloc(DES_CHARS_NUM * sizeof (char));
    m = (char *) rainbowTable[(l + r) / 2][1];

    do {
        while (strcmp(rainbowTable[i][1], m) < 0)
            i++;
        while (strcmp(rainbowTable[j][1], m) > 0)
            j--;

        if (i <= j) {
            swap(&rainbowTable[i][0], &rainbowTable[j][0]);
            swap(&rainbowTable[i][1], &rainbowTable[j][1]);
            i++;
            j--;
        }
    } while (i <= j);

    if (l < j)
        quicksort(rainbowTable, l, j);

    if (i < r)
        quicksort(rainbowTable, i, r);
}
*/