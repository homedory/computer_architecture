        lw      0       1       three	load reg1 with 3 (symbolic address)
        lw      1       2       9       load reg2 with -1 (numeric address)
		lw		0		3		stAddr	load reg3 with 3 (symoblic address)
start   add     1       2       1       decrement reg1
		jalr    4		4				jump to pc + 1
        beq     0       1       1       loop terminates when reg1==0
		jalr	3		4				go back to the beginning of the loop
		sw		0		3		r3val	store reg3 into memory (reg3==3)
		nor		0		3		3		nor reg3
		sw		0		3		r3val	store reg3 into memory (reg3==-4)
done    halt                            end of program
three   .fill   3
neg1    .fill   -1
r3val	.fill	0
stAddr  .fill   start                   will contain the address of start (2)
