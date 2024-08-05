        lw      0       1       plus1   load reg2 with 1 (numeric address)
        lw      0       2       four    load reg1 with 4 (numeric address)
start	add		0		3		3		i = 0
loop	sw		3		0		arr		arr[i] = i	
        beq     2       3       3       goto end of program when reg3==reg2 (i==4)
		add     1       3       3       increment reg3
        beq     0       0       loop    go back to the beginning of the loop
        noop
done    halt                            end of program
four    .fill   4
plus1   .fill   1
arr		.fill	0
		.fill	0
		.fill	0
		.fill	0
		.fill	0
stAddr  .fill   start                   will contain the address of start (2)
