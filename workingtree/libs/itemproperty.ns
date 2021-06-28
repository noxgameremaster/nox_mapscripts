
int g_importAllowDrop;

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

int WeaponPower(int num)
{
    "export WeaponPower";
    int addr[6];

    if (!addr[0])
    {
        addr[0] = 0x5a00a4; addr[1] = 0x5BA714; addr[2] = 0x5BA72C; addr[3] = 0x5BA744; addr[4] = 0x5BA75C; addr[5] = 0x5BA774;
        return 0;
    }
    return GetMemory(addr[num]);
}

int ArmorQuality(int num)
{
    "export ArmorQuality";
    int addr[6];

    if (!addr[0])
    {
        addr[0] = 0x5a00a4; addr[1] = 0x5BA7A4; addr[2] = 0x5BA7BC; addr[3] = 0x5BA7D4; addr[4] = 0x5BA7EC; addr[5] = 0x5BA804;
        return 0;
    }
    return GetMemory(addr[num]);
}

int MaterialList(int num)
{
    "export MaterialList";
    int addr[6];

    if (!addr[0])
    {
        //Lv.3 ~ 7, null
        addr[0] = 0x5a00a4; addr[1] = 0x5ba834; addr[2] = 0x5ba84c; addr[3] = 0x5ba864; addr[4] = 0x5ba87c; addr[5] = 0x5ba894;
        return 0;
    }
    return GetMemory(addr[num]);
}

int WeaponEffect(int num)
{
    "export WeaponEffect";
    int addr[37];

    if (!addr[0])
    {
        addr[0] = 0x5a00a4;
        addr[1] = 0x5BA1BC; addr[2] = 0x5BA1D4; addr[3] = 0x5BA1EC; addr[4] = 0x5BA204; addr[5] = 0x5BA21C; addr[6] = 0x5BA234; addr[7] = 0x5BA24C; addr[8] = 0x5BA264;
        addr[9] = 0x5BA27C; addr[10] = 0x5BA294; addr[11] = 0x5BA2AC; addr[12] = 0x5BA2C4; addr[13] = 0x5BA2DC; addr[14] = 0x5BA2F4; addr[15] = 0x5BA30C; addr[16] = 0x5BA324;
        addr[17] = 0x5BA33C; addr[18] = 0x5BA354; addr[19] = 0x5BA36C; addr[20] = 0x5BA384; addr[21] = 0x5BA39C; addr[22] = 0x5BA3B4; addr[23] = 0x5BA3CC; addr[24] = 0x5BA3E4;
        addr[25] = 0x5BA3FC; addr[26] = 0x5BA414; addr[27] = 0x5BA42C; addr[28] = 0x5BA444;
        addr[29] = 0x5BA63C; addr[30] = 0x5BA654; addr[31] = 0x5BA66C; addr[32] = 0x5BA684;
        addr[33] = 0x5BA69C; addr[34] = 0x5BA6B4; addr[35] = 0x5BA6CC; addr[36] = 0x5BA6E4;
        return 0;
    }
    return GetMemory(addr[num]);
}

int ArmorEffect(int num)
{
    "export ArmorEffect";
    int addr[21];

    if (!addr[0])
    {
        addr[0] = 0x5a00a4;
        addr[1] = 0x5BA45C; addr[2] = 0x5BA474; addr[3] = 0x5BA48C; addr[4] = 0x5BA4A4; addr[5] = 0x5BA4BC; addr[6] = 0x5BA4D4; addr[7] = 0x5BA4EC; addr[8] = 0x5BA504;
        addr[9] = 0x5BA51C; addr[10] = 0x5BA534; addr[11] = 0x5BA54C; addr[12] = 0x5BA564; addr[13] = 0x5BA57C; addr[14] = 0x5BA594; addr[15] = 0x5BA5AC; addr[16] = 0x5BA5C4;
        addr[17] = 0x5BA5DC; addr[18] = 0x5BA5F4; addr[19] = 0x5BA60C; addr[20] = 0x5BA624;
        return 0;
    }
    return GetMemory(addr[num]);
}

void InitInvPropertiesSet()
{
    "export needinit InitInvPropertiesSet";

    WeaponEffect(0);
    ArmorEffect(0);
    WeaponPower(0);
    ArmorQuality(0);
    MaterialList(0);
}

void SetWeaponProperties(int weapon, int power, int mt_lv, int wfx1, int wfx2)
{
    "export SetWeaponProperties";
    int k, ptr = UnitToPtr(weapon);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2b4), WeaponPower(power));
        SetMemory(GetMemory(ptr + 0x2b4) + 4, MaterialList(mt_lv));
        SetMemory(GetMemory(ptr + 0x2b4) + 8, WeaponEffect(wfx1));
        SetMemory(GetMemory(ptr + 0x2b4) + 12, WeaponEffect(wfx2));
        for (k = 31 ; k >= 0 ; k --)
            SetMemory(ptr + 0x230 + (k * 4), 0x200);
    }
}

void SetArmorProperties(int armor, int qual, int mt_lv, int afx1, int afx2)
{
    "export SetArmorProperties";
    int k, ptr = UnitToPtr(armor);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2b4), ArmorQuality(qual));
        SetMemory(GetMemory(ptr + 0x2b4) + 4, MaterialList(mt_lv));
        SetMemory(GetMemory(ptr + 0x2b4) + 8, ArmorEffect(afx1));
        SetMemory(GetMemory(ptr + 0x2b4) + 12, ArmorEffect(afx2));
        for (k = 31 ; k >= 0 ; k --)
            SetMemory(ptr + 0x230 + (k * 4), 0x200);
    }
}

int ImportAllowAllDrop()
{
    "export ImportAllowAllDrop";
	int arr[19], link;

	if (!link)
	{
		arr[0] = 0x550CEC83; arr[1] = 0x14246C8B; arr[2] = 0x24748B56; arr[3] = 0xECAE391C; arr[4] = 0x74000001; arr[5] = 0xC0315E08; arr[6] = 0x0CC4835D;
		arr[7] = 0x0845F6C3; arr[8] = 0x68207404; arr[9] = 0x0053EBF0; arr[10] = 0x2454FF56; arr[11] = 0x08C48304; arr[12] = 0x0F74C085; arr[13] = 0x53EC8068;
		arr[14] = 0x56016A00; arr[15] = 0x082454FF; arr[16] = 0x680CC483; arr[17] = 0x004ED301; arr[18] = 0x909090C3;
        int flink = g_importAllowDrop;
        link = GetScrDataField(-flink);
	}
	return link;
}

void NOXLibraryEntryPointFunction()
{
    g_importAllowDrop = ImportAllowAllDrop;
}

