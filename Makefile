#!/bin/bash
#Makefile for generator and sauna binaries
all: gerador sauna
DEBUG: gerDBUG saunDBUG


gerador: gerador.c request.h utilities.c utilities.h
	gcc -Wall gerador.c utilities.c -o gerador -pthread -lm
sauna: sauna.c request.h utilities.c utilities.h
	gcc -Wall sauna.c utilities.c -o sauna -pthread -lm

gerDBUG: gerador.c request.h utilities.c utilities.h
	gcc -Wall -g gerador.c utilities.c -o gerador -pthread -lm
saunDBUG: sauna.c request.h utilities.c utilities.h
	gcc -Wall -g sauna.c utilities.c -o sauna -pthread -lm
	rm -f /tmp/rejeitados

clean:
	rm -f gerador sauna
