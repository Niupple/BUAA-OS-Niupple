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

#include	<print.h>

/* macros */
#define		IsDigit(x)	( ((x) >= '0') && ((x) <= '9') )
#define		Ctod(x)		( (x) - '0')

/* forward declaration */
extern int PrintChar(char *, char, int, int);
extern int PrintString(char *, char *, int, int);
extern int PrintNum(char *, unsigned long, int, int, int, int, char, int);
extern int PrintArray(char *, void *, int, int, int, int, char);

/* private variable */
static const char theFatalMsg[] = "fatal error in lp_Print!";

/* -*-
 * A low level printf() function.
 */
	void
lp_Print(void (*output)(void *, char *, int), 
		void * arg,
		char *fmt, 
		va_list ap)
{

#define 	OUTPUT(arg, s, l)  \
	{ if (((l) < 0) || ((l) > LP_MAX_BUF)) { \
											   (*output)(arg, (char*)theFatalMsg, sizeof(theFatalMsg)-1); for(;;); \
										   } else { \
											   (*output)(arg, s, l); \
										   } \
	}

	char buf[LP_MAX_BUF];

	void *arr;
	char c;
	char *s;
	long int num;

	int arrSize;
	int longFlag;
	int negFlag;
	int width;
	int prec;
	int ladjust;
	int arrFlag;
	int numSize;
	char padc;

	int length;
	int state = 0;

	for(; *fmt; ++fmt) {
		{ 
			if (state == 0) {
				if (*fmt != '%') {
					OUTPUT(arg, fmt, 1);
				} else {
					state = 1;
					ladjust = 0;
					padc = ' ';
					prec = 0;
					width = 0;
					longFlag = 0;
					arrFlag = 0;
					arrSize = 0;
					numSize = 0;
				}
				continue;
			} else if(state == 1) {
				if(*fmt == '-') {
					ladjust = 1;
				} else if(*fmt == '0') {
					padc = '0';
				} else {
					state = 2;
					--fmt;
				}
				continue;
			} else if(state == 2) {
				if(IsDigit(*fmt)) {
					width = width*10+Ctod(*fmt);
				} else {
					--fmt;
					state = 3;
				}
				continue;
			} else if(state == 3) {
				if(*fmt == '.') {
					state = 4;
				} else {
					--fmt;
					state = 5;
				}
				continue;
			} else if(state == 4) {
				if(IsDigit(*fmt)) {
					prec = prec*10+Ctod(*fmt);
				} else {
					--fmt;
					state = 5;
				}
				continue;
			} else if(state == 5) {
				if(*fmt == '#') {
					arrFlag = 1;
					state = 6;
				} else {
					--fmt;
					state = 7;
				}
				continue;
			} else if(state == 6) {
				if(IsDigit(*fmt)) {
					arrSize = arrSize*10+Ctod(*fmt);
				} else {
					--fmt;
					state = 7;
				}
				continue;
			} else if(state == 7) {
				if(*fmt == 'l') {
					longFlag = 1;
				} else {
					--fmt;
				}
				state = 8;
				continue;
			}
			state = 0;

			/* scan for the next '%' */
			/* flush the string found so far */
			/* are we hitting the end? */
		}

		/* we found a '%' */

		/* check for long */

		/* check for other prefixes */

		/* check format flag */
		negFlag = 0;
		switch (*fmt) {
			case 'a':
			case 'A':
				if(longFlag) {
					numSize = sizeof(long int);
				} else {
					numSize = sizeof(int);
				}
				arr = va_arg(ap, void*);
				//printf("numSize = %d, arrSize = %d, width = %d\n", numSize, arrSize, width);
				length = PrintArray(buf, arr, numSize, arrSize, width, ladjust, padc);
				OUTPUT(arg, buf, length);
				break;

			case 'b':
				if (longFlag) { 
					num = va_arg(ap, long int); 
				} else { 
					num = va_arg(ap, int);
				}
				length = PrintNum(buf, num, 2, 0, width, ladjust, padc, 0);
				OUTPUT(arg, buf, length);
				break;

			case 'd':
			case 'D':
				if (longFlag){ 
					num = va_arg(ap, long int);
				} else { 
					num = va_arg(ap, int); 
				}
				if (num < 0) {
					num = - num;
					negFlag = 1;
				}
				length = PrintNum(buf, num, 10, negFlag, width, ladjust, padc, 0);
				OUTPUT(arg, buf, length);
				break;

			case 'o':
			case 'O':
				if (longFlag) { 
					num = va_arg(ap, long int);
				} else { 
					num = va_arg(ap, int); 
				}
				length = PrintNum(buf, num, 8, 0, width, ladjust, padc, 0);
				OUTPUT(arg, buf, length);
				break;

			case 'u':
			case 'U':
				if (longFlag) { 
					num = va_arg(ap, long int);
				} else { 
					num = va_arg(ap, int); 
				}
				length = PrintNum(buf, num, 10, 0, width, ladjust, padc, 0);
				OUTPUT(arg, buf, length);
				break;

			case 'x':
				if (longFlag) { 
					num = va_arg(ap, long int);
				} else { 
					num = va_arg(ap, int); 
				}
				length = PrintNum(buf, num, 16, 0, width, ladjust, padc, 0);
				OUTPUT(arg, buf, length);
				break;

			case 'X':
				if (longFlag) { 
					num = va_arg(ap, long int);
				} else { 
					num = va_arg(ap, int); 
				}
				length = PrintNum(buf, num, 16, 0, width, ladjust, padc, 1);
				OUTPUT(arg, buf, length);
				break;

			case 'c':
				c = (char)va_arg(ap, int);
				length = PrintChar(buf, c, width, ladjust);
				OUTPUT(arg, buf, length);
				break;

			case 's':
				s = (char*)va_arg(ap, char *);
				length = PrintString(buf, s, width, ladjust);
				OUTPUT(arg, buf, length);
				break;

			case '\0':
				fmt --;
				break;

			default:
				/* output this char as it is */
				OUTPUT(arg, fmt, 1);
		}	/* switch (*fmt) */

	}		/* for(;;) */

	/* special termination call */
	OUTPUT(arg, "\0", 1);
}


/* --------------- local help functions --------------------- */
	int
PrintChar(char * buf, char c, int length, int ladjust)
{
	int i;

	if (length < 1) length = 1;
	if (ladjust) {
		*buf = c;
		for (i=1; i< length; i++) buf[i] = ' ';
	} else {
		for (i=0; i< length-1; i++) buf[i] = ' ';
		buf[length - 1] = c;
	}
	return length;
}

	int
PrintString(char * buf, char* s, int length, int ladjust)
{
	int i;
	int len=0;
	char* s1 = s;
	while (*s1++) len++;
	if (length < len) length = len;

	if (ladjust) {
		for (i=0; i< len; i++) buf[i] = s[i];
		for (i=len; i< length; i++) buf[i] = ' ';
	} else {
		for (i=0; i< length-len; i++) buf[i] = ' ';
		for (i=length-len; i < length; i++) buf[i] = s[i-length+len];
	}
	return length;
}

int
PrintArray(char *buf, void *a, int width, int size, int length, int ladjust, char padc)
{
	int i;
	int negFlag;
	int ret = 0;
	long tmp;
	ret += PrintChar(buf, '{', 0, 0);	//print {
	for(i = 0; i < size; ++i) {
		if(width == sizeof(int)) {
			tmp = *((int *)(a+i*width));
		} else {
			tmp = *((long int *)(a+i*width));
		}
		negFlag = 0;
		if(tmp < 0) {
			tmp = -tmp;
			negFlag = 1;
		}
		ret += PrintNum(buf+ret, tmp, 10, negFlag, length, ladjust, padc, 0);
		if(i < size-1) {
			ret += PrintChar(buf+ret, ',', 0, 0);	//print ,
		}
	}
	ret += PrintChar(buf+ret, '}', 0, 0);	//print }
	return ret;
}

	int
PrintNum(char * buf, unsigned long u, int base, int negFlag, 
		int length, int ladjust, char padc, int upcase)
{
	/* algorithm :
	 *  1. prints the number from left to right in reverse form.
	 *  2. fill the remaining spaces with padc if length is longer than
	 *     the actual length
	 *     TRICKY : if left adjusted, no "0" padding.
	 *		    if negtive, insert  "0" padding between "0" and number.
	 *  3. if (!ladjust) we reverse the whole string including paddings
	 *  4. otherwise we only reverse the actual string representing the num.
	 */

	int actualLength =0;
	char *p = buf;
	int i;

	do {
		int tmp = u %base;
		if (tmp <= 9) {
			*p++ = '0' + tmp;
		} else if (upcase) {
			*p++ = 'A' + tmp - 10;
		} else {
			*p++ = 'a' + tmp - 10;
		}
		u /= base;
	} while (u != 0);

	if (negFlag) {
		*p++ = '-';
	}

	/* figure out actual length and adjust the maximum length */
	actualLength = p - buf;
	if (length < actualLength) length = actualLength;

	/* add padding */
	if (ladjust) {
		padc = ' ';
	}
	if (negFlag && !ladjust && (padc == '0')) {
		for (i = actualLength-1; i< length-1; i++) buf[i] = padc;
		buf[length -1] = '-';
	} else {
		for (i = actualLength; i< length; i++) buf[i] = padc;
	}


	/* prepare to reverse the string */
	{
		int begin = 0;
		int end;
		if (ladjust) {
			end = actualLength - 1;
		} else {
			end = length -1;
		}

		while (end > begin) {
			char tmp = buf[begin];
			buf[begin] = buf[end];
			buf[end] = tmp;
			begin ++;
			end --;
		}
	}

	/* adjust the string pointer */
	return length;
}
