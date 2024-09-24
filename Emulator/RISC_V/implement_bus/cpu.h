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

        uint64_t wrapping_add(uint64_t a1, uint64_t a2){
            if ((a1 + a2) > 0xffffffff){
                exit(0);
                return 0;
            }else{
                return a1 + a2;
            }
        }

        uint64_t wrapping_sub(uint64_t s1, uint64_t s2){
            if ((s1 - s2) <= 0){
                exit(0);
                return 0;
            }else{
                return s1 - s2;
            }
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
            uint32_t shamt;
            T val;
            regs[0] = 0;

            switch(opcode){
                case 0x03: // load
                    imm = imm_I(inst);
                    addr = wrapping_add(regs[rs1<T>(inst)], imm);
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
                case 0x23: //store
                    imm = imm_S(inst);
                    addr = wrapping_add(regs[rs1<T>(inst)], imm);
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
                    imm = imm_I<T>(inst);
                    shamt = (uint32_t)(imm & 0x3f);
                    switch (funct3){
                        case 0x0: //addi
                            regs[rd<T>(inst)] = wrapping_add(regs[rs1<T>(inst)], imm);
                            break;
                        case 0x2: //slti
                            regs[rd<T>(inst)] = (int64_t)(regs[rs1<T>(inst)]) < (int64_t)(regs[rs2<T>(inst)]);
                            break;
                        case 0x3: //sltiu
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] < regs[rs2<T>(inst)];
                            break;
                        case 0x4: //xori
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] ^ imm;
                            break;
                        case 0x6: //ori
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] | imm;
                            break;
                        case 0x7: //andi
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] & imm;
                            break;
                        case 0x1: //slli
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] << shamt;
                            break;
                        case 0x5:
                            switch(funct7){
                                case 0x0: //srli
                                    regs[rd<T>(inst)] = regs[rs1<T>(inst)] >> shamt;
                                    break;
                                case 0x10: //srai
                                    regs[rd<T>(inst)] = (uint64_t)((int64_t)regs[rs1<T>(inst)] >> shamt);
                                    break;
                            }
                            break;

                    }
                    break;
                case 0x33: 
                    switch(funct3){
                        case 0x0:
                            switch(funct7){
                                case 0x0: //add
                                    regs[rd<T>(inst)] = wrapping_add(regs[rs1<T>(inst)], regs[rs2<T>(inst)]);
                                    break;
                                case 0x10: //sub
                                    regs[rd<T>(inst)] = wrapping_sub(regs[rs1<T>(inst)], regs[rs2<T>(inst)]);
                                    break;
                            }
                            break;
                        case 0x1: //sll
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] << regs[rs2<T>(inst)];
                            break;
                        case 0x2: //slt
                            regs[rd<T>(inst)] = (int64_t)regs[rs1<T>(inst)] < (int64_t)regs[rs2<T>(inst)];
                            break;
                        case 0x3: //sltu
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] < regs[rs2<T>(inst)];
                            break;
                        case 0x4: //xor
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] ^ regs[rs2<T>(inst)];
                            break;
                        case 0x5:
                            switch(funct7){
                                case 0x0: //srl
                                    regs[rd<T>(inst)] = regs[rs1<T>(inst)] >> regs[rs2<T>(inst)];
                                    break;
                                case 0x10: //sra
                                    regs[rd<T>(inst)] = (uint64_t)((int64_t)regs[rs1<T>(inst)] >> (int64_t)regs[rs2<T>(inst)]);
                                    break;

                            }
                            break;
                        case 0x6: //or
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] | regs[rs2<T>(inst)];
                            break;
                        case 0x7: //and
                            regs[rd<T>(inst)] = regs[rs1<T>(inst)] & regs[rs2<T>(inst)];
                            break;
                    }
                
                    break;
                case 0x37: //lui
                    regs[rd<T>(inst)] = (uint64_t)(imm_U<T>(inst) << 12);
                    break;
                case 0x17: //auipc
                    regs[rd<T>(inst)] = wrapping_add(pc, (uint64_t)(imm_U<T>(inst) << 12));
                    break;
                case 0x6F: //jal
                    regs[rd<T>(inst)] = wrapping_add(pc, 4);
                    imm = (uint64_t)((int64_t)(int32_t)(inst & 0x80000000) >> 11) //imm[20]
                        | (inst & 0xff000) //imm[19:12]
                        | ((inst >> 9) & 0x800) //imm[11]
                        | ((inst >> 20) & 0x7fe); //imm[10:1]
                    pc = wrapping_add(pc, imm);
                    break;
                case 0x67: //jalr
                    regs[rd<T>(inst)] = wrapping_add(pc, 4);
                    pc = wrapping_add(regs[rs1<T>(inst)], imm_I<T>(inst) & !1);
                    break;
                case 0x63: //branch
                    imm = (uint64_t)(((int64_t)(int32_t)(inst & 0x80000000) >> 19)) //imm[12]
                        | ((inst & 0x80) << 4) // imm[11]
                        | ((inst >> 20) & 0x7e0) // imm[10:5]
                        | ((inst >> 7) & 0x1e); // imm[4:1]
                    switch(funct3){
                        case 0x0: //beq
                            if (rs1<T>(inst) == rs2<T>(inst)){
                                pc = wrapping_add(pc, imm);
                            }
                            break;
                        case 0x1: //bne
                            if ((int64_t)rs1<T>(inst) != (int64_t)rs2<T>(inst)){
                                pc = wrapping_add(pc, imm);
                            }                        
                            break;
                        case 0x4: //blt
                            if ((int64_t)rs1<T>(inst) < (int64_t)rs2<T>(inst)){
                                pc = wrapping_add(pc, imm);
                            }
                            break;
                        case 0x5: //bge
                            if ((int64_t)rs1<T>(inst) >= (int64_t)rs2<T>(inst)){
                                pc = wrapping_add(pc, imm);
                            }
                            break;
                        case 0x6: //bltu
                            if (rs1<T>(inst) < rs2<T>(inst)){
                                pc = wrapping_add(pc, imm);
                            }
                            break;
                        case 0x7: //bgeu
                            if (rs1<T>(inst) >= rs2<T>(inst)){
                                pc = wrapping_add(pc, imm);
                            }
                            break;
                    }
                    break;
                case 0x1B:
                    imm = imm_I<T>(inst);
                    shamt = (uint32_t)(imm & 0x3f);
                    switch(funct3){
                        case 0x0: //addiw
                            regs[rd<T>(inst)] = (uint64_t)(int64_t)(int32_t)wrapping_add(regs[rs1<T>(inst)], imm);
                            break;
                        case 0x1: //slliw
                            regs[rd<T>(inst)] = (uint64_t)(int64_t)(int32_t)(regs[rs1<T>(inst)] << shamt);
                            break;
                        case 0x3: 
                            switch(funct7){
                                case 0x0: //srliw
                                    regs[rd<T>(inst)] = (uint64_t)(int64_t)(int32_t)((uint32_t)regs[rs1<T>(inst)] >> shamt);
                                    break;
                                case 0x10: //sraiw
                                    regs[rd<T>(inst)] = (uint64_t)(int64_t)((int32_t)regs[rs1<T>(inst)] >> shamt);
                                    break;
                            }
                            break;
                    }
                    break;
                case 0x3B:
                    switch(funct3){
                        case 0x0:
                            switch(funct7){
                                case 0x0: //addw
                                    regs[rd<T>(inst)] = (uint64_t)(int64_t)(int32_t)wrapping_add(regs[rs1<T>(inst)], regs[rs2<T>(inst)]);
                                    break;
                                case 0x10: //subw
                                    regs[rd<T>(inst)] = (uint64_t)(int32_t)wrapping_sub(regs[rs1<T>(inst)], regs[rs2<T>(inst)]);
                                    break;
                            }
                            break;
                        case 0x1: //sllw
                            regs[rd<T>(inst)] = (uint64_t)(int32_t)((uint32_t)regs[rs1<T>(inst)] << shamt);
                            break;
                        case 0x5:
                            switch(funct7){
                                case 0x0: //srlw
                                    regs[rd<T>(inst)] = (uint64_t)(int32_t)((uint32_t)regs[rs1<T>(inst)] >> shamt);
                                    break;
                                case 0x10: //sraw
                                    regs[rd<T>(inst)] = (uint64_t)((int32_t)regs[rs1<T>(inst)] >> (int32_t)(shamt));
                                    break;
                            }
                            break;

                    }
                default:
                    cout << "opcode = " << "0x" << hex << opcode << endl;
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