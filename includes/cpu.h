#pragma once  

#include "common.h"
#include "instructions.h"

typedef struct {
  u8 a;
  u8 b;
  u8 c;
  u8 d;
  u8 e;
  u8 f;
  u8 h;
  u8 l;
  u16 sp;
  u16 pc;
} cpu_registers;

typedef struct {
  cpu_registers registers;

  /* Current instruction */
  instruction *curr_inst;

  /* Current fetch */
  u16 fetched_data;

  u16 mem_dest;
  u8 curr_opcode;

  bool dest_is_mem;

  /* State */
  bool halted;
  bool stepping;

  bool int_master_enabled;

} cpu_context;

void cpu_init();
bool cpu_step();

typedef void (*IN_PROC)(cpu_context *);
IN_PROC inst_get_proc(in_type type);
void fetch_data();

#define CPU_FLAG_Z BIT(ctx->registers.f, 7)
#define CPU_FLAG_C BIT(ctx->registers.f, 4)
