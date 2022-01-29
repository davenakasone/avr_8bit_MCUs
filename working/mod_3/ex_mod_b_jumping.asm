; some examples, mod3b

; basic branch
SUB r20, r21, Z    ; Z set if r20 == r21
BRNE next          ; (r20 == r21) ? increment : goto next
INC r22            ; r22 = r22 + 1
next:

; basic branch
SUB r26, r24, Z    ; Z will be set if r26 == r24  , C/B set if r26 > r24
BRCS lab1          ; if carry cleared, goto lab1
INC r22
lab1:

; basic branch, reverse logic
sub r24, r26    ; Z set if r26 == r24  ,  C/B set if r24 > r26
BRCS lab2       ; if carry cleared, goto lab2
INC r22         ; r22 = r22 + 1
lab2:

; exb4, if else
LDI r17, 5
SUB r21, r20
BRCS lab_if
dec r22
JMP continue
lab_if:
    inc r22
continue:
    inc r17

; exb, looping
.ORG 0
LDI r16, 9    ; i = 9
for_loop:
    DEC r16
    BRNE for_loop    ; looking for i = 0

; exb, 1+3+5+...+27
LDI r20, 0
LDI r16, 1
LDI r19, 27
while_loop:
    add r20, r16
    LDI r17, 2
    ADD r16, r17
    SUB r19, r16
    BRCC while_loop



