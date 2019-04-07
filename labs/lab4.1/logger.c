#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <syslog.h>

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

int type = 1;

int initLogger(char *logType){
    if(strcmp(logType, "stdout") == 0){
        type = 1;
    } else if (strcmp(logType, "syslog") == 0){
        type = 2;
    } else if (strlen(logType) == 0){
        logType = "stdout";
        type = 1;
    }
    printf("Initializing logger on %s\n", logType);
    return 0;
}

int infof(const char *format, ...) {
    if (type == 1){
        int result;
        va_list args;
        va_start(args, format);
        result = vfprintf(stdout, format, args);
        va_end(args);
        return result;
    } else if (type == 2){
        openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, format);
        closelog();
    }
}

int warnf(const char *format, ...) {
    if(type == 1){
        textcolor(BRIGHT, YELLOW);  
        int result;
        va_list args;
        va_start(args, format);
        fputs("Warning: ", stdout);
        result = vfprintf(stdout, format, args);
        va_end(args);
        textcolor(RESET, WHITE);  
        return result;
    } else if (type == 2){
        openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_WARNING, format);
        closelog();
    }
}

int errorf(const char *format, ...) {
    if(type == 1){
        textcolor(BRIGHT, RED);  
        int result;
        va_list args;
        va_start(args, format);
        fputs("Error: ", stdout);
        result = vfprintf(stdout, format, args);
        va_end(args);
        textcolor(RESET, WHITE);
        return result;
    } else if (type == 2){
        openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_ERR, format);
        closelog();
    } 
}

int panicf(const char *format, ...) {
    if(type == 1){
        textcolor(BRIGHT, CYAN);  
        int result;
        va_list args;
        va_start(args, format);
        fputs("Panic: ", stdout);
        result = vfprintf(stdout, format, args);
        va_end(args);
        textcolor(BRIGHT, CYAN);  
        return result;
    } else if (type == 2){
        openlog("logger", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, format);
        closelog();
    }
}

void textcolor(int attr, int fg){   
    
    char command[13];

    sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
    printf("%s", command);
    
}