#include "cart.h"
#include "common.h"

u8 bus_read (u16 address)
{
  /* Read from cart */
  if(address < 0x8000)
  {
    return cart_read(address);
  }

  /* Not yet implemented */
  NO_IMPL

}

void bus_write (u16 address, u8 value)
{
  if(address < 0x8000)
  {
    return cart_write(address, value);
  }

  /* Not yet implemented */
  NO_IMPL
}

