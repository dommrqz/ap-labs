#include <stdio.h>


int strlen(char *str);
char* stradd(char *origin, char *addition);
int strfind(char *origin, char *substr);

main(int argc, int **argv){

    printf("%s", "Initial length: ");
    printf("%d\n", strlen(argv[1]));

    char *new_word = stradd(argv[1], argv[2]);

    printf("%s", "New string: ");
    printf("%s\n", new_word);

    printf("%s", "SubString found: ");
    if(strfind(new_word, argv[3]) == 1){
        printf("%s", "yes");
    } else {
        printf("%s", "no");
    }

    return 0;
}

