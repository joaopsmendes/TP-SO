#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_PROGRAMS 30;

typedef struct {
    pid_t pid;                  // ID do processo
    char program_name[50];      // Nome do programa
    time_t start_time;          // Timestamp de início do programa
    time_t end_time;            // Timestamp de fim do programa
}Program;

Program programs[MAX_PROGRAMS];

void add_program(pid_t pid, char *program_name, struct timespec start_time);
void add_endtime_to_program(pid_t pid, struct timespec end_time);
