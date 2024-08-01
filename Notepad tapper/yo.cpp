#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <ctime>
#include <fstream>

using namespace std;

HWND notepad_hwnd;
DWORD width, height;
size_t colCount = 127;
size_t rowCount = 35;
size_t charCount = colCount * rowCount;
size_t bufferSize = charCount * 2;
int offsetX = 200;
int offsetY = 100;
char *buffer = (char *)malloc(bufferSize);

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
    // cout << endl << endl;

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

    // char *payload = UTF16Convert((char *)target.c_str());

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
        char v = 0x23;

        buffer[2*i] = v;
        buffer[2*i + 1] = 0x0;
        PostMessage(FindWindowEx(notepad_hwnd, NULL, "Edit", NULL), WM_CHAR, v, 0);
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

void Display(DWORD proc_id, LPVOID base){
  for (int i = 0; i < bufferSize; i++){
    if (i >= 2){
      buffer[2*(i - 1)] = 0x23;
      buffer[2*i] = 0x2e;
    }else{
      buffer[2*i] = 0x2e;
    }
    UpdateProcessMemory(proc_id, base, bufferSize);
    Sleep(300);
  }
}

int main(int argc, char **argv){
    Prepare();
    cout << "Finish pre-run process" << endl;

    LPCSTR target = "notepad.exe";
    DWORD proc_id = GetProcessIdFromName(target);
    cout << proc_id << endl;
    cout << "-----------------------------------" << endl;

    char *pattern = buffer;

    cout << endl;
    LPVOID found_base = GetProcessMemory(proc_id, pattern, bufferSize);
    cout << found_base << endl;
    
    Display(proc_id, found_base);

}
