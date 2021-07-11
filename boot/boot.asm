BITS 16
ORG 0x7C00

; 1st stage bootloader loaded by BIOS

; jump over data
jmp 0x0:RmS1Boot

; bootloader info saved for kernel
BlInfo_Drive:
  db 0 ; drive number
BlInfo_VBE:
  dw 0x0101 ; VBE mode 640x480x8bpp
BlInfo_GDT:
  dd 0 ; GDT physical address
BlInfo_LFB:
  dd 0 ; LFB physical address

RmS1Boot:
  ; setup realmode segments
  xor ax,ax
  mov es,ax
  mov ds,ax
  mov ss,ax
  
  ; setup stack
  mov bp,0x7C00
  mov sp,bp
  
  ; save drive number
  mov [BlInfo_Drive],dl
  
  ; Check if we are running on ancient cpu which will clear high bits in flags register.
  ; In this case the cpu does not support protected mode nor cpuid so we cannot boot ;(
  pushf ; flags will get nuked so save it first
  mov ax,0xF000 ; high bits to check
  ; load flags from ax
  push ax
  popf
  ; load ax back from flags
  pushf
  pop ax
  mov cl,0x01 ; error id
  ; were high bits preserved? (restore flags)
  and ax,0xF000
  popf
  test ah,ah
  je RmError
  
  ; basic cpu info = cpuid(eax=1)
  xor eax,eax
  inc eax
  cpuid
  
  mov cl,0x02 ; SSE error id
  bt edx,0x19 ; SSE bit - 0x19
  jnc RmError
  
  mov cl,0x03 ; SSE2 error id
  bt edx,0x1A ; SSE2 bit - 0x19
  jnc RmError
  
  ; get LFB physical address
  sub sp,0x100 ; alloc space in stack for VBE info
  mov ax,0x4F01 ; BIOS VBE
  mov cx,[BlInfo_VBE]
  mov di,sp
  int 0x10
  movzx esp,sp
  mov eax,[esp+0x28] ; LFB @offset 0x25
  add sp,0x100 ; free allocated buffer
  mov [BlInfo_LFB],eax ; save LFB
  
  ; TODO

; In case of an error convert error id to char and print it, then halt cpu
RmError:
  add cl,0x30
  mov [0xB800],cl
  cli
  hlt
  jmp $

; Error codes:
; 1 = cpu does not support 32bit mode
; 2 = no SSE supported
; 3 = no SSE2 supported
; 4 = disk read error

times 0x1FE - ($ - $$) db 0x00
dw 0xAA55
