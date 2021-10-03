.setcpu		"6502"
.importzp	sp, sreg, regsave, regbank
.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4

SCREENMEM       = $0400         ; Start Screenbuffer C64
COLORMEM        = $D800         ; Start Color Memory C64

COL_BLACK       = $0 ; CTRL+1
COL_WHITE       = $1 ; CTRL+2
COL_RED         = $2 ; CTRL+3
COL_CYAN        = $3 ; CTRL+4
COL_PURPLE      = $4 ; CTRL+5
COL_GREEN       = $5 ; CTRL+6
COL_BLUE        = $6 ; CTRL+7
COL_YELLOW      = $7 ; CTRL+8
COL_ORANGE      = $8 ; C= + 1
COL_BROWN       = $9 ; C= + 2
COL_LIGHTRED    = $A ; C= + 3
COL_DARKGRAY    = $B ; C= + 4
COL_GRAY        = $C ; C= + 5
COL_LIGHTGREEN  = $D ; C= + 6
COL_LIGHTBLUE   = $E ; C= + 7
COL_LIGHTGRAY   = $F ; C= + 8

CHAR_SPACE      = 32;
CHAR_SC_A       = 65;

.code 

;
; INTERNAL: Cleans up the C Parameter stack. Akku holds param count.
cleanCStack:    adc sp
.scope
                sta sp
                bcc skipInc
                inc sp+1
skipInc:        rts
.endscope

;
; void ClearScreen()
.scope
_ClearScreen:
.export _ClearScreen

                lda #<SCREENMEM         ; Low- and High byte into the zeropage pointer.
                sta ptr1
                lda #>SCREENMEM
                sta ptr1+1
                ldx #$04                ; the screen memory are not 4 complete segments so...
outerLoop:      lda screenSegs-1, x     ; ... we get the byte count for each segment from ...
                tay                     ; ...a table.
                lda #CHAR_SC_A         ; space into the screen buffer.
innerLoop:      sta (ptr1), y
                dey
                bne innerLoop           ; one segment.
                sta (ptr1), y           ; account for 0 index in y.
                inc ptr1+1              ; increase high byte of pointer for next segment
                dex
                bne outerLoop           ; no underflow - we keep clearing.
                rts                     ; done!

.endscope

.rodata
screenSegs:     .byte $E8, $FF, $FF, $FF
.code

;                            y=3                 y=2                y=1                 y=0              
; void ClearScreenEx(unsigned char column, unsigned char line, unsigned char width, unsigned char height, unsigned char clearChar)

.scope
_ClearScreenEx:
.export _ClearScreenEx

                ; Get pointer to left, upper corner in screen memory.
                pha                     ; Clear/Fill Char on stack.
                lda #>(SCREENMEM-1)     ; Highbyte ScreenMem.
                sta ptr1+1
                ldy #$02
                clc                     ; we must not rol the carry into the accu
                lda (sp), y             ; Get line Param.
                rol                     ; *2
                tax                     ; becomes index into lineBeginnings
                clc
                lda #<(SCREENMEM-1)     ; Low- and High byte into the zeropage pointer
                adc lineBeginnings, x
                sta ptr1
                lda #>(SCREENMEM-1)
                adc lineBeginnings+1, x ; Add with carry - so prev. low-byte overflow 
                sta ptr1+1              ; is taken into account.

                clc
                ldy #$03                ; ScreenPtr+X1
                lda ptr1
                adc (sp),y
                sta ptr1
                bcc skipInc
                inc ptr1+1

                ; Get character count per line in tmp1
skipInc:        ldy #$01
                lda (sp), y
                tay
                iny
                sty tmp1                ; +1 because we count column+1 down to 1 (not 0).

                ; Get total lines in tmp2
                ldy #$00
                lda (sp), y
                tay
                iny
                sty tmp2                ; +1 because we count line+1 down to 1 (not 0).

                ; get the clear char in the accu
                ldx tmp2
outerLoop:      ldy tmp1
                pla
innerLoop:      sta (ptr1), y
                dey
                bne innerLoop
                pha
                clc
                lda ptr1
                adc #40
                sta ptr1
                bcc skipInc2
                inc ptr1+1
skipInc2:       dex
                bne outerLoop
                pla                     ; clean up stack.
                lda #5                  ; We have 5 paramters...
                jmp cleanCStack         ; ...we restore the C params stack and rts.

.rodata
lineBeginnings: .word 00 * 40, 01 * 40, 02 * 40, 03 * 40, 04 * 40
                .word 05 * 40, 06 * 40, 07 * 40, 08 * 40, 09 * 40 
                .word 10 * 40, 11 * 40, 12 * 40, 13 * 40, 14 * 40
                .word 15 * 40, 16 * 40, 17 * 40, 18 * 40, 19 * 40
                .word 20 * 40, 21 * 40, 22 * 40, 23 * 40, 24 * 40
.endscope


; Convert PET-ASCII Code to ScreenCode (Commodore 64)
; Based on snippet by Mace: https://codebase64.org/doku.php?id=base:petscii_to_screencode

.scope
_PetAsciiToScreenCode:
.export _PetAsciiToScreenCode
                cmp #$20		; if A<32 then...
		bcc sml32
		cmp #$60		; if A<96 then...
		bcc sml96
		cmp #$80		; if A<128 then...
		bcc sml128
		cmp #$a0		; if A<160 then...
		bcc sml160
		cmp #$c0		; if A<192 then...
		bcc sml192
		cmp #$ff		; if A<255 then...
		bcc sml32               ; same as A<32.
		lda #$7e		; A=255, then A=126
		rts

sml128: 	and #$5f		; if A=96..127 then strip bits 5 and 7
		rts

sml160:         ora #$40		; if A=128..159, then set bit 6
		rts

sml192:	        eor #$c0		; if A=160..191 then flip bits 6 and 7
		rts

sml96:  	and #$3f		; if A=32..95 then strip bits 6 and 7
		rts

sml32:  	eor #$80		; flip bit 7 (reverse on when off and vice versa)
                ldx #0                  ; For C compat, we return 16-bit in X/A (X-high, A-low)
                rts
.endscope
.code
 