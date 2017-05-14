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
#include <string.h>
#include "utilities.h"

#define FIFO_PERM 0700
#define MAXTIME 50
#define MINTIME 0

//Prototypes
void* generateRequests(void* arg);
void* rejectedListener(void* arg);
void sigIntHandler(int signal);
void writetofile(unsigned long pid, unsigned int rnumber, char gender, unsigned int rtime, char * msg);

int genFifoFD;
int rejFifoFD;

unsigned int MGenerated = 0;
unsigned int FGenerated = 0;
unsigned int MRejected = 0;
unsigned int FRejected = 0;
unsigned int MDiscarded = 0;
unsigned int FDiscarded = 0;

int gerpid;

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
		fprintf(stderr, "Invalid number of max usage time, must be between 1 and 50\n");
		exit(3);
	}
	
    //Create entrada
	if(mkfifo("/tmp/entrada", FIFO_PERM) == -1){
        perror("Error on creating FIFO entrada");
        exit(4);
    }
    
    //Open entrada in writing mode
    if((genFifoFD = open("/tmp/entrada", O_WRONLY | O_CREAT)) == -1){
        perror("Fail on opening entrada for writing");
        exit(5);
    }

    //Open rejeitados in reading mode
    if ((rejFifoFD = open("/tmp/rejeitados", O_RDONLY)) == -1){
        perror("Fail on opening entrada for writing");
        exit(7);
    }

   //OPENS File to write the information    
   char filename[256];
   sprintf(filename, "/tmp/ger.%d",getpid());
   if((gerpid=open(filename, O_WRONLY | O_CREAT))==-1){
	perror("Error on creating the file to write the statistics");
	exit(8);
    }    

    //Send the number of total requests to the sauna
    write(genFifoFD, &nRequests, sizeof(nRequests));
    
    int generatorArgs[] = {nRequests, maxUsage};
    pthread_t genTID, rejTID;
    pthread_create(&genTID, NULL, generateRequests, generatorArgs);
    pthread_create(&rejTID, NULL, rejectedListener, NULL);
    
    pthread_join(genTID, NULL);
    pthread_join(rejTID, NULL);


     char buf2[256];
    sprintf(buf2,"\n\t\tEstatisticas:\n\n\tPedidos Gerados:\n\tHomens: %d\n\tMulheres: %d\n\tTotal: %d\n\n\tPedidos Rejeitados:\n\tHomens: %d\n\tMulheres: %d\n\tTotal: %d\n\n\tPedidos Descartados:\n\tHomens: %d\n\tMulheres: %d\n\tTotal: %d\n", MGenerated, FGenerated, MGenerated + FGenerated, MRejected, FRejected, MRejected + FRejected, MDiscarded, FDiscarded, MDiscarded + FDiscarded);
    write(STDOUT_FILENO, buf2, strlen(buf2));

    close(genFifoFD);
    close(rejFifoFD);
    close(gerpid);
    unlink("/tmp/entrada");

    return 0;
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
	writetofile(getpid(), request.request_number, request.gender,request.time, "PEDIDO");
        write(genFifoFD, & request, sizeof(Request));
         
    }

    pthread_exit(NULL);
}

void* rejectedListener(void* arg){
    char keepReading = 1;

    while (keepReading){

        Request request;
        read(rejFifoFD, & request, sizeof(Request));
        
        if (request.gender == 'E') // End Marker 
            keepReading = 0;
        
        else if(request.rejection_number < 3){
	        
             writetofile(getpid(), request.request_number, request.gender,request.time, "REJEITADO");
	        write(genFifoFD, & request, sizeof(Request));
           
            if (request.gender == 'M'){
                MRejected++;
            }
            
            else{
                FRejected++;
            }
	 }
	 else{
	        

            writetofile(getpid(), request.request_number, request.gender, request.time, "DESCARTADO");

            
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
    pthread_exit(NULL);
}

void sigIntHandler(int signal){
	unlink("/tmp/entrada");
    exit(-6);
}



void writetofile(unsigned long pid, unsigned int rnumber, char gender, unsigned int rtime, char * msg){
	char str[256];
	char buf[256];

	double time=round(getProcTime()*100)/100;  
	char stime[256];
	swidth(stime, time, 11,2);
	
	sprintf(buf, "%lu", pid);
	char spid[256];
	strwidth(spid, buf, 8);
	
	char srnumber[256];
	swidth(srnumber, rnumber, 7, 0);

	char srtime[256];
	swidth(srtime, rtime, 7, 0);

	sprintf(str,"%s   - %s   - %s:   %c - %s   -  %s\n",stime, spid, srnumber, gender, srtime, msg);
	write(gerpid, str, strlen(str));
}
