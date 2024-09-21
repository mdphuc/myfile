#include <iostream>
#include <cstdint>

using namespace std;

int main(){
  unsigned short a = 2;

  cout << ((a & 0x80) >> 7) << endl;
}