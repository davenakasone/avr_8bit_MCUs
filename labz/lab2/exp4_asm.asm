; lab2, exp4
.include <m328Pdef.inc>

.def rTemp = r16
.def counter = r17
.def rToggle = r18
.def rDIP = r19
.equ stepz = $FF
.equ mask = $3
.equ LED_CONFIG = 0b11110000

.equ case00 =   0b11110000
.equ case01_a = 0b00010000
.equ case01_b = 0b00100000
.equ case10 =   0
.equ case11 =   0b11000000

; initialize SP
	LDI rTemp, HIGH(RAMEND)
	OUT SPH, rTemp
	LDI rTemp, LOW(RAMEND)
	OUT SPH, rTemp 

; configure
	LDI counter, stepz
	LDI rTemp, LED_CONFIG
	LDI rToggle, 0
	OUT DDRB, rTemp


; get the input and take action
get_input:
	IN rDIP, PINB
	ANDI rDIP, mask
		CPI rDIP, $0
		BREQ apply_00
		CPI rDIP, $1
		BREQ apply_01
		CPI rDIP, $2
		BREQ apply_10
		CPI rDIP, $3
		BREQ apply_11
	RJMP get_input

apply_00:
	LDI rTemp, case00
	OUT PORTB, rTemp
	RJMP get_input

apply_01:
	CPI rToggle, 0
	BREQ t_0_1
	CPI rToggle, 1
	BREQ t_1_0
t_0_1:
	LDI rToggle, 1
	LDI rTemp, case01_a
	RJMP start_delay
t_1_0:
	LDI rToggle, 0
	LDI rTemp, case01_b
start_delay:
	OUT PORTB, rTemp
	LDI rTemp, $FF
delay_loop:
	DEC rTemp
	BRNE delay_loop
	RJMP get_input

apply_10:
	LDI rTemp, case10
	OUT PORTB, rTemp
	RJMP get_input

apply_11:
	LDI rTemp, case11
	OUT PORTB, rTemp
	RJMP get_input
	
	



