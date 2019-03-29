#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

char result[500][500];

FILE *command;
FILE *curr_command_result;
void clear();
void getTop();
void printTable();
void printTableDocument();

struct process {
  char pid[10];
  char ppid[5];
  char name[30];
  char state[30];
  int memory;
  char threads[30];
  int files;
};

struct process processes[1000];

int process_count = 0;

int main(){
  getTop();
  printTable();
  //clear();
  return 0;
}

void clear() {
  printf("\e[1;1H\e[2J"); 
}

void getTop(){
  int c, m, r, word_count = 0, memory_count = 0, i, open_files = 0;
  char current_process[5];
  char memory[10];
  char res[11];

  command = (FILE *)popen("ls /proc | grep -Eo '[0-9]{1,4}'", "r");

  while ((c = fgetc(command)) != EOF) {

    if (signal(SIGINT, printTableDocument) == SIG_ERR)
      exit("Error in SIGINT");

    if (c == '\n'){
      strcpy(processes[process_count].pid, current_process);
     char *line = malloc(1024);
     word_count=0;

     processes[process_count].memory = 0;
     processes[process_count].files = 0;
     
     sprintf(line, "cat /proc/%s/status | grep PPid | awk '{printf($2); next}1'\n", current_process);
     curr_command_result = popen(line, "r");
     fgets(res, sizeof(res), curr_command_result);
     strcpy(processes[process_count].ppid, res);
     free(line);

    *line = malloc(1024);
     sprintf(line, "cat /proc/%s/status | grep Name | awk '{printf($2); next}1'\n", current_process);
     curr_command_result = popen(line, "r");
     fgets(res, sizeof(res), curr_command_result);
     strcpy(processes[process_count].name, res);
     free(line);

     *line = malloc(1024);
     sprintf(line, "cat /proc/%s/status | grep State | awk '{printf($3); next}1'\n", current_process);
     curr_command_result = popen(line, "r");
     fgets(res, sizeof(res), curr_command_result);
     strcpy(processes[process_count].state, res);
     free(line);

     *line = malloc(1024);
     sprintf(line, "cat /proc/%s/status | grep Threads | awk '{printf($2); next}1'\n", current_process);
     curr_command_result = popen(line, "r");
     fgets(res, sizeof(res), curr_command_result);
     strcpy(processes[process_count].threads, res);
     free(line);

     *line = malloc(1024);
     sprintf(line, "cat /proc/%s/status | grep -E 'Vm|Rss|Huge' | awk '{print $2}'\n", current_process);
     curr_command_result = popen(line, "r");
     while((m = fgetc(curr_command_result)) != EOF){
       if (m == '\n'){
         processes[process_count].memory = processes[process_count].memory + atoi(memory);
        memset(memory, 0, sizeof(memory));
         memory_count=0;
       }else {
         memory[memory_count] = m;
         memory_count++;
       }
     }
     pclose(curr_command_result);
     free(line);

     *line = malloc(1024);
     sprintf(line, "sudo ls /proc/%s/fd\n", current_process);
     curr_command_result = popen(line, "r");
     while((r = fgetc(curr_command_result)) != EOF){
       if (r == '\n'){
         open_files++;
       }
     }
     processes[process_count].files = open_files;
     open_files = 0;
     pclose(curr_command_result);
     free(line);

     memset(current_process, 0, sizeof(current_process));
     process_count++;
    } else {
      current_process[word_count] = c;
      word_count++;
    }
  }
  pclose(command);

}

void printTable(){
  int i;
  printf("+-------+--------+------------------------------------------+------------+----------+-----------+------------+\n");
  printf("|  PID  | Parent |                   Name                   |   State    |  Memory  | # Threads | Open Files |\n");
  printf("+-------+--------+------------------------------------------+------------+----------+-----------+------------+\n");
    for (i = 0; i < process_count; i++) {
    printf("| %-5s | %-6s | %-40s | %-10s | %-7dM | %-9s | %-10d |\n", processes[i].pid, processes[i].ppid, processes[i].name, processes[i].state, processes[i].memory/1000, processes[i].threads, processes[i].files);
    }
  printf("+-------+--------+------------------------------------------+------------+----------+-----------+------------+\n");
}

void printTableDocument(){
  char filename[40];
  struct tm *timenow;

  time_t now = time(NULL);
  timenow = gmtime(&now);

  strftime(filename, sizeof(filename), "mytop-status-%d-%m-%Y", timenow);

  FILE *f = fopen(filename, "wb");
  int i;
  fprintf(f, "+-------+--------+------------------------------------------+------------+----------+-----------+------------+\n");
  fprintf(f, "|  PID  | Parent |                   Name                   |   State    |  Memory  | # Threads | Open Files |\n");
  fprintf(f, "+-------+--------+------------------------------------------+------------+----------+-----------+------------+\n");
    for (i = 0; i < process_count; i++) {
    fprintf(f, "| %-5s | %-6s | %-40s | %-10s | %-7dM | %-9s | %-10d |\n", processes[i].pid, processes[i].ppid, processes[i].name, processes[i].state, processes[i].memory/1000, processes[i].threads, processes[i].files);
    }
  fprintf(f, "+-------+--------+------------------------------------------+------------+----------+-----------+------------+\n");
  fclose(f);
}