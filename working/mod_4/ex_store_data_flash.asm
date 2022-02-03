LPM r20, Z    ; load program memory

LDI r30, 0x40
LDI r31, 0
LPM r18, Z   ; reads from low byte, location 0x40

; an example
.org $0000 ; start the ROM at [0]
LDI r20, 0xFF ; r20 = 0xFF
OUT DDRB, r20    ; makes PB an output
LDI ZL, LOW(MYDATA<<1)    ; ZL = 0, using the look-up table
LDI ZH, HIGH(MYDATA<<1)    ; ZH = 