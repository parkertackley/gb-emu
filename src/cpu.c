#include <cpu.h>

cpu_context ctx = {0};

void cpu_init()
{
  // TODO: implement this
}

static void fetch_instruction()
{
  ctx.curr_opcode = bus_read(ctx.regs.pc++);
  ctx.curr_inst = instruciton_by_opcode(ctx.curr_opcode);
}

static void fetch_data()
{
  ctx.mem_dest = 0;
  cpu.dest_is_mem = false;

  swtich(ctx.curr_inst->mode)
  {
    case AM_IMP:
      return;

    case AM_R:
      ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_1);
      return;

    
  }
}

static void execute()
{

}

bool cpu_step()
{
  if(!ctx.halted)
  {
    fetch_instruction();
    fetch_data();
    execute();
  }

  return false;
}
