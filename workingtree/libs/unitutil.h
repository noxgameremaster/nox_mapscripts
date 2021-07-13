
int g_importUnitDie;
int g_importUnitDrop;
int g_importUnitCollide;
int g_importUseitem;
int g_importCreateAt;
int g_importUnitToPtr;
int g_importUnitPickupFunc;

int SToInt(string x) {}
int ToInt(float x) {}
void SetMemory(int a, int v){}
int GetMemory(int a){}

int ImportCreateAtFunc()
{
    int arr[20], link;

    if (!link)
    {
        arr[0] = 0xAA506856; arr[1] = 0x5068004D; arr[2] = 0xFF005072; arr[3] = 0xFF502414; arr[4] = 0x50042454;
        arr[5] = 0x082454FF; arr[6] = 0x4085048B; arr[7] = 0x680097BB; arr[8] = 0x004E3810; arr[9] = 0x2454FF50;
        arr[10] = 0x08C48304; arr[11] = 0xF685F08B; arr[12] = 0x006A0A74; arr[13] = 0x2454FF56; arr[14] = 0x08C48314;
        arr[15] = 0x50723068; arr[16] = 0x54FF5600; arr[17] = 0xC4830424; arr[18] = 0x5EC03118; arr[19] = 0x909090C3;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importCreateAt)) + 0x1c);
    }
    return link;
}

int CreateObjectAt(string name, float x, float y)
{
    int temp = GetMemory(0x5c3160), res;

    SetMemory(0x5c3160, ImportCreateAtFunc());
    res = CreateMover(SToInt(name), ToInt(x), y);
    SetMemory(0x5c3160, temp);
    if (res) return GetMemory(res + 0x2c);
    else return 0;
}

int ImportUnitToPtrFunc()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x50725068; arr[1] = 0x2414FF00; arr[2] = 0x511B6068; arr[3] = 0x54FF5000; arr[4] = 0xC4830424;
        arr[5] = 0x7230680C; arr[6] = 0xFF500050; arr[7] = 0x83042454; arr[8] = 0xC03108C4; arr[9] = 0x909090C3;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importUnitToPtr)) + 0x1c);
    }
    return link;
}

int UnitToPtr(int unit)
{
    int temp = GetMemory(0x5c336c), res;
    SetMemory(0x5c336c, ImportUnitToPtrFunc());
    res = Unknownb8(unit);
    SetMemory(0x5c336c, temp);
	return res;
}

int ImportUseItemFuncOffset()
{
	return 0x2fc;
}

int ImportUseItemFunc()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x50731068; arr[1] = 0x50565500; arr[2] = 0x1424748B; arr[3] = 0x18246C8B;
        arr[4] = 0x858B | (ImportUseItemFuncOffset() << 0x10); arr[5] = 0x56550000; arr[6] = 0x2454FF50; arr[7] = 0x0CC48318;
        arr[8] = 0x835D5E58; arr[9] = 0x90C304C4;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importUseitem)) + 0x1c);
    }
    return link;
}

//아이템을 사용할 때 실행할 함수를 등록합니다
void SetUnitCallbackOnUseItem(int unit, int callback)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
	{
		SetMemory(ptr + 0x2dc, ImportUseItemFunc());
		SetMemory(ptr + ImportUseItemFuncOffset(), callback);
	}
}

int ImportUnitCollideFuncOffset()
{
	return 0x2fc;
}

int ImportUnitCollideFunc()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x50731068; arr[1] = 0x50565500; arr[2] = 0x14246C8B; arr[3] = 0x1824748B;
        arr[4] = 0x858B | (ImportUnitCollideFuncOffset() << 0x10); arr[5] = 0x56550000; arr[6] = 0x2454FF50; arr[7] = 0x0CC48318;
        arr[8] = 0x835D5E58; arr[9] = 0x90C304C4;
		
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importUnitCollide)) + 0x1c);
    }
    return link;
}

//유닛이 충돌할 때 실행할 함수를 등록합니다
void SetUnitCallbackOnCollide(int unit, int callback)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
	{
		SetMemory(ptr + 0x2b8, ImportUnitCollideFunc());
		SetMemory(ptr + ImportUnitCollideFuncOffset(), callback);
	}
}

int ImportUnitDieFuncOffset()
{
	return 0x228;
}

int ImportUnitDieFunc()
{
    int arr[9], link;

    if (!link)
    {
        arr[0] = 0x50731068; arr[1] = 0x8B565000; arr[2] = 0x8B102474; arr[3] = 0x86 | (ImportUnitDieFuncOffset() << 8);
        arr[4] = 0x006A5600; arr[5] = 0x2454FF50; arr[6] = 0x0CC48314; arr[7] = 0xC483585E;
        arr[8] = 0x9090C304; 
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importUnitDie)) + 0x1c);
    }
    return link;
}

//유닛이 파괴될 때 실행할 함수를 등록합니다
void SetUnitCallbackOnDeath(int unit, int callback)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
	{
		SetMemory(ptr + 0x2d4, ImportUnitDieFunc());
		SetMemory(ptr + ImportUnitDieFuncOffset(), callback);
	}
}

int ImportUnitDropFuncOffset()
{
	return 0x90;
}

int ImportUnitDropFunc() //0x2fc
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x50731068; arr[1] = 0x55565300; arr[2] = 0x14245C8B; arr[3] = 0x1824748B;
        arr[4] = 0x02FCAE8B; arr[5] = 0x53560000; arr[6] = 0x2454FF55; arr[7] = 0x0CC48318;
        arr[8] = 0x835B5E5D; arr[9] = 0x90C304C4;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importUnitDrop)) + 0x1c);
    }
    return link;
}

//유닛을 버릴 때 실행할 함수를 등록합니다
void SetUnitCallbackOnDiscard(int unit, int callback)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
	{
		SetMemory(ptr + 0x2c8, ImportUnitDropFunc());
		SetMemory(ptr + ImportUnitDropFuncOffset(), callback);
	}
}

int ImportUnitPickupFuncOffset()
{
	return 0xb8;
}

int ImportUnitPickupFunc()	//ptr+144, 184,188
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x50731068; arr[1] = 0x55565300; arr[2] = 0x14245C8B; arr[3] = 0x1824748B;
        arr[4] = 0x02E4AE8B; arr[5] = 0x53560000; arr[6] = 0x2454FF55; arr[7] = 0x0CC48318;
        arr[8] = 0x835B5E5D; arr[9] = 0x90C304C4;
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importUnitPickupFunc)) + 0x1c);
    }
    return link;
}

//유닛을 픽업했을 때 실행할 함수를 등록합니다
void SetUnitCallbackOnPickup(int unit, int callback)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
	{
		SetMemory(ptr + 0x2c4, ImportUnitPickupFunc());
		SetMemory(ptr + ImportUnitPickupFuncOffset(), callback);
	}
}

//아이템을 주웠을 때 실행할 함수를 등록합니다. 이 함수는 1회성입니다
void RegistItemPickupCallback(int unit, int callback)
{
	int ptr = UnitToPtr(unit);
	
	if (ptr)
		SetMemory(ptr + 0x300, callback);
}

void NOXLibraryEntryPointFunction()
{
	"export SetMemory";
    "export GetMemory";
    "export needinit ImportCreateAtFunc";
    "export CreateObjectAt";
    "export needinit ImportUnitToPtrFunc";
    "export UnitToPtr";
	"export ImportUseItemFuncOffset";
    "export needinit ImportUseItemFunc";
	"export SetUnitCallbackOnUseItem";
	"export ImportUnitCollideFuncOffset";
    "export ImportUnitCollideFunc";
	"export SetUnitCallbackOnCollide";
	"export ImportUnitDieFuncOffset";
    "export ImportUnitDieFunc";
	"export SetUnitCallbackOnDeath";
	"export ImportUnitDropFuncOffset";
    "export ImportUnitDropFunc";
	"export SetUnitCallbackOnDiscard";
	"export ImportUnitPickupFuncOffset";
	"export ImportUnitPickupFunc";
	"export SetUnitCallbackOnPickup";
	"export RegistItemPickupCallback";

    g_importCreateAt = ImportCreateAtFunc;
    g_importUnitToPtr = ImportUnitToPtrFunc;
    g_importUseitem = ImportUseItemFunc;
    g_importUnitCollide = ImportUnitCollideFunc;
    g_importUnitDie = ImportUnitDieFunc;
    g_importUnitDrop = ImportUnitDropFunc;
	g_importUnitPickupFunc = ImportUnitPickupFunc;
}