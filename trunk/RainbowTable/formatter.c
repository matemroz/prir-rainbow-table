/*formatter.c*/

/*
 * Funkcja konwertujaca lancuch do postaci int. Zwraca wartość typu int w wypadku powodzenia*/
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