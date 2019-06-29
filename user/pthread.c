#include <env.h>
#include <thread.h>
#include <error.h>
#include "lib.h"

static int
thrdid2thrd(u_int thrdid, struct Thrd **thrd) {
	int r;
	struct Thrd *t;

	if(thrdid == 0) {
		thrdid = pthread_self();
	}

	t = (struct Thrd *)UTHRD(THRDX(thrdid));
	if ((t->thrd_status & THRD_RUN_MASK) == THRD_FREE || t->thrd_id != thrdid) {
		return -E_INVAL;
	}
	*thrd = t;
	//writef("thrdid %d = addr %x\n", thrdid, t);
	return 0;
}

pthread_t
pthread_self(void) {
	return syscall_thread_self();
}

void
run_and_sleep(void *(*start_routine)(void *), void *arg)
{
	//writef("in run and sleep\n");
	start_routine(arg);
	syscall_thread_finish(0);
}

int
pthread_create(pthread_t *thread, void *attr, void *(*start_routine)(void *), void *arg)
{
	int r;
	struct Thrd *t;
	pthread_t tid;
	pthread_t curthrdid;

	//writef("run and sleep = %x\n", run_and_sleep);
	tid = syscall_thread_create(start_routine, arg, run_and_sleep);
	if (thread != NULL) {
		*thread = tid;
	}
	return 0;
}

int
pthread_cancel(pthread_t thread)
{
	struct Thrd *t;
	int r;

	if((r = thrdid2thrd(thread, &t)) < 0) {
		return r;
	}
	t->thrd_status = (t->thrd_status & ~THRD_CANING_MASK) | THRD_CANCELING;

	if((t->thrd_status & THRD_CANST_MASK) == THRD_CANCEL_ENABLE && (t->thrd_status & THRD_CANTP_MASK) == THRD_CANCEL_ASYNC) {
		syscall_thread_cancel(thread);
	}
	return 0;
}

void
pthread_testcancel(void)
{
	struct Thrd *t;
	int r;
	if((r = thrdid2thrd(0, &t)) < 0) {
		return r;
	}

	if ((t->thrd_status & THRD_CANST_MASK) == THRD_CANCEL_DISABLE) {
		return;
	}
	if ((t->thrd_status & THRD_CANING_MASK) == THRD_CANCELING) {
		syscall_thread_cancel(0);
	}
}

void
pthread_exit(void *retval)
{
	struct Thrd *t;
	int r;

	if((r = thrdid2thrd(0, &t)) < 0) {
		return r;
	}
	t->thrd_retval = retval;
	syscall_thread_finish(0);
	writef("have never reached here\n");
	user_panic("QAQ\n");
}

int
pthread_join(pthread_t thread, void **retval) {
	writef("in pthread_join with %d\n", thread);
	struct Thrd *t;
	int r;

	if((r = thrdid2thrd(thread, &t)) < 0) {
		return r;
	}
	//writef("addr = %x\n", t);
	if(t->thrd_join_id == NULL) {
		//writef("join_id set\n");
		t->thrd_join_id = pthread_self();
	} else {
		return -E_INVAL;	// join twice
	}
	while((t->thrd_status & THRD_JOIN_MASK) == THRD_JOINABLE && (t->thrd_status & THRD_SCHED_MASK) == THRD_SCHED_RUN) {
		// wait
		//writef("waiting\n");
		syscall_yield();
	}
	//writef("sta = %x\n", t->thrd_status);
	if (retval != NULL) {
		//writef("retrived retval as %x\n", t->thrd_retval);
		*retval = t->thrd_retval;
	}
	user_assert(syscall_thread_destroy(t->thrd_id) == 0);
	return 0;
}

int
pthread_detach(pthread_t thread) {
	struct Thrd *t;
	int r;

	if((r = thrdid2thrd(thread, &t)) < 0) {
		return r;
	}
	t->thrd_status = (t->thrd_status & ~THRD_JOIN_MASK) | THRD_DETACHED;

	if((t->thrd_status & THRD_SCHED_MASK) == THRD_SCHED_FINISHED) {
		syscall_thread_finish(thread);
	}
	return 0;
}

