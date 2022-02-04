; cpe301, David Nakasone
; DA_1, task:: 4
;	store ten 16-bit numbers starting at program memory/flash [0x0910]
;	retrieve the numbers to SRAM [0x500] using X-pointer
;	sum the 10 numbers and store result to SRAM [0x0406]

.include <m328PBdef.inc>

.cseg
.org 0

.equ sram_base = $0500    ; first number here, others follow, 10 total
.equ sram_sum = $0406     ; the sum will be stored here
.equ elements = 20        ; there are 10 numbers in the array, each number uses 2 bytes
.def counter = r16        ; loop control variable
.def temp = r17           ; temporarily hold a byte
.def sum_hi = r18         ; running sum, high
.def sum_lo = r19         ; running sum, low
		
	LDI counter, elements	      ; initialize the counter to 10
	LDI temp, 0		      ; initialize the temporary register
	LDI ZH, HIGH(int_array<<1)    ; initialize ZH to flash location	
	LDI ZL, LOW(int_array<<1)     ; initialize ZL to flash location
	LDI XH, HIGH(sram_base)       ; initialize XH to SRAM location
	LDI XL, LOW(sram_base)	      ; initialize XL to SRAM location

load_sram:
	LPM temp, Z+	  ; store contents of Z pointer to temp, then increment Z pointer
	ST X+, temp	  ; store temp to flash, then increment the X pointer	
	DEC counter       ; counter--
	BRNE load_sram    ; (20 bytes transfered to SRAM) ? continue : keep loading from flash

	LDI XH, HIGH(sram_base)    ; initialize XH to SRAM base address
	LDI XL, LOW(sram_base)     ; initialize XL to SRAM base address
	LDI counter, elements/2    ; counter = 10, using 2 bytes at a time
	LDI YH, 0                  ; initialize YH = 0
	LDI YL, 0                  ; initialize YL = 0
	LDI sum_hi, 0              ; initialize sum_hi = 0 
	LDI sum_lo, 0              ; initialize sum_lo = 0 

sum_nums:
	LD YL, X+
	LD YH, X+
	ADD sum_lo, YL    ; adding, carray flag could be set
	ADC sum_hi, YH    ; adding with carry if carry set previously
	DEC counter       ; counter--
	BRNE sum_nums     ; (10 number summmed) ? continue : keep summing

	LDI XH, HIGH(sram_sum)    ; initialize XH to SRAM sum storage location
	LDI XL, LOW(sram_sum)     ; initialize XL to SRAM sum storage location
	ST X+, sum_hi             ; store sum_hi to location X, then increment X to store next byte
	ST X, sum_lo              ; store sum_lo to location X

program_complete:
	RJMP program_complete
		
.org 0x0910    ; after off-set, this will be stored at 0x1220
int_array: .dw $0111, $0122, $0133, $0144, $0155, $0166, $0177, $0188, $0199, $01AA


;;;;END
