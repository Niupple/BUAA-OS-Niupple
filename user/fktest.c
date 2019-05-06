#include "lib.h"


void umain()
{
	int a = 0;
	int id = 0;
	int i;

	writef("now in fktest, &id = %x\n", &id);
	if ((id = tfork()) == 0) {
		writef("son has returned\n");
		if ((id = tfork()) == 0) {
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
