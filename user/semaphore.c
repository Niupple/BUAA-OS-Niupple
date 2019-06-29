#include <mmu.h>
#include <env.h>
#include <thread.h>
#include <error.h>
#include <unistd.h>
#include <semaphore.h>
#include "lib.h"

int
sem_init(sem_t *sem, int unused, unsigned value)
{
	if (sem->status != SEMA_FREE || value > SEMA_MAXVAL) {
		return -E_INVAL;
	}
	sem->status = SEMA_USING;
	sem->count = value;
	return 0;
}

int
sem_getvalue(sem_t *sem, int *ret)
{
	if (sem->status != SEMA_USING) {
		return -E_INVAL;
	}
	*ret = sem->count;
	return 0;
}

int 
sem_trywait(sem_t *sem)
{
	return syscall_sem_trywait(sem);
}

int
sem_wait(sem_t *sem)
{
	int r;
	do {
		r = syscall_sem_trywait(sem);
		if (r == -E_AGAIN) {
			//writef("not yet\n");
			syscall_yield();
		}
	} while(r == -E_AGAIN);
	return r;
	//return syscall_sem_wait(sem);
}

int
sem_post(sem_t *sem)
{
	return syscall_sem_post(sem);
}

int
sem_destroy(sem_t *sem)
{
	if (sem->status != SEMA_USING) {
		return -E_INVAL;
	}
	sem->status = SEMA_FREE;
	return 0;
}

