#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorter.h"
#include "rainbow.h"

void swap(char **str1, char **str2) {
    char *temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}


/* UŻYCIE quicksorta:
   char *tab[3][2] = {
        {"jaifd", "blablablabl"},
        {"fafaf", "iahdfoahafh"},
        {"fafaf", "iahdfoahafh"}
    };

    quicksort(tab, 0, 3);
*/
void quicksort(char* rainbowTable[][2], int l, int r) {


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
