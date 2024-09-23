#include <iostream>
#include "cpu.h"

using namespace std;

int main(int argc, char **args){
    char *filename = args[1];
    CPU cpu(filename);

    while(1){
        cout << "yo" << endl;
        uint32_t inst = cpu.Fetch<uint32_t>();
        
        if (!cpu.Execute<uint32_t>(inst)){
            break;
        }

        cpu.pc += 4;

    }

    cpu.DumpRegister();
}
