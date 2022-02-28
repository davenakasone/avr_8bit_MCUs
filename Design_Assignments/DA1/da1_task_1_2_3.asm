; CPE 301, David Nakasone
; DA_1, task:: 1, 2, 3
;	store a 16-bit number (0x1234) into SRAM location [0x0402]
;	store a 16-bit number (0x5678) into SRAM location [0x0410]
;	add the 2 numbers and store in EEPROM starting location [0x0000]

.include "m328PBdef.inc"    ; ensure MCU definition 

.cseg
.org 0                      ; start instructions from flash[0]

.def rTemp = r19       ; r19 == rTemp, a helper register
.def rNum1_hi = r20    ; r20 == rNum1_hi, [15:8] > HIGH(number_1)
.def rNum1_lo = r21    ; r21 == rNum1_lo, [7:0] > LOW(number_1)
.def rNum2_hi = r22    ; r22 == rNum2_hi, [15:8] > HIGH(number_2)
.def rNum2_lo = r23    ; r23 == rNum2_lo, [7:0] > LOW(number_2)
.def rSum_hi = r24     ; r24 == rSum_hi [15:8] > HIGH(number_1 + number_2)
.def rSum_lo = r25     ; r25 == rSum_lo [7:0] > LOW(number_1 + number_2)

.equ number_1 = 0x1234     ; value of first number
.equ address_1 = 0x0402    ; SRAM address to store first number
.equ number_2 = 0x5678     ; value of second number
.equ address_2 = 0x0410    ; SRAM address to store second number

	; initialize SP, using r16
	LDI rTemp, HIGH(RAMEND)
	OUT SPH, rTemp
	LDI rTemp, LOW(RAMEND)
	OUT SPH, rTemp

	; prepare registers to hold sum
	LDI rSum_hi, 0    ; rSum_hi = 0
	LDI rSum_lo, 0    ; rSum_lo = 0

	; first number stored to SRAM
	LDI rNum1_hi, HIGH(number_1)    ; rNum1_hi = 0x12
	STS address_1, rNum1_hi         ; [0x0402] = 0x12
	LDI rNum1_lo, LOW(number_1)     ; rNum1_lo = 0x34
	STS address_1 + 1, rNum1_lo     ; [0x0403] = 0x34

	; second number stored to SRAM
	LDI rNum2_hi, HIGH(number_2)    ; rNum2_hi = 0x56
	STS address_2, rNum2_hi         ; [0x0410] = 0x56
	LDI rNum2_lo, LOW(number_2)     ; rNum2_lo = 0x78
	STS address_2 + 1, rNum2_lo     ; [0x0411] = 0x78

	; sum = number_1 + number_2
	ADD rSum_lo, rNum1_lo    ; rSum_lo = rSum_lo + rNum1_lo = 0 + 0x34 = 0x34
	ADD rSum_lo, rNum2_lo    ; rSum_lo = rSum_lo + rNum2_lo = 0x34 + 0x78 = 0xAC
	ADC rSum_hi, rNum1_hi    ; rSum_hi = rSum_hi + rNum1_hi + carry = 0 + 0x12 + 0 = 0x12
	ADD rSum_hi, rNum2_hi    ; rSum_hi = rSum_hi + rNum2_hi = 0x12 + 0x56 = 0x68

	; write the sum into EEPROM    
	LDI XL, 0                ; *XH = 0
	LDI XH, 0                ; *XL = 0
	MOV rTemp, rSum_hi       ; rTemp = rSum_hi
	CALL write_to_eeprom     ; write upper byte to EEPROM[0x0000]
	INC XL                   ; *X = 0x0001, increment to next EEPROM location to write
	MOV rTemp, rSum_lo       ; rTemp = rSum_lo
	CALL write_to_eeprom     ; write lower byte to EEPROM[0x0001]

program_complete:
	RJMP program_complete    ; loop to stay alive

write_to_eeprom:
		SBIC	EECR, EEPE         ; (ready to write EEPROM) ? skip next instruction, wait
		RJMP	write_to_eeprom    ; not ready to write EEPROM, try again		
		OUT		EEARH, XH		   ; initialize EEPROM (high) address
		OUT		EEARL ,XL		   ; initialize EEPROM (low) address	
		OUT		EEDR, rTemp        ; place data in EEPROM data register 
		SBI		EECR, EEMPE		   ; write data, master check	
		SBI		EECR ,EEPE		   ; write data, final check
		CALL read_from_eeprom      ; spin
		CBI EECR, EEPE             ; reset writting
		CBI EECR, EERE             ; reset reading
		RET

read_from_eeprom:
		SBIC EECR, EEPE          ; check status of EEPROM
		RJMP read_from_eeprom    ; spins until EEPROM ready	
		OUT EEARH,XH		     ; pass address, high, using pointer
		OUT	EEARL,XL			 ; pass address, low, using pointer
		SBI	EECR,EERE			 ; enable reading
		IN	rTemp,EEDR			 ; verifies write operation
		RET


;;;;END
