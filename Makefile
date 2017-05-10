#!/bin/bash
#Makefile for generator and sauna binaries
all: gerador sauna
gerador: gerador.c request.h utilities.c utilities.h
	gcc -Wall gerador.c utilities.c -o gerador -pthread -lm
	
gerDBUG: gerador.c request.h utilities.c utilities.h
	gcc -Wall -g gerador.c utilities.c -o gerador -pthread

sauna: sauna.c request.h 
	gcc -Wall sauna.c -o sauna -pthread

clean:
	rm -f gerador sauna
