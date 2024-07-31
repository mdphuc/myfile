bits 16
org 0x7c00

print_16bit:
  ; mov si, message1
  mov ah, 0x0e

.loop:
  lodsb
  cmp al, 0
  je halt
  int 0x10
  jmp .loop

halt:
  hlt


