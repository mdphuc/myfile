#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdio>

// #pragma comment(lib, "Urlmon.lib")

using namespace std;

int main(){
  // TCHAR url[MAX_PATH] = TEXT("https://www.python.org/ftp/python/3.12.4/python-3.12.4-amd64.exe");
  // TCHAR dest[MAX_PATH] = TEXT("C:/Users/Mai Dinh Phuc/Desktop/MDP/CTF/BoxCreate/ProcInj/Web/python-3.12.4-amd64.exe");

  // URLDownloadToFile(NULL, url, dest, 0, NULL);
  ofstream OutFile("output.txt");
  OutFile << "TEST   YOOOOO" << endl;
  OutFile.close();
}