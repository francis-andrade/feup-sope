/*
ESTE PROGRAMA SERVE PARA VEREM COMO funciona a função swidth()
ATENÇAO: ESTE FICHEIRO NAO É PARA SER POSTO NO MAKEFILE
*/

#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

int main(void){
	printf("1 test swidth(567, 8, 0):%sfinish\n",swidth(567, 8, 0));
	printf("2 test swidth(54.980987,5,1):%sfinish\n",swidth(54.980987,5,1));
	printf("3 test swidth(545.005987,7,2):%sfinish\n",swidth(545.005987,7,2));
	printf("4 test swidth(100000,6,0):%sfinish\n",swidth(100000,6,0));
	printf("5 test swidth(1000000,12,1):%sfinish\n",swidth(1000000,12,1));
	printf("6 test swidth(24.1, 6, 2):%sfinish\n", swidth(24.1, 6, 2));
	printf("7 test swidth(23,5,2):%sfinish\n", swidth(23,5,2));
	printf("8 test swidth(1000,7,2):%sfinish\n", swidth(1000,7,2));
	printf("9 test swidth(1000,8,2):%sfinish\n", swidth(1000,8,2));
	printf("10 test swidth(1000,6,2):%sfinish\n", swidth(1000,6,2));

}
