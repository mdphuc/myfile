#include <iostream>
#include <cstdint>
#include <cstring>
#include <time.h>
#include "const.h"

class DRAM{
    private:
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




};