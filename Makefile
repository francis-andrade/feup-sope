#!/bin/bash
#!Makefile for generator and sauna binaries
all: gerador sauna
gerador: gerador.c request.h
	gcc -Wall gerador.c -o gerador

sauna: sauna.c request.h
	gcc -Wall sauna.c -o sauna

clean:
	rm -f gerador sauna
