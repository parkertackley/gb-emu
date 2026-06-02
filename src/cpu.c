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
}

static void
fetch_instruction()
{
  ctx.curr_opcode = bus_read(ctx.registers.pc++);
  ctx.curr_inst = instruction_by_opcode(ctx.curr_opcode);

  if (ctx.curr_inst == NULL)
  {
    printf("Unknown instruction! %02X\n", ctx.curr_opcode);
    exit(-7);
  }

}

static void
fetch_data()
{
  ctx.mem_dest = 0;
  ctx.dest_is_mem = false;

  switch(ctx.curr_inst->mode)
  {
    case AM_IMP:
      return;

    case AM_R:
      ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_1);
      return;

    case AM_R_D8:
      ctx.fetched_data = bus_read(ctx.registers.pc);
      emu_cycles(1);
      ctx.registers.pc++;
      return;

    case AM_D16:
    {
      u16 lo = bus_read(ctx.registers.pc);
      emu_cycles(1);

      u16 hi = bus_read(ctx.registers.pc);
      emu_cycles(1);

      ctx.fetched_data = lo | (hi << 8);

      ctx.registers.pc += 2;

      return;
    }

    default:
      printf("Unknown addressing mode! %d\n", ctx.curr_inst->mode);
      exit(1);

  }
}

static void
execute()
{

}

bool
cpu_step()
{
  if(!ctx.halted)
  {
    const u16 pc = ctx.registers.pc;
    fetch_instruction();
    fetch_data();
    printf("Executing instruction: %02X   PC: %04X\n", ctx.curr_opcode, pc);
    execute();
  }

  return true;
}
