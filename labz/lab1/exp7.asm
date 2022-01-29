# gcc assembler
# https://www.onlinegdb.com/online_gcc_assembler#
# put the break-point at "hang_here" label

.data
.text
.global main
main:
	xor	%rax, %rax
	xor	%rbx, %rbx
	xor	%rcx, %rcx
	add $5, %rax
	
loop1:
    add $3, %rbx
    jnz loop2

loop2:
    add $1, %rcx
    dec %rbx
    jnz loop2
    dec %rax
    jnz loop1
    jmp hang_here

hang_here:
    jmp hang_here

#END
