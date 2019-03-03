#pragma once
#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include "packet.h"
#include "msg_queue.h"

struct msg_queue msg;
key_t key;
int msgID;
void initProcess(void);