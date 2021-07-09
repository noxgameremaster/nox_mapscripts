
int g_importPlayerLookFunc;

int GetMemory(int a){}
void SetMemory(int a, int b){}

/*
*유닛이 관찰 가능한 상태인지를 확인합니다
*이미 관찰중인 경우 0 이 반환됩니다
*/
int CheckWatchFocus(int observer)
{
    int ptr = UnitToPtr(observer), buff;

    if (ptr)
    {
        buff = GetMemory(ptr + 0x2ec);
        if (buff)
        {
            buff = GetMemory(buff + 0x114);
            if (buff)
                return GetMemory(buff + 0xe60) ^ 0x12;
        }
    }
    return 0;
}

int ImportPlayerLookFunc()
{
    int arr[17], link;

    if (!arr[0])
    {
        arr[0] = 0x50725068; arr[1] = 0x1B606800; arr[2] = 0x80680051; arr[3] = 0xFF004DDE; arr[4] = 0x50082454; arr[5] = 0x082454FF; arr[6] = 0x8B04C483; arr[7] = 0x74FF85F8;
        arr[8] = 0x2454FF19; arr[9] = 0x54FF5008; arr[10] = 0xC4830824; arr[11] = 0x74C08504; arr[12] = 0xFF505709; arr[13] = 0x83082454; arr[14] = 0xC48308C4;
        arr[15] = 0x31FF310C; arr[16] = 0x0000C3C0;
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-(g_importPlayerLookFunc)) + 0x1c));
    }
    return link;
}

/*
*observer 유닛이 target 유닛을 관찰합니다
*/
void PlayerLook(int observer, int target)
{
    SetMemory(0x5c31f4, ImportPlayerLookFunc());
    Unused5a(observer, target);
    SetMemory(0x5c31f4, 0x513f30);
}

void NOXLibraryEntryPointFunction()
{
	"export CheckWatchFocus";
	"export needinit ImportPlayerLookFunc";
	"export PlayerLook";
	
	g_importPlayerLookFunc = ImportPlayerLookFunc;
}


