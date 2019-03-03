#pragma once
#include <stdio.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct msg_queue {
	long type;
	char text[5];
};