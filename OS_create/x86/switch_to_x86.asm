[bits 16]
switch_to_x86:
    cli
    lgdt [gdt_descriptor] 
    mov eax, cr0
    or eax, 0x1 
    mov cr0, eax
    jmp CODE_SEG:init_x86

[bits 32]
init_x86:
    mov ax, DATA_SEG 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 
    mov esp, ebp

    call BEGIN_PM 

