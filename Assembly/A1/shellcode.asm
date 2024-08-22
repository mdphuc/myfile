section .rodata
    msg:    db 'hello, world', 10
    msglen: equ $-msg

section .text
global main
  main:
      ; write(1, msg, msglen)
      mov eax, 1
      mov ecx, msg
      mov edx, msglen
      mov ebx, 1
      int 80h