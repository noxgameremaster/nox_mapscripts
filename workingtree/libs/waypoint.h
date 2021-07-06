

int g_waypointTable;

int ToInt(float x) { StopScript(x); }
float ToFloat(int x) { StopScript(x); }

void SetMemory(int a, int v){}
int GetMemory(int a){}

int MapWaypointTable(int idx)
{
    "export MapWaypointTable";
    int table[600];

    return table[idx - 1];
}

float LocationX(int location)
{
    StopScript(GetMemory(MapWaypointTable(location) + 8));
    "export LocationX";
}

float LocationY(int location)
{
    StopScript(GetMemory(MapWaypointTable(location) + 12));
    "export LocationY";
}

void TeleportLocation(int location, float xProfile, float yProfile)
{
    int wTable = MapWaypointTable(location);

    SetMemory(wTable + 8, ToInt(xProfile));
    SetMemory(wTable + 12, ToInt(yProfile));
    return;
    "export TeleportLocation";
}

void TeleportLocationVector(int location, float xVect, float yVect)
{
    int wTable = MapWaypointTable(location);

    SetMemory(wTable + 8, ToInt(ToFloat(GetMemory(wTable + 8)) + xVect));
    SetMemory(wTable + 12, ToInt(ToFloat(GetMemory(wTable + 12)) + yVect));
    return;
    "export TeleportLocationVector";
}

void MapWaypointFill(int wAddr, int tPtr)
{
    "export MapWaypointFill";
    
    int num;

    if (wAddr)
    {
        num = GetMemory(wAddr);
        SetMemory(tPtr + (num * 4), wAddr);
        MapWaypointFill(GetMemory(wAddr + 484), tPtr);
    }
}

void MapWaypointInit()
{
    "export needinit MapWaypointInit";

    int funcIndex = g_waypointTable;
    int link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-funcIndex) + 0x1c));

    MapWaypointFill(GetMemory(0x83c7fc), link);
}

void NOXLibraryEntryPointFunction()
{
    g_waypointTable = MapWaypointTable;
}