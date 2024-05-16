/*
GRUPO 1 
1. Os processos uma vez que se encontram na memória principal e a correr pelo CPU podem ser interrompidos ou pode acointecer desafectação forçada.
Efetivamente quando um programa é interrompido a qualquer moemnto enviado um sinal ao CPU normalmente enviado
pelo bus sistem atraves de um system call. Uma interrupção pode ser gerada por dispositivos de hardware
para sinalizar o facto de precisarem de atenção do Sistema operativo.Podem ter recebido dados ou completaram
uma tarefa que o sistema operativo tinha pedido anteriormente. Também pode ser causada por um software 
pedir uma chamada ao sistema para ser executada pelo sistema operativo como por exemplo erros. As interrupçoes 
tb podem ser chamdas de traps ou exceptions.
A desafectação forçada ocorro quando um processo é forçado a parar mesmo sem ter terminado para correr outro .
Isso pode acontecer quando um programa está travado ou não está respondendo, e o sistema operacional precisa
encerrá-lo para liberar recursos e manter a estabilidade do sistema.
Por exemplo, no robin round scheduleling
cada programa tem apenas uma slice time para correr no CPU e após isso é interrompido e outro 
processo toma o seu lugar.O processo que estava a correr pode ir para o fim da fila dos ready caso não esteja
a espera de nada e assim sucessivamente. No caso do priority schedule se um processo estiver a correr e 
entretanto chegar outro processo com maior prioridade o processo que esta a correr para para dar lugar ao novo
processo que chegou que tem maior prioridade.

2.O processo está a fazer trashing se passar mais tempo em paginação do que em execução. 
Quando ha mais tempo desperdicado em trocas de paginas entre memoria principal e memoria secundaria
devido a falta de memoria disponivel.
O sistema de memoria virtual é uma tecnica usada pelos pelos sistemas
operacionais para permitor que os processos acessem mais memoria do 
que a fisicamente doisponivel na RAM.Ele utiliza uma combinação de
memória fisica.Ele utiliza uma combinaçãp de memoria fisica 
e espaço em disco patra armazenar paginas de memoria. Quando um programa 
tenta acessar uma pagina que não esta na RAM ocorre
uma page fault  e o sistema operacional precisa buscar uma pagina ao disco 
e traze-la para RAM antes que o programa possa continuar a execução.

Um escalonador ingênuo, que prioriza apenas a utilização da CPU para a tomada
 de decisões de escalonamento de processos, não leva em consideração o uso 
 eficiente da memória. Isso significa que ele pode permitir que mais processos 
 sejam executados simultaneamente do que a capacidade de memória física disponível pode suportar.
Quando vários processos estão em execução simultaneamente e competindo pela memória física, 
e o escalonador não leva em conta o uso eficiente da memória, pode ocorrer uma situação em que
 a maioria das páginas de memória dos processos está sendo constantemente movida para o disco 
 e trazida de volta, devido à falta de memória física adequada. Isso resulta em um ciclo vicioso 
 de trocas constantes de páginas, levando ao trashing.
 O trashing causa um impacto negativo no desempenho do sistema, pois a maior parte do tempo da CPU
  é gasto na movimentação de páginas de/para o disco, em vez de executar as instruções reais dos 
programas. Isso leva a um baixo throughput (taxa de processamento) e um alto tempo de resposta, 
resultando em uma experiência de usuário lenta e insatisfatória.
Para evitar o trashing, os escalonadores de processos devem levar em conta a alocação eficiente de
 memória e considerar a quantidade de memória disponível antes de permitir que um novo processo seja 
 iniciado. Algoritmos de escalonamento mais avançados podem considerar métricas como a quantidade de 
 memória utilizada por um processo, sua prioridade e a quantidade de tempo da CPU já utilizada, para 
 tomar decisões mais inteligentes e evitar a sobrecarga de memória que leva ao trashing.
PARTE 2 
'''*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int servidor () {
  int fd = open("myfifo", O_RDONLY);
  printf("Abri o fifo\n");
  int res1=0;
  char* buffer[50];
  int res= fork();
  while( res1!= 9){
    res1++;
      if ( res=0){
      _exit(res);
      }
      else{
        int status;
        //codigo pai 
        wait(&status);
        while((res=read(fd,buffer,50))>0){
                write(1,buffer, res); // write 1 para o stdout
                printf("Li %s\n", buffer);
              
      }
    }
  
  }
}