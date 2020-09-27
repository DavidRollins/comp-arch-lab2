;This program counts from 10 to 0
	.ORIG x3000   
	AND R0, R0, #0	
	ADD R0, R0, #1
	AND R1, R1, #0
	ADD R1, R1, #1
	XOR R0, R0, R1
	LEA R0, LABEL
	LDW R1, R0, #2
	STW R1, R0, #0
	BRnz LABEL

LABEL ADD R0, R0, #4
	LSHF R0, R0, #2
	RSHFL R0, R0, #4
	
	.END		;The pseudo-op, delimiting the source program
