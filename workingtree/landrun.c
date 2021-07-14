
#include "noxscript\builtins.h"

#include "libs\typecast.h"
#include "libs\callmethod.h"
#include "libs\unitutil.h"
#include "libs\unitstruct.h"
#include "libs\printutil.h"
#include "libs\mathlab.h"
#include "libs\reaction.h"
#include "libs\observer.h"
#include "libs\playerinfo.h"
#include "libs\username.h"
#include "libs\waypoint.h"

#define NULLPTR             0

#define OTHER       -1
#define SELF        -2

#define TRUE        1
#define FALSE        1

int player[20], s_plrCre[10], JunkRow;
float Junk_vect[2], Fvect[2], Fturn[4];
int ElevBridge[6];

int PlayerClassCheckDeathFlag(int plr)
{
    return player[plr + 10] & 2;
}

void PlayerClassSetDeathFlag(int plr)
{
    player[plr + 10] ^= 2;
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

static int InitMathSineBaseWaypointNumber() { return 73; }

void MapInitialize()
{
    MusicEvent();
    
    FrameTimer(1, StrFinish);
    
    DefElevators();
    
    FrameTimer(1, AfterTriggersExec);
    FrameTimer(2, DefJunkLump);
    FrameTimer(20, StrSelectChar);
    FrameTimer(30, InitPutCharset);
    FrameTimer(30, LoopPreservePlayers);
}

string GameGuide(int num)
{
    string table[] = {
        "맵 이름: 크리쳐 컨트렛",
        "게임방법을 안내하겠습니다, 이 게임의 승리 목적은 가장 먼저 컨트렛에 통과하는 것입니다",
        "하지만 이것은 일반적인 컨트렛이 아닙니다, 보시다 시피 시작지점에 8 개의 유닛이 놓여있습니다",
        "8개의 유닛 중 1개를 선택하시면 선택된 유닛으로 변신됩니다, 선택하실 때에는 유닛을 '클릭(말걸기)' 해주시면 되겠습니다",
        "유닛으로 변하지 않으면 크리쳐 컨트렛에 도전하실 수 없으니 반드시 크리쳐로 변신해 주시기 바랍니다",
        "크리쳐로 변신하셨을 때, 크리쳐를 이동하려면 이동하려는 위치로 '클릭' 하시면 됩니다",
        "이동할 지역을 클릭하게 되면 해당 지역에 푸른색 마크가 생성되며 이 마크는 자신 외에는 볼 수 없습니다",
        "컨트렛 도중 적이 나타나거나 상대방 크리처를 격추하려면 K 버튼(이모션: 가리키기)을 누르시면 데미지30의 화살이 발사됩니다",
        "견제를 위해 서로를 공격하거나 트롤링하셔도 되지만 난이도도 상당히 높으므로 협동해서 클리어 하시는 것을 더 추천해드립니다",
        "크리쳐 마다 이동속도나 체력은 다르지만 공격은 모두 동일하니 이점 참고하시어 착오 없으시기 바랍니다",
        "시작: 이제 서둘러 출발 하십시오!"
    };
    return table[num];
}

void HowToPlay(int num)
{
    if (num < 11)
    {
        PrintToAll(GameGuide(num));
        SecondTimerWithArg(5, num + 1, HowToPlay);
    }
}

void DefElevators()
{
    ElevBridge[0] = Object("ElevBridge21");
    ElevBridge[1] = Object("ElevBridge22");
    ElevBridge[2] = Object("ElevBridge23");
    ElevBridge[3] = Object("ElevBridge31");
    ElevBridge[4] = Object("ElevBridge32");
    ElevBridge[5] = Object("ElevBridge33");
    FrameTimerWithArg(30, Object("ElevBridge11"), DelayElevatorOff);
    FrameTimerWithArg(30, Object("ElevBridge12"), DelayElevatorOff);
}

void DefJunkLump()
{
    FlowMarker();
    JunkRow = CreateObject("SpikeBlock", 54);
    CreateObject("InvisibleLightBlueHigh", 54);
    CreateObject("InvisibleLightBlueHigh", 44);
    Frozen(JunkRow, 1);
    Junk_vect[0] = 3.0;
    Junk_vect[1] = -3.0;
    Fvect[0] = 23.0;
    Fvect[1] = -23.0;
    Fturn[0] = 46.0;
    Fturn[1] = 0.0;
    Fturn[2] = 0.0;
    Fturn[3] = 46.0;
}

void DummyFunction()
{
    //
}

void AfterTriggersExec()
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 1);
    GetOwner(ptr);
    Raise(ptr, DemonSelected);
    Delete(ptr);
    WallAngleTable(-1);
    InitBeacons();
    FrameTimer(39, MapTrapSet);
    FrameTimerWithArg(41, 10, TableMax);
    FrameTimerWithArg(42, 8, PutSpikeRots);
    FrameTimerWithArg(100, 28 | (2 << 8), BallTrap);
    FrameTimerWithArg(111, 30 | (119 << 8), BallTrap);
    FrameTimerWithArg(160, 0, HowToPlay);
}

void BallTrap(int flag)
{
    int wp = flag & 0xff, dir = (flag >> 8) & 0xff;
    int unit = SpawnBall(wp);

    LookWithAngle(unit, dir);
    LookWithAngle(unit + 1, 4);
    Raise(unit + 1, 5.0);
}

void InitBeacons()
{
    LookWithAngle(Object("SentrySwitch1"), 0);
    LookWithAngle(Object("SentrySwitch2"), 1);
    LookWithAngle(Object("SentrySwitch3"), 2);
    FrameTimerWithArg(10, 15, DrawRingAroundPortal);
}

void DrawRingAroundPortal(int wp)
{
    int k;

    TeleportLocationVector(wp, 0.0, 23.0);
    for (k = 0 ; k < 60 ; k += 1)
    {
        TeleportLocation(16, LocationX(wp) + MathSine(k * 6 + 90, 25.0), LocationY(wp) + MathSine(k * 6, 25.0));
        ObjectOff(CreateObject("CharmOrb", 16));
    }
}

int UnitHpTable(int num)
{
    int arr[] = { 97, 64, 98, 105,
        64, 110, 110, 120, 100};

    return arr[num];
}

string UnitTable(int num)
{
    string table[] = {"GruntAxe", "WeirdlingBeast", "Wizard", "Necromancer",
        "UrchinShaman", "Wolf", "AirshipCaptain", "Hecubah", "Bear2"};

    return table[num];
}

void InitPutCharset()
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 71) + 1, k;
    
    for (k = 0 ; k < 9 ; k ++)
    {
        Frozen(CreateObject(UnitTable(k), k + 1), 1);
        CreateObject("InvisibleLightBlueHigh", k + 1);
        CheckMonsterThing(ptr + (k * 2));
        if (k < 4)
            LookWithAngle(ptr + (k * 2), 64);
        else
            LookWithAngle(ptr + (k * 2), 192);
        SetDialog(ptr + (k * 2), "NORMAL", SelectedUnit, DummyFunction);
        LookWithAngle(ptr + (k * 2) + 1, k);
    }
    CreateObject("BlueSummons", 32);
    CreateObject("PlayerWaypoint", 33);
    CreateObject("PlayerWaypoint", 34);
    FrameTimer(3, MarkPutOnceAgain);
}

void MarkPutOnceAgain()
{
    int ptr = CreateObject("CarnivorousPlant", 33);
    CreateObject("CarnivorousPlant", 34);
    Damage(ptr, 0, MaxHealth(ptr) + 1, -1);
    Damage(ptr + 1, 0, MaxHealth(ptr + 1) + 1, -1);
    SetCallback(ptr, 9, BallTeleport);
    SetCallback(ptr + 1, 9, BallTeleport);
    CreateObject("PlayerWaypoint", 33);
    CreateObject("PlayerWaypoint", 34);
}

void CreatureDead()
{
    int owner = GetUnit1C(GetTrigger() + 1);

    if (MaxHealth(owner))
    {
        EnchantOff(owner, "ENCHANT_AFRAID");
        MoveObject(owner, LocationX(71), LocationY(71));
        UniPrint(owner, "당신의 크리쳐가 방금 생을 마감했어요...ㅠㅠ");
    }
    DeleteObjectTimer(self, 75);
    Delete(GetTrigger() + 1);
}

void CreatureAlwaysRunning(int unit)
{
    if (GetUnitStatus(unit) & 0x8000)
        return;
    else
        SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x8000);
}

void SelectedUnit()
{
    int idx = GetDirection(GetTrigger() + 1), unit, plr = CheckPlayer();

    if (!HasEnchant(other, "ENCHANT_AFRAID") && plr >= 0)
    {
        unit = CreateObject(UnitTable(idx), 18 + plr);
        SetUnit1C(CreateObject("InvisibleLightBlueHigh", 18 + plr), GetCaller());
        CheckMonsterThing(unit);
        SetUnitMaxHealth(unit, UnitHpTable(idx));
        CreatureAlwaysRunning(unit);
        LookWithAngle(unit + 1, plr);
        AggressionLevel(unit, 0.0);
        RetreatLevel(unit, 0.0);
        SetOwner(player[plr], unit);
        GiveCreatureToPlayer(OTHER, unit);
        SetCallback(unit, 5, CreatureDead);
        s_plrCre[plr] = unit;
        Enchant(player[plr], "ENCHANT_AFRAID", 0.0);
        MoveObject(player[plr], LocationX(plr + 18), LocationY(plr + 18));
        CallFunctionWithArg(MobSelectFunctions(idx), unit);
        FrameTimerWithArg(1, unit, TeleportCreature);
    }
}

int SetupPlayerCamera(int plr)
{
    int unit = CreateObjectAt("Maiden", GetObjectX(player[plr]), GetObjectY(player[plr]));

    UnitNoCollide(unit);
    LookWithAngle(unit, plr);
    SetOwner(player[plr], unit);
    GiveCreatureToPlayer(player[plr], unit);
    ObjectOff(unit);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    return unit;
}

void TeleportCreature(int creat)
{
    int plr = GetDirection(creat + 1);

    if (IsObjectOn(creat + 1))
    {
        if (CurrentHealth(player[plr]))
        {
            MoveObject(creat, LocationX(71), LocationY(71));
            Effect("RICOCHET", GetObjectX(creat), GetObjectY(creat), 0.0, 0.0);
        }
        else
        {
            Delete(creat);
            Delete(creat + 1);
        }
    }
}

int MobSelectFunctions(int index)
{
    int monsters[] = {DemonSelected, SecondSelected, MysticSelected, NecromancerSelected, ShamanSelected,
        WolfSelected, OgreSelected, HecubahSelected, SelectCrazyGirl, BufferNothing};

    return monsters[index];
}

void DemonSelected(int ptr)
{
    Raise(ptr + 1, 6.5);
}

void SecondSelected(int ptr)
{
    Raise(ptr + 1, 4.5);
}

void MysticSelected(int ptr)
{
    Raise(ptr + 1, 4.2);
}

void NecromancerSelected(int ptr)
{
    Raise(ptr + 1, 4.3);
}

void ShamanSelected(int ptr)
{
    Raise(ptr + 1, 4.0);
}

void WolfSelected(int ptr)
{
    Raise(ptr + 1, 5.0);
}

void OgreSelected(int ptr)
{
    Raise(ptr + 1, 5.0);
}

void HecubahSelected(int ptr)
{
    Raise(ptr + 1, 5.5);
}

void SelectCrazyGirl(int ptr)
{
    Raise(ptr + 1, 6.0);
}

void BufferNothing(int ptr)
{
    return;
}

int PlayerClassOnInit(int plr, int pUnit)
{
    player[plr] = pUnit;
    player[plr + 10] = 1;
    InitiPlayerCamera(pUnit);

    UniPrintToAll(PlayerIngameNick(pUnit) + " 님께서 게임에 참가했어요~");

    return plr;
}

void RegistPlayer()
{
    int k, plr;

    while (1)
    {
        if (CurrentHealth(other))
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
                PlayerEntry(plr);
                break;
            }
        }
        CantJoin();
        break;
    }
}

void PlayerEntry(int plr)
{
    if (PlayerClassCheckDeathFlag(plr))
        PlayerClassSetDeathFlag(plr);

    EnchantOff(player[plr], "ENCHANT_AFRAID");
    Enchant(player[plr], "ENCHANT_ANCHORED", 0.0);
    MoveObject(player[plr], LocationX(71), LocationY(71));
}

void CantJoin()
{
    UniPrint(OTHER, "이 맵은 지원되지 않습니다");
    Enchant(other, "ENCHANT_BLINDED", 0.0);
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    MoveObject(other, LocationX(13), LocationY(13));
}

void MovingPlayerCreature(int plr)
{
    int glow;

    if (CheckRightClick(player[plr]))
    {
        glow = CreateObject("Moonglow", 16);
        LookWithAngle(glow, plr);
        SetOwner(player[plr], glow);
        FrameTimerWithArg(1, glow, GoCreatureMove);
    }
    else
        StopCreature(plr);
}

void GoCreatureMove(int ptr)
{
    int plr = GetDirection(ptr);
    float sp;

    if (IsObjectOn(ptr))
    {
        if (CurrentHealth(s_plrCre[plr]))
        {
            if (Distance(GetObjectX(s_plrCre[plr]), GetObjectY(s_plrCre[plr]), GetObjectX(ptr), GetObjectY(ptr)) > 8.0)
            {
                sp = GetObjectZ(s_plrCre[plr] + 1);
                LookAtObject(s_plrCre[plr], ptr);
                Walk(s_plrCre[plr], GetObjectX(s_plrCre[plr]), GetObjectY(s_plrCre[plr]));
                MoveObject(s_plrCre[plr], GetObjectX(s_plrCre[plr]) + UnitRatioX(ptr, s_plrCre[plr], sp), GetObjectY(s_plrCre[plr]) + UnitRatioY(ptr, s_plrCre[plr], sp));
            }
        }
        Delete(ptr);
    }
}

void StopCreature(int plr)
{
    CreatureIdle(s_plrCre[plr]);
    //LookAtObject(s_plrCre[plr], ptr);
}

void PlayerCameraMoving(int plr)
{
    if (IsVisibleTo(s_plrCre[plr] + 2, s_plrCre[plr]))
    {
        if (Distance(GetObjectX(s_plrCre[plr] + 2), GetObjectY(s_plrCre[plr] + 2), GetObjectX(s_plrCre[plr]), GetObjectY(s_plrCre[plr])) > 4.0)
            PushObjectTo(s_plrCre[plr] + 2, UnitRatioX(s_plrCre[plr], s_plrCre[plr] + 2, 2.0), UnitRatioY(s_plrCre[plr], s_plrCre[plr] + 2, 2.0));
    }
    else
        MoveObject(s_plrCre[plr] + 2, GetObjectX(s_plrCre[plr]), GetObjectY(s_plrCre[plr]));
}

void NormalAttack(int glow)
{
    int plr = GetDirection(glow), mis;
    float pos_x, pos_y;

    if (CurrentHealth(player[plr]) && CurrentHealth(s_plrCre[plr]))
    {
        if (!HasEnchant(player[plr], "ENCHANT_PROTECT_FROM_ELECTRICITY"))
        {
            pos_x = UnitRatioX(s_plrCre[plr], glow, -15.0);
            pos_y = UnitRatioY(s_plrCre[plr], glow, -15.0);
            TeleportLocation(16, GetObjectX(s_plrCre[plr]) + pos_x, GetObjectY(s_plrCre[plr]) + pos_y);
            mis = CreateObject("WeakArcherArrow", 16);
            CreateObject("WeakArcherArrow", 16);
            CreateObject("WeakArcherArrow", 16);
            SetOwner(s_plrCre[plr], mis);
            SetOwner(s_plrCre[plr], mis + 1);
            SetOwner(s_plrCre[plr], mis + 2);
            LookAtObject(mis, glow);
            LookAtObject(mis + 1, glow);
            LookAtObject(mis + 2, glow);
            PushObjectTo(mis, pos_x * 2.0, pos_y * 2.0);
            PushObjectTo(mis + 1, pos_x * 2.0, pos_y * 2.0);
            PushObjectTo(mis + 2, pos_x * 2.0, pos_y * 2.0);
            DeleteObjectTimer(mis, 20);
            DeleteObjectTimer(mis + 1, 20);
            DeleteObjectTimer(mis + 2, 20);
            Enchant(player[plr], "ENCHANT_PROTECT_FROM_ELECTRICITY", 0.7);
            LookAtObject(s_plrCre[plr], glow);
        }
    }
    Delete(glow);
}

void PlayerClassCreatureAttack(int plr, int pUnit)
{
    int plrCre = s_plrCre[plr], glow = CreateObjectAt("Moonglow", GetObjectX(pUnit), GetObjectY(pUnit));

    LookWithAngle(glow, plr);
    SetOwner(pUnit, glow);
    FrameTimerWithArg(1, glow, NormalAttack);
    Effect("DAMAGE_POOF", GetObjectX(plrCre), GetObjectY(plrCre), 0.0, 0.0);
}

void PlayerCreatureErase(int plr)
{
    if (MaxHealth(s_plrCre[plr]))
    {
        Delete(s_plrCre[plr]);
        Delete(s_plrCre[plr + 10]);
    }
}

void PlayerClassOnDeath(int plr)
{
    int pUnit = player[plr];

    if (CurrentHealth(s_plrCre[plr]))
    {
        PlayerCreatureErase(plr);
    }
    UniPrintToAll(PlayerIngameNick(pUnit) + " 이 죽었습니다");
}

void PlayerClassOnAlive(int plr, int pUnit)
{
    if (MaxHealth(s_plrCre[plr]))
    {
        if (CheckWatchFocus(pUnit))
        {
            PlayerLook(pUnit, s_plrCre[plr]);
            if (CheckPlayerInput(pUnit) == 6)
                PlayerClassCreatureAttack(plr, pUnit);
            
        }
        MovingPlayerCreature(plr);
        
    }
}

void PlayerClassOnExit(int plr)
{
    PlayerCreatureErase(plr);
    player[plr] = 0;
    player[plr + 10] = 0;
}

//here
void LoopPreservePlayers()
{
    int i;

    for (i = 9 ; i >= 0 ; i-=1)
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
                    if (PlayerClassCheckDeathFlag(i)) break;
                    else
                    {
                        PlayerClassOnDeath(i);
                        PlayerClassSetDeathFlag(i);
                    }
                    break;
                }
            }
            if (player[i + 10])
                PlayerClassOnExit(i);
            break;
        }
    }
    FrameTimer(1, LoopPreservePlayers);
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

void StrYourWinner()
{
	int arr[26];
	string name = "CharmOrb";
	int i = 0;
	arr[0] = 1008796286; arr[1] = 1338015748; arr[2] = 270541312; arr[3] = 134236228; arr[4] = 35653888; arr[5] = 75776516; arr[6] = 9438912; arr[7] = 1146208324; arr[8] = 25362576; arr[9] = 100417554; 
	arr[10] = 585874; arr[11] = 67190528; arr[12] = 659584; arr[13] = 269745921; arr[14] = 8388798; arr[15] = 4097; arr[16] = 2114454018; arr[17] = 16781312; arr[18] = 2105848; arr[19] = 1879183392; 
	arr[20] = 35668031; arr[21] = 553783300; arr[22] = 67239936; arr[23] = 1090543556; arr[24] = 537132063; arr[25] = 67124992; 
	while(i < 26)
	{
		drawStrYourWinner(arr[i], name);
		i ++;
	}
}

void drawStrYourWinner(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = LocationX(14);
		pos_y = LocationY(14);
	}
	for (i = 1 ; i > 0 && count < 806 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 14);
		if (count % 74 == 73)
            TeleportLocationVector(14, -146.0, 2.0);
		else
            TeleportLocationVector(14, 2.0, 0.0);
		count ++;
	}
	if (count >= 806)
	{
		count = 0;
		TeleportLocation(14, pos_x, pos_y);
	}
}

void StrSelectChar()
{
	int arr[23];
	string name = "HealOrb";
	int i = 0;
	arr[0] = 1042801246; arr[1] = 1271144452; arr[2] = 143657280; arr[3] = 554729488; arr[4] = 578692354; arr[5] = 1014628416; arr[6] = 134779919; arr[7] = 286523649; arr[8] = 539123236; arr[9] = 1150289788; 
	arr[10] = 1066352784; arr[11] = 337645585; arr[12] = 2392670; arr[13] = 1077952576; arr[14] = 1888616448; arr[15] = 65823; arr[16] = 38027202; arr[17] = 134480960; arr[18] = 151552000; arr[19] = 538177792; 
	arr[20] = 604110848; arr[21] = 4195328; arr[22] = 524543; 
	while(i < 23)
	{
		drawStrSelectChar(arr[i], name);
		i ++;
	}
}

void drawStrSelectChar(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = LocationX(10);
		pos_y = LocationY(10);
	}
	for (i = 1 ; i > 0 && count < 713 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 10);
		if (count % 64 == 63)
            TeleportLocationVector(10, -126.0, 2.0);
		else
            TeleportLocationVector(10, 2.0, 0.0);
		count ++;
	}
	if (count >= 713)
	{
		count = 0;
		TeleportLocation(10, pos_x, pos_y);
	}
}

int SpawnBall(int wp)
{
    int unit = CreateObject("Maiden", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    CreateObject("StormCloud", wp);
    CreateObject("InvisibleLightBlueHigh", wp); //+3
    CreateObject("InvisibleLightBlueHigh", wp); //+4
    CreateObject("InvisibleLightBlueHigh", wp); //+5
    CreateObject("InvisibleLightBlueHigh", wp); //+6
    LookWithAngle(unit, 0);
    LookWithAngle(unit + 1, 4);
    ObjectOff(unit);
    Damage(unit, 0, CurrentHealth(unit) + 1, -1);
    //SetUnitFlags(unit + 2, GetUnitFlags(unit + 2) ^ 0x40);
    SetCallback(unit, 9, TouchedBall);
    FrameTimerWithArg(1, unit, LoopCheckingBall);
    return unit;
}

void LoopCheckingBall(int ball)
{
    PushObjectTo(ball, UnitAngleCos(ball, GetObjectZ(ball + 1)), UnitAngleSin(ball, GetObjectZ(ball + 1)));
    MoveObject(ball + 2, GetObjectX(ball), GetObjectY(ball));
    FrameTimerWithArg(1, ball, LoopCheckingBall);
}

int WallAngleTable(int num)
{
    int angle[4] = {45, 135, 315, 225};

    return angle[num];
}

void TouchedBall()
{
    int wall_t, reflect, c_wall = GetDirection(GetTrigger() + 1);

    if (!GetCaller())
    {
        wall_t = GetWallDirection(GetTrigger());
        if (wall_t != c_wall && wall_t >= 0)
        {
            reflect = (2 * WallAngleTable(wall_t)) - ((DirToAngle(GetDirection(self)) + 180) % 360);
            if (reflect < 0) reflect += 360;
            LookWithAngle(self, AngleToDir(reflect));
            LookWithAngle(GetTrigger() + 1, wall_t);
            TeleportLocation(16, GetObjectX(self), GetObjectY(self));
            AudioEvent("LightningWand", 16);
            Effect("VIOLET_SPARKS", GetWaypointX(16), GetWaypointY(16), 0.0, 0.0);
        }
    }
    else if (IsObjectOn(other))
    {
        if (CurrentHealth(other) && MaxHealth(other))
            Damage(other, 0, 30, 16);
    }
}

int DirToAngle(int num)
{
    return num * 45 / 32;
}

int AngleToDir(int num)
{
    return num * 32 / 45;
}

int GetWallDirection(int unit)
{
    int res = -1, k;
    float pos_x, pos_y;

    for (k = 0 ; k < 4 ; k ++)
    {
        if (k & 1) pos_x = 20.0;
        else pos_x = -20.0;
        if (k & 2) pos_y = 20.0;
        else pos_y = -20.0;
        MoveObject(unit + k + 3, GetObjectX(unit) + pos_x, GetObjectY(unit) + pos_y);
        if (!IsVisibleTo(unit + k + 3, unit))
            res = k;
        if (res >= 0) break;
    }
    return res;
}

void BallTeleport()
{
    float pos_x, pos_y;
    if (!IsObjectOn(other))
    {
        pos_x = RandomFloat(-23.0, 23.0);
        pos_y = RandomFloat(-23.0, 23.0);
        Effect("LIGHTNING", GetWaypointX(32) + pos_x, GetWaypointY(32) + pos_y, GetObjectX(other), GetObjectY(other));
        MoveObject(GetCaller() + 1, GetWaypointX(32) + pos_x, GetWaypointY(32) + pos_y);
        MoveObject(other, GetWaypointX(32) + pos_x, GetWaypointY(32) + pos_y);
        AudioEvent("LightningBolt", 32);
    }
}

void SingleRotToggleMoving()
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 35);

    if (!GetDirection(other))
        LookWithAngle(ptr, 35);
    else
        LookWithAngle(ptr, 45);
    LookWithAngle(other, GetDirection(other) ^ 1);
    Raise(ptr, ToFloat(GetCaller()));
    FrameTimerWithArg(65, ptr, DelayMoveRot);
}

void DelayMoveRot(int ptr)
{
    Move(ToInt(GetObjectZ(ptr)), GetDirection(ptr));
    Delete(ptr);
}

void ActivateJunkLump()
{
    if (!GetDirection(JunkRow))
    {
        UniChatMessage(other, "블럭이 움직이고 있습니다", 130);
        TeleportLocation(16, GetObjectX(JunkRow), GetObjectY(JunkRow));
        AudioEvent("SpikeBlockMove", 16);
        LookWithAngle(JunkRow, 1);
        FrameTimerWithArg(1, JunkRow, MovingJunk);
    }
}

void MovingJunk(int unit)
{
    int count = GetDirection(unit + 1), tog = GetDirection(unit + 2), open;
    if (count < 92)
    {
        MoveObject(unit, GetObjectX(unit) - Junk_vect[tog], GetObjectY(unit) + Junk_vect[tog]);
        LookWithAngle(unit + 1, count + 1);
        FrameTimerWithArg(1, unit, MovingJunk);
    }
    else
    {
        LookWithAngle(unit + 1, 0);
        LookWithAngle(unit, 0);
        LookWithAngle(unit + 2, (tog + 1) % 2);
        if (!open)
        {
            open = 1;
            WallOpen(Wall(89, 137));
        }
    }
}

int FlowMarker()
{
    int ptr;

    if (!ptr)
    {
        ptr = CreateObject("InvisibleLightBlueHigh", 12);
        CreateObject("InvisibleLightBlueHigh", 12);
        ObjectOff(ptr + 1);
        FrameTimerWithArg(200, ptr, RiseFlameTrap);
    }
    return ptr;
}

void RiseFlameTrap(int ptr)
{
    int count = GetDirection(ptr), tog = GetDirection(ptr + 1), fire, tog2;
    int line_count;

    if (IsObjectOn(ptr))
    {
        if (count < 9)
        {
            TeleportLocation(16, GetObjectX(ptr), GetObjectY(ptr));
            fire = CreateObject("FireGrateFlame", 16);
            Enchant(fire, "ENCHANT_FREEZE", 0.0);
            DeleteObjectTimer(fire, 12);
            LookWithAngle(ptr, count + 1);
            MoveObject(ptr, GetObjectX(ptr) + Fvect[tog], GetObjectY(ptr) + Fvect[tog]);
        }
        else
        {
            line_count = ToInt(GetObjectZ(ptr + 1)) + 1;
            if (line_count > 10)
            {
                line_count = 1;
                ObjectToggle(ptr + 1);
            }
            Raise(ptr + 1, ToFloat(line_count));
            tog2 = IsObjectOn(ptr + 1) * 2; //46, -46
            if (!tog)
                MoveObject(ptr, GetObjectX(ptr) - Fturn[tog2], GetObjectY(ptr) - Fturn[tog2 + 1]); //left, up
            else
                MoveObject(ptr, GetObjectX(ptr) + Fturn[tog2 + 1], GetObjectY(ptr) + Fturn[tog2]); //down, right
            LookWithAngle(ptr, 0);
            LookWithAngle(ptr + 1, tog ^ 1);
        }
        FrameTimerWithArg(3, ptr, RiseFlameTrap);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
    }
}

void SentryTrapSetting()
{
    int idx, k;
    float x = GetObjectX(self);
    if (x == 2508.0)
        idx = 0;
    else if (x == 2535.0)
        idx = 1;
    else
        idx = 2;
    for (k = 0 ; k < 3 ; k ++)
    {
        if (idx == k)
            ObjectOff(Object("SentryTrap" + IntToString(k + 1)));
        else
            ObjectOn(Object("SentryTrap" + IntToString(k + 1)));
    }
}

void YoureWinner()
{
    int plr = GetDirection(GetCaller() + 1), owner = GetOwner(other);

    ObjectOff(self);
    TeleportLocation(16, GetObjectX(self), GetObjectY(self));
    AudioEvent("FlagCapture", 16);
    AudioEvent("AmuletDrop", 16);
    Effect("WHITE_FLASH", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    if (plr >= 0 && IsPlayerUnit(owner))
    {
        UniPrintToAll(PlayerIngameNick(player[plr]) + " 님께서 먼저 레이스를 완주하셨습니다: 승리");
    }
    UniPrint(owner, "축하드립니다, 당신이 승리자 입니다");
    FrameTimer(30, StrYourWinner);
}

void SummonBat()
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 57);
    SummonEffect(CreateObject("InvisibleLightBlueHigh", 57));

    Raise(ptr, ToFloat(GetTrigger()));
    FrameTimerWithArg(40, ptr, RespectBat);
    ObjectOff(self);
}

void RespectBat(int ptr)
{
    int unit;
    
    TeleportLocation(16, GetObjectX(ptr), GetObjectY(ptr));
    unit = CreateObject("Swordsman", 16);
    CreateObject("InvisibleLightBlueHigh", 16);
    SetUnitMaxHealth(unit, 60);
    Raise(unit + 1, ToFloat(ToInt(GetObjectZ(ptr))));
    SetCallback(unit, 5, EnemyDie);
    Delete(ptr);
}

void EnemyDie()
{
    ObjectOn(ToInt(GetObjectZ(GetTrigger() + 1)));
    Delete(GetTrigger() + 1);
}

void SummonEffect(int unit)
{
    if (!GetDirection(unit))
    {
        CastSpellObjectLocation("SPELL_SUMMON_BEHOLDER", unit, GetObjectX(unit), GetObjectY(unit));
        LookWithAngle(unit, 1);
        FrameTimerWithArg(40, unit, SummonEffect);
    }
    else
    {
        TeleportLocation(16, GetObjectX(unit), GetObjectY(unit));
        DeleteObjectTimer(CreateObject("BigSmoke", 16), 9);
        Delete(unit);
    }
}

void PutDownTable(int wp)
{
    int unit = CreateObject("Table4", wp);
    CreateObject("InvisibleLightBlueHigh", wp); //distance
    CreateObject("InvisibleLightBlueHigh", wp); //speed
    Enchant(unit, "ENCHANT_FREEZE", 0.0);
    Frozen(unit, 1);
    FrameTimerWithArg(1, unit, DownTable);
}

void DownTable(int ptr)
{
    int wp;
    float r = GetObjectZ(ptr + 1), boost = GetObjectZ(ptr + 2);
    if (r > 0.0)
    {
        MoveObject(ptr, GetObjectX(ptr) + boost, GetObjectY(ptr) - boost);
        Raise(ptr + 1, r - boost);
    }
    else
    {
        wp = Random(58, 64);
        MoveObject(ptr, GetWaypointX(wp), GetWaypointY(wp));
        Raise(ptr + 1, 288.0);
        Raise(ptr + 2, RandomFloat(1.0, 20.0));
    }
    if (IsObjectOn(ptr))
        FrameTimerWithArg(1, ptr, DownTable);
}

void TableMax(int num)
{
    if (num)
    {
        PutDownTable(Random(58, 64));
        FrameTimerWithArg(20, num - 1, TableMax);
    }
}

void PutSpikeRots(int max)
{
    int k;

    for (k = 0 ; k < max ; k += 1)
    {
        Frozen(CreateObject("RotatingSpikes", 65), TRUE);
        TeleportLocationVector(65, 23.0, 23.0);
    }
}

void DelayElevatorOff(int unit)
{
    ObjectOff(unit);
}

void ElevBridgePart2On()
{
    if (!GetElevatorStatus(ElevBridge[0]))
    {
        ObjectOn(ElevBridge[0]);
        ObjectOn(ElevBridge[1]);
        ObjectOn(ElevBridge[2]);
        FrameTimerWithArg(30, ElevBridge[0], DelayElevatorOff);
        FrameTimerWithArg(30, ElevBridge[1], DelayElevatorOff);
        FrameTimerWithArg(30, ElevBridge[2], DelayElevatorOff);
        SecondTimerWithArg(25, 0, DescentBridge);
    }
}

void ElevBridgePart3On()
{
    if (!GetElevatorStatus(ElevBridge[3]))
    {
        ObjectOn(ElevBridge[3]);
        ObjectOn(ElevBridge[4]);
        ObjectOn(ElevBridge[5]);
        FrameTimerWithArg(30, ElevBridge[3], DelayElevatorOff);
        FrameTimerWithArg(30, ElevBridge[4], DelayElevatorOff);
        FrameTimerWithArg(30, ElevBridge[5], DelayElevatorOff);
        SecondTimerWithArg(15, 3, DescentBridge);
    }
}

void DescentBridge(int pic)
{
    ObjectOn(ElevBridge[pic]);
    ObjectOn(ElevBridge[pic + 1]);
    ObjectOn(ElevBridge[pic + 2]);
    FrameTimerWithArg(30, ElevBridge[pic], DelayElevatorOff);
    FrameTimerWithArg(30, ElevBridge[pic + 1], DelayElevatorOff);
    FrameTimerWithArg(30, ElevBridge[pic + 2], DelayElevatorOff);
}

void MapTrapSet()
{
    int ptr;
    CreateObject("MovableStatue2g", 66);
    CreateObject("MovableStatue2g", 67);
    TeleportLocationVector(66, -3.0, 3.0);
    TeleportLocationVector(67, -3.0, 3.0);
    CreateObject("Skull3", 66);
    CreateObject("Skull3", 67);
    ptr = CreateObject("InvisibleLightBlueHigh", 69) + 1;
    Enchant(CreateObject("MovableStatueVictory1SE", 68), "ENCHANT_FREEZE", 0.0);
    Enchant(CreateObject("MovableStatueVictory1SE", 69), "ENCHANT_FREEZE", 0.0);
    Enchant(CreateObject("MovableStatueVictory1SE", 70), "ENCHANT_FREEZE", 0.0);
    FrameTimerWithArg(1, ptr, ForceOfNatureLoop);
}

void ForceOfNatureLoop(int ptr)
{
    CastSpellObjectLocation("SPELL_FORCE_OF_NATURE", ptr, GetObjectX(ptr) + 23.0, GetObjectY(ptr) + 23.0);
    CastSpellObjectLocation("SPELL_FORCE_OF_NATURE", ptr + 1, GetObjectX(ptr + 1) + 23.0, GetObjectY(ptr + 1) + 23.0);
    CastSpellObjectLocation("SPELL_FORCE_OF_NATURE", ptr + 2, GetObjectX(ptr + 2) + 23.0, GetObjectY(ptr + 2) + 23.0);
    SecondTimerWithArg(5, ptr, ForceOfNatureLoop);
}

int ColorMaiden(int red, int grn, int blue, int wp)
{
    int unit = CreateObject("Bear2", wp);
    int ptr1 = GetMemory(0x750710), k;

    SetMemory(ptr1 + 4, 1385);
    for (k = 0 ; k < 32 ; k ++)
        SetMemory(ptr1 + 0x230 + (k * 4), 0x400);
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x178, 0xa0);
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c, red | (grn << 8) | (blue << 16) | (red << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 4, grn | (blue << 8) | (red << 16) | (grn << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 8, blue | (red << 8) | (grn << 16) | (blue << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 12, red | (grn << 8) | (blue << 16) | (red << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 16, grn | (blue << 8));
    SetUnitVoice(unit, 7);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x8000);
    SetUnitSpeed(unit, 2.3);

    return unit;
}

void StrFinish()
{
	int arr[13];
	string name = "CharmOrb";
	int i = 0;
	arr[0] = 252844414; arr[1] = 1225556257; arr[2] = 172003472; arr[3] = 541608009; arr[4] = 1679828106; arr[5] = 1885489447; arr[6] = 1092915239; arr[7] = 340267537; arr[8] = 604571905; arr[9] = 75793556; 
	arr[10] = 1111641124; arr[11] = 505811460; arr[12] = 66; 
	while(i < 13)
	{
		drawStrFinish(arr[i], name);
		i ++;
	}
}

void drawStrFinish(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = LocationX(72);
		pos_y = LocationY(72);
	}
	for (i = 1 ; i > 0 && count < 403 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 72);
		if (count % 38 == 37)
            TeleportLocationVector(72, -74.0, 2.0);
		else
            TeleportLocationVector(72, 2.0, 0.0);
		count ++;
	}
	if (count >= 403)
	{
		count = 0;
		TeleportLocation(72, pos_x, pos_y);
	}
}

int CheckRightClick(int plr_unit)
{
    return !(CheckPlayerInput(plr_unit) ^ 0x02);
}

void InitiPlayerCamera(int unit)
{
	int ptr = UnitToPtr(unit);

	if (ptr) SetMemory(GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0xe58, 0);
}