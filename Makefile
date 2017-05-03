#!/bin/bash
#!Makefile for generator and sauna binaries
all: gerador sauna
gerador: gerador.c request.h
	gcc -Wall gerador.c -o gerador -pthread

sauna: sauna.c request.h
	gcc -Wall sauna.c -o sauna -pthread

clean:
	rm -f gerador sauna
