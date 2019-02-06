#include <stdio.h>

#define MAX 1000
#define IN 1
#define OUT 0

int getline(char line[], int maxline);

main(){

	char curr_line[MAX];
    char result[5000];
    int len;
    int i = 0;
    int j;
    int state = OUT;

    while ((len = getline(curr_line, MAX)) > 0){
        j = 0;
        while(curr_line[j] != '\0'){
            if((curr_line[j] == '/' && curr_line[++j] == '*') || (curr_line[j] == '/' && curr_line[++j] == '/')){
                state = IN;
            } else if (curr_line[j] == '*' && curr_line[++j] == '/'){
                state = OUT;
            }

            if (state == OUT && curr_line[j] != '/'){
                result[i] = curr_line[j];
                ++i;
                ++j;
            } else {
                if (curr_line[j] == '\n'){
                    state = OUT;
                }
                ++j;
            }
        }
    }

    printf("%s", result);

    return 0;

}

int getline(char s[], int lim){

    int c, i;

    for(i = 0; i < lim-1 && (c = getchar()) != EOF && c!='\n'; ++i){
        s[i] = c;
    }
    s[i] = '\0';
    return i;

}
