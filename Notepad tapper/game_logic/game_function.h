#include <iostream>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <ctime>
#include <fstream>
#include <thread> 
#include <random>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

char *message1 = "YOU LOSE";

int bullet_id = 0;
int alien_id = 0;
int score = 0;

HWND notepad_hwnd;
DWORD width, height;
HANDLE notepad_thread;

size_t colCount = 127;
size_t rowCount = 35;
size_t charCount = colCount * rowCount;
size_t bufferSize = charCount * 2;
int offsetX = 200;
int offsetY = 100;
char *buffer = (char *)malloc(bufferSize);
float DISTANCE = 1;

int spawn_rate = 13000;

bool end_game = false; 

class Alien{
    private:
        int health;
        vector<char *> shape;
        int x;
        int y;
        int id;
        bool destroyed;
        int speed;
    public:
        Alien(int h, int id){
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib(1, 125);
            uniform_int_distribution<> distrib2(1400, 1700);
            
            this->x = distrib(gen);
            this->y = 0;
            this->health = h;
            this->id = id;
            this->destroyed = false;
            this->speed = distrib2(gen);
            SetShape();
        }

        bool operator==(const Alien &alien2){
            if(this->id == alien2.id){
                return true;
            }else{
                return false;
            }
        }

        int GetHealth(){
            return this->health;
        }

        vector<char *>GetShape(){
            return this->shape;
        }

        int GetSpeed(){
            return speed;
        }

        int GetX(){
            return x;
        }

        int GetY(){
            return y;
        }

        int GetId(){
            return id;
        }

        bool GetDestroyed(){
            return destroyed;
        }

        void SetDestroyed(bool destroyed){
            this->destroyed = destroyed;
        }

        void SetX(int x){
            this->x = x;
        }

        void SetY(int y){
            this->y = y;
        }

        void SetShape(){
            for (int i = 0; i < this->health; i++){
                char *s = "###";
                shape.push_back(s);
            }
            shape.push_back(" # ");
        } 
};

class Gun{
    private:
        int x;
        int y;
    public:
        Gun(int x){
            this->x = x;
        }
        int GetX(){
            return x;
        }

        void SetX(int x){
            this->x = x;
        }
};

class Bullet{
    private:
        int x;
        int y = 35 - 4;
        int id;
        bool destroyed;
    public:
        Bullet(int x, int id){
            this->x = x;
            this->id = id;
            this->destroyed = false;
        }

        bool operator==(const Bullet &bullet2){
            if(this->id == bullet2.id){
                return true;
            }else{
                return false;
            }
        }

        bool GetDestroyed(){
            return destroyed;
        }

        void SetDestroyed(bool destroyed){
            this->destroyed = destroyed;
        }

        int GetX(){
            return x;
        }

        int GetY(){
            return y;
        }

        void SetX(int x){
            this->x = x;
        }

        void SetY(int y){
            this->y = y;
        }

};

vector<Alien>ALIENS;
vector<Bullet>BULLETS;

bool UpdateProcessMemory(DWORD proc_id, LPVOID base, size_t size);

void InstallHook(DWORD thread_id);

void PrintYouLose(DWORD proc_id, LPVOID base){
  for(int i = 0; i < strlen(message1); i++){
    buffer[2*i] = message1[i];
  }
  UpdateProcessMemory(proc_id, base, bufferSize);
}

void UpdateScore(DWORD proc_id, LPVOID base){
  string s = to_string(score);
  for (int i = 0; i < s.size(); i++){
    buffer[2*(i + colCount*(rowCount - 3))] = s[i];
  }
  UpdateProcessMemory(proc_id, base, bufferSize);
  // cout << "score = " << score << endl;
}

void AlienMove(DWORD proc_id, LPVOID base, Alien a, int id){
  int x = a.GetX();
  int health = a.GetHealth();
  int y = 0;

  while (y <= rowCount - 3){
    if (end_game == false){
      y = a.GetY();

      if (ALIENS[id].GetDestroyed() == false){
        buffer[2*(x + colCount*y)] = 0x23;
        if (y > 0){
          for (int i = 1; i <= y % health + 1; i++){
            buffer[2*(x + colCount*(y-i) - 1)] = 0x23;
            buffer[2*(x + colCount*(y-i))] = 0x23;
            buffer[2*(x + colCount*(y-i) + 1)] = 0x23;
          }
        }
        if (y > health){
          buffer[2*(x + colCount*(y-health-1) - 1)] = 0x2e;
          buffer[2*(x + colCount*(y-health-1))] = 0x2e;
          buffer[2*(x + colCount*(y-health-1) + 1)] = 0x2e;
        }

        UpdateProcessMemory(proc_id, base, bufferSize);

        y++;
        a.SetY(y);
        ALIENS[id].SetY(y);
      }else{
        buffer[2*(x + colCount*(y - 1))] = 0x2e;
        for (int i = 1; i <= (y) % health + 1; i++){
          buffer[2*(x + colCount*((y)-i) - 3)] = 0x2e;
          buffer[2*(x + colCount*((y)-i) - 2)] = 0x2e;
          buffer[2*(x + colCount*((y)-i) - 1)] = 0x2e;
          buffer[2*(x + colCount*((y)-i))] = 0x2e;
          buffer[2*(x + colCount*((y)-i) + 1)] = 0x2e;
          buffer[2*(x + colCount*((y)-i) + 2)] = 0x2e;
          buffer[2*(x + colCount*((y)-i) + 3)] = 0x2e;
        }
        UpdateProcessMemory(proc_id, base, bufferSize);
        ALIENS.erase(ALIENS.begin() + id);
        score++;
        UpdateScore(proc_id, base);
        return;
        break;
      }
    
      Sleep(a.GetSpeed());
    }
    if (ALIENS[id].GetY() > rowCount - 3){
      end_game = true;
      PrintYouLose(proc_id, base);
    }
  }
}

void GunMove(DWORD proc_id, LPVOID base, Gun prev_g, Gun this_g){
  buffer[2*(prev_g.GetX() + (rowCount - 1)*colCount)] = 0x2e;
  buffer[2*(prev_g.GetX() + (rowCount - 2)*colCount)] = 0x2e;
  buffer[2*(this_g.GetX() + (rowCount - 1)*colCount)] = 0x23;
  buffer[2*(this_g.GetX() + (rowCount - 2)*colCount)] = 0x23;
  UpdateProcessMemory(proc_id, base, bufferSize);
}

float GetDistance(Alien a, Bullet b){
  float x_diff_square = (a.GetX() - b.GetX())*(a.GetX() - b.GetX());
  float y_diff_square = (a.GetY() - b.GetY())*(a.GetY() - b.GetY());
  return (sqrt(x_diff_square + y_diff_square));
}

void DeleteAlien(Alien a){
  for (int i = 0; i < ALIENS.size(); i++){
    if (a == ALIENS[i]){
      ALIENS[i].SetDestroyed(true);
    }
  }
}

void DeleteBullet(Bullet b){
  for (int i = 0; i < BULLETS.size(); i++){
    if (b == BULLETS[i]){
      BULLETS[i].SetDestroyed(true);
    }
  }
}

void IsCollision(Bullet b){
  for (Alien a: ALIENS){
    if (GetDistance(a, b) <= DISTANCE + 2){
      DeleteAlien(a);
      DeleteBullet(b);
    }
  }
}

void BulletMove(DWORD proc_id, LPVOID base, Bullet b, int id){
  int y = b.GetY();
  int x = b.GetX();
  while(y >= 0){
    y = b.GetY();
    IsCollision(b);
    if (BULLETS[id].GetDestroyed() == false){
      buffer[2*(x + colCount*y)] = 0x7c;
      if (y <= 31){
        buffer[2*(x + colCount*(y+1))] = 0x2e;
      }
      buffer[2*x] = 0x2e;
      UpdateProcessMemory(proc_id, base, bufferSize); 
      y--;
      b.SetY(y);
      BULLETS[id].SetY(y);
    }
    Sleep(80);
  }  
  BULLETS.erase(BULLETS.begin() + id);
}


void Display(DWORD proc_id, LPVOID base, Alien a){
  for (int i = 0; i < ALIENS.size(); i++){
    if (ALIENS[i] == a){
      thread t(AlienMove, proc_id, base, ALIENS[i], i);

      t.detach(); 
    }
  }
}

void SpawnAlien(DWORD proc_id, LPVOID base){
  while(true){
    alien_id++;
    Alien a(2, alien_id);
    ALIENS.push_back(a);

    thread t1(Display, proc_id, base, a);

    t1.join();

    Sleep(spawn_rate);

  }
}

void Shoot(DWORD proc_id, LPVOID base, Bullet b, int bullet_id){
  thread t(BulletMove, proc_id, base, b, bullet_id);
  t.detach();
}

void Server(DWORD proc_id, LPVOID base, Gun original){
  WSADATA wsaData;

  Gun prev_gun = original;
  Gun this_gun = original;

  SOCKET ListenSocket = INVALID_SOCKET;
  SOCKET ClientSocket = INVALID_SOCKET;

  struct addrinfo *result = NULL;
  struct addrinfo hints;

  int iSendResult;
  char recvbuf[DEFAULT_BUFLEN];
  int recvbuflen = DEFAULT_BUFLEN;
  
  WSAStartup(MAKEWORD(2,2), &wsaData);

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM; 
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

  bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

  freeaddrinfo(result);

  cout << "Listening..." << endl;

  listen(ListenSocket, 1);

  do {
      if (end_game == false){
        ClientSocket = accept(ListenSocket, NULL, NULL);
        int err = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (err > 0) {
            string output(recvbuf);
            int vkCode = stoi(output.substr(0, 2));
            if (vkCode == VK_RIGHT){
              cout << "ALIENS.size() = " << ALIENS.size() << endl;
              cout << "score = " << score << endl;
              this_gun.SetX(prev_gun.GetX() + 1);
              GunMove(proc_id, base, prev_gun, this_gun);
              prev_gun.SetX(prev_gun.GetX() + 1);
            }else if(vkCode == VK_LEFT){
              cout << "BULLETS.size() = " << BULLETS.size() << endl;
              cout << "score = " << score << endl;
              this_gun.SetX(prev_gun.GetX() - 1);
              GunMove(proc_id, base, prev_gun, this_gun);
              prev_gun.SetX(prev_gun.GetX() - 1);
            }else if(vkCode == VK_UP){
              // cout << "this_gun.GetX() = " << this_gun.GetX() << endl;
              bullet_id++;
              Bullet b = Bullet(this_gun.GetX(), bullet_id);
              BULLETS.push_back(b);
              Shoot(proc_id, base, b, bullet_id);
            }
        }
      }
  } while (true);

  shutdown(ClientSocket, SD_SEND);

  closesocket(ClientSocket);
  WSACleanup();
}

void Launch(DWORD proc_id, LPVOID found_base, DWORD thread_id){
  UpdateScore(proc_id, found_base);

  Gun original_gun = Gun(10);
  Gun prev_gun = Gun(0);

  GunMove(proc_id, found_base, prev_gun, original_gun);
  cout << "Initiate display sequence" << endl;

  thread t1(SpawnAlien, proc_id, found_base);
  thread t2(Server, proc_id, found_base, original_gun);

  InstallHook(thread_id);

  t1.join();
  t2.join();

}


