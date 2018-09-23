			.text
			.globl			__start
__start:
_cnsts:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$t0, 			1
			sw			$t0, 			_i
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
			li			$t0, 			10
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
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
			jr			$ra
			.data
_i:			.word			0
