#include <iostream>
#include <cstdint>
#include <cstring>
#include <time.h>
#include <vector>

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
    public:
        uint64_t pc;

        CPU(char *filename){
            regs[0] = 0;
            regs[2] = MEMORY_SIZE; //sp
            pc = MEMORY_BASE;
            bus = BUS(filename);
        }

        template<typename T> uint64_t rd(T inst){
            return (inst >> 7) & 0x1f;
        }

        template<typename T> uint64_t rs1(T inst){
            return (inst >> 15) & 0x1f;
        }

        template<typename T> uint64_t rs2(T inst){
            return (inst >> 20) & 0x1f;
        }

        template<typename T> uint64_t imm_I(T inst){
            return (int64_t)(int32_t)((inst & 0xfff00000) >> 20);
        }

        template<typename T> uint64_t imm_S(T inst){
            return (int64_t)(int32_t)((inst & 0xfe000000) >> 25)
                | ((inst >> 7) >> 0x1f);
        }

        template<typename T> uint64_t imm_U(T inst){
            return (int64_t)(int32_t)(inst & 0xfffff999);
        }

        template<typename T> T Fetch(){
            return bus.Load<T>(pc, 32);
        }

        template<typename T> int Execute(T inst){
            int opcode = inst & 0x7f; // opcode in bits 0...6
            uint64_t addr;
            int funct3 = (inst >> 12) & 0x7;
            int funct7 = (inst >> 25) & 0x7f;
            uint64_t imm; 
            T val;
            regs[0] = 0;

            switch(opcode){
                case 0x03:
                    imm = imm_I(inst);
                    addr = regs[rs1<T>(inst)] + imm;
                    switch(funct3){
                        case 0x0: //lb
                            val = bus.Load<T>(addr, 8);
                            regs[rd<T>(inst)] = (uint64_t)(int64_t)(int8_t)val;
                            break;
                        case 0x1: //lh
                            val = bus.Load<T>(addr, 16);
                            regs[rd<T>(inst)] = (uint64_t)(int64_t)(int16_t)val;
                            break;
                        case 0x2: //lw
                            val = bus.Load<T>(addr, 32);
                            regs[rd<T>(inst)] = (uint64_t)(int64_t)(int32_t)val;
                            break;
                        case 0x3: //ld
                            val = bus.Load<T>(addr, 64);
                            regs[rd<T>(inst)] = val;
                            break;
                        case 0x4: //lbu
                            val = bus.Load<T>(addr, 8);
                            regs[rd<T>(inst)] = val;
                            break;
                        case 0x5: //lhu
                            val = bus.Load<T>(addr, 16);
                            regs[rd<T>(inst)] = val;
                            break;
                        case 0x6:
                            val = bus.Load<T>(addr, 32);
                            regs[rd<T>(inst)] = val;
                            break;
                    }
                    break;
                case 0x23:
                    imm = imm_S(inst);
                    addr = regs[rs1<T>(inst)] + imm;
                    switch(funct3){
                        case 0x0:
                            bus.Store(addr, 8, regs[rs2<T>(inst)]);
                            break;
                        case 0x1:
                            bus.Store(addr, 16, regs[rs2<T>(inst)]);
                            break;
                        case 0x2:
                            bus.Store(addr, 32, regs[rs2<T>(inst)]);
                            break;
                        case 0x3:
                            bus.Store(addr, 64, regs[rs2<T>(inst)]);
                            break;
                    }
                    break;
                case 0x13:
                    imm = imm_I(inst);
                    regs[rd<T>(inst)] = regs[rs1<T>(inst)] + imm;
                    break;
                case 0x33:
                    regs[rd<T>(inst)] = regs[rs1<T>(inst)] + regs[rs2<T>(inst)];
                    break;
                default:
                    return 0;
            }

            return 1;
        }

        void DumpRegister(){
            string abi[32] = {
                "zero", " ra ", " sp ", " gp ", " tp ", " t0 ", " t1 ", " t2 ", " s0 ", " s1 ", " a0 ",
                " a1 ", " a2 ", " a3 ", " a4 ", " a5 ", " a6 ", " a7 ", " s2 ", " s3 ", " s4 ", " s5 ",
                " s6 ", " s7 ", " s8 ", " s9 ", " s10 ", " s11 ", " t3 ", " t4 ", " t5 ", " t6 ",
            };
            for (int i = 0; i < 32; i++){
                cout << abi[i] << "= " << "0x" << hex << regs[i] << endl;
            }
        }


};