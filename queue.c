#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


queue * queue_constructor(){
	queue * q=malloc(sizeof(queue));
	q->first=malloc(sizeof(queue_node));
	q->first->info=NULL;
	q->first->next=NULL;
	q->size=0;
	return q;
}

void pop(queue * q){
	if(q->size>0){
		q->first=q->first->next;
		q->size--;

	}
}

void * top(queue * q){
	if(q->size>0){
		return q->first->info;
	}
	else{
		return NULL;
	}
}

void * back(queue * q){
	if(q->size>0){
		int i=1;
		queue_node * qn=q->first;
		while(i<q->size){
			qn=qn->next;
			i++;
		}
		return qn->info;
	}
	else{
		return NULL;
	}
}

void push(queue * q, void * inf){
	if(q->size==0){
		q->first->info=inf;
		q->size++;
	}
	else{
		int i=1;
	queue_node * qn=q->first;
		while(i<q->size){
			qn=qn->next;
			i++;
		}
		qn->next=malloc(sizeof(queue_node));
		qn->next->info=inf;
		qn->next->next=NULL;
		q->size++;
	}
}

int empty(queue * q){
	return (q->size==0);
}

int size(queue * q){
	return q->size;
}

void clear(queue * q){
	int i=1;
	queue_node * qn=q->first;
	while(i<q->size){
		qn=qn->next;
		free(q->first);
		q->first=qn;
		i++;
	}
	free(q);
}
