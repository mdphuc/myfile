; A boot sector that enters 32 - bit protected mode.
[bits 16]
[org 0x7c00]

mov si, MSG_REAL_MODE

%include "print_16bit.asm"
call print_16bit

%include "gdt.asm"
%include "switch_to_x86.asm"
call switch_to_x86
jmp $

[bits 32]
BEGIN_PM:
  mov ebx , MSG_PROT_MODE
  %include "print_x86.asm"
  call print_x86

MSG_REAL_MODE:
  db "Start in 16 bit"

MSG_PROT_MODE:
  db 0ah, "Landed in x86"

times 510 - ( $ - $$ ) db 0
dw 0xaa55

