#include <iostream>
#include <cstdint>
#include <cstring>
#include <time.h>

#ifndef DRAM_
    #define DRAM_
    #include "dram.h"
#endif

class BUS{
    private:
        DRAM dram;

    public:
        template<typename T> T Load(uint64_t addr, uint64_t size){
            if (addr >= MEMORY_BASE){
                return dram.Load<T>(addr, size);
            }
        }

        void Store(uint64_t addr, uint64_t size, uint64_t value){
            if (addr >= MEMORY_BASE){
                return dram.Store(addr, size, value);
            }
        }

        BUS(char *filename){
            dram = DRAM(filename);
        };

        BUS(){
            int c;
        }
};
