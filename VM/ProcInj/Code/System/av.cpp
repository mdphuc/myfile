#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <algorithm> 
#include <psapi.h>
#include <fstream>
#include <vector>
#include <ctime>
#include <sstream>

using namespace std;

vector<string>whitelist;

int GetFileSize(string filename){
    ifstream InFile(filename.c_str(), ios::binary);
    InFile.seekg(0, ios::end);

    int file_size = InFile.tellg();
    

    return file_size;
}

vector<string> RemoveDuplicate(vector<string> target){
    sort(target.begin(), target.end());

    auto it = unique(target.begin(), target.end()); 
  
    target.erase(it, target.end()); 

    return target;
}

vector <int> GetProcessID(string proc_name){
    HANDLE process_snap;
    PROCESSENTRY32 pe32;
    HANDLE handle_process;

    vector <int> pid;

    process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    do{
        if (strcmp((char *)pe32.szExeFile, proc_name.c_str()) == 0){
            pid.push_back(pe32.th32ProcessID);
        }
    }while(Process32Next(process_snap, &pe32));

    return pid;
}

void Hide(){
    HWND hWnd = GetConsoleWindow();
    ShowWindow( hWnd, 0 );
}

int GetProcess(vector<string> &proc, int state){
    HANDLE process_snap;
    PROCESSENTRY32 pe32;
    HANDLE handle_process;

    ofstream OutFile;

    if (state == 0){
        OutFile.open("C://Users/ProcInj/Desktop/whitelist.txt");
    }else{
        OutFile.open("C://Users/ProcInj/Desktop/proc.txt");
    }

    process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    do{ 
        OutFile << pe32.szExeFile << endl;

    }while(Process32Next(process_snap, &pe32));

    OutFile << "python.exe" << endl;
    OutFile << "tasklist.exe" << endl;
    OutFile << "cmd.exe" << endl;

    CloseHandle(process_snap);

    ifstream InFile;
    
    if (state == 0){
        InFile.open("C://Users/ProcInj/Desktop/whitelist.txt");
    }else{
        InFile.open("C://Users/ProcInj/Desktop/proc.txt");
    }

    string exe;

    while (getline(InFile, exe)){
        proc.push_back(exe);
    }

    proc = RemoveDuplicate(proc);

    InFile.close();
    return 0;
}

vector<string> CheckProcess(){
    vector<string> current_procs;
    vector<string> block_procs;

    GetProcess(current_procs, 1);

    for (int i = 0; i < current_procs.size(); i++){
        bool diff = true;
        for (int j = 0; j < whitelist.size(); j++){
            if (current_procs[i] == whitelist[j]){
                diff = false;
                break;
            }
        }
        if (diff == true){
            block_procs.push_back(current_procs[i]);
        }
    }

    block_procs.pop_back();

    return block_procs;
}

DWORD GetThreadID(DWORD process_id){
    HANDLE thread_snap;
    THREADENTRY32 te32;
    HANDLE handle_thread;

    thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    te32.dwSize = sizeof(THREADENTRY32);

    Thread32First(thread_snap, &te32);

    do{
        if (te32.th32OwnerProcessID == process_id){
            return te32.th32ThreadID;
        }
    }while(Thread32Next(thread_snap, &te32));

    return 0;
}

string TCHARToString(TCHAR *target){
    string target_str;
    for (int i = 0; i < strlen((char *)target); i++){
        target_str += target[i];
    }
    return target_str;
}

string SplitString(string str, char token){
    stringstream ss(str); 
  
    // Tokenize the input string by comma delimiter 
    string lastone; 
  
    while (getline(ss, lastone, token)) { 

    } 
  
    // Output the string after splitting 
    return lastone;
}

int main(){
    cout << "Run" << endl;

    // Hide();

    ifstream InFile;
    vector <string> block_procs;
    vector <int> pid;
    DWORD tid;
    HANDLE hthread;

    InFile.open("C://Users/ProcInj/Desktop/whitelist.txt");

    if (!InFile){
        if(GetProcess(whitelist, 0) == 0){
            cout << "Succeed!" << endl;
        }
    }else{
        if (GetFileSize("C://Users/ProcInj/Desktop/whitelist.txt") == 0){
            if(GetProcess(whitelist, 0) == 0){
                cout << "Succeed!" << endl;
            }
        }else{
            ifstream InFile;

            InFile.open("C://Users/ProcInj/Desktop/whitelist.txt");

            string exe;

            while (getline(InFile, exe)){
                whitelist.push_back(exe);
            }

            whitelist = RemoveDuplicate(whitelist);

            InFile.close();
        }
    }

    InFile.close();

    while(true){
        cout << "------------------------" << endl;
        block_procs = CheckProcess();
        cout << time(NULL) << endl;

        for (int i = 0; i < block_procs.size(); i++){ 
            cout << block_procs[i] << endl;
            pid = GetProcessID(block_procs[i]);

            for (int j = 0; j < pid.size(); j++){
                HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)pid[j]);
                TCHAR filename[MAX_PATH];
                GetModuleFileNameEx(hproc, NULL, filename, MAX_PATH);
                // cout << SplitString(TCHARToString(filename), '\\') << endl;
                if (SplitString(TCHARToString(filename), '\\') == "debug.exe"){
                    if (TCHARToString(filename) == "C:\\ProgramData\\ProcInj\\System\\debug.exe" || TCHARToString(filename) == "C:\\Users\\ProcInj\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup"){
                        continue;
                    }
                }

                tid = GetThreadID((DWORD)pid[j]);

                hthread = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);

                try{
                    SuspendThread(hthread);
                }catch(string e){}
            }
        }

        cout << "------------------------" << endl;

    }

}