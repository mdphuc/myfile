#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    WSAStartup(MAKEWORD(2,2), &wsaData);

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    freeaddrinfo(result);

    listen(ListenSocket, 1);

    do {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        int err = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (err > 0) {
            string output(recvbuf);
            cout << stoi(output.substr(0, 2)) << endl;
        }
    } while (true);

    shutdown(ClientSocket, SD_SEND);

    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
