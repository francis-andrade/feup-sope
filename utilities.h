#ifndef UTILITIES_H_ 
#define UTILITIES_H_

#include <sys/time.h>
#include <time.h>

struct timespec processStart;


/**
*@brief The instruction 'clock_gettime(CLOCK_MONOTONIC_RAW, &processStart);' 
*must be where you want to start your counter
*@return Returns the current time in miliseconds since the begining of the process
*/
double getProcTime();


#endif
