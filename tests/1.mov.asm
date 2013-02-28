;
; Test #1 - mov instruction
; bits - 16, 32, 64
;

bits    16

mov     ax, bx
mov     ss, cx
mov     ah, 1

; mov     ss, 1   ; fail test
mov     eax, dword [0]
mov     ecx, [0xb8000]  ; size unnecessary in RASM; it's defaulted to register size

bits    32

mov     eax, cr0
mov     [0xb8000], edx

bits    64

mov     rsp, 0
mov     eax, 0
mov     qword [0], rax
