
int GetMemory(int a){}
void SetMemory(int a, int b){}
int UnitToPtr(int a){}

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

int GetPlayerAction(int ptr)
{
    //01- berserker, 05- run, 1a- laugh, 1b- point, 19- taunt
    return GetMemory(GetMemory(ptr + 0x2ec) + 0x58) & 0xff;
}

void SetPlayerAction(int ptr, int val)
{
    SetMemory(GetMemory(ptr + 0x2ec) + 0x58, val);
}

void NOXLibraryEntryPointFunction()
{
	"export CheckPlayerInput";
	"export GetPlayerAction";
	"export SetPlayerAction";
}
