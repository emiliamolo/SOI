	.file	"ej7.c"
	.text
	.p2align 4
	.globl	molinete
	.type	molinete, @function
molinete:
.LFB36:
	.cfi_startproc
	endbr64
	addl	$100000, visitas(%rip)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE36:
	.size	molinete, .-molinete
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Visitantes: %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB37:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	leaq	molinete(%rip), %r12
	xorl	%ecx, %ecx
	xorl	%esi, %esi
	movq	%r12, %rdx
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	leaq	8(%rsp), %rdi
	call	pthread_create@PLT
	xorl	%ecx, %ecx
	movq	%r12, %rdx
	xorl	%esi, %esi
	leaq	16(%rsp), %rdi
	call	pthread_create@PLT
	movq	8(%rsp), %rdi
	xorl	%esi, %esi
	call	pthread_join@PLT
	movq	16(%rsp), %rdi
	xorl	%esi, %esi
	call	pthread_join@PLT
	movl	visitas(%rip), %edx
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	call	__printf_chk@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L6
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE37:
	.size	main, .-main
	.globl	visitas
	.bss
	.align 4
	.type	visitas, @object
	.size	visitas, 4
visitas:
	.zero	4
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
