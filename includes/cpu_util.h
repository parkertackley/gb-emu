#pragma once

#include "common.h"
#include "cpu.h"
#include "instructions.h"

u16 reverse(u16 n);
u16 cpu_read_reg(reg_type reg);
void cpu_set_reg(reg_type reg_type, u16 value);
