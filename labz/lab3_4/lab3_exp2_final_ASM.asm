; lab3, exp2
; create a 2 second delay and toggle an LED
; using normal mode and checking TOV0
; the LED should be on PB0

.org 0
.include <m328pdef.inc>

.def rTemp = r16       ; helper
.def rLED = r17        ; LED state variable
.def rEOR = r18        ; used to toggle
.def rCounter = r19    ; manages outter loop as timer0 cycles

.equ LOOPS = 62        ; need 62 repititions of timer0, prescaled to 1024, to achieve 2-second delay 

	JMP main
main:
	LDI rTemp, (1 << 0)
	OUT DDRB, rTemp        ; PB0 configured as output
	LDI rLED, 0      
	LDI rEOR, 1
	LDI rCounter, LOOPS    ; initialize for outter loop

again:
	LDI rTemp, 0
	OUT TCNT0, rTemp     ; reset timer0
	LDI rTemp, 0x05
	OUT TCCR0B, rTemp    ; prescale timer0 for 1024, counting begins

inner_loop:
	IN rTemp, TIFR0           ; get interrupt flag register
	SBRS rTemp, TOV0          ; (TOV0 == 1) ? skip next instruction : let timer cycle
	RJMP inner_loop           ; no overflow, let timer0 run
	LDI rTemp, 0
	OUT TCCR0B, rTemp         ; stop timer0
	LDI rTemp, (1 << TOV0)
	OUT TIFR0, rTemp          ; reset TOV0

outter_loop:
	DEC rCounter
	BRNE again             ; (counter != 0) ? perform more cycles : toggle LED
	EOR rLED, rEOR         ; toggle state
	OUT PORTB, rLED        ; write to toggle LED
	LDI rCounter, LOOPS    ; reset counter
	RJMP again             ; continue making 2-second delays

;;;;~~~~END>  main.asm
