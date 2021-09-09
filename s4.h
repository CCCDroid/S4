// S4 - a stack VM, inspired by Sandor Schneider's STABLE - https://w3group.de/stable.html

typedef unsigned char byte;
#define MAX_FUNC (52*62)
#define MEM_SZ (MEM_SZB/4)

extern void vmInit();
extern int run(int pc);
extern void dumpStack(int hdr);
extern void setCodeByte(int addr, char ch);
extern long registerVal(int reg);
extern int functionAddress(const char* fname);
extern void printString(const char*);
extern void printStringF(const char* fmt, ...);

#ifdef _WIN32
#define __PC__
#define INPUT 0
#define INPUT_PULLUP 1
#define OUTPUT 2
extern long millis();
extern int analogRead(int pin);
extern void analogWrite(int pin, int val);
extern int digitalRead(int pin);
extern void digitalWrite(int pin, int val);
extern void pinMode(int pin, int mode);
extern void delay(unsigned long ms);
extern FILE* input_fp;
extern byte isBye;
#define CODE_SZ      (1024*64)
#define MEM_SZB      (1024*256)
#define NUM_FUNCS     MAX_FUNC
#else
#define _DEV_BOARD_
#include <Arduino.h>
#define CODE_SZ      (64*1024)  // PICO
#define MEM_SZB      (96*1024)  // PICO
#define NUM_FUNCS    MAX_FUNC   // PICO
#define ILED          25        // PICO
//#define CODE_SZ      (16*1024)  // XIAO
//#define MEM_SZB      ( 8*1024)  // XIAO
//#define NUM_FUNCS    (13*62)    // XIAO
//#define ILED          13        // XIAO
#endif // _WIN32
