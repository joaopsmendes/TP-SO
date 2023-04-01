#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char** argv){
    int fd = open("myfifo", O_RDONLY);
    printf("Abri o fifo\n");
    int res;
    char* buffer = malloc(50*sizeof(char));
    while((res=read(fd,buffer,50))>0){
        write(1,buffer, res);
        printf("Li %s\n", buffer);
    }
    printf("Terminei de ler\n");
    free(buffer);
    return 0;
}