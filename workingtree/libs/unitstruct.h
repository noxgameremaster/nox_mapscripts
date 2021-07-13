

int GetMemory(int a){}
void SetMemory(int a, int b){}
int UnitToPtr(int a){}
int ToInt(float x){}


float GetMemoryFloat(int a)
{
	StopScript(GetMemory(a));
}

void SetUnitFlags(int unit, int flag)
{
	int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(ptr + 0x10, flag);
}

int GetUnitFlags(int unit)
{
	int ptr = UnitToPtr(unit);

    if (ptr)
        return GetMemory(ptr + 0x10);
    return 0;
}

void UnitNoCollide(int unit)
{
    SetUnitFlags(unit, GetUnitFlags(unit) ^ 0x40);
}

int GetUnitSubclass(int unit)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
		return GetMemory(ptr + 0x0c);
	return 0;
}

void SetUnitSubclass(int unit, int subclassFlag)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
		SetMemory(ptr + 0x0c, subclassFlag);
}

int GetUnitThingID(int unit)
{
    int ptr = UnitToPtr(unit);
    if (ptr)
        return GetMemory(ptr + 0x04);
    return 0;
}

void SetUnitSpeed(int unit, float amount)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(ptr + 0x224, ToInt(amount));
}

void SetUnitMass(int unit, float ms)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(ptr + 0x78, ToInt(ms));
}

int GetUnitStatus(int unit)
{
    int temp, ptr = UnitToPtr(unit);

    if (ptr)
    {
        temp = GetMemory(ptr + 0x2ec);
        if (temp)
            return GetMemory(temp + 0x5a0);
    }
    return 0;
}

void SetUnitStatus(int unit, int stat)
{
    int temp, ptr = UnitToPtr(unit);

    if (ptr)
    {
        temp = GetMemory(ptr + 0x2ec);
        if (temp)
            SetMemory(temp + 0x5a0, stat);
    }
}

void SetUnitMaxHealth(int unit, int amount)
{
    int ptr = UnitToPtr(unit);
    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x22c), amount);
        SetMemory(GetMemory(ptr + 0x22c) + 0x4, amount);
    }
}

void UnitZeroFleeRange(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0); //Flee Range set to 0
}

void SetUnitVoice(int unit, int voiceIndex)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(voiceIndex));
}

void UnitLinkBinScript(int unit, int binScrAddr)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, binScrAddr);
}

int VoiceList(int num)
{
    int list[75], addr, k;

    if (!list[0])
    {
        addr = GetMemory(0x663eec);
        for (k = 0 ; k < 75 ; k ++)
        {
            list[k] = addr;
            addr = GetMemory(addr + 0x4c);
        }
    }
    return list[num];
}

void PreloadVoiceList()
{
	VoiceList(0);
}

int GetUnit1C(int sUnit)
{
    int ptr = UnitToPtr(sUnit);

    if (ptr)
        return GetMemory(ptr + 0x1c);
    return 0;
}

void SetUnit1C(int sUnit, int sData)
{
    int ptr = UnitToPtr(sUnit);

    if (ptr)
        SetMemory(ptr + 0x1c, sData);
}

int IsMissileUnit(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        return GetMemory(ptr + 8) & 1;
    return false;
}

int IsPlayerUnit(int checkUnit)
{
    int ptr = UnitToPtr(checkUnit);

    if (ptr)
        return GetMemory(ptr + 0x08) & 4;
    return false;
}

int IsMonsterUnit(int checkUnit)
{
    int ptr = UnitToPtr(checkUnit);

    if (ptr)
        return GetMemory(ptr + 0x08) & 2;
    return false;
}

float DistanceUnitToUnit(int unit1, int unit2)
{
    int ptr1 = UnitToPtr(unit1);
    int ptr2 = UnitToPtr(unit2);

    if (ptr1 && ptr2)
        return Distance(GetMemoryFloat(ptr1 + 0x38), GetMemoryFloat(ptr1 + 0x3c), GetMemoryFloat(ptr2 + 0x38), GetMemoryFloat(ptr2 + 0x3c));
    return 0.0;
}

int GetOwner(int unit)
{
    int ptr = UnitToPtr(unit), res;

    if (ptr)
    {
        res = GetMemory(ptr + 0x1fc);
        if (res)
            return GetMemory(res + 0x2c);
    }
    return 0;
}

int IsPoisonedUnit(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        return GetMemory(ptr + 0x21c) & 0xff;
    return 0;
}

void SetUnitScanRange(int unit, float range)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
		if (GetMemory(ptr + 0x08) & 2)
			SetMemory(GetMemory(ptr + 0x2ec) + 0x520, ToInt(range));
    }
}

int IsMonsterPoisonImmune(int unit)
{
	int ptr = UnitToPtr(unit);
	
	if (IsMonsterUnit(unit))
	{
		return GetMemory(ptr + 0x0c) & 0x200;
	}
	return false;
}

void SetMonsterPoisonImmune(int unit)
{
    int ptr = UnitToPtr(unit);

    if (IsMonsterUnit(unit))
        SetMemory(ptr + 0x0c, GetMemory(ptr + 0x0c) ^ 0x200);
}

int IsMonsterFireImmune(int unit)
{
	int ptr = UnitToPtr(unit);
	
	if (IsMonsterUnit(unit))
	{
		return GetMemory(ptr + 0x0c) & 0x400;
	}
	return false;
}

void SetMonsterFireImmune(int unit)
{
    int ptr = UnitToPtr(unit);

    if (IsMonsterUnit(unit))
        SetMemory(ptr + 0x0c, GetMemory(ptr + 0x0c) ^ 0x400);
}

int IsMonsterElectricityImmune(int unit)
{
	int ptr = UnitToPtr(unit);
	
	if (IsMonsterUnit(unit))
	{
		return GetMemory(ptr + 0x0c) & 0x800;
	}
	return false;
}

void SetMonsterElectricityImmune(int unit)
{
    int ptr = UnitToPtr(unit);

    if (IsMonsterUnit(unit))
        SetMemory(ptr + 0x0c, GetMemory(ptr + 0x0c) ^ 0x800);
}

void NOXLibraryEntryPointFunction()
{
	"export GetMemoryFloat";
	"export SetUnitFlags";
	"export GetUnitFlags";
	"export GetUnitStatus";
	"export SetUnitSubclass";
	"export UnitNoCollide";
	"export GetUnitThingID";
	"export SetUnitSpeed";
	"export SetUnitMass";
	"export GetUnitStatus";
	"export SetUnitStatus";
	"export SetUnitMaxHealth";
	"export UnitZeroFleeRange";
	"export VoiceList";
	"export SetUnitVoice";
	"export UnitLinkBinScript";
	"export needinit PreloadVoiceList";
	"export GetUnit1C";
	"export SetUnit1C";
	"export IsMissileUnit";
	"export IsPlayerUnit";
	"export IsMonsterUnit";
	"export IsPlayerUnit";
	"export DistanceUnitToUnit";
	"export GetOwner";
	"export IsPoisonedUnit";
	"export SetUnitScanRange";
	"export IsMonsterPoisonImmune";
	"export SetMonsterPoisonImmune";
	"export IsMonsterFireImmune";
	"export SetMonsterFireImmune";
	"export IsMonsterElectricityImmune";
	"export SetMonsterElectricityImmune";
}
