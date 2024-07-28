[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_x86:
    pusha
    mov edx, VIDEO_MEMORY  

print_x86_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je print_x86_done

    mov [edx], ax
    add ebx, 1
    add edx, 2

    jmp print_x86_loop

print_x86_done:
    popa
    ret