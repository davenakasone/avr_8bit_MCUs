; a division example

.DEF num = r20
.DEF denominator = r21
.DEF quotient = r22

LDI num, 95
LDI denominator, 10

lab1:
    INC quotient
    SUB num, denominator
    BRCC lab1
    DEC quotient
    add num, denominator

here:
    JMP here