/*file_reader.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_PASS_NUM 1024

char** readPassFile(char* filename, int maxCharacters) {
    FILE *fp;
    int i;
    int passCount = 0;
    char line[maxCharacters+1];
    
    char **tab;

    tab = (char **)malloc(DEFAULT_PASS_NUM*sizeof(char *));
    for (i = 0; i < DEFAULT_PASS_NUM; i++){
    	tab[i] = (char *)malloc((maxCharacters+1)*sizeof(char));
    }

    if (filename == NULL) {
   	 return 0;
    }

    if ( (fp = fopen(filename,"r")) == NULL) {
   	 printf("Cannot open a file containing passwords!\n");
   	 return 0;
    }

    while ( feof(fp) == 0 ) {
   	 fgets(line,sizeof(line),fp);
   	 line[strlen(line)-1] = 0;
   	 printf("Zczytano haslo: %s\n",line);
   	 /* Obsluga przepelnienia tablicy */
   	 if(passCount+1 >= DEFAULT_PASS_NUM*(passCount/DEFAULT_PASS_NUM + 1)) {
   		 //TODO realloc
   	 }
   	 strcpy(tab[passCount++],line);
    }

    tab[passCount] = 0;//ostatnia komorka zakonczona nullem, aby w main mozna bylo zliczyc hasla

    fclose(fp);

    return tab;
}
