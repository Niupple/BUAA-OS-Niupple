#include "lib.h"


void umain()
{
	//while(1);
	//syscall_getenvid();
	//syscall_putchar('c');
	//syscall_yield();
	//syscall_panic("wtf\n");
	//while(1);
	//writef("here!!!!!!!!!!\n");
	int a = 0;
	int id = 0;

	if ((id = fork()) == 0) {
		if ((id = fork()) == 0) {
			a += 3;

			for (;;) {
				writef("\t\tthis is child2 :a:%d\n", a);
			}
		}

		a += 2;

		for (;;) {
			writef("\tthis is child :a:%d\n", a);
		}
	}

	a++;

	for (;;) {
		writef("this is father: a:%d\n", a);
	}
}
