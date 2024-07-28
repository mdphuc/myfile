ORG 0x7C00
BITS 16

section .data
  msg db "Hello World"
  len equ $ - msg

section .text
  global _start

_start:
  mov eax, 4
  mov ebx, 1
  mov ecx, msg
  mov edx, len
  int 0x80

  mov eax, 1
  int 0x80
  HLT
  jmp _start

TIMES 510-($-$$) DB 0
DW 0AA5h