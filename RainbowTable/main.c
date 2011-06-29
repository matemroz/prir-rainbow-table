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
    int passType = 0; /*zbior znakow*/
    char *hash_rt;
    char **passTab;
    char ***rainbowTab;
    int passCount = 0;
    int workSize = 0;
    int workRestSize = 0;
    int dest;
    int src = 0;
    int i = 0;
    char * msgStream;
    char *filename_rt;

    /*Generowanie tablicy teczowej*/
    if (argc > 1 && strcmp(argv[1], "-gen") == 0) {
        if (strcmp(argv[2], "-n") == 0 && strcmp(argv[4], "-p") == 0 && strcmp(argv[6], "-t") == 0 && strcmp(argv[8], "-d") == 0 && strcmp(argv[10], "-o") == 0) {
            size = atoi(argv[3]); //liczba procesow
            passSize = atoi(argv[5]); //dlugosc lancucha
            passType = atoi(argv[7]); //typ znakow
            depth = atoi(argv[9]); //glebokosc
            filename_rt = argv[11]; //nazwa pliku z tablica teczowa
        } else {
            fprintf(stdout, "Niepoprawne parametry! Uruchom program z opcja '-h', zeby zobaczyc pomoc dla programu"
                    " lub '-u', zeby zobaczyc uzycie.");
            return -1;
        }
    }       /*Lamanie hasla*/
    else if (argc > 1 && strcmp(argv[1], "-b") == 0) {
        if (strcmp(argv[2],"-cr") == 0 && strcmp(argv[4],"-r") == 0) {
            hash_rt = argv[3];  //TODO: wykorzystanie hasha
            filename_rt = argv[5];
        } else {
            fprintf(stdout, "Niepoprawne parametry! Uruchom program z opcja '-h', zeby zobaczyc pomoc dla programu"
                    " lub '-u', zeby zobaczyc uzycie.");
            return -1;
        }
    }        /*Wyswietlenie pomocy*/
    else if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        fprintf(stdout, "Dzialanie programu oparte jest na wykorzystaniu tablic teczowych. W programie"
                " istnieje mozliwosc wygenerowania tablicy zgodnie z parametrami jakie oczekujemy:\n"
                "-gen - tryb generowania lancuchow tablicy teczowej'\n"
                "-n <liczba procesow> - okreslenie liczby procesow\n"
                "-p <dlugosc hasla> - dlugosc hasla\n"
                "-t <typ znakow> - typ znakow (0 - alphanum, 1 - bigalphanum, 2 - smallalphanum, 3 - alpha,\n"
                "4 - smallalpha, 5 - bigalpha, 6 - num, 7 - ext_alphanum).\n"
                "-d <glebokosc> - okreslenie glebokosci tablicy teczowej.\n"
                "-o mazwa pliku wyjsciowego z tablica teczowa.\n\n"
                "W programie istnieje rowniez mozliwosc lamania hasel:\n"
                "-cr <hash> hash, ktory ma zostac zlamany,\n"
                "-r <nazwa pliku> plik z tablica teczowa, ktory zostanie wczytany do programu.\n"
                );
    }/*Wyswietlenie sposobu uzycia programu*/
    else if (argc > 1 && strcmp(argv[1], "-u") == 0) {
        fprintf(stdout, "Generowanie tablicy:\n"
                "rtg -gen -n <liczba procesow> -p <dlugosc hasla> -t <typ znakow> -d <glebokosc> -r <nazwa pliku>\n\n");

        fprintf(stdout, "Lamanie hasla:\n"
                "rtg -b -cr <hash> -r <nazwa pliku>\n\n");

        fprintf(stdout, "Wyswietlenie pomocy:\n"
                "rtg -h\n\n");

        fprintf(stdout, "Uzycie programu:\n"
                "rtg -u\n\n");
    } else {
        fprintf(stdout, "Generowanie tablicy:\n"
                "rtg -gen -n <liczba procesow> -p <dlugosc hasla> -t <typ znakow> -d <glebokosc> -r <nazwa pliku>\n\n");

        fprintf(stdout, "Lamanie hasla:\n"
                "rtg -b -cr <hash> -r <nazwa pliku>\n\n");

        fprintf(stdout, "Wyswietlenie pomocy:\n"
                "rtg -h\n\n");

        fprintf(stdout, "Uzycie programu:\n"
                "rtg -u\n\n");
    }


    //char *tab[28] = {"asdas", "fjgja", "xijff", "wijff", "eijff", "oqijff", "yeyjff", "ieqief", "iqiff", "ssadaf", "yeyjff", "ieqief", "iqiff", "ssadaf","asdas", "fjgja", "xijff", "wijff", "eijff", "oqijff", "yeyjff", "ieqief", "iqiff", "ssadaf", "yeyjff", "ieqief", "iqiff", "ssadaf"};
    /* utworzenie tablicy teczowej */
    //rainbowTab = (char ***) createRainbowTable(tab, depth, 28, passSize, 0);
    /* posortowanie utworzonej tablicy */
    //quicksort(rainbowTab, 0, 27);

        /*for (i = 0; i < 28; i++) {
            printf("%s\n", rainbowTab[i][1]);
            if(i != 27) {
            	printf("[wynik sortowania:%d-%d] %s <-> %s = %d\n",i,i+1,rainbowTab[i][1],rainbowTab[i+1][1],strcmp(rainbowTab[i][1],rainbowTab[i+1][1]));
            }
        }*/


    //crackPassword("ABuezp4NpihjQ",rainbowTab,28,depth,passSize,0);


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
    		/* Laczenie napisow, gdzie '\n' oddziela poszczegolne wyrazy */
    		i = (dest-1)*workSize;
    		while (i < workSize*dest) {
    			strcat(msgStream,passTab[i]);
    			strcat(msgStream,"\n");
    			i++;
    		}
    		printf("Wynik polaczenia %d[rozmiar: %d]:\n%s\n",dest,strlen(msgStream),msgStream);

    		MPI_Send(msgStream,strlen(msgStream)+1,MPI_CHAR,dest,TAG,MPI_COMM_WORLD);//wyslanie lancucha z polaczonymi haslami

    		msgStream = NULL;
    		free(msgStream);
    	}

    	passTab = passTab+30;
    } else {
    	//TODO zczytanie danych przez potomkow
    	MPI_Recv(&workSize,1,MPI_INT,0,TAG,MPI_COMM_WORLD,&status);//odebranie wiadomosci o ilosci przydzielonych hasel
    	printf("Odebrano [workSize: %d]\n",workSize);

    	msgStream = (char *)malloc(workSize*(passSize+1)*sizeof(char));

    	MPI_Recv(msgStream,workSize*(passSize+1),MPI_CHAR,0,TAG,MPI_COMM_WORLD,&status);//odebranie lancucha z polaczonymi haslami
    	printf("Odebrano lancuch[rozmiar: %d]:\n%s\n",strlen(msgStream),msgStream);

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


    	/*for (i = 0; i < workSize; i++){
    		printf("Otrzymana tablica[%d] w procesie %d:%s\n",i,rank,passTab[i]);
    	}*/

    }

    /* GENEROWANIE LANCUCHOW */


    rainbowTab = (char ***) createRainbowTable(passTab, depth, workSize, passSize, passType);
    quicksort(rainbowTab, 0, workSize-1);
    for (i = 0; i < workSize; i++){
    	printf("Moja [rank: %d] tablica: %s -> %s\n",rank,rainbowTab[i][0],rainbowTab[i][1]);
    }


    MPI_Barrier(MPI_COMM_WORLD);

    //printf("%d %d\n",workSize,rank);

    /* ODEBRANIE PUNKTOW KONCOWYCH LANCUCHOW */
    //if (rank == 0) {
    	//TODO zczytanie koncow lancuchow przez rodzica
    	//TODO zapis do pliku
    //}

    /* ZAKONCZENIE MPI*/
    MPI_Finalize();
    return 0;
}

