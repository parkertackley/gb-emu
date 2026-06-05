#include "cart.h"

/**
 * Represents the physical ROM cartridge data and file metadata
 */
typedef struct {
  char filename[1024];
  u32 rom_size;
  u8 *rom_data;
  rom_header *header;
} cart_context;

/**
 * Initialization of cart_context object
 */
static cart_context ctx;

/**
 * Lookup table for Game Boy Cartridge Types
 */
static const char
*ROM_TYPES[] = {
	[0x00] = "ROM ONLY",
	[0x01] = "MBC1",
	[0x02] = "MBC1+RAM",
	[0x03] = "MBC1+RAM+BATTERY",
	[0x05] = "MBC2",
	[0x06] = "MBC2+BATTERY",
	[0x08] = "ROM+RAM",
	[0x09] = "ROM+RAM+BATTERY",
	[0x0B] = "MMM01",
	[0x0C] = "MMM01+RAM",
	[0x0D] = "MMM01+RAM+BATTERY",
	[0x0F] = "MBC3+TIMER+BATTERY",
	[0x10] = "MBC3+TIMER+RAM+BATTERY",
	[0x11] = "MBC3",
	[0x12] = "MBC3+RAM",
	[0x13] = "MBC3+RAM+BATTERY",
	[0x19] = "MBC5",
	[0x1A] = "MBC5+RAM",
	[0x1B] = "MBC5+RAM+BATTERY",
	[0x1C] = "MBC5+RUMBLE",
	[0x1D] = "MBC5+RUMBLE+RAM",
	[0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
	[0x20] = "MBC6",
	[0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
	[0xFC] = "POCKET CAMERA",
	[0xFD] = "BANDAI TAMA5",
	[0xFE] = "HuC3",
	[0xFF] = "HuC1+RAM+BATTERY",
};

/**
 * Lookup table for Game Boy Cartridge New Licensee code
 */
static const char
*LIC_CODE[0xA5] = {
  [0x00] = "None",
  [0x01] = "Nintendo Research & Development 1",
  [0x08] = "Capcom",
  [0x13] = "EA (Electronic Arts)",
  [0x18] = "Hudson Soft",
  [0x19] = "B-AI",
  [0x20] = "KSS",
  [0x22] = "Planning Office WADA",
  [0x24] = "PCM Complete",
  [0x25] = "San-X",
  [0x28] = "Kemco",
  [0x29] = "SETA Corporation",
  [0x30] = "Viacom",
  [0x31] = "Nintendo",
  [0x32] = "Bandai",
  [0x33] = " Ocean Software/Acclaim Entertainment",
  [0x34] = "Konami",
  [0x35] = "HectorSoft",
  [0x37] = "Taito",
  [0x38] = "Hudson Soft",
  [0x39] = "Banpresto",
  [0x41] = "Ubi Soft1",
  [0x42] = "Atlus",
  [0x44] = "Malibu Interactive",
  [0x46] = "Angel",
  [0x47] = "Bullet-Proof Software2",
  [0x49] = "Irem",
  [0x50] = "Absolute",
  [0x51] = "Acclaim Entertainment",
  [0x52] = "Activision",
  [0x53] = "Sammy USA Corporation",
  [0x54] = "Konami",
  [0x55] = "Hi Tech Expressions",
  [0x56] = "LJN",
  [0x57] = "Matchbox",
  [0x58] = "Mattel",
  [0x59] = "Milton Bradley Company",
  [0x60] = "Titus Interactive",
  [0x61] = "Virgin Games Ltd.3",
  [0x64] = "Lucasfilm Games4",
  [0x67] = "Ocean Software",
  [0x69] = "EA (Electronic Arts)",
  [0x70] = "Infogrames5",
  [0x71] = "Interplay Entertainment",
  [0x72] = "Broderbund",
  [0x73] = "Sculptured Software6",
  [0x75] = "The Sales Curve Limited7",
  [0x78] = "THQ",
  [0x79] = "Accolade8",
  [0x80] = "Misawa Entertainment",
  [0x83] = "LOZC G.",
  [0x86] = "Tokuma Shoten",
  [0x87] = "Tsukuda Original",
  [0x91] = "Chunsoft Co.9",
  [0x92] = "Video System",
  [0x93] = "Ocean Software/Acclaim Entertainment",
  [0x95] = "Varie",
  [0x96] = "Yonezawa10/S’Pal",
  [0x97] = "Kaneko",
  [0x99] = "Pack-In-Video",
  [0xA4] = "Konami (Yu-Gi-Oh!)"
};

/**
 * Maps the cartridge licensee code to the corresponding string in LIC_CODE
 *
 * @return const char* A pointer to the static publisher string
 * @return "UNKNOWN" if the code is invalid or unmapped
 */
const char
*cart_lic_name()
{
  if (ctx.header->new_lic_code <= 0xA5)
  {
    return LIC_CODE[ctx.header->new_lic_code];
  }
  return "UNKNOWN";
}

/**
 * Maps the cartridge type to the corresponding string in ROM_TYPES
 *
 * @return const char* A pointer to the static rom type string
 * @return "UNKNOWN" if the code is invalid or unmapped
 */
const char
*cart_type_name()
{
  if (ctx.header->type <= 0x1B)
  {
    return ROM_TYPES[ctx.header->type]; 
  }
  return "UNKNOWN";
}

/**
 * Loads a ROM file from disk, allocates internal memory buffers,
 * and validates the hardware header metadata
 *
 * 1. Streams file to determine its size
 * 2. Allocates heap buffer to copy ROM into memory
 * 3. Maps rom_header directly over memory offset 0x100
 * 4. Parses internal metrics
 * 5. Executes a checksum to ensure data integrity
 *
 * @warning Memory allocation: ctx.rom_data must be freed during system cleanup
 *
 * @param cart Name of the file to be opened
 * @return true		Success, the file was read
 * @return false	Error, the file could not be opened
 */
bool
cart_load(char *cart)
{
  snprintf(ctx.filename, sizeof(ctx.filename), "%s", cart);
  
  /* Open ROM file */
  FILE* fptr;
  fptr = fopen(cart, "r");
  if(fptr == NULL)
  {
    printf("File could not be opened!\n");
    return false;
  }
  printf("%s opened!\n", ctx.filename);

  /* Get size of ROM */
  fseek(fptr, 0, SEEK_END);
  ctx.rom_size = ftell(fptr);
  rewind(fptr);

  /* Read ROM into memory */
  ctx.rom_data = malloc(ctx.rom_size);
  fread(ctx.rom_data, ctx.rom_size, 1, fptr);
  fclose(fptr);

  /* Header starts at 0x100, so we populate from here */
  ctx.header = (rom_header *)(ctx.rom_data + 0x100);
  ctx.header->title[15] = 0;
  
  /* Print information about the cartridge */
  printf("\t Title    : %s\n", ctx.header->title);
  printf("\t Type     : %2.2X (%s)\n", ctx.header->type, cart_type_name());
  printf("\t ROM Size : %d KB\n", 32 << ctx.header->rom_size);
  printf("\t RAM Size : %2.2X\n", ctx.header->ram_size);
  printf("\t LIC Code : %2.2X (%s)\n", ctx.header->new_lic_code, cart_lic_name());
  printf("\t ROM Vers : %2.2X\n", ctx.header->version);

  /* Checksum to ensure ROM was loaded correctly */
  u8 checksum = 0;
  for(u16 address = 0x134; address <= 0x014C; address++)
  {
    checksum = checksum - ctx.rom_data[address] - 1;
  }
  printf("\t Checksum : %2.2X (%s)\n", ctx.header->checksum, (checksum & 0xFF) ? "PASSED" : "FAILED");

  return true;

}

/**
 * Reads a single byte of data from the cartridge ROM at the specified memory address
 *
 * @param address A 16-bit memory address from the CPU bus
 * @return u8 The 8-bit byte of data stored at the ROM address
 */
u8
cart_read(const u16 address)
{
  /* TODO: For now, just rom type supported */
  return ctx.rom_data[address];
}

/**
 * Writes the given 8-bit value to the cartridge ROM at the specified memory address
 * 
 * @param address A 16-bit memory address from the CPU bus
 * @param value An 8-bit value to be written
 */
void
cart_write(u16 address, u8 value)
{
  // TODO: implement this
}

