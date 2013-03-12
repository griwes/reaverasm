;
; Test #1 - mov instruction
; bits - 16, 32, 64
;

%define foo(a, b, c) a + b + c

bits    16

mov     ax, bx
mov     ss, cx
mov     ah, 1

; mov     ss, 1   ; fail test
mov     eax, dword [0]
mov     ax, 0xb800
mov     es, ax
mov     ecx, dword [es:0]

bits    32

mov     eax, cr0
mov     [0xb8000], edx

bits    64

mov     rsp, 0
mov     eax, 0
mov     qword [0], rax

lea     rax, [foo(rbx, rcx, 2)]
