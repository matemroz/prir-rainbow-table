/*rainbow.c*/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "rainbow.h"

/*
 * Funkcja haszująca. Zwraca wygenerowany hasz na podstawie podanego
 * łańcucha. Wykorzystuje algorytm DES.
 */
char *hash(char *password) {

    char salt[3] = "AB";

    if (strcmp(password, "") == 0) {
        fprintf(stderr, "Nie podano hasza, ktore ma byc poddane haszowaniu!");
        return NULL;
    }

    password = strdup(crypt(password, salt));

    return password;
}

int str_to_int(char *str, int n) {
    int str_int;
    int i;

    str_int = 0;
    i = 0;

    for (i = 0; i < n; i++) {

        char c = *(str + i);
        str_int += (int) c;
    }

    return str_int;
}

char *reduce(char *hash, int deep, int passw_size, int passw_type) {
    char red_hash[passw_size + 1];
    int hash_int[2];
    int reduce_tab[2];
    char hash_left_half[DES_CHARS_NUM / 2];
    char hash_right_half[DES_CHARS_NUM / 2 + 1];
    int i;
    int j;
    int chars_size = 0;
    char *charset;

    switch (passw_type) {

        case 0:
            charset = (char *) alphanum;
            chars_size = sizeof (alphanum);
            break;

        case 1:
            charset = (char *) bigalphanum;
            chars_size = sizeof (bigalphanum);
            break;

        case 2:
            charset = (char *) smallalphanum;
            chars_size = sizeof (smallalphanum);
            break;

        case 3:
            charset = (char *) alpha;
            chars_size = sizeof (alpha);
            break;

        case 4:
            charset = (char *) smallalpha;
            chars_size = sizeof (smallalpha);
            break;

        case 5:
            charset = (char *) bigalpha;
            chars_size = sizeof (bigalpha);
            break;

        case 6:
            charset = (char *) num;
            chars_size = sizeof (num);
            break;

        default:
            break;
    }

    j = 0;
    for (i = 0; i < DES_CHARS_NUM / 2 - 1; i++) {
        hash_left_half[j++] = *(hash + i);
    }

    j = 0;
    for (i = DES_CHARS_NUM / 2 - 1; i < DES_CHARS_NUM - 1; i++) {
        hash_right_half[j++] = *(hash + i);
    }

    hash_int[0] = str_to_int(hash_left_half, DES_CHARS_NUM / 2 - 1);
    hash_int[1] = str_to_int(hash_right_half, DES_CHARS_NUM / 2);

    reduce_tab[0] = hash_int[0] ^ deep;
    reduce_tab[1] = hash_int[1] & deep;

    for (i = 0; i < passw_size / 2; i++) {
        red_hash[i] = charset[(reduce_tab[0] * i) % chars_size];
    }

    for (i = passw_size / 2; i < passw_size; i++) {
        red_hash[i] = charset[(reduce_tab[1] * i) % chars_size];
    }

    red_hash[passw_size] = '\0';
    printf("redd_hash: %s\n", red_hash);

    return red_hash;
}

/*
 * Funkcja redukująca. Zwraca łańcuch po przeprowadzeniu redukcji. Należy podać głębokość
 * tablicy tęczowej.
 *
 * min_length
 * max_length
 * charset: num, alpha, alphanum
 */
/*char *reduce(char *hash, int deep, int charset_size) {
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

    if (red_hash == NULL) {
        fprintf(stderr, "Nie mozna przydzielic pamieci 01");
        return NULL;
    }

    while (*(hash + i) != '\0') {
        if ((i >= red) && (*(hash + i) != '.') && (*(hash + i) != '/')) {
 *(red_hash + j++) = *(hash + i);
        }
        i++;
    }

    return red_hash;
}
 */

/*
 * Funkcja zwracająca dwuwymiarową tablicę tęczową, o danej głębokości i rozmiarze. Jako
 * argument otrzymuje również listę wyrazów, z których będą generowane ciągi.
 */
char*** createRainbowTable(char **wordstab, int deep, int n) {
    int i;
    int j;
    char *h;
    char *r;

    char ***rainbowtab = (char ***) malloc(n * sizeof (char **));

    for (i = 0; i < n; i++) {
        *(rainbowtab + i) = (char **) malloc(deep * sizeof (char *));
    }

    if (rainbowtab == NULL) {
        fprintf(stderr, "Nie mozna przydzielic pamieci 02");
        return NULL;
    }

    char *tmp;
    for (j = 0; j < n; j++) {
        printf("lancuch[%d]:\n", j);

        for (i = 0; i < deep; i++) {

            if (i == 0) {
                printf("rainbowtab[%d][0]=wordstab[%d]\n", j, j);
                h = (char *) hash(*(wordstab + j));
                printf("rainbowtab1[%d][%d]=%s\n", j, i + 1, h);
            }

            if (i > 0 && i % 2 == 0) {
                h = (char *) hash(r);
                printf("rainbowtab2[%d][%d]=%s\n", j, i + 1, h);
            }


            if (i % 2 != 0) {
                r = (char *) reduce(h, 0, 0, 0);
                printf("rainbowtab3[%d][%d]=%s\n", j, i + 1, r);
            }

            if (i == deep - 1) {
                rainbowtab[j][1] = h;
            }
        }

        rainbowtab[j][0] = *(wordstab + j);
    }

    quicksort(rainbowtab, 0, 14);
    for (i = 0; i < n; i++) {
        printf("tab[%d]: %s\n", i, rainbowtab[i][1]);
    }

    return rainbowtab;
}