
#include "noxscript\builtins.h"

#include "libs\typecast.h"
#include "libs\callmethod.h"
#include "libs\mathlab.h"
#include "libs\unitutil.h"
#include "libs\unitstruct.h"
#include "libs\printutil.h"
#include "libs\fxeffect.h"
#include "libs\waypoint.h"
#include "libs\username.h"


//!test - warHammerBattle

#define MAX_HAMMER_COUNT    8
#define NULLPTR             0
#define MOBMAKE_FUNCTION_COUNT      12

#define TRUE 1

int LastUnitID = 129;
int player[20];
int g_subStone[10]; //stones
int HAMMER[16]; //hammers + cooldown_table
int g_maxMobCount = 32;


int BlackWidowBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1667329090; arr[1] = 1684625259; arr[2] = 30575; arr[17] = 250; arr[18] = 45; 
		arr[19] = 85; arr[21] = 1065353216; arr[23] = 4; arr[24] = 1066192077; arr[26] = 4; 
		arr[27] = 3; arr[28] = 1097859072; arr[29] = 25; arr[31] = 8; arr[32] = 13; 
		arr[33] = 21; arr[34] = 50; arr[35] = 3; arr[36] = 12; arr[37] = 1684631635; 
		arr[38] = 1884516965; arr[39] = 29801; arr[53] = 1128792064; arr[55] = 20; arr[56] = 28; 
		arr[59] = 5544896; arr[61] = 45071360;
        link = &arr;
	}
	return link;
}

void BlackWidowSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1076048691);
		SetMemory(ptr + 0x224, 1076048691);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 4);
		SetMemory(GetMemory(ptr + 0x22c), 250);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 250);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BlackWidowBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int BomberBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1651339074; arr[1] = 29285; arr[17] = 160; arr[18] = 12; arr[19] = 100; 
		arr[21] = 1065353216; arr[23] = 1; arr[24] = 1065353216; arr[26] = 4; arr[37] = 1399876937; 
		arr[38] = 7630696; arr[53] = 1128792064; arr[55] = 8; arr[56] = 14; arr[58] = 5547856; 
		arr[60] = 1348; arr[61] = 46899968;
        link = &arr;
	}
	return link;
}

void BomberSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077936128);
		SetMemory(ptr + 0x224, 1077936128);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 1);
		SetMemory(GetMemory(ptr + 0x22c), 160);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 160);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BomberBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MaidenBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1684627789; arr[1] = 28261; arr[17] = 250; arr[19] = 45; arr[21] = 1065353216; 
		arr[23] = 32768; arr[24] = 1065353216; arr[25] = 3; arr[26] = 9; arr[27] = 5; 
		arr[28] = 1106247680; arr[29] = 35; arr[32] = 9; arr[33] = 17; arr[59] = 5542432; 
		arr[60] = 1385; arr[61] = 46907904; 
		link = &arr;
	}
	return link;
}

void MaidenSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1068289229);
		SetMemory(ptr + 0x224, 1068289229);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 250);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 250);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, MaidenBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int LichLordBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751345484; arr[1] = 1685221196; arr[17] = 325; arr[19] = 100; arr[21] = 1065353216; 
		arr[23] = 2060; arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; 
		arr[28] = 1108082688; arr[29] = 40; arr[30] = 1092616192; arr[32] = 13; arr[33] = 21; 
		arr[57] = 5548288; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

void LichLordSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077936128);
		SetMemory(ptr + 0x224, 1077936128);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 2060);
		SetMemory(GetMemory(ptr + 0x22c), 325);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 325);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, LichLordBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int HecubahWithOrbBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 1466458466; arr[2] = 1332245609; arr[3] = 25202; arr[17] = 320; 
		arr[19] = 80; arr[21] = 1051931443; arr[23] = 67596; arr[24] = 1065353216; arr[26] = 4; 
		arr[27] = 5; arr[37] = 1952539972; arr[38] = 1818313320; arr[39] = 1634879084; arr[40] = 1852140903; 
		arr[41] = 116; arr[53] = 1128792064; arr[55] = 16; arr[56] = 26; arr[60] = 1384; 
		arr[61] = 46914560; 
		link = &arr;
	}
	return link;
}

void HecubahWithOrbSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1075419545);
		SetMemory(ptr + 0x224, 1075419545);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 67596);
		SetMemory(GetMemory(ptr + 0x22c), 320);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 320);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HecubahWithOrbBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1051931443);
	}
}

int FireSpriteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972; arr[17] = 135; arr[19] = 65; 
		arr[21] = 1065353216; arr[23] = 65536; arr[24] = 1065353216; arr[37] = 1769236816; arr[38] = 1181513062; 
		arr[39] = 1650815593; arr[40] = 7105633; arr[53] = 1133903872; arr[55] = 13; arr[56] = 21; 
		arr[58] = 5546320; 
		link = &arr;
	}
	return link;
}

void FireSpriteSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1073322393);
		SetMemory(ptr + 0x224, 1073322393);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 65536);
		SetMemory(GetMemory(ptr + 0x22c), 135);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 135);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FireSpriteBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int GoonBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1852796743; arr[17] = 160; arr[19] = 85; arr[21] = 1065353216; arr[23] = 2056; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 3; arr[28] = 1103626240; arr[29] = 28; 
		arr[30] = 1090519040; arr[31] = 3; arr[32] = 14; arr[33] = 24; arr[34] = 1; 
		arr[35] = 1; arr[36] = 20; arr[57] = 5548368; arr[58] = 5546608; arr[59] = 5544320; 
		link = &arr;
	}
	return link;
}

void GoonSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1076048691);
		SetMemory(ptr + 0x224, 1076048691);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 2056);
		SetMemory(GetMemory(ptr + 0x22c), 160);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 160);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, GoonBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int AirshipCaptainBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1936877889; arr[1] = 1131440488; arr[2] = 1635020897; arr[3] = 28265; arr[17] = 350; 
		arr[19] = 90; arr[21] = 1065353216; arr[23] = 2061; arr[24] = 1065353216; arr[26] = 4; 
		arr[28] = 1101004800; arr[29] = 20; arr[32] = 6; arr[33] = 12; arr[58] = 5546320; 
		arr[59] = 5542784; arr[60] = 1387; arr[61] = 46915328; 
		link = &arr;
	}
	return link;
}

void AirshipCaptainSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1076677837);
		SetMemory(ptr + 0x224, 1076677837);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 2061);
		SetMemory(GetMemory(ptr + 0x22c), 350);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 350);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, AirshipCaptainBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int WeirdlingBeastBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811; arr[17] = 270; 
		arr[19] = 55; arr[21] = 1065353216; arr[24] = 1071225242; arr[26] = 4; arr[28] = 1101004800; 
		arr[29] = 21; arr[31] = 3; arr[32] = 5; arr[33] = 10; arr[59] = 5542784; 
		arr[60] = 1388; arr[61] = 46915072; 
		link = &arr;
	}
	return link;
}

void WeirdlingBeastSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1070805811);
		SetMemory(ptr + 0x224, 1070805811);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 0);
		SetMemory(GetMemory(ptr + 0x22c), 270);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 270);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WeirdlingBeastBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int HecubahBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 6840674; arr[17] = 400; arr[19] = 100; arr[21] = 1065353216; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; arr[28] = 1108082688; 
		arr[29] = 60; arr[30] = 1092616192; arr[31] = 11; arr[32] = 10; arr[33] = 18; 
		arr[57] = 5548288; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

void HecubahSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077936128);
		SetMemory(ptr + 0x224, 1077936128);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 0);
		SetMemory(GetMemory(ptr + 0x22c), 400);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 400);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HecubahBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int GreenFrogBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701147207; arr[1] = 1869760110; arr[2] = 103; arr[17] = 130; arr[19] = 64; 
		arr[21] = 1065353216; arr[23] = 2049; arr[24] = 1066192077; arr[26] = 4; arr[28] = 1103101952; 
		arr[29] = 15; arr[31] = 8; arr[32] = 7; arr[33] = 11; arr[59] = 5542784; 
		arr[60] = 1313; arr[61] = 46905856; 
		link = &arr;
	}
	return link;
}

void GreenFrogSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1073070735);
		SetMemory(ptr + 0x224, 1073070735);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 2049);
		SetMemory(GetMemory(ptr + 0x22c), 130);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 130);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, GreenFrogBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void BomberSetMonsterCollide(int bombUnit)
{
    int ptr = UnitToPtr(bombUnit);

    if (ptr)
        SetMemory(ptr + 0x2b8, 0x4e83b0);
}

int MobClassBlackSpider(int sUnit)
{
    int mob = CreateObjectAt("BlackWidow", GetObjectX(sUnit), GetObjectY(sUnit));

    BlackWidowSubProcess(mob);
    return mob;
}

int MobClassJandor(int sUnit)
{
    int mob = CreateObjectAt("AirshipCaptain", GetObjectX(sUnit), GetObjectY(sUnit));

    AirshipCaptainSubProcess(mob);
    return mob;
}

int MobClassBeast(int sUnit)
{
    int mob = CreateObjectAt("WeirdlingBeast", GetObjectX(sUnit), GetObjectY(sUnit));

    WeirdlingBeastSubProcess(mob);
    return mob;
}

int MobClassHecubah(int sUnit)
{
    int mob = CreateObjectAt("Hecubah", GetObjectX(sUnit), GetObjectY(sUnit));

    HecubahSubProcess(mob);
    return mob;
}

int MobClassCrazyGirl(int sUnit)
{
    int mob = ColorMaidenAt(35, 27, 31, GetObjectX(sUnit), GetObjectY(sUnit));

    MaidenSubProcess(mob);
    return mob;
}

int MobClassGoon(int sUnit)
{
    int mob = CreateObjectAt("Goon", GetObjectX(sUnit), GetObjectY(sUnit));

    GoonSubProcess(mob);
    return mob;
}

int MobClassBomber(int sUnit)
{
    string bombName[] = {"Bomber", "BomberBlue", "BomberGreen", "BomberYellow"};
    int mob = CreateObjectAt(bombName[Random(0, 3)], GetObjectX(sUnit), GetObjectY(sUnit));

    BomberSetMonsterCollide(mob);
    BomberSubProcess(mob);
    return mob;
}

int MobClassLichLord(int sUnit)
{
    int mob = CreateObjectAt("LichLord", GetObjectX(sUnit), GetObjectY(sUnit));

    LichLordSubProcess(mob);
    return mob;
}

int MobClassOrbHecubah(int sUnit)
{
    int mob = CreateObjectAt("HecubahWithOrb", GetObjectX(sUnit), GetObjectY(sUnit));

    HecubahWithOrbSubProcess(mob);
    return mob;
}

int MobClassFireSprite(int sUnit)
{
    int mob = CreateObjectAt("FireSprite", GetObjectX(sUnit), GetObjectY(sUnit));

    FireSpriteSubProcess(mob);
    return mob;
}

int MobClassFrog(int sUnit)
{
    int mob = CreateObjectAt("GreenFrog", GetObjectX(sUnit), GetObjectY(sUnit));

    GreenFrogSubProcess(mob);
    return mob;
}

int MobClassEmberDemon(int sUnit)
{
    string uName[] = {"MeleeDemon", "MeleeDemon", "EmberDemon"};
    int mob = CreateObjectAt(uName[Random(0, 2)], GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 185);
    return mob;
}

int MobClassOgre(int sUnit)
{
    string uName[] = {"GruntAxe", "OgreBrute", "OgreWarlord", "Scorpion", "Shade"};
    int mob = CreateObjectAt(uName[Random(0, 4)], GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, (CurrentHealth(mob) * 25) / 10);
    return mob;
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

void MapStartGuideMessage()
{
    InitMapPickets();
    UniPrintToAll("지금부터 최강의 해머대전이 펼쳐집니다!\n과연 누가 더 강력한 해머를 소유하게 될 지 벌써부터 궁굼하네요!");
    FrameTimer(60, StartMent);
}

void MobMakeStandBy()
{
    int mobMake1 = MobMakeClassInit(GetWaypointX(7), GetWaypointY(7), g_maxMobCount, 30);

    Raise(mobMake1 + 1, MobClassBeast);
    Raise(mobMake1 + 2, MobClassBlackSpider);
    Raise(mobMake1 + 3, MobClassBomber);
    Raise(mobMake1 + 4, MobClassCrazyGirl);
    Raise(mobMake1 + 5, MobClassEmberDemon);
    Raise(mobMake1 + 6, MobClassFireSprite);
    Raise(mobMake1 + 7, MobClassFrog);
    Raise(mobMake1 + 8, MobClassOgre);
    Raise(mobMake1 + 9, MobClassLichLord);
    Raise(mobMake1 + 10, MobClassOrbHecubah);
    Raise(mobMake1 + 11, MobClassJandor);
    Raise(mobMake1 + 12, MobClassGoon);
    SecondTimerWithArg(1, mobMake1, MobMakeClassStartInitSum);
    UniPrintToAll("MobMake 생성완료");
}

void MapInitialize()
{
    MusicEvent();
    InitMathSine(1);
    
    VoiceList(0);
    FrameTimer(1, initializeUsemapSetting);
    FrameTimer(2, InitSpells);
    FrameTimer(10, LoopPreservePlayers);
    FrameTimer(10, LoopHitHammer);
    FrameTimer(10, loopDecreaseCooldown);
    FrameTimer(10, LoopHammerRespawn);
    FrameTimerWithArg(50, 0, MapStartGuideMessage);
    FrameTimerWithArg(80, CreateObject("InvisibleLightBlueLow", 39), CheckAdvanceHammerLoop);
    SecondTimer(3, InitModePicket);
}

void initializeUsemapSetting()
{
    int k;
    for (k = 9 ; k >= 0 ; k --)
    {
        g_subStone[k] = CreateObject("CaveRocksPebbles", k + 8);
    }
    SetHostileCritter();
    FrameTimerWithArg(10, 10, ChakramEffect);
}

void ChakramEffect(int num)
{
    int unit;
    if (num)
    {
        unit = CreateObject("RoundChakramInMotion", 51);
        Frozen(unit, true);
        CreateMoverFix(unit, 52, 20.0);
        FrameTimerWithArg(30, num - 1, ChakramEffect);
    }
}

void MapExit()
{
    ResetHostileCritter();
    MusicEvent();
}

string HammerEnchants(int index)
{
    string name[] =
    {
        "ENCHANT_INFRAVISION", "ENCHANT_SLOWED", "ENCHANT_SHOCK", "ENCHANT_VAMPIRISM", "ENCHANT_TELEKINESIS",
        "ENCHANT_AFRAID", "ENCHANT_ANCHORED", "ENCHANT_RUN"};

    return name[index];
}

string GuideMessage(int num)
{
    string table[] =
    {
        "게임의 목표_ 맵 곳곳에 배치된 워해머를 사용하여 상대 플레이어를 죽이세요, 가장 많이 킬을 낸 플레이어가 승리합니다",
        "이 맵은 각각의 워해머 마다 고유한 마법능력이 들어있습니다, 좋은 해머를 차지하는 것이 빠른 승리의 지름길입니다",
        "게임 팁_ 워해머의 중복소유 방지를 위해서는 게임 설정(Home버튼)에서 '무기 및 갑옷 되살리기' 기능을 활성화 하십시오",
        "게임 팁_ 무기 및 갑옷 되살리기 기능이 활성화 된 상태에서 바닥에 워해머를 내려놓을 경우 잠시 후 원래 위치로 옮겨집니다",
        "이 맵은 마법사와 소환술사 입장을 허용하지만 그들의 무기나 갑옷은 전혀 없기 때문에 정상적인 플레이는 힘들 것입니다",
        "알림_ 최신 맵에 대한 정보는 blog.daum.net/ky10613 에서 확인하실 수 있습니다"
    };
    return table[num];
}

void StartMent(int idx)
{
    if (idx < 6)
    {
        UniPrintToAll(GuideMessage(idx));
        SecondTimerWithArg(5, idx + 1, StartMent);
    }
    else
    {
        HallOfFame();
    }
}

void getPlayers()
{
    int k, plr;

    while (true)
    {
        if (CurrentHealth(other))
        {
            plr = CheckPlayer();
            for (k = 9 ; k >= 0 && plr < 0 ; k -= 1)
            {
                if (!MaxHealth(player[k]))
                {
                    player[k] = GetCaller();
                    player[k + 10] = 1;
                    UniPrintToAll(PlayerIngameNick(player[k]) + " 이 전투에 참가했습니다");
                    plr = k;
                    break;
                }
            }
            if (plr >= 0)
            {
                PlayerJoinMap(plr);
                break;
            }
        }
        CantMapPlay();
        break;
    }
}

void PlayerJoinMap(int plr)
{
    int wp = Random(55, 58);

    MoveObject(player[plr], GetWaypointX(wp), GetWaypointY(wp));
    Effect("RICOCHET", GetWaypointX(wp), GetWaypointY(wp), 0.0, 0.0);
    AudioEvent("HecubahDieFrame439", wp);
}

void CantMapPlay()
{
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    MoveObject(other, GetWaypointX(59), GetWaypointY(59));
    UniPrint(other, "맵 로드 중 문제발생, 죄송합니다, 이 맵은 구동될 수 없습니다");
    UniPrint(other, "이 맵은 현재 녹스버전에서는 지원되지 않습니다, blog.naver.com/ky10613 메인페이지를 방문하여 이 문제를 해결하세요");
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

void SetHammerPIndex(int plr)
{
    int inv = GetLastItem(player[plr]);

    if (HasSubclass(inv, "HAMMER"))
    {
        if (plr ^ GetDirection(inv))
        {
            LookWithAngle(inv, plr);
        }
    }
}

int PlayerClassCheckDeathFlag(int plr)
{
    return player[plr + 10] & 2;
}

void PlayerClassSetDeathFlag(int plr)
{
    player[plr + 10] ^= 2;
}

void PlayerClassOnAlive(int plr, int pUnit)
{
    SetHammerPIndex(plr);
    MoveObject(g_subStone[plr], GetObjectX(pUnit) + UnitAngleCos(pUnit, 30.0), GetObjectY(pUnit) + UnitAngleSin(pUnit, 30.0));
}

void PlayerClassOnDeath(int plr)
{
    UniPrintToAll(PlayerIngameNick(player[plr]) + " 님께서 격추되었습니다");
}

void PlayerClassOnExit(int plr)
{
    player[plr] = 0;
    player[plr + 10] = 0;
}

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

void LoopHitHammer()
{
    int k, plr;
    
    for (k = 7 ; k >= 0 ; k -= 1)
    {
        if (CurrentHealth(HAMMER[k]) ^ MaxHealth(HAMMER[k]))
        {
            RestoreHealth(HAMMER[k], 100);
            plr = GetDirection(HAMMER[k]);
            if (HasItem(player[plr], HAMMER[k]))
                actionReceived(plr, k);
        }
    }
    FrameTimer(1, LoopHitHammer);
}

void actionReceived(int arg_0, int arg_1)
{
    if (!HAMMER[arg_1 + MAX_HAMMER_COUNT] && CurrentHealth(player[arg_0]))
    {
        MoveWaypoint(35, GetObjectX(player[arg_0]) + UnitAngleCos(player[arg_0], 30.0), GetObjectY(player[arg_0]) + UnitAngleSin(player[arg_0], 30.0));
        CallFunctionWithArg(FuncTable(arg_1), arg_0);
        HAMMER[arg_1 + MAX_HAMMER_COUNT] = FuncCooldown(arg_1);
    }
}

int FuncTable(int num)
{
    int functions[] = {StraightScrew, earthQuakeVolcanoSword, ShotArrowHammer, summoningHammer,
        firewindHammer, CastFistLine, CastTripleScrew, WhiteCircle};

    return functions[num];
}

int FuncCooldown(int num)
{
    int cool[8] = {1, 5, 4, 4, 3, 4, 3, 3};

    return cool[num] * 30;
}

void InitSpells()
{
    FuncTable(0);
    FuncCooldown(0);
}

void loopDecreaseCooldown()
{
    int k;

    for (k = 7 ; k >= 0 ; k --)
    {
        if (HAMMER[k + MAX_HAMMER_COUNT])
            HAMMER[k + MAX_HAMMER_COUNT] --;
    }
    FrameTimer(1, loopDecreaseCooldown);
}

void WhiteCircle(int plr)
{
    int ptr, owner = player[plr];

    ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
    GreenExplosion(GetObjectX(ptr), GetObjectY(ptr));
    CastSpellObjectObject("SPELL_PUSH", owner, owner);
    CastSpellObjectObject("SPELL_PUSH", owner, owner);
    SplashDamageAt(player[plr], 145, 180.0, GetObjectX(ptr), GetObjectY(ptr));
    DeleteObjectTimer(ptr, 150);
}

void StraightScrew(int plr)
{
    float pos_x = UnitAngleCos(player[plr], 40.0), pos_y = UnitAngleSin(player[plr], 40.0);
    int k, ptr;

    MoveWaypoint(27, GetObjectX(player[plr]) + pos_x, GetObjectY(player[plr]) + pos_y);
    ptr = CreateObject("InvisibleLightBlueHigh", 27) + 1;
    LookWithAngle(ptr - 1, 0);
    Raise(ptr - 1, ScrewTouched);
    for (k = 0 ; k < 16 ; k += 1)
    {
        if (IsVisibleTo(ptr - 1, player[plr]))
        {
            PutRisk(ptr - 1, plr, ToInt(GetObjectZ(ptr - 1)));
            DeleteObjectTimer(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(ptr - 1), GetObjectY(ptr - 1)), 30);
            LookWithAngle(ptr - 1, GetDirection(ptr - 1) + 1);
            MoveObject(ptr - 1, GetObjectX(ptr - 1) + pos_x, GetObjectY(ptr - 1) + pos_y);
        }
        else
            break;
    }
    YellowThunderFx(ptr - 1);
    Delete(ptr - 1);
    AudioEvent("FireballExplode", 27);
    AudioEvent("BeholderMove", 27);
}

void YellowThunderFx(int ptr)
{
    int max = GetDirection(ptr), unit = ptr + 2, i;

    if (max)
    {
        for (i = 0 ; i < max - 1 ; i += 1)
            FrameTimerWithArg(1, unit + (i * 2), StartYellowLightning);
    }
}

int PutRisk(int ptr, int plr, int func)
{
    int unit = CreateObjectAt("Maiden", GetObjectX(ptr), GetObjectY(ptr));

    Frozen(unit, 1);
    LookWithAngle(unit, plr);
    DeleteObjectTimer(unit, 1);
    SetCallback(unit, 9, func);
    return unit;
}

void ScrewTouched()
{
    int plr = GetDirection(self);

    if (CurrentHealth(other) && IsAttackedBy(other, player[plr]))
    {
        Damage(other, player[plr], 80, 1);
        Enchant(other, "ENCHANT_CHARMING", 0.6);
    }
}

void CollideObstacleAnkh()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 4, 14);
    }
}

int ObstacleAnkh(int sOwner, float sX, float sY)
{
    int unit = CreateObjectAt("Ankh", sX, sY);

    SetOwner(sOwner, unit);
    SetUnitFlags(unit, GetUnitFlags(unit) ^ 0x10);
    SetUnitCallbackOnCollide(unit, CollideObstacleAnkh);
    return unit;
}

void earthQuakeVolcanoSword(int plr)
{
    int ptr, k;
    int owner = player[plr];
    float xProfile = GetObjectX(owner), yProfile = GetObjectY(owner);

    ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
    LookWithAngle(ptr, 80);
    Raise(ptr, ToFloat(plr));
    for (k = 0 ; k < 90 ; k += 1)
        ObstacleAnkh(owner, xProfile + MathSine(k * 4 + 90, 60.0), yProfile + MathSine(k * 4, 60.0));
    spreadBlocks(ptr);
}

void spreadBlocks(int ptr)
{
    int plr = ToInt(GetObjectZ(ptr)), k;

    if (CurrentHealth(player[plr]) && GetDirection(ptr))
    {
        for (k = 0 ; k < 90 ; k += 1)
            MoveObject(ptr + k + 1, GetObjectX(ptr + k + 1) + UnitRatioX(ptr + k + 1, ptr, 2.3), GetObjectY(ptr + k + 1) + UnitRatioY(ptr + k + 1, ptr, 2.3));
        LookWithAngle(ptr, GetDirection(ptr) - 1);
        FrameTimerWithArg(1, ptr, spreadBlocks);
    }
    else
    {
        Delete(ptr);
        for (k = 0 ; k < 90 ; k += 1)
            Delete(ptr + k + 1);
    }
}

void ShotArrowHammer(int arg_0)
{
    float pos_x = UnitAngleCos(player[arg_0], -21.0), pos_y = UnitAngleSin(player[arg_0], -21.0);
    float multiple = 0.1;

    SpecialMissile(player[arg_0], GetObjectX(player[arg_0]) - pos_x, GetObjectY(player[arg_0]) - pos_y, 9.0);

    while(multiple < 0.4)
    {
        SpecialMissile(player[arg_0], (multiple * pos_y) + GetObjectX(player[arg_0]) - pos_x, (-multiple * pos_x) + GetObjectY(player[arg_0]) - pos_y, 9.0);
        SpecialMissile(player[arg_0],  (-multiple * pos_y) + GetObjectX(player[arg_0]) - pos_x, (multiple * pos_x) + GetObjectY(player[arg_0]) - pos_y, 9.0);
        multiple += 0.1;
    }
    Enchant(player[arg_0], "ENCHANT_SHOCK", 10.0);
}

void summoningHammer(int plr)
{
    int ptr;
    float pos_x, pos_y;
    if (CurrentHealth(player[plr]))
    {
        pos_x = UnitAngleCos(player[plr], 20.0);
        pos_y = UnitAngleSin(player[plr], 20.0);
        MoveWaypoint(36, GetObjectX(player[plr]) + pos_x, GetObjectY(player[plr]) + pos_y);
        ptr = CreateObject("WeirdlingBeast", 36);
        SetOwner(player[plr], ptr);
        LookWithAngle(ptr, GetDirection(player[plr]));
        CreatureGuard(ptr, GetObjectX(ptr), GetObjectY(ptr), GetObjectX(ptr) + pos_x, GetObjectY(ptr) + pos_y, 420.0);
        DeleteObjectTimer(ptr, 1);
        SetCallback(ptr, 3, AutoTargetDeathRay);
        AudioEvent("ForceFieldCast", 36);
        setDrawingStar(-1);
    }
    
}

void AutoTargetDeathRay()
{
    CastSpellObjectLocation("SPELL_DEATH_RAY", self, GetObjectX(other), GetObjectY(other));
}

void setDrawingStar(int num)
{
    float pos_x[5], pos_y[5];

    if (num < 0)
    {
        pos_x[0] = 1.0;
        pos_y[0] = 0.0;
        pos_x[1] = MathSine(54, -1.0);
        pos_y[1] = MathSine(54 + 90, 1.0);
        pos_x[2] = MathSine(72 + 90, 1.0);
        pos_y[2] = MathSine(72, -1.0);
        pos_x[3] = MathSine(72 + 90, 1.0);
        pos_y[3] = MathSine(72, 1.0);
        pos_x[4] = MathSine(36 + 90, -1.0);
        pos_y[4] = MathSine(36, -1.0);
        FrameTimerWithArg(1, num + 1, setDrawingStar);
    }
    else if (num < 5)
    {
        drawingStar(pos_x[num], pos_y[num]);
        FrameTimerWithArg(1, num + 1, setDrawingStar);
    }
}
void drawingStar(float x, float y)
{
    int k;

    for (k = 49 ; k >= 0 ; k --)
    {
        DeleteObjectTimer(CreateObject("HealOrb",36), 25);
        MoveWaypoint(36, GetWaypointX(36) + x, GetWaypointY(36) + y);
    }
}
void summonHorrendous(int arg_0)
{
    int var_0[2], var_1 = 0;
    if (!HasEnchant(var_0[0], "ENCHANT_VAMPIRISM") && !HasEnchant(var_0[1],"ENCHANT_VAMPIRISM"))
    {
        MoveWaypoint(1, IntToFloat(arg_0) + 300.0 ,300.0);
        while(var_1 < 2) {
            var_0[var_1] = CreateObject("Horrendous", 37);
            SetUnitMaxHealth(var_0[var_1], 1000);
            RetreatLevel(var_0[var_1], 0.0);
            ResumeLevel(var_0[var_1], 0.0);
            Enchant(var_0[var_1], "ENCHANT_VAMPIRISM", 0.0);
            Enchant(var_0[var_1], "ENCHANT_HASTED", 0.0);
            SetCallback(var_0[var_1], 5, setDeathHorrenous);
            SetOwner(player[arg_0],var_0[var_1]);
            Effect("SMOKE_BLAST", GetObjectX(var_0[var_1]), GetObjectY(var_0[var_1]), 0.0, 0.0);
            FrameTimerWithArg(1, var_0[var_1], setUnitAi);
            var_1 += 1;
        }
    }
    else
    {
        while(var_1 < 2)
        {
            if (CurrentHealth(var_0[var_1]))
            {
                if (IsAttackedBy(var_0[var_1],player[arg_0]))
                {
                    SetOwner(player[arg_0], var_0[var_1]);
                }
                MoveObject(var_0[var_1], GetWaypointX(37), GetWaypointY(37));
                Effect("SMOKE_BLAST", GetObjectX(var_0[var_1]), GetObjectY(var_0[var_1]), 0.0, 0.0);
                CastSpellObjectLocation(
                    "SPELL_MAGIC_MISSILE", 
                    var_0[var_1], 
                    GetObjectX(var_0[var_1]) + UnitAngleCos(var_0[var_1], 30.0), 
                    GetObjectY(var_0[var_1]) + UnitAngleSin(var_0[var_1], 30.0));
            }
            var_1 += 1;
        }
    }
}
void setDeathHorrenous()
{
    int mis;
    if (CurrentHealth(player[3]))
    {
        CastSpellObjectLocation("SPELL_METEOR_SHOWER", player[3], GetObjectX(self), GetObjectY(self));
        MoveWaypoint(39, GetObjectX(self), GetObjectY(self));
        mis = CreateObject("Deathball", 39);
        SetOwner(player[3], mis);
        PushObject(mis, -10.0, GetObjectX(player[3]), GetObjectY(player[3]));
    }
    DeleteObjectTimer(self, 10);
}

void setUnitAi(int arg_0)
{
    int var_0;
    var_0 = FloatToInt(GetWaypointX(1) - 300.0);
    if (CurrentHealth(player[var_0]) > 0) {
        UniChatMessage(arg_0, " 나는 당신을 지켜드릴 겁니다 !!... by. 호렌더스", 90);
        CreatureFollow(arg_0, player[var_0]);
        AggressionLevel(arg_0, 0.83);
    }
}
void firewindHammer(int arg_0)
{
    float pos_x, pos_y;
    int rep, ptr;

    if (CurrentHealth(player[arg_0]) && rep < 30)
    {
        if (!rep)
        {
            pos_x = UnitAngleCos(player[arg_0], 8.0);
            pos_y = UnitAngleSin(player[arg_0], 8.0);
            MoveWaypoint(38, GetObjectX(player[arg_0]) + pos_x, GetObjectY(player[arg_0]) + pos_y);
            MoveWaypoint(39, GetObjectX(player[arg_0]) + pos_x, GetObjectY(player[arg_0]) + pos_y);
        }
        ptr = CreateObject("Fireball", 38);
        AudioEvent("BeholderMove", 38);
        DeleteObjectTimer(ptr, 30);
        SetOwner(player[arg_0], ptr);
        PushObject(ptr, -25.0, GetObjectX(ptr) + pos_x, GetObjectY(ptr) + pos_y);
        CreateObject("Fireball", 39);
        DeleteObjectTimer(ptr + 1, 30);
        SetOwner(player[arg_0], ptr + 1);
        PushObject(ptr + 1, -25.0, GetObjectX(ptr + 1) + pos_x, GetObjectY(ptr + 1) + pos_y);
        MoveWaypoint(38, GetWaypointX(38) - (pos_y * 2.0), GetWaypointY(38) + (pos_x * 2.0));
        MoveWaypoint(39, GetWaypointX(39) + (pos_y * 2.0), GetWaypointY(39) - (pos_x * 2.0));
        rep ++;
        FrameTimerWithArg(1, arg_0, firewindHammer);
    }
    else
        rep = 0;
}

int CreateFistSingle(int location, int owner)
{
    int fist = CreateObjectAt("Wizard", LocationX(location), LocationY(location));

    SetOwner(owner, fist);
    CastSpellObjectObject("SPELL_FIST", fist, fist);
    Delete(fist);
}

void CastFistLine(int plr)
{
    int k, ptr;
    float pos_x = UnitAngleCos(player[plr], 30.0), pos_y = UnitAngleSin(player[plr], 30.0);

    PushObject(player[plr], 70.0, GetObjectX(player[plr]) + pos_x, GetObjectY(player[plr]) + pos_y);
    MoveWaypoint(40, GetObjectX(player[plr]) + pos_x, GetObjectY(player[plr]) + pos_y);
    ptr = CreateObject("InvisibleLightBlueHigh", 40);
    for (k = 0 ; k < 15 ; k += 1)
    {
        // CreateObject("wizard", 40); //+1
        // SetOwner(player[plr], ptr + (k * 2) + 1);
        // CastSpellObjectLocation("SPELL_FIST", ptr + (k * 2) + 1, GetObjectX(ptr + (k * 2) + 1), GetObjectY(ptr + (k * 2) + 1)); //+2
        CreateFistSingle(40, player[plr]);
        TeleportLocationVector(40, pos_x, pos_y);
        // MoveWaypoint(40, GetWaypointX(40) + pos_x, GetWaypointY(40) + pos_y);
        // Delete(ptr + (k*2)+1);
    }
    Enchant(player[plr], "ENCHANT_INVISIBLE", 5.0);
}

int SpawnDummyUnitAt(string sName, float sX, float sY)
{
    int dUnit = CreateObjectAt(sName, sX, sY);

    if (dUnit)
    {
        ObjectOff(dUnit);
        Damage(dUnit, 0, MaxHealth(dUnit) + 1, -1);
        Frozen(dUnit, 1);
    }
    return dUnit;
}

void SingleStraightFx(int sUnit, int sPoint, int sOwner)
{
    int dUnit;

    if (IsObjectOn(sUnit))
    {
        if (IsVisibleTo(sUnit, sPoint) || IsVisibleTo(sPoint, sUnit))
        {
            dUnit = SpawnDummyUnitAt("Demon", GetObjectX(sUnit), GetObjectY(sUnit));
            SetOwner(sOwner, dUnit);
            SetCallback(dUnit, 9, TripleTouched);
            DeleteObjectTimer(dUnit, 1);
            PlaySoundAround(dUnit, 167);
            GreenSparkAt(GetObjectX(sUnit), GetObjectY(sUnit));
            MoveObject(sUnit, GetObjectX(sUnit) + UnitRatioX(sUnit, sPoint, 28.0), GetObjectY(sUnit) + UnitRatioY(sUnit, sPoint, 28.0));
        }
        else
            Delete(sUnit);
    }
}

void TripleScrewPunch(int sUnit)
{
    int amount = GetDirection(sUnit), owner = GetOwner(sUnit);

    while (IsObjectOn(sUnit))
    {
        if (CurrentHealth(owner) && amount)
        {
            SingleStraightFx(sUnit + 1, sUnit, owner);
            SingleStraightFx(sUnit + 2, sUnit, owner);
            SingleStraightFx(sUnit + 3, sUnit, owner);
            LookWithAngle(sUnit, amount - 1);
            FrameTimerWithArg(1, sUnit, TripleScrewPunch);
            break;
        }
        Delete(sUnit);
        Delete(sUnit + 1);
        Delete(sUnit + 2);
        Delete(sUnit + 3);
        break;
    }
}

void CastTripleScrew(int plr)
{
    int owner = player[plr], unit;
    float xVect = UnitAngleCos(owner, -30.0), yVect = UnitAngleSin(owner, -30.0);
    float xProfile = GetObjectX(owner), yProfile = GetObjectY(owner);

    if (CurrentHealth(owner))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", xProfile, yProfile);
        CreateObjectAt("InvisibleLightBlueLow", xProfile - xVect, yProfile - yVect);
        CreateObjectAt("InvisibleLightBlueLow", 0.1 * yVect + (xProfile - xVect), -0.1 * xVect + (yProfile - yVect));
        CreateObjectAt("InvisibleLightBlueLow", -0.1 * yVect + (xProfile - xVect), 0.1 * xVect + (yProfile - yVect));
        LookWithAngle(unit, 32);
        SetOwner(owner, unit);
        FrameTimerWithArg(1, unit, TripleScrewPunch);
    }
}

void GreenSparkAt(float sX, float sY)
{
    int fxunit = CreateObjectAt("MonsterGenerator", sX, sY);

    Damage(fxunit, 0, 200, 14);
    Delete(fxunit);
}

void GreenSpark(int wp)
{
    int ptr = CreateObject("MonsterGenerator", wp);
    Damage(ptr, 0, 200, 14);
    Delete(ptr);
}

void TripleTouched()
{
    if (HasClass(other,"PLAYER") || HasClass(other,"MONSTER"))
    {
        if (IsAttackedBy(other,self) && !HasEnchant(other,"ENCHANT_VILLAIN"))
        {
            Enchant(other,"ENCHANT_VILLAIN", 0.4);
            Enchant(other, "ENCHANT_FREEZE", 2.0);
            Damage(other, self, 100, 14);
        }
    }
}
void castExplosion(int arg_0)
{
    int var_0, var_1, var_2;
    float dx, dy;
    if (var_0 < 200 && CurrentHealth(player[arg_0]))
    {
        dx = RandomFloat(0.0, 300.0);
        dy = RandomFloat(0.0, 300.0 - dx);
        var_1 = Random(0,3);
        if (var_1 == 0)
        {
            dx *= -1.0;
        }
        else if (var_1 == 1)
        {
            dy *= -1.0;
        }
        else if (var_1 == 2)
        {
            dx *= -1.0;
            dy *= -1.0;
        }
        MoveWaypoint(45, GetObjectX(player[arg_0]) + dx, GetObjectY(player[arg_0]) + dy);
        var_2 = CreateObject("CarnivorousPlant", 45);
        Frozen(var_2, 1);
        SetOwner(player[arg_0], var_2);
        SetCallback(var_2, 9, explosionTouched);
        DeleteObjectTimer(var_2, 1);
        var_2 = CreateObject("MediumFireBoom", 45);
        DeleteObjectTimer(var_2, 10);
        AudioEvent("HammerMissing", 45);
        var_0 += 1;
        FrameTimerWithArg(1, arg_0, castExplosion);
    }
    else
        var_0 = 0;
}
void explosionTouched()
{
    if (HasClass(other,"PLAYER") || HasClass(other,"MONSTER"))
    {
        if (IsAttackedBy(other, self))
            Damage(other, self, 100, 15);
    }
}

void ResetHostileCritter()
{
	SetMemory(0x833e64, 0x55b);		//CarnivorousPlant
	//SetMemory(0x833e70, 1329);		//FishBig
	//SetMemory(0x833e74, 1330);		//FishSmall
	//SetMemory(0x833e78, 1359);		//Rat
	SetMemory(0x833e7c, 1313);		//GreenFrog
}

void SetHostileCritter()
{
	SetMemory(0x833e64, 0x540);		//CarnivorousPlant
	//SetMemory(0x833e70, 0x540);		//FishBig
	//SetMemory(0x833e74, 0x540);		//FishSmall
	//SetMemory(0x833e78, 0x540);		//Rat
	SetMemory(0x833e7c, 0x540);		//GreenFrog
}

void LoopHammerRespawn()
{
    int idx, unit;
    string berf = HammerEnchants(idx);

    if (IsObjectOn(HAMMER[idx]))
        1;
    else
    {
        Delete(HAMMER[idx] + 1);
        unit = CreateObject("WarHammer", idx + 18);
        CreateObject("InvisibleLightBlueHigh", idx + 18);
        Enchant(unit, berf, 0.0);
        Enchant(unit, "ENCHANT_PROTECT_FROM_MAGIC", 0.0);
        if (idx == 4)
        {
            FrameTimerWithArg(1, unit, HammerEffect);
        }
        HAMMER[idx] = unit;
    }
    idx = (idx + 1) % 8;
    FrameTimer(1, LoopHammerRespawn);
}

void HammerEffect(int unit)
{
	int fx;

	if (IsObjectOn(unit))
	{
		fx = ToInt(GetObjectZ(unit + 1));
		if (GetOwner(unit))
		{
			if (IsObjectOn(fx))
				Delete(fx);
		}
		else
		{
			if (!IsObjectOn(fx))
			{
				MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
				MoveObject(unit + 1, GetObjectX(unit), GetObjectY(unit));
				Raise(unit + 1, ToFloat(CreateObject("Meteor", 1)));
				Frozen(ToInt(GetObjectZ(unit + 1)), 1);
			}
		}
		FrameTimerWithArg(1, unit, HammerEffect);
	}
	else
	{
        Delete(ToInt(GetObjectZ(unit + 1)));
		Delete(unit + 1);
	}
}

int ListClassGetPrevNode(int sCur)
{
    return GetOwner(sCur);
}

int ListClassGetNextNode(int sCur)
{
    return ToInt(GetObjectZ(sCur));
}

void ListClassSetPrevNode(int sCur, int sTarget)
{
    SetOwner(sTarget, sCur);
}

void ListClassSetNextNode(int sCur, int sTarget)
{
    Raise(sCur, sTarget);
}

void ListClassAddList(int sHead, int sNew, int sData)
{
    int tNode = ListClassGetNextNode(sHead);

    if (IsObjectOn(tNode))
        ListClassSetNextNode(sNew, tNode);
    ListClassSetNextNode(sHead, sNew);
    SetUnit1C(sNew, sData);
}

void MobMakeClassUnitDeath()
{
    int sUnit = GetOwner(GetTrigger() + 1);
    int sNew = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(self), GetObjectY(self));

    ListClassAddList(sUnit, sNew, GetTrigger());
    MoveObject(GetTrigger() + 1, GetObjectX(self), GetObjectY(self));
    if (GetDirection(sUnit) ^ GetDirection(sUnit + 1))
        LookWithAngle(sUnit, GetDirection(sUnit + 1));
}

int MobMakeClassCreate(int sHeadNode, int sUnit, object sFunc)
{
    int unit = CallFunctionWithArgInt(sFunc, sUnit);

    if (unit)
    {
        SetCallback(unit, 5, MobMakeClassUnitDeath);
        RetreatLevel(unit, 0.0);
        AggressionLevel(unit, 1.0);
        SetOwner(sHeadNode, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)));
    }
    return unit;
}

void MobMakeClassRespawn(int sCurNode)
{
    int dNode, headNode, func;

    if (IsObjectOn(sCurNode))
    {
        dNode = GetUnit1C(sCurNode);
        headNode = GetOwner(dNode + 1);
        if (IsObjectOn(dNode + 1))
        {
            func = ToInt(GetObjectZ(headNode + Random(1, MOBMAKE_FUNCTION_COUNT)));
            if (func)
                MobMakeClassCreate(headNode, sCurNode, func);
            FrameTimerWithArg(1, ListClassGetNextNode(sCurNode), MobMakeClassRespawn);
            if (MaxHealth(dNode))
                Delete(dNode);
            Delete(dNode + 1);
        }
        Delete(sCurNode);
    }
}

void MobMakeClassLoop(int sUnit)
{
    int count = GetDirection(sUnit);

    if (IsObjectOn(sUnit))
    {
        if (count)
        {
            LookWithAngle(sUnit, count - 1);
        }
        else
        {
            MobMakeClassRespawn(ListClassGetNextNode(sUnit));
            LookWithAngle(sUnit, GetDirection(sUnit + 1));
        }
        SecondTimerWithArg(1, sUnit, MobMakeClassLoop);
    }
}

void MobMakeClassStartInitSum(int sUnit)
{
    int count = GetDirection(sUnit);

    if (IsObjectOn(sUnit))
    {
        if (count)
        {
            object function = ToInt(GetObjectZ(sUnit + Random(1, MOBMAKE_FUNCTION_COUNT)));

            if (function)
                MobMakeClassCreate(sUnit, sUnit, function);
            else
                UniPrintToAll("mobMake 클래스 생성 에러");
            LookWithAngle(sUnit, count - 1);
            FrameTimerWithArg(1, sUnit, MobMakeClassStartInitSum);
        }
        else
            MobMakeClassLoop(sUnit);
    }
    else
        UniPrintToAll("mobMake 클래스 유효성 에러!");
}

int MobMakeClassInit(float sX, float sY, int sAmount, int sRespTime)
{
    string mobMakeName = "AmbBeachBirds";
    int mobMake = CreateObjectAt(mobMakeName, sX, sY);

    LookWithAngle(CreateObjectAt(mobMakeName, sX, sY), sRespTime);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    CreateObjectAt(mobMakeName, sX, sY);
    LookWithAngle(mobMake, sAmount);
    return mobMake;
}

int ColorMaidenAt(int red, int grn, int blue, float xProfile, float yProfile)
{
    int unit = CreateObjectAt("Bear2", xProfile, yProfile);
    int ptr1 = GetMemory(0x750710), k;

    SetMemory(ptr1 + 4, 1385);
    for (k = 0 ; k < 32 ; k += 1)
        SetMemory(ptr1 + 0x230 + (k * 4), 0x400);
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x178, 0xa0);
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c, red | (grn << 8) | (blue << 16) | (red << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 4, grn | (blue << 8) | (red << 16) | (grn << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 8, blue | (red << 8) | (grn << 16) | (blue << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 12, red | (grn << 8) | (blue << 16) | (red << 24));
    SetMemory(GetMemory(ptr1 + 0x2ec) + 0x81c + 16, grn | (blue << 8));

    return unit;
}

void SplashDamageAt(int owner, int dam, float range, float sX, float sY)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", sX, sY) + 1, k;

    SetOwner(owner, ptr - 1);
    MoveObject(ptr - 1, range, GetObjectY(ptr - 1));
    Raise(ptr - 1, ToFloat(dam));
    for (k = 0 ; k < 8 ; k += 1)
    {
        DeleteObjectTimer(CreateObjectAt("WeirdlingBeast", sX, sY), 1);
        UnitNoCollide(GetMemory(0x750710));
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

void StartYellowLightning(int ptr)
{
    CastSpellObjectObject("SPELL_LIGHTNING", ptr, ptr + 2);
}

void HallOfFame()
{
	int arr[42], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(60), pos_y = GetWaypointY(60);

	arr[0] = 536875008; arr[1] = 8192; arr[2] = 2080899080; arr[3] = 4718627; arr[4] = 150929472; arr[5] = 1040716284; arr[6] = 1136693440; arr[7] = 68165632; 
	arr[8] = 541331488; arr[9] = 8929570; arr[10] = 537403456; arr[11] = 1008764960; arr[12] = 1073815618; arr[13] = 599789692; arr[14] = 1112031728; arr[15] = 1094713920; 
	arr[16] = 272662560; arr[17] = 553947266; arr[18] = 545554436; arr[19] = 37716448; arr[20] = 71043361; arr[21] = 532746368; arr[22] = 1627652098; arr[23] = 1074266131; 
	arr[24] = 33555008; arr[25] = 306316256; arr[26] = 4198400; arr[27] = 268562436; arr[28] = 2377736; arr[29] = 67108896; arr[30] = 269485072; arr[31] = 1073498208; 
	arr[32] = 134218752; arr[33] = 2105360; arr[34] = 4194448; arr[35] = 269484040; arr[36] = 268443776; arr[37] = 134250497; arr[38] = 1064960; arr[39] = 16777278; 
	arr[40] = 1072693504; arr[41] = 7936; 
	for (i = 0 ; i < 42 ; i += 1)
		count = DrawHallOfFame(arr[i], name, count);
	MoveWaypoint(60, pos_x, pos_y);
}

int DrawHallOfFame(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 1302 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 60);
		if (count % 86 == 85)
			MoveWaypoint(60, GetWaypointX(60) - 170.0, GetWaypointY(60) + 2.0);
		else
			MoveWaypoint(60, GetWaypointX(60) + 2.0, GetWaypointY(60));
		count ++;
	}
	return count;
}

void InitMapPickets()
{
    RegistSignMessage(Object("SignDesc1"), "망치를 들어라!! 그리고 서로를 향해 격렬하게 싸워라!!");
    RegistSignMessage(Object("SignDesc2"), "누가 가장 강력한 해머를 갖게될지 넌 안 궁굼하니? 응 안궁굼해 ㅋㅋㅋ");
    RegistSignMessage(Object("SignDesc3"), "강한자가 되어라! 그 누구도 널 막을 수 없어");
    RegistSignMessage(Object("SignDesc4"), "누구나 그럴싸한 계획을 갖고있다. 처 맞기 전까지는...");
}

void SpecialMissileCollide()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        if (HasEnchant(other, "ENCHANT_AFRAID"))
            return;
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        Enchant(other, "ENCHANT_SLOWED", 2.0);
        Damage(other, self, 160, 0);
    }
}

int SpecialMissile(int sOwner, float sX, float sY, float sForce)
{
    int unit = CreateObjectAt("SpiderSpit", sX, sY);

    SetUnitCallbackOnCollide(unit, SpecialMissileCollide);

    SetOwner(sOwner, unit);
    LookAtObject(unit, sOwner);
    LookWithAngle(unit, GetDirection(unit) + 128);
    PushObject(unit, sForce, GetObjectX(sOwner), GetObjectY(sOwner));
    return unit;
}

void HammerCollide()
{
    int owner = GetOwner(self);

    PrintToAll("test");
    if (CurrentHealth(owner))
    {
        Effect("SPARK_EXPLOSION", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
    }
}

int ImportWeaponHitFunc()
{
    int arr[9], link;

    if (!link)
    {
        arr[0] = 0x50731068; arr[1] = 0x8B565000; arr[2] = 0x8B102474; arr[3] = 0x0002FC86;
        arr[4] = 0x006A5600; arr[5] = 0x2454FF50; arr[6] = 0x0CC48314; arr[7] = 0xC483585E;
        arr[8] = 0x9090C304;
        link = &arr;
    }
    return link;
}

void ForceOfNatureCollide()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        SplashDamageAt(owner, 130, 150.0, GetObjectX(self), GetObjectY(self));
        GreenExplosion(GetObjectX(self), GetObjectY(self));
        DeleteObjectTimer(CreateObjectAt("ForceOfNatureCharge", GetObjectX(other), GetObjectY(other)), 24);
        Delete(self);
    }
}

int CheckHammerHitCooldown(int sUnit, int sCooldown)
{
    int *fps = 0x84ea04, cTime = ToInt(GetObjectZ(sUnit));

    if (MathAbs(fps[0] - cTime) > sCooldown)
    {
        Raise(sUnit, fps[0]);
        return 1;
    }
    return 0;
}

void WeaponHitEvent()
{
    int owner = GetOwner(self);

    if (CurrentHealth(owner) && CheckHammerHitCooldown(self, 80))
    {
        CastSpellObjectLocation("SPELL_ANCHOR", owner, GetObjectX(owner) + UnitAngleCos(owner, 17.0), GetObjectY(owner) + UnitAngleSin(owner, 17.0));
        int *uptr = 0x750710;

        if (uptr[0] == NULLPTR)
            return;
        int *ptr = uptr[0];
        int mis = ptr[11];
        //SetMemory(ptr + 0x2e8, 5483536); //projectile update

        PlaySoundAround(owner, 38);
        Enchant(mis, "ENCHANT_RUN", 0.0);
        Enchant(mis, "ENCHANT_HASTED", 0.0);
        SetUnitCallbackOnCollide(mis, ForceOfNatureCollide);
    }
}

int AdvanceHammer(int sUnit)
{
    int unit = CreateObjectAt("WarHammer", GetObjectX(sUnit), GetObjectY(sUnit));
    int *uptr = 0x750710;

    if (uptr[0] != NULLPTR)
    {
        int *ptr = uptr[0];

        ptr[180] = ImportWeaponHitFunc();
        ptr[191] = WeaponHitEvent;
        Enchant(unit, "ENCHANT_REFLECTIVE_SHIELD", 0.0);
    }
    return unit;
}

void CheckAdvanceHammerLoop(int sUnit)
{
    int hammer;

    if (IsObjectOn(sUnit))
    {
        hammer = ToInt(GetObjectZ(sUnit));
        if (!IsObjectOn(hammer))
            Raise(sUnit, AdvanceHammer(sUnit));
        SecondTimerWithArg(5, sUnit, CheckAdvanceHammerLoop);
    }
}

void TestHammerChecking()
{
    int unit = CreateObject("WarHammer", 44);

    SetUnitCallbackOnCollide(unit, HammerCollide);
}

void InitModePicket()
{
    RegistSignMessage(Object("noUseMon"), "몬스터를 사용하지 않습니다");
    RegistSignMessage(Object("use32"), "32개의 몬스터가 등장합니다");
    RegistSignMessage(Object("use64"), "64개의 몬스터가 생성됩니다");
    RegistSignMessage(Object("use128"), "128개의 몬스터가 생성됩니다");
    RegistSignMessage(Object("selSign"), "게임 모드를 선택하여 주십시오");

    MoveObject(GetHost(), LocationX(61), LocationY(61));
}

void HostPlayerTeleportToCenter()
{
    Enchant(GetHost(), "ENCHANT_INVULNERABLE", 3.0);
    MoveObject(GetHost(), LocationX(7), LocationY(7));
}

void UseNoMons()
{
    HostPlayerTeleportToCenter();
    UniPrint(other, "몬스터를 사용하지 않습니다");
}

void UseMon1()
{
    HostPlayerTeleportToCenter();
    FrameTimer(90, MobMakeStandBy);
    g_maxMobCount = 32;
    UniPrint(other, "32개의 몬스터를 사용합니다");
}

void UseMon2()
{
    HostPlayerTeleportToCenter();
    FrameTimer(90, MobMakeStandBy);
    g_maxMobCount = 64;
    UniPrint(other, "64개의 몬스터를 사용합니다");
}

void UseMon3()
{
    HostPlayerTeleportToCenter();
    FrameTimer(90, MobMakeStandBy);
    g_maxMobCount = 128;
    UniPrint(other, "128개의 몬스터를 사용합니다");
}

