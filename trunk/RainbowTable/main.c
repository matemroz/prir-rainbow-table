/*main.c*/

#include <stdio.h>
//#include "mpi.h"

#define MAXSIZE 1024

int main( int argc, char *argv[] )
{
/* BLOK DEKLARACJI ZMIENNYCH */
    int rank;/* numer procesu */
    int size;/* ilosc procesow */
    int depth;/* glebokosc lancucha teczowego */
    int passSize;/* dlugosc hasla */

/* BLOK DEKLARACJI ZNAKOW */
    char alpha[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    int alphaSize = 26;

//readPassFile("test.txt",10);
   char *tab[10] = {"asdas","fjgja","eijff", "wijff", "eijff", "oqijff", "yeyjff", "ieqief", "iqiff", "ssadaf"}; 
   createRainbowTable(tab,5,10);

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
