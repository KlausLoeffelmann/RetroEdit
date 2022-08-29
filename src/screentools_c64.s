.setcpu		"6502"
.importzp	sp, sreg, regsave, regbank
.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.importzp   pusha, pushax

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

CHAR_SPACE      = 32
CHAR_SC_A       = 65

CHAR_LEFT_UPPER_CORNER  = 112
CHAR_RIGHT_UPPER_CORNER = 110
CHAR_LEFT_LOWER_CORNER  = 109
CHAR_RIGHT_LOWER_CORNER = 125
CHAR_HORIZONTAL_LINE    = 64
CHAR_VERTICAL_LINE      = 93

.code 

;
; INTERNAL: Cleans up the C Parameter stack. Y holds param byte count.
cleanCStack:
.scope
                pha             ; we need to save a, since it could hold
                tya             ; a return value's low byte (along with X, high byte of result)
                clc
                adc sp
                sta sp
                bcc skipInc
                inc sp+1
skipInc:        pla
                rts
.endscope

;
; INTERNAL: PetAscii to Screencode
; (Based on snippet by Mace: https://codebase64.org/doku.php?id=base:petscii_to_screencode)
petsciiToSCode: 
.scope
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

sml128: 	    and #$5f		; if A=96..127 then strip bits 5 and 7
		        rts

sml160:         ora #$40		; if A=128..159, then set bit 6
		        rts

sml192:	        eor #$c0		; if A=160..191 then flip bits 6 and 7
		        rts

sml96:  	    and #$3f		; if A=32..95 then strip bits 6 and 7
		        rts

sml32:  	    eor #$80		; flip bit 7 (reverse on when off and vice versa)
                rts;
.endscope

;
; INTERNAL: Draws a horizontal text line from characters.
hCharRepeat: 
.scope
                ; tmp1: X, tmp2: y, tmp3: width, akku: character, x: color (todo)
                xPos = tmp1
                yPos = tmp2
                width = tmp3

                pha
                lda yPos                ; Get yPos Param.
                clc                     ; we don't want to rol the carry!
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
                lda ptr1                ; add xPos
                adc xPos
                sta ptr1
                bcc skipInc
                inc ptr1+1
skipInc:        pla                     ; that's the char we want to print
                ldy width
loop:           sta (ptr1),Y
                dey
                bne loop
                rts
.endscope

                ; tmp1: X, tmp2: y, tmp4: height, akku: lineChar, x: color (todo)
vCharRepeat:
.scope
                xPos = tmp1
                yPos = tmp2
                height = tmp4

                pha
                lda yPos                ; Get yPos Param.
                clc                     ; we don't want to rol the carry!
                rol                     ; *2
                tax                     ; becomes index into lineBeginnings
                clc
                lda #<(SCREENMEM)     ; Low- and High byte into the zeropage pointer
                adc lineBeginnings, x
                sta ptr1
                lda #>(SCREENMEM)
                adc lineBeginnings+1, x ; Add with carry - so prev. low-byte overflow 
                sta ptr1+1              ; is taken into account.
                clc
                lda ptr1                ; add xPos
                adc xPos
                sta ptr1
                bcc skipInc
                inc ptr1+1
skipInc:        ldx height
                ldy #0
loop:           pla                     ; the character to repeast
                sta (ptr1),y
                pha
                clc
                lda ptr1
                adc #40
                sta ptr1
                bcc skipInc2
                inc ptr1+1
skipInc2:       dex
                bne loop
                pla
                rts
.endscope

                ; tmp1: xPos, tmp2: yPos, akku: char, x: color (todo)
sCharOut:
.scope
                xPos = tmp1
                yPos = tmp2
                height = tmp4

                pha                     ; save the character to print, first.
                clc                     ; we don't want to rol the carry!
                lda yPos                ; Get yPos Param.
                rol                     ; *2
                tax                     ; becomes index into lineBeginnings
                clc
                lda #<(SCREENMEM)       ; Low- and High byte into the zeropage pointer
                adc lineBeginnings, x
                sta ptr1
                lda #>(SCREENMEM)
                adc lineBeginnings+1, x ; Add with carry - so prev. low-byte overflow 
                sta ptr1+1              ; is taken into account.
                clc
                lda ptr1                ; add xPos
                adc xPos
                sta ptr1
                bcc skipInc
                inc ptr1+1
skipInc:        ldy #0
                pla
loop:           sta (ptr1),y
                rts
.endscope

;
; void ClearScreen()
.scope
_ClearScreen:
.export _ClearScreen

                ldy #0
                lda #CHAR_SPACE
loop:           sta SCREENMEM, y
                sta SCREENMEM+$100, y
                sta SCREENMEM+$200, y
                iny
                bne loop
                ldy #$e8
loop2:          sta SCREENMEM+$2ff, y
                dey
                bne loop2
                rts

.endscope

;                            y=3                 y=2                y=1                 y=0              
; void ClearScreenEx(unsigned char column, unsigned char line, unsigned char width, unsigned char height, unsigned char clearChar)
.scope
_ClearScreenEx:
.export _ClearScreenEx

                ; Get pointer to left, upper corner in screen memory.
                pha                     ; Clear/Fill Char on stack.
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
                iny                      ; ScreenPtr+X1
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
                ldy #0                  ; We have 5 paramters...
                jmp cleanCStack         ; ...we restore the C params stack and rts.
.endscope

.rodata
lineBeginnings: .word 00 * 40, 01 * 40, 02 * 40, 03 * 40, 04 * 40
                .word 05 * 40, 06 * 40, 07 * 40, 08 * 40, 09 * 40 
                .word 10 * 40, 11 * 40, 12 * 40, 13 * 40, 14 * 40
                .word 15 * 40, 16 * 40, 17 * 40, 18 * 40, 19 * 40
                .word 20 * 40, 21 * 40, 22 * 40, 23 * 40, 24 * 40
.code

; Convert PET-ASCII Code to ScreenCode (Commodore 64)
.scope
_PetAsciiToScreenCode:
.export _PetAsciiToScreenCode
                jsr petsciiToSCode
                ldx #0                  ; We are returning A/X, petasciiToSCode only returned A
                ldy #1                  ; We have 1 byte param  
                jmp cleanCStack         ; 
.endscope

; Draw Window
;                            y=3                 y=2                y=1                 y=0                   Akku
; void DrawWindow(unsigned char column, unsigned char line, unsigned char width, unsigned char height, unsigned char color)
_DrawWindow:
                xPos = tmp1
                yPos = tmp2
                width = tmp3
                height = tmp4
.scope
.export _DrawWindow
                ldy #$03
                lda (sp), y             ; Get column (xPos) param.
                sta xPos                ; For DrawLine internal.
                dey
                lda (sp), y             ; Get line (yPos) param.
                sta yPos
                dey
                lda (sp), y             ; Get width param.
                sta width
                dey
                lda (sp), y             ; Get width param.
                sta height

                ; upper left corner:
                lda #CHAR_LEFT_UPPER_CORNER
                jsr sCharOut

                ; upper right corner:
                lda xPos
                pha                     ; temp-save xpos;
                clc
                adc width
                sta xPos
                lda #CHAR_RIGHT_UPPER_CORNER
                jsr sCharOut
                pla
                sta xPos

                ; upper line
                inc xPos
                dec width
                lda #CHAR_HORIZONTAL_LINE
                jsr hCharRepeat
                dec xPos
                inc width

                ; lower left conner:
                lda yPos            ; tempsave ypos
                pha
                adc height
                sta yPos
                lda #CHAR_LEFT_LOWER_CORNER
                jsr sCharOut

                ;lower right corner
                lda xPos               ; tempsave xpos;
                pha
                clc
                adc width
                sta xPos
                lda #CHAR_RIGHT_LOWER_CORNER
                jsr sCharOut
                pla 
                sta xPos

                ; lower line
                inc xPos
                dec width
                lda #CHAR_HORIZONTAL_LINE
                jsr hCharRepeat
                dec xPos
                inc width
                pla
                sta yPos

                ; left line
                inc yPos
                dec height
                lda #CHAR_VERTICAL_LINE
                jsr vCharRepeat

                ; right line
                lda xPos               
                clc
                adc width
                sta xPos
                lda #CHAR_VERTICAL_LINE
                jsr vCharRepeat
                ldy #5
                jmp cleanCStack         ; ...we restore the C params stack and rts.
.endscope

; Draw UIText - Draws string containing accelerator key - defined with "&"" - in revers. (&File, &OK, S&earch)
;                  y=3/2             y=1                y=0                     Akku
; void DrawUIText(char *text, unsigned char line, unsigned char column, unsigned char color)
_DrawUIText:
.scope
                color = tmp3
                ptrString = ptr1
                ptrScreen = ptr2
.export _DrawUIText
                jsr pusha
                sta color
                ldy #$04
                lda (sp),y             ; Get address of string...
                sta ptrString+1
                dey
                lda (sp),y 
                sta ptrString          ; store in ptrString.
                dey
                dey
                clc                     ; we must not rol the carry into the accu
                lda (sp), y             ; Get line Param.
                rol                     ; *2
                tax                     ; becomes index into lineBeginnings
                clc
                lda #<(SCREENMEM-1)     ; Low- and High byte into the zeropage pointer
                adc lineBeginnings, x
                sta ptrScreen
                lda #>(SCREENMEM-1)
                adc lineBeginnings+1, x ; Add with carry - so prev. low-byte overflow 
                sta ptrScreen+1         ; is taken into account.

                clc
                iny                      ; ScreenPtr+X1
                lda ptrScreen
                adc (sp),y
                sta ptrScreen
                bcc skipInc
                inc ptrScreen+1

skipInc:        ldy #0
                ldx #0
                clc
loop:           lda (ptrString),y       ; Get the first char
                beq finish              ; 0-end -> we're done.
                bcs skipRevTest         ; We found the & already, so we skip the test.
                cmp #'&'                ; Hotkey?
                beq nextChar            ; we overread this. Carry now is set!
                clc                     ; Might have been >'&'', so the Carry has to recleared.
skipRevTest:    php                     ; We need the original Carry
                jsr petsciiToSCode      ; Convert to Screencode
                plp                     ; Carry says, if we need to invert
                bcc skipRevChar         ; carry was clear, so there wasn't an Ampersand
                clc                     ; carry was set, we need to revert, but with clear Carry.
                adc #128                ; +128 equals the revert char.
                clc
skipRevChar:    sta (ptrScreen),y
                inx                     ; char count without '&'
nextChar:       iny
                bne loop
finish:         txa
                ldx #0                  ; high byte of return value
                ldy #5                  ; 4 Params, but 1 is pointer.
                jmp cleanCStack
.endscope

; GetUITextLength - Calculates length of string containing accelerator key - defined with "&"" - in revers. (&File, &OK, S&earch)
;                          ptrText=1/0
; unsigned char DrawUIText(char *text)
_GetUITextLength:
.scope
                ptrString = ptr1
.export _GetUITextLength
                ldy #$01
                lda (sp),y             ; Get address of string...
                sta ptrString+1
                dey
                lda (sp),y 
                sta ptrString          ; store in ptrString.

                ldy #0
                ldx #0
loop:           lda (ptrString),y       ; Get the first char
                beq finish              ; 0-end -> we're done.
                cmp #'&'                ; Hotkey?
                beq nextChar            ; we overread this. Carry now is set!
nextChar:       iny
                bne loop
finish:         txa
                ldy #2                  ; 1 Param, but pointer (2 bytes)
                jmp cleanCStack
.endscope
