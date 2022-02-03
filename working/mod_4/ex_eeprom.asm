; reading EEPROM
; 1) wait for EEPE to be 0  ...if it is 1, it is busy
; 2) write address to EEAR
; 3) set EERE to 1
; 4) read data from EEDR
load_from_eeprom:
    SBIC EECR, EEPE
    RJMP load_from_eeprom
    OUT EEARH, XH
    OUT EEARL, XL
    SBI EECR, EERE
    IN r20, EEDR
    RET

; write EEPROM
; 1) wait until EEPE becomes 0
; 2) optional, write to EEAR
; 3) optional, write to EEDR
; 4) set EEMPE to 1
; 5) do this in <4 clock cycles of setting EEMPE to 1
store_in_eeprom:
    SBIC EECR, EEPE
    RJMP store_in_eeprom
    OUT EEARH, XH
    OUT EEARL, XL
    OUT EEDR, r20
    SBI EECR, EEMPE
    SBI EECR, EEPE
    RET
    