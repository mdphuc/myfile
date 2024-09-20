#include <cstdint>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <vector>

#define MEMORY_SIZE 1024 * 1024 * 1 //1MiB

using namespace std;

class CPU{
    // private:
    //     uint64_t regs[32];
    //     uint64_t pc;
    //     uint8_t dram[MEMORY_SIZE];
    //     unsigned long fileLen;
    public:
        uint64_t regs[32];
        uint64_t pc;
        uint8_t dram[MEMORY_SIZE];
        unsigned long fileLen;

        CPU(char *filename){
            pc = 0;
            regs[0] = 0;
            regs[2] = MEMORY_SIZE; //sp (stack pointer)
            read_bin(filename);
        }

        void read_bin(char *filename){
            FILE *file;
            uint8_t *buffer;
            // unsigned long fileLen;

            file = fopen(filename, "rb");

            fseek(file, 0, SEEK_END);
            fileLen = ftell(file);
            fseek(file, 0, SEEK_SET);

            buffer = (uint8_t *)malloc(fileLen + 1);

            fread(buffer, fileLen + 1, 1, file);

            for (int i=0; i<fileLen; i+=2) {
                if (i%16==0) printf("\n%.8x: ", i);
                printf("%02x%02x ", *(buffer+i), *(buffer+i+1));
            }
            printf("\n");


            memcpy(dram, buffer, fileLen);
        }

        void get_memory(){
            for (int i=0; i<fileLen; i+=2) {
                if (i%16==0) printf("\n%.8x: ", i);
                printf("%02x%02x ", *(dram+i), *(dram+i+1));
            }
            printf("\n");
        }

        void dump_register(){
            string abi[32] = {
            "zero", " ra ", " sp ", " gp ", " tp ", " t0 ", " t1 ", " t2 ", " s0 ", " s1 ", " a0 ",
            " a1 ", " a2 ", " a3 ", " a4 ", " a5 ", " a6 ", " a7 ", " s2 ", " s3 ", " s4 ", " s5 ",
            " s6 ", " s7 ", " s8 ", " s9 ", " s10 ", " s11 ", " t3 ", " t4 ", " t5 ", " t6 ",
            };
            for (int i = 0; i < 32; i++){
                cout << abi[i] << "= " << "0x" << hex << regs[i] << endl;
            }
        }

        uint32_t fetch(){
            return (uint32_t) dram[pc]
                | (uint32_t) dram[pc + 1] << 8
                | (uint32_t) dram[pc + 2] << 16
                | (uint32_t) dram[pc + 3] << 24;
        }

        int execute(uint32_t inst){
            int opcode = inst & 0x7f; // opcode in bits 0...6
            int rd;
            int rs1;
            int rs2;
            uint64_t imm;
            regs[0] = 0;

            switch(opcode){
                case 0x13:
                    rd = (inst >> 7) & 0x1f; // rd in bits 7...11
                    rs1 = (inst >> 15) & 0x1f; // rs1 in bits 15...19
                    rs2 = (inst >> 20) & 0x1f; // rs2 in bits 20...24
                    imm = uint64_t((inst & 0xfff00000) >> 20);
                    regs[rd] = regs[rs2] + imm;
                    break;
                case 0x33:
                    rd = (inst >> 7) & 0x1f; // rd in bits 7...11
                    rs1 = (inst >> 15) & 0x1f; // rs1 in bits 15...19
                    rs2 = (inst >> 20) & 0x1f; // rs2 in bits 20...24
                    regs[rd] = regs[rs1] + regs[rs2];
                    break;
                default:
                    return 0;
            }

            return 1;
        }
};

int main(int argc, char **args){
    char *filename = args[1];
    CPU cpu((char *)"./add-addi.bin");

    while(1){
        uint32_t inst = cpu.fetch();
        if (!cpu.execute(inst)){
            break;
        }
    

        cpu.pc += 4;
    }

    cpu.dump_register();

}

