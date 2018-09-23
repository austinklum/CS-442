			.text
			.globl			__start
__start:
_cnsts:
			li			$t0, 			3
			sw			$t0, 			_j
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$t0, 			7
			sw			$t0, 			_i
			addiu			$sp, 			$sp, 			-16
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			sw			$t0, 			12($sp)
			addiu			$fp, 			$sp, 			12
			addiu			$s7, 			$sp, 			-4
			jal			_printJ
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			lw			$t0, 			12($sp)
			addiu			$sp, 			$sp, 			16
			addiu			$sp, 			$sp, 			-20
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			sw			$t0, 			12($sp)
			sw			$t1, 			16($sp)
			addiu			$fp, 			$sp, 			16
			addiu			$s7, 			$sp, 			-4
			jal			_printJ
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			lw			$t0, 			12($sp)
			lw			$t1, 			16($sp)
			addiu			$sp, 			$sp, 			20
			addiu			$sp, 			$sp, 			-20
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			sw			$t0, 			12($sp)
			sw			$t1, 			16($sp)
			addiu			$fp, 			$sp, 			16
			addiu			$s7, 			$sp, 			-4
			jal			_printJ
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			lw			$t0, 			12($sp)
			lw			$t1, 			16($sp)
			addiu			$sp, 			$sp, 			20
			addiu			$sp, 			$sp, 			-24
			sw			$ra, 			0($sp)
			sw			$fp, 			4($sp)
			sw			$s7, 			8($sp)
			sw			$t0, 			12($sp)
			sw			$t1, 			16($sp)
			sw			$t2, 			20($sp)
			addiu			$fp, 			$sp, 			20
			addiu			$s7, 			$sp, 			-4
			jal			_printJ
			lw			$ra, 			0($sp)
			lw			$fp, 			4($sp)
			lw			$s7, 			8($sp)
			lw			$t0, 			12($sp)
			lw			$t1, 			16($sp)
			lw			$t2, 			20($sp)
			addiu			$sp, 			$sp, 			24
			lw			$t3, 			_j
			li			$t4, 			5
			add			$t5, 			$t3, 			$t4
			sw			$t5, 			_i
			lw			$t3, 			_i
			li			$v0, 			1
			move			$a0, 			$t3
			syscall
			li			$v0, 			4
			la			$a0, 			L1
			syscall
			jr			$ra
_printJ:
			li			$v0, 			4
			la			$a0, 			L2
			syscall
			lw			$t4, 			_j
			li			$v0, 			1
			move			$a0, 			$t4
			syscall
			li			$t4, 			10
			li			$v0, 			11
			move			$a0, 			$t4
			syscall
			jr			$ra
_ret6:
			li			$t3, 			6
			move			$v0, 			$t3
			jr			$ra
			jr			$ra
			.data
_i:			.word			0
_j:			.word			0
__iobuf:			.space			4
L1:			.asciiz			" = j + 5\n"
L2:			.asciiz			"j = "
