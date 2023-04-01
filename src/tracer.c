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
    if(strcmp(argv[1],"execute")==0){ // VERIFICAR
            if(strcmp(argv[2], "-u")==0 ){ 
                   for (int a=3,i =0; argv[a]!=NULL; a++,i++){      
                        token[i] = argv[a];      
            }
        
        }
    }

    //if(strcmp(argv[1],"status")==0){
        // fazer o execvp de "ps" para ver os processos (duvidas)
    //}
    
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
        int res1;
        char* buffer = malloc(50*sizeof(char));
        while((res1=read(0,buffer,50))>0){
            write(fd,buffer, 50);
            printf("Escrevi\n");
            execlp(argv[2],token,NULL);  // filho 

            
        }
        _exit(0);
    }else{ 
        struct timeval current_time;
        gettimeofday(&current_time, NULL); // funcao time 

        char msg[100];
        sprintf(msg, "%d %s %ld", res, argv[1], current_time.tv_sec);
        
        int status;
        //char* tempo= malloc(20*sizeof(char));

        write(fd,msg,strlen(msg)+1);

        waitpid(res,&status,0);

        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        sprintf(msg, "%d %ld", res, current_time.tv_sec);

        write(fd,msg,strlen(msg)+1);

        long int elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;
        printf("O programa %s, pid %d, terminou em %ld ms\n", argv[1],res,elapsed_time);
        
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

