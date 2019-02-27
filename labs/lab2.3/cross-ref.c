#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CHAR_SIZE 1
#define SIZE_BUF 65000
#define SIZE_NOISY 22
#define SIZE_WORDS 65000

char * readFile(char *nameFile);
int isNoisyWord(char * word);
void getWordLines(char * buffer);
int isAdded(char *word, int wordCount);
void addLine(int index, int line);
void printLines(int totalWords);

char *noisyWords[] = {
	    "and",  
	    "as", 
	    "at",
	    "because",
	    "but",
	    "for",
	    "has",
	    "if",
	    "in",
	    "into",
	    "is",
	    "of",
	    "on",
	    "or", 
	    "so",
	    "the",
	    "to",
	    "too",
	    "with",
	    "by",
	    "i",
  	    "am",
	    "are",
        "a",
        "an",
		"so"
 };

struct word{
	 char word[30];
	 int lines[100];
 };

struct word words[SIZE_WORDS];

int main(int argc, char *argv[]){
	char *nameFile= argv[1];
	char *buffer= (char *)malloc(SIZE_BUF);
	buffer = readFile(nameFile);
	getWordLines(buffer);
	free(buffer);
}

char * readFile(char *nameFile){
	int c;
	FILE *fp = fopen(nameFile, "r");

	if (fp == NULL){
      		perror("Error while opening the file.\n");
      		exit(EXIT_FAILURE);
   	}
	
	char *buffer= (char *)malloc(SIZE_BUF);
	fread(buffer, CHAR_SIZE, SIZE_BUF, fp);
	fclose(fp);
	return buffer;
}

void getWordLines(char * buffer){
	int i;
	int wordIndex;
	int line = 1;
	int tempCount = 0;
	int wordCount = 0;
	char *temp= (char *)malloc(30);
	for(i=0; i < SIZE_BUF; i++){
		if((((buffer[i] >= 32) && (buffer[i] <= 47)) || ((buffer[i] >= 58) && (buffer[i] <= 64)))|| ((buffer[i] >= 91) && (buffer[i] <= 96)) || buffer[i] == '\n'){
            if(isNoisyWord(temp) == 0 && tempCount > 2){
				wordIndex = isAdded(temp, wordCount);
				if(wordIndex == -1){
					//printf(" ");
					strcpy(words[wordCount].word, temp);
					addLine(wordCount, line);
					wordCount++;
				} else {
					addLine(wordIndex, line);
				}
			}

            if(buffer[i] == '\n'){
			    line++;			
		    }

			tempCount = 0;
		}else{
			temp[tempCount] = tolower(buffer[i]);
			tempCount++;
		}
	}
	free(temp);
	printLines(wordCount);

}

int isNoisyWord(char *word){
	int i;
	for(i=0; i < SIZE_NOISY; i++){
		if(strcmp(word, noisyWords[i]) == 0){
			return 1;
		}
	}
	return 0;
}

int isAdded(char *word, int wordCount){
	int i;
	for(i= 0; i < wordCount; i++){
		if(strcmp(word, words[i].word) == 0){
			return i;
		}
	}
	return -1;
}


void addLine(int index, int line){
	int i;
	for(i = 0; i < 1000; i++){
		if (words[index].lines[i] == 0){
			words[index].lines[i] = line;
			break;
		} else if (words[index].lines[i] == line){
			break;
		}
	}
}

void printLines(int totalWords){
	int i;
	int j;
	for(i = 0; i < totalWords; i++){
		printf("%s\n", words[i].word);
		printf("Lines: ");
		for(j = 0; words[i].lines[j] != 0; j++){
			printf("%d ", words[i].lines[j]);
		}
		printf("\n\n");
	}
}