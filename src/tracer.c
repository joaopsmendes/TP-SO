#include "../include/tracer.h"


char* strtrim(char* str) {
    // Remove espaços em branco à esquerda
    while (*str == ' ') {
        str++;
    }
    // Remove espaços em branco à direita
    char* end = str + strlen(str) - 1;
    while (end > str && *end == ' ') {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}


int execute_commands(char* arg){
    char * exec_args[10];

    int i = 0;
    char* string;
    char* command = strdup(arg);

    while((string = strsep(&command, " ")) != NULL){
        exec_args[i] = string;
        i++;
    }

    exec_args[i] = NULL;

    return execvp(exec_args[0], exec_args);
}



int main(int argc, char** argv){

    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);


    int fifo = mkfifo("tracer_to_monitor", 0600);
        if(fifo == -1){
            perror("Erro na criação do FIFO.");
        // exit(1);
        }
        int fd = open("tracer_to_monitor", O_WRONLY,0600);
        printf("Abri o FIFO para escrita\n");
        int p[2];
        pipe(p);

    int i=0,a ;
    char **token = malloc((argc-2)*sizeof(char));
    if (strcmp(argv[1], "execute") == 0) {
        if (strcmp(argv[2], "-u") == 0) {
            for (a = 3; a <argc; a++) {
                //printf("%s\n",argv[a]);
                token[i] = strdup((argv[a]));
                i++;
                //printf("%s\n",&token[i]);
            }
            token[i]=NULL;

            int res = fork();

            if(res==0){
                //filho

                execvp(argv[3],token);
            
                _exit(1);
            }else{ 
                struct timespec current_time;
                clock_gettime(CLOCK_REALTIME, &current_time); // funcao time 

                //enviar PID , nome do programa, timestamp

                char msg[100];
                //1 ->//print_programs();
                sprintf(msg, "%d %d %s %ld %ld",1, getpid(), argv[3], current_time.tv_sec, current_time.tv_nsec);
                
                int status;
                //char* tempo= malloc(20*sizeof(char));

                printf("Running PID: %d\n", getpid());

                write(fd,msg,100);

                waitpid(res,&status,0);

                struct timespec end_time;
                clock_gettime(CLOCK_REALTIME, &end_time);;

                //enviar msg do pid terminado e timesatamp final
                sprintf(msg, "%d %d %ld %ld",2, getpid(), end_time.tv_sec, end_time.tv_nsec);

                //printf(msg);

                write(fd,msg,100);

                double elapsed_time_ms = (end_time.tv_sec - current_time.tv_sec) * 1000.0 + (end_time.tv_nsec - current_time.tv_nsec) / 1000000.0;

                printf("Ended in %f ms\n",elapsed_time_ms);
                
            }

        }else if (strcmp(argv[2], "-p") == 0) {
                
            char **commands = malloc((argc - 3) * sizeof(char *));
            int i = 0;

            for (int a = 3; a < argc; a++) {
                commands[i] = strdup(argv[a]);
                i++;
            }

            // Imprimir os comandos
            // for (int j = 0; j < i; j++) {
            //     printf("%s\n", commands[j]);
            // }

            char **exec_args = malloc((i + 1) * sizeof(char *));
            i = 0;
            char *command = strdup(commands[0]);
            char *string;

            while ((string = strsep(&command, "|")) != NULL) {
                exec_args[i] = strtrim(string);
                i++;
            }
            exec_args[i] = NULL;

            char *first_command = strdup(commands[0]);
            exec_args[0] = strtrim(strsep(&first_command, "|"));

            // Imprimir exec_args
            // for (int j = 0; j < i; j++) {
            //     printf("arg[%d] =%s\n",j, exec_args[j]);
            // }



            int res = fork();

            if(res == 0){ 
                //processo filho

                int number_of_numbers = i;

                int pipes[number_of_numbers-1][2];

                for(int i = 0; i < number_of_numbers; i++){
                    if(i==0){
                        pipe(pipes[i]);
                        // if para ver se nao da erro
                        int fres = fork();
                        if(fres == 0){
                            // filho
                            close(pipes[i][0]); //fechar a leitura
                            dup2(pipes[i][1],1);
                            close(pipes[i][1]);
                            execute_commands(exec_args[i]);
                        } else {
                            // pai
                            close(pipes[i][1]); //fechar a escrita   
                        }
                    }
                    else if(i == number_of_numbers-1){
                        int fres = fork();
                        if(fres == 0){
                            // filho
                            dup2(pipes[i-1][0],0);
                            close(pipes[i-1][0]);
                            execute_commands(exec_args[i]);
                        } else {
                            // pai
                            close(pipes[i-1][0]); //fechar a leitura
                        }
                    }
                    else{
                        pipe(pipes[i]);
                        int fres = fork();
                        if(fres == 0){
                            // filho
                            dup2(pipes[i-1][0],0); //redirecionar a leitura
                            close(pipes[i-1][0]);

                            close(pipes[i][0]);
                            dup2(pipes[i][1],1); //redirecionar a escrita
                            close(pipes[i][1]);
                            execute_commands(exec_args[i]);
                        } else {
                            // pai
                            close(pipes[i-1][0]); //fechar a leitura
                            close(pipes[i][1]); //fechar a escrita
                        }
                    }

                }
                for (int i = 0; i < number_of_numbers; i++){
                    wait(NULL);
                }

                printf("Pipeline finished.\n");

            }else{
                struct timespec current_time;
                clock_gettime(CLOCK_REALTIME, &current_time); // funcao time 

                //enviar PID , nome do programa, timestamp

                char msg[100];

                sprintf(msg, "%d %d %ld %ld %s",7, getpid(), current_time.tv_sec,current_time.tv_nsec, commands[0]);

                //printf(msg);
                
                int status;
                //char* tempo= malloc(20*sizeof(char));

                printf("Running PID: %d\n", getpid());

                write(fd,msg,100);

                waitpid(res,&status,0);

                struct timespec end_time;
                clock_gettime(CLOCK_REALTIME, &end_time);;

                //enviar msg do pid terminado e timesatamp final
                sprintf(msg, "%d %d %ld %ld",2, getpid(), end_time.tv_sec, end_time.tv_nsec);

                write(fd,msg,100);

                double elapsed_time_ms = (end_time.tv_sec - current_time.tv_sec) * 1000.0 + (end_time.tv_nsec - current_time.tv_nsec) / 1000000.0;

                printf("Ended in %.3f ms\n",elapsed_time_ms);

             }

        }else{

            printf("Comando não reconhecido!\n");

        }

    }else if (strcmp(argv[1], "status") == 0) {

        char statusMsg[20];
        sprintf(statusMsg, "%d %s",3, argv[1]);
        printf("Status enviado ao servidor!\n");
        write(fd,statusMsg,20);

        //char* buffer = malloc(128*sizeof(char));

        int fd_rd_ServertoClient = open("monitor_to_tracer",O_RDONLY,0600);
        if(fd_rd_ServertoClient <0) perror("fd1");

        char* buffer = (char*)malloc(50);
        int bytesRead;

        while ((bytesRead = read(fd_rd_ServertoClient, buffer, 50)) > 0) {
            int bytesWritten = write(1, buffer, strlen(buffer)+1);

            if (bytesWritten != bytesRead) {
                break;
            }
        }

        free(buffer);
        close(fd_rd_ServertoClient);

    }else if (strcmp(argv[1], "stats-time") == 0) {

        if (argc < 3) {
            printf("Syntax correta: %s status-time <pid1> <pid2> ...\n", argv[0]);
            return 1;
        }

        int pidString = (argc - 2) * 10;
        char* pids_string = (char*)malloc(pidString * sizeof(char));
        pids_string[0] = '\0';

        int number_of_numbers=0;

        for (int i = 2; i < argc; i++) {
            sprintf(pids_string + strlen(pids_string), "%s ", argv[i]);
            number_of_numbers++;
        }

        char statusMsg[100];
        sprintf(statusMsg, "%d %d %s",4,number_of_numbers, pids_string);//4 - status-time
        printf("Status-time enviado ao servidor!\n");
        write(fd,statusMsg,100);

        //char* buffer = malloc(128*sizeof(char));

        int fd_rd_ServertoClient_status_time = open("monitor_to_tracer",O_RDONLY,0600);
        if(fd_rd_ServertoClient_status_time <0) perror("fd1");

        char* buffer = (char*)malloc(50);
        int bytesRead = read(fd_rd_ServertoClient_status_time, buffer, 50);

        int bytesWritten = write(1, buffer, strlen(buffer)+1);

        free(buffer);
        close(fd_rd_ServertoClient_status_time);  
        
    }else if (strcmp(argv[1], "stats-command") == 0) {

        if (argc < 3) {
            printf("Syntax correta: %s status-command <program_name> <pid1> <pid2> ...\n", argv[0]);
            return 1;
        }

        int pidString = (argc - 2) * 10;
        char* pids_string = (char*)malloc(pidString * sizeof(char));
        pids_string[0] = '\0';

        int count=0;

        //.tracer status-command 
        for (int i = 3; i < argc; i++) {
            sprintf(pids_string + strlen(pids_string), "%s ", argv[i]);
            count++;
        }

        char statusMsg[100];
        sprintf(statusMsg, "%d %s %d %s",5,argv[2],count, pids_string);//5 - status-command   typo 5, nome_prog, numero de pids, array pids

        printf(statusMsg);

        printf("Status-command enviado ao servidor!\n");
        write(fd,statusMsg,100);



        int fd_rd_ServertoClient_status_command = open("monitor_to_tracer",O_RDONLY,0600);
        if(fd_rd_ServertoClient_status_command <0) perror("fd1");

        char* buffer = (char*)malloc(50);
        int bytesRead = read(fd_rd_ServertoClient_status_command, buffer, 50);

        int bytesWritten = write(1, buffer, strlen(buffer)+1);

        free(buffer);
        close(fd_rd_ServertoClient_status_command);

    }

        
        return 0;
}