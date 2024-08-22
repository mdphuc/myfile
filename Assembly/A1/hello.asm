extern CreateFileA
extern MessageBoxA
extern ExitProcess
extern OpenFile

section .rodata
    stdout_query equ -11

section .data
    stdout dw 0
    bytes_written dw 0
    msg db "Hello Worldddddddddddd!", 0
    msg_len equ $ - msg
    filename  db "yo.txt", 0
    filename2 db "yo2.txt", 0


section .text

global main

main:
    sub rsp, 8
    sub rsp, 64

    mov rcx, filename2
    mov rdx, 1
    mov r8, 0
    xor r9, r9
    push 2
    push 0x80
    push 0
    call CreateFileA

    pop rbx
    pop rcx
    pop rdx

    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    
    xor rcx, rcx
    lea rdx, [rel msg]
    lea r8, [rel msg]
    mov r9, 0
    call MessageBoxA

    xor rcx, rcx
    call ExitProcess