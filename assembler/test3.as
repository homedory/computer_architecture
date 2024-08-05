        lw      0       1       three   load reg1 with 3 (symbolic address)
        lw      0       2    	neg1	load reg2 with -1 (symbolic address)
start   add     1       2       1       decrement reg1
		lw 		0		3		m2Addr	load address o mul2 func		
		add		0		1		4		pass reg4 as a parameter of mul2 func
		jalr	3		5				jump to mul2
        beq     0       1       5       goto end of program when reg1==0
        beq     0       0       start   go back to the beginning of the loop
mul2	add		4		4		4		multiply 2
		sw		0		4		result	store result into memory
		jalr	5		3				return to main func
        noop
done    halt                            end of program
three   .fill   3
neg1    .fill   -1
result	.fill	0
m2Addr	.fill	mul2
stAddr  .fill   start                   will contain the address of start (2)
