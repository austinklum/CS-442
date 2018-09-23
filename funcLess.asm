			.text
			.globl			__start
__start:
_cnsts:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			addiu			$sp, 			$sp, 			-12
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			addiu			$fp, 			$sp, 			8
			addiu			$s7, 			$sp, 			-4
			jal			_msg
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			addiu			$sp, 			$sp, 			12
			addiu			$sp, 			$sp, 			-12
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			addiu			$fp, 			$sp, 			8
			addiu			$s7, 			$sp, 			-4
			jal			_ret5
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			addiu			$sp, 			$sp, 			12
			sw			$v0, 			_i
L2:
			lw			$t0, 			_i
			li			$t1, 			10
			bgt			$t0, 			$t1, 			L1
			lw			$t0, 			_i
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			li			$t0, 			10
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_i
			addi			$t0, 			$t0, 			1
			sw			$t0, 			_i
			b			L2
L1:
			addiu			$sp, 			$sp, 			-20
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			sw			$t0, 			12($sp)
			sw			$t1, 			16($sp)
			addiu			$fp, 			$sp, 			16
			addiu			$s7, 			$sp, 			-4
			jal			_msg
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			lw			$t0, 			12($sp)
			lw			$t1, 			16($sp)
			addiu			$sp, 			$sp, 			20
			li			$t0, 			10
			sw			$t0, 			_i
L4:
			lw			$t0, 			_i
			li			$t1, 			0
			ble			$t0, 			$t1, 			L3
			lw			$t0, 			_i
			addi			$t0, 			$t0, 			-1
			sw			$t0, 			_i
			lw			$t0, 			_i
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			li			$t0, 			10
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_i
			addi			$t0, 			$t0, 			-1
			sw			$t0, 			_i
			b			L4
L3:
			addiu			$sp, 			$sp, 			-20
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			sw			$t0, 			12($sp)
			sw			$t1, 			16($sp)
			addiu			$fp, 			$sp, 			16
			addiu			$s7, 			$sp, 			-4
			jal			_msg
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			lw			$t0, 			12($sp)
			lw			$t1, 			16($sp)
			addiu			$sp, 			$sp, 			20
			jr			$ra
_msg:
			li			$v0, 			4
			la			$a0, 			L5
			syscall
			jr			$ra
_ret5:
			li			$v0, 			4
			la			$a0, 			L6
			syscall
			li			$t0, 			5
			move			$v0, 			$t0
			jr			$ra
			jr			$ra
			.data
_i:			.word			0
__iobuf:			.space			4
L5:			.asciiz			"\nvoid function was called!\n\n"
L6:			.asciiz			"ret5 was called!\n"
