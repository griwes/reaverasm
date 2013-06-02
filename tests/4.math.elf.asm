global _start
extern printf
extern exit

bits    64

section .data

format:     db "%d", 0x0a, 0

section .text

_start:
    mov     eax, 2
    mov     ebx, 100
    mul     ebx
    lea     esi, [eax + ebx + 10]
    mov     edi, format
    call    printf

    mov     eax, 1
    mov     ebx, 0
    call    exit
