#!/bin/bash
#Makefile for generator and sauna binaries
all: gerador sauna
gerador: gerador.c request.h queue.h queue.c
	gcc -Wall gerador.c queue.c -o gerador -pthread

sauna: sauna.c request.h queue.h queue.c
	gcc -Wall sauna.c queue.c -o sauna -pthread

clean:
	rm -f gerador sauna
