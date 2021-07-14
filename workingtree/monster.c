
#include "noxscript\builtins.h"
#include "libs\waypoint.h"
#include "libs\observer.h"
#include "libs\coopteam.h"
#include "libs\spellutil.h"
#include "libs\callmethod.h"
#include "libs\unitutil.h"
#include "libs\unitstruct.h"
#include "libs\typecast.h"
#include "libs\fixtellstory.h"
#include "libs\reaction.h"
#include "libs\mathlab.h"
#include "libs\printutil.h"
#include "libs\playerupdate.h"
#include "libs\wandpatch.h"
#include "libs\network.h"
#include "libs\memutil.h"

#include "libs\fxeffect.h"
#include "libs\buff.h"
#include "libs\playerinfo.h"

int player[20];
int PlrCam[10];
int Gvar_4; //stage_value
int FieldMob[50];
int isGameStandby = 1;

#define OTHER -1
#define SELF -2

#define NULLPTR         0

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
#define ENCHANT_INVULNERABLE        23
#define ENCHANT_TELEKINESIS         24
#define ENCHANT_FREEZE              25
#define ENCHANT_SHIELD              26
#define ENCHANT_REFLECTIVE_SHIELD   27
#define ENCHANT_CHARMING            28
#define ENCHANT_ANTI_MAGIC          29
#define ENCHANT_CROWN               30
#define ENCHANT_SNEAK               31

#define TRUE 1
#define FALSE 0

int DrawMagicIcon(float x, float y)
{
    int unit = CreateObjectAt("AirshipBasketShadow", x, y);
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x04, 1416);
    return unit;
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

int HecubahWithOrbBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 1466458466; arr[2] = 1332245609; arr[3] = 25202; arr[17] = 750; 
		arr[19] = 60; arr[21] = 1065353216; arr[23] = 67584; arr[24] = 1069547520; arr[26] = 4; 
		arr[27] = 5; arr[37] = 1952539972; arr[38] = 1818313320; arr[39] = 1634879084; arr[40] = 1852140903; 
		arr[41] = 116; arr[53] = 1133903872; arr[55] = 12; arr[56] = 18; arr[60] = 1384; 
		arr[61] = 46914560; 
		link = &arr;
	}
	return link;
}

int WizardRedBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1635412311; arr[1] = 1699898482; arr[2] = 100; arr[16] = 80000; arr[17] = 300; 
		arr[18] = 100; arr[19] = 50; arr[21] = 1065353216; arr[23] = 32; arr[24] = 1067869798; 
		arr[26] = 4; arr[27] = 4; arr[53] = 1128792064; arr[54] = 4; 
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
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1069547520; arr[27] = 1; arr[28] = 1106247680; 
		arr[29] = 80; arr[30] = 1101004800; arr[31] = 8; arr[32] = 22; arr[33] = 30; 
		arr[58] = 5546320; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int GoonBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1852796743; arr[17] = 85; arr[19] = 80; arr[21] = 1065353216; arr[23] = 32768; 
		arr[24] = 1066192077; arr[26] = 4; arr[28] = 1106247680; arr[29] = 50; arr[30] = 1092616192; 
		arr[31] = 8; arr[32] = 20; arr[33] = 28; arr[34] = 1; arr[35] = 5; 
		arr[36] = 20; arr[57] = 5548176; arr[58] = 5546608; arr[59] = 5543680; 
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
		arr[16] = 80000; arr[17] = 200; arr[18] = 55; arr[19] = 60; arr[21] = 1065353216; 
		arr[23] = 32768; arr[24] = 1069547520; arr[37] = 1701996870; arr[38] = 1819042146; arr[53] = 1128792064; 
		arr[54] = 4; arr[55] = 20; arr[56] = 30; arr[57] = 5547984; 
		link = &arr;
	}
	return link;
}

int WeirdlingBeastBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811; arr[17] = 85; 
		arr[18] = 50; arr[19] = 55; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1068708659; 
		arr[26] = 4; arr[28] = 1082130432; arr[29] = 40; arr[31] = 2; arr[32] = 8; 
		arr[33] = 16; arr[57] = 5548112; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int BlackWidowBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1667329090; arr[1] = 1684625259; arr[2] = 30575; arr[17] = 130; arr[18] = 45; 
		arr[19] = 85; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1069547520; arr[26] = 4; 
		arr[27] = 3; arr[28] = 1097859072; arr[29] = 100; arr[31] = 8; arr[32] = 13; 
		arr[33] = 21; arr[34] = 1; arr[35] = 5; arr[36] = 20; arr[37] = 1684631635; 
		arr[38] = 1884516965; arr[39] = 29801; arr[53] = 1128792064; arr[55] = 20; arr[56] = 28; 
		arr[59] = 5544896; arr[61] = 45071360; 
		link = &arr;
	}
	return link;
}

int FireSpriteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972; arr[17] = 135; arr[19] = 65; 
		arr[21] = 1065353216; arr[23] = 65536; arr[24] = 1067450368; arr[37] = 1769236816; arr[38] = 1181513062; 
		arr[39] = 1650815593; arr[40] = 7105633; arr[53] = 1133903872; arr[55] = 13; arr[56] = 21; 
		arr[58] = 5545472; 
		link = &arr;
	}
	return link;
}

int AirshipCaptainBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1936877889; arr[1] = 1131440488; arr[2] = 1635020897; arr[3] = 28265; arr[17] = 130; 
		arr[18] = 100; arr[19] = 60; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1070386381; 
		arr[26] = 4; arr[28] = 1077936128; arr[29] = 150; arr[31] = 8; arr[32] = 12; 
		arr[33] = 20; arr[57] = 5547984; arr[58] = 5546320; arr[59] = 5542432; 
		link = &arr;
	}
	return link;
}

int LichBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751345484; arr[17] = 375; arr[19] = 60; arr[21] = 1065353216; arr[23] = 2048; 
		arr[24] = 1067869798; arr[26] = 4; arr[27] = 4; arr[28] = 1106771968; arr[29] = 80; 
		arr[31] = 10; arr[32] = 8; arr[33] = 11; arr[53] = 1128792064; arr[54] = 4; 
		arr[59] = 5542784; arr[60] = 1342; arr[61] = 46909440; 
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
        arr[12] = MonsterFireSpriteProcess; arr[73] = MonsterWizardRedProcess; arr[29] = MonsterAirshipCaptainProcess; arr[80] = LichSubProcess;
        arr[26] = HecubahWithOrbSubProcess;
    }
    if (thingID && arr[key] != 0)
    {
        CallFunctionWithArg(arr[key], unit);
        return TRUE;
    }
    return FALSE;
}

void HecubahWithOrbSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1072064102);
		SetMemory(ptr + 0x224, 1072064102);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 67584);
		SetMemory(GetMemory(ptr + 0x22c), 750);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 750);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HecubahWithOrbBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void LichSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1072064102);
		SetMemory(ptr + 0x224, 1072064102);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 2048);
		SetMemory(GetMemory(ptr + 0x22c), 375);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 375);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, LichBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void MonsterGoonProcess(int unit)
{
    //TODO: Index. 1, ThingName= Goon
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, GoonBinTable());
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(63));       //MimicVoice
        SetUnitMaxHealth(unit, 180);
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
        SetUnitMaxHealth(unit, 225);
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
        SetUnitMaxHealth(unit, 255);
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
        SetUnitMaxHealth(unit, 175);
        SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000);
    }
}

void MonsterWizardRedProcess(int unit)
{
    //TODO: Index. 7, ThingName= WizardRed
    int ptr = UnitToPtr(unit), uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WizardRedBinTable());
        SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0); //Flee Range set to 0
        SetUnitMaxHealth(unit, 325);
        uec = GetMemory(ptr + 0x2ec);
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
        }
    }
}

void MonsterAirshipCaptainProcess(int unit)
{
    //TODO: Index. 8, ThingName= AirshipCaptain
    int ptr = UnitToPtr(unit), uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, AirshipCaptainBinTable());
        SetUnitMaxHealth(unit, 600);
        RetreatLevel(unit, 0.0);
        ResumeLevel(unit, 1.0);
    }
}

void DelayMakeCoopTeam()
{
    MakeCoopTeam();
}

void MapInitialize()
{
    MusicEvent();
    FrameTimer(1, DelayMakeCoopTeam);
    SetMemory(0x5cb394, 0);
    SetMemory(0x5cb3a0, 0);
    SetMemory(0x5cb3b8, 0);
    SetMemory(0x5cb3ac, 0);
    
    Gvar_4 = 0;
    initializeStructPacker();

    //delay
    FrameTimer(1, PutStampString);
    FrameTimer(3, MapDecorations);
    FrameTimer(10, placedArmor);
    FrameTimer(11, placedWeapon);
    FrameTimerWithArg(120, 0, guideMent);

    //loop
    FrameTimer(3, LoopSearchIndex);
    FrameTimer(3, LoopPreservePlayer);
    FrameTimerWithArg(10, 0, LoopUnitAI);
}


void guideMent(int arg_0) {
    if (arg_0 == 0)
        UniPrintToAll("게임방법을 알려드리겠습니다, 방법은 의외로 단순합니다 .");
    else if (arg_0 == 1)
        UniPrintToAll("10 시방향 문을 열고 직진하면 시작 버튼이 있습니다 .");
    else if (arg_0 == 2)
        UniPrintToAll("이 버튼을 누르게 되면 잠시 후 몬스터들이 몰려올 것입니다 .");
    else if (arg_0 == 3)
        UniPrintToAll("몰려 오는 몬스터들을 모두 처치 하면 다음 스테이지로 갈 수 있습니다 . 최종 스테이지: 20 ");
    else if (arg_0 == 4)
        UniPrintToAll("만약 죽게 되면 생존한 플레이어가 그 판을 깰때까지 대기실에서 기다려야 합니다 .");
    else if (arg_0 == 5)
        UniPrintToAll("생존한 플레이어가 아무도 없다면 부활 할 수 없으므로 게임오버가 되겠지요 ...");
    else if (arg_0 == 6)
        UniPrintToAll("별도로 이 맵에는 상점이 존재합니다 . 몬스터를 잡아 돈을 벌고 상점에서 더 강력한 갑옷과 무기를 이용하세요 .");
    if (arg_0 != 7)
        FrameTimerWithArg(90, arg_0 + 1, guideMent);
}
void initializeStructPacker()
{
    structUnitType(-1);
    structUnitHealth(-1);
}

void PlayerCommonEntryEvent()
{
    AppendAllDummyStaffsToWeaponList();
}

static void NetworkUtilClientMain()
{
    PlayerCommonEntryEvent();
}

int CheckPlayer()
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (IsCaller(player[i]))
            return i;
    }
    return -1;
}

void ApplyAllEnchantToPlayer(int pUnit)
{
    SetUnitEnchantCopy(pUnit, 
        GetLShift(ENCHANT_HASTED) | GetLShift(ENCHANT_INFRAVISION) | GetLShift(ENCHANT_PROTECT_FROM_ELECTRICITY) | GetLShift(ENCHANT_PROTECT_FROM_FIRE)
        | GetLShift(ENCHANT_REFLECTIVE_SHIELD) | GetLShift(ENCHANT_VAMPIRISM));
}

void OnPlayerJoin(int plr)
{
    Enchant(player[plr], EnchantList(ENCHANT_ANCHORED), 0.0);
    if (CheckPlayerFlagAllEnchantment(plr))
        ApplyAllEnchantToPlayer(player[plr]);
    if (isGameStandby)
        MoveObject(player[plr], GetWaypointX(11), GetWaypointY(11));
    else
    {
        if (CheckPlayerDeath(plr))
            SetPlayerDeathFlag(plr);
        SetPlayerWatching(player[plr], plr);
        MoveObject(player[plr], GetWaypointX(14), GetWaypointY(14));
    }
}

void OnPlayerJoinFailed()
{
    Enchant(OTHER, EnchantList(ENCHANT_FREEZE), 3.0);
    MoveObject(OTHER, GetWaypointX(12), GetWaypointY(12));
    AudioEvent("PlayerEliminated", 12);
    UniPrint(OTHER, "맵 입장에 실패하였습니다. 지도에 참가중인 유저가 10을 넘었습니다");
    UniPrint(OTHER, "잠시 후 다시 시도해 보세요");
}

int GetAlivePlayer()
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (CurrentHealth(player[i]))
        {
            if (CheckPlayerAlive(i))
                return i;
        }
    }
    return -1;
}

int GetNextAlivePlayer(int index)
{
    int i, cur = index;

    for (i = 9 ; i ; i --)
    {
        cur = (cur + 1) % 10;
        if (CurrentHealth(player[cur]))
        {
            if (CheckPlayerAlive(cur))
                return cur;
        }
    }
    if (CurrentHealth(player[index]))
    {
        if (CheckPlayerAlive(index))
            return index;
    }
    else
        return -1;
}

int SetupPlayerCamera(int plr)
{
    int cam = CreateObjectAt("Maiden", GetObjectX(player[plr]), GetObjectY(player[plr]));

    //TODO
    /*
    plr = cur->dir
    playerUnit = (cur+1)->owner
    watchPlayerIndex = (cur+1)->dir
    */
    SetOwner(player[plr], CreateObjectAt("InvisibleLightBlueLow", GetObjectX(cam), GetObjectY(cam)));
    UnitNoCollide(cam);
    ObjectOff(cam);
    LookWithAngle(cam, plr);
    Damage(cam, 0, MaxHealth(cam) + 1, -1);
    return cam;
}

void FreePlayerCamera(int plr)
{
    if (CurrentHealth(player[plr]))
    {
        if (UnitCheckEnchant(player[plr], GetLShift(ENCHANT_CROWN)))
            EnchantOff(player[plr], EnchantList(ENCHANT_CROWN));
    }
    if (MaxHealth(PlrCam[plr]))
    {
        Delete(PlrCam[plr]);
        Delete(PlrCam[plr] + 1);
    }
}

void SetPlayerWatching(int unit, int plr)
{
    int cam, watchPlayerIndex = GetAlivePlayer();

    if (watchPlayerIndex + 1)
    {
        cam = SetupPlayerCamera(plr);
        if (cam)
        {
            Enchant(unit, EnchantList(ENCHANT_CROWN), 0.0);
            LookWithAngle(cam + 1, watchPlayerIndex);
            PlrCam[plr] = cam;
        }
    }
}

void OnPlayerCameraHandler(int plr)
{
    int cam = PlrCam[plr], target;

    if (MaxHealth(cam))
    {
        target = player[GetDirection(cam + 1)];
        if (CheckWatchFocus(player[plr]))
        {
            PlayerLook(player[plr], cam);
        }
        if (CurrentHealth(target))
        {
            if (ToInt(Distance(GetObjectX(cam), GetObjectY(cam), GetObjectX(target), GetObjectY(target))))
                MoveObject(cam, GetObjectX(target), GetObjectY(target));
            if (CheckPlayerInput(player[plr]) == 7)
            {
                target = GetNextAlivePlayer(GetDirection(cam + 1));
                if (target + 1) //TODO: Change focus
                    LookWithAngle(cam + 1, target);
            }
        }
        else
        {
            target = GetNextAlivePlayer(GetDirection(cam + 1));
            if (target + 1) //TODO: Change focus
                LookWithAngle(cam + 1, target);
            else
                FreePlayerCamera(plr);
        }
    }
}

void SetPlayerDeathFlag(int plr)
{
    player[plr + 10] ^= 0x02;
}

void SetPlayerAliveFlag(int plr)
{
    player[plr + 10] ^= 0x04;
}

void SetPlayerAllEnchantFlag(int plr)
{
    player[plr + 10] ^= 0x08;
}

int CheckPlayerDeath(int plr)
{
    return player[plr + 10] & 0x02;
}

int CheckPlayerAlive(int plr)
{
    return player[plr + 10] & 0x04;
}

int CheckPlayerFlagAllEnchantment(int plr)
{
    return player[plr + 10] & 0x08;
}

void OnPlayerDeath(int plr)
{
    if (CheckPlayerAlive(plr))
        SetPlayerAliveFlag(plr); //TODO: Set to 0
    FreePlayerCamera(plr);
    UniPrint(player[plr], "당신은 죽었습니다... 생존한 유저가 있다면 옵저버 모드로 전환됩니다        ");
    UniPrint(player[plr], "누군가 이번 스테이지를 클리어 하면 다시 부활되니 나가지 말고 기다려 주세요~");
}

int OnPlayerInit(int plr, int pUnit)
{
    player[plr] = pUnit;
    player[plr + 10] = 1;
    ChangeGold(pUnit, -GetGold(pUnit));
    SelfDamageClassEntry(pUnit);
    DiePlayerHandlerEntry(pUnit);
    if (ValidPlayerCheck(pUnit))
    {
        if (pUnit ^ GetHost())
        {
            NetworkUtilClientEntry(pUnit);
        }
        else
        {
            PlayerCommonEntryEvent();
        }
        
    }

    if (UnitCheckEnchant(pUnit, GetLShift(ENCHANT_CROWN)))
        EnchantOff(pUnit, EnchantList(ENCHANT_CROWN));

    return plr;
}

void OnPlayerFastJoin()
{
    int plr = CheckPlayer();

    if (plr + 1)
    {
        MoveObject(OTHER, LocationX(15), LocationY(15));
    }
}

void OnPlayerEntry()
{
    int plr, i;

    if (CurrentHealth(OTHER))
    {
        plr = CheckPlayer();
        for (i = 9 ; i >= 0 && plr < 0 ; i -= 1)
        {
            if (!MaxHealth(player[i]))
            {
                plr = OnPlayerInit(i, GetCaller());
                break;
            }
        }
        if (plr + 1)
            OnPlayerJoin(plr);
        else
            OnPlayerJoinFailed();
    }
}

void PlayerClassMissileWandFix(int plr, int pUnit)
{
    int wand[10];

    if (wand[plr] ^ PlayerGetEquipedWeapon(pUnit))
    {
        if (HasItem(pUnit, wand[plr]))
        {
            if (GetUnitThingID(wand[plr]) == 213)
            {
                Drop(pUnit, wand[plr]);
                SetOwner(pUnit, wand[plr]);
                FrameTimerWithArg(1, wand[plr], DelayGiveToOwner);
            }
        }
        wand[plr] = PlayerGetEquipedWeapon(pUnit);
    }
}

void PlayerClassOnAlive(int plr, int pUnit)
{
    if (UnitCheckEnchant(pUnit, GetLShift(ENCHANT_SNEAK)))
        Windbooster(pUnit);
    if (UnitCheckEnchant(pUnit, GetLShift(ENCHANT_CROWN)))
        OnPlayerCameraHandler(plr);

    PlayerClassMissileWandFix(plr, pUnit);
    ForceChangePlaerUpdateHandler(pUnit);

    int poisonTime[10];

    if (IsPoisonedUnit(pUnit))
    {
        if (CheckPlayerFlagAllEnchantment(plr))
            CastSpellObjectObject("SPELL_CURE_POISON", pUnit, pUnit);
        else if (poisonTime[plr] < 30)
            poisonTime[plr] += IsPoisonedUnit(pUnit);
        else
        {
            Damage(pUnit, 0, IsPoisonedUnit(pUnit), 5);
            poisonTime[plr] = 0;
        }
    }
    else if (poisonTime[plr])
        poisonTime[plr] = 0;

    if (CheckPlayerFlagAllEnchantment(plr))
        RestoreHealth(pUnit, 1);
}

void PlayerClassOnFree(int plr)
{
    FreePlayerCamera(plr);
    player[plr] = 0;
    player[plr + 10] = 0;
}

void ForceChangePlaerUpdateHandler(int pUnit)
{
    int ptr = UnitToPtr(pUnit);

    if (ptr)
    {
        if (GetMemory(ptr + 0x2e8) == 0x4f8100)
            SetMemory(ptr + 0x2e8, PlayerUpdate4f8100());
    }
}

void LoopPreservePlayer()
{
    int i;

    for (i = 9 ; i >= 0 ; i -= 1)
    {
        while (TRUE)
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
                    if (CheckPlayerDeath(i)) break;
                    else
                    {
                        OnPlayerDeath(i);
                        SetPlayerDeathFlag(i);
                    }
                    break;
                }
            }
            if (player[i + 10])
                PlayerClassOnFree(i);
            break;
        }
    }
    FrameTimer(1, LoopPreservePlayer);
}

string ArmorTable(int index)
{
    string armors[15] = {
        "OrnateHelm", "Breastplate", "PlateArms", "PlateBoots", "PlateLeggings",
        "LeatherArmbands", "ConjurerHelm", "LeatherArmor", "LeatherArmoredBoots",
        "LeatherLeggings", "WizardHelm", "WizardRobe", "LeatherBoots", "MedievalCloak",
        "MedievalShirt"
    };
    return armors[index];
}

string WeaponTable(int index)
{
    string weapons[15] = {
        "WarHammer", "GreatSword", "RoundChakram", "RoundChakram", "RoundChakram",
        "CrossBow", "Quiver", "Quiver", "Quiver", "Quiver",
        "DeathRayWand", "LesserFireballWand", "FireStormWand", "ForceWand", "InfinitePainWand"
    };
    return weapons[index];
}

void placedArmor()
{
    int repeat;
    float buffer;

    if (repeat < 15)
    {
        TeleportLocation(33, LocationX(31) + buffer, LocationY(31) - buffer);

        int u = 0;
        while (u < 5)
        {
            CreateObjectAt(ArmorTable(repeat), LocationX(33), LocationY(33));
            TeleportLocationVector(33, 23.0, 23.0);
            u += 1;
        }
        buffer += 23.0;
        ++repeat;
        FrameTimer(1, placedArmor);
    }
}
void placedWeapon()
{
    int repeat;
    float buffer;

    if (repeat < 15)
    {
        TeleportLocation(34, LocationX(32) + buffer, LocationY(32) - buffer);

        int u = 0;
        while (u < 5)
        {
            CreateObjectAt(WeaponTable(repeat), LocationX(34), LocationY(34));
            TeleportLocationVector(34, 23.0, 23.0);
            u += 1;
        }
        buffer += 23.0;
        ++repeat;
        FrameTimer(1, placedWeapon);
    }
}
string structUnitType(int arg_0) {
    string var_0[60];

    if (arg_0 == -1) {
        //area1
        var_0[0] = "imp";
        var_0[1] = "flyingGolem";
        var_0[2] = "bat";
        //area2
        var_0[3] = "urchin";
        var_0[4] = "flyingGolem";
        var_0[5] = "smallSpider";
        //area3
        var_0[6] = "giantLeech";
        var_0[7] = "urchin";
        var_0[8] = "smallAlbinoSpider";
        //area4
        var_0[9] = "spider";
        var_0[10] = "giantLeech";
        var_0[11] = "wolf";
        //area5
        var_0[12] = "swordsman";
        var_0[13] = "archer";
        var_0[14] = "wolf";
        //area6
        var_0[15] = "goon";
        var_0[16] = "whiteWolf";
        var_0[17] = "archer";
        //area7
        var_0[18] = "urchinShaman";
        var_0[19] = "blackBear";
        var_0[20] = "blackWolf";
        //area8
        var_0[21] = "troll";
        var_0[22] = "blackBear";
        var_0[23] = "spittingSpider";
        //area9
        var_0[24] = "gruntAxe";
        var_0[25] = "OgreBrute";
        var_0[26] = "OgreWarlord";
        //area10
        var_0[27] = "bear";
        var_0[28] = "Swordsman";
        var_0[29] = "bear2";
        //area11
        var_0[30] = "FireSprite";
        var_0[31] = "MeleeDemon";
        var_0[32] = "flyingGolem";
        //area12
        var_0[33] = "emberDemon";
        var_0[34] = "FireSprite";
        var_0[35] = "Bear2";
        //area13
        var_0[36] = "Skeleton";
        var_0[37] = "SkeletonLord";
        var_0[38] = "Wizard";
        //area14
        var_0[39] = "EvilCherub";
        var_0[40] = "LichLord";
        var_0[41] = "MechanicalGolem";
        //area15
        var_0[42] = "WeirdlingBeast";
        var_0[43] = "WizardGreen";
        var_0[44] = "Scorpion";
        //area16
        var_0[45] = "Beholder";
        var_0[46] = "Mimic";
        var_0[47] = "BlackWidow";
        //area17
        var_0[48] = "bear2";
        var_0[49] = "StoneGolem";
        var_0[50] = "Archer";
        //area18
        var_0[51] = "MechanicalGolem";
        var_0[52] = "SkeletonLord";
        var_0[53] = "AirshipCaptain";
        //area19
        var_0[54] = "WizardWhite";
        var_0[55] = "Horrendous";
        var_0[56] = "FireSprite";
        //area20
        var_0[57] = "HecubahWithOrb";
        var_0[58] = "LichLord";
        var_0[59] = "Horrendous";
        return var_0[0];
    }
    return var_0[arg_0];
}
int structUnitHealth(int arg_0) {
    int var_0[60];

    if (arg_0 == -1) {
        //area1
        var_0[0] = 40;
        var_0[1] = 40;
        var_0[2] = 60;
        //area2
        var_0[3] = 50;
        var_0[4] = 60;
        var_0[5] = 60;
        //area3
        var_0[6] = 90;
        var_0[7] = 70;
        var_0[8] = 70;
        //area4
        var_0[9] = 130;
        var_0[10] = 110;
        var_0[11] = 130;
        //area5
        var_0[12] = 150;
        var_0[13] = 96;
        var_0[14] = 160;
        //area6
        var_0[15] = 160;
        var_0[16] = 180;
        var_0[17] = 125;
        //area7
        var_0[18] = 160;
        var_0[19] = 230;
        var_0[20] = 170;
        //area8
        var_0[21] = 260;
        var_0[22] = 260;
        var_0[23] = 180;
        //area9
        var_0[24] = 200;
        var_0[25] = 260;
        var_0[26] = 295;
        //area10
        var_0[27] = 325;
        var_0[28] = 295;
        var_0[29] = 295;
        //area11
        var_0[30] = 130;
        var_0[31] = 192;
        var_0[32] = 108;
        //area12
        var_0[33] = 192;
        var_0[34] = 130;
        var_0[35] = 306;
        //area13
        var_0[36] = 225;
        var_0[37] = 325;
        var_0[38] = 265;
        //area14
        var_0[39] = 130;
        var_0[40] = 325;
        var_0[41] = 500;
        //area15
        var_0[42] = 130;
        var_0[43] = 195;
        var_0[44] = 300;
        //area16
        var_0[45] = 325;
        var_0[46] = 450;
        var_0[47] = 130;
        //area17
        var_0[48] = 360;
        var_0[49] = 600;
        var_0[50] = 130;
        //area18
        var_0[51] = 650;
        var_0[52] = 340;
        var_0[53] = 500;
        //area19
        var_0[54] = 225;
        var_0[55] = 400;
        var_0[56] = 130;
        //area20
        var_0[57] = 500;
        var_0[58] = 400;
        var_0[59] = 400;
        return var_0[0];
    }
    return var_0[arg_0];
}

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

void SetGoldAmount(int gUnit, int sAmount)
{
    int ptr = UnitToPtr(gUnit);

    if (ptr)
        SetMemory(GetMemory(ptr + 0x2b4), sAmount);
}

int CheckPotionOrGoldThingID(int unit)
{
    int thingID = GetUnitThingID(unit), x = unit;

    if (thingID == 639)
        x = CreateYellowPotion(125, GetObjectX(unit), GetObjectY(unit));
    else if (thingID == 640)
        x = CreateWhitePotion(100, GetObjectX(unit), GetObjectY(unit));
    else if (thingID == 641)
        x = CreateBlackPotion(85, GetObjectX(unit), GetObjectY(unit));
    else if (thingID == 2670 || thingID == 2671)
        SetGoldAmount(unit, Random(1000, 5000));
    if (x ^ unit) Delete(unit);
    return x;
}

string StructUnitPick(int num)
{
    string name[] = {
        "RedPotion", "BluePotion", "CurePoisonPotion", "HastePotion", "VampirismPotion",
        "ShieldPotion", "YellowPotion", "WhitePotion", "BlackPotion", "ShockProtectPotion",
        "FireProtectPotion", "PoisonProtectPotion", "Quiver", "FanChakram", "RoundChakram",
        "QuestGoldChest", "QuestGoldPile", "InvisibilityPotion", "InvulnerabilityPotion", "InfravisionPotion"
        };
    return name[num];
}

void putUnit() {
    int var_0;
    int var_1;

    if (var_0 < 25) {
        var_1 = Random((Gvar_4 - 1) * 3, ((Gvar_4 - 1) * 3) + 2);
        spawnUnitLocation(var_1, 27);
        spawnUnitLocation(var_1, 28);
        var_0 += 1;
        FrameTimer(2, putUnit);
    }
    else
        var_0 = 0;
}

//unit_index, location
void spawnUnitLocation(int index, int location)
{
    int unit = CreateObject(structUnitType(index), location);
    int procResult = CheckMonsterThing(unit);

    CreateObject("InvisibleLightBlueHigh", location);

    if (procResult == FALSE)
        SetUnitMaxHealth(unit, structUnitHealth(index));
    SetCallback(unit, 5, SetUnitDeath);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    insertUnitTable(unit);
    specialEvent(unit, index);
}

void SetUnitDeath()
{
    countofDeaths();
    MoveWaypoint(1, GetObjectX(SELF), GetObjectY(SELF));
    DeleteObjectTimer(CheckPotionOrGoldThingID(CreateObject(StructUnitPick(Random(0, 19)), 1)), 1500);
    DeleteObjectTimer(SELF, 30);
    Delete(GetTrigger() + 1);
}

void countofDeaths()
{
    int count;

    if (++count == 50)
    {
        RevivePlayers();
        FrameTimer(30, startButtonOn);
        count = 0;
        isGameStandby = 1;
    }
}

void startButtonOn() {
    ObjectOn(startGameButton());
}

void SetUnitHealth(int unit, int amount)
{
    Damage(unit, 0, CurrentHealth(unit) - 1 , -1);
    Pickup(unit, CreateObject("RottenMeat", 1));
    Damage(unit, 0, CurrentHealth(unit) - amount, -1);
}

void insertUnitTable(int arg_0)
{
    int var_0;

    FieldMob[var_0] = arg_0;
    var_0 = (var_0 + 1) % 50;
}

//index, unit_number
void specialEvent(int arg_0, int arg_1)
{
    if (arg_1 == 5)
        Enchant(arg_0, "ENCHANT_SLOWED", 0.0);
    else if (arg_1 == 18 || arg_1 == 38 || arg_1 == 43 || arg_1 == 45 || arg_1 == 54)
        Enchant(arg_0, "ENCHANT_ANCHORED", 0.0);
    else if (arg_1 == 32)
        SetCallback(arg_0, 3, mecaFlyingVisibleAttack);
}

//유닛과 가까운 플레이어를 찾습니다
int GetNearlyPlayer(int unit)
{
    float dist, temp = 9999.0;
    int plr = -1, i;

    for (i = 9 ; i >= 0 ; i -= 1)
    {
        if (CurrentHealth(player[i]))
        {
            if (CheckPlayerAlive(i))
            {
                dist = DistanceUnitToUnit(unit, player[i]);
                if (dist < temp)
                {
                    temp = dist;
                    plr = i;
                }
            }
        }
    }
    return plr;
}

void LoopUnitAI(int index)
{
    int target, unit = FieldMob[index];

    if (CurrentHealth(unit))
    {
        target = GetNearlyPlayer(unit);
        if (target + 1)
        {
            if (ToInt(GetObjectZ(unit + 1)) ^ player[target])
            {
                Raise(unit + 1, player[target]);
                CreatureFollow(unit, player[target]);
                AggressionLevel(unit, 1.0);
            }
        }
        if (UnitCheckEnchant(unit, GetLShift(ENCHANT_CHARMING)))
        {
            MoveObject(cancelCharm(), GetObjectX(unit), GetObjectY(unit));
            CastSpellObjectObject("SPELL_COUNTERSPELL", cancelCharm(), cancelCharm());
        }
        else if (UnitCheckEnchant(unit, GetLShift(ENCHANT_INVISIBLE)))
        {
            EnchantOff(unit, "ENCHANT_INVISIBLE");
            CastSpellObjectObject("SPELL_SHIELD", unit, unit);
        }
    }
    FrameTimerWithArg(1, (index + 1) % 50, LoopUnitAI);
}

int cancelCharm()
{
    int charmBlock;

    if (charmBlock == 0)
        charmBlock = CreateObject("InvisibleLightBlueHigh", 9);
    return charmBlock;
}
void summoningMonsters()
{
    if (isGameStandby)
    {
        putUnit();
        isGameStandby = 0;
    }
}

void RevivePlayers()
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (CurrentHealth(player[i]))
        {
            FreePlayerCamera(i);
            MoveObject(player[i], LocationX(11), LocationY(11));
            if (!CheckPlayerAlive(i))
                SetPlayerAliveFlag(i);
        }
    }
    AudioEvent("BlindOff", 11);
    Effect("TELEPORT", LocationX(11), LocationY(11), 0.0, 0.0);
    UniPrintToAll("리스폰 타임~ 죽은 유저가 되살아납니다");
}

void SetResource(int unit, int goldUnit)
{
    int goldPic = Random(1000, 5000);

    Delete(goldUnit);
    MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
    ChangeGold(unit, goldPic);
    AudioEvent("TreasurePickup", 1);
    UniPrint(unit, IntToString(goldPic) + " 원 득템!");
}

void startGameOn()
{
    ObjectOff(startGameButton());
    if (Gvar_4 < 20)
    {
        Gvar_4 += 1;
        RevivePlayers();
        UniPrintToAll("방금 시작 버튼이 눌러 졌습니다 .");
        UniPrintToAll("현재 " + IntToString(Gvar_4) + " 스테이지 도전중 ...!");
        FrameTimer(30, summoningMonsters);
    }
    else {
        UniPrintToAll("승리 - 모든 스테이지를 전부 클리어 하셨습니다 ..!!");
        victoryEvent();
    }
}

int startGameButton()
{
    return Object("startGameButton");
}

void teleportToShop()
{
    if (isGameStandby)
        MoveObject(OTHER, GetWaypointX(5), GetWaypointY(5));
}
void teleportToField()
{
    if (isGameStandby)
        MoveObject(OTHER, GetWaypointX(19), GetWaypointY(19));
}

void MapDecorations()
{
    string var_0 = "InvisibleLightBlueHigh";
    int i;

    for(i = 0 ; i < 4 ; i ++)
        CreateObject(var_0, i + 27);
    for(i = 0; i < 5 ; i ++)
        CreateObject(var_0, i + 36);
    DrawMagicIcon(GetWaypointX(13), GetWaypointY(13));
    OblivionMarket(16);
    AllEnchantMarket(17);
    DemonsWandMarket(18);
    MissileWandMarket(20);
}

void DetectedSpecficIndex(int curId)
{
    int thingID = GetUnitThingID(curId);

    if (thingID == 526)
        HarpoonEvent(curId);
    else if (thingID == 709)
        StarShotChanged(curId);
}

void LoopSearchIndex()
{
    int curId, tempId;

    if (GetMemory(0x750710))
    {
        tempId = GetMemory(GetMemory(0x750710) + 0x2c);
        if (curId)
        {
            while (curId < tempId)
            {
                curId += 1;
                DetectedSpecficIndex(curId);
            }
        }
        else
            curId = tempId;
    }
    FrameTimer(1, LoopSearchIndex);
}

int SummonTripleArrow(float x, float y, int owner, float force)
{
    int unit = CreateObjectAt("ArcherArrow", x, y);

    SetOwner(owner, unit);
    LookAtObject(unit, owner);
    LookWithAngle(unit, GetDirection(unit) + 128);
    PushObject(unit, force, GetObjectX(owner), GetObjectY(owner));

    return unit;
}

void HarpoonEvent(int cur)
{
    int owner = GetOwner(cur), unit;
    float vectX, vectY;
    float var_3 = 0.1;
    
    if (CurrentHealth(owner))
    {
        vectX = UnitRatioX(owner, cur, 7.0);
        vectY = UnitRatioY(owner, cur, 7.0);
        unit = SummonTripleArrow(GetObjectX(owner) - vectX, GetObjectY(owner) - vectY, owner, 10.0);
        while (var_3 < 0.4)
        {
            SummonTripleArrow((var_3 * vectY) + GetObjectX(owner) - vectX, (-var_3 * vectX) + GetObjectY(owner) - vectY, owner, 10.0);
            SummonTripleArrow((-var_3 * vectY) + GetObjectX(owner) - vectX, (var_3 * vectX) + GetObjectY(owner) - vectY, owner, 10.0);
            var_3 += 0.1;
        }
    }
    Delete(cur);
}

void nonCrashWeapons()
{
    int var_0;
    int result = 0;

    var_0 = GetLastItem(OTHER);
    while(IsObjectOn(var_0))
    {
        if (HasClass(var_0, "WEAPON") && !HasEnchant(var_0, "ENCHANT_INVULNERABLE"))
        {
            result ++;
            Enchant(var_0, "ENCHANT_INVULNERABLE", 0.0);
        }
        var_0 = GetPreviousItem(var_0);
    }
    if (result)
    {
        UniPrint(OTHER, "처리결과 총 " + IntToString(result) + " 개의 아이템을 무적화 했습니다 .");
        Enchant(OTHER, "ENCHANT_FREEZE", 1.0);
    }
}
void nonCrashArmors() {
    int var_0;
    int result = 0;

    var_0 = GetLastItem(OTHER);
    while(IsObjectOn(var_0))
    {
        if (HasClass(var_0, "ARMOR") && !HasEnchant(var_0, "ENCHANT_INVULNERABLE")) {
            result += 1;
            Enchant(var_0, "ENCHANT_INVULNERABLE", 0.0);
        }
        var_0 = GetPreviousItem(var_0);
    }
    if (result) {
        UniPrintToAll("처리결과 총 " + IntToString(result) + " 개의 아이템을 무적화 했습니다 .");
        Enchant(OTHER, "ENCHANT_FREEZE", 1.0);
    }
}
void nonCrashStaffs()
{
    int var_0;
    int result = 0;

    var_0 = GetLastItem(OTHER);
    while(IsObjectOn(var_0))
    {
        if (HasClass(var_0, "WAND") && !HasEnchant(var_0, "ENCHANT_INVULNERABLE"))
        {
            result += 1;
            Enchant(var_0, "ENCHANT_INVULNERABLE", 0.0);
        }
        var_0 = GetPreviousItem(var_0);
    }
    if (result)
    {
        UniPrintToAll("처리결과 총 " + IntToString(result) + " 개의 아이템을 무적화 했습니다 .");
        Enchant(OTHER, "ENCHANT_FREEZE", 1.0);
    }
}
void Windbooster(int unit)
{
    EnchantOff(unit, EnchantList(ENCHANT_SNEAK));
    RemoveTreadLightly(unit);
    PushObjectTo(unit, UnitAngleCos(unit, 80.0), UnitAngleSin(unit, 80.0));
    Effect("RICOCHET", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
}

void StarShotChanged(int cur)
{
    int owner = GetOwner(cur), unit;

    if (CurrentHealth(owner))
    {
        unit = CreateObjectAt("DeathBallFragment", GetObjectX(owner) + UnitAngleCos(owner, 11.0), GetObjectY(owner) + UnitAngleSin(owner, 11.0));
        SetOwner(owner, unit);
        PushObject(unit, -30.0, GetObjectX(owner), GetObjectY(owner));
    }
    Delete(cur);
}

void mecaFlyingVisibleAttack()
{
    if (CurrentHealth(SELF))
    {
        if (!UnitCheckEnchant(SELF, GetLShift(ENCHANT_PROTECT_FROM_MAGIC)))
        {
            Effect("DAMAGE_POOF", GetObjectX(SELF), GetObjectY(SELF), 0.0, 0.0);
            Effect("DAMAGE_POOF", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
            Effect("SMOKE_BLAST", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
            Damage(OTHER, SELF, 18, 2);
            TeleportLocation(6, GetObjectX(SELF), GetObjectY(SELF));
            AudioEvent("BarrelBreak", 6);
            Enchant(SELF, EnchantList(ENCHANT_PROTECT_FROM_MAGIC), 0.9);
        }
        Enchant(SELF, "ENCHANT_BLINDED", 0.3);
        Attack(SELF, OTHER);
        AggressionLevel(SELF, 0.83);
    }
}

void victoryEvent() {
    int var_0 = 0;

    Effect("WHITE_FLASH", GetWaypointX(10), GetWaypointY(10), 0.0, 0.0);
    CreateObject("blueRain", 10);
    AudioEvent("BigGong", 10);

    while(var_0 < 10) {
        if (CurrentHealth(player[var_0]) > 0)
            MoveObject(player[var_0], GetWaypointX(10), GetWaypointY(10));
        var_0 += 1;
    }
}

void StrEntryShop()
{
	int arr[18], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(1), pos_y = GetWaypointY(1);

	arr[0] = 1009246736; arr[1] = 138436548; arr[2] = 135401732; arr[3] = 1200637186; arr[4] = 604520592; arr[5] = 37888398; arr[6] = 151654853; arr[7] = 555880929; 
	arr[8] = 296000; arr[9] = 2568; arr[10] = 132096; arr[11] = 285149152; arr[12] = 541129744; arr[13] = 142574600; arr[14] = 270598664; arr[15] = 1881162244; 
	arr[16] = 267452167; arr[17] = 252; 
	for (i = 0 ; i < 18 ; i ++)
		count = DrawStrEntryShop(arr[i], name, count);
	MoveWaypoint(1, pos_x, pos_y);
}

int DrawStrEntryShop(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 558 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 1);
		if (count % 49 == 48)
			MoveWaypoint(1, GetWaypointX(1) - 96.0, GetWaypointY(1) + 2.0);
		else
			MoveWaypoint(1, GetWaypointX(1) + 2.0, GetWaypointY(1));
		count ++;
	}
	return count;
}

void StrGameBegin()
{
	int arr[18], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(1), pos_y = GetWaypointY(1);

	arr[0] = 270779038; arr[1] = 171990980; arr[2] = 135299361; arr[3] = 75780610; arr[4] = 1208500356; arr[5] = 34612258; arr[6] = 252428741; arr[7] = 19009665; 
	arr[8] = 1157890130; arr[9] = 21236232; arr[10] = 141312; arr[11] = 285149728; arr[12] = 675413522; arr[13] = 4752392; arr[14] = 270614792; arr[15] = 2097794; 
	arr[16] = 134283018; arr[17] = 256; 
	for (i = 0 ; i < 18 ; i ++)
		count = DrawStrGameBegin(arr[i], name, count);
	MoveWaypoint(1, pos_x, pos_y);
}

int DrawStrGameBegin(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 558 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 1);
		if (count % 49 == 48)
			MoveWaypoint(1, GetWaypointX(1) - 96.0, GetWaypointY(1) + 2.0);
		else
			MoveWaypoint(1, GetWaypointX(1) + 2.0, GetWaypointY(1));
		count ++;
	}
	return count;
}

void PutStampString()
{
    MoveWaypoint(1, GetWaypointX(19), GetWaypointY(19));
    StrEntryShop();
    MoveWaypoint(1, GetWaypointX(29), GetWaypointY(29));
    StrGameBegin();
}

void MapExit()
{
    SetMemory(0x5cb394, 6075528);
    SetMemory(0x5cb3a0, 6075544);
    SetMemory(0x5cb3b8, 6075580);
    SetMemory(0x5cb3ac, 6075560);
    ResetPlayerHandlerWhenExitMap();
    RemoveCoopTeamMode();
    MusicEvent();
    // FixPlayerDialogBug();
}

void BurnningZombieCollide()
{
    int owner = GetOwner(SELF);

    while (TRUE)
    {
        if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
        {
            Damage(OTHER, owner, 225, 14);
            Enchant(OTHER, "ENCHANT_FREEZE", 1.0);
            Enchant(OTHER, EnchantList(ENCHANT_CHARMING), 0.2);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(SELF);
        break;
    }
}

int FxBurnningZombie(float xProfile, float yProfile)
{
    int fxU = CreateObjectAt("Zombie", xProfile, yProfile);

    UnitNoCollide(fxU);
    ObjectOff(fxU);
    Damage(fxU, 0, MaxHealth(fxU) + 1, 1);
    return fxU;
}

void BurnningSunFlying(int subunit)
{
    int durate = GetDirection(subunit);

    while (IsObjectOn(subunit))
    {
        int owner = GetOwner(subunit);

        if (CurrentHealth(owner) && durate)
        {
            FrameTimerWithArg(1, subunit, BurnningSunFlying);
            LookWithAngle(subunit, durate - 1);
            int coll = DummyUnitCreateAt("Troll", GetObjectX(subunit), GetObjectY(subunit));
            SetCallback(coll, 9, BurnningZombieCollide);
            SetOwner(owner, coll);
            DeleteObjectTimer(coll, 1);
            DeleteObjectTimer(FxBurnningZombie(GetObjectX(subunit), GetObjectY(subunit)), 18);
            MoveObject(subunit, GetObjectX(subunit) + GetObjectZ(subunit), GetObjectY(subunit) + GetObjectZ(subunit + 1));
            MoveObject(subunit + 1, GetObjectX(subunit + 1) + GetObjectZ(subunit), GetObjectY(subunit + 1) + GetObjectZ(subunit + 1));
            if (IsVisibleTo(subunit + 1, subunit))
                break;
        }
        Delete(subunit);
        break;
    }
}

void BlueMissileCollide()
{
    int owner = GetOwner(SELF);

    while (1)
    {
        if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
        {
            GreenSparkFxAt(GetObjectX(OTHER), GetObjectY(OTHER));
            Damage(OTHER, owner, 185, 14);
            Enchant(OTHER, "ENCHANT_FREEZE", 1.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(SELF);
        break;
    }
}

int BlueOrbSummon(int sOwner)
{
    int unit = CreateObjectAt("MagicMissile", GetObjectX(sOwner) + UnitAngleCos(sOwner, 14.0), GetObjectY(sOwner) + UnitAngleSin(sOwner, 14.0));
    int ptr = GetMemory(0x750710);

    if (ptr)
    {
        SetMemory(ptr + 0x2e8, 5483536); //projectile update
        SetUnitCallbackOnCollide(unit, BlueMissileCollide);
        SetOwner(sOwner, unit);
    }
    return unit;
}

void OblivionUseHandler()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(SELF);

    if (MathAbs(cFps - cTime) < 20)
        return;
    if (CurrentHealth(OTHER))
    {
        SetUnit1C(SELF, cFps);
        float xVect = UnitAngleCos(OTHER, 19.0), yVect = UnitAngleSin(OTHER, 19.0);
        int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(OTHER) + xVect, GetObjectY(OTHER) + yVect);

        Raise(unit, xVect);
        Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit) - xVect, GetObjectY(unit) - yVect), yVect);
        SetOwner(OTHER, unit);
        LookWithAngle(unit, 32);
        FrameTimerWithArg(1, unit, BurnningSunFlying);
        PlaySoundAround(OTHER, 221);
    }
}

void DelayGiveToOwner(int sTarget)
{
    int sOwner = GetOwner(sTarget);

    if (IsObjectOn(sTarget) && CurrentHealth(sOwner))
        Pickup(sOwner, sTarget);
    else
        Delete(sTarget);
}

int SummonOblivionStaff(float sX, float sY)
{
    int unit = CreateObjectAt("OblivionOrb", sX, sY);
    int ptr = GetMemory(0x750710);

    if (ptr)
    {
        SetUnitCallbackOnUseItem(unit, OblivionUseHandler);
        SetMemory(ptr + 0x2c4, 0x53a720);
    }
    return unit;
}

int SummonMissileWand(float xProfile, float yProfile)
{
    int wand = CreateObjectAt("MissileWand", xProfile, yProfile);
    int *ptr = UnitToPtr(wand);

    if (ptr != NULLPTR)
    {
        SetUnitCallbackOnUseItem(wand, UseMissileWand);
        ptr[3] = 65536;
        int *amount = ptr[184];

        if (amount != NULLPTR)
            amount[27] = 0xfafa;
    }
    return wand;
}

int SummonDemonsWand(float xProfile, float yProfile)
{
    int wand = CreateObjectAt("DemonsBreathWand", xProfile, yProfile);
    int *ptr = UnitToPtr(wand);

    if (ptr != NULLPTR)
    {
        int *amount = ptr[184];

        if (amount != NULLPTR)
            amount[27] = 0xfafa;
    }
    return wand;
}

void GreenSparkFxAt(float sX, float sY)
{
    int fxUnit = CreateObjectAt("MonsterGenerator", sX, sY);

    Damage(fxUnit, 0, 10, 100);
    Delete(fxUnit);
}

void DrawGeometryRingAt(string orbName, float xProfile, float yProfile, int angleAdder)
{
    int u;
    float speed = 2.3;

    for ( u = 0 ; u < 60 ; u += 1)
        LinearOrbMove(CreateObjectAt(orbName, xProfile, yProfile), MathSine((u * 6) + 90 + angleAdder, -12.0), MathSine((u * 6) + angleAdder, -12.0), speed, 10);
}

void UseMissileWand()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(SELF);

    if (MathAbs(cFps - cTime) < 20)
        return;
    if (CurrentHealth(OTHER))
    {
        SetUnit1C(SELF, cFps);
        PushObject(BlueOrbSummon(OTHER), 20.0, GetObjectX(OTHER), GetObjectY(OTHER));
        PlaySoundAround(OTHER, 221);
    }
}

void AllEnchantMarketDesc()
{
    UniPrint(OTHER, "유용한 버프가 항상 지속되길 원하시면 5만골드 내고 구입하세요");
    TellStoryUnitName("aa", "Con05A.scr:HoundGreeting", "올엔첸5만");
}

void AllEnchantMarketTrade()
{
    if (GetAnswer(SELF) ^ 1)
        return;
    int plr = CheckPlayer();

    if (plr < 0)
        return;

    if (GetGold(OTHER) >= 50000)
    {
        if (CheckPlayerFlagAllEnchantment(plr))
        {
            UniPrint(OTHER, "거래실패! 당신은 이미 이 능력을 가졌습니다");
        }
        else
        {
            ChangeGold(OTHER, -50000);
            SetPlayerAllEnchantFlag(plr);
            ApplyAllEnchantToPlayer(OTHER);
            PlaySoundAround(OTHER, 226);
            DrawGeometryRingAt("CharmOrb", GetObjectX(OTHER), GetObjectY(OTHER), 30);
            UniPrint(OTHER, "거래완료! 올 엔첸 능력은 이제 당신의 것 입니다");
        }
    }
    else
    {
        UniPrint(OTHER, "거래실패! 잔액이 부족합니다");
    }
}

void OblivionMarketDesc()
{
    UniPrint(OTHER, "망가의 지팡이를 구입하시겠어요(7만골드 필요)");
    TellStoryUnitName("aa", "mmissile.c:TooManyMissiles", "마법미사일\n지팡이\n7만골드");
}

void OblivionMarketTrade()
{
    if (GetAnswer(SELF) ^ 1)
        return;

    if (GetGold(OTHER) >= 70000)
    {
        ChangeGold(OTHER, -70000);
        int staff = SummonOblivionStaff(GetObjectX(OTHER), GetObjectY(OTHER));
        SetOwner(OTHER, staff);
        FrameTimerWithArg(1, staff, DelayGiveToOwner);
        UniPrint(OTHER, "거래성공!:: 망가의 지팡이를 구입하였습니다 (7만 골드 차감)");
    }
    else
    {
        UniPrint(OTHER, "거래가 취소되었습니다:: 잔액 부족");
    }
}

void DemonsWandMarketDesc()
{
    UniPrint(OTHER, "용의 숨결 지팡이를 구입하시겠어요? (25000골드 필요!)");
    TellStoryUnitName("aa", "Wiz07C.scr:DemonTalk04", "용의숨결 지팡이\n2만5천골드");
}

void DemonsWandMarketTrade()
{
    if (GetAnswer(SELF) ^ 1)
        return;
    
    if (GetGold(OTHER) >= 25000)
    {
        ChangeGold(OTHER, -25000);
        int wand = SummonDemonsWand(GetObjectX(OTHER), GetObjectY(OTHER));
        SetOwner(OTHER, wand);
        FrameTimerWithArg(1, wand, DelayGiveToOwner);
        UniPrint(OTHER, "거래가 완료되었습니다");
    }
    else
    {
        UniPrint(OTHER, "거래가 취소되었습니다:: 잔액 부족");
    }
}

void MissileWandMarketDesc()
{
    UniPrint(OTHER, "미사일 완드를 구입하시겠어요? (2만골드 필요!)");
    TellStoryUnitName("aa", "Wiz07C.scr:DemonTalk04", "미사일완드\n2만골드");
}

void MissileWandMarketTrade()
{
    if (GetAnswer(SELF) ^ 1)
        return;
    
    if (GetGold(OTHER) >= 20000)
    {
        ChangeGold(OTHER, -20000);
        int wand = SummonMissileWand(GetObjectX(OTHER), GetObjectY(OTHER));
        SetOwner(OTHER, wand);
        FrameTimerWithArg(1, wand, DelayGiveToOwner);
        UniPrint(OTHER, "거래가 완료되었습니다");
    }
    else
    {
        UniPrint(OTHER, "거래가 취소되었습니다:: 잔액 부족");
    }
}

int DummyUnitCreate(string name, int wp)
{
    int unit = CreateObjectAt(name, LocationX(wp), LocationY(wp));

    if (CurrentHealth(unit))
    {
        ObjectOff(unit);
        Damage(unit, 0, MaxHealth(unit) + 1, -1);
        Frozen(unit, 1);
    }
    return unit;
}

int DummyUnitCreateAt(string name, float xProfile, float yProfile)
{
    int unit = CreateObjectAt(name, xProfile, yProfile);

    if (CurrentHealth(unit))
    {
        ObjectOff(unit);
        Damage(unit, 0, MaxHealth(unit) + 1, -1);
        Frozen(unit, 1);
    }
    return unit;
}

int OblivionMarket(int sLocation)
{
    int unit = DummyUnitCreate("WizardWhite", sLocation);

    SetDialog(unit, "YESNO", OblivionMarketDesc, OblivionMarketTrade);
    return unit;
}

int DemonsWandMarket(int sLocation)
{
    int unit = DummyUnitCreate("Demon", sLocation);

    SetDialog(unit, "YESNO", DemonsWandMarketDesc, DemonsWandMarketTrade);
    return unit;
}

int MissileWandMarket(int sLocation)
{
    int unit = DummyUnitCreate("WizardRed", sLocation);

    SetDialog(unit, "YESNO", MissileWandMarketDesc, MissileWandMarketTrade);
    return unit;
}

int AllEnchantMarket(int location)
{
    int unit = DummyUnitCreate("WizardGreen", location);

    SetDialog(unit, "YESNO", AllEnchantMarketDesc, AllEnchantMarketTrade);
    return unit;
}