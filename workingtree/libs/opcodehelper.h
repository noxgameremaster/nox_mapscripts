
int g_opcodeCopiesAd;

int GetMemory(int a){}
void SetMemory(int a,int b){}
int GetScrDataField(int functionName){}

int FixCallOpcode(int curAddr, int targetAddr)
{
    SetMemory(curAddr + 1, targetAddr - curAddr - 5);
    return 0;
}

int OpcodeGetTargetAddr(int curAddr)
{
    return GetMemory(curAddr + 1) + curAddr + 5;
}

void OpcodeLoadEffectiveAddr(int loadAddr, int codeAddr, int offset)
{
    int targetAddr = OpcodeGetTargetAddr(codeAddr + offset);

    SetMemory(loadAddr + offset + 1, targetAddr - (loadAddr + offset) - 5);
}

void OpcodeCopiesAdvance(int destPtr, int callNodePtr, int startAddr, int endAddr)
{
    int destBase = destPtr, curAddr = startAddr;
    int vptr = GetScrDataField(-(g_opcodeCopiesAd));

    while (curAddr <= endAddr)
    {
        SetMemory(destPtr, GetMemory(curAddr));
        if (GetMemory(callNodePtr))
        {
            if (GetMemory(callNodePtr) + 1 <= curAddr)
            {
                OpcodeLoadEffectiveAddr(destBase, startAddr, GetMemory(callNodePtr) - startAddr);
                SetMemory(vptr + 4, GetMemory(vptr + 4) + 4);
            }
        }
        SetMemory(vptr + 0x14, GetMemory(vptr + 0x14) + 4);
        SetMemory(vptr, GetMemory(vptr) + 4);
    }
}

void NOXLibraryEntryPointFunction()
{
	"export FixCallOpcode";
	"export OpcodeGetTargetAddr";
	"export OpcodeLoadEffectiveAddr";
	"export OpcodeCopiesAdvance";
	g_opcodeCopiesAd = OpcodeCopiesAdvance;
}