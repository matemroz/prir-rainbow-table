#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_writer.h"

int saveRTabToFile(char *filename, char ***rainbowTab, int tabSize) {
    FILE *fp;
    int i;
    int j;

    if (tabSize <= 0) {
        fprintf(stderr, "You must give rainbow table size.\n");
        return -1;
    }

    if (filename == NULL) {
        fprintf(stderr, "There is no filename given.\n");
        return -1;
    }

    if ((fp = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "Cannot open a file for storing rainbow table data!\n");
        return -1;
    }

    for(i = 0; i < tabSize; i++){
        fprintf(fp, "%s:%s\n", rainbowTab[i][0], rainbowTab[i][1]);
    }

    fclose(fp);

    return 0;
}
