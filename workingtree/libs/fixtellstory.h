
int g_tellA;
int g_tellB;

void SetMemory(int a, int v){}
int GetMemory(int a){}
int SToInt(string x) { StopScript(x); }

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

int ImportTellStoryUniNamePartB()
{
    int arr[84], link;
    int funcIndex = g_tellB;

    if (!link)
    {
        arr[0] = 0x0424448B; arr[1] = 0x0824548B; arr[2] = 0x008CEC81; arr[3] = 0xC0850000; arr[4] = 0xEC9A8B53; arr[5] = 0x55000002; arr[6] = 0x02ECA88B;
        arr[7] = 0x0F570000; arr[8] = 0x00012184; arr[9] = 0x0840F600; arr[10] = 0x17840F04; arr[11] = 0x8B000001; arr[12] = 0x00A824BC; arr[13] = 0xFF850000;
        arr[14] = 0x0108840F; arr[15] = 0xC9830000; arr[16] = 0x89C033FF; arr[17] = 0x00011C95; arr[18] = 0xAEF25600; arr[19] = 0x748DD1F7; arr[20] = 0xF92B1624;
        arr[21] = 0x7489C18B; arr[22] = 0xF78B1024; arr[23] = 0x10247C8B; arr[24] = 0xC602E9C1; arr[25] = 0xD0142444; arr[26] = 0x152444C6; arr[27] = 0x8BA5F303;
        arr[28] = 0x5CC868C8; arr[29] = 0x1F6A0056; arr[30] = 0xF303E183; arr[31] = 0xC9C8B8A4; arr[32] = 0x4C8D0069; arr[33] = 0x51503E24; arr[34] = 0x0C2454FF;
        arr[35] = 0xB824848B; arr[36] = 0x66000000; arr[37] = 0x842484C7; arr[38] = 0x00000000; arr[39] = 0x38938A00; arr[40] = 0x8D000008; arr[41] = 0x00083ABB;
        arr[42] = 0x24948800; arr[43] = 0x000000AA; arr[44] = 0x86248489; arr[45] = 0x8A000000; arr[46] = 0x10C48307; arr[47] = 0x2374C084; arr[48] = 0x31FFC983;
        arr[49] = 0xF7AEF2C0; arr[50] = 0x8DCF29D1; arr[51] = 0x8B7A2454; arr[52] = 0x8BF78BC1; arr[53] = 0x02E9C1FA; arr[54] = 0xC88BA5F3; arr[55] = 0xF303E183;
        arr[56] = 0x8B2FEBA4; arr[57] = 0x5CD1640D; arr[58] = 0x68158B00; arr[59] = 0xA1005CD1; arr[60] = 0x005CD16C; arr[61] = 0x7A244C89; arr[62] = 0x700D8B66;
        arr[63] = 0x89005CD1; arr[64] = 0x897E2454; arr[65] = 0x00822484; arr[66] = 0x89660000; arr[67] = 0x0086248C; arr[68] = 0x858B0000; arr[69] = 0x00000114;
        arr[70] = 0x4E542068; arr[71] = 0x31016A00; arr[72] = 0x8A006AC9; arr[73] = 0x00081088; arr[74] = 0x24548D00; arr[75] = 0x00876820; arr[76] = 0x51520000;
        arr[77] = 0x142454FF; arr[78] = 0xB424948B; arr[79] = 0x6A000000; arr[80] = 0xC4835200; arr[81] = 0x5D5F5E20; arr[82] = 0x8CC4815B; arr[83] = 0xC3000000;
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-funcIndex)) + 0x1c);
    }
    return link;
}

int ImportTellStoryUniNamePartA()
{
    int arr[19], link;
    int funcIndex = g_tellA;

    if (!link)
    {
        arr[0] = 0x8DE06856; arr[1] = 0x50680054; arr[2] = 0xFF005072; arr[3] = 0xF08B2414; arr[4] = 0x8B2414FF; 
        arr[5] = 0xBB40B50C; arr[6] = 0x148B0097; arr[7] = 0x97BB4085; arr[8] = 0x50685100; arr[9] = 0x520040AF; 
        arr[10] = 0x042454FF; arr[11] = 0x97200D8B; arr[12] = 0xC4830097; arr[13] = 0x24A15008; arr[14] = 0x50009797; 
        arr[15] = 0x2454FF51; arr[16] = 0x10C48314; arr[17] = 0xC483C031; arr[18] = 0x90C35E08;
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-funcIndex)) + 0x1c);
        SetMemory(link + 2, ImportTellStoryUniNamePartB());
    }
    return link;
}

void TellStoryUnitName(string sAudio, string sDesc, string sUnitName)
{
    int temp = GetMemory(0x5c3290), src = GetMemory(0x97bb40 + (SToInt(sUnitName) * 4));

    SetMemory(0x5c3290, ImportTellStoryUniNamePartA());
    NoxUtf8ToUnicode(src, 0x69C9C8);
    TellStory(sAudio, sDesc);
    SetMemory(0x5c3290, temp);
}

void NOXLibraryEntryPointFunction()
{
    "export SToInt";
    "export GetByteValue";
    "export WriteAddressWordValue";
    "export NoxUtf8ToUnicode";
    "export needinit ImportTellStoryUniNamePartB";
    "export needinit ImportTellStoryUniNamePartA";
    "export TellStoryUnitName";

    g_tellA = ImportTellStoryUniNamePartA;
    g_tellB = ImportTellStoryUniNamePartB;
}
