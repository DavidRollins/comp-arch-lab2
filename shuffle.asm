.ORIG x3000
; r0 holds length of array 
; let r1 hold addrNum
; let r2 hold first word mask
; let r3 hold second word mask
LEA R1, addrNum
LEA R2, firstWord
LDW R2, R2, #0
LEA R3, secondWord
LDW R3, R3, #0
LSHF R1, R1, #1 ; <- double
LOOP ADD R1, R1, R0 
LDW R4, R1, #0 ; R4 <- x3050 + R0
AND R5, R4, R2 ; first word
AND R6, R4, R3 ; second word
LSHF R6, R6, #8
RSHFL R5, R5, #8
ADD R5, R5, R6
STW R5, R1, #0
ADD R0, R0, #-2
BRnzp LOOP
addrNum     .FILL x3050
firstWord   .FILL xFF00
secondWord  .FILL x00FF
            .END



