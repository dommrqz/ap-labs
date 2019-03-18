compile:
	gcc -c logger.c -o logger.o
	gcc -c monitor.c -o monitor.o
	gcc logger.o monitor.o -o main.o
	./main.o .

clean: 
	rm logger.o
	rm monitor.o
	rm main.o