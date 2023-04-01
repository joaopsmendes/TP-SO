//PIPE_BUF =1 se as palavras forem maiores podem ser escritas sem ordem 
//se for igual ao tamanho da palavra ha garantias q n e escrito por cima

#include <unistd.h>
#include <stdio.h>

/*
int main(){
    int p[2];
    pipe(p);
    int res = fork();
    if(res==0){
        close(p[1]);
        int recebido;
        read(p[0],&recebido,sizeof(int));
        printf("Sou filho e recebi %d\n",recebido);
        close(p[0]);
        _exit(0);
    }
    else{
        close(p[0]);
        int i = 23;
        printf("Sou o pai \n");
        write(p[1],&i,sizeof(int));
        printf("Sou o pai e ja escrevi\n");
        close(p[1]);
        wait(NULL);
    }
}
*/

int main(){
    int p[2];
    pipe(p);
    int res = fork();
    if(res==0){
        close(p[0]);
        int i = 23;
        printf("Sou o filho \n");
        write(p[1],&i,sizeof(int));
        printf("Sou o filho e ja escrevi\n");
        close(p[1]);
        _exit(0);   
    }
    else{
        close(p[1]);
        int recebido;
        read(p[0],&recebido,sizeof(int));
        printf("Sou pai e recebi %d\n",recebido);
        close(p[0]);
          
    }
}

/*

int main (){
    int p[2];
    pipe(p);
    int res = fork();
    if(res==0){
        close(p[0]);
        int i = 23;
        printf("Sou o filho \n");
        for ( int enviado=0; enviado!= 100000; enviado++){
            printf("Escrevi");
            write(p[1],&i,sizeof(int));
        }
        printf("Sou o filho e ja escrevi\n");
        close(p[1]);
        _exit(0);   
    }
    else{
        close(p[1]);
        int recebido;
        int res;
        while((res =read(p[0],&recebido,sizeof(int))=0)){
        printf("Sou pai e recebi %d\n",recebido);
        }
        close(p[0]);
        wait(NULL);
          
    }
}

*/

struct ponto  {
    int x;
    int y ;
};

//arcg e o valor q se quer procurar
int main (int argc, char **argv){
    int columns = 1000;
    int rows = 10;
    int nocor;
    int randomx = 50000;
    int matrix[rows][columns];
    int obj=679;
    int pids[rows];
    int number=5;
    for (int i=0; i!=rows;i++){
    for(int j=0; j!=columns;j++){
        matrix[i][j]= rand() % randomx;
    }
}

}

// cria um processo para cada linha 
int lookup (int **matrix, int rows, int cols, int target, int vetor){}