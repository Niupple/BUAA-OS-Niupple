// Ping-pong a counter between two processes.
// Only need to start one of these -- splits into two with fork.

#include "lib.h"

void
umain(void)
{
	//writef("I am ping pong_a, my envid is %d\n", syscall_getenvid());
	//writef("I got a %d\n", ipc_recv(NULL, NULL, NULL));
	int now = 0;
	while(1) {
		now = 1 + ipc_recv(NULL, NULL, NULL);
		writef("2048 got %d from 4097\n", now-1);
		if(now-1 >= 9) {
			break;
		}
		writef("@@@@@send %d from 2048 to 4097\n", now);
		ipc_send(4097, now, 0, 0);
		if(now >= 10) {
			break;
		}
		writef("2048 am waiting\n");
	}
}

