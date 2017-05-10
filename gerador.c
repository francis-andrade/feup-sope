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
#include <stdbool.h>
#include <math.h>
#include <fenv.h>
#include <signal.h>
#include "utilities.h"

#define FIFO_PERM 0700
#define DEBUG
#define MAXTIME 50
#define MINTIME 0

//Prototypes
void* generateRequests(void* arg);
void* rejectedListener(void* arg);
void sigIntHandler(int signal);

int genFifoFD;
int rejFifoFD;

unsigned int MGenerated = 0;
unsigned int FGenerated = 0;
unsigned int MRejected = 0;
unsigned int FRejected = 0;
unsigned int MDiscarded = 0;
unsigned int FDiscarded = 0;

FILE * gerpid;

int main(int argc, char* argv[]) {

	if (argc != 3){
		fprintf(stderr, "Wrong usage: ./gerador <n. requests> <max. usage time>\n");
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
	
	
	int nRequests = atoi(argv[1]);

	if (nRequests <= 0){
		fprintf(stderr, "Invalid number of requests\n");
		exit(2);
	}

	int maxUsage = atoi(argv[2]);

	if (maxUsage > MAXTIME  || maxUsage <= MINTIME){
		fprintf(stderr, "Invalid number of max usage time\n");
		exit(3);
	}
	
    //Create entrada
	if(mkfifo("/tmp/entrada", FIFO_PERM) == -1){
        perror("Error on creating FIFO entrada");
        exit(4);
    }
    
    //Open entrada in writing mode
    if((genFifoFD = open("/tmp/entrada", O_WRONLY | O_CREAT | O_NONBLOCK)) == -1){
        perror("Fail on opening entrada for writing");
        exit(5);
    }

    #ifndef DEBUG
        //Open rejeitados in reading mode
        if ((rejFifoFD = open("/tmp/rejeitados", O_RDONLY)) == -1){
            perror("Fail on opening entrada for writing");
            exit(7);
        }
    #endif

    gerpid=fopen("/tmp/ger.pid", "w");
    //Send the number of total requests to the sauna
    write(genFifoFD, &nRequests, sizeof(nRequests));
    
    int generatorArgs[] = {nRequests, maxUsage};
    pthread_t genTID, rejTID;
    pthread_create(&genTID, NULL, generateRequests, generatorArgs);
    pthread_create(&rejTID, NULL, rejectedListener, NULL);
    

    pthread_join(genTID, NULL);
    pthread_join(rejTID, NULL);

    close(genFifoFD);
    close(rejFifoFD);

    unlink("/tmp/entrada");
    fprintf(gerpid,"\t\tEstatisticas\n\n\tPedidos Gerados:\nHomens: %d\nMulheres: %d\n\n\tPedidos Rejeitados:\nHomens: %d\nMulheres: %d\n\n\tPedidos Descartados:\nHomens: %d\nMulheres: %d\n", MGenerated, FGenerated, MRejected, FRejected, MDiscarded, FDiscarded);

}

void* generateRequests(void* arg){
    
    int nRequests = ((int*) arg)[0];
    int maxUsage = ((int*) arg)[1];
    
    srand(time(NULL));
    
    int i;
    for(i = 0; i < nRequests; i++){

        Request request;


        request.request_number = i + 1;
        
        if(rand() % 2){ //0 or 1 upper boundry is included
            request.gender = 'M';
            MGenerated++;
        }
        else{
            request.gender = 'F';
            FGenerated++;
        }
        
        request.time = rand() % (maxUsage - MINTIME) + MINTIME;
        request.rejection_number = 0;  
	double time=round(getProcTime()*100)/100;      
        fprintf(gerpid,"%s - %s - %s: %c - %s - %s\n",swidth(time, 11,2), swidth(getpid(), 7,2), swidth(request.request_number,7,2), request.gender, swidth(request.time,7,2), "PEDIDO");
        write(genFifoFD, & request, sizeof(Request));
       

        
    }
    
    return NULL;
}

void* rejectedListener(void* arg){
    char keepReading = 1;
    while (keepReading){

        Request request;
        read(rejFifoFD, & request, sizeof(Request));
        if (request.gender == 'E') // End Marker 
            keepReading = 0;
        else if(request.rejection_number < 3){
	    double time=round(getProcTime()*100)/100;  
	   fprintf(gerpid,"%s - %s - %s: %c - %s - %s\n",swidth(time, 11,2), swidth(getpid(), 7,2), swidth(request.request_number,7,2), request.gender, swidth(request.time,7,2), "REJEITADO");
	    write(genFifoFD, & request, sizeof(Request));
            if (request.gender == 'M'){
                MRejected++;
            }
            else{
                FRejected++;
            }
       
	}
	else{
	    double time=round(getProcTime()*100)/100;  
	    fprintf(gerpid,"%s - %s - %s: %c - %s - %s\n",swidth(time, 11,2), swidth(getpid(), 7,2), swidth(request.request_number,7,2), request.gender, swidth(request.time,7,2), "DESCARTADO");
	    write(genFifoFD, & request, sizeof(Request));
            if (request.gender == 'M'){
                MRejected++;
		MDiscarded++;
            }
            else{
                FRejected++;
		FDiscarded++;
            }
	}
    } 
    return NULL;
}

void sigIntHandler(int signal){
	unlink("/tmp/entrada");
    exit(-6);
}
