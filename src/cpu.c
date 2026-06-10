#include "cpu.h"
#include "bus.h"
#include "cpu_util.h"
#include "emu.h"

cpu_context ctx = {0};

void cpu_init()
{
  // TODO: implement this
  /* Set entry point */
  ctx.registers.pc = 0x100;
  ctx.registers.a = 0x01;
}

static void
fetch_instruction()
{
  ctx.curr_opcode = bus_read(ctx.registers.pc++);
  ctx.curr_inst = instruction_by_opcode(ctx.curr_opcode);
}

static void
execute()
{
  const IN_PROC proc = inst_get_proc(ctx.curr_inst->type);

  if (!proc)
  {
    NO_IMPL
  }

  proc(&ctx);

}

bool
cpu_step()
{
  if(!ctx.halted)
  {
    const u16 pc = ctx.registers.pc;

    fetch_instruction();
    fetch_data();

    printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
            pc, inst_name(ctx.curr_inst->type), ctx.curr_opcode,
            bus_read(pc + 1), bus_read(pc + 2), ctx.registers.a, ctx.registers.b, ctx.registers.c);

    execute();
  }

  return true;
}
