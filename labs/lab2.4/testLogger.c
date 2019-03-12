#include "logger.h"
#include <stdio.h>

int main () {


   infof("INFO message %d\n", 1);
   warnf("WARN message %d\n", 2);
   errorf("ERROR message %d\n", 2);

   printf("\n");

   initLogger("stdout");   
   infof("INFO message %d\n", 1);
   warnf("WARN message %d\n", 2);
   errorf("ERROR message %d\n", 2);

   printf("\n");

   initLogger("syslog");   
   infof("INFO message %d\n", 1);
   warnf("WARN message %d\n", 2);
   errorf("ERROR message %d\n", 2);
   
   return 0;
}