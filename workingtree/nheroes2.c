
#include "noxscript\builtins.h"

#include "libs\typecast.h"
#include "libs\callmethod.h"

#include "libs\mathlab.h"
#include "libs\spellutil.h"
#include "libs\fxeffect.h"
#include "libs\waypoint.h"
#include "libs\fixtellstory.h"
#include "libs\walls.h"

#include "libs\unitutil.h"
#include "libs\unitstruct.h"
#include "libs\printutil.h"
#include "libs\itemproperty.h"

#include "libs\coopteam.h"
#include "libs\username.h"
#include "libs\playerupdate.h"

#define USER_LASTPOS_WAYID 192

int LastUnit;
int player[20];

int s_playerLastPositions;

#define TRUE 1
#define NULLPTR 0

int CheckPlayerSkillFlag1(int sPlr)
{
    return player[sPlr + 10] & 0x04;
}

int CheckPlayerSkillFlag2(int sPlr)
{
    return player[sPlr + 10] & 0x08;
}

void PlayerSetSkillFlag1(int sPlr)
{
    player[sPlr + 10] = player[sPlr + 10] ^ 0x04;
}

void PlayerSetSkillFlag2(int sPlr)
{
    player[sPlr + 10] = player[sPlr + 10] ^ 0x08;
}

#define SELF -2
#define OTHER -1

#define MAGIC_CASTER    "ImaginaryCaster"

void LightningBoltCollide()
{
    int owner = GetOwner(SELF);

    if (!GetTrigger() || !owner)
        return;

    if (CurrentHealth(OTHER) && IsAttackedBy(owner, OTHER))
    {
        int damageSub = CreateObjectAt(MAGIC_CASTER, GetObjectX(OTHER) - UnitAngleCos(OTHER, 3.0), GetObjectY(OTHER) - UnitAngleSin(OTHER, 3.0));

        SetOwner(owner, damageSub);
        Damage(OTHER, damageSub, 100, 0);
        Delete(SELF);
    }
}

int WoundedConjurerBinTable()
{
	int link, arr[62];

	if (!link)
	{
		arr[0] = 1853189975; arr[1] = 1130653028; arr[2] = 1969909359; arr[3] = 7497074; arr[17] = 275; 
		arr[19] = 120; arr[21] = 1065353216; arr[23] = 32832; arr[24] = 1065353216; arr[26] = 4; 
		arr[37] = 1751607628; arr[38] = 1852403316; arr[39] = 1819230823; arr[40] = 116; arr[53] = 1133903872; 
		arr[55] = 14; arr[56] = 24; arr[60] = 2271; arr[61] = 46912768; 
		link = &arr;
	}
	return link;
}

void WoundedConjurerSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1080452710);
		SetMemory(ptr + 0x224, 1080452710);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32832);
		SetMemory(GetMemory(ptr + 0x22c), 275);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 275);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WoundedConjurerBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void BeastLinkBinScript(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WeirdlingBeastBinTable());
        UnitZeroFleeRange(unit);
    }
}

void UnitVisibleSplash()
{
    int parent = GetOwner(self);
    int spIdx = ToInt(GetObjectZ(parent + 1)), ptr = UnitToPtr(other);

    if (ptr)
    {
        if (GetMemory(ptr + 0x1c) ^ spIdx && CurrentHealth(GetOwner(parent)))
        {
            if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) <= GetObjectX(parent))
            {
                CallFunction(ToInt(GetObjectZ(parent)));
                if (!HasClass(other, "PLAYER"))
                    SetMemory(ptr + 0x1c, spIdx);
            }
        }
    }
}

void SplashHandler(int owner, int func, float x, float y, float range)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", range, y) + 2, k, SplashIdx;

    Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(ptr), GetObjectY(ptr)), SplashIdx);
    SetOwner(owner, ptr - 2);
    Raise(ptr - 2, ToFloat(func));
    for (k = 0 ; k < 8 ; k ++)
    {
        DeleteObjectTimer(CreateObjectAt("WeirdlingBeast", x, y), 1);
        BeastLinkBinScript(ptr + k);
        UnitNoCollide(ptr + k);
        LookWithAngle(ptr + k, k * 32);
        SetOwner(ptr - 2, ptr + k);
        SetCallback(ptr + k, 3, UnitVisibleSplash);
    }
    DeleteObjectTimer(ptr - 2, 2);
    SplashIdx ++;
}

void GreenLightningEffect(float x1, float y1, float x2, float y2)
{
    GreenLightningFx(FloatToInt(x1), FloatToInt(y1), FloatToInt(x2), FloatToInt(y2), 12);
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
		arr[0] = 1635412311; arr[1] = 1699898482; arr[2] = 100;
		arr[16] = 80000; arr[17] = 300; arr[18] = 100; arr[19] = 50; 
		arr[21] = 1065353216; arr[23] = 40; arr[24] = 1067869798; 
		arr[26] = 4; arr[27] = 4;  
		
		arr[53] = 1128792064; arr[54] = 4; 
		link = &arr;
	}
	return link;
}

int MaidenBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1684627789; arr[1] = 28261; 
		arr[17] = 30; arr[18] = 92; arr[19] = 60; 
		arr[21] = 1065353216; arr[23] = 32776; arr[24] = 1065688760; 
		arr[27] = 1; arr[28] = 1106247680; arr[29] = 22; 
		arr[30] = 1101004800; arr[31] = 2; arr[32] = 22; arr[33] = 30;
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
		arr[0] = 1852796743; 
		arr[17] = 85; arr[19] = 15; 
		arr[21] = 1065353216; arr[23] = 32776; arr[24] = 1066192077; 
		arr[26] = 4; arr[28] = 1106247680; arr[29] = 25; 
		arr[30] = 1092616192; arr[31] = 4; arr[32] = 20; arr[33] = 28; arr[34] = 2; 
		arr[35] = 3; arr[36] = 20; 
		
		arr[57] = 5548176; arr[58] = 5546608; arr[59] = 5543680; 
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
        arr[16] = 80000; arr[17] = 200; arr[18] = 55; arr[19] = 60; 
		arr[21] = 1065353216; arr[23] = 8; arr[24] = 1069547520; 
		arr[37] = 1701996870; arr[38] = 1819042146;
		arr[53] = 1128792064; arr[54] = 4; 
		arr[55] = 20; arr[56] = 30; arr[57] = 5547984;
		link = &arr;
	}
	return link;
}

int WeirdlingBeastBinTable()
{
	int arr[62], link, unit;
	if (!link)
	{
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811;
		
		
		arr[17] = 85; arr[18] = 50; arr[19] = 55; 
		arr[21] = 1065353216; arr[23] = 32776; arr[24] = 1068708659; 
		arr[26] = 4; arr[28] = 1082130432; arr[29] = 20; 
		arr[31] = 2; arr[32] = 8; arr[33] = 16;
		arr[57] = 5548112; arr[59] = 5542784; 
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
		arr[21] = 1065353216; arr[23] = 4; arr[24] = 1069547520; 
		arr[26] = 4; arr[27] = 3; arr[28] = 1097859072; arr[29] = 25; 
		arr[31] = 8; arr[32] = 13; arr[33] = 21; arr[34] = 50; 
		arr[35] = 3; arr[36] = 6; arr[37] = 1684631635; arr[38] = 1884516965; arr[39] = 29801; 
		arr[53] = 1128792064;
		arr[55] = 20; arr[56] = 28; arr[59] = 5544896; 
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
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972;
        arr[17] = 85; arr[18] = 25; arr[19] = 120; 
		arr[21] = 1065353216; arr[23] = 65544; arr[24] = 1065353216; 
		arr[37] = 1801545047; arr[38] = 1701996870; arr[39] = 1819042146; 
		arr[53] = 1128792064;
		arr[55] = 15; arr[56] = 21; arr[58] = 5545472;
        link = &arr;
	}
	return link;
}

int AirshipCaptainBinTable()
{
	int arr[62], link;

	if (!link)
	{
		arr[0] = 1936877889; arr[1] = 1131440488; arr[2] = 1635020897; arr[3] = 28265;
		
		arr[17] = 130; arr[18] = 100; arr[19] = 60; 
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1067869798; 
		arr[26] = 4; arr[28] = 1077936128; arr[29] = 20; 
		arr[31] = 8; arr[32] = 12; arr[33] = 20;
		arr[57] = 5547984; arr[58] = 5546320; arr[59] = 5542432; 
		link = &arr;
	}
	return link;
}

int HecubahBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 6840674;
		arr[17] = 300; arr[19] = 100; 
		arr[21] = 1065353216; arr[24] = 1065353216; 
		arr[25] = 1; arr[26] = 4; arr[27] = 7; arr[28] = 1108082688; arr[29] = 50; 
		arr[30] = 1092616192; arr[32] = 10; arr[33] = 18;
		arr[57] = 5548288; arr[59] = 5542784; 

        link = &arr;
	}
	return link;
}

void MonsterWizardRedProcess(int unit)
{
    //TODO: Index. 7, ThingName= WizardRed
    int ptr = UnitToPtr(unit), uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WizardRedBinTable());
        UnitZeroFleeRange(unit);
        SetUnitMaxHealth(unit, 225);
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

int UserDamageArrowCreateThing(int owner, float x, float y, int dam, int thingID)
{
    int unit = CreateObjectAt("MercArcherArrow", x, y);
    int ptr = GetMemory(0x750710);

    SetOwner(owner, unit);
    //SetMemory(ptr + 0x14, 0x32);
    if (thingID)
        SetMemory(ptr + 0x04, thingID);
    SetMemory(GetMemory(ptr + 0x2bc) + 4, dam);
    return unit;
}

int IsFlame(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        return GetMemory(ptr + 8) & 0x2000;
    return false;
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

void HomePortalHandler(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner))
        {
            if (count)
            {
                if (DistanceUnitToUnit(owner, ptr) < 13.0)
                {
                    LookWithAngle(ptr, count - 1);
                    FrameTimerWithArg(1, ptr, HomePortalHandler);
                    break;
                }
                else
                    UniPrint(owner, "공간이동이 취소되었습니다");
            }
            else
            {
                Effect("TELEPORT", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
                Effect("SMOKE_BLAST", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
                MoveObject(owner, GetWaypointX(89), GetWaypointY(89));
                Effect("TELEPORT", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
                Effect("BLUE_SPARKS", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
            }
        }
        Delete(ptr);
        Delete(ptr + 1);
        break;
    }
}

int CheckPlayer()
{
    int i;

    for (i = 9 ; i >= 0 ; i -= 1)
    {
        if (IsCaller(player[i]))
            return i;
    }
    return -1;
}

int CheckPlayerDeathFlag(int plr)
{
    return player[plr + 10] & 0x02;
}

void SetPlayerDeathFlag(int plr)
{
    player[plr + 10] = player[plr + 10] ^ 0x02;
}

void PlayerOnDeath(int plr)
{
    UniPrintToAll(PlayerIngameNick(player[plr]) + " 님께서 적에게 격추되었습니다");
}

void PlayerOnJoin(int plr)
{
    int unit = player[plr];

    if (CheckPlayerDeathFlag(plr))
        SetPlayerDeathFlag(plr);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    MoveObject(unit, GetWaypointX(89), GetWaypointY(89));
    DeleteObjectTimer(CreateObject("BlueRain", 89), 30);
    AudioEvent("BlindOff", 89);
}

void PlayerOnFailed()
{
    Enchant(other, "ENCHANT_ANCHORED", 0.0);
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    Enchant(other, "ENCHANT_BLINDED", 0.0);
    MoveObject(other, GetWaypointX(90), GetWaypointY(90));
    UniPrint(other, "이 지도가 수용할 수 있는 유저 수 10 개를 초과했기 때문에 해당 유저는 지도 입장에 실패하였습니다");
    UniPrint(other, "잠시 후 다시 시도해보세요. 같은 문제가 반복되면 지도의 담당자에게 연락주세요");
}

int PlayerClassOnInit(int plr, int pUnit)
{
    player[plr] = pUnit;
    player[plr + 10] = 1;

    SelfDamageClassEntry(pUnit);
    DiePlayerHandlerEntry(pUnit);
    ChangeGold(pUnit, -GetGold(pUnit));

    int posUnit = GetPlayerLastPositionUnit(plr);

    if (posUnit)
    {
        MoveObject(posUnit, LocationX(USER_LASTPOS_WAYID), LocationY(USER_LASTPOS_WAYID));
        SetOwner(pUnit, posUnit);
    }
    else
    {
        UniPrintToAll("!오류발생! 유저 최근 위치유닛이 소멸되었습니다");
    }

    return plr;
}

void inputVariableToPlayers()
{
    int i, plr;

    while (TRUE)
    {
        if (CurrentHealth(other))
        {
            plr = CheckPlayer();
            for (i = 9 ; i >= 0 && plr < 0 ; i -= 1)
            {
                if (!MaxHealth(player[i]))
                {
                    plr = PlayerClassOnInit(i, GetCaller());
                    break;
                }
            }
            if (plr + 1)
            {
                PlayerOnJoin(plr);
                break;
            }
        }
        PlayerOnFailed();
        break;
    }
}

void PlayerOnShutdown(int plr)
{
    player[plr] = 0;
    player[plr + 10] = 0;
}

void PlayerOnCastHomePortal(int unit)
{
    int ptr = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit));

    CreateObjectAt("OblivionUp", GetObjectX(ptr), GetObjectY(ptr));
    SetOwner(unit, ptr);
    LookWithAngle(ptr, 88);

    FrameTimerWithArg(1, ptr, HomePortalHandler);
    UniPrint(unit, "시작위치로 이동중입니다... 취소하려면 움직이세요");
}

void PlayerClassOnAlive(int plr)
{
    int pUnit = player[plr];

    if (HasEnchant(pUnit, "ENCHANT_SNEAK"))
    {
        EnchantOff(pUnit, "ENCHANT_SNEAK");
        if (CheckPlayerSkillFlag2(plr))
            PlayerOnCastWindBooster(pUnit);
        RemoveTreadLightly(pUnit);
    }
}

void PreservePlayerLoop()
{
    int i;

    for (i = 9 ; i >= 0 ; i -= 1)
    {
        while (true)
        {
            if (MaxHealth(player[i]))
            {
                if (GetUnitFlags(player[i]) & 0x40)
                    1;
                else if (CurrentHealth(player[i]))
                {
                    PlayerClassOnAlive(i);
                    break;
                }
                else
                {
                    if (!CheckPlayerDeathFlag(i))
                    {
                        SetPlayerDeathFlag(i);
                        PlayerOnDeath(i);
                    }
                    break;
                }
            }
            if (player[i + 10])
                PlayerOnShutdown(i);
            break;
        }
    }
    FrameTimer(1, PreservePlayerLoop);
}

void GreenSparkFx(float x, float y)
{
    int ptr = CreateObjectAt("MonsterGenerator", x, y);

    Damage(ptr, 0, 10, 100);
    Delete(ptr);
}

void TeleportProcess(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner))
        {
            if (count)
            {
                if (DistanceUnitToUnit(owner, ToInt(GetObjectZ(ptr))) < 23.0)
                {
                    LookWithAngle(ptr, count - 1);
                    FrameTimerWithArg(1, ptr, TeleportProcess);
                    break;
                }
            }
            else
            {
                Effect("TELEPORT", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
                Effect("SMOKE_BLAST", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
                MoveObject(owner, GetObjectX(ptr), GetObjectY(ptr));
                MoveWaypoint(1, GetObjectX(owner), GetObjectY(owner));
                AudioEvent("BlindOff", 1);
                Effect("TELEPORT", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
                Effect("SMOKE_BLAST", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
            }
            EnchantOff(owner, "ENCHANT_BURNING");
        }
        Delete(ptr);
        Delete(ptr + 1);
        break;
    }
}

void UnitTeleportHandler()
{
    int unit;

    if (CurrentHealth(other))
    {
        if (!HasEnchant(other, "ENCHANT_BURNING"))
        {
            Enchant(other, "ENCHANT_BURNING", 4.0);
            unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(GetTrigger() + 1), GetObjectY(GetTrigger() + 1));
            SetOwner(other, unit);
            Raise(unit, ToFloat(GetTrigger()));
            LookWithAngle(unit, 48);
            CreateObjectAt("VortexSource", GetObjectX(self), GetObjectY(self));
            GreenSparkFx(GetObjectX(self), GetObjectY(self));
            FrameTimerWithArg(1, unit, TeleportProcess);
            UniPrint(other, "공간이동을 준비중입니다, 취소하려면 이곳에서 벗어나세요");
        }
    }
}

int TeleportPortal(int srcLocation, int destLocation)
{
    int unit = CreateObject("WeirdlingBeast", srcLocation);

    SetUnitMaxHealth(CreateObject("InvisibleLightBlueLow", destLocation) - 1, 30);
    UnitNoCollide(CreateObjectAt("TeleportWake", GetObjectX(unit), GetObjectY(unit)));
    Frozen(unit + 2, 1);
    SetCallback(unit, 9, UnitTeleportHandler);
    Damage(unit, 0, 999, -1);

    return unit;
}

void FieldTeleportSetup()
{
    TeleportPortal(176, 50);
    TeleportPortal(175, 51);
    TeleportPortal(177, 143);
    TeleportPortal(178, 144);
    TeleportPortal(179, 145);
    TeleportPortal(180, 142);
    TeleportPortal(181, 142);
    TeleportPortal(182, 142);
}

void DisableObject(int unit)
{
    if (IsObjectOn(unit))
        ObjectOff(unit);
}

void ZombieDeadHandler()
{
    MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
    FieldMonsterDeath();
    if (MaxHealth(self))
        Damage(self, 0, 100, 14);
    DeleteObjectTimer(CreateObject("MediumFlame", 1), 150);
    AudioEvent("BurnCast", 1);
    Effect("SPARK_EXPLOSION", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
}

void FieldMonsterDeath()
{
    SpawnRandomItem(self);
    DeleteObjectTimer(self, 120);
}

void FieldMonsterCommon(int unit)
{
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    AggressionLevel(unit, 1.0);
    SetCallback(unit, 5, FieldMonsterDeath);
}

int FieldMonsterFunctions(int index)
{
    int mobMethod[] = {FieldMobFireSprite, FieldMobSwordsman, FieldMobArcher, FieldMobFlyingGolem, FieldMobSkeleton, FieldMobSkeletonLord,
        FieldMobOgreAxe, FieldMobOgre, FieldMobOgreLord, FieldMobCherub, FieldMobImp, FieldMobMystic, FieldMobBeholder,
        FieldMobUrchin, FieldMobSUrchin, FieldMobGoon, FieldMobTroll, FieldMobBlackSpider, FieldMobBeast, FieldMobMaiden,
        FieldMobWolf, FieldMobWhiteWolf, FieldMobBlackWolf, FieldMobBear, FieldMobBlackBear, FieldMobGhost, FieldMobHecubah,
        FieldMobGolem, FieldMobMecaGolem, FieldMobWasp, FieldMobBat, FieldMobScorpion, FieldMobRedWizard, FieldMobDemon,
        FieldMobPlant, FieldMobWhiteSpider, FieldMobVileZombie, FieldMobZombie};

    return mobMethod[index];
}

int FieldMobFireSprite(int target)
{
    int unit = CreateObjectAt("FireSprite", GetObjectX(target), GetObjectY(target));

    UnitLinkBinScript(unit, FireSpriteBinTable());
    SetUnitVoice(unit, 57);
    SetUnitMaxHealth(unit, 128);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000);
    FieldMonsterCommon(unit);

    return unit;
}

int FieldMobSwordsman(int target)
{
    int unit = CreateObjectAt("Swordsman", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 325);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobArcher(int target)
{
    int unit = CreateObjectAt("Archer", GetObjectX(target), GetObjectY(target));
    
    SetUnitVoice(unit, 10);
    SetUnitMaxHealth(unit, 98);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobFlyingGolem(int target)
{
    int unit = CreateObjectAt("FlyingGolem", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 98);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobSkeleton(int target)
{
    int unit = CreateObjectAt("Skeleton", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 250);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobSkeletonLord(int target)
{
    int unit = CreateObjectAt("SkeletonLord", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 295);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobOgreAxe(int target)
{
    int unit = CreateObjectAt("GruntAxe", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 275);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobOgre(int target)
{
    int unit = CreateObjectAt("OgreBrute", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 306);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobOgreLord(int target)
{
    int unit = CreateObjectAt("OgreWarlord", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 325);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobCherub(int target)
{
    int unit = CreateObjectAt("EvilCherub", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 96);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobImp(int target)
{
    int unit = CreateObjectAt("Imp", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 64);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobMystic(int target)
{
    int unit = CreateObjectAt("Wizard", GetObjectX(target), GetObjectY(target));

    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    SetUnitMaxHealth(unit, 250);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobBeholder(int target)
{
    int unit = CreateObjectAt("Beholder", GetObjectX(target), GetObjectY(target));

    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    SetUnitMaxHealth(unit, 306);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobUrchin(int target)
{
    int unit = CreateObjectAt("Urchin", GetObjectX(target), GetObjectY(target));

    UnitZeroFleeRange(unit);
    SetUnitMaxHealth(unit, 108);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobSUrchin(int target)
{
    int unit = CreateObjectAt("UrchinShaman", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 138);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobGoon(int target)
{
    int unit = CreateObjectAt("Goon", GetObjectX(target), GetObjectY(target));
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, GoonBinTable());
    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(63)); //TODO: Mimic's voice
    SetUnitMaxHealth(unit, 180);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobTroll(int target)
{
    int unit = CreateObjectAt("Troll", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 325);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobBlackSpider(int target)
{
    int unit = CreateObjectAt("BlackWidow", GetObjectX(target), GetObjectY(target));
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BlackWidowBinTable());
    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(19));
    SetUnitMaxHealth(unit, 295);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobBeast(int target)
{
    int unit = CreateObjectAt("WeirdlingBeast", GetObjectX(target), GetObjectY(target));
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WeirdlingBeastBinTable());
    UnitZeroFleeRange(unit);
    SetUnitMaxHealth(unit, 185);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobMaiden(int target)
{
    int unit = CreateObjectAt("Bear2", GetObjectX(target), GetObjectY(target));
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, MaidenBinTable());
    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(25));
    ChangeColorMaiden(Random(0, 255), Random(0, 255), Random(0, 255), unit);
    SetUnitMaxHealth(unit, 306);
    FieldMonsterCommon(unit);

    return unit;
}

int FieldMobWolf(int target)
{
    int unit = CreateObjectAt("Wolf", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 135);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobWhiteWolf(int target)
{
    int unit = CreateObjectAt("WhiteWolf", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 195);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobBlackWolf(int target)
{
    int unit = CreateObjectAt("BlackWolf", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 230);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobBear(int target)
{
    int unit = CreateObjectAt("Bear", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 350);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobBlackBear(int target)
{
    int unit = CreateObjectAt("BlackBear", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 295);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobGhost(int target)
{
    int unit = CreateObjectAt("Ghost", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 98);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobHecubah(int target)
{
    int unit = CreateObjectAt("Hecubah", GetObjectX(target), GetObjectY(target));

    UnitZeroFleeRange(unit);
    UnitLinkBinScript(unit, HecubahBinTable());
    SetUnitMaxHealth(unit, 280);
    FieldMonsterCommon(unit);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x20);
    return unit;
}

int FieldMobGolem(int target)
{
    int unit = CreateObjectAt("StoneGolem", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 600);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobMecaGolem(int target)
{
    int unit = CreateObjectAt("MechanicalGolem", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 700);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobWasp(int target)
{
    int unit = CreateObjectAt("Wasp", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 60);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobBat(int target)
{
    int unit = CreateObjectAt("Bat", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 60);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobScorpion(int target)
{
    int unit = CreateObjectAt("Scorpion", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 285);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobRedWizard(int target)
{
    int unit = CreateObjectAt("WizardRed", GetObjectX(target), GetObjectY(target));

    UnitZeroFleeRange(unit);
    MonsterWizardRedProcess(unit);
    SetUnitMaxHealth(unit, 285);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobDemon(int target)
{
    int unit;
    
    if (Random(0, 3))
        unit = CreateObjectAt("MeleeDemon", GetObjectX(target), GetObjectY(target));
    else
        unit = CreateObjectAt("EmberDemon", GetObjectX(target), GetObjectY(target));
    SetUnitMaxHealth(unit, 225);
    FieldMonsterCommon(unit);
    return unit;
}

int FieldMobPlant(int target)
{
    int unit = CreateObjectAt("CarnivorousPlant", GetObjectX(target), GetObjectY(target));

    SetUnitSpeed(unit, 1.5);
    SetUnitMaxHealth(unit, 400);
    FieldMonsterCommon(unit);

    return unit;
}

int FieldMobWhiteSpider(int target)
{
    int unit = CreateObjectAt("AlbinoSpider", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 200);
    FieldMonsterCommon(unit);

    return unit;
}

int FieldMobVileZombie(int target)
{
    int unit = CreateObjectAt("VileZombie", GetObjectX(target), GetObjectY(target));

    SetUnitSpeed(unit, 1.8);
    SetUnitMaxHealth(unit, 306);
    FieldMonsterCommon(unit);
    SetCallback(unit, 5, ZombieDeadHandler);

    return unit;
}

int FieldMobZombie(int target)
{
    int unit = CreateObjectAt("Zombie", GetObjectX(target), GetObjectY(target));

    SetUnitMaxHealth(unit, 240);
    FieldMonsterCommon(unit);
    SetCallback(unit, 5, ZombieDeadHandler);

    return unit;
}

void GenDetectEnemy()
{
    if (ToInt(DistanceUnitToUnit(self, other)))
    {
        GreenLightningEffect(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other));
        Damage(other, self, 20, 16);
    }
}

void GenSummonHandler()
{
    int ptr = UnitToPtr(self), data, unit;

    if (ptr)
    {
        data = GetMemory(GetMemory(ptr + 0x2ec) + 84) & 0xff00ffff;
        SetMemory(GetMemory(ptr + 0x2ec) + 84, data);
        unit = CreateObjectAt("WeirdlingBeast", GetObjectX(self), GetObjectY(self));
        UnitNoCollide(unit);
        SetUnitScanRange(unit, 500.0);
        LookAtObject(unit, other);
        DeleteObjectTimer(unit, 1);
        SetCallback(unit, 3, GenDetectEnemy);
        Delete(other);
    }
}

void NPCRemoveAllEquipments(int unit)
{
	int inven = unit + 2;
	
	while (HasClass(inven, "WEAPON") || HasClass(inven, "ARMOR") || HasClass(inven, "TREASURE"))
    {
		Delete(inven);
		inven += 2;
	}
}

void NPCHandlerOnHurt()
{
    return;
}

void NPCHandlerOnDeath()
{
    NPCRemoveAllEquipments(GetTrigger());
    SpawnRandomItem(GetTrigger());
    DeleteObjectTimer(self, 60);
}

void NPCHandlerOnDetect()
{
    return;
}

void NPCHandlerOnLostEnemy()
{
    return;
}

void NPCHandlerOnCollide()
{
    return;
}

void CheckArmor34Property(int unit)
{
    if (HasClass(unit, "ARMOR"))
    {
        SetNpcArmorProperties(unit);
    }
}

void PowerInventoryNPC(int unit)
{
    int inv = GetLastItem(unit);

    while (inv)
    {
        if (!HasEnchant(inv, "ENCHANT_INVULNERABLE"))
        {
            Enchant(inv, "ENCHANT_INVULNERABLE", 0.0);
            CheckArmor34Property(inv);
        }
        inv = GetPreviousItem(inv);
    }
}

void ForestWizDeath()
{
    ObjectOn(Object("bossElevator"));
    NPCHandlerOnDeath();
    UniPrintToAll("방금 숲속의 마법사가 죽었습니다, 보스로 가는 엘리베이터가 작동되었습니다");
}

void ForestWizard(int unit)
{
    if (CurrentHealth(unit))
    {
        SetCallback(unit, 5, ForestWizDeath);
    }
}

void DetetctForceOfNature(int cur)
{
    int owner = GetOwner(cur), unit;

    if (CurrentHealth(owner))
    {
        unit = CreateObjectAt("TitanFireball", GetObjectX(cur), GetObjectY(cur));
        SetOwner(owner, unit);
        LookWithAngle(unit, GetDirection(owner));
        PushObjectTo(unit, UnitAngleCos(owner, 32.0), UnitAngleSin(owner, 32.0));
        Delete(cur);
    }
}

void DetectMagicWandMissile(int cur)
{
    int owner = GetOwner(cur), unit;

    if (CurrentHealth(owner))
    {
        unit = CreateObjectAt("DeathBallFragment", GetObjectX(cur), GetObjectY(cur));
        SetOwner(owner, unit);
        PushObjectTo(unit, UnitAngleCos(owner, 35.0), UnitAngleSin(owner, 35.0));
        Delete(cur);
    }
}

int SpawnDummyUnit(string unitName, float xProfile, float yProfile)
{
    int unit = CreateObjectAt(unitName, xProfile, yProfile);

    ObjectOff(unit);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    Frozen(unit, 1);
    return unit;
}

void RedRingFlying(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner) && count)
        {
            if (IsVisibleTo(ptr, ptr - 1))
            {
                MoveObject(ptr, GetObjectX(ptr) + GetObjectZ(ptr + 2), GetObjectY(ptr) + GetObjectZ(ptr + 3));
                MoveObject(ptr + 1, GetObjectX(ptr), GetObjectY(ptr));
                RedRingUpdatePosition(ptr + 2, ptr);
                LookWithAngle(ptr, count - 1);
                FrameTimerWithArg(1, ptr, RedRingFlying);
                break;
            }
        }
        Delete(ptr - 1);
        Delete(ptr);
        if (MaxHealth(ptr + 1))
            Delete(ptr + 1);
        RemoveRedRings(ptr + 2);
        break;
    }
}

void RedRingUpdatePosition(int ptr, int posUnit)
{
    float xProfile = GetObjectX(posUnit), yProfile = GetObjectY(posUnit);
    int i, count = GetDirection(ptr);

    for (i = 0 ; i < count ; i ++)
        MoveObject(ptr + i, xProfile, yProfile);
}

void RemoveRedRings(int ptr)
{
    int count = GetDirection(ptr), i;

    for (i = 0 ; i < count ; i ++)
        Delete(ptr + i);
}

void DrawRedRings(int ptr, int count)
{
    int i;

    for (i = count ; i ; i --)
        Enchant(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(ptr), GetObjectY(ptr)), "ENCHANT_PROTECT_FROM_FIRE", 0.0);
}

void ExplosionRedRing()
{
    Damage(other, GetOwner(GetOwner(self)), 165, 1);
}

void OnCollideRedRing()
{
    int owner = GetOwner(GetTrigger() - 1);

    if (CurrentHealth(other) && MaxHealth(self) && IsAttackedBy(other, owner))
    {
        Raise(self, ExplosionRedRing);
        SplashHandler(owner, ToInt(GetObjectZ(self)), GetObjectX(self), GetObjectY(self), 40.0);
        Effect("THIN_EXPLOSION", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Delete(self);
    }
}

void SkillShotRedRing(int owner)
{
    float xVect = UnitAngleCos(owner, 20.0), yVect = UnitAngleSin(owner, 20.0);
    int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner) + xVect, GetObjectY(owner) + yVect) + 1;

    SetOwner(owner, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner) + xVect, GetObjectY(owner) + yVect));
    DrawRedRings(SpawnDummyUnit("Maiden", GetObjectX(unit), GetObjectY(unit)) - 1, 12);
    LookWithAngle(unit + 2, 12);
    Raise(unit + 2, xVect);
    Raise(unit + 3, yVect);
    LookWithAngle(unit, 20);
    SetCallback(unit + 1, 9, OnCollideRedRing);
    FrameTimerWithArg(1, unit, RedRingFlying);
}

void FlameHandlerFlame(int cur, int owner)
{
    int mis;

    if (CurrentHealth(owner))
    {
        mis = UserDamageArrowCreateThing(owner, GetObjectX(cur), GetObjectY(cur), 48, 526);
        Enchant(mis, "ENCHANT_DETECTING", 0.0);
        LookAtObject(mis, owner);
        LookWithAngle(mis, GetDirection(mis) + 128);
        PushObjectTo(mis, UnitRatioX(mis, owner, 8.0), UnitRatioY(mis, owner, 8.0));
    }
    Delete(cur);
}

void FlameHandlerBlueFlame(int cur, int owner)
{
    if (CurrentHealth(owner))
    {
        if (!HasEnchant(owner, "ENCHANT_VILLAIN"))
        {
            Enchant(owner, "ENCHANT_VILLAIN", 1.0);
            SkillShotRedRing(owner);
        }
        Delete(cur);
    }
}

void FlameHandler(int cur)
{
    int thingID, owner = GetOwner(cur);

    if (HasClass(owner, "PLAYER"))
    {
        thingID = GetUnitThingID(cur);
        if (thingID >= 192 && thingID <= 194)
            FlameHandlerBlueFlame(cur, owner);
        else if (thingID >= 199 && thingID <= 205)
            FlameHandlerFlame(cur, owner);
    }
}

void DetectedSpecificIndex(int curId)
{
    if (IsMissileUnit(curId))
    {
        int thingID = GetUnitThingID(curId);

        if (thingID == 526)
            PlayerOnCastWallKite(curId);
        else if (thingID == 709)
            DetectMagicWandMissile(curId);
        else if (thingID == 706)
            DetetctForceOfNature(curId);
        else if (thingID == 710)
            SetUnitCallbackOnCollide(curId, LightningBoltCollide);
    }
    else if (IsFlame(curId))
        FlameHandler(curId);
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

void SayRemainHealth(int unit)
{
    if (HasEnchant(unit, "ENCHANT_ETHEREAL"))
        return;
    else
    {
        Enchant(unit, "ENCHANT_ETHEREAL", 0.8);
        Chat(unit, "남은 체력: " + IntToString(CurrentHealth(unit)));
    }
}

void ResetUnitSight(int unit)
{
    EnchantOff(unit, "ENCHANT_DETECTING");
    Enchant(unit, "ENCHANT_BLINDED", 0.08);
}

void CheckResetSight(int unit, int delay)
{
    if (!HasEnchant(unit, "ENCHANT_DETECTING"))
    {
        Enchant(unit, "ENCHANT_DETECTING", 0.0);
        FrameTimerWithArg(delay, unit, ResetUnitSight);
    }
}

void BossWarriorHurt()
{
    SayRemainHealth(self);
}

void BossWarriorDeath()
{
    UniBroadcast("축하합니다! 전사 보스를 격추시켰습니다");
    UniPrintToAll("전사 보스가 격추되었습니다");
}

void BossWarriorDetect()
{
    int unit;

    if (!HasEnchant(self, "ENCHANT_PROTECT_FROM_MAGIC"))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(self), GetObjectY(self));
        SetOwner(self, unit);
        Raise(unit, GetCaller());
        FrameTimerWithArg(1, unit, WarriorBossSkills(Random(0, 3)));
        Enchant(self, "ENCHANT_PROTECT_FROM_MAGIC", 2.0);
    }
    CheckResetSight(GetTrigger(), 20);
}

void BossWarriorSetup(int unit)
{
    if (CurrentHealth(unit))
    {
        SetCallback(unit, 3, BossWarriorDetect);
        SetCallback(unit, 5, BossWarriorDeath);
        SetCallback(unit, 7, BossWarriorHurt);
    }
}

void BossWizardRealDetect()
{
    int unit;

    if (!HasEnchant(self, "ENCHANT_PROTECT_FROM_MAGIC"))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(self), GetObjectY(self));
        SetOwner(self, unit);
        Raise(unit, GetCaller());
        FrameTimerWithArg(1, unit, WizardBossSkills(Random(0, 3)));
        Enchant(self, "ENCHANT_PROTECT_FROM_MAGIC", 2.0);
    }
    CheckResetSight(GetTrigger(), 20);
}

void BossWizardHurt()
{
    SayRemainHealth(self);
}

void BossWizardDeath()
{
    UniBroadcast("축하합니다! 보스 마법사를 격추시켰습니다");
    UniPrintToAll("보스 마법사를 격추시켰습니다");
}

void BossWizardDetect()
{
    Effect("TELEPORT", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    Effect("SMOKE_BLAST", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    MoveObject(self, GetWaypointX(46), GetWaypointY(46));
    Effect("TELEPORT", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    Effect("SMOKE_BLAST", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    CastSpellObjectObject("SPELL_FIREBALL", self, other);
    SetCallback(self, 3, BossWizardRealDetect);
    Chat(self, "여기가 어디라고 함부로 들어오느냐? 덤벼라");
    CheckResetSight(GetTrigger(), 20);
}

void BossWizardSetup(int unit)
{
    if (CurrentHealth(unit))
    {
        ObjectOff(unit);
        SetCallback(unit, 3, BossWizardDetect);
        SetCallback(unit, 5, BossWizardDeath);
        SetCallback(unit, 7, BossWizardHurt);
        PutWizardBossWakeSwitch(10, unit);
    }
}

void WarriorLaiserTouched()
{
    int owner = GetOwner(GetTrigger() + 1);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 100, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.1);
    }
}

void WarBossLaiserShot(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), unit;

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner) && count)
        {
            if (IsVisibleTo(ptr, ptr + 1))
            {
                MoveObject(ptr, GetObjectX(ptr) + GetObjectZ(ptr), GetObjectY(ptr) + GetObjectZ(ptr + 1));
                MoveObject(ptr + 1, GetObjectX(ptr + 1) + GetObjectZ(ptr), GetObjectY(ptr + 1) + GetObjectZ(ptr + 1));
                Effect("SENTRY_RAY", GetObjectX(ptr), GetObjectY(ptr), GetObjectX(ptr + 1), GetObjectY(ptr + 1));
                unit = DummyUnitCreate("Troll", GetObjectX(ptr), GetObjectY(ptr));
                SetOwner(owner, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)));
                DeleteObjectTimer(unit, 1);
                DeleteObjectTimer(unit + 1, 3);
                SetCallback(unit, 9, WarriorLaiserTouched);
                FrameTimerWithArg(1, ptr, WarBossLaiserShot);
                break;
            }
        }
        Delete(ptr);
        Delete(ptr + 1);
        break;
    }
}

void UnitBlocking(int unit, int target, float gap)
{
    int pusher = CreateObjectAt("FlyingMachines3", GetObjectX(unit) + UnitRatioX(unit, target, gap), GetObjectY(unit) + UnitRatioY(unit, target, gap));

    Frozen(pusher, 1);
    DeleteObjectTimer(pusher, 1);
}

void BombSpiderWalking()
{
    if (DistanceUnitToUnit(self, other) > 46.0)
    {
        UnitBlocking(self, other, 2.0);
        LookAtObject(self, other);
        if (ToInt(GetObjectZ(GetTrigger() + 1)) ^ GetCaller())
        {
            Raise(GetTrigger() + 1, GetCaller());
        }
    }
    else
    {
        Damage(other, self, 100, 10);
        Effect("EXPLOSION", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        Delete(self);
    }
    Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void BombSpiderClearSight()
{
    int enemy = ToInt(GetObjectZ(GetTrigger() + 1));

    EnchantOff(self, "ENCHANT_BLINDED");
    if (CurrentHealth(enemy) && IsVisibleTo(self, enemy))
    {
        LookAtObject(self, enemy);
        Attack(self, enemy);
    }
}

void BombSpiderCheckLifeTime(int ptr)
{
    int count = GetDirection(ptr + 1);

    while (1)
    {
        if (CurrentHealth(ptr) && count)
        {
            LookWithAngle(ptr + 1, count - 1);
            FrameTimerWithArg(1, ptr, BombSpiderCheckLifeTime);
            break;
        }
        if (MaxHealth(ptr))
        {
            Effect("EXPLOSION", GetObjectX(ptr), GetObjectY(ptr), 0.0, 0.0);
            Delete(ptr);
        }
        Delete(ptr + 1);
        break;
    }
}

int WarSummonBombSpider(int owner)
{
    int unit = CreateObjectAt("SmallAlbinoSpider", GetObjectX(owner) + UnitAngleCos(owner, 23.0), GetObjectY(owner) + UnitAngleSin(owner, 23.0));

    LookWithAngle(unit, GetDirection(owner));
    SetUnitScanRange(unit, 450.0);
    SetUnitMaxHealth(unit, 80);
    SetOwner(owner, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)));
    LookWithAngle(unit + 1, 180);
    Enchant(unit, "ENCHANT_FREEZE", 0.0);
    DeleteObjectTimer(CreateObjectAt("Smoke", GetObjectX(unit), GetObjectY(unit)), 20);
    SetCallback(unit, 3, BombSpiderWalking);
    SetCallback(unit, 13, BombSpiderClearSight);
    return unit;
}

int MakeShuriken(int owner, float xProfile, float yProfile, float force)
{
    int unit = CreateObjectAt("OgreShuriken", xProfile, yProfile);

    SetOwner(owner, unit);
    PushObject(unit, force, GetObjectX(owner), GetObjectY(owner));

    return unit;
}

void WarBossShurikenRing(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), i;
    int angle = GetDirection(ptr + 1);

    while (1)
    {
        if (CurrentHealth(owner) && count)
        {
            for (i = 0 ; i < 30 ; i ++)
                MakeShuriken(owner, GetObjectX(owner) + MathSine(i * 12 + 90 + angle, 17.0), GetObjectY(owner) + MathSine(i * 12 + angle, 17.0), 22.0);
            LookWithAngle(ptr, count - 1);
            LookWithAngle(ptr + 1, angle + 2);
            FrameTimerWithArg(2, ptr, WarBossShurikenRing);
            break;
        }
        if (CurrentHealth(owner))
            EnchantOff(owner, "ENCHANT_FREEZE");
        Delete(ptr);
        Delete(ptr + 1);
        break;
    }
}

void WizThunderStormTarget(int ptr)
{
    int owner = GetOwner(ptr);

    if (CurrentHealth(owner))
    {
        CastSpellObjectLocation("SPELL_DEATH_RAY", owner, GetObjectX(ptr), GetObjectY(ptr));
        CastSpellObjectLocation("SPELL_BURN", owner, GetObjectX(ptr), GetObjectY(ptr));
        Effect("VIOLET_SPARKS", GetObjectX(ptr), GetObjectY(ptr), 0.0, 0.0);
        Effect("SPARK_EXPLOSION", GetObjectX(ptr), GetObjectY(ptr), 0.0, 0.0);
    }
    Delete(ptr);
}

void RemoveFireRing(int ptr)
{
    int count = ToInt(GetObjectZ(ptr)), i;

    for (i = 0 ; i <= count ; i ++)
        Delete(ptr + i);
}

void FireRingExplosion(int ptr)
{
    int i;

    Effect("EXPLOSION", GetObjectX(ptr), GetObjectY(ptr), 0.0, 0.0);
    for (i = 0 ; i < 8 ; i ++)
        Effect("EXPLOSION", GetObjectX(ptr) + MathSine(45 * i + 90, 38.0), GetObjectY(ptr) + MathSine(45 * i, 38.0), 0.0, 0.0);
}

void FireRingExplosionDamage()
{
    Damage(other, GetOwner(GetOwner(self)), 100, 14);
}

void WizFireRing(int ptr)
{
    int owner = GetOwner(ptr), i, count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner))
        {
            if (count)
            {
                for (i = 36 ; i ; i --)
                    MoveObject(ptr + i, GetObjectX(ptr + i) - MathSine(GetDirection(ptr + i) * 10 + 90, 5.0), GetObjectY(ptr + i) - MathSine(GetDirection(ptr + i) * 10, 5.0));
                LookWithAngle(ptr, count - 1);
                FrameTimerWithArg(1, ptr, WizFireRing);
                break;
            }
            FireRingExplosion(ptr);
            Raise(ptr + 1, FireRingExplosionDamage);
            SplashHandler(owner, ToInt(GetObjectZ(ptr + 1)), GetObjectX(ptr), GetObjectY(ptr), 200.0);
        }
        FrameTimerWithArg(1, ptr, RemoveFireRing);
        break;
    }
}

void WizFireFairyDetectEnemy()
{
    int mis = CreateObjectAt("ImpShot", GetObjectX(self) + UnitRatioX(other, self, 13.0), GetObjectY(self) + UnitRatioY(other, self, 13.0));

    SetOwner(self, mis);
    PushObject(mis, -28.0, GetObjectX(other), GetObjectY(other));
    if (ToInt(GetObjectZ(GetTrigger() + 1)) ^ GetCaller())
        Raise(GetTrigger() + 1, GetCaller());
    Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void WizFireFairyLostEnemy()
{
    int enemy = ToInt(GetObjectZ(GetTrigger() + 1));

	EnchantOff(self, "ENCHANT_BLINDED");
	if (CurrentHealth(enemy) && IsVisibleTo(self, enemy))
	{
		LookAtObject(self, enemy);
		Attack(self, enemy);
	}
}

int WizSummonFireFairy(int location)
{
    int unit = CreateObject("FireSprite", location);
    int ptr = GetMemory(0x750710);

    CreateObject("InvisibleLightBlueLow", location);
    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FireSpriteBinTable());
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000);
    SetUnitScanRange(unit, 600.0);
    SetUnitMaxHealth(unit, 192);
    SetCallback(unit, 3, WizFireFairyDetectEnemy);
    SetCallback(unit, 13, WizFireFairyLostEnemy);
    return unit;
}

void WizSummonFx(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), unit, target = ToInt(GetObjectZ(ptr));

    while (1)
    {
        if (CurrentHealth(owner))
        {
            if (count)
            {
                MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
                unit = WizSummonFireFairy(1);
                if (CurrentHealth(target))
                    LookAtObject(unit, target);
                else
                    LookAtObject(unit, owner);
                DeleteObjectTimer(unit, 120);
                DeleteObjectTimer(unit + 1, 121);
                GreenLightningEffect(GetObjectX(owner), GetObjectY(owner), GetObjectX(owner), GetObjectY(owner));
                Effect("SMOKE_BLAST", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
                AudioEvent("SummonComplete", 1);
            }
            else
            {
                Effect("COUNTERSPELL_EXPLOSION", GetObjectX(ptr), GetObjectY(ptr), 0.0, 0.0);
                GreenSparkFx(GetObjectX(ptr), GetObjectY(ptr));
                LookWithAngle(ptr, count + 1);
                FrameTimerWithArg(18, ptr, WizSummonFx);
                break;
            }
        }
        Delete(ptr);
        break;
    }
}

int SpawnToxicCloud(float xProfile, float yProfile, int time)
{
    int unit = CreateObjectAt("ToxicColud", xProfile, yProfile);
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2ec), time);
    return unit;
}

void RemoveSoulStone(int ptr)
{
    int owner = GetOwner(ptr + 1);

    if (MaxHealth(ptr))
    {
        GreenExplosion(GetObjectX(ptr), GetObjectY(ptr));
        //not_yet
        SpawnToxicCloud(GetObjectX(ptr), GetObjectY(ptr), 90);
        Delete(ptr + 1);
        Delete(ptr);
    }
}

void PreserveSoulStone()
{
    int ptr = GetTrigger() + 1, owner;

    if (MaxHealth(self))
    {
        if (IsCaller(ptr))
        {
            if (GetDirection(ptr))
            {
                LookWithAngle(ptr, GetDirection(ptr) - 1);
                MoveObject(self, GetObjectX(ptr), GetObjectY(ptr));
            }
            else
            {
                RemoveSoulStone(ptr - 1);
            }
        }
        else if (CurrentHealth(other))
        {
            owner = GetOwner(ptr);
            if (IsAttackedBy(other, owner))
            {
                GreenSparkFx(GetObjectX(self), GetObjectY(self));
                Damage(other, owner, 50, 14);
            }
        }
    }
}

int SpawnSoulStone(int owner, float xProfile, float yProfile)
{
    int unit = DummyUnitCreate("Maiden", xProfile, yProfile);

    SetOwner(owner, CreateObjectAt("SpinningCrown", xProfile, yProfile));
    Enchant(unit + 1, "ENCHANT_RUN", 0.0);
    SetUnitMass(unit + 1, 100.0);
    DeleteObjectTimer(CreateObjectAt("MagicSpark", GetObjectX(unit), GetObjectY(unit)), 12);
    DeleteObjectTimer(CreateObjectAt("ReleasedSoul", GetObjectX(unit), GetObjectY(unit)), 12);
    LookWithAngle(unit + 1, 240);
    SetCallback(unit, 9, PreserveSoulStone);
    return unit;
}

int ConjurerSummonFist(int targetUnit, int owner)
{
    int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(targetUnit), GetObjectY(targetUnit));

    CastSpellObjectObject("SPELL_FIST", unit, unit);
    SetOwner(owner, unit + 1);
    Delete(unit);
    return unit + 1;
}

void ConjurerWindmillShot(int ptr)
{
    int owner = GetOwner(ptr), count = ToInt(GetObjectZ(ptr)), unit;

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner) && count)
        {
            unit = CreateObjectAt("DeathBallFragment", GetObjectX(owner) + UnitAngleCos(ptr, 17.0), GetObjectY(owner) + UnitAngleSin(ptr, 17.0));
            SetOwner(owner, unit);
            LookWithAngle(unit, GetDirection(ptr) + 64);
            SetOwner(owner, CreateObjectAt("DeathBallFragment", GetObjectX(owner) + UnitAngleCos(unit, 17.0), GetObjectY(owner) + UnitAngleSin(unit, 17.0)));
            LookWithAngle(unit, GetDirection(unit) + 64);
            SetOwner(owner, CreateObjectAt("DeathBallFragment", GetObjectX(owner) + UnitAngleCos(unit, 17.0), GetObjectY(owner) + UnitAngleSin(unit, 17.0)));
            LookWithAngle(unit, GetDirection(unit) + 64);
            SetOwner(owner, CreateObjectAt("DeathBallFragment", GetObjectX(owner) + UnitAngleCos(unit, 17.0), GetObjectY(owner) + UnitAngleSin(unit, 17.0)));
            PushObject(unit, 26.0, GetObjectX(owner), GetObjectY(owner));
            PushObject(unit + 1, 26.0, GetObjectX(owner), GetObjectY(owner));
            PushObject(unit + 2, 26.0, GetObjectX(owner), GetObjectY(owner));
            PushObject(unit + 3, 26.0, GetObjectX(owner), GetObjectY(owner));
            Raise(ptr, count - 1);
            LookWithAngle(ptr, GetDirection(ptr) + 2);
            FrameTimerWithArg(1, ptr, ConjurerWindmillShot);
            break;
        }
        Delete(ptr);
        break;
    }
}

void MeteorFallDamage()
{
    Damage(other, GetOwner(GetOwner(self)), 85, 14);
    Enchant(other, "ENCHANT_CHARMING", 0.08);
}

void MeteorSkyDrop(int ptr)
{
    int owner = GetOwner(ptr);

    Raise(ptr, MeteorFallDamage);
    SplashHandler(owner, ToInt(GetObjectZ(ptr)), GetObjectX(ptr), GetObjectY(ptr), 110.0);
    DeleteObjectTimer(CreateObjectAt("MeteorExplode", GetObjectX(ptr), GetObjectY(ptr)), 15);
    Delete(ptr);
    Delete(ptr + 1);
}

void HitMeteor(float posX, float posY, int owner)
{
    int unit = CreateObjectAt("InvisibleLightBlueHigh", posX, posY);

    SetOwner(owner, unit);
    CastSpellObjectObject("SPELL_METEOR", unit, unit);
    FrameTimerWithArg(17, unit, MeteorSkyDrop);
}

void ConjurerMeteorShower(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), pic;
    float rRange;

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner) && count)
        {
            pic = Random(0, 359);
            rRange = RandomFloat(20.0, 250.0);
            HitMeteor(GetObjectX(ptr) + MathSine(pic + 90, rRange), GetObjectY(ptr) + MathSine(pic, rRange), owner);
            LookWithAngle(ptr, count - 1);
            FrameTimerWithArg(1, ptr, ConjurerMeteorShower);
            break;
        }
        Delete(ptr);
        break;
    }
}

void NatureForceHandler(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), target = ToInt(GetObjectZ(ptr));

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(target) && count)
        {
            if (DistanceUnitToUnit(ptr, target) > 46.0)
            {
                MoveObject(ptr, GetObjectX(ptr) + UnitRatioX(target, ptr, 19.0), GetObjectY(ptr) + UnitRatioY(target, ptr, 19.0));
                MoveObject(ptr + 1, GetObjectX(ptr), GetObjectY(ptr));
                LookWithAngle(ptr, count - 1);
                FrameTimerWithArg(1, ptr, NatureForceHandler);
                break;
            }
            else
            {
                Damage(target, owner, 140, 14);
                GreenExplosion(GetObjectX(ptr), GetObjectY(ptr));
            }
        }
        Delete(ptr);
        break;
    }
}

int FakeForceOfNature(int target)
{
    int unit = CreateObjectAt("AirshipBasketShadow", GetObjectX(target), GetObjectY(target));
    int ptr = GetMemory(0x750710);

    if (ptr)
        SetMemory(ptr + 0x04, 706);
    return unit;
}

void SummonForceOfNature(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(ptr), GetObjectY(ptr));
        SetOwner(owner, FakeForceOfNature(ptr) - 1);
        Raise(unit, target);
        LookWithAngle(unit, 90);
        FrameTimerWithArg(1, unit, NatureForceHandler);
    }
    Delete(ptr);
    Delete(ptr + 1);
}

int WarriorBossSkills(int index)
{
    int skills[] = {WarriorSkill1, WarriorSkill2, WarriorSkill3, WarriorSkill4};

    return skills[index];
}

void WarriorSkill1(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(target) + UnitRatioX(target, owner, 31.0), GetObjectY(target) + UnitRatioY(target, owner, 31.0));
        if (IsVisibleTo(unit, target))
        {
            Effect("SMOKE_BLAST", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
            GreenSparkFx(GetObjectX(owner), GetObjectY(owner));
            MoveObject(owner, GetObjectX(unit), GetObjectY(unit));
            LookAtObject(owner, target);
            HitLocation(owner, GetObjectX(owner), GetObjectY(owner));
            GreenSparkFx(GetObjectX(owner), GetObjectY(owner));
        }
    }
    Delete(unit);
    Delete(ptr);
}

void WarriorSkill2(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));

        Raise(unit, UnitRatioX(target, owner, 26.0));
        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit) - UnitRatioX(target, owner, 52.0), GetObjectY(unit) - UnitRatioY(target, owner, 52.0)), UnitRatioY(target, owner, 26.0));
        SetOwner(owner, unit);
        LookWithAngle(unit, 18);
        FrameTimerWithArg(1, unit, WarBossLaiserShot);
    }
    Delete(ptr);
}

void WarriorSkill3(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        LookAtObject(owner, target);
        FrameTimerWithArg(1, WarSummonBombSpider(owner), BombSpiderCheckLifeTime);
        FrameTimerWithArg(1, WarSummonBombSpider(owner), BombSpiderCheckLifeTime);
    }
    Delete(ptr);
}

void WarriorSkill4(int ptr)
{
    int owner = GetOwner(ptr), unit;

    if (CurrentHealth(owner))
    {
        Enchant(owner, "ENCHANT_FREEZE", 0.0);
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner));
        CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner));
        SetOwner(owner, unit);
        LookWithAngle(unit, 20);
        FrameTimerWithArg(30, unit, WarBossShurikenRing);
    }
    Delete(ptr);
}

int WizardBossSkills(int index)
{
    int wizardSkills[] = {WizardSkill1, WizardSkill2, WizardSkill3, WizardSkill4};

    return wizardSkills[index];
}

void WizardSkill1(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        Effect("SENTRY_RAY", GetObjectX(owner), GetObjectY(owner), GetObjectX(target), GetObjectY(target));
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(target), GetObjectY(target));
        SetOwner(owner, unit);
        FrameTimerWithArg(5, unit, WizThunderStormTarget);
    }
    Delete(ptr);
}

void WizardSkill2(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit, i;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        Enchant(owner, "ENCHANT_INVULNERABLE", 2.0);
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
        LookAtObject(unit, target);
        for (i = 0 ; i < 18 ; i ++)
        {
            SetOwner(owner, CreateObjectAt("Fireball", GetObjectX(owner) + MathSine(i * 20 + 90, 18.0), GetObjectY(owner) + MathSine(i * 20, 18.0)));
            LookAtObject(unit + i + 1, owner);
            LookWithAngle(unit + i + 1, GetDirection(unit + i + 1) + 128);
            PushObject(unit + i + 1, 31.0, GetObjectX(owner), GetObjectY(owner));
        }
    }
    Delete(ptr);
}

void WizardSkill3(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit, i;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(target), GetObjectY(target));
        SetOwner(owner, unit);
        LookWithAngle(unit, 37);
        Raise(unit, 36);
        for (i = 1 ; i <= 36 ; i ++)
        {
            LookWithAngle(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(target) + MathSine(i * 10 + 90, 200.0), GetObjectY(target) + MathSine(i * 10, 200.0)), i);
            Enchant(unit + i, "ENCHANT_PROTECT_FROM_FIRE", 0.0);
        }
        FrameTimerWithArg(1, unit, WizFireRing);
    }
    Delete(ptr);
}

void WizardSkill4(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit, i;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(ptr), GetObjectY(ptr));
        FrameTimerWithArg(1, CreateObject("InvisibleLightBlueLow", 12), WizSummonFx);
        FrameTimerWithArg(2, CreateObject("InvisibleLightBlueLow", 13), WizSummonFx);
        FrameTimerWithArg(3, CreateObject("InvisibleLightBlueLow", 14), WizSummonFx);
        FrameTimerWithArg(4, CreateObject("InvisibleLightBlueLow", 15), WizSummonFx);
        FrameTimerWithArg(5, CreateObject("InvisibleLightBlueLow", 16), WizSummonFx);
        FrameTimerWithArg(6, CreateObject("InvisibleLightBlueLow", 17), WizSummonFx);
        for (i = 1 ; i <= 6 ; i ++)
        {
            SetOwner(owner, unit + i);
            Raise(unit + i, target);
        }
        Delete(unit);
    }
    Delete(ptr);
}

int ConjurerBossSkills(int index)
{
    int skills[] = {ConSkill1, ConSkill2, ConSkill3, ConSkill4, ConSkill5};

    return skills[index];
}

void ConSkill1(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;
    float vectX, vectY;

    if (CurrentHealth(owner))
    {
        vectX = UnitRatioX(target, owner, 23.0);
        vectY = UnitRatioY(target, owner, 23.0);
        unit = SpawnSoulStone(owner, GetObjectX(owner) + vectX, GetObjectY(owner) + vectY);
        SpawnSoulStone(owner, GetObjectX(unit) - (vectY * 0.3), GetObjectY(unit) + (vectX * 0.3));
        SpawnSoulStone(owner, GetObjectX(unit) + (vectY * 0.3), GetObjectY(unit) - (vectX * 0.3));
    }
    Delete(ptr);
}

void ConSkill2(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit, i;
    float xVect, yVect;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        xVect = UnitRatioX(target, owner, 20.0);
        yVect = UnitRatioY(target, owner, 20.0);
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner) + xVect, GetObjectY(owner) + yVect);
        for (i = 0 ; i < 14 ; i ++)
        {
            MoveObject(unit, GetObjectX(unit) + xVect, GetObjectY(unit) + yVect);
            if (IsVisibleTo(unit, owner))
            {
                ConjurerSummonFist(unit, owner);
            }
            else
                break;
        }
        Enchant(owner, "ENCHANT_FREEZE", 1.8);
        Enchant(owner, "ENCHANT_INVULNERABLE", 1.8);
    }
    Delete(ptr);
}

void ConSkill3(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner));
        SetOwner(owner, unit);
        LookAtObject(unit, target);
        Raise(unit, 32);
        FrameTimerWithArg(1, unit, ConjurerWindmillShot);
    }
    Delete(ptr);
}

void ConSkill4(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(target), GetObjectY(target));
        SetOwner(owner, unit);
        LookWithAngle(unit, 64);
        FrameTimerWithArg(1, unit, ConjurerMeteorShower);
    }
    Delete(ptr);
}

void ConSkill5(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner) + UnitRatioX(target, owner, 19.0), GetObjectY(owner) + UnitRatioY(target, owner, 19.0));
        SetOwner(owner, CreateObjectAt("ForceOfNatureCharge", GetObjectX(unit), GetObjectY(unit)) - 1);
        Raise(unit, ToFloat(target));
        FrameTimerWithArg(30, unit, SummonForceOfNature);
    }
    Delete(ptr);
}

void BossConjurerHurt()
{
    SayRemainHealth(self);
}

void BossConjurerDeath()
{
    UniBroadcast("축하합니다! 소환술사 보스를 격추시켰습니다");
    UniPrintToAll("보스 소환술사가 격추되었습니다");
}

void BossConjurerDetect()
{
    int unit;

    if (!HasEnchant(self, "ENCHANT_PROTECT_FROM_MAGIC"))
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(self), GetObjectY(self));
        SetOwner(self, unit);
        Raise(unit, GetCaller());
        FrameTimerWithArg(1, unit, ConjurerBossSkills(Random(0, 4)));
        Enchant(self, "ENCHANT_PROTECT_FROM_MAGIC", 2.0);
    }
    CheckResetSight(GetTrigger(), 20);
}

void BossConjurerSetup(int unit)
{
    int ptr = UnitToPtr(unit);

    if (CurrentHealth(unit))
    {
        SetCallback(unit, 3, BossConjurerDetect);
        SetCallback(unit, 5, BossConjurerDeath);
        SetCallback(unit, 7, BossConjurerHurt);
        if (ptr)
            SetMemory(ptr + 0x0c, GetMemory(ptr + 0x0c) ^ 0x200); //TODO: Immune poison
    }
}

void InitBossMonsters()
{
    BossWarriorSetup(Object("WarriorBoss"));
    BossWizardSetup(Object("WizardBoss"));
    BossConjurerSetup(Object("ConjurerBoss"));
}

void ScanEnd(int count)
{
    ForestWizard(Object("ForestGuardian"));
    FrameTimer(3, LoopSearchIndex);
    FrameTimer(1, InitBossMonsters);
    UniPrintToAll("방금 " + IntToString(count) + " 개의 유닛이 처리 완료되었습니다");
}

int MobGeneratorSearch(int cur)
{
    if (GetUnitThingID(cur) == 2622)
    {
        ObjectOff(cur);
        Enchant(cur, "ENCHANT_FREEZE", 0.0);
        SetUnitMaxHealth(cur, 600);
        FrameTimerWithArg(1, cur, GuardTowerLoop);
        return 1;
    }
    return 0;
}

int NpcUnitSearch(int cur)
{
    if (GetUnitThingID(cur) == 1343)
    {
        SetCallback(cur, 3, NPCHandlerOnDetect);
        SetCallback(cur, 5, NPCHandlerOnDeath);
        SetCallback(cur, 7, NPCHandlerOnHurt);
        SetCallback(cur, 9, NPCHandlerOnCollide);
        SetCallback(cur, 13, NPCHandlerOnLostEnemy);
        PowerInventoryNPC(cur);
        RetreatLevel(cur, 0.0);
        ResumeLevel(cur, 1.0);
        AggressionLevel(cur, 1.0);
        return 1;
    }
    return 0;
}

int MonsterMarkerScan(int cur)
{
    int unit;

    if (GetUnitThingID(cur) == 2675)
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(cur), GetObjectY(cur));
        Delete(cur);
        CallFunctionWithArgInt(FieldMonsterFunctions(Random(0, 37)), unit);
        Delete(unit);
        return 1;
    }
    return 0;
}

int RewardMarkerScan(int cur)
{
    int unit;

    if (GetUnitThingID(cur) == 2672)
    {
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(cur), GetObjectY(cur));
        Delete(cur);
        SpawnRandomItem(unit);
        Delete(unit);
        return 1;
    }
    return 0;
}

void InitSearchBefore(int cur)
{
    int i, count;

    for (i = 0 ; i < 20 ; i ++)
    {
        if (cur < LastUnit)
        {
            count += MobGeneratorSearch(cur + (i * 2));
            count += NpcUnitSearch(cur + (i * 2));
            count += MonsterMarkerScan(cur + (i * 2));
            count += RewardMarkerScan(cur + (i * 2));
        }
        else
        {
            ScanEnd(count);
            return;
        }
    }
    FrameTimerWithArg(1, cur + (i * 2), InitSearchBefore);
}

void StartSearch(int cur)
{
    if (cur)
        InitSearchBefore(cur);
}

void DelayGiveItem(int ptr)
{
	int owner = GetOwner(ptr), item = ToInt(GetObjectZ(ptr));

	if (CurrentHealth(owner) && IsObjectOn(item))
		Pickup(owner, item);
	Delete(ptr);
}

void DelayPickup(int owner, int item)
{
	int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner));

	Raise(unit, item);
	SetOwner(owner, unit);
	FrameTimerWithArg(1, unit, DelayGiveItem);
}

void SetGoldAmount(int unit, int amount)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
	    SetMemory(GetMemory(ptr + 0x2b4), amount);
}

int CheckMaterial(int checkValue)
{
    int i;

    for (i = 0 ; i < 5 ; i ++)
    {
        if (checkValue ^ MaterialList(i + 1))
            continue;
        else
            return 1;
    }
    return 0;
}

void SetNpcArmorProperties(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        if (CheckMaterial(GetMemory(GetMemory(ptr + 0x2b4) + 8)))
            SetMemory(GetMemory(ptr + 0x2b4) + 8, ArmorEffect(Random(1, 5) * 4));
    }
}

string getMagicalStaffs(int num)
{
    string table[] = {
        "DeathRayWand", "LesserFireballWand", "SulphorousFlareWand", "InfinitePainWand", "FireStormWand",
        "ForceWand"
    };
    return table[num];
}

string getPotions(int num)
{
    string potions[] = {
        "RedPotion", "BluePotion", "CurePoisonPotion", "VampirismPotion", "HastePotion",
        "ShockProtectPotion", "YellowPotion", "WhitePotion", "BlackPotion", "ShieldPotion",
        "FireProtectPotion", "PoisonProtectPotion", "InvisibilityPotion", "Mushroom"
    };
    return potions[num];
}

string getWeapons(int num)
{
    string weapons[] = {
        "WarHammer", "GreatSword", "Sword", "MorningStar", "CrossBow",
        "Bow", "Quiver", "FanChakram", "RoundChakram", "Longsword", "StaffWooden",
        "BattleAxe", "OgreAxe"
    };
    return weapons[num];
}

string getArmors(int num)
{
    string table[] = {
        "OrnateHelm", "Breastplate", "PlateArms", "PlateBoots", "PlateLeggings",
    "ChainCoif", "ChainLeggings", "LeatherArmbands", "LeatherArmor", "LeatherArmoredBoots", "LeatherBoots",
    "LeatherHelm", "LeatherLeggings", "WizardHelm", "WizardRobe", "MedievalCloak", "MedievalPants", "MedievalShirt" };

    return table[num];
}

string getTreasure(int num)
{
    string table[] = {"QuestGoldChest", "QuestGoldPile", "Gold"};

    return table[num];
}

string GetGerm(int num)
{
    string table[] = {"Ruby", "Ruby", "Ruby", "Diamond", "Emerald", "Emerald"};

    return table[num];
}

int SpawnRandomItem(int target)
{
    int itemlist[] = {HotPotionDrop, PotionDrop, StaffDrop, NormalWeaponDrop, NormalArmorDrop,
        GermDrop, PowerWeaponDrop, PowerArmorDrop, MoneyDrop};
    
    return CallFunctionWithArgInt(itemlist[Random(0, 8)], target);
}

int HotPotionDrop(int target)
{
    return CreateObjectAt("RedPotion", GetObjectX(target), GetObjectY(target));
}

int PotionDrop(int target)
{
    int unit = CreateObjectAt(getPotions(Random(0, 13)), GetObjectX(target), GetObjectY(target));

    return CheckPotionThingID(unit);
}

int StaffDrop(int target)
{
    return CreateObjectAt(getMagicalStaffs(Random(0, 5)), GetObjectX(target), GetObjectY(target));
}

int NormalWeaponDrop(int target)
{
    return CreateObjectAt(getWeapons(Random(0, 12)), GetObjectX(target), GetObjectY(target));
}

int NormalArmorDrop(int target)
{
    return CreateObjectAt(getArmors(Random(0, 17)), GetObjectX(target), GetObjectY(target));
}

int GermDrop(int target)
{
    return CreateObjectAt(GetGerm(Random(0, 5)), GetObjectX(target), GetObjectY(target));
}

int PowerWeaponDrop(int target)
{
    int unit = CreateObjectAt(getWeapons(Random(0, 12)), GetObjectX(target), GetObjectY(target));

    SetWeaponProperties(unit, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    return unit;
}

int PowerArmorDrop(int target)
{
    int unit = CreateObjectAt(getArmors(Random(0, 17)), GetObjectX(target), GetObjectY(target));

    SetArmorProperties(unit, Random(0, 5), Random(0, 5), Random(0, 20), Random(0, 20));
    return unit;
}

int MoneyDrop(int target)
{
    int unit = CreateObjectAt(getTreasure(Random(0, 2)), GetObjectX(target), GetObjectY(target));

    SetGoldAmount(unit, Random(1000, 6000));
    return unit;
}

int DummyUnitCreate(string name, float xProfile, float yProfile)
{
    int unit = CreateObjectAt(name, xProfile, yProfile);

    if (unit)
    {
        ObjectOff(unit);
        Damage(unit, 0, CurrentHealth(unit) + 1, -1);
        Frozen(unit, 1);
    }
    return unit;
}

void FastRunTouched()
{
    int owner = GetOwner(GetTrigger() + 1);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 100, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.2);
    }
}

void WindboosterHandler(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), unit;

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner) && count)
        {
            unit = DummyUnitCreate("CarnivorousPlant", GetObjectX(owner) - GetObjectZ(ptr), GetObjectY(owner) - GetObjectZ(ptr + 1));
            SetOwner(owner, CreateObjectAt("PlayerWaypoint", GetObjectX(unit), GetObjectY(unit)));
            DeleteObjectTimer(unit + 1, 15);
            SetCallback(unit, 9, FastRunTouched);
            DeleteObjectTimer(unit, 1);
            GreenSparkFx(GetObjectX(unit), GetObjectY(unit));
            MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
            AudioEvent("SentryRayHitWall", 1);
            LookWithAngle(ptr, --count);
            FrameTimerWithArg(1, ptr, WindboosterHandler);
            break;
        }
        Delete(ptr);
        Delete(ptr + 1);
        break;
    }
}

void TouchedWallKite()
{
    int owner = GetOwner(GetTrigger() + 1);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
    {
        Damage(other, owner, 150, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.2);
    }
}

void WallKiteHandler(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr), unit;

    while (IsObjectOn(ptr))
    {
        if (CurrentHealth(owner) && count)
        {
            if (IsVisibleTo(ptr + 1, ptr + 2))
            {
                MoveObject(ptr, GetObjectX(ptr) + GetObjectZ(ptr), GetObjectY(ptr) + GetObjectZ(ptr + 1));
                MoveObject(ptr + 2, GetObjectX(ptr + 2) + GetObjectZ(ptr), GetObjectY(ptr + 2) + GetObjectZ(ptr + 1));
                unit = DummyUnitCreate("CarnivorousPlant", GetObjectX(ptr), GetObjectY(ptr));
                SetOwner(owner, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)));
                CastSpellObjectLocation("SPELL_WALL", unit + 1, GetObjectX(ptr) + (GetObjectZ(ptr) * 2.0), GetObjectY(ptr) + (GetObjectZ(ptr + 1) * 2.0));
                GreenSparkFx(GetObjectX(ptr), GetObjectY(ptr));
                SetCallback(unit, 9, TouchedWallKite);
                DeleteObjectTimer(unit, 1);
                DeleteObjectTimer(unit + 1, 3);
                LookWithAngle(ptr, --count);
                FrameTimerWithArg(1, ptr, WallKiteHandler);
                break;
            }
        }
        Delete(ptr);
        Delete(ptr + 1);
        break;
    }
}

void PlayerOnCastWindBooster(int owner)
{
    int unit;
    
    if (!HasEnchant(owner, "ENCHANT_AFRAID"))
    {
        Enchant(owner, "ENCHANT_AFRAID", 13.0);
        unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner));

        Raise(unit, UnitAngleCos(owner, 12.0));
        Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)), UnitAngleSin(owner, 12.0));
        SetOwner(owner, unit);
        LookWithAngle(unit, 15);
        FrameTimerWithArg(1, unit, WindboosterHandler);
        MoveWaypoint(1, GetObjectX(owner), GetObjectY(owner));
        AudioEvent("SummonAbort", 1);
    }
}

int GetPlayerScrIndex(int sPlrUnit)
{
    int i;

    for (i = 9 ; i >= 0 ; i -= 1)
    {
        if (!(player[i] ^ sPlrUnit))
            return i;
    }
    return -1;
}

void PlayerOnCastWallKite(int cur)
{
    int owner = GetOwner(cur), unit;
    int plr = GetPlayerScrIndex(owner);

    if (CurrentHealth(owner) && plr + 1)
    {
        if (HasEnchant(owner, "ENCHANT_ANTI_MAGIC"))
            1;
        else if (CheckPlayerSkillFlag1(plr))
        {
            Enchant(owner, "ENCHANT_ANTI_MAGIC", 12.0);
            unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner) + UnitAngleCos(owner, 20.0), GetObjectY(owner) + UnitAngleSin(owner, 20.0));
            Raise(unit, UnitAngleCos(owner, 23.0));
            Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)), UnitAngleSin(owner, 23.0));
            CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit) - UnitAngleCos(owner, 50.0), GetObjectY(unit) - UnitAngleSin(owner, 50.0));
            SetOwner(owner, unit);
            LookWithAngle(unit, 18);
            FrameTimerWithArg(1, unit, WallKiteHandler);
            Delete(cur);
        }
    }
}

void removeSecretPassageWalls()
{
    ObjectOff(self);
    WallOpen(Wall(213, 209));
    WallOpen(Wall(214, 210));
    WallOpen(Wall(215, 211));
    UniPrint(other, "벽이 열립니다");
}

void testFunction()
{
    int i;

    ObjectOff(self);
    for (i = 0 ; i < 6 ; i ++)
        WallOpen(Wall(80 - i, 226 + i));
    UniPrint(other, "주변 어디선가 벽이 열렸습니다");
}

void WakeWizardBoss()
{
    int wizBossUnit;

    if (MaxHealth(self) && CurrentHealth(other))
    {
        wizBossUnit = GetOwner(GetTrigger() + 1);
        ObjectOn(wizBossUnit);
        MoveObject(wizBossUnit, GetWaypointX(183), GetWaypointY(183));
        LookAtObject(wizBossUnit, self);
        Delete(GetTrigger() + 2);
        Delete(GetTrigger() + 1);
        Delete(self);
    }
}

void PutWizardBossWakeSwitch(int location, int wizBossUnit)
{
    int unit = CreateObject("WeirdlingBeast", location);

    SetOwner(wizBossUnit, CreateObject("BlueSummons", location));
    UnitNoCollide(CreateObject("LargeBlueFlame", location));
    SetUnitMaxHealth(unit, 30);
    Damage(unit, 0, 999, -1);
    SetCallback(unit, 9, WakeWizardBoss);
}

void TestUserTeleport() //RemoveMe
{
    MoveObject(other, LocationX(142), LocationY(142));
}

void InitializePlayerLastPos()
{
    int u;

    s_playerLastPositions = CreateObjectAt("TeleportGlyph4", LocationX(USER_LASTPOS_WAYID), LocationY(USER_LASTPOS_WAYID)) + 1;

    for (u = 0 ; u < 10 ; u += 1)
        ObjectOn(CreateObjectAt("TeleportGlyph4", LocationX(USER_LASTPOS_WAYID), LocationY(USER_LASTPOS_WAYID)));
    Delete(s_playerLastPositions - 1);
}

int GetPlayerLastPositionUnit(int plr)
{
    int res = s_playerLastPositions + plr;

    if (IsObjectOn(res))
        return res;
    return 0;
}

void DelayInitRun()
{
    MusicEvent();
    RegistSignMessage(Object("MapPick1"), "경고!- 출입 제한구역. 이 아래층은 위험합니다");
    RegistSignMessage(Object("MapPick2"), "필드로 나가는 포탈입니다. 포탈위에 서서 가만히 있으면 잠시 후 이동됩니다");
    RegistSignMessage(Object("MapPick3"), "이 윗층부터 괴수들이 득실 거리는 필드입니다");
    RegistSignMessage(Object("MapPick4"), "공간이동 목걸이를 구입할 수 있는 우물이에요. 공간이동의 목걸이는 개당 1000골드가 필요합니다");
    RegistSignMessage(Object("picket1"), "문 안에 화이어볼 함정 있음. 미리 알려드리는 것이니 괜히 걸려놓고 제작자 탓 ㄴㄴ");
    RegistSignMessage(Object("picket2"), "이 스위치는 안 누르는 것이 정신건강에 도움이 될 것입니다");
    Frozen(Object("MainNPC"), 1);
    Frozen(Object("DummyWarrior"), 1);
    Frozen(Object("DummyWizard"), 1);
    Frozen(Object("DummyConjurer"), 1);
    WarSkillMarket(184);
    OblivionStaffShopPlace(185);
    PlacingGuideForTeleportToLastPosition(193, 32);
    MakeCoopTeam();
    CreateObjectAt("TargetBarrel1", LocationX(190), LocationY(190));

    TeleportSafeZoneAmulet(LocationX(191), LocationY(191)); //!RemoveMe! // testfunctions

    PlacingSavePosGuide(194);
    PlacingSavePosGuide(196);
    InitPlaceFistTraps();
    PlacingSavePosGuide(221);
}

void MapExit()
{
    RemoveCoopTeamMode();
    ResetPlayerHandlerWhenExitMap();
    MusicEvent();
}

void MapInitialize()
{
    LastUnit = CreateObject("InvisibleLightBlueLow", 1);
    Delete(LastUnit);

    InitializePlayerLastPos();
    LookWithAngle(Object("DecoSkull"), 160);

    //DelayRun
    FrameTimer(1, DelayInitRun);
    FrameTimer(1, FieldTeleportSetup);
    FrameTimerWithArg(30, Object("bossElevator"), DisableObject);
    FrameTimerWithArg(1, Object("FirstSearching"), StartSearch);
    
    //LoopRun
    FrameTimer(30, PreservePlayerLoop);
}

void GreenSparkFxAt(float sX, float sY)
{
    int fxUnit = CreateObjectAt("MonsterGenerator", sX, sY);

    Damage(fxUnit, 0, 1, 14);
    Delete(fxUnit);
}

void MobGenClassMissileCollide()
{
    int owner = GetOwner(self);

    while (1)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 10, 14);
            Effect("DAMAGE_POOF", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

int MobGenClassMissile(int sTower)
{
    int mis = CreateObjectAt("Pixie", GetObjectX(sTower), GetObjectY(sTower));
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(mis, MobGenClassMissileCollide);
    SetOwner(sTower, mis);
    return mis;
}

void GuardTowerShot(int sTower, int sTarget)
{
    if (CurrentHealth(sTarget))
    {
        PushObjectTo(MobGenClassMissile(sTower), UnitRatioX(sTarget, sTower, 32.0), UnitRatioY(sTarget, sTower, 32.0));
    }
}

int CheckVisible(int sUnit1, int sUnit2)
{
    return (IsVisibleTo(sUnit1, sUnit2) || IsVisibleTo(sUnit2, sUnit1));
}

void GuardTowerLoop(int sTower)
{
    int i;

    if (CurrentHealth(sTower))
    {
        for (i = 9 ; i >= 0 ; i --)
        {
            if (CheckVisible(sTower, player[i]))
                GuardTowerShot(sTower, player[i]);
        }
        FrameTimerWithArg(50, sTower, GuardTowerLoop);
    }
}

int PlayerAwardNewSkill(int sPlr)
{
    if (CheckPlayerSkillFlag1(sPlr))
    {
        if (!CheckPlayerSkillFlag2(sPlr))
        {
            PlayerSetSkillFlag2(sPlr);
            return 2;
        }
    }
    else
    {
        PlayerSetSkillFlag1(sPlr);
        return 1;
    }
    return 0;
}

void SkillShopTrade()
{
    int plr = CheckPlayer(), awardSkill;

    if (HasEnchant(other, "ENCHANT_CHARMING"))
    {
        EnchantOff(other, "ENCHANT_CHARMING");
        if (plr + 1)
        {
            if (GetGold(other) >= 6000)
            {
                ChangeGold(other, -6000);
                awardSkill = PlayerAwardNewSkill(plr);
                if (awardSkill == 1)
                    UniPrint(other, "첫번째 스킬을 배웠습니다! 작살을 시전하면 새로운 스킬이 발동됩니다");
                else if (awardSkill == 2)
                    UniPrint(other, "두번째 스킬을 배웠습니다! 조심스럽게 걷기를 시전하면 새로운 스킬이 발동됩니다");
                else if (!awardSkill)
                {
                    ChangeGold(other, 6000);
                    UniPrint(other, "이미 모든 능력을 배우셨습니다 (금화 환수완료)");
                }
            }
            else
                UniPrint(other, "거래가 취소되었습니다- 잔액이 부족합니다");
        }
    }
    else
    {
        UniPrint(other, "전사의 능력을 강화하시겠어요? 1회당 6천 골드가 필요하며 총 2회까지 업그레이드 가능합니다");
        UniPrint(other, "거래를 계속하려면 더블클릭 하세요");
        Enchant(other, "ENCHANT_CHARMING", 0.3);
    }
}

void OblivionStaffShopTrade()
{
    if (HasEnchant(other, "ENCHANT_CHARMING"))
    {
        EnchantOff(other, "ENCHANT_CHARMING");
        if (GetGold(other) >= 50000)
        {
            DelayPickup(GetCaller(), SummonOblivionStaff(GetObjectX(other), GetObjectY(other)));
            ChangeGold(other, -50000);
            UniPrint(other, "거래성공! 마법 미사일 지팡이가 수여됩니다");
        }
        else
            UniPrint(other, "거래가 취소되었습니다- 잔액이 부족합니다");
    }
    else
    {
        UniPrint(other, "마법 미사일 지팡이를 구입하시겠어요? 이 작업은 5만 골드를 요구합니다.");
        UniPrint(other, "거래를 계속하려면 더블클릭 해주세요");
        Enchant(other, "ENCHANT_CHARMING", 0.3);
    }
}

int WarSkillMarket(int sLocation)
{
    int shop = DummyUnitCreate("Horrendous", GetWaypointX(sLocation), GetWaypointY(sLocation));

    SetDialog(shop, "aa", SkillShopTrade, SkillShopTrade);
    return shop;
}

int OblivionStaffShopPlace(int sLocation)
{
    int shop = DummyUnitCreate("WizardWhite", GetWaypointX(sLocation), GetWaypointY(sLocation));

    SetDialog(shop, "aa", OblivionStaffShopTrade, OblivionStaffShopTrade);
    return shop;
}

void BlueMissileCollide()
{
    int owner = GetOwner(self);

    while (1)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            GreenSparkFxAt(GetObjectX(other), GetObjectY(other));
            Damage(other, owner, 185, 14);
            Enchant(other, "ENCHANT_FREEZE", 1.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
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
    int cTime = GetUnit1C(self);

    if (MathAbs(cFps - cTime) < 20)
        return;
    if (CurrentHealth(other))
    {
        SetUnit1C(self, cFps);
        PushObject(BlueOrbSummon(other), 20.0, GetObjectX(other), GetObjectY(other));
        PlaySoundAround(other, 221);
    }
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

void TeleportSafeZoneAmuletUse()
{
    if (CurrentHealth(other))
    {
        FrameTimerWithArg(1, GetCaller(), PlayerOnCastHomePortal);
        Delete(self);
        UniPrint(other, "공간이동의 목걸이를 사용하였습니다");
    }
}

int TeleportSafeZoneAmulet(float sX, float sY)
{
    int aml = CreateObjectAt("AmuletofManipulation", sX, sY);
    
    SetUnitCallbackOnUseItem(aml, TeleportSafeZoneAmuletUse);
    return aml;
}

void BuyTeleportAmulet()
{
    if (GetGold(other) >= 1000)
    {
        ChangeGold(other, -1000);
        TeleportSafeZoneAmulet(GetObjectX(other), GetObjectY(other));
        UniPrint(other, "공간이동의 목걸이를 구입하셨습니다 (-1000골드 차감)");
    }
}

int SpawnLichSingle(int location)
{
    int lich = CreateObjectAt("Lich", LocationX(location), LocationY(location));

    SetUnitMaxHealth(lich, 325);
    UniChatMessage(lich, "뭐라구? 마법스파이!!", 150);
    return lich;
}

void SpawnThreeLichs()
{
    int lich = SpawnLichSingle(187);

    SpawnLichSingle(188);
    SpawnLichSingle(189);
}

void RemoveExtendAreaWalls()
{
    ObjectOff(self);
    WallGroupOpen(0);
    FrameTimer(1, SpawnThreeLichs);
}

void RemoveExtendAreaWalls2()
{
    ObjectOff(self);
    WallGroupOpen(2);
}

void ClearOrbEffect(int _fxUnit)
{
    Delete(_fxUnit);
}

void ElasticOrb()
{
    int fxUnit = CreateObjectAt("MagicEnergy", GetObjectX(self), GetObjectY(self));

    PlaySoundAround(fxUnit, 928);
    PushObject(other, 60.0, GetObjectX(self) + RandomFloat(-30.0, 30.0), GetObjectY(self) + RandomFloat(-30.0, 30.0));
    FrameTimerWithArg(4, fxUnit, ClearOrbEffect);
}

void SouthFireShotOn()
{
    ObjectGroupOn(3);
}

void SouthFireShotOff()
{
    ObjectGroupOff(3);
}

void GargoyleMoveToTarget(int tempUnit)
{
    int owner = GetOwner(tempUnit);
    int target = ToInt(GetObjectZ(tempUnit));

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        AggressionLevel(owner, 1.0);
        CreatureFollow(owner, target);
        UniChatMessage(owner, "낄낄낄...", 120);
    }
    Delete(tempUnit);
}

void SpawnGargoyleSingle(int spawnUnit, int target)
{
    int unit = CreateObjectAt("EvilCherub", GetObjectX(spawnUnit), GetObjectY(spawnUnit));

    AggressionLevel(unit, 1.0);
    SetUnitMaxHealth(unit, 135);
    SetCallback(unit, 5, FieldMonsterDeath);
    if (CurrentHealth(target))
    {
        FrameTimerWithArg(1, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(spawnUnit), GetObjectY(spawnUnit)), GargoyleMoveToTarget);
        SetOwner(unit, unit + 1);
        Raise(unit + 1, target);
    }
}

void GargoyleSpawn(int spawnUnit)
{
    int count = GetDirection(spawnUnit);

    while (IsObjectOn(spawnUnit))
    {
        if (count)
        {
            int target = ToInt(GetObjectZ(spawnUnit));

            SpawnGargoyleSingle(spawnUnit, target);
            LookWithAngle(spawnUnit, count - 1);
            FrameTimerWithArg(1, spawnUnit, GargoyleSpawn);
            break;
        }
        Delete(spawnUnit);
        break;
    }
}

void StartGargoyleAttack()
{
    int spawnMarker = CreateObjectAt("InvisibleLightBlueHigh", LocationX(186), LocationY(186));

    LookWithAngle(spawnMarker, 64);
    Raise(spawnMarker, GetCaller());
    FrameTimerWithArg(3, spawnMarker, GargoyleSpawn);
    ObjectOff(self);
}

void TeleportToLastPositionDesc()
{
    UniPrint(other, "최근 위치로 빠르게 이동하고 싶다면 \"예\" 버튼을 누르십시오");
    PlaySoundAround(self, 695);
    TellStoryUnitName("oo", "War07A.scr:BrightBladesAngry01", "최근 위치로 이동?");
}

void TeleportToLastPositionTrade()
{
    if (GetAnswer(self) ^ 1)
        return;
    int plr = CheckPlayer();

    if (plr < 0)
        return;

    int posUnit = GetPlayerLastPositionUnit(plr);

    if (posUnit)
    {
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        MoveObject(other, GetObjectX(posUnit), GetObjectY(posUnit));
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        UniPrint(other, "최근에 저장된 위치로 이동 되셨습니다~");
    }
    else
    {
        UniPrint(other, "해당 유저의 위치유닛에 뭔가 문제가 발생했습니다. 관리자에게 문의해주세요");
    }
    
}

int PlacingGuideForTeleportToLastPosition(int locationId, int lookAngle)
{
    int guide = DummyUnitCreate("LichLord", LocationX(locationId), LocationY(locationId));

    SetDialog(guide, "YESNO", TeleportToLastPositionDesc, TeleportToLastPositionTrade);
    if (lookAngle < 0)
        lookAngle = 64;
    LookWithAngle(guide, lookAngle);
    return guide;
}

void PlayerSaveCurrentPosition()
{
    if (CurrentHealth(other))
    {
        int plr = CheckPlayer();

        if (plr < 0)
            return;
        int posUnit = GetPlayerLastPositionUnit(plr);

        if (!posUnit)
            return;

        if (DistanceUnitToUnit(posUnit, self) > 200.0)
        {
            MoveObject(posUnit, GetObjectX(other), GetObjectY(other));
            Effect("YELLOW_SPARKS", GetObjectX(posUnit), GetObjectY(posUnit), 0.0, 0.0);
            PlaySoundAround(posUnit, 1005);
            UniPrint(other, "현재 위치로 최근위치가 변경되었습니다");
        }
    }
}

int PlacingSavePosGuide(int location)
{
    int guide = DummyUnitCreate("Bomber", LocationX(location), LocationY(location));
    int deco = CreateObjectAt("SpinningCrown", GetObjectX(guide), GetObjectY(guide));

    Frozen(deco, TRUE);
    SetDialog(guide, "NORMAL", PlayerSaveCurrentPosition, PlayerSaveCurrentPosition);
    return guide;
}

void DisableFireTrap()
{
    ObjectOff(Object("BFireTrap1"));
    ObjectOff(Object("BFireTrap11"));
}

void FireTrapTriggered()
{
    ObjectOn(Object("BFireTrap1"));
    ObjectOn(Object("BFireTrap11"));
}

void RemoveNorthWalls()
{
    ObjectOff(self);

    int u;

    for (u = 0 ; u < 6 ; u+=1)
    {
        RemoveWallAtObjectPos(195);
        TeleportLocationVector(195, -23.0, 23.0);
    }
}

void FistTrapCountdown(int trp)
{
    int count = GetDirection(trp);

    if (count)
    {
        LookWithAngle(trp, --count);

        FrameTimerWithArg(3, trp, FistTrapCountdown);
    }
}

void TriggeredFistTrap()
{
    if (CurrentHealth(other) && MaxHealth(self))
    {
        if (GetDirection(self))
            return;
        else
        {
            int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(self), GetObjectY(self));

            Delete(unit);
            CastSpellLocationLocation("SPELL_FIST", GetObjectX(self) - 2.0, GetObjectY(self), GetObjectX(self), GetObjectY(self));
            SetOwner(self, unit + 1);
            LookWithAngle(self, 30);
            FrameTimerWithArg(1, GetTrigger(), FistTrapCountdown);
        }
    }
}

int PutFistTrap(int locationId)
{
    int trp = CreateObjectAt("Wizard", LocationX(locationId), LocationY(locationId));

    Damage(trp, 0, MaxHealth(trp) + 1, -1);
    LookWithAngle(trp, 0);
    SetCallback(trp, 9, TriggeredFistTrap);
    return trp;
}

void InitPlaceFistTraps()
{
    int u;

    for (u = 197 ; u < 220 ; u += 1)
        PutFistTrap(u);
}

void RemoveBarWalls()
{
    int u;

    for (u = 0 ; u < 9 ; u+=1)
    {
        RemoveWallAtObjectPos(223);
        TeleportLocationVector(223, -23.0, 23.0);
    }
}

int SingleSummonGay(int posUnit)
{
    int gay = CreateObjectAt("WoundedApprentice", GetObjectX(posUnit), GetObjectY(posUnit));

    WoundedConjurerSubProcess(gay);
    SetCallback(gay, 5, FieldMonsterDeath);
    SetUnitScanRange(gay, 350.0);
    return gay;
}

void SummonGayEveryFrames(int posUnit)
{
    while (IsObjectOn(posUnit))
    {
        int remain = GetDirection(posUnit);

        if (remain > 0)
        {
            FrameTimerWithArg(1, posUnit, SummonGayEveryFrames);
            LookWithAngle(posUnit, --remain);
            int gayit = SingleSummonGay(posUnit);

            UniChatMessage(gayit, "게이쉑:\n오우 저기 섹시하고 건장한 성인남성이 있다~~(하트)", 120);
            PlaySoundAround(gayit, 650);
            break;
        }
        Delete(posUnit);
        break;
    }
}

void StartSummonGays(int locationId)
{
    int posUnit = CreateObjectAt(MAGIC_CASTER, LocationX(locationId), LocationY(locationId));

    LookWithAngle(posUnit, 24);
    FrameTimerWithArg(3, posUnit, SummonGayEveryFrames);
}

void OpenGayRoom()
{
    ObjectOff(SELF);

    FrameTimer(60, RemoveBarWalls);
    StartSummonGays(222);
    UniPrintToAll("젠장, 하필 '그곳' 여셨습니다..");
}