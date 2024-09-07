#include <iostream>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <stdio.h> 
#include <stdint.h>
#include <ctime>
#include <fstream>
#include <thread> 
// #include "./game_logic/game_function.h"
#include "./spinning donut/sd.h"

using namespace std;

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

char* FindPattern(char* src, size_t srcLen, const char* pattern, size_t patternLen){
	char* cur = src;
	size_t curPos = 0;

	while (curPos < srcLen){
		if (memcmp(cur, pattern, patternLen) == 0){
			return cur;
		}

		curPos++;
		cur = &src[curPos];
	}
	return nullptr;
}

char* GetProcessMemory(DWORD proc_id, char *pattern, size_t patternLen){
  HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, proc_id);
  LPVOID base = 0x0;
  MEMORY_BASIC_INFORMATION memInfo;
  size_t bytesread;

  int skip = 0;

  while (VirtualQueryEx(hproc, base, &memInfo, sizeof(memInfo))){
    if (skip > 4){
      if (memInfo.State == MEM_COMMIT && memInfo.Protect == PAGE_READWRITE){
        char *lpBuffer = (char *)malloc(memInfo.RegionSize);

        ReadProcessMemory(hproc, base, lpBuffer, memInfo.RegionSize, NULL);
        char* match = FindPattern(lpBuffer, memInfo.RegionSize, pattern, patternLen);
        
        if (match){
            uint64_t diff = (uint64_t)match - (uint64_t)(lpBuffer);
            char* processPtr = (char *)base + diff;
            return processPtr;
        }
      }
    }

    skip ++;
    base += memInfo.RegionSize;
  }

  return nullptr;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam){
    char *buffer = new char[MAX_PATH];
    GetWindowTextA(hwnd, buffer, MAX_PATH);

    string windows_title(buffer);

    if (windows_title == "*Untitled - Notepad" || windows_title == "Untitled - Notepad"){
        notepad_hwnd = hwnd;
        return false;
    }
    return true;
}

char* UTF16Convert(char* target){
    size_t patternLen = strlen(target); 
    char *pattern = new char[patternLen * 2];

    for (int i = 0; i < patternLen; i++){
        pattern[2*i] = target[i];
        pattern[2*i + 1] = 0x0;
    }
    return pattern;
}

bool UpdateProcessMemory(DWORD proc_id, LPVOID base, size_t size){
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, proc_id);
    MEMORY_BASIC_INFORMATION memInfo;

    if(!WriteProcessMemory(hproc, base, buffer, size, NULL)){
        return false;
    }

    RECT r;
    GetClientRect(notepad_hwnd, &r);
    InvalidateRect(notepad_hwnd, &r, false);
    return true;
}

void GetWindowsInfo(){
    EnumWindows(EnumWindowsProc, 0);
    RECT r;
    GetWindowRect(notepad_hwnd, &r);
    height = r.bottom - r.top;
    width = r.right - r.left;
}

void Prepare(){
    GetWindowsInfo();
    cout << "width = " << width << endl;
    cout << "height = " << height << endl;

    MoveWindow(notepad_hwnd, offsetX, offsetY, 1058, 727, true);

    for (int i = 0; i < charCount; i++){
        // char v = 0x41 + char(rand() % 26);
        char v = 0x2e;
 
        buffer[2*i] = v;
        buffer[2*i + 1] = 0x0;
        PostMessage(FindWindowExA(notepad_hwnd, NULL, "Edit", NULL), WM_CHAR, v, 0);
    }

    Sleep(5000);
}

void Clear(DWORD proc_id, LPVOID base){
  HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, proc_id); 

  for (int i = 0; i < charCount; i++){
    buffer[2*i] = 0x0;
    buffer[2*i + 1] = 0x0;
  }
  WriteProcessMemory(hproc, base, buffer, bufferSize, NULL);

  RECT r;
  GetClientRect(notepad_hwnd, &r);
  InvalidateRect(notepad_hwnd, &r, false);
}

void InstallHook(DWORD thread_id){
  TCHAR dll_path[MAX_PATH];

  GetFullPathName(TEXT("./keyboard_dll.dll"), MAX_PATH, dll_path, NULL);

  cout << dll_path << endl;

  HMODULE hooklib = LoadLibraryA((LPCSTR)dll_path);
  HOOKPROC hookfunc = (HOOKPROC)GetProcAddress(hooklib, "HookProcedure");

  HHOOK keyboardhook = SetWindowsHookExA(WH_KEYBOARD, hookfunc, hooklib, thread_id);

  cout << "Run" << endl;
  while(GetMessage(NULL, NULL, WM_KEYFIRST, WM_KEYLAST)){}

  UnhookWindowsHookEx(keyboardhook);
}

void CleanUp(){
  cout << "Cleaning up..." << endl;
  try{
    system("powershell.exe Stop-Process -Name 'notepad'");
    throw -1;
  }catch(int e){
    cout << "Error: " << e << endl;
  }

  cout << "Finished closing" << endl;

  PROCESS_INFORMATION ProcessInfo;

  STARTUPINFO StartupInfo;

  ZeroMemory(&StartupInfo, sizeof(StartupInfo));
  StartupInfo.cb = sizeof StartupInfo ;

  if(CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOA)&StartupInfo, &ProcessInfo)){ 
    WaitForSingleObject(ProcessInfo.hProcess, 2000);
    CloseHandle(ProcessInfo.hThread);
    CloseHandle(ProcessInfo.hProcess);
  }  
  cout << "Succeed!" << endl;
}

int main(){
    CleanUp();
    Prepare();
    cout << "Finish pre-run process" << endl;

    LPCSTR target = "notepad.exe";
    DWORD proc_id = GetProcessIdFromName(target);
    cout << "proc_id = " << proc_id << endl;
    cout << "-----------------------------------" << endl;

    char *pattern = buffer;

    cout << endl;
    LPVOID found_base = GetProcessMemory(proc_id, pattern, bufferSize);
    cout << "base = " << found_base << endl;

    DWORD thread_id = GetThreadIDFromProcID(proc_id);
    cout << "thread_id = " << thread_id << endl;
    cout << "-----------------------------------" << endl;

    notepad_thread = OpenThread(THREAD_ALL_ACCESS, false, thread_id);

    // Launch(proc_id, found_base, thread_id);

    Start(proc_id, found_base);
    
}