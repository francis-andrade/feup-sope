#ifndef MISC_FUNC_H_ 
#define MISC_FUNC_H_

#include <sys/time.h>
#include <time.h>

struct timespec processStart;


/**
*@brief The instruction 'clock_gettime(CLOCK_MONOTONIC_RAW, &processStart);' 
*must be at the begining of the process
*@return Returns the current time in miliseconds since the begining of the process
*/
double getProcTime();

#endif

