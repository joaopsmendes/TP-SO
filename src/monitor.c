#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define MAX_PROGRAMS 30

typedef struct {
    pid_t pid;                  // ID do processo
    char program_name[50];      // Nome do programa
    time_t start_time;          // Timestamp de início do programa
    time_t end_time;            // Timestamp de fim do programa
}Program;

Program programs[30];
// para gaurdar a informacao

int num_programs = 0;

void add_program(pid_t pid, char *program_name, time_t start_time) {
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

void add_endtime_to_program(pid_t pid, time_t end_time) {
    if (num_programs >= 30) {
        fprintf(stderr, "Maximum number of programs reached\n");
        return;
    }

    for(int i=0; i<30; i++){
        if(programs[i].pid == pid){

            programs[i].end_time = end_time;

        }
    }
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
}

int number_of_programs_in_list(){//ver quantos programas estão na lista

    int n=0;

    for(int i =0; i<30; i++){

        if(programs[i].pid>0) n++;
    }

    return n;

}

int main(int argc, char** argv){
    //int fd = open("myfifo", O_RDONLY);
    printf("Abri o fifo para leitura\n");

    int fifo = mkfifo("monitor_to_tracer", 0600);
        if(fifo == -1){
            perror("Erro na criação do FIFO.");
        // exit(1);
    }

    int fd_rd_ClientToServer = open("tracer_to_monitor", O_RDONLY);
    if(fd_rd_ClientToServer <0) perror("fd1");

    int fd_wr_ServerToClient = open("monitor_to_tracer", O_WRONLY);
    if(fd_wr_ServerToClient<0) perror("fd2");

    //int fd_log = dopen("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);

    //struct programas
    

    int res;
    char* buffer = malloc(200*sizeof(char));
    while((res=read(fd_rd_ClientToServer,buffer,200))>0){
        //printf("buffer :%s",buffer);
        char * temp1 = strdup(buffer);
       
        //temp1 = strtok(temp1, "\0"); 
        char * typeofservice = strtok(strdup(temp1), " ");

        char * argPidProgram = strtok(NULL, " ");
        char * argSecond = strtok(NULL, " ");//nome programa ou end_time
        char * argThird = strtok(NULL, " ");
        printf ( "typeofservice: %s!\n",typeofservice);

        if(strcmp(typeofservice,"status")==0){

            struct timeval current_time;
            gettimeofday(&current_time, NULL);

            Program statusMsg[30];
            printf( " imprimir status \n");

            for(int i=0; i<number_of_programs_in_list();i++){

                long int elapsed_time = (current_time.tv_sec - programs[i].start_time) * 1000 + (current_time.tv_usec - programs[i].start_time) / 1000;//é preciso tv.sec no start_time?
                //printf("Ended in %ld ms\n", argv[3],getpid(),elapsed_time);
                

                char msg[100];
                sprintf(msg, "Program %d, Pid: %d, Em execução:  %ld ms",argv[3], getpid(),elapsed_time);

                write(fd_wr_ServerToClient,msg,strlen(msg)+1);//enviar linha por linha para o cliente!

            }

            

           /* for(int i = 0; i<30; i++){
                
                if(programs[i].end_time == 0){
                    strcpy(statusMsg[i].program_name,programs[i].program_name);
                    statusMsg[i].pid=programs[i].pid;
                    statusMsg[i].start_time= programs[i].start_time;
                }
            }*/

            //sprintf(statusMsg, "%s", );

        //printf( " a enviar "); 
        //write(fd_wr_ServerToClient,statusMsg,strlen(statusMsg+1));

        //printf("Mensagem enviada ao cliente! %s\n ",statusMsg);
        
        }

        else if(strcmp(typeofservice,"1")==0){

            //adicionar programa à lista apenas com tempo inicial (ainda em execução).
            add_program(argPidProgram,argSecond,argThird);
            printf("Programa adicionado à lista | Pid: %s | Nome: %s | tInicial: %s\n",argPidProgram,argSecond,argThird);
        } 

        else if(strcmp(typeofservice,"2")==0){
            //acrescentar end time ao programa
            add_endtime_to_program(argPidProgram,argSecond); //pid e end_time.
            printf("Programa com pid: %s Terminado!\n",argPidProgram);
        } 
        
       // write(1, buffer, res);
       // printf("Li %s\n", buffer);
    }
   // printf("Terminei de ler\n");
   // free(buffer);
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





