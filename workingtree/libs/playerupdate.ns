
int g_callnode54d2b0;
int g_dieplayerhandlercc;
int g_callnode4f8100;
int g_player4f8100;
int g_player4f8460;
int g_opcodeCopiesAd;

int g_callnode4f7ef0;
int g_playerRespawn4f7ef0;
int g_redrawRespawn;
int g_importSelfDmg;
int g_selfDmgExit;


int GetMemory(int a){}
void SetMemory(int a, int b){}

int UnitToPtr(int a){}

int GetScrDataField(int functionName)
{
    return GetMemory(GetMemory(0x75ae28) + (0x30 * functionName + 0x1c));

    "export GetScrDataField";
}

int GetScrCodeField(int functionName)
{
    return GetMemory(GetMemory(0x75ae28) + (0x30 * functionName + 0x20));

    "export GetScrCodeField";
}

int FixCallOpcode(int curAddr, int targetAddr)
{
    SetMemory(curAddr + 1, targetAddr - curAddr - 5);
    return 0;

    "export FixCallOpcode";
}

int OpcodeGetTargetAddr(int curAddr)
{
    return GetMemory(curAddr + 1) + curAddr + 5;
    "export OpcodeGetTargetAddr";
}

void OpcodeLoadEffectiveAddr(int loadAddr, int codeAddr, int offset)
{
    int targetAddr = OpcodeGetTargetAddr(codeAddr + offset);

    SetMemory(loadAddr + offset + 1, targetAddr - (loadAddr + offset) - 5);

    "export OpcodeLoadEffectiveAddr";
}

void OpcodeCopiesAdvance(int destPtr, int callNodePtr, int startAddr, int endAddr)
{
    int destBase = destPtr, curAddr = startAddr;
    int flink = g_opcodeCopiesAd;
    int vptr = GetScrDataField(-flink);

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
    "export OpcodeCopiesAdvance";
}

void ChangePlayerDeathSound(int handle, int soundNumber) //@brief. 유저가 죽었을 때 효과음을 변경합니다
{
    SetMemory(handle + 0x258, soundNumber);  //@brief. SoundList.txt 파일을 참고하세요
    "export ChangePlayerDeathSound";
}

int CallNode54d2b0()
{
    int arr[40];
    int flink = g_callnode54d2b0;

    if (!arr[0])
    {
        arr[0] = 0x54d2d1;
        arr[1] = 0x54d2e3;
        arr[2] = 0x54d2f3;
        arr[3] = 0x54d30a;
        arr[4] = 0x54d348;
        arr[5] = 0x54d375;
        arr[6] = 0x54d3aa;
        arr[7] = 0x54d45c;
        arr[8] = 0x54d47b;
        arr[9] = 0x54d513;
        arr[10] = 0x54d527;
        arr[11] = 0x54d566;
        arr[12] = 0x54d574;
        arr[13] = 0x54d588;
        arr[14] = 0x54d594;
        arr[15] = 0x54d5a6;
        arr[16] = 0x54d5b2;
        arr[17] = 0x54d5c4;
        arr[18] = 0x54d5d1;
        arr[19] = 0x54d5e2;
        arr[20] = 0x54d5f4;
        arr[21] = 0x54d610;
        arr[22] = 0x54d622;
        arr[23] = 0x54d62c;
        arr[24] = 0x54d639;
        arr[25] = 0x54d642;
        arr[26] = 0x54d65b;
        arr[27] = 0x54d662;
        arr[28] = 0x54d668;
        arr[29] = 0x54d67a;
        arr[30] = 0x54d680;
        arr[31] = 0x54d693;
        arr[32] = 0x54d6a6;
        arr[33] = 0x54d727;
        arr[34] = 0x54d72d;
        arr[35] = 0x54d733;
        arr[36] = 0x54d73d;
        arr[37] = 0x54d749;
        arr[38] = 0x54d782;
        "export CallNode54d2b0";
    }
    return GetScrDataField(-flink);
}

int DiePlayerHandlerCopiedCode()
{
    int arr[314], link;

    if (!link)
    {
        int flink = g_dieplayerhandlercc;
        link = GetScrDataField(-flink);
        OpcodeCopiesAdvance(link, CallNode54d2b0(), 0x54d2b0, 0x54d794);
        "export DiePlayerHandlerCopiedCode";
    }
    return link;
}

int Callnode4f8100()
{
    "export Callnode4f8100";
    int node[20], link;

    if (!link)
    {
        int flink = g_callnode4f8100;
        node[0] = 0x4f8145;
        node[1] = 0x4f816e;
        node[2] = 0x4f81ad;
        node[3] = 0x4f820b;
        node[4] = 0x4f8218;
        node[5] = 0x4f8221;
        node[6] = 0x4f826b;
        node[7] = 0x4f8326;
        node[8] = 0x4f836e;
        node[9] = 0x4f8381;
        node[10] = 0x4f8387;
        node[11] = 0x4f838f;
        node[12] = 0x4f83a3;
        node[13] = 0x4f83ac;
        node[14] = 0x4f83c3;
        node[15] = 0x4f83dc;
        node[16] = 0x4f83eb;
        node[17] = 0x4f8407;
        node[18] = 0; //nullptr
        link = GetScrDataField(-flink);
    }
    return link;
}

int Callnode004f7ef0()
{
    "export Callnode004f7ef0";
    int node[21], link;

    if (!link)
    {
        int flink = g_callnode4f7ef0;
        node[0] = 0x4f7ef7;
        node[1] = 0x4f7f1b;
        node[2] = 0x4f7f48;
        node[3] = 0x4f7f59;
        node[4] = 0x4f7f7c;
        node[5] = 0x4f7f8b;
        node[6] = 0x4f7f98;
        node[7] = 0x4f7fb5;
        node[8] = 0x4f7fd6;
        node[9] = 0x4f7ff1;
        node[10] = 0x4f800d;
        node[11] = 0x4f801a;
        node[12] = 0x4f8028;
        node[13] = 0x4f802f;
        node[14] = 0x4f803d;
        node[15] = 0x4f8055;
        node[16] = 0x4f807a;
        node[17] = 0x4f8087;
        node[18] = 0x4f80a1;
        node[19] = 0; //nullptr
        link = GetScrDataField(-flink);
    }
    return link;
}

int RedrawOnRespawn()
{
    "export RedrawOnRespawn";
    int arr[16], link;

    if (!link)
    {
        int flink = g_redrawRespawn;
        arr[0] = 0x50EC8B55; arr[1] = 0x758B5651; arr[2] = 0xF88E8B08; arr[3] = 0x85000001; arr[4] = 0x8B2674C9; arr[5] = 0x00251041; arr[6] = 0x85000001; arr[7] = 0x511274C0;
        arr[8] = 0x1F8AE856; arr[9] = 0x595EFFDA; arr[10] = 0x41E85651; arr[11] = 0x5EFFDA1F; arr[12] = 0xF0898B59; arr[13] = 0xEB000001; arr[14] = 0x58595ED6; arr[15] = 0x9090C35D;
        link = GetScrDataField(-flink);
        FixCallOpcode(link + 0x21, 0x4f2fb0);
        FixCallOpcode(link + 0x2a, 0x4f2f70);
    }
    return link;
}

int PlayerRespawn004f7ef0()     //@brief. 유저 리스폰 복사본
{
    "export PlayerRespawn004f7ef0";
    int codes[115], link;

    if (!link)
    {
        int flink = g_playerRespawn4f7ef0;
        link = GetScrDataField(-flink);
        OpcodeCopiesAdvance(link, Callnode004f7ef0(), 0x4f7ef0, 0x4f80b4);
        SetMemory(link + 0x62, 0x16a9090);
        SetMemory(link + 0x94, (GetMemory(link + 0x94) & (~0xffff)) ^ 0x9090);
        FixCallOpcode(link + 0x9b, RedrawOnRespawn());
    }
    return link;
}

int PlayerUpdate4f8460()
{
    "export PlayerUpdate4f8460";
    int arr[75], link;

    if (!link)
    {
        int flink = g_player4f8460;
        arr[0] = 0x530CEC83; arr[1] = 0x748B5655; arr[2] = 0x31571C24; arr[3] = 0xECBE8BC0; arr[4] = 0x89000002; arr[5] = 0x89202444; arr[6] = 0x8A142444;
        arr[7] = 0xF8835847; arr[8] = 0x83427721; arr[9] = 0x077404F8; arr[10] = 0x8C8524FF; arr[11] = 0x8B004F99; arr[12] = 0x84EA0415; arr[13] = 0x88AE8B00;
        arr[14] = 0xA1000000; arr[15] = 0x0085B3FC; arr[16] = 0xE8D1EA29; arr[17] = 0x1776C239; arr[18] = 0x00040068; arr[19] = 0x4CDEE800; arr[20] = 0xC483F28D;
        arr[21] = 0x74C08504; arr[22] = 0x90806812; arr[23] = 0x68C3004F; arr[24] = 0x004F9983; arr[25] = 0x850B68C3; arr[26] = 0x68C3004F; arr[27] = 0x00002000;
        arr[28] = 0x8D4CBBE8; arr[29] = 0x04C483F2; arr[30] = 0x4A74C085; arr[31] = 0x0114878B; arr[32] = 0x80F60000; arr[33] = 0x00000E60; arr[34] = 0x313B7501;
        arr[35] = 0x10888AC9; arr[36] = 0x51000008; arr[37] = 0x9E5227E8; arr[38] = 0x04C483F2; arr[39] = 0x2674C085; arr[40] = 0x000006BB; arr[41] = 0x08583900;
        arr[42] = 0x978B3A74; arr[43] = 0x00000114; arr[44] = 0x8AC03150; arr[45] = 0x00081082; arr[46] = 0x71E85000; arr[47] = 0x83F29E52; arr[48] = 0xC08508C4;
        arr[49] = 0x95E8DF74; arr[50] = 0x85F28D4C; arr[51] = 0x560F74C0; arr[52] = 0x00011BE8; arr[53] = 0x04C48300; arr[54] = 0x4F850B68; arr[55] = 0x8368C300;
        arr[56] = 0xC3004F99; arr[57] = 0x01148F8B; arr[58] = 0xD2310000; arr[59] = 0x0810918A; arr[60] = 0xE8520000; arr[61] = 0xF29E52A8; arr[62] = 0x5604C483;
        arr[63] = 0x9C255FE8; arr[64] = 0x04C483F2; arr[65] = 0x4F850B68; arr[66] = 0x9090C300;
        link = GetScrDataField(-flink);
        
        FixCallOpcode(link + 0x4d, 0x40a5c0);
        FixCallOpcode(link + 0x70, 0x40a5c0);
        FixCallOpcode(link + 0x94, 0x51ab50);
        FixCallOpcode(link + 0xba, 0x51abc0);
        FixCallOpcode(link + 0xc6, 0x40a5f0);
        // FixCallOpcode(link + 0xd0, 0x4f7ef0);       //@todo. 여기를 서브클래싱 해줘야 한다!
        FixCallOpcode(link + 0xd0, PlayerRespawn004f7ef0());       //@brief. 서브클래싱 링크
        FixCallOpcode(link + 0xf3, 0x51ac30);
        // FixCallOpcode(link + 0xfc, 0x4f7ef0);           //@brief. todo
        FixCallOpcode(link + 0xfc, PlayerRespawn004f7ef0());           //@brief. todo
    }
    return link;
}

int PlayerUpdate4f8100()    //@brief. 유저 업데이트 복사본
{
    "export PlayerUpdate4f8100";
    int codes[200], link;

    if (!link)
    {
        int flink = g_player4f8100;
        link = GetScrDataField(-flink);
        OpcodeCopiesAdvance(link, Callnode4f8100(), 0x4f8100, 0x4f8414);
        FixCallOpcode(link + 0x287, PlayerUpdate4f8460());  //@brief. 4f8460으로 링크
    }
    return link;
}

void DiePlayerHandlerEntry(int plrUnit)  //@brief. 해당 유저를 재정의된 시스템에 등록합니다
{
    "export DiePlayerHandlerEntry";
    int ptr = UnitToPtr(plrUnit);

    if (ptr)
    {
        SetMemory(ptr + 0x2d4, DiePlayerHandlerCopiedCode());  //@brief. 플레이어 데스 재정의 핸들러 등록!
        SetMemory(ptr + 0x2e8, PlayerUpdate4f8100());       //@brief. 플레이어 업데이트 재정의 핸들러 등록!
    }
}

void PlayerInitializeUpdateHook()
{
    "export needinit PlayerInitializeUpdateHook";
    int ptr = DiePlayerHandlerCopiedCode();

    ChangePlayerDeathSound(ptr, 913);
    PlayerUpdate4f8100();
    SetMemory(ptr + 0x386, (GetMemory(ptr + 0x386) & 0xffff0000) | 0x9eb);
}

int ImportCheckSelfDamage()
{
    "export needinit ImportCheckSelfDamage";
    int arr[14], link;

    if (!link)
    {
        int flink = g_importSelfDmg;
        arr[0] = 0x4C8B5651; arr[1] = 0xC9850C24; arr[2] = 0x748B2374; arr[3] = 0xF6851024; arr[4] = 0xF1391B74; arr[5] = 0x8B501374; arr[6] = 0x0001FC86;
        arr[7] = 0x74C08500; arr[8] = 0x58F08B05; arr[9] = 0xEB58ECEB; arr[10] = 0xC3595E04; arr[11] = 0x68595E90; arr[12] = 0x004E17B0; arr[13] = 0x909090C3;
        link = GetScrDataField(-flink);
    }
    return link;
}

void SelfDamageClassEntry(int plrUnit)
{
    "export SelfDamageClassEntry";
    int ptr = UnitToPtr(plrUnit);

    if (ptr)
        SetMemory(ptr + 0x2cc, ImportCheckSelfDamage());
}

void CancelPlayerDialogWithPTR(int plrPtr)
{
    "export CancelPlayerDialogWithPTR";
    if (GetMemory(GetMemory(plrPtr + 0x2ec) + 0x11c))
    {
        SetMemory(GetMemory(plrPtr + 0x2ec) + 0x11c, 0);
        SetMemory(GetMemory(GetMemory(plrPtr + 0x2ec) + 0x114) + 0xe60, 0x10);
    }
}

void ResetPlayerHandlerWhenExitMap()
{
    "export ResetPlayerHandlerWhenExitMap";
    int i, pTable = 0x62f9e0, ptr;
    int flink = g_selfDmgExit;
    int link = GetScrDataField(-flink);

    for (i = 31 ; i >= 0 ; SetMemory(link, GetMemory(link) - 1))
    {
        SetMemory(link + 8, GetMemory(pTable));
        if (ptr)
        {
            SetMemory(ptr + 0x2cc, 0x4e17b0);
            SetMemory(ptr + 0x2d4, 0x54d2b0);
            if (GetMemory(ptr + 0x2e8) ^ 0x4E62F0)      //@brief. 관객모드가 아닐 경우에만 되돌립니다//
                SetMemory(GetMemory(pTable) + 0x2e8, 0x4f8100);     //@brief. 업데이트 되돌리기//
            CancelPlayerDialogWithPTR(GetMemory(pTable));
        }
        SetMemory(link + 4, GetMemory(link + 4) + 0x12dc);
    }
}

void NOXLibraryEntryPointFunction()
{
    g_callnode54d2b0 = CallNode54d2b0;
    g_dieplayerhandlercc = DiePlayerHandlerCopiedCode;
    g_callnode4f8100 = Callnode4f8100;
    g_player4f8100 = PlayerUpdate4f8100;
    g_player4f8460 = PlayerUpdate4f8460;
    g_opcodeCopiesAd = OpcodeCopiesAdvance;
    g_callnode4f7ef0 = Callnode004f7ef0;
    g_playerRespawn4f7ef0 = PlayerRespawn004f7ef0;
    g_redrawRespawn = RedrawOnRespawn;
    g_importSelfDmg = ImportCheckSelfDamage;
    g_selfDmgExit = ResetPlayerHandlerWhenExitMap;
}