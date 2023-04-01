#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv){
    
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