#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>

void identify(int idx) {
	envs[idx].env_id = mkenvid(envs+idx);
	envs[idx].env_parent_id = 0;
	envs[idx].env_status = ENV_FREE;
}

void make_son(int x, int y) {
	envs[x].env_parent_id = envs[y].env_id;
}

void check(int x) {
	printf("%d :", x);
	if(envs[x].env_status == ENV_NOT_RUNNABLE) {
		printf("killed\n");
	} else {
		printf("alive\n");
	}
}

void check_check_same_root(int x, int y) {
	printf("csr(%d, %d) %d\n", x, y, check_same_root(envs[x].env_id, envs[y].env_id));
}

void mips_init()
{
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();

	mips_vm_init();
	page_init();

	env_init();
	printf("env_init end\n");
	env_check();
	printf("env_check end\n");

	identify(0);
	identify(1);
	identify(2);
	identify(3);
	identify(4);
	identify(5);
	identify(6);

	make_son(0, 1);
	make_son(1, 2);
	make_son(2, 3);
	make_son(4, 2);
	make_son(5, 3);
	
	printf("%x, %x\n", envs[3].env_parent_id, envs[6].env_parent_id);

	check_check_same_root(0, 1);
	check_check_same_root(1, 0);
	check_check_same_root(0, 4);
	check_check_same_root(3, 5);
	check_check_same_root(3, 6);
	envs[3].env_status = ENV_NOT_RUNNABLE;
	check_check_same_root(0, 3);
	check_check_same_root(0, 5);
	
	envs[3].env_status = ENV_RUNNABLE;

	kill_all(envs[0].env_id);
	int i;
	for(i = 0; i < 6; ++i) {
		check(i);
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
