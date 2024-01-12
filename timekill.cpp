#include <cstdlib>
#include <stdio.h>
#include <windows.h>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
using namespace std;
int char_int(char *c) {
    int r = 0;
    for (int i = 0; i < strlen(c); i++)
        r = r * 10 + (c[i] - '0');
    return r;
}
int main(int argc, char **argv) {
    char ch[105];
    Sleep(char_int(argv[1]) * 1000);
    sprintf(ch, "taskkill /f /im %s", argv[2]), system(ch);
    return 0;
}