
int GetMemory(int a){}
void SetMemory(int a, int b){}
string ToStr(int a){}
int UnitToPtr(int a){}
int CreateObjectAt(string a, float b, float c){}
float UnitAngleCos(int a, float b){}
float UnitAngleSin(int a, float b){}				//mathlab.h
int ToInt(float a){}
int MemAlloc(int a){}								//memutil.h


string ReadStringAddress(int t)
{
    return ToStr((t - 0x97bb40) / 4);
}

string StringUtilFindUnitNameById(int thingId)
{
	if (thingId > 0 && thingId < GetMemory(0x69f228))
	{
		int thindDbLink = GetMemory(0x7520d4);
		int thingDbUnitBase = GetMemory(thindDbLink + (thingId << 2));
		
		return ReadStringAddress(thingDbUnitBase + 4);
	}
	return ReadStringAddress(0x587a50);
}

//커스텀 마법미사일을 생성합니다
//misName: 미사일 유닛 이름입니다. 미사일 타입이 와야합니다
//owner: 소유자입니다
//speed: 추진속도 값입니다. 0.0 인 경우 기본값 3.3을 사용합니다
int SummonCustomMagicMissile(string misName, int owner, float speed)
{
    float xVect = UnitAngleCos(owner, 18.0), yVect = UnitAngleSin(owner, 18.0);
    int target = CreateObjectAt(StringUtilFindUnitNameById(1399), GetObjectX(owner) + (xVect * 10.0), GetObjectY(owner) + (yVect * 10.0));
    int mis = CreateObjectAt(misName, GetObjectX(owner) + xVect, GetObjectY(owner) + yVect);
    int ptr = GetMemory(0x750710);
    int ecPtr;
    int casterPtr = UnitToPtr(target);

    DeleteObjectTimer(target, 3);
    if (ptr && target)
    {
        SetOwner(owner, mis);
		if (ToInt(speed) == 0)
			speed = 3.3;
        SetMemory(ptr + 0x220, ToInt(speed)); //Speed
        SetMemory(ptr + 0x224, 0); //accel
        ecPtr = MemAlloc(20);
        SetMemory(ecPtr, UnitToPtr(owner));
        SetMemory(ecPtr + 4, casterPtr);
        SetMemory(ptr + 0x2ec, ecPtr);
        SetMemory(ptr + 0x2e8, 5488032);
    }
    return mis;
}

void NOXLibraryEntryPointFunction()
{
	"export ReadStringAddress";
	"export StringUtilFindUnitNameById";
	"export SummonCustomMagicMissile";
}