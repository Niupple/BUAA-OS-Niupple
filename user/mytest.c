#include "lib.h"
//#include <printf.h>

void umain(void) {
	writef("I am here!!!\n");
	syscall_putchar(':');
	syscall_putchar(')');

	return 0;
}
