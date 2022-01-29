;LSR shifts bits left to right
; "logical shift right"

LDI r20, 0x26    ; r20 = 0010_0110
LSR r20          ; r20 = 0001_0011  carry = 0  
LSR r20          ; r20 = 0000_1001  carry = 1
LSR r20          ; r20 = 0000_0100  carry = 1

; 0 moved to MSB, LSB goes to carry
; effectively divides by 2, carry flag is the remainder