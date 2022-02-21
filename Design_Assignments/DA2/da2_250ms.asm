; cpe301, s22, da2, David Nakasone
; f_cpu = 16MHz, each clock period is T_cpu 0.00625 us
; inner loop runs from 0:LOOP_INNER
; middle loop runs from 0:LOOP_MIDDLE
; outer loop runs from 0:LOOP_OUTER, making a delay of 0.25 seconds
; reach any time desired with another loop that tracks calls to "delay025"
; tunning to micro-second range is possible
; no clocks allowed

.org 0
.include <m328pbdef.inc>

.def rTemp = r16          ; general purpose helper register
.def rInner = r17         ; counter for inner loop
.def rInnerTune = r18     ; tunner for inner loop
.def rMiddle = r19        ; counter for middle loop
.def rMiddleTune = r20    ; tunner for middle loop
.def rOuter = r21         ; counter for outer loop
.def rOuterTune = r22     ; tunner for outerr loop
.def rLED = r23           ; maintains state of LED / payload

.equ LOOP_INNER = 128     ; lowest loop level, iterations to execute
.equ TUNE_INNER = 128     ; tuning adjustment applied to inner loop
.equ LOOP_MIDDLE = 128    ; middle loop level, iterations to execute
.equ TUNE_MIDDLE = 128    ; tunning adjustment applied to outer loop
.equ LOOP_OUTER = 34      ; top loop level, iterations to achieve 0.25 sec delay
.equ TUNE_OUTER = 160     ; tunning adjustment applied to outter loop
.equ SPINZ = 255          ; extra NOPs

	JMP main
main:
    ; initialize SP
	LDI rTemp, LOW(RAMEND)
	OUT SPL, rTemp
	LDI rTemp, HIGH(RAMEND)
	OUT SPH, rTemp

	SBI DDRB, DDRB5    ; portB.5 as output


main_loop:
	RCALL delay025            ; wait 250 ms
	LDI rLED, 0
	OUT PORTB, rLED           ; LED off 
	RCALL delay025            ; wait 250 ms
	LDI rLED, (1 << DDRB5)    
	OUT PORTB, rLED           ; LED on
	RJMP main_loop
	

delay025:
	LDI rInner, LOOP_INNER          ; initialize inner loop
	LDI rInnerTune, TUNE_INNER      ; initialize tuning parameter for inner loop
	LDI rMiddle, LOOP_MIDDLE        ; initialize middle loop
	LDI rMiddleTune, TUNE_MIDDLE    ; initialize tuning parameter for middle loop
	LDI rOuter, LOOP_OUTER          ; initialize outer loop
	LDI rOuterTune, TUNE_OUTER      ; initialize tuning parameter for outer loop
	LDI rTemp, SPINZ                ; initialize single NOPs
outer_loop:
	RCALL outer_loop_tune    ; apply NOPs specified by TUNE_OUTER
	RCALL middle_loop        ; iterate exterior loop C through A and B
	DEC rOuter
	BRNE outer_loop          ; (LOOP_OUTER repititions complete) ? sub-routine complete : goto outter_loop
	RCALL spin_isolated      ; execute single NOPs
	RET                      ; returning out of this sub-routine
outer_loop_tune:
	NOP                           ; kill a cycle
	DEC rOuterTune 
	BRNE outer_loop_tune          ; (TUNE_OUTER time cycles executed) ? spins complete : keep spinning
	LDI rOuterTune, TUNE_OUTER    ; reset
	RET                           ; returns to outter_loop after spins are complete
middle_loop:
	RCALL middle_loop_tune      ; apply NOPs specified by TUNE_MIDDLE
	RCALL inner_loop            ; complete one iteration of interior loop
	DEC rMiddle
	BRNE middle_loop            ; (LOOP_MIDDLE repetitions occured) ? break : goto middle_loop
	LDI rMiddle, LOOP_MIDDLE    ; reset
	RET                         ; returning to loopC
middle_loop_tune:
	NOP                             ; kill a cycle
	DEC rMiddleTune 
	BRNE middle_loop_tune           ; (TUNE_MIDDLE time cycles executed) ? spins complete : keep spinning
	LDI rMiddleTune, TUNE_MIDDLE    ; reset
	RET                             ; returns to middle_loop after spins are complete
inner_loop:
	DEC rInner
	BRNE inner_loop           ; (LOOP_INNER repititions occured) ? break : goto inner_loop
	LDI rInner, LOOP_INNER    ; reset
	RET                       ; returning to loopB
inner_loop_tune:
	NOP                           ; kill a cycle
	DEC rInnerTune 
	BRNE inner_loop_tune          ; (TUNE_INNER time cycles executed) ? spins complete : keep spinning
	LDI rInnerTune, TUNE_INNER    ; reset
	RET                           ; returns to inner_loop after spins are complete
spin_isolated:
	DEC rTemp                  
	BRNE spin_isolated            ; (SPINZ single cycles) ? task complete : NOP more
	RET


program_complete:
	RJMP program_complete    ; catch


;;;;~~~~END>  main.asm
