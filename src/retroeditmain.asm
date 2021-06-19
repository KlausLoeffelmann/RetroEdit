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
	.forceimport	__STARTUP__
	.import		_realloc
	.import		_printf
	.import		_clrscr
	.import		_gotoxy
	.import		_cputc
	.import		_cgetc
	.import		_cursor
	.import		_screensize
	.import		_SomeMethod
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
	.export		_UpdateDocInfo
	.export		_EnsureEditorLinesCapacity
	.export		_Initialize
	.export		_PrintLineNumber
	.export		_GetNextFreeLineSegment
	.export		_SaveBufferToEditorMemory
	.export		_GetBufferFromEditorMemoryAndPrint
	.export		_GetWorkingLine
	.export		_Invalidate
	.export		_LineBufferToCurrentScreenLine
	.export		_CursorLeft
	.export		_CursorRight
	.export		_DeleteLine
	.export		_InsertChar
	.export		_DeleteChar
	.export		_Backspace
	.export		_CursorUp
	.export		_CursorDown
	.export		_HandleReturnKey
	.export		_main

.segment	"RODATA"

L003F:
	.byte	$CC,$49,$4E,$45,$53,$2D,$2A,$3A,$20,$25,$D8,$2F,$25,$D8,$00
L0036:
	.byte	$CB,$45,$59,$43,$4F,$44,$45,$3A,$20,$25,$30,$33,$44,$00
L002F:
	.byte	$C3,$4F,$4C,$55,$4D,$4E,$3A,$20,$25,$30,$33,$44,$00
L0028:
	.byte	$CC,$49,$4E,$45,$3A,$20,$25,$30,$34,$44,$00
L0047:
	.byte	$4D,$41,$58,$CC,$D3,$3A,$20,$25,$44,$00
L0147:
	.byte	$25,$30,$34,$44,$3A,$00
L008C	:=	L0147+0

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
	jsr     _SomeMethod
	ldy     #$00
	tya
	sta     (sp),y
	iny
	sta     (sp),y
L0018:	ldy     #$01
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	cmp     #$20
	txa
	sbc     #$4E
	bvc     L001F
	eor     #$80
L001F:	bpl     L0019
	iny
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	clc
	adc     #$01
	bcc     L0021
	inx
L0021:	jsr     stax0sp
	jmp     L0018
L0019:	jmp     incsp4

.endproc

; ---------------------------------------------------------------
; void __near__ UpdateDocInfo (int, int, unsigned char)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_UpdateDocInfo: near

.segment	"CODE"

	jsr     pusha
	lda     __editorLineSegments
	ldx     __editorLineSegments+1
	jsr     pushax
	lda     #$01
	jsr     pusha
	lda     __statusBarLineNo
	jsr     _gotoxy
	lda     #<(L0028)
	ldx     #>(L0028)
	jsr     pushax
	ldy     #$0A
	jsr     pushwysp
	ldy     #$04
	jsr     _printf
	lda     #$0F
	jsr     pusha
	lda     __statusBarLineNo
	jsr     _gotoxy
	lda     #<(L002F)
	ldx     #>(L002F)
	jsr     pushax
	ldy     #$08
	jsr     pushwysp
	ldy     #$04
	jsr     _printf
	lda     #$1E
	jsr     pusha
	lda     __statusBarLineNo
	jsr     _gotoxy
	lda     #<(L0036)
	ldx     #>(L0036)
	jsr     pushax
	ldy     #$04
	lda     (sp),y
	jsr     pusha0
	ldy     #$04
	jsr     _printf
	lda     #$2B
	jsr     pusha
	lda     __statusBarLineNo
	jsr     _gotoxy
	lda     __maxLineSegment
	ldx     __maxLineSegment+1
	jsr     pushax
	lda     #$0C
	jsr     tosumula0
	jsr     addeq0sp
	lda     #<(L003F)
	ldx     #>(L003F)
	jsr     pushax
	ldy     #$05
	jsr     pushwysp
	lda     __firstFreeSegment
	ldx     __firstFreeSegment+1
	jsr     pushax
	ldy     #$06
	jsr     _printf
	lda     #$3E
	jsr     pusha
	lda     __statusBarLineNo
	jsr     _gotoxy
	lda     #<(L0047)
	ldx     #>(L0047)
	jsr     pushax
	lda     __maxLineSegment
	ldx     __maxLineSegment+1
	jsr     pushax
	ldy     #$04
	jsr     _printf
	lda     __textPos+4
	clc
	adc     #$06
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	jmp     incsp7

.endproc

; ---------------------------------------------------------------
; void __near__ EnsureEditorLinesCapacity (unsigned char)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_EnsureEditorLinesCapacity: near

.segment	"CODE"

	jsr     pusha
	lda     __maxLine
	sec
	sbc     __editorLinesCapacity
	sta     tmp1
	lda     __maxLine+1
	sbc     __editorLinesCapacity+1
	ora     tmp1
	beq     L028C
	bcs     L028E
L028C:	ldy     #$00
	lda     (sp),y
	cmp     #$01
	jne     incsp1
L028E:	lda     __editorLinesCapacity
	clc
	adc     __editorLinesCapacity
	sta     __editorLinesCapacity
	lda     __editorLinesCapacity+1
	adc     __editorLinesCapacity+1
	sta     __editorLinesCapacity+1
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __editorLinesCapacity
	ldx     __editorLinesCapacity+1
	jsr     mulax3
	jsr     _realloc
	sta     __editorLines
	stx     __editorLines+1
	jmp     incsp1

.endproc

; ---------------------------------------------------------------
; void __near__ Initialize (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_Initialize: near

.segment	"CODE"

	ldx     #$00
	txa
	sta     __maxLine
	sta     __maxLine+1
	sta     __textPos+2
	sta     __textPos
	sta     __textPos+1
	sta     __textPos+4
	sta     __textPos+3
	lda     #$04
	sta     __editorLinesCapacity
	stx     __editorLinesCapacity+1
	lda     #$01
	jsr     _EnsureEditorLinesCapacity
	lda     #<(__screenSize)
	ldx     #>(__screenSize)
	jsr     pushax
	lda     #<(__screenSize+1)
	ldx     #>(__screenSize+1)
	jsr     _screensize
	lda     __screenSize
	sec
	sbc     #$06
	bcs     L0290
	sec
L0290:	sbc     #$01
	sta     __screenSize
	lda     __screenSize+1
	sec
	sbc     #$01
	sta     __statusBarLineNo
	lda     #$01
	sta     __screenSize+2
	lda     __screenSize+1
	sec
	sbc     #$02
	sta     __screenSize+3
	sec
	sbc     __screenSize+2
	sta     __screenSize+4
	lda     __screenSize+1
	sec
	sbc     #$02
	sta     __debugLineNo
	jsr     _clrscr
	lda     #$01
	jsr     _cursor
	lda     #$00
	sta     __maxLineSegment
	sta     __maxLineSegment+1
	ldx     #$90
	sta     __editorLineSegments
	stx     __editorLineSegments+1
	sta     __firstFreeSegment
	sta     __firstFreeSegment+1
	lda     #$06
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jmp     _gotoxy

.endproc

; ---------------------------------------------------------------
; void __near__ PrintLineNumber (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_PrintLineNumber: near

.segment	"CODE"

	lda     #$00
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	lda     #<(L008C)
	ldx     #>(L008C)
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	clc
	adc     #$01
	bcc     L008F
	inx
L008F:	jsr     pushax
	ldy     #$04
	jmp     _printf

.endproc

; ---------------------------------------------------------------
; __near__ struct LineSegment * __near__ GetNextFreeLineSegment (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_GetNextFreeLineSegment: near

.segment	"CODE"

	jsr     decsp2
	lda     __firstFreeSegment
	ora     __firstFreeSegment+1
	bne     L0091
	lda     __editorLineSegments
	ldx     __editorLineSegments+1
	jsr     pushax
	lda     __maxLineSegment
	ldx     __maxLineSegment+1
	jsr     pushax
	lda     #$0C
	jsr     tosmula0
	jsr     tosaddax
	jsr     stax0sp
	lda     __maxLineSegment
	ldx     __maxLineSegment+1
	clc
	adc     #$01
	bcc     L0097
	inx
L0097:	sta     __maxLineSegment
	stx     __maxLineSegment+1
	jmp     L0098
L0091:	lda     __firstFreeSegment
	ldx     __firstFreeSegment+1
	jsr     stax0sp
	lda     __firstFreeSegment+1
	sta     ptr1+1
	lda     __firstFreeSegment
	sta     ptr1
	ldy     #$0B
	lda     (ptr1),y
	sta     __firstFreeSegment+1
	dey
	lda     (ptr1),y
	sta     __firstFreeSegment
L0098:	ldy     #$01
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jmp     incsp2

.endproc

; ---------------------------------------------------------------
; void __near__ SaveBufferToEditorMemory (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_SaveBufferToEditorMemory: near

.segment	"CODE"

	jsr     decsp6
	lda     #$00
	jsr     _EnsureEditorLinesCapacity
	jsr     _GetNextFreeLineSegment
	jsr     stax0sp
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$01
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	iny
	sta     (ptr1),y
	iny
	txa
	sta     (ptr1),y
	lda     #$00
	ldy     #$05
	sta     (sp),y
	dey
L0294:	sta     (sp),y
	cmp     __textPos+5
	bcs     L00A9
	iny
	lda     (sp),y
	cmp     #$0A
	bne     L00B0
	jsr     _GetNextFreeLineSegment
	ldy     #$02
	jsr     staxysp
	ldy     #$01
	lda     (sp),y
	sta     ptr1+1
	dey
	lda     (sp),y
	sta     ptr1
	ldy     #$03
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	ldy     #$0A
	sta     (ptr1),y
	iny
	txa
	sta     (ptr1),y
	ldy     #$03
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jsr     stax0sp
	lda     #$00
	ldy     #$05
	sta     (sp),y
L00B0:	jsr     pushw0sp
	ldy     #$07
	lda     (sp),y
	pha
	clc
	adc     #$01
	sta     (sp),y
	pla
	jsr     tosadda0
	jsr     pushax
	ldy     #$06
	lda     (sp),y
	tay
	lda     __lineBuffer,y
	ldy     #$00
	jsr     staspidx
	ldy     #$04
	lda     (sp),y
	clc
	adc     #$01
	jmp     L0294
L00A9:	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	lda     __textPos+5
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     (sp),y
	sta     ptr1+1
	dey
	lda     (sp),y
	sta     ptr1
	tya
	ldy     #$0A
	sta     (ptr1),y
	iny
	sta     (ptr1),y
	jmp     incsp6

.endproc

; ---------------------------------------------------------------
; void __near__ GetBufferFromEditorMemoryAndPrint (unsigned char)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_GetBufferFromEditorMemoryAndPrint: near

.segment	"CODE"

	jsr     pusha
	jsr     decsp8
	ldy     #$08
	lda     (sp),y
	jne     L0298
	lda     __firstFreeSegment
	ora     __firstFreeSegment+1
	beq     L00C7
	lda     __firstFreeSegment
	ldx     __firstFreeSegment+1
	jmp     L029D
L00CC:	ldy     #$05
	lda     (sp),y
	sta     ptr1+1
	dey
	lda     (sp),y
	sta     ptr1
	ldy     #$0B
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
L029D:	ldy     #$04
	jsr     staxysp
	sta     ptr1
	stx     ptr1+1
	ldy     #$0B
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$00
	bne     L00CC
	cmp     #$00
	bne     L00CC
	ldy     #$07
	jsr     pushwysp
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$02
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	ldy     #$0A
	jsr     staxspidx
	jmp     L0298
L00C7:	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$02
	lda     (ptr1),y
	sta     __firstFreeSegment+1
	dey
	lda     (ptr1),y
	sta     __firstFreeSegment
L0298:	lda     __textPos+2
	sec
	sbc     __textPos+4
	ldy     #$00
	sta     (sp),y
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$02
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	ldy     #$06
	jsr     staxysp
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	ldy     #$03
	sta     (sp),y
	sta     __textPos+5
	lda     #$00
	dey
	sta     (sp),y
	lda     #$06
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	lda     #$00
	ldy     #$01
L029E:	sta     (sp),y
	cmp     #$FF
	jcs     L00EA
	lda     (sp),y
	ldy     #$03
	cmp     (sp),y
	bcs     L00F1
	lda     #<(__lineBuffer)
	ldx     #>(__lineBuffer)
	ldy     #$01
	clc
	adc     (sp),y
	bcc     L00F5
	inx
L00F5:	jsr     pushax
	ldy     #$0B
	jsr     pushwysp
	ldy     #$06
	lda     (sp),y
	pha
	clc
	adc     #$01
	sta     (sp),y
	pla
	jsr     tosadda0
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	ldy     #$02
	lda     (sp),y
	cmp     #$0A
	bne     L00FE
	ldy     #$07
	lda     (sp),y
	sta     ptr1+1
	dey
	lda     (sp),y
	sta     ptr1
	ldy     #$0B
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	ldy     #$06
	jsr     staxysp
	lda     #$00
	ldy     #$02
	sta     (sp),y
	jmp     L00FE
L00F1:	ldy     #$01
	lda     (sp),y
	tay
	lda     #$20
	sta     __lineBuffer,y
L00FE:	ldy     #$01
	lda     (sp),y
	dey
	cmp     (sp),y
	bcc     L00EB
	iny
	lda     (sp),y
	jsr     pusha0
	ldy     #$02
	lda     (sp),y
	clc
	adc     __screenSize
	bcc     L0296
	ldx     #$01
L0296:	jsr     tosicmp
	bcc     L0104
	bne     L00EB
L0104:	ldy     #$01
	lda     (sp),y
	tay
	lda     __lineBuffer,y
	jsr     _cputc
L00EB:	ldy     #$01
	lda     (sp),y
	clc
	adc     #$01
	jmp     L029E
L00EA:	ldy     #$09
	jmp     addysp

.endproc

; ---------------------------------------------------------------
; unsigned int __near__ GetWorkingLine (unsigned int)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_GetWorkingLine: near

.segment	"CODE"

	jsr     pushax
	jsr     decsp5
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	ldy     #$08
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$02
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     stax0sp
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	ldy     #$08
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	ldy     #$03
	sta     (sp),y
	lda     #$00
	dey
	sta     (sp),y
	ldy     #$04
L02A1:	sta     (sp),y
	cmp     #$FF
	bcs     L0116
	lda     (sp),y
	dey
	cmp     (sp),y
	bcs     L011D
	lda     #<(__workingLineBuffer)
	ldx     #>(__workingLineBuffer)
	iny
	clc
	adc     (sp),y
	bcc     L0121
	inx
L0121:	jsr     pushax
	ldy     #$05
	jsr     pushwysp
	ldy     #$06
	lda     (sp),y
	pha
	clc
	adc     #$01
	sta     (sp),y
	pla
	jsr     tosadda0
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	ldy     #$02
	lda     (sp),y
	cmp     #$0A
	bne     L0117
	dey
	lda     (sp),y
	sta     ptr1+1
	dey
	lda     (sp),y
	sta     ptr1
	ldy     #$0B
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     stax0sp
	lda     #$00
	ldy     #$02
	sta     (sp),y
	jmp     L0117
L011D:	iny
	lda     (sp),y
	tay
	lda     #$20
	sta     __workingLineBuffer,y
L0117:	ldy     #$04
	lda     (sp),y
	clc
	adc     #$01
	jmp     L02A1
L0116:	dey
	lda     (sp),y
	ldx     #$00
	jmp     incsp7

.endproc

; ---------------------------------------------------------------
; void __near__ Invalidate (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_Invalidate: near

.segment	"CODE"

	jsr     decsp7
	lda     __textPos+2
	sec
	sbc     __textPos+4
	ldy     #$01
	sta     (sp),y
	clc
	adc     __screenSize
	dey
	sta     (sp),y
	lda     __textPos
	sec
	sbc     __textPos+3
	pha
	lda     __textPos+1
	sbc     #$00
	pla
	ldy     #$04
	sta     (sp),y
	clc
	adc     __screenSize+4
	dey
	sta     (sp),y
	iny
	lda     (sp),y
	iny
	sta     (sp),y
	ldx     #$00
L02A9:	lda     (sp),y
	ldy     #$03
	cmp     (sp),y
	jcs     L02B4
	ldy     #$05
	lda     (sp),y
	sec
	sbc     __maxLine
	sta     tmp1
	txa
	sbc     __maxLine+1
	ora     tmp1
	beq     L02AA
	jcs     L0141
	txa
L02AA:	jsr     pusha
	ldy     #$06
	lda     (sp),y
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	lda     #<(L0147)
	ldx     #>(L0147)
	jsr     pushax
	ldy     #$07
	ldx     #$00
	lda     (sp),y
	clc
	adc     #$01
	bcc     L014A
	inx
L014A:	jsr     pushax
	ldy     #$04
	jsr     _printf
	lda     #$06
	jsr     pusha
	tay
	lda     (sp),y
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	ldy     #$05
	ldx     #$00
	lda     (sp),y
	cmp     __maxLine
	txa
	sbc     __maxLine+1
	bcs     L02B3
	lda     (sp),y
	jsr     _GetWorkingLine
	ldy     #$02
	sta     (sp),y
	lda     #$00
	ldy     #$06
L02B5:	sta     (sp),y
	cmp     #$FF
	bcs     L013B
	lda     (sp),y
	ldy     #$01
	cmp     (sp),y
	bcc     L0155
	ldy     #$06
	lda     (sp),y
	sec
	ldy     #$00
	sbc     (sp),y
	bcc     L02AF
	bne     L0155
L02AF:	ldy     #$06
	lda     (sp),y
	tay
	lda     __workingLineBuffer,y
	jsr     _cputc
L0155:	ldy     #$06
	lda     (sp),y
	clc
	adc     #$01
	jmp     L02B5
L0141:	ldy     #$01
	lda     (sp),y
	ldy     #$06
L02B6:	sta     (sp),y
	sec
	ldy     #$00
	sbc     (sp),y
	bcc     L0167
	bne     L013B
L0167:	lda     #$20
	jsr     _cputc
	ldy     #$06
	lda     (sp),y
	clc
	adc     #$01
	jmp     L02B6
L013B:	ldy     #$05
	ldx     #$00
L02B3:	lda     (sp),y
	clc
	adc     #$01
	sta     (sp),y
	jmp     L02A9
L02B4:	lda     __textPos+4
	clc
	adc     #$06
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	sta     __textPos+5
	jmp     incsp7

.endproc

; ---------------------------------------------------------------
; void __near__ LineBufferToCurrentScreenLine (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_LineBufferToCurrentScreenLine: near

.segment	"CODE"

	jsr     decsp3
	lda     __textPos+2
	sec
	sbc     __textPos+4
	ldy     #$01
	sta     (sp),y
	clc
	adc     __screenSize
	dey
	sta     (sp),y
	lda     #$06
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	lda     #$00
	ldy     #$02
L02C3:	sta     (sp),y
	cmp     #$FF
	bcs     L02C2
	lda     (sp),y
	dey
	cmp     (sp),y
	bcc     L017F
	iny
	lda     (sp),y
	sec
	ldy     #$00
	sbc     (sp),y
	bcc     L02C0
	bne     L017F
L02C0:	ldy     #$02
	lda     (sp),y
	sec
	sbc     __textPos+5
	sta     tmp1
	lda     tmp1
	beq     L02C1
	bcs     L0189
L02C1:	lda     (sp),y
	tay
	lda     __lineBuffer,y
	jmp     L02BB
L0189:	lda     #$20
L02BB:	jsr     _cputc
L017F:	ldy     #$02
	lda     (sp),y
	clc
	adc     #$01
	jmp     L02C3
L02C2:	lda     __textPos+4
	clc
	adc     #$06
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jsr     _gotoxy
	jmp     incsp3

.endproc

; ---------------------------------------------------------------
; void __near__ CursorLeft (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_CursorLeft: near

.segment	"CODE"

	lda     __textPos+2
	beq     L019B
	dec     __textPos+2
	dec     __textPos+4
	lda     __textPos+4
	cmp     #$FF
	bne     L019B
	lda     #$00
	sta     __textPos+4
L019B:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ CursorRight (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_CursorRight: near

.segment	"CODE"

	lda     __textPos+2
	cmp     __textPos+5
	bcs     L01A4
	inc     __textPos+2
	inc     __textPos+4
	lda     __textPos+4
	jsr     pusha0
	lda     __screenSize
	sec
	sbc     #$01
	bcs     L01A6
	ldx     #$FF
L01A6:	jsr     tosicmp
	bcc     L01A4
	beq     L01A4
	lda     __screenSize
	sec
	sbc     #$01
	sta     __textPos+4
L01A4:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ DeleteLine (unsigned int)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_DeleteLine: near

.segment	"CODE"

	jsr     pushax
	jsr     decsp3
	lda     __maxLine
	ora     __maxLine+1
	bne     L02C8
	jmp     incsp5
L02C8:	lda     __maxLine
	ldy     #$03
	cmp     (sp),y
	lda     __maxLine+1
	iny
	sbc     (sp),y
	bcs     L02C9
	jmp     incsp5
L02C9:	lda     __firstFreeSegment
	ora     __firstFreeSegment+1
	beq     L01B0
	lda     __firstFreeSegment
	ldx     __firstFreeSegment+1
	jmp     L02C7
L01B5:	ldy     #$02
	lda     (sp),y
	sta     ptr1+1
	dey
	lda     (sp),y
	sta     ptr1
	ldy     #$0B
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
L02C7:	ldy     #$01
	jsr     staxysp
	sta     ptr1
	stx     ptr1+1
	ldy     #$0B
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$00
	bne     L01B5
	cmp     #$00
	bne     L01B5
	ldy     #$04
	jsr     pushwysp
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	ldy     #$08
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$02
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	ldy     #$0A
	jsr     staxspidx
	jmp     L01BF
L01B0:	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	ldy     #$06
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$02
	lda     (ptr1),y
	sta     __firstFreeSegment+1
	dey
	lda     (ptr1),y
	sta     __firstFreeSegment
L01BF:	ldy     #$03
	lda     (sp),y
	ldy     #$00
	sta     (sp),y
	ldx     #$00
L02C6:	lda     (sp),y
	cmp     __maxLine
	txa
	sbc     __maxLine+1
	bcs     L01C4
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	ldy     #$02
	ldx     #$00
	lda     (sp),y
	jsr     mulax3
	jsr     tosaddax
	jsr     pushax
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	ldy     #$04
	ldx     #$00
	lda     (sp),y
	clc
	adc     #$01
	bcc     L01CF
	inx
L01CF:	jsr     mulax3
	jsr     tosaddax
	jsr     pushax
	ldx     #$00
	lda     #$03
	jsr     _memcpy
	ldy     #$00
	ldx     #$00
	lda     (sp),y
	clc
	adc     #$01
	sta     (sp),y
	jmp     L02C6
L01C4:	lda     __maxLine
	ldx     __maxLine+1
	sec
	sbc     #$01
	bcs     L01D1
	dex
L01D1:	sta     __maxLine
	stx     __maxLine+1
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	sta     __textPos+5
	jsr     _Invalidate
	lda     #$01
	jsr     _GetBufferFromEditorMemoryAndPrint
	jmp     incsp5

.endproc

; ---------------------------------------------------------------
; void __near__ InsertChar (unsigned char)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_InsertChar: near

.segment	"CODE"

	jsr     pusha
	jsr     decsp1
	lda     __textPos+2
	cmp     #$FF
	bcc     L02D2
	jmp     incsp2
L02D2:	cmp     __textPos+5
	bne     L01DB
	ldy     #$01
	lda     (sp),y
	ldy     __textPos+2
	sta     __lineBuffer,y
	inc     __textPos+2
	lda     __textPos+4
	cmp     __screenSize
	bcs     L02D1
	ldy     #$01
	lda     (sp),y
	jsr     _cputc
	inc     __textPos+4
	jmp     L02D1
L01DB:	jsr     decsp1
	lda     __textPos+5
	ldy     #$01
	sta     (sp),y
	cmp     #$FF
	bne     L02CE
	lda     (sp),y
	sec
	sbc     #$01
	sta     (sp),y
L02CE:	lda     (sp),y
	dey
	sta     (sp),y
	ldx     #$00
L02D0:	lda     (sp),y
	clc
	adc     #$01
	bcc     L02CC
	inx
	clc
L02CC:	adc     #<(__lineBuffer)
	tay
	txa
	adc     #>(__lineBuffer)
	tax
	tya
	jsr     pushax
	ldy     #$02
	lda     (sp),y
	tay
	lda     __lineBuffer,y
	ldy     #$00
	jsr     staspidx
	ldy     #$00
	lda     (sp),y
	cmp     __textPos+2
	beq     L01EE
	ldx     #$00
	lda     (sp),y
	sec
	sbc     #$01
	sta     (sp),y
	jmp     L02D0
L01EE:	ldy     #$02
	lda     (sp),y
	ldy     __textPos+2
	sta     __lineBuffer,y
	jsr     _LineBufferToCurrentScreenLine
	jsr     incsp1
L02D1:	inc     __textPos+5
	jsr     _CursorRight
	jmp     incsp2

.endproc

; ---------------------------------------------------------------
; void __near__ DeleteChar (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_DeleteChar: near

.segment	"CODE"

	jsr     decsp1
	ldx     #$00
	lda     __textPos+2
	cmp     __textPos+5
	bcs     L0204
	ldy     #$00
L02D6:	sta     (sp),y
	cmp     __textPos+5
	bcs     L02D5
	lda     #<(__lineBuffer)
	ldx     #>(__lineBuffer)
	clc
	adc     (sp),y
	bcc     L0210
	inx
L0210:	sta     sreg
	stx     sreg+1
	ldx     #$00
	lda     (sp),y
	clc
	adc     #$01
	bcc     L0213
	inx
L0213:	sta     ptr1
	txa
	clc
	adc     #>(__lineBuffer)
	sta     ptr1+1
	ldy     #<(__lineBuffer)
	lda     (ptr1),y
	ldy     #$00
	sta     (sreg),y
	ldx     #$00
	lda     (sp),y
	clc
	adc     #$01
	jmp     L02D6
L02D5:	lda     __textPos+5
	sec
	sbc     #$01
	bcs     L0216
	dex
L0216:	clc
	adc     #<(__lineBuffer)
	sta     ptr1
	txa
	adc     #>(__lineBuffer)
	sta     ptr1+1
	lda     #$20
	sta     (ptr1),y
	dec     __textPos+5
	jsr     _LineBufferToCurrentScreenLine
L0204:	jmp     incsp1

.endproc

; ---------------------------------------------------------------
; void __near__ Backspace (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_Backspace: near

.segment	"CODE"

	lda     __textPos+2
	beq     L021C
	jsr     _CursorLeft
	jmp     _DeleteChar
L021C:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ CursorUp (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_CursorUp: near

.segment	"CODE"

	jsr     _SaveBufferToEditorMemory
	lda     __textPos
	ldx     __textPos+1
	sec
	sbc     #$01
	bcs     L0224
	dex
L0224:	sta     __textPos
	stx     __textPos+1
	dec     __textPos+3
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	sta     __textPos+5
	tya
	jsr     _GetBufferFromEditorMemoryAndPrint
	lda     __textPos+4
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jmp     _gotoxy

.endproc

; ---------------------------------------------------------------
; void __near__ CursorDown (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_CursorDown: near

.segment	"CODE"

	lda     __maxLine
	ldx     __maxLine+1
	cpx     __textPos+1
	bne     L02DA
	cmp     __textPos
	beq     L022E
L02DA:	jsr     _SaveBufferToEditorMemory
	lda     __textPos
	ldx     __textPos+1
	clc
	adc     #$01
	bcc     L0233
	inx
L0233:	sta     __textPos
	stx     __textPos+1
	inc     __textPos+3
	lda     __editorLines
	ldx     __editorLines+1
	jsr     pushax
	lda     __textPos
	ldx     __textPos+1
	jsr     mulax3
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	sta     __textPos+5
	tya
	jsr     _GetBufferFromEditorMemoryAndPrint
	lda     __textPos+4
	jsr     pusha
	lda     __textPos+3
	clc
	adc     __screenSize+2
	jmp     _gotoxy
L022E:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ HandleReturnKey (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_HandleReturnKey: near

.segment	"CODE"

	lda     __maxLine
	ldx     __maxLine+1
	cpx     __textPos+1
	bne     L023E
	cmp     __textPos
	bne     L023E
	jsr     _SaveBufferToEditorMemory
	lda     __maxLine
	ldx     __maxLine+1
	clc
	adc     #$01
	bcc     L0242
	inx
L0242:	sta     __maxLine
	stx     __maxLine+1
	lda     __textPos
	ldx     __textPos+1
	clc
	adc     #$01
	bcc     L0244
	inx
L0244:	sta     __textPos
	stx     __textPos+1
	lda     #$00
	sta     __textPos+4
	sta     __textPos+2
	sta     __textPos+5
	lda     __textPos+3
	cmp     __screenSize+1
	bcs     L024B
	inc     __textPos+3
	jmp     _PrintLineNumber
L024B:	jsr     _Invalidate
	jmp     _PrintLineNumber
L023E:	lda     #$00
	sta     __textPos+4
	sta     __textPos+2
	jmp     _CursorDown

.endproc

; ---------------------------------------------------------------
; void __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

	jsr     decsp1
	jsr     _Initialize
	jsr     _PrintLineNumber
L025A:	jsr     decsp4
	lda     __textPos
	ldy     #$02
	sta     (sp),y
	iny
	lda     __textPos+1
	sta     (sp),y
	lda     __textPos+2
	ldy     #$00
	sta     (sp),y
	iny
	lda     #$00
	sta     (sp),y
	ldy     #$04
	lda     (sp),y
	jsr     _UpdateDocInfo
	jsr     _cgetc
	ldy     #$00
	sta     (sp),y
	cmp     #$0D
	bne     L02DD
	jsr     _HandleReturnKey
	jmp     L0287
L02DD:	lda     (sp),y
	cmp     #$14
	bne     L02DF
	jsr     _Backspace
	jmp     L0287
L02DF:	lda     (sp),y
	cmp     #$94
	bne     L02E1
	jsr     _DeleteChar
	jmp     L0287
L02E1:	lda     (sp),y
	cmp     #$05
	bne     L02E3
	lda     __textPos
	cmp     __maxLine
	lda     __textPos+1
	sbc     __maxLine+1
	bcs     L02EC
	lda     __textPos
	ldx     __textPos+1
	jsr     _DeleteLine
	jmp     L0287
L02E3:	lda     (sp),y
	cmp     #$11
	bne     L02E5
	jsr     _CursorDown
	jmp     L0287
L02E5:	lda     (sp),y
	cmp     #$91
	bne     L02E7
	lda     __textPos+3
	beq     L02EC
	jsr     _CursorUp
	jmp     L0287
L02E7:	lda     (sp),y
	cmp     #$9D
	bne     L02E9
	jsr     _CursorLeft
	jmp     L0287
L02E9:	lda     (sp),y
	cmp     #$1D
	bne     L02EA
	jsr     _CursorRight
	jmp     L0287
L02EA:	lda     (sp),y
	jsr     _InsertChar
L0287:	ldy     #$00
L02EC:	lda     (sp),y
	cmp     #$03
	jne     L025A
	jmp     incsp1

.endproc
