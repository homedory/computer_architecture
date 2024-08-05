        lw      0       1       one    load reg1 with 1 (symbolic address)
		lw      0       2       bound    load reg4 with -2147483644 (symbolic address)
		lw		0		3		intmin  load reg3 intmin
start   add     3       1       3       increment reg3
        beq     3       2       2       goto end of program when reg1==0
        beq     0       0       start   go back to the beginning of the loop
        noop
done    halt                            end of program
intmin  .fill   -2147483648
bound   .fill   -2147483644
one     .fill   1
stAddr  .fill   start                   will contain the address of start (2)
