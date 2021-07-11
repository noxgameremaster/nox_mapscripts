
int g_callNode4f31e0;
int g_callNode4f37d0;
int g_potionPickupPart2Code;
int g_potionPickupPartCode;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int UnitToPtr(int a){}
int GetScrDataField(int functionName){}
void OpcodeCopiesAdvance(int destPtr, int callNodePtr, int startAddr, int endAddr){}

int CallNode4f37d0()
{
    int arr[21];

    if (!arr[0])
    {
        arr[0] = 0x4f37f1;
        arr[1] = 0x4f3806;
        arr[2] = 0x4f382e;
        arr[3] = 0x4f3841;
        arr[4] = 0x4f3852;
        arr[5] = 0x4f3862;
        arr[6] = 0x4f38d4;
        arr[7] = 0x4f38fb;
        arr[8] = 0x4f390a;
        arr[9] = 0x4f3973;
        arr[10] = 0x4f3992;
        arr[11] = 0x4f39a1;
        arr[12] = 0x4f39c8;
        arr[13] = 0x4f39d9;
        arr[14] = 0x4f39e2;
        arr[15] = 0x4f39eb;
        arr[16] = 0x4f3a06;
        arr[17] = 0x4f3a19;
        arr[18] = 0x4f3a2a;
        arr[19] = 0x4f3a43;
    }
    return GetScrDataField(-(g_callNode4f37d0));
}

int CallNode4f31e0()
{
    int arr[12];

    if (!arr[0])
    {
        arr[0] = 0x4f31e8;
        arr[1] = 0x4f3200;
        arr[2] = 0x4f3211;
        arr[3] = 0x4f3223;
        arr[4] = 0x4f325a;
        arr[5] = 0x4f32c8;
        arr[6] = 0x4f32e9;
        arr[7] = 0x4f32f5;
        arr[8] = 0x4f3315;
        arr[9] = 0x4f3324;
        arr[10] = 0x4f3330;
    }
    return GetScrDataField(-(g_callNode4f31e0));
}

int PotionPickupPart2Code()
{
    int arr[90], link;

    if (!link)
    {
        link = GetScrDataField(-(g_potionPickupPart2Code));
        OpcodeCopiesAdvance(link, CallNode4f31e0(), 0x4f31e0, 0x4f3344);
    }
    return link;
}

int PotionPickupPartCode()
{
    int arr[162], link;

    if (!link)
    {
        link = GetScrDataField(-(g_potionPickupPartCode));
        OpcodeCopiesAdvance(link, CallNode4f37d0(), 0x4f37d0, 0x4f3a54);
    }
    return link;
}

void PotionPickupCustomizing()
{
    int ptr1 = PotionPickupPartCode();
    int ptr2 = PotionPickupPart2Code();

    SetMemory(ptr1 + 0x68, (GetMemory(ptr1 + 0x68) & 0xffffff00) | 0xeb);
    SetMemory(ptr2 + 0x128, (GetMemory(ptr2 + 0x128) & 0xffffff00) | 0x09);
    SetMemory(ptr1 + 0x25a + 1, ptr2 - (ptr1 + 0x25a) - 5);
}

int PotionPickupRegist(int potion)
{
    int ptr = UnitToPtr(potion);

    if (ptr)
    {
        if (GetMemory(ptr + 0x2c4) == 0x4f37d0)
            SetMemory(ptr + 0x2c4, PotionPickupPartCode());
    }
    return potion;
}

void NOXLibraryEntryPointFunction()
{
	"export CallNode4f37d0";
	"export CallNode4f31e0";
	"export PotionPickupPart2Code";
	"export PotionPickupPartCode";
	"export PotionPickupCustomizing";
	"export PotionPickupRegist";
	
	g_callNode4f31e0 = CallNode4f31e0;
	g_callNode4f37d0 = CallNode4f37d0;
	g_potionPickupPart2Code = PotionPickupPart2Code;
	g_potionPickupPartCode = PotionPickupPartCode;
}
