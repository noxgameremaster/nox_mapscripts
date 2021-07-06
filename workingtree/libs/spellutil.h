
int g_importRemoveSneakDel, g_importgetspell;

int GetMemory(int a){}
void SetMemory(int a, int b){}

int UnitToPtr(int a){}

int ImportRemoveSneakDelay()
{
    int arr[7], link;

    if (!link)
    {
        arr[0] = 0x72506850; arr[1] = 0x14FF0050; arr[2] = 0xC3006824; arr[3] = 0x046A004F; arr[4] = 0x2454FF50; arr[5] = 0x10C48308; arr[6] = 0x9090C358;
        int flink = g_importRemoveSneakDel;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-flink)) + 0x1c);
    }
    return link;
}

void RemoveTreadLightly(int plrUnit)
{
    int ptr = UnitToPtr(plrUnit), temp = GetMemory(0x5c336c);

    if (ptr)
    {
        if (GetMemory(ptr + 0x08) & 0x04)
        {
            SetMemory(0x5c336c, ImportRemoveSneakDelay());
            Unknownb8(ptr);
            SetMemory(0x5c336c, temp);
        }
    }
}

int ImportGetSpellNumber()
{
	int arr[11], link;

	if (!link)
	{
		arr[0] = 0x50725068; arr[1] = 0x2414FF00; arr[2] = 0x4085048B; arr[3] = 0x680097BB; arr[4] = 0x004243F0; arr[5] = 0x2454FF50;
		arr[6] = 0x08C48304; arr[7] = 0x50723068; arr[8] = 0x54FF5000; arr[9] = 0xC4830424; arr[10] = 0xC3C0310C;
        int flink = g_importgetspell;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-flink)) + 0x1c);
	}
	return link;
}

int GetSpellNumber(string spell)
{
	int temp = GetMemory(0x5c3204), res;

	SetMemory(0x5c3204, ImportGetSpellNumber());
	res = Unused5e(spell);
	SetMemory(0x5c3204, temp);
	return res * 4;
}

void NOXLibraryEntryPointFunction()
{
    "export ImportRemoveSneakDelay";
    "export RemoveTreadLightly";
    "export needinit ImportGetSpellNumber";
    "export GetSpellNumber";

    g_importRemoveSneakDel = ImportRemoveSneakDelay;
    g_importgetspell = ImportGetSpellNumber;
}