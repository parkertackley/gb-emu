#pragma once

#include <common.h>

typedef struct {
  bool paused,
  bool running,
  u64 ticks;
} emu_context;

int emu_run(ing arc, char** argv);

emu_context *emu_get_context();
