#include <stdio.h>
#include <stdarg.h>

#define RESET       0
#define BRIGHT      1
#define DIM         2
#define UNDERLINE   3
#define BLINK       4
#define REVERSE     7
#define HIDDEN      8

#define BLACK       0
#define RED         1
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7

int infof(const char *format, ...) {
    int result;
    va_list args;
    va_start(args, format);
    result = vfprintf(stdout, format, args);
    va_end(args);
    return result;
}

int warnf(const char *format, ...) {
    textcolor(BRIGHT, YELLOW);  
    int result;
    va_list args;
    va_start(args, format);
    fputs("Warning: ", stdout);
    result = vfprintf(stdout, format, args);
    va_end(args);
    textcolor(RESET, WHITE);  
    return result;
}

int errorf(const char *format, ...) {
    textcolor(BRIGHT, RED);  
    int result;
    va_list args;
    va_start(args, format);
    fputs("Error: ", stdout);
    result = vfprintf(stdout, format, args);
    va_end(args);
    textcolor(RESET, WHITE);
    return result;
     
}

int panicf(const char *format, ...) {
    textcolor(BRIGHT, CYAN);  
    int result;
    va_list args;
    va_start(args, format);
    fputs("Panic: ", stdout);
    result = vfprintf(stdout, format, args);
    va_end(args);
    textcolor(BRIGHT, CYAN);  
    return result;
}

void textcolor(int attr, int fg){   
    
    char command[13];

    sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
    printf("%s", command);
    
}