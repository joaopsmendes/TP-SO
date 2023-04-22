#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


///// 0 teclado 1 output  --p[0] leitura
int main (int argc,char *argv[]) {
    int p[2];
    pipe(p);
    int fres= fork();
    if(fres==0){  
        close(p[0]); // vai fechar o da leitura
        dup2(p[0],0);
        close(p[0]);
        execlp("ls/etc",NULL);
        _exit(1);
        
    }else{
        close(p[0]);// vai fechar o da escrita
        dup2(1,p[1]);
        close(p[1]);
        execlp("wc","wc","-l",NULL);
    }
}