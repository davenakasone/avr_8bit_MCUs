; lab2, exp3
; on sequence: 2 leds are on and rotate right
; off sequence: 1 led is on and rotates left
.include <m328Pdef.inc>

.def rTemp = r16
.def rMode = r17    ; holds push-button value
.def rCounter = r18 ; maintains sequence in [7:0] range
.equ mask_on = $3   ; button is pushed
.edu mask_of = $1   ; button is not pushed

; initialize SP
	LDI rTemp, HIGH(RAMEND)
	OUT SPH, rTemp
	LDI rTemp, LOW(RAMEND)
	OUT SPH, rTemp 

; configure
	LDI rCounter 0x3 ; placed in middle
	LDI rTemp, 0xFF ; all LEDs are output
	OUT DDRB, rTemp

check_button:
	IN rMode, PINC ; checks if button pushed
	CPI rMode, $1  ; if button pushed
	BREQ seq_on    ; then goto "seq_on"
	CPI rMode, $0  ; if button not pushed
	BREQ seq_off   ; then goto "seq_off"
	start_delay:
	; wait some for switch bounce
	LDI rTemp, $FF
delay_loop:
	DEC rTemp
	BRNE delay_loop
	
	OUT PORTB, rMode  ; write the sequence
	RJMP check_button ; and repeat main loop



seq_on:
	CPI rCounter, 7
	BREQ clear_hi
reset0:
	LDI rMode, (rCounter>>mask_on)
	ROR rMode
	RET

clear_hi:
	LDI rCounter, 0
	RJMP reset0

seq_off:
	CPI rCounter, 0
	BREQ clear_low
reset7:
	LDI rMode, (mask_off<<rCounter)
	ROL rMode
	RET

clear_low:
	LDI rCounter, 7
	RJMP reset7
	
