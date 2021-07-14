
int g_importChatcore;
int g_uniprint;
int g_importPrintcore;
int g_unichat;
int g_importBroadcast;
int g_uniBroadcast;
int g_signNotify;

int SToInt(string x) { }
string ToStr(int x) { }
void SetMemory(int a, int v){}
int GetMemory(int a){}
int ImportUseItemFunc(){}
int UnitToPtr(int a){}

//사용자의 언어환경이 '한글'인지 확인합니다. 한글이 아닌 경우 0가 반환됩니다
int CheckGameKorLanguage()
{
    if (GetMemory(0x611c04))
        return GetMemory(GetMemory(GetMemory(0x611c04))) == 0xc774d300;
    return 0;
}

int GetByteValue(int ptr)
{
    return GetMemory(ptr) & 0xff;
}

void WriteAddressWordValue(int addr, int word)
{
    int temp = GetMemory(addr) & 0xffff0000;
    SetMemory(addr, temp | word);
}

void NoxUtf8ToUnicode(int src, int dest)
{
    int i = 0, byt;

    while (1)
    {
        byt = GetByteValue(src + i);
        if (!byt) break;
        if (!(byt & 0x80))
        {
            WriteAddressWordValue(dest, byt);
            i ++;
        }
        else if ((byt & 0xe0) == 0xc0)
        {
            WriteAddressWordValue(dest, ((byt & 0x1f) <<6) | (GetByteValue(src + i + 1) & 0x3f));
            i += 2;
        }
        else if ((byt & 0xf0) == 0xe0)
        {
            WriteAddressWordValue(dest, ((byt & 0xf) << 12) | ((GetByteValue(src + i + 1) & 0x3f) << 6) | (GetByteValue(src + i + 2) & 0x3f));
            i += 3;
        }
        dest += 2;
    }
    WriteAddressWordValue(dest, 0);
}

int ImportUniChatCore()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0xC0685657; arr[1] = 0x6800528A; arr[2] = 0x00507250; arr[3] = 0x8B2414FF; arr[4] = 0x2414FFF8;
        arr[5] = 0x14FFF08B; arr[6] = 0x56505724; arr[7] = 0x102454FF; arr[8] = 0x5E14C483; arr[9] = 0x9090C35F;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importChatcore)) + 0x1c);
    }
    return link;
}

void UniChatCore(int plrPtr, int sPtr, int sTime)
{
    int temp = GetMemory(0x5c3320);

    SetMemory(0x5c3320, ImportUniChatCore());
    GroupRunAway(sPtr, plrPtr, sTime);
    SetMemory(0x5c3320, temp);
}

int ImportUniPrintCore()
{
    int arr[8], link;

    if (!link)
    {
        arr[0] = 0x9EB06856; arr[1] = 0x5068004D; arr[2] = 0xFF005072; arr[3] = 0xF08B2414; arr[4] = 0x502414FF;
        arr[5] = 0x2454FF56; arr[6] = 0x10C4830C; arr[7] = 0x9090C35E;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importPrintcore)) + 0x1c);
    }
    return link;
}

void UniPrintCore(int plrPtr, int sPtr)
{
    int temp = GetMemory(0x5c31f4);

    SetMemory(0x5c31f4, ImportUniPrintCore());
    Unused5a(sPtr, plrPtr);
    SetMemory(0x5c31f4, temp);
}

//특정 플레이어에게 메시지를 표시합니다
void UniPrint(int sUnit, string sMsg)
{
    int wDest[200];
    int ptr = UnitToPtr(sUnit), str = SToInt(sMsg), link;

    if (ptr)
    {
        str = GetMemory(0x97bb40 + (str * 4));
        if (!link)
        {
            link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_uniprint)) + 0x1c);
        }
        NoxUtf8ToUnicode(str, link + 8);
        UniPrintCore(ptr, link + 8);
    }
}

//duration 프레임 초 동안 sUnit 유닛이 sMsg 메시지 내용으로 채팅창을 띄웁니다
void UniChatMessage(int sUnit, string sMsg, int duration)
{
    int wDest[200];
    int ptr = UnitToPtr(sUnit), str = SToInt(sMsg), link;

    if (ptr)
    {
        str = GetMemory(0x97bb40 + (str * 4));
        if (!link)
        {
            link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_unichat)) + 0x1c);
        }
        NoxUtf8ToUnicode(str, link + 12);
        UniChatCore(ptr, link + 12, duration);
    }
}

//모든 플레이어의 화면에 메시지를 표시합니다
void UniPrintToAll(string sMsg)
{
    int wDest[200];
    int plrPtr = 0x62f9e0, link, str = SToInt(sMsg), i;

    if (!link)
    {
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_uniprint)) + 0x1c) + 4;
    }
    str = GetMemory(0x97bb40 + (str * 4));
    NoxUtf8ToUnicode(str, link);
    for (i = 0 ; i < 32 ; i ++)
    {
        if (GetMemory(plrPtr))
            UniPrintCore(GetMemory(plrPtr), link);
        plrPtr += 0x12dc;
    }
}

int MathAbs(int num)
{
    if (num < 0)
        num = -num;
    return num;
}

int CheckSignDelay(int sPtr, int gap)
{
    int cFps = GetMemory(0x84ea04);

    if (MathAbs(cFps - GetMemory(sPtr)) > gap)
    {
        SetMemory(sPtr, cFps);
        return 1;
    }
    return 0;
}

void SignNotification()
{
    int otPtr = GetMemory(0x979720), sePtr = GetMemory(0x979724);

    if (otPtr && sePtr)
    {
        if (CheckSignDelay(GetMemory(sePtr + 0x2e0) + 100, 60))
        {
            UniPrint(other, ToStr(GetMemory(sePtr + 0x2f0)));
        }
    }
}

//표지판 메시지를 등록합니다
void RegistSignMessage(int sUnit, string sMsg)
{
    int ptr = UnitToPtr(sUnit), str = SToInt(sMsg), link;

    if (ptr)
    {
        SetMemory(ptr + 0x2f0, str);
        SetMemory(ptr + 0x2dc, ImportUseItemFunc());
        SetMemory(ptr + 0x2fc, -g_signNotify);
    }
}

int ImportUniBroadcast()
{
    int arr[6], link;

    if (!link)
    {
        arr[0] = 0x4D9FD068; arr[1] = 0x72506800; arr[2] = 0x14FF0050; arr[3] = 0x106A5024;
        arr[4] = 0x0C2454FF; arr[5] = 0xC310C483;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_importBroadcast)) + 0x1c);
    }
    return link;
}

//모든 플레이어에게 공고 메시지를 보여줍니다.
//공고 메시지 버그로 인해 이 함수사용을 비추천합니다
void UniBroadcast(string sMsg)
{
    int wDest[100];
    int temp = GetMemory(0x5c3108), link, str = GetMemory(0x97bb40 + (SToInt(sMsg) * 4));

    if (!link)
    {
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-g_uniBroadcast)) + 0x1c);
    }
    NoxUtf8ToUnicode(str, link + 4);
    SetMemory(0x5c3108, ImportUniBroadcast());
    Unused1f(link + 4);
    SetMemory(0x5c3108, temp);
}

void NOXLibraryEntryPointFunction()
{
	"export CheckGameKorLanguage";
    "export GetByteValue";
    "export WriteAddressWordValue";
    "export NoxUtf8ToUnicode";
    "export needinit ImportUniChatCore";
    "export UniChatCore";
    "export needinit ImportUniPrintCore";
    "export UniPrintCore";
    "export UniPrint";
    "export UniChatMessage";
    "export UniPrintToAll";
    "export MathAbs";
    "export CheckSignDelay";
    "export SignNotification";
    "export RegistSignMessage";
    "export ImportUniBroadcast";
    "export UniBroadcast";

    g_importChatcore = ImportUniChatCore;
    g_uniprint = UniPrint;
    g_importPrintcore = ImportUniPrintCore;
    g_unichat = UniChatMessage;
    g_importBroadcast = ImportUniBroadcast;
    g_uniBroadcast = UniBroadcast;
    g_signNotify = SignNotification;
}
