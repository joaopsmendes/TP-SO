#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int func2(int argc, char **argv){
    int myid=getpid();
    int parentid = getppid();
    printf(myid);
    printf(parentid);

    int res = fork(); //cria o filho --devolve 0 se for o filho ou o id do pai
    if ( res==0){
        //SOU O FILHO
        myid= getpid();
        parentid=getppid();
        //**sleep()
        _exit(1); // no filho tem q se por 
    }
    else {
        //CODIGO DO PAI
        myid=getpid();
        //*sleep(10) -> so isto o filho fica zombie
        parentid=getppid();
        int myson = res;
        //** wait ()-> espera pelo filho so faz para a frente qd o filho acabar
        printf("PAI %d parentid ",&parentid);
    }
}


// filho pode ter parent a 1 ---foi adotado

int func1 (int argc, char **argv){
    int myid=getpid();
    int parentid = getppid();
    printf(myid);
    printf(parentid);

    int res = fork(); //cria o filho --devolve 0 se for o filho ou o id do pai
    if ( res==0){
        //SOU O FILHO
        myid= getpid();
        parentid=getppid();
        //**sleep()
        _exit(0); // no filho tem q se por  ---- valor de saida saiu com o valor zero que dps e colhido no status
    }
    else {
        //CODIGO DO PAI

        int status;
        //wait(&status);//capta o q saiu no _exit 
        waitpid (res,&status,0)// espera especificamente por um filho em esp
        if(WIFEXITED(status)){
            printf()
        }
        //** wait ()-> espera pelo filho so faz para a frente qd o filho acabar
        printf("PAI %d parentid e o filho devolveu %d ",&parentid, &WEXITSTATUS(status)); // cuidado com os bits -1 ==255 // valores apenas entre 0 e 255 
    }
}


// sequencial cria 10 
int main(){
int i =1;
int res;
    while(i!=11){
    res=fork();
    i++;
    if(res==0){
        //É FILHO
        printf("pid do filho %d e pid do pai %d", &getpid(),&getppid());
        _exit(i);
    }else {
        int status;
        //codigo pai 
        wait(&status);
        printf("PAI : exit code filho %d", &WEXITSTATUS(status));
    }
    }
}

// concorrente
int main (){
    int i = 1; 
    int res;
    while(i!=11){
        res=fork();
        i++;
        if(res==0){
            //É FILHO
            printf("pid do filho %d e pid do pai %d", &getpid(),&getppid());
            _exit(i); // como fez exit o codigo abaixo nnc é executado por nenhum dos filhos 
        }
    }
    for(int j=0 ; j=10;j++){

        int status;
        //codigo pai 
        wait(&status);
        printf("PAI : exit code filho %d", &WEXITSTATUS(status));
    }
    return 0;
}
    



// 1 processo por linha que fica responsavel por procurar 
int main(){
    int columns = 1000;
    int rows = 10;
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
    for(int i=0; i!=rows;i++){
        int res = fork();
        if(res==0){
            //filho
            for(int j=0; j!=columns;j++){
                    //É FILHO
                    if(matrix[i][j]==number){
                        _exit(i); // como fez exit o codigo abaixo nnc é executado por nenhum dos filhos 
            }

        }
        _exit(-1);
    }
    }
    for(int j=0 ; j!=rows;j++){
        int status;
        //codigo pai 
       // wait(&status) alinea 5
        waitpid(pids[i],&status,0);//alinea 6
        if(WEXITSTATUS<255){
        printf("Foi encontrado valor na linha %d", &WEXITSTATUS(status));
    }
    }
    return 0;
}


// exercicio 4 usar o lseek ;