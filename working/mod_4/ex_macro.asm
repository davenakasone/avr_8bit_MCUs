; use keywords, write instructions   initializing stack is common
.macro initStack
    LDI r16, HIGH(RAMEND)
    OUT SPH, r16
    LDI r16, LOW(RAMEND)
    OUT SPL, r16
.endmacro

; use a macro to get I/O reading, this macro has args
.macro loadIO
    LDI r20, @1
    OUT @0, r20
.endmacro

loadIO DDRB, 0xFF
loadIO PORTB, 0x55

; a macro to delay
.macro DELAY
    LDI @0, @1
    back:
        NOP
        NOP
        NOP
        DEC@0
        BRNE back
.endmacro

