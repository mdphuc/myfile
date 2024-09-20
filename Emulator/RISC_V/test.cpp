#include <iostream>
#include <cstdint>
#include <cstring>
#include <time.h>

using namespace std;

int main(){
    // char *buff;
    // time_t now = time(NULL);
    // strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    // FILE *file = fopen("power_trace.txt", "w");
    // fprintf(file, buff);

    FILE *file = fopen("opwer_trace.txt", "a");
    fprintf(file, to_string(1).c_str());

}