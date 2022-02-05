.org 0
    ldi r16, 0xff    ; load register with 0xff
    out DDRB,r16     ; set PORTB as an output for LED
                 ; no need to set DDRC = 0x00
                 ; by default PORTs are set as input
    out PORTC,r16    ; activate the pull-up resistor for PORTC

loop:
    in r16, PINC      ; read the input status from PINC
    andi r16, 0x01    ; masking
    cpi r16,0x01      ; compare if PINC == 0x01, to check the button press
    BREQ LED_ON       ; if button is pressed, branch to label LED_ON
    ldi r16, 0x00     ; turn off the led, PORTB = 0x00
    out PORTB, r16 
    rjmp loop

LED_ON:           ; turn on the led , PORTB = 0xFF
    ldi r16, 0xFF
    out PORTB, r16
    rjmp loop
