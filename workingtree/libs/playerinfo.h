
int g_getPlayerPtrByNetCode;
int g_importGiveUnitFunc;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int UnitToPtr(int a){}
int GetScrDataField(int functionName){}

//플레이어가 현재 장착중인 무기의 id 을 얻습니다
int PlayerGetEquipedWeapon(int plrUnit)
{
    int ptr = UnitToPtr(plrUnit), gid;
    
    if (ptr)
    {
        gid = GetMemory(GetMemory(ptr + 0x2ec) + 0x68);
        if (gid)
            return GetMemory(gid + 0x2c);
    }
    return 0;
}

//플레이어의 두번째 무기의 id 을 얻습니다
int PlayerGetNextWeapon(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit), gid;
	
	if (ptr)
	{
		gid = GetMemory(GetMemory(ptr + 0x2ec) + 0x6c);
		if (gid)
			return GetMemory(gid + 0x2c);
	}
	return 0;
}

//플레이어의 힘 수치를 얻습니다
int PlayerGetStrength(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit);
	
	if (ptr)
	{
		int pInfo = GetMemory(GetMemory(ptr + 0x2ec) + 0x114);
		
		return GetMemory(pInfo + 0x8bf);
	}
	return 0;
}

void PlayerSetStrength(int plrUnit, int strength)
{
	int ptr = UnitToPtr(plrUnit);
	
	if (ptr)
	{
		int pInfo = GetMemory(GetMemory(ptr + 0x2ec) + 0x114);
		
		SetMemory(pInfo + 0x8bf, strength);
	}
}

int PlayerGetMaxSpeed(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit);
	
	if (ptr)
	{
		int pInfo = GetMemory(GetMemory(ptr + 0x2ec) + 0x114);
		
		return GetMemory(pInfo + 0x8bb);
	}
	return 0;
}

int PlayerGetMaxWeight(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit);
	
	if (ptr)
	{
		int pInfo = GetMemory(GetMemory(ptr + 0x2ec) + 0x114);
		
		return GetMemory(pInfo + 0xe44);
	}
	return 0;
}

float PlayerGetArmorValue(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit);
	
	if (ptr)
	{
		int pEC = GetMemory(ptr + 0x2ec);
		
		StopScript(GetMemory(pEC + 0xe8));
	}
	return 0.0;
}

int PlayerGetPtrFromIndex(int pIndex)
{
	return GetMemory(0x62f9e0 + ((pIndex & 0x1f) * 0x12dc));
}

int GetPlayerMouseX(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit);

	while (ptr)
	{
		if (!(GetMemory(ptr + 8) & 4))
			break;
		return GetMemory(GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0x8ec);
	}
	return 0;
}

int GetPlayerMouseY(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit);

	while (ptr)
	{
		if (!(GetMemory(ptr + 8) & 4))
			break;
		return GetMemory(GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0x8f0);
	}
	return 0;
}

int GetPlayerPtrByNetCode(int netId)
{
	int k, plr_ptr = 0x62f9e4, link = GetScrDataField(-(g_getPlayerPtrByNetCode)) + 4;

	for (k = 0 ; k < 32 ; SetMemory(link, k + 1))
	{
		if (GetMemory(plr_ptr) ^ netId)
			SetMemory(link + 4, plr_ptr + 0x12dc);
		else
			return GetMemory(plr_ptr - 4);
	}
	return 0;
}

int ImportGiveUnitFunc()
{
    int arr[17], link;
	
    if (!link)
    {
        arr[0] = 0x50725068; arr[1] = 0x1B606800; arr[2] = 0x00680051; arr[3] = 0xFF004E7B; arr[4] = 0x50082454; arr[5] = 0x082454FF; arr[6] = 0x8B04C483; arr[7] = 0x74FF85F8;
        arr[8] = 0x2454FF19; arr[9] = 0x54FF5008; arr[10] = 0xC4830824; arr[11] = 0x74C08504; arr[12] = 0xFF505709; arr[13] = 0x83082454; arr[14] = 0xC48308C4; arr[15] = 0x31FF310C;
        arr[16] = 0x0000C3C0;
		link = GetScrDataField(-(g_importGiveUnitFunc));
    }
    return link;
}

void GiveCreatureToPlayer(int owner, int unit)
{
    SetMemory(0x5c31f4, ImportGiveUnitFunc());
    Unused5a(owner, unit);
    SetMemory(0x5c31f4, 0x513f30);
}

void NOXLibraryEntryPointFunction()
{
	"export PlayerGetEquipedWeapon";
	"export PlayerGetNextWeapon";
	"export PlayerGetStrength";
	"export PlayerSetStrength";
	"export PlayerGetMaxSpeed";
	"export PlayerGetMaxWeight";
	"export PlayerGetArmorValue";
	"export PlayerGetPtrFromIndex";
	"export GetPlayerMouseX";
	"export GetPlayerMouseY";
	"export GetPlayerPtrByNetCode";
	"export needinit ImportGiveUnitFunc";
	"export GiveCreatureToPlayer";
	
	g_getPlayerPtrByNetCode = GetPlayerPtrByNetCode;
	g_importGiveUnitFunc = ImportGiveUnitFunc;
}