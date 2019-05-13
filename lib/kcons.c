#include <kcons.h>

extern void set_console();

void
kcons_init(void)
{
	//printf("in kcons.c\n");
	set_console();
}
