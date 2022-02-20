; lab 3, exp 3
; toggle LED at different rates by DIP
; CTC, using TOV0 on timer1
; put LED on PB0 and switches on [PC1, PC0]

.org 0
.include <m328pdef.inc>

.def rTemp = r16       ; holder register
.def rEOR = r17        ; used for toggling 
.def rLED = r18        ; state of LED
.def rDip = r19        ; state of 2 DIP switches
.def rCounter = r20    ; outter loop

.equ LOOP_HI = 0xAF    ; CTC control, HIGH
.equ LOOP_LO = 0xBF    ; CTC control, LOW
.equ REPZ = 0x1F       ; control the outter loop

	JMP main
main:
	LDI rCounter, REPZ    ; initialize the counter
	LDI rTemp, 0
	OUT DDRC, rTemp       ; PORTC as input
	LDI rTemp, 0x11
	OUT PORTC, rTemp      ; PC.0, PC.1 pull-ups activated
	LDI rTemp, 0
	LDI rEOR, 0x01        ; used to XOR
	SBI DDRB, 0           ; set PB.0 as output
	OUT PORTB, rTemp      ; initialize, LED off
	LDI rLED, 0           ; initialize, LED state is off

select_delay:
	LDI rTemp, LOOP_HI      ; prepare CTC reset, high
	STS OCR1AH, rTemp       ; CTC hi is set
	LDI rTemp, LOOP_LO      ; prepare CTC reset, low
	STS OCR1AL, rTemp       ; CTC low is set
	LDI rTemp, 0
	STS TCNT1H, rTemp       ; reset timer1 counter, high
	STS TCNT1L, rTemp       ; reset timer1 counter, low
	LDI rTemp, 0x02
	STS TCCR1A, rTemp       ; timer1 in CTC mode
    ; preparations complete, identify the delay to apply
	IN rDip, PINC      ; get DIP positions, PC[7:0]
	MOV rTemp, rDip    ; copy DIP positions
	ANDI rDip, 0x3     ; mask to verify [11]
	CPI rDip, 0x03     ; DIP == [11] ?
	BREQ set11         ; if yes, apply setting for [11]
	MOV rDIP, rTemp    ; if no, copy original DIP value
	ANDI rDip, 0x02    ; mask to verify [10]
	CPI rDip, 0x02     ; DIP == [10] ?
	BREQ set10         ; if yes, apply setting for [10]
	MOV rDIP, rTemp    ; if no, copy original DIP value
	ANDI rDip, 0x01    ; mask to verify [01]
	CPI rDip, 0x01     ; DIP == [01] ?
	BREQ set01         ; if yes, apply setting for [01]
	RJMP set00         ; if no, apply setting for [00]

counter_delay:
	IN rTemp, TIFR1            ; check timer1 interrupt flags
	SBRS rTemp, OCF1A          ; (overflow occured) ? skip next instruction : keep counting
	RJMP counter_delay
	LDI rTemp, (1 << OCF1A)    
	OUT TIFR1, rTemp           ; clear the OCF1A flag
	LDI rTemp, 0
	STS TCCR1B, rTemp          ; stop timer 1 of 2
	STS TCCR1A, rTemp          ; stop timer 2 of 2
	DEC rCounter
	BRNE select_delay          ; (enough repititions) ? keep looping : toggle LED
	LDI rCounter, REPZ         ; reset counter
	EOR rLED, rEOR             ; prepare toggle
	OUT PORTB, rLED            ; write value to PB0 to toggle LED
	RJMP select_delay          ; looping back to check switches

set00:
	LDI rTemp, 0x05
	STS TCCR1B, rTemp     ; prescale 1024, timer1 begins
	RJMP counter_delay    ; starting cycle
set01:
	LDI rTemp, 0x04
	STS TCCR1B, rTemp     ; prescale 256, timer1 begins
	RJMP counter_delay    ; starting cycle
set10:
	LDI rTemp, 0x03
	STS TCCR1B, rTemp     ; prescale 64, timer1 begins
	RJMP counter_delay    ; starting cycle
set11:
	LDI rTemp, 0x02
	STS TCCR1B, rTemp     ; prescale 8, timer1 begins
	RJMP counter_delay    ; starting cycle

program_complete :
	RJMP program_complete    ; catch all, should not be reached

;;;;~~~~END
