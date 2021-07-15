
#include "libs\waypoint.h"
#include "noxscript\builtins.h"
#include "libs\typecast.h"
#include "libs\unitutil.h"
#include "libs\unitstruct.h"
#include "libs\printutil.h"
#include "libs\callmethod.h"
#include "libs\buff.h"

#include "libs\mathlab.h"


int LastUnitID = 920;
int player[30], Floor_lv, Mob_count, Set_fptr, Next_fptr, Exit_wp, Start_wp, MEMSET_FN_PTR;
int Jandors[10], DescFunc, Plr_flag[10];
int Weapon_fire4 = 0x5BA264, Weapon_fire3 = 0x5BA24C, Weapon_vam4 = 0x5BA3E4, Weapon_vam3 = 0x5BA3CC;
int Weapon_blue4 = 0x5BA384, Weapon_blue3 = 0x5BA36C;
int Armor_fire4 = 0x5BA4A4, Armor_blue4 = 0x5BA504, Armor_cure3 = 0x5BA5AC, Armor_wind3 = 0x5BA60C;
int P_inven[1000], PlrSkill[10];

#include "libs\playerinfo.h"

#define TRUE    1
#define FALSE   0

#include "libs\reaction.h"
#include "libs\spellutil.h"

#include "libs\cmdline.h"

#define OTHER   -1
#define SELF -2

#define NULLPTR 0
#include "libs\itemproperty.h"
#define ENCHANT_INVISIBLE   0
#define ENCHANT_MOONGLOW    1
#define ENCHANT_BLINDED     2
#define ENCHANT_CONFUSED        3
#define ENCHANT_SLOWED          4
#define ENCHANT_HELD            5
#define ENCHANT_DETECTING       6
#define ENCHANT_ETHEREAL        7
#define ENCHANT_RUN             8
#define ENCHANT_HASTED          9
#define ENCHANT_VILLAIN         10
#define ENCHANT_AFRAID          11
#define ENCHANT_BURNING         12
#define ENCHANT_VAMPIRISM       13
#define ENCHANT_ANCHORED        14
#define ENCHANT_LIGHT           15
#define ENCHANT_DEATH           16
#define ENCHANT_PROTECT_FROM_FIRE       17
#define ENCHANT_PROTECT_FROM_POISON     18
#define ENCHANT_PROTECT_FROM_MAGIC      19  
#define ENCHANT_PROTECT_FROM_ELECTRICITY            20
#define ENCHANT_INFRAVISION         21
#define ENCHANT_SHOCK               22
#define  ENCHANT_INVULNERABLE        23
#define  ENCHANT_TELEKINESIS         24
#define ENCHANT_FREEZE              25
#define ENCHANT_SHIELD              26
#define ENCHANT_REFLECTIVE_SHIELD   27
#define ENCHANT_CHARMING            28
#define ENCHANT_ANTI_MAGIC          29
#define ENCHANT_CROWN               30
#define ENCHANT_SNEAK               31

int CreateYellowPotion(int restoreAmount, float xProfile, float yProfile)
{
    int unit = CreateObjectAt("RedPotion", xProfile, yProfile);
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 4, 639); //YellowPotion
    SetMemory(GetMemory(ptr + 0x2e0), restoreAmount);

    return unit;
}

int CreateBlackPotion(int restoreAmount, float xProfile, float yProfile)
{
    int unit = CreateObjectAt("RedPotion", xProfile, yProfile);
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 4, 641); //BlackPotion
    SetMemory(GetMemory(ptr + 0x2e0), restoreAmount);

    return unit;
}

int CreateWhitePotion(int restoreAmount, float xProfile, float yProfile)
{
    int unit = CreateObjectAt("RedPotion", xProfile, yProfile);
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 4, 640); //WhitePotion
    SetMemory(ptr + 12, GetMemory(ptr + 12) ^ 0x20);
    SetMemory(GetMemory(ptr + 0x2e0), restoreAmount);

    return unit;
}

int CheckPotionThingID(int unit)
{
    int thingID = GetUnitThingID(unit), x = unit;

    if (thingID == 639)
        x = CreateYellowPotion(125, GetObjectX(unit), GetObjectY(unit));
    else if (thingID == 640)
        x = CreateWhitePotion(100, GetObjectX(unit), GetObjectY(unit));
    else if (thingID == 641)
        x = CreateBlackPotion(85, GetObjectX(unit), GetObjectY(unit));
    if (x ^ unit) Delete(unit);

    return x;
}

int CreateMoverFix(int targetUnit, int destLocation, float speed)
{
    int unitMover = CreateMover(targetUnit, destLocation, speed), unitPtr = UnitToPtr(targetUnit);
    int movPtr;

    if (unitMover)
    {
        movPtr = UnitToPtr(unitMover);
        if (movPtr && unitPtr)
        {
            SetMemory(unitPtr + 0x28, LastUnitID);
            movPtr = GetMemory(movPtr + 0x2ec);
            SetMemory(movPtr + 0x20, LastUnitID);
        }
    }
    LastUnitID ++;

    return unitMover;
}

void ChangeColorMaiden(int red, int grn, int blue, int unit)
{
    int ptr1 = UnitToPtr(unit), k, num;

    if (!ptr1) return;
    SetMemory(ptr1 + 4, 1385);  //TODO: Maiden Unit Thing ID
    for (k = 0 ; k < 32 ; k ++)
        SetMemory(ptr1 + 0x230 + (k * 4), 0x400);
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x178, 0xa0);
    // R  G  B  R    G  B  R  G    B  R  G  B    R  G  B  R    G  B 
    // 00 ff 00 00 / ff 00 00 ff / 00 00 ff 00 / 00 ff 00 00 / ff 00 00
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c, red | (grn << 8) | (blue << 16) | (red << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 4, grn | (blue << 8) | (red << 16) | (grn << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 8, blue | (red << 8) | (grn << 16) | (blue << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 12, red | (grn << 8) | (blue << 16) | (red << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 16, grn | (blue << 8));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x1e8, VoiceList(7));
}

int WizardRedBinTable()
{
	int link, arr[62];

	if (!link)
	{
		arr[0] = 1635412311; arr[1] = 1699898482; arr[2] = 100; arr[16] = 80000; arr[17] = 300; 
		arr[18] = 100; arr[19] = 50; arr[21] = 1065353216; arr[23] = 32800; arr[24] = 1067869798; 
		arr[26] = 4; arr[27] = 4; arr[37] = 1701996870; arr[38] = 1819042146; arr[53] = 1128792064; 
		arr[54] = 4; arr[55] = 14; arr[56] = 24; 
		link = &arr;
	}
	return link;
}

int MaidenBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1684627789; arr[1] = 28261; arr[17] = 30; arr[18] = 92; arr[19] = 60; 
		arr[21] = 1065353216; arr[22] = 0; arr[23] = 32776; arr[24] = 1065688760; 
		arr[25] = 0; arr[26] = 0; arr[27] = 1; arr[28] = 1106247680; arr[29] = 22; 
		arr[30] = 1101004800; arr[31] = 2; arr[32] = 22; arr[33] = 30; arr[58] = 5546320; arr[59] = 5542784; 
        link = &arr;
	}
	return link;
}

int GoonBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1852796743; arr[17] = 85; arr[19] = 80; 
		arr[21] = 1065353216; arr[22] = 0; arr[23] = 32776; arr[24] = 1066192077; 
		arr[26] = 4; arr[27] = 0; arr[28] = 1106247680; arr[29] = 25; 
		arr[30] = 1092616192; arr[31] = 4; arr[32] = 20; arr[33] = 28; arr[34] = 2; 
		arr[35] = 3; arr[36] = 20; arr[57] = 5548176; arr[58] = 5546608; arr[59] = 5543680;
        link = &arr;
	}
	return link;
}

int StrongWizardWhiteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1869771859; arr[1] = 1767335790; arr[2] = 1685217658; arr[3] = 1953065047; arr[4] = 101; 
		arr[19] = 1; arr[24] = 1065772646; arr[37] = 1701996870; arr[38] = 1819042146; arr[53] = 1128792064; 
		arr[55] = 20; arr[56] = 30; arr[57] = 5547984;
        link = &arr;
	}
	return link;
}

int WeirdlingBeastBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811; arr[17] = 470; 
		arr[19] = 55; arr[21] = 1065353216; arr[24] = 1071225242; arr[26] = 4; arr[28] = 1106247680; 
		arr[29] = 44; arr[31] = 3; arr[32] = 4; arr[33] = 5; arr[59] = 5542784; 
		arr[60] = 1388; arr[61] = 46915072; 
		link = &arr;
	}
	return link;
}

int BlackWidowBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1667329090; arr[1] = 1684625259; arr[2] = 30575; arr[17] = 180; arr[19] = 85; 
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1065353216; arr[26] = 4; arr[27] = 3; 
		arr[28] = 1097859072; arr[29] = 25; arr[31] = 8; arr[32] = 13; arr[33] = 21; 
		arr[34] = 4; arr[35] = 2; arr[36] = 9; arr[37] = 1684631635; arr[38] = 1884516965; 
		arr[39] = 29801; arr[53] = 1128792064; arr[55] = 20; arr[56] = 28; arr[59] = 5544896; 
		arr[61] = 45071360; 
		link = &arr;
	}
	return link;
}

int FireSpriteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972; arr[17] = 168; arr[19] = 71; 
		arr[21] = 1065353216; arr[23] = 65536; arr[24] = 1065353216; arr[27] = 3; arr[28] = 1106247680; 
		arr[29] = 10; arr[31] = 10; arr[32] = 13; arr[33] = 21; arr[37] = 1769236816; 
		arr[38] = 1181513062; arr[39] = 1650815593; arr[40] = 7105633; arr[53] = 1133903872; arr[55] = 18; 
		arr[56] = 28; arr[58] = 5545472; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int AirshipCaptainBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1936877889; arr[1] = 1131440488; arr[2] = 1635020897; arr[3] = 28265; arr[17] = 325; 
		arr[19] = 90; arr[21] = 1065353216; arr[23] = 34817; arr[24] = 1065353216; arr[26] = 4; 
		arr[37] = 1919248451; arr[38] = 1916887669; arr[39] = 7827314; arr[53] = 1133903872; arr[55] = 3; 
		arr[56] = 8; arr[58] = 5546320; arr[60] = 1387; arr[61] = 46915328; 
		link = &arr;
	}
	return link;
}

int WoundedApprenticeBinTable()
{
	int arr[62], link, unit;
	if (!link)
	{
		arr[0] = 1853189975; arr[1] = 1097098596; arr[2] = 1701998704; arr[3] = 1667855470; arr[4] = 101; 
		arr[17] = 50; arr[18] = 10; arr[19] = 40; 
		arr[20] = 0; arr[21] = 1065353216; arr[22] = 0; arr[23] = 0; arr[24] = 1067450368; 
		arr[25] = 0; arr[26] = 4; arr[27] = 0; arr[28] = 1112014848; arr[29] = 40; 
		arr[30] = 1106247680; arr[31] = 2; arr[32] = 12; arr[33] = 20;
		arr[57] = 5548112; arr[58] = 0; arr[59] = 5542784; 
        link = &arr;
	}
	return link;
}

int CheckMonsterThing(int unit)
{
    int arr[97], init;
    int thingID = GetUnitThingID(unit);
    int key = thingID % 97;

    if (!init)
    {
        init = 1;
        arr[5] = MonsterGoonProcess; arr[72] = MonsterStrongWhiteWizProcess; arr[30] = MonsterWeirdlingBeastProcess; arr[34] = MonsterBlackWidowProcess; arr[6] = MonsterBear2Process;
        arr[12] = MonsterFireSpriteProcess; arr[73] = MonsterWizardRedProcess; arr[29] = MonsterAirshipCaptainProcess;
    }
    if (thingID && arr[key] != 0)
    {
        CallFunctionWithArg(arr[key], unit);
        return TRUE;
    }
    return FALSE;
}

void MonsterGoonProcess(int unit)
{
    //TODO: Index. 1, ThingName= Goon
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, GoonBinTable());
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(63));       //MimicVoice
        SetUnitMaxHealth(unit, 130);
    }
}

void MonsterStrongWhiteWizProcess(int unit)
{
    //TODO: Index. 2, ThingName= StrongWizardWhite
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, StrongWizardWhiteBinTable());
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(34));       //HorvathVoice
        SetUnitMaxHealth(unit, 200);
    }
}

void MonsterWeirdlingBeastProcess(int unit)
{
    //TODO: Index. 3, ThingName= WeirdlingBeast
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WeirdlingBeastBinTable());
        SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0); //Flee Range set to 0
        //SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(34));       //HorvathVoice
        SetUnitMaxHealth(unit, 150);
    }
}

void MonsterBlackWidowProcess(int unit)
{
    //TODO: Index. 4, ThingName= BlackWidow
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BlackWidowBinTable());
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(19));       //HorvathVoice
        SetUnitMaxHealth(unit, 225);
    }
}

void MonsterBear2Process(int unit)
{
    //TODO: Index. 5, ThingName= Bear2 -> Maiden
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        ChangeColorMaiden(Random(0, 255), Random(0, 255), Random(0, 255), unit);
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, MaidenBinTable());
        SetUnitMaxHealth(unit, 325);
    }
}

void MonsterFireSpriteProcess(int unit)
{
    //TODO: Index. 6, ThingName= FireSprite
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FireSpriteBinTable());
        SetUnitMaxHealth(unit, 135);
        SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000);
    }
}

void MonsterWizardRedProcess(int unit)
{
    //TODO: Index. 7, ThingName= WizardRed
    int ptr = UnitToPtr(unit); //, uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WizardRedBinTable());
        SetUnitMaxHealth(unit, 225);
        SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0); //Flee Range set to 0
        /*uec = GetMemory(ptr + 0x2ec);
        if (uec)
        {
            SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x20);
            SetMemory(uec + 0x528, ToInt(1.0));
            SetMemory(uec + 0x520, ToInt(300.0));
            uec += 0x5d0;
            SetMemory(uec + GetSpellNumber("SPELL_MAGIC_MISSILE"), 0x40000000);
            SetMemory(uec + GetSpellNumber("SPELL_SHIELD"), 0x10000000);
            SetMemory(uec + GetSpellNumber("SPELL_STUN"), 0x20000000);
            SetMemory(uec + GetSpellNumber("SPELL_SHOCK"), 0x10000000);
            SetMemory(uec + GetSpellNumber("SPELL_FIREBALL"), 0x40000000);
            SetMemory(uec + GetSpellNumber("SPELL_DEATH_RAY"), 0x40000000);
            SetMemory(uec + GetSpellNumber("SPELL_BURN"), 0x40000000);
            SetMemory(uec + GetSpellNumber("SPELL_INVERSION"), 0x08000000);
            SetMemory(uec + GetSpellNumber("SPELL_COUNTERSPELL"), 0x08000000);
        }*/
    }
}

void MonsterAirshipCaptainProcess(int unit)
{
    //TODO: Index. 8, ThingName= AirshipCaptain
    int ptr = UnitToPtr(unit), uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, AirshipCaptainBinTable());
        SetUnitMaxHealth(unit, 250);
        RetreatLevel(unit, 0.0);
        ResumeLevel(unit, 1.0);
    }
}

void MonsterWoundedApprentice(int unit)
{
    //TODO: Index. 9, ThingName= WoundedApprentice
    int ptr = UnitToPtr(unit), uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WoundedApprenticeBinTable());
        SetUnitMaxHealth(unit, 230);
        RetreatLevel(unit, 0.0);
        ResumeLevel(unit, 1.0);
        SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x8000);
    }
}

void FunctionNumber(int func)
{
    int temp = GetMemory(0x75ae28) + (0x30 * func) + 0x1c;

    if (temp)
    {
        MEMSET_FN_PTR = GetMemory(temp);
    }
}

void MapExit()
{
    MusicEvent();
}
static int InitMathSineBaseWaypointNumber() { return 253; }

void MapInitialize()
{
    MusicEvent();
    MasterUnit();
    //init_def

    LockDoor(Object("Area2Gate"));

    PlaceWaspNest(120);
    PlaceWaspNest(121);
    PlaceWaspNest(122);
    PlaceWaspNest(123);
    FirstBossBlocks();

    //delay_run
    FrameTimer(1, AfterTriggersExec);
    FrameTimer(60, PutGenerators);
    FrameTimer(10, PutHorrendous);
    FrameTimer(122, StrOrbisShop);
    //loop_run
    FrameTimer(3, PlayerClassOnLoop);
    FrameTimer(100, LoopSearchIndex);
}

void AfterTriggersExec()
{
    int entry = CreateObject("Maiden", 78);

    Frozen(entry, 1);
    SetCallback(entry, 9, GoLastTransmission);
    Putchar();
    CmdLine("set spell SPELL_CHARM off", FALSE);
    DefPointerOfFunc();
    PutDecorations();
    InitStatues(0);
    FrameTimer(30, StrGoShop);
    FrameTimer(70, InitShopItems);
    FrameTimer(120, PlacedMobF1);
    FrameTimerWithArg(100, 0, HowToPlay);
}

string GamePlayStr(int num)
{
    string table[] = {
        "게임방법- 오르비스 탑 최상층에 도달하는 것이 당신의 임무에요",
        "총 24층이 존재하며, 한 층에 존재하는 모든 몬스터를 처리해야만 다음 층으로 넘어갈 수 있습니다",
        "참고로 다음층으로 넘어가면 이전 층으로는 더 이상 이동할 수 없습니다",
        "일부 구간에서는 보스 몬스터가 존재합니다, 보스 몬스터는 특수한 스킬을 사용하므로 주의하세요",
        "몬스터를 잡을 때 마다 돈이 증가되며, 돈은 상점에서 여러가지 능력이 깃든 아이템들을 구입하는 데 유용할 것입니다",
        "상점 품목 중 열쇠 아이템은 상점 내 3개의 창고를 여는데 쓰입니다, 일부 품목의 경우 최대 5개 까지만 구입할 수 있습니다",
        "필드에서 마을로 돌아가려면 필드 시작지점에 있는 요정을 클릭하면, 잠시 후 마을로 이동됩니다",
        "필드에서 마을로 이동하는 것은 독저항 물약을 사용해도 가능합니다",
        "설명이 모두 끝났습니다, 이제 게임을 진행하세요"
    };
    
    return table[num];
}

void HowToPlay(int order)
{
    if (order < 9)
    {
        PrintToAll(GamePlayStr(order));
        SecondTimerWithArg(4, order + 1, HowToPlay);
    }
}

int FirstBossBlocks()
{
    int ptr, k;

    if (!ptr)
    {
        ptr = CreateObject("InvisibleLightBlueHigh", 150) + 1;
        CreateObject("SpikeBlock", 150);
        CreateObject("SpikeBlock", 151);
        CreateObject("SpikeBlock", 152);
        CreateObject("SpikeBlock", 153);
        CreateObject("SpikeBlock", 154);
        CreateObject("SpikeBlock", 155);
        CreateObject("SpikeBlock", 156);
        CreateObject("SpikeBlock", 157);
        CreateObject("SpikeBlock", 158);
        CreateObject("SpikeBlock", 159);
        CreateObject("SpikeBlock", 170);
        CreateObject("SpikeBlock", 171);
        CreateObject("SpikeBlock", 172);
        CreateObject("SpikeBlock", 173);
        CreateObject("SpikeBlock", 174);
        CreateObject("SpikeBlock", 175);
        CreateObject("SpikeBlock", 176);
        CreateObject("SpikeBlock", 177);
        CreateObject("SpikeBlock", 178);
        CreateObject("SpikeBlock", 179);
        for (k = 0 ; k < 10 ; k += 1)
        {
            LookWithAngle(ptr + k, 160 + k);
            LookWithAngle(ptr + k + 10, 180 + k);
            CreateMoverFix(ptr + k, 0, 6.0);
            CreateMoverFix(ptr + k + 10, 0, 6.0);
        }
    }
    return ptr;
}

void StartBlockTriggers()
{
    int ptr = FirstBossBlocks(), k;

    if (!GetDirection(ptr - 1))
    {
        LookWithAngle(ptr - 1, 1);
        AudioEvent("SpikeBlockMove", 164);
        AudioEvent("SpikeBlockMove", 169);
        for (k = 19 ; k >= 0 ; k -= 1)
        {
            Move(ptr + k, GetDirection(ptr + k));
        }
        FrameTimerWithArg(150, ptr - 1, ResetRow);
    }
}

void ResetRow(int ptr)
{
    LookWithAngle(ptr, 0);
}

void RemoveGardenWalls()
{
    int k;

    ObjectOff(self);
    for (k = 13 ; k >= 0 ; k --)
        WallOpen(Wall(60 - k, 36 + k));
}

void EntryPointPart15()
{
    int k;

    ObjectOff(self);
    if (!k)
    {
        for (k = 6 ; k >= 0 ; k --)
            WallOpen(Wall(18 + k, 68 + k));
    }
}

void EntryPointPart25()
{
    int k;

    ObjectOff(self);
    if (!k)
    {
        for (k = 0 ; k < 6 ; k ++)
            WallOpen(Wall(122 - k, 46 + k));
    }
}

void Putchar()
{
    int count, k;

    if (count < 12)
    {
        MoveWaypoint(79, GetWaypointX(80), GetWaypointY(80));
        for (k = 9 ; k >= 0 ; k --)
        {
            Frozen(CreateObject(ItemList(count), 79), 1);
            CheckWeaponProperty(GetMemory(0x750710));
            MoveWaypoint(79, GetWaypointX(79) + 32.0, GetWaypointY(79) + 32.0);
        }
        MoveWaypoint(80, GetWaypointX(80) - 25.0, GetWaypointY(80) + 25.0);
        count ++;
        FrameTimer(1, Putchar);
    }
}

void CheckWeaponProperty(int ptr)
{
    int id = GetMemory(ptr + 4);

    if (id >= 222 && id <= 225)
        SetMemory(ptr + 0x2c4, 0x53a720);
    else if (id == 1178)
        SetMemory(GetMemory(ptr + 0x2e0), 0xffff);
    else if (id == 1168)
        SetMemory(GetMemory(ptr + 0x2e0), 0xffff);
}

string ItemList(int num)
{
    string table[] =
    {
        "FanChakram", "Breastplate", "PlateArms", "PlateBoots", "PlateLeggings",
        "WarHammer", "GreatSword", "RoundChakram", "OrnateHelm", "SteelShield",
        "RedPotion", "CurePoisonPotion", "OblivionHeart", "OgreAxe"
    };
    return table[num];
}

string PotionList(int num)
{
    string table[] =
    {
        "RedPotion", "CurePoisonPotion","PoisonProtectPotion", "FireProtectPotion", "ShockProtectPotion",
        "HastePotion", "ShieldPotion", "YellowPotion", "BlackPotion"
    };
    return table[num];
}

string GoldList(int num)
{
    string table[] = {"QuestGoldChest", "QuestGoldPile", "Gold"};

    return table[num];
}

int GoldDrop(int posUnit)
{
    int unit = CreateObjectAt(GoldList(Random(0, 2)), GetObjectX(posUnit), GetObjectY(posUnit));
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2b4), Random(1000, 9000));
    return unit;
}

void DefPointerOfFunc()
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 10);

    CreateObject("InvisibleLightBlueHigh", 11);
    Raise(ptr, PlacedMobF1);
    Raise(ptr + 1, OpenWallsArea11);
    Set_fptr = ToInt(GetObjectZ(ptr));
    Next_fptr = ToInt(GetObjectZ(ptr + 1));
    Delete(ptr);
    Delete(ptr + 1);
}

void PutDecorations()
{
    int ptr = CreateObject("MovableStatue2h", 4);
    CreateObject("MovableStatue2b", 5);
    CreateObject("MovableStatue2f", 6);
    CreateObject("MovableStatue2d", 7);
    Frozen(CreateObject("DunMirScaleTorch1", 191), 1);
    Frozen(CreateObject("DunMirScaleTorch1", 192), 1);

    Frozen(CreateObject("PiledBarrels1", 193), 1);
    MoveWaypoint(193, GetWaypointX(193) + 23.0, GetWaypointY(193) + 23.0);
    Frozen(CreateObject("PiledBarrels1", 193), 1);
    Enchant(ptr, "ENCHANT_FREEZE", 0.0);
    Enchant(ptr + 1, "ENCHANT_FREEZE", 0.0);
    Enchant(ptr + 2, "ENCHANT_FREEZE", 0.0);
    Enchant(ptr + 3, "ENCHANT_FREEZE", 0.0);
    FrameTimer(1, PutDecorations2);
}

void PutDecorations2()
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 190);
    Frozen(CreateObject("Maiden", 291), 1);
    Frozen(CreateObject("Maiden", 293), 1);
    SetCallback(ptr + 1, 9, GoShop);
    SetCallback(ptr + 2, 9, BackToBasecamp);

    Frozen(CreateObject("DunMirScaleTorch2", 284), 1);
    Frozen(CreateObject("DunMirScaleTorch2", 285), 1);
    Frozen(CreateObject("DunMirScaleTorch2", 287), 1);
    Frozen(CreateObject("DunMirScaleTorch2", 288), 1);
    Frozen(CreateObject("DunMirScaleTorch1", 286), 1);
    Frozen(CreateObject("TorchPole", 289), 1);
}

void PlacedMobF1()
{
    Mob_count = 6;
    Exit_wp = 13;
    SpawnMonster("Swordsman", 10, 100, 70);
    SpawnMonster("Swordsman", 10, 100, 70);
    SpawnMonster("Swordsman", 10, 100, 70);
    SpawnMecaFlier(11);
    SpawnMecaFlier(12);
    SpawnMecaFlier(12);
}

void PlacedMobF2()
{
    Mob_count = 18;
    Exit_wp = 15;
    SpawnMonster("Ghost", 17, 60, 50);
    SpawnMonster("Ghost", 18, 60, 50);
    SpawnMonster("Ghost", 19, 60, 50);
    SpawnMonster("Ghost", 20, 60, 50);
    SpawnMonster("Skeleton", 21, 150, 90);
    SpawnMonster("Skeleton", 22, 150, 90);
    SpawnMonster("SkeletonLord", 21, 220, 130);
    SpawnMonster("SkeletonLord", 22, 220, 130);
    SpawnMonster("EvilCherub", 23, 80, 50);
    SpawnMonster("EvilCherub", 24, 80, 50);
    SpawnMonster("EvilCherub", 25, 80, 50);
    SpawnMonster("EvilCherub", 26, 80, 50);
    SpawnStaffLich(27);
    SpawnStaffLich(28);
    SpawnMonster("SkeletonLord", 29, 220, 160);
    SpawnMonster("SkeletonLord", 30, 220, 160);
    SpawnMonster("SkeletonLord", 31, 220, 160);
    SpawnMonster("SkeletonLord", 32, 220, 160);
}

void PlacedMobF3()
{
    Mob_count = 12;
    Exit_wp = 39;
    SpawnMonster("BlackBear", 33, 225, 250);
    SpawnMonster("BlackBear", 34, 225, 250);
    SpawnMonster("Shade", 33, 128, 180);
    SpawnMonster("Shade", 34, 128, 180);
    SpawnBear(35);
    SpawnMonster("SpittingSpider", 35, 95, 80);
    SpawnBear(36);
    SpawnMonster("SpittingSpider", 36, 95, 80);
    SpawnDryad(37);
    SpawnBear(38);
    SpawnMonster("Shade", 37, 128, 180);
    SpawnMonster("Shade", 37, 128, 180);
}

void PlacedMobF4()
{
    Mob_count = 27;
    Exit_wp = 42;
    SpawnMonster("Shade", 44, 128, 80);
    SpawnMonster("Shade", 44, 128, 80);
    SpawnGoon(44);
    SpawnGoon(44);
    SpawnDryad(45);
    SpawnDryad(46);
    SpawnMonster("Shade", 47, 128, 80);
    SpawnMonster("Shade", 47, 128, 80);
    SpawnMonster("BlackWolf", 48, 195, 105);
    SpawnMonster("BlackWolf", 48, 195, 105);
    SpawnMonster("BlackWolf", 48, 195, 105);
    SpawnGoon(49);
    SpawnGoon(49);
    SpawnGoon(49);
    SpawnDryad(50);
    SpawnDryad(51);
    SpawnGoon(52);
    SpawnMonster("Shade", 52, 128, 80);
    SpawnMonster("Shade", 52, 128, 80);
    SpawnMonster("Shade", 52, 128, 80);
    SpawnMonster("Scorpion", 53, 225, 130);
    SpawnMonster("Scorpion", 53, 225, 130);
    SpawnMonster("Scorpion", 53, 225, 130);
    SpawnMonster("BlackWolf", 53, 195, 105);
    SpawnMonster("BlackWolf", 53, 195, 105);
    SpawnMonster("Scorpion", 53, 225, 130);
    SpawnMonster("Scorpion", 53, 225, 130);
}

void PlacedMobF5()
{
    Mob_count = 18;
    Exit_wp = 54;
    SpawnMonster("Imp", 58, 60, 30);
    SpawnMonster("Imp", 58, 60, 30);
    SpawnMonster("MeleeDemon", 58, 128, 210);
    SpawnMonster("MeleeDemon", 58, 128, 210);
    SpawnMonster("MeleeDemon", 58, 128, 210);
    SpawnMonster("MeleeDemon", 58, 128, 210);
    SpawnMonster("MeleeDemon", 58, 128, 210);
    SpawnFireSprite(59);
    SpawnFireSprite(56);
    SpawnMonster("EmberDemon", 56, 108, 230);
    SpawnFireSprite(57);
    SpawnMonster("EmberDemon", 57, 108, 230);
    SpawnFireSprite(60);
    SpawnMonster("EmberDemon", 60, 108, 230);
    SpawnMonster("EmberDemon", 59, 108, 230);
    SpawnMonster("MeleeDemon", 59, 128, 210);
    SpawnMonster("MeleeDemon", 59, 128, 210);
    SpawnMonster("MeleeDemon", 59, 128, 210);
}

void PlacedMobF6()
{
    Mob_count = 22;
    Exit_wp = 61;
    SpawnShopkeeper(67);
    SpawnShopkeeper(67);
    SpawnShopkeeper(67);
    SpawnMonster("GruntAxe", 67, 260, 340);
    SpawnMonster("OgreWarlord", 63, 325, 420);
    SpawnMonster("OgreBrute", 63, 325, 420);
    SpawnMonster("OgreBrute", 63, 325, 420);
    SpawnMonster("GruntAxe", 64, 325, 420);
    SpawnMonster("GruntAxe", 64, 325, 420);
    SpawnMonster("GruntAxe", 64, 325, 420);
    SpawnGoon(66);
    SpawnGoon(66);
    SpawnShopkeeper(65);
    SpawnShopkeeper(65);
    SpawnMonster("OgreWarlord", 65, 325, 440);
    SpawnMonster("OgreWarlord", 65, 325, 440);
    SpawnGoon(65);
    SpawnMonster("GruntAxe", 68, 260, 340);
    SpawnMonster("GruntAxe", 68, 260, 340);
    SpawnMonster("OgreBrute", 68, 325, 420);
    SpawnMonster("OgreBrute", 68, 325, 420);
    SpawnMonster("Scorpion", 68, 240, 130);
}

void PlacedMobBoss1()
{
    int boss;
    Mob_count = 8;
    Exit_wp = 69;
    SpawnPlant(74);
    SpawnPlant(75);
    SpawnPlant(76);
    SpawnMonster("UrchinShaman", 70, 98, 260);
    SpawnMonster("UrchinShaman", 71, 98, 260);
    SpawnMonster("UrchinShaman", 72, 98, 260);
    SpawnMonster("UrchinShaman", 73, 98, 260);
    boss = SpawnBoss("StoneGolem", 77, 1050, 1700);
    SetCallback(boss, 3, BossGolemWeapon);
    SetCallback(boss, 9, GolemTouchedOnNormal);
}

void PlacedMobF21()
{
    Mob_count = 14;
    Exit_wp = 82;
    SpawnMystic(90);
    SpawnMystic(91);
    SpawnMonster("Zombie", 83, 97, 60);
    SpawnMonster("Zombie", 84, 97, 60);
    SpawnMonster("Zombie", 85, 97, 60);
    SpawnMonster("Zombie", 86, 97, 60);
    SpawnMonster("SkeletonLord", 83, 225, 110);
    SpawnMonster("SkeletonLord", 84, 225, 110);
    SpawnMonster("SkeletonLord", 85, 225, 110);
    SpawnMonster("SkeletonLord", 86, 225, 110);
    SpawnDeadWorker(87);
    SpawnMonster("Ghost", 88, 60, 20);
    SpawnDeadWorker(89);
    SpawnMonster("WillOWisp", 92, 300, 450);
}

void PlacedMobF22()
{
    Mob_count = 16;
    Exit_wp = 94;

    SpawnBlackSpider(99);
    SpawnBlackSpider(102);
    SpawnBlackSpider(105);
    SpawnBlackSpider(108);
    SpawnMonster("WillOWisp", 96, 300, 450);
    SpawnMonster("WillOWisp", 101, 300, 450);
    SpawnMonster("WillOWisp", 106, 300, 450);
    SpawnMonster("WillOWisp", 111, 300, 450);
    SpawnImp(97);
    SpawnImp(98);
    SpawnImp(100);
    SpawnImp(103);
    SpawnImp(104);
    SpawnImp(107);
    SpawnImp(109);
    SpawnImp(110);
}

void PlacedMobF23()
{
    int k;

    Mob_count = 36;
    Exit_wp = 112;

    SpawnMonster("Lich", 114, 350, 560);
    SpawnMonster("Lich", 115, 350, 560);
    for (k = 0 ; k < 17 ; k ++)
    {
        SpawnMonster("SkeletonLord", 116, 250, 200);
        SpawnMonster("Skeleton", 117, 195, 137);
        MoveWaypoint(116, GetWaypointX(116) + 23.0, GetWaypointY(116) + 23.0);
        MoveWaypoint(117, GetWaypointX(117) + 23.0, GetWaypointY(117) + 23.0);
    }
}

void PlacedMobF24()
{
    int k;

    Mob_count = 53;
    Exit_wp = 118;
    for (k = 16 ; k >= 0 ; k --)
    {
        SpawnMonster("EvilCherub", 124, 64, 100);
        SpawnMonster("Shade", 125, 128, 160);
        SpawnBlackSpider(126);
        MoveWaypoint(124, GetWaypointX(124) - 23.0, GetWaypointY(124) + 23.0);
        MoveWaypoint(125, GetWaypointX(125) - 23.0, GetWaypointY(125) + 23.0);
        MoveWaypoint(126, GetWaypointX(126) - 23.0, GetWaypointY(126) + 23.0);
    }
    Enchant(SpawnMonster("StoneGolem", 127, 700, 842), "ENCHANT_INVISIBLE", 0.0);
    Enchant(SpawnMonster("StoneGolem", 128, 700, 842), "ENCHANT_INVISIBLE", 0.0);
}

void PlacedMobF25()
{
    int k;

    Mob_count = 60;
    Exit_wp = 130;
    for (k = 17 ; k >= 0 ; k --)
    {
        SpawnImp(131);
        SpawnMonster("MeleeDemon", 132, 98, 226);
        SpawnDeadWorker(133);
        MoveWaypoint(131, GetWaypointX(131) - 23.0, GetWaypointY(131) + 23.0);
        MoveWaypoint(132, GetWaypointX(132) - 23.0, GetWaypointY(132) + 23.0);
        MoveWaypoint(133, GetWaypointX(133) - 23.0, GetWaypointY(133) + 23.0);
    }
    SpawnMystic(135);
    SpawnMystic(136);
    SpawnMystic(137);
    SpawnMystic(138);
    SpawnMystic(139);
    SpawnMonster("Demon", 134, 400, 360);
}

void PlacedMobF26()
{
    int k;

    Mob_count = 35;
    Exit_wp = 142;

    for (k = 14 ; k >= 0 ; k --)
    {
        SpawnMonster("WillOWisp", 140, 225, 472);
        SpawnFireSprite(140);
    }
    SpawnBeholder(143);
    SpawnBeholder(144);
    SpawnBeholder(145);
    SpawnBeholder(146);
    SpawnBeholder(147);
}

void PlacedMobBoss2()
{
    int boss;

    Mob_count = 1;
    Exit_wp = 149;

    boss = SpawnBoss("WizardRed", 190, 2080, 3800);
    SetUnitStatus(boss, GetUnitStatus(boss) ^ 0x20);
    SetCallback(boss, 3, SecondBossWeapon);
}

void PlacedMobF31()
{
    Mob_count = 12;
    Exit_wp = 195;
    SpawnMonster("EmberDemon", 204, 98, 225);
    SpawnMonster("EmberDemon", 205, 98, 225);
    SpawnMonster("EmberDemon", 206, 98, 225);
    SpawnMonster("EmberDemon", 207, 98, 225);
    SpawnMonster("WillOWisp", 196, 225, 370);
    SpawnMonster("WillOWisp", 197, 225, 370);
    SpawnMonster("WillOWisp", 198, 225, 370);
    SpawnMonster("WillOWisp", 199, 225, 370);
    SpawnHorrendous(200);
    SpawnHorrendous(201);
    SpawnHorrendous(202);
    SpawnHorrendous(203);
}

void PlacedMobF32()
{
    int k;
    Mob_count = 44;
    Exit_wp = 209;

    for (k = 0 ; k < 18 ; k ++)
    {
        SpawnMonster("Swordsman", 210, 306, 400);
        SpawnMonster("Archer", 211, 98, 78);
        if (k < 8)
        {
            SpawnHorrendous(212);
            MoveWaypoint(212, GetWaypointX(212) + 30.0, GetWaypointY(212) + 30.0);
        }
        MoveWaypoint(210, GetWaypointX(210) + 23.0, GetWaypointY(210) + 23.0);
        MoveWaypoint(211, GetWaypointX(211) + 23.0, GetWaypointY(211) + 23.0);
    }
}

void PlacedMobF33()
{
    int k;
    Mob_count = 78;
    Exit_wp = 213;
    
    for (k = 0 ; k < 18 ; k ++)
    {
        SpawnMonster("Shade", 217, 98, 80);
        SpawnMonster("SpittingSpider", 218, 64, 40);
        SpawnMonster("MeleeDemon", 219, 98, 91);
        SpawnMonster("SkeletonLord", 220, 240, 105);
        if (k < 6)
        {
            SpawnMystic(221);
            MoveWaypoint(221, GetWaypointX(221) + 30.0, GetWaypointY(221) + 30.0);
        }
        MoveWaypoint(217, GetWaypointX(217) + 23.0, GetWaypointY(217) + 23.0);
        MoveWaypoint(218, GetWaypointX(218) + 23.0, GetWaypointY(218) + 23.0);
        MoveWaypoint(219, GetWaypointX(219) + 23.0, GetWaypointY(219) + 23.0);
        MoveWaypoint(220, GetWaypointX(220) + 23.0, GetWaypointY(220) + 23.0);
    }
}

void PlacedMobF34()
{
    int k;
    Mob_count = 14;
    Exit_wp = 216;
    for (k = 0 ; k < 7 ; k ++)
    {
        SpawnWhiteWiz(222);
        SpawnMystic(223);
    }
}

void PlacedMobF35()
{
    int k;

    Mob_count = 14;
    Exit_wp = 225;
    SpawnMonster("MechanicalGolem", 226, 700, 1000);
    SpawnMonster("MechanicalGolem", 227, 700, 1000);
    SpawnMonster("MechanicalGolem", 228, 700, 1000);
    SpawnMonster("MechanicalGolem", 229, 700, 1000);

    for (k = 0 ; k < 5 ; k ++)
    {
        SpawnDeadWorker(230);
        SpawnMonster("WeirdlingBeast", 231, 175, 600);
        MoveWaypoint(230, GetWaypointY(230) - 32.0, GetWaypointY(230) + 32.0);
        MoveWaypoint(231, GetWaypointY(231) - 32.0, GetWaypointY(231) + 32.0);
    }
}

void PlacedMobF36()
{
    int k;
    Mob_count = 25;
    Exit_wp = 232;
    
    for (k = 4 ; k >= 0 ; k --)
    {
        SpawnMonster("WeirdlingBeast", 234, 147, 620);
        SpawnMonster("WoundedApprentice", 235, 237, 700);
        SpawnBlackSpider(236);
        SpawnMystic(237);
        SpawnMonster("OgreWarlord", 238, 325, 340);
        MoveWaypoint(234, GetWaypointX(234) + 46.0, GetWaypointY(234));
        MoveWaypoint(235, GetWaypointX(235) + 46.0, GetWaypointY(235));
        MoveWaypoint(236, GetWaypointX(236) + 46.0, GetWaypointY(236));
        MoveWaypoint(237, GetWaypointX(237) + 46.0, GetWaypointY(237));
        MoveWaypoint(238, GetWaypointX(238) + 46.0, GetWaypointY(238));
    }
}

void PlacedMob37()
{
    int k;
    Mob_count = 8 * 7;
    Exit_wp = 240;

    for (k = 0 ; k < 8 ; k ++)
    {
        SpawnImp(241);
        SpawnFireSprite(242);
        SpawnMonster("GruntAxe", 243, 237, 130);
        MoveWaypoint(241, GetWaypointX(241) + 46.0, GetWaypointY(241) + 46.0);
        MoveWaypoint(242, GetWaypointX(242) + 46.0, GetWaypointY(242) + 46.0);
        MoveWaypoint(243, GetWaypointX(243) + 46.0, GetWaypointY(243) + 46.0);
    }
    FrameTimer(1, DelaySpawnF37);
}

void PlacedMob38()
{
    int k, boss;
    Mob_count = 12;
    Exit_wp = 252;
    
    for (k = 0 ; k < 8 ; k ++)
    {
        SpawnHorrendous(248);
        if (k < 3)
        {
            SpawnMonster("MechanicalGolem", 249, 750, 900);
            MoveWaypoint(249, GetWaypointX(249) - 46.0, GetWaypointY(249) + 46.0);
        }
        MoveWaypoint(248, GetWaypointX(248) - 23.0, GetWaypointY(248) + 23.0);
    }
    boss = SpawnBoss("Demon", 250, 1260, 3000);
    SetCallback(boss, 3, ThirdBossSkills);
}

void PlacedCastleTop()
{
    int boss, addr;

    Mob_count = 1;
    Exit_wp = 256;
    boss = SpawnBoss("Goon", 257, 2000, 300000);
    CreateObject("InvisibleLightBlueHigh", 258);
    addr = UnitToPtr(boss);
    if (addr)
    {
        SetMemory(addr + 0x14, 0x22);                           //can't visible
        SetMemory(addr + 0xc, GetMemory(addr + 0xc) ^ 0x200);   //Immune_poison
    }
    SetCallback(boss, 3, KeeperWaepon);
    LookWithAngle(boss, 0);
    FrameTimerWithArg(1, boss, LoopDetectPlayers);
}

void DestinationCastleTop()
{
    Mob_count = 9999;
    Exit_wp = 258;
}

int PlaceWaspNest(int wp)
{
    int unit = CreateObject("WaspNest", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    SetUnitMaxHealth(unit, 500);
    FrameTimerWithArg(1, unit, NestCheckLoop);
}

void NestCheckLoop(int unit)
{
    if (CurrentHealth(unit))
    {
        FrameTimerWithArg(2, unit, NestCheckLoop);
    }
    else
    {
        MoveWaypoint(41, GetObjectX(unit + 1), GetObjectY(unit + 1));
        AudioEvent("PoisonTrapTriggered", 41);
        DeleteObjectTimer(CreateObject("BigSmoke", 41), 12);
        CreateObject("Wasp", 41);
        CreateObject("Wasp", 41);
        CreateObject("Wasp", 41);
        CreateObject("Wasp", 41);
        Delete(unit + 1);
    }
}

int MasterUnit()
{
    int unit;

    if (!unit)
    {
        unit = CreateObject("Hecubah", 95);
        Frozen(unit, 1);
    }
    return unit;
}

int SpawnBeholder(int wp)
{
    int unit = CreateObject("Beholder", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(833));
    UnitSettingBased(unit, 295);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    return unit;
}

int SpawnBoss(string name, int wp, int hp, int score)
{
    int unit = CreateObject(name, wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    CheckMonsterThing(unit);
    Raise(unit + 1, ToFloat(score));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, hp);
    SetCallback(unit, 5, DeadBossMonster);
    return unit;
}

int SpawnMonster(string name, int wp, int hp, int score)
{
    int unit = CreateObject(name, wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(score));
    UnitSettingBased(unit, hp);
    return unit;
}

int SpawnImp(int wp)
{
    int unit = CreateObject("Imp", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(95));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    UnitSettingBased(unit, 64);
    SetCallback(unit, 3, ImpShots);
    return unit;
}

int SpawnMecaFlier(int wp)
{
    int unit = CreateObject("FlyingGolem", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(60));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    UnitSettingBased(unit, 70);
    SetCallback(unit, 3, MecaFlyingWeapon);
    return unit;
}

int SpawnStaffLich(int wp)
{
    int unit = CreateObject("Bear2", wp);
    int ptr = GetMemory(0x750710);

    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(300));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(32));
    UnitSettingBased(unit,  225);
    //SetCallback(unit, 3, LichWeapon);
    return unit;
}

int SpawnBear(int wp)
{
    int unit = CreateObject("Bear", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(300));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    UnitSettingBased(unit, 250);
    SetCallback(unit, 3, BearJumping);
    return unit;
}

int SpawnDryad(int wp)
{
    int unit = CreateObject("WizardGreen", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(300));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    UnitSettingBased(unit, 175);
    return unit;
}

int SpawnShopkeeper(int wp)
{
    int unit = CreateObject("Bear2", wp);

    Raise(CreateObject("InvisibleLightBlueHigh", wp), ToFloat(30));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    UnitSettingBased(unit, 275);
    return unit;
}

int SpawnGoon(int wp)
{
    int unit = CreateObject("Goon", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(30));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit), 500.0);
    UnitSettingBased(unit, 128);
    return unit;
}

int SpawnFireSprite(int wp)
{
    int unit = CreateObject("FireSprite", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(60));
    SetUnitSpeed(unit, 0.8);
    UnitSettingBased(unit, 96);
    return unit;
}

int SpawnPlant(int wp)
{
    int unit = CreateObject("CarnivorousPlant", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(450));
    SetUnitSpeed(unit, 1.0);
    UnitSettingBased(unit, 300);
    AggressionLevel(unit, 1.0);
    return unit;
}

int SpawnBlackSpider(int wp)
{
    int unit = CreateObject("BlackWidow", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(328));
    UnitSettingBased(unit, 245);
    SetCallback(unit, 5, SpiderWhenDead);
    return unit;
}

int SpawnDeadWorker(int wp)
{
    int unit = CreateObject("VileZombie", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(411));
    SetUnitSpeed(unit, 1.2);
    UnitSettingBased(unit, 325);
    SetCallback(unit, 3, DeadWorkerWeapon);
    return unit;
}

int SpawnMystic(int wp)
{
    int unit = CreateObject("Wizard", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(400));
    UnitSettingBased(unit, 175);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    MysticLoop(unit);
    return unit;
}

int SpawnWhiteWiz(int wp)
{
    int unit = CreateObject("WizardWhite", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(480));
    UnitSettingBased(unit, 237);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    return unit;
}

int SpawnHorrendous(int wp)
{
    int unit = CreateObject("Horrendous", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Raise(unit + 1, ToFloat(780));
    SetUnitSpeed(unit, 0.9);
    UnitSettingBased(unit, 300);
    SetCallback(unit, 3, HorrendousDash);
    return unit;
}

void UnitSettingBased(int unit, int hp)
{
    CheckMonsterThing(unit);
    SetUnitMaxHealth(unit, hp + 1000);
    AggressionLevel(unit, 1.0);
    RetreatLevel(unit, 0.0);
    SetCallback(unit, 5, DeadMonster);
    SetCallback(unit, 7, HurtMonster);
    SetOwner(MasterUnit(), unit);
}

void HurtMonster()
{
    int plr;
    if (CurrentHealth(self) <= 1000)
    {
        Damage(self, other, CurrentHealth(self) + 1, 14);
        plr = CheckOwner(other);
        if (plr >= 0)
        {
            ChangeGold(player[plr], ToInt(GetObjectZ(GetTrigger() + 1)));
        }
    }
}

void DeadMonster()
{
    int rnd = Random(0, 4);

    if (!rnd)
    {
        int item = CreateObjectAt(ItemList(Random(0, 13)), GetObjectX(SELF), GetObjectY(SELF));

        CheckWeaponProperty(item);
        SetWeaponProperties(item, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    }
    else if (rnd == 1)
        GoldDrop(SELF);
    else
        CheckPotionThingID(CreateObjectAt(PotionList(Random(0, 8)), GetObjectX(SELF), GetObjectY(SELF)));
    DeleteObjectTimer(SELF, 90);
    Delete(GetTrigger() + 1);
    CountDungeonMobDeaths();
}

void DeadBossMonster()
{
    GoldEventWhenDeadBoss(ToInt(GetObjectZ(GetTrigger() + 1)));
    Delete(GetTrigger() + 1);
    CountDungeonMobDeaths();
}

void CountDungeonMobDeaths()
{
    int unit;

    Mob_count --;
    if (!Mob_count)
    {
        if (Floor_lv < 22)
        {
            unit = CreateObject("Maiden", Exit_wp);
            ObjectOff(unit);
            Frozen(unit, 1);
            SetCallback(unit, 9, TeleportToNextArea);
            PrintToAll("다음 구역으로 가는 비밀계단이 개방됩니다");
            FrameTimer(1, Next_fptr + Floor_lv);
            FrameTimer(1, FixGoingBackMark);
        }
        else
        {
            VictoryEvent();
        }
    }
}

void FixGoingBackMark()
{
    FrameTimerWithArg(3, Start_wp, HomePortal);
}

void TeleportToNextArea()
{
    if (CurrentHealth(OTHER) && IsPlayerUnit(OTHER))
    {
        if (!IsObjectOn(SELF))
        {
            MoveWaypoint(3, GetWaypointX(Start_wp), GetWaypointY(Start_wp));
            // Floor_lv ++;
            FrameTimer(1, Set_fptr + ++Floor_lv);
            ObjectOn(SELF);
        }
        else
        {
            MoveObject(OTHER, GetWaypointX(3), GetWaypointY(3));
            UniPrint(OTHER, "오르비스 탑 " + IntToString(Floor_lv + 1) + " 층");
        }
    }
}

void GoldEventWhenDeadBoss(int amount)
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (CurrentHealth(player[k]))
            ChangeGold(player[k], amount);
    }
    UniPrintToAll("보스를 처치하셨습니다, 모든 플레이어에게 미네랄 " + IntToString(amount) + " 이(가) 제공됩니다");
}

void OpenWallsArea11()
{
    WallOpen(Wall(24, 196));
    WallOpen(Wall(23, 197));
    WallOpen(Wall(22, 198));
    WallOpen(Wall(21, 199));
    Start_wp = 14;
}

void OpenWallsArea12()
{
    UnlockDoor(Object("Area2Gate"));
    Start_wp = 16;
}

void OpenWallsArea13()
{
    int k;

    Start_wp = 40;
    for (k = 7 ; k >= 0 ; k --)
        WallOpen(Wall(39 + k, 115 + k));
}

void OpenWallsArea14()
{
    int k;

    Start_wp = 43;
    for (k = 7 ; k >= 0 ; k --)
        WallOpen(Wall(51 - k, 101 + k));
}

void OpenWallsArea15()
{
    int k;

    Start_wp = 55;
    for (k = 4 ; k >= 0 ; k --)
        WallOpen(Wall(42 - k, 58 + k));
}

void OpenWallsArea16()
{
    int k;

    Start_wp = 62;
    for (k = 4 ; k >= 0 ; k --)
        WallOpen(Wall(38 - k, 18 + k));
}

void OpenWallsArea1Boss()
{
    int k;

    Start_wp = 81;
    for (k = 3 ; k >= 0 ; k --)
        WallOpen(Wall(77 + k, 233 + k));
}

void OpenWallsArea21()
{
    Start_wp = 93;
}

void OpenWallsArea22()
{
    int k;

    Start_wp = 113;
    for (k = 6 ; k >= 0 ; k --)
        WallOpen(Wall(63 + k, 125 + k));
}

void OpenWallsArea23()
{
    Start_wp = 119;
    WallOpen(Wall(69, 85));
    WallOpen(Wall(68, 86));
    WallOpen(Wall(67, 87));
}

void OpenWallsArea24()
{
    Start_wp = 129;
}

void OpenWallsArea25()
{
    int k;
    Start_wp = 141;

    for (k = 6 ; k >= 0 ; k --)
        WallOpen(Wall(95 - k, 29 + k));
}

void OpenWallsArea2Boss()
{
    PrintToAll("보스를 처치하셨습니다, 방금 포탈이 열렸습니다");
    Start_wp = 194;
}

void OpenWallsArea31()
{
    int k;

    Start_wp = 208;
    for (k = 4 ; k >= 0 ; k --)
        WallOpen(Wall(130 - k, 198 + k));
}

void OpenWallsArea32()
{
    Start_wp = 214;
    WallOpen(Wall(114, 206));
    WallOpen(Wall(115, 207));
    WallOpen(Wall(116, 208));
    WallOpen(Wall(117, 209));
}

void OpenWallsArea33()
{
    int k;
    Start_wp = 215;
    for (k = 0 ; k < 8 ; k ++)
        WallOpen(Wall(101 + k, 175 + k));
}

void OpenWallsArea34()
{
    Start_wp = 224;
}

void OpenWallsArea35()
{
    Start_wp = 233;
    WallOpen(Wall(133, 127));
    WallOpen(Wall(134, 128));
    WallOpen(Wall(135, 129));
    WallOpen(Wall(136, 130));
    WallOpen(Wall(137, 131));
}

void OpenWallsArea36()
{
    Start_wp = 239;
}

void OpenWallsArea37()
{
    Start_wp = 251;
}

void OpenWallsArea38()
{
    Start_wp = 255;
    WallOpen(Wall(164, 48));
    WallOpen(Wall(165, 49));
    WallOpen(Wall(166, 50));
    WallOpen(Wall(167, 51));
    WallOpen(Wall(168, 52));
}

void DelaySpawnF37()
{
    int k;

    for (k = 0 ; k < 8 ; k ++)
    {
        SpawnMonster("TalkingSkull", 244, 180, 200);
        SpawnMonster("Swordsman", 245, 325, 240);
        MoveWaypoint(244, GetWaypointX(244) + 46.0, GetWaypointY(244) + 46.0);
        MoveWaypoint(245, GetWaypointX(245) + 46.0, GetWaypointY(245) + 46.0);
    }
    FrameTimer(1, DelaySpawnF371);
}

void DelaySpawnF371()
{
    int k;

    for (k = 0 ; k < 8 ; k ++)
    {
        SpawnHorrendous(246);
        SpawnMystic(247);
        MoveWaypoint(246, GetWaypointX(246) + 46.0, GetWaypointY(246) + 46.0);
        MoveWaypoint(247, GetWaypointX(247) + 46.0, GetWaypointY(247) + 46.0);
    }
}

void VictoryEvent()
{ }

void MecaFlyingWeapon()
{
    int mis;
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 0.3);
            MoveWaypoint(9, GetObjectX(self) - UnitRatioX(self, other, 7.0), GetObjectY(self) - UnitRatioY(self, other, 7.0));
            mis = CreateObject("WeakArcherArrow", 9);
            Enchant(mis, "ENCHANT_SHOCK", 0.0);
            SetOwner(self, mis);
            LookAtObject(mis, other);
            PushObject(mis, -38.0, GetObjectX(other), GetObjectY(other));
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(21, GetTrigger(), ResetUnitSight);
        }
    }
}

void LichWeapon()
{
    int mis;
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 0.6);
            MoveWaypoint(9, GetObjectX(self) - UnitRatioX(self, other, 7.0), GetObjectY(self) - UnitRatioY(self, other, 7.0));
            mis = CreateObject("DeathBallFragment", 9);
            SetOwner(self, mis);
            PushObject(mis, -17.0, GetObjectX(other), GetObjectY(other));
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(21, GetTrigger(), ResetUnitSight);
        }
    }
}

void BearJumping()
{
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 2.0);
            Raise(self, 250.0);
            FrameTimerWithArg(21, GetTrigger(), EarthQuakeFromBear);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(90, GetTrigger(), ResetUnitSight);
        }
    }
}

void DryadDoSummon()
{
    int ptr;
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 10.0);
            MoveWaypoint(9, GetObjectX(other), GetObjectY(other));
            ptr = CreateObject("InvisibleLightBlueHigh", 9);
            CreateObject("InvisibleLightBlueHigh", 9);
            Raise(ptr, UnitRatioX(self, other, 46.0));
            Raise(ptr + 1, UnitRatioY(self, other, 46.0));
            SetOwner(self, ptr);
            FrameTimerWithArg(1, GetTrigger(), SpawningPool);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(300, GetTrigger(), ResetUnitSight);
        }
    }
}

void GoonWeapon()
{
    int mis;

    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 0.7);
            MoveWaypoint(9, GetObjectX(self) - UnitRatioX(self, other, 12.0), GetObjectY(self) - UnitRatioY(self, other, 12.0));
            AudioEvent("EggBreak", 9);
            mis = CreateObject("ThrowingStone", 9);
            CreateObject("SpiderSpit", 9);
            SetOwner(self, mis);
            SetOwner(self, mis + 1);
            PushObject(mis, -30.0, GetObjectX(other), GetObjectY(other));
            PushObject(mis + 1, -30.0, GetObjectX(other), GetObjectY(other));
        }
        Enchant(self, "ENCHANT_BLINDED", 0.07);
        LookAtObject(self, other);
        CreatureFollow(self, other);
        AggressionLevel(self, 1.0);
    }
}

void PlantAi()
{
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 5.0);
            LookAtObject(self, other);
            CreatureFollow(self, other);
            AggressionLevel(self, 1.0);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(90, GetTrigger(), ResetUnitSight);
        }
    }
}

void BossGolemWeapon()
{
    int rnd, ptr;

    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 4.0);
            LookAtObject(self, other);
            rnd = Random(0, 3);
            if (!rnd)
            {
                MoveWaypoint(9, GetObjectX(other), GetObjectY(other));
                ptr = CreateObject("InvisibleLightBlueHigh", 9);
                SetOwner(self, ptr);
                Raise(ptr, ToFloat(GetCaller()));
                Enchant(self, "ENCHANT_FREEZE", 1.0);
                FrameTimerWithArg(30, ptr, DropFist);
            }
            else if (rnd == 1)
            {
                MoveWaypoint(9, GetObjectX(self) - UnitRatioX(self, other, 13.0), GetObjectY(self) - UnitRatioY(self, other, 13.0));
                ptr = CreateObject("InvisibleLightBlueHigh", 9);
                CreateObject("InvisibleLightBlueHigh", 9);
                MoveObject(ptr + 1, GetObjectX(other), GetObjectY(other));
                Raise(ptr, UnitRatioX(self, other, 15.0));
                Raise(ptr + 1, UnitRatioY(self, other, 15.0));
                Enchant(ptr, "ENCHANT_SHOCK", 0.0);
                SetOwner(self, ptr);
                FrameTimerWithArg(1, ptr, GolemSkill);
            }
            else if (rnd == 2)
            {
                MoveWaypoint(9, GetObjectX(self), GetObjectY(self));
                ptr = CreateObject("InvisibleLightBlueHigh", 9);
                CreateObject("InvisibleLightBlueHigh", 9);
                Raise(ptr, UnitRatioX(self, other, 10.0));
                Raise(ptr + 1, UnitRatioY(self, other, 10.0));
                SetOwner(self, ptr);
                ObjectOff(self);
                Enchant(self, "ENCHANT_VAMPIRISM", 0.0);
                SetCallback(self, 9, GolemTouchedOnRush);
                WindRush(ptr);
            }
            else
            {
                LookAtObject(self, other);
                HitLocation(self, GetObjectX(self), GetObjectY(self));
                Effect("DAMAGE_POOF", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
                CastSpellObjectObject("SPELL_EARTHQUAKE", self, self);
                CastSpellObjectObject("SPELL_EARTHQUAKE", self, self);
                CastSpellObjectObject("SPELL_EARTHQUAKE", self, self);
            }
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(150, GetTrigger(), ResetUnitSight);
        }
    }
}

void SpiderWhenDead()
{
    int ptr;
    MoveWaypoint(9, GetObjectX(self), GetObjectY(self));
    AudioEvent("BeholderDie", 9);
    ptr = CreateObject("WaterBarrel", 9);
    DeleteObjectTimer(CreateObject("BigSmoke", 9), 12);
    Damage(ptr, 0, CurrentHealth(ptr) + 1, -1);
    CreateObject("ArachnaphobiaFocus", 9);
    DeadMonster();
}

void DeadWorkerWeapon()
{
    int ptr;
    float pos_x, pos_y;

    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 0.9);
            pos_x = UnitRatioX(self, other, 16.0);
            pos_y = UnitRatioY(self, other, 16.0);
            MoveWaypoint(9, GetObjectX(self) - pos_x * 2.0, GetObjectY(self) - pos_y * 2.0);
            ptr = CreateObject("Maiden", 9);
            CreateObject("MagicEnergy", 9);
            CreateObject("InvisibleLightBlueHigh", 9);
            CreateObject("InvisibleLightBlueHigh", 9);
            LookWithAngle(ptr, 0);
            SetOwner(self, ptr);
            Raise(ptr + 2, pos_x);
            Raise(ptr + 3, pos_y);
            SetCallback(ptr, 9, TouchWorkMis);
            Frozen(ptr, 1);
            FrameTimerWithArg(1, ptr, WorkerFlyingMissile);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(35, GetTrigger(), ResetUnitSight);
        }
    }
}

void ResetUnitSight(int unit)
{
    EnchantOff(unit, "ENCHANT_DETECTING");
    Enchant(unit, "ENCHANT_BLINDED", 0.07);
    AggressionLevel(unit, 1.0);
}

void EarthQuakeFromBear(int unit)
{
    if (CurrentHealth(unit))
    {
        Effect("SMOKE_BLAST", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        Effect("DAMAGE_POOF", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        CastSpellObjectObject("SPELL_EARTHQUAKE", unit, unit);
        CastSpellObjectObject("SPELL_EARTHQUAKE", unit, unit);
        CastSpellObjectObject("SPELL_EARTHQUAKE", unit, unit);
    }
}

void SpawningPool(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), unit;

    if (CurrentHealth(owner) && count < 4)
    {
        MoveWaypoint(9, GetObjectX(ptr), GetObjectY(ptr));
        MoveWaypoint(41, GetObjectX(ptr + 1), GetObjectY(ptr + 1));
        unit = CreateObject("CarnivorousPlant", 9);
        if (count)
        {
            CreateObject("CarnivorousPlant", 41);
            DeleteObjectTimer(CreateObject("BigSmoke", 41), 9);
            Effect("RICOCHET", GetWaypointX(41), GetWaypointY(41), 0.0, 0.0);
            SetOwner(owner, unit + 1);
        }
        DeleteObjectTimer(CreateObject("BigSmoke", 9), 9);
        Effect("RICOCHET", GetWaypointX(9), GetWaypointY(9), 0.0, 0.0);
        SetOwner(owner, unit);
        MoveObject(ptr, GetObjectX(ptr) + GetObjectZ(ptr + 1), GetObjectY(ptr) - GetObjectZ(ptr));
        MoveObject(ptr + 1, GetObjectX(ptr + 1) - GetObjectZ(ptr + 1), GetObjectY(ptr + 1) + GetObjectZ(ptr));
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(3, ptr, SpawningPool);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void DropFist(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        CastSpellObjectObject("SPELL_FIST", owner, target);
    }
    Delete(ptr);
}

void WindRush(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr);

    if (count < 60 && CurrentHealth(owner))
    {
        Effect("DAMAGE_POOF", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
        PushObjectTo(owner, -GetObjectZ(ptr), -GetObjectZ(ptr + 1));
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, WindRush);
    }
    else
    {
        if (CurrentHealth(owner))
        {
            ObjectOn(owner);
            EnchantOff(owner, "ENCHANT_VAMPIRISM");
            SetCallback(owner, 9, GolemTouchedOnNormal);
        }
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void GolemTouchedOnNormal()
{
    return;
}

void GolemTouchedOnRush()
{
    if (CurrentHealth(self) && CurrentHealth(other))
    {
        if (IsAttackedBy(other, MasterUnit()) && HasEnchant(self, "ENCHANT_AFRAID"))
        {
            Damage(other, self, 100, 4);
            Enchant(other, "ENCHANT_CHARMING", 0.5);
        }
    }
}

void WorkerFlyingMissile(int ptr)
{
    int count = GetDirection(ptr);

    if (count < 32 && IsObjectOn(ptr))
    {
        MoveObject(ptr, GetObjectX(ptr) - GetObjectZ(ptr + 2), GetObjectY(ptr) - GetObjectZ(ptr + 3));
        MoveObject(ptr + 1, GetObjectX(ptr), GetObjectY(ptr));
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, WorkerFlyingMissile);
    }
    else
    {
        if (IsObjectOn(ptr))
            Delete(ptr);
        Delete(ptr + 1);
        Delete(ptr + 2);
        Delete(ptr + 3);
    }
}

void TouchWorkMis()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 20, 4);
        Delete(self);
    }
}

void MysticLoop(int unit)
{
    if (CurrentHealth(unit))
    {
        if (HasEnchant(unit, "ENCHANT_INVISIBLE"))
        {
            EnchantOff(unit, "ENCHANT_INVISIBLE");
            Enchant(unit, "ENCHANT_SHOCK", 0.0);
            Enchant(unit, "ENCHANT_SHIELD", 0.0);
        }
        Raise(unit, 100.0);
        FrameTimerWithArg(1, unit, MysticLoop);
    }
}

void ImpShots()
{
    int ptr;

    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 0.4);
            MoveWaypoint(9, GetObjectX(self), GetObjectY(self));
            ptr = CreateObject("InvisibleLightBlueHigh", 9);
            CastSpellObjectObject("SPELL_MAGIC_MISSILE", self, other);
            Delete(ptr);
            Delete(ptr + 2);
            Delete(ptr + 3);
            Delete(ptr + 4);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(20, GetTrigger(), ResetUnitSight);
        }
    }
}

void SecondBossWeapon()
{
    int ptr, rnd;
    float pos_x, pos_y;

    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 7.0);
            rnd = Random(0, 3);
            if (!rnd)
            {
                MoveWaypoint(95, GetObjectX(other), GetObjectY(other));
                ptr = CreateObject("InvisibleLightBlueHigh", 95);
                CreateObject("BlueSummons", 95);
                SetOwner(self, ptr);
                AudioEvent("PullCast", 95);
                Effect("RICOCHET", GetWaypointX(95), GetWaypointY(95), 0.0, 0.0);
                FrameTimerWithArg(21, ptr, DelayTargetDeathray);
            }
            else if (rnd == 1)
            {
                MoveWaypoint(95, GetObjectX(self) - UnitRatioX(self, other, 30.0), GetObjectY(self) - UnitRatioY(self, other, 30.0));
                ptr = CreateObject("VandegrafLargeMovable", 95);
                SetOwner(ptr, self);
                CastSpellObjectObject("SPELL_CHAIN_LIGHTNING", ptr, ptr);
                FrameTimerWithArg(60, ptr, DelayRemoveUnit);
            }
            else if (rnd == 2)
                ThuderStorm();
            else
                Boss2SplashSkill();
        }
        else if (!HasEnchant(self, "ENCHANT_LIGHT"))
        {
            Enchant(self, "ENCHANT_LIGHT", 0.8);
            Boss2NormalAttack();
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(90, GetTrigger(), ResetUnitSight);
        }
    }
}

void ShootingWhiteCrystal(int ptr)
{
    int count = GetDirection(ptr);

    if (count < 32 && IsObjectOn(ptr))
    {
        MoveObject(ptr, GetObjectX(ptr) - GetObjectZ(ptr + 2), GetObjectY(ptr) - GetObjectZ(ptr + 3));
        MoveObject(ptr + 1, GetObjectX(ptr), GetObjectY(ptr));
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, ShootingWhiteCrystal);
    }
    else
    {
        if (IsObjectOn(ptr))
            Delete(ptr);
        Delete(ptr + 1);
        Delete(ptr + 2);
        Delete(ptr + 3);
    }
}

void WhiteCrystalExplosion()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 25, 16);
        Delete(self);
    }
    else if (!GetCaller())
        Delete(self);
}

void DelayTargetDeathray(int ptr)
{
    int owner = GetOwner(ptr);

    if (CurrentHealth(owner))
    {
        CastSpellObjectLocation("SPELL_DEATH_RAY", ptr, GetObjectX(ptr), GetObjectY(ptr));
        CastSpellObjectLocation("SPELL_DEATH_RAY", owner, GetObjectX(ptr), GetObjectY(ptr));
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void DelayRemoveUnit(int unit)
{
    Delete(unit);
}

void ThuderStorm()
{
    int ptr;
    float pos_x = UnitRatioX(self, other, 30.0), pos_y = UnitRatioY(self, other, 30.0);

    Enchant(self, "ENCHANT_FREEZE", 3.0);
    Enchant(self, "ENCHANT_INVULNERABLE", 3.0);
    MoveWaypoint(9, GetObjectX(self) - pos_x, GetObjectY(self) - pos_y);
    ptr = CreateObject("InvisibleLightBlueHigh", 9);
    CreateObject("InvisibleLightBlueHigh", 9);
    Raise(ptr, pos_x);
    Raise(ptr + 1, pos_y);
    SetOwner(self, ptr);
}

void GoAwayLightning(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), unit;

    if (count < 32 && IsVisibleTo(ptr, owner) && CurrentHealth(owner))
    {
        MoveObject(ptr, GetObjectX(ptr) - GetObjectZ(ptr), GetObjectY(ptr) - GetObjectZ(ptr + 1));
        MoveWaypoint(9, GetObjectX(ptr), GetObjectY(ptr));
        unit = CreateObject("InvisibleLightBlueHigh", 9);
        SetOwner(self, unit);
        CreateObject("MagicSpark", 9);
        AudioEvent("LightningBolt", 9);
        AudioEvent("LightningWand", 9);
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(17, unit, ThunderDown);
        FrameTimerWithArg(1, ptr, GoAwayLightning);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void ThunderDown(int ptr)
{
    int owner = GetOwner(ptr), unit;

    if (CurrentHealth(owner))
    {
        MoveWaypoint(41, GetObjectX(ptr), GetObjectY(ptr));
        Effect("LIGHTNING", GetWaypointX(41), GetWaypointY(41), GetWaypointY(41), GetWaypointY(41) - 120.0);
        Effect("JIGGLE", GetWaypointX(41), GetWaypointY(41), 41.0, 0.0);
        AudioEvent("EarthquakeCast", 41);
        AudioEvent("EnergyBoltCast", 41);
        unit = CreateObject("Maiden", 41);
        Frozen(unit, 1);
        SetCallback(unit, 9, ThunderHurt);
        DeleteObjectTimer(unit, 1);
        SetOwner(owner, unit);
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void ThunderHurt()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 99, 9);
        Enchant(other, "ENCHANT_CHARMING", 0.8);
    }
}

void Boss2NormalAttack()
{
    float pos_x, pos_y;
    int ptr;

    pos_x = UnitRatioX(self, other, 20.0);
    pos_y = UnitRatioY(self, other, 20.0);
    MoveWaypoint(95, GetObjectX(self) - pos_x, GetObjectY(self) - pos_y);
    AudioEvent("BeholderMove", 95);
    ptr = CreateObject("Maiden", 95);
    CreateObject("GameBall", 95);
    CreateObject("InvisibleLightBlueHigh", 95);
    CreateObject("InvisibleLightBlueHigh", 95);
    SetUnitFlags(ptr + 1, GetUnitFlags(ptr + 1) ^ 0x40);
    Raise(ptr + 2, pos_x);
    Raise(ptr + 3, pos_y);
    LookWithAngle(ptr, 0);
    Enchant(ptr, "ENCHANT_HASTED", 0.0);
    Frozen(ptr, 1);
    SetCallback(ptr, 9, WhiteCrystalExplosion);
    FrameTimerWithArg(1, ptr, ShootingWhiteCrystal);
    CreatureFollow(self, other);
    AggressionLevel(self, 1.0);
}

void Boss2SplashSkill()
{
    int k;

    CastSpellObjectObject("SPELL_TURN_UNDEAD", self, self);
    for (k = 9 ; k >= 0 ; k --)
    {
        if (CurrentHealth(player[k]))
        {
            if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(player[k]), GetObjectY(player[k])) < 175.0)
                Damage(player[k], self, 100, 17);
        }
    }
}

void HorrendousDash()
{
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 10.0);
            MoveWaypoint(95, GetObjectX(other) - UnitRatioX(other, self, 30.0), GetObjectY(other) - UnitRatioY(other, self, 30.0));
            AudioEvent("BlindOff", 95);
            Effect("LIGHTNING", GetObjectX(self), GetObjectY(self), GetWaypointX(95), GetWaypointY(95));
            Effect("TELEPORT", GetWaypointX(95), GetWaypointY(95), 0.0, 0.0);
            MoveObject(self, GetWaypointX(95), GetWaypointY(95));
            CreatureFollow(self, other);
            AggressionLevel(self, 1.0);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(60, GetTrigger(), ResetUnitSight);
        }
    }
}

void ThirdBossSkills()
{
    int rnd, ptr;
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            rnd = Random(0, 5);
            Enchant(self, "ENCHANT_ETHEREAL", 7.0);
            if (!rnd)
                CastFireway();
            else if (rnd == 1)
            {
                CastSpellObjectLocation("SPELL_SUMMON_EMBER_DEMON", self, GetObjectX(self) - UnitRatioX(self, other, 30.0), GetObjectY(self) - UnitRatioY(self, other, 30.0));
                CastSpellObjectObject("SPELL_SHIELD", self, self);
            }
            else if (rnd == 2)
                FireSpinShot();
            else if (rnd == 3)
                FallenMeteor();
            else
                CastSpellObjectObject("SPELL_FIREBALL", self, other);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(60, GetTrigger(), ResetUnitSight);
        }
    }
}

void CastFireway()
{
    float pos_x = UnitRatioX(self, other, 23.0), pos_y = UnitRatioY(self, other, 23.0);
    int ptr = CreateObject("InvisibleLightBlueHigh", 9);
    CreateObject("InvisibleLightBlueHigh", 9);

    SetOwner(self, ptr);
    Raise(ptr, pos_x);
    Raise(ptr + 1, pos_y);
    MoveWaypoint(9, GetObjectX(self) - pos_x, GetObjectY(self) - pos_y);
}

void StrightFireway(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), fire;

    if (count < 24)
    {
        MoveWaypoint(9, GetObjectX(ptr), GetObjectY(ptr));
        Effect("SPARK_EXPLOSION", GetWaypointX(9), GetWaypointY(9), 0.0, 0.0);
        AudioEvent("BurnCast", 9);
        fire = CreateObject("LargeFlame", 9);
        Enchant(fire, "ENCHANT_FREEZE", 0.0);
        SetOwner(owner, fire);
        FrameTimerWithArg(65, fire, RemoveUnit);
        MoveObject(ptr, GetObjectX(ptr) - GetObjectZ(ptr), GetObjectY(ptr) - GetObjectZ(ptr + 1));
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, StrightFireway);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void FireSpinShot()
{
    int ptr;
    MoveWaypoint(253, GetObjectX(self), GetObjectY(self));
    ptr = CreateObject("InvisibleLightBlueHigh", 253);
    SetOwner(self, ptr);
    Enchant(self, "ENCHANT_FREEZE", 0.0);
    FrameTimerWithArg(1, ptr, RiseupFlameTonado);
}

void RiseupFlameTonado(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), k, unit;
    float size = 34.0;

    if (count < 18 && CurrentHealth(owner))
    {
        unit = CreateObject("InvisibleLightBlueHigh", 253) + 1;
        Delete(unit - 1);
        for (k = 0 ; k < 6 ; k ++)
        {
            MoveWaypoint(253, GetObjectX(ptr) + MathSine(k * 20 + 90, size), GetObjectY(ptr) + MathSine(k * 20, size));
            Frozen(CreateObject("CarnivorousPlant", 253), 1);
            DeleteObjectTimer(CreateObject("Explosion", 253), 9);
            DeleteObjectTimer(unit + (k * 2), 1);
            SetCallback(unit + (k * 2), 9, FireSpinTouch);
            SetOwner(owner, unit + (k * 2));
            size *= 2.0;
        }
        AudioEvent("FireballCast", 253);
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, RiseupFlameTonado);
    }
    else
    {
        Delete(ptr);
        if (CurrentHealth(owner))
        {
            EnchantOff(owner, "ENCHANT_FREEZE");
        }
    }
}

void FireSpinTouch()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 150, 1);
        Enchant(other, "ENCHANT_CHARMING", 1.0);
    }
}

void FallenMeteor()
{
    int ptr;

    MoveWaypoint(253, GetObjectX(other), GetObjectY(other));
    ptr = CreateObject("InvisibleLightBlueHigh", 253);
    Raise(CreateObject("Flame", 253), 200.0);
    SetOwner(self, ptr + 1);
    AudioEvent("MeteorShowerCast", 253);
    FrameTimerWithArg(21, ptr, MeteorHitAtTarget);
}

void MeteorHitAtTarget(int ptr)
{
    int owner = GetOwner(ptr + 1), unit;

    if (CurrentHealth(owner))
    {
        MoveWaypoint(253, GetObjectX(ptr), GetObjectY(ptr));
        unit = CreateObject("TargetBarrel1", 253);
        SetOwner(owner, CreateObject("TitanFireball", 253));
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void KeeperWaepon()
{
    int rnd;
    if (CurrentHealth(other))
    {
        if (!HasEnchant(self, "ENCHANT_ETHEREAL"))
        {
            Enchant(self, "ENCHANT_ETHEREAL", 7.0);
            rnd = Random(0, 5);
            if (!rnd)
                CastToxicCloud();
            else if (rnd == 1)
                CastOnikiri();
            else if (rnd == 2)
                CastTripleShot();
            else if (rnd == 3)
                CastPoisonBall();
            else
                FrameTimerWithArg(30, GetTrigger(), TeleportInLabyrinth);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(50, GetTrigger(), ResetUnitSight);
        }
    }
}

void CastToxicCloud()
{
    int ptr;

    MoveWaypoint(253, GetObjectX(self), GetObjectY(self));
    ptr = CreateObject("InvisibleLightBlueHigh", 253);
    SetOwner(self, ptr);

    Chat(self, "내 독에 범벅이 되어 죽어라-!");
}

void ToxicSpin(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), k, unit;
    float size = 65.0;

    if (count < 15 && CurrentHealth(owner))
    {
        unit = CreateObject("InvisibleLightBlueHigh", 253) + 1;
        Delete(unit - 1);
        for (k = 0 ; k < 5 ; k ++)
        {
            MoveWaypoint(253, GetObjectX(ptr) + MathSine(k * 24 + 90, size), GetObjectY(ptr) + MathSine(k * 24, size));
            SetOwner(owner, CreateObject("Wizard", 253));
            CastSpellObjectObject("SPELL_TOXIC_CLOUD", unit + k, unit + k);
            size *= 2.0;
        }
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, ToxicSpin);
    }
    else
        Delete(ptr);
}

void CastOnikiri()
{
    float pos_x = UnitRatioX(self, other, 25.0), pos_y = UnitRatioY(self, other, 25.0);
    int ptr;

    MoveWaypoint(253, GetObjectX(self) - pos_x, GetObjectY(self) - pos_y);
    ptr = CreateObject("InvisibleLightBlueHigh", 253);
    CreateObject("InvisibleLightBlueHigh", 253);
    CreateObject("StormCloud", 253);
    SetOwner(self, ptr);
    Raise(ptr, pos_x);
    Raise(ptr + 1, pos_y);
    FrameTimerWithArg(1, ptr, OniCharge);
    Chat(self, "도깨비 참수-!");
}

void OniCharge(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), unit;

    if (CurrentHealth(owner) && count < 16)
    {
        MoveObject(ptr, GetObjectX(ptr) - GetObjectZ(ptr), GetObjectY(ptr) - GetObjectZ(ptr + 1));
        MoveObject(ptr + 2, GetObjectX(ptr), GetObjectY(ptr));
        MoveWaypoint(253, GetObjectX(ptr), GetObjectY(ptr));
        AudioEvent("ElectricalArc1", 253);
        AudioEvent("EnergyBoltSustain", 253);
        unit = CreateObject("CarnivorousPlant", 253);
        SetOwner(owner, unit);
        Frozen(unit, 1);
        DeleteObjectTimer(unit, 1);
        SetCallback(unit, 9, OnikiriTouch);
        Effect("VIOLET_SPARKS", GetWaypointX(253), GetWaypointY(253), 0.0, 0.0);
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, OniCharge);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
        Delete(ptr + 2);
    }
}

void OnikiriTouch()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 150, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.9);
    }
}

void CastTripleShot()
{
    float pos_x = UnitRatioX(self, other, 23.0), pos_y = UnitRatioY(self, other, 23.0);
    int ptr;

    MoveWaypoint(253, GetObjectX(self), GetObjectY(self));
    ptr = CreateObject("InvisibleLightBlueHigh", 253);
    CreateObject("InvisibleLightBlueHigh", 253);
    Raise(ptr, pos_x);
    Raise(ptr + 1, pos_y);
    SetOwner(self, ptr);
    Enchant(self, "ENCHANT_FREEZE", 0.0);
    FrameTimerWithArg(1, ptr, TripleArrow);
}

void TripleArrow(int ptr)
{
    float pos_x = GetObjectZ(ptr), pos_y = GetObjectZ(ptr + 1);
    int owner = GetOwner(ptr), count = GetDirection(ptr), k, unit;

    if (CurrentHealth(owner) && count < 32)
    {
        MoveWaypoint(253, GetObjectX(ptr) + (1.0 / 1.6 * pos_y) - pos_x, GetObjectY(ptr) - (1.0 / 1.6 * pos_x) - pos_y);
        unit = CreateObject("InvisibleLightBlueHigh", 253) + 1;
        Delete(unit - 1);
        for (k = 0 ; k < 9 ; k ++)
        {
            pos_x = GetRatioUnitWpX(ptr, 253, 23.0);
            pos_y = GetRatioUnitWpY(ptr, 253, 23.0);
            MoveWaypoint(253, GetObjectX(ptr) - (1.0 / 8.0 * pos_y) - pos_x, GetObjectY(ptr) + (1.0 / 8.0 * pos_x) - pos_y);
            SetOwner(owner, CreateObject("CherubArrow", 253));
            LookAtObject(unit + k, ptr);
            LookWithAngle(unit + k, GetDirection(unit + k) + 128);
            PushObject(unit + k, 27.0, GetObjectX(ptr), GetObjectY(ptr));
        }
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, TripleArrow);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void TeleportInLabyrinth(int unit)
{
    int wp = Random(267, 276);

    if (CurrentHealth(unit))
    {
        Effect("TELEPORT", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        MoveObject(unit, GetWaypointX(wp), GetWaypointY(wp));
        Effect("TELEPORT", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
    }
}

void CastPoisonBall()
{
    float pos_x = UnitRatioX(self, other, 20.0), pos_y = UnitRatioY(self, other, 20.0);
    int ptr;

    MoveWaypoint(253, GetObjectX(self) - pos_x, GetObjectY(self) - pos_y);
    ptr = CreateObject("InvisibleLightBlueHigh", 253);
    CreateObject("InvisibleLightBlueHigh", 253);
    Frozen(CreateObject("Maiden", 253), 1);
    Frozen(CreateObject("GreenOrb", 253), 1);
    Raise(ptr, pos_x);
    Raise(ptr + 1, pos_y);
    SetOwner(self, ptr + 2);
    ObjectOff(ptr + 2);
    SetCallback(ptr + 2, 9, EnemyPoisoned);
    FrameTimerWithArg(1, ptr, FlyPoisonBall);
}

void FlyPoisonBall(int ptr)
{
    int owner = GetOwner(ptr + 2), count = GetDirection(ptr);

    if (count < 32)
    {
        MoveObject(ptr + 2, GetObjectX(ptr + 2) - GetObjectZ(ptr), GetObjectY(ptr + 2) - GetObjectZ(ptr + 1));
        MoveObject(ptr + 3, GetObjectX(ptr + 2), GetObjectY(ptr + 2));
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, FlyPoisonBall);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
        if (CurrentHealth(ptr + 2))
            Delete(ptr + 2);
        Delete(ptr + 3);
    }
}

void EnemyPoisoned()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        if (!HasEnchant(other, "ENCHANT_AFRAID") && IsObjectOn(other))
        {
            Damage(other, owner, 30, 5);
            Enchant(other, "ENCHANT_AFRAID", 10.0);
            FrameTimerWithArg(1, GetCaller(), PlayerPoisoned);
        }
    }
    else if (!GetCaller())
        Delete(self);
}

void PlayerPoisoned(int unit)
{
    if (CurrentHealth(unit) && HasEnchant(unit, "ENCHANT_AFRAID"))
    {
        MoveWaypoint(253, GetObjectX(unit), GetObjectY(unit));
        DeleteObjectTimer(CreateObject("GreenSmoke", 253), 9);
        Damage(unit, MasterUnit(), 15, 5);
        FrameTimerWithArg(20, unit, PlayerPoisoned);
    }
}

void JandorWeapon()
{
    if (CurrentHealth(self))
    {
        if (CurrentHealth(other))
        {
            Effect("SENTRY_RAY", GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other));
            Damage(other, self, 35, 16);
        }
        if (!HasEnchant(self, "ENCHANT_DETECTING"))
        {
            Enchant(self, "ENCHANT_DETECTING", 0.0);
            FrameTimerWithArg(22, GetTrigger(), ResetUnitSight);
        }
    }
}

int GetNearbyPlayer(int unit)
{
    float r, cmp = 9999.0;
    int k, res = -1;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (CurrentHealth(player[k]))
        {
            r = Distance(GetObjectX(unit), GetObjectY(unit), GetObjectX(player[k]), GetObjectY(player[k]));
            if (r < cmp)
            {
                res = k;
                cmp = r;
                continue;
            }
        }
    }
    return res;
}

void LoopDetectPlayers(int unit)
{
    int target, idx;

    if (CurrentHealth(unit))
    {
        if (!HasEnchant(unit, "ENCHANT_VILLAIN"))
        {
            target = GetNearbyPlayer(unit);
            if (target >= 0)
            {
                CreatureFollow(unit, player[target]);
                AggressionLevel(unit, 1.0);
            }
            Enchant(unit, "ENCHANT_VILLAIN", 1.8);
        }
        idx = GetDirection(unit + 2);
        if (GetDirection(unit) / 32 != idx)
        {
            MoveObject(InitStatues(idx), GetWaypointX(idx + 259), GetWaypointY(idx + 259));
            LookWithAngle(unit + 2, GetDirection(unit) / 32);
        }
        MoveObject(InitStatues(GetDirection(unit + 2)), GetObjectX(unit), GetObjectY(unit));
        FrameTimerWithArg(1, unit, LoopDetectPlayers);
    }
}

void RemoveUnit(int unit)
{
    Delete(unit);
}

int CheckOwner(int unit)
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (IsOwnedBy(unit, player[k]))
            return k;
    }
    return -1;
}

void GoLastTransmission()
{
    if (HasClass(other, "PLAYER") && CurrentHealth(other))
    {
        MoveObject(other, GetWaypointX(3), GetWaypointY(3));
        AudioEvent("BlindOff", 3);
        DeleteObjectTimer(CreateObject("BlueRain", 3), 30);
        Print("오르비스 탑 " + IntToString(Floor_lv + 1) + "층");
    }
}

void GoShop()
{
    if (HasClass(other, "PLAYER") && CurrentHealth(other))
    {
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        MoveObject(other, GetWaypointX(292), GetWaypointY(292));
        AudioEvent("BlindOff", 292);
        Chat(other, "이곳은 상점입니다-!");
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
    }
}

void BackToBasecamp()
{
    if (HasClass(other, "PLAYER") && CurrentHealth(other))
    {
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        MoveObject(other, GetWaypointX(333), GetWaypointY(333));
        AudioEvent("BlindOff", 333);
        Chat(other, "상점에서 나갔습니다");
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
    }
}

void BackToStartLocation()
{
    int ptr;

    if (!HasEnchant(other, "ENCHANT_BURNING"))
    {
        MoveWaypoint(9, GetObjectX(other), GetObjectY(other));
        ptr = CreateObject("InvisibleLightBlueHigh", 9);
        CreateObject("VortexSource", 9);
        Raise(ptr, ToFloat(GetCaller()));
        CheckTeleportLoop(ptr);
        Print("시작위치로 공간이동을 합니다, 취소하려면 비콘에서 벗어나십시오");
        Enchant(other, "ENCHANT_BURNING", 0.0);
    }
}

void CheckTeleportLoop(int ptr)
{
    int count = GetDirection(ptr), unit;

    if (count < 90)
    {
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, CheckTeleportLoop);
    }
    else
    {
        unit = ToInt(GetObjectZ(ptr));
        if (CurrentHealth(unit) && HasEnchant(unit, "ENCHANT_BURNING"))
        {
            EnchantOff(unit, "ENCHANT_BURNING");
            MoveObject(unit, GetWaypointX(8), GetWaypointY(8));
            CrossLightningFx();
            AudioEvent("BlindOff", 8);
            AudioEvent("LightningBolt", 8);
        }
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void CrossLightningFx()
{
    Effect("LIGHTNING", GetWaypointX(4), GetWaypointY(4), GetWaypointY(5), GetWaypointY(5));
    Effect("LIGHTNING", GetWaypointX(5), GetWaypointY(5), GetWaypointY(7), GetWaypointY(7));
    Effect("LIGHTNING", GetWaypointX(6), GetWaypointY(6), GetWaypointY(7), GetWaypointY(7));
    Effect("LIGHTNING", GetWaypointX(4), GetWaypointY(4), GetWaypointY(6), GetWaypointY(6));
    Effect("LIGHTNING", GetWaypointX(4), GetWaypointY(4), GetWaypointY(7), GetWaypointY(7));
    Effect("LIGHTNING", GetWaypointX(6), GetWaypointY(6), GetWaypointY(5), GetWaypointY(5));
}

void RemovePlayerInven(int plr)
{
    int k, pic = plr * 100;
    for (k = 99 ; k >= 0 ; k--)
    {
        if (IsObjectOn(P_inven[pic + k]))
            Delete(P_inven[pic + k]);
    }
}

void GiveAllToPlayer(int plr)
{
    int k, pic = plr * 100;

    for (k = 99 ; k >= 0 ; k --)
    {
        if (IsObjectOn(P_inven[pic + k]))
            Pickup(player[plr], P_inven[pic + k]);
    }
}

void GetInventory(int plr)
{
    int unit = GetLastItem(player[plr]), k, pic = plr * 100;

    for (k = 99 ; k >= 0 && IsObjectOn(unit) ; k --)
    {
        P_inven[pic + k] = unit;
        unit = GetPreviousItem(unit);
    }
}

void HideDropedItem(int plr)
{
    int pic = plr * 100, k;
    for (k = 99 ; k >= 0 ; k --)
    {
        if (IsObjectOn(P_inven[pic + k]))
            MoveObject(P_inven[pic + k], GetWaypointX(316 + plr), GetWaypointY(316 + plr));
    }
}

int PlayerClassCheckWindboostSkillFlag(int plr)
{
    return player[plr + 10] & 2;
}

void PlayerClassSetWindboostSkillFlag(int plr)
{
    player[plr + 10] ^= 2;
}

int PlayerClassCheckDeathFlag(int plr)
{
    return player[plr + 10] & 0x80000000;
}

void PlayerClassSetDeathFlag(int plr)
{
    player[plr + 10] ^= 0x80000000;
}

int PlayerClassOnInit(int plr, int pUnit)
{
    player[plr] = pUnit;
    player[plr + 10] = 1;

    ChangeGold(pUnit, -GetGold(pUnit));
    PlrSkill[plr] = 0;

    return plr;
}

void RegistPlayer()
{
    int k, plr;

    while (1)
    {
        if (CurrentHealth(other))
        {
            if (MaxHealth(other) == 150)
            {
                plr = CheckPlayer();
                for (k = 9 ; k >= 0 && plr < 0 ; k --)
                {
                    if (!MaxHealth(player[k]))
                    {
                        plr = PlayerClassOnInit(k, GetCaller());
                        break;
                    }
                }
                if (plr >= 0)
                {
                    PlayerJoin(plr);
                    break;
                }
            }
            else
                OnlyWarrior();
        }
        JoinErr();
        break;
    }
}

void PlayerJoin(int plr)
{
    Enchant(player[plr], "ENCHANT_ANCHORED", 0.0);
    MoveObject(player[plr], GetWaypointX(1), GetWaypointY(1));
    if (PlayerClassCheckDeathFlag(plr))
        PlayerClassSetDeathFlag(plr);
    if (Plr_flag[plr] & 2)
    {
        GiveAllToPlayer(plr);
        Plr_flag[plr] = Plr_flag[plr] ^ 2;
    }
    if (PlrSkill[plr])
        Enchant(player[plr], "ENCHANT_CROWN", 0.0);
    Effect("TELEPORT", GetWaypointX(333), GetWaypointY(333), 0.0, 0.0);
    DeleteObjectTimer(CreateObject("BlueRain", 333), 35);
    AudioEvent("BlindOff", 333);
}

void OnlyWarrior()
{
    UniPrint(OTHER, "죄송합니다, 이 맵은 전사만 입장을 허용합니다");
}

void JoinErr()
{
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    MoveObject(other, GetWaypointX(2), GetWaypointY(2));
    UniPrint(OTHER, "이 맵은 현재 녹스 실행파일에서 지원되지 않습니다");
}

int CheckPlayer()
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (IsCaller(player[k]))
            return k;
    }
    return -1;
}

void PlayerClassOnAlive(int plr, int pUnit)
{
    if (UnitCheckEnchant(pUnit, GetLShift(ENCHANT_SNEAK)))
    {
        EnchantOff(pUnit, EnchantList(ENCHANT_SNEAK));
        RemoveTreadLightly(pUnit);
        if (PlayerClassCheckWindboostSkillFlag(plr))
            WindBooster(pUnit);
    }
    else if (CheckPlayerInput(pUnit) == 48)
        BringYourGuardian(plr);
}

void PlayerClassOnDeath(int plr)
{ }

void PlayerClassOnExit(int plr)
{
    if (MaxHealth(Jandors[plr]))
    {
        Delete(Jandors[plr]);
        Delete(Jandors[plr] + 1);
        Jandors[plr] = 0;
    }
    player[plr] = 0;
    player[plr + 10] = 0;
}

void PlayerClassOnLoop()
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        while (true)
        {
            if (MaxHealth(player[i]))
            {
                if (GetUnitFlags(player[i]) & 0x40)
                    1;
                else if (CurrentHealth(player[i]))
                {
                    PlayerClassOnAlive(i, player[i]);
                    break;
                }
                else
                {
                    if (PlayerClassCheckDeathFlag(i))
                        break;
                    else
                    {
                        PlayerClassSetDeathFlag(i);
                        PlayerClassOnDeath(i);
                    }
                    break;
                }
            }
            if (player[i + 10])
                PlayerClassOnExit(i);
            break;
        }
    }
    FrameTimer(1, PlayerClassOnLoop);
}

void BringYourGuardian(int plr)
{
    if (!HasEnchant(player[plr], "ENCHANT_PROTECT_FROM_MAGIC"))
    {
        Enchant(player[plr], "ENCHANT_PROTECT_FROM_MAGIC", 3.0);
        if (CurrentHealth(Jandors[plr]))
        {
            MoveObject(Jandors[plr], GetObjectX(player[plr]) + UnitAngleCos(player[plr], 11.0), GetObjectY(player[plr]) + UnitAngleSin(player[plr], 11.0));
            Effect("TELEPORT", GetObjectX(Jandors[plr]), GetObjectY(Jandors[plr]), 0.0, 0.0);
            Chat(Jandors[plr], "소환수가 로드됨(쿨다운 3초)");
        }
    }
}

void WindBooster(int pUnit)
{
    PushObjectTo(pUnit, UnitAngleCos(pUnit, 85.0), UnitAngleSin(pUnit, 85.0));
    Enchant(pUnit, EnchantList(ENCHANT_RUN), 0.2);
    Effect("RICOCHET", GetObjectX(pUnit), GetObjectY(pUnit), 0.0, 0.0);
}

void EmptyInventory(int unit)
{
    while (GetLastItem(unit))
        Delete(GetLastItem(unit));
}

string FxStatue(int num)
{
    string table[] = {
        "MovableStatue2B", "MovableStatue2A", "MovableStatue2H", "MovableStatue2G",
        "MovableStatue2F", "MovableStatue2E", "MovableStatue2D", "MovableStatue2C" };

    return table[num];
}

int InitStatues(int num)
{
    int k, arr[8];

    if (!arr[0])
    {
        for (k = 0 ; k < 8 ; k ++)
        {
            arr[k] = CreateObject(FxStatue(k), 259 + k);
            SetUnitFlags(arr[k], GetUnitFlags(arr[k]) ^ 0x40);
            Enchant(arr[k], "ENCHANT_FREEZE", 0.0);
        }
        return 0;
    }
    return arr[num];
}

float GetRatioUnitWpX(int unit, int wp, float size)
{
    return (GetObjectX(unit) - GetWaypointX(wp)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetWaypointX(wp), GetWaypointY(wp));
}

float GetRatioUnitWpY(int unit, int wp, float size)
{
    return (GetObjectY(unit) - GetWaypointY(wp)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetWaypointX(wp), GetWaypointY(wp));
}

void InitRainyDay()
{
    CosTable(-1);
    SinTable(-1);
    FrameTimerWithArg(1, 278, SpawnRainy);
    FrameTimerWithArg(2, 279, SpawnRainy);
    FrameTimerWithArg(3, 280, SpawnRainy);
    FrameTimerWithArg(4, 281, SpawnRainy);
    FrameTimerWithArg(5, 282, SpawnRainy);
    FrameTimerWithArg(6, 283, SpawnRainy);
    FrameTimer(30, EndMission);
    ObjectOff(self);
}

void EndMission()
{
    Effect("WHITE_FLASH", GetWaypointX(258), GetWaypointY(258), 0.0, 0.0);
    PrintToAll("미션완료!, 오르비스 탑의 꼭대기 층에 도달하셨습니다");
}

void SpawnRainy(int wp)
{
    int ptr = CreateObject("InvisibleLightBlueLow", wp) + 1, k;

    for (k = 0 ; k < 30 ; k += 1)
    {
        ObjectOff(CreateObject("CorpseRightUpperArmE", wp));
    }
    FrameTimerWithArg(1, ptr, RainDrop);
}

void RainDrop(int ptr)
{
    int idx, rnd;
    float x, y;

    if (IsObjectOn(ptr - 1))
    {
        idx = GetDirection(ptr - 1);
        x = GetObjectX(ptr + idx);
        y = GetObjectY(ptr + idx);
        rnd = Random(0, 359);
        MoveObject(ptr + idx, GetObjectX(ptr - 1) + MathSine(rnd + 90, RandomFloat(20.0, 600.0)), GetObjectY(ptr - 1) + MathSine(rnd, RandomFloat(20.0, 600.0)));
        if (!IsVisibleTo(ptr - 1, ptr + idx))
            MoveObject(ptr + idx, x, y);
        Raise(ptr + idx, 280.0);
        LookWithAngle(ptr - 1, (idx + 1) % 30);
        FrameTimerWithArg(1, ptr, RainDrop);
    }
}

float CosTable(int num)
{
    float arr[360];
    int k;
    if (num < 0)
    {
        for (k = 359 ; k >= 0 ; k -= 1)
            arr[k] = MathSine(k + 90, 1.0);
        return ToFloat(0);
    }
    return arr[num];
}

float SinTable(int num)
{
    float arr[360];
    int k;
    if (num < 0)
    {
        for (k = 359 ; k >= 0 ; k -= 1)
            arr[k] = MathSine(k, 1.0);
        return ToFloat(0);
    }
    return arr[num];
}

void StrGoShop()
{
	int arr[35], i;
	string name = "ManaBombOrb";

	arr[0] = 1613226512; arr[1] = 326735; arr[2] = 2614268; arr[3] = 272697344; arr[4] = 37757952; arr[5] = 302055424; arr[6] = 545915008; arr[7] = 538050590; arr[8] = 2029977602; arr[9] = 1224769555; 
	arr[10] = 150997091; arr[11] = 536871200; arr[12] = 1082132488; arr[13] = 280656; arr[14] = 1610649609; arr[15] = 263423; arr[16] = 37888033; arr[17] = 4463680; arr[18] = 1660813312; arr[19] = 263; 
	arr[20] = 570564608; arr[21] = 2088960; arr[22] = 132136960; arr[23] = 17302014; arr[24] = 134217761; arr[25] = 135266814; arr[26] = 33587464; arr[27] = 2013270081; arr[28] = 67141647; arr[29] = 1077969928; 
	arr[30] = 260577409; arr[31] = 4194312; arr[32] = 1069676546; arr[33] = 67125248; arr[34] = 537132032; 
	while(i < 35)
	{
		drawStrGoShop(arr[i], name);
		i ++;
	}
}

void drawStrGoShop(int arg_0, string name)
{
	int count, i;
	float pos_x, pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(290);
		pos_y = GetWaypointY(290);
	}
	for (i = 1 ; i > 0 && count < 1085 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 290);
		if (count % 100 == 99)
			MoveWaypoint(290, GetWaypointX(290) - 198.000000, GetWaypointY(290) + 2.000000);
		else
			MoveWaypoint(290, GetWaypointX(290) + 2.000000, GetWaypointY(290));
		count ++;
	}
	if (count >= 1085)
	{
		count = 0;
		MoveWaypoint(290, pos_x, pos_y);
	}
}

string ShopItemDescript(int num)
{
    string table[] = {
        "재생의 팔찌[27000G]: 소유하면 체력회복 속도가 증가됩니다(중복적용 가능)",
        "핵폭발 검[21000G]: 적을 치거나 공격을 막을 때 전방에 있는 적에게 피해를 입힙니다",
        "피의 목걸이[22000G]: 이 목걸이를 지니면 항상 흡혈상태가 유지됩니다, 벗게되면 즉시 무효됩니다",
        "잔도 소환[18000G]: 잔도를 소환합니다, 당신을 따라다니며 서로 다른 공간에 있는경우 L 키로 불러올 수 있습니다",
        "혼돈의 해머[32000G]: 해머를 내리칠 때 마다 전방에 에너지파가 시전됩니다, 닿은 적은 일시적으로 혼란상태가 됩니다",
        "대쉬 배우기[18000G]: 조심스럽게 걷기를 시전하면 대쉬가 발동됩니다, 대쉬는 짧은 거리를 순간적으로 이동합니다",
        "창고 개방열쇠[30000G]: 강력한 갑옷이나 무기가 있는 비밀창고를 개방하기 위한 열쇠를 구입합니다, 창고는 총 3곳입니다",
        "망각의 지팡이[36000G]: 망각의 지팡이를 얻습니다, 이때 자신의 체력이 1000 으로 증가됩니다"};

    return table[num];
}

void PutDesc(int idx)
{
    int unit = CreateObject("FoilCandleLit", 303 + idx);
    CreateObject("InvisibleLightBlueHigh", 303 + idx);
    LookWithAngle(unit + 1, idx);
    RegistItemPickupCallback(unit, DescFunc);
}

void DummyFunction()
{
    //
}

void InitShopItems()
{
    int ptr = CreateObject("Maiden", 294);
    Frozen(CreateObject("BraceletofHealth", 294), 1);

    Frozen(ptr, 1);
    LookWithAngle(ptr, 0);
    SetDialog(ptr, "NORMAL", BuyHealthItem, DummyFunction);

    Frozen(CreateObject("Maiden", 295), 1); //+2
    Enchant(CreateObject("GreatSword", 295), "ENCHANT_FREEZE", 0.0);
    LookWithAngle(ptr + 2, 0);
    SetDialog(ptr + 2, "NORMAL", BuyMagicSword, DummyFunction);

    Frozen(CreateObject("Maiden", 296), 1); //+4
    Frozen(CreateObject("AmuletofManipulation", 296), 1);
    LookWithAngle(ptr + 4, 0);
    SetDialog(ptr + 4, "NORMAL", BuyAmuletOfBlood, DummyFunction);

    Frozen(CreateObject("Maiden", 297), 1); //+6
    Frozen(CreateObject("FieldGuide", 297), 1);
    LookWithAngle(ptr + 6, 0);
    SetDialog(ptr + 6, "NORMAL", BuySummonJando, DummyFunction);

    Frozen(CreateObject("Maiden", 298), 1); //+8
    Frozen(CreateObject("WarHammer", 298), 1);
    LookWithAngle(ptr + 8, 0);
    SetDialog(ptr + 8, "NORMAL", BuySummonJando, DummyFunction);

    Frozen(CreateObject("Maiden", 299), 1); //+10
    Frozen(CreateObject("AbilityBook", 299), 1);
    LookWithAngle(ptr + 10, 0);
    SetDialog(ptr + 10, "NORMAL", AwardDash, DummyFunction);

    Frozen(CreateObject("Maiden", 300), 1); //+12
    Frozen(CreateObject("SilverKey", 300), 1);
    LookWithAngle(ptr + 12, 0);
    SetDialog(ptr + 12, "NORMAL", BuySilverkey, DummyFunction);

    Frozen(CreateObject("Maiden", 301), 1); //+14
    Frozen(CreateObject("OblivionOrb", 301), 1);
    SetDialog(ptr + 14, "NORMAL", BuyOblivion, DummyFunction);

    FrameTimer(1, PutShopItemDescrtions);
    FrameTimerWithArg(120, 0, Garage1Puts);
}

void PutShopItemDescrtions()
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 292), k;

    Raise(ptr, ReadPage);
    DescFunc = ToInt(GetObjectZ(ptr));
    for (k = 0 ; k < 8 ; k ++)
        PutDesc(k);
}

void ReadPage()
{
    int idx = GetDirection(GetTrigger() + 1);

    Print(ShopItemDescript(idx));
    Delete(self);
    Delete(GetTrigger() + 1);
    FrameTimerWithArg(30, idx, PutDesc);
}

void BuyHealthItem()
{
    int count = GetDirection(self), ptr;

    if (count < 5)
    {
        if (GetGold(other) >= 27000)
        {
            MoveWaypoint(254, GetObjectX(self), GetObjectY(self));
            ptr = CreateObject("BraceletofHealth", 254);
            CreateObject("InvisibleLightBlueHigh", 254);
            Raise(ptr + 1, ToFloat(GetCaller()));
            FrameTimerWithArg(1, ptr, DelayFeed);
            FrameTimerWithArg(1, ptr, IncreaseRiseHp);
            LookWithAngle(self, count + 1);
            ChangeGold(other, -27000);
        }
    }
    else
    {
        Chat(self, "현재 본 제품은 품절 상태입니다");
        if (IsObjectOn(GetTrigger() + 1))
            Delete(GetTrigger() + 1);
    }
}

void BuySilverkey()
{
    int count = GetDirection(self), ptr;

    if (count < 3)
    {
        if (GetGold(other) >= 30000)
        {
            MoveWaypoint(254, GetObjectX(self), GetObjectY(self));
            ptr = CreateObject("SilverKey", 254);
            CreateObject("InvisibleLightBlueHigh", 254);
            Raise(ptr + 1, ToFloat(GetCaller()));
            Enchant(ptr, "ENCHANT_FREEZE", 0.0);
            FrameTimerWithArg(1, ptr, DelayFeed);
            ChangeGold(other, -30000);
            LookWithAngle(self, count + 1);
        }
    }
    else
        Print("매진");
}

void BuyMagicSword()
{
    int count = GetDirection(self), ptr;

    if (count < 5)
    {
        if (GetGold(other) >= 21000)
        {
            MoveWaypoint(254, GetObjectX(self), GetObjectY(self));
            ptr = CreateObject("GreatSword", 254);
            CreateObject("InvisibleLightBlueHigh", 254);
            Raise(ptr + 1, ToFloat(GetCaller()));
            Enchant(ptr, "ENCHANT_FREEZE", 0.0);
            FrameTimerWithArg(1, ptr, DelayFeed);
            FrameTimerWithArg(1, ptr, MagicSwordLoop);
            ChangeGold(other, -21000);
            LookWithAngle(self, count + 1);
        }
    }
    else
    {
        Chat(self, "이미 품절된 상품입니다");
        if (IsObjectOn(GetTrigger() + 1))
            Delete(GetTrigger() + 1);
    }
}

void BuyAmuletOfBlood()
{
    int count = GetDirection(self), ptr;

    if (count < 5)
    {
        if (GetGold(other) >= 22000)
        {
            MoveWaypoint(254, GetObjectX(self), GetObjectY(self));
            ptr = CreateObject("AmuletofManipulation", 254);
            CreateObject("InvisibleLightBlueHigh", 254);
            Raise(ptr + 1, ToFloat(GetCaller()));
            Enchant(ptr, "ENCHANT_FREEZE", 0.0);
            FrameTimerWithArg(1, ptr, DelayFeed);
            FrameTimerWithArg(1, ptr, BloodAmuletLoop);
            ChangeGold(other, -22000);
            LookWithAngle(self, count + 1);
        }
    }
}

void BuySummonJando()
{
    int ptr, plr = CheckPlayer();

    if (plr >= 0)
    {
        if (GetGold(other) >= 18000 && !CurrentHealth(Jandors[plr]))
        {
            MoveWaypoint(254, GetObjectX(other), GetObjectY(other));
            ptr = CreateObject("AirshipCaptain", 254);
            CreateObject("InvisibleLightBlueHigh", 254);
            LookWithAngle(ptr + 1, plr);
            SetUnitMaxHealth(ptr, 700);
            RetreatLevel(ptr, 0.0);
            SetOwner(other, ptr);
            SetCallback(ptr, 3, JandorWeapon);
            SetCallback(ptr, 5, SummonedUnitDeaths);
            FrameTimerWithArg(1, ptr, DelayFollow);
            Jandors[plr] = ptr;
            ChangeGold(other, -18000);
        }
        else
            Chat(self, "금액이 부족하거나 이미 필드에 소환된 잔도가 존재하기 때문에 지금은 구입할 수 없습니다");
    }
}

void BuyMagicHammer()
{
    int ptr, count = GetDirection(self);

    if (count < 5)
    {
        if (GetGold(other) >= 32000)
        {
            MoveWaypoint(254, GetObjectX(self), GetObjectY(self));
            ptr = CreateObject("WarHammer", 254);
            CreateObject("InvisibleLightBlueHigh", 254);
            Raise(ptr + 1, ToFloat(GetCaller()));
            Enchant(ptr, "ENCHANT_FREEZE", 0.0);
            FrameTimerWithArg(1, ptr, DelayFeed);
            FrameTimerWithArg(1, ptr, MagicHammerLoop);
            ChangeGold(other, -32000);
            LookWithAngle(self, count + 1);
        }
    }
    else
        Print("이 제품은 품절입니다");
}

void BuyOblivion()
{
    int ptr;

    if (GetGold(other) >= 36000)
    {
        MoveWaypoint(254, GetObjectX(self), GetObjectY(self));
        ptr = CreateObject("OblivionOrb", 254);
        CreateObject("InvisibleLightBlueHigh", 254);
        Raise(ptr + 1, ToFloat(GetCaller()));
        Enchant(ptr, "ENCHANT_FREEZE", 0.0);
        FrameTimerWithArg(1, ptr + 1, RiseHealth);
        FrameTimerWithArg(1, ptr, DelayFeed);
        ChangeGold(other, -36000);
    }
}

void RiseHealth(int ptr)
{
    int unit = ToInt(GetObjectZ(ptr));

    if (CurrentHealth(unit))
    {
        EnchantOff(unit, "ENCHANT_INVULNERABLE");
        EnchantOff(unit, "ENCHANT_SHIELD");
        SetUnitHealth(unit, 1000);
    }
}

void AwardDash()
{
    int plr;

    if (GetGold(other) >= 18000)
    {
        plr = CheckPlayer();
        if (plr >= 0)
        {
            if (!(Plr_flag[plr] & 1))
            {
                UniPrint(OTHER, "대쉬 마술을 구입했습니다, 조심스럽게 걷기를 시전하면 발동됩니다");
                Plr_flag[plr] = Plr_flag[plr] ^ 1;
                MoveWaypoint(253, GetObjectX(other), GetObjectY(other));
                Effect("YELLOW_SPARKS", GetWaypointX(253), GetWaypointY(253), 0.0, 0.0);
                AudioEvent("SpellPopOffBook", 253);
            }
            else
                UniPrint(OTHER, "이미 대쉬 마술을 배웠습니다");
        }
    }
}

void DelayFeed(int ptr)
{
    int target = ToInt(GetObjectZ(ptr + 1));
    if (CurrentHealth(target))
        Pickup(target, ptr);
    else
        Delete(ptr);
    Delete(ptr + 1);
}

void DelayFollow(int unit)
{
    int owner = GetOwner(unit);

    if (CurrentHealth(unit))
    {
        CreatureFollow(unit, owner);
        AggressionLevel(unit, 1.0);
        GiveCreatureToPlayer(owner, unit);
        UniChatMessage(unit, "소환된 유닛, L 키를 누르면 캐릭터 위치로 유닛을 불러올 수 있습니다", 150);
    }
}

void SummonedUnitDeaths()
{
    int plr = GetDirection(GetTrigger() + 1);
    Delete(self);
    Delete(GetTrigger() + 1);
    Jandors[plr] = 0;
}

void IncreaseRiseHp(int unit)
{
    int owner;
    if (IsObjectOn(unit))
    {
        owner = GetOwner(unit);
        if (CurrentHealth(owner))
        {
            RestoreHealth(owner, 2);
        }
        FrameTimerWithArg(9, unit, IncreaseRiseHp);
    }
}

void MagicSwordLoop(int unit)
{
    int owner;

    if (IsObjectOn(unit))
    {
        if (MaxHealth(unit) - CurrentHealth(unit))
        {
            owner = GetOwner(unit);
            if (CurrentHealth(owner) && !HasEnchant(owner, "ENCHANT_ETHEREAL"))
            {
                SplashFrontArea(owner);
                Enchant(owner, "ENCHANT_ETHEREAL", 0.7);
            }
            RestoreHealth(unit, MaxHealth(unit));
        }
        FrameTimerWithArg(1, unit, MagicSwordLoop);
    }
}

void MagicHammerLoop(int unit)
{
    int owner;

    if (IsObjectOn(unit))
    {
        if (MaxHealth(unit) - CurrentHealth(unit))
        {
            owner = GetOwner(unit);
            if (CurrentHealth(owner) && !HasEnchant(owner, "ENCHANT_ETHEREAL"))
            {
                FrameTimerWithArg(1, owner, CastEnergyPar);
                Enchant(owner, "ENCHANT_ETHEREAL", 3.0);
            }
            RestoreHealth(unit, MaxHealth(unit));
        }
        FrameTimerWithArg(1, unit, MagicHammerLoop);
    }
}

void SplashFrontArea(int unit)
{
    int ptr;

    MoveWaypoint(253, GetObjectX(unit) + UnitAngleCos(unit, 34.0), GetObjectY(unit) + UnitAngleSin(unit, 34.0));
    ptr = CreateObject("Maiden", 253);
    DeleteObjectTimer(CreateObject("InvisibleLightBlueHigh", 253), 6);
    DeleteObjectTimer(CreateObject("Explosion", 253), 9);
    Raise(ptr + 1, ToFloat(unit));
    Frozen(ptr, 1);
    DeleteObjectTimer(ptr, 1);
    SetCallback(ptr, 9, SplashDamage);
}

void SplashDamage()
{
    int owner = ToInt(GetObjectZ(GetTrigger() + 1));

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Effect("DAMAGE_POOF", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Damage(other, owner, 140, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.6);
    }
}

void CastEnergyPar(int unit)
{
    int k, ptr;
    float pos_x = UnitAngleCos(unit, 30.0), pos_y = UnitAngleSin(unit, 30.0);
    
    MoveWaypoint(253, GetObjectX(unit) + pos_x, GetObjectY(unit) + pos_y);
    ptr = CreateObject("InvisibleLightBlueHigh", 253) + 1;
    Raise(ptr, ToFloat(unit));
    for (k = 0 ; k < 15 && CheckLimitLine(253) ; k ++)
    {
        AudioEvent("FireballExplode", 253);
        Effect("SPARK_EXPLOSION", GetWaypointX(253), GetWaypointY(253), 0.0, 0.0);
        Frozen(CreateObject("Maiden", 253), 1);
        DeleteObjectTimer(ptr + k, 1);
        SetOwner(ptr - 1, ptr + k);
        SetCallback(ptr + k, 9, EnergyParDamage);
        MoveWaypoint(253, GetWaypointX(253) + pos_x, GetWaypointY(253) + pos_y);
    }
    Delete(ptr - 1);
}

void EnergyParDamage()
{
    int owner = ToInt(GetObjectZ(GetOwner(self)));

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 50, 1);
        Enchant(other, "ENCHANT_CHARMING", 0.7);
    }
}

void BloodAmuletLoop(int unit)
{
    int owner, temp;

	if (IsObjectOn(unit))
	{
		temp = GetOwner(unit);
		if (temp != owner)
		{
			if (CurrentHealth(owner))
				EnchantOff(owner, "ENCHANT_VAMPIRISM");
			owner = temp;
			if (CurrentHealth(owner))
				Enchant(owner, "ENCHANT_VAMPIRISM", 0.0);
		}
		FrameTimerWithArg(1, unit, BloodAmuletLoop);
	}
}

void PlayerSendToStartZone(int plr)
{
    int ptr;

    if (!HasEnchant(player[plr], "ENCHANT_LIGHT"))
    {
        Enchant(player[plr], "ENCHANT_LIGHT", 5.0);
        Chat(player[plr], "시작위치로 귀환중 입니다, 취소하려면 움직이세요");
        MoveWaypoint(253, GetObjectX(player[plr]), GetObjectY(player[plr]));
        AudioEvent("HecubahDieFrame194", 253);
        ptr = CreateObject("InvisibleLightBlueHigh", 253);
        CreateObject("VortexSource", 253);
        LookWithAngle(ptr, plr);
        FrameTimerWithArg(60, ptr, TeleportToHome);
    }
}

void TeleportToHome(int ptr)
{
    int plr = GetDirection(ptr);

    if (CurrentHealth(player[plr]))
    {
        if (Distance(GetObjectX(player[plr]), GetObjectY(player[plr]), GetObjectX(ptr), GetObjectY(ptr)) < 12.0)
        {
            Effect("COUNTERSPELL_EXPLOSION", GetObjectX(player[plr]), GetObjectY(player[plr]), 0.0, 0.0);
            MoveObject(player[plr], GetWaypointX(1), GetWaypointY(1));
            AudioEvent("HecubahDieFrame98", 1);
            Effect("COUNTERSPELL_EXPLOSION", GetObjectX(player[plr]), GetObjectY(player[plr]), 0.0, 0.0);
            Effect("SMOKE_BLAST", GetObjectX(player[plr]), GetObjectY(player[plr]), 0.0, 0.0);
        }
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void HomePortal(int wp)
{
    int unit = CreateObject("WeirdlingBeast", wp), ptr;
    Frozen(CreateObject("MagicMissile", wp), 1);
    ptr = GetMemory(0x750710);
    SetMemory(ptr + 0xc, GetMemory(ptr + 0xc) ^ 0x1);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    ObjectOff(unit);
    SetDialog(unit, "NORMAL", ClickPortal, DummyFunction);
    SetUnitFlags(unit, GetUnitFlags(unit) ^ 0x40);
}

void ClickPortal()
{
    if (!HasEnchant(other, "ENCHANT_LIGHT"))
    {
        Effect("YELLOW_SPARKS", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Enchant(other, "ENCHANT_PROTECT_FROM_POISON", 1.0);
    }
}

int CheckLimitLine(int wp)
{
    float x = GetWaypointX(wp), y = GetWaypointY(wp);
    if (x > 100.0 && y > 100.0 && x < 5710.0 && y < 5710.0)
        return 1;
    return 0;
}

string ArmorTable(int num)
{
    string table[] = {"OrnateHelm", "Breastplate", "PlateArms", "PlateBoots", "PlateLeggings", "SteelShield"};

    return table[num];
}

string WeaponTable(int num)
{
    string table[] = {"GreatSword", "WarHammer", "RoundChakram", "OgreAxe", "Longsword", "Sword", "MorningStar", "BattleAxe"};

    return table[num];
}

void Garage1Puts(int idx)
{
    int k;

    if (idx < 6)
    {
        MoveWaypoint(315, GetWaypointX(313), GetWaypointY(313));
        for (k = 0 ; k < 4 ; k ++)
        {
            Frozen(GarageArmor(ArmorTable(idx), 315, Armor_fire4, Armor_cure3), 1);
            MoveWaypoint(315, GetWaypointX(315) - 23.0, GetWaypointY(315) + 23.0);
        }
        MoveWaypoint(313, GetWaypointX(313) + 23.0, GetWaypointY(313) + 23.0);
        FrameTimerWithArg(1, idx + 1, Garage1Puts);
    }
    else
        Garage2Puts(0);
}

void Garage2Puts(int idx)
{
    int k;

    if (idx < 6)
    {
        MoveWaypoint(315, GetWaypointX(312), GetWaypointY(312));
        for (k = 0 ; k < 4 ; k ++)
        {
            Frozen(GarageArmor(ArmorTable(idx), 315, Armor_blue4, Armor_cure3), 1);
            MoveWaypoint(315, GetWaypointX(315) - 23.0, GetWaypointY(315) + 23.0);
        }
        MoveWaypoint(312, GetWaypointX(312) + 23.0, GetWaypointY(312) + 23.0);
        FrameTimerWithArg(1, idx + 1, Garage2Puts);
    }
    else
        Garage3Puts(0);
}

void Garage3Puts(int idx)
{
    int k;

    if (idx < 4)
    {
        MoveWaypoint(315, GetWaypointX(314), GetWaypointY(314));
        for (k = 0 ; k < 4 ; k ++)
        {
            Frozen(GarageWeapon(WeaponTable(idx), 315, Weapon_fire4, Weapon_vam3), 1);
            MoveWaypoint(315, GetWaypointX(315) - 23.0, GetWaypointY(315) + 23.0);
        }
        MoveWaypoint(314, GetWaypointX(314) + 23.0, GetWaypointY(314) + 23.0);
        FrameTimerWithArg(1, idx + 1, Garage3Puts);
    }
}

int GarageWeapon(string weapon, int wp, int fst, int scd)
{
    int ptr, k;

    if (IsObjectOn(CreateObject(weapon, wp)))
    {
        ptr = GetMemory(0x750710);
        SetMemory(GetMemory(ptr + 0x2b4), GetMemory(0x5BA774));
        SetMemory(GetMemory(ptr + 0x2b4) + 4, GetMemory(0x5ba894));
        if (fst)
            SetMemory(GetMemory(ptr + 0x2b4) + 8, GetMemory(fst));
        if (scd)
            SetMemory(GetMemory(ptr + 0x2b4) + 12, GetMemory(scd));
        for (k = 31 ; k >= 0 ; k --)
                SetMemory(ptr + 0x230 + (k*4), 0x200);
        return GetMemory(ptr + 0x2c);
    }
    return 0;
}

int GarageArmor(string armor, int wp, int fst, int scd)
{
    int ptr, k;
    if (IsObjectOn(CreateObject(armor, wp)))
    {
        ptr = GetMemory(0x750710);
        SetMemory(GetMemory(ptr + 0x2b4), GetMemory(0x5BA804));
        SetMemory(GetMemory(ptr + 0x2b4) + 4, GetMemory(0x5ba894));
        if (fst)
            SetMemory(GetMemory(ptr + 0x2b4) + 8, GetMemory(fst));
        if (scd)
            SetMemory(GetMemory(ptr + 0x2b4) + 12, GetMemory(scd));
        for (k = 31 ; k >= 0 ; k --)
                SetMemory(ptr + 0x230 + (k*4), 0x200);
        return GetMemory(ptr + 0x2c);
    }
    return 0;
}

void SetUnitHealth(int unit, int amount)
{
    Damage(unit, 0, CurrentHealth(unit) - 1, -1);
    Pickup(unit, CreateObject("RottenMeat", 313));
    Damage(unit, 0, CurrentHealth(unit) - amount, -1);
}

void PutGenerators()
{
    int k, base = CreateObjectAt("InvisibleLightBlueLow", LocationX(326), LocationY(326)) + 1;

    Delete(base);
    for (k = 0 ; k < 5 ; k += 1)
    {
        Enchant(CreateObject("NecromancerGenerator", 326), "ENCHANT_INVULNERABLE", 0.0);
        ObjectOff(base + k);
        SetUnitMass(base + k, 22.0);
        TeleportLocationVector(326, 46.0, 46.0);
    }
}

void DetectedSpecificIndex(int curId)
{
    if (!IsMissileUnit(curId))
        return;

    int thingId = GetUnitThingID(curId), owner = GetOwner(curId);

    if (thingId == 1177)
        ChakramEvent(owner, curId);
    else if (thingId == 1179)
        ShurikenEvent(owner, curId);
    else if (thingId == 526)
        HarpoonEvent(owner, curId);
}

void LoopSearchIndex()
{
    int curId, tempId;
    int *ptr = 0x750710;

    ptr = ptr[0];
    while (ptr != NULLPTR)
    {
        tempId = ptr[11];
        if (curId)
        {
            while (curId < tempId)
            {
                curId += 1;
                DetectedSpecificIndex(curId);
            }
            break;
        }
        curId = tempId;
        break;
    }
    FrameTimer(1, LoopSearchIndex);
}

void HarpoonEvent(int owner, int cur)
{
    int ptr;
    float x_vect, y_vect;

    if (CurrentHealth(owner))
    {
        if (HasEnchant(owner, "ENCHANT_CROWN"))
        {
            Delete(cur);
            x_vect = UnitAngleCos(owner, 13.0);
            y_vect = UnitAngleSin(owner, 13.0);
            MoveWaypoint(329, GetObjectX(owner) + x_vect, GetObjectY(owner) + y_vect);
            ptr = CreateObject("InvisibleLightBlueHigh", 329);
            Raise(CreateObject("InvisibleLightBlueHigh", 329), y_vect);
            Frozen(HecubahDeadMotion(329), 1);
            //LookWithAngle(ptr + 2, GetDirection(owner));
            SetOwner(owner, ptr);
            Raise(ptr, x_vect);
            SetCallback(ptr + 2, 9, FlierTouchEvent);
            FrameTimerWithArg(1, ptr, FlyingFlier);
        }
    }
}

int HecubahDeadMotion(int wp)
{
    int unit = CreateObject("Hecubah", wp);

    ObjectOff(unit);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    SetMemory(GetMemory(GetMemory(0x750710) + 0x2ec) + 0x1e0, 38817);
    return unit;
}

void ShurikenEvent(int owner, int cur)
{
    int mis;

    if (CurrentHealth(owner))
    {
        MoveWaypoint(327, GetObjectX(cur), GetObjectY(cur));
        mis = CreateObject("WeakFireball", 327);
        SetOwner(owner, mis);
        PushObjectTo(mis, UnitRatioX(cur, owner, 25.0), UnitRatioY(cur, owner, 25.0));
    }
}

void ChakramEvent(int owner, int cur)
{
    Enchant(cur, "ENCHANT_REFLECTIVE_SHIELD", 0.0);
    Enchant(cur, "ENCHANT_HASTED", 0.0);
}

void SplashDamageOnRange(int owner, int dam, float range, int wp)
{
    int ptr = CreateObject("InvisibleLightBlueHigh", wp) + 1, k;

    SetOwner(owner, ptr - 1);
    MoveObject(ptr - 1, range, GetObjectY(ptr - 1));
    Raise(ptr - 1, ToFloat(dam));
    for (k = 0 ; k < 8 ; k ++)
    {
        DeleteObjectTimer(CreateObject("WeirdlingBeast", wp), 1);
        UnitNoCollide(ptr + k);
        LookWithAngle(ptr + k, k * 32);
        SetOwner(ptr - 1, ptr + k);
        SetCallback(ptr + k, 3, UnitVisibleSplash);
    }
    DeleteObjectTimer(ptr - 1, 2);
}

void UnitVisibleSplash()
{
    int parent;

    if (!HasEnchant(other, "ENCHANT_VILLAIN"))
    {
        parent = GetOwner(self);
        if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) <= GetObjectX(parent))
        {
            Enchant(other, "ENCHANT_VILLAIN", 0.1);
            Damage(other, GetOwner(parent), ToInt(GetObjectZ(parent)), 14);
        }
    }
}

void FlyingFlier(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr);

    if (CurrentHealth(owner) && count < 50)
    {
        if (IsVisibleTo(ptr + 2, ptr))
        {
            MoveObject(ptr + 2, GetObjectX(ptr + 2) + GetObjectZ(ptr), GetObjectY(ptr + 2) + GetObjectZ(ptr + 1));
            LookWithAngle(ptr, count + 1);
        }
        else
        {
            LookWithAngle(ptr, 200);
        }
        FrameTimerWithArg(1, ptr, FlyingFlier);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
        Delete(ptr + 2);
    }
}

void FlierTouchEvent()
{
    int owner = GetOwner(GetTrigger() - 2);

    if (IsAttackedBy(other, owner) && CurrentHealth(other))
    {
        MoveWaypoint(329, GetObjectX(self), GetObjectY(self));
        SplashDamageOnRange(owner, 100, 150.0, 329);
        DeleteObjectTimer(CreateObject("Explosion", 329), 9);
        AudioEvent("PowderBarrelExplode", 329);
        Delete(self);
    }
}

void PutHorrendous()
{
    int ptr = CreateObject("Horrendous", 327);

    Damage(ptr, 0, MaxHealth(ptr) + 1, -1);
    SetDialog(ptr, "NORMAL", HorrendousTalk, Nothing);
    FrameTimerWithArg(23, ptr, DelayUnitDisable);
}

void Nothing()
{
    //
}

void HorrendousTalk()
{
    int plr;
    if (HasEnchant(other, "ENCHANT_VILLAIN"))
    {
        EnchantOff(other, "ENCHANT_VILLAIN");
        if (HasEnchant(other, "ENCHANT_CROWN"))
            Print("구입실패_ 이 능력을 이미 배우셨습니다");
        else if (GetGold(other) >= 20000)
        {
            plr = CheckPlayer();
            if (plr + 1)
            {
                PlrSkill[plr] = 1;
                Enchant(player[plr], "ENCHANT_CROWN", 0.0);
                ChangeGold(other, -20000);
                MoveWaypoint(327, GetObjectX(other), GetObjectY(other));
                GreenSparkFx(327);
                AudioEvent("AwardSpell", 327);
                Print("구입에 성공했습니다, 이제 작살을 시전하면 다른 능력이 발사될 것입니다");
            }
            else
                Print("허용되지 않은 플레이어 입니다");
        }
        else
            Print("구입실패_ 골드가 부족합니다, [2만 골드 필요]");
    }
    else
    {
        Enchant(other, "ENCHANT_VILLAIN", 0.6);
        Print("작살 능력을 강화합니다, 계속하려면 더블클릭 하십시오 [2만 골드 필요]");
    }
}

void DelayUnitDisable(int unit)
{
    ObjectOff(unit);
    Frozen(unit, 1);
}

void GreenSparkFx(int wp)
{
    int ptr = CreateObject("MonsterGenerator", wp);

    Damage(ptr, 0, 10, 100);
    Delete(ptr);
}

void GolemSkill(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr);

    if (CurrentHealth(owner) && count < 50)
    {
        if (Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(ptr + 1), GetObjectY(ptr + 1)) > 30.0)
        {
            MoveObject(ptr, GetObjectX(ptr) - GetObjectZ(ptr), GetObjectY(ptr) - GetObjectZ(ptr + 1));
            LookWithAngle(ptr, count + 1);
        }
        else
        {
            MoveWaypoint(329, GetObjectX(ptr), GetObjectY(ptr));
            CastSpellObjectObject("SPELL_TURN_UNDEAD", ptr, ptr);
            SplashDamageOnRange(owner, 130, 100.0, 329);
            LookWithAngle(ptr, 200);
        }
        FrameTimerWithArg(1, ptr, GolemSkill);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void CureAllYourItems()
{
    int cur, res = 0;

    if (CurrentHealth(other))
    {
        Enchant(other, "ENCHANT_FREEZE", 0.5);
        cur = GetLastItem(other);
        while (IsObjectOn(cur))
        {
            if (MaxHealth(cur) - CurrentHealth(cur))
            {
                RestoreHealth(cur, MaxHealth(cur));
                res ++;
            }
            cur = GetPreviousItem(cur);
        }
        if (res)
            Print("내구도 회복처리 된 인벤토리 개수: " + IntToString(res));
    }
}

void StrOrbisShop()
{
	int arr[36];
	string name = "ManaBombOrb";
	int i = 0;
	arr[0] = 1111474416; arr[1] = 2139097092; arr[2] = 2023886832; arr[3] = 268469265; arr[4] = 1074004546; arr[5] = 1359216704; arr[6] = 8456200; arr[7] = 33628432; arr[8] = 1140604864; arr[9] = 1082393693; 
	arr[10] = 9474032; arr[11] = 33554437; arr[12] = 33696064; arr[13] = 1337985057; arr[14] = 536347712; arr[15] = 1595187202; arr[16] = 537136899; arr[17] = 792648; arr[18] = 176750337; arr[19] = 2114060545; 
	arr[20] = 9251; arr[21] = 2040; arr[22] = 16810122; arr[23] = 1183744; arr[24] = 2145517568; arr[25] = 4211968; arr[26] = 151519233; arr[27] = 1107165184; arr[28] = 538969104; arr[29] = 2097139708; 
	arr[30] = 16908169; arr[31] = 1879181376; arr[32] = 39; arr[33] = 1024; arr[34] = 2130977023; arr[35] = 4097; 
	while(i < 36)
	{
		drawStrOrbisShop(arr[i], name);
		i ++;
	}
}

void drawStrOrbisShop(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(332);
		pos_y = GetWaypointY(332);
	}
	for (i = 1 ; i > 0 && count < 1116 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 332);
		if (count % 100 == 99)
			MoveWaypoint(332, GetWaypointX(332) - 198.000000, GetWaypointY(332) + 2.000000);
		else
			MoveWaypoint(332, GetWaypointX(332) + 2.000000, GetWaypointY(332));
		count ++;
	}
	if (count >= 1116)
	{
		count = 0;
		MoveWaypoint(332, pos_x, pos_y);
	}
}


