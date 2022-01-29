; the common directives

; .EQU <name> = <value>
.EQU COUNT = 0x25
LDI r21, COUNT        ; r21 = 0x25
LDI r22, COUNT + 3    ; r22 = 0x28

; .SET <name> = <value>
.SET COUNT = 0x25
LDI r21, COUNT        ; r21 = 0x25
LDI r21, COUNT + 3    ; r22 = 0x28
.SET COUNT = 0x19
LDI r21, COUNT        ; r21 = 0x19

; .INCLUE "<file_name.extension>"
.INCLUDE "m328pbdef.inc"
; want so device gets configured

; .ORG <address>
.ORG 0
LDI r16, 0x25
.ORG 0x7
LDI r17, 0x34
LDI r18, 0x31
; where to map...tries to place instruction in program memory
; use NOP if it does not work  