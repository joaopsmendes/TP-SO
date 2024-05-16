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

void add_program(pid_t pid, char *program_name, time_t start_time_sec, __syscall_slong_t start_time_nsec) {
    if (num_programs >= 30) {
        fprintf(stderr, "Maximum number of programs reached\n");
        return;
    }

    programs[num_programs].pid = pid;
    strncpy(programs[num_programs].program_name, program_name, 49); 
    programs[num_programs].program_name[49] = '\0'; 
    programs[num_programs].start_time.tv_sec = start_time_sec;
    programs[num_programs].start_time.tv_nsec = start_time_nsec;
    programs[num_programs].end_time.tv_sec = 0;
    programs[num_programs].end_time.tv_nsec = 0;

    num_programs++;
}

void add_endtime_to_program(pid_t pid, time_t  end_time_sec, __syscall_slong_t end_time_nsec) {
    if (num_programs >= 30) {
        fprintf(stderr, "Maximum number of programs reached\n");
        return;
    }

    for(int i=0; i<30; i++){
        if(programs[i].pid == pid){
            programs[i].end_time.tv_sec = end_time_sec;
            programs[i].end_time.tv_nsec = end_time_nsec;
        }
    }
}

int main(int argc, char** argv){

    if (argc < 2) {
        printf("Syntax correta: %s <ended_programs_file_path>\n", argv[0]);
        return 1;
    }

    const char* path = argv[1];

    printf("Abri o fifo para leitura\n");

    int fifo = mkfifo("monitor_to_tracer", 0600);
    if(fifo == -1){
        perror("Erro na criação do FIFO.");
        // exit(1);
    }

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

                    double elapsed_time_ms = (current_time.tv_sec - programs[i].start_time.tv_sec) * 1000.0 +
                        (current_time.tv_nsec - programs[i].start_time.tv_nsec) / 1e6;

                    double elapsed_time_sec = (current_time.tv_sec - programs[i].start_time.tv_sec) +
                          (current_time.tv_nsec - programs[i].start_time.tv_nsec) / 1e9;


                    printf("Program %s, Pid: %d, Em execução: %f ms, %f s\n",programs[i].program_name, programs[i].pid, elapsed_time_ms,elapsed_time_sec);
                    
                    sprintf(msg,"%d %s %f ms | %f s\n",  programs[i].pid, programs[i].program_name, elapsed_time_ms,elapsed_time_sec);   
                }else{

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
            char *argNSEC = strtok(NULL, " ");

            struct timespec start_time;
            start_time.tv_sec = atoi(argThird); // Converte string para long int
            start_time.tv_nsec = atoi(argNSEC); // Define nanosegundos como zero

            add_program(atoi(argPidProgram), argSecond, start_time.tv_sec, start_time.tv_nsec); // Converte string para int

            printf("Programa adicionado à lista | Pid: %s | Nome: %s | tInicial: %s\n",argPidProgram,argSecond,argThird);

            //print_programs();

        }else if(strcmp(typeofservice,"2")==0){

            char * argPidProgram = strtok(NULL, " ");
            char * argSecond = strtok(NULL, " ");//nome programa ou end_time
            char * argNSEC = strtok(NULL, " ");
            
            struct timespec end_time;
            end_time.tv_sec = atoi(argSecond); // Converte string para long int
            end_time.tv_nsec = atoi(argNSEC); // Define nanosegundos como zero

            add_endtime_to_program(atoi(argPidProgram), end_time.tv_sec, end_time.tv_nsec); // Converte string para int

            printf("Programa com pid: %s foi Terminado!\n",argPidProgram);

            for(int i=0; i<num_programs; i++){
                if(programs[i].pid == atoi(argPidProgram)){


                    double elapsed_time_ms = (end_time.tv_sec - programs[i].start_time.tv_sec) * 1000.0 + (end_time.tv_nsec - programs[i].start_time.tv_nsec) / 1000000.0;

                    //printf("elapsed time: %f \n", elapsed_time_ms);

                    char filename[50];
                    sprintf(filename, "%s/%d.txt", path, programs[i].pid);//   /pids_folder/pid.txt

                    int saida = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
                    if(saida<0) perror("error on open saida\n");

                    char msg[20];
                    sprintf(msg, "Elapsed time: %f ms\n", elapsed_time_ms);

                    write(saida, msg, strlen(msg));

                    close(filename);

                    printf("File %s created successfully.\n", filename);

                    //createElapsedFile(path,programs[i].pid, elapsed_time_ms);

                    break;

                }
            }
        }else if(strcmp(typeofservice,"4")==0){//status-time sprintf(statusMsg, "%d %d %s",4,count, pids_string);


            char * max_pids = strtok(NULL, " ");

            int* pid_array = (int*)malloc(atoi(max_pids) * sizeof(int));

            int pid_array_size=0;

            char * pid_token = strtok(NULL, " ");

            while (pid_token != NULL && pid_array_size < max_pids) {
                pid_array[pid_array_size++] = atoi(pid_token);
                pid_token = strtok(NULL, " ");
            }

            double max_time =0;


            for(int i=0; i<num_programs;i++){

                for (int j = 0; j < pid_array_size; j++) {

                    //printf("PID %d: %d\n", i + 1, pid_array[i]);

                    if((programs[i].pid == pid_array[j]) && (programs[i].end_time.tv_sec != 0) ){

                        //flag_running_progs = 1;

                        double elapsed_time_ms = (programs[i].end_time.tv_sec - programs[i].start_time.tv_sec) * 1000.0 +
                            (programs[i].end_time.tv_nsec - programs[i].start_time.tv_nsec) / 1e6;

                        //printf("elapsed time | status-time = %f \n",elapsed_time_ms);

                        max_time += elapsed_time_ms;

                        //printf("Program %s, Pid: %d, Em execução: %f ms, %f s\n",programs[i].program_name, programs[i].pid, elapsed_time_ms,elapsed_time_sec);
                          
                    }
                }
            }
            


            printf("A imprimir status time..\n");
            //printf("Existem %d programas na lista!\n",num_programs);

            int fd_wr_ServerToClient_status_time = open("monitor_to_tracer", O_WRONLY);
            if(fd_wr_ServerToClient_status_time<0) perror("fd2");



            char msg[50];
            sprintf(msg, "Total execution time is: %f ms\n", max_time);

            int write_res = write(fd_wr_ServerToClient_status_time, msg, strlen(msg)+1);

                if (write_res == -1) {
                    perror("write");
                    // Lide com o erro de escrita conforme necessário
                }

            close(fd_wr_ServerToClient_status_time);

        }else if(strcmp(typeofservice,"5")==0){//5 - status-command   typo 5, nome_prog, numero de pids, array pids


            char * nomeProg = strtok(NULL, " ");
            char * maxPids = strtok(NULL, " ");



            int* pid_array = (int)malloc(atoi(maxPids) * sizeof(int));

            int pid_array_size=0;

            char * pid_token = strtok(NULL, " ");

            while (pid_token != NULL && pid_array_size < maxPids) {
                pid_array[pid_array_size++] = atoi(pid_token);
                pid_token = strtok(NULL, " ");
            }

            int vezes_executado =0;


            for(int i=0; i<num_programs;i++){

                for (int j = 0; j < pid_array_size; j++) {

                    //printf("PID %d: %d\n", i + 1, pid_array[i]);

                    if((programs[i].pid == pid_array[j]) && (programs[i].program_name == nomeProg) ){


                        vezes_executado ++;

                        //printf("Program %s, Pid: %d, Em execução: %f ms, %f s\n",programs[i].program_name, programs[i].pid, elapsed_time_ms,elapsed_time_sec);

                    }
                }
            }



            printf("A imprimir status-command..\n");
            //printf("Existem %d programas na lista!\n",num_programs);

            int fd_wr_ServerToClient_status_command = open("monitor_to_tracer", O_WRONLY);
            if(fd_wr_ServerToClient_status_command<0) perror("fd2");



            char msg[50];
            sprintf(msg, " %s Was execued %d times\n", vezes_executado);

            int write_res = write(fd_wr_ServerToClient_status_command, msg, strlen(msg)+1);

                if (write_res == -1) {
                    perror("write");
                    // Lide com o erro de escrita conforme necessário
                }

            close(fd_wr_ServerToClient_status_command);
        }
    }
    
    return 0;
}