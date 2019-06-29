#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <env.h>
#include <thread.h>

typedef struct {
	u_int count;
	u_int status;
} sem_t;

#define SEMA_FREE 0
#define SEMA_USING 1
#define SEMA_MAXVAL (0x80000000-1)

#define E_AGAIN 13

#endif	/*semaphore.h*/
