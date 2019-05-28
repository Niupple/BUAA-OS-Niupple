#include "lib.h"
#define ARRAYSIZE (1024*10)

int bigarray[ARRAYSIZE] = {};

void
umain(int argc, char **argv) {
	int i;
	writef("in bss test\n");
	for(i = 0; i < ARRAYSIZE; ++i) {
		if(bigarray[i] != 0) {
			user_panic("bigarray[%d] != 0\n", i);
		}
		bigarray[i] = i;
	}
	for(i = 0; i < ARRAYSIZE; ++i) {
		if(bigarray[i] != i) {
			user_panic("bigarray[%d] != %d\n", i, i);
		}
	}
	writef("bsstest succeed\n");
}
