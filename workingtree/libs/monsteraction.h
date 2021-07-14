
int g_importMonsterActionPush;
int g_monsterActionPush;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int ToInt(float a){}
int GetScrDataField(int a){}
int UnitToPtr(int a){}
int IsMonsterUnit(int checkUnit){}

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
		int ptr = UnitToPtr(sUnit);
		
		if (GetMemory(ptr + 8) & 2)
		{
			SetMemory(act + 4, sSpellNumber);
			SetMemory(act + 12, ToInt(xProfile));
			SetMemory(act + 16, ToInt(yProfile));
		}
	}
}

//크리쳐를 보호 모드로 설정합니다
//이 특징을 적용하려면 유닛이 생성된 후 최소 1 프레임의 딜레이가 필요합니다
void MonsterSetActionGuard(int sUnit)
{
    //Need to delay at least 1 frame
    int ptr = UnitToPtr(sUnit);

    if (ptr)
    {
		if (GetMemory(ptr + 8) & 2)
		{
			SetMemory(GetMemory(ptr + 0x2ec) + 0x228, 4);
			SetMemory(GetMemory(ptr + 0x2ec) + 0x22c, GetMemory(ptr + 0x38));
			SetMemory(GetMemory(ptr + 0x2ec) + 0x230, GetMemory(ptr + 0x3c));
			SetMemory(GetMemory(ptr + 0x2ec) + 0x234, (GetMemory(ptr + 0x7c) >> 0x10) & 0xff); //Direction
			SetMemory(GetMemory(ptr + 0x2ec) + 0x238, 0);
			SetMemory(GetMemory(ptr + 0x2ec) + 0x23c, 1);
		}
    }
}

//크리쳐의 현재 행동정보를 얻습니다
int MonsterGetCurrentAction(int cre)
{
    int ptr = UnitToPtr(cre);

    if (ptr)
    {
		if (GetMemory(ptr + 8) & 2)
			return GetMemory(GetMemory(ptr + 0x2ec) + 0x228);
    }
    return 0;
}

void NOXLibraryEntryPointFunction()
{
	"export ImportMonsterActionPush";
	"export MonsterActionPush";
	"export MonsterForceCastSpell";
	"export MonsterSetActionGuard";
	"export MonsterGetCurrentAction";
	
	g_importMonsterActionPush = ImportMonsterActionPush;
	g_monsterActionPush = MonsterActionPush;
}