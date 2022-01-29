; functions need the stack space
; the stack grows down...

; pushing
; PUSH rR     [SP] = rR, SP = SP - 1

; popping
; POP rD    SP = SP + 1, Rd = [SP]

; calling a function, address of next instruction is saved
LDI r20, 15
LDI r21, 5
CALL my_fun
RJMP stop
stop:
my_fun:
    add r20, r21
    SUBI r20, 3
    RET

; is it best to use directive + macro for 16-bit number
.EQU MY_NUM = 0x2211    ; 16 bit const
LDI r20, LOW(MY_NUM)    ; r20 = 0x11
LDI r21, HIGH(MY_NUM)   ; r21 = 0x22

; or 
LDI r20, 0x11
LDI r21, 0x22ke
