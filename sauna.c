//sauna.c
//gerador.c
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

#define FIFO_PERM 0700

int main(int argc, char* argv[]) {
    if (argc != 2){
		fprintf(stderr, "Wrong usage: ./sauna <max capacity>\n");
		exit(1);
	}
	
	int maxCapacity = atoi(argv[1]);
    
    if (maxCapacity <= 0){
		fprintf(stderr, "Invalid capacity %d\n", maxCapacity);
		exit(2);
	}
	
    if(mkfifo("/tmp/rejeitados", FIFO_PERM) == -1)
        perror("Error on creating FIFO rejeitados");    

	unlink("/tmp/rejeitados");
}
