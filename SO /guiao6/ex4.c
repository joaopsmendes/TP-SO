#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main( int argc, char* argv[]){
    
    //criar um pipe entre o pai e o filho
    int p[2];
    pipe(p);
    
    int fres = fork(); //dois processos

    if(fres==0){
        //filho

        close(p[1]); // fechar ponta de escrita
        dup2(p[0], 0);
        close(p[0]); //fechar aquela que nao se vai usar, para nao ter dois abertos
        // outra abordagem:
        // char * args[2] = {"wc", NULL};
        // execvp("wc", args);
        execlp("wc", "wc", NULL);
        _exit(1);

    }else{
        //pai 
        close(p[0]); //fechar ponta de leitura
        int res;
        char buffer[10];
        while((res=read(0,&buffer,10))>0){
            write(p[1], &buffer, res); //escrevemos na ponta de escrita
            //este ciclo while termina assim que o utilizar mandar um end of file (control D)
        }
        close(p[1]);
        int status;
        wait(&status);
        printf("Filho terminou.\n");
    
    }
    return 0;

}