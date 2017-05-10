#ifndef MISC_FUNC_H_ 
#define MISC_FUNC_H_

#include <sys/time.h>
#include <time.h>

struct timespec processStart;


/**
*@brief The instruction 'clock_gettime(CLOCK_MONOTONIC_RAW, &processStart);' 
*must be where you want to start your counter
*@return Returns the current time in miliseconds since the begining of the process
*/
double getProcTime();

/**
*@brief This function returns a number in the form of a string with a certain width
*@param n number that is going to be passed to a string with a certain width
*@param width width of the string that is going to be returned
*@param decimal number of decimal places. They can only be 0, 1 or 2
*@return Returns a string
*/
char * swidth(double n, int width, int decimal);

#endif

