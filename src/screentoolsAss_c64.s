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

;                            y=4                 y=3                y=2                 y=1                  y=0
; void ClearScreen()

.scope
_ClearScreen:
.export _ClearScreen

                lda #<SCREENMEM         ; Low- and High byte into the zeropage pointer
                sta ptr1
                lda #>SCREENMEM
                sta ptr1+1
                ldx #$03                ; the screen memory are not 4 complete segments so...
outerLoop:      lda screenSegs,x        ; ... we get the byte count for each segment from ...
                tay                     ; ...a table.
                lda #CHAR_SPACE         ; space into the screen buffer.
innerLoop:      sta (ptr1),y
                dey
                bvc innerLoop           ; one segment.
                inc ptr1+1              ; increase High byte of pointer for next segment
                clv                     ; clear overflow, since it'll remain
                dex
                bvc outerLoop           ; no underflow - we keep clearing.
                rts                     ; done!

.endscope

.rodata
screenSegs:     .byte $D5, $ff, $ff, $FF
.code

;                            y=3                 y=2                y=1                 y=0              
; void ClearScreenEx(unsigned char column, unsigned char line, unsigned char width, unsigned char height, unsigned char clearChar)

.scope
_ClearScreenEx:
.export _ClearScreenEx

                ; Get pointer to left, upper corner in screen memory.
                lda #>SCREENMEM
                sta ptr1+1
                ldy #$02
                lda (sp), y
                rol                     ; *2
                tax                     ; index in lineBeginnings
                clc
                lda #<SCREENMEM         ; Low- and High byte into the zeropage pointer
                adc lineBeginnings+1, x
                sta ptr1
                lda #>SCREENMEM
                adc lineBeginnings, x
                sta ptr1+1

                ldy #$03                ; ScreenPtr+X1
                lda #<SCREENMEM         
                adc (sp),y
                sta ptr1
                lda #>SCREENMEM
                adc #00
                sta ptr1+1

                ; Get character count per line in tmp1
                ldy #$03
                lda (sp), y
                sta tmp1

                ; Get total lines in tmp2
                ldy #$02
                lda (sp), y
                sta tmp2

                ; get the clear char in the accu
                ldy #$01
                lda (sp), y

                ldx tmp2
outerLoop:      ldy tmp1
innerLoop:      sta (ptr1), y
                dey
                bvc innerLoop
                pha
                clc
                lda ptr1
                adc #40
                sta ptr1
                lda ptr1+1
                adc #0
                sta ptr1+1
                clv
                dex
                bvc outerLoop
                rts

.rodata
lineBeginnings: .word 0 * 40, 1 * 40, 2 * 40, 3 * 40, 4 * 40
                .word 5 * 40, 6 * 40, 7 * 40, 8 * 40, 9 * 40 
                .word 10 * 40, 11 * 40, 12 * 40, 13 * 40, 14 * 40
                .word 15 * 40, 16 * 40, 17 * 40, 18 * 40, 19 * 40
                .word 20 * 40, 21 * 40, 22 * 40, 23 * 40, 24 * 40
.endscope

.code
 