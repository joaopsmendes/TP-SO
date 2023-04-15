#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
// etc/passwd
int main (int argc,char *argv[]) {
    int saida= open("saida.txt",O_CREAT | O_TRUNC | O_WRONLY);
    if(saida<0){
        write(2,"erro\n",19);
    }
    int erros = open("erros.txt",O_CREAT | O_TRUNC | O_WRONLY);
    if(erros<0){
        write(2,"Erro open saida\n",20);
    }
    int passwd = open("etc/passwd",O_CREAT | O_TRUNC | O_WRONLY);
    if(passwd<0){
        write(2,"Erro open passwd\n",21);
    }
    int backup = dup(0);// para poder escrever no ecra
    dup2(passwd,0);
    dup2(saida,1);
    dup2(erros,2);
    close(passwd);
    close(saida);
    close(erros);
    int fres=fork();
        if(fres==0){
            char buffer[20];
            int res;
            while((res=read(0,&buffer,20))>0){
                write(1,&buffer,res);
                write(2,&buffer,res);
            }
        }
        dup2(backup,1);
        close(backup);
        printf("Terminei");

    }


// 3 exec no principal e no filho