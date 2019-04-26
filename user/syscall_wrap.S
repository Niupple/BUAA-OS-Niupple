#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>


LEAF(msyscall)
    // TODO: execute a `syscall` instruction and return from msyscall
	addiu	sp, sp, -4
	sw		ra, (sp)
	syscall
	nop
	lw		ra, (sp)
	addiu	sp, sp, 4
	jr ra
	nop

END(msyscall)

