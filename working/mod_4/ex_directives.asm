; the arithmetic and logical directives help code

LDI r20, 5+3    ; r20 = 8
LDI r30, 9-3    ; r30 = 6
LDI r25, 5*7    ; r25 = 35
LDI r19, 8/2    ; r19 = 4

LDI r20, 0x50 & 0x10    ; r20 = 0x10
LDI r25, 0x50 | 0x1     ; r25 = 0x51
LDI r23, 0x50 ^ 0x10    ; r23 = 0x40

LDI r16, 0x10 << 1    ; r16 = 0x20
LDI r16, 0x8 >> 2     ; r16 = 0x2

LDI r20, LOW(0x1234)     ; r20 = 0x34
LDI r21, HIGH(0x1234)    ; r21 = 0x12
LDI r20, LOW(-200)       ; r20 = $FF
LDI r20, HIGH(-200)      ; r20 = $38