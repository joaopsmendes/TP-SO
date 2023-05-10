#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define MAX_PROGRAMS 30


int main(int argc, char** argv){

    struct timeval start_time;
    gettimeofday(&start_time, NULL);


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
        }

        int res = fork();

        if(res==0){
            //filho

            execvp(argv[3],token);
        
            _exit(1);
        }else{ 
            struct timeval current_time;
            gettimeofday(&current_time, NULL); // funcao time 

            //enviar PID , nome do programa, timestamp

            char msg[100];
            //1 ->//print_programs();
            sprintf(msg, "%d %d %s %ld",1, getpid(), argv[3], current_time);
            
            int status;
            //char* tempo= malloc(20*sizeof(char));

            printf("Running PID: %d\n", getpid());

            write(fd,msg,100);

            waitpid(res,&status,0);

            struct timeval end_time;
            gettimeofday(&end_time, NULL);

            //enviar msg do pid terminado e timesatamp final
            sprintf(msg, "%d %d %ld",2, getpid(), current_time);

            write(fd,msg,100);

            long int elapsed_time = (end_time.tv_sec - current_time.tv_sec) * 1000 + (end_time.tv_usec - current_time.tv_usec) / 1000;//tava start_time?
            printf("Ended in %ld ms\n",elapsed_time);
            
        }

    }else if (strcmp(argv[1], "status") == 0) {

        char statusMsg[20];
        sprintf(statusMsg, "%d %s",3, argv[1]);
        printf("Status enviado ao servidor!\n");
        write(fd,statusMsg,20);

        int fd_rd_ServertoClient = open("monitor_to_tracer",O_RDONLY,0600);
        if(fd_rd_ServertoClient <0) perror("fd1");

        //Program * buffer =malloc(30*sizeof(Program));
        int res;

        char* buffer = malloc(30*sizeof(char));

        while((res=read(fd_rd_ServertoClient,buffer,30))>0){

            write(1,buffer,30);

        }



        
        }
        return 0;
}
    
    

