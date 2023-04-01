
#include <stdio.h>
#include "person.h"
#include <unistd.h>
#include <fcntl.h>

int new_person(char *name, int age){
    Person p ; 
    strcpy(p.name,name);
    p.age=age;
    int fd = open( "fd.txt",O_CREAT|O_WRONLY|O_APPEND, 0640);// append acrescenta no final  alternativa usar o lseek offset 0 a partir do seek
    int res =  write ( fd, &p, sizeof(Person)); // tem que ser endereço de p pois se fosse de age ou assim era segm fault uma vez que n podemos aceder a end dentro de structs 
    
    if (res==0){
        printtf("Erro");
    }
    else return 0 ;
}
//SEEK_SET ( inicio ) SEEK_CUR

// ler aos bocados do ficheiro passar para uma struct cada um e depois comparar 
int person_change_age(char * name, int age){
    Person p ;
    int bytes_read;
    int fd = open( "fd.txt",O_RDWR, 0600);
    if (fd==0) printf("Erro");

    while ((bytes_read =read(fd,&p,sizeof(p)))=0){
        if (strcmp (p.name, name)==0){
            p.age=age;
            //res = lseek(fd,&p,sizeof(Person)); // volta para tras para a reescrever pois o apontador como ja leu esta a frente

        }


    }
}

int person_change_age_v2(long pos, int age){
    int fd = open("fd.txt",O_RDWR,0600);
    int seek_res= lseek(fd,sizeof(Person),SEEK_CUR);
    res = 
}

int main(int argc, int argv[]){
    struct Person s1 ;
}