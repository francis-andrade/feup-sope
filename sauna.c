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
void * handleRequest(void * arg);

//Global variables
int generatorFD;
int rejectedFD;
sem_t steamRoomSem;



int main(int argc, char* argv[]){
	
    if (argc != 2){
		fprintf(stderr, "Wrong usage: ./sauna <max capacity>\n");
		exit(1);
	}
	
	int maxCapacity = atoi(argv[1]);
    
    if (maxCapacity <= 0){
		fprintf(stderr, "Invalid capacity %d\n", maxCapacity);
		exit(2);
	}
	
	sem_init(&steamRoomSem,0, maxCapacity);//Steam Room semaphore is initialized with maxCapacity
	

	//FIFO's
	if ((generatorFD = open("/tmp/entrada", O_RDONLY)) == -1){
		perror("Fail on opening entrada for reading");
		exit(3);
	}
	
	char nRequests[257];
	read(generatorFD, nRequests, size(int));
	pthread_t* threadIDs = malloc(atoi(nRequests) * sizeof(pthread));
	int index = 0;
	

    if(mkfifo("/tmp/rejeitados", FIFO_PERM) == -1){
        perror("Error on creating FIFO rejeitados");
		exit(4);
	}


	if ((rejectedFD = open("/tmp/rejeitados", O_WRONLY | O_CREAT)) == -1){
		perror("Error on opening rejeitados for writing");
		exit(5);
	}


	Request* request;
	read(generatorFD, &request, sizeof(Request*)
	char currentGender = request.gender; //1st person to enter
	
	while(!read(generatorFD, &request, sizeof(Request*){
		if(request->gender != currentGender){
			request->rejection_number++;			
			write(rejectedFD, &request, sizeof(Request*));
			
		else{
			currentGender = request->gender;
			pthread_create(threadIDs[index], NULL, requestHandler, request);
			index++;
		}
	}
	
	int existingThrds = index - 1;
	for(index = 0; index < existingThrds, index++)
		pthread_join(threadIDs[index], NULL);

	//Missing stuff
	//Free / delete stuff
	ptread_join(waitingLine, NULL);
	free(threadIDs);
	unlink("/tmp/rejeitados");
}

void * handleRequest(void * arg){
	sem_wait(&steamRoomSem);
	sem_post(&steamRoomSem);
	return NULL;

}
