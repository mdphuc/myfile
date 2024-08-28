#include <windows.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <gdiplus/gdiplus.h>
#include <gdiplus/gdiplusflat.h> 
#include <time.h>

__declspec(dllexport) BYTE *addr;
__declspec(dllexport) BYTE originalBytes[12] = {0};

__declspec(dllexport) GpStatus PaintColor(GpBrush* brush, ARGB color);

__declspec(dllexport) bool DeleteHook(){
    DWORD original_protection;

    VirtualProtect(addr, 12, PAGE_EXECUTE_READWRITE, &original_protection);

    for (int i = 0; i < 12; i++){
        *(DWORD64*)(addr + i) = *(DWORD64*)(originalBytes + i) ;
    }

    VirtualProtect(addr, 12, original_protection, &original_protection);

    return true;
}

__declspec(dllexport) bool SetHookPaintColor(){
    DWORD original_protection;

    VirtualProtect(addr, 12, PAGE_EXECUTE_READWRITE, &original_protection);

    for (int i = 0; i < 12; i++){
        *(DWORD64*)(originalBytes + i) = *(DWORD64*)(addr + i);
    }

    *(BYTE *)(addr) = 0x48;
    *(BYTE *)(addr + 1) = 0xB8; //mov rax, ...
    *(DWORD64 *)(addr + 2) = PaintColor;
    *(BYTE *)(addr + 10) = 0xFF;
    *(BYTE *)(addr + 11) = 0xE0; // jmp rax

    VirtualProtect(addr, 12, original_protection, &original_protection);

    return true; 
}


__declspec(dllexport) GpStatus PaintColor(GpBrush* brush, ARGB color){
    DeleteHook();
    // MessageBoxA(NULL, "hello", "world", MB_OK);
    // GdipCreatePen1(4286611584, 3, unit, pen);
    GpStatus p = GdipSetSolidFillColor(brush, 4293729316);
    SetHookPaintColor();
    return p;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved){
    switch(fdwReason){ 
        case DLL_PROCESS_ATTACH:
            // mbox(L"Run");
            // addr = (BYTE *)GetProcAddress(GetModuleHandleA("user32.dll"), "GetClipboardData");
            addr = (BYTE *)GetProcAddress(GetModuleHandleA("gdiplus.dll"), "GdipSetSolidFillColor");
            SetHookPaintColor();

            // addr = (BYTE *)GetProcAddress(GetModuleHandleA("user32.dll"), "SetClipboardData");
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;

}