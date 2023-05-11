#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
                sprintf(msg, "%d %d %s %ld",1, getpid(), argv[3], current_time.tv_sec);
                
                int status;
                //char* tempo= malloc(20*sizeof(char));

                printf("Running PID: %d\n", getpid());

                write(fd,msg,100);

                waitpid(res,&status,0);

                struct timespec end_time;
                clock_gettime(CLOCK_REALTIME, &end_time);;

                //enviar msg do pid terminado e timesatamp final
                sprintf(msg, "%d %d %ld",2, getpid(), current_time.tv_sec);

                write(fd,msg,100);

                double elapsed_time_ms = (end_time.tv_sec - current_time.tv_sec) * 1000.0 + (end_time.tv_nsec - current_time.tv_nsec) / 1000000.0;


                //double elapsed_time = (end_time.tv_sec - current_time.tv_sec) + (end_time.tv_nsec - current_time.tv_nsec) / 1000000000.0;

                //long int elapsed_time = (end_time.tv_sec - current_time.tv_sec) * 1000 + (end_time.tv_nsec - current_time.tv_nsec) / 1000;//tava start_time?
                printf("Ended in %f ms\n",elapsed_time_ms);
                
            }



        }else{

            printf("Comando não reconhecido!\n");

        

        }
         // else if (strcmp(argv[2], "-p") == 0) {
        //     //cat fich1 | grep "palavra" | wc -l
        //     //SIZE == numero de commandos, no caso em cima seria 3
        //     int p[SIZE-1][2];
        //     //tem de ter no terminal as aspas entre os diferentes argumentos
        //     //seguir o exercicio 6

        

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

            /*if(strlen(buffer) == 0){
                continue;
            }*/


            if (bytesWritten != bytesRead) {
                break;
            }
        }

        free(buffer);
        close(fd_rd_ServertoClient);
    }
        return 0;
}