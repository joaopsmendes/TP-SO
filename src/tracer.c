#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



int main(int argc, char** argv){

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    int src = open("input", O_RDONLY);
    if(src<0){
        perror("Error on Open src");
    }
    int dst = open("output",  O_CREAT | O_TRUNC | O_WRONLY , 0640); 
    if(dst<0){
        perror("Error on Open dst");
    }
    char token[20];
    if (strcmp(argv[1], "execute") == 0) {
        if (strcmp(argv[2], "-u") == 0) {
            for (int a = 3, i = 0; argv[a] != NULL; a++, i++) {
                strcpy(&token[i], argv[a]);
            }
        }
 
    }
    
    int fifo = mkfifo("myfifo", 0600);
    if(fifo == -1){
        perror("Erro na criação do FIFO.");
       // exit(1);
    }
    int fd = open("myfifo", O_WRONLY);
    printf("Abri o FIFO para escrita\n");
    int p[2];
    pipe(p);
    int res = fork();

    if(res==0){
        char * newtoken = strtok(token, " ");
   
   /* walk through other tokens */
        while( newtoken != NULL ) {
            printf( "%s\n", newtoken);
            newtoken = strtok(NULL, " ");
        }
        
        execvp(&newtoken[0],&newtoken);  // filho 
        //execlp(program, program, token, NULL);
        
            
        

            
       // }
        _exit(1);
    }else{ 
        struct timeval current_time;
        gettimeofday(&current_time, NULL); // funcao time 

        //enviar PID , nome do programa, timestamp

        char msg[100];
        sprintf(msg, "%d %s %ld", getpid(), argv[3], current_time.tv_sec);
        
        int status;
        //char* tempo= malloc(20*sizeof(char));

        printf("A executar: Pid: %d, Programa: %s, Tempo Inicial: %ld\n", getpid(),  argv[3],current_time.tv_sec);

        write(fd,msg,strlen(msg)+1);

        waitpid(res,&status,0);

        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        sprintf(msg, "%d %ld", getpid(), current_time.tv_sec);

        write(fd,msg,strlen(msg)+1);

        long int elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;
        printf("O programa %s, pid %d, terminou em %ld ms\n", argv[3],getpid(),elapsed_time);
        
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
    
    close(src);
    close(dst);
    return 0;

}

