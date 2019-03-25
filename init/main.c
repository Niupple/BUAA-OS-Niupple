/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>

int main()
{
	int a[] = {1, 2, 10000, 1000};
	long int b[] = {1, 2, 4, 8};
	printf("main.c:\tmain is start ...\n");
	printf("array test: %#4a, %4#4a, %04#4a, %-4#4a\n", a, a, a, a);
	printf("empty %#0a, long %#4la\n", b, b);

	mips_init();
	panic("main is over is error!");

	return 0;
}
