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
#include <semaphore.h>
#include <signal.h>
#include "utilities.h"

#define FIFO_PERM 0700

//prototypes
void * requestHandler(void * arg);
void sigIntHandler(int signal);

//Global variables
int generatorFD;
int rejectedFD;

pthread_mutex_t mutexAccessSaunaSpace;

sem_t thrdArgSem;

sem_t steamRoomSem;
sem_t remainingRequests;

int maxCapacity;
char currentGender = 'O';

int main(int argc, char* argv[]){
	
    if (argc != 2){
		fprintf(stderr, "Wrong usage: ./sauna <max capacity>\n");
		exit(1);
	}
	
	
	//Intalling sigIntHandler
	struct sigaction sigActInt;
    sigActInt.sa_handler = sigIntHandler;
    sigemptyset(&sigActInt.sa_mask);
    sigActInt.sa_flags = 0;
    sigaction(SIGINT, &sigActInt, NULL);
    //--
    
	
	maxCapacity = atoi(argv[1]);
    
    if (maxCapacity <= 0){
		fprintf(stderr, "Invalid capacity %d\n", maxCapacity);
		exit(2);
	}
	
	if(mkfifo("/tmp/rejeitados", FIFO_PERM) == -1){
        perror("Error on creating FIFO rejeitados");
		exit(4);
	}

	if ((generatorFD = open("/tmp/entrada", O_RDONLY)) == -1){
		perror("Fail on opening entrada for reading");
		exit(3);
	}
	
	if ((rejectedFD = open("/tmp/rejeitados", O_WRONLY | O_CREAT)) == -1){
		perror("Error on opening rejeitados for writing");
		exit(5);
	}

	int nRequests;
	read(generatorFD, &nRequests, sizeof(int));

	sem_init(&steamRoomSem,0, maxCapacity);//Steam Room semaphore is initialized with maxCapacity
	sem_init(&remainingRequests, 0, nRequests);
	sem_init(&thrdArgSem, 0, 0);

	pthread_mutex_init(&mutexAccessSaunaSpace, NULL);

	pthread_t* threadIDs = malloc(nRequests * sizeof(pthread_t));
	int index = 0;


	Request request;

	int sval = 0;
	sem_getvalue(&remainingRequests, &sval);
	while(sval){
		read(generatorFD, &request, sizeof(Request));
		if (currentGender == 'O'){
			currentGender = request.gender;
			pthread_create(&threadIDs[index], NULL, requestHandler, &request);		
			sem_wait(&remainingRequests);
			sem_wait(&thrdArgSem);
			index++;
		}
		else if(request.gender != currentGender){
			request.rejection_number++;
			if (request.rejection_number >= 3)
				sem_wait(&remainingRequests);	
			write(rejectedFD, &request, sizeof(Request));
		}
		else{			
			pthread_create(&threadIDs[index], NULL, requestHandler, &request);		
			sem_wait(&remainingRequests);
			sem_wait(&thrdArgSem);
			index++;
		}
		sem_getvalue(&remainingRequests, &sval);
	}
	int existingThrds = index - 1;
	
	for(index = 0; index <= existingThrds; index++){
		pthread_join(threadIDs[index], NULL);
	}

	request.gender = 'E';

	write(rejectedFD, &request, sizeof(Request));
	close(rejectedFD);
	close(generatorFD);
	
	free(threadIDs);
	unlink("/tmp/rejeitados");
}

void* requestHandler(void* arg){

	Request* request = (Request*)arg;
	sem_post(&thrdArgSem);

	struct timespec sleepTime;

	sleepTime.tv_sec = 0;
	sleepTime.tv_nsec = request->time * 1000000;
	
	sem_wait(&steamRoomSem);

	while(nanosleep(&sleepTime, &sleepTime) == -1);

	sem_post(&steamRoomSem);

	pthread_mutex_lock(&mutexAccessSaunaSpace);

	int remainReq;
	sem_getvalue(&steamRoomSem, &remainReq);
	
	if (remainReq == maxCapacity){
		currentGender = 'O';
	}

	pthread_mutex_unlock(&mutexAccessSaunaSpace);
	
	pthread_exit(NULL);
}

void sigIntHandler(int signal){
	unlink("/tmp/rejeitados");
    exit(6);
}
