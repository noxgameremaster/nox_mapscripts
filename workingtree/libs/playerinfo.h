
int GetMemory(int a){}
int UnitToPtr(int a){}

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