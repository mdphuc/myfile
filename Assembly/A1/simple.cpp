#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

void LaunchCalc(){
  WinExec("calc.exe", 0);
}

int main() {
  // LaunchCalc();

  unsigned long WinExec_addr = (unsigned long)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "ExitProcess");
  cout << WinExec_addr << endl;

  ofstream OutFile("addr.txt", ios::app);

  OutFile << WinExec_addr << endl;
}