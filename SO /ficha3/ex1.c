// arg vao ser agrv -- argv[0] é o nome do proprio comando -- podem existir variantes do argv[0]

//bin/exec, ls, ... NULL

// vai executar  a main do novo programa -- caso n execute devolve um inteiro -- o codigo é substituido pelo novo 


// execlp -- p vem de path primeiro argumento do caminho pode considerar executaveis que estejam em diretorias do meu path - como o /bion esta no ambiente path 

// execv executa um comando com varios argumentos ( estes e que estao no array)

#include <unistd.h>
#include <fcntl.h>
/*
// ex1 
int main (int argc, char ** argv){
    int res= execl("/bin/ls","ls","-l",NULL);
   
    printf("Terminou %d\n", res);
}
*/
#include <stdio.h>

/*
int main (int argc, char ** argv){
    int res=fork();
    if(res==0){
        // sleep(3);
    int res= execl("/bin/ls","ls","-l",NULL);
    printf("Correu mal %d",res);
    _exit(1);// e importante sinalizar o exit para o pai ser avisado na hora certa porser baixo nivel  so se correu mal senao o exit e feito pelo que esta dentro do execl 
    }else{
        wait(NULL); //-- sequencial 
        printf("terminei");
    }
   return 0;
   // printf("Terminou %d\n", res);
}
// o pai do pai e o terminal 
*/

/*
int main (int argc, char ** argv){
    int res;
    for ( int i =1; argv[i]!=NULL;i++){ // tem q comecar no 1 pq o zero é o próprio com argv a 0 ia correr sempre a si proprio 
    res = fork();
        if(res==0){
             execlp(argv[i],argv[i],NULL); // esta a fazer sem argumentos é sempre ele próprio
            _exit(1);
        }
    }
    for(int i =1; argv[i]!=NULL;i++){
        int status;
        wait(&status);
        if(WEFEXITED(status)){
            printf("PAI : exit code "); // completar esta frase dps 
        }
    }
    return 0;
}
*/

/** ver melhor o my system*/

//SYSTEM 
int main (char ** command){
    char *res = "ls -a";
    char token[20];
    token[0] = strtok(res," ");
    char *temp=token[0];
    int fork_ret= fork(); // so um comando 
    for ( int i =2 ; temp!=NULL; i++){ // tem q comecar no 1 pq o zero é o próprio
        execlp(token[1],token[i],NULL); // esta a fazer sem argumentos
        }
    return 0;
}

// fread da zero qd chega ao fim do ficheiro 

// execução sequencial dentro de uma paralela 




