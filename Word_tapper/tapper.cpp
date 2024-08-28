#include <iostream>
// #include <ws2tcpip.h>
// #include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>  
#include <stdio.h> 
#include <stdint.h>
#include <ctime>
#include <fstream>
#include <thread> 
#include <random> 

using namespace std;

HWND word_hwnd;

// #pragma comment (lib, "Ws2_32.lib")

// #define DEFAULT_BUFLEN 512
// #define DEFAULT_PORT "27015"

DWORD GetProcessIdFromName(LPCSTR proc_name){
  PROCESSENTRY32 pe32;

  HANDLE proc_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  pe32.dwSize = sizeof(PROCESSENTRY32);

  Process32First(proc_snap, &pe32);

  do{
    if (strcmp((char *)pe32.szExeFile, proc_name) == 0){
      return pe32.th32ProcessID;
    }
  }while(Process32Next(proc_snap, &pe32));
  return 0;
}

DWORD GetThreadIDFromProcID(DWORD proc_id){
  THREADENTRY32 te32;

  HANDLE thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

  te32.dwSize = sizeof(THREADENTRY32);

  Thread32First(thread_snap, &te32);

  do{
    if(te32.th32OwnerProcessID == proc_id){
      return te32.th32ThreadID;
    }
  }while(Thread32Next(thread_snap, &te32));

  return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam){
    char *buffer = new char[MAX_PATH];
    GetWindowTextA(hwnd, buffer, MAX_PATH);

    string windows_title(buffer);

    // cout << buffer << endl;

    if (windows_title == "Document - WordPad" || windows_title == "*Document - WordPad"){
        word_hwnd = hwnd;
        return false;
    }
    return true;
}

// void InstallHook(DWORD thread_id){
//   TCHAR dll_path[MAX_PATH];

//   GetFullPathName(TEXT("./picture_tapper.dll"), MAX_PATH, dll_path, NULL);

//   cout << dll_path << endl;

//   HMODULE hooklib = LoadLibraryA((LPCSTR)dll_path);
//   HOOKPROC hookfunc = (HOOKPROC)GetProcAddress(hooklib, "HookProcedure");

//   HHOOK keyboardhook = SetWindowsHookExA(WH_KEYBOARD, hookfunc, hooklib, thread_id);

//   MSG msg = { };

//   cout << "Run" << endl;
//   while(GetMessage(&msg, word_hwnd, WM_KEYFIRST, WM_KEYLAST) > 0){
//     TranslateMessage(&msg);
//     DispatchMessage(&msg);
//   }

//   UnhookWindowsHookEx(keyboardhook);
// }

BOOL Inject(TCHAR *dll_to_inject, DWORD process_id_inject_to){
  TCHAR dll_path[MAX_PATH];

  GetFullPathName(dll_to_inject, MAX_PATH, dll_path, NULL);

  HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id_inject_to);

  LPVOID dllalloc = VirtualAllocEx(hproc, NULL, strlen((CHAR*)dll_path), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

  WriteProcessMemory(hproc, dllalloc, dll_path, strlen((CHAR*)dll_path), NULL);

  FARPROC loadlibraryA_addr = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");

  HANDLE rthread = CreateRemoteThread(hproc, NULL, 0, (LPTHREAD_START_ROUTINE)loadlibraryA_addr, dllalloc, 0, NULL);
  WaitForSingleObject(rthread, INFINITE);

  return true;
}


int main(){
  // EnumWindows(EnumWindowsProc, 0);

  CHAR process_to_get_id[] = "mspaint.exe";
  // TCHAR dll_to_inject[MAX_PATH] = TEXT("./picture_tapper.dll");
  TCHAR dll_to_inject[MAX_PATH] = TEXT("./paintcolor_tapper.dll");

  DWORD proc_id = GetProcessIdFromName(process_to_get_id);

  // cout << dll_to_inject << endl;
  cout << "proc_id = " << proc_id << endl;

  if (proc_id == 0){
    cout << "Cannot find process with that name!" << endl;
    return 1;
  }

  DWORD thread_id = GetThreadIDFromProcID(proc_id);
  cout << "thread_id = " << thread_id << endl;
  cout << "-----------------------------------" << endl;

  // cout << word_hwnd << endl;

  // PostMessage(FindWindowExA(word_hwnd, NULL, "Edit", NULL), WM_CHAR, 'f', 0);

  
  // PostThreadMessageA(thread_id, WM_CHAR, 'f', 0);

  // InstallHook(thread_id);


  Inject(dll_to_inject, proc_id);



  
}