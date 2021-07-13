
int g_createMagicWall, g_getWallAtPtr, g_importAddBreakableWall, g_importCreateMagicWall, g_importGetWallAtPtr;

//Dependency
int GetMemory(int a){}
void SetMemory(int a, int b){}
int GetScrDataField(int functionName){}
int FloatToInt(float x){}	//typecast
float LocationX(int x){}		//waypoint
float LocationY(int x){}
///

void DestroyWallAtObjectPos(int anyunit)
{
	int xPos, yPos;
	
	if (anyunit >= GetHost())
	{
		xPos = FloatToInt(GetObjectX(anyunit));
		yPos = FloatToInt(GetObjectY(anyunit));
	}
	else
	{
		xPos = FloatToInt(LocationX(anyunit));
		yPos = FloatToInt(LocationY(anyunit));
	}
    if (xPos > yPos) xPos += 23;
    else             yPos += 23;
    int spX = (xPos + yPos - 22) / 46;
    int spY = (xPos - yPos) / 46;
    int tx = spX * 46;
    int ty = spY * 46;
    int rx = (tx + ty) / 2;
    WallBreak(Wall(rx / 23, (rx - ty) / 23));
}

void CloseWallAtObjectPos(int anyunit)
{
	int xPos, yPos;
	
	if (anyunit >= GetHost())
	{
		xPos = FloatToInt(GetObjectX(anyunit));
		yPos = FloatToInt(GetObjectY(anyunit));
	}
	else
	{
		xPos = FloatToInt(LocationX(anyunit));
		yPos = FloatToInt(LocationY(anyunit));
	}
    if (xPos > yPos) xPos += 23;
    else             yPos += 23;
    int spX = (xPos + yPos - 22) / 46;
    int spY = (xPos - yPos) / 46;
    int tx = spX * 46;
    int ty = spY * 46;
    int rx = (tx + ty) / 2;
    WallClose(Wall(rx / 23, (rx - ty) / 23));
}

int GetWallAtObjectPos(int anyunit)
{
    int xPos, yPos;
	
	if (anyunit >= GetHost())
	{
		xPos = FloatToInt(GetObjectX(anyunit));
		yPos = FloatToInt(GetObjectY(anyunit));
	}
	else
	{
		xPos = FloatToInt(LocationX(anyunit));
		yPos = FloatToInt(LocationY(anyunit));
	}
    if (xPos > yPos) xPos += 23;
    else             yPos += 23;
    int spX = (xPos + yPos - 22) / 46;
    int spY = (xPos - yPos) / 46;
    int tx = spX * 46;
    int ty = spY * 46;
    int rx = (tx + ty) / 2;
    return Wall(rx / 23, (rx - ty) / 23);
}

int GetWallCoorAtObjectPos(int anyunit)
{
    int xPos, yPos;
	
	if (anyunit >= GetHost())
	{
		xPos = FloatToInt(GetObjectX(anyunit));
		yPos = FloatToInt(GetObjectY(anyunit));
	}
	else
	{
		xPos = FloatToInt(LocationX(anyunit));
		yPos = FloatToInt(LocationY(anyunit));
	}
    if (xPos > yPos) xPos += 23;
    else             yPos += 23;
    int spX = (xPos + yPos - 22) / 46;
    int spY = (xPos - yPos) / 46;
    int tx = spX * 46;
    int ty = spY * 46;
    int rx = (tx + ty) / 2;
    return (rx / 23) | (((rx - ty) / 23) << 0x10);
}

void RemoveWallAtObjectPos(int anyunit)
{
    int xPos, yPos;
	
	if (anyunit >= GetHost())
	{
		xPos = FloatToInt(GetObjectX(anyunit));
		yPos = FloatToInt(GetObjectY(anyunit));
	}
	else
	{
		xPos = FloatToInt(LocationX(anyunit));
		yPos = FloatToInt(LocationY(anyunit));
	}
    if (xPos > yPos) xPos += 23;
    else             yPos += 23;
    int spX = (xPos + yPos - 22) / 46;
    int spY = (xPos - yPos) / 46;
    int tx = spX * 46;
    int ty = spY * 46;
    int rx = (tx + ty) / 2;
    WallOpen(Wall(rx / 23, (rx - ty) / 23));
}

int ImportCreateMagicWall()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x72506856; arr[1] = 0x14FF0050; arr[2] = 0x68F08B24; arr[3] = 0x004FFD00; arr[4] = 0xFF1076FF; arr[5] = 0x76FF0C76;
        arr[6] = 0x0476FF08; arr[7] = 0x54FF36FF; arr[8] = 0xC4831424; arr[9] = 0x90C35E1C;
		link = GetScrDataField(-(g_importCreateMagicWall));
    }
    return link;
}

void CreateMagicWall(int sPtr, int sWallX, int sWallY, int sWallDir, int sUnk2)
{
    int link, temp = GetMemory(0x5c3108);

    if (!link)
		link = GetScrDataField(-(g_createMagicWall));
    SetMemory(0x5c3108, ImportCreateMagicWall());
    Unused1f(link);
    SetMemory(0x5c3108, temp);
}

int ImportAddBreakableWall()
{
    int arr[6], link;

    if (!link)
    {
        arr[0] = 0x08406850; arr[1] = 0x50680041; arr[2] = 0xFF005072; arr[3] = 0xFF502414; arr[4] = 0x83082454; arr[5] = 0xC3580CC4;
		link = GetScrDataField(-(g_importAddBreakableWall));
    }
    return link;
}

void AddBreakableWall(int sWallPtr)
{
    int temp = GetMemory(0x5c3108);

    SetMemory(0x5c3108, ImportAddBreakableWall());
    Unused1f(sWallPtr);
    SetMemory(0x5c3108, temp);
}

int ImportGetWallAtPtr()
{
    int arr[12], link;

    if (!link)
    {
        arr[0] = 0x50685056; arr[1] = 0xFF005072; arr[2] = 0xF08B2414; arr[3] = 0x832414FF; arr[4] = 0x506804C4; arr[5] = 0x56004102;
        arr[6] = 0x2454FF50; arr[7] = 0x0CC48308; arr[8] = 0x50723068; arr[9] = 0x54FF5000; arr[10] = 0xC4830424; arr[11] = 0xC35E5808;
		link = GetScrDataField(-(g_importGetWallAtPtr));
    }
    return link;
}

int GetWallAtPtr(int sX, int sY)
{
    int temp = GetMemory(0x5c325c);

    SetMemory(0x5c325c, ImportGetWallAtPtr());
    CancelTimer(FrameTimerWithArg(10, Unused74(sX, sY), -(g_getWallAtPtr)));
    SetMemory(0x5c325c, temp);
    return GetMemory(GetMemory(0x83395c) + 8);
}

void NOXLibraryEntryPointFunction()
{
	"export DestroyWallAtObjectPos";
	"export GetWallCoorAtObjectPos";
	"export CloseWallAtObjectPos";
	"export GetWallAtObjectPos";
	"export RemoveWallAtObjectPos";
	"export needinit ImportCreateMagicWall";
	"export CreateMagicWall";
	"export ImportAddBreakableWall";
	"export AddBreakableWall";
	"export needinit ImportGetWallAtPtr";
	"export GetWallAtPtr";
	
	g_createMagicWall = CreateMagicWall;
	g_getWallAtPtr = GetWallAtPtr;
	g_importAddBreakableWall = ImportAddBreakableWall;
	g_importCreateMagicWall = ImportCreateMagicWall;
	g_importGetWallAtPtr = ImportGetWallAtPtr;
}