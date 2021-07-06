
int g_importNetSendClient, g_playmusicPacket;
int g_netClientExec;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int UnitToPtr(int a){}
void CallFunction(int func){}

int GetPlayerIndex(int plrUnit)
{
    int ptr = UnitToPtr(plrUnit);

    if (ptr)
        return GetMemory(GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0x810);
    return -1;
}

int ValidPlayerCheck(int plrUnit)
{
    int plrArr[32], pIndex = GetPlayerIndex(plrUnit);

    if (pIndex >= 0)
    {
        if (plrUnit ^ plrArr[pIndex])
        {
            plrArr[pIndex] = plrUnit;
            return true;
        }
    }
    return false;
}

int NetClientExec()
{
    int arr[3];

    arr[0] = 0xdf;
    return GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_netClientExec))) + 0x1c));
}

void ClientSetMemory(int user, int targetAddr, int byte)
{
    int ptr = NetClientExec();

    SetMemory(ptr + 1, targetAddr - 0x6d495c);
    SetMemory(ptr + 5, byte);
    NetClientSend(user, ptr, 6);
}

int ImportNetSendClient()
{
    int arr[15], link;

    if (!link)
    {
        arr[0] = 0x40EBC068; arr[1] = 0x72506800; arr[2] = 0x83500050; arr[3] = 0x54FF10EC; arr[4] = 0x44891424;
        arr[5] = 0x54FF0C24; arr[6] = 0x44891424; arr[7] = 0x54FF0824; arr[8] = 0x44891424; arr[9] = 0x54FF0424;
        arr[10] = 0x04891424; arr[11] = 0x2454FF24; arr[12] = 0x10C48318; arr[13] = 0x08C48358; arr[14] = 0x909090C3;
        link = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_importNetSendClient))) + 0x1c));
    }
    return link;
}

void NetClientSend(int plrUnit, int buffPtr, int buffSize)
{
    //netClientSend,0x0040EBC0
    int plrPtr = UnitToPtr(plrUnit), plrIdx;
    int temp = GetMemory(0x5c315c);

    if (plrPtr)
    {
        if (GetMemory(plrPtr + 0x08) & 0x04)
        {
            plrIdx = GetMemory(GetMemory(GetMemory(plrPtr + 0x2ec) + 0x114) + 0x810);
            //5c31ac
            SetMemory(0x5c315c, ImportNetSendClient());
            GroupDamage(plrIdx, 1, buffPtr, buffSize);
            SetMemory(0x5c315c, temp);
        }
    }
}

void NetClientSendRaw(int pIndex, int boolDirection, int buffPtr, int buffSize)
{
    int temp = GetMemory(0x5c315c);

    SetMemory(0x5c315c, ImportNetSendClient());
    GroupDamage(pIndex, boolDirection, buffPtr, buffSize);
    SetMemory(0x5c315c, temp);
}

void NetClientAfterLinking(int pUnit)
{
    ClientSetMemory(pUnit, 0x69ba98 + 0, 0);
    ClientSetMemory(pUnit, 0x69ba98 + 1, 0x10);
    ClientSetMemory(pUnit, 0x69ba98 + 2, 0x75);
}

int PlayMusicPacket()
{
    int packet = 0x6466e5, link;

    if (!link)
    {
        link = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_playmusicPacket))) + 0x1c));
    }
    return link;
}

void NetPlayCustomBgm(int user)
{
    if (CurrentHealth(user))
    {
        NetClientSend(user, PlayMusicPacket(), 3);
    }
}

void NetPlayBgm(int user, int bgmId, int volume)
{
	if (MaxHealth(user))
	{
		int packet = PlayMusicPacket();
		
		SetMemory(packet, 0xe5 | ((bgmId & 0xff) << 8) | ((volume & 0xff) << 16));
		NetClientSend(user, packet, 3);
	}
}

void ClientMainWithCallback(int callbackFunction)
{
	int enabled;
	
	if (!enabled)
	{
		CallFunction(callbackFunction);
		SetMemory(0x69ba98, 0x43de10);
		enabled = true;
	}
}

void ClientEntryWithCallback(int cliUnit, int callbackFunction)
{
	int cliMain = ClientMainWithCallback;
	int targetAddr = 0x751000;
	
    if (!MaxHealth(cliUnit))
        return;
    //6A 00 6A 00/ 68 7F 00 00/ 00 E8 02 63/ DB FF 83 C4/ 0C 68 B0 95/ 4B 00 C3 90: 24Byte
    ClientSetMemory(cliUnit, targetAddr, 0x6a);
    ClientSetMemory(cliUnit, targetAddr + 1, 0x00);
    ClientSetMemory(cliUnit, targetAddr + 2, 0x6a);
    ClientSetMemory(cliUnit, targetAddr + 3, 0x00);
    ClientSetMemory(cliUnit, targetAddr + 4, 0x68);
    ClientSetMemory(cliUnit, targetAddr + 5, cliMain & 0xff);
    ClientSetMemory(cliUnit, targetAddr + 6, (cliMain >> 8) & 0xff);
    ClientSetMemory(cliUnit, targetAddr + 7, 0);
    ClientSetMemory(cliUnit, targetAddr + 8, 0);
    ClientSetMemory(cliUnit, targetAddr + 9, 0xe8);
    ClientSetMemory(cliUnit, targetAddr + 0x0a, 0x02);
    ClientSetMemory(cliUnit, targetAddr + 0x0b, 0x63);
    ClientSetMemory(cliUnit, targetAddr + 0x0c, 0xdb);
    ClientSetMemory(cliUnit, targetAddr + 0x0d, 0xff);
    ClientSetMemory(cliUnit, targetAddr + 0x0e, 0x83);
    ClientSetMemory(cliUnit, targetAddr + 0x0f, 0xc4);
    ClientSetMemory(cliUnit, targetAddr + 0x10, 0x0c);
    ClientSetMemory(cliUnit, targetAddr + 0x11, 0x68);
    ClientSetMemory(cliUnit, targetAddr + 0x12, 0x10);
    ClientSetMemory(cliUnit, targetAddr + 0x13, 0xde);
    ClientSetMemory(cliUnit, targetAddr + 0x14, 0x43);
    ClientSetMemory(cliUnit, targetAddr + 0x15, 0x00);
    ClientSetMemory(cliUnit, targetAddr + 0x16, 0xc3);
    FrameTimerWithArg(1, cliUnit, NetClientAfterLinking);
}

void NOXLibraryEntryPointFunction()
{
	"export GetPlayerIndex";
	"export ValidPlayerCheck";
	"export NetClientExec";
	"export ClientSetMemory";
	"export ImportNetSendClient";
	"export NetClientSend";
	"export NetClientSendRaw";
	"export NetClientAfterLinking";
	"export PlayMusicPacket";
	"export NetPlayCustomBgm";
	"export NetPlayBgm";
	"export ClientMainWithCallback";
	"export ClientEntryWithCallback";
	
	g_importNetSendClient = ImportNetSendClient;
	g_playmusicPacket = PlayMusicPacket;
	g_netClientExec = NetClientExec;
}