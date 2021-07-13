
#include "noxscript\builtins.h"

#include "libs\typecast.h"
#include "libs\callmethod.h"
#include "libs\mathlab.h"
#include "libs\unitutil.h"
#include "libs\unitstruct.h"
#include "libs\printutil.h"
#include "libs\username.h"
#include "libs\fxeffect.h"
#include "libs\waypoint.h"
#include "libs\network.h"
#include "libs\fixtellstory.h"
#include "libs\playerinfo.h"
#include "libs\reaction.h"
#include "libs\playerupdate.h"
#include "libs\buff.h"
#include "libs\potionpickup.h"
#include "libs\meleeattack.h"
#include "libs\coopteam.h"
#include "libs\walls.h"
#include "libs\spellutil.h"
#include "libs\imageutil.h"
#include "libs\monsteraction.h"
#include "libs\absolutelypickup.h"
#include "libs\itemproperty.h"

int SafeZone;
int XtraUserCoins = 20;
int DunCnt = 6, BossCnt = 20, GOver;
int LastUnit, GlobalHeadNode;
int player[20], Dungeon[6], DGate[6];

#define NULLPTR             0

#define OTHER       -1
#define SELF        -2

static void NetworkUtilClientMain()
{
    PlayerClassCommonWhenEntry();
}

void HealingBuff(int sUnit)
{
    int owner = GetOwner(sUnit), durate = GetDirection(sUnit);

    while (IsObjectOn(sUnit))
    {
        if (CurrentHealth(owner))
        {
            if (durate)
            {
                RestoreHealth(owner, 1);
                LookWithAngle(sUnit, durate - 1);
                MoveObject(sUnit, GetObjectX(owner), GetObjectY(owner));
                FrameTimerWithArg(1, sUnit, HealingBuff);
                break;
            }
            GreenSparkFx(GetObjectX(owner), GetObjectY(owner));
            UniPrint(owner, "힐링 포션의 효력이 사라졌습니다");
        }
        Delete(sUnit);
        break;
    }
}

void ItemUseClassTeleportAmulet()
{
    if (CurrentHealth(other))
    {
        Delete(self);
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        MoveObject(other, LocationX(12), LocationY(12));
        PlaySoundAround(other, 592);
        Effect("COUNTERSPELL_EXPLOSION", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        UniPrint(other, "시작 위치로 공간이동했습니다");
    }
}

void ItemUseFastHealingPotion()
{
    int unit;

    if (CurrentHealth(other))
    {
        Delete(self);
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(other), GetObjectY(other));
        SetOwner(other, unit);
        UnitNoCollide(unit);
        LookWithAngle(unit, 250);
        UnitSetEnchantTime(unit, 8, 0);
        UnitSetEnchantTime(unit, 26, 0);
        UnitSetEnchantTime(unit, 21, 0);
        FrameTimerWithArg(3, unit, HealingBuff);
        UniPrint(other, "힐링 포션을 사용했습니다. 잠시동안 체력이 빠르게 회복될 것입니다");
    }
}

void ItemUseShockEnchant()
{
    if (CurrentHealth(other))
    {
        Delete(self);
        UnitSetEnchantTime(other, 22, 30 * 70);
    }
}

void ItemUseOneCoin()
{
    Delete(self);
    if (XtraUserCoins)
    {
        XtraUserCoins ++;
        UniPrint(other, "코인 1개가 추가되어 모두 " + IntToString(XtraUserCoins) + "개를 보유하고 있습니다");
    }
}

////////////Impletmented ChainingList//////


int AddNewNode(int headNode, int data)
{
    int node = CreateObjectAt("ImaginaryCaster", GetObjectX(data), GetObjectY(data));

    SetUnit1C(node, data);
    SetNextNode(node, GetNextNode(headNode));
    SetPrevNode(node, headNode);
    SetPrevNode(GetNextNode(headNode), node);
    SetNextNode(headNode, node);
    LookWithAngle(headNode, GetDirection(headNode) + 1);

    return node;
}

///////////////////////////////////////////

int AirshipCaptainBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1936877889; arr[1] = 1131440488; arr[2] = 1635020897; arr[3] = 28265; arr[4] = 0; 
		arr[10] = 0; arr[11] = 0; arr[12] = 0; arr[13] = 0; arr[14] = 0; 
		arr[15] = 0; arr[16] = 0; arr[17] = 130; arr[18] = 100; arr[19] = 60; 
		arr[20] = 0; arr[21] = 1065353216; arr[22] = 0; arr[23] = 32768; arr[24] = 1067869798; 
		arr[25] = 0; arr[26] = 4; arr[27] = 0; arr[28] = 1077936128; arr[29] = 20; 
		arr[30] = 0; arr[31] = 8; arr[32] = 12; arr[33] = 20;
		arr[55] = 0; arr[56] = 0; arr[57] = 5547984; arr[58] = 5546320; arr[59] = 5542432; 
        link = GetScrDataField(AirshipCaptainBinTable);
	}
	return link;
}

int WizardRedBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1635412311; arr[1] = 1699898482; arr[2] = 100;
		arr[15] = 0; arr[16] = 80000; arr[17] = 300; arr[18] = 100; arr[19] = 50; 
		arr[20] = 0; arr[21] = 1065353216; arr[22] = 0; arr[23] = 40; arr[24] = 1067869798; 
		arr[25] = 0; arr[26] = 4; arr[27] = 0; arr[28] = 1106247680; arr[29] = 20; 
		arr[30] = 0; arr[31] = 0; arr[32] = 12; arr[33] = 20; arr[34] = 0; 
		arr[35] = 0; arr[36] = 0; arr[37] = 1952539972; arr[38] = 1818313320; arr[39] = 1634879084; 
		arr[40] = 1852140903; arr[41] = 116; arr[42] = 0;
		arr[53] = 1128792064; arr[54] = 0; 
		arr[55] = 15; arr[56] = 21; arr[57] = 0; arr[58] = 0; arr[59] = 5542784;
        link = GetScrDataField(WizardRedBinTable);
	}
	return link;
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

void TeleportAmulet(int sUnit)
{
    SetUnitCallbackOnUseItem(sUnit, ItemUseClassTeleportAmulet);
}

void HealingPotion(int sUnit)
{
    SetUnitCallbackOnUseItem(sUnit, ItemUseFastHealingPotion);
}

void ElectricAmulet(int sUnit)
{
    SetUnitCallbackOnUseItem(sUnit, ItemUseShockEnchant);
}

void AddOneCoin(int sUnit)
{
    int ptr = UnitToPtr(sUnit);

    if (ptr)
    {
        SetMemory(ptr + 0x2c4, 0x4f31e0);
        SetUnitCallbackOnUseItem(sUnit, ItemUseOneCoin);
    }
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
    else if (thingID == 1184)
        TeleportAmulet(unit);
    else if (thingID == 239)
        ElectricAmulet(unit);
    else if (thingID == 1185)
        HealingPotion(unit);
    else if (thingID == 2689)
        AddOneCoin(unit);

    if (x ^ unit) Delete(unit);
    PotionPickupRegist(x);
    return x;
}

int WeirdlingBeastBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811;
		arr[17] = 85; arr[18] = 50; arr[19] = 55; 
		arr[21] = 1065353216; arr[23] = 32776; arr[24] = 1068708659; 
		arr[26] = 4; arr[28] = 1082130432; arr[29] = 20; 
		arr[31] = 8; arr[32] = 8; arr[33] = 16; arr[57] = 5548112; arr[59] = 5542784;
        link = &arr;
	}
	return link;
}

int HecubahBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 6840674; arr[17] = 600; arr[19] = 100; arr[21] = 1065353216; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; arr[28] = 1092616192; 
		arr[29] = 50; arr[31] = 11; arr[57] = 5548288; arr[59] = 5542784;
        link = &arr;
	}
	return link;
}

int LichLordBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751345484; arr[1] = 1685221196; arr[17] = 20; arr[19] = 100; 
		arr[21] = 1065353216; arr[24] = 1065353216; 
		arr[25] = 1; arr[26] = 4; arr[27] = 7; arr[28] = 1108082688; arr[29] = 50; 
		arr[30] = 1092616192; arr[32] = 9; arr[33] = 17; arr[57] = 5548288; arr[59] = 5542784;
        link = &arr;
	}
	return link;
}

int BomberGreenBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1651339074; arr[1] = 1917284965; arr[2] = 7234917;
		arr[17] = 20; arr[19] = 80; 
		arr[20] = 0; arr[21] = 1065353216; arr[22] = 0; arr[23] = 0; arr[24] = 1065353216; 
		arr[31] = 4; arr[32] = 0; arr[33] = 0; arr[34] = 0; 
		arr[35] = 0; arr[36] = 0; arr[37] = 1801545047; arr[38] = 1701996870; arr[39] = 1819042146; 
		arr[53] = 1128792064;
		arr[55] = 3; arr[56] = 6; arr[57] = 5548112; arr[58] = 5545344; arr[59] = 5543344;
        link = &arr;
	}
	return link;
}

int FireSpriteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972; arr[17] = 85; arr[18] = 25; arr[19] = 90; 
		arr[21] = 1065353216; arr[23] = 65544; arr[24] = 1065353216; 
		arr[37] = 1801545047; arr[38] = 1701996870; arr[39] = 1819042146; 
		arr[53] = 1128792064;
		arr[55] = 15; arr[56] = 21; arr[58] = 5545472;
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

int BlackWidowBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1667329090; arr[1] = 1684625259; arr[2] = 30575;
		arr[17] = 130; arr[18] = 45; arr[19] = 85; 
		arr[20] = 0; arr[21] = 1065353216; arr[22] = 0; arr[23] = 4; arr[24] = 1069547520; 
		arr[25] = 0; arr[26] = 4; arr[27] = 3; arr[28] = 1097859072; arr[29] = 25; 
		arr[30] = 0; arr[31] = 8; arr[32] = 13; arr[33] = 21; arr[34] = 50; 
		arr[35] = 3; arr[36] = 6; arr[37] = 1684631635; arr[38] = 1884516965; arr[39] = 29801; 
		arr[53] = 1128792064;
		arr[55] = 20; arr[56] = 28; arr[59] = 5544896; 
		arr[61] = 45071360;
        link = &arr;
	}
	return link;
}

int Bear2BinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1918985538; arr[1] = 50;
		arr[15] = 0; arr[16] = 20000; arr[17] = 90; arr[18] = 100; arr[19] = 40; 
		arr[20] = 0; arr[21] = 1065353216; arr[22] = 0; arr[23] = 65545; arr[24] = 1067450368; 
		arr[27] = 1; arr[28] = 1106247680; arr[29] = 50; 
		arr[30] = 1103626240; arr[31] = 2; arr[32] = 20; arr[33] = 30; arr[58] = 5547856; arr[59] = 5542784;
        link = &arr;
	}
	return link;
}

int NecromancerBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1919116622; arr[1] = 1851878767; arr[2] = 7497059; arr[17] = 350; arr[19] = 93; 
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1065353216; arr[25] = 1; arr[26] = 2; 
		arr[28] = 1103626240; arr[29] = 80; arr[30] = 1092616192; arr[31] = 11; arr[32] = 7; 
		arr[33] = 15; arr[34] = 1; arr[35] = 2; arr[36] = 30; arr[59] = 5542784;
        link = &arr;
	}
	return link;
}

void NecromancerSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077055324);
		SetMemory(ptr + 0x224, 1077055324);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 350);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 350);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, NecromancerBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void HecubahSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077936128);
		SetMemory(ptr + 0x224, 1077936128);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 0);
		SetMemory(GetMemory(ptr + 0x22c), 600);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 600);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HecubahBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int DummyUnitCreate(string name, float locX, float locY)
{
    int unit = CreateObjectAt(name, locX, locY);

    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    ObjectOff(unit);
    Frozen(unit, 1);
    return unit;
}

void MoneyImage() {}

void BuldakImage() {}

void KeepOutMonsterHere()
{
    if (IsOwnedBy(OTHER, GetMaster()))
    {
        PushObjectTo(OTHER, UnitRatioX(OTHER, SafeZone, 100.0), UnitRatioY(OTHER, SafeZone, 100.0));
    }
}

int HealingWishWell(int wp)
{
    int unit = CreateObject("WizardWhite", wp);

    Enchant(CreateObject("InvisibleLightBlueHigh", wp), "ENCHANT_SHIELD", 0.0);
    LookWithAngle(unit, 64);
    ObjectOff(unit);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    Frozen(unit, 1);
    SetDialog(unit, "a", StartUnitHealing, StartUnitHealing);

    return unit;
}

void StartUnitHealing()
{
    int unit;

    if (!HasEnchant(other, "ENCHANT_CROWN"))
    {
        Enchant(other, "ENCHANT_CROWN", 8.0);
        MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
        AudioEvent("ProtectionFromPoisonEffect", 1);
        AudioEvent("LongBellsDown", 1);
        unit = CreateObject("InvisibleLightBlueHigh", 1);
        SetOwner(other, unit);
        Enchant(unit, "ENCHANT_RUN", 0.0);
        FrameTimerWithArg(1, unit, UnitHealingHandler);
        UniPrint(other, "힐링버프 감지됨, 잠시동안 당신의 체력이 지속적으로 회복됩니다");
    }
}

void UnitHealingHandler(int ptr)
{
    int owner = GetOwner(ptr);

    if (HasEnchant(owner, "ENCHANT_CROWN") && IsObjectOn(ptr))
    {
        RestoreHealth(owner, 1);
        MoveObject(ptr, GetObjectX(owner), GetObjectY(owner));
        FrameTimerWithArg(1, ptr, UnitHealingHandler);
    }
    else
        Delete(ptr);
}

void TestFunction()
{
    UniPrintToAll("***        던전 레이드하기_____ 제작. Rainbow Company                   ***");
    UniPrintToAll("*** 주 임무: 6개의 던전을 다 털고나서 열리는 마지막 던전을 털면 승리합니다 ***");
    UniPrintToAll("초반 20라이프가 제공됩니다, 라이프는 돈으로 추가할 수 있으며 라이프가 모두 소모되면 게임오버됩니다");
}

int GetPrevNode(int cur)
{
    return GetOwner(cur);
}

int GetNextNode(int cur)
{
    return ToInt(GetObjectZ(cur));
}

void SetPrevNode(int cur, int tg)
{
    SetOwner(tg, cur);
}

void SetNextNode(int cur, int tg)
{
    Raise(cur, ToFloat(tg));
}

int NewList(int data)
{
    int cur = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(data), GetObjectY(data));
    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(cur), GetObjectY(cur)), ToFloat(data));

    SetPrevNode(cur, GetPrevNode(GlobalHeadNode + 1));
	SetNextNode(cur, GlobalHeadNode + 1);
	SetNextNode(GetPrevNode(GlobalHeadNode + 1), cur);
	SetPrevNode(GlobalHeadNode + 1, cur);

    return cur;
}

int RemoveList(int cur)
{
    int next = GetNextNode(cur);
    SetNextNode(GetPrevNode(cur), GetNextNode(cur));
    SetPrevNode(GetNextNode(cur), GetPrevNode(cur));
    Delete(cur);
    Delete(cur + 1);
    return next;
}

void RemoveMagicSword(int unit)
{
    if (IsObjectOn(unit))
        Delete(unit);
    Delete(unit + 1);
}

void MagicWeaponLoop()
{
    int cur = GetNextNode(GlobalHeadNode);

    while (IsObjectOn(cur))
    {
        if (ToInt(GetObjectZ(cur + 1)))
        {
            if (MagicWeaponProperty(ToInt(GetObjectZ(cur + 1))))
                cur = GetNextNode(cur);
            else
            {
                RemoveMagicSword(ToInt(GetObjectZ(cur + 1)));
                cur = RemoveList(cur);
            }
        }
        else
            break;
    }
    if (GOver)  return;
    else        FrameTimer(1, MagicWeaponLoop);
}

int MagicWeaponFunctionTable(int index)
{
    int functions[] = {
        LaiserSword,
        WhiteOutHammer,
        EnergyParHammer,
        TripleArrowHammer,
        DeathraySword,
        ShurikenSword,
        WolfShooting,
        BomberSummon };
    return functions[index];
}

int MagicWeaponProperty(int unit)
{
    int owner = GetOwner(unit);

    if (CurrentHealth(owner))
    {
        if (PlayerGetEquipedWeapon(owner) == unit)
        {
            if (CheckWeaponStrike(owner))
                CallFunctionWithArg(MagicWeaponFunctionTable(GetDirection(unit + 1)), owner);
        }
    }
    return IsObjectOn(unit);
}

int CheckWeaponStrike(int unit)
{
    int inv = PlayerGetEquipedWeapon(unit);

    if (IsObjectOn(inv))
        return ((CheckPlayerInput(unit) == 6) && !HasEnchant(unit, "ENCHANT_PROTECT_FROM_MAGIC"));
    return 0;
}

void LaiserSword(int owner)     //SpecialProperty - Laiser Sword
{
    float x_vect = UnitAngleCos(owner, 30.0), y_vect = UnitAngleSin(owner, 30.0);
    int k, ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner)) + 1;

    Enchant(owner, EnchantList(19), 0.9);
    Delete(ptr - 1);
    MoveWaypoint(1, GetObjectX(owner) + x_vect, GetObjectY(owner) + y_vect);
    for (k = 0 ; k < 11 ; k ++)
    {
        Frozen(CreateObject("Maiden", 1), 1);
        SetOwner(owner, ptr + k);
        DeleteObjectTimer(ptr + k, 1);
        SetCallback(ptr + k, 9, LaiserLifleTouched);
        Effect("SENTRY_RAY", GetObjectX(owner), GetObjectY(owner), GetObjectX(ptr + k), GetObjectY(ptr + k));
        TeleportLocationVector(1, x_vect, y_vect);
        if (!IsVisibleTo(owner, ptr + k))
            break;
    }
}

void WhiteOutHammer(int owner)      // SpecialProperty - Whiteout Hammer
{
    int unit;

    UnitSetEnchantTime(owner, 19, 33);
    SplashDamageAt(owner, 225, GetObjectX(owner), GetObjectY(owner), 200.0);
    unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
    CastSpellObjectObject("SPELL_TURN_UNDEAD", unit, unit);
    DeleteObjectTimer(unit, 30);
}

void EnergyParHammer(int owner)     // SpecialProperty - 
{
    float xVect = UnitAngleCos(owner, 23.0), yVect = UnitAngleSin(owner, 23.0);
    int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner) + xVect, GetObjectY(owner) + yVect);

    Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner) - xVect, GetObjectY(owner) - yVect), yVect);
    Raise(unit, xVect);

    SetOwner(owner, unit);
    LookWithAngle(unit, 150);
    UnitSetEnchantTime(owner, 19, 50);
    FrameTimerWithArg(1, unit, EnergyParHandler);
}

void TripleArrowHammer(int owner)       // SpecialProperty -
{
    int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));

    SetOwner(owner, unit);
    UnitSetEnchantTime(owner, 19, 45);
    FrameTimerWithArg(1, unit, TripleArrowShot);
}

void DeathraySword(int owner)       // SpecialProperty -
{
    float xVect = UnitAngleCos(owner, 23.0), yVect = UnitAngleSin(owner, 23.0);
    int unit = CreateObjectAt("WeirdlingBeast", GetObjectX(owner) + xVect, GetObjectY(owner) + yVect);

    MoveWaypoint(1, GetObjectX(owner), GetObjectY(owner));
    AudioEvent("TripleChime", 1);
    AudioEvent("FirewalkOn", 1);
    Effect("WHITE_FLASH", LocationX(1), LocationY(1), 0.0, 0.0);
    DeleteObjectTimer(CreateObjectAt("ManaBombCharge", GetObjectX(owner), GetObjectY(owner)), 18);
    SetOwner(owner, unit);
    SetUnitScanRange(unit, 450.0);
    LookWithAngle(unit, GetDirection(owner));
    SetCallback(unit, 3, DeathrayDetectedTarget);
    DeleteObjectTimer(unit, 1);
    Enchant(owner, "ENCHANT_PROTECT_FROM_MAGIC", 0.8);
}

void ShurikenSword(int owner)       // SpecialProperty -
{
    int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner) + UnitAngleCos(owner, 21.0), GetObjectY(owner) + UnitAngleSin(owner, 21.0));
    LookWithAngle(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), GetDirection(owner));
    Raise(unit, 250.0);
    SetOwner(owner, unit);
    LookWithAngle(unit, 35);
    UnitSetEnchantTime(owner, 19, 60);
    FrameTimerWithArg(1, unit, AutoTrackingMissile);
}

void WolfShooting(int owner)        // SpecialProperty - NEW
{
    int unit = DummyUnitCreate("WhiteWolf", GetObjectX(owner) + UnitAngleCos(owner, 23.0), GetObjectY(owner) + UnitAngleSin(owner, 23.0));

    SetOwner(owner, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)));
    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)), UnitAngleSin(owner, 21.0));
    Raise(unit + 1, UnitAngleCos(owner, 21.0));
    LookWithAngle(unit + 1, 30);
    LookWithAngle(unit, GetDirection(owner));
    SetCallback(unit, 9, FlyingWolfCollideHandler);
    FrameTimerWithArg(1, unit + 1, FlyingWolf);
    Enchant(owner, "ENCHANT_PROTECT_FROM_MAGIC", 0.8);
}

void BomberSummon(int owner)        // SpecialProperty - NEW
{
    int unit = CreateObjectAt("BomberGreen", GetObjectX(owner) + UnitAngleCos(owner, 23.0), GetObjectY(owner) + UnitAngleSin(owner, 23.0));

    LookWithAngle(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)), 180); //TODO: Duration 6 seconds
    Enchant(unit, "ENCHANT_INVULNERABLE", 0.0);
    SetOwner(owner, unit);
    SetOwner(owner, unit + 1);
    SetCallback(unit, 9, BomberCollideHandler);
    FrameTimerWithArg(1, unit + 1, BomberLifeTime);
    Enchant(owner, "ENCHANT_PROTECT_FROM_MAGIC", 1.8);
}

void CollideHandlerEnergyPar()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 200, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
        Effect("SPARK_EXPLOSION", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
    }
}

void EnergyParHandler(int sUnit)
{
    int owner = GetOwner(sUnit), durate = GetDirection(sUnit), unit;

    while (IsObjectOn(sUnit))
    {
        if (CurrentHealth(owner) && IsVisibleTo(sUnit, sUnit + 1))
        {
            if (durate)
            {
                unit = DummyUnitCreate("Demon", GetObjectX(sUnit), GetObjectY(sUnit));
                DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(unit), GetObjectY(unit)), 9);
                MoveObject(sUnit, GetObjectX(sUnit) + GetObjectZ(sUnit), GetObjectY(sUnit) + GetObjectZ(sUnit + 1));
                MoveObject(sUnit + 1, GetObjectX(sUnit + 1) + GetObjectZ(sUnit), GetObjectY(sUnit + 1) + GetObjectZ(sUnit + 1));
                SetOwner(owner, unit);
                DeleteObjectTimer(unit, 1);
                SetCallback(unit, 9, CollideHandlerEnergyPar);
                FrameTimerWithArg(1, sUnit, EnergyParHandler);
                LookWithAngle(sUnit, durate - 1);
                break;
            }
        }
        Delete(sUnit);
        Delete(sUnit + 1);
        break;
    }
}

void LaiserLifleTouched()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
        AudioEvent("SentryRayHit", 1);
        Damage(other, owner, 185, 16);
        Enchant(other, "ENCHANT_CHARMING", 0.1);
        Enchant(other, "ENCHANT_FREEZE", 1.0);
        Effect("VIOLET_SPARKS", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    }
}

void TripleArrowShot(int ptr)
{
    int owner = GetOwner(ptr), unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(ptr), GetObjectY(ptr)) + 1, i;

    if (CurrentHealth(owner))
    {
        LookWithAngle(ptr, GetDirection(owner) - 15);
        for (i = 0 ; i < 11 ; i ++)
        {
            SpawnBullet(owner, GetObjectX(unit - 1) + UnitAngleCos(ptr, 21.0), GetObjectY(unit - 1) + UnitAngleSin(ptr, 21.0), 175, 38.0);
            LookWithAngle(ptr, GetDirection(ptr) + 3);
        }
    }
    Delete(ptr);
}

void TripleArrowCollide()
{
    int owner = GetOwner(self);

    while (1)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 175, 9);
            UnitSetEnchantTime(other, 25, 35);
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        Delete(self);
        break;
    }
}

int SpawnBullet(int owner, float x, float y, int dam, float force)
{
    int unit = CreateObjectAt("LightningBolt", x, y);

    SetUnitCallbackOnCollide(unit, TripleArrowCollide);
    LookAtObject(unit, owner);
    LookWithAngle(unit, GetDirection(unit) + 128);
    PushObjectTo(unit, UnitRatioX(unit, owner, force), UnitRatioY(unit, owner, force));
    SetOwner(owner, unit);
    return unit;
}

void DeathrayDetectedTarget()
{
    Effect("GREATER_HEAL", GetObjectX(other), GetObjectX(other), GetObjectX(self), GetObjectY(self));
    CastSpellObjectObject("SPELL_DEATH_RAY", self, other);
    RestoreHealth(GetOwner(self), 2);
}

void AutoTrackingMissile(int sUnit)
{
    int owner = GetOwner(sUnit), durate = GetDirection(sUnit), unit;

    while (1)
    {
        if (CurrentHealth(owner))
        {
            if (durate && IsVisibleTo(sUnit, sUnit + 1))
            {
                unit = CreateObjectAt("AirshipCaptain", GetObjectX(sUnit), GetObjectY(sUnit));
                Frozen(CreateObjectAt("HarpoonBolt", GetObjectX(unit), GetObjectY(unit)), 1);
                SetOwner(sUnit, unit);
                LookWithAngle(unit, GetDirection(sUnit + 1));
                LookWithAngle(unit + 1, GetDirection(sUnit + 1));
                DeleteObjectTimer(unit, 1);
                DeleteObjectTimer(unit + 1, 3);
                SetCallback(unit, 3, DetectTrackingMissile);
                SetCallback(unit, 9, CollideTrackingMissile);
                MoveObject(sUnit + 1, GetObjectX(sUnit), GetObjectY(sUnit));
                MoveObject(sUnit, GetObjectX(sUnit) + UnitAngleCos(sUnit + 1, 21.0), GetObjectY(sUnit) + UnitAngleSin(sUnit + 1, 21.0));
                LookWithAngle(sUnit, durate - 1);
                FrameTimerWithArg(1, sUnit, AutoTrackingMissile);
                break;
            }
        }
        Delete(sUnit);
        Delete(sUnit + 1);
        break;
    }
}

void DetectTrackingMissile()
{
    int ptr = GetOwner(self);
    int tg = ToInt(GetObjectZ(ptr + 1));

    if (CurrentHealth(tg))
    {
        if (IsVisibleTo(tg, ptr))
        {
            LookAtObject(self, tg);
            LookWithAngle(ptr + 1, GetDirection(self));
        }
        else
            Raise(ptr + 1, ToFloat(0));
    }
    else
    {
        if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) < GetObjectZ(ptr))
            Raise(ptr + 1, ToFloat(GetCaller()));
    }
}

void CollideTrackingMissile()
{
    int ptr = GetOwner(self);
    int owner = GetOwner(ptr);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Effect("SPARK_EXPLOSION", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Damage(other, owner, 750, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
        Delete(self);
        Delete(ptr);
    }
}

void FlyingWolfCollideHandler()
{
    int owner = GetOwner(GetTrigger() + 1);

    if (CurrentHealth(other) && CurrentHealth(owner) && IsAttackedBy(other, owner) && MaxHealth(self))
    {
        MoveObject(GetTrigger() + 1, GetObjectX(self), GetObjectY(self));
        LookWithAngle(GetTrigger() + 2, 1); //TODO: Start Explosion
        Delete(self);
    }
}

void FlyingWolfExplosion(int sUnit)
{
    float xProfile = GetObjectX(sUnit), yProfile = GetObjectY(sUnit);
    int owner = GetOwner(sUnit);

    PlaySoundAround(sUnit, 594);
    PlaySoundAround(sUnit, 487);
    SplashDamageAt(owner, 170, xProfile, yProfile, 120.0);
    GreenExplosion(xProfile, yProfile);
    DeleteObjectTimer(CreateObjectAt("ForceOfNatureCharge", xProfile, yProfile), 30);
    Effect("JIGGLE", xProfile, yProfile, 20.0, 0.0);
}

void FlyingWolf(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), explo = GetDirection(ptr + 1);

    while (1)
    {
        if (CurrentHealth(owner) && count)
        {
            if (explo)
                FlyingWolfExplosion(ptr);
            else if (IsVisibleTo(ptr, ptr - 1))
            {
                MoveObject(ptr - 1, GetObjectX(ptr - 1) + GetObjectZ(ptr), GetObjectY(ptr - 1) + GetObjectZ(ptr + 1));
                LookWithAngle(ptr, count - 1);
                FrameTimerWithArg(1, ptr, FlyingWolf);
                break;
            }
        }
        Delete(ptr);
        Delete(ptr + 1);
        if (MaxHealth(ptr - 1))
            Delete(ptr - 1);
        break;
    }
}

void ReleaseForceofNature(int ptr)
{
    int owner = GetOwner(ptr), mis;

    if (CurrentHealth(owner))
    {
        MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
        mis = CreateObject("DeathBall", 1);
        SetOwner(owner, mis);
        DeleteObjectTimer(mis, 85);
        AudioEvent("ForceOfNatureRelease", 1);
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void BomberExplosion(int unit)
{
    int owner = GetOwner(unit), ptr;

    if (CurrentHealth(unit))
    {
        if (CurrentHealth(owner))
        {
            MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
            ptr = CreateObject("InvisibleLightBlueLow", 1);
            SetOwner(owner, CreateObject("ForceOfNatureCharge", 1) - 1);
            AudioEvent("ForceOfNatureCast", 1);
            FrameTimerWithArg(25, ptr, ReleaseForceofNature);
        }
        Delete(unit);
    }
    Delete(unit + 1);
}

void BomberCollideHandler()
{
    if (CurrentHealth(self) && CurrentHealth(other) && IsAttackedBy(other, self))
    {
        Enchant(other, "ENCHANT_FREEZE", 1.0);
        BomberExplosion(GetTrigger());
        LookWithAngle(GetTrigger(), 0); //TODO: LifeTime SetTo 0
    }
}

void BomberLifeTime(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr);

    while (1)
    {
        if (CurrentHealth(owner))
        {
            if (count)
            {
                LookWithAngle(ptr, count - 1);
                FrameTimerWithArg(1, ptr, BomberLifeTime);
                break;
            }
            else
                BomberExplosion(ptr - 1);
        }
        if (MaxHealth(ptr - 1))
            Delete(ptr - 1);
        Delete(ptr);
        break;
    }
}

void InitList()
{
    GlobalHeadNode = CreateObject("InvisibleLightBlueHigh", 1);
    CreateObject("InvisibleLightBlueHigh", 1);
    CreateObject("InvisibleLightBlueHigh", 1);
    SetNextNode(GlobalHeadNode, GlobalHeadNode + 1);
    SetPrevNode(GlobalHeadNode + 1, GlobalHeadNode);
    FrameTimer(100, MagicWeaponLoop);
}

void YellowLightningFx(float x1, float y1, float x2, float y2, int dur)
{
	int unit = CreateObjectAt("InvisibleLightBlueHigh", x1, y1) + 2, count, i;
	float vectX, vectY;

	CreateObjectAt("InvisibleLightBlueHigh", x2, y2);
	vectX = UnitRatioX(unit - 1, unit - 2, 32.0);
	vectY = UnitRatioY(unit - 1, unit - 2, 32.0);
	count = FloatToInt(DistanceUnitToUnit(unit - 2, unit - 1) / 32.0);
	DeleteObjectTimer(CreateObjectAt("InvisibleLightBlueHigh", x1, y1), dur);
	for (i = 0 ; i < count ; i += 1)
	{
		MoveObject(unit - 2, GetObjectX(unit - 2) + vectX, GetObjectY(unit - 2) + vectY);
		DeleteObjectTimer(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit - 2), GetObjectY(unit - 2)), dur);
		if (IsVisibleTo(unit + i, unit + i + 1))
			LookWithAngle(unit, i + 1);
		else break;
	}
	Delete(unit - 1);
	Delete(unit - 2);
	FrameTimerWithArg(1, unit, DelayYellowLightning);
}

void DelayYellowLightning(int ptr)
{
	int i, max = GetDirection(ptr);

	if (IsObjectOn(ptr))
	{
		for (i = 0 ; i < max ; i += 1)
			CastSpellObjectObject("SPELL_LIGHTNING", ptr + i, ptr + i + 1);
	}
}

void BerserkerNoDelayCore(int plr)
{
    int arr[10];

    if (!MaxHealth(arr[plr]))
    {
        arr[plr] = CreateObject("Bear2", 22 + plr);
        UnitLinkBinScript(CreateObjectAt("Rat", GetObjectX(arr[plr]), GetObjectY(arr[plr]) + 20.0) - 1, Bear2BinTable());
        SetOwner(player[plr], arr[plr]);
        LookAtObject(arr[plr], arr[plr] + 1);
        HitLocation(arr[plr], GetObjectX(arr[plr]), GetObjectY(arr[plr]));
        FrameTimerWithArg(3, arr[plr], RemoveCoreUnits);
    }
}

void RemoveCoreUnits(int ptr)
{
    Delete(ptr);
    Delete(ptr + 1);
}

void SetDirectWeaponProperty(int unit, int propertyOffs1, int propertyOffs2, int propertyOffs3, int propertyOffs4)
{
    int i, ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2b4), propertyOffs1);
        SetMemory(GetMemory(ptr + 0x2b4) + 4, propertyOffs2);
        SetMemory(GetMemory(ptr + 0x2b4) + 8, propertyOffs3);
        SetMemory(GetMemory(ptr + 0x2b4) + 12, propertyOffs4);
        for (i = 31 ; i >= 0 ; i --)
            SetMemory(ptr + 0x230 + (i * 4), 0x200);
    }
}

void CheckSpecialItem(int ptr)
{
    int id = GetMemory(ptr + 0x04);

    if (id >= 222 && id <= 225)
    {
        SetMemory(ptr + 0x2c4, 0x53a720);
        SetMemory(ptr + 0x2c8, ImportAllowAllDrop());
    }
    else if (id == 1178)
        SetMemory(GetMemory(ptr + 0x2e0), 0xffff);
    else if (id == 1168)
        SetMemory(GetMemory(ptr + 0x2e0), 0xffff);
}

void NormalMonsterItemRelease(int unit)
{
    MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
    CallFunctionWithArgInt(FieldItemFunctionTable(Random(0, 4)), 1);
}

void DungeonMonsterItemRelease(int unit)
{
    MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
    CallFunctionWithArgInt(DungeonItemFunctionTable(Random(0, 7)), 1);
}

int FieldItemFunctionTable(int index)
{
    int functions[] = {LittleMoneyDrop, HotPotion, PotionItemDrop, NormalWeaponItemDrop, NormalArmorItemDrop};

    return functions[index % 5];
}

int DungeonItemFunctionTable(int index)
{
    int functions[] = {
        HotPotion, PotionItemDrop, NormalWeaponItemDrop, NormalArmorItemDrop,
        MoneyDrop, SomeGermDrop, WeaponItemDrop, ArmorItemDrop
    };
    return functions[index % 8];
}

int LittleMoneyDrop(int wp)
{
    int money = CreateObject("Gold", wp);

    SetMemory(GetMemory(GetMemory(0x750710) + 0x2b4), Random(1000, 4000));
    return money;
}

int HotPotion(int wp)
{
    return PotionPickupRegist(CreateObject("RedPotion", wp));
}

int PotionItemDrop(int wp)
{
    return CheckPotionThingID(CreateObject(PotionList(Random(0, 19)), 1));
}

int NormalWeaponItemDrop(int wp)
{
    int unit = CreateObject(WeaponList(Random(0, 7)), 1);
    int ptr = GetMemory(0x750710);

    CheckSpecialItem(ptr);
    return unit;
}

int NormalArmorItemDrop(int wp)
{
    return CreateObject(ArmorList(Random(0, 17)), 1);
}

int MoneyDrop(int wp)
{
    int money = CreateObject("Gold", wp);
    SetMemory(GetMemory(GetMemory(0x750710) + 0x2b4), Random(2400, 7000));
    return money;
}

int SomeGermDrop(int wp)
{
    string name[] = {"Ruby", "Emerald", "Diamond"};
    return CreateObject(name[Random(0, 2)], wp);
}

int WeaponItemDrop(int wp)
{
    int unit = CreateObject(WeaponList(Random(0, 12)), 1);
    int ptr = GetMemory(0x750710);

    CheckSpecialItem(ptr);
    SetWeaponProperties(unit, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    return unit;
}

int ArmorItemDrop(int wp)
{
    int unit = CreateObject(ArmorList(Random(0, 17)), 1);

    SetArmorProperties(unit, Random(0, 5), Random(0, 5), Random(0, 20), Random(0, 20));
    return unit;
}

string PotionList(int index)
{
    string name[] = {
        "RedPotion", "CurePoisonPotion", "YellowPotion", "BlackPotion",
        "VampirismPotion", "Mushroom", "PoisonProtectPotion", "ShockProtectPotion",
        "FireProtectPotion", "HastePotion", "ShieldPotion", "InfravisionPotion",
        "InvisibilityPotion", "AmuletofManipulation", "AmuletofManipulation", "AmuletofNature",
        "Fear", "WhitePotion", "BluePotion", "AnkhTradable"
    };
    return name[index];
}

string WeaponList(int index)
{
    string name[] = {
        "GreatSword", "Longsword", "Sword", "MorningStar",
        "OgreAxe", "StaffWooden", "BattleAxe", "FanChakram",
        "RoundChakram", "WarHammer", "OblivionHalberd", "OblivionWierdling",
        "OblivionHeart"
    };
    return name[index];
}

string ArmorList(int index)
{
    string name[] = {
        "OrnateHelm", "Breastplate", "PlateArms", "PlateBoots",
        "PlateLeggings", "MedievalCloak", "ChainCoif", "ChainLeggings",
        "ChainTunic", "SteelHelm", "LeatherArmbands", "LeatherArmor",
        "LeatherArmoredBoots", "LeatherBoots", "LeatherHelm", "LeatherLeggings",
        "MedievalPants", "MedievalShirt"
    };
    return name[index];
}

int GetMaster()
{
    int unit;
    
    if (!unit)
    {
        unit = CreateObjectAt("Hecubah", 5500.0, 100.0);
        MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
        CreateObject("BlackPowder", 1);
        Frozen(unit, 1);
        //SetCallback(unit, 9, DisplayLadderBoard);
    }
    return unit;
}

int GetUnitParent(int sUnit)
{
    int cur = sUnit, res;

    while (1)
    {
        res = cur;
        cur = GetOwner(cur);
        if (!cur) break;
    }
    return res;
}

void FieldMonsterHurt()
{
    int enemy;

    if (GetCaller())
    {
        enemy = GetUnitParent(other);
        if (CurrentHealth(enemy))
            Attack(self, enemy);
    }
    else
    {
        if (IsPoisonedUnit(self))
        {
            Damage(self, 0, IsPoisonedUnit(self) * 2, 5);
            DeleteObjectTimer(CreateObjectAt("OldPuff", GetObjectX(self), GetObjectY(self)), 15);
        }
    }
}

void SpawnMarkerMonster(int cur)
{
    int mob = CallFunctionWithArgInt(DungeonMobFunctions(Random(0, 25)), cur);

    SetCallback(mob, 7, FieldMonsterHurt);
    Delete(cur);
}

void SpawnReward(int cur)
{
    MoveWaypoint(1, GetObjectX(cur), GetObjectY(cur));
    CallFunctionWithArgInt(DungeonItemFunctionTable(Random(0, 4)), 1);
    Delete(cur);
}

void SpawnRewardPlus(int cur)
{
    MoveWaypoint(1, GetObjectX(cur), GetObjectY(cur));
    CallFunctionWithArgInt(DungeonItemFunctionTable(Random(0, 7)), 1);
    Delete(cur);
}

int ThingIDCheckingProcess(int cur)
{
    int thingId = GetUnitThingID(cur);

    if (thingId == 2675)
    {
        SpawnMarkerMonster(cur);
        return 1;
    }
    else if (thingId == 2672)
    {
        SpawnReward(cur);
        return 1;
    }
    else if (thingId == 2673)
    {
        SpawnRewardPlus(cur);
        return 1;
    }
    return 0;
}

void InitMarkerScan(int cur)
{
    int count, k;

    if (cur < LastUnit)
    {
        for (k = 0 ; k < 30 ; k += 1)
        {
            if (ThingIDCheckingProcess(cur + (k * 2)))
                count += 1;
        }
        FrameTimerWithArg(1, cur + 60, InitMarkerScan);
    }
    else
        EndScan(count);
}

void StartMarkerScan(int unit)
{
    if (IsObjectOn(unit))
    {
        InitMarkerScan(unit);
    }
}

void EndScan(int count)
{
    // RegistUnitStrikeCallback(MonsterStrikeCallback);
    int spawnMark = CreateObjectAt("InvisibleLightBlueHigh", LocationX(1), LocationY(1));

    LookWithAngle(spawnMark, 180);
    FrameTimerWithArg(180, spawnMark, SpawnLowLevelMonsters);
    UniPrintToAll("스캔완료!: " + IntToString(count) + " 개 유닛을 처리완료 하였습니다");
}

void SpawnLowLevelMonsters(int mark)
{
    int count = GetDirection(mark);

    if (count > 0)
    {
        RhombusUnitTeleport(mark, 2981.0, 3320.0, 3353.0, 4106.0);
        SetCallback(CallFunctionWithArgInt(LowLevelMobFunctions(Random(0, 5)), mark), 7, FieldMonsterHurt);
        LookWithAngle(mark, --count);
        FrameTimerWithArg(1, mark, SpawnLowLevelMonsters);
    }
}

int AbilityButton(string name, int wp, int idx, int flag, int pay)
{
    int unit = CreateObject(name, wp);

    Raise(CreateObject("InvisibleLightBlueHigh", wp), ToFloat(pay));
    LookWithAngle(unit + 1, flag);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    LookWithAngle(unit, idx);
    ObjectOff(unit);
    Frozen(unit, 1);
    return unit;
}

int SpecialWeaponShop(string name, int wp, int idx, int pay)
{
    int unit = AbilityButton(name, wp, idx, 0, pay);

    Enchant(CreateObjectAt("RoundChakramInMotion", GetObjectX(unit), GetObjectY(unit)), "ENCHANT_FREEZE", 0.0);
    Enchant(unit + 2, "ENCHANT_SHOCK", 0.0);
    Frozen(unit + 2, 1);
    return unit;
}

int SpecialWeaponPlaceFunctionTable(int index)
{
    int functions[] = {
        SpecialSword1, SpecialSword2, SpecialSword3,
        SpecialSword4, SpecialSword5, SpecialSword6, SpecialSword7,
        SpecialSword8
    };
    return functions[index % 8];
}

int SpecialSword1(int wp)
{
    int unit = CreateObject("GreatSword", wp);

    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 0);
    SetDirectWeaponProperty(unit, WeaponPower(5), GetMemory(0x5BA24C), GetMemory(0x005BA264), GetMemory(0x5BA30C));
    NewList(unit);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);

    return unit;
}

int SpecialSword2(int wp)
{
    int unit = CreateObject("WarHammer", wp);

    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 1);
    SetDirectWeaponProperty(unit, WeaponPower(5), GetMemory(0x005BA354), GetMemory(0x5BA384), GetMemory(0x5BA42C));
    NewList(unit);
    Enchant(unit, "ENCHANT_FREEZE", 0.0);
    Enchant(unit, "ENCHANT_RUN", 0.0);
    return unit;
}

int SpecialSword3(int wp)
{
    int unit = CreateObject("GreatSword", wp);

    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 2);
    SetDirectWeaponProperty(unit, WeaponPower(5), GetMemory(0x005BA354), GetMemory(0x5BA384), GetMemory(0x5BA42C));
    NewList(unit);
    Enchant(unit, "ENCHANT_PROTECT_FROM_FIRE", 0.0);
    Enchant(unit, "ENCHANT_RUN", 0.0);
    return unit;
}

int SpecialSword4(int wp)
{
    int unit = CreateObject("WarHammer", wp);

    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 3);
    SetDirectWeaponProperty(unit, WeaponPower(5), GetMemory(0x5BA2AC), GetMemory(0x005BA30C), GetMemory(0x5BA3CC));
    NewList(unit);
    Enchant(unit, "ENCHANT_AFRAID", 0.0);
    return unit;
}

int SpecialSword5(int wp)
{
    int unit = CreateObject("GreatSword", wp);

    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 4);
    NewList(unit);
    SetDirectWeaponProperty(unit, WeaponPower(5), GetMemory(0x5BA60C), GetMemory(0x5BA204), GetMemory(0x5BA36C));
    Enchant(unit, "ENCHANT_AFRAID", 0.0);
    return unit;
}

int SpecialSword6(int wp)
{
    int unit = CreateObject("GreatSword", wp);

    SetDirectWeaponProperty(unit, WeaponPower(5), GetMemory(0x5BA42C), GetMemory(0x5BA444), GetMemory(0x5BA24C)); //Venom+fire
    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 5);
    NewList(unit);
    Enchant(unit, "ENCHANT_SLOWED", 0.0);
    return unit;
}

int SpecialSword7(int wp)
{
    int unit = CreateObject("GreatSword", wp);

    SetDirectWeaponProperty(unit, WeaponPower(5), WeaponEffect(21), GetMemory(0x5BA2C4), GetMemory(0x5BA36C)); //Impact+light
    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 6);
    NewList(unit);
    Enchant(unit, "ENCHANT_REFLECTIVE_SHIELD", 0.0);
    return unit;
}

int SpecialSword8(int wp)
{
    int unit = CreateObject("WarHammer", wp);
    int ptr = GetMemory(0x750710);

    SetDirectWeaponProperty(unit, WeaponPower(5), WeaponEffect(21), WeaponEffect(20), WeaponEffect(15));
    LookWithAngle(CreateObject("InvisibleLightBlueHigh", wp), 7);
    NewList(unit);
    Enchant(unit, "ENCHANT_PROTECT_FROM_POISON", 0.0);
    return unit;
}

void PutSpecialShop()
{
    //LifeShop
    //DamageUpgradeShop
    //etc...
    int ptr = CreateObject("Maiden", 21);

    Frozen(CreateObject("Ankh", 21), 1);
    SetDialog(AbilityButton("Swordsman", 67, 0, 2, 15000), "a", BuyNewSkill, BuyNewSkill);
    SetDialog(AbilityButton("Swordsman", 32, 1, 4, 80000), "a", BuyNewSkill, BuyNewSkill);
    SetDialog(AbilityButton("Swordsman", 33, 2, 8, 30000), "a", BuyNewSkill, BuyNewSkill);
    SetDialog(AbilityButton("Swordsman", 49, 3, 0x10, 80000), "a", BuyNewSkill, BuyNewSkill);

    SetDialog(SpecialWeaponShop("Maiden", 34, 0, 85000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(SpecialWeaponShop("Maiden", 35, 1, 80000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(SpecialWeaponShop("Maiden", 36, 2, 80000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(SpecialWeaponShop("Maiden", 37, 3, 87000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(SpecialWeaponShop("Maiden", 38, 4, 90000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(SpecialWeaponShop("Maiden", 39, 5, 75000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(SpecialWeaponShop("Maiden", 40, 6, 75000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(SpecialWeaponShop("Maiden", 41, 7, 75000), "a", BuySpecialWeapon, BuySpecialWeapon);
    SetDialog(AbilityButton("Necromancer", 43, 0, 0, 0), "a", RepairShoppingNet, RepairShoppingNet);
    Frozen(ptr, 1);
    SetDialog(ptr, "YESNO", TakeNewLifeDesc, TakeNewLifeTrade);
}

void AbilityAwardEvent(int unit)
{
    TeleportLocation(1, GetObjectX(unit), GetObjectY(unit));
    Effect("WHITE_FLASH", LocationX(1), LocationY(1), 0.0, 0.0);
    GreenSparkFx(LocationX(1), LocationY(1));
    AudioEvent("AwardSpell", 1);
}

string SkillDescript(int num)
{
    string name[] = {
        "[스킬설명] 순보: 짧은 거리를 빠르게 이동한다. 조심스럽게 걷기 시전을 통해 발동된다",
        "[스킬설명] 피카츄의 \"백만볼트\": 피카 츄~~~~~~. 발동키: J",
        "[스킬설명] 버저커차지 시전 쿨다운을 없앱니다. 조심스럽게 걷기 시전을 통해 발동되며 순보 마술과 중복됨에 유의하세요",
        "[패시브] 캐릭터의 체력회복과 이동속도 모두 눈에띄게 빨라집니다"};

    return name[num];
}

string DescriptSpecialWeapon(int num)
{
    string name[] = {
        "[스페셜 무기] 광선검: 스타워즈에서 나오는 그 광선검이다",
        "[스페셜 무기] 백야 혼동망치: 내리치면 흰색 오로라가 생기며 주위에 200의 피해를 준다",
        "[스페셜 무기] 에너지파 소드: 손오공의 에너지파가 발사되는 검이다",
        "[스페셜 무기] 수리검해머: 해머를 내리치면 해머 파편이 사방으로 튀면서 적에게 피해를 준다",
        "[스페셜 무기] 죽음의 흡혈소드: 전방 적에게 데스레이 한방을 쏘고 맞은 적으로 부터 체력을 2씩 회복한다",
        "[스페셜 무기] 극사 소드: 적을 끝까지 추적하며 \"대상은 무조건 죽는다\"",
        "[스페셜 무기] 하이에나 소드: 전방으로 하이에나를 돌진시키며 적과 닿으면 폭발한다",
        "[스페셜 무기] 봄버 소환 해머: 해머를 내리칠 때 마다 봄버가 소환된다. 이 봄버는 6 초 후 사라진다"};

    return name[num];
}

void RepairShoppingNet()
{
    int res;

    if (HasEnchant(other, "ENCHANT_AFRAID"))
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (GetGold(other) >= 3000)
        {
            res = RepairAll(other);
            if (res)
            {
                MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
                DeleteObjectTimer(CreateObjectAt("BigSmoke", GetObjectX(self), GetObjectY(self)), 12);
                Effect("SPARK_EXPLOSION", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
                AudioEvent("ShopRepairItem", 1);
                ChangeGold(other, -3000);
                UniPrint(other, "결제성공! 모두 " + IntToString(res) + "개의 인벤토리가 수리되었습니다");
            }
        }
        else
            UniPrint(other, "금액이 부족합니다");
    }
    else
    {
        Enchant(other, "ENCHANT_AFRAID", 0.3);
        UniPrint(other, "자신이 소유한 모든 인벤토리를 수리합니다, 이 작업은 3000원이 필요하며 계속 진행하려면 더블클릭 하세요");
    }
}

void BuySpecialWeapon()
{
    int plr, pay = ToInt(GetObjectZ(GetTrigger() + 1)), idx = GetDirection(self);

    if (HasEnchant(other, "ENCHANT_AFRAID"))
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (GetGold(other) >= pay)
        {
            ChangeGold(other, -pay);
            MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
            AudioEvent("TreasureDrop", 1);
            CallFunctionWithArg(SpecialWeaponPlaceFunctionTable(idx), 1);
            UniPrint(other, "결제완료");
        }
        else
            UniPrint(other, "잔액이 부족합니다!");
    }
    else
    {
        Enchant(other, "ENCHANT_AFRAID", 0.3);
        UniPrint(other, DescriptSpecialWeapon(idx));
        UniPrint(other, "이 작업은 당신에게 " + IntToString(pay) + "원을 요구합니다. 계속하려면 더블클릭 하세요");
    }
}

void BuyNewSkill()
{
    int plr, pay = ToInt(GetObjectZ(GetTrigger() + 1)), byte = GetDirection(GetTrigger() + 1);

    if (HasEnchant(other, "ENCHANT_AFRAID"))
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (GetGold(other) >= pay)
        {
            plr = CheckPlayer();
            if (plr + 1)
            {
                if (player[plr + 10] & byte)
                {
                    UniPrint(other, "오류_ 이미 습득한 상태 입니다!");
                }
                else
                {
                    player[plr + 10] = player[plr + 10] ^ byte;
                    ChangeGold(other, -pay);
                    AbilityAwardEvent(GetCaller());
                    UniPrint(other, "성공_ 마법 습득완료");
                }
            }
        }
    }
    else
    {
        Enchant(other, "ENCHANT_AFRAID", 0.3);
        UniPrint(other, SkillDescript(GetDirection(self)));
        UniPrint(other, "이 작업은 당신에게 " + IntToString(pay) + "원을 요구합니다. 계속하려면 더블클릭 하세요");
    }
}

void TakeNewLifeDesc()
{
    int coin = XtraUserCoins;
    PlaySoundAround(other, 801);
    UniChatMessage(self, "원활한 게임 진행을 위해 필요한 코인 1개를 구입하시겠어요?", 150);
    UniPrint(other, "코인 1개 구입은 3만 골드가 요구됩니다. 거래를 계속하려면 '예'를 누르세요");
    UniPrint(other, "현재 " + IntToString(coin) + " 개 코인을 보유 중 입니다");
    TellStoryUnitName("aa", "Con10B.scr:HecubahLine3", "코인1개 구입");
}

void TakeNewLifeTrade()
{
    int coin = XtraUserCoins;
    if (GetAnswer(self) ^ 1) return;
    if (GetGold(other) >= 30000)
    {
        if (coin < 100)
        {
            PlaySoundAround(other, 1004);
            ChangeGold(other, -30000);
            XtraUserCoins ++;
            Effect("YELLOW_SPARKS", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        }
        else
            UniPrint(other, "소유 가능한 최대 라이프 수에 도달했기 때문에 더 이상 라이프를 추가할 수 없습니다");
    }
    else
        UniPrint(other, "금액이 부족합니다");
    UniPrint(other, "현재 보유중인 코인: " + IntToString(XtraUserCoins));
}

void MapDecorations()
{
    BackRow();
    Frozen(CreateObjectAt("DunMirScaleTorch2", 2588.0, 4635.0), 1);
    Frozen(CreateObjectAt("DunMirScaleTorch2", 2771.0, 4446.0), 1);
    Frozen(CreateObjectAt("DunMirScaleTorch2", 3197.0, 3820.0), 1);
    Frozen(CreateObject("DunMirScaleTorch1", 42), 1);
    HealingWishWell(58);
    HealingWishWell(59);
    HealingWishWell(60);
    InitDungeonGates(DunCnt);
    FrameTimer(1, InitMapSigns);
    FrameTimer(1, PutSpecialShop);
    //FrameTimer(3, PutDungeonNameStampString);
    FrameTimer(15, InitStampStrings);
    FrameTimer(220, TestFunction);
    FrameTimer(31, InitList);
    ShopClassInit(68);
    FrameTimer(50, InitUpLevelSecretZone);
}

void MapExit()
{
    MusicEvent();
    RemoveCoopTeamMode();
    ResetPlayerHandlerWhenExitMap();
}

void PoisonZombieDeathHandler()
{
    MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
    AudioEvent("BurnCast", 1);
    Effect("SPARK_EXPLOSION", LocationX(1), LocationY(1), 0.0, 0.0);
    DeleteObjectTimer(CreateObject("MediumFlame", 1), 90);
    DeleteObjectTimer(CreateObject("ReleasedSoul", 1), 30);
    DungeonUnitDeathHandler();
}

void BlackSpiderDeathHandler()
{
    MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
    AudioEvent("BeholderDie", 1);
    DeleteObjectTimer(CreateObject("BigSmoke", 1), 9);
    DeleteObjectTimer(CreateObject("WaterBarrelBreaking", 1), 9);
    CreateObject("ArachnaphobiaFocus", 1);
    DungeonUnitDeathHandler();
}

void UnitDeathHandler()
{
    NormalMonsterItemRelease(self);
    DeleteObjectTimer(self, 90);
}

void DungeonUnitDeathHandler()
{
    DungeonMonsterItemRelease(self);
    DeleteObjectTimer(self, 90);
}

void WeirdlingDie()
{
    CreateObjectAt("BreakingSoup", GetObjectX(self), GetObjectY(self));
    DungeonUnitDeathHandler();
}

int LowLevelMobFunctions(int index)
{
    int lowLevels[] = {SpawnUrchin, SpawnBat, SpawnSmallWhiteSpider, SpawnLeech, SpawnSwordman, SpawnArcher};

    return lowLevels[index];
}

int DungeonMobFunctions(int index)
{
    int medLevels[] = { SpawnSwordman, SpawnArcher, SpawnEmberDemon, SpawnGargoyle, SpawnBlackSpider,
        SpawnWolf, SpawnBlackWolf, SpawnOgre, SpawnOgreAxe, SpawnDarkBear, SpawnBrownBear,
        SpawnSkeleton, SpawnSkeletonLord, SpawnGhost, SpawnScorpion, SpawnGoon, SpawnOgreLord,
        SpawnRedGirl, SpawnWhiteSpider, SpawnPoisonZombie, SpawnMeleeLich, SpawnMeleeNecromancer,
        SpawnDeadWizard, SpawnCaptain, SpawnPlant, SpawnBeast };

    return medLevels[index];
}

int SpawnUrchin(int ptr)
{
    int unit = CreateObjectAt("Urchin", GetObjectX(ptr), GetObjectY(ptr));

    UnitZeroFleeRange(unit);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 64);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnBat(int ptr)
{
    int unit = CreateObjectAt("Bat", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 64);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnSmallWhiteSpider(int ptr)
{
    int unit = CreateObjectAt("SmallAlbinoSpider", GetObjectX(ptr), GetObjectY(ptr));

    Enchant(unit, "ENCHANT_SLOWED", 0.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 74);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnLeech(int ptr)
{
    int unit = CreateObjectAt("GiantLeech", GetObjectX(ptr), GetObjectY(ptr));

    Enchant(unit, "ENCHANT_VAMPIRISM", 0.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 120);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnSwordman(int ptr)
{
    int unit = CreateObjectAt("Swordsman", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, 180);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnArcher(int ptr)
{
    int unit = CreateObjectAt("Archer", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 98);
    RetreatLevel(unit, 0.0);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnEmberDemon(int ptr)
{
    int unit = CreateObjectAt("EmberDemon", GetObjectX(ptr), GetObjectY(ptr));
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 170);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnGargoyle(int ptr)
{
    int unit = CreateObjectAt("EvilCherub", GetObjectX(ptr), GetObjectY(ptr));
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 98);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnBlackSpider(int ptr)
{
    int unit = CreateObjectAt("BlackWidow", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, BlackWidowBinTable());
    SetUnitVoice(unit, 19);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 275);
    SetCallback(unit, 5, BlackSpiderDeathHandler);
    return unit;
}

int SpawnWolf(int ptr)
{
    int unit = CreateObjectAt("Wolf", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, 135);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnBlackWolf(int ptr)
{
    int unit = CreateObjectAt("BlackWolf", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnOgre(int ptr)
{
    int unit = CreateObjectAt("OgreBrute", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, 295);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnOgreAxe(int ptr)
{
    int unit = CreateObjectAt("GruntAxe", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnDarkBear(int ptr)
{
    int unit = CreateObjectAt("BlackBear", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, 306);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnBrownBear(int ptr)
{
    int unit = CreateObjectAt("Bear", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    RetreatLevel(unit, 0.0);
    SetUnitMaxHealth(unit, 400);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnSkeleton(int ptr)
{
    int unit = CreateObjectAt("Skeleton", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 250);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnSkeletonLord(int ptr)
{
    int unit = CreateObjectAt("SkeletonLord", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 295);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnGhost(int ptr)
{
    int unit = CreateObjectAt("Ghost", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    SetUnitMaxHealth(unit, 98);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnScorpion(int ptr)
{
    int unit = CreateObjectAt("Scorpion", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 260);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnGoon(int ptr)
{
    int unit = CreateObjectAt("Goon", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, GoonBinTable());
    SetUnitVoice(unit, 63);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 128);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnOgreLord(int ptr)
{
    int unit = CreateObjectAt("OgreWarlord", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 325);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnRedGirl(int ptr)
{
    int unit = ColorMaidenAt(0, 225, 64, ptr);

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnWhiteSpider(int ptr)
{
    int unit = CreateObjectAt("AlbinoSpider", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 128);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnPoisonZombie(int ptr)
{
    int unit = CreateObjectAt("VileZombie", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 275);
    SetUnitSpeed(unit, 3.0);
    SetCallback(unit, 5, PoisonZombieDeathHandler);
    return unit;
}

int SpawnMeleeLich(int ptr)
{
    int unit = CreateObjectAt("Lich", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, LichLordBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 260);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    UnitZeroFleeRange(unit);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x20);
    AggressionLevel(unit, 1.0);
    RetreatLevel(unit, 0.0);
    return unit;
}

int SpawnMeleeNecromancer(int ptr)
{
    int unit = CreateObjectAt("Necromancer", GetObjectX(ptr), GetObjectY(ptr));

    NecromancerSubProcess(unit);
    SetOwner(GetMaster(), unit);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    return unit;
}

int SpawnDeadWizard(int ptr)
{
    int unit = CreateObjectAt("WizardRed", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, WizardRedBinTable());
    UnitZeroFleeRange(unit);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    AggressionLevel(unit, 1.0);
    RetreatLevel(unit, 0.0);
    return unit;
}

int SpawnCaptain(int ptr)
{
    int unit = CreateObjectAt("AirshipCaptain", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, AirshipCaptainBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 325);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    AggressionLevel(unit, 1.0);
    RetreatLevel(unit, 0.0);
    return unit;
}

int SpawnPlant(int ptr)
{
    int unit = CreateObjectAt("CarnivorousPlant", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 300);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    SetUnitSpeed(unit, 1.6);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    AggressionLevel(unit, 1.0);
    return unit;
}

int SpawnBeast(int ptr)
{
    int unit = CreateObjectAt("WeirdlingBeast", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, WeirdlingBeastBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 175);
    //SetCallback(unit, 5, DungeonUnitDeathHandler);
    SetCallback(unit, 5, WeirdlingDie);
    UnitZeroFleeRange(unit);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    AggressionLevel(unit, 1.0);
    return unit;
}

int ColorMaidenAt(int red, int grn, int blue, int ptr)
{
    int unit = CreateObjectAt("Bear2", GetObjectX(ptr), GetObjectY(ptr));
    int ptr1 = GetMemory(0x750710), k, num;

    UnitLinkBinScript(unit, MaidenBinTable());
    SetMemory(ptr1 + 4, 1385);
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
    SetUnitVoice(unit, 7);

    return unit;
}

void TeleportAllPlayer(int wp)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (CurrentHealth(player[i]))
            MoveObject(player[i], LocationX(wp), LocationY(wp));
    }
}

void ShutdownThisGame(int wp)
{
    ObjectOff(Object("MainProcBeacon"));
    TeleportAllPlayer(wp);
    MoveObject(StartLocationWithPlayer(), LocationX(wp), LocationY(wp));
}

void PlayerDeath()
{
    if (XtraUserCoins)
    {
        UniPrintToAll("누군가 죽었습니다, 라이프가 하나 차감되며 더 이상 라이프가 없을 시 패배처리가 됩니다, 남은 라이프: " + IntToString(--XtraUserCoins));
    }
    else
    {
        DefeatMission();
    }
}

void DefeatMission()
{
    if (!GOver)
    {
        GOver = 1;
        ShutdownThisGame(20);
        AudioEvent("StaffOblivionAchieve1", 20);
        UniPrintToAll("미션실패! 라이프가 모두 소모되었습니다!!");

        FrameTimer(30, StrMissionFail);
    }
}

int PlayerClassCheckDeathFlag(int plr)
{
    return player[plr + 10] & 0x80;
}

void PlayerClassSetDeathFlag(int plr)
{
    player[plr + 10] = player[plr + 10] ^ 0x80;
}

int PlayerClassOnInit(int plrIdx, int plrUnit)
{
    int pResult = plrIdx;

    player[plrIdx] = plrUnit;
    player[plrIdx + 10] = 1;
    ChangeGold(plrUnit, -GetGold(plrUnit));
    SelfDamageClassEntry(plrUnit);
    DiePlayerHandlerEntry(plrUnit);
    EmptyAll(plrUnit);
    if (ValidPlayerCheck(plrUnit))
    {
        if (plrUnit ^ GetHost())
            NetworkUtilClientEntry(plrUnit);
        else
            PlayerClassCommonWhenEntry();
        pResult |= (1 << 8);
    }
    UniPrintToAll(PlayerIngameNick(plrUnit) + " 님께서 게임에 참가하셨습니다");
    return pResult;
}

void PlayerEntryPoint()
{
    int plr, i;

    while (1)
    {
        if (CurrentHealth(other))
        {
            if (MaxHealth(other) == 150)
            {
                plr = CheckPlayer();
                for (i = 9 ; i >= 0 && plr < 0 ; i --)
                {
                    if (!MaxHealth(player[i]))
                    {
                        plr = PlayerClassOnInit(i, GetCaller());
                        break;
                    }
                }
                if (plr + 1)
                {
                    if (plr >> 8)
                        PlayerClassFirstJoin(other, plr & 0xff);
                    else
                        PlayerJoin(plr);
                    break;
                }
            }
        }
        PlayerCantJoin();
        break;
    }
}

void EmptyAll(int unit)
{
    while (GetLastItem(unit))
        Delete(GetLastItem(unit));
}

void EquipFix(int plrUnit)
{
    int invPtr;
    int vptr = GetScrDataField(EquipFix) + 4;
    int ptr = UnitToPtr(plrUnit);

    if (!ptr)
        return;
    SetMemory(vptr, GetMemory(ptr + 0x1f8));
    while (invPtr)
    {
        if (GetMemory(invPtr + 0x10) & 0x100)
        {
            ItemEquip(plrUnit, GetMemory(invPtr + 0x2c), false);
            ItemEquip(plrUnit, GetMemory(invPtr + 0x2c), true);
        }
        SetMemory(vptr, GetMemory(invPtr + 0x1f0));
    }
}

int ImportItemEquip()
{
    int arr[10], link;

    if (!link)
    {
        arr[0] = 0x50EC8B55; arr[1] = 0xE804EC83; arr[2] = 0xFFDB6244; arr[3] = 0xE8240489; arr[4] = 0xFFDB623C;
        arr[5] = 0x502434FF; arr[6] = 0xDA1F53E8; arr[7] = 0x0CC483FF; arr[8] = 0x90C35D58;
        link = GetScrDataField(ImportItemEquip);
        FixCallOpcode(link + 0x7, 0x507250);
        FixCallOpcode(link + 0xf, 0x507250);
        FixCallOpcode(link + 0x18, 0x4f2f70);
    }
    return link;
}

void ItemEquip(int humanUnit, int item, int equipMode)
{
    int unitPtr = UnitToPtr(humanUnit);
    int itemPtr = UnitToPtr(item), temp;

    if (unitPtr && itemPtr)
    {
        temp = GetMemory(0x5c31f4);
        int link = ImportItemEquip();
        if (equipMode)
            FixCallOpcode(link + 0x18, 0x4f2f70);
        else
            FixCallOpcode(link + 0x18, 0x4f2fb0);
        SetMemory(0x5c31f4, ImportItemEquip());
        Unused5a(unitPtr, itemPtr);
        SetMemory(0x5c31f4, temp);
    }
}

void TeleportPlayer(int pArg)
{
    int location = pArg & 0x3ff, plr = (pArg >> 10) & 0x3ff;
    int pUnit = player[plr];

    if (CurrentHealth(pUnit))
        MoveObject(pUnit, LocationX(location), LocationY(location));
}

void PlayerClassFirstJoin(int pUnit, int plr)
{
    Enchant(pUnit, "ENCHANT_BLINDED", 1.0);
    MoveObject(pUnit, LocationX(91), LocationY(91));
    FrameTimerWithArg(25, (plr << 10) | 89, TeleportPlayer);
    UniPrint(pUnit, "지도에 입장을 시도하고 있습니다... 잠시만 기다려 주세요");
}

void PlayerJoin(int plr)
{
    if (PlayerClassCheckDeathFlag(plr))
    {
        // EquipFix(player[plr]);  //@brief. 28th march 2021 23:04 추가
        PlayerClassSetDeathFlag(plr);
    }
    MoveObject(player[plr], LocationX(12), LocationY(12));
    Effect("TELEPORT", LocationX(12), LocationY(12), 0.0, 0.0);
    DeleteObjectTimer(CreateObject("BlueRain", 12), 10);
    AudioEvent("BlindOff", 12);
}

void PlayerCantJoin()
{
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    Enchant(other, "ENCHANT_ANCHORED", 0.0);
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    Enchant(other, "ENCHANT_BLINDED", 0.0);
    MoveObject(other, LocationX(11), LocationY(11));
    UniPrintToAll("맵 입장에 실패했습니다");
}

void PlayerClassOnFree(int plr)
{
    int *ptr = UnitToPtr(player[plr]);

    if (ptr != NULLPTR)
        CancelPlayerDialogWithPTR(ptr);
    player[plr] = 0;
    player[plr + 10] = 0;
}

void PlayerClassOnDeath(int plr)
{
    UniPrintToAll(PlayerIngameNick(player[plr]) + " 님께서 적에게 격추되었습니다");
}

void PlayerPreserveHandler()
{
    int i;

    for (i = 9 ; i >= 0 ; i-=1)
    {
        while (1)
        {
            if (MaxHealth(player[i]))
            {
                if (GetUnitFlags(player[i]) & 0x40)
                    1;
                else if (CurrentHealth(player[i]))
                {
                    CheckUseSkill(i);
                    break;
                }
                else
                {
                    if (PlayerClassCheckDeathFlag(i)) break;
                    else
                    {
                        PlayerClassSetDeathFlag(i);
                        PlayerClassOnDeath(i);
                    }
                    break;
                }
            }
            if (player[i + 10])
                PlayerClassOnFree(i);
            break;
        }
    }
    FrameTimer(1, PlayerPreserveHandler);
}

void CheckUseSkill(int plr)
{
    if (HasEnchant(player[plr], "ENCHANT_SNEAK"))
    {
        EnchantOff(player[plr], "ENCHANT_SNEAK");
        RemoveTreadLightly(player[plr]);
        if (player[plr + 10] & 0x2)
            PlayerWindBoost(player[plr]);
        else if (player[plr + 10] & 8)
        {
            BerserkerNoDelayCore(plr);
            GreenSparkFx(GetObjectX(player[plr]), GetObjectY(player[plr]));
        }
    }
    if (player[plr + 10] & 0x04)
    {
        if (CheckPlayerInput(player[plr]) == 47)
        {
            if (!HasEnchant(player[plr], "ENCHANT_LIGHT"))
                ThunderBolt(player[plr]);
        }
    }
    if (player[plr + 10] & 0x10)
    {
        AdvancedStat(player[plr]);
    }
}

int StartLocationWithPlayer()
{
    int start;

    if (!start)
        start = Object("PlayerStartLocation");
    return start;
}

void PlayerFastJoin()
{
    int plr = CheckPlayer();

    if (CurrentHealth(other))
    {
        if (plr < 0)
            MoveObject(other, LocationX(90), LocationY(90));
        else
        {
            MoveObject(other, LocationX(89), LocationY(89));
            UniPrint(other, "패스트 조인되었습니다");
        }
    }
}

void AdvancedStat(int unit)
{
    if (!GOver)
    {
        if (!HasEnchant(unit, "ENCHANT_BURNING"))
        {
            Enchant(unit, "ENCHANT_BURNING", 0.1);
            RestoreHealth(unit, 1);
            Effect("GREATER_HEAL", GetObjectX(unit), GetObjectY(unit), GetObjectX(unit), GetObjectY(unit) - 180.0);
        }
        if (GetPlayerAction(unit) == 5)
            PushObjectTo(unit, UnitAngleCos(unit, 1.3), UnitAngleSin(unit, 1.3));
    }
}

void PlayerWindBoost(int unit)
{
    Effect("RICOCHET", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
    PushObjectTo(unit, UnitAngleCos(unit, 60.0), UnitAngleSin(unit, 60.0));
}

void ThunderBolt(int unit)
{
    float vectX = UnitAngleCos(unit, 38.0), vectY = UnitAngleSin(unit, 38.0);
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit) + vectX, GetObjectY(unit) + vectY);

    Enchant(unit, "ENCHANT_LIGHT", 8.0);
    Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(ptr), GetObjectY(ptr)), vectY);
    Raise(ptr, vectX);
    SetOwner(unit, ptr);
    FrameTimerWithArg(3, ptr, ThunderBoltCritical);
}

void ThunderBoltCritical(int ptr)
{
    float vectX = GetObjectZ(ptr), vectY = GetObjectZ(ptr + 1);
    int owner = GetOwner(ptr), i, unit;

    if (CurrentHealth(owner))
    {
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(ptr), GetObjectY(ptr)) + 1;
        Raise(unit - 1, ToFloat(owner));
        for (i = 0 ; i < 13 ; i ++)
        {
            Frozen(CreateObjectAt("ShopkeeperConjurerRealm", GetObjectX(ptr), GetObjectY(ptr)), 1);
            SetOwner(unit - 1, unit + i);
            DeleteObjectTimer(unit + i, 1);
            SetUnitFlags(unit + i, GetUnitFlags(unit + i) ^ 0x2000);
            SetCallback(unit + i, 9, ThunderBoltCollideHandler);
            MoveObject(ptr, GetObjectX(ptr) + vectX, GetObjectY(ptr) + vectY);
            if (!IsVisibleTo(ptr, ptr + 1))
                break;
        }
        YellowLightningFx(GetObjectX(ptr + 1), GetObjectY(ptr + 1), GetObjectX(ptr), GetObjectY(ptr), 24);
        DeleteObjectTimer(unit - 1, 3);
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void ThunderBoltCollideHandler()
{
    int owner = ToInt(GetObjectZ(GetOwner(self)));

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 200, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.09);
    }
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

int UnitEquipedWeapon(int unit)
{
    int ptr = UnitToPtr(unit), pic;
    
    if (ptr)
    {
        pic = GetMemory(GetMemory(ptr + 0x2ec) + 0x810);
        if (pic)
            return GetMemory(pic + 0x2c);
    }
    return 0;
}

void CheckResetSight(int unit, int delay)
{
    if (!UnitCheckEnchant(unit, GetLShift(6)))
    {
        Enchant(unit, EnchantList(6), 0.0);
        FrameTimerWithArg(delay, unit, ResetUnitSight);
    }
}

void ResetUnitSight(int unit)
{
    EnchantOff(unit, EnchantList(6));
    Enchant(unit, EnchantList(2), 0.06);
    AggressionLevel(unit, 1.0);
}

void UserMapSetting()
{
    SetMemory(0x5d5330, 0x2000);
    SetMemory(0x5d5394, 1);
    StartLocationWithPlayer();
}

static int MonsterMeleeAttackRegistCallback()
{
    return MonsterStrikeCallback;
}

void MapInitialize()
{
    MusicEvent();
    LastUnit = CreateObject("RedPotion", 1);
    Delete(LastUnit);

    InitMathSine(1);
    SafeZone = CreateObject("InvisibleLightBlueHigh", 12);
    Enchant(CreateObject("InvisibleLightBlueHigh", 12), "ENCHANT_SHIELD", 0.0);
    Enchant(CreateObject("InvisibleLightBlueHigh", 12), "ENCHANT_VAMPIRISM", 0.0);
    GetMaster();
    FrameTimer(1, MakeCoopTeam);
    FrameTimer(10, UserMapSetting);
    FrameTimer(8, MapDecorations);
    FrameTimerWithArg(20, Object("FirstMonsterMarker"), StartMarkerScan);   //fixme. 여기서 계속 팅김
    FrameTimer(10, PlayerPreserveHandler);
    FrameTimerWithArg(20, Object("UndergroundElev"), DisableObject);
}

int SummonPowerGhost(int ptr)
{
    int unit = CreateObjectAt("Ghost", GetObjectX(ptr), GetObjectY(ptr));

    SetUnitScanRange(unit, 450.0);
    SetUnitMaxHealth(unit, 170);
    SetCallback(unit, 3, GhostStrike);
    SetCallback(unit, 13, WhenRunout);

    return unit;
}

int SummonPowerPurpleGirl(int ptr)
{
    int unit = CreateObjectAt("TalkingSkull", GetObjectX(ptr), GetObjectY(ptr));

    UnitNoCollide(CreateObjectAt("MovableStatueVictory3NE", GetObjectX(unit), GetObjectY(unit)));
    SetUnitScanRange(unit, 400.0);
    BullDakSubProcess(unit);
    RegistUnitStrikeHook(unit);
    FrameTimerWithArg(1, unit + 1, BullDakImageLoop);
    return unit;
}

int SummonPowerMonster3(int ptr)
{
    int unit = CreateObjectAt("Demon", GetObjectX(ptr), GetObjectY(ptr));

    CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit));
    SetCallback(unit, 3, DemonSightEvent);
    SetCallback(unit, 13, DemonResetSight);
    SetUnitScanRange(unit, 400.0);
    SetUnitMaxHealth(unit, 500);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x8020); //ALWAYS_RUN

    return unit;
}

int SummonGWizard(int ptr)
{
    int unit = CreateObjectAt("WizardGreen", GetObjectX(ptr), GetObjectY(ptr));

    CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit));
    SetCallback(unit, 3, GWizSightEvent);
	SetCallback(unit, 13, GWizResetSight);
    SetUnitScanRange(unit, 400.0);
    SetUnitMaxHealth(unit, 420);
    UnitZeroFleeRange(unit);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x8020); //ALWAYS_RUN

    return unit;
}

int SummonPowerMonster5(int ptr)
{
    int unit = CreateObjectAt("FireSprite", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, FireSpriteBinTable());
    SetUnitScanRange(unit, 400.0);
    SetUnitMaxHealth(unit, 420);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000); //ALWAYS_RUN
    SetCallback(unit, 3, FireSpritEnemyFind);
    SetCallback(unit, 13, WhenRunout);
    SetUnitSpeed(unit, ToFloat(1076426178));

    return unit;
}

int SummonPowerMecaGolem(int ptr)
{
    int unit = CreateObjectAt("StoneGolem", GetObjectX(ptr), GetObjectY(ptr));

    SetUnitScanRange(unit, 450.0);
    SetUnitMaxHealth(unit, 800);
    SetCallback(unit, 3, MecaGolemStrike);
    SetCallback(unit, 13, WhenRunout);

    return unit;
}

int SummonHecubah(int target)
{
    int unit = CreateObjectAt("Hecubah", GetObjectX(target), GetObjectY(target));
    
    HecubahSubProcess(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)) - 1);
    SetUnitScanRange(unit, 500.0);
    SetUnitMaxHealth(unit, 1350);
    SetCallback(unit, 3, HecubahSight);
    SetCallback(unit, 13, HecubahLostEnemy);

    return unit;
}

int SummonBomber(int target)
{
    string name[] = {"Bomber", "BomberBlue", "BomberGreen", "BomberYellow"};
    int unit = CreateObjectAt(name[Random(0, 3)], GetObjectX(target), GetObjectY(target));
    int ptr = GetMemory(0x750710);

    UnitLinkBinScript(unit, BomberGreenBinTable());
    SetUnitVoice(unit, 57);
    SetUnitMaxHealth(unit, 600);
    SetMemory(ptr + 0x2b8, 0x4e83b0);
    SetUnitScanRange(unit, 330.0);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    AggressionLevel(unit, 1.0);

    return unit;
}

void HecubahImpactDamage(int sUnit)
{
    int owner = GetOwner(sUnit), durate = GetDirection(sUnit), target = ToInt(GetObjectZ(sUnit));

    while (IsObjectOn(sUnit))
    {
        if (CurrentHealth(target))
        {
            if (GetWallAtObjectPos(target))
            {
                Damage(target, 0, 120, 11);
                PlaySoundAround(target, 42);
                Effect("SPARK_EXPLOSION", GetObjectX(target), GetObjectY(target), 0.0, 0.0);
            }
            else if (durate)
            {
                FrameTimerWithArg(1, sUnit, HecubahImpactDamage);
                LookWithAngle(sUnit, durate - 1);
                MoveObject(sUnit + 1, GetObjectX(target) + UnitAngleCos(sUnit + 1, 8.0), GetObjectY(target) + UnitAngleSin(sUnit + 1, 8.0));
                if (!ToInt(GetObjectZ(sUnit + 1)))
                {
                    Raise(sUnit + 1, 6.0);
                    Damage(target, 0, 20, 11);
                }
                PlaySoundAround(target, 171);
                DeleteObjectTimer(CreateObjectAt("OldSmoke", GetObjectX(sUnit + 1), GetObjectY(sUnit + 1)), 12);
                break;
            }
        }
        Delete(sUnit);
        Delete(sUnit + 1);
        break;
    }
}

void HecubahMeleeAttack(int caster, int target)
{
    int unit = CreateObjectAt("ImaginaryCaster", GetObjectX(caster), GetObjectY(caster));

    Frozen(CreateObjectAt("Maiden", GetObjectX(target) + UnitRatioX(caster, target, 3.0), GetObjectY(target) + UnitRatioY(caster, target, 3.0)), 1);
    LookAtObject(unit + 1, caster);
    Raise(unit, target);
    SetOwner(caster, unit);
    LookWithAngle(unit, 128);
    Enchant(target, EnchantList(25), 3.0);
    FrameTimerWithArg(1, unit, HecubahImpactDamage);
}

void BlueCrystalCollide()
{
    int owner = GetOwner(self);

    while (1)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 200, 9);
            Effect("VIOLET_SPARKS", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        Delete(self);
        break;
    }
}

int ThrowBlueCrystal(int sUnit, int sTarget)
{
    int mis = CreateObjectAt("Gameball", GetObjectX(sUnit) + UnitRatioX(sTarget, sUnit, 19.0), GetObjectY(sUnit) + UnitRatioY(sTarget, sUnit, 19.0));

    SetUnitCallbackOnCollide(mis, BlueCrystalCollide);
    SetOwner(sUnit, mis);
    DeleteObjectTimer(mis, 240);

    return mis;
}

void DelayShootToTarget(int sUnit)
{
    int durate = GetDirection(sUnit);
    int mis = sUnit + 1, target = ToInt(GetObjectZ(sUnit));
    
    while (1)
    {
        if (IsObjectOn(mis) && CurrentHealth(target))
        {
            if (durate)
            {
                PushObjectTo(mis, UnitRatioX(target, mis, 0.6), UnitRatioY(target, mis, 0.6));
                LookWithAngle(sUnit, durate - 1);
                FrameTimerWithArg(1, sUnit, DelayShootToTarget);
                break;
            }
        }
        Delete(sUnit);
        break;
    }
}

void HecubahSight()
{
    int delay = 30, unit;

    if (!UnitCheckEnchant(self, GetLShift(30)))
    {
        if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) < 200.0)
        {
            LookAtObject(self, other);
            HitLocation(self, GetObjectX(self), GetObjectY(self));
            HecubahMeleeAttack(GetTrigger(), GetCaller());
        }
        else
        {
            unit = CreateObjectAt("ImaginaryCaster", GetObjectX(self), GetObjectY(self));
            Raise(unit, GetCaller());
            LookWithAngle(unit, 250);
            FrameTimerWithArg(1, unit, DelayShootToTarget);
            CreatureIdle(self);
            MonsterForceCastSpell(self, 0, GetObjectX(self) + UnitRatioX(other, self, 30.0), GetObjectY(self) + UnitRatioY(other, self, 30.0));
            PushObject(ThrowBlueCrystal(self, other), -1.5, GetObjectX(other), GetObjectY(other));
            delay = 20;
        }
        Enchant(self, EnchantList(30), 0.5);
    }
    if (GetCaller() ^ GetUnit1C(GetTrigger() + 1))
    {
        SetUnit1C(GetTrigger() + 1, GetCaller());
        AggressionLevel(self, 1.0);
    }
    CheckResetSight(GetTrigger(), delay);
}

void HecubahLostEnemy()
{
    int enemy = GetUnit1C(GetTrigger() + 1);

    if (CurrentHealth(enemy))
        Attack(self, enemy);
}

void WhenRunout()
{
    EnchantOff(self, EnchantList(2));
}

void GhostStrike()
{
    int ptr;

    LookAtObject(self, other);
    if (DistanceUnitToUnit(self, other) > 50.0)
        PushObjectTo(self, UnitRatioX(other, self, 13.0), UnitRatioY(other, self, 13.0));
    else
    {
        HitLocation(self, GetObjectX(self), GetObjectY(self));
        ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(self), GetObjectY(self));
        SetOwner(self, ptr);
        Raise(ptr, ToFloat(GetCaller()));
        RemoveGhostHitDelay(ptr);
    }
    Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void RemoveGhostHitDelay(int ptr)
{
    int unit = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));
    int hp = CurrentHealth(unit), mxHp = MaxHealth(unit), pic, func;

    if (hp)
    {
        if (DistanceUnitToUnit(unit, target) > 28.0)
            MoveObject(ptr, GetObjectX(unit) + UnitRatioX(target, unit, 1.0), GetObjectY(unit) + UnitRatioY(target, unit, 1.0));
        else
            MoveObject(ptr, GetObjectX(unit), GetObjectY(unit));
        func = GetUnitDeathFunc(unit);
        Delete(unit);
        pic = SummonPowerGhost(ptr);
        SetCallback(pic, 5, func);
        Damage(pic, 0, mxHp - hp, -1);
        LookAtObject(pic, target);
        MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
        DeleteObjectTimer(CreateObject("ReleasedSoul", 1), 9);
        AudioEvent("DeathRayKill", 1);
    }
    Delete(ptr);
}

void MecaGolemStrike()
{
    int ptr;
    float dist = DistanceUnitToUnit(self, other);

    LookAtObject(self, other);
    if (dist > 39.0)
        PushObjectTo(self, UnitRatioX(other, self, 21.0), UnitRatioY(other, self, 21.0));
    if (!HasEnchant(self, "ENCHANT_BURNING") && dist < 110.0)
    {
        Enchant(self, "ENCHANT_BURNING", 0.3);
        HitLocation(self, GetObjectX(self), GetObjectY(self));
        ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(self), GetObjectY(self));
        SetOwner(self, ptr);
        Raise(ptr, ToFloat(GetCaller()));
        FrameTimerWithArg(6, ptr, RemoveMecaGolemHitDelay);
    }
    Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void RemoveMecaGolemHitDelay(int ptr)
{
    int unit = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));
    int hp = CurrentHealth(unit), mxHp = MaxHealth(unit), pic, func;

    if (hp)
    {
        if (DistanceUnitToUnit(unit, target) > 27.0)
            MoveObject(ptr, GetObjectX(unit) + UnitRatioX(target, unit, 4.0), GetObjectY(unit) + UnitRatioY(target, unit, 4.0));
        else
            MoveObject(ptr, GetObjectX(unit), GetObjectY(unit));
        func = GetUnitDeathFunc(unit);
        Delete(unit);
        pic = SummonPowerMecaGolem(ptr);
        SetCallback(pic, 5, func);
        Damage(pic, 0, mxHp - hp, -1);
        LookAtObject(pic, target);
        HitLocation(pic, GetObjectX(pic), GetObjectY(pic));
        MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
        DeleteObjectTimer(CreateObject("ReleasedSoul", 1), 9);
        AudioEvent("HitStoneBreakable", 1);
    }
    Delete(ptr);
}

void DemonSightEvent()
{
    if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) < 98.0)
	{
		DeleteObjectTimer(CreateObjectAt("MeteorExplode", GetObjectX(other), GetObjectY(other)), 9);
		Damage(other, self, 30, 14);
	}
	if (GetCaller() ^ ToInt(GetObjectZ(GetTrigger() + 1)))
	{
		CreatureFollow(self, other);
		Raise(GetTrigger() + 1, ToFloat(GetCaller()));
	}
	MoveObject(GetTrigger() + 1, GetObjectX(self), GetObjectY(self));
	Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void DemonResetSight()
{
	int target = ToInt(GetObjectZ(GetTrigger() + 1));

	EnchantOff(self, "ENCHANT_BLINDED");
	if (!IsVisibleTo(self, target) || !CurrentHealth(target))
	{
		Raise(GetTrigger() + 1, ToFloat(0));
		CreatureIdle(self);
	}
}

void GWizSightEvent()
{
	LookAtObject(self, other);
	SummonFrog(GetTrigger(), GetCaller());
	if (GetCaller() ^ ToInt(GetObjectZ(GetTrigger() + 1)))
	{
		CreatureFollow(self, other);
		Raise(GetTrigger() + 1, ToFloat(GetCaller()));
	}
	MoveObject(GetTrigger() + 1, GetObjectX(self), GetObjectY(self));
	Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void GWizResetSight()
{
	int target = ToInt(GetObjectZ(GetTrigger() + 1));

	EnchantOff(self, "ENCHANT_BLINDED");
	if (!IsVisibleTo(self, target) || !CurrentHealth(target))
	{
		Raise(GetTrigger() + 1, ToFloat(0));
		CreatureIdle(self);
	}
}

void SummonFrog(int owner, int target)
{
	int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));

	UnitNoCollide(CreateObjectAt("GreenFrog", GetObjectX(unit), GetObjectY(unit)));
	ObjectOff(unit + 1);
	SetOwner(owner, unit);
	Raise(unit, ToFloat(target));
	FrameTimerWithArg(1, unit, FrogFlyingHandler);
}

void FrogFlyingHandler(int ptr)
{
	int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), count = GetDirection(ptr);

	if (CurrentHealth(owner) && CurrentHealth(target) && count < 50)
	{
		if (IsVisibleTo(ptr + 1, target))
		{
			if (Distance(GetObjectX(ptr + 1), GetObjectY(ptr + 1), GetObjectX(target), GetObjectY(target)) > 29.0)
			{
				MoveObject(ptr + 1, GetObjectX(ptr + 1) + UnitRatioX(target, ptr + 1, 11.0), GetObjectY(ptr + 1) + UnitRatioY(target, ptr + 1, 11.0));
				LookAtObject(ptr + 1, target);
				Walk(ptr + 1, GetObjectX(ptr + 1), GetObjectY(ptr + 1));
				LookWithAngle(ptr, count + 1);
			}
			else
			{
				DeleteObjectTimer(CreateObjectAt("GreenPuff", GetObjectX(target), GetObjectY(target)), 9);
				Damage(target, owner, 10, 14);
				ObjectOn(ptr + 1);
				Damage(ptr + 1, 0, MaxHealth(ptr + 1) + 1, -1);
				LookWithAngle(ptr, 100);
			}
		}
		else
			LookWithAngle(ptr, 100);
		FrameTimerWithArg(1, ptr, FrogFlyingHandler);
	}
	else
	{
		Delete(ptr);
		Delete(ptr + 1);
	}
}

void AbsoluteTargetStrike(int owner, int target, float threshold, int func)
{
    int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(target), GetObjectY(target));
    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner)), threshold);

    SetOwner(owner, unit);
    Raise(unit, ToFloat(target));
    FrameTimerWithArg(1, unit, func);
}

void FireSpritEnemyFind()
{
    AbsoluteTargetStrike(GetTrigger(), GetCaller(), DistanceUnitToUnit(self, other) / 42.0, SpitImpShot);
    UnitSetEnchantTime(self, 2, 60);
}

void WizRunAway()
{
	if (HasEnchant(self, "ENCHANT_ANTI_MAGIC"))
	{
		EnchantOff(self, "ENCHANT_ANTI_MAGIC");
	}
}

int ThrowSmallFist(int owner, int target)
{
    int fistTrap = CreateObjectAt("ImaginaryCaster", GetObjectX(target), GetObjectY(target)) + 1;

    Delete(fistTrap - 1);
    CastSpellLocationObject("SPELL_FIST", GetObjectX(owner), GetObjectY(owner), target);
    SetOwner(owner, fistTrap);
    return fistTrap;
}

void SingleMissileShooter(int owner, int target)
{
    int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));

    SetOwner(owner, unit);
    ShotSingleMagicMissile(unit, target);
    DeleteObjectTimer(unit, 60);
}

void ShotSingleMagicMissile(int unit, int target)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit));

    CastSpellObjectLocation("SPELL_MAGIC_MISSILE", unit, GetObjectX(target), GetObjectY(target));
    Delete(ptr);
    Delete(ptr + 2);
    Delete(ptr + 3);
    Delete(ptr + 4);
}

void OpenBossGate()
{
    int key, count;

    count ++;
    ObjectOff(self);
    if (count ^ 6)
    {
        MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
        key = CreateObjectAt("RedOrbKeyOfTheLich", 200.0, 200.0);
        Enchant(key, "ENCHANT_FREEZE", 0.0);
        MoveObject(key, GetObjectX(other), GetObjectY(other));
        Raise(key, 225.0);
        AudioEvent("KeyDrop", 1);
    }
    else
    {
        UnlockDoor(Object("BossGate1"));
        UnlockDoor(Object("BossGate11"));
        FrameTimerWithArg(30, BossCnt, StartBossMonsterSummons);
        UniPrint(other, "보스 방 게이트를 열었다");
        UniPrintToAll("방금 " + PlayerIngameNick(other) + " 님께서 보스 방 출입문의 잠금을 해제시켰습니다");
    }
}

void WallsLineOpen(int x, int y, int count, int dir)
{
    int i;

    for (i = 0 ; i < count ; i ++)
        WallOpen(Wall(x + i, y + (i*dir)));
}

int DungeonMobDeathFunc()
{
    StopScript(DungeonMonster1Death);
}

void DungeonMonster1Death()
{
    int ptr = Dungeon[0] + 1;
    int count = GetDirection(ptr);

    LookWithAngle(ptr, count - 1);
    DungeonUnitDeathHandler();
    if (!GetDirection(ptr))
    {
        WallsLineOpen(49, 129, 7, -1);
        UniPrintToAll("던전1 의 비밀벽이 열립니다");
    }
}

void DungeonMonster2Death()
{
    int ptr = Dungeon[1] + 1;
    int count = GetDirection(ptr);

    LookWithAngle(ptr, count - 1);
    DungeonUnitDeathHandler();
    if (!GetDirection(ptr))
    {
        WallsLineOpen(62, 116, 7, -1);
        UniPrintToAll("던전2 의 비밀벽이 열립니다");
    }
}

void DungeonMonster3Death()
{
    int ptr = Dungeon[2] + 1;
    int count = GetDirection(ptr);

    LookWithAngle(ptr, count - 1);
    Delete(GetTrigger() + 1);
    DungeonUnitDeathHandler();
    if (!GetDirection(ptr))
    {
        WallsLineOpen(77, 101, 7, -1);
        UniPrintToAll("던전3 의 비밀벽이 열립니다");
    }
}

void DungeonMonster4Death()
{
    int ptr = Dungeon[3] + 1;
    int count = GetDirection(ptr);

    LookWithAngle(ptr, count - 1);
    Delete(GetTrigger() + 1);
    DungeonUnitDeathHandler();
    if (!GetDirection(ptr))
    {
        WallsLineOpen(142, 98, 6, 1);
        UniPrintToAll("던전4 의 비밀벽이 열립니다");
    }
}

void DungeonMonster5Death()
{
    int ptr = Dungeon[4] + 1;
    int count = GetDirection(ptr);

    LookWithAngle(ptr, count - 1);
    DungeonUnitDeathHandler();
    if (!GetDirection(ptr))
    {
        WallsLineOpen(151, 107, 6, 1);
        UniPrintToAll("던전5 의 비밀벽이 열립니다");
    }
}

void DungeonMonster6Death()
{
    int ptr = Dungeon[5] + 1;
    int count = GetDirection(ptr);

    LookWithAngle(ptr, count - 1);
    DungeonUnitDeathHandler();
    if (!GetDirection(ptr))
    {
        WallsLineOpen(169, 125, 6, 1);
        UniPrintToAll("던전6 의 비밀벽이 열립니다");
    }
}

void StartDungeon(int idx)
{
    int dungeons[] = {SpawnDungeon1Monsters, SpawnDungeon2Monsters, SpawnDungeon3Monsters, SpawnDungeon4Monsters, SpawnDungeon5Monsters, SpawnDungeon6Monsters};

    Dungeon[idx] = CallFunctionWithArgInt(dungeons[idx], 80);
    UnlockDoor(Object("DunGate" + IntToString(idx + 1)));
    UnlockDoor(Object("DunGate" + IntToString(idx + 1) + "1"));
    UniPrintToAll(IntToString(idx + 1) + " 번째 던전 출입문이 열렸습니다");
}

int SpawnDungeon1Monsters(int count)
{
    int unit = CreateObject("InvisibleLightBlueHigh", 13);
    CreateObject("InvisibleLightBlueHigh", 13);
    LookWithAngle(unit, count);
    LookWithAngle(unit + 1, count);
    Raise(unit, ToFloat(0));
    FrameTimerWithArg(1, unit, SpawnDungeonMonster);
    return unit;
}

int SpawnDungeon2Monsters(int count)
{
    int unit = CreateObject("InvisibleLightBlueHigh", 14);
    CreateObject("InvisibleLightBlueHigh", 14);
    LookWithAngle(unit, count);
    LookWithAngle(unit + 1, count);
    Raise(unit, ToFloat(1));
    FrameTimerWithArg(1, unit, SpawnDungeonMonster);
    return unit;
}

int SpawnDungeon3Monsters(int count)
{
    int unit = CreateObject("InvisibleLightBlueHigh", 15);
    CreateObject("InvisibleLightBlueHigh", 15);
    LookWithAngle(unit, count);
    LookWithAngle(unit + 1, count);
    Raise(unit, ToFloat(2));
    FrameTimerWithArg(1, unit, SpawnDungeonMonster);
    return unit;
}

int SpawnDungeon4Monsters(int count)
{
    int unit = CreateObject("InvisibleLightBlueHigh", 16);
    CreateObject("InvisibleLightBlueHigh", 16);
    LookWithAngle(unit, count);
    LookWithAngle(unit + 1, count);
    Raise(unit, ToFloat(3));
    FrameTimerWithArg(1, unit, SpawnDungeonMonster);
    return unit;
}

int SpawnDungeon5Monsters(int count)
{
    int unit = CreateObject("InvisibleLightBlueHigh", 17);
    CreateObject("InvisibleLightBlueHigh", 17);
    LookWithAngle(unit, count);
    LookWithAngle(unit + 1, count);
    Raise(unit, ToFloat(4));
    FrameTimerWithArg(1, unit, SpawnDungeonMonster);
    return unit;
}

int SpawnDungeon6Monsters(int count)
{
    int unit = CreateObject("InvisibleLightBlueHigh", 18);
    CreateObject("InvisibleLightBlueHigh", 18);
    LookWithAngle(unit, count);
    LookWithAngle(unit + 1, count);
    Raise(unit, ToFloat(5));
    FrameTimerWithArg(1, unit, SpawnDungeonMonster);
    return unit;
}

int DungeonPowerMobFunctions(int type)
{
    int powerMobs[] = {SummonPowerGhost, SummonPowerPurpleGirl, SummonPowerMonster3, 
        SummonGWizard, SummonPowerMonster5, SummonPowerMecaGolem, SummonHecubah, SummonBomber};

    return powerMobs[type];
}

void SpawnDungeonMonster(int ptr)
{
    int count = GetDirection(ptr), type = ToInt(GetObjectZ(ptr)), unit;

    if (count)
    {
        unit = CallFunctionWithArgInt(DungeonPowerMobFunctions(type), ptr);
        SetCallback(unit, 5, DungeonMobDeathFunc() + type);
        LookWithAngle(ptr, count - 1);
        FrameTimerWithArg(1, ptr, SpawnDungeonMonster);
    }
}

void EnableObject(int disUnit)
{
    if (ToInt(GetObjectX(disUnit)))
    {
        if (IsObjectOn(disUnit))
            return;
        else
            ObjectOn(disUnit);
    }
}

void OpenDungenGate()
{
    int key = HasGateKey(OTHER), dun;

    ObjectOff(SELF);
    if (IsObjectOn(key))
    {
        dun = CheckDungeon(OTHER);
        if (dun + 1)
        {
            StartDungeon(dun);
            Delete(key);
        }
    }
    else
    {
        FrameTimerWithArg(70, GetTrigger(), EnableObject);
        UniPrint(OTHER, "[!] 이 던전 게이트를 열려면 리치의 붉은색 열쇠가 필요합니다");
    }
}

int CheckDungeon(int unit)
{
    float temp = 5138.0, dist;
    int i, res = -1;

    for (i = 0 ; i < DunCnt ; i += 1)
    {
        dist = DistanceUnitToUnit(unit, DGate[i]);
        if (dist < temp)
        {
            temp = dist;
            res = i;
        }
    }
    return res;
}

int HasGateKey(int unit)
{
    int inv = GetLastItem(unit);

    while (IsObjectOn(inv))
    {
        if (GetUnitThingID(inv) ^ 2182)
            inv = GetPreviousItem(inv);
        else
            return inv;
    }
    return 0;
}

void InitDungeonGates(int max)
{
    int i;

    for (i = 0 ; i < max ; i += 1)
        DGate[i] = Object("DunGate" + IntToString(i + 1));
}

void BossMonsterDeath()
{
    int count;

    if ((count++) == BossCnt)
    {
        VictoryEvent();
    }
    DungeonUnitDeathHandler();
}

void StartBossMonsterSummons(int count)
{
    int unit = CreateObject("InvisibleLightBlueHigh", 19);

    LookWithAngle(unit, count);
    FrameTimerWithArg(3, unit, SummonBossMonster);
}

void SummonBossMonster(int sUnit)
{
    int count = GetDirection(sUnit);

    if (count)
    {
        SetCallback(SummonHecubah(sUnit), 5, BossMonsterDeath);
        LookWithAngle(sUnit, count - 1);
        FrameTimerWithArg(1, sUnit, SummonBossMonster);
    }
}

void VictoryEvent()
{
    if (!GOver)
    {
        GOver = 1;
        TeleportAllPlayer(48);
        FrameTimer(30, StrMissionClear);
        UniPrintToAll("승리하셨습니다! 모든 보스 몬스터를 물리쳤습니다!");
    }
}

void ImpShotCollide()
{
    int owner = GetOwner(self);

    while (1)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, 0, 40, 1);
            break;
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        Delete(self);
        break;
    }
}

int ImpMissile(int sOwner, int sTarget, float gap)
{
    int mis = CreateObjectAt("RoundChakramInMotion", GetObjectX(sOwner) + UnitRatioX(sTarget, sOwner, gap), GetObjectY(sOwner) + UnitRatioY(sTarget, sOwner, gap));
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(mis, ImpShotCollide);
    SetOwner(sOwner, mis);
    return mis;
}

void SpitImpShot(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), mis;
    float dt = Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(target), GetObjectY(target));
    float vectX = UnitRatioX(target, ptr, dt), vectY = UnitRatioY(target, ptr, dt);

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        mis = ImpMissile(owner, target, 14.0);
        SetOwner(owner, mis);
        UnitSetEnchantTime(mis, 8, 90);
        MoveObject(ptr, GetObjectX(target) + (vectX * GetObjectZ(ptr + 1)), GetObjectY(target) + (vectY * GetObjectZ(ptr + 1)));
        if (IsVisibleTo(ptr, owner))
            PushObject(mis, -35.0, GetObjectX(ptr), GetObjectY(ptr));
        else
            PushObject(mis, -35.0, GetObjectX(target), GetObjectY(target));
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void RhombusPut(int wp, float x_low, float x_high, float y_low, float y_high)
{
    float xf = RandomFloat(y_low, y_high), yf = RandomFloat(0.0, x_high - x_low);

    MoveWaypoint(wp, x_high - y_high + xf - yf, xf + yf);
}

void RhombusUnitTeleport(int unit, float x_low, float x_high, float y_low, float y_high)
{
    float xf = RandomFloat(y_low, y_high), yf = RandomFloat(0.0, x_high - x_low);

    MoveObject(unit, x_high - y_high + xf - yf, xf + yf);
}

void SplashHandler(int owner, int func, float x, float y, float range)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", range, y) + 1, k;

    SetOwner(owner, ptr - 1);
    Raise(ptr - 1, ToFloat(func));
    for (k = 0 ; k < 8 ; k += 1)
    {
        DeleteObjectTimer(CreateObjectAt("WeirdlingBeast", x, y), 1);
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
        if (CurrentHealth(GetOwner(parent)))
        {
            if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) <= GetObjectX(parent))
            {
                Enchant(other, "ENCHANT_VILLAIN", 0.1);
                CallFunction(ToInt(GetObjectZ(parent)));
            }
        }
    }
}

void GreenSparkFx(float x, float y)
{
    int ptr = CreateObjectAt("MonsterGenerator", x, y);

    Damage(ptr, 0, 10, 100);
    Delete(ptr);
}

void UpdateRepairItem(int plrIndex, int item)
{
    int link, temp = GetMemory(0x5c3108), *ptr = UnitToPtr(item);
    int arr[9];

    if (!link)
    {
        arr[0] = 0x50685056; arr[1] = 0xFF005072; arr[2] = 0x708B2414; arr[3] = 0x04C48304; arr[4] = 0x4D87A068; arr[5] = 0x30FF5600; arr[6] = 0x082454FF;
        arr[7] = 0x580CC483; arr[8] = 0x9090C35E;
        link = &arr;
    }
    if (ptr != NULLPTR)
    {
        item = ptr;
        SetMemory(0x5c3108, link);
        Unused1f(&plrIndex);
        SetMemory(0x5c3108, temp);
    }
}

int RepairAll(int unit)
{
    int inv = GetLastItem(unit), count = 0, plrIndex = GetPlayerIndex(unit);

    if (plrIndex < 0)
        return 0;
    while (IsObjectOn(inv))
    {
        if (MaxHealth(inv) ^ CurrentHealth(inv))
        {
            RestoreHealth(inv, MaxHealth(inv) - CurrentHealth(inv));
            UpdateRepairItem(plrIndex, inv);
            count += 1;
        }
        inv = GetPreviousItem(inv);
    }
    return count;
}

int GetUnitDeathFunc(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        return GetMemory(GetMemory(ptr + 0x2ec) + 0x4f4);
    }
    return 0;
}

void InitStampStrings()
{
    StrWarSkill();
    StrSpecialWeapon();
}

void StrWarSkill()
{
	int arr[26], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = LocationX(45), pos_y = LocationY(45);

	arr[0] = 1075905278; arr[1] = 133713856; arr[2] = 67649679; arr[3] = 18874881; arr[4] = 956449312; arr[5] = 2080900128; arr[6] = 151552009; arr[7] = 1343258900; 
	arr[8] = 570434048; arr[9] = 1107595336; arr[10] = 8881216; arr[11] = 337714704; arr[12] = 6344232; arr[13] = 1073750020; arr[14] = 565376; arr[15] = 603922400; 
	arr[16] = 608305184; arr[17] = 4194304; arr[18] = 1081601; arr[19] = 2113929506; arr[20] = 1140589571; arr[21] = 2143618048; arr[22] = 269484049; arr[23] = 268566032; 
	arr[24] = 1077919744; arr[25] = 32640; 
	for (i = 0 ; i < 26 ; i ++)
		count = DrawStrWarSkill(arr[i], name, count);
	TeleportLocation(45, pos_x, pos_y);
}

int DrawStrWarSkill(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 806 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 45);
		if (count % 72 == 71)
            TeleportLocationVector(45, -140.0, 144.0);
		else
            TeleportLocationVector(45, 2.0, -2.0);
		count ++;
	}
	return count;
}

void StrSpecialWeapon()
{
	int arr[32], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(46), pos_y = GetWaypointY(46);

	arr[0] = 271310912; arr[1] = 1338505220; arr[2] = 9375728; arr[3] = 478232578; arr[4] = 37756992; arr[5] = 134513728; arr[6] = 8784464; arr[7] = 1179906; 
	arr[8] = 1084236834; arr[9] = 134697042; arr[10] = 134254600; arr[11] = 2047377697; arr[12] = 1069556802; arr[13] = 2017461376; arr[14] = 173114120; arr[15] = 257; 
	arr[16] = 65570; arr[17] = 2018672640; arr[18] = 134217743; arr[19] = 16924657; arr[20] = 537039872; arr[21] = 69271488; arr[22] = 528400; arr[23] = 33428640; 
	arr[24] = 1075871808; arr[25] = 1341947776; arr[26] = 4271; arr[27] = 33620738; arr[28] = 516; arr[29] = 134250373; arr[30] = 1880098816; arr[31] = 31; 
	
	for (i = 0 ; i < 32 ; i ++)
		count = DrawStrSpecialWeapon(arr[i], name, count);
	TeleportLocation(46, pos_x, pos_y);
}

int DrawStrSpecialWeapon(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 992 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 46);
		if (count % 88 == 87)
            TeleportLocationVector(46, -172.0, 176.0);
		else
            TeleportLocationVector(46, 2.0, -2.0);
		count ++;
	}
	return count;
}

void StrMissionFail()
{
	int arr[17], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(47), pos_y = GetWaypointY(47);

	arr[0] = 272613948; arr[1] = 75782084; arr[2] = 537411713; arr[3] = 1090654737; arr[4] = 606245922; arr[5] = 675873800; arr[6] = 267423498; arr[7] = 1352002; 
	arr[8] = 1073872898; arr[9] = 114; arr[10] = 1889865712; arr[11] = 268697375; arr[12] = 1075856020; arr[13] = 86540160; arr[14] = 16842881; arr[15] = 33428976; 
	arr[16] = 671349764; 
	for (i = 0 ; i < 17 ; i ++)
		count = DrawStrMissionFail(arr[i], name, count);
	MoveWaypoint(47, pos_x, pos_y);
}

int DrawStrMissionFail(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 527 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 47);
		if (count % 48 == 47)
			TeleportLocationVector(47, -92.0, 96.0);
		else
			TeleportLocationVector(47, 2.0, -2.0);
		count ++;
	}
	return count;
}

void StrMissionClear()
{
	int arr[23], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(48), pos_y = GetWaypointY(48);

	arr[0] = 1008796286; arr[1] = 1338015748; arr[2] = 142870536; arr[3] = 537002002; arr[4] = 571482146; arr[5] = 3539016; arr[6] = 144933001; arr[7] = 50725153; 
	arr[8] = 612564516; arr[9] = 1073742968; arr[10] = 554172575; arr[11] = 1072697345; arr[12] = 33554946; arr[13] = 16388; arr[14] = 8259592; arr[15] = 2080440336; 
	arr[16] = 1107566625; arr[17] = 134740992; arr[18] = 135299208; arr[19] = 537919490; arr[20] = 528678432; arr[21] = 4196344; arr[22] = 524414; 
	for (i = 0 ; i < 23 ; i ++)
		count = DrawStrMissionClear(arr[i], name, count);
	MoveWaypoint(48, pos_x, pos_y);
}

int DrawStrMissionClear(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 713 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 48);
		if (count % 64 == 63)
			TeleportLocationVector(48, -124.0, 128.0);
		else
			TeleportLocationVector(48, 2.0, -2.0);
		count ++;
	}
	return count;
}

void StrDungeonBoss()
{
	int arr[18], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(50), pos_y = GetWaypointY(50);

	arr[0] = 1076879998; arr[1] = 1077968960; arr[2] = 135268416; arr[3] = 1209012256; arr[4] = 67369987; arr[5] = 4523908; arr[6] = 8552577; arr[7] = 270537281; 
	arr[8] = 69214480; arr[9] = 1099429893; arr[10] = 8424441; arr[11] = 32; arr[12] = 67117058; arr[13] = 134250496; arr[14] = 65536; arr[15] = 131104; 
	arr[16] = 2088757246; arr[17] = 32647; 
	for (i = 0 ; i < 18 ; i ++)
		count = DrawStrDungeonBoss(arr[i], name, count);
	MoveWaypoint(50, pos_x, pos_y);
}

int DrawStrDungeonBoss(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 558 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 50);
		if (count % 52 == 51)
			TeleportLocationVector(50, -104.0, -100.0);
		else
			TeleportLocationVector(50, 2.0, 2.0);
		count ++;
	}
	return count;
}

int BackRow()
{
    int ptr;

    if (!ptr)
    {
        ptr = CreateObject("IronBlock", 61);
        Frozen(CreateObject("IronBlock", 62), 1);
        Frozen(CreateObject("IronBlock", 63), 1);
        Frozen(CreateObject("IronBlock", 64), 1);
        Frozen(CreateObject("IronBlock", 65), 1);
        Frozen(CreateObject("IronBlock", 66), 1);
        Frozen(ptr, 1);
    }
    return ptr;
}

void MovingBackEntranceRow(int ptr)
{
    int count = GetDirection(ptr);

    if (count < 92)
    {
        MoveObject(ptr, GetObjectX(ptr) + 2.0, GetObjectY(ptr) - 2.0);
        MoveObject(ptr + 1, GetObjectX(ptr + 1) + 2.0, GetObjectY(ptr + 1) - 2.0);
        MoveObject(ptr + 2, GetObjectX(ptr + 2) + 2.0, GetObjectY(ptr + 2) - 2.0);
        MoveObject(ptr + 3, GetObjectX(ptr + 3) - 2.0, GetObjectY(ptr + 3) + 2.0);
        MoveObject(ptr + 4, GetObjectX(ptr + 4) - 2.0, GetObjectY(ptr + 4) + 2.0);
        MoveObject(ptr + 5, GetObjectX(ptr + 5) - 2.0, GetObjectY(ptr + 5) + 2.0);
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, MovingBackEntranceRow);
    }
    else
    {
        WallOpen(Wall(145, 185));
        WallOpen(Wall(144, 186));
        WallOpen(Wall(137, 193));
        WallOpen(Wall(136, 194));
    }
}

void OpenCastleBackEntrance()
{
    ObjectOff(self);
    MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
    AudioEvent("CreatureCageAppears", 1);
    AudioEvent("BoulderMove", 1);
    AudioEvent("ChainPull", 1);
    ObjectOn(Object("CastleGear1"));
    ObjectOn(Object("CastleGear2"));
    ObjectOn(Object("CastleGear3"));
    WallOpen(Wall(130, 202));
    WallOpen(Wall(132, 204));
    WallOpen(Wall(153, 179));
    FrameTimerWithArg(12, BackRow(), MovingBackEntranceRow);
    Enchant(CreateObjectAt("RedOrbKeyOfTheLich", GetObjectX(other), GetObjectY(other)), "ENCHANT_FREEZE", 0.0);
    UniPrint(other, "후문 출입구가 열립니다");
}

void InitMapSigns()
{
    RegistSignMessage(Object("UniSign1"), "베이스 캠프 마켓: 없는 것 빼고는 여기에 다 있다");
    RegistSignMessage(Object("UniSign2"), "슬리피우드 던전 입구: 무기와 갑옷 빡세게 챙길 것");
    RegistSignMessage(Object("UniSign3"), "아테네 신전 후문 입구: 원래는 여기가 정문이었음");
    RegistSignMessage(Object("UniSign4"), "세상에서 가장 강한 몬스터가 이 방안에 서식하고 있음");
    RegistSignMessage(Object("UniSign5"), "비밀 휴게실: 호바스를 클릭하면 힐링 버프를 시전해 줍니다");
    RegistSignMessage(Object("UniSign6"), "마법 무기마켓: 각 무기에 대한 설명을 보려면 클릭, 구입하려면 더블클릭하세요");
    RegistSignMessage(Object("UniSign7"), "전사 신규능력을 구입할 수 있습니다");
    RegistSignMessage(Object("UniSign8"), "이곳은 아테네 신전 입니다. 강력했던 고대 괴물들이 이곳에서 여전히 살아 숨쉬고 있습니다");
    RegistSignMessage(Object("UniSign9"), "-신전2층- 특수한 아이템을 판매하고 있으니 지갑 빵빵한 사람이라면 언제든 환영!");
    RegistSignMessage(Object("UniSign10"), "가운데 NPC 는 3천 골드만으로 모든 아이템을 수리해줍니다");
    RegistSignMessage(Object("UniSign11"), "-고대괴물- 투명인간: 투명인간 손 끝에 한번만 스쳐도 당신은 죽음이다");
    RegistSignMessage(Object("UniSign12"), "-고대괴물- 키러리언: 이동속도가 매우 빠르며 근접한 거리에서 화염을 내뿜는다");
    RegistSignMessage(Object("UniSign13"), "타락한 여신: 빠른 속도로 무수히 많은 개구리를 토해낸다");
    RegistSignMessage(Object("UniSign14"), "주인없는 별장: 이제 이곳은 먼지만 가득하다...");
    RegistSignMessage(Object("UniSign15"), "스톤골렘: 이 녀석을 깨우는 순간 공포게임 된다");
    RegistSignMessage(Object("UniSign16"), "불닭 볶음면: 불닭의 매운맛을 느껴봐라");
    RegistSignMessage(Object("UniSign17"), "불의 영혼: 용광로에 빠져죽은 자가 불의 괴물로 부활했다는 소문이 있다");
}

int HorrendousBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1920102216; arr[1] = 1868852837; arr[2] = 29557; arr[17] = 400; arr[18] = 400; 
		arr[19] = 95; arr[23] = 32768; arr[24] = 1065437102; arr[25] = 1; arr[26] = 9; 
		arr[27] = 5; arr[28] = 1137180672; arr[31] = 17; arr[54] = 4; arr[59] = 5542784; 
		arr[60] = 1386; arr[61] = 46907648; 
        link = &arr;
	}
	return link;
}

void HorrendousSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077306982);
		SetMemory(ptr + 0x224, 1077306982);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 400);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 400);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HorrendousBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 0);
	}
}

void HorrendousHarpoonCollide()
{
    int owner = GetOwner(self);

    while (IsObjectOn(self))
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 12, 1);
            Effect("DAMAGE_POOF", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        Delete(self);
        break;
    }
}

int HorrendousHarpoon(int sUnit, int sTarget)
{
    int mis = CreateObjectAt("HarpoonBolt", GetObjectX(sUnit) + UnitRatioX(sTarget, sUnit, 16.0), GetObjectY(sUnit) + UnitRatioY(sTarget, sUnit, 16.0));
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(mis, HorrendousHarpoonCollide);
    SetOwner(sUnit, mis);
    LookAtObject(mis, sTarget);
    return mis;
}

void GuardianHitShuriken(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), mis;
    float dt = Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(target), GetObjectY(target));
    float vectX = UnitRatioX(target, ptr, dt), vectY = UnitRatioY(target, ptr, dt);
    float thresHold;

    if (CurrentHealth(owner) && CurrentHealth(target) && IsObjectOn(owner))
    {
        mis = HorrendousHarpoon(owner, target);
        thresHold = DistanceUnitToUnit(mis, target) / GetObjectZ(ptr + 1);
        MoveObject(ptr, GetObjectX(target) + UnitRatioX(target, ptr, dt * thresHold), GetObjectY(target) + UnitRatioY(target, ptr, dt * thresHold));
        if (IsVisibleTo(ptr, owner))
            PushObject(mis, -42.0, GetObjectX(ptr), GetObjectY(ptr));
        else
            PushObject(mis, -42.0, GetObjectX(target), GetObjectY(target));
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void HorrendousMissileAttack()
{
    AbsoluteTargetStrike(GetTrigger(), GetCaller(), 72.0, GuardianHitShuriken);
}

int SummonHorrendous(float xProfile, float yProfile)
{
    int unit = CreateObjectAt("Horrendous", xProfile, yProfile);

    HorrendousSubProcess(unit);
    RegistUnitStrikeHook(unit);
    SetCallback(unit, 5, DungeonUnitDeathHandler);
    SetCallback(unit, 7, FieldMonsterHurt);
    return unit;
}

void RespectThreeHorrendous(int sUnit)
{
    int enemy = ToInt(GetObjectZ(sUnit)), cre;

    if (IsObjectOn(sUnit))
    {
        if (!GetDirection(sUnit))
        {
            cre = SummonHorrendous(GetObjectX(sUnit), GetObjectY(sUnit));
            LookAtObject(cre, enemy);
            Raise(sUnit + 1, enemy);
            FrameTimerWithArg(1, sUnit + 1, RespectThreeHorrendous);
            DeleteObjectTimer(CreateObjectAt("OldSmoke", GetObjectX(cre), GetObjectY(cre)), 15);
            UniChatMessage(cre, "너의 그 자존심! 내가 박살내주지", 150);
        }
        Delete(sUnit);
    }
}

void RespectSixHorrendous()
{
    ObjectOff(self);

    FrameTimerWithArg(5, CreateObject("InvisibleLightBlueLow", 69), RespectThreeHorrendous);
    Raise(CreateObject("InvisibleLightBlueLow", 70) - 1, GetCaller());
    CreateObject("InvisibleLightBlueLow", 71);
    LookWithAngle(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(self), GetObjectY(self)), 1);
    FrameTimerWithArg(5, CreateObject("InvisibleLightBlueLow", 72), RespectThreeHorrendous);
    Raise(CreateObject("InvisibleLightBlueLow", 73) - 1, GetCaller());
    CreateObject("InvisibleLightBlueLow", 74);
    LookWithAngle(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(self), GetObjectY(self)), 1);
}

void UnitVisibleSplashA()
{
    int parent = GetOwner(self);
    int spIdx = ToInt(GetObjectZ(parent + 1));

    if (CurrentHealth(GetOwner(parent)))
    {
        if (GetUnit1C(other) ^ spIdx)
        {
            if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) <= GetObjectX(parent))
            {
                Damage(other, GetOwner(parent), ToInt(GetObjectZ(parent)), 14);
                SetUnit1C(other, spIdx);
            }
        }
    }
}

void SplashDamageAt(int owner, int dam, float x, float y, float range)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", range, y) + 2, k, SplashIdx;

    SplashIdx ++;
    Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(ptr), GetObjectY(ptr)), SplashIdx);
    SetOwner(owner, ptr - 2);
    Raise(ptr - 2, ToFloat(dam));
    for (k = 0 ; k < 4 ; k ++)
    {
        DeleteObjectTimer(CreateObjectAt("WeirdlingBeast", x, y), 1);
        UnitNoCollide(ptr + k);
        LookWithAngle(ptr + k, k * 64);
        SetOwner(ptr - 2, ptr + k);
        SetCallback(ptr + k, 3, UnitVisibleSplashA);
    }
    DeleteObjectTimer(ptr - 1, 2);
    DeleteObjectTimer(ptr - 2, 2);
}

void FONCollide()
{
    int owner = GetOwner(self);

    while (1)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 255, 1);
            GreenSparkFx(GetObjectX(self), GetObjectY(self));
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        Delete(self);
        break;
    }
}

int ForceOfNature(int sUnit, float gap)
{
    int mis = CreateObjectAt("DeathBall", GetObjectX(sUnit) + UnitAngleCos(sUnit, gap), GetObjectY(sUnit) + UnitAngleSin(sUnit, gap));
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(mis, FONCollide);
    SetOwner(sUnit, mis);
    return mis;
}

void FONStaffClassUse()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(self);

    if (MathAbs(cFps - cTime) < 10)
        return;
    if (CurrentHealth(other))
    {
        SetUnit1C(self, cFps);
        PushObject(ForceOfNature(other, 16.0), 18.0, GetObjectX(other), GetObjectY(other));
        PlaySoundAround(other, 560);
    }
}

int FallingMeteor(float sX, float sY, int sDamage, float sSpeed)
{
    int mUnit = CreateObjectAt("Meteor", sX, sY);
    int ptr = GetMemory(0x750710);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec), sDamage);
        SetMemory(ptr + 0x14, GetMemory(ptr + 0x14) | 0x20);
        Raise(mUnit, 255.0);
        SetMemory(ptr + 0x6c, ToInt(sSpeed));
    }
    return mUnit;
}

void MeteorWandClassStrike(int sGlow)
{
    int owner = GetOwner(sGlow);

    if (CurrentHealth(owner))
    {
        if (IsVisibleTo(owner, sGlow))
        {
            SetOwner(owner, FallingMeteor(GetObjectX(sGlow), GetObjectY(sGlow), 220, -8.0));
            PlaySoundAround(sGlow, 85);
        }
        else
            UniPrint(owner, "마법 시전을 실패하였습니다. 현재의 마우스 포인터 위치는 캐릭터가 볼 수 없는 지역입니다");
    }
    Delete(sGlow);
}

void MeteorWandClassUse()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(self), glow;

    if (MathAbs(cFps - cTime) < 25)
    {
        UniPrint(other, "메테오 완드 재사용 대기중 입니다");
        return;
    }
    if (CurrentHealth(other))
    {
        SetUnit1C(self, cFps);
        glow = CreateObjectAt("Moonglow", GetObjectX(other), GetObjectY(other));
        SetOwner(other, glow);
        FrameTimerWithArg(1, glow, MeteorWandClassStrike);
    }
}

void MagicMissileCollide()
{
    int owner = GetOwner(self);

    while (1)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 150, 9);
            GreenSparkFx(GetObjectX(self), GetObjectY(self));
            break;
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        Delete(self);
        break;
    }
}

int MagicMissileCreate(int sUnit, float gap)
{
    int mis = CreateObjectAt("Magic", GetObjectX(sUnit) + UnitAngleCos(sUnit, gap), GetObjectY(sUnit) + UnitAngleSin(sUnit, gap));
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(mis, MagicMissileCollide);
    SetOwner(sUnit, mis);
    return mis;
}

void MagicMissileWandClassUse()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(self);

    if (MathAbs(cFps - cTime) < 30)
        return;
    if (CurrentHealth(other))
    {
        SetUnit1C(self, cFps);
        PushObject(MagicMissileCreate(other, 16.0), 24.0, GetObjectX(other), GetObjectY(other));
        PlaySoundAround(other, 480);
    }
}

int ZombieBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1651339098; arr[1] = 25961; arr[17] = 75; arr[19] = 150; arr[23] = 32768; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 5; arr[27] = 5; arr[28] = 1106247680; 
		arr[31] = 10; arr[59] = 5542784; arr[60] = 1360; arr[61] = 46895440;
		link = GetScrDataField(ZombieBinTable);
	}
	return link;
}

void ZombieSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1083179008);
		SetMemory(ptr + 0x224, 1083179008);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 75);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 75);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, ZombieBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 0);
	}
}

void RunningZombieLifeTime(int sUnit)
{
    int durate = GetDirection(sUnit), owner;

    while (IsObjectOn(sUnit))
    {
        if (MaxHealth(sUnit - 1))
        {
            if (durate && CurrentHealth(sUnit - 1))
            {
                LookWithAngle(sUnit, durate - 1);
                FrameTimerWithArg(1, sUnit, RunningZombieLifeTime);
                break;
            }
            owner = GetOwner(sUnit - 1);
            MoveObject(sUnit, GetObjectX(sUnit - 1), GetObjectY(sUnit - 1));
            Damage(sUnit - 1, 0, CurrentHealth(sUnit - 1) + 1, 14);
            DeleteObjectTimer(CreateObjectAt("MediumFlame", GetObjectX(sUnit - 1), GetObjectY(sUnit - 1)), 12);
            DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(sUnit - 1), GetObjectY(sUnit - 1)), 12);
            Effect("SPARK_EXPLOSION", GetObjectX(sUnit - 1), GetObjectY(sUnit - 1), 0.0, 0.0);
            if (CurrentHealth(owner))
                SplashDamageAt(owner, 98, GetObjectX(sUnit), GetObjectY(sUnit), 84.0);
        }
        Delete(sUnit);
        break;
    }
}

void ZombieAttackReportComplete()
{
    Damage(other, self, 100, 5);
    if (GetDirection(GetTrigger() + 1))
        LookWithAngle(GetTrigger() + 1, 0);
}

int RunningZombieSpawn(int owner, float gap)
{
    int zomb = CreateObjectAt("Zombie", GetObjectX(owner) + UnitAngleCos(owner, gap), GetObjectY(owner) + UnitAngleSin(owner, gap));
    LookWithAngle(CreateObjectAt("ImaginaryCaster", GetObjectX(zomb), GetObjectY(zomb)), 120);

    SetOwner(owner, zomb);
    FrameTimerWithArg(1, zomb + 1, RunningZombieLifeTime);
    ZombieSubProcess(zomb);
    RegistUnitStrikeHook(zomb);
    LookWithAngle(zomb, GetDirection(owner));
    return zomb;
}

void RunningZombieSummonWandClassUse()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(self), zomb;

    if (MathAbs(cFps - cTime) < 40)
    {
        UniPrint(other, "쿨다운 입니다... 잠시만 기다려 주세요");
        return;
    }
    if (CurrentHealth(other))
    {
        SetUnit1C(self, cFps);
        zomb = RunningZombieSpawn(other, 23.0);
        GreenLightningFx(FloatToInt(GetObjectX(other)), FloatToInt(GetObjectY(other)), FloatToInt(GetObjectX(zomb)), FloatToInt(GetObjectY(zomb)), 24);
        CreatureGuard(zomb, GetObjectX(zomb), GetObjectY(zomb), GetObjectX(zomb) + UnitAngleCos(zomb, 128.0), GetObjectY(zomb) + UnitAngleSin(zomb, 128.0), 600.0);
        PlaySoundAround(other, 591);
    }
}

void ChainLightningProc(int cur)
{
    int unit = GetUnit1C(cur), owner = GetOwner(cur + 1), next;

    if (IsObjectOn(cur))
    {
        next = GetNextNode(cur);
        if (IsObjectOn(next))
            GreenLightningFx(FloatToInt(GetObjectX(cur)), FloatToInt(GetObjectY(cur)), FloatToInt(GetObjectX(next)), FloatToInt(GetObjectY(next)), 24);
            //Effect("SENTRY_RAY", GetObjectX(cur), GetObjectY(cur), GetObjectX(next), GetObjectY(next));
            //GreenLightningFx(FloatToInt(GetObjectX(cur)), FloatToInt(GetObjectY(cur)), FloatToInt(GetObjectX(next)), FloatToInt(GetObjectY(next)), 20);
        if (CurrentHealth(unit) && CurrentHealth(owner))
            Damage(unit, owner, 80, 9);
        //FrameTimerWithArg(1, next, ChainLightningProc);
        Delete(cur);
        Delete(cur + 1);
        ChainLightningProc(next);
    }
}

void StartChain(int headNode)
{
    FrameTimerWithArg(1, GetNextNode(headNode), ChainLightningProc);
    Delete(headNode);
}

void MultiAutoRayInSight()
{
    AddNewNode(GetTrigger() + 1, GetCaller());
    SetOwner(GetOwner(self), CreateObjectAt("InvisibleLightBlueLow", GetObjectX(other), GetObjectY(other)));
}

void DeathRayCasting(int sUnit)
{
    int owner = GetOwner(sUnit + 1);
    int sightMonster = CreateObjectAt("WeirdlingBeast", GetObjectX(sUnit), GetObjectY(sUnit));

    AddNewNode(CreateObjectAt("ImaginaryCaster", GetObjectX(sightMonster), GetObjectY(sightMonster)), sightMonster + 1);
    SetOwner(owner, CreateObjectAt("ImaginaryCaster", GetObjectX(sightMonster + 1), GetObjectY(sightMonster + 1)));
    SetOwner(owner, sightMonster);
    UnitNoCollide(sightMonster);
    SetCallback(sightMonster, 3, MultiAutoRayInSight);
    LookWithAngle(sightMonster, GetDirection(sUnit));
    DeleteObjectTimer(sightMonster, 1);
    FrameTimerWithArg(1, sightMonster + 1, StartChain);

    Delete(sUnit);
}

void DeathRayWandClassUse()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(self), caster;

    if (MathAbs(cFps - cTime) < 68)
    {
        UniPrint(other, "쿨다운 입니다... 잠시만 기다려 주세요");
        return;
    }
    if (CurrentHealth(other))
    {
        SetUnit1C(self, cFps);
        caster = CreateObjectAt("ImaginaryCaster", GetObjectX(other) + UnitAngleCos(other, 21.0), GetObjectY(other) + UnitAngleSin(other, 21.0));
        UnitSetEnchantTime(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(caster), GetObjectY(caster)), 13, 60);
        SetOwner(other, caster + 1);
        DeleteObjectTimer(caster + 1, 48);
        LookWithAngle(caster, GetDirection(other));
        PlaySoundAround(other, 44);
        FrameTimerWithArg(3, caster, DeathRayCasting);
    }
}

int ShopClassMagicStaffShopData(int index)
{
    int pay[5] = {51000, 55000, 48000, 65000, 60000};

    return pay[index];
}

void MagicStaffClassPick()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(self);

    if (MathAbs(cFps - cTime) < 10)
        return;
    if (CurrentHealth(other))
    {
        SetUnit1C(self, cFps);
        AbsolutelyWeaponPickupAndEquip(OTHER, SELF);
        UniPrint(other, "무기를 바꾼 상태에서 이 마법 지팡이를 장착하려면 버렸다가 다시 주워야 합니다");
    }
}

void MagicStaffPreProc(int staff, int useFunc)
{
    SetUnitCallbackOnPickup(staff, MagicStaffClassPick);
    SetUnitCallbackOnUseItem(staff, useFunc);
}

int WandCreateFunctionPtr()
{
    StopScript(FONStaffClassCreate);
}

int FONStaffClassCreate(int owner)
{
    int stf = CreateObjectAt("InfinitePainWand", GetObjectX(owner), GetObjectY(owner));

    MagicStaffPreProc(stf, FONStaffClassUse);
    return stf;
}

int MeteorWandClassCreate(int owner)
{
    int wnd = CreateObjectAt("FireStormWand", GetObjectX(owner), GetObjectY(owner));

    MagicStaffPreProc(wnd, MeteorWandClassUse);
    return wnd;
}

int MagicMissileWandClassCreate(int owner)
{
    int stf = CreateObjectAt("SulphorousFlareWand", GetObjectX(owner), GetObjectY(owner));

    MagicStaffPreProc(stf, MagicMissileWandClassUse);
    return stf;
}

int RunningZombieSumStaffCreate(int owner)
{
    int stf = CreateObjectAt("ForceWand", GetObjectX(owner), GetObjectY(owner));

    MagicStaffPreProc(stf, RunningZombieSummonWandClassUse);
    return stf;
}

int AutoTargetRayStaffCreate(int owner)
{
    int stf = CreateObjectAt("DeathRayWand", GetObjectX(owner), GetObjectY(owner));

    MagicStaffPreProc(stf, DeathRayWandClassUse);
    return stf;
}

void ShopClassMagicStaffDescr()
{
    int curIndex = GetDirection(SELF);
    string descMessage[] = {
        "포스오브네이쳐 지팡이",
        "메테오 완드",
        "매직 미사일 완드",
        "러닝 좀비서먼 완드",
        "오토 타겟팅 데스레이 지팡이"
    };

    //CancelDialog(self);
    UniPrint(OTHER, descMessage[curIndex] + " 을 구입하시겠어요? 그것의 가격은 " + IntToString(ShopClassMagicStaffShopData(curIndex)) + "골드 입니다");
    UniPrint(OTHER, "구입하려면 '예' 를 누르시고 다른 아이템을 보시려면 '아니오' 를 누르세요. 거래를 취소하려면 '떠나기' 를 누르세요");
    Raise(GetTrigger() + 1, &descMessage);
    TellStoryUnitName("AA", "thing.db:IdentifyDescription", descMessage[curIndex]);
}

void ShopClassMagicStaffTrade()
{
    int dlgRes = GetAnswer(SELF), curIndex = GetDirection(SELF), staff;
    string *staffName = ToStr(ToInt(GetObjectZ(GetTrigger() + 1)));

    if (dlgRes == 1)
    {
        if (GetGold(OTHER) >= ShopClassMagicStaffShopData(curIndex))
        {
            ChangeGold(OTHER, -ShopClassMagicStaffShopData(curIndex));
            PlaySoundAround(OTHER, 308);
            staff = CallFunctionWithArgInt(WandCreateFunctionPtr() + curIndex, OTHER);
            SetOwner(OTHER, staff);
            FrameTimerWithArg(1, staff, DelayForcePickItemToOwner);
            UniPrint(OTHER, staffName[curIndex] + "구입 거래가 완료되었습니다. " + staffName[curIndex] + "이 캐릭터에게 장착되었습니다");
            UniPrintToAll(PlayerIngameNick(OTHER) + " 님께서 " + staffName[curIndex] + "을 구입하셨습니다");
        }
        else
            UniPrint(OTHER, "거래가 취소되었습니다. 잔액이 부족합니다 (금화" + IntToString(ShopClassMagicStaffShopData(curIndex) - GetGold(OTHER)) + " 이 더 필요함)");
    }
    else if (dlgRes == 2)
    {
        UniPrint(OTHER, "'아니오' 를 누르셨습니다. 다음 판매 품목을 보여드립니다");
        LookWithAngle(SELF, (curIndex + 1) % 5);
        ShopClassMagicStaffDescr();
    }
}

int ShopClassMagicalStaffCreate(int location)
{
    int shop = DummyUnitCreate("Horrendous", LocationX(location), LocationY(location));

    LookWithAngle(CreateObjectAt("ImaginaryCaster", GetObjectX(shop), GetObjectY(shop)) - 1, 0);
    SetDialog(shop, "YESNO", ShopClassMagicStaffDescr, ShopClassMagicStaffTrade);
    return shop;
}

void ShopClassInit(int location)
{
    ShopClassMagicStaffShopData(0);
    ShopClassMagicalStaffCreate(location);
}

void SlideSecretCase(int sUnit)
{
    int durate = GetDirection(sUnit), wallCoor;

    if (durate)
    {
        MoveObject(sUnit, GetObjectX(sUnit) + 1.0, GetObjectY(sUnit) - 1.0);
        LookWithAngle(sUnit, durate - 1);
        FrameTimerWithArg(1, sUnit, SlideSecretCase);
    }
    else
    {
        wallCoor = GetWallCoorAtObjectPos(sUnit + 1);
        WallOpen(Wall(wallCoor & 0xffff, wallCoor >> 0x10));
        Delete(sUnit + 1);
        FrameTimer(1, InitCandleRoom);
    }
}

void MikyDeadTouched()
{
    int ptr;

    if (CurrentHealth(other) && MaxHealth(self))
    {
        ptr = UnitToPtr(other);
        if (ptr)
        {
            if (GetMemory(ptr + 8) & 0x04)
            {
                FrameTimerWithArg(3, GetTrigger() + 1, SlideSecretCase);
                Delete(self);
                UniPrint(other, "뒤쪽에서 뭔가가 움직이고 있습니다");
            }
        }
    }
}

void RemoveSecretEntranceWalls(int sUnit)
{
    int i;

    for (i = 0 ; i < 4 ; i ++)
        RemoveWallAtObjectPos(sUnit + i);
}

void HiddenCandleTouch()
{
    if (IsObjectOn(self) && CurrentHealth(other))
    {
        Frozen(CreateObjectAt("DunMirScaleTorch1", GetObjectX(self), GetObjectY(self)), 1);
        UniChatMessage(other, "근처에서 비밀의 벽이 열렸습니다", 150);
        FrameTimerWithArg(3, GetTrigger() + 1, RemoveSecretEntranceWalls);
        Delete(self);
    }
}

int HiddenCandle(int location)
{
    int candle = CreateObject("DunMirScaleTorch1", location);

    SetUnitCallbackOnCollide(candle, HiddenCandleTouch);
    Frozen(candle, 1);
    return candle;
}

void RemoveSecretExitWalls(int sUnit)
{
    int i;

    for (i = 0 ; i < 5 ; i ++)
        RemoveWallAtObjectPos(sUnit + i);
}

void PressExitSecretZoneBeacon()
{
    if (CurrentHealth(other) && MaxHealth(self))
    {
        if (IsPlayerUnit(other))
        {
            FrameTimerWithArg(3, GetTrigger() + 1, RemoveSecretExitWalls);
            UniPrint(other, "전방에 있는 비밀벽이 열립니다");
            Delete(self);
        }
    }
}

void InitSecretExitWalls(int location)
{
    int beacon = CreateObject("CarnivorousPlant", location), i;

    for (i = 0 ; i < 5 ; i ++)
        CreateObject("InvisibleLightBlueLow", location + 1 + i);
    Damage(beacon, 0, CurrentHealth(beacon) + 1, -1);
    SetCallback(beacon, 9, PressExitSecretZoneBeacon);
}

void InitCandleRoom()
{
    int unit;
    
    if (!unit)
    {
        unit = HiddenCandle(77);
        CreateObject("InvisibleLightBlueLow", 78);
        CreateObject("InvisibleLightBlueLow", 79);
        CreateObject("InvisibleLightBlueLow", 80);
        CreateObject("InvisibleLightBlueLow", 81);
        FrameTimerWithArg(1, 82, InitSecretExitWalls);
    }
}

void InitUpLevelSecretZone()
{
    int dam = CreateObject("Rat", 75);

    Frozen(CreateObject("MovableBookcase1", 76), 1);
    SetCallback(CreateObject("InvisibleLightBlueLow", 76) - 2, 9, MikyDeadTouched);
    LookWithAngle(dam + 1, 48);
    Damage(dam, 0, 100, 14);
}

void RemoveEntranceFence(int sUnit)
{
    int i, count = GetDirection(sUnit);

    for (i = 0 ; i < count ; i ++)
        RemoveWallAtObjectPos(sUnit + i);
}

void RemoveDungeonEntranceWalls()
{
    int unit = CreateObject("ImaginaryCaster", 88), i;

    ObjectOff(self);
    FrameTimerWithArg(3, unit, RemoveEntranceFence);
    LookWithAngle(unit, 6);
    for (i = 0 ; i < 5 ; i ++)
        CreateObjectAt("ImaginaryCaster", GetObjectX(unit + i) - 23.0, GetObjectY(unit + i) + 23.0);
    
    UniPrint(other, "던전 입구 앞 철조망을 걷어내었습니다. 행운을 빕니다 후보생!");
}

int ImportDataSetWordNumber()
{
    StopScript(ImportDataSetWord);
}

int ImportDataSetWord()
{
    int arr[7], link;

    if (!link)
    {
        arr[0] = 0x50575156; arr[1] = 0xDB6247E8; arr[2] = 0x04788BFF; arr[3] = 0x8B08488B; arr[4] = 0xAB66F300; arr[5] = 0x5E595F58; arr[6] = 0x909090C3;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * ImportDataSetWordNumber() + 0x1c));
        FixCallOpcode(link + 4, 0x507250);
    }
    return link;
}

void MemSetWordPtr(int wordValue, int destPtr, int len)
{
    int temp = GetMemory(0x5c3108), argPtr;

    SetMemory(0x5c3108, ImportDataSetWord());
    argPtr = GetMemory(GetMemory(0x75ae28) + (0x30 * (ImportDataSetWordNumber() + 1) + 0x1c));
    Unused1f(argPtr);
    SetMemory(0x5c3108, temp);
}

int Get16BitColor(int rgb)
{
    int red = rgb & 0xff, grn = (rgb >> 8) & 0xff, blu = (rgb >> 0x10) & 0xff;
    //(red << 11) + (green << 5) + blue

    return ((red << 11) + (grn << 5) + blu) & 0xffff;
}

int GetTileImgData(int tilePtr, int tileVar)
{
    int ptr = GetMemory(GetMemory(tilePtr + 32) + (tileVar * 4));
    int offset = GetMemory(ptr);
    int section = GetMemory(ptr + 8);

    return (section << 0x10) | offset;
}

int TileSetFillSolidColor(int color16, int tilePtr, int tileVar)
{
    int imgSect = GetMemory(0x694860), imgPtr, section = GetTileImgData(tilePtr, tileVar), offset;

    if (imgSect)
    {
        offset = section & 0xffff;
        section = section >> 0x10;
        imgPtr = GetMemory(imgSect + (36 * section));
        if (imgPtr)
        {
            imgPtr += offset;
            MemSetWordPtr(color16, imgPtr, 1058); //2116 / 2
            return 1;
        }
    }
    return 0;
}

void FillTransparentFloorTile()
{
    TileSetFillSolidColor(0xf800, 0x864d5c, 0); //Red
    TileSetFillSolidColor(0x2f4f, 0x864d5c, 1); //cyan
    TileSetFillSolidColor(0xffff, 0x864d5c, 2); //white
    TileSetFillSolidColor(0xff00, 0x864d5c, 3); //blue
    TileSetFillSolidColor(0xf601, 0x864d5c, 4); //orange
    TileSetFillSolidColor(0xf00f, 0x864d5c, 5); //pink
    TileSetFillSolidColor(0xf00, 0x864d5c, 6); //green
}

void DungeonNameLeft() {}
void DungeonNameRight() {}

void RedPotionImage() {}
void PoisonPotionImage() {}

void BossHereImage() {}
void VampPotionImage() {}
void YellowPotionImage() {}
void MagicPotionImage() {}
void DungeonPicketImage() {}
void ProtossNexusImage() {}

void PlayerClassCommonWhenEntry()
{
    int drawF = ImageResourceDrawFunctionFix(0x7510c4);

    ApplyImageResourceFix(636, GetScrCodeField(RedPotionImage) + 4, drawF);         //RedPotion
    ApplyImageResourceFix(2682, GetScrCodeField(VampPotionImage) + 4, drawF);       //VampirismPotion
    ApplyImageResourceFix(631, GetScrCodeField(PoisonPotionImage) + 4, drawF);      //CurePoisonPotion
    ApplyImageResourceFix(2686, GetScrCodeField(MagicPotionImage) + 4, drawF);      //InvulnerabilityPotion
    ApplyImageResourceFix(639, GetScrCodeField(YellowPotionImage) + 4, drawF);      //YellowPotion
    ApplyImageResourceFix(544, GetScrCodeField(DungeonPicketImage) + 4, drawF);     //MovableSign1
    ApplyImageResourceFix(365, GetScrCodeField(ProtossNexusImage) + 4, drawF);      //MovableStatue2a
    ApplyImageResourceFix(472, GetScrCodeField(DungeonNameLeft) + 4, drawF);        //MovableStatueVictory1E
    ApplyImageResourceFix(470, GetScrCodeField(DungeonNameRight) + 4, drawF);       //MovableStatueVictory1N
    ApplyImageResourceFix(471, GetScrCodeField(BossHereImage) + 4, drawF);          //MovableStatueVictory1NE
    ApplyImageResourceFix(979, GetScrCodeField(MoneyImage) + 4, drawF);             //Gold
    ApplyImageResourceFix(487, GetScrCodeField(BuldakImage) + 4, drawF);            //MovableStatueVictory3NE
}

void MonsterStrikeCallback()
{
    int thingId = GetUnitThingID(self);

    if (CurrentHealth(other))
    {
        if (thingId == 1386)
            HorrendousMissileAttack();
        else if (thingId == 1360)
            ZombieAttackReportComplete();
        else if (thingId == 2303)
            BullDakStrike();
    }
}

int BullDakBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1918985538; arr[1] = 50; arr[17] = 430; arr[19] = 200; arr[21] = 1065353216; 
		arr[23] = 32769; arr[24] = 1065353216; arr[28] = 1117782016; arr[29] = 10; arr[31] = 4; 
		arr[32] = 18; arr[33] = 26; arr[34] = 5; arr[35] = 5; arr[36] = 50; 
		arr[58] = 5545472; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

void BullDakSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1086324736);
		SetMemory(ptr + 0x224, 1086324736);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32769);
		SetMemory(GetMemory(ptr + 0x22c), 430);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 430);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BullDakBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void BullDakStrike()
{
    Damage(other, self, 160, 8);
    DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(other), GetObjectY(other)), 18);
}

void BullDakImageLoop(int imgUnit)
{
    int parent = imgUnit - 1;
    if (CurrentHealth(parent))
    {
        if (ToInt(DistanceUnitToUnit(imgUnit, parent)))
            MoveObject(imgUnit, GetObjectX(parent), GetObjectY(parent));
        FrameTimerWithArg(2, imgUnit, BullDakImageLoop);
    }
    else
        Delete(imgUnit);
}

void DisableObject(int obj)
{
    ObjectOff(obj);
}

void UnderGroundElevOn()
{
    ObjectOff(self);
    ObjectOn(Object("UndergroundElev"));
    UniPrint(other, "엘리베이터가 작동을 시작했습니다!");
}

void SecretWallOpen()
{
    ObjectOff(self);
    WallOpen(Wall(118, 204));
    WallOpen(Wall(119, 203));
    WallOpen(Wall(120, 202));

    WallOpen(Wall(122, 200));
    WallOpen(Wall(123, 199));
    WallOpen(Wall(124, 198));

    WallOpen(Wall(126, 198));
    WallOpen(Wall(127, 199));
    WallOpen(Wall(128, 200));

}
