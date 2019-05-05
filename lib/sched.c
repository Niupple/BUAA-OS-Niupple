#include <env.h>
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
void sched_yield(void)
{
	static int nowat = 0;
	static int counter = 0;
	struct Env *e = curenv;
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
	//printf("running %d\n", e->env_id);

	/*
	//if(e->env_id == 4097) {
	Pte *ppt;
	pgdir_walk(e->env_pgdir, 0x4000c8, 0, &ppt);
	printf("id = %d, pgwalk = %x\n", e->env_id, *ppt);
	//}
	*/
	env_run(e);
}
