
int g_callNode00549380;
int g_monsterCodeSection;
int g_monsterStrikeHookSub;
int g_monsterStrikeDefaultCallback;

int GetMemory(int a){}
void SetMemory(int a,int b){}
int UnitToPtr(int u){}
int GetScrDataField(int functionName){}
int FixCallOpcode(int curAddr, int targetAddr){}
void OpcodeCopiesAdvance(int destPtr, int callNodePtr, int startAddr, int endAddr){}

int CallNode00549380()
{
    int link, node[4];

    if (!link)
    {
        node[0] = 0x549394;
        node[1] = 0x5493d0;
        node[2] = 0x52df80;
        node[3] = 0;    //nullptr
        link = GetScrDataField(-(g_callNode00549380)) + 4;
    }
    return link;
}

int MonsterStrikeHookSub()
{
    int arr[8], link;

    if (!link)
    {
        arr[0] = 0x02CC96FF; arr[1] = 0x51500000; arr[2] = 0x0000FFB8; arr[3] = 0x50565700; arr[4] = 0xDB623BE8; arr[5] = 0x0CC483FF; arr[6] = 0xFB685859; arr[7] = 0xC3005493;

        link = GetScrDataField(-(g_monsterStrikeHookSub));
    }
    return link;
}

int MonsterStrikeCodeSection()
{
	int padding[10];
    int code[48], endChunk = 0xdeadface;
}

void MonsterStrikeDefaultCallback()
{ }

int MonsterStrikeHandlerCopiedCode(int callback)
{
    int link; //192Bytes 0xc0
    int subPart;

    if (!link)
    {
        link = GetScrDataField(-(g_monsterCodeSection)) + 40;
        OpcodeCopiesAdvance(link, CallNode00549380(), 0x549380, 0x549434);
        subPart = MonsterStrikeHookSub();
        SetMemory(subPart + 27, link + 0x7b);
        SetMemory(subPart + 9, callback);
        FixCallOpcode(subPart + 16, 0x507310);

        SetMemory(link + 0x75, (GetMemory(link + 0x75) & (~0xff)) ^ 0x68);  //e8 xx xx xx xx
        SetMemory(link + 0x7a, (GetMemory(link + 0x7a) & (~0xff)) ^ 0xc3);
        SetMemory(link + 0x76, subPart);
    }
    return link;
}

//can override
int MonsterMeleeAttackRegistCallback()
{
	return (-(g_monsterStrikeDefaultCallback));
}

void InitMonsterStrikeHandler()
{
	MonsterStrikeCodeSection();
	MonsterStrikeHandlerCopiedCode(MonsterMeleeAttackRegistCallback());
}

void RegistUnitStrikeCallback(int callback)
{
	int subPart = MonsterStrikeHookSub();
	
	SetMemory(subPart + 9, callback);
}

void RegistUnitStrikeHook(int sUnit)
{
    int ptr = UnitToPtr(sUnit), temp, binScrPtr;

    if (ptr)
    {
        temp = GetMemory(ptr + 0x2ec);
        if (temp)
        {
            binScrPtr = GetMemory(GetMemory(ptr + 0x2ec) + 0x1e4);
            if (binScrPtr)
                SetMemory(binScrPtr + 0xec, MonsterStrikeHandlerCopiedCode(0));
        }
    }
}

void NOXLibraryEntryPointFunction()
{
	"export CallNode00549380";
	"export MonsterStrikeHookSub";
	"export MonsterStrikeCodeSection";
	"export MonsterStrikeDefaultCallback";
	"export MonsterStrikeHandlerCopiedCode";
	"export MonsterMeleeAttackRegistCallback";
	"export needinit InitMonsterStrikeHandler";
	"export RegistUnitStrikeCallback";
	"export RegistUnitStrikeHook";
	
	g_callNode00549380 = CallNode00549380;
	g_monsterCodeSection = MonsterStrikeCodeSection;
	g_monsterStrikeHookSub = MonsterStrikeHookSub;
	g_monsterStrikeDefaultCallback = MonsterStrikeDefaultCallback;
}


