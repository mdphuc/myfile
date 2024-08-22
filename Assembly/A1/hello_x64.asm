extern MessageBoxA                              ; Import external symbols
extern ExitProcess                              ; Windows API functions, not decorated

global main                                    ; Export symbols. The entry point

section .data                                   ; Initialized data segment
 MessageBoxText    db "Do you want to exit?", 0
 MessageBoxCaption db "MessageBox 64", 0

section .text                                   ; Code segment
main:
 sub   RSP, 8                                   ; Align the stack to a multiple of 16 bytes
 sub   RSP, 64                                  ; 32 bytes of shadow space

 xor   rcx, rcx                                 ; 1st parameter
 lea   RDX, [REL MessageBoxText]                ; 2nd parameter
 lea   R8, [REL MessageBoxCaption]              ; 3rd parameter
 mov   R9D, 0            ; 4th parameter. 2 constants ORed together
 call  MessageBoxA

 add   RSP, 32                                  ; Remove the 32 bytes

 xor   ECX, ECX
 call  ExitProcess