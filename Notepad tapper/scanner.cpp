#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <stdio.h>
#include <stdint.h>

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

LPVOID GetProcessMemory(DWORD proc_id, char *pattern, size_t patternLen){
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

  return base;
}

int main(int argc, char **argv){
  LPCSTR target = "Notepad.exe";
  DWORD proc_id = GetProcessIdFromName(target);
  cout << proc_id << endl;
  cout << "-----------------------------------" << endl;

  size_t patternLen = strlen(argv[1]); 
  char *pattern = new char[patternLen * 2];

  for (int i = 0; i < patternLen; i++){
    pattern[2*i] = argv[1][i];
    pattern[2*i + 1] = 0x0;
  }

  cout << endl;
  cout << GetProcessMemory(proc_id, pattern, patternLen * 2);
}