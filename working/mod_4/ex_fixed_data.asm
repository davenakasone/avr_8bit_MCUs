; store fixed data into memory
DATA1: .db 28              ; decimal value, 0x1C
DATA2: .db 0b00110101      ; binary value, 0x35
DATA3: .db 0x39            ; hex value
DATA4: .db 'y'             ; single ASCII char
DATA5: .db "Hello C", 0    ; ASCII string

; store fixed data to flash memory
LDI r30, 0x40
LDI r31, 0
LPM r18, Z    ; r18 = ZL "load program memory"