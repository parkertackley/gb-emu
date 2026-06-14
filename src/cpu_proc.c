#include "bus.h"
#include "cpu.h"
#include "cpu_util.h"
#include "emu.h"

/**
 * Handles invalid or unimplemented CPU instructions by terminating the emulator
 *
 * @warning This function terminates the program
 *
 * @param ctx Pointer to the active CPU context
 */
static void
proc_none(cpu_context *ctx)
{
    printf("Invalid instruction! proc_none\n");
    exit(-7);
}

/**
 * TODO: implement and document
 *
 * @param ctx Pointer to the active CPU context
 */
static void
proc_ld(cpu_context *ctx)
{
    if (ctx->dest_is_mem)
    {
        /* If address is 16 bit value */
        if (ctx->curr_inst->reg_2 >= RT_AF)
        {
            emu_cycles(1);
            bus_write16(ctx->mem_dest, ctx->fetched_data);
        }
        else
        {
            bus_write(ctx->mem_dest, ctx->fetched_data);
        }
        return;
    }

    if (ctx->curr_inst->mode == AM_HL_SPR)
    {
        u8 hflag = (cpu_read_reg(ctx->curr_inst->reg_2) & 0xF) +
            (ctx->fetched_data & 0xF) >= 0x10;

        u8 cflag = (cpu_read_reg(ctx->curr_inst->reg_2) & 0xFF) +
            (ctx->fetched_data & 0xFF) >= 0x100;

        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->curr_inst->reg_1, cpu_read_reg(ctx->curr_inst->reg_2) + (char)ctx->fetched_data);

        return;
    }

    /* Main case -> set register to fetched data */
    cpu_set_reg(ctx->curr_inst->reg_1, ctx->fetched_data);
}

/**
 * @brief Evaluates if a conditional instruction's criteria are met
 *
 * Some instructions only execute if specific flags match a given condition.
 * This function reads the current instructions conditions and tests against
 * the active CPU's F register flags
 *
 * @param ctx Pointer to the active CPU context
 * @return true The condition was met, otherwise, return false
 */
static bool
check_cond(const cpu_context *ctx)
{
    const bool z = CPU_FLAG_Z;
    const bool c = CPU_FLAG_C;

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
}

/**
 * Executes the Jump (JP) instruction
 *
 * @note The conditions must be met in order to successfully jump
 *      A cycle penalty must be used if the jump is successful
 *
 * @param ctx Pointer to the active CPU context
 */
static void
proc_jp(cpu_context *ctx)
{
    if (check_cond(ctx))
    {
        ctx->registers.pc = ctx->fetched_data;
        emu_cycles(1);
    }
}

/**
 * Executes the No OPeration (NOP) instruction
 *
 * @note This does nothing
 *
 * @param ctx Pointer to the active CPU context
 */
static void
proc_nop(cpu_context *ctx)
{
    /* Does nothing */
}

/**
 * Executes the Disable Interrupts (DI) instruction
 *
 * @note This clears the IME flag
 *
 * @param ctx Pointer to the active CPU context
 */
static void
proc_di(cpu_context *ctx)
{
    ctx->int_master_enabled = false;
}

/**
 * Updates the CPU's flags register (F) based on the provided values
 *
 * @note Register F Bit layout:
 * Bit 7: z - Zero flag
 * Bit 6: n - Subtraction flag
 * Bit 5: h - Half Carry flag
 * Bit 4: c - Carry flag
 *
 * @param ctx Pointer to active CPU context
 * @param z Zero flag state: [1 flag is set, 0 flag is reset, -1 flag is left untouched]
 * @param n Subtraction flag state: ^
 * @param h Half carry flag state: ^
 * @param c Carry Flag state: ^
 */
void
cpu_set_flags(cpu_context *ctx, const char z, const char n, const char h, const char c)
{
    if (z != -1)
    {
        BIT_SET(ctx->registers.f, 7, z);
    }

    if (n != -1)
    {
        BIT_SET(ctx->registers.f, 6, n);
    }

    if (h != -1)
    {
        BIT_SET(ctx->registers.f, 5, h);
    }

    if (c != -1)
    {
        BIT_SET(ctx->registers.f, 4, c);
    }
}

/**
 * Executes the Bitwise XOR (XOR) instruction against the A register.
 * The result is stored back into the A register
 *
 * @note Flag Alterations:
 *  Z: Set if XOR result is 0
 *  N: Always cleared
 *  H: Always cleared
 *  C: Always cleared
 *
 * @param ctx Pointer to the active CPU context
 */
static void
proc_xor(cpu_context *ctx)
{
    /* XOR the A register with the value in the fetched register
     * We only care about the bottom byte of fetched_data, so we & 0xFF
     */
    ctx->registers.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->registers.a == 0, 0, 0, 0);
}

/**
 * Instruction lookup table
 *
 * @note Any instruction enum value that is not explicitly listed here
 * will default to 'NULL' pointer
 *
 */
static
IN_PROC processors[] = {
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di,
    [IN_XOR] = proc_xor,
};

IN_PROC
inst_get_proc(const in_type type)
{
    return processors[type];
}
