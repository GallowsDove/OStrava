BITS 16
ORG 0x7C00

push 0xb800
pop ds

mov al,0x41
mov [0],al

cli
hlt
jmp $

times 510 - ($-$$) db 0
dw 0xaa55
