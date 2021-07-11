# OStrava
An experimental Operating System written in Assembly and C.

# Features
...

# Compiling
...

## TODO
- 1st stage bootloader (cpu check, load 2nd stage)
- 2nd stage bootloader (create LFB, load kernel, a20, GDT, PM 32bit, relocate)
- kernel init (paging & remap, IDT, TSS, ...)
- kernel (init drivers, mm, io, ioctl, ...)
- km drivers (kb, mouse, cmos, pci, ...)
- usermode
- um apps
