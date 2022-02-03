; copy 55 into memory 140, 144

LDI r19, 0x5          ; counter
LDI r16, 0x55         ; r16 = 0x55
LDI YL, LOW($140)     ; r28 = 0x40   == LDI YL, LOW(0x140)
LDI YH, HIGH($140)    ; r29 = 0x1    == LDI YH, HIGH(0x140)
lab1:
    ST Y, r16     ; [0x0140] = r16
    DEC r19       ; decrement counter
    BRNE lab1     ; loop until counter is 0

; another way
LDI r19, 0x5          ; r19 = 5, counter
LDI r16, 0x55         ; r16 = 0x55
LDI YL, LOW($140)     ; YL = 0x40
LDI YH, HIGH($140)    ; YH = 0x01
lab1:
    ST Y+, r16    ; Y = r16
    DEC r19       ; decrement counter
    BRNE lab1
    