#include "cpu.h"
#include "emu.h"

static void
proc_none(cpu_context *ctx)
{
    printf("Invalid instruction! proc_none\n");
    exit(-7);
}

static void
proc_ld(cpu_context *ctx)
{
    // TODO: implement this shit
}

static bool
check_cond(cpu_context *ctx)
{
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;

    switch (ctx->curr_inst->cond) {
        case CT_NONE:
            return true;
        case CT_C:
            return c;
        case CT_NC:
            return !c;
        case CT_Z:
            return z;
        case CT_NZ:
            return !z;
        default:
            return false;
    }

    return false;

}

static void
proc_jp(cpu_context *ctx)
{
    if (check_cond(ctx))
    {
        ctx->registers.pc = ctx->fetched_data;
        emu_cycles(1);
    }
}

static void
proc_nop(cpu_context *ctx)
{
    /* Does nothing */
}

static void
proc_di(cpu_context *ctx)
{
    ctx->int_master_enabled = false;
}

static
IN_PROC processors[] = {
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di,
};

IN_PROC
inst_get_proc(in_type type)
{
    return processors[type];
}
