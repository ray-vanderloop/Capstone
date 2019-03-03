#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include "msg_queue.h"

struct msg_queue msg;
key_t key;
int msgID;
void initReceive(void);
