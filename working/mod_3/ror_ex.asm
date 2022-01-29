; ROR moves bits left to right
; "rotate right through carry"
; carry is moved to LSB, LSB moved to carry

CLC              ; carry is 0
LDI r20, 0x26    ; r20 = 0010_0110  
ROR r20          ; r20 = 0001_0011  carry = 0
ROR r20          ; r20 = 0000_1001  carry = 1
ROR r20          ; r20 = 1000_0100  carry = 1

; carry flag enters MSB, LSB exits to carry flag

