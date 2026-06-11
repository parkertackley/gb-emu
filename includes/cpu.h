#pragma once  

#include "common.h"
#include "instructions.h"

typedef struct {
  u8 a;
  u8 f;
  u8 b;
  u8 c;
  u8 d;
  u8 e;
  u8 h;
  u8 l;
  u16 sp;
  u16 pc;
} cpu_registers;

/**
 * 1. Store registers directly in the cpu context for fastest lookup time
 * 2. Instruction points to a large lookup table so we don't have to
 *    copy the lookup table everytime a new instruction is fetched.
 */
typedef struct {
  /* Registers */
  cpu_registers registers;

  /* Current instruction */
  instruction *curr_inst;

  /* Current fetch */
  u16 fetched_data;

  /* Write location */
  u16 mem_dest;

  /* Current opcode */
  u8 curr_opcode;

  /* If the write location is memory (false if writing to register) */
  bool dest_is_mem;

  /* State */
  bool halted;
  bool stepping;

  /* Allowing interrupts */
  bool int_master_enabled;

} cpu_context;

void cpu_init();
bool cpu_step();

typedef void (*IN_PROC)(cpu_context *);
IN_PROC inst_get_proc(in_type type);
void fetch_data();

#define CPU_FLAG_Z BIT(ctx->registers.f, 7)
#define CPU_FLAG_C BIT(ctx->registers.f, 4)
