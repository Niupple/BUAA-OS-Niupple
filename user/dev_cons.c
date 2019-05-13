#include "lib.h"

char ugetc();

void uwritef(char *fmt, ...);

extern void user_lp_Print(void (*output)(void *, const char *, int), void *arg, const char *fmt, va_list ap);

char ugetc() {
	char c;
	do {
		syscall_read_dev(&c, (void *)0x10000000, sizeof(c));
	} while(c == 0);
	syscall_write_dev(&c, (void *)0x10000000, sizeof(c));
	return c;
}

static void user_myoutput(void *arg, const char *s, int l)
{
	int i;

	// special termination call
	if ((l == 1) && (s[0] == '\0')) {
		return;
	}

	char c;
	for (i = 0; i < l; i++) {
		//syscall_putchar(s[i]);
		syscall_write_dev(s+i, (void *)0x10000000, sizeof(c));

		if (s[i] == '\n') {
			//syscall_putchar('\n');
			c = '\n';
			syscall_write_dev(&c, (void *)0x10000000, sizeof(c));
		}
	}
}

void uwritef(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	user_lp_Print(user_myoutput, 0, fmt, ap);
	va_end(ap);
}
