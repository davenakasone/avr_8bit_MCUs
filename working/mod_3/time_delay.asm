; using machine cycle for delay

LDI r17, 20    ; r17 = 20

lab1:
    LDI r16, 50    ; r16 = 50

lab2:
    NOP
    NOP
    DEC r16      ; r16 = r16 - 1
    BRNE lab2    ; (sreg.Z == 0) ? goto lab2 : continue
    DEC r17      ; r17 = r17 - 1
    BRNE lab1    ; (sreg.Z == 0) ? goto lab1 : continue

; END
