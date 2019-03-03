#include "packet_process.h"
#include "packet_receive.h"
#include <stdio.h>

int main(void) {
	initReceive();
	initProcess();
	return 0;
}