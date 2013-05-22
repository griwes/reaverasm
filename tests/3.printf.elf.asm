global _start
extern printf
extern exit

bits    32

format:     db "%d", 0x0a, 0

_start:
    mov     esi, 10
    mov     edi, format
    call    printf

    mov     eax, 1
    mov     ebx, 0
    call    exit
