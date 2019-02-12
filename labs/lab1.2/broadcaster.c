#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

main(int argc, int **argv){

    int fp;
    int i;
    char str[512];
    char result[10];
    FILE *command;
    int sessions;
    char file[512];

    command = popen("who | grep pts | cut -d ' ' -f 3 | cut -d '/' -f 2 | tail -1", "r");
    sessions = atoi(fgets(result, sizeof(result), command));
    pclose(command);

    str[0] = '\0';

    for(i = 1; i < argc; i++){
        strcat(str, argv[i]);
        strcat(str, " ");
    }

    for(i = 0; i < sessions + 1; i++){
        sprintf(file, "/dev/pts/%d", i);
        fp = open(file, O_WRONLY);
        write(fp, str, strlen(str));
        close(fp);
    }

    return 0;
}