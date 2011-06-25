/*rainbow.h*/

#define DES_CHARS_NUM 14
#define RED_CHARS_NUM 10

char *hash(char *);

char *reduce(char *);

char ***createRainbowTable(char *wordstab[], int, int);
