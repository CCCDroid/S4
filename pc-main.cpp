#define __PC__
#ifdef __PC__

#ifdef _WIN32
#include <Windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "s4.h"

typedef unsigned long DWORD;
typedef unsigned long HANDLE;
typedef unsigned char byte;

// These are used only be the PC version
static HANDLE hStdOut = 0;
static char input_fn[32];

// These are in the <Arduino.h> file
int analogRead(int pin) { printStringF("-AR(%d)-", pin); return 0; }
void analogWrite(int pin, int val) { printStringF("-AW(%d,%d)-", pin, val); }
int digitalRead(int pin) { printStringF("-DR(%d)-", pin); return 0; }
void digitalWrite(int pin, int val) { printStringF("-DW(%d,%d)-", pin, val); }
void pinMode(int pin, int mode) { printStringF("-pinMode(%d,%d)-", pin, mode); }
void delay(DWORD ms) { usleep(ms*1000); }

void printString(const char* str) {
//    DWORD n = 0, l = strlen(str);
//    if (l) { WriteConsoleA(hStdOut, str, l, &n, 0); }
      fputs(str, stdout);
}

// Returns the number of ticks since an undefined time (usually system startup).
static long GetTickCountMs()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (long)(ts.tv_nsec / 1000000) + ((uint64_t)ts.tv_sec * 1000ull);
}

long millis() { return GetTickCountMs(); }

void ok() {
    printString("\r\nS4:"); dumpStack(0); printString(">");
}

void doHistory(const char* txt) {
    FILE* fp = NULL;
    fp = fopen( "history.txt", "at");
    if (fp) {
        fprintf(fp, "%s", txt);
        fclose(fp);
    }
}

void strToTIB(addr loc, const char *txt) {
    while (*txt) {
        setCodeByte(loc++, *(txt++));
    }
    setCodeByte(loc++, 0);
}

void loop() {
    char tib[100];
    addr nTib = CODE_SZ - 100;
    FILE* fp = (input_fp) ? input_fp : stdin;
    if (fp == stdin) { ok(); }
    if (fgets(tib, 100, fp) == tib) {
        if (fp == stdin) { doHistory(tib); }
        strToTIB(nTib, tib);
        run(nTib);
        return;
    }
    if (input_fp) {
        fclose(input_fp);
        input_fp = NULL;
    }
}

void process_arg(char* arg)
{
    if ((*arg == 'i') && (*(arg + 1) == ':')) {
        arg = arg + 2;
        strcpy(input_fn, arg);
    }
    else if (*arg == '?') {
        printString("usage s4 [args] [source-file]\n");
        printString("  -i:file\n");
        printString("  -? - Prints this message\n");
        exit(0);
    }
    else { printf("unknown arg '-%s'\n", arg); }
}

int main(int argc, char** argv) {
//    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//    DWORD m; GetConsoleMode(hStdOut, &m);
//    SetConsoleMode(hStdOut, (m | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
    vmInit();
    // 0(111: dump all code currently defined)
    strToTIB(100, "{a11 iFnn0Hq[xId@#,125=(n)]}");
    run(100);

    strcpy(input_fn, "");
    input_fp = NULL;

    for (int i = 1; i < argc; i++)
    {
        char* cp = argv[i];
        if (*cp == '-') { process_arg(++cp); }
        else { strcpy(input_fn, cp); }
    }

    if (strlen(input_fn) > 0) {
        input_fp = fopen(input_fn, "rt");
    }

    while (isBye == 0) { loop(); }
}

#endif // __PC__
