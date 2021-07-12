BITS 16
ORG 0x7C00

; 1st stage bootloader loaded by BIOS

; jump over data
jmp 0x0:RmS1Boot

; bootloader info saved for kernel
BlInfo:
BlInfo_Drive:
	db 0 ; drive number
BlInfo_VBE:
	dw 0x0101 ; VBE mode 640x480x8bpp
BlInfo_LFB:
	dd 0 ; LFB physical address

; include GDT here
%include "GDT.inc"

RmS1Boot:
	; setup realmode segments
	xor ax, ax
	mov es, ax
	mov ds, ax
	mov ss, ax
	
	; setup stack
	mov bp, 0x7C00
	mov sp, bp
	
	; save drive number
	mov [BlInfo_Drive], dl
	
	; Check if we are running on ancient cpu which will clear high bits in flags register.
	; In this case the cpu does not support protected mode nor cpuid so we cannot boot ;(
	pushf ; flags will get nuked so save it first
	mov ax, 0xF000 ; high bits to check
	; load flags from ax
	push ax
	popf
	; load ax back from flags
	pushf
	pop ax
	mov cl, 0x01 ; error id
	; were high bits preserved? (restore flags)
	and ax, 0xF000
	popf
	test ah, ah
	je RmError
	
	; basic cpu info = cpuid(eax=1)
	xor eax, eax
	inc eax
	cpuid
	
	mov cl, 0x02 ; SSE error id
	bt edx, 0x19 ; SSE bit - 0x19
	jnc RmError
	
	mov cl, 0x03 ; SSE2 error id
	bt edx, 0x1A ; SSE2 bit - 0x19
	jnc RmError
	
	; get LFB physical address
	sub sp, 0x100 ; alloc space in stack for VBE info
	mov ax, 0x4F01 ; BIOS VBE
	mov cx, [BlInfo_VBE]
	mov di, sp
	int 0x10
	; bios will return some info onto stack
	movzx esp, sp
	mov eax, [esp+0x28]
	add sp, 0x100 ; free allocated buffer
	mov cl, 0x04 ; error id, then check if we got LFB
	test eax, eax
	je RmError
	mov [BlInfo_LFB], eax ; save LFB
	
	; disable interrupts, enable a20 (fast method) TODO: fix
	cli
	in al, 0x92
	or al, 0x02
	out 0x92, al
	
	; load GDT
	lgdt [GDT]
	
	; enable PM
	mov eax, cr0
	or al, 0x01
	mov cr0, eax
	
	; switch to PM
	jmp SELECTOR_UNREAL_CODE:PmS1Boot
	
PmS1Boot:
	; setup segments
	mov dx, SELECTOR_UNREAL_DATA
	mov ds, dx
	mov es, dx
	mov ss, dx
	mov fs, dx
	mov gs, dx
	
	; disable PM
	and al, 0xFE
	mov cr0, eax
	
	; switch back
	jmp 0x0:UrS1Boot
	
UrS1Boot:
	; setup segments
	xor dx, dx
	mov ds, dx
	mov es, dx
	mov ss, dx
	mov fs, dx
	mov gs, dx
	
	; enable interrupts
	sti
	
	; TODO: load 2nd stage bootloader @ 0x7E00
	
	; jump to 2nd stage
	jmp RmS2Boot
	
; In case of an error convert error id to char and print it, then halt cpu
RmError:
	add cl, 0x30
	mov [0xB800], cl
	cli
	hlt

; Error codes:
; 1 = cpu does not support 32bit mode
; 2 = no SSE supported
; 3 = no SSE2 supported
; 4 = LFB not supported
; 5 = disk read error

times 0x1FE - ($ - $$) db 0x00
dw 0xAA55

RmS2Boot:
	
	; TODO: load kernel @ 0x400000
	
	; set VBE mode
	mov ax, 0x4F02
	mov bx, [BlInfo_VBE]
	int 0x10
	
	; disable interrupts and enable PM
	cli
	mov eax, cr0
	or al, 0x01
	mov cr0, eax
	
	; switch to PM 32bit
	jmp SELECTOR_PMODE_CODE:PmS2Boot
	
BITS 32
PmS2Boot:
	
	; setup 32bit segments
	mov ax, SELECTOR_PMODE_DATA
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	; setup stack
	mov ebp, 0x400000
	mov esp, ebp
	
	; call kernel
	lea ecx,[BlInfo]
	jmp SELECTOR_PMODE_CODE:0x400000
