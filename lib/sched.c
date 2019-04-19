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
		if(++counter < e->env_pri) {
			//printf("not yet\n");
			env_run(e);
			return;
		}
		LIST_REMOVE(e, env_sched_link);
		//printf("list removed\n");
		LIST_INSERT_HEAD(&env_sched_list[nowat^1], e, env_sched_link);
		//printf("list inserted\n");
	}
	while(LIST_EMPTY(&env_sched_list[nowat])) {
		//printf("switching sched list\n");
		nowat ^= 1;
	}
	counter = 0;
	e = LIST_FIRST(&env_sched_list[nowat]);
	//printf("now try to run %x\n", e);
	//check RUNNABLE?
	//timer_init();
	env_run(e);
}
