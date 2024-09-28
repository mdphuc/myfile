#include <iostream>
#include <cstdint>
#include <cstring>
#include <time.h>
#include <vector>
#include "const.h"

using namespace std;

#ifndef BUS_
    #define BUS_
    #include "bus.h"
#endif

class CPU{
    private:
        uint64_t regs[32];
        uint64_t regs_old[32];
        BUS bus;
        vector <int> power_trace;
};