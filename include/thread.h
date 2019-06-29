/* Copyright dyj 2019 (c) */

#ifndef _THREAD_H_
#define _THREAD_H_ 1

#include "queue.h"

#define _SIZE_THRD_ 256

#define MAX_THREAD	64
#define STACK_SIZE	0x100000
#define XSTACK_SIZE	2*BY2PG

#define THRD_RUN_BASE		0
#define THRD_FREE			(0 << (THRD_RUN_BASE))
#define THRD_RUNNABLE		(1 << (THRD_RUN_BASE))
#define THRD_NOT_RUNNABLE	(2 << (THRD_RUN_BASE))
#define THRD_RUN_MASK		(3 << (THRD_RUN_BASE))
#define THRD_JOIN_BASE		(THRD_RUN_BASE + 2)
#define THRD_JOINABLE		(0 << (THRD_JOIN_BASE))
#define THRD_DETACHED		(1 << (THRD_JOIN_BASE))
#define THRD_JOIN_MASK		(1 << (THRD_JOIN_BASE))
#define THRD_CANST_BASE		(THRD_JOIN_BASE + 1)
#define THRD_CANCEL_ENABLE	(0 << (THRD_CANST_BASE))
#define THRD_CANCEL_DISABLE (1 << (THRD_CANST_BASE))
#define THRD_CANST_MASK		(1 << (THRD_CANST_BASE))
#define THRD_CANTP_BASE		(THRD_CANST_BASE + 1)
#define THRD_CANCEL_DEFER	(0 << (THRD_CANST_BASE))
#define THRD_CANCEL_ASYNC	(1 << (THRD_CANST_BASE))
#define THRD_CANTP_MASK		(1 << (THRD_CANST_BASE))
#define THRD_CANING_BASE	(THRD_CANTP_BASE + 1)
#define THRD_NOT_CANCELING	(0 << (THRD_CANING_BASE))
#define THRD_CANCELING		(1 << (THRD_CANING_BASE))
#define THRD_CANING_MASK	(1 << (THRD_CANING_BASE))
#define THRD_SCHED_BASE		(THRD_CANING_BASE + 1)
#define THRD_SCHED_RUN		(0 << (THRD_SCHED_BASE))
#define THRD_SCHED_FINISHED (1 << (THRD_SCHED_BASE))
#define THRD_SCHED_MASK		(1 << (THRD_SCHED_BASE))

#define LOG2NTHRD	6
#define NTHRD		(1<<LOG2NTHRD)
#define THRDX(thrdid)	((thrdid) & (NTHRD - 1))

#define STACKTOP(idx)	(USTACKBASE + (1 + (idx)) * STACK_SIZE)
#define id2thrd(id, env)	(KADDR((u_int)((env)->env_thrd_phy[(id) >> 4] + ((id) & 0xF))))

struct Thrd {
	struct Trapframe thrd_tf;
	u_int thrd_id;
	u_int thrd_status;
	u_int thrd_join_id;
	u_int thrd_pri;
	u_int thrd_stacktop;
	u_int thrd_pgfault_handler;
	u_int thrd_xstacktop;
	u_int thrd_wait;
	void *thrd_retval;
	struct Env *thrd_env;
	LIST_ENTRY(Thrd) thrd_sched_link;
	u_char nop[_SIZE_THRD_-sizeof(struct Trapframe)-4-4-4-4-4-4-4-4-4-4-8];
};

LIST_HEAD(Thrd_list, Thrd);
extern struct Thrd *curthrd;
extern struct Thrd_list thrd_sched_list[2];

void thrd_init(void);
int thrd_alloc(struct Thrd **new, struct Env *e);
void thrd_free(struct Thrd *t);
void thrd_create_priority(int priority);
void thrd_destroy(struct Thrd *t);

int kthrdid2thrd(u_int thrdid, struct Thrd **pthrd);
void thrd_run(struct Thrd *t);

typedef u_int pthread_t;

#define PTHREAD_CANCELED 233

#endif /*thread.h*/
