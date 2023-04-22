# Dup
cria uma cópia de um descritor de ficheiro => copiar/duplicar um file de escrita

int dup(int fd) -> devolve a copia do descritor ou erro 

# Dup2
cria uma copia para cima de algo que ja exista

int dup2(int fd1, int fd2)

## NOTA:
* close() depois de um dup()/dup2()
* fflush(stdout) forçar a persistencia de dados em buffer
