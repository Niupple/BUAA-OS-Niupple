#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>

NESTED(handle_ov, TF_SIZE, sp)	/*int is for interrupt*/
.set	noat

//1: j 1b
nop

//here I changed the order of CLI and SAVE_ALL
CLI
SAVE_ALL
.set	at
//print instruction
mfc0 t0, CP0_EPC
lw a0, (t0)
jal print_instr
nop

//get two regs out of the address
srl t0, a0, 20
andi t0, t0, 0x1f
srl t1, a0, 15
andi t1, t1, 0x1f
move a0, t0
move a1, t1
jal print_regs
nop
sw $0, TF_REG8(sp)
sw $0, TF_REG9(sp)
sw $0, TF_REG10(sp)
sw $0, TF_REG11(sp)
sw $0, TF_REG12(sp)
sw $0, TF_REG13(sp)
sw $0, TF_REG14(sp)
sw $0, TF_REG15(sp)

jal ov_check
nop

j ret_from_exception
nop

nop
END(handle_ov)
