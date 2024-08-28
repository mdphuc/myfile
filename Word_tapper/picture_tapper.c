#include <windows.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <gdiplus/gdiplus.h>
#include <gdiplus/gdiplusflat.h> 
#include <time.h>

__declspec(dllexport) BYTE *addr;
__declspec(dllexport) BYTE originalBytes[12] = {0};

__declspec(dllexport) GpStatus Picture(WCHAR* filename, GpImage** image);

__declspec(dllexport) bool DeleteHook(){
    DWORD original_protection;

    VirtualProtect(addr, 12, PAGE_EXECUTE_READWRITE, &original_protection);

    for (int i = 0; i < 12; i++){
        *(DWORD64*)(addr + i) = *(DWORD64*)(originalBytes + i) ;
    }

    VirtualProtect(addr, 12, original_protection, &original_protection);

    return true;
}

__declspec(dllexport) bool SetHookPicture(){
    DWORD original_protection;

    VirtualProtect(addr, 12, PAGE_EXECUTE_READWRITE, &original_protection);

    for (int i = 0; i < 12; i++){
        *(DWORD64*)(originalBytes + i) = *(DWORD64*)(addr + i);
    }

    *(BYTE *)(addr) = 0x48;
    *(BYTE *)(addr + 1) = 0xB8; //mov rax, ...
    *(DWORD64 *)(addr + 2) = Picture;
    *(BYTE *)(addr + 10) = 0xFF;
    *(BYTE *)(addr + 11) = 0xE0; // jmp rax

    VirtualProtect(addr, 12, original_protection, &original_protection);

    return true; 
}

__declspec(dllexport) GpStatus Picture(WCHAR* filename, GpImage** image){
    DeleteHook();
    // char fn[MAX_PATH];
    // sprintf_s(fn, sizeof(fn), "%ws", filename);

    system("start C:/Users/Phuc/Desktop/Word_tapper/capture_image.exe");

    Sleep(1000);

    // MessageBoxA(NULL, fn, "world", MB_OK);

    WCHAR *f = L"C:/Users/Phuc/Desktop/Word_tapper/Screenshot.jpg";

    GpStatus p = GdipLoadImageFromFile(f, image);
    SetHookPicture();
    return p;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved){
    switch(fdwReason){ 
        case DLL_PROCESS_ATTACH:
            // mbox(L"Run");
            // addr = (BYTE *)GetProcAddress(GetModuleHandleA("user32.dll"), "GetClipboardData");
            addr = (BYTE *)GetProcAddress(GetModuleHandleA("gdiplus.dll"), "GdipLoadImageFromFile");
            SetHookPicture();

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