;
; Test #1 - mov instruction
; bits - 16, 32, 64
;

bits    16

mov     ax, bx
mov     ss, cx
mov     ah, 1

; mov     ss, 1   ; fail test
mov     ax, [0]
mov     ax, 0xb800
mov     es, ax
mov     ecx, [es:0]

mov     ax, [eax + ebx + 5] ; SIB ain't generated

bits    32

mov     eax, cr0
mov     [0xb8000], edx

bits    64

mov     rsp, 0
mov     eax, 0
