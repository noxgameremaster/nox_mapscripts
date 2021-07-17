
int g_importWeaponCapacityUpdate, g_updateConsumablesWeaponCapacity;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int GetScrDataField(int functionName){}
int GetPlayerIndex(int a){}
int UnitToPtr(int a){}
void FixCallOpcode(int a, int b){}

int ImportWeaponCapacityUpdate()
{
    int arr[8], link;

    if (!link)
    {
        arr[0] = 0x624AE851; arr[1] = 0x488BFFDB; arr[2] = 0x488B510C; arr[3] = 0x488B5108; arr[4] = 0x30FF5104; arr[5] = 0xD87297E8; arr[6] = 0x10C483FF; arr[7] = 0x9090C359;
        link = GetScrDataField(-g_importWeaponCapacityUpdate);
        FixCallOpcode(link + 1, 0x507250);
        FixCallOpcode(link + 0x14, 0x4d82b0);
    }
    return link;
}

//지팡이의 잔량 표시정보를 갱신합니다
//plrUnit: 지팡이를 소유한 플레이어 유닛id 입니다
//staff: 지팡이 유닛 id 입니다
void UpdateConsumablesWeaponCapacity(int plrUnit, int weapon)
{
    int pIndex = GetPlayerIndex(plrUnit), staffPtr = UnitToPtr(weapon), cur, max;
	int link = GetScrDataField(-g_updateConsumablesWeaponCapacity) + 8;

    if (staffPtr && pIndex >= 0)
    {
        cur = GetMemory(GetMemory(staffPtr + 0x2e0) + 0x6c) & 0xff;
        max = (GetMemory(GetMemory(staffPtr + 0x2e0) + 0x6c) >> 8) & 0xff;

        int temp = GetMemory(0x5c3108);
        SetMemory(0x5c3108, ImportWeaponCapacityUpdate());
        Unused1f(link);
        SetMemory(0x5c3108, temp);
    }
}

//소모류 무기의 남은 잔량을 얻습니다
int GetConsumablesWeaponCurrentAmount(int weapon)
{
	int ptr = UnitToPtr(weapon);
	
	if (ptr)
		return GetMemory(GetMemory(ptr + 0x2e0) + 0x6c) & 0xff;
	return 0;
}

//소모류 무기의 최대 수용량을 얻습니다
int GetConsumablesWeaponMaxCapacity(int weapon)
{
	int ptr = UnitToPtr(weapon);
	
	if (ptr)
		return (GetMemory(GetMemory(ptr + 0x2e0) + 0x6c) >> 8) & 0xff;
	return 0;
}

void GetConsumablesWeaponRemainingAmount(int weapon, int setTo)
{
	int ptr = UnitToPtr(weapon);
	
	if (ptr)
	{
		SetMemory(
			GetMemory(ptr + 0x2e0) + 0x6c, (GetMemory(GetMemory(ptr + 0x2e0) + 0x6c) & (~0xff)) | (setTo & 0xff));
	}
}

void NOXLibraryEntryPointFunction()
{
	g_importWeaponCapacityUpdate = ImportWeaponCapacityUpdate;
	g_updateConsumablesWeaponCapacity = UpdateConsumablesWeaponCapacity;
}
