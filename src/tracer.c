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
        // else if (strcmp(argv[2], "-p") == 0) {
        //     //cat fich1 | grep "palavra" | wc -l
        //     //SIZE == numero de commandos, no caso em cima seria 3
        // char ** exec_args = malloc((argc-2)*sizeof(char));
        // char* string;
        // char* command = strdup(arg);

        // while((string = strsep(&command, " | ")) != NULL){
        //     exec_args[i] = string;
        //     i++;
        // }
        // exec_args[i] = NULL;
        //     int p[SIZE-1][2];
        //     //tem de ter no terminal as aspas entre os diferentes argumentos
        //     //seguir o exercicio 6
                       

        // }
        
        

    //for (i = 0; token[i] != NULL; i++) {
    //    printf("%s\n", token[i]);
    //}
       
    
        
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

            write(fd,msg,strlen(msg)+1);

            waitpid(res,&status,0);

            struct timeval end_time;
            gettimeofday(&end_time, NULL);

            //enviar msg do pid terminado e timesatamp final
            sprintf(msg, "%d %d %ld",2, getpid(), current_time);

            write(fd,msg,strlen(msg)+1);

            long int elapsed_time = (end_time.tv_sec - current_time.tv_sec) * 1000 + (end_time.tv_usec - current_time.tv_usec) / 1000;//tava start_time?
            printf("Ended in %ld ms\n",elapsed_time);
            
        }

    }else if (strcmp(argv[1], "status") == 0) {

        char statusMsg[20];
        sprintf(statusMsg, "%d %s",3, argv[1]);
        printf("Status enviado ao servidor!\n");
        write(fd,statusMsg,strlen(statusMsg)+1);

        int fd_rd_ServertoClient = open("monitor_to_tracer",O_RDONLY,0600);
        if(fd_rd_ServertoClient <0) perror("fd1");

        //Program * buffer =malloc(30*sizeof(Program));
        int res;

        char* buffer = malloc(30*sizeof(char));

        while((res=read(fd_rd_ServertoClient,buffer,30))>0){

            write(1,buffer,strlen(buffer)+1);

        }

        /*for(int i = 0; buffer!=NULL ; i++){
                    printf("%s",buffer[i].program_name);
                    printf("%s",buffer[i].pid);
                    printf("%s\n",buffer[i].start_time);
                }*/
        /*if (res < 0) {
        perror("Error reading from FIFO");
        exit(1);
        }*/

        /*
        else if(strcmp(argv[1], "stats-time") == 0){

        }
        else if(strcmp(argv[1], "stats-command") == 0){
            
        }
        else if(strcmp(argv[1], "stats-uniq") == 0){
            
        }
        
        
        */

        
        }
        return 0;
}
    
    

    /*
    int p[2];
    pipe(p);
    int res = fork();
    if(res==0){
        close(p[0]);
        int i = 23; 
        int res = execlp(argv[2],token,NULL); 
        write(p[1],&res,sizeof(int));
        printf("Sou o filho e ja escrevi\n");
        close(p[1]);
        wait(NULL);
        
        //exec();
    }else[
        
    ]
   */

    // A opção execute -u; ==> arv[0]: execute; argv[1]: -u
    
    //close(src);
    //close(dst);
   


