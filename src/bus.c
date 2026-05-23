#include <bus.h>

u8 bus_read (u16 address)
{
  /* Read from cart */
  if(address < 0x8000)
  {
    return cart_read(address);
  }

  /* Not yet implemented */
  exit(-1);

}

void bus_write (u16 address, u8 value)
{
  if(address < 0x8000)
  {
    return cart_read(address, value);
  }

  /* Not yet implemented */
  exit(-1);

}

