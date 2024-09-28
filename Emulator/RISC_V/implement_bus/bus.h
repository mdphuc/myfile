#include <iostream>
#include <cstdint>
#include <cstring>
#include <time.h>
#include "const.h"

#ifndef DRAM_
    #define DRAM_
    #include "dram.h"
#endif

class BUS{
    private:
        DRAM dram;
};
