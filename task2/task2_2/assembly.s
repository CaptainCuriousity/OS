	.file	"withoutsyscall.c"
	.text
	.section	.rodata
.LC0:
	.string	"Hello world\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	// leaq	.LC0(%rip), %rax
	
	// movq $13, %rdx //length
	//movq $1, %rdi
	# write(1, message, 13)
        mov     $10, %rax                # system call 1 is write
        mov     $1, %rdi                # file handle 1 is stdout
        lea     .LC0(%rip), %rsi     # address of string to output
        mov     $13, %rdx               # number of bytes
        syscall
	// int $0x80 

        # exit(0)
        mov     $60, %rax               # system call 60 is exit
        xor     %rdi, %rdi              # return code 0
        syscall	
	// movq $1, %rax
	// movq $1, %rdi
	// movq $.LC0, %rsi
	// movq $13, %rdx 	
	// syscall

	// movq $60, %rax
	// xorq %rdi, %rdi
	// syscall
	// movq	%rax, %rdi
	// call	puts@PLT
	// movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
