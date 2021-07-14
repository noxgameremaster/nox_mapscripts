
int g_registWeaponCPropertyExecScript;
int g_importWeaponCPropertyExecScript;
int g_weaponCPropertyFxOpcode1;
int g_weaponCPropertyTableExecScript;
	
	
int GetMemory(int a){}
void SetMemory(int a, int b){}
int SToInt(string a){}
int ToInt(float a){}
int UnitToPtr(int a){}
int GetScrDataField(int functionName){}
void FixCallOpcode(int a, int b){}
void OpcodeCopiesAdvance(int a, int n, int c, int d){}


int ImportWeaponCPropertyExecScript()
{
    int arr[13], link;

    if (!link)
    {
        arr[0] = 0x24448D50; arr[1] = 0xEC83520C; arr[2] = 0x85108B0C; arr[3] = 0x501C74D2; arr[4] = 0x90828B; arr[5] = 0x44890000;
        arr[6] = 0x89580424; arr[7] = 0x8B082454; arr[8] = 0x44890440; arr[9] = 0xE5E80424; arr[10] = 0x83FFDB62; arr[11] = 0x585A0CC4;
        arr[12] = 0x000090C3;
        link = GetScrDataField(-g_importWeaponCPropertyExecScript);
        FixCallOpcode(link + 0x26, 0x507310);
    }
    return link;
}

int WeaponCPropertyFxOpcode1()
{
    int code[21], call1 = 0x4e0702, call2 = 0x4e0722, call3 = 0x4e0731, nop;
    int link;

    if (!link)
    {
        link = GetScrDataField(-g_weaponCPropertyFxOpcode1);
        OpcodeCopiesAdvance(link, link + (21 * 4), 0x4e06f0, 0x4e073c);

        SetMemory(link + 46, 0x83);
        SetMemory(link + 61, 97);
    }
    return link;
}

string WeaponCPropertyTableExecScriptPropertyName()
{
	StopScript(0);
}

int WeaponCPropertyTableExecScriptPropertyId()
{
	return 0x61;
}

float WeaponCPropertyTableExecScriptPropertyPlusDamage()
{
	return 4.0;
}

int WeaponCPropertyTableExecScript()
{
    int dat[36], link;

    if (!link)
    {
		if (SToInt(WeaponCPropertyTableExecScriptPropertyName()) == 0)
			dat[0] = 0x5bac5c;	//Fire4
		else
			dat[0] = GetMemory(0x97bb40 + (SToInt(WeaponCPropertyTableExecScriptPropertyName()) * 4));
        dat[1] = WeaponCPropertyTableExecScriptPropertyId(); dat[6] = 0xb40000; dat[5] = 0x4b0; dat[7] = 0x1b2ff2;
        dat[10] = ImportWeaponCPropertyExecScript();
        dat[13] = WeaponCPropertyFxOpcode1();
        dat[14] = ToInt(WeaponCPropertyTableExecScriptPropertyPlusDamage());
		dat[9] = 1;
        link = GetScrDataField(-g_weaponCPropertyTableExecScript);
    }
    return link;
}

//무기를 휘두를 시 스크립트가 실행됩니다
//wUnit: 무기입니다
//slot: 마법속성 슬릇번호 입니다 (맨 위부터 0, 1, 2, 3)
//execFunction: 실행할 함수입니다
void RegistWeaponCPropertyExecScript(int wUnit, int slot, int execFunction)
{
	int u, ulink = GetScrDataField(-g_registWeaponCPropertyExecScript) + 12;
    int ptr = UnitToPtr(wUnit);

    if (ptr)
    {
        SetMemory(ptr + 0x90, execFunction);
        SetMemory(GetMemory(ptr + 0x2b4) + (slot * 4), WeaponCPropertyTableExecScript());
		for (u = 0 ; u < 32 ; SetMemory(ulink, u + 1))
			SetMemory(ptr + 0x230 + (u*4), 0x200);
    }
}

void NOXLibraryEntryPointFunction()
{
	"export ImportWeaponCPropertyExecScript";
	"export WeaponCPropertyFxOpcode1";
	"export WeaponCPropertyTableExecScriptPropertyName";
	"export WeaponCPropertyTableExecScriptPropertyId";
	"export WeaponCPropertyTableExecScriptPropertyPlusDamage";
	"export needinit WeaponCPropertyTableExecScript";
	"export RegistWeaponCPropertyExecScript";
	
	g_importWeaponCPropertyExecScript = ImportWeaponCPropertyExecScript;
	g_weaponCPropertyFxOpcode1 = WeaponCPropertyFxOpcode1;
	g_weaponCPropertyTableExecScript = WeaponCPropertyTableExecScript;
	g_registWeaponCPropertyExecScript = RegistWeaponCPropertyExecScript;
}

