#include <stdio.h>
#include <stdbool.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include "msg_queue.h"
#include "packet_receive.h"
/*This file simulates the receiving functionality of our design by placing 32-byte hexidecimal value (packet) into a message queue at different intervals*/

void initReceive(void) {
	// ftok to generate unique key 
	key = ftok("progfile", 65);

	// msgget creates a message queue 
	// and returns identifier 
	msgID = msgget(key, 0666 | IPC_CREAT);
	msg.type = 1;

	printf("Write Data : ");
	gets(msg.text); //<-gets stdin
	//msg.text = txt;

	// msgsnd to send message 
	msgsnd(msgID, &msg, sizeof(msg), 0);

	// display the message 
	//printf("Data send is : %s \n", msg.text);

}

