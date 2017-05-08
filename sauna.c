//sauna.c
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

int generatorFD;
int rejectedFD;

int main(int argc, char* argv[]) 
    
	if (argc != 2){
		fprintf(stderr, "Wrong usage: ./sauna <max capacity>\n");
		exit(1);
	}
	
	int maxCapacity = atoi(argv[1]);
    
    if (maxCapacity <= 0){
		fprintf(stderr, "Invalid capacity %d\n", maxCapacity);
		exit(2);
	}

	//Open entrada in reading mode
	if ((generatorFD = open("/tmp/entrada", O_RDONLY)) == -1){
		perror("Fail on opening entrada for reading");
		exit(3);
	}
	
	//Create rejeitados
    if(mkfifo("/tmp/rejeitados", FIFO_PERM) == -1){
        perror("Error on creating FIFO rejeitados");
		exit(4);
	}

	//Open rejeitados in writing mode
	if ((rejectedFD = open("/tmp/rejeitados", O_WRONLY | O_CREAT)) == -1){
		perror("Error on opening rejeitados for writing");
		exit(5);
	}

	int nRequests;

	//Receive the number of total requests from the generator
	read(generatorFD, &nRequests, sizeof(nRequests));

	pthread_t waitingLine;
	pthread_create(&waitingLine, NULL, handleLine, );

	//Missing stuff

	ptread_join(waitingLine, NULL);
	unlink("/tmp/rejeitados");
}

void * handleLine(void * arg){

	queue * saunaLine = queue_constructor();

}