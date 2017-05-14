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
#include <string.h>
#include <math.h>
#include "utilities.h"

#define FIFO_PERM 0700

//prototypes
void * requestHandler(void * arg);
void sigIntHandler(int signal);
void writetofile(unsigned long pid, unsigned long tid, unsigned int rnumber, char gender, unsigned int rtime, char * msg);

//Global variables
int generatorFD;
int rejectedFD;

pthread_mutex_t mutexAccessSaunaSpace;

sem_t thrdArgSem;

sem_t steamRoomSem;
sem_t remainingRequests;

int maxCapacity;
char currentGender = 'O';

int balpid;

unsigned int MReceived = 0;
unsigned int FReceived = 0;
unsigned int MRejected = 0;
unsigned int FRejected = 0;
unsigned int MServed = 0;
unsigned int FServed = 0;


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
    
	
	clock_gettime(CLOCK_MONOTONIC_RAW, &processStart);
	
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
	
	
	char filename[256];
   	sprintf(filename, "/tmp/bal.%d",getpid());
    	if((balpid=open(filename, O_WRONLY | O_CREAT))==-1){
		perror("Error on creating the file to write the statistics");
		exit(7);
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
		 writetofile(getpid(),pthread_self(),request.request_number, request.gender, request.time, "RECEBIDO");
		if(request.gender=='M'){
			MReceived++;		
		}
		else{
			FReceived++;
		}
		if (currentGender == 'O'){
			currentGender = request.gender;
			pthread_create(&threadIDs[index], NULL, requestHandler, &request);
			if(request.gender=='M'){
				MServed++;		
			}
			else{
				FServed++;
			}		
			sem_wait(&remainingRequests);
			sem_wait(&thrdArgSem);
			index++;
		}
		else if(request.gender != currentGender){
			request.rejection_number++;
			if (request.rejection_number >= 3)
				sem_wait(&remainingRequests);	
			write(rejectedFD, &request, sizeof(Request));
			writetofile(getpid(),pthread_self(),request.request_number, request.gender, request.time, "REJEITADO");
			if(request.gender=='M'){
				MRejected++;		
			}
			else{
				FRejected++;
			}
		}
		else{			
			pthread_create(&threadIDs[index], NULL, requestHandler, &request);
			if(request.gender=='M'){
				MServed++;		
			}
			else{
				FServed++;
			}		
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


	char buf2[256];
	sprintf(buf2,"\n\n\t\tEstatisticas:\n\n\tPedidos Recebidos:\n\tHomens: %d\n\tMulheres: %d\n\n\tPedidos Rejeitados:\n\tHomens: %d\n\tMulheres: %d\n\n\tPedidos Servidos:\n\tHomens: %d\n\tMulheres: %d\n", MReceived, FReceived, MRejected, FRejected, MServed, FServed);
	write(STDOUT_FILENO, buf2, strlen(buf2));
	close(rejectedFD);
	close(generatorFD);
	
	free(threadIDs);
	unlink("/tmp/rejeitados");
	close(balpid);
}

void* requestHandler(void* arg){

	Request request = * (Request*)arg;
	sem_post(&thrdArgSem);

	struct timespec sleepTime;

	sleepTime.tv_sec = 0;
	sleepTime.tv_nsec = request.time * 1000000;
	
	sem_wait(&steamRoomSem);


	while(nanosleep(&sleepTime, &sleepTime) == -1);

	
	writetofile(getpid(),pthread_self(),request.request_number, request.gender, request.time, "SERVIDO");

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

void writetofile(unsigned long pid, unsigned long tid, unsigned int rnumber, char gender, unsigned int rtime, char * msg){
	char str[256];
	char buf[256];

	double time=round(getProcTime()*100)/100;  
	char stime[256];
	swidth(stime, time, 11,2);
	
	sprintf(buf, "%lu", pid);
	char spid[256];
	strwidth(spid, buf, 8);

	sprintf(buf, "%lu", tid);
	char stid[256];
	strwidth(stid, buf, 18);
	
	char srnumber[256];
	swidth(srnumber, rnumber, 7, 0);

	char srtime[256];
	swidth(srtime, rtime, 7, 0);

	sprintf(str,"%s  -%s   -%s   -%s:  %c   -%s   -  %s\n",stime, spid,stid, srnumber, gender, srtime, msg);
	write(balpid, str, strlen(str));
}
