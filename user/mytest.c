#include "lib.h"
//#include <printf.h>

int recursive_test(int nowat, int to) {
	int r;
	if(nowat >= to) {
		return nowat;
	} else {
		writef("recursion at %d\n", nowat);
		r = recursive_test(nowat+1, to);
		writef("return from r\n");
	}
	return nowat;
}

void *subthread(void *args) {
	writef("thread works alright\n");
	return NULL;
}

void jointest() {
	pthread_t tid;
	pthread_create(&tid, NULL, subthread, NULL);
	user_assert(pthread_join(tid, NULL) == 0);
	writef("join test passed\n");
}

void detachtest() {
	pthread_t tid;
	pthread_create(&tid, NULL, subthread, NULL);
	user_assert(pthread_detach(tid) == 0);
	writef("detach test passed\n");
}

void *thread_with_retval(void *args) {
	writef("thread with retval, args = %x\n", args);
	pthread_exit(args);
	return NULL;
}

void exittest() {
	void *ret;
	pthread_t tid;
	pthread_create(&tid, NULL, thread_with_retval, &tid);
	user_assert(pthread_join(tid, &ret) == 0);
	writef("got retval = %x\n", ret);
}

void *thread_canceling(void *args) {
	int i;
	for (i = 1; i; ++i) {
		writef("test canceling %d\n", i);
		pthread_testcancel();
	}
	return NULL;
}

void cancel_test() {
	void *ret;
	pthread_t tid;
	pthread_create(&tid, NULL, thread_canceling, &tid);
	user_assert(pthread_cancel(tid) == 0);
	user_assert(pthread_join(tid, &ret) == 0);
	writef("cancel and join, with retval of %d\n", ret);
}

void pv_test1() {
	sem_t s, *p;
	int r, v;

	p = &s;
	sem_init(p, 0, 0);
	user_assert(sem_getvalue(p, &v) == 0);
	writef("sem = %d\n", v);
	sem_post(p);
	sem_post(p);
	user_assert(sem_getvalue(p, &v) == 0);
	writef("sem = %d\n", v);
	sem_trywait(p);
	user_assert(sem_getvalue(p, &v) == 0);
	writef("sem = %d\n", v);
	sem_wait(p);
	user_assert(sem_getvalue(p, &v) == 0);
	writef("sem = %d\n", v);

	user_assert(sem_trywait(p) == -E_AGAIN);
	sem_wait(p);
	writef("this line shall not be printed\n");
}

sem_t *ping, *pong;

void *thread_pong(void *args) {
	int i;
	for (i = 0; i < 10; ++i) {
		sem_wait(pong);
		writef("pong +%d+\n", i);
		sem_post(ping);
	}
}

void pv_test_pingpong() {
	int i;
	sem_t a, b;
	pthread_t p;

	ping = &a;
	pong = &b;
	sem_init(ping, 0, 0);
	sem_init(pong, 0, 1);
	user_assert(pthread_create(&p, NULL, thread_pong, NULL) == 0);
	for (i = 0; i < 10; ++i) {
		sem_wait(ping);
		writef("ping -%d-\n", i);
		sem_post(pong);
	}
	user_assert(pthread_join(p, NULL) == 0);
	writef("ping pong test succeed\n");
}

void *thread_keyholder(void *args) {
	int i;
	sem_t *p = (sem_t *)args;
	for (i = 0; i < 10; ++i) {
		syscall_yield();
	}
	writef("the keeper opened the door\n");
	sem_post(p);
}

void pv_test_block() {
	sem_t a, *p;
	pthread_t pt;

	p = &a;
	user_assert(sem_init(p, 0, 0) == 0);
	user_assert(pthread_create(&pt, NULL, thread_keyholder, (void *)p) == 0);
	user_assert(pthread_detach(pt) == 0);
	sem_wait(p);
	writef("the waiter entered the door\n");
}

void umain(void) {
	int r;
	writef("I am here!!!\n");

	//exittest();
	//cancel_test();
	//pv_test1();
	pv_test_pingpong();
	//pv_test_block();
	while(1);

}
