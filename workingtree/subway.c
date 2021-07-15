
#include "noxscript\builtins.h"
#include "libs\callmethod.h"

#include "libs\username.h"
#include "libs\itemproperty.h"
#include "libs\typecast.h"
#include "libs\spellutil.h"
#include "libs\reaction.h"
#include "libs\observer.h"
#include "libs\unitutil.h"
#include "libs\printutil.h"
#include "libs\mathlab.h"
#include "libs\unitstruct.h"
#include "libs\playerinfo.h"

#define OTHER   -1
#define SELF    -2
#define TRUE    1
#define FALSE 0
int LastUnit, DemonMax = 20;
int player[20], PlrCre[10], AGate[8];
int LUnit[10], DLord[21];

#define SHOP_NOT_ENOUGH_GOLD    "잔액이 부족합니다"

//2180 Blue
//2182 Red

#include "libs\coopteam.h"
#include "libs\playerupdate.h"

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

int HecubahBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 6840674; arr[17] = 20; arr[19] = 300; arr[21] = 1065353216; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; arr[28] = 1108082688; 
		arr[29] = 85; arr[30] = 1092616192; arr[32] = 10; arr[33] = 18; arr[57] = 5548288; 
		arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int AirshipCaptainBinTable()
{
	int link, arr[62];

	if (!link)
	{
		arr[0] = 1936877889; arr[1] = 1131440488; arr[2] = 1635020897; arr[3] = 28265; arr[17] = 130; 
		arr[19] = 60; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1067869798; arr[26] = 4; 
		arr[28] = 1106247680; arr[29] = 33; arr[31] = 8; arr[32] = 12; arr[33] = 20; 
		arr[57] = 5547984; arr[58] = 5546320; arr[59] = 5542432; 
		link = &arr;
	}
	return link;
}

int WeirdlingBeastBinTable()
{
	int link, arr[62];

	if (!link)
	{
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811; arr[17] = 85; 
		arr[18] = 50; arr[19] = 55; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1068708659; 
		arr[26] = 4; arr[28] = 1109393408; arr[29] = 30; arr[31] = 2; arr[32] = 8; 
		arr[33] = 16; arr[57] = 5548112; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int FireSpriteBinTable()
{
	int link, arr[62];

	if (!link)
	{
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972; arr[17] = 85; arr[18] = 25; 
		arr[19] = 90; arr[21] = 1065353216; arr[23] = 65536; arr[24] = 1065353216; arr[37] = 1801545047; 
		arr[38] = 1701996870; arr[39] = 1819042146; arr[53] = 1128792064; arr[55] = 15; arr[56] = 21; 
		arr[58] = 5545472; 
		link = &arr;
	}
	return link;
}

int GoonBinTable()
{
	int link, arr[62];

	if (!link)
	{
		arr[0] = 1852796743; arr[17] = 85; arr[19] = 60; arr[21] = 1065353216; arr[23] = 32768; 
		arr[24] = 1065353216; arr[26] = 4; arr[28] = 1106247680; arr[29] = 25; arr[30] = 1092616192; 
		arr[31] = 4; arr[32] = 18; arr[33] = 24; arr[34] = 2; arr[35] = 3; 
		arr[36] = 20; arr[57] = 5548176; arr[58] = 5546608; arr[59] = 5543680; 
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

void MakeStar(int wp, float size)
{
    float posX = GetWaypointX(wp), posY = GetWaypointY(wp), fArr[10];
    int i;

    for (i = 0; i < 5; i += 1)
    {
        fArr[i * 2] = posX - MathSine(i * 72, size);
        fArr[i * 2 + 1] = posY - MathSine(i * 72 + 90, size);
    }
    Effect("SENTRY_RAY", fArr[0], fArr[1], fArr[4], fArr[5]);
    Effect("SENTRY_RAY", fArr[0], fArr[1], fArr[6], fArr[7]);
    Effect("SENTRY_RAY", fArr[2], fArr[3], fArr[6], fArr[7]);
    Effect("SENTRY_RAY", fArr[2], fArr[3], fArr[8], fArr[9]);
    Effect("SENTRY_RAY", fArr[4], fArr[5], fArr[8], fArr[9]);
}

void AutoTargetDeathRay()
{
    CastSpellObjectLocation("SPELL_DEATH_RAY", self, GetObjectX(other), GetObjectY(other));
}

void HitDeathRayHammer(int owner)
{
    int unit;

    if (CurrentHealth(owner))
    {
        MoveWaypoint(1, GetObjectX(owner) + UnitAngleCos(owner, 21.0), GetObjectY(owner) + UnitAngleSin(owner, 21.0));
        MakeStar(1, 63.0);
        unit = CreateObject("WeirdlingBeast", 1);
        UnitNoCollide(unit);
        SetOwner(owner, unit);
        LookWithAngle(unit, GetDirection(owner));
        SetCallback(unit, 3, AutoTargetDeathRay);
        DeleteObjectTimer(unit, 1);
        AudioEvent("ForceFieldCast", 1);    
    }
}

void DeathRayHammerFx(int unit)
{
	int fx, owner;

	if (IsObjectOn(unit))
	{
		fx = ToInt(GetObjectZ(unit + 1));
        owner = GetOwner(unit);
		if (CurrentHealth(owner))
		{
			if (IsObjectOn(fx))
				Delete(fx);
            else if (MaxHealth(unit) ^ CurrentHealth(unit))
            {
                if (!HasEnchant(owner, "ENCHANT_VILLAIN"))
                {
                    Enchant(owner, "ENCHANT_VILLAIN", 0.9);
                    FrameTimerWithArg(1, owner, HitDeathRayHammer);
                }
                RestoreHealth(unit, MaxHealth(unit));
            }
		}
		else
		{
			if (!IsObjectOn(fx))
			{
				MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
				MoveObject(unit + 1, GetObjectX(unit), GetObjectY(unit));
				Raise(unit + 1, ToFloat(CreateObject("BlueSummons", 1)));
				Frozen(ToInt(GetObjectZ(unit + 1)), 1);
			}
		}
		FrameTimerWithArg(1, unit, DeathRayHammerFx);
	}
	else
	{
        Delete(ToInt(GetObjectZ(unit + 1)));
		Delete(unit + 1);
	}
}

int PutOblivionStaff(int owner)
{
    int unit = 0;
    if (CurrentHealth(owner))
    {
        if (HasEnchant(owner, "ENCHANT_INVULNERABLE"))
            EnchantOff(owner, "ENCHANT_INVULNERABLE");
        if (HasEnchant(owner, "ENCHANT_SHIELD"))
            EnchantOff(owner, "ENCHANT_SHIELD");
        Enchant(owner, "ENCHANT_RUN", 0.1);
        unit = CreateObjectAt("OblivionOrb", GetObjectX(owner), GetObjectY(owner));
        SetUnitHealth(owner, CurrentHealth(owner) + 1000);
        Effect("RICOCHET", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
    }
    return unit;
}

void SpreadNucExplosion(int ptr)
{
    float size = GetObjectZ(ptr);
    int count = GetDirection(ptr), i;

    if (count)
    {
        for (i = 0 ; i < 18 ; i += 1)
            DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(ptr) + MathSine(i * 20 + 90, size), GetObjectY(ptr) + MathSine(i * 20, size)), 6);
        LookWithAngle(ptr, count - 1);
        Raise(ptr, size + 21.0);
        FrameTimerWithArg(1, ptr, SpreadNucExplosion);
    }
    else
    {
        Delete(GetOwner(ptr));
        Delete(ptr);
    }
}

void NuclearFx(int unit)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit));

    DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(ptr), GetObjectY(ptr)), 6);
    LookWithAngle(ptr, 6);
    Raise(ptr, 23.0);
    SetOwner(unit, ptr);
    FrameTimerWithArg(1, ptr, SpreadNucExplosion);
}

int BlueMeteor(int wp)
{
    int unit = CreateObject("Meteor", wp);

    Enchant(unit, "ENCHANT_FREEZE", 0.0);
    Frozen(unit, 1);
    return unit;
}

void NuclearHammerHurt()
{
    Damage(other, GetOwner(GetOwner(self)), 250, 1);
    Enchant(other, "ENCHANT_CHARMING", 0.3);
}

void HitNuclearHammer(int owner)
{
    if (CurrentHealth(owner))
    {
        MoveWaypoint(1, GetObjectX(owner), GetObjectY(owner));
        AudioEvent("PowderBarrelExplode", 1);
        NuclearFx(CreateObject("InvisibleLightBlueHigh", 1));
        SplashHandler(owner, NuclearHammerHurt, GetWaypointX(1), GetWaypointY(1), 200.0);
    }
}

void NuclearHammerFx(int unit)
{
	int fx, owner;

	if (IsObjectOn(unit))
	{
		fx = ToInt(GetObjectZ(unit + 1));
        owner = GetOwner(unit);
		if (CurrentHealth(owner))
		{
			if (IsObjectOn(fx))
				Delete(fx);
            else if (MaxHealth(unit) ^ CurrentHealth(unit))
            {
                if (!HasEnchant(owner, "ENCHANT_VILLAIN"))
                {
                    Enchant(owner, "ENCHANT_VILLAIN", 0.9);
                    FrameTimerWithArg(1, owner, HitNuclearHammer);
                }
                RestoreHealth(unit, MaxHealth(unit));
            }
		}
		else
		{
			if (!IsObjectOn(fx))
			{
				MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
				MoveObject(unit + 1, GetObjectX(unit), GetObjectY(unit));
				Raise(unit + 1, ToFloat(BlueMeteor(1)));
				Frozen(ToInt(GetObjectZ(unit + 1)), 1);
			}
		}
		FrameTimerWithArg(1, unit, NuclearHammerFx);
	}
	else
	{
        Delete(ToInt(GetObjectZ(unit + 1)));
		Delete(unit + 1);
	}
}

int SpecialItemDeathRayHammer(int wp)
{
    int unit = CreateObject("WarHammer", wp);
    CreateObject("InvisibleLightBlueHigh", wp);
    Enchant(unit, "ENCHANT_FREEZE", 0.0);
    FrameTimerWithArg(1, unit, DeathRayHammerFx);
}

int SpecialItemNuclearHammer(int wp)
{
    int unit = CreateObject("WarHammer", wp);

    CreateObject("InvisibleLightBlueHigh", wp);
    FrameTimerWithArg(1, unit, NuclearHammerFx);
}

int SelectHovath(int plr)
{
    int unit = CreateObjectAt("WizardWhite", GetObjectX(player[plr]), GetObjectY(player[plr]));
    LookWithAngle(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), plr);
    SetUnitMaxHealth(unit, 800);
    SetOwner(player[plr], unit);
    GiveCreatureToPlayer(player[plr], unit);
    return unit;
}

int ReadyPlayerChanging(int plr)
{
    if (CurrentHealth(player[plr]) && !(player[plr + 10] & 0x02))
    {
        player[plr + 10] = player[plr + 10] ^ 2;
        Enchant(player[plr], "ENCHANT_FREEZE", 0.0);
        Enchant(player[plr], "ENCHANT_INVULNERABLE", 0.0);
        InitiPlayerCamera(player[plr]);
        MoveObject(player[plr], GetWaypointX(52), GetWaypointY(52));
        return 1;
    }
    return 0;
}

void BuyOblivionStaff()
{
    if (HasEnchant(other, "ENCHANT_AFRAID"))
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (GetGold(other) >= 60000)
        {
            PutOblivionStaff(other);
            ChangeGold(other, -60000);
            UniPrint(OTHER, "결제완료, 망각의 지팡이는 당신 아래에 있습니다");
        }
        else
            UniPrint(OTHER, SHOP_NOT_ENOUGH_GOLD);
    }
    else
    {
        Enchant(other, "ENCHANT_AFRAID", 0.4);
        UniPrint(OTHER, "망각의 지팡이를 구입합니다, 보너스 효과: 캐릭터의 체력이 1000 으로 설정됩니다. [6만원 필요] 계속하려면 더블클릭 하세요");
    }
}

void BuyDeathRayHammer()
{
    if (HasEnchant(other, "ENCHANT_AFRAID"))
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (GetGold(other) >= 80000)
        {
            MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
            SpecialItemDeathRayHammer(1);
            ChangeGold(other, -80000);
            UniPrint(OTHER, "거래완료, 데스레이 워해머가 당신 아래에 있습니다");
        }
        else
            UniPrint(OTHER, SHOP_NOT_ENOUGH_GOLD);
    }
    else
    {
        Enchant(other, "ENCHANT_AFRAID", 0.4);
        UniPrint(OTHER, "데스레이 워해머를 구입합니다, 이 작업은 8만원이 필요합니다. 계속하려면 더블클릭 하세요");
    }
}

void BuyNuclearHammer()
{
    if (HasEnchant(other, "ENCHANT_AFRAID"))
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (GetGold(other) >= 75000)
        {
            MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
            SpecialItemNuclearHammer(1);
            ChangeGold(other, -75000);
            UniPrint(OTHER, "거래완료, 핵폭탄 해머가 당신 아래에 있습니다");
        }
        else
            UniPrint(OTHER, SHOP_NOT_ENOUGH_GOLD);
    }
    else
    {
        Enchant(other, "ENCHANT_AFRAID", 0.4);
        UniPrint(OTHER, "핵폭탄 해머를 구입합니다, 이 작업은 75000원이 필요합니다.  계속하려면 더블클릭하세요");
    }
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
                UniPrint(OTHER, "결제성공! 모두 " + IntToString(res) + "개의 인벤토리가 수리되었습니다");
            }
        }
        else
            UniPrint(OTHER, SHOP_NOT_ENOUGH_GOLD);
    }
    else
    {
        Enchant(other, "ENCHANT_AFRAID", 0.3);
        UniPrint(OTHER, "자신이 소유한 모든 인벤토리를 수리합니다, 이 작업은 3000원이 필요하며 계속 진행하려면 더블클릭 하세요");
    }
}

void BuyHovath()
{
    int plr = CheckPlayer();

    if (plr + 1)
    {
        if (HasEnchant(other, "ENCHANT_AFRAID"))
        {
            EnchantOff(other, "ENCHANT_AFRAID");
            if (GetGold(other) >= 80000)
            {
                ChangeGold(other, -80000);
                if (ReadyPlayerChanging(plr))
                {
                    PlrCre[plr] = SelectHovath(plr);
                    UniPrint(OTHER, "캐릭터가 호바스로 변경되었습니다, 돌아오고 싶으시면 L 키를 누르시면 됩니다");
                }
            }
            else
                UniPrint(OTHER, SHOP_NOT_ENOUGH_GOLD);
        }
        else
        {
            Enchant(other, "ENCHANT_AFRAID", 0.4);
            UniPrint(OTHER, "자신의 캐릭터를 호바스로 변신합니다, 이 작업은 8만원이 필요합니다");
            UniPrint(OTHER, "계속하려면 더블클릭 하십시오");
        }
    }
}

void BuyAbilityShockField()
{
    int plr = CheckPlayer();

    if (plr + 1)
    {
        if (HasEnchant(other, "ENCHANT_AFRAID"))
        {
            EnchantOff(other, "ENCHANT_AFRAID");
            if (player[plr + 10] & 0x08)
                UniPrint(OTHER, "이미 이 능력을 습득하셨습니다");
            else
            {
                if (GetGold(other) >= 80000)
                {
                    ChangeGold(other, -80000);
                    player[plr + 10] = player[plr + 10] ^ 0x08;
                    UniPrint(OTHER, "결제완료: 쇼크필드 패시브 능력을 습득하셨습니다");
                }
                else
                    UniPrint(OTHER, SHOP_NOT_ENOUGH_GOLD);
            }
        }
        else
        {
            Enchant(other, "ENCHANT_AFRAID", 0.4);
            UniPrint(OTHER, "패시브: 쇼크필드를 배웁니다, 이 패시브를 습득하면 캐릭터는 항상 쇼크가 걸립니다");
            UniPrint(OTHER, "이 작업은 8만원이 필요하며 계속하려면 더블클릭 하세요");
        }
    }
}

void BuyBerserkerNoCooldown()
{
    int plr = CheckPlayer();

    if (plr + 1)
    {
        if (HasEnchant(other, "ENCHANT_AFRAID"))
        {
            EnchantOff(other, "ENCHANT_AFRAID");
            if (GetGold(other) >= 70000)
            {
                if (player[plr + 10] & 0x04)
                    UniPrint(OTHER, "이미 이 능력을 습득하셨습니다");
                else
                {
                    MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
                    AudioEvent("AwardSpell", 1);
                    Effect("WHITE_FLASH", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
                    GreenSparkFx(GetWaypointX(1), GetWaypointY(1));
                    Effect("BLUE_SPARKS", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
                    ChangeGold(other, -70000);
                    player[plr + 10] = player[plr + 10] ^ 4;
                }
            }
            else
                UniPrint(OTHER, SHOP_NOT_ENOUGH_GOLD);
        }
        else
        {
            Enchant(other, "ENCHANT_AFRAID", 0.4);
            UniPrint(OTHER, "조심스럽게 걷기를 사용하면 버저커 차지의 쿨다운 상태를 없앱니다, 이 능력을 구입하려면 7만원이 필요합니다");
            UniPrint(OTHER, "계속하려면 더블클릭 하세요");
        }
    }
}

void ShowVictoryStampString(int wp)
{
    MoveWaypoint(1, GetWaypointX(wp) - 100.0, GetWaypointY(wp) - 100.0);
    StrVictory();
}

void VictoryEvent()
{
    int ptr, unit;

    if (CurrentHealth(other))
    {
        unit = GetPlayerUnit(other);
        if (unit < 0) return;
        if (IsObjectOn(self) && !ptr)
        {
            ptr = PutTeleportMark(50, 51);
            MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
            DeleteObjectTimer(CreateObject("OblivionUp", 1), 60);
            Effect("WHITE_FLASH", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
            AudioEvent("StaffOblivionAchieve1", 1);
            FrameTimerWithArg(48, 49, ShowVictoryStampString);
            Delete(self);
            UniPrintToAll("축하드립니다! 승리하셨습니다!");
        }
    }
}

void TeleportPortal()
{
    int unit;

    if (CurrentHealth(other))
    {
        unit = GetPlayerUnit(other);
        if (unit < 0) return;
        Effect("TELEPORT", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        MoveObject(unit, GetObjectX(GetTrigger() + 1), GetObjectY(GetTrigger() + 1));
        MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
        AudioEvent("BlindOff", 1);
        Effect("SMOKE_BLAST", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        Effect("COUNTERSPELL_EXPLOSION", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
    }
}

void OpenExitWalls()
{
    int i;

    for (i = 0 ; i < 7 ; i ++)
        WallOpen(Wall(138 + i, 158 - i));
}

void CheckDemonDeathCount(int count)
{
    int ptr;

    if (count == DemonMax && !ptr)
    {
        OpenExitWalls();
        ptr = PutTeleportMark(48, 49);
        SetCallback(PutBeacon(49), 9, VictoryEvent);
        UniPrintToAll("출구가 열렸습니다");
    }
}

int StartSummonDemonlord(int ptr)
{
    int count = GetDirection(ptr), unit;

    if (count < DemonMax)
    {
        SpawnDragon(ptr);
        GreenSparkFx(GetObjectX(ptr), GetObjectY(ptr));
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(8, ptr, StartSummonDemonlord);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
        Delete(ptr + 2);
    }
}

void OpenDragonWalls()
{
    int i;

    for (i = 0 ; i < 13 ; i ++)
        WallOpen(Wall(i + 146, 172 - i));
}

void CheckKeybaseOn()
{
    int ptr;

    if (IsObjectOn(Object("RedKeyBase")) && IsObjectOn(Object("BlueKeyBase")) && !ptr)
    {
        ObjectOn(Object("LeftBossLit"));
        ObjectOn(Object("RightBossLit"));
        ptr = CreateObject("InvisibleLightBlueHigh", 47);
        CreateObject("BlueSummons", 47);
        CreateObject("ForceOfNatureCharge", 47);
        AudioEvent("DemonDie", 47);
        AudioEvent("HorrendousIsKilled", 47);
        OpenDragonWalls();
        FrameTimerWithArg(45, ptr, StartSummonDemonlord);
        UniPrintToAll("전설의 레드 드래곤이 등장합니다, 레드 드레곤을 모두 물리치면 출구가 열립니다");
    }
}

void CheckPutRedKey()
{
    if (GetUnitThingID(other) == 2182)
    {
        Effect("VIOLET_SPARKS", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
        AudioEvent("Gear2", 1);
        ObjectOn(Object("RedKeyBase"));
        Delete(other);
        Delete(self);
        CheckKeybaseOn();
    }
}

void CheckPutBlueKey()
{
    if (GetUnitThingID(other) == 2180)
    {
        Effect("BLUE_SPARKS", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
        AudioEvent("Gear2", 1);
        ObjectOn(Object("BlueKeyBase"));
        Delete(other);
        Delete(self);
        CheckKeybaseOn();
    }
}

void OpenPart1Gate()
{
    int unit = GetPlayerUnit(other);
    if (unit)
    {
        UnlockDoor(Object("MecaGate1"));
        UnlockDoor(Object("MecaGate2"));
        ObjectOff(self);
        UniPrint(unit, "역무원실 출입문의 잠금이 해제되었습니다");
    }
}

int Part2EntranceBlock()
{
    int ptr;

    if (!ptr)
    {
        ptr = CreateObject("IronBlock", 36);
        CreateObject("IronBlock", 37);
        CreateObject("IronBlock", 38);
    }
    return ptr;
}

void Part2BlocksMoving(int ptr)
{
    int count = GetDirection(ptr);

    if (count < 80 && IsObjectOn(ptr))
    {
        MoveObject(ptr, GetObjectX(ptr) + 2.0, GetObjectY(ptr) - 2.0);
        MoveObject(ptr + 1, GetObjectX(ptr + 1) + 2.0, GetObjectY(ptr + 1) - 2.0);
        MoveObject(ptr + 2, GetObjectX(ptr + 2) + 2.0, GetObjectY(ptr + 2) - 2.0);
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, Part2BlocksMoving);
    }
}

void EnableMyMecaGears()
{
    int i;

    for (i = 0 ; i < 12 ; i ++)
        ObjectOn(Object("SeoulMetrolGear" + IntToString(i + 1)));
}

void MecaGearActivateFx(float x, float y)
{
    MoveWaypoint(1, x, y);
    AudioEvent("MechGolemDie", 1);
    DeleteObjectTimer(CreateObject("BigSmoke", 1), 9);
    Effect("DAMAGE_POOF", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
    Effect("JIGGLE", GetWaypointX(1), GetWaypointY(1), 30.0, 0.0);
}

void RollPart2Blocks()
{
    int ptr = Part2EntranceBlock(), unit = GetPlayerUnit(other);

    if (unit)
    {
        MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
        AudioEvent("CreatureCageAppears", 1);
        AudioEvent("ChainPull", 1);

        MecaGearActivateFx(1578.0, 1878.0);
        MecaGearActivateFx(1464.0, 1997.0);

        MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
        FrameTimerWithArg(15, ptr, Part2BlocksMoving);
        FrameTimerWithArg(30, LUnit[2], AwakeLineMonster);
        AudioEvent("SpikeBlockMove", 1);
        UniPrintToAll("지하 2층 대합실의 비밀벽이 열렸습니다");
        ObjectOff(self);
    }
}

void OpenPairWalls(int x, int y)
{
    WallOpen(Wall(x, y));
    WallOpen(Wall(x + 1, y - 1));
}

void OpenWallAtAllign(int x, int y)
{
    OpenPairWalls(x, y);
    OpenPairWalls(x + 9, y - 9);
    OpenPairWalls(x + 18, y - 18);
    OpenPairWalls(x + 27, y - 27);
}

void OpenFirstArea()
{
    int unit = GetPlayerUnit(other);

    if (unit)
    {
        OpenWallAtAllign(163, 57);
        OpenWallAtAllign(125, 95);
        OpenWallAtAllign(87, 133);
        OpenWallAtAllign(11, 209);
        FrameTimerWithArg(3, LUnit[1], AwakeLineMonster);
        ObjectOff(self);
        UniPrintToAll("열차의 모든 출입문을 개방하였습니다");
    }
}

void OpenSecondArea()
{
    int unit = GetPlayerUnit(other);

    if (unit)
    {
        OpenWallAtAllign(36, 234);
        OpenWallAtAllign(74, 196);
        OpenWallAtAllign(112, 158);
        OpenWallAtAllign(150, 120);
        OpenWallAtAllign(188, 82);
        FrameTimerWithArg(3, LUnit[3], AwakeLineMonster);
        ObjectOff(self);
        UniPrintToAll("열차의 모든 출입문을 개방하였습니다");
    }
}

int GetPlayerUnit(int unit)
{
    if (IsPlayerUnit(unit))
        return unit;
    if (IsPlayerUnit(GetOwner(unit)))
        return GetOwner(unit);
    return 0;
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

void DisplayLadderBoard()
{
    /*string txt = "플레이어 킬 스코어:\n";
    int scd, min, hor;

    if (IsCaller(GetTrigger() + 1))
    {
        if (GetDirection(self) < 30)
            LookWithAngle(self, GetDirection(self) + 1);
        else
        {
            LookWithAngle(self, 0);
            txt += (IntToString(GenCnt) + "\n\n데스: \n" + IntToString(PlrDeadCnt) + "\n\n경과시간:\n");
            scd ++;
            if (scd == 60)
            {
                scd = 0;
                min ++;
                if (min == 60)
                {
                    min = 0;
                    hor ++;
                }
            }
            txt += (IntToString(hor) + "시간 " + IntToString(min) + "분 " + IntToString(scd) + "초");
            Chat(self, txt);
        }
    }*/
    return;
}

int PutTeleportMark(int srcWp, int dstWp)
{
    int unit = CreateObject("WeirdlingBeast", srcWp);

    SetUnitMaxHealth(CreateObject("InvisibleLightBlueHigh", dstWp) - 1, 10);
    Enchant(CreateObject("InvisibleLightBlueHigh", srcWp), "ENCHANT_ANCHORED", 0.0);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    SetCallback(unit, 9, TeleportPortal);

    return unit;
}

int PutBeacon(int wp)
{
    int unit = CreateObject("WeirdlingBeast", wp);

    SetUnitMaxHealth(unit, 10);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    return unit;
}

int SpawnDummy(string name, int dir, int wp)
{
    int unit = CreateObject(name, wp);

    ObjectOff(unit);
    LookWithAngle(unit, dir);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    Frozen(unit, 1);

    return unit;
}

void MapPutSpecialShop()
{
    int ptr = SpawnDummy("Necromancer", 96, 54);

    SetDialog(ptr, "aa", BuyHovath, BuyHovath);
    SetDialog(SpawnDummy("Horrendous", 96, 55), "aa", BuyBerserkerNoCooldown, BuyBerserkerNoCooldown);
    SetDialog(SpawnDummy("WizardWhite", 32, 66), "aa", RepairShoppingNet, RepairShoppingNet);
    SetDialog(SpawnDummy("Demon", 96, 67), "aa", BuyNuclearHammer, BuyNuclearHammer);
    SetDialog(SpawnDummy("LichLord", 96, 68), "aa", BuyOblivionStaff, BuyOblivionStaff);
    SetDialog(SpawnDummy("Wizard", 225, 69), "aa", BuyDeathRayHammer, BuyBerserkerNoCooldown);
    SetDialog(SpawnDummy("Necromancer", 225, 70), "aa", BuyAbilityShockField, BuyAbilityShockField);
    Enchant(CreateObject("InvisibleLightBlueHigh", 70), "ENCHANT_SHOCK", 0.0);
}

void MapDecorations()
{
    Part2EntranceBlock();
    SetCallback(PutBeacon(45), 9, CheckPutRedKey);
    SetCallback(PutBeacon(46), 9, CheckPutBlueKey);
    CreateObject("RedOrbKeyOfTheLich", 43);
    CreateObject("BlueOrbKeyOfTheLich", 44);
    CreateObject("BlackPowderBarrel", 34);
    CreateObject("BlackPowderBarrel", 35);
    CreateObject("BlackPowderBarrel2", 39);
    CreateObject("BlackPowderBarrel", 40);
    CreateObject("BlackPowderBarrel2", 41);
    CreateObject("BlackPowderBarrel", 42);
    FrameTimer(1, MapPutSpecialShop);
}

void InitAutoGates(int arg0)
{
    if (arg0 >= 0)
    {
        AGate[arg0] = SpawnAutoGate(arg0 + 25, 32.0);
        SpawnAutoGateBeacon(arg0 + 25, arg0);
        FrameTimerWithArg(1, arg0 - 1, InitAutoGates);
    }
}

int SpawnAutoGateBeacon(int wp, int idx)
{
    int unit = CreateObjectAt("WeirdlingBeast", GetWaypointX(wp) - 39.0, GetWaypointY(wp) + 39.0);
    LookWithAngle(CreateObjectAt("WeirdlingBeast", GetWaypointX(wp) + 39.0, GetWaypointY(wp) - 39.0), idx);
    LookWithAngle(CreateObjectAt("WeirdlingBeast", GetWaypointX(wp), GetWaypointY(wp)), idx);

    SetUnitMaxHealth(unit, 10);
    SetUnitMaxHealth(unit + 1, 10);
    SetUnitMaxHealth(unit + 2, 10);
    Enchant(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)), "ENCHANT_ANCHORED", 0.0);
    Enchant(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit + 1), GetObjectY(unit + 1)), "ENCHANT_ANCHORED", 0.0);
    Enchant(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit + 2), GetObjectY(unit + 2)), "ENCHANT_ANCHORED", 0.0);
    Frozen(CreateObjectAt("IronBlock", GetWaypointX(wp) - 81.0, GetWaypointY(wp) - 81.0), 1);
    Frozen(CreateObjectAt("IronBlock", GetWaypointX(wp) + 81.0, GetWaypointY(wp) + 81.0), 1);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    Damage(unit + 1, 0, MaxHealth(unit + 1) + 1, -1);
    Damage(unit + 2, 0, MaxHealth(unit + 2) + 1, -1);
    LookWithAngle(unit, idx);
    SetCallback(unit, 9, StartMoveAGate);
    SetCallback(unit + 1, 9, StartMoveAGate);
    SetCallback(unit + 2, 9, StartMoveAGate);
    return unit;
}

int SpawnAutoGate(int wp, float gap) //gap is 32.0
{
    int unit = CreateObject("InvisibleLightBlueHigh", wp);

    Frozen(CreateObjectAt("MovableBookcase4", GetObjectX(unit), GetObjectY(unit) - gap), 1);
    Frozen(CreateObjectAt("MovableBookcase4", GetObjectX(unit) + gap, GetObjectY(unit)), 1);
    Frozen(CreateObjectAt("MovableBookcase4", GetObjectX(unit) - gap, GetObjectY(unit)), 1);
    Frozen(CreateObjectAt("MovableBookcase4", GetObjectX(unit), GetObjectY(unit) + gap), 1);
    return unit;
}

void StartMoveAGate()
{
    int unit = -1;

    if (CurrentHealth(other))
    {
        if (OpenAutoGate(AGate[GetDirection(self)]))
        {
            MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
            AudioEvent("SpikeBlockMove", 1);
            if (HasClass(other, "PLAYER"))
                unit = GetCaller();
            else if (HasClass(GetOwner(other), "PLAYER"))
                unit = GetOwner(other);
            if (unit >= 0)
                UniPrint(unit, "열차 통로 출입문이 열립니다");
        }
    }
}

int OpenAutoGate(int ptr)
{
    if (!GetDirection(ptr + 1))
    {
        LookWithAngle(ptr + 1, 1);
        FrameTimerWithArg(1, ptr, OpenGateHandler);
        return 1;
    }
    return 0;
}

void OpenGateHandler(int ptr)
{
    int count = GetDirection(ptr);

    if (count < 40)
    {
        MoveObject(ptr + 1, GetObjectX(ptr + 1) - 1.0, GetObjectY(ptr + 1) - 1.0);
        MoveObject(ptr + 2, GetObjectX(ptr + 2) + 1.0, GetObjectY(ptr + 2) + 1.0);
        MoveObject(ptr + 3, GetObjectX(ptr + 3) - 1.0, GetObjectY(ptr + 3) - 1.0);
        MoveObject(ptr + 4, GetObjectX(ptr + 4) + 1.0, GetObjectY(ptr + 4) + 1.0);
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, OpenGateHandler);
    }
    else
    {
        LookWithAngle(ptr, 0);
        FrameTimerWithArg(80, ptr, CloseGateHandler);
    }
}

void CloseGateHandler(int ptr)
{
    int count = GetDirection(ptr);

    if (count < 40)
    {
        MoveObject(ptr + 1, GetObjectX(ptr + 1) + 1.0, GetObjectY(ptr + 1) + 1.0);
        MoveObject(ptr + 2, GetObjectX(ptr + 2) - 1.0, GetObjectY(ptr + 2) - 1.0);
        MoveObject(ptr + 3, GetObjectX(ptr + 3) + 1.0, GetObjectY(ptr + 3) + 1.0);
        MoveObject(ptr + 4, GetObjectX(ptr + 4) - 1.0, GetObjectY(ptr + 4) - 1.0);
        LookWithAngle(ptr, count + 1);
        FrameTimerWithArg(1, ptr, CloseGateHandler);
    }
    else
    {
        LookWithAngle(ptr, 0);
        LookWithAngle(ptr + 1, 0);
    }
}

void MapExit()
{
    MusicEvent();
    
    ResetPlayerHandlerWhenExitMap();
    RemoveCoopTeamMode();
    UniPrintToAll("subway.map 정보: 맵에 사용되었던 리소스가 성공적으로 해제되었습니다");
}

void MapInitialize()
{
    LastUnit = CreateObject("RedPotion", 1);
    Delete(LastUnit);
    FrameTimer(1, MakeCoopTeam);
    MusicEvent();
    GetMaster();
    FrameTimerWithArg(3, 7, InitAutoGates);
    FrameTimerWithArg(6, Object("0000FirstMarker"), InitScanStart);
    FrameTimer(10, MapDecorations);
    FrameTimer(30, LoopPreservePlayers);
}

void InitScanStart(int unit)
{
    if (IsObjectOn(unit))
    {
        InitiGame(unit);
    }
}

void EndScan(int count)
{
    WallOpen(Wall(15, 235));
    WallOpen(Wall(16, 236));
    AwakeLineMonster(LUnit[0]);
    UniPrintToAll(IntToString(count) + " 개의 유닛을 스캔완료 하였습니다");
    SecondTimer(2, StartGameMent);
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

    SelfDamageClassEntry(pUnit);
    DiePlayerHandlerEntry(pUnit);

    UniPrintToAll(PlayerIngameNick(pUnit) + " 님께서 게임에 들어왔습니다");

    return plr;
}

void PlayerEntryPoint()
{
    int plr, i;

    while (1)
    {
        if (CurrentHealth(other))
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
                PlayerJoin(plr);
                break;
            }
        }
        PlayerCantJoin();
        break;
    }
}

void PlayerJoin(int plr)
{
    int destLocation = 19;
    
    if (PlayerClassCheckDeathFlag(plr))
        PlayerClassSetDeathFlag(plr);
    MoveObject(player[plr], GetWaypointX(destLocation), GetWaypointY(destLocation));
    Effect("TELEPORT", GetWaypointX(destLocation), GetWaypointY(destLocation), 0.0, 0.0);
    DeleteObjectTimer(CreateObject("BlueRain", destLocation), 12);
    AudioEvent("BlindOff", destLocation);
}

void PlayerCantJoin()
{
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    Enchant(other, "ENCHANT_ANCHORED", 0.0);
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    Enchant(other, "ENCHANT_BLINDED", 0.0);
    MoveObject(other, GetWaypointX(33), GetWaypointY(33));
    UniPrintToAll("맵 입장에 실패했습니다");
}

int PlayerInputTable(int key)
{
    int arr[100];

    if (key < 0)
    {
        arr[2] = 2; arr[6] = 1; arr[48] = 3;
    }
    return arr[key];
}

int GetProcFunc()
{
    StopScript(ProcNothing);
}

void ProcNothing(int arg)
{
    return;
}

void ProcLClick(int plr)
{
    ThunderBolt(plr);
}

void ProcRClick(int plr)
{
    int glow = CreateObjectAt("Moonglow", GetObjectX(player[plr]), GetObjectY(player[plr]));

    LookWithAngle(glow, plr);
    SetOwner(player[plr], glow);
    FrameTimerWithArg(1, glow, GoCreatureMove);
}

void ProcLButtonPress(int plr)
{
    PlayerOutFromCreature(plr);
    UniPrint(player[plr], "기존 캐릭터로 복귀하였습니다");
}

void PlayerCreatureHandler(int plr)
{
    if (CurrentHealth(PlrCre[plr]))
    {
        if (CheckWatchFocus(player[plr]))
            PlayerLook(player[plr], PlrCre[plr]);
        CallFunctionWithArg(GetProcFunc() + PlayerInputTable(CheckPlayerInput(player[plr])), plr);
    }
    else
    {
        PlayerOutFromCreature(plr);
    }
}

void FreePlayerCreature(int plr)
{
    if (MaxHealth(PlrCre[plr]))
    {
        Delete(PlrCre[plr]);
        Delete(PlrCre[plr] + 1);
    }
}

void PlayerOutFromCreature(int plr)
{
    int cre = PlrCre[plr];

    EnchantOff(player[plr], "ENCHANT_FREEZE");
    EnchantOff(player[plr], "ENCHANT_INVULNERABLE");
    if (MaxHealth(cre))
        MoveObject(player[plr], GetObjectX(cre), GetObjectY(cre));
    else
        MoveObject(player[plr], GetWaypointX(53), GetWaypointY(53));
    player[plr + 10] = player[plr + 10] ^ 2;
    FreePlayerCreature(plr);
}

void GoCreatureMove(int ptr)
{
    int plr = GetDirection(ptr), arr[10];

    if (IsObjectOn(ptr))
    {
        if (CurrentHealth(PlrCre[plr]))
        {
            if (Distance(GetObjectX(PlrCre[plr]), GetObjectY(PlrCre[plr]), GetObjectX(ptr), GetObjectY(ptr)) > 8.0)
            {
                LookAtObject(PlrCre[plr], ptr);
                Walk(PlrCre[plr], GetObjectX(PlrCre[plr]), GetObjectY(PlrCre[plr]));
                MoveObject(PlrCre[plr], GetObjectX(PlrCre[plr]) + UnitRatioX(ptr, PlrCre[plr], 13.0), GetObjectY(PlrCre[plr]) + UnitRatioY(ptr, PlrCre[plr], 13.0));
            }
            if (!IsObjectOn(arr[plr]))
            {
                arr[plr] = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(PlrCre[plr]), GetObjectY(PlrCre[plr]));
                SetOwner(PlrCre[plr], arr[plr]);
                FrameTimerWithArg(1, arr[plr], PlayerCreatureStopWalk);
            }
            LookWithAngle(arr[plr], 12);
        }
        Delete(ptr);
    }
}

void PlayerCreatureStopWalk(int ptr)
{
    int count = GetDirection(ptr), cre = GetOwner(ptr);

    if (count)
    {
        LookWithAngle(ptr, count - 1);
        FrameTimerWithArg(1, ptr, PlayerCreatureStopWalk);
    }
    else
    {
        if (CurrentHealth(cre))
        {
            CreatureIdle(cre);
        }
        Delete(ptr);
    }
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

void PlayerClassOnAlive(int plr, int pUnit)
{
    if (player[plr + 10] & 2)
        PlayerCreatureHandler(plr);
    if (player[plr + 10] & 4)
        RemoveBerserkerCool(plr);
    if (player[plr + 10] & 8)
        AlwaysApplyShockField(plr);
    ForceChangePlaerUpdateHandler(pUnit);
}

void PlayerClassOnDeath(int plr)
{ }

void PlayerClassOnExit(int plr)
{
    FreePlayerCreature(plr);
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

void AlwaysApplyShockField(int plr)
{
    if (!HasEnchant(player[plr], "ENCHANT_SHOCK") && !HasEnchant(player[plr], "ENCHANT_BURNING"))
    {
        Enchant(player[plr], "ENCHANT_BURNING", 0.2);
        Enchant(player[plr], "ENCHANT_SHOCK", 0.0);
    }
}

void RemoveBerserkerCool(int plr)
{
    if (HasEnchant(player[plr], "ENCHANT_SNEAK"))
    {
        BerserkerNoDelayCore(plr);
        EnchantOff(player[plr], "ENCHANT_SNEAK");
    }
}

void ThunderBolt(int plr)
{
    int glow, cre = PlrCre[plr];

    if (CurrentHealth(player[plr]) && CurrentHealth(cre))
    {
        if (HasEnchant(cre, "ENCHANT_LIGHT")) return;
        Enchant(cre, "ENCHANT_LIGHT", 0.7);
        glow = CreateObjectAt("Moonglow", GetObjectX(cre), GetObjectY(cre));
        SetOwner(cre, CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(cre), GetObjectY(cre)));
        SetOwner(player[plr], glow);
    }
    FrameTimerWithArg(1, glow, ThunderBoltCritical);
}

void ThunderBoltCritical(int ptr)
{
    int owner = GetOwner(ptr), cre = GetOwner(ptr + 1), unit, i;
    float vectX = UnitRatioX(ptr, cre, 38.0), vectY = UnitRatioY(ptr, cre, 38.0);

    if (CurrentHealth(owner) && CurrentHealth(cre))
    {
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(ptr), GetObjectY(ptr)) + 1;
        Raise(unit - 1, ToFloat(owner));
        for (i = 0 ; i < 13 ; i ++)
        {
            MoveObject(ptr + 1, GetObjectX(ptr + 1) + vectX, GetObjectY(ptr + 1) + vectY);
            if (IsVisibleTo(ptr + 1, cre))
            {
                Frozen(CreateObjectAt("ShopkeeperConjurerRealm", GetObjectX(ptr + 1), GetObjectY(ptr + 1)), 1);
                SetOwner(unit - 1, unit + i);
                DeleteObjectTimer(unit + i, 1);
                SetCallback(unit + i, 9, ThunderBoltCollideHandler);
            }
            else
                break;
        }
        YellowLightningFx(GetObjectX(cre), GetObjectY(cre), GetObjectX(ptr + 1), GetObjectY(ptr + 1), 24);
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
    if (!HasEnchant(unit, "ENCHANT_DETECTING"))
    {
        Enchant(unit, "ENCHANT_DETECTING", 0.0);
        FrameTimerWithArg(delay, unit, ResetUnitSight);
    }
}

void ResetUnitSight(int unit)
{
    EnchantOff(unit, "ENCHANT_DETECTING");
    Enchant(unit, "ENCHANT_BLINDED", 0.06);
    AggressionLevel(unit, 1.0);
}

void UnitDeathHandler()
{
    MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
    CallFunctionWithArgInt(FieldItemFuncPtr() + Random(0, 7), 1);
    DeleteObjectTimer(self, 90);
}

int FieldItemFuncPtr()
{
    StopScript(HotPotion);
}

int HotPotion(int wp)
{
    return CreateObject("RedPotion", wp);
}

int PotionItemDrop(int wp)
{
    return CheckPotionThingID(CreateObject(PotionList(Random(0, 11)), 1));
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
    string name[] = {"QuestGoldChest", "QuestGoldPile", "Gold"};
    int money = CreateObject(name[Random(0, 2)], wp);

    SetMemory(GetMemory(GetMemory(0x750710) + 0x2b4), Random(1000, 5000));
    return money;
}

int SomeGermDrop(int wp)
{
    string name[] = {"Ruby", "Ruby", "Ruby", "Emerald", "Emerald", "Diamond"};

    return CreateObject(name[Random(0, 5)], wp);
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
        "FireProtectPotion", "HastePotion", "ShieldPotion", "InfravisionPotion"
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

void CheckSpecialItem(int ptr)
{
    int id = GetMemory(ptr + 0x04);

    if (id >= 222 && id <= 225)
        SetMemory(ptr + 0x2c4, 0x53a720);
    else if (id == 1178)
        SetMemory(GetMemory(ptr + 0x2e0), 0xffff);
    else if (id == 1168)
        SetMemory(GetMemory(ptr + 0x2e0), 0xffff);
}

void SpawnMarkerMonster(int cur)
{
    int unit, idx = UnitScrNameToNumber(cur), res = 0;
    int low = idx % 100, high = idx / 100;
    
    if (idx >= 0)
    {
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(cur), GetObjectY(cur));
        LookWithAngle(unit, low);
        if (IsObjectOn(LUnit[high]))
            SetOwner(LUnit[high], unit);
        LUnit[high] = unit;
    }
    Delete(cur);
}

void AwakeLineMonster(int cur)
{
    int pic = cur;

    if (IsObjectOn(pic))
    {
        CallFunctionWithArgInt(MonsterFuncPtr() + (GetDirection(pic) % 100), pic);
        cur = GetOwner(cur);
        FrameTimerWithArg(1, cur, AwakeLineMonster);
        Delete(pic);
    }
}

/*
MonsterSet
num, name
0   SmallSpider
1   Imp
2   Swordsman
3   GiantLeech
4   Wolf
5   Archer
6   BlackWidow
7   PurpleMaiden
8   Mystic
9   RWiz
10  Necromancer
11  Lich
12  GWiz
13  DarkBear
14  Goon
15  Bear
16  OgreBrute
17  Ogreaxe
18  OgreLord
19  FireSprite
20  EmberDemon
21  Bat
22  MecaFly
23  EvilCherub
24  Shade
25  Wisp
26  BlackWolf
27  Urchin
28 shaman
29  Skeleton
30  SkeletonLord
31  Ghost
32  Scorpion
33  Spider
34  AlbinoSpider
35    SmallAlbinoSpider
36  WeirdlingBeast
37    Mimic
38  StoneGolem
39  MechanicalGolem
40  Troll
41  Beholder
42  CarnivorousPlant
43  MeleeDemon
44  Wasp
45  SpittingSpider
46  VileZombie
47 Zombie
48 AirshipCaptain
49 Horrendous
50 hecubah
*/

int MonsterFuncPtr()
{
    StopScript(SpawnSmallSpider);
}

int SpawnSmallSpider(int ptr)
{
    int unit = CreateObjectAt("SmallSpider", GetObjectX(ptr), GetObjectY(ptr));

    Enchant(unit, "ENCHANT_SLOWED", 0.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 68);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnImp(int ptr)
{
    int unit = CreateObjectAt("Swordsman", GetObjectX(ptr), GetObjectY(ptr));

    Enchant(unit, "ENCHANT_SLOWED", 0.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 64);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnSwordman(int ptr)
{
    int unit = CreateObjectAt("Swordsman", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 325);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 0.0);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnLeech(int ptr)
{
    int unit = CreateObjectAt("GiantLeech", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 96);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnWolf(int ptr)
{
    int unit = CreateObjectAt("Wolf", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 128);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 0.0);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnArcher(int ptr)
{
    int unit = CreateObjectAt("Archer", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 96);
    RetreatLevel(unit, 0.0);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnBlackSpider(int ptr)
{
    int unit = CreateObjectAt("BlackWidow", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, BlackWidowBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 295);
    SetCallback(unit, 5, BlackSpiderDead);
    return unit;
}

int SpawnVioletMaiden(int ptr)
{
    int unit;

    MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
    unit = ColorMaiden(240, 0, 225, 1);

    SetOwner(GetMaster(), unit);
    SetUnitScanRange(unit, 400.0);
    SetUnitMaxHealth(unit, 260);
    SetCallback(unit, 3, PurpleGirlDetectHandler);
    SetCallback(unit, 5, PurpleGirlDead);
    SetCallback(unit, 13, PurpleGirlLostEnemyOnSight);

    return unit;
}

int SummonMystic(int target)
{
    int unit = CreateObjectAt("Wizard", GetObjectX(target), GetObjectY(target));
    int uec = GetMemory(GetMemory(0x750710) + 0x2ec);

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 237);
    SetCallback(unit, 5, UnitDeathHandler);
    //SetCallback(unit, 8, WizRunAway);
    if (uec)
    {
        SetMemory(uec + 0x528, ToInt(1.0));
        uec += 0x5d0;
        SetMemory(uec + GetSpellNumber("SPELL_BLINK"), 0);
    }
    return unit;
}

int SummonRedWiz(int target)
{
    int unit = CreateObjectAt("WizardRed", GetObjectX(target), GetObjectY(target));
    int uec = GetMemory(GetMemory(0x750710) + 0x2ec);

    UnitLinkBinScript(unit, WizardRedBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 435);
    SetCallback(unit, 5, UnitDeathHandler);
    SetCallback(unit, 8, WizRunAway);
    UnitZeroFleeRange(unit);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x20);
    if (uec)
    {
        SetMemory(uec + 0x528, ToInt(1.0));
		SetMemory(uec + 0x520, ToInt(400.0));
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
    return unit;
}

int SummonNecromancer(int target)
{
    int unit = CreateObjectAt("Necromancer", GetObjectX(target), GetObjectY(target));
    int uec = GetMemory(GetMemory(0x750710) + 0x2ec);

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 308);
    SetCallback(unit, 5, UnitDeathHandler);
    SetCallback(unit, 8, WizRunAway);
    if (uec)
    {
        SetMemory(uec + 0x528, ToInt(1.0));
        SetMemory(uec + 0x520, ToInt(400.0));
        uec += 0x5d0;
        SetMemory(uec + GetSpellNumber("SPELL_LIGHTNING"), 0x40000000);
		SetMemory(uec + GetSpellNumber("SPELL_SHIELD"), 0x10000000);
        SetMemory(uec + GetSpellNumber("SPELL_SLOW"), 0x20000000);
		SetMemory(uec + GetSpellNumber("SPELL_INVISIBILITY"), 0x10000000);
		SetMemory(uec + GetSpellNumber("SPELL_FIREBALL"), 0x40000000);
        SetMemory(uec + GetSpellNumber("SPELL_INVERSION"), 0x8000000);
        SetMemory(uec + GetSpellNumber("SPELL_COUNTERSPELL"), 0x8000000);
    }
    return unit;
}

int SummonLich(int target)
{
    int unit = CreateObjectAt("Lich", GetObjectX(target), GetObjectY(target));

    UnitLinkBinScript(unit, LichLordBinTable());
    UnitZeroFleeRange(unit);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x20);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 405);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SummonGreenWiz(int target)
{
    int unit = CreateObjectAt("WizardGreen", GetObjectX(target), GetObjectY(target));
    int uec = GetMemory(GetMemory(0x750710) + 0x2ec);

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 265);
    SetCallback(unit, 5, UnitDeathHandler);
    SetCallback(unit, 8, WizRunAway);
    if (uec)
    {
        SetMemory(uec + 0x528, ToInt(1.0));
        SetMemory(uec + 0x520, ToInt(400.0));
        uec += 0x5d0;
        SetMemory(uec + GetSpellNumber("SPELL_METEOR"), 0x40000000);
		SetMemory(uec + GetSpellNumber("SPELL_SHIELD"), 0x10000000);
		SetMemory(uec + GetSpellNumber("SPELL_CONFUSE"), 0x20000000);
        SetMemory(uec + GetSpellNumber("SPELL_SLOW"), 0x20000000);
        SetMemory(uec + GetSpellNumber("SPELL_POISON"), 0x20000000);
		SetMemory(uec + GetSpellNumber("SPELL_INVISIBILITY"), 0x10000000);
		SetMemory(uec + GetSpellNumber("SPELL_PIXIE_SWARM"), 0x40000000);
		SetMemory(uec + GetSpellNumber("SPELL_SUMMON_URCHIN_SHAMAN"), 0x40000000);
        SetMemory(uec + GetSpellNumber("SPELL_FIST"), 0x40000000);
        SetMemory(uec + GetSpellNumber("SPELL_INVERSION"), 0x08000000);
        SetMemory(uec + GetSpellNumber("SPELL_COUNTERSPELL"), 0x08000000);
        SetMemory(uec + GetSpellNumber("SPELL_BLINK"), 0);
    }
    return unit;
}

int SpawnDarkbear(int ptr)
{
    int unit = CreateObjectAt("BlackBear", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 325);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnGoons(int ptr)
{
    int unit = CreateObjectAt("Goon", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, GoonBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 128);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnBear(int ptr)
{
    int unit = CreateObjectAt("Bear", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 360);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnOgre(int ptr)
{
    int unit = CreateObjectAt("OgreBrute", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 325);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnOgreAxe(int ptr)
{
    int unit = CreateObjectAt("GruntAxe", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnOgreLord(int ptr)
{
    int unit = CreateObjectAt("OgreWarlord", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 360);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnFireSprite(int ptr)
{
    int unit = CreateObjectAt("FireSprite", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, FireSpriteBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 135);
    SetCallback(unit, 5, UnitDeathHandler);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 8000);

    return unit;
}

int SpawnEmberDemon(int ptr)
{
    int unit = CreateObjectAt("EmberDemon", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 128);
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

int SpawnMecaFly(int ptr)
{
    int unit = CreateObjectAt("FlyingGolem", GetObjectX(ptr), GetObjectY(ptr));

    Enchant(unit, "ENCHANT_SLOWED", 0.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 64);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnGargoyle(int ptr)
{
    int unit = CreateObjectAt("EvilCherub", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 64);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnShade(int ptr)
{
    int unit = CreateObjectAt("Shade", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 160);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnWisp(int ptr)
{
    int unit = CreateObjectAt("WillOWisp", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 420);
    SetCallback(unit, 5, UnitDeathHandler);
    SetCallback(unit, 8, WizRunAway);

    return unit;
}

int SpawnBlackWolf(int ptr)
{
    int unit = CreateObjectAt("BlackWolf", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnUrchin(int ptr)
{
    int unit = CreateObjectAt("Urchin", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 64);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnShaman(int ptr)
{
    int unit = CreateObjectAt("UrchinShaman", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 108);
    SetCallback(unit, 5, UnitDeathHandler);
    SetCallback(unit, 8, WizRunAway);

    return unit;
}

int SpawnSkeleton(int ptr)
{
    int unit = CreateObjectAt("Skeleton", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnSkeletonLord(int ptr)
{
    int unit = CreateObjectAt("SkeletonLord", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 280);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnGhost(int ptr)
{
    int unit = CreateObjectAt("Ghost", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 64);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnScorpion(int ptr)
{
    int unit = CreateObjectAt("Scorpion", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 250);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnNormalSpider(int ptr)
{
    int unit = CreateObjectAt("Spider", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 192);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnWhiteSpider(int ptr)
{
    int unit = CreateObjectAt("AlbinoSpider", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 128);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnMiniWhiteSpider(int ptr)
{
    int unit = CreateObjectAt("SmallAlbinoSpider", GetObjectX(ptr), GetObjectY(ptr));

    Enchant(unit, "ENCHANT_SLOWED", 0.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 60);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnBeast(int ptr)
{
    int unit = CreateObjectAt("WeirdlingBeast", GetObjectX(ptr), GetObjectY(ptr));

    UnitZeroFleeRange(unit);
    UnitLinkBinScript(unit, WeirdlingBeastBinTable());
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 256);
    SetCallback(unit, 5, UnitDeathHandler);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);

    return unit;
}

int SpawnMimic(int ptr)
{
    int unit = CreateObjectAt("Mimic", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 700);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnRockGolem(int ptr)
{
    int unit = CreateObjectAt("StoneGolem", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 800);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnMecaGolem(int ptr)
{
    int unit = CreateObjectAt("MechanicalGolem", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 900);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnTroll(int ptr)
{
    int unit = CreateObjectAt("Troll", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 325);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnBeholder(int ptr)
{
    int unit = CreateObjectAt("Beholder", GetObjectX(ptr), GetObjectY(ptr));

    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 425);
    SetCallback(unit, 5, UnitDeathHandler);
    RetreatLevel(unit, 0.0);

    return unit;
}

int SpawnPlant(int ptr)
{
    int unit = CreateObjectAt("CarnivorousPlant", GetObjectX(ptr), GetObjectY(ptr));

    CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit));
    SetUnitScanRange(unit, 420.0);
    SetUnitSpeed(unit, 1.21);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 680);
    SetCallback(unit, 3, PlantDetectHandler);
    SetCallback(unit, 5, PlantDeathHandler);
    SetCallback(unit, 13, PlantLostEnemyEvent);
    AggressionLevel(unit, 1.0);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);

    return unit;
}

int SpawnMeleeDemon(int ptr)
{
    int unit = CreateObjectAt("MeleeDemon", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnWasp(int ptr)
{
    int unit = CreateObjectAt("Wasp", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnWebSpider(int ptr)
{
    int unit = CreateObjectAt("SpittingSpider", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 120);
    SetCallback(unit, 5, UnitDeathHandler);

    return unit;
}

int SpawnVileZombie(int ptr)
{
    int unit = CreateObjectAt("VileZombie", GetObjectX(ptr), GetObjectY(ptr));

    SetUnitSpeed(unit, 1.28);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 325);
    SetCallback(unit, 5, ZombieDeadHandler);

    return unit;
}

int SpawnZombie(int ptr)
{
    int unit = CreateObjectAt("Zombie", GetObjectX(ptr), GetObjectY(ptr));

    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 225);
    SetCallback(unit, 5, ZombieDeadHandler);

    return unit;
}

int SpawnJandor(int ptr)
{
    int unit = CreateObjectAt("AirshipCaptain", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, AirshipCaptainBinTable());
    SetUnitScanRange(unit, 450.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 550);
    SetCallback(unit, 3, CaptainDetectHandler);
    SetCallback(unit, 5, UnitDeathHandler);
    SetCallback(unit, 13, PurpleGirlLostEnemyOnSight);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    AggressionLevel(unit, 1.0);
    return unit;
}

int SpawnHorrendous(int ptr)
{
    int unit = CreateObjectAt("Horrendous", GetObjectX(ptr), GetObjectY(ptr));

    SetUnitSpeed(unit, 1.1);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 500);
    SetCallback(unit, 5, UnitDeathHandler);
    return unit;
}

int SpawnOrbHecubah(int ptr)
{
    int unit = CreateObjectAt("Hecubah", GetObjectX(ptr), GetObjectY(ptr));

    UnitLinkBinScript(unit, HecubahBinTable());
    SetUnitScanRange(unit, 480.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 900);
    SetCallback(unit, 5, UnitDeathHandler);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x20);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    AggressionLevel(unit, 1.0);
    UnitZeroFleeRange(unit);

    return unit;
}

int SpawnDragon(int ptr)
{
    int unit = CreateObjectAt("Demon", GetObjectX(ptr), GetObjectY(ptr));

    CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit));
    SetUnitScanRange(unit, 480.0);
    SetOwner(GetMaster(), unit);
    SetUnitMaxHealth(unit, 1250);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x8020); //ALWAYS_RUN
    SetCallback(unit, 3, DemonSightEvent);
    SetCallback(unit, 5, DemonDeadHandler);
    SetCallback(unit, 13, DemonResetSight);

    return unit;
}

void BlackSpiderDead()
{
    MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
    AudioEvent("BeholderDie", 1);
    DeleteObjectTimer(CreateObject("BigSmoke", 1), 9);
    DeleteObjectTimer(CreateObject("WaterBarrelBreaking", 1), 12);
    CreateObject("ArachnaphobiaFocus", 1);
    UnitDeathHandler();
}

int UnitScrNameToNumber(int unit)
{
    int ptr = UnitToPtr(unit), raw, num = -1;

    if (ptr)
    {
        ptr = GetMemory(ptr);
        if (ptr)
        {
            raw = GetMemory(ptr);
            if (raw)
            {
                if (CheckVaildNumber(raw))
                    num = StringByteToInt(raw);
            }
        }
    }
    return num;
}

int StringByteToInt(int byte)
{
    return ((byte>>24) & 0xff) - 0x30 + ((((byte>>16) & 0xff) - 0x30) * 10) + ((((byte>>8) & 0xff) - 0x30) * 100) + (((byte & 0xff) - 0x30) * 1000);
}

int CheckVaildNumber(int num)
{
    return (IsNumber(num & 0xff) && IsNumber((num>>8) & 0xff) && IsNumber((num>>16) & 0xff) && IsNumber((num>>24) & 0xff));
}

int IsNumber(int ch)
{
    return (ch >= 0x30 && ch <= 0x39);
}

void InitiGame(int cur)
{
    int count, k;

    if (cur < LastUnit)
    {
        for (k = 0 ; k < 30 ; k ++)
        {
            if (GetUnitThingID(cur + (k * 2)) == 2675)
            {
                SpawnMarkerMonster(cur + (k * 2));
                count ++;
            }
        }
        FrameTimerWithArg(1, cur + 60, InitiGame);
    }
    else
        EndScan(count);
}

int ColorMaiden(int red, int grn, int blue, int wp)
{
    int unit = CreateObject("Bear2", wp);
    int ptr1 = GetMemory(0x750710), k, num;

    SetMemory(ptr1 + 4, 1385);
    UnitLinkBinScript(unit, MaidenBinTable());
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

void DemonDeadHandler()
{
    DLord[DemonMax] ++;
    CheckDemonDeathCount(DLord[DemonMax]);
    Delete(GetTrigger() + 1);
    UnitDeathHandler();
}

void DemonSightEvent()
{
    if (DistanceUnitToUnit(self, other) < 98.0)
	{
		DeleteObjectTimer(CreateObjectAt("MeteorExplode", GetObjectX(other), GetObjectY(other)), 9);
		Damage(other, self, 35, 0);
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
	if (!CurrentHealth(target))
	{
		Raise(GetTrigger() + 1, ToFloat(0));
		CreatureIdle(self);
	}
}

void ZombieDeadHandler()
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(self), GetObjectY(self));

    Raise(ptr, ToFloat(GetTrigger()));
    FrameTimerWithArg(23, ptr, ReviveZombie);
}

void ReviveZombie(int ptr)
{
    int unit = GetOwner(ptr);

    if (MaxHealth(unit))
    {
        MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
        DeleteObjectTimer(CreateObject("MagicSpark", 1), 6);
        AudioEvent("BeholderMove", 1);
        RaiseZombie(unit);
    }
    else
    {
        MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
        CallFunctionWithArgInt(FieldItemFuncPtr() + Random(0, 7), 1);
        GreenSparkFx(GetObjectX(ptr), GetObjectY(ptr));
    }
    Delete(ptr);
}

void PlantDeathHandler()
{
    Delete(GetTrigger() + 1);
    UnitDeathHandler();
}

void PlantDetectHandler()
{
    int ptr = GetTrigger() + 1;
    int enemy = ToInt(GetObjectZ(ptr));

    if (GetCaller() ^ enemy)
    {
        LookAtObject(self, other);
        AggressionLevel(self, 1.0);
        CreatureFollow(self, other);
        Raise(ptr, ToFloat(GetCaller()));
    }
}

void PlantLostEnemyEvent()
{
    int ptr = GetTrigger() + 1;
    int enemy = ToInt(GetObjectZ(ptr));

    if (!CurrentHealth(enemy))
    {
        Raise(ptr, 0.0);
        HitLocation(self, GetObjectX(self), GetObjectY(self));
        Enchant(self, "ENCHANT_BLINDED", 0.09);
    }
}

void CaptainDetectHandler()
{
	if (!HasEnchant(self, "ENCHANT_VILLAIN"))
	{
		Enchant(self, "ENCHANT_VILLAIN", 0.8);
		AbsoluteTargetStrike(GetTrigger(), GetCaller(), DistanceUnitToUnit(self, other) / 66.0, GetJandorShotFunc());
	}
	Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void PurpleGirlDetectHandler()
{
    AbsoluteTargetStrike(GetTrigger(), GetCaller(), DistanceUnitToUnit(self, other) / 30.0, GetSingleMissileShotFunc());
    Enchant(self, "ENCHANT_BLINDED", 0.0);
}

void PurpleGirlLostEnemyOnSight()
{
    EnchantOff(self, "ENCHANT_BLINDED");
}

void PurpleGirlDead()
{
    UnitDeathHandler();
}

void AbsoluteTargetStrike(int owner, int target, float threshold, int func)
{
    int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(target), GetObjectY(target));
    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner)), threshold);

    SetOwner(owner, unit);
    Raise(unit, ToFloat(target));
    FrameTimerWithArg(1, unit, func);
}

void SingleMagicMissileShot(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));
    float dt = Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(target), GetObjectY(target));
    float vectX = UnitRatioX(target, ptr, dt), vectY = UnitRatioY(target, ptr, dt);

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        MoveObject(ptr, GetObjectX(target) + (vectX * GetObjectZ(ptr + 1)), GetObjectY(target) + (vectY * GetObjectZ(ptr + 1)));
        if (IsVisibleTo(ptr, owner))
            SingleMissileShooter(owner, ptr);
        else
            SingleMissileShooter(owner, target);
    }
    Delete(ptr);
    Delete(ptr + 1);
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

int GetSingleMissileShotFunc()
{
    StopScript(SingleMagicMissileShot);
}

int GetJandorShotFunc()
{
    StopScript(JandorShotGun);
}

void JandorShotGun(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), mis;
    float dt = DistanceUnitToUnit(ptr, target);
    float vectX = UnitRatioX(target, ptr, dt), vectY = UnitRatioY(target, ptr, dt);

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        mis = CreateObjectAt("OgreShuriken", GetObjectX(owner) + UnitRatioX(target, owner, 14.0), GetObjectY(owner) + UnitRatioY(target, owner, 14.0));
        SetOwner(owner, mis);
        MoveObject(ptr, GetObjectX(target) + (vectX * GetObjectZ(ptr + 1)), GetObjectY(target) + (vectY * GetObjectZ(ptr + 1)));
        if (IsVisibleTo(ptr, owner))
        {
            PushObject(mis, -40.0, GetObjectX(ptr), GetObjectY(ptr));
        }
        else
        {
            PushObject(mis, -40.0, GetObjectX(target), GetObjectY(target));
        }
        Enchant(mis, "ENCHANT_SHOCK", 0.0);
    }
    Delete(ptr);
}


void GreenSparkFx(float x, float y)
{
    int ptr = CreateObjectAt("MonsterGenerator", x, y);

    Damage(ptr, 0, 10, 100);
    Delete(ptr);
}

void InitiPlayerCamera(int unit)
{
	int ptr = UnitToPtr(unit);

	if (ptr) SetMemory(GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0xe58, 0);
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
	for (i = 0 ; i < count ; i ++)
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
		for (i = 0 ; i < max ; i ++)
			CastSpellObjectObject("SPELL_LIGHTNING", ptr + i, ptr + i + 1);
	}
}

void BerserkerNoDelayCore(int plr)
{
    int arr[10];

    if (!MaxHealth(arr[plr]))
    {
        arr[plr] = CreateObject("Bear2", 56 + plr);
        UnitLinkBinScript(CreateObjectAt("Rat", GetObjectX(arr[plr]), GetObjectY(arr[plr]) + 23.0) - 1, Bear2BinTable());
        SetOwner(player[plr], arr[plr]);
        LookAtObject(arr[plr], arr[plr] + 1);
        HitLocation(arr[plr], GetObjectX(arr[plr] + 1), GetObjectY(arr[plr] + 1));
        FrameTimerWithArg(3, arr[plr], RemoveCoreUnits);
    }
}

void RemoveCoreUnits(int ptr)
{
    Delete(ptr);
    Delete(ptr + 1);
}

void SplashHandler(int owner, int func, float x, float y, float range)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", range, y) + 1, k;

    SetOwner(owner, ptr - 1);
    Raise(ptr - 1, ToFloat(func));
    for (k = 0 ; k < 8 ; k ++)
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

void SetUnitHealth(int unit, int amount)
{
    Damage(unit, 0, CurrentHealth(unit) - 1, -1);
    Pickup(unit, CreateObject("RottenMeat", 1));
    Damage(unit, 0, CurrentHealth(unit) - amount, -1);
}

int RepairAll(int unit)
{
    int inv = GetLastItem(unit), count = 0;

    while (IsObjectOn(inv))
    {
        if (MaxHealth(inv) ^ CurrentHealth(inv))
        {
            RestoreHealth(inv, MaxHealth(inv) - CurrentHealth(inv));
            count ++;
        }
        inv = GetPreviousItem(inv);
    }
    return count;
}

void WizRunAway()
{
	if (HasEnchant(self, "ENCHANT_ANTI_MAGIC"))
	{
		EnchantOff(self, "ENCHANT_ANTI_MAGIC");
	}
}

void StrVictory()
{
	int arr[38], i, count = 0;
	string name = "HealOrb";
	float pos_x = GetWaypointX(1), pos_y = GetWaypointY(1);

	arr[0] = 2082988096; arr[1] = 1132584967; arr[2] = 2021689456; arr[3] = 2101523; arr[4] = 131108; arr[5] = 1140867072; arr[6] = 143132800; arr[7] = 2115109080; 
	arr[8] = 670301187; arr[9] = 71320830; arr[10] = 25364616; arr[11] = 805306898; arr[12] = 773070848; arr[13] = 71566400; arr[14] = 2139748352; arr[15] = 283233795; 
	arr[16] = 35660322; arr[17] = 83870788; arr[18] = 8; arr[19] = 286355728; arr[20] = 36569632; arr[21] = 2139358208; arr[22] = 143261568; arr[23] = 17829902; 
	arr[24] = 35619362; arr[25] = 134234116; arr[26] = 142674056; arr[27] = 536936720; arr[28] = 1069679136; arr[29] = 71597952; arr[30] = 8915199; arr[31] = 2131763473; 
	arr[32] = 536870978; arr[33] = 2084602936; arr[34] = 1619574667; arr[35] = 534839311; arr[36] = 1107304448; arr[37] = 1074004095; 
	for (i = 0 ; i < 38 ; i ++)
		count = DrawStrVictory(arr[i], name, count);
	MoveWaypoint(1, pos_x, pos_y);
}

int DrawStrVictory(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 1178 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 1);
		if (count % 108 == 107)
			MoveWaypoint(1, GetWaypointX(1) - 214.0, GetWaypointY(1) + 2.5);
		else
			MoveWaypoint(1, GetWaypointX(1) + 2.0, GetWaypointY(1));
		count ++;
	}
	return count;
}

void StartGameMent()
{
    UniPrintToAll("오늘은 지겨운 일상속에서 벗어나보세요");
    UniPrintToAll("지금 당신에게 주어진 임무는 도심의 한 지하철역을 테러하는 것이에요");
    UniPrintToAll("이곳의 모든 사람들은 이미 당신을 테러리스트로 간주하여 사살을 시도할 것입니다");
    SecondTimer(2, GameMent2);
}

void GameMent2()
{
    UniPrintToAll("따라서 당신은 지하철 역 내 모든 사람을 죽이고 최종 목적지에 도달하면 승리합니다");
    UniPrintToAll("시작 위치에는 장비를 구입할 수 있는 상점과 특수용품 숍이 구축되어 있으니 많은 이용바랍니다");
}