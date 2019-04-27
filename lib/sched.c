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
	//printf("yielding of %x\n", curenv);
	static int nowat = 0;
	static int counter = 0;
	struct Env *e = curenv;
	if(e) {
		if(e->env_status == ENV_NOT_RUNNABLE || ++counter < e->env_pri) {
			env_run(e);
			return;
		}
		LIST_REMOVE(e, env_sched_link);
		LIST_INSERT_HEAD(&env_sched_list[nowat^1], e, env_sched_link);
	}
	while(1) {
		while(!LIST_EMPTY(&env_sched_list[nowat]) && LIST_FIRST(&env_sched_list[nowat])->env_status == ENV_NOT_RUNNABLE) {
			e = LIST_FIRST(&env_sched_list[nowat]);
			LIST_REMOVE(e, env_sched_link);
			LIST_INSERT_HEAD(&env_sched_list[nowat^1], e, env_sched_link);
		}
		if (LIST_EMPTY(&env_sched_list[nowat])) {
			nowat ^= 1;
		}
	}
	counter = 0;
	e = LIST_FIRST(&env_sched_list[nowat]);
	env_run(e);
}
