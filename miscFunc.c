#include "miscFunc.h"
#include <sys/time.h>

double getProcTime(){
    struct timeval currTime;
    gettimeofday(&currTime, NULL);
    
    double uSec = (currTime.tv_sec - processStart.tv_sec) * 1000000 + (currTime.tv_nsec - processStart.tv_nsec);
    uSec = (int)uSec / 10;
    uSec = uSec/100;
    return  uSec;
}
