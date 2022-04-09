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
  38:	0c 94 64 00 	jmp	0xc8	; 0xc8 <__bad_interrupt>
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
  c4:	0c 94 88 00 	jmp	0x110	; 0x110 <_exit>

000000c8 <__bad_interrupt>:
  c8:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

000000cc <main>:
#include <util/delay.h>


int main(void)
{
	DDRB = (1 << PWM_A) | (1 << PWM_B);    // output, OC1A and OC1B
  cc:	86 e0       	ldi	r24, 0x06	; 6
  ce:	84 b9       	out	0x04, r24	; 4
	TCNT1 = 0;                             // clear coutner
  d0:	10 92 85 00 	sts	0x0085, r1	; 0x800085 <__TEXT_REGION_LENGTH__+0x7f8085>
  d4:	10 92 84 00 	sts	0x0084, r1	; 0x800084 <__TEXT_REGION_LENGTH__+0x7f8084>
	
	// non-inverting, OC1A and OC1B clear on compare match
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << COM1B0) | (1 << WGM11);
  d8:	e0 e8       	ldi	r30, 0x80	; 128
  da:	f0 e0       	ldi	r31, 0x00	; 0
  dc:	80 81       	ld	r24, Z
  de:	82 6b       	ori	r24, 0xB2	; 178
  e0:	80 83       	st	Z, r24
		OCR1A = 1000;
		OCR1B = 19000;
		// prescale 8, finish setting mode 14, fast PWM, 
		TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	#else
		ICR1 = 46874;
  e2:	8a e1       	ldi	r24, 0x1A	; 26
  e4:	97 eb       	ldi	r25, 0xB7	; 183
  e6:	90 93 87 00 	sts	0x0087, r25	; 0x800087 <__TEXT_REGION_LENGTH__+0x7f8087>
  ea:	80 93 86 00 	sts	0x0086, r24	; 0x800086 <__TEXT_REGION_LENGTH__+0x7f8086>
		OCR1A = 2343;
  ee:	87 e2       	ldi	r24, 0x27	; 39
  f0:	99 e0       	ldi	r25, 0x09	; 9
  f2:	90 93 89 00 	sts	0x0089, r25	; 0x800089 <__TEXT_REGION_LENGTH__+0x7f8089>
  f6:	80 93 88 00 	sts	0x0088, r24	; 0x800088 <__TEXT_REGION_LENGTH__+0x7f8088>
		OCR1B = 44530;
  fa:	82 ef       	ldi	r24, 0xF2	; 242
  fc:	9d ea       	ldi	r25, 0xAD	; 173
  fe:	90 93 8b 00 	sts	0x008B, r25	; 0x80008b <__TEXT_REGION_LENGTH__+0x7f808b>
 102:	80 93 8a 00 	sts	0x008A, r24	; 0x80008a <__TEXT_REGION_LENGTH__+0x7f808a>
		// prescale 1024, finish setting mode 14, fast PWM,
		TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
 106:	8d e1       	ldi	r24, 0x1D	; 29
 108:	80 93 81 00 	sts	0x0081, r24	; 0x800081 <__TEXT_REGION_LENGTH__+0x7f8081>
	#endif
	
	sei();
 10c:	78 94       	sei
 10e:	ff cf       	rjmp	.-2      	; 0x10e <main+0x42>

00000110 <_exit>:
 110:	f8 94       	cli

00000112 <__stop_program>:
 112:	ff cf       	rjmp	.-2      	; 0x112 <__stop_program>