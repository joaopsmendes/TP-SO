#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_PROGRAMS 30;
// para gaurdar a informacao

typedef struct {
    pid_t pid;                  // ID do processo
    char program_name[50];      // Nome do programa
    time_t start_time;          // Timestamp de início do programa
    time_t end_time;            // Timestamp de fim do programa
}Program;

Program programs[30];
int num_programs = 0;

void add_program1(pid_t pid, char *program_name, time_t start_time) {
    if (num_programs >= 30) {
        fprintf(stderr, "Maximum number of programs reached\n");
        return;
    }
    // add the program to the array
    programs[num_programs].pid = pid;
    
    strncpy(programs[num_programs].program_name, program_name, 50);

    programs[num_programs].start_time = start_time;

    programs[num_programs].end_time = 0;

    num_programs++;
}






// function to print information about all programs
void print_programs() {
    for (int i = 0; i < num_programs; i++) {
        printf("Program %s with PID %d started at %s", programs[i].program_name, programs[i].pid, ctime(&programs[i].start_time));
        if (programs[i].end_time != 0) {
            printf("Program %s with PID %d ended at %s", programs[i].program_name, programs[i].pid, ctime(&programs[i].end_time));
        }
    }
}

// function to remove a program from the array given its PID
void remove_program(pid_t pid) {
    for (int i = 0; i < num_programs; i++) {
        if (programs[i].pid == pid) {
            // shift all programs after this one back by one position in the array
            for (int j = i; j < num_programs - 1; j++) {
                programs[j] = programs[j + 1];
            }
            num_programs--;
            break;
        }
    }

int main(int argc, char** argv){
    //int fd = open("myfifo", O_RDONLY);
    //printf("Abri o fifo para leitura\n");

    int fd_rd_ClientToServer = open("myfifo", O_RDONLY);
    if(fd_rd_clientToServer<0) perror("fd1");

    int fd_wr_ClientToServer = open("myfifo", O_WRONLY);
    if(fd_wr_ClientToServer<0) perror("fd2");

    //int fd_log = dopen("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);

    //satruct programas

    

    int res;
    char* buffer = malloc(200*sizeof(char));
    while((res=read(fd,buffer,200))>0){

        char * temp1 = strdup(buffer);

        temp1 = strtok(temp1, "\n"); 
        char * typeofservice = strtok(strdup(temp1), " ");
        
        if(strcmp(typeofservice,"status")==0){

            char statusMsg[20];

            sprintf(statusMsg, "%s", );

            write(fd,statusMsg,strlen(statusMsg+1));

            
        }

        //

        if(strcmp(typeofservice,"1")==0){
            //add p pid, tI, nome
            

        } 

        //783464 ls 12:00



        if(strcmp(typeofservice,"2")==0){
            //acrescentar end time ao programa

        } 

        





        

        write(1, buffer, res);
        printf("Li %s\n", buffer);
    }
    printf("Terminei de ler\n");
    free(buffer);
    //close(fd_log);
    return 0;

   // int res;
   // char* buffer = malloc(50*sizeof(char));
   // while((res=read(0,buffer,50))>0){
   //     write(fd,buffer, res);
   //     printf("Escrevi\n");
   // }
   // 
   // return 0;
}
}