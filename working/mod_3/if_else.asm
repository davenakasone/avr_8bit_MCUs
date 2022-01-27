; if...else example
;
;   r17 = 5
;   if (r20 > r21)
;       r22++
;   else
;       r22--
;   r17++

LDI r17, 5      ; r17 = 5
sub r21, r20    ; r21 = r21 - r20
BRCS if_label   ; (sreg.C == 1) ? goto if_label : continue
DEC r22         ; r22 = r22 - 1
JMP next        ; goto next

if_label:
    INC r22    ; r22 = r22 + 1

next:
    INC r17;    r17 = r17 + 1

; END
