#define SC_PEEK_LV2						(6)
#define SC_POKE_LV2						(7)
#define SC_PEEK_LV1 					(8)
#define SC_POKE_LV1 					(9)

#define PS3MAPI_OPCODE_LV2_PEEK			0x1006
#define PS3MAPI_OPCODE_LV2_POKE			0x1007
#define PS3MAPI_OPCODE_LV1_PEEK			0x1008
#define PS3MAPI_OPCODE_LV1_POKE			0x1009

#define SYSCALL8_OPCODE_PS3MAPI			0x7777
#define PS3MAPI_OPCODE_LV1_POKE			0x1009

#define CFW_SYSCALLS_REMOVED(a)			((lv2_peek_hen(a) & 0xFFFFFFFFFF000000) != 0x8000000000000000)

/////////////////// LV1 PEEK //////////////////////
static u64 lv1_peek_cfw(u64 addr)
{
	system_call_1(SC_PEEK_LV1, addr);
	return (u64) p1;
}

#ifdef COBRA_ONLY
static u64 lv1_peek_ps3mapi(u64 addr)
{
	system_call_3(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_PS3MAPI, PS3MAPI_OPCODE_LV1_PEEK, addr);
	return (u64) p1;
}
#endif

/////////////////// LV1 POKE //////////////////////
static void lv1_poke_cfw( u64 addr, u64 value)
{
	system_call_2(SC_POKE_LV1, addr, value);
}

#ifdef COBRA_ONLY
static void lv1_poke_ps3mapi(u64 addr, u64 value)
{
	system_call_4(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_PS3MAPI, PS3MAPI_OPCODE_LV1_POKE, addr, value);
}
#endif

/////////////////// LV2 PEEK //////////////////////
static u64 lv2_peek_cfw(u64 addr) //sc8 + LV2_OFFSET_ON_LV1
{
	system_call_1(SC_PEEK_LV1, addr + LV2_OFFSET_ON_LV1); //old: {system_call_1(SC_PEEK_LV2, addr);}
	return (u64) p1;
}

static u64 lv2_peek_hen(u64 addr) //sc6
{
	system_call_1(SC_PEEK_LV2, addr);
	return (u64) p1;
}

#ifdef COBRA_ONLY
static u64 lv2_peek_ps3mapi(u64 addr) //sc8 + ps3mapi
{
	system_call_3(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_PS3MAPI, PS3MAPI_OPCODE_LV2_PEEK, addr);
	return (u64) p1;
}

/////////////////// LV2 POKE //////////////////////
static void lv2_poke_hen(u64 addr, u64 value) //sc7
{
	system_call_2(SC_POKE_LV2, addr, value);
}
#endif

static void lv2_poke_cfw(u64 addr, u64 value) //sc8 + LV2_OFFSET_ON_LV1
{
	system_call_2(SC_POKE_LV1, addr + LV2_OFFSET_ON_LV1, value);
}

#ifdef COBRA_ONLY
static void lv2_poke_ps3mapi(u64 addr, u64 value) //sc8 + ps3mapi
{
	system_call_4(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_PS3MAPI, PS3MAPI_OPCODE_LV2_POKE, addr, value);
}
#endif
///////////////////////////////////////////////////

static void (*lv2_poke_fan)(u64, u64) = lv2_poke_cfw;  // ps3hen: lv2_poke_fan = lv2_poke_fan_hen;

static u64  (*peekq)(u64) = lv2_peek_cfw;
static void (*pokeq)(u64, u64) = lv2_poke_cfw;

static u64  (*peek_lv1)(u64) = lv1_peek_cfw;
static void (*poke_lv1)(u64, u64) = lv1_poke_cfw;

static u64 peek(u64 addr)
{
	return peekq(addr | 0x8000000000000000ULL);
}

#ifdef COBRA_ONLY
///////////////// LV1/LV2 POKE HEN ////////////////
static void lv2_poke_fan_hen(u64 addr, u64 value)
{
	if(payload_ps3hen && IS_INGAME)
	{
		CellRtcTick pTick; cellRtcGetCurrentTick(&pTick);
		if((pTick.tick - gTick.tick) < 15000000) return; // do not poke within the first 15 seconds ingame
	}

	system_call_2(SC_POKE_LV2, addr, value); //{system_call_3(SC_COBRA_SYSCALL8, 0x7003ULL, addr, value);} // advanced poke (requires restore original value)
}

static void lv1_poke_hen(u64 addr, u64 value)
{
	if(addr >= LV2_OFFSET_ON_LV1)
		pokeq((addr - LV2_OFFSET_ON_LV1) | 0x8000000000000000ULL, value);
	else
		poke_lv1(addr, value);
}

static u64 lv1_peek_hen(u64 addr)
{
	if(addr >= LV2_OFFSET_ON_LV1)
		return peek(addr - LV2_OFFSET_ON_LV1);
	else
		return peek_lv1(addr);
}
///////////////////////////////////////////////////
#endif

#ifndef LITE_EDITION
/***********************************************************************
* lv2 peek 32 bit
***********************************************************************/
static u32 lv2_peek_32(u64 addr)
{
	return (u32)(peekq(addr) >>32);
}

/***********************************************************************
* lv2 poke 32 bit
***********************************************************************/
static void lv2_poke_32(u64 addr, u32 value)
{
	u64 value_org = peekq(addr);
	pokeq(addr, (value_org & 0xFFFFFFFFULL) | (((u64)value) <<32));
}
#endif

#ifndef COBRA_ONLY
static inline void remove_lv2_memory_protection(void)
{
	u64 HV_START_OFFSET = 0;

	//Remove Lv2 memory protection
	if(c_firmware==3.55f)
	{
		HV_START_OFFSET = HV_START_OFFSET_355;
	}
	else
	if(c_firmware==4.21f)
	{
		HV_START_OFFSET = HV_START_OFFSET_421;
	}
	else
	if(c_firmware>=4.30f && c_firmware<=4.53f)
	{
		HV_START_OFFSET = HV_START_OFFSET_430; // same for 4.30-4.53
	}
	else
	if(c_firmware>=4.55f /*&& c_firmware<=4.88f*/)
	{
		HV_START_OFFSET = HV_START_OFFSET_455; // same for 4.55-4.88
	}

	if(!HV_START_OFFSET) return;

	poke_lv1(HV_START_OFFSET + 0x00, 0x0000000000000001ULL);
	poke_lv1(HV_START_OFFSET + 0x08, 0xe0d251b556c59f05ULL);
	poke_lv1(HV_START_OFFSET + 0x10, 0xc232fcad552c80d7ULL);
	poke_lv1(HV_START_OFFSET + 0x18, 0x65140cd200000000ULL);
}

static void install_peek_poke(void)
{
	remove_lv2_memory_protection();

	if(c_firmware>=4.30f /*&& c_firmware<=4.88f*/)
	{	// add lv2 peek/poke + lv1 peek/poke
		pokeq(0x800000000000171CULL + 0x00, 0x7C0802A6F8010010ULL);
		pokeq(0x800000000000171CULL + 0x08, 0x396000B644000022ULL);
		pokeq(0x800000000000171CULL + 0x10, 0x7C832378E8010010ULL);
		pokeq(0x800000000000171CULL + 0x18, 0x7C0803A64E800020ULL);
		pokeq(0x800000000000171CULL + 0x20, 0x7C0802A6F8010010ULL);
		pokeq(0x800000000000171CULL + 0x28, 0x396000B744000022ULL);
		pokeq(0x800000000000171CULL + 0x30, 0x38600000E8010010ULL);
		pokeq(0x800000000000171CULL + 0x38, 0x7C0803A64E800020ULL);
		pokeq(0x800000000000171CULL + 0x40, 0x7C0802A6F8010010ULL);
		pokeq(0x800000000000171CULL + 0x48, 0x7D4B537844000022ULL);
		pokeq(0x800000000000171CULL + 0x50, 0xE80100107C0803A6ULL);
		pokeq(0x800000000000171CULL + 0x58, 0x4E80002080000000ULL); // sc6  @ 0x8000000000001778 = 800000000000170C
		pokeq(0x800000000000171CULL + 0x60, 0x0000170C80000000ULL); // sc7  @ 0x8000000000001780 = 8000000000001714
		pokeq(0x800000000000171CULL + 0x68, 0x0000171480000000ULL); // sc8  @ 0x8000000000001788 = 800000000000171C
		pokeq(0x800000000000171CULL + 0x70, 0x0000171C80000000ULL); // sc9  @ 0x8000000000001790 = 800000000000173C
		pokeq(0x800000000000171CULL + 0x78, 0x0000173C80000000ULL); // sc10 @ 0x8000000000001798 = 800000000000175C
		pokeq(0x800000000000171CULL + 0x80, 0x0000175C00000000ULL);

		// enable syscalls 6, 7, 8, 9, 10
		for(u8 sc = 6; sc < 11; sc++)
			pokeq(SYSCALL_PTR(sc), 0x8000000000001748ULL + sc * 8ULL); // 0x8000000000001778 (sc6) to 0x8000000000001798 (sc10)
	}
}

#define MAX_PATH_MAP	384

typedef struct
{
	char src[MAX_PATH_MAP];
	char dst[MAX_PATH_MAP];
} redir_files_struct;

static redir_files_struct file_to_map[10];

static void add_to_map(const char *path1, const char *path2)
{
	if(max_mapped == 0) pokeq(MAP_BASE + 0x00, 0x0000000000000000ULL);

	if(max_mapped < 10)
	{
		for(u8 n = 0; n < max_mapped; n++)
		{
			if(IS(file_to_map[n].src, path1)) return;
		}

		strcpy(file_to_map[max_mapped].src, path1);
		strcpy(file_to_map[max_mapped].dst, path2);
		max_mapped++;
	}
}

static u16 string_to_lv2(const char *path, u64 addr)
{
	u16 len = MIN(strlen(path), MAX_PATH_MAP - 1);

	u8 data[MAX_PATH_MAP];
	u64 *data2 = (u64 *)data;
	memset(data, 0, MAX_PATH_MAP);
	memcpy(data, path, len);

	len = (len + 7) >> 3;
	for(u8 n = 0; n < (MAX_PATH_MAP / 8); n++, addr += 8)
	{
		pokeq(addr, data2[n]);
	}
	return len * 8;
}
#endif

static u64 convertH(const char *val)
{
	if(!val || (*val == 0)) return 0;

	u64 ret = 0; char c; u8 n = 0;

	if(islike(val, "0x")) n = 2;

	for(u8 buff, i = n; i < 16 + n; i++)
	{
		if(val[i]==' ') {n++; continue;}

		c = (val[i] | 0x20);
		if(c >= '0' && c <= '9') buff = (c - '0'); else
		if(c >= 'a' && c <= 'f') buff = (c - 'W'); else // <= c - 'a' + 10
		return ret;

		ret = (ret << 4) | buff;
	}

	return ret;
}

#ifndef LITE_EDITION
static bool isHEX(const char *value)
{
	char c;
	if(islike(value, "0x")) value += 2;
	for(; *value; ++value)
	{
		c = (*value | 0x20);
		if(!(ISDIGIT(c) || (c >= 'a' && c <= 'f') || (c == ' ') || (c == '*'))) return false;
	}
	return true;
}

static u16 Hex2Bin(const char *src, char *out)
{
	char *target = out;
	char value[3]; value[2] = '\0';
	if(islike(src, "0x")) src += 2;
	while(*src && src[1])
	{
		if(*src <= ' ') {++src; continue;} // ignore spaces & line breaks
		if(*src == '*') {*(target++) = '*'; src += 2; continue;} // convert mask ** to binary *

		value[0] = src[0], value[1] = src[1];
		*(target++) = (u8)convertH(value);
		src += 2;
	}
	return (target - out);
}
#endif

#if defined(USE_INTERNAL_NTFS_PLUGIN) || defined(NET_SUPPORT) || defined(USE_NTFS) || defined(DEBUG_MEM)
static void memcpy64(void *dst, void *src, int n)
{
	uint8_t p = n & 7;

	n >>= 3;
	uint64_t *d = (uint64_t *) dst;
	uint64_t *s = (uint64_t *) src;
	while (n--) *d++ = *s++;

	if(p)
	{
		char *m = (char *) d;
		char *c = (char *) s;
		while (p--) *m++ = *c++;
	}
}
#endif
