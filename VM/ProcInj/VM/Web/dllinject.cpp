#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <psapi.h>

using namespace std;

BOOL CompareCHAR(CHAR *char1, CHAR *char2){
    int i = 0;
    while(char1[i] != '\0' && char2[i] != '\0'){
        if (char1[i] != char2[i]){
            return false;
        }
        i ++;
    }
    if ((char1[i] == '\0' && char2[i] != '\0') || (char1[i] != '\0' && char2[i] == '\0')){
        return false;
    }
    return true;
}

DWORD GetProcessID(CHAR *process_name){
    HANDLE process_snap;
    PROCESSENTRY32 pe32;
    HANDLE handle_process;

    process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    Process32First(process_snap, &pe32);

    do{
        if (CompareCHAR((CHAR*)pe32.szExeFile, process_name)){
            return pe32.th32ProcessID;
        }

        // cout << pe32.szExeFile << " " << process_name << endl;
    }while(Process32Next(process_snap, &pe32));
    return 0;
}

BOOL Inject(TCHAR *dll_to_inject, DWORD process_id_inject_to){
    TCHAR dll_path[MAX_PATH];

    GetFullPathName(dll_to_inject, MAX_PATH, dll_path, NULL);

    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id_inject_to);
    // HANDLE hfile = CreateFileA((LPCSTR)dll_path, GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    // DWORD dllfilesize = GetFileSize(hfile, NULL);

    LPVOID dllalloc = VirtualAllocEx(hproc, NULL, strlen((CHAR*)dll_path), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    WriteProcessMemory(hproc, dllalloc, dll_path, strlen((CHAR*)dll_path), NULL);

    FARPROC loadlibraryA_addr = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");

    HANDLE rthread = CreateRemoteThread(hproc, NULL, 0, (LPTHREAD_START_ROUTINE)loadlibraryA_addr, dllalloc, 0, NULL);
    WaitForSingleObject(rthread, INFINITE);

    return true;
}

int main(){
    TCHAR dll_to_inject[MAX_PATH] = TEXT("C://ProgramData/ProcInj/Web/compiled/testdll.dll");

    CHAR process_to_get_id[] = "notepad.exe";

    DWORD pid = GetProcessID(process_to_get_id);

    if (pid == 0){
        cout << "Cannot find id of target process";
        return 1;
    }

    cout << pid << endl;
    cout << Inject(dll_to_inject, pid);
}