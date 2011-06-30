/*rainbow.h*/

#define DES_CHARS_NUM 14
#define RED_CHARS_NUM 10
#define MIN_PASS_SIZE 3
#define MAX_PASS_SIZE 10

static const char alphanum[62] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char bigalphanum[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char smallalphanum[36] = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char alpha[52] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char smallalpha[26] = "abcdefghijklmnopqrstuvwxyz";
static const char bigalpha[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char ext_alphanum[95] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
static const char num[10] = "0123456789";

char *hash(char *);

char *reduce(char *, int, int, int);

char ***createRainbowTable(char *wordstab[], int, int, int, int);
