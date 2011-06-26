/*main.c*/

#include <stdio.h>
//#include "mpi.h"

#define MAXSIZE 1024

int main(int argc, char *argv[]) {


    /* BLOK DEKLARACJI ZMIENNYCH */

    int rank; /* numer procesu */
    int size; /* ilosc procesow */
    int depth; /* glebokosc lancucha teczowego */
    int passSize; /* dlugosc hasla */


/*
    char *tab[6][2] = {
        {"jaifd", "blablablabl"},
        {"fafaf", "iahdfoahafh"},
        {"fafaf", "zahdfoahafh"},
        {"fafaf", "aahdfoahafh"},
        {"fafaf", "cahdfoahafh"},
        {"fafaf", "tahdfoahafh"}

    };

    quicksort(tab, 0, 6);

    int i;

    for (i = 0; i < 6; i++) {
        printf("%s\n", tab[i][1]);
    }*/

    char *tab[22][2] = {
        {"jaifd", "blablablabl"},
        {"fafaf", "iahdfoahafh"},
        {"fjaid", "aiuoihahfof"},
        {"idafh", "idafjhaifia"},
        {"jdfai", "dupadupadup"},
        {"aojfa", "oajfhafhiah"},
        {"jaifh", "aojfjaihihf"},
        {"dfghh", "zjdfajfiadi"},
        {"fkaof", "dojfafdojfa"},
        {"fkaof", "wojfafdojfa"},
        {"fkaof", "dojfafdojfa"},
        {"fkaof", "3ojfafdojfa"},
        {"fkaof", "1ojfafdojfa"},
        {"fkaof", "6ojfafdojfa"},
        {"fkaof", "dsafafdojfa"},
        {"fkaof", "1ojfafdojfa"},
        {"fkaof", "dljfafdojfa"},
        {"fkaof", "zmjfafdojfa"},
        {"fkaof", "zmjfafdojfa"},
        {"fkaof", "Zmjfafdojfa"},
        {"fkaof", "dajfafdojfa"},
        {"adfjo", "aadfjaofajo"}
    };

    quicksort(tab, 0, 22);
    int i;

    for (i = 0; i < 22; i++) {
        printf("%s\n", tab[i][1]);
    }

     
    //printf("%s\n", hash("adasd"));

    //readPassFile("test.txt",10);
    /*char *tab[10] = {"asdas", "fjgja", "eijff", "wijff", "eijff", "oqijff", "yeyjff", "ieqief", "iqiff", "ssadaf"};
    char*** rainbowTable = (char ***) createRainbowTable(tab, 5, 10);
    
         int i = 0;
         for (i = 0; i < 10; i++){
             printf("tab[%d][1]=%s\n", i, rainbowTable[i][0]);
         }

         swap(&rainbowTable[0][0], &rainbowTable[2][0]);
         swap(&rainbowTable[0][1], &rainbowTable[2][1]);

         printf("[0][0]%s:[2][0]%s\n", rainbowTable[0][0], rainbowTable[2][0]);
         printf("[0][1]%s:[2][1]%s\n", rainbowTable[0][1], rainbowTable[5][1]);
     */
    /* INICJALIZACJA MPI */
    //MPI_Init( 0, 0 );
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* WYSYLANIE DANYCH O POCZATKACH LANUCHOW */
    //if (rank == 0) {
    //TODO zczytanie argumentow uruchomienia
    //TODO wyslanie danych przez rodzica
    //} else {
    //TODO zczytanie danych przez potomkow
    //}

    /* GENEROWANIE LANCUCHOW */
    //TODO generowanie we wszystkich procesach

    /* ODEBRANIE PUNKTOW KONCOWYCH LANCUCHOW */
    //if (rank == 0) {
    //TODO zczytanie koncow lancuchow przez rodzica
    //TODO zapis do pliku
    //}

    /* ZAKONCZENIE MPI*/
    //MPI_Finalize();
    return 0;
}
