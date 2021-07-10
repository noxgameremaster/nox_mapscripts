
int g_weaponListSectionFindThingId;
int g_noxCopyString;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int ToInt(float x){}
int GetScrDataField(int functionName){}
int MemAlloc(int size){}
void NoxDwordMemset(int destPtr, int length, int value){}

int CsfDbFindStringDescriptionFromId(int csfId)
{
	int key = GetMemory(GetMemory(0x611c00) + (52 * csfId) + 48) >> 0x10;
	
	return GetMemory(GetMemory(0x611c04) + (key * 4));
}

int WeaponListSectionFindThingId(int thingId)
{
	int page = GetMemory(0x611C64), pageLink = GetScrDataField(-(g_weaponListSectionFindThingId)) + 4;
	
	while (page != 0)
	{
		if (GetMemory(page + 4) == thingId)
			return page;
		SetMemory(pageLink, GetMemory(page + 80));
	}
	return 0;
}

int WandPatchWordStringNullTerminateCheck(int srcPtr)
{
    int stream = GetMemory(srcPtr);

	if (!(stream & 0xffff))
		return 0;
	if (!(stream >> 0x10))
		return 0;

    return 1;
}

void NoxCopyString(int srcPtr, int destPtr)
{
	int i, iLink = GetScrDataField(-(g_noxCopyString)) + 8;
	
    for (i = 0 ; 1; SetMemory(iLink, i + 1))
    {
        SetMemory(destPtr + (i * 4), GetMemory(srcPtr + (i * 4)));
        if (!WandPatchWordStringNullTerminateCheck(srcPtr + (i * 4)))
            break;
    }
}

void AppendMissileWand()
{
    int thingId = 213;
    int ptr = WeaponListSectionFindThingId(thingId);

    if (ptr != 0)
    {
		if (GetMemory(ptr + 8) == 0)
		{
			int desc = MemAlloc(24);
			
			NoxDwordMemset(desc, 24, 0);
			NoxCopyString(CsfDbFindStringDescriptionFromId(3651), desc);
			SetMemory(ptr + 8, desc);
		}
		SetMemory(ptr + 12, 0xa1000000);
		SetMemory(ptr + 16, 0x788c);
		SetMemory(ptr + 40, 1);
		SetMemory(ptr + 60, 0x70014);
		SetMemory(ptr + 64, ToInt(0.5));
		SetMemory(ptr + 68, ToInt(50.0));
		SetMemory(ptr + 72, 5);
		SetMemory(0x58f260, GetMemory(ptr));
		SetMemory(0x58f264, thingId);
		SetMemory(0x58f268, 131072);
    }
}

void AppendDemonsBreathWand()
{
    int thingId = 215;
    int ptr = WeaponListSectionFindThingId(thingId);

    if (ptr != 0)
    {
		if (GetMemory(ptr + 8) == 0)
		{
			int desc = MemAlloc(24);
			
			NoxCopyString(CsfDbFindStringDescriptionFromId(3521), desc);
			SetMemory(ptr + 8, desc);
		}
		SetMemory(ptr + 12, 0xa1000000);
		SetMemory(ptr + 16, 0x788c);
		SetMemory(ptr + 40, 1);
		SetMemory(ptr + 60, 0x70014);
		SetMemory(ptr + 64, ToInt(0.5));
		SetMemory(ptr + 68, ToInt(50.0));
		SetMemory(ptr + 72, 5);
		SetMemory(0x58f1f0, 0x200000);
    }
}

void AppendAllDummyStaffsToWeaponList()
{
	AppendDemonsBreathWand();
	AppendMissileWand();
}

void NOXLibraryEntryPointFunction()
{
	"export CsfDbFindStringDescriptionFromId";
	"export WandPatchWordStringNullTerminateCheck";
	"export NoxCopyString";
	"export WeaponListSectionFindThingId";
	"export AppendMissileWand";
	"export AppendDemonsBreathWand";
	"export AppendAllDummyStaffsToWeaponList";
	
	g_weaponListSectionFindThingId = WeaponListSectionFindThingId;
	g_noxCopyString = NoxCopyString;
}

