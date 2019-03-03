#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include "packet.h"
#include "msg_queue.h"
#include "packet_process.h"
/*
This file reads from the message queue (created in packet_receive.c) where received packets are stored and processes them based on the header data*/

void initProcess(void)
{
	// ftok to generate unique key 
	key = ftok("progfile", 65);

	// msgget creates a message queue 
	// and returns identifier 
	msgID = msgget(key, 0666 | IPC_CREAT);

	// msgrcv to receive message 
	msgrcv(msgID, &msg, sizeof(msg), 1, 0);

	// display the message 
	print("Data Received is : %s \n",
		msg.text);

	// to destroy the message queue 
	//msgctl(msgID, IPC_RMID, NULL);
}