#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


// grep -v ˆ# /etc/passwd | cut -f7 -d: | uniq | wc -l => 4 comandos 

// p[2] = p[1][2] => vai executar dois comandos
// p[3][2] => vai executar 4 comandos 
// N comandos => N-1 pipes
int main(int argc, char* argv[]){
    int p[3][2];
    for(){
        pipe();
    }

    
    if(fork() == 0){
        close(p[0][0]);
        dup2();
        close();
        execlp("grep", "grep", "-v", "^#", "/etc/passwd", NULL);
        _exit(1);

    }

    if(fork() == 0){
            
    }

    if(fork() == 0){
            
    }

    if(fork() == 0){
            
    }
    
    
    return 0;
}