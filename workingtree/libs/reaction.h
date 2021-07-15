
int GetMemory(int a){}
void SetMemory(int a, int b){}
int UnitToPtr(int a){}

//유저 입력을 얻습니다
//plr_unit: 유저유닛 id 입니다
//return
//1- 평소에, 2- 우클릭, 6- 좌클릭, 7- 점프, 47- 약올리기
//48- 웃기, 49- 가리키기
int CheckPlayerInput(int plr_unit)
{
    int ptr = UnitToPtr(plr_unit), temp;

    if (ptr)
    {
        temp = GetMemory(GetMemory(ptr + 0x2ec) + 0x114);
        if (temp)
            return GetMemory(0x81b960 + (GetMemory(temp + 0x810) * 3072));
    }
    return 0;
}

//플레이어의 행동을 얻습니다
//01- berserker, 05- run, 1a- laugh, 1b- point, 19- taunt
int GetPlayerAction(int plrUnit)
{
	int ptr = UnitToPtr(plrUnit);
	
	if (ptr)
	{
		if (GetMemory(ptr + 0x8) & 4)
			return GetMemory(GetMemory(ptr + 0x2ec) + 0x58) & 0xff;
	}
	return 0;
}

//플레이어의 행동을 설정합니다
//01- berserker, 05- run, 1a- laugh, 1b- point, 19- taunt
void SetPlayerAction(int plrUnit, int val)
{
	int ptr = UnitToPtr(plrUnit);
	
	if (ptr)
	{
		if (GetMemory(ptr + 0x08) & 4)
			SetMemory(GetMemory(ptr + 0x2ec) + 0x58, val);
	}
}

void NOXLibraryEntryPointFunction()
{
	"export CheckPlayerInput";
	"export GetPlayerAction";
	"export SetPlayerAction";
}
