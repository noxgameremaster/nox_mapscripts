
int g_cmdline;
int g_noxopcodecmdline;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int SToInt(string x) { StopScript(x); }
int GetScrDataField(int functionName){}
void NoxUtf8ToUnicode(int src, int dest) { }

int FixCallOpcode(int curAddr, int targetAddr)
{
    SetMemory(curAddr + 1, targetAddr - curAddr - 5);
    return 0;
}

int NoxOpcodeCmdLine()
{
    int arr[8], link;

    if (!link)
    {
        arr[0] = 0x49E85650; arr[1] = 0x85FFDB62; arr[2] = 0x8B1174C0; arr[3] = 0x8D30FFF0;
        arr[4] = 0x30FF0446; arr[5] = 0xCF2C68E8; arr[6] = 0x08C483FF; arr[7] = 0x90C3585E;
        int flink = g_noxopcodecmdline;
        link = GetScrDataField(-flink);
        FixCallOpcode(link + 2, 0x507250);
        FixCallOpcode(link + 0x14, 0x443c80);
    }
    return link;
}

void CmdLine(string commandMessage, int isShow)
{
    int destPtr = GetScrDataField(-(g_cmdline)) + 12;
    int dest[100];
    int srcPtr = GetMemory(0x97bb40 + (SToInt(commandMessage) * 4));
    int previousFunctionPtr = GetMemory(0x5c3108);

    NoxUtf8ToUnicode(srcPtr, destPtr);
    SetMemory(0x5c3108, NoxOpcodeCmdLine());
    Unused1f(GetScrDataField(-(g_cmdline)) + 4);
    SetMemory(0x5c3108, previousFunctionPtr);
}

void NOXLibraryEntryPointFunction()
{
    "export FixCallOpcode";
    "export needinit NoxOpcodeCmdLine";
    "export CmdLine";

    g_noxopcodecmdline = NoxOpcodeCmdLine;
    g_cmdline = CmdLine;
}