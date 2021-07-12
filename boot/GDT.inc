GDT:
	dq 0

SELECTOR_UNREAL_CODE equ ($ - GDT)
	dw 0xFFFF			; Limit 0 - 15 bits
	dw 0x0000			; Base 0 - 15 bits (low)
	db 0x00				; Base 16 - 23 bits (middle)
	db 10011010b		; Access: 1, ring level 2bits (0 = kernel 3 = user), 1 = segment (0 = TSS), execute, 0 = segment grows up (1 = down), read/write, is accessed
	db 10001111b		; Granularity: 1 = 4KiB page (0 = 1B page), 1 = 32 - bit PM (0 = 16bit PM), 0, 0, limit 16 - 19 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x00				; Base 24 - 31 bits (high)

SELECTOR_UNREAL_DATA equ ($ - GDT)
	dw 0xFFFF			; Limit 0 - 15 bits
	dw 0x0000			; Base 0 - 15 bits (low)
	db 0x00				; Base 16 - 23 bits (middle)
	db 10010010b		; Access: 1, ring level 2bits (0 = kernel 3 = user), 1 = segment (0 = TSS), execute, 0 = segment grows up (1 = down), read/write, is accessed
	db 10001111b		; Granularity: 1 = 4KiB page (0 = 1B page), 1 = 32 - bit PM (0 = 16bit PM), 0, 0, limit 16 - 19 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x00				; Base 24 - 31 bits (high)

SELECTOR_PMODE_CODE equ ($ - GDT)
	dw 0xFFFF			; Limit 0 - 15 bits
	dw 0x0000			; Base 0 - 15 bits (low)
	db 0x00				; Base 16 - 23 bits (middle)
	db 10011010b		; Access: 1, ring level 2bits (0 = kernel 3 = user), 1 = segment (0 = TSS), execute, 0 = segment grows up (1 = down), read/write, is accessed
	db 11001111b		; Granularity: 1 = 4KiB page (0 = 1B page), 1 = 32 - bit PM (0 = 16bit PM), 0, 0, limit 16 - 19 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x00				; Base 24 - 31 bits (high)

SELECTOR_PMODE_DATA equ ($ - GDT)
	dw 0xFFFF			; Limit 0 - 15 bits
	dw 0x0000			; Base 0 - 15 bits (low)
	db 0x00				; Base 16 - 23 bits (middle)
	db 10010010b		; Access: 1, ring level 2bits (0 = kernel 3 = user), 1 = segment (0 = TSS), execute, 0 = segment grows up (1 = down), read/write, is accessed
	db 11001111b		; Granularity: 1 = 4KiB page (0 = 1B page), 1 = 32 - bit PM (0 = 16bit PM), 0, 0, limit 16 - 19 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x00				; Base 24 - 31 bits (high)

GDTReg:
	dw $ - GDT - 1	; GDT size
	dd GDT			; GDT physical address
