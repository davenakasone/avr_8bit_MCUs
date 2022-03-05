; lab5, exp2b
; timer1 in CTC
; blinks LED every second
; using output compare1A interrupt 
; set compare value; f_out = f_cpu / (2 * pre * comp)

.org 0
	JMP main    ; reset point
.org 0x16
	JMP t1_compA_isr    ; service timer1 overflow interrupt

;.include <m328pbdef.inc>
.include <m328pdef.inc>

;.equ CTC_hi = 0x1E ; for 328pb
;.equ CTC_lo = 0x84 ; for 328pb
.equ CTC_hi = 0xAA; 0x0F ; for 328p
.equ CTC_lo = 0xCC; 0x41 ; for 238p
.equ MY_LED = 5 ; LED for output on PORTB

.def rTemp = r16
.def rEOR = r17
.def rLED = r18

main:
	; initialize SP
	LDI rTemp, HIGH(RAMEND)
	OUT SPH, rTemp
	LDI rTemp, LOW(RAMEND)
	OUT SPL, rTemp

	SBI DDRB, MY_LED           ; configure LED output
	LDI rEOR, (1 << MY_LED)    ; prepare for toggling LED
	LDI rLED, 0
	OUT PORTB, rLED            ; start LED off 
	LDI rTemp, (1 << OCIE1A)   
	STS TIMSK1, rTemp          ; enable compare1A interrupt
	SEI                        ; enable interrupts 

my_delay:
	LDI rTemp, 0
	STS TCNT1H, rTemp     ; clear higher counter
	STS TCNT1L, rTemp     ; clear lower counter
	LDI rTemp, CTC_hi
	STS OCR1AH, rTemp    ; load the higher compare value
	LDI rTemp, CTC_lo     
	STS OCR1AL, rTemp    ; load the lower compare value
	LDI rTemp, (1 << WGM12) | (1 << CS12) | (1 << CS10)
	STS TCCR1B, rTemp     ; timer1 prescaled to 1024, CTC mode, count begins
hang:
	IN rTemp, TIFR1       ; get the flag value
	SBRS rTemp, OCF1A     ; if the compare1A flag is set, then skip the next instruction                                             
	RJMP hang             ; program stays here, isr works
	LDI rTemp, 0
	STS TCCR1B, rTemp     ; stop time
	RJMP my_delay         ; this location should not be reached, but resets in case


t1_compA_isr:          
	IN rLED, PORTB     ; check state of LED
	EOR rLED, rEOR     ; prepare toggle
	OUT PORTB, rLED    ; toggles the LED
	RETI               ; the OCF1A flag is automatically cleared 


program_complete:
	RJMP program_complete


;;;;;;;;~~~~~~~~END>  main.asm
