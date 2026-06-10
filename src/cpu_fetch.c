#include "bus.h"
#include "cpu.h"
#include "cpu_util.h"
#include "emu.h"

extern cpu_context ctx;

/**
 *
 */
void
fetch_data()
{
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    if (ctx.curr_inst == NULL)
    {
        printf("null instruction\n");
        return;
    }

    switch(ctx.curr_inst->mode)
    {
        case AM_IMP:
            return;

        case AM_R:
            ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_1);
            return;

        case AM_R_R:
            ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_2);
            return;

        case AM_R_D8:
            ctx.fetched_data = bus_read(ctx.registers.pc);
            emu_cycles(1);
            ctx.registers.pc++;
            return;

        case AM_R_D16:
        case AM_D16:
        {
            const u16 lo = bus_read(ctx.registers.pc);
            emu_cycles(1);

            const u16 hi = bus_read(ctx.registers.pc + 1);
            emu_cycles(1);

            ctx.fetched_data = lo | (hi << 8);

            ctx.registers.pc += 2;

            return;
        }

        case AM_MR_R:
            ctx.fetched_data = bus_read(ctx.curr_inst->reg_2);
            ctx.mem_dest = bus_read(ctx.curr_inst->reg_1);
            ctx.dest_is_mem = true;

            if (ctx.curr_inst->reg_1 == RT_C)
            {
                ctx.mem_dest |= 0xFF00;
            }

            return;

        case AM_R_MR:
            u16 addr = cpu_read_reg(ctx.curr_inst->reg_2);

            if (ctx.curr_inst->reg_1 == RT_C)
            {
                addr |= 0xFF00;
            }

            ctx.fetched_data = bus_read(addr);
            emu_cycles(1);

            return;

        case AM_R_HLI:
            ctx.fetched_data = bus_read(cpu_read_reg(ctx.curr_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_R_HLD:
            ctx.fetched_data = bus_read(cpu_read_reg(ctx.curr_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_HLI_R:
            ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_2);
            ctx.mem_dest = cpu_read_reg(ctx.curr_inst->reg_1);
            ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_HLD_R:
            ctx.fetched_data = cpu_read_reg(ctx.curr_inst->reg_2);
            ctx.mem_dest = cpu_read_reg(ctx.curr_inst->reg_1);
            ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_R_A8:
            ctx.fetched_data = bus_read(ctx.registers.pc);
            emu_cycles(1);
            ctx.registers.pc++;
            return;

        case AM_A8_R:
            ctx.mem_dest = bus_read(ctx.registers.pc) | 0xFF00;
            ctx.dest_is_mem = true;
            emu_cycles(1);
            ctx.registers.pc++;
            return;

        case AM_HL_SPR:
            ctx.fetched_data = bus_read(ctx.registers.pc);
            emu_cycles(1);
            ctx.registers.pc++;
            return;

        case AM_D8:
            ctx.fetched_data = bus_read(ctx.registers.pc);
            emu_cycles(1);
            ctx.registers.pc++;
            return;

        case AM_MR_D8:
            // TODO: this

        default:
            printf("Unknown addressing mode! %d\n", ctx.curr_inst->mode);
            exit(-7);
    }
}
