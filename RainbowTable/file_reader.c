/*file_reader.c*/
#include <stdio.h>
#include <stdlib.h>

int readPassFile(char* filename, int maxCharacters) {
    FILE *fp;
    int i = 0;
    char line[maxCharacters+1];
    
    if (filename == NULL) {
   	 return 0;
    }

    if ( (fp = fopen(filename,"r")) == NULL) {
   	 printf("Cannot open a file containing passwords!\n");
   	 return 0;
    }

    while ( feof(fp) == 0 ) {
   	 fgets(line,sizeof(line),fp);
   	 printf("Zczytano haslo: %s\n",line);
   	 //TODO zapisanie hasla do tablicy
    }

    fclose(fp);
    return 1;
}
