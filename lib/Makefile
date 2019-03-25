INCLUDES := -I./ -I../ -I../include/
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: print.o printf.o 

test: main

main: print.o printf.o main.o

clean:
	rm -rf *~ *.o


include ../include.mk
