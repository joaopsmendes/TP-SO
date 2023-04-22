#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int passwd = open("/etc/passwd", O_RDONLY );
    if(passwd<0) perror("error on open passwd\n");
                                                    // O_TRUNC => ao correr de nova paga o que la estava
    int saida = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(saida<0) perror("error on open saida\n");

    int erros = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(erros<0) perror("error on open erros\n");


    int stdout = dup(1); // copiar 1 para stdout 

    dup2(passwd, 0);
    close(passwd);
    dup2(saida, 1);
    dup2(erros, 2);
    close(erros);

    execlp("wc", "wc", NULL); // se for com sucesso, tudo o resto é ignorado

    int read_res;
    char buf[20];
    while((read_res = read(0, &buf, 20))>0){
        write(1, &buf, read_res);
    }

    dup2(stdout, 1);
    close(stdout);
    printf("Terminei\n");


    return 0;
}