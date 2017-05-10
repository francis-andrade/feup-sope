/*
ESTE PROGRAMA SERVE PARA VEREM COMO FUNCIONA A INTERFACE DE TEMPO
ATENÇAO: ESTE FICHEIRO É SO DE TESTES NAO É PARA SER POSTO NO MAKEFILE
*/

#include "miscFunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    clock_gettime(CLOCK_MONOTONIC_RAW, &processStart);
    sleep(5);
    double currTime = getProcTime();
    printf("Current Time is: %f (Should be around 5000)\n", currTime);
    sleep(20);
    currTime = getProcTime();
    printf("Current Time is: %f (Should be around 25000)\n", currTime);
}