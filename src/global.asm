;
; File generated by cc65 v 2.18 - Git f8be35b
;
	.fopt		compiler,"cc65 v 2.18 - Git f8be35b"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.import		_printf
	.import		_gotoxy
	.export		__lineBuffer
	.export		__workingLineBuffer
	.export		__editorLines
	.export		__editorLinesCapacity
	.export		__editorLineSegments
	.export		__firstFreeSegment
	.export		__maxLineSegment
	.export		__textPos
	.export		__screenSize
	.export		__statusBarLineNo
	.export		__debugLineNo
	.export		__maxLine
	.export		_DebugPrint
	.export		_DebugPrintSlow

.segment	"BSS"

__lineBuffer:
	.res	255,$00
__workingLineBuffer:
	.res	255,$00
__editorLines:
	.res	2,$00
__editorLinesCapacity:
	.res	2,$00
__editorLineSegments:
	.res	2,$00
__firstFreeSegment:
	.res	2,$00
__maxLineSegment:
	.res	2,$00
__textPos:
	.res	6,$00
__screenSize:
	.res	5,$00
__statusBarLineNo:
	.res	1,$00
__debugLineNo:
	.res	1,$00
__maxLine:
	.res	2,$00

; ---------------------------------------------------------------
; void __near__ DebugPrint (__near__ const unsigned char *, int)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_DebugPrint: near

.segment	"CODE"

	jsr     pushax
	lda     #$01
	jsr     pusha
	lda     __debugLineNo
	jsr     _gotoxy
	ldy     #$05
	jsr     pushwysp
	ldy     #$05
	jsr     pushwysp
	ldy     #$04
	jsr     _printf
	jmp     incsp4

.endproc

; ---------------------------------------------------------------
; void __near__ DebugPrintSlow (__near__ const unsigned char *, int)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_DebugPrintSlow: near

.segment	"CODE"

	jsr     pushax
	lda     #$01
	jsr     pusha
	lda     __debugLineNo
	jsr     _gotoxy
	ldy     #$05
	jsr     pushwysp
	ldy     #$05
	jsr     pushwysp
	ldy     #$04
	jsr     _printf
	ldy     #$00
	tya
	sta     (sp),y
	iny
	sta     (sp),y
L0017:	ldy     #$01
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	cmp     #$20
	txa
	sbc     #$4E
	bvc     L001E
	eor     #$80
L001E:	bpl     L0018
	iny
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	clc
	adc     #$01
	bcc     L0020
	inx
L0020:	jsr     stax0sp
	jmp     L0017
L0018:	jmp     incsp4

.endproc
