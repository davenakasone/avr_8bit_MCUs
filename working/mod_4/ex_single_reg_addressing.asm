; immidiate addressing, only 4 bits of read address
INC r19
DEC r23    ;  r23 = r23 - 1

; immidiate addressning, 8-bit immidiate, 4-bit read
LDI r19, 25
SUBI r23, 5
ANDI r21, 0x15
; also the 2-cycle "to word"

; 2 reg immidiate address (each is 5-bit)
ADD r26, r23
LDI r20, r10

; direct addressing
LDS r19, 0x313    ;  r19 = [0x0313]
STS 0x95, r19     ; [0x0095] = r19

; I/O addressing, the 64 regs
OUT 0x70, r16    ; [0x70] = r16
IN r19, 0x90     ; r19 = [0x90]

; indirect with displacement
STD Z+5, r20     ; store r20 in location Z+5
LDD r20, Z+8     ; load from Z+8 into r20

