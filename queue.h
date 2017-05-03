#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct queue_node queue_node;

struct queue_node{
	void * info;
	queue_node * next;
};

typedef struct{
	queue_node * first;
	unsigned int size;	
	
} queue;

queue * queue_constructor();

void pop(queue * q);

void * top(queue * q);

void * back(queue * q);

void push(queue * q, void * inf);

int empty(queue * q);

int size(queue * q);

void clear(queue * q);








#endif
