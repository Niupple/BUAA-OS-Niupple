#include "lib.h"


void umain()
{
	//while(1);
	//writef("I am fktest, my envid is %d\n", syscall_getenvid());
	//syscall_putchar('c');
	//syscall_yield();
	//while(1);
	/*
	writef("sending\n");
	ipc_send(4097, 233, 0, 0);
	writef("wtf\n");
	while(1);
	//syscall_panic("wtf\n");
	*/
	int a = 0;
	int id = 0;
	int i;

	writef("now in fktest, &id = %x\n", &id);
	/*
	if((id = fork()) == 0) {
		writef("%d: this is son\n", syscall_getenvid());
		return;
	} else {
		//while(1);
		writef("%d: this is father, sonid is %d\n", syscall_getenvid(), id);
		return;
	}
	*/
	if ((id = fork()) == 0) {
		writef("son has returned\n");
		if ((id = fork()) == 0) {
			a += 3;

			for(;;){
				writef("\t\tthis is child2 :a:%d\n", a);
			}
		}

		a += 2;

		for(;;){
			writef("\tthis is child :a:%d\n", a);
		}
	}
	writef("father has returned\n");

	a++;

	for(;;){
		writef("%d: this is father: a:%d\n", syscall_getenvid(), a);
	}
}
