
int g_makecoopTeam;
int g_importPlayerTeamAssign;
int g_removeCoopTeam;

int GetMemory(int a){}
void SetMemory(int a, int b){}

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

int ImportPlayerAutoTeamAssign()
{
    "export ImportPlayerAutoTeamAssign";
    int arr[17], link;

    if (!link)
    {
        int flink = g_importPlayerTeamAssign;
        arr[0] = 0x4191D068; arr[1] = 0x50515600; arr[2] = 0x000020B9; arr[3] = 0xF9E0B800; arr[4] = 0xC9850062;
        arr[5] = 0x8B492774; arr[6] = 0x12DC0530; arr[7] = 0xF6850000; arr[8] = 0x5150F074; arr[9] = 0x8D24468B;
        arr[10] = 0x016A304E; arr[11] = 0x51016A50; arr[12] = 0x54FF016A; arr[13] = 0xC4832824; arr[14] = 0xEB585914;
        arr[15] = 0x5E5958D5; arr[16] = 0xC304C483;
        link = GetScrDataField(-flink);
    }
    return link;
}

void PlayerAutoTeamAssign()
{
    "export PlayerAutoTeamAssign";
    int temp = GetMemory(0x5c31cc);

    SetMemory(0x5c31cc, ImportPlayerAutoTeamAssign());
    Unused50();
    SetMemory(0x5c31cc, temp);
}

void MakeCoopTeam()
{
    "export MakeCoopTeam";
    int arr[3];
    int teamCount = GetMemory(0x654D5C), temp, link;

    if (!teamCount && !link)
    {
        arr[0] = 0x417E1068; arr[1] = 0x2414FF00; arr[2] = 0xC304C483;
        int flink = g_makecoopTeam;
        link = GetScrDataField(-flink);
        temp = GetMemory(0x5c31cc);
        SetMemory(0x5c31cc, link);
        Unused50();
        SetMemory(0x5c31cc, temp);
        SetMemory(0x5d53a4, 268640519);

        PlayerAutoTeamAssign();
    }
}

void RemoveCoopTeamMode()
{
    "export RemoveCoopTeamMode";
    int arr[6], link, temp;

    if (!link && GetMemory(0x654d5c) == 1)
    {
        arr[0] = 0x4DB8BE56; arr[1] = 0x20680065; arr[2] = 0x6A00418F; arr[3] = 0x54FF5600; arr[4] = 0xC4830824; arr[5] = 0x90C35E0C;
        int flink = g_removeCoopTeam;
        link = GetScrDataField(-flink);
        temp = GetMemory(0x5c31cc);
        SetMemory(0x5c31cc, link);
        Unused50();
        SetMemory(0x5c31cc, temp);
    }
}

void NOXLibraryEntryPointFunction()
{
    g_makecoopTeam = MakeCoopTeam;
    g_importPlayerTeamAssign = ImportPlayerAutoTeamAssign;
    g_removeCoopTeam = RemoveCoopTeamMode;
}