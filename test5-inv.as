	lw	0	2	five	load reg1 with 5 (symbolic address)
	lw	1	2	32768	load reg2 with -1 (numeric address)
start	add	1	2	1	decrement reg1
	beqq	0	1	2	goto end of program when reg1==0
	beq	0	0	start	go back to the beginning of the loop
self	noop
done	halt				end of program
self	lw	0	3	self	this is to test comment
five	.fill	5
neg1	.fill	-1
stAddr	.fill	start			will contain the address of start (2)