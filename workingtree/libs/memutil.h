
int g_noxMemset;
int g_importMemAlloc;
int	g_importMemFree;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int FixCallOpcode(int curAddr, int targetAddr){}

int ImportMemAlloc()
{
    int arr[7], link;

    if (!link)
    {
        arr[0] = 0x49E85055; arr[1] = 0x50FFDB62; arr[2] = 0xCB2553E8; arr[3] = 0x1DE850FF; arr[4] = 0x83FFDB62; arr[5] = 0x5D5808C4; arr[6] = 0x909090C3;
		link = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_importMemAlloc))) + 0x1c));
        FixCallOpcode(link + 2, 0x507250);
        FixCallOpcode(link + 8, 0x403560);
        FixCallOpcode(link + 0x0e, 0x507230);
    }
    return link;
}

int MemAlloc(int size)
{
    int temp = GetMemory(0x5c3208), ptr;

    SetMemory(0x5c3208, ImportMemAlloc());
    ptr = GetCharacterData(size);
    SetMemory(0x5c3208, temp);
    return ptr;
}

int ImportMemFree()
{
    int arr[5], link;

    if (!link)
    {
        arr[0] = 0x624AE850; arr[1] = 0xE850FFDB; arr[2] = 0xFFCB3251; arr[3] = 0x5804C483; arr[4] = 0x909090C3;
		link = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_importMemFree))) + 0x1c));
        FixCallOpcode(link + 1, 0x507250);
        FixCallOpcode(link + 7, 0x40425d);
    }
    return link;
}

void MemFree(int ptr)
{
    int temp = GetMemory(0x5c3108);

    SetMemory(0x5c3108, ImportMemFree());
    Unused1f(ptr);
    SetMemory(0x5c3108, temp);
}

void NoxDwordMemset(int destPtr, int length, int value)
{
	int u = 0, ulink = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_noxMemset))) + 0x1c)) + 12;
	int maxCount = length >> 2;
	
	for (u ; u < maxCount ; SetMemory(ulink, u + 1))
		SetMemory(destPtr + (u << 2), value);
}

void NOXLibraryEntryPointFunction()
{
	"export ImportMemAlloc";
	"export MemAlloc";
	"export ImportMemFree";
	"export MemFree";
	"export NoxDwordMemset";
	
	g_importMemAlloc = ImportMemAlloc;
	g_importMemFree = ImportMemFree;
	g_noxMemset = NoxDwordMemset;
}

