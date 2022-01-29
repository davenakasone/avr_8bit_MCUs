; be able to do this comforatibly 

; LDI "load immidiate"
LDI r16, 53      ; r16 = 53
LDI r19, 0x27    ; r19 = 0x27

; addition
ADD r17, r30    ; r17 = r17 + r30
LDI r16, 19     ; r16 = 19
LDI r20, 95     ; r20 = 95
ADD r16, r20    ; r16 = 19 + 95 ...

; subtraction
SUB r25, r9    ; r25 = r25 - r9

; increment
INC r25    ; r25 = r25 + 1

; decrement
DEC r23    ; r23 = r23 - 1

; copy data of one memory location to another
LDS r20, 0x80    ; r20 = [0x80]           "load direct from data space"
STS 0x81, r20    ; [0x81] = r20 = [0x80]  "store direct to data space"

; store a value to memory
LDI r20, 55      ; r20 = 55
STS 0x80, r20    ; [0x80] = r20 = 55

; register to register
LDS r20, 3    ; "r20 = r3"  ....3 is address of r3

; "load an I/O location to a register"  get value of I/O reg to GP reg
IN r1, 0x3f      ; r1 = SREG   ...SREG has address 0x3f
IN r17, 0x3e     ; r17 = SPH   ...SPH has address of 0x3e
IN r20, PINC     ; r20 = PINC
IN r21, PIND     ; r21 = PIND
ADD r20, r21     ; r20 = r20 + r21
STS 0x90, r20    ; [0x90] = r20

; "store register to I/O"
OUT 0x3f, r12    ; SREG = r12
OUT SPH, r15     ; 0x3e = r15   ...ok to use names or addresses of registers
