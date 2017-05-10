#include "utilities.h"
#include <sys/time.h>

double getProcTime(){
    struct timespec currTime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &currTime);
    
    double mSec = ((currTime.tv_sec - processStart.tv_sec) * 1000000000 + (currTime.tv_nsec - processStart.tv_nsec)) / 1000;
    mSec = (int)mSec / 10;
    mSec = mSec/100;
    return  mSec;
}
