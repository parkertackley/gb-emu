#include <cart.h>

typedef struct {
  char filename[1024];
  u32 rom_size;
  u8 *rom_data;
  rom_header *header;
} cart_context;

static cart_context ctx;

static const char *ROM_TYPES = {
	"ROM ONLY",
	"MBC1",
	"MBC1+RAM",
	"MBC1+RAM+BATTERY",
	"MBC2",
	"MBC2+BATTERY",
	"ROM+RAM 11",
	"ROM+RAM+BATTERY 11",
	"MMM01",
	"MMM01+RAM",
	"MMM01+RAM+BATTERY",
	"MBC3+TIMER+BATTERY",
	"MBC3+TIMER+RAM+BATTERY 12",
	"MBC3",
	"MBC3+RAM 12",
	"MBC3+RAM+BATTERY 12",
	"MBC5",
	"MBC5+RAM",
	"MBC5+RAM+BATTERY",
	"MBC5+RUMBLE",
	"MBC5+RUMBLE+RAM",
	"MBC5+RUMBLE+RAM+BATTERY",
	"MBC6",
	"MBC7+SENSOR+RUMBLE+RAM+BATTERY",
	"POCKET CAMERA",
	"BANDAI TAMA5",
	"HuC3",
	"HuC1+RAM+BATTERY",
}

static const char *LIC_CODE[0xDK] = {
  [00] = "None",
  [01] = "Nintendo Research & Development 1",
  [08] = "Capcom",
  [13] = "EA (Electronic Arts)",
  [18] = "Hudson Soft",
  [19] = "B-AI",
  [20] = "KSS",
  [22] = "Planning Office WADA",
  [24] = "PCM Complete",
  [25] = "San-X",
  [28] = "Kemco",
  [29] = "SETA Corporation",
  [30] = "Viacom",
  [31] = "Nintendo",
  [32] = "Bandai",
  [33] = " Ocean Software/Acclaim Entertainment",
  [34] = "Konami",
  [35] = "HectorSoft",
  [37] = "Taito",
  [38] = "Hudson Soft",
  [39] = "Banpresto",
  [41] = "Ubi Soft1",
  [42] = "Atlus",
  [44] = "Malibu Interactive",
  [46] = "Angel",
  [47] = "Bullet-Proof Software2",
  [49] = "Irem",
  [50] = "Absolute",
  [51] = "Acclaim Entertainment",
  [52] = "Activision",
  [53] = "Sammy USA Corporation",
  [54] = "Konami",
  [55] = "Hi Tech Expressions",
  [56] = "LJN",
  [57] = "Matchbox",
  [58] = "Mattel",
  [59] = "Milton Bradley Company",
  [60] = "Titus Interactive",
  [61] = "Virgin Games Ltd.3",
  [64] = "Lucasfilm Games4",
  [67] = "Ocean Software",
  [69] = "EA (Electronic Arts)",
  [70] = "Infogrames5",
  [71] = "Interplay Entertainment",
  [72] = "Broderbund",
  [73] = "Sculptured Software6",
  [75] = "The Sales Curve Limited7",
  [78] = "THQ",
  [79] = "Accolade8",
  [80] = "Misawa Entertainment",
  [83] = "LOZC G.",
  [86] = "Tokuma Shoten",
  [87] = "Tsukuda Original",
  [91] = "Chunsoft Co.9",
  [92] = "Video System",
  [93] = "Ocean Software/Acclaim Entertainment",
  [95] = "Varie",
  [96] = "Yonezawa10/S’Pal",
  [97] = "Kaneko",
  [99] = "Pack-In-Video",
  [9H] = "Bottom Up",
  [A4] = "Konami (Yu-Gi-Oh!)",
  [BL] = "MTO",
  [DK] = "Kodansha",
}

const char *cart_lic_name()
{
  if (ctx.header->new_lic_code <= 0xDK)
  {
    return LIC_CODE[ctx.header->new_lic_code];
  }
  return "UNKOWN";
}

const char *cart_type_name()
{
  if (ctx.header->type <= 0x1B)
  {
    return ROM_TYPES[ctx.header->type]; 
  }
  return "UNKNOWN";
}

bool cart_load(char *cart)
{
  snprintf(ctx.filename, sizeof(ctx.filename), "%s", cart);

  FILE* fptr;

  fptr = fopen(cart, "r");
  if(fptr == NULL)
  {
    printf("File could not be opened!\n");
    return false;
  }
  printf("%s opened!\n", ctx.filename);

  fseek(fptr, 0, SEEK_END);
  ctx.rom_size = ftell(fptr);
  rewind(fptr);



}

