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

//Prototypes
void* generateRequests(void* arg);


int genFifoFD;
//int nRequests;
Request* queue;

int main(int argc, char* argv[]) {

	if (argc != 3){
		fprintf(stderr, "Wrong usage: ./gerador <n. requests> <max. usage time>\n");
		exit(1);
	}
	
	int nRequests = atoi(argv[1]);

	if (nRequests <= 0){
		fprintf(stderr, "Invalid number of requests\n");
		exit(2);
	}

	int maxUsage = atoi(argv[2]);

	if (maxUsage <= 1 || maxUsage >= 999){
		fprintf(stderr, "Invalid number of max usage time\n");
		exit(3);
	}
	
	if(mkfifo("/tmp/entrada", FIFO_PERM) == -1)
        perror("Error on creating FIFO entrada");    
    
    queue = malloc(maxUsage * sizeof(Request));
    
    int generatorArgs[] = {nRequests, maxUsage};
    pthread_t genTID;
    pthread_create(&genTID, NULL, generateRequests, generatorArgs);
    
   
    pthread_join(genTID, NULL);
	free(queue);
    unlink("/tmp/entrada");

}

void* generateRequests(void* arg){
    
    if((genFifoFD = open("/temp/entrada", O_WRONLY | O_CREAT)) == -1)
        perror("Fail on opening entrada for writing");
    
    int nRequests = ((int*) arg)[0];
    int maxUsage = ((int*) arg)[1];
    
    Request request;
    srand(time(NULL));
    
    int i;
    for(i = 0; i < nRequests; i++){
        request.request_number = i + 1;
        
        if(rand() % 2) //0 or 1 upper boundry is included
            request.gender = 'M';
        else
            request.gender = 'F';
        
        request.time = rand() % (maxUsage + 1);
        request.rejection_number = 0;        
        
    }
    
    return NULL;
}
