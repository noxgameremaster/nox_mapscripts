

int GetMemory(int a){}
void SetMemory(int a, int b){}

int GetByteValue(int ptr)
{
    return GetMemory(ptr) & 0xff;
}

void WriteAddressWordValue(int addr, int word)
{
    int temp = GetMemory(addr) & 0xffff0000;
    SetMemory(addr, temp | word);
}

void NoxUtf8ToUnicode(int src, int dest)
{
    int i = 0, byt;

    while (1)
    {
        byt = GetByteValue(src + i);
        if (!byt) break;
        if (!(byt & 0x80))
        {
            WriteAddressWordValue(dest, byt);
            i ++;
        }
        else if ((byt & 0xe0) == 0xc0)
        {
            WriteAddressWordValue(dest, ((byt & 0x1f) <<6) | (GetByteValue(src + i + 1) & 0x3f));
            i += 2;
        }
        else if ((byt & 0xf0) == 0xe0)
        {
            WriteAddressWordValue(dest, ((byt & 0xf) << 12) | ((GetByteValue(src + i + 1) & 0x3f) << 6) | (GetByteValue(src + i + 2) & 0x3f));
            i += 3;
        }
        dest += 2;
    }
    WriteAddressWordValue(dest, 0);
}

int GetWordValue(int num)
{
	return num & 0xffff;
}

void WriteAddressByteValue(int addr, int byt)
{
	int temp = GetMemory(addr) & 0xffffff00;
	SetMemory(addr, temp | byt);
}

int NoxUnicodeToUtf8(int src, int destPtr)
{
	int i, byt, dest = destPtr;

	for (i = 0 ; i < 20 ; i ++)
	{
		byt = GetWordValue(GetMemory(src));
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

void NOXLibraryEntryPointFunction()
{
    "export GetByteValue";
    "export WriteAddressWordValue";
    "export NoxUtf8ToUnicode";
    "export GetWordValue";
    "export WriteAddressByteValue";
    "export NoxUnicodeToUtf8";
}