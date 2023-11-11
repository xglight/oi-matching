#include <cstdlib>
#include <stdio.h>
#include <windows.h>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
using namespace std;
int main(int argc, char **argv) {
    char ch[105];
    sprintf(ch, "timeout /t %s", argv[0]), system(ch);
    sprintf(ch, "taskkill /f /im %s", argv[1]), system(ch);
    return 0;
}