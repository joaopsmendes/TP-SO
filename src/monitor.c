#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// para gaurdar a informacao
int main(int argc, char** argv){
    int fd = open("myfifo", O_RDONLY);
    printf("Abri o fifo para leitura\n");

    int fd_log = open("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);

    int res;
    char* buffer = malloc(200*sizeof(char));
    while((res=read(fd,buffer,200))>0){
        write(fd_log, buffer, res);
        printf("Li %s\n", buffer);
    }
    printf("Terminei de ler\n");
    free(buffer);
    close(fd_log);
    return 0;

   // int res;
   // char* buffer = malloc(50*sizeof(char));
   // while((res=read(0,buffer,50))>0){
   //     write(fd,buffer, res);
   //     printf("Escrevi\n");
   // }
   // 
   // return 0;
}