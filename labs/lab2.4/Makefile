compile:
	gcc -c logger.c -o logger.o
	gcc -c testLogger.c -o testLogger.o
	gcc logger.o testLogger.o -o main.o
	./main.o

clean: 
	rm logger.o
	rm testLogger.o
	rm main.o