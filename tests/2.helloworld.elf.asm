bits    32

text:   db "Hello world!", 0x0a

global _start

_start:
    mov     eax, 4
    mov     ebx, 1
    mov     ecx, text
    mov     edx, 13
    int     0x80

    mov     eax, 1
    mov     ebx, 0
    int     0x80
