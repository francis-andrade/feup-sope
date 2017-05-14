#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

double getProcTime(){
    struct timespec currTime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &currTime);
    
    double mSec = ((currTime.tv_sec - processStart.tv_sec) * 1000000000 + (currTime.tv_nsec - processStart.tv_nsec)) / 1000;
    mSec = (int)mSec / 10;
    mSec = mSec/100;
    return  mSec;
}

int ndigits(double d){
	int r=1;
	unsigned long k=1;
	while((k*10)<=d){
		k=k*10;
		r++;
	}
	return r;
}

int min(int a, int b){
	if(a>b){
		return b;
	}
	else{
		return a;
	}
}

void swidth(char * rs, double n, int width, int decimal){
	if(decimal==0){
		unsigned long r=(unsigned long) (round(n));
		int dig=ndigits(n);
		int i;
		for(i=0;i<width-dig;i++){
			rs[i]=' ';
		}
		sprintf(& rs[width-dig], "%lu",r);
	}
	else if(decimal==1){
		int r=(int) (round(n*10));
		int dig=ndigits(r);
		if(r/10==0){
			dig++;
		}
		int i;
		for(i=0;i<width-dig-1;i++){
			rs[i]=' ';
		}
		sprintf(& rs[width-dig-1], "%d.%d",r/10, r%10);
	}
	else if(decimal==2){
		int r=(int) (round(n*100));
		int dig=ndigits(r);
		if(r/100==0){
			dig++;
		}
		int i;
		for(i=0;i<width-dig-1;i++){
			rs[i]=' ';
		}
		if(r%100<10){
			sprintf(& rs[width-dig-1], "%d.0%d",r/100, r%100);
		}
		else{
			sprintf(& rs[width-dig-1], "%d.%d",r/100, r%100);
		}
	}
	
}

void strwidth(char * rs,char * str, int width){
	if(strlen(str)>width){
		return;	
	}
	int i;
	for(i=0;i<width-strlen(str);i++){
		rs[i]=' ';
	}
	for(i=width-strlen(str);i<width;i++){
		rs[i]=str[i+strlen(str)-width];
	}
	rs[width]=0;
}
