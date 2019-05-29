#include "lib.h"



static void user_out2string(void *arg, char *s, int l)
{
	int i;
	char * b = (char *)arg;
	// special termination call
	if ((l==1) && (s[0] == '\0')) return;

	u_int buf_len = strlen(b);
	for (i=0; i< l; i++) {
		b[buf_len+i]=s[i];
	}
	b[buf_len+i] = '\0';
}


int fwritef(int fd, const char *fmt, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, fmt);
	buf[0] = 0;
	user_lp_Print(user_out2string, buf, fmt, ap);
	va_end(ap);
	return write(fd, buf, strlen(buf));
}
