# Lab 3.1
## Jorge Dominic Márquez Muñoz, A00569034

### File event monitorin within a directory (passed as an arg) and all its subdirectories.
### Compilation instructions:

#### This lab includes a Makefile, so can be compiled and cleaned by
	1. make
	2. make clean
#### For manual compiling
	1. gcc -c logger.c -o logger.o
	2. gcc -c monitor.c -o monitor.o
	3. gcc logger.o monitor.o -o main.o
### Testing code:
#### If compiled manually, run step from step 1 to 10. If compiled with Makefile, run from step 2 to 10.
    1. ./main.o [directory | .]
    2. touch file
    3. echo "Hello" >> file
    4. rm file
    4. mkdir directory
    5. cd directory
    6. touch file
    7. echo "Hello" >> file
    8. rm file
    9. cd ..
    10. rmdir directory
#### Considerations
	1. If compiled by Makefile, the default is that main.o will monitor the directory where it is located and all the subdirectories.
    2. If compiled manually, you can pass another directory (full path) to monitor or "." to monitor the directory where main.o is located.
    3. Steps 2 to 10 of testing should be executed in a new terminal, not in the one main.o is executing.
    4. Steps 2 to 10 should be executed in the directory that main.o is monitoring, and in any of its subdirectories.
    5. Messages will appear in the terminal where main.o is executing.
    6. Monitor is sensitive to new directories. This means that, if a new directory is created in the directory or any subdirectory that main.o is monitoring, main.o will also monitor it.