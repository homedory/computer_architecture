        lw      0       1       ten    load reg1 with 10 (symbolic address)
        lw      0       2       two     load reg2 with 2 (numeric address)
start 	add		0		0		3		i = 0
loop	lw		0		6		f_addr	load address of add10 func	
		add		0		3		4		pass i to add10
		jalr	6		7				jump to add10		
        beq     3       1       5       goto end of program when reg3==reg1 (i==10)
		add     2       3       3       increment 2 reg1 (i+=2)
        beq     0       0       loop    go back to the beginning of the loop
add10	add		1		4		4		calc i + 10	
		jalr	7		6				go back to loop
        noop
done    halt                            end of program
ten     .fill   10
two    	.fill   2
f_addr	.fill	add10
stAddr  .fill   start                   will contain the address of start (2)
