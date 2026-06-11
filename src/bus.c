#include "cart.h"
#include "common.h"

u8
bus_read (u16 address)
{
  /* Read from cart */
  if(address < 0x8000)
  {
    return cart_read(address);
  }

  /* Not yet implemented */
  printf("Bus read!\n");
  NO_IMPL

}

void
bus_write (u16 address, u8 value)
{
  if(address < 0x8000)
  {
    cart_write(address, value);
  }

  /* Not yet implemented */
  printf("Bus write!\n");
  NO_IMPL
}

void
bus_write16(u16 address, u16 value)
{
  // TODO: implement
}

