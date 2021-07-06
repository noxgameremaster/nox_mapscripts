
int GetMemory(int a){}
void SetMemory(int a, int b){}
string ToStr(int a){}
int UnitToPtr(int a){}

string ReadStringAddress(int t)
{
	return ToStr((t - 0x97bb40) / 4);
}

string EnchantList(int sNumber)
{
	return ReadStringAddress(0x596f24 + (sNumber * 4));
}

int UnitCheckEnchant(int sUnit, int sMagicFlag)
{
    int ptr = UnitToPtr(sUnit);

    if (ptr)
        return GetMemory(ptr + 0x154) & sMagicFlag;
    return 0;
}

int GetLShift(int sCount)
{
    return (1 << sCount);
}

void SetUnitEnchantCopy(int sUnit, int sMagicFlag)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		Enchant(sUnit, EnchantList(6), 0.0);
		SetMemory(ptr + 0x154, GetMemory(ptr + 0x154) | sMagicFlag);
	}
}

void UnitSetEnchantTime(int unit, int enchantNumber, int durateTime)
{
	int ptr = UnitToPtr(unit), temp, tempPtr;

	if (ptr)
	{
		SetUnitEnchantCopy(unit, GetLShift(enchantNumber));
		tempPtr = ptr + 0x158 + ((enchantNumber / 2) * 4);
		temp = GetMemory(tempPtr);
		if (enchantNumber % 2)
		{
			SetMemory(tempPtr, (GetMemory(tempPtr) & 0xffff) | (durateTime << 0x10));
		}
		else
			SetMemory(tempPtr, ((GetMemory(tempPtr) >> 0x10) << 0x10) | durateTime);
	}
}

void NOXLibraryEntryPointFunction()
{
	"export ReadStringAddress";
	"export EnchantList";
	"export UnitCheckEnchant";
	"export GetLShift";
	"export SetUnitEnchantCopy";
	"export UnitSetEnchantTime";
}
