#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

//ls /ect/ | wc -l 
// p[1]    | p[0]
//       
// escrita | leitura

int main(int argc, char* argv[]){
    int p[2];
    pipe(p);

    int fres = fork();
    if(fres == 0){
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);

        execlp("ls", "ls", "/etc/", NULL);
        _exit(1);
    }   
    else{
        close(p[1]);
        dup2(p[0], 0);
        close(p[0]);

        execlp("wc", "wc", "-l", NULL);
        _exit(1);
    }


    return 0;
}