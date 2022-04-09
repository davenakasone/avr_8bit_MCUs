00000000 <__vectors>:
   0:	0c 94 5a 00 	jmp	0xb4	; 0xb4 <__ctors_end>
   4:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
   8:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
   c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  10:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  14:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  18:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  1c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  20:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  24:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  28:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  2c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  30:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  34:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  38:	0c 94 73 00 	jmp	0xe6	; 0xe6 <__vector_14>
  3c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  40:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  44:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  48:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  4c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  50:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  54:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  58:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  5c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  60:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  64:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  68:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  6c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  70:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  74:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  78:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  7c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  80:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  84:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  88:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  8c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  90:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  94:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  98:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  9c:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  a0:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  a4:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  a8:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  ac:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
  b0:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>

000000b4 <__ctors_end>:
  b4:	11 24       	eor	r1, r1
  b6:	1f be       	out	0x3f, r1	; 63
  b8:	cf ef       	ldi	r28, 0xFF	; 255
  ba:	d8 e0       	ldi	r29, 0x08	; 8
  bc:	de bf       	out	0x3e, r29	; 62
  be:	cd bf       	out	0x3d, r28	; 61
  c0:	0e 94 66 00 	call	0xcc	; 0xcc <main>
  c4:	0c 94 7e 00 	jmp	0xfc	; 0xfc <_exit>

000000c8 <__bad_interrupt>:
  c8:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

000000cc <main>:
#include <util/delay.h>


int main(void)
{
	DDRD = (1 << PWM_PIN);    // output, OC0A
  cc:	80 e4       	ldi	r24, 0x40	; 64
  ce:	8a b9       	out	0x0a, r24	; 10
	TCNT0 = 0;          // clear coutner
  d0:	16 bc       	out	0x26, r1	; 38
	
	// toggle OC0A, ctc mode 
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
  d2:	82 e4       	ldi	r24, 0x42	; 66
  d4:	84 bd       	out	0x24, r24	; 36
	TIMSK0 = (1 << OCIE0A); // for reset
  d6:	82 e0       	ldi	r24, 0x02	; 2
  d8:	80 93 6e 00 	sts	0x006E, r24	; 0x80006e <__TEXT_REGION_LENGTH__+0x7f806e>
	sei();
  dc:	78 94       	sei
	
	#ifdef PART_A
		OCR0A = 1;
		TCCR0B = (1 << CS00); // no prescale
	#else
		OCR0A = 49;
  de:	91 e3       	ldi	r25, 0x31	; 49
  e0:	97 bd       	out	0x27, r25	; 39
		TCCR0B = (1 << CS01); // prescale to 8
  e2:	85 bd       	out	0x25, r24	; 37
  e4:	ff cf       	rjmp	.-2      	; 0xe4 <main+0x18>

000000e6 <__vector_14>:

////~~~~


ISR (TIMER0_COMPA_vect)
{
  e6:	1f 92       	push	r1
  e8:	0f 92       	push	r0
  ea:	0f b6       	in	r0, 0x3f	; 63
  ec:	0f 92       	push	r0
  ee:	11 24       	eor	r1, r1
	TCNT0 = 0;
  f0:	16 bc       	out	0x26, r1	; 38
}
  f2:	0f 90       	pop	r0
  f4:	0f be       	out	0x3f, r0	; 63
  f6:	0f 90       	pop	r0
  f8:	1f 90       	pop	r1
  fa:	18 95       	reti

000000fc <_exit>:
  fc:	f8 94       	cli

000000fe <__stop_program>:
  fe:	ff cf       	rjmp	.-2      	; 0xfe <__stop_program>