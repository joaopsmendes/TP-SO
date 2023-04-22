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


int main(int argc, char** argv){
    struct timeval start_time;
    gettimeofday(&start_time, NULL);



/*
    int src = open("input", O_RDONLY);
    if(src<0){
        perror("Error on Open src");
    }
    int dst = open("output",  O_CREAT | O_TRUNC | O_WRONLY , 0640); 
    if(dst<0){
        perror("Error on Open dst");
    }*/

    int fifo = mkfifo("myfifo", 0600);
        if(fifo == -1){
            perror("Erro na criação do FIFO.");
        // exit(1);
        }
        int fd = open("myfifo", O_WRONLY,0600);
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
        

    //for (i = 0; token[i] != NULL; i++) {
    //    printf("%s\n", token[i]);
    //}
       
    
        
        int res = fork();

        if(res==0){

            execvp(argv[3],token);
        
            _exit(1);
        }else{ 
            struct timeval current_time;
            gettimeofday(&current_time, NULL); // funcao time 

            //enviar PID , nome do programa, timestamp

            char msg[100];
            //1 ->
            sprintf(msg, "%d %d %s %ld",1, getpid(), argv[3], current_time.tv_sec);
            
            int status;
            //char* tempo= malloc(20*sizeof(char));

            printf("A executar: Pid: %d, Programa: %s, Tempo Inicial: %ld\n", getpid(),  argv[3],current_time.tv_sec);

            write(fd,msg,strlen(msg)+1);

            waitpid(res,&status,0);

            struct timeval end_time;
            gettimeofday(&end_time, NULL);

            //enviar msg do pid terminado e timesatamp final
            sprintf(msg, "%d %d %ld",2, getpid(), current_time.tv_sec);

            write(fd,msg,strlen(msg)+1);

            long int elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;
            printf("O programa %s, pid %d, terminou em %ld ms\n", argv[3],getpid(),elapsed_time);
            
        }
    }else if (strcmp(argv[1], "status") == 0) {

        char statusMsg[20];
        sprintf(statusMsg, "%s", argv[1]);
        write(fd,statusMsg,strlen(statusMsg+1));

        int fd_rd_ServertoClient = open("myfifo",O_RDONLY,0600);
        if(fd_rd_ServertoClient <0) perror("fd1");

        Program * buffer =malloc(30*sizeof(Program));
        int res;

        res = read(fd_rd_ServertoClient,buffer,30);
        
            for(int i = 0; i<30; i++){
                    printf("%s",buffer[i].program_name);
                    printf("%s",buffer[i].pid);
                    printf("%s\n",buffer[i].start_time);
                }
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
   


