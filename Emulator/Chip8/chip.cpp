#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>

using namespace std;

class Chip8{
  public:
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32]; //Graphic 64 * 32
    unsigned short delay_timer;
    unsigned short sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key_state[16];
    unsigned char chip8_fontset[80] =
    { 
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    
    bool drawFlag = false;

    void initialize(){
      pc = 0x200;
      opcode = 0x000;
      I = 0x000;
      sp = 0x000;
      drawFlag = false;

      for (int i = 0; i < 80; i++){
        memory[i] = chip8_fontset[i];
      }

      memset(key_state, 0, sizeof(key_state));
    }

    void _00E0(){
      memset(gfx, 0, sizeof(gfx));
    }

    void _00EE(){
      sp--;
      pc = stack[sp];
    }

    void _1NNN(){
      pc = opcode & 0x0FFF;
    }

    void _2NNN(){
      stack[sp] = pc;
      sp++;
      pc = opcode & 0x0FFF;
    }

    void _3XNN(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      if (V[vx] == NN){
        pc += 2;
      }
    }

    void _4XNN(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      if (V[vx] != NN){
        pc += 2;
      }
    }

    void _5XY0(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      if (V[vx] == V[vy]){
        pc += 2;
      }
    }

    void _6XNN(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      V[vx] = NN;
      pc += 2;
    }

    void _7XNN(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      V[vx] += NN;
      pc += 2;
    }

    void _8XY0(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      V[vx] = V[vy];
    }

    void _8XY1(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      V[vx] = V[vx] | V[vy];
    }

    void _8XY2(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      V[vx] = V[vx] & V[vy];
    }

    void _8XY3(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      V[vx] = V[vx] ^ V[vy];
    }

    void _8XY4(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      unsigned short sum = V[vx] + V[vy];

      if (sum > 255){
        V[0xF] = 1;
      }else{
        V[0xF] = 0;
      }

      V[vx] = sum;
    }

    void _8XY5(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      unsigned short minus = V[vx] - V[vy];

      if (minus > 0){
        V[0xF] = 1;
      }else{
        V[0xF] = 0;
      }

      V[vx] = minus;
    }

    void _8XY6(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      
      V[0xF] = V[vx] & 0x1;

      V[vx] = V[vy] >> 1;
    }

    void _8XY7(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      unsigned short minus = V[vy] - V[vx];

      if (minus > 0){
        V[0xF] = 1;
      }else{
        V[0xF] = 0;
      }

      V[vx] = minus;
    }

    void _8XYE(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;

      V[0xf] = (V[vx] & 0x80) >> 7;

      V[vx] = V[vy] << 1;
    }

    void _9XY0(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;

      if (V[vx] != V[vy]){
        pc += 2;
      }
    }

    void _ANNN(){
      unsigned short NNN = (opcode & 0x0FFF);
      I = NNN;
    }

    void _BNNN(){
      unsigned short NNN = (opcode & 0x0FFF);
      pc = V[0] + NNN;
    }

    void _CXNN(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short NN = (opcode & 0x00FF);
      srand(time(0));

      V[vx] = rand() & NN;
    }

    void _DXYN(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short vy = (opcode & 0x00F0) >> 4;
      unsigned short height = (opcode & 0x000F);
      unsigned short pixel;

      V[0xF] = 0;
      for (int y = 0; y < height; y++){
        pixel = memory[I + y];
        for (int x = 0; x < 8; x++){
          if ((pixel & (0x80 >> x)) != 0){
            if (gfx[vx + x + ((vy + y)*64)] == 1){
              V[0xF] = 1;
            }
            gfx[vx + x + ((vy + y)*64)] ^= 1;
          }
        }
      }

      drawFlag = true;
    }

    void _EX9E(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      if (key_state[V[vx]] != 0){
        pc += 2;
      }
    }

    void _EXA1(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      if (key_state[V[vx]] == 0){
        pc += 2;
      }
    }

    void _FX07(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      V[vx] = delay_timer;
    }

    void _FX0A(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      if (key_state[0] != 0){
        V[vx] = 0;
      }else if(key_state[1] != 0){
        V[vx] = 1;
      }else if(key_state[2] != 0){
        V[vx] = 2;
      }else if(key_state[3] != 0){
        V[vx] = 3;
      }else if(key_state[4] != 0){
        V[vx] = 4;
      }else if(key_state[5] != 0){
        V[vx] = 5;
      }else if(key_state[6] != 0){
        V[vx] = 6;
      }else if(key_state[7] != 0){
        V[vx] = 7;
      }else if(key_state[8] != 0){
        V[vx] = 8;
      }else if(key_state[9] != 0){
        V[vx] = 9;
      }else if(key_state[10] != 0){
        V[vx] = 10;
      }else if(key_state[11] != 0){
        V[vx] = 11;
      }else if(key_state[12] != 0){
        V[vx] = 12;
      }else if(key_state[13] != 0){
        V[vx] = 13;
      }else if(key_state[14] != 0){
        V[vx] = 14;
      }else if(key_state[15] != 0){
        V[vx] = 15;
      }else{
        pc -= 2;
      }
    }

    void _FX15(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      delay_timer = V[vx];
    }

    void _FX18(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      sound_timer - V[vx];
    }

    void _FX1E(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      I += V[vx];
    }

    void _FX29(){
      unsigned short vx = (opcode & 0x0F00) >> 8;
      unsigned short digit = V[vx];

      I = digit * 5;
    }

    void _FX33(){
      unsigned short vx = (opcode & 0x0F00) >> 8;

      unsigned short value = V[vx];

      memory[I + 2] = value % 10;
      value /= 10;

      memory[I + 1] = value % 10;
      value /= 10;

      memory[I] = value % 10;
    }

    void _FX55(){
      unsigned short vx = (opcode & 0x0F00) >> 8;

      for (int i = 0; i < vx; i++){
        memory[I + i] = V[i];
      }
    }

    void _FX65(){
      unsigned short vx = (opcode & 0x0F00) >> 8;

      for (int i = 0; i < vx; i++){
        V[i] = memory[I + i];
      }
    }

    void cycle(){
      opcode = memory[pc] << 8 | memory[pc + 1];
    
      switch (opcode & 0xF000){
        case 0x0000:
          switch (opcode & 0x000F){
            case 0x0000:
              _00E0();
              pc += 2;
              break;
            case 0x000E:
              _00EE();
              pc += 2;
              break;
          }
          break;
        case 0x1000:
          _1NNN();
          pc += 2;
          break;
        case 0x2000:
          _2NNN();
          pc += 2;
          break;
        case 0x3000:
          _3XNN();
          pc += 2;
          break;
        case 0x4000:
          _4XNN();
          pc += 2;
          break;
        case 0x5000:
          _5XY0();
          pc += 2;
          break;
        case 0x6000:
          _6XNN();
          pc += 2;
          break;
        case 0x7000:
          _7XNN();
          pc += 2;
          break;
        case 0x8000:
          switch(opcode & 0x000F){
            case 0x0000:
              _8XY0();
              pc += 2;
              break;
            case 0x0001:
              _8XY1();
              pc += 2;
              break;
            case 0x0002:
              _8XY2();
              pc += 2;
              break;
            case 0x0003:
              _8XY3();
              pc += 2;
              break;
            case 0x0004:
              _8XY4();
              pc += 2;
              break;
            case 0x0005:
              _8XY5();
              pc += 2;
              break;
            case 0x0006:
              _8XY6();
              pc += 2;
              break;
            case 0x0007:
              _8XY7();
              pc += 2;
              break;
            case 0x000E:
              _8XYE();
              pc += 2;
              break;
          }
          break;
        case 0x9000:
          _9XY0();
          pc += 2;
          break;
        case 0xA000:
          _ANNN();
          pc += 2;
          break;
        case 0xB000:
          _BNNN();
          pc += 2;
          break;
        case 0xC000:
          _CXNN();
          pc += 2;
          break;
        case 0xD000:
          _DXYN();
          pc += 2;
          break;
        case 0xE000:
          switch (opcode & 0x000F){
            case 0x000E:
              _EX9E();
              pc += 2;
              break;
            case 0x0001:
              _EXA1();
              pc += 2;
              break;
          }
          break;
        case 0xF000:
          switch (opcode & 0x00FF){
            case 0x0007:
              _FX07();
              pc += 2;
              break;
            case 0x000A:
              _FX0A();
              pc += 2;
              break;
            case 0x0015:
              _FX15();
              pc += 2;
              break;
            case 0x0018:
              _FX18();
              pc += 2;
              break;
            case 0x001E:
              _FX1E();
              pc += 2;
              break;
            case 0x0029:
              _FX29();
              pc += 2;
              break;
            case 0x0033:
              _FX33();
              pc += 2;
              break;
            case 0x0055:
              _FX55();
              pc += 2;
              break;
            case 0x0065:
              _FX65();
              pc += 2;
              break;
          }
          break;
      }

    if(delay_timer > 0)
      --delay_timer;
  
    if(sound_timer > 0)
    {
      if(sound_timer == 1)
        printf("BEEP!\n");
      --sound_timer;
    }  
  }
};

int main(){

}