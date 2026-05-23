#pragma once  

#include <common.h>

typedef struct {
  u8 a;
  u8 b;
  u8 c;
  u8 d;
  u8 e;
  u8 f;
  u8 g;
  u8 h;
  u8 l;
  u16 sp;
  u16 pc;
} cpu_registers;

void cpu_init();
bool cpu_step();

