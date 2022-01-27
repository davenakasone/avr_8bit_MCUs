; [interupt{I}, temporary{T}, half-carry{H}, sign{S}, overflow{V}, negative{N}, zero{Z}, carry{C}]
; SREG is 8-bits, all flags


; status register "SREG" example with adding
LDI r16, 0x38    ; r16 = 0x38
LDI r17, 0x2F    ; r17 = 0x2F
ADD r16, r17     ; r16 = r16 + r17
; C = 0, because no carry beyond D7 bit
; H = 1, becuase D3 to D4 carry
; Z = 0, because result has value other than 0 after addition

; status register "SREG" example with subtraction
LDI r20, 0x52    ; r20 = 0x52
LDI r21, 0x73    ; r21 = 0x73
SUB r20, r21     ; r20 = r20 - r21
; C = 1, because r21 is bigger than r20
; H = 1, because there is a borrow D3 to D4
; Z = 0, becuase result has value other than 0 after subtraction

