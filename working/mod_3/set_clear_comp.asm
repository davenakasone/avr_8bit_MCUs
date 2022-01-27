; setting, clearing, and comparing

; set bits in a reg
SBR r16, 3      ; sets [0] and [1]
SBR r17, $F0    ; sets 4 MSB in r17

; clear bits in a reg
CBR r16, $F0    ; clear upper nibble of r16
CBR r18, 1      ; clear [0] in r18

; comparing
CP r4, r19    ; compare r4 and r19
BRNE noteq    ; branch ?

; compare with carry
CP r2, r0     ; compares low byte
CP r3, r1     ; compares high byte
BRNE noteq    ; branch if not equal

; compare with immidiate 
CPI r19, 3    ; compare r19 with 3
BRNE error    ; branch if not equal


; END
