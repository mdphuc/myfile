extern GetStdHandle
; extern WriteFile
extern MessageBoxA
extern ExitProcess

section .rodata
    stdout_query equ -11

section .data
    stdout dw 0
    bytes_written dw 0
    msg db "Hello World!", 0
    msg_len equ $ - msg

section .text

global main

main:
    ; mov rcx, stdout_query
    ; call GetStdHandle

    ; mov  rcx, rax
    ; mov  rdx, msg
    ; mov  r8, msg_len
    ; mov  r9, bytes_written
    ; push 0
    ; call WriteFile

    mov rcx, 0
    xor rcx, rcx
    lea rdx, msg
    lea r8, msg
    mov r9d, 0
    call MessageBoxA

    xor rcx, rcx
    call ExitProcess