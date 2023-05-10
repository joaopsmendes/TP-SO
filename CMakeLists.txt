cmake_minimum_required(VERSION 3.21)
project(TP_SO C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories("SO /ficha 1")
include_directories(src)

add_executable(TP_SO
        include/monitor.h
        include/tracer.h
        "SO /ficha 1/person.c"
        "SO /ficha 1/person.h"
        "SO /ficha2/ex1.c"
        "SO /ficha3/ex1.c"
        "SO /ficha4/ficha4.c"
        "SO /ficha6/ex1.c"
        "SO /ficha6/ex2.c"
        "SO /ficha6/ex4.c"
        "SO /ficha6/ex5.c"
        "SO /ficha6/ex6.c"
        "SO /guiao5/Ex2/cliente.c"
        "SO /guiao5/Ex2/servidor.c"
        "SO /guiao5/ex1Make.c"
        "SO /guiao5/ex1Read.c"
        "SO /guiao5/ex1Write.c"
        "SO /guiao6/ex1.c"
        "SO /guiao6/ex2.c"
        "SO /guiao6/ex3.c"
        "SO /guiao6/ex4.c"
        "SO /guiao6/ex5.c"
        "SO /guiao6/ex6.c"
        src/monitor.c
        src/monitor.h
        src/tracer.c)
