/*
	Filename	: fibonacci.s
	Author		: Zachary Zuch
	Course		: CSCI 370
	Description	: Compute the fibonacci number for the nth number
				  and subtract the number by the number of zeros 
				  in the binary representation.
*/

/***************************************************************/

// DATA
		.data

prompt:
		.string "n ==> "

formatSpec:
		.string "%d"

output:
		.string "fib(|%d|) - zeros(%d) = %d - %d = %d\n"

/***************************************************************/

// CODE

		.text
		.align 	4
		.globl 	main

main:

		# Setup Code
		pushl 	%ebp
		movl 	%esp, %ebp
		pushl 	%ebx
		subl 	$12, %esp
		
		pushl	$prompt
		call	printf

		# -8(%ebp) == n
		leal	-8(%ebp), %eax
		pushl 	%eax
		pushl	$formatSpec
		call	scanf

		# -12(%ebp) == fib(n)
		pushl	-8(%ebp)
		call	fib
		movl	%eax, -12(%ebp)
		
		# -16(%ebp) == zeros(n)
		pushl	-8(%ebp)
		call	zeros
		movl	%eax, -16(%ebp)

		# %eax == fib(n) - zeros(n)
		movl 	-12(%ebp), %eax
		subl 	-16(%ebp), %eax

		pushl	%eax
		pushl	-16(%ebp)
		pushl	-12(%ebp)
		pushl	-8(%ebp)
		pushl	-8(%ebp)
		pushl	$output
		call	printf

		# Finish Code
		movl	$0, %eax
		movl	-4(%ebp), %ebx
		movl	%ebp, %esp
		popl	%ebp
		ret

/***************************************************************/

// FUNCTIONS

fib:

		# Setup Code
		pushl 	%ebp
		movl 	%esp, %ebp
		pushl 	%ebx
		
		# IF ( n < 0 )
		cmpl	$0, 8(%ebp)
		jge		.L1
		negl 	8(%ebp)

	.L1:
		# IF n <= 1
		cmpl 	$1, 8(%ebp)
		jg 		.L2
		# return 1
		movl	$1, %eax
		jmp		.D1

	.L2:
		# int i = 1
		movl 	$1, %eax

		# int temp = 1
		movl	$1, %ecx

	.L3:
		# while ( i <= n )
		cmpl 	8(%ebp), %eax
		jg 		.D1

		# temp2 = i
		movl	%eax, %edx

		# i += temp
		addl	%ecx, %eax

		# temp = temp2
		movl	%edx, %ecx

		jmp 	.L3

	.D1:
		# Finish Code
		movl	-4(%ebp), %ebx
		movl	%ebp, %esp
		popl	%ebp
		ret

/***************************************************************/

zeros:

		# Setup Code
		pushl 	%ebp
		movl 	%esp, %ebp
		pushl 	%ebx

		# int zeros = 0
		# int i = 0
		movl	$0, %eax
		movl 	$0, %ecx

	.L4:
		# IF ( i < 32 )
		cmpl 	$32, %ecx
		je  	.D2

		# n & ( 1 << i )
		movl	$1, %edx
		sall	%cl, %edx
		andl	8(%ebp), %edx

		# IF ( n & ( 1 << i ) ) == 0
		# zeros++
		cmpl 	$0, %edx
		jne 	.L5
		incl 	%eax

	.L5:
		# i++
		incl	%ecx
		jmp 	.L4

	.D2:
		# Finish Code
		movl	-4(%ebp), %ebx
		movl	%ebp, %esp
		popl	%ebp
		ret

/***************************************************************/
