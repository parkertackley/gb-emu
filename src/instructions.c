#include "instructions.h"
#include "cpu.h"

instruction instructions[0x100] = {
  [0x0] = {IN_NOP, AM_IMP},

  [0x05] = {IN_DEC, AM_R, RT_B},

  [0x0E] = {IN_LD, AM_R_D8, RT_C},

  [0xAF] = {IN_XOR, AM_R, RT_A},

  [0xC3] = {IN_JP, AM_D16},

  [0xF3] = {IN_DI},

};

instruction
*instruction_by_opcode(const u8 opcode)
{
  return &instructions[opcode];
}

char *inst_lookup[] = {
  "<NONE>",
  "NOP",
  "LD",
  "INC",
  "DEC",
  "RLCA",
  "ADD",
  "RRCA",
  "STOP",
  "RLA",
  "JP",
  "RRA",
  "DAA",
  "CPL",
  "SCF",
  "CCF",
  "HALT",
  "ADC",
  "SUB",
  "SBC",
  "AND",
  "XOR",
  "OR",
  "CP",
  "RET",
  "POP",
  "CALL",
  "PUSH",
  "RST",
  "PREFIX",
  "RETI",
  "LDH",
  "DI",
  "EI",
  "RLC",
  "RRC",
  "RL",
  "RR",
  "SLA",
  "SRA",
  "SWAP",
  "SRL",
  "BIT",
  "RES",
  "SET"
};

char
*inst_name(const in_type t) {
  return inst_lookup[t];
}
