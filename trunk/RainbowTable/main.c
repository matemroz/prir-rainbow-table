/*main.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define MAXSIZE 1024
#define TAG 1


int main(int argc, char *argv[]) {


    /* BLOK DEKLARACJI ZMIENNYCH */

    int rank; /* numer procesu */
    int size; /* ilosc procesow */
    int depth = 5; /* glebokosc lancucha teczowego */
    int passSize = 10; /* dlugosc hasla */
    char **passTab;
    char ***rainbowTab;
    int passCount = 0;
    int workSize = 0;
    int workRestSize = 0;
    int dest;
    int src = 0;
    int i = 0;
    char * msgStream;


    //char *tab[28] = {"asdas", "fjgja", "xijff", "wijff", "eijff", "oqijff", "yeyjff", "ieqief", "iqiff", "ssadaf", "yeyjff", "ieqief", "iqiff", "ssadaf","asdas", "fjgja", "xijff", "wijff", "eijff", "oqijff", "yeyjff", "ieqief", "iqiff", "ssadaf", "yeyjff", "ieqief", "iqiff", "ssadaf"};
    /* utworzenie tablicy teczowej */
    //char*** rainbowTab = (char ***) createRainbowTable(tab, depth, passCount);
    /* posortowanie utworzonej tablicy */
    //quicksort(rainbowTab, 0, passCount-1);

    /*
    	int i;
        for (i = 0; i < passCount; i++) {
            printf("%s\n", rainbowTab[i][1]);
            if(i != passCount-1) {
            	printf("[wynik sortowania:%d-%d] %s <-> %s = %d\n",i,i+1,rainbowTab[i][1],rainbowTab[i+1][1],strcmp(rainbowTab[i][1],rainbowTab[i+1][1]));
            }
        }
	*/

    //crackPassword("ABXIo/MRLE2F.",rainbowTab,passCount,depth);



    /* INICJALIZACJA MPI */
    MPI_Init( 0, 0 );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    if (rank == 0) {
       	//TODO zczytanie argumentow uruchomienia

       	/* pobranie tablicy hasel z pliku */
       	passTab = (char**)readPassFile("test.txt",passSize);
       	/* zliczenie ilosci pobranych hasel */
       	while(passTab[passCount] != 0) {
       		passCount++;
       	}
    }

    MPI_Barrier(MPI_COMM_WORLD);

    /* WYSYLANIE DANYCH O POCZATKACH LANUCHOW */
    if (rank == 0) {
    	//TODO zczytanie argumentow uruchomienia

    	printf("passCount: %d\n",passCount);
    	/* obliczenie wielkosci czesci tablicy przydzielanych dla kazdego procesu */
    	workSize = passCount / size;
    	workRestSize = passCount % size;

    	for (dest = 1; dest < size; dest++) {
    		MPI_Send(&workSize,1,MPI_INT,dest,TAG,MPI_COMM_WORLD);//wyslanie wiadomosci o ilosci przydzielonych hasel

    	msgStream = (char *)malloc(workSize*(passSize+1)*sizeof(char));//alokacja pamieci dla lancucha skladajacego sie z napisow czesci tablicy
    	/* Laczenie napisow, gdzie '\n' oddziela poszczegolne wyrazy*/
    	i = (dest-1)*workSize;
    	while (i < workSize*dest) {
    		strcat(msgStream,passTab[i]);
    		strcat(msgStream,"\n");
    		i++;
    	}
    	//printf("Wynik polaczenia[rozmiar: %d]:\n%s\n",strlen(msgStream),msgStream);

    	MPI_Send(msgStream,strlen(msgStream)+1,MPI_CHAR,dest,TAG,MPI_COMM_WORLD);//wyslanie lancucha z polaczonymi haslami

    	free(msgStream);
    	}

    } else {
    	//TODO zczytanie danych przez potomkow
    	MPI_Recv(&workSize,1,MPI_INT,0,TAG,MPI_COMM_WORLD,&status);//odebranie wiadomosci o ilosci przydzielonych hasel

    	msgStream = (char *)malloc(workSize*(passSize+1)*sizeof(char));

    	MPI_Recv(msgStream,workSize*(passSize+1),MPI_CHAR,0,TAG,MPI_COMM_WORLD,&status);//odebranie lancucha z polaczonymi haslami
    	//printf("Odebrano lancuch[rozmiar: %d]:\n%s\n",strlen(msgStream),msgStream);

    	passTab = (char **)malloc(workSize*sizeof(char *));
    	for (i = 0; i < workSize; i++){
    		passTab[i] = (char *)malloc(passSize*sizeof(char));
    	}

    	/* Rozdzielenie otrzymanego lancucha na wyrazy i skopiowanie ich do tablicy passTab*/
    	char *recvPass = strtok(msgStream,"\n");
    	i = 0;
    	while (recvPass != NULL) {
    		strcpy(passTab[i],recvPass);
    		recvPass = strtok(NULL,"\n");
    		i++;
    	}

    	/*
    	for (i = 0; i < workSize; i++){
    		printf("Otrzymana tablica[%d] w procesie %d:%s\n",i,rank,passTab[i]);
    	}
		*/
    }

    /* GENEROWANIE LANCUCHOW */
    	//TODO generowanie we wszystkich procesach

    printf("%d %d\n",workSize,rank);

    /* ODEBRANIE PUNKTOW KONCOWYCH LANCUCHOW */
   // if (rank == 0) {
    	//TODO zczytanie koncow lancuchow przez rodzica
    	//TODO zapis do pliku
   // }

    /* ZAKONCZENIE MPI*/
    MPI_Finalize();
    return 0;
}

