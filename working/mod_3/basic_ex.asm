; some basic examples from mod3 summary

; add contents of location 0x90 and 0x95, store in location 0x313
LDS r20, 0x90     ; r20 = [0x90]
LDS r21, 0x95     ; r21 = [0x95]
ADD r20, r21      ; r20 = r20 + r21
STS 0x313, r20    ; [0x313] = r20

; copy r2 contents into r20  "2" is address of r2
LDS r20, 2

; add I/O regs and store in SRAM
IN r20, PINC     ; r20 = PINC
IN r21, PIND     ; r21 = PIND
ADD r20, r21     ; r20 = r20 + r21
STS 0x90, R20    ; [0x90] = r20

; store 0x53 into SPH reg ... SPH lives at address 0x5E
LDI r20, 0x53    ; r20 = [0x53]
STS 0x5E, R20    ; SPH = r20

