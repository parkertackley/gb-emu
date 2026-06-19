#include "cart.h"
#include "common.h"

u8
bus_read (const u16 address)
{
  /* Read from cart */
  if(address < 0x8000)
  {
    return cart_read(address);
  } else if (address < 0xA000)
  {
    printf("UNSUPPORTED Bus read! (%04X)\n", address);
    NO_IMPL
  } else if (address < 0xC000)
  {
    return cart_read(address);
  } else if (address < 0xE000)
  {
    /* WRAM */
    return wram_read(address);
  } else if (address < 0xFE00)
  {
    return 0;
  } else if (0xFEA0)
  {
    printf("UNSUPPORTED Bus read! (%04X)\n", address);
    NO_IMPL
  } else if (address < 0xFF00)
  {
    return 0;
  } else if (address < 0xFF80)
  {
    printf("UNSUPPORTED Bus read! (%04X)\n", address);
    NO_IMPL
  } else if (address < 0xFFFF)
  {
    printf("UNSUPPORTED Bus read! (%04X)\n", address);
    NO_IMPL
  }

  hram_read(address);
}

void
bus_write (const u16 address, const u8 value)
{
  if(address < 0x8000)
  {
    cart_write(address, value);
  }

  /* Not yet implemented */
  printf("UNSUPPORTED Bus write! (%04X)\n", address);
  // NO_IMPL
}

u16
bus_read16(const u16 address)
{
  printf("Bus read 16! (%04X)\n", address);

  const u16 lo = bus_read(address);
  const u16 hi = bus_read(address + 1);

  return lo | (hi << 8);
}

void
bus_write16(const u16 address, const u16 value)
{
  printf("Bus write 16! (%04X)\n", address);
  bus_write(address + 1, (value >> 8) & 0xFF);
  bus_write(address, value & 0xFF);
}
