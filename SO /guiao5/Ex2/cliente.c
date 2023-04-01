#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char** argv){

// criacao do fifo -> pode ser criado numm ficheiro a parte como foi feito para o exer1 ou criar aqui em consunto com o clinete mas tirar o _exit
    int fifo = mkfifo("myfifo", 0600);
    if(fifo == -1){
        perror("Erro na criação do FIFO.");
    }

    int fd = open("myfifo", O_WRONLY);
    printf("Abri o FIFO para escrita\n");

    int res;
    char* buffer = malloc(50*sizeof(char));
    while((res=read(0,buffer,50))>0){
        write(fd,buffer, res);
        printf("Escrevi\n");
    }
    
    return 0;
}