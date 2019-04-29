#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>

NESTED(handle_sys,TF_SIZE, sp)
    SAVE_ALL                            // Macro used to save trapframe
    CLI                                 // Clean Interrupt Mask
    nop
    .set at                             // Resume use of $at

    // TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.
	//mfc0	t0, CP0_CAUSE				// see if this syscall is in delay slot; I will just ignore this :)
	lw		t0, TF_EPC(sp)				// fetch EPF from trapframe
	addiu	t0, t0, 4					// add it by 4 to jump over the syscall instr.
	sw		t0, TF_EPC(sp)				// and then write it back

    // TODO: Copy the syscall number into $a0.
	lw		a0, TF_REG2(sp)
    
    addiu   a0, a0, -__SYSCALL_BASE     // a0 <- relative syscall number
    sll     t0, a0, 2                   // t0 <- relative syscall number times 4
    la      t1, sys_call_table          // t1 <- syscall table base
    addu    t1, t1, t0                  // t1 <- table entry of specific syscall
    lw      t2, 0(t1)                   // t2 <- function entry of specific syscall

    lw      t0, TF_REG29(sp)            // t0 <- user's stack pointer
	lw		t1, TF_REG5(sp)				// t1 <- number of arguments
	lw		a0, TF_REG2(sp)				// a0 <- syscall number
	lw		a1, TF_REG6(sp)				// a1 <- 1st arg
	lw		a2, TF_REG7(sp)				// a2 <- 2nd arg
	lw		a3, 16(t0)					// a3 <- 3rd arg
	addiu	t1, t1, 1					// t1 += 1 because #args takes a place
	sll		t3, t1, 2					// t3 <- stack space required
	subu	sp, sp, t3					// now sp is at the right place for further calling

	li		t1, 16						// t1 <- iterator of for loop
	arg_loop:
	slt		t4, t1, t3
	beqz	t4, arg_loop_end
	nop

	addu	t4, t0, t1					// t4 <- &arg[i] in user stack
	addiu	t4, t4, 4					// 1 more shift due to the #arg
	lw		t4, (t4)					// t4 <- arg[i]
	addu	t5, sp, t1					// t5 <- &arg[i] in new callee stack
	sw		t4, (t5)					// store arg to where it should be

	j arg_loop
	addiu	t1, t1, 4
	arg_loop_end:

	move	s0, t3						// temporarily store stack size, for popping

	/*
    lw      t3, 16(t0)                  // t3 <- the 5th argument of msyscall
    lw      t4, 20(t0)                  // t4 <- the 6th argument of msyscall
	lw		a0, TF_REG4(sp)				// 
	lw		a1, TF_REG5(sp)				// 
	lw		a2, TF_REG6(sp)				// 
	lw		a3, TF_REG7(sp)
    addiu	sp, sp, -24					// 6 word = 24 bytes
	sw		a0, 0(sp)
	sw		a1, 4(sp)
	sw		a2, 8(sp)
	sw		a3, 12(sp)
	sw		t3, 16(sp)					// 
	sw		t4, 20(sp)
	*/
    
    jalr    t2                          // Invoke sys_* function
    nop

	/*
	addiu sp, sp, 24
	*/
	addu	sp, sp, s0
    
    // TODO: Resume current kernel stack
    
    sw      v0, TF_REG2(sp)             // Store return value of function sys_* (in $v0) into trapframe

    j       ret_from_exception          // Return from exeception
    nop
END(handle_sys)

sys_call_table:                         // Syscall Table
.align 2
    .word sys_putchar
    .word sys_getenvid
    .word sys_yield
    .word sys_env_destroy
    .word sys_set_pgfault_handler
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_env_alloc
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv
    .word sys_cgetc
	.word sys_super_multi_parameter
