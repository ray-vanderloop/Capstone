#pragma once
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdint.h>
#include <semaphore.h>
#include "packet.h"



struct msg {
	char text[65]; //size 65 for the 64 bits of data + null terminator at the end
};


sem_t cntSem; //counting semaphore used to send signal from bufferWrite function to packetProcess function
sem_t keySem; //key semaphore to protect shared msgBuffer variable

//This buffer is a circular buffer (circular part is implemented through the code)
struct buffer {
	uint8_t writeIndex;
	uint8_t readIndex;
	struct msg msgBuffer[100]; //size of buffer should depend on how fast you are receiving data vs processing it. It should be large enough so the read and write indexes are never the same.
};

