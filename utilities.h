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

/**
*@brief This function transforms a string in a number with a certain width (that is with a certain number of spaces to be determined on front of it)
*@param rs string that is going to be transformed
*@param n number that is going to be passed to a string with a certain width
*@param width width of the string that is going to be passed to rs
*@param decimal number of decimal places. They can only be 0, 1 or 2
*/
void swidth(char * rs, double n, int width, int decimal);

/**
*@brief This function transforms a string in a nstring with a certain width (that is with a certain number of spaces to be determined on front of it)
*@param rs string that is going to be transformed
*@param str string that is going to be passed to a string with a certain width
*@param width width of the string that is going to be passed to rs
*/
void strwidth(char * rs,char * str, int width);
#endif


