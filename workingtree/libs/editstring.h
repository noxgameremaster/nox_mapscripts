
int GetMemory(int a){}
void SetMemory(int a, int b){}
int MemAlloc(int a){}	//memutil.h
void MemFree(int a){}
int NoxUtf8ToUnicode(int a, int b){}	//stringutil.h
int SToInt(string a){}		//typecast.h

void NOXLibraryEntryPointFunction()
{
	"export CsfEditSectionString";
	"export ThingDbEditThingName";
	"export ThingDbEditThingDescription";
}

void CsfEditSectionString(int csfId, string desc)
{
    int key = GetMemory(GetMemory(0x611c00) + (52 * csfId) + 48) >> 0x10;
    int ptr = GetMemory(0x611c04) + (key * 4);
    int prevPtr = GetMemory(ptr);
    int src = GetMemory(0x97bb40 + (SToInt(desc) * 4));
    int newStr = MemAlloc(256);

    NoxUtf8ToUnicode(src, newStr);
    SetMemory(ptr, newStr);
    MemFree(prevPtr);
}
void ThingDbEditThingName(int thingId, string desc)
{
    int thingPtr = GetMemory(GetMemory(0x69f224) + (thingId * 4));
    int srcPtr = GetMemory(0x97bb40 + (SToInt(desc) * 4));
    int destPtr;

    if (thingPtr)
    {
        destPtr = MemAlloc(0x80);   //max 64chars
        NoxUtf8ToUnicode(srcPtr, destPtr);
        SetMemory(thingPtr + 4, destPtr);
    }
}
void ThingDbEditThingDescription(int thingId, string desc)
{
    int thingPtr = GetMemory(GetMemory(0x69f224) + (thingId * 4));
    int srcPtr = GetMemory(0x97bb40 + (SToInt(desc) * 4));
    int destPtr;

    if (thingPtr)
    {
        destPtr = MemAlloc(0x80);   //max 64chars
        NoxUtf8ToUnicode(srcPtr, destPtr);
        SetMemory(thingPtr + 8, destPtr);
    }
}

