# Lab 2.4
## Jorge Dominic Márquez Muñoz, A00569034

### Logger library with syslog option implementation.
### Compilation instructions:

#### This lab includes a Makefile, so can be compiled and cleaned by
	1. make
	2. make clean
#### For manual compiling
	1. gcc -c logger.c -o logger.o
	2. gcc -c testLogger.c -o testLogger.o
	3. gcc logger.o testLogger.o -o main.o
### Testing code:
#### If compiled manually, run steps 1 and 2. If compiled with Makefile, just run step 2.
    1. ./main.o
    2. tail /var/log/syslog | grep logger (for seeing the messages in the syslog).