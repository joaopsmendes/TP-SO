#include "include/tracer.h"

int main(int argc, char** argv){

    int src = open("input", O_RDONLY);
    if(src<0){
        perror("Error on Open src");
    }
    
    int dst = open("output",  O_CREAT | O_TRUNC | O_WRONLY , 0640); 
    if(dst<0){
        perror("Error on Open dst");
    }

    // A opção execute -u; ==> arv[0]: execute; argv[1]: -u
    if(strcmp(argv[1], "-u") == 0){
    }

}