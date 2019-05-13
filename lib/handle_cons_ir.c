#include <printf.h>

void handle_cons_ir(char c, int status) {

	static char buf[70] = {};
	static int len = 0;
	printf("cp0 status: %x\n", status);
	//printf("char is %d $$%c$$\n", c, c);
	buf[len++] = c;
	if(c == '\r') {
		buf[len] = '\0';
		printf("length: %d\n", len);
		printf("content: %s\n", buf);
		len = 0;
	}
}

