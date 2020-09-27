;This program counts from 10 to 0
	.ORIG x3000   
	AND R0, R0, #0	
	ADD R0, R0, #1
	AND R1, R1, #0
	ADD R1, R1, #1
	AND R2, R2, #10
	XOR R0, R0, R1
	LEA R0, LABEL
	LDW R1, R0, #2
	LDB R1, R0, #3
	STB R1, R0, #0
	JSRR R0
	AND R0, R0, #0
LABEL ADD R0, R0, #4 ; 0x3018, 0x3018 0x3019
	LSHF R0, R0, #2  ; 0x301A, 0x301A 0x301B
	RSHFA R0, R0, #4 ; 0x301C
	
	.END		;The pseudo-op, delimiting the source program
; 0: 0x3018 = 0011 0000 0001 1000
; 0: 0x6030 = 0110 0000 0011 0000 


; 0: 0xc060 = 1111 1100 0000 0110
; 0: 0xfc06 = 1111 1100 0000 0110

