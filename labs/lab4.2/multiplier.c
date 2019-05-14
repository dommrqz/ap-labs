#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "logger.h"

long *readMatrix(char *filename);
long *getColumn(int col, long *matrix);
long *getRow(int row, long *matrix);
int getLock(void);
int releaseLock(int lock);
long dotProduct(long *vec1, long *vec2);
long *multiply(long *matA, long *matB);
int saveResultMatrix(long *result);
void *threadFunc(void *arg);

int NUM_BUFFERS;
long **buffers;
pthread_mutex_t *mutexes;
long *result;
pthread_t threads[2000];

struct matSruct {
	long *matA;
	long *matB;
	long *result;
	size_t posRow;
	size_t posColumn;
};

long *readMatrix(char *filename)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		errorf("Error oppening file: %s\n", filename);
		return NULL;
	}
	long *matrix;
	matrix = (long *)malloc(4000000 * sizeof(long));
	int i = 0;
	while (getline(&line, &len, fp) != -1) {
		matrix[i++] = strtol(line, NULL, 10);
	}
	free(line);
	return matrix;
}

long *getColumn(int col, long *matrix)
{
	if (col < 0 || col > 2000) {
		errorf("col must be 0>col>2000\n");
		exit(EXIT_FAILURE);
	}
	size_t shift = col - 1;
	long *currCol;
	currCol = (long *)malloc(2000 * sizeof(long));

	for (int i = 0; i < 2000; i++) {
		currCol[i] = matrix[shift];
		shift += 2000;
	}
	return currCol;
}

long *getRow(int row, long *matrix)
{
	if (row < 0 || row > 2000) {
		errorf("Try with more rows");
		exit(EXIT_FAILURE);
	}
	size_t shift = ((2 * row) - 2) * 1000;
	long *currRow;
	currRow = (long *)malloc(2000 * sizeof(long));

	for (int i = 0; i < 2000; i++) {
		currRow[i] = matrix[shift++];
	}
	return currRow;
}

int getLock(void)
{
	for (int i = 0; i < NUM_BUFFERS; i++) {
		if (pthread_mutex_trylock(&mutexes[i]) == 0)
			return i;
	}
	return -1;
}

int releaseLock(int lock)
{
	if (pthread_mutex_unlock(&mutexes[lock]) == 0) {
		return 0;
	}
	return -1;
}

long dotProduct(long *vec1, long *vec2)
{
	long result = 0;
	for (int i = 0; i < 2000; i++)
		result += vec1[i] * vec2[i];
	return result;
}

long *multiply(long *matA, long *matB)
{
	result = (long *)malloc(4000000 * sizeof(long));

	for (size_t i = 0; i < 2000; i++) {
		for (size_t j = 0; j < 2000; j++) {
			struct matSruct *currentStruct;
			currentStruct =
			    (struct matSruct *)
			    malloc(sizeof(struct matSruct));
			currentStruct->matA = matA;
			currentStruct->matB = matB;
			currentStruct->posRow = i + 1;
			currentStruct->posColumn = j + 1;
			currentStruct->result = result;
			pthread_create(&threads[j], NULL, threadFunc,
				       (void *)currentStruct);
		}

		for (size_t j = 0; j < 2000; j++)
			pthread_join(threads[j], NULL);
		fflush(stdout);
	}
	printf("\n");
	return result;
}

int saveResultMatrix(long *result)
{
	FILE *f = fopen("result.dat", "w");
	if (f == NULL) {
		errorf("error: couldn't open/create result.dat\n");
		return -1;
	}

	for (size_t i = 0; i < 4000000; i++)
		fprintf(f, "%ld\n", result[i]);

	fclose(f);
	return 0;
}

void *threadFunc(void *arg)
{
	struct matSruct *data = (struct matSruct *)arg;
	long index;

	int lock1, lock2;
	while ((lock1 = getLock()) == -1);
	while ((lock2 = getLock()) == -1);
	buffers[lock1] = getRow(data->posRow, data->matA);
	buffers[lock2] = getColumn(data->posColumn, data->matB);
	
	index = ((((data->posRow - 1) * 2000) + data->posColumn) - 1);
	data->result[index] = dotProduct(buffers[lock1], buffers[lock2]);

	free(buffers[lock1]);
	free(buffers[lock2]);
	free(arg);
	while (releaseLock(lock1) != 0);
	while (releaseLock(lock2) != 0);
	return NULL;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		errorf("Not enough arguments, use -n <number of buffers>\n");
		exit(EXIT_FAILURE);
	}

	NUM_BUFFERS = strtol(argv[2], NULL, 10);
	if (NUM_BUFFERS < 8){
		errorf("You should use more than 8 buffers.\n");
		exit(EXIT_FAILURE);
	}
	infof("Multiplying matrices using %d buffers\n", NUM_BUFFERS);

	buffers = (long **)malloc(NUM_BUFFERS * sizeof(long *));
	mutexes =
	    (pthread_mutex_t *) malloc(NUM_BUFFERS * sizeof(pthread_mutex_t));

	for (int i = 0; i < NUM_BUFFERS; i++) {
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		mutexes[i] = mutex;
		pthread_mutex_init(&mutexes[i], NULL);
	}

	infof("Reading matrices\n");

	long *matrixA, *matrixB;
	matrixA = readMatrix("matA.dat");
	matrixB = readMatrix("matB.dat");

	infof("Multiplying... (please wait a lot)\n");

	result = multiply(matrixA, matrixB);
	saveResultMatrix(result);
	infof("Result saved in result.dat\n");
	
	return 0;
}
