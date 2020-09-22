;This program counts from 10 to 0
	.ORIG x3000   
	AND R0, R0, #0	
	ADD R0, R0, #1
	.END			;The pseudo-op, delimiting the source program
