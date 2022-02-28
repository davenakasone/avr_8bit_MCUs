; lab3, exp4
; create a 12 second delay and toggle an LED
; using normal mode and checking TOV0, call 2second delay * 6 = 12 seconds
; the LED should be on PB0

.org 0
.include <m328pdef.inc>

.def rTemp = r16       ; helper register
.def rLED = r17        ; state of LED
.def rEOR = r18        ; used to toggle PB0
.def rCounter = r19    ; outter loop, achieves 2 second delay
.def rSteps = r20      ; calls outter loop 6 times

.equ LOOPS = 70    ; achieves 2 second delay on timer0 with prescale = 1024
.equ STEPS = 6     ; 6*2 = 12 sec

	JMP main
main:
	LDI rTemp, (1 << 0)
	OUT DDRB, rTemp         ; PB0 configured as output
	LDI rLED, 0
	LDI rEOR, 1
	LDI rCounter, LOOPS     ; initialize 2-second time steps
	LDI rSteps, STEPS

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
	BRNE again             ; (counter != 0) ? perform more cycles : increase 2-second step
	LDI rCounter, LOOPS    ; reset counter
	DEC rSteps
	BRNE again             ; (steps != 0) ? run the inner and outter loop again : 12 seconds achieved
	LDI rSteps, STEPS      ; reset steps
	EOR rLED, rEOR         ; toggle state
	OUT PORTB, rLED        ; write to toggle LED
	RJMP again             ; continue {on/off} 12 seconds

;;;;~~~~END>  main.asm
