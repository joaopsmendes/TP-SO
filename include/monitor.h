#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#define MAX_PROGRAMS 30

typedef struct program{
    pid_t pid;                  // ID do processo
    char program_name[50];      // Nome do programa
    struct timespec start_time;          // Timestamp de início do programa
    struct timespec end_time;            // Timestamp de fim do programa
}Program;

Program programs[MAX_PROGRAMS];

int num_programs = 0;

void add_program(pid_t pid, char *program_name, struct timespec start_time);
void add_endtime_to_program(pid_t pid, struct timespec end_time);
