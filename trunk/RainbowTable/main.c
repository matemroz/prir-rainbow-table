/*main.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define TAG 1


int main(int argc, char *argv[]) {

    /* BLOK DEKLARACJI ZMIENNYCH */
    int rank; /* numer procesu */
    int lp; /* ilosc procesow */
    int depth = 5; /* glebokosc lancucha teczowego */
    int passSize = 10; /* dlugosc hasla */
    int passType = 0; /*zbior znakow*/
    int hashSize = 13;
    int passCount = 0;
    int workSize = 0;
    int workRestSize = 0;
    int dest;
    int src = 0;
    int i = 0;
    char *hash_rt;
    char **passTab;
    char ***rainbowTab;
    char *msgStream;
    char **msgTab;
    char *filename_rt;
    char *filename_out;
    char ***finalRainbowTab;
    time_t startTime;
    time_t endTime;
    MPI_Status status;

    /*Generowanie tablicy teczowej*/
    if (argc > 1 && strcmp(argv[1], "-gen") == 0) {
        if (strcmp(argv[2], "-n") == 0 && strcmp(argv[4], "-p") == 0 && strcmp(argv[6], "-t") == 0 && strcmp(argv[8], "-d") == 0 && strcmp(argv[10], "-r") == 0 && strcmp(argv[12], "-o") == 0) {
            lp = atoi(argv[3]);
            passSize = atoi(argv[5]); 
            passType = atoi(argv[7]); 
            depth = atoi(argv[9]); 
            filename_rt = argv[11];
            filename_out = argv[13];
        } else {
        	fprintf(stdout, "Niepoprawne parametry! Uruchom program z opcja '-h', zeby zobaczyc pomoc dla programu lub '-u', zeby zobaczyc uzycie.");
        	return -1;
        }
   }       /*Lamanie hasla*/
    else if (argc > 1 && strcmp(argv[1], "-b") == 0) {
        if (strcmp(argv[2],"-cr") == 0 && strcmp(argv[4],"-r") == 0) {
            hash_rt = argv[3];  
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
                "-t <typ znakow> - typ znakow (0 - alphanum, 1 - bigalphanum, 2 - smallalphanum, 3 - alpha,\n");
        
       fprintf(stdout, "4 - smallalpha, 5 - bigalpha, 6 - num, 7 - ext_alphanum).\n"
                "-d <glebokosc> - okreslenie glebokosci tablicy teczowej.\n"
                "-o mazwa pliku wyjsciowego z tablica teczowa.\n\n"
                "W programie istnieje rowniez mozliwosc lamania hasel:\n"
                "-cr <hash> hash, ktory ma zostac zlamany,\n"
                "-r <nazwa pliku> plik z tablica teczowa, ktory zostanie wczytany do programu.\n");
       return 0;
    }/*Wyswietlenie sposobu uzycia programu*/
    else if (argc > 1 && strcmp(argv[1], "-u") == 0) {
        fprintf(stdout, "Generowanie tablicy:\n"
                "rainbowtab -gen -n <liczba procesow> -p <dlugosc hasla> -t <typ znakow> -d <glebokosc> -r <nazwa pliku> -o <nazwa pliku>\n\n");

        fprintf(stdout, "Lamanie hasla:\n"
                "rainbowtab -b -cr <hash> -r <nazwa pliku>\n\n");

        fprintf(stdout, "Wyswietlenie pomocy:\n"
                "rainbowtab -h\n\n");

        fprintf(stdout, "Uzycie programu:\n"
                "rainbowtab -u\n\n");

        return 0;

    } else {
        fprintf(stdout, "Generowanie tablicy:\n"
                "rainbowtab -gen -n <liczba procesow> -p <dlugosc hasla> -t <typ znakow> -d <glebokosc> -r <nazwa pliku> -o <nazwa pliku>\n\n");

        fprintf(stdout, "Lamanie hasla:\n"
                "rainbowtab -b -cr <hash> -r <nazwa pliku>\n\n");

        fprintf(stdout, "Wyswietlenie pomocy:\n"
                "rainbowtab -h\n\n");

        fprintf(stdout, "Uzycie programu:\n"
                "rainbowtab -u\n\n");

        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "-gen") == 0) {
        /* INICJALIZACJA MPI */
        MPI_Init(0, 0);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &lp);

        if (rank == 0) {
            /* pobranie tablicy hasel z pliku */
            passTab = (char**) readPassFile(filename_rt, passSize);
            /* zliczenie ilosci pobranych hasel */
            while (passTab[passCount] != 0) {
                passCount++;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        /* WYSYLANIE DANYCH O POCZATKACH LANUCHOW */
        if (rank == 0) {
            printf("Zczytanie hasel: %d\n", passCount);
            /* obliczenie wielkosci czesci tablicy przydzielanych dla kazdego procesu */
            workSize = passCount / lp;
            workRestSize = passCount % lp;
            printf("Hasel przydzielonych do jednego procesu: %d\n", workSize);

            for (dest = 1; dest < lp; dest++) {
                MPI_Send(&workSize, 1, MPI_INT, dest, TAG, MPI_COMM_WORLD); /*wyslanie wiadomosci o ilosci przydzielonych hasel*/

                msgStream = (char *) malloc(workSize * (passSize + 1) * sizeof (char)); /*alokacja pamieci dla lancucha skladajacego sie z napisow czesci tablicy*/
                /* Laczenie napisow, gdzie '\n' oddziela poszczegolne wyrazy */
                i = (dest - 1) * workSize;
                while (i < workSize * dest) {
                    strcat(msgStream, passTab[i]);
                    strcat(msgStream, "\n");
                    i++;
                }
                //printf("Wynik polaczenia %d[rozmiar: %d]:\n%s\n", dest, strlen(msgStream), msgStream);

                MPI_Send(msgStream, strlen(msgStream) + 1, MPI_CHAR, dest, TAG, MPI_COMM_WORLD); /*wyslanie lancucha z polaczonymi haslami*/

                msgStream = NULL;
                free(msgStream);
            }

            passTab = passTab + workSize * (lp - 1);
        } else {
            MPI_Recv(&workSize, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status); /*odebranie wiadomosci o ilosci przydzielonych hasel*/
            //printf("Odebrano [workSize: %d]\n", workSize);

            msgStream = (char *) malloc(workSize * (passSize + 1) * sizeof (char));

            MPI_Recv(msgStream, workSize * (passSize + 1) + 1, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status); /*odebranie lancucha z polaczonymi haslami*/
            //printf("Odebrano lancuch[rozmiar: %d]:\n%s\n", strlen(msgStream), msgStream);

            passTab = (char **) malloc((workSize + 1) * sizeof (char *));
            for (i = 0; i < workSize; i++) {
                passTab[i] = (char *) malloc((passSize + 1) * sizeof (char));
            }

            /* Rozdzielenie otrzymanego lancucha na wyrazy i skopiowanie ich do tablicy passTab*/
            char *recvPass = strtok(msgStream, "\n");
            i = 0;
            while (recvPass != NULL && i < workSize) {
                strcpy(passTab[i], recvPass);
                recvPass = strtok(NULL, "\n");
                i++;
            }
            free(msgStream);
        }

        /* GENEROWANIE LANCUCHOW */


        rainbowTab = (char ***) createRainbowTable(passTab, depth, workSize + workRestSize, passSize, passType);

        MPI_Barrier(MPI_COMM_WORLD);

        /* WYSLANIE I ODEBRANIE KONCOWYCH LANCUCHOW */

        if (rank != 0) {
            msgStream = (char *) malloc(workSize * (passSize + 1 + hashSize + 1));
            i = 0;
            /* Budowania lancucha do wyslania, gdzie znacznik miedzy wyrazami to '\n' */
            strcpy(msgStream, "");
            while (i < workSize) {
                strcat(msgStream, rainbowTab[i][0]);
                strcat(msgStream, "\n");
                strcat(msgStream, rainbowTab[i][1]);
                strcat(msgStream, "\n");
                i++;
            }

            MPI_Send(msgStream, strlen(msgStream) + 1, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);
            /*printf("Wyslano lancuch koncowy z proc %d: %s/n",rank,msgStream);*/
            free(msgStream);
        } else {
            msgTab = (char **) malloc((lp - 1) * sizeof (char *));
            for (src = 1; src < lp; src++) {
                msgTab[src - 1] = malloc(workSize * (passSize + 1 + hashSize + 1) * sizeof (char));
                MPI_Recv(msgTab[src - 1], workSize * (passSize + 1 + hashSize + 1) + 1, MPI_CHAR, src, TAG, MPI_COMM_WORLD, &status);
                printf("Odebrano lancuch koncowy z %d: %s\n", src, msgTab[src - 1]);
            }
        }

        /* POLACZANIE ODEBRANYCH LANCUCHOW */
        if (rank == 0) {
            /* Alokacja pamieci na ostateczna wersje tablicy teczowej */
            finalRainbowTab = (char ***) malloc(passCount * sizeof (char **));
            for (i = 0; i < passCount; i++) {
                finalRainbowTab[i] = (char **) malloc(2 * sizeof (char *));
                finalRainbowTab[i][0] = (char *) malloc((passSize + 1) * sizeof (char));
                finalRainbowTab[i][1] = (char *) malloc((hashSize + 1) * sizeof (char));
            }

            /* Budowanie ostatecznej tablicy z wczesniej otrzymanych strumieni wiadomosci */
            char *strTmp;
            int j = 0;
            for (i = 0; i < (lp - 1); i++) {
                strTmp = strtok(msgTab[i], "\n");
                while (strTmp != NULL) {
                    strcpy(finalRainbowTab[j][0], strTmp);
                    strTmp = strtok(NULL, "\n");
                    strcpy(finalRainbowTab[j][1], strTmp);
                    strTmp = strtok(NULL, "\n");
                    j++;
                }
            }
            for (i = 0; i < lp; i++) {
                free(msgTab[i]);
            }
            free(msgTab);
            /* Dolaczenie do tablicy reszty lancuchow z procesu glownego */
            i = 0;
            while (j < passCount) {
                strcpy(finalRainbowTab[j][0], rainbowTab[i][0]);
                strcpy(finalRainbowTab[j][1], rainbowTab[i][1]);
                j++;
                i++;
            }

            /* Posortowanie i wypisanie tablicy */
            quicksort(finalRainbowTab, 0, passCount - 1);
            printf("|---> WYGENEROWANA TABLICA TECZOWA <---|\n");
            for (i = 0; i < passCount; i++) {
                printf("%s -> %s\n", finalRainbowTab[i][0], finalRainbowTab[i][1]);
            }
            printf("----------------------------------------\n");

            saveRTabToFile(filename_out, finalRainbowTab, passCount);

            /*crackPassword("AB.pF9XiT7ZVE", finalRainbowTab, passCount, depth, passSize, passType);*/

            for (i = 0; i < passCount; i++) {
                free(finalRainbowTab[i][0]);
                free(finalRainbowTab[i][1]);
                free(finalRainbowTab[i]);
            }
            free(finalRainbowTab);
        }

        /* ZAKONCZENIE MPI*/
        MPI_Finalize();
    }

    if (argc > 1 && strcmp(argv[1], "-b") == 0) {
        getRTabFromFile(filename_rt);
        /*crackPassword(hash_rt, finalRainbowTab, passCount, depth, passSize, passType);*/
    }

    return 0;
}

