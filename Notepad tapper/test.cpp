#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main(int argc, char **argv){
  char* test = "test";
  cout << &test[3] << endl;
}