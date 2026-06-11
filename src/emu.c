#include "stdio.h"
#include "emu.h"
#include "cart.h"
#include "cpu.h"
#include "SDL2/SDL.h"
#include "SDL_ttf.h"

/*  
  |CART|
  |CPU|
  |Addr Bus|
  |PPU|
  |Timer|
*/

static emu_context ctx;

/**
 * Provides global access to the core emulator context
 *
 * @warning Returns a mutable pointer. Modifying this directly impacts the state of the emulation thread.
 *
 * @return emu_context* Pointer to the active emulator context
 */
emu_context
*get_emu_context(void)
{
  return &ctx;
}

/**
 * Pauses execution of the current thread for specified duration
 *
 * @param ms Time in milliseconds to pause the thread
 */
void
delay(const u32 ms)
{
  SDL_Delay(ms);
}

/**
 * Main execution entry point for the emulator
 *
 * 1. Validate CLI arguments
 * 2. Load ROM cartridge
 * 3. INIT SDL subsystems
 * 4. Bootstrap CPU
 * 5. Execute game loop until exit condition is met
 *
 * @param argc Number of arguments passed at the command line
 * @param argv Array of command line arguments
 *
 * @return 0  Success The emulation completed cleanly
 * @return -1 Error Missing CLI arguments (ROM name not provided)
 * @return -2 Error Cartridge could not be loaded
 * @return -3 Error CPU encountered an error
 */
int
emu_run(const int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Usage: emu <rom_file>\n");
    return -1;
  }

  if (!cart_load(argv[1]))
  {
    printf("Failed to load ROM file: %s\n", argv[1]);
    return -2;
  }

  printf("Cart loaded...\n");
  SDL_Init(SDL_INIT_VIDEO);
  printf("SDL Init\n");
  TTF_Init();
  printf("TTF INIT\n");

  cpu_init();
  
  ctx.running = true;
  ctx.paused = false;
  ctx.ticks = 0;

  /* Game loop */ 
  while (ctx.running)
  {
    if (ctx.paused)
    {
      delay(10);
      continue;
    }

    if (!cpu_step()) 
    {
      printf("CPU stopped!\n");
      return -3;
    }

    ctx.ticks++;

  }

  return 0;

}

/**
 * TODO:This
 *
 * @param cpu_cycles Number of CPU cycles to emulate
 */
void
emu_cycles(int cpu_cycles)
{
  // TODO: implement this
}
