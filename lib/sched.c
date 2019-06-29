#include <env.h>
#include <thread.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion statrting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
/*
void sched_yield_old(void)
{
	static int nowat = 0;
	static int counter = 0;
	struct Env *e = curenv;
	struct Thrd *t = curthrd;
	if(e) {
		//printf("yielding of %d\n", curenv->env_id);
		if(e->env_status == ENV_RUNNABLE && ++counter < e->env_pri) {
			env_run(e);
			return;
		}
		LIST_REMOVE(e, env_sched_link);
		LIST_INSERT_HEAD(&env_sched_list[nowat^1], e, env_sched_link);
	}
	while(1) {
		//printf("looking for runnable\n");
		while(!LIST_EMPTY(&env_sched_list[nowat]) &&
			LIST_FIRST(&env_sched_list[nowat])->env_status == ENV_NOT_RUNNABLE) {
			//printf("not runnable\n");
			e = LIST_FIRST(&env_sched_list[nowat]);
			LIST_REMOVE(e, env_sched_link);
			LIST_INSERT_HEAD(&env_sched_list[nowat^1], e, env_sched_link);
		}
		if (LIST_EMPTY(&env_sched_list[nowat])) {
			nowat ^= 1;
			continue;
		}
		counter = 0;
		e = LIST_FIRST(&env_sched_list[nowat]);
		assert(e->env_status == ENV_RUNNABLE);
		break;
	}
	env_run(e);
}
*/

void sched_yield(void)
{
	//printf("in sched_yield\n");
	static int nowat = 0;
	static int counter = 0;
	struct Thrd *t = curthrd;
	if(t) {
		//printf("yielding of %d\n", curenv->env_id);
		if((t->thrd_status & THRD_RUN_MASK) == THRD_RUNNABLE && ++counter < t->thrd_pri) {
			thrd_run(t);
			return;
		}
		LIST_REMOVE(t, thrd_sched_link);
		LIST_INSERT_HEAD(&thrd_sched_list[nowat^1], t, thrd_sched_link);
	}
	while(1) {
		//printf("looking for runnable\n");
		while(!LIST_EMPTY(&thrd_sched_list[nowat]) &&
			((LIST_FIRST(&thrd_sched_list[nowat])->thrd_status & THRD_RUN_MASK) == THRD_NOT_RUNNABLE)) {
			//printf("not runnable\n");
			t = LIST_FIRST(&thrd_sched_list[nowat]);
			LIST_REMOVE(t, thrd_sched_link);
			LIST_INSERT_HEAD(&thrd_sched_list[nowat^1], t, thrd_sched_link);
		}
		if (LIST_EMPTY(&thrd_sched_list[nowat])) {
			nowat ^= 1;
			continue;
		}
		//printf("found a runnable, %d\n", nowat);
		counter = 0;
		t = LIST_FIRST(&thrd_sched_list[nowat]);
		//printf("checking %x\n", t);
		assert((t->thrd_status & THRD_RUN_MASK) == THRD_RUNNABLE);
		break;
	}
	thrd_run(t);
}
