#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    int fres= fork();
    if(fres==0){
        close(p[1]);
        dup2(p[0],0);
        close(p[0]);
        execlp("wc","wc",NULL);
        _exit(1);

    }else{
        close(p[0]);
        int res;
        char buffer[10];
        while((res=read(0,&buffer,10))>0){
            write(p[1],&buffer,res);
        }
        close(p[1]);

    }
    return 0;
}