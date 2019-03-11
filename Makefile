%.o: %.c
	gcc -c -o $@ $^ -Iinclude/

fibo: code/fibo.o code/main.o
	gcc -o fibo code/fibo.o code/main.o

.PHONY: clean
clean: code/fibo.o code/main.o
	rm -rf code/fibo.o code/main.o
