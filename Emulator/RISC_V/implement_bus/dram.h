#include <iostream>
#include <cstdint>
#include <cstring>
#include <time.h>
#include "const.h"

using namespace std;

#ifndef CONST_
    #define CONST_
    #include "const.h"
#endif

class DRAM{
    private:
<<<<<<< HEAD
      uint8_t memory[DRAM_SIZE];
      void load8(uint64_t addr);
      void load16(uint64_t addr);
      void load32(uint64_t addr){
        uint64_t index = index - 
      };
      
      
      void load64(uint64_t addr);
      void store8(uint64_t addr);
      void store16(uint64_t addr);
      void store32(uint64_t addr);
      void store64(uint64_t addr);

    public:
      unsigned long fileLen;

      DRAM(char *filename){
        ReadBin(filename);
      }

      uint8_t* GetMemory(){
        uint8_t *m = memory;
        return m;
      }

      void ReadBin(char *filename){
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


        memcpy(memory, buffer, fileLen);
    }

    void Load(uint64_t addr, uint64_t size){
      switch(size){
        case 8:
          load8(addr);
          break;
        case 16:
          load16(addr);
          break;
        case 32:
          load32(addr);
          break;
        case 64:
          load64(addr);
          break;
      }
    }

    void Store(uint64_t addr, uint64_t size){
      switch(size){
        case 8:
          store8(addr);
          break;
        case 16:
          store16(addr);
          break;
        case 32:
          store32(addr);
          break;
        case 64:
          store64(addr);
          break;
      }
    }

=======
        uint8_t memory[MEMORY_SIZE];

        uint8_t load8(uint64_t addr){
            uint64_t index_pc = addr - MEMORY_BASE;
            return (uint32_t) memory[index_pc];
        }
        uint16_t load16(uint64_t addr){
            uint64_t index_pc = addr - MEMORY_BASE;
            return (uint32_t) memory[index_pc]
                | (uint32_t) memory[index_pc + 1] << 8;
        }

        uint32_t load32(uint64_t addr){
            uint64_t index_pc = addr - MEMORY_BASE;
            return (uint32_t) memory[index_pc]
                | (uint32_t) memory[index_pc + 1] << 8
                | (uint32_t) memory[index_pc + 2] << 16
                | (uint32_t) memory[index_pc + 3] << 24;
        }

        uint64_t load64(uint64_t addr){
            uint64_t index_pc = addr - MEMORY_BASE;
            return (uint32_t) memory[index_pc]
                | (uint32_t) memory[index_pc + 1] << 8
                | (uint32_t) memory[index_pc + 2] << 16
                | (uint32_t) memory[index_pc + 3] << 24
                | (uint32_t) memory[index_pc + 3] << 32
                | (uint32_t) memory[index_pc + 3] << 40
                | (uint32_t) memory[index_pc + 3] << 48
                | (uint32_t) memory[index_pc + 3] << 56;

        };

        void store8(uint64_t addr, uint64_t value){
            uint64_t index_pc = addr - MEMORY_BASE;
            memory[index_pc] = (uint8_t)(value & 0xff);
        }

        void store16(uint64_t addr, uint64_t value){
            uint64_t index_pc = addr - MEMORY_BASE;
            memory[index_pc] = (uint8_t)(value & 0xff);
            memory[index_pc + 1] = (uint8_t)((value >> 8) & 0xff);
        }

        void store32(uint64_t addr, uint64_t value){
            uint64_t index_pc = addr - MEMORY_BASE;
            memory[index_pc] = (uint8_t)(value & 0xff);
            memory[index_pc + 1] = (uint8_t)((value >> 8) & 0xff);
            memory[index_pc + 2] = (uint8_t)((value >> 16) & 0xff);
            memory[index_pc + 3] = (uint8_t)((value >> 24) & 0xff);
        };

        void store64(uint64_t addr, uint64_t value){
            uint64_t index_pc = addr - MEMORY_BASE;
            memory[index_pc] = (uint8_t)(value & 0xff);
            memory[index_pc + 1] = (uint8_t)((value >> 8) & 0xff);
            memory[index_pc + 2] = (uint8_t)((value >> 16) & 0xff);
            memory[index_pc + 3] = (uint8_t)((value >> 24) & 0xff);
            memory[index_pc + 4] = (uint8_t)((value >> 32) & 0xff);
            memory[index_pc + 5] = (uint8_t)((value >> 40) & 0xff);
            memory[index_pc + 6] = (uint8_t)((value >> 48) & 0xff);
            memory[index_pc + 7] = (uint8_t)((value >> 56) & 0xff);
        };

    public:
        unsigned long fileLen;

        DRAM(char *filename){
            ReadBin(filename);
        }

        DRAM(){
            int c;
        }
        
        void ReadBin(char *filename){
            FILE *file;
            uint8_t *buffer;
            // unsigned long fileLen;

            file = fopen(filename, "rb");

            fseek(file, 0, SEEK_END);
            fileLen = ftell(file);
            fseek(file, 0, SEEK_SET);

            buffer = (uint8_t *)malloc(fileLen + 1);

            fread(buffer, fileLen + 1, 1, file);

            // for (int i=0; i<fileLen; i+=2) {
            //     if (i%16==0) printf("\n%.8x: ", i);
            //     printf("%02x%02x ", *(buffer+i), *(buffer+i+1));
            // }
            // printf("\n");


            memcpy(memory, buffer, fileLen);
        }

        void GetMemory(){
            for (int i = 0; i < fileLen; i += 2) {
                if (i % 16 == 0) printf("\n%.8x: ", i);
                printf("%02x%02x ", *(memory + i), *(memory + i + 1));
            }
            printf("\n");
        }

        template<typename T> T Load(uint64_t addr, uint64_t size){
            switch(size){
                case 8:
                    return load8(addr);
                case 16:
                    return load16(addr);
                case 32:
                    return load32(addr);
                case 64:
                    return load64(addr);
            }
        }

        void Store(uint64_t addr, uint64_t size, uint64_t value){
            switch(size){
                case 8:
                    store8(addr, value);
                case 16:
                    store16(addr, value);
                case 32:
                    store32(addr, value);
                case 64:
                    store64(addr, value);
            }
        }
>>>>>>> 84bfd94c49aa5481d8ae959dce48ff0504331c1c



};