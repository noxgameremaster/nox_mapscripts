
int g_importMonsterActionPush;
int g_monsterActionPush;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int ToInt(float a){}
int GetScrDataField(int a){}
int UnitToPtr(int a){}

int ImportMonsterActionPush()
{
    int arr[12], link;

    if (!link)
    {
        arr[0] = 0x50685650; arr[1] = 0xFF005072; arr[2] = 0x708D2414; arr[3] = 0xA2606804; arr[4] = 0x36FF0050; arr[5] = 0x54FF30FF;
        arr[6] = 0xC4830824; arr[7] = 0x7230680C; arr[8] = 0xFF500050; arr[9] = 0x83042454; arr[10] = 0x585E0CC4; arr[11] = 0x909090C3;
        link = GetScrDataField(-(g_importMonsterActionPush));
    }
    return link;
}

int MonsterActionPush(int sUnit, int sActType)
{
    int ptr = UnitToPtr(sUnit), link, temp = GetMemory(0x5c31bc), res = 0;

    if (!link)
        link = GetScrDataField(-(g_monsterActionPush));
    if (ptr)
    {
        sUnit = ptr;
        SetMemory(0x5c31bc, ImportMonsterActionPush());
        res = GetHolder(link);
        SetMemory(0x5c31bc, temp);
    }
    return res;
}

void MonsterForceCastSpell(int sUnit, int sSpellNumber, float xProfile, float yProfile)
{
	int act = MonsterActionPush(sUnit, 19);

	if (act)
	{
		SetMemory(act + 4, sSpellNumber);
		SetMemory(act + 12, ToInt(xProfile));
		SetMemory(act + 16, ToInt(yProfile));
	}
}

void NOXLibraryEntryPointFunction()
{
	"export ImportMonsterActionPush";
	"export MonsterActionPush";
	"export MonsterForceCastSpell";
	
	g_importMonsterActionPush = ImportMonsterActionPush;
	g_monsterActionPush = MonsterActionPush;
}