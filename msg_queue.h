#pragma once
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdint.h>
#include <semaphore.h>
#include "packet.h"

struct msg {
	char text[64];;
};
sem_t cntSem;
sem_t keySem;

struct buffer {
	uint8_t writeIndex;
	uint8_t readIndex;
	//sem_t cntrSem;
	//sem_t keySem;
	struct msg msgBuffer[20];
};
