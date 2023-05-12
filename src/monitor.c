#include "../include/monitor.h"



/*void add_program(pid_t pid, char *program_name, time_t start_time) {
    if (num_programs >= 30) {
        fprintf(stderr, "Maximum number of programs reached\n");
        return;
    }

    Program *program = malloc(sizeof(Program)); // allocate memory for one Program structure
    if (program == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    program.pid = pid;
    strncpy(program.program_name, program_name, 50);
    program.start_time = start_time;
    program.end_time = 0;

    programs[num_programs] = *program; // copy the contents of the newly allocated program to the array
    num_programs++;

    //free(program); // free the dynamically allocated memory
}*/

void add_program(pid_t pid, char *program_name, struct timespec start_time) {
    if (num_programs >= 30) {
        fprintf(stderr, "Maximum number of programs reached\n");
        return;
    }

    programs[num_programs].pid = pid;
    strncpy(programs[num_programs].program_name, program_name, 49); // Copy up to 49 characters to avoid buffer overflow
    programs[num_programs].program_name[49] = '\0'; // Ensure the string is null-terminated
    programs[num_programs].start_time = start_time;
    programs[num_programs].end_time.tv_sec = 0;
    programs[num_programs].end_time.tv_nsec = 0;

    num_programs++;
}

void add_endtime_to_program(pid_t pid, struct timespec end_time) {
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
/*void print_programs() {
    for (int i = 0; i < number_of_programs_in_list(); i++) {
        printf("Program %s with PID %d started at %s", programs[i].program_name, programs[i].pid, ctime(&programs[i].start_time));
        if (programs[i].end_time != 0) {
            printf("Program %s with PID %d ended at %s", programs[i].program_name, programs[i].pid, ctime(&programs[i].end_time));
        }
    }
}*/

// function to remove a program from the array given its PID
/*void remove_program(pid_t pid) {
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
}*/

/*int number_of_programs_in_list(){//ver quantos programas estão na lista

    int n=0;

    for(int i =0; programs[i]!=NULL ; i++){

        n++;
    }

    return n;

}*/

int main(int argc, char** argv){
    printf("Abri o fifo para leitura\n");

    int fifo = mkfifo("monitor_to_tracer", 0600);
    if(fifo == -1){
        perror("Erro na criação do FIFO.");
        // exit(1);
    }

    //create a file with the name of the PID the program that was executed and that will saved/keeped in the folder PIDS-FOLDER
    char pid_file_name[50];
    // sprintf(pid_file_name, "PIDS-FOLDER/%d", getpid());



    // int flag=0;
    while(1){
        char* buffer = malloc(1000*sizeof(char));

        int fd_rd_ClientToServer = open("tracer_to_monitor", O_RDONLY);
        if(fd_rd_ClientToServer <0) perror("fd1");

        read(fd_rd_ClientToServer,buffer,1000);

        close(fd_rd_ClientToServer);

        if(strlen(buffer) == 0){
            continue;
        }

        //printf("buffer:%s\n",buffer);
        char * temp1 = strdup(buffer);

        //temp1 = strtok(temp1, "\0");
        char * typeofservice = strtok(strdup(temp1), " ");

        printf ( "Typeofservice: %s | ",typeofservice);
        

        if(strcmp(typeofservice,"3")==0){
            printf("A imprimir status ...\n");
            printf("Existem %d programas na lista!\n",num_programs);

            int fd_wr_ServerToClient = open("monitor_to_tracer", O_WRONLY);
            if(fd_wr_ServerToClient<0) perror("fd2");

            int flag_running_progs = 0;

            for(int i=0; i<num_programs;i++){
                char msg[50];
                if(programs[i].end_time.tv_sec == 0){

                    flag_running_progs = 1;

                    struct timespec current_time;
                    clock_gettime(CLOCK_REALTIME, &current_time);

                    double elapsed_time_ms = (current_time.tv_sec - programs[i].start_time.tv_sec) * 1000.0 + (current_time.tv_nsec - programs[i].start_time.tv_nsec) / 1000000.0;

                    //double elapsed_time = (current_time.tv_sec - programs[i].start_time.tv_sec) + (current_time.tv_nsec - programs[i].start_time.tv_nsec) / 1000000000.0;
                    
                    printf("Program %s, Pid: %d, Em execução: %f ms\n",programs[i].program_name, programs[i].pid, elapsed_time_ms);
                    
                    sprintf(msg,"%d %s %.3f ms\n",  programs[i].pid, programs[i].program_name, elapsed_time_ms);   
                }else{
                    
                    //double elapsed_time = (current_time.tv_sec - programs[i].start_time.tv_sec) + (current_time.tv_nsec - programs[i].start_time.tv_nsec) / 1000000000.0;
                    //double elapsed_time = (programs[i].end_time.tv_sec - programs[i].start_time.tv_sec) + (programs[i].end_time.tv_nsec - programs[i].start_time.tv_nsec) / 1000000000.0;
                    printf("Program: %s com Pid: %d já foi Terminado!\n",programs[i].program_name, programs[i].pid);
                    
                }

                int write_res = write(fd_wr_ServerToClient, msg, strlen(msg)+1);

                if (write_res == -1) {
                    perror("write");
                    // Lide com o erro de escrita conforme necessário
                }
            }

            char msgerror[50];

            if(!flag_running_progs){
                sprintf(msgerror, "Não existem programas a executar!\n");
                int write_res = write(fd_wr_ServerToClient, msgerror, strlen(msgerror)+1);

                if (write_res == -1) {
                    perror("write");
                    // Lide com o erro de escrita conforme necessário
                }

            }

            close(fd_wr_ServerToClient);

        } else if(strcmp(typeofservice,"1")==0){

            char * argPidProgram = strtok(NULL, " ");
            char * argSecond = strtok(NULL, " ");//nome programa ou end_time
            char *argThird = strtok(NULL, " ");
            struct timespec start_time;
            start_time.tv_sec = atoi(argThird); // Converte string para long int
            start_time.tv_nsec = 0; // Define nanosegundos como zero

        add_program(atoi(argPidProgram), argSecond, start_time); // Converte string para int

            printf("Programa adicionado à lista | Pid: %s | Nome: %s | tInicial: %s\n",argPidProgram,argSecond,argThird);

            //print_programs();
        } 

        else if(strcmp(typeofservice,"2")==0){

            char * argPidProgram = strtok(NULL, " ");
            char * argSecond = strtok(NULL, " ");//nome programa ou end_time
        //char * argThird = strtok(NULL, " ");
            
            struct timespec end_time;
            end_time.tv_sec = atoi(argSecond); // Converte string para long int
            end_time.tv_nsec = 0; // Define nanosegundos como zero

            add_endtime_to_program(atoi(argPidProgram), end_time); // Converte string para int

            printf("Programa com pid: %s foi Terminado!\n",argPidProgram);
        } 


    }
    return 0;



}