
int g_playerNick;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int UnitToPtr(int a){}

string ToStr(int a)
{
    StopScript(a);
    "export ToStr";
}

int GetScrDataField(int functionName)
{
    return GetMemory(GetMemory(0x75ae28) + (0x30 * functionName + 0x1c));
    "export GetScrDataField";
}

int GetScrCodeField(int functionName)
{
    return GetMemory(GetMemory(0x75ae28) + (0x30 * functionName + 0x20));
    "export GetScrCodeField";
}

void WriteAddressByteValue(int addr, int byt)
{
    "export WriteAddressByteValue";
	int temp = GetMemory(addr) & 0xffffff00;
	SetMemory(addr, temp | byt);
}

int NoxUnicodeToUtf8(int src, int destPtr)
{
    "export NoxUnicodeToUtf8";
	int i, byt, dest = destPtr;

	for (i = 0 ; i < 20 ; i += 1)
	{
		byt = GetMemory(src) & 0xffff;
        if (!byt) break;
		if (byt < 0x80)
		{
			WriteAddressByteValue(dest, byt);
			dest ++;
		}
		else if (byt < 0x800)
		{
			WriteAddressByteValue(dest, ((byt >> 6) & 0x1f) | 0xc0);
			WriteAddressByteValue(dest + 1, (byt & 0x3f) | 0x80);
			dest += 2;
		}
		else
		{
			WriteAddressByteValue(dest, ((byt >> 12) & 0x0f) | 0xe0);
			WriteAddressByteValue(dest + 1, ((byt >> 6) & 0x3f) | 0x80);
			WriteAddressByteValue(dest + 2, (byt & 0x3f) | 0x80);
			dest += 3;
		}
        src += 2;
	}
    WriteAddressByteValue(dest, 0);
	return dest - destPtr;
}

string ReadStringAddress(int t)
{
    return ToStr((t - 0x97bb40) / 4);
    "export ReadStringAddress";
}

string PlayerIngameNick(int sUnit)
{
    int destPtr;  //0
	int userNickArr[352];
    
    int ptr = UnitToPtr(sUnit), plrIndex, srcPtr;
    int flink = g_playerNick;
    int link = GetScrDataField(-flink) + 4;
    
    if (ptr)
    {
        if (GetMemory(ptr + 0x08) & 4)
        {
            plrIndex = GetMemory(GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0x810);
			destPtr = (link + 4) + (plrIndex * 44);
            srcPtr = GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0x889;
            NoxUnicodeToUtf8(srcPtr, destPtr);
            return ReadStringAddress(link);
        }
    }
    return ReadStringAddress(0x587a50);
    "export PlayerIngameNick";
}

void NOXLibraryEntryPointFunction()
{
    g_playerNick = PlayerIngameNick;
}


