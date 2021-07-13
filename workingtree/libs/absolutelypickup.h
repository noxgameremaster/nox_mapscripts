
int g_delayForcePickItemToOwner;
int g_importAbsoluteWPickup;
int g_importAbsoluteWRun;
int g_importEquipWeapon;
int g_importEquipWeaponRun;


int GetMemory(int a){}
void SetMemory(int a, int b){}
int GetScrDataField(int a){}
int UnitToPtr(int a){}
int GetOwner(int a){}	//include unitstruct.h


int ImportAbsoluteWeaponPickup()
{
    int arr[100], link;

    if (!link)
    {
        arr[0] = 0x8B565553; arr[1] = 0x31102474; arr[2] = 0x468A57ED; arr[3] = 0x247C8B08; arr[4] = 0x6804A818; arr[5] = 0x0040A5C0; arr[6] = 0x00080068; arr[7] = 0x2454FF00;
        arr[8] = 0x08C48304; arr[9] = 0xC068C085; arr[10] = 0x680040A5; arr[11] = 0x00000800; arr[12] = 0x042454FF; arr[13] = 0x8508C483; arr[14] = 0x245C8BC0;
        arr[15] = 0x24548B20; arr[16] = 0x31E0681C; arr[17] = 0x5253004F; arr[18] = 0x54FF5657; arr[19] = 0xC4831024; arr[20] = 0x01F88314; arr[21] = 0x0122850F;
        arr[22] = 0x46F60000; arr[23] = 0x840F0408; arr[24] = 0x000000FA; arr[25] = 0x02EC868B; arr[26] = 0xED310000; arr[27] = 0x14244489; arr[28] = 0x8568488B;
        arr[29] = 0x683575C9; arr[30] = 0x00419E60; arr[31] = 0x2454FF56; arr[32] = 0x08C48304; arr[33] = 0x2475C085; arr[34] = 0x41582068; arr[35] = 0x54FF5700;
        arr[36] = 0xC4830424; arr[37] = 0x02F88308; arr[38] = 0x20681274; arr[39] = 0x550053A4; arr[40] = 0xFF565753; arr[41] = 0x83102454; arr[42] = 0xE88B14C4;
        arr[43] = 0x419E6068; arr[44] = 0x54FF5600; arr[45] = 0xC4830424; arr[46] = 0x75C08508; arr[47] = 0x5820683A; arr[48] = 0xFF570041; arr[49] = 0x83042454;
        arr[50] = 0xF88308C4; arr[51] = 0x8B287502; arr[52] = 0x8B142444; arr[53] = 0x00011488; arr[54] = 0x04418B00; arr[55] = 0x17740CA8; arr[56] = 0x137502A8;
        arr[57] = 0x53A42068; arr[58] = 0x53006A00; arr[59] = 0x54FF5657; arr[60] = 0xC4831024; arr[61] = 0x85E88B14; arr[62] = 0x8B6775ED; arr[63] = 0xC4F60847;
        arr[64] = 0xF7217410; arr[65] = 0x00000C47; arr[66] = 0x1874047F; arr[67] = 0x02E0878B; arr[68] = 0xD2310000; arr[69] = 0x508AC931; arr[70] = 0x6C488A6D;
        arr[71] = 0x4D82B068; arr[72] = 0x22EB5200; arr[73] = 0x000000A9; arr[74] = 0xF6377401; arr[75] = 0x74820C47; arr[76] = 0xE0878B31; arr[77] = 0x31000002;
        arr[78] = 0x8AC931D2; arr[79] = 0x01488A10; arr[80] = 0x4D82B068; arr[81] = 0x548B5200; arr[82] = 0x31511C24; arr[83] = 0x828B57C9; arr[84] = 0x00000114;
        arr[85] = 0x0810888A; arr[86] = 0xFF510000; arr[87] = 0x83102454; arr[88] = 0xF06814C4; arr[89] = 0x68005116; arr[90] = 0x0053A6C0; arr[91] = 0x54FF5657;
        arr[92] = 0xFF570824; arr[93] = 0x83102454; arr[94] = 0x01B814C4; arr[95] = 0x5F000000; arr[96] = 0xC35B5D5E; arr[97] = 0x315D5E5F; arr[98] = 0x90C35BC0;
        link = GetScrDataField(-(g_importAbsoluteWPickup));
    }
    return link;
}

int ImportAbsoluteWeaponRun()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x90909068; arr[1] = 0x72506890; arr[2] = 0x016A0050; arr[3] = 0x54FF016A; arr[4] = 0xFF500824;
        arr[5] = 0x500C2454; arr[6] = 0x142454FF; arr[7] = 0xC318C483; arr[8] = 0x90909090;
        link = GetScrDataField(-(g_importAbsoluteWRun));
        SetMemory(link + 1, ImportAbsoluteWeaponPickup());
    }
    return link;
}

//소유자에게 이 무기를 강제 픽업시켜 줍니다
void AbsolutelyWeaponPickup(int sOwner, int sWeapon)
{
    int temp = GetMemory(0x5c31f4);
    int ownerPtr = UnitToPtr(sOwner), wPtr = UnitToPtr(sWeapon);

    if (ownerPtr && wPtr)
    {
        SetMemory(0x5c31f4, ImportAbsoluteWeaponRun());
        Unused5a(ownerPtr, wPtr);
        SetMemory(0x5c31f4, temp);
    }
}

int ImportEquipWeapon()
{
    int arr[170], link;

    if (!link)
    {
        arr[0] = 0x8B565553; arr[1] = 0x57142474; arr[2] = 0x41582068; arr[3] = 0x54FF5600; arr[4] = 0xD88B0424; arr[5] = 0x8308468B; arr[6] = 0x00A908C4;
        arr[7] = 0x0F010010; arr[8] = 0x0000AD84; arr[9] = 0x10468B00; arr[10] = 0x0F01C4F6; arr[11] = 0x0000A185; arr[12] = 0x247C8B00; arr[13] = 0x08478B14;
        arr[14] = 0x137402A8; arr[15] = 0x53A2C068; arr[16] = 0xFF575600; arr[17] = 0x83082454; arr[18] = 0x5E5F0CC4; arr[19] = 0xA8C35B5D; arr[20] = 0x8B7F7404;
        arr[21] = 0x0002ECAF; arr[22] = 0xC3E06800; arr[23] = 0x026A004F; arr[24] = 0x2454FF57; arr[25] = 0x0CC48308; arr[26] = 0x6675C085; arr[27] = 0x4FC3E068;
        arr[28] = 0x57016A00; arr[29] = 0x082454FF; arr[30] = 0x850CC483; arr[31] = 0x8B5375C0; arr[32] = 0x00011485; arr[33] = 0x8AC93100; arr[34] = 0x0008CB88;
        arr[35] = 0xB3D06800; arr[36] = 0x56510057; arr[37] = 0x082454FF; arr[38] = 0x850CC483; arr[39] = 0x683A75C0; arr[40] = 0x004DA2C0; arr[41] = 0xABD86850;
        arr[42] = 0xFF57005C; arr[43] = 0x8B0C2454; arr[44] = 0x83302444; arr[45] = 0xC08510C4; arr[46] = 0x578B1874; arr[47] = 0x19606824; arr[48] = 0x6A520050;
        arr[49] = 0x9D685702; arr[50] = 0xFF000003; arr[51] = 0x83102454; arr[52] = 0x5E5F14C4; arr[53] = 0x5BC0315D; arr[54] = 0x318068C3; arr[55] = 0x5756004F;
        arr[56] = 0x082454FF; arr[57] = 0x850CC483; arr[58] = 0x682175C0; arr[59] = 0x004DA2C0; arr[60] = 0xABF86850; arr[61] = 0xFF57005C; arr[62] = 0x8B0C2454;
        arr[63] = 0x83302444; arr[64] = 0xC08510C4; arr[65] = 0x478BCC74; arr[66] = 0xB7EB5024; arr[67] = 0x01F8878B; arr[68] = 0xC0850000; arr[69] = 0xF039BC74;
        arr[70] = 0x808B0F74; arr[71] = 0x000001F0; arr[72] = 0xF275C085; arr[73] = 0x5B5D5E5F; arr[74] = 0x74C085C3; arr[75] = 0x587D80A5; arr[76] = 0x680F7501;
        arr[77] = 0x004FA020; arr[78] = 0xFF570D6A; arr[79] = 0x83082454; arr[80] = 0xFB830CC4; arr[81] = 0x8B3B7502; arr[82] = 0x0001148D; arr[83] = 0x0441F600;
        arr[84] = 0x681E750C; arr[85] = 0x0053A680; arr[86] = 0x2454FF57; arr[87] = 0x08C48304; arr[88] = 0x0D75C085; arr[89] = 0x4DA2C068; arr[90] = 0x1C685000;
        arr[91] = 0xEB005CAC; arr[92] = 0xA0F06885; arr[93] = 0x016A0053; arr[94] = 0xFF57016A; arr[95] = 0x830C2454; arr[96] = 0x458B10C4; arr[97] = 0x74C08568;
        arr[98] = 0x02FB831F; arr[99] = 0x40681A74; arr[100] = 0x6A0053A1; arr[101] = 0x50016A01; arr[102] = 0x2454FF57; arr[103] = 0x14C48310; arr[104] = 0x840FC085;
        arr[105] = 0xFFFFFF2A; arr[106] = 0x8010568B; arr[107] = 0x568901CE; arr[108] = 0x14858B10; arr[109] = 0x31000001; arr[110] = 0x04488BD2; arr[111] = 0x4889D909;
        arr[112] = 0x148D8B04; arr[113] = 0x8B000001; arr[114] = 0x8A1C2444; arr[115] = 0x00081091; arr[116] = 0x85406800; arr[117] = 0x5650004D; arr[118] = 0x2454FF52;
        arr[119] = 0x10C4830C; arr[120] = 0x7402FB83; arr[121] = 0x68758903; arr[122] = 0xF608468B; arr[123] = 0x397410C4; arr[124] = 0x000C46F7; arr[125] = 0x74047F00;
        arr[126] = 0xE0868B30; arr[127] = 0x31000002; arr[128] = 0x8AD231C9; arr[129] = 0x508A6D48; arr[130] = 0x14858B6C; arr[131] = 0x68000001; arr[132] = 0x004D82B0;
        arr[133] = 0x52C93151; arr[134] = 0x0810888A; arr[135] = 0x51560000; arr[136] = 0x102454FF; arr[137] = 0xEB14C483; arr[138] = 0x0000A949; arr[139] = 0x42740100;
        arr[140] = 0x7482C3F6; arr[141] = 0xE0868B2F; arr[142] = 0x31000002; arr[143] = 0x8AC931D2; arr[144] = 0x01488A10; arr[145] = 0x4D82B068; arr[146] = 0x958B5200;
        arr[147] = 0x00000114; arr[148] = 0x8A51C031; arr[149] = 0x00081082; arr[150] = 0xFF505600; arr[151] = 0x83102454; arr[152] = 0x0EEB14C4; arr[153] = 0x740CC3F6;
        arr[154] = 0xE08E8B09; arr[155] = 0xC6000002; arr[156] = 0xF0680001; arr[157] = 0x57004F2F; arr[158] = 0x2454FF56; arr[159] = 0x0CC48308; arr[160] = 0xE40CC3F7;
        arr[161] = 0x0D7407FF; arr[162] = 0x53A3D068; arr[163] = 0x54FF5700; arr[164] = 0xC4830424; arr[165] = 0x5D5E5F08; arr[166] = 0x000001B8; arr[167] = 0x90C35B00;
        
        arr[39] = 0x683aebc0;
        link = GetScrDataField(-g_importEquipWeapon);
    }
    return link;
}

int ImportEquipWeaponRun()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x90909068; arr[1] = 0x72506890; arr[2] = 0x016A0050; arr[3] = 0x54FF016A; arr[4] = 0xFF500824;
        arr[5] = 0x500C2454; arr[6] = 0x142454FF; arr[7] = 0xC318C483; arr[8] = 0x90909090;
        link = GetScrDataField(-g_importEquipWeaponRun);
        SetMemory(link + 1, ImportEquipWeapon());
    }
    return link;
}

//소유자에게 무기를 강제로 장착시켜 줍니다
void AbsolutelyPlayerEquipWeapon(int sOwner, int sWeapon)
{
    int temp = GetMemory(0x5c31f4);
    int ownerPtr = UnitToPtr(sOwner), wPtr = UnitToPtr(sWeapon);

    if (ownerPtr && wPtr)
    {
		if (HasItem(sOwner, sWeapon))
		{
			SetMemory(0x5c31f4, ImportEquipWeaponRun());
			Unused5a(ownerPtr, wPtr);
			SetMemory(0x5c31f4, temp);
		}
    }
}

void DelayForcePickItemToOwner(int sItem)
{
    int owner = GetOwner(sItem);

    if (IsObjectOn(sItem))
    {
        if (CurrentHealth(owner))
        {
            AbsolutelyWeaponPickup(owner, sItem);
            AbsolutelyPlayerEquipWeapon(owner, sItem);
        }
        else
            ClearOwner(sItem);
    }
}

//플레이어 유닛에게 특정 무기를 강제로 픽업 및 장착해 줍니다
void AbsolutelyWeaponPickupAndEquip(int plrUnit, int weapon)
{
	if (CurrentHealth(plrUnit) && IsObjectOn(weapon))
	{
		SetOwner(plrUnit, weapon);
		FrameTimerWithArg(1, weapon, -g_delayForcePickItemToOwner);
	}
}

void NOXLibraryEntryPointFunction()
{
	"export needinit ImportAbsoluteWeaponPickup";
	"export ImportAbsoluteWeaponRun";
	"export AbsolutelyWeaponPickup";
	"export needinit ImportEquipWeapon";
	"export ImportEquipWeaponRun";
	"export AbsolutelyPlayerEquipWeapon";
	"export DelayForcePickItemToOwner";
	"export AbsolutelyWeaponPickupAndEquip";
	
	g_delayForcePickItemToOwner = DelayForcePickItemToOwner;
	g_importAbsoluteWPickup = ImportAbsoluteWeaponPickup;
	g_importAbsoluteWRun = ImportAbsoluteWeaponRun;
	g_importEquipWeapon = ImportEquipWeapon;
	g_importEquipWeaponRun = ImportEquipWeaponRun;
}
