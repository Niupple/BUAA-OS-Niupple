#include <mmu.h>
#include <error.h>
#include <env.h>
#include <sched.h>
#include <thread.h>
#include <pmap.h>
#include <printf.h>

struct Thrd *thrds = THRDS;
struct Thrd *curthrd = NULL;	// the current Thread
struct Thrd_list thrd_sched_list[2];

extern Pde *boot_pgdir;
extern char *KERNEL_SP;

u_int mkthrdid(struct Thrd *t, u_int idx) {
	// TODO here minus is wrong
	struct Env *e = t->thrd_env;
	//printf("QaQ\n");

	return (++e->env_thrd_counter << (1 + LOG2NTHRD)) | idx;
}

int kthrdid2thrd(u_int thrdid, struct Thrd **pthrd) {
	struct Thrd *t;
	struct Env *e;
	u_int status;

	if(thrdid == 0) {
		*pthrd = curthrd;
		return 0;
	}

	printf("curenv = %x\n", curenv);
	t = id2thrd(THRDX(thrdid), curenv);
	printf("status of %d(%d) is %x\n", thrdid, t->thrd_id, t->thrd_status);
	status = (t->thrd_status & THRD_RUN_MASK);
	if (status == THRD_FREE || t->thrd_id != thrdid) {
		if(status == THRD_FREE) {
			printf("wrong status\n");
		} else {
			printf("%d vs. %d\n", t->thrd_id, thrdid);
		}
		*pthrd = NULL;
		return -E_BAD_THRD;
	}

	*pthrd = t;
	return 0;
}

	void
thrd_init(void) {
	int i;

	LIST_INIT(&thrd_sched_list[0]);
	LIST_INIT(&thrd_sched_list[1]);

	assert(LIST_EMPTY(&thrd_sched_list[0]));
	assert(LIST_EMPTY(&thrd_sched_list[1]));
}

	int
thrd_alloc(struct Thrd **new, struct Env *e) {
	int i, j;
	int r;
	struct Thrd *t;

	r = -1;
	for (j = 0; j < 2; ++j) {
		if (e->env_free_thrd[j]) {
			for (i = 0; i < 32; ++i) {
				if (e->env_free_thrd[j] & (1 << i)) {
					r = i + j*32;
					e->env_free_thrd[j] ^= (1 << i);
					break;
				}
			}
			if (r != -1) {
				break;
			}
		}
	}
	printf("found empty, %d, %d\n", i, j);
	if (r == -1) {
		return -E_NO_FREE_THRD;
	}
	t = id2thrd(r, e);
	//printf("haha, %x\n", t);

	t->thrd_env = e;
	t->thrd_id = mkthrdid(t, r);
	printf("id = %x\n", t->thrd_id);
	t->thrd_status = THRD_RUNNABLE;

	t->thrd_join_id = 0;
	t->thrd_wait = 0;
	t->thrd_retval = NULL;

	t->thrd_tf.cp0_status = 0x10001004;
	t->thrd_tf.pc = UTEXT+0xb0;
	t->thrd_tf.regs[29] = STACKTOP(THRDX(t->thrd_id));

	*new = t;
	return 0;
}

	void
thrd_free(struct Thrd *t) {
	printf("thrd free %x\n", t);
	t->thrd_status = THRD_FREE;
	if (curthrd == t) {
		curthrd = NULL;
	}
	LIST_REMOVE(t, thrd_sched_link);
	//printf("%d, %d\n", LIST_EMPTY(&thrd_sched_list[0]), LIST_EMPTY(&thrd_sched_list[1]));
}

	void
thrd_destroy(struct Thrd *t) {
	printf("in destroy of %x\n", t);
	thrd_free(t);
	if (curthrd == t) {
		curthrd = NULL;
		bcopy((void *)KERNEL_SP - sizeof(struct Trapframe),
				(void *)TIMESTACK - sizeof(struct Trapframe),
				sizeof(struct Trapframe));
		printf("I am killed\n");
		sched_yield();
	}
}

	void
thrd_run(struct Thrd *t) {
	if (curthrd) {
		bcopy((void *)TIMESTACK-sizeof(struct Trapframe),
				(void *)&(curthrd->thrd_tf), sizeof(struct Trapframe));
		curthrd->thrd_tf.pc = curthrd->thrd_tf.cp0_epc;
	}
	curthrd = t;
	curenv = t->thrd_env;
	//printf("now running %x\n", curenv);
	lcontext(t->thrd_env->env_pgdir);
	env_pop_tf(&(t->thrd_tf), GET_ENV_ASID(t->thrd_env->env_id));
}

	void
thrd_finish(struct Thrd *t) {
	printf("in thread_finish %d, %x\n", t->thrd_id, t);
	struct Thrd *waiter;
	int r;

	if ((t->thrd_status & THRD_JOIN_MASK) == THRD_DETACHED) {
		printf("destroy due to detached\n");
		thrd_destroy(t);
	}
	//printf("s before = %x\n", t->thrd_status);
	t->thrd_status = (t->thrd_status & ~THRD_SCHED_MASK) | THRD_SCHED_FINISHED;
	t->thrd_status = (t->thrd_status & ~THRD_RUN_MASK) | THRD_NOT_RUNNABLE;
	//printf("s after = %x\n", t->thrd_status);

	printf("hold up to wait for joining\n");
	sched_yield();
	//syscall_yield();
}
