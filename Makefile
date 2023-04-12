all: server client
server: bin/monitor
client: bin/tracer



bin/monitor: src/monitor.c include/monitor.h
gcc -Wall -g -c src/monitor.c  

bin/tracer: src/tracer.c include/tracer.h
gcc -Wall -g -c src/tracer.c 

clean:
rm -f obj/* tmp/* bin/{tracer,monitor}