#ifndef REQUEST_H_
#define REQUEST_H_

typedef struct{
	unsigned int request_number;
	char gender;
	unsigned int time; // durantion in milliseconds
	unsigned int rejection_number; //N times rejected
}Request;

#endif
