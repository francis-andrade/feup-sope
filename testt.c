/*
Este programa de teste serve para verem como funcionam as queues
*/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
typedef struct{
	unsigned int request_number;
	char gender;
	unsigned int time;
	unsigned int rejection_number;
}Request;


int main(void){
	Request * r1=malloc(sizeof(Request));
	Request * r2=malloc(sizeof(Request));
	Request * r3=malloc(sizeof(Request));
	Request * r4=malloc(sizeof(Request));
	Request * r5=malloc(sizeof(Request));

	r1->request_number=1;
	r2->request_number=2;
	r3->request_number=3;
	r4->request_number=4;
	r5->request_number=5;
	queue * q=queue_constructor();
	printf("size: %d\n", size(q));
	printf("empty: %d\n", empty(q));
	top(q);
	back(q);
	/*printf("top: %d\n", ((Request *)top(q))->request_number);
	printf("back: %d\n", ((Request *)back(q))->request_number);*/

	printf("Adds a few elements\n");
	push(q, r1);
	push(q, r2);
	push(q, r3);
	printf("size: %d\n", size(q));
	printf("empty: %d\n", empty(q));
	printf("top: %d\n", ((Request *)top(q))->request_number);
	printf("back: %d\n", ((Request *)back(q))->request_number);
	printf("It's now going to pop\n");
	pop(q);
	printf("size: %d\n", size(q));
	printf("empty: %d\n", empty(q));
	printf("top: %d\n", ((Request *) top(q))->request_number);
	printf("back: %d\n", ((Request *) back(q))->request_number);
	printf("Adds a few elements\n");
	push(q, r4);
	push(q, r5);
	printf("size: %d\n", size(q));
	printf("empty: %d\n", empty(q));
	printf("top: %d\n", ((Request *)top(q))->request_number);
	printf("back: %d\n", ((Request *)back(q))->request_number);
	printf("Pops a few elements\n");
	pop(q);
	pop(q);
	pop(q);
	pop(q);
	printf("size: %d\n", size(q));
	printf("empty: %d\n", empty(q));
	top(q);
	back(q);
	printf("Pops another time\n");
	pop(q);
	printf("size: %d\n", size(q));
	printf("empty: %d\n", empty(q));
	top(q);
	back(q);
	clear(q);
	
}
