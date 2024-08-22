extern MessageBoxA
extern ExitProcess

section .data
    msg db "Hello Worldddddddddddd!", 0
    msg_len equ $ - msg

section .text

global main

main:
  sub rsp, 8
  sub rsp, 64

  xor rcx, rcx
  lea rdx, [rel msg]
  lea r8, [rel msg]
  mov r9, 0
  call MessageBoxA

  xor rcx, rcx
  call ExitProcess