#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>

void mips_init()
{
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();

	mips_vm_init();
	page_init();

	env_init();
	env_check();
	
	int i;
	envs[0].env_status = ENV_RUNNABLE;
	envs[0].env_pri = 0;
	envs[1].env_status = ENV_RUNNABLE;
	envs[1].env_pri = 1;
	envs[2].env_status = ENV_RUNNABLE;
	envs[2].env_pri = 3;
	envs[3].env_status = ENV_RUNNABLE;
	envs[3].env_pri = 15;
	envs[4].env_status = ENV_RUNNABLE;
	envs[4].env_pri = 10;
	envs[1023].env_status = ENV_RUNNABLE;
	envs[1023].env_pri = 0;
	init_envid();
	for(i = 0; i < NENV; ++i) {
		if(envs[i].env_status == ENV_RUNNABLE) {
			output_env_info(envs[i].env_id);
		}
	}

	/*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
	 * code_b.c*/
	/*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
	 * interesting, have fun please*/
	ENV_CREATE_PRIORITY(user_A, 2);
	ENV_CREATE_PRIORITY(user_B, 1);

	//trap_init();
	//kclock_init();
	panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of mips_init() reached!");
}

void bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	max = dst + len;
	// copy machine words while possible
	while (dst + 3 < max)
	{
		*(int *)dst = *(int *)src;
		dst+=4;
		src+=4;
	}
	// finish remaining 0-3 bytes
	while (dst < max)
	{
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
}

void bzero(void *b, size_t len)
{
	void *max;

	max = b + len;

	//printf("init.c:\tzero from %x to %x\n",(int)b,(int)max);

	// zero machine words while possible

	while (b + 3 < max)
	{
		*(int *)b = 0;
		b+=4;
	}

	// finish remaining 0-3 bytes
	while (b < max)
	{
		*(char *)b++ = 0;
	}		

}
