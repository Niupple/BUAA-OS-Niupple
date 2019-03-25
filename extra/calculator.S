#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

#s0: driver
#s1: operator
.text
LEAF(calculator) /*LEAF is defined in asm.h and LEAF functions don't call other functions*/                       

li s0, 0x90000000 #address of driver

main_loop:
	addi sp, sp, -4
	sw ra, 0(sp)

	#get operator
	jal get_byte
	nop
	or s1, zero, v0
	li t0, 81	#Q
	bne s1, t0, quit_else
	nop
	jal quit
	nop
	quit_else:

	#then input two operands
	jal get_number
	nop
	or s2, zero, v0	#first operand

	jal get_number
	nop
	or s3, zero, v0	#second operand

	#then calculate
	add:
	addi t0, s1, -43
	bnez t0, sub
	nop

	add a0, s2, s3
	j end_calc

	sub:
	sub a0, s2, s3
	end_calc:
	jal get_byte
	nop
	jal output
	nop

	j main_loop
	nop

get_byte:
	input_loop:
	lb v0, 0(s0)	#byte inputed
	beqz v0, input_loop
	nop
	sb v0, 0(s0)
	jr ra
	nop

get_number:
	addi sp, sp, -16
	sw t0, 0(sp)
	sw t1, 4(sp)
	sw t2, 8(sp)
	sw ra, 12(sp)

	li t1, 0
	number_loop:
	jal get_byte
	nop
	li t0, 68	#D
	beq t0, v0, end_number_loop
	nop
	li t0, 10
	mult t1, t0
	mflo t0		#t0 = t1*10
	addi v0, v0, -48 #to_digit
	addu t1, t0, v0
	j number_loop
	nop
	end_number_loop:
	or v0, t1, zero

	lw t0, 0(sp)
	lw t1, 4(sp)
	lw t2, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	jr ra
	nop

output:
	addi sp, sp, -16
	sw t0, 0(sp)
	sw t1, 4(sp)
	sw t2, 8(sp)
	sw ra, 12(sp)


	output_loop:
	beqz a0, end_output
	nop
	li t0, 10
	div a0, t0
	mfhi t0
	addiu t0, 48
	sb t0, 0(s0)	#output
	mflo a0
	j output_loop
	nop
	end_output:
	li t0, 10
	sb t0, 0(s0)


	lw t0, 0(sp)
	lw t1, 4(sp)
	lw t2, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16

	jr ra
	nop

quit:
	sw s0, 0x10(s0)
	lw ra, 0(sp)
	addi sp, sp, 4

END(calculator)            /*the function defined in asm.h*/
