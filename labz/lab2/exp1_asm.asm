.org 0
    SBI DDRB, 4       ; set PB4 as an output
    LDI R17, 16       ; value is 16 to light up bit 4
    OUT PORTB, R17    ; sends value of R17 to corresponding bit
    