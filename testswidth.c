/*
ESTE PROGRAMA SERVE PARA VEREM COMO funciona a função swidth()
ATENÇAO: ESTE FICHEIRO NAO É PARA SER POSTO NO MAKEFILE
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "utilities.h"

int main(void){
	char str[256];
	swidth(str, 567, 8, 0);
	printf("1 test swidth(567, 8, 0):%sfinish\n",str);

	swidth(str, 54.980987,5,1);
	printf("2 test swidth(54.980987,5,1):%sfinish\n",str);

	swidth(str, 545.005987,7,2),
	printf("3 test swidth(545.005987,7,2):%sfinish\n",str);

	swidth(str, 100000,6,0);
	printf("4 test swidth(100000,6,0):%sfinish\n",str);

	swidth(str, 1000000,12,1);
	printf("5 test swidth(1000000,12,1):%sfinish\n",str);

	swidth(str, 24.1, 6, 2);
	printf("6 test swidth(24.1, 6, 2):%sfinish\n", str);

	swidth(str, 23,5,2);
	printf("7 test swidth(23,5,2):%sfinish\n", str);

	swidth(str, 1000,7,2);
	printf("8 test swidth(1000,7,2):%sfinish\n", str);

	swidth(str, 1000,8,2);
	printf("9 test swidth(1000,8,2):%sfinish\n", str);

	swidth(str, 1000,6,2);
	printf("10 test swidth(1000,6,2):%sfinish\n", str);

	swidth(str, 0.378,6,2);
	printf("11 test swidth(0.378,6,2):%sfinish\n", str);

	swidth(str, 1.378,6,2);
	printf("12 test swidth(1.378,6,2):%sfinish\n", str);

	printf("thread: %lu\n",pthread_self());
	char buf[256];
	sprintf(buf,"%lu", pthread_self());
	strwidth(str, buf, 16);
	printf("13 test strwidth(pthread_self(),16):%sfinish\n", str);


}
