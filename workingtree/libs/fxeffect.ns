
int g_importGreenLightning;
int g_importMoveOrb;
int g_importPlaysound;
int g_importGreenExplo;

//dependency
void SetMemory(int a, int v){}
int GetMemory(int a){}
int ToInt(float x) { }
float ToFloat(int x){}
string ToStr(int x) {}
int GetScrDataField(int x){}
int UnitToPtr(int unit) {}
//

int ImportPlaySoundAround()
{
	int arr[17], link;
    int funcIndex = g_importPlaysound;

	if (!link)
	{
		arr[0] = 0x50196068; arr[1] = 0x72506800; arr[2] = 0x50560050; arr[3] = 0x082454FF;
		arr[4] = 0x54FFF08B; arr[5] = 0x006A0824; arr[6] = 0x5650006A; arr[7] = 0x1C2454FF;
		arr[8] = 0x5810C483; arr[9] = 0x08C4835E; arr[10] = 0x909090C3;
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-funcIndex) + 0x1c));
	}
	return link;
}

void PlaySoundAround(int sUnit, int sNumber)
{
	int unitPtr = UnitToPtr(sUnit), temp = GetMemory(0x5c325c);

	if (unitPtr)
	{
		SetMemory(0x5c325c, ImportPlaySoundAround());
		Unused74(unitPtr, sNumber);
		SetMemory(0x5c325c, temp);
	}
}

int ImportGreenExplosionFunc()
{
    int arr[17], link;
    int funcIndex = g_importGreenExplo;

    if (!link)
    {
        arr[0] = 0x32006856; arr[1] = 0x50680052; arr[2] = 0x68005072; arr[3] = 0x00403560; arr[4] = 0x54FF086A;
		arr[5] = 0xC4830424; arr[6] = 0xFFF08B04; arr[7] = 0x89042454; arr[8] = 0x2454FF06; arr[9] = 0x04468904;
		arr[10] = 0x0000C868; arr[11] = 0x54FF5600; arr[12] = 0xC4831024; arr[13] = 0x425D6814; arr[14] = 0xFF560040;
		arr[15] = 0x83042454; arr[16] = 0xC35E08C4; 
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-funcIndex) + 0x1c));
    }
    return link;
}

void GreenExplosion(float x, float y)
{
    int temp = GetMemory(0x5c31f4);

    SetMemory(0x5c31f4, ImportGreenExplosionFunc());
    Unused5a(ToInt(y), ToInt(x));
    SetMemory(0x5c31f4, temp);
}

int ImportOrbMoveFunc()
{
	int arr[6], link;
    int funcIndex = g_importMoveOrb;

	if (!link)
	{
		arr[0] = 0x52353068; arr[1] = 0x72506800; arr[2] = 0x14FF0050; arr[3] = 0x54FF5024; arr[4] = 0xC4830824; arr[5] = 0x9090C30C;
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-funcIndex) + 0x1c));
	}
	return link;
}

void LinearOrbMove(int unit, float x_vect, float y_vect, float speed, int time)
{
	int ptr = UnitToPtr(unit), temp = GetMemory(0x5c336c);

	if (ptr)
	{
		SetMemory(ptr + 0x50, ToInt(x_vect));
		SetMemory(ptr + 0x54, ToInt(y_vect));
		SetMemory(ptr + 0x70, ToInt(speed));
		SetMemory(0x5c336c, ImportOrbMoveFunc());
		Unknownb8(ptr);
		SetMemory(0x5c336c, temp);
		DeleteObjectTimer(unit, time);
	}
}

int ImportGreenLightningFunc()
{
    int arr[21], link;

    if (!link)
    {
		arr[0] = 0x5D685657; arr[1] = 0x68004042; arr[2] = 0x00523790; arr[3] = 0x50725068; arr[4] = 0x35606800;
		arr[5] = 0x106A0040; arr[6] = 0x042454FF; arr[7] = 0x8B08C483; arr[8] = 0x2414FFF0; arr[9] = 0x14FFF88B;
		arr[10] = 0x0C468924; arr[11] = 0x892414FF; arr[12] = 0x14FF0846; arr[13] = 0x04468924; arr[14] = 0x892414FF;
		arr[15] = 0xFF565706; arr[16] = 0x830C2454; arr[17] = 0xFF5608C4; arr[18] = 0x830C2454; arr[19] = 0x5F5E10C4; arr[20] = 0x909090C3; 
		int flink = g_importGreenLightning;
        link = GetScrDataField(-flink);
    }
    return link;
}

void GreenLightningFx(int x1, int y1, int x2, int y2, int time)
{
    int temp = GetMemory(0x5c321c);

    SetMemory(0x5c321c, ImportGreenLightningFunc());
    Effect(ToStr(x1), ToFloat(y1), ToFloat(x2), ToFloat(y2), ToFloat(time));
    SetMemory(0x5c321c, temp);
}

void NOXLibraryEntryPointFunction()
{
    "export ImportOrbMoveFunc";
    "export LinearOrbMove";
    "export ImportPlaySoundAround";
    "export PlaySoundAround";
    "export ImportGreenExplosionFunc";
    "export GreenExplosion";
	"export ImportGreenLightningFunc";
	"export GreenLightningFx";

    g_importMoveOrb = ImportOrbMoveFunc;
    g_importPlaysound = ImportPlaySoundAround;
    g_importGreenExplo = ImportGreenExplosionFunc;
	g_importGreenLightning = ImportGreenLightningFunc;
}
