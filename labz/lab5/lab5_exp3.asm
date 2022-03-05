00000000 <__vectors>:
   0:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__ctors_end>
   4:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
   8:	0c 94 08 01 	jmp	0x210	; 0x210 <__vector_2>
   c:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  10:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  14:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  18:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  1c:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  20:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  24:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  28:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  2c:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  30:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  34:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  38:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  3c:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  40:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  44:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  48:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  4c:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  50:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  54:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  58:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  5c:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  60:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  64:	0c 94 5b 00 	jmp	0xb6	; 0xb6 <__bad_interrupt>
  68:	84 00       	.word	0x0084	; ????
  6a:	8d 00       	.word	0x008d	; ????
  6c:	96 00       	.word	0x0096	; ????
  6e:	9f 00       	.word	0x009f	; ????
  70:	a8 00       	.word	0x00a8	; ????
  72:	af 00       	.word	0x00af	; ????
  74:	b8 00       	.word	0x00b8	; ????
  76:	c1 00       	.word	0x00c1	; ????
  78:	ca 00       	.word	0x00ca	; ????
  7a:	d3 00       	.word	0x00d3	; ????

0000007c <__ctors_end>:
  7c:	11 24       	eor	r1, r1
  7e:	1f be       	out	0x3f, r1	; 63
  80:	cf ef       	ldi	r28, 0xFF	; 255
  82:	d8 e0       	ldi	r29, 0x08	; 8
  84:	de bf       	out	0x3e, r29	; 62
  86:	cd bf       	out	0x3d, r28	; 61

00000088 <__do_copy_data>:
  88:	11 e0       	ldi	r17, 0x01	; 1
  8a:	a0 e0       	ldi	r26, 0x00	; 0
  8c:	b1 e0       	ldi	r27, 0x01	; 1
  8e:	ee e6       	ldi	r30, 0x6E	; 110
  90:	f2 e0       	ldi	r31, 0x02	; 2
  92:	02 c0       	rjmp	.+4      	; 0x98 <__do_copy_data+0x10>
  94:	05 90       	lpm	r0, Z+
  96:	0d 92       	st	X+, r0
  98:	a0 30       	cpi	r26, 0x00	; 0
  9a:	b1 07       	cpc	r27, r17
  9c:	d9 f7       	brne	.-10     	; 0x94 <__do_copy_data+0xc>

0000009e <__do_clear_bss>:
  9e:	21 e0       	ldi	r18, 0x01	; 1
  a0:	a0 e0       	ldi	r26, 0x00	; 0
  a2:	b1 e0       	ldi	r27, 0x01	; 1
  a4:	01 c0       	rjmp	.+2      	; 0xa8 <.do_clear_bss_start>

000000a6 <.do_clear_bss_loop>:
  a6:	1d 92       	st	X+, r1

000000a8 <.do_clear_bss_start>:
  a8:	a1 30       	cpi	r26, 0x01	; 1
  aa:	b2 07       	cpc	r27, r18
  ac:	e1 f7       	brne	.-8      	; 0xa6 <.do_clear_bss_loop>
  ae:	0e 94 5d 00 	call	0xba	; 0xba <main>
  b2:	0c 94 35 01 	jmp	0x26a	; 0x26a <_exit>

000000b6 <__bad_interrupt>:
  b6:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

000000ba <main>:

int main
(
	void
)
{
  ba:	cf 93       	push	r28
  bc:	df 93       	push	r29
  be:	00 d0       	rcall	.+0      	; 0xc0 <main+0x6>
  c0:	00 d0       	rcall	.+0      	; 0xc2 <main+0x8>
  c2:	1f 92       	push	r1
  c4:	cd b7       	in	r28, 0x3d	; 61
  c6:	de b7       	in	r29, 0x3e	; 62
    DDRB = (1 << MY_LED) | (1 << INT_IND);      // configure PORTB as output for LEDs
  c8:	81 e2       	ldi	r24, 0x21	; 33
  ca:	84 b9       	out	0x04, r24	; 4
	PORTB = 0;                                  // start with LEDs off
  cc:	15 b8       	out	0x05, r1	; 5
	DDRD = 0;                                   // PORTD as input
  ce:	1a b8       	out	0x0a, r1	; 10
	PORTD = (1 << INT_PIN);                     // turn on pull-up resistor
  d0:	88 e0       	ldi	r24, 0x08	; 8
  d2:	8b b9       	out	0x0b, r24	; 11
	EIMSK = (1 << INT1);                        // enable INT1 external interrupt
  d4:	92 e0       	ldi	r25, 0x02	; 2
  d6:	9d bb       	out	0x1d, r25	; 29
	EICRA = (1 << ISC11);                       // make INT1 falling-edge triggered
  d8:	80 93 69 00 	sts	0x0069, r24	; 0x800069 <__TEXT_REGION_LENGTH__+0x7f8069>
	
	volatile uint16_t on_time = 0;     // find duty cycle D
  dc:	1a 82       	std	Y+2, r1	; 0x02
  de:	19 82       	std	Y+1, r1	; 0x01
	volatile uint16_t off_time = 0;    // find remainder of T, 1 - D
  e0:	1c 82       	std	Y+4, r1	; 0x04
  e2:	1b 82       	std	Y+3, r1	; 0x03
	volatile uint8_t pass = 0;         // stores global counter
  e4:	1d 82       	std	Y+5, r1	; 0x05
	sei();                             // enable interrupts
  e6:	78 94       	sei

    while (1) 
	{
		pass = int1_counter;
  e8:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <__DATA_REGION_ORIGIN__>
  ec:	8d 83       	std	Y+5, r24	; 0x05
		//
		switch(pass)
  ee:	ed 81       	ldd	r30, Y+5	; 0x05
  f0:	8e 2f       	mov	r24, r30
  f2:	90 e0       	ldi	r25, 0x00	; 0
  f4:	fc 01       	movw	r30, r24
  f6:	31 97       	sbiw	r30, 0x01	; 1
  f8:	ea 30       	cpi	r30, 0x0A	; 10
  fa:	f1 05       	cpc	r31, r1
  fc:	08 f0       	brcs	.+2      	; 0x100 <main+0x46>
  fe:	5c c0       	rjmp	.+184    	; 0x1b8 <main+0xfe>
 100:	ec 5c       	subi	r30, 0xCC	; 204
 102:	ff 4f       	sbci	r31, 0xFF	; 255
 104:	0c 94 2f 01 	jmp	0x25e	; 0x25e <__tablejump2__>
		{
			case 1 :
				on_time = 300;
 108:	8c e2       	ldi	r24, 0x2C	; 44
 10a:	91 e0       	ldi	r25, 0x01	; 1
 10c:	9a 83       	std	Y+2, r25	; 0x02
 10e:	89 83       	std	Y+1, r24	; 0x01
				off_time = 2700;
 110:	8c e8       	ldi	r24, 0x8C	; 140
 112:	9a e0       	ldi	r25, 0x0A	; 10
 114:	9c 83       	std	Y+4, r25	; 0x04
 116:	8b 83       	std	Y+3, r24	; 0x03
				break;
 118:	57 c0       	rjmp	.+174    	; 0x1c8 <main+0x10e>
			case 2 :
				on_time = 600;
 11a:	88 e5       	ldi	r24, 0x58	; 88
 11c:	92 e0       	ldi	r25, 0x02	; 2
 11e:	9a 83       	std	Y+2, r25	; 0x02
 120:	89 83       	std	Y+1, r24	; 0x01
				off_time = 2400;
 122:	80 e6       	ldi	r24, 0x60	; 96
 124:	99 e0       	ldi	r25, 0x09	; 9
 126:	9c 83       	std	Y+4, r25	; 0x04
 128:	8b 83       	std	Y+3, r24	; 0x03
				break;
 12a:	4e c0       	rjmp	.+156    	; 0x1c8 <main+0x10e>
			case 3 :
				on_time = 900;
 12c:	84 e8       	ldi	r24, 0x84	; 132
 12e:	93 e0       	ldi	r25, 0x03	; 3
 130:	9a 83       	std	Y+2, r25	; 0x02
 132:	89 83       	std	Y+1, r24	; 0x01
				off_time = 2100;
 134:	84 e3       	ldi	r24, 0x34	; 52
 136:	98 e0       	ldi	r25, 0x08	; 8
 138:	9c 83       	std	Y+4, r25	; 0x04
 13a:	8b 83       	std	Y+3, r24	; 0x03
				break;
 13c:	45 c0       	rjmp	.+138    	; 0x1c8 <main+0x10e>
			case 4 :
				on_time = 1200;
 13e:	80 eb       	ldi	r24, 0xB0	; 176
 140:	94 e0       	ldi	r25, 0x04	; 4
 142:	9a 83       	std	Y+2, r25	; 0x02
 144:	89 83       	std	Y+1, r24	; 0x01
				off_time = 1800;
 146:	88 e0       	ldi	r24, 0x08	; 8
 148:	97 e0       	ldi	r25, 0x07	; 7
 14a:	9c 83       	std	Y+4, r25	; 0x04
 14c:	8b 83       	std	Y+3, r24	; 0x03
				break;
 14e:	3c c0       	rjmp	.+120    	; 0x1c8 <main+0x10e>
			case 5 :
				on_time = 1500;
 150:	8c ed       	ldi	r24, 0xDC	; 220
 152:	95 e0       	ldi	r25, 0x05	; 5
 154:	9a 83       	std	Y+2, r25	; 0x02
 156:	89 83       	std	Y+1, r24	; 0x01
				off_time = 1500;
 158:	9c 83       	std	Y+4, r25	; 0x04
 15a:	8b 83       	std	Y+3, r24	; 0x03
				break;
 15c:	35 c0       	rjmp	.+106    	; 0x1c8 <main+0x10e>
			case 6 :
				on_time = 1800;
 15e:	88 e0       	ldi	r24, 0x08	; 8
 160:	97 e0       	ldi	r25, 0x07	; 7
 162:	9a 83       	std	Y+2, r25	; 0x02
 164:	89 83       	std	Y+1, r24	; 0x01
				off_time = 1200;
 166:	80 eb       	ldi	r24, 0xB0	; 176
 168:	94 e0       	ldi	r25, 0x04	; 4
 16a:	9c 83       	std	Y+4, r25	; 0x04
 16c:	8b 83       	std	Y+3, r24	; 0x03
				break;
 16e:	2c c0       	rjmp	.+88     	; 0x1c8 <main+0x10e>
			case 7 :
				on_time = 2100;
 170:	84 e3       	ldi	r24, 0x34	; 52
 172:	98 e0       	ldi	r25, 0x08	; 8
 174:	9a 83       	std	Y+2, r25	; 0x02
 176:	89 83       	std	Y+1, r24	; 0x01
				off_time = 900;
 178:	84 e8       	ldi	r24, 0x84	; 132
 17a:	93 e0       	ldi	r25, 0x03	; 3
 17c:	9c 83       	std	Y+4, r25	; 0x04
 17e:	8b 83       	std	Y+3, r24	; 0x03
				break;
 180:	23 c0       	rjmp	.+70     	; 0x1c8 <main+0x10e>
			case 8 :
				on_time = 2400;
 182:	80 e6       	ldi	r24, 0x60	; 96
 184:	99 e0       	ldi	r25, 0x09	; 9
 186:	9a 83       	std	Y+2, r25	; 0x02
 188:	89 83       	std	Y+1, r24	; 0x01
				off_time = 600;
 18a:	88 e5       	ldi	r24, 0x58	; 88
 18c:	92 e0       	ldi	r25, 0x02	; 2
 18e:	9c 83       	std	Y+4, r25	; 0x04
 190:	8b 83       	std	Y+3, r24	; 0x03
				break;
 192:	1a c0       	rjmp	.+52     	; 0x1c8 <main+0x10e>
			case 9 :
				on_time = 2700;
 194:	8c e8       	ldi	r24, 0x8C	; 140
 196:	9a e0       	ldi	r25, 0x0A	; 10
 198:	9a 83       	std	Y+2, r25	; 0x02
 19a:	89 83       	std	Y+1, r24	; 0x01
				off_time = 300;
 19c:	8c e2       	ldi	r24, 0x2C	; 44
 19e:	91 e0       	ldi	r25, 0x01	; 1
 1a0:	9c 83       	std	Y+4, r25	; 0x04
 1a2:	8b 83       	std	Y+3, r24	; 0x03
				break;
 1a4:	11 c0       	rjmp	.+34     	; 0x1c8 <main+0x10e>
			case 10 :
				on_time = 2900;
 1a6:	84 e5       	ldi	r24, 0x54	; 84
 1a8:	9b e0       	ldi	r25, 0x0B	; 11
 1aa:	9a 83       	std	Y+2, r25	; 0x02
 1ac:	89 83       	std	Y+1, r24	; 0x01
				off_time = 10;
 1ae:	8a e0       	ldi	r24, 0x0A	; 10
 1b0:	90 e0       	ldi	r25, 0x00	; 0
 1b2:	9c 83       	std	Y+4, r25	; 0x04
 1b4:	8b 83       	std	Y+3, r24	; 0x03
				break;
 1b6:	08 c0       	rjmp	.+16     	; 0x1c8 <main+0x10e>
			default :
				on_time = 10;
 1b8:	8a e0       	ldi	r24, 0x0A	; 10
 1ba:	90 e0       	ldi	r25, 0x00	; 0
 1bc:	9a 83       	std	Y+2, r25	; 0x02
 1be:	89 83       	std	Y+1, r24	; 0x01
				off_time = 2990;
 1c0:	8e ea       	ldi	r24, 0xAE	; 174
 1c2:	9b e0       	ldi	r25, 0x0B	; 11
 1c4:	9c 83       	std	Y+4, r25	; 0x04
 1c6:	8b 83       	std	Y+3, r24	; 0x03
		}
		//
		//on_time = (pass/10) * 3000;
		//off_time = 3000 - pass;
		PORTB = (1 << MY_LED);
 1c8:	80 e2       	ldi	r24, 0x20	; 32
 1ca:	85 b9       	out	0x05, r24	; 5
		while(on_time > 0)
 1cc:	0b c0       	rjmp	.+22     	; 0x1e4 <main+0x12a>
	#else
		//round up by default
		__ticks_dc = (uint32_t)(ceil(fabs(__tmp)));
	#endif

	__builtin_avr_delay_cycles(__ticks_dc);
 1ce:	8f ec       	ldi	r24, 0xCF	; 207
 1d0:	97 e0       	ldi	r25, 0x07	; 7
 1d2:	01 97       	sbiw	r24, 0x01	; 1
 1d4:	f1 f7       	brne	.-4      	; 0x1d2 <main+0x118>
 1d6:	00 c0       	rjmp	.+0      	; 0x1d8 <main+0x11e>
 1d8:	00 00       	nop
		{
			_delay_ms(1);
			on_time = on_time - 1;
 1da:	89 81       	ldd	r24, Y+1	; 0x01
 1dc:	9a 81       	ldd	r25, Y+2	; 0x02
 1de:	01 97       	sbiw	r24, 0x01	; 1
 1e0:	9a 83       	std	Y+2, r25	; 0x02
 1e2:	89 83       	std	Y+1, r24	; 0x01
		}
		//
		//on_time = (pass/10) * 3000;
		//off_time = 3000 - pass;
		PORTB = (1 << MY_LED);
		while(on_time > 0)
 1e4:	89 81       	ldd	r24, Y+1	; 0x01
 1e6:	9a 81       	ldd	r25, Y+2	; 0x02
 1e8:	89 2b       	or	r24, r25
 1ea:	89 f7       	brne	.-30     	; 0x1ce <main+0x114>
		{
			_delay_ms(1);
			on_time = on_time - 1;
		}
		PORTB = 0;
 1ec:	15 b8       	out	0x05, r1	; 5
		while(off_time > 0)
 1ee:	0b c0       	rjmp	.+22     	; 0x206 <main+0x14c>
 1f0:	8f ec       	ldi	r24, 0xCF	; 207
 1f2:	97 e0       	ldi	r25, 0x07	; 7
 1f4:	01 97       	sbiw	r24, 0x01	; 1
 1f6:	f1 f7       	brne	.-4      	; 0x1f4 <main+0x13a>
 1f8:	00 c0       	rjmp	.+0      	; 0x1fa <main+0x140>
 1fa:	00 00       	nop
		{
			_delay_ms(1);
			off_time = off_time - 1;
 1fc:	8b 81       	ldd	r24, Y+3	; 0x03
 1fe:	9c 81       	ldd	r25, Y+4	; 0x04
 200:	01 97       	sbiw	r24, 0x01	; 1
 202:	9c 83       	std	Y+4, r25	; 0x04
 204:	8b 83       	std	Y+3, r24	; 0x03
		{
			_delay_ms(1);
			on_time = on_time - 1;
		}
		PORTB = 0;
		while(off_time > 0)
 206:	8b 81       	ldd	r24, Y+3	; 0x03
 208:	9c 81       	ldd	r25, Y+4	; 0x04
 20a:	89 2b       	or	r24, r25
 20c:	89 f7       	brne	.-30     	; 0x1f0 <main+0x136>
 20e:	6c cf       	rjmp	.-296    	; 0xe8 <main+0x2e>

00000210 <__vector_2>:

////~~~~


ISR(INT1_vect)    
{
 210:	1f 92       	push	r1
 212:	0f 92       	push	r0
 214:	0f b6       	in	r0, 0x3f	; 63
 216:	0f 92       	push	r0
 218:	11 24       	eor	r1, r1
 21a:	8f 93       	push	r24
 21c:	9f 93       	push	r25
	PORTB |= (1 << INT_IND); 
 21e:	85 b1       	in	r24, 0x05	; 5
 220:	81 60       	ori	r24, 0x01	; 1
 222:	85 b9       	out	0x05, r24	; 5
 224:	8f e1       	ldi	r24, 0x1F	; 31
 226:	9e e4       	ldi	r25, 0x4E	; 78
 228:	01 97       	sbiw	r24, 0x01	; 1
 22a:	f1 f7       	brne	.-4      	; 0x228 <__vector_2+0x18>
 22c:	00 c0       	rjmp	.+0      	; 0x22e <__vector_2+0x1e>
 22e:	00 00       	nop
	_delay_ms(10);
	PORTB &= ~(1 << INT_IND);
 230:	85 b1       	in	r24, 0x05	; 5
 232:	8e 7f       	andi	r24, 0xFE	; 254
 234:	85 b9       	out	0x05, r24	; 5
	int1_counter++;
 236:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <__DATA_REGION_ORIGIN__>
 23a:	8f 5f       	subi	r24, 0xFF	; 255
 23c:	80 93 00 01 	sts	0x0100, r24	; 0x800100 <__DATA_REGION_ORIGIN__>
	if (int1_counter < 0 || int1_counter > 10)
 240:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <__DATA_REGION_ORIGIN__>
 244:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <__DATA_REGION_ORIGIN__>
 248:	8b 30       	cpi	r24, 0x0B	; 11
 24a:	10 f0       	brcs	.+4      	; 0x250 <__vector_2+0x40>
	{
		int1_counter = 0;
 24c:	10 92 00 01 	sts	0x0100, r1	; 0x800100 <__DATA_REGION_ORIGIN__>
	}
}                         
 250:	9f 91       	pop	r25
 252:	8f 91       	pop	r24
 254:	0f 90       	pop	r0
 256:	0f be       	out	0x3f, r0	; 63
 258:	0f 90       	pop	r0
 25a:	1f 90       	pop	r1
 25c:	18 95       	reti

0000025e <__tablejump2__>:
 25e:	ee 0f       	add	r30, r30
 260:	ff 1f       	adc	r31, r31
 262:	05 90       	lpm	r0, Z+
 264:	f4 91       	lpm	r31, Z
 266:	e0 2d       	mov	r30, r0
 268:	09 94       	ijmp

0000026a <_exit>:
 26a:	f8 94       	cli

0000026c <__stop_program>:
 26c:	ff cf       	rjmp	.-2      	; 0x26c <__stop_program>