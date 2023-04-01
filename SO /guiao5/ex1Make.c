#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//named pipes (fifos) => buffer em memoria com a sincronização entre produtores e consumidores
//num so sentido
//nao da para andar para atras
//mutiplos escritores e consumidores
int main(int argc, char** argv){
    int res = mkfifo("myfifo", 0600);
    if(res == -1){
        perror("Erro na criação do FIFO.");
        exit(1);
    }

    return 0;
    
}