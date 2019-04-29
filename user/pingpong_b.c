// Ping-pong a counter between two processes.
// Only need to start one of these -- splits into two with fork.

#include "lib.h"

void
umain(void)
{
	//writef("I am ping pong_b, my envid is %d\n", syscall_getenvid());
	int now = 0;
	while(1) {
		writef("@@@@@send %d from 4097 to 2048\n", now);
		ipc_send(2048, now, 0, 0);
		if(now >= 10) {
			break;
		}
		writef("4097 am waiting\n");
		now = 1 + ipc_recv(NULL, NULL, NULL);
		writef("4097 got %d from 2048\n", now-1);
		if(now-1 >= 10) {
			break;
		}
	}
}

