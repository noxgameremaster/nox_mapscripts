
#include "builtins.h"
#include "libs\typecast.h"
#include "libs\printutil.h"
#include "libs\unitutil.h"
#include "libs\mathlab.h"
#include "libs\callmethod.h"
#include "libs\fxeffect.h"
#include "libs\fixtellstory.h"
#include "libs\playerupdate.h"
#include "libs\coopteam.h"
#include "libs\spellutil.h"
#include "libs\itemproperty.h"
#include "libs\username.h"
#include "libs\reaction.h"
#include "libs\unitstruct.h"
#include "libs\waypoint.h"
#include "libs\walls.h"


#define NULLPTR         0 
#define ITEM_NULL_PROPERTY      0x5a00a4

#define SELF -2
#define OTHER -1

int player[20];
int MapLang = 0;
int Gvar_6[10];
int Gvar_9[40]; //wizard
int tower[25];
int MobFieldFunc;
int MapLastUnit;
int HCenterUnit;
int PlrInven[10], PlrNodePtr[10];


int DrawMagicIcon(int wp)
{
    int unit = CreateObject("AirshipBasketShadow", wp);
    int *ptr = 0x750710;
    
    ptr = ptr[0];
    ptr[1] = 1416;
    
    return unit;
}

void BomberSetMonsterCollide(int bombUnit)
{
    int ptr = UnitToPtr(bombUnit);

    if (ptr)
        SetMemory(ptr + 0x2b8, 0x4e83b0);
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
		arr[0] = 1684627789; arr[1] = 28261; arr[24] = 1065688760; arr[27] = 1; arr[28] = 1106247680; 
		arr[29] = 35; arr[30] = 1101004800; arr[31] = 2; arr[32] = 22; arr[33] = 30; 
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
		arr[0] = 1852796743; arr[19] = 80; arr[24] = 1066192077; arr[26] = 4; arr[28] = 1106247680; 
		arr[29] = 25; arr[30] = 1092616192; arr[31] = 4; arr[32] = 22; arr[33] = 30; 
		arr[34] = 2; arr[35] = 3; arr[36] = 20; arr[57] = 5548176; arr[58] = 5546608; 
		arr[59] = 5543680; 
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
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811; arr[24] = 1065353216; 
		arr[26] = 4; arr[28] = 1101004800; arr[29] = 20; arr[31] = 8; arr[32] = 8; 
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
		arr[0] = 1667329090; arr[1] = 1684625259; arr[2] = 30575; arr[24] = 1069547520; arr[26] = 4; 
		arr[27] = 3; arr[28] = 1097859072; arr[29] = 25; arr[31] = 8; arr[32] = 13; 
		arr[33] = 21; arr[34] = 50; arr[35] = 3; arr[36] = 9; arr[37] = 1684631635; 
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
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972; arr[17] = 85; arr[18] = 25; 
		arr[19] = 90; arr[21] = 1065353216; arr[23] = 65544; arr[24] = 1065353216; arr[37] = 1801545047; 
		arr[38] = 1701996870; arr[39] = 1819042146; arr[53] = 1128792064; arr[55] = 15; arr[56] = 21; 
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
		arr[18] = 100; arr[19] = 60; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1067869798; 
		arr[26] = 4; arr[28] = 1106247680; arr[29] = 20; arr[31] = 8; arr[32] = 12; 
		arr[33] = 20; arr[57] = 5547984; arr[58] = 5546320; arr[59] = 5542432; 
		link = &arr;
	}
	return link;
}

int LichLord2BinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751345484; arr[1] = 1685221196; arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; 
		arr[27] = 7; arr[28] = 1108082688; arr[29] = 50; arr[30] = 1106247680; arr[31] = 11; 
		arr[32] = 13; arr[33] = 21; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int LichLordBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751345484; arr[1] = 1685221196; arr[17] = 20; arr[19] = 100; arr[21] = 1065353216; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; arr[28] = 1108082688; 
		arr[29] = 50; arr[30] = 1092616192; arr[32] = 9; arr[33] = 17; arr[57] = 5548288; 
		arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int HecubahWithOrbBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 1466458466; arr[2] = 1332245609; arr[3] = 25202; arr[17] = 250; 
		arr[18] = 100; arr[19] = 90; arr[21] = 1065353216; arr[24] = 1066192077; arr[25] = 1; 
		arr[26] = 6; arr[27] = 5; arr[37] = 1952539972; arr[38] = 1818313320; arr[39] = 1634879084; 
		arr[40] = 1852140903; arr[41] = 116; arr[53] = 1133903872; arr[55] = 19; arr[56] = 25; 
		link = &arr;
	}
	return link;
}

int BomberGreenBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1651339074; arr[1] = 1917284965; arr[2] = 7234917; arr[17] = 20; arr[19] = 80; 
		arr[21] = 1065353216; arr[24] = 1065353216; arr[37] = 1819043161; arr[38] = 1951627119; arr[39] = 1750299233; 
		arr[40] = 29807; arr[53] = 1128792064; arr[55] = 7; arr[56] = 11; arr[57] = 5548112; 
		arr[58] = 5547856; 
		link = &arr;
	}
	return link;
}

int HecubahBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 6840674; arr[17] = 300; arr[19] = 100; arr[21] = 1065353216; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; arr[28] = 1108082688; 
		arr[29] = 50; arr[30] = 1092616192; arr[32] = 10; arr[33] = 18; arr[57] = 5548288; 
		arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int TalkingSkullBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1802264916; arr[1] = 1399287401; arr[2] = 1819047275; arr[24] = 1065353216; arr[28] = 1106247680; 
		arr[29] = 20; arr[31] = 11; arr[32] = 9; arr[33] = 17; arr[58] = 5546320; 
		arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

int GreenFrogBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701147207; arr[1] = 1869760110; arr[2] = 103; arr[24] = 1065353216; arr[28] = 1101004800; 
		arr[29] = 20; arr[31] = 10; arr[32] = 6; arr[33] = 11; arr[59] = 5544320; 
		link = &arr;
	}
	return link;
}

void ImportBinTable()
{
    WizardRedBinTable();
    BlackWidowBinTable();
    FireSpriteBinTable();
    MaidenBinTable();
    AirshipCaptainBinTable();
    StrongWizardWhiteBinTable();
    WeirdlingBeastBinTable();
    GoonBinTable();
}

void InitMapDecorations()
{
    FrameTimerWithArg(1, 0, InitGauntletChest);
    FrameTimerWithArg(2, 1, InitGauntletChest);
    FrameTimerWithArg(3, 2, InitGauntletChest);
    FrameTimerWithArg(4, 3, InitGauntletChest);
    FrameTimerWithArg(5, 4, InitGauntletChest);
}

int CheckGameKorLanguage()
{
    if (GetMemory(0x611c04))
        return GetMemory(GetMemory(GetMemory(0x611c04))) == 0xc774d300;
    return 0;
}

void MapInitialize()
{
    MapLastUnit = CreateObject("InvisibleLightBlueLow", 1);
    MusicEvent();
    ImportBinTable();

    InitMathSine(1);

    MasterUnit();
    initializeGuardTower(25);
    
    MapLang = !CheckGameKorLanguage();
    ObjectOn(Object("mainSwitchForPlayer"));
    FrameTimer(10, PreservePlayerLoop);
    FrameTimer(20, loopIsVisbleTower);
    FrameTimerWithArg(50, 0, HostPlayerTeleport);
    FrameTimer(5, InitMapDecorations);
    FrameTimer(1, SetGameTypeCoopMode);
}

int MasterUnit()
{
    int unit;

    if (!unit)
    {
        MoveWaypoint(137, 5000.0, 200.0);
        unit = CreateObject("Hecubah", 137);
        Frozen(unit, 1);
    }
    return unit;
}

void TurnOffLights()
{
    int door;

    if (!door)
        door = Object("ServiceDoor");
    if (IsLocked(door))
    {
        UniPrint(OTHER, "도시 지하 공동구로 향하는 엘리베이터의 게이트가 열렸습니다.");
        UnlockDoor(door);
    }
    else
    {
        UniPrint(OTHER, "게이트를 잠궜습니다.");
        LockDoor(door);
    }
}

void initializeGuardTower(int max)
{
    int k;
    for (k = max - 1 ; k >= 0 ; k --)
        tower[k] = Object("attackTop" + IntToString(k + 1));
}

int CheckPlayerSkillFlag(int sPlr)
{
    return player[sPlr + 10] & 0x02;
}

int CheckPlayerSkillFlag2(int sPlr)
{
    return player[sPlr + 10] & 0x04;
}

int CheckPlayerSkillFlag3(int sPlr)
{
    return player[sPlr + 10] & 0x08;
}

void SetPlayerSkillFlag(int sPlr)
{
    player[sPlr + 10] = player[sPlr + 10] ^ 0x02;
}

void SetPlayerSkillFlag2(int sPlr)
{
    player[sPlr + 10] = player[sPlr + 10] ^ 0x04;
}

void SetPlayerSkillFlag3(int sPlr)
{
    player[sPlr + 10] = player[sPlr + 10] ^ 0x08;
}

int CheckPlayerDeathFlag(int sPlr)
{
    return player[sPlr + 10] & 0x80000000;
}

void SetPlayerDeathFlag(int sPlr)
{
    player[sPlr + 10] = player[sPlr + 10] ^ 0x80000000;
}

void EmptyAll(int sUnit)
{
    while (GetLastItem(sUnit))
        Delete(GetLastItem(sUnit));
}

void PlayerJoinTheMap(int sPlr)
{
    if (CheckPlayerDeathFlag(sPlr))
        SetPlayerDeathFlag(sPlr);
    if (CheckPlayerSkillFlag3(sPlr))
        PlayerAllEnchantments(player[sPlr]);
    Enchant(player[sPlr], "ENCHANT_ANCHORED", 0.0);
    MoveObject(player[sPlr], GetWaypointX(14), GetWaypointY(14));
    DeleteObjectTimer(CreateObject("BlueRain", 14), 20);
    AudioEvent("BlindOff", 14);
}

void PlayerFailJoin(int sUnit)
{
    Enchant(sUnit, "ENCHANT_FREEZE", 0.0);
    Enchant(sUnit, "ENCHANT_BLINDED", 0.0);
    Enchant(sUnit, "ENCHANT_ANTI_MAGIC", 0.0);
    Enchant(sUnit, "ENCHANT_ANCHORED", 0.0);
    MoveObject(sUnit, GetWaypointX(15), GetWaypointY(15));
    if (HasClass(sUnit, "PLAYER"))
    {
        UniPrint(sUnit, "지금 지도에 플레이어가 너무 많아서 입장하실 수 없습니다");
        UniPrint(sUnit, "Failed join the map- Too may players on the map");
    }
}

void PlayerClassOnInit(int plr, int unit)
{
    player[plr] = unit;
    player[plr + 10] = 1;
    ChangeGold(unit, -GetGold(unit));
    SelfDamageClassEntry(unit);
    DiePlayerHandlerEntry(unit);
}

void getPlayer()
{
    int plr, i;

    if (CurrentHealth(OTHER))
    {
        plr = CheckPlayer();
        for (i = 9 ; i >= 0 && plr < 0 ; i --)
        {
            if (!MaxHealth(player[i]))
            {
                PlayerClassOnInit(i, GetCaller());
                plr = i;
                break;
            }
        }
        if (plr + 1)
            PlayerJoinTheMap(plr);
        else
            PlayerFailJoin(OTHER);
    }
}

void CancelPlayerDialog(int plrUnit)
{
    int temp = UnitToPtr(plrUnit);

    if (temp)
        CancelPlayerDialogWithPTR(temp);
}

void PlayerFree(int sPlr)
{
    CancelPlayerDialog(player[sPlr]);
    player[sPlr + 10] = 0;
    player[sPlr] = 0;
}

void PlayerOnDeath(int sPlr)
{
    if (MapLang)
        UniPrintToAll(PlayerIngameNick(player[sPlr]) + " was shot down by the enemy.");
    else
        UniPrintToAll(PlayerIngameNick(player[sPlr]) + " 님께서 적에게 격추되었습니다");
}

void PlayerCheckUseSkill(int sPlr)
{
    if (HasEnchant(player[sPlr], "ENCHANT_SNEAK"))
    {
        EnchantOff(player[sPlr], "ENCHANT_SNEAK");
        RemoveTreadLightly(player[sPlr]);
        if (CheckPlayerSkillFlag2(sPlr))
            WindBooster(player[sPlr]);
    }
    else if (CheckPlayerSkillFlag(sPlr))
    {
        if (CheckPlayerInput(player[sPlr]) == 47)
            ShotRailGun(player[sPlr]);
    }
    if (CheckPlayerSkillFlag3(sPlr))
        PlayerPoisonAutoCure(player[sPlr]);
}

void PreservePlayerLoop()
{
    int i;

    for (i = 9 ; i >= 0 ; i -= 1)
    {
        while (1)
        {
            if (MaxHealth(player[i]))
            {
                if (GetUnitFlags(player[i]) & 0x40)
                    1;
                else if (CurrentHealth(player[i]))
                {
                    PlayerCheckUseSkill(i);
                    break;
                }
                else
                {
                    if (CheckPlayerDeathFlag(i)) break;
                    else
                    {
                        PlayerOnDeath(i);
                        SetPlayerDeathFlag(i);
                    }
                    break;
                }
            }
            if (player[i + 10])
                PlayerFree(i);
            break;
        }
    }
    FrameTimer(1, PreservePlayerLoop);
}

int DummyUnitCreateAt(string sName, float sX, float sY)
{
    int dUnit = CreateObjectAt(sName, sX, sY);

    ObjectOff(dUnit);
    Damage(dUnit, 0, MaxHealth(dUnit) + 1, -1);
    Frozen(dUnit, 1);
    return dUnit;
}

void RailGunLoop(int gUnit)
{
    int owner = GetOwner(gUnit), dUnit, count = GetDirection(gUnit);

    while (IsObjectOn(gUnit))
    {
        if (CurrentHealth(owner) && count)
        {
            if (IsVisibleTo(gUnit, gUnit + 1) || IsVisibleTo(gUnit + 1, gUnit))
            {
                dUnit = DummyUnitCreateAt("Demon", GetObjectX(gUnit), GetObjectY(gUnit));
                SetOwner(owner, dUnit);
                DeleteObjectTimer(dUnit, 1);
                SetCallback(dUnit, 9, DeathTouched);
                MoveObject(gUnit, GetObjectX(gUnit) + GetObjectZ(gUnit), GetObjectY(gUnit) + GetObjectZ(gUnit + 1));
                LookWithAngle(gUnit, count - 1);
                FrameTimerWithArg(1, gUnit, RailGunLoop);
                Effect("LIGHTNING", GetObjectX(gUnit), GetObjectY(gUnit), GetObjectX(gUnit + 1), GetObjectY(gUnit + 1));
                break;
            }
        }
        Delete(gUnit);
        Delete(gUnit + 1);
        break;
    }
}

void ShotRailGun(int sUnit)
{
    int unit;

    if (CurrentHealth(sUnit))
    {
        if (HasEnchant(sUnit, "ENCHANT_CROWN"))
        {
            if (MapLang)
                UniPrint(sUnit, "please wait... cooldown");
            else
                UniPrint(sUnit, "쿨다운 입니다...");
        }
        else
        {
            unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(sUnit) + UnitAngleCos(sUnit, 25.0), GetObjectY(sUnit) + UnitAngleSin(sUnit, 25.0));
            SetOwner(sUnit, CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit), GetObjectY(unit)) - 1);
            LookWithAngle(unit, 24);
            Raise(unit, UnitAngleCos(sUnit, 24.0));
            Raise(unit + 1, UnitAngleSin(sUnit, 24.0));
            FrameTimerWithArg(1, unit, RailGunLoop);
            if (MapLang)
                UniChatMessage(sUnit, "MISAKA: Rail Gun!!", 90);
            else
                UniChatMessage(sUnit, "레일건!!", 90);
            Enchant(sUnit, "ENCHANT_CROWN", 10.0);
        }
    }
}

void WindBooster(int sUnit)
{
    PushObjectTo(sUnit, UnitAngleCos(sUnit, 108.0), UnitAngleSin(sUnit, 108.0));
    Effect("RICOCHET", GetObjectX(sUnit), GetObjectY(sUnit), 0.0, 0.0);
    Enchant(sUnit, "ENCHANT_RUN", 0.1);
}

int checkMapBoundary(int location)
{
    float pos_x = GetWaypointX(location);
    float pos_y = GetWaypointY(location);

    if (pos_x > 100.0 && pos_y > 100.0 && pos_x < 5532.0 && pos_y < 5600.0)
        return 1;
    else
        return 0;
}

void DeathTouched()
{
    int owner = GetOwner(SELF);

    if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
    {
        Damage(OTHER, owner, 200, 13);
        Enchant(OTHER, "ENCHANT_CHARMING", 0.8);
    }
}

void InitStack()
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
        StackIndex(k);
}

int StackIndex(int num)
{
    int arr[10];
    int cur;

    if (num < 10) //push_data
    {
        if (cur == 10)
        {
            UniPrintToAll("스택이 포화상태 입니다.");
            return -1;
        }
        arr[cur] = num;
        cur ++;
        return 0;
    }
    if (!cur)
    {
        UniPrintToAll("스택이 비어있습니다.");
        return -1;
    }
    cur --;
    return arr[cur];
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

void buyYourPet()
{
    int unit;
    int idx;
    int plr = CheckPlayer();
    //location: 134
    if (GetGold(OTHER) >= 70000 && plr >= 0)
    {
        idx = StackIndex(10);
        if (idx >= 0)
        {
            if (MapLang)
                UniPrint(OTHER, "I purchased a bodyguard.");
            else
                UniPrint(OTHER, "잔도를 구입하셨습니다.");
            MoveWaypoint(134, GetObjectX(OTHER), GetObjectY(OTHER));
            unit = CreateObject("AirshipCaptain", 134);
            CreateObject("InvisibleLightBlueHigh", 134);
            LookWithAngle(unit + 1, plr);
            Raise(unit + 1, ToFloat(idx));
            SetOwner(player[plr], unit);
            ChangeGold(OTHER, -70000);
            SetCallback(unit, 3, XandorWeapon);
            SetCallback(unit, 5, XandorDead);
            SetDialog(unit, "NORMAL", guardUnit, nullPointer);
        }
    }
    else
    {
        if (MapLang)
            UniPrintToAll("There is not enough gold coins. The bodyguard is 70,000 won.");
        else
            UniPrintToAll("금화가 부족합니다. 잔도는 7만원 입니다.");
    }
}

void XandorDead()
{
    int ptr = GetTrigger() + 1;

    DeleteObjectTimer(SELF, 60);
    StackIndex(ToInt(GetObjectZ(ptr)));
    Delete(ptr);
}

void XandorWeapon()
{
    int plr = GetDirection(GetTrigger() + 1);

    if (!MaxHealth(player[plr]))
        Damage(SELF, 0, 9999, 14);
    else if (CurrentHealth(SELF))
    {
        if (CurrentHealth(OTHER))
        {
            Effect("SENTRY_RAY", GetObjectX(SELF), GetObjectY(SELF), GetObjectX(OTHER), GetObjectY(OTHER));
            Damage(OTHER, player[plr], 75, 16);
        }
        if (!HasEnchant(SELF, "ENCHANT_VILLAIN"))
        {
            Enchant(SELF, "ENCHANT_VILLAIN", 0.0);
            FrameTimerWithArg(22, GetTrigger(), XandorResetSight);
        }
    }
}

void XandorResetSight(int unit)
{
    EnchantOff(unit, "ENCHANT_VILLAIN");
    Enchant(unit, "ENCHANT_BLINDED", 0.1);
    AggressionLevel(unit, 1.0);
}

void guardUnit()
{
    if (!IsAttackedBy(SELF, OTHER) && !HasEnchant(OTHER, "ENCHANT_VILLAIN"))
    {
        Enchant(OTHER, "ENCHANT_VILLAIN", 3.0);
        CreatureFollow(SELF, OTHER);
        AggressionLevel(SELF, 0.83);
        if (MapLang)
            UniChatMessage(SELF, "잔도, 남은체력: " + IntToString(CurrentHealth(SELF)), 150);
        else
            UniChatMessage(SELF, "Jandor, HP: " + IntToString(CurrentHealth(SELF)), 150);
        MoveWaypoint(141, GetObjectX(SELF), GetObjectY(SELF));
        AudioEvent("BigGong", 141);
    }
}

void nullPointer()
{ }

void OrbClassCollide()
{
    int owner = GetOwner(SELF);

    while (1)
    {
        if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
        {
            Damage(OTHER, owner, 300, 14);
            Enchant(OTHER, "ENCHANT_CHARMING", 0.3);
            Effect("YELLOW_SPARKS", GetObjectX(SELF), GetObjectY(SELF), 0.0, 0.0);
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(SELF);
        else
            break;
        Delete(SELF);
        break;
    }
}

int OrbClassTakeShot(int sOwner)
{
    int mis = CreateObjectAt("DeathBall", GetObjectX(sOwner) + UnitAngleCos(sOwner, 17.0), GetObjectY(sOwner) + UnitAngleSin(sOwner, 17.0));
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(mis, OrbClassCollide);
    SetOwner(sOwner, mis);
    return mis;
}

void OrbClassUseStaff()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(SELF);

    if (MathAbs(cFps - cTime) < 12)
        return;
    if (CurrentHealth(other))
    {
        SetUnit1C(SELF, cFps);
        PushObject(OrbClassTakeShot(other), 22.0, GetObjectX(other), GetObjectY(other));
        PlaySoundAround(other, 221);
    }
}

int OrbClassCreate(int sUnit)
{
    int staff = CreateObjectAt("OblivionOrb", GetObjectX(sUnit), GetObjectY(sUnit));

    OblivionSetting(staff);
    SetUnitCallbackOnUseItem(staff, OrbClassUseStaff);
    return staff;
}

void ShopClassTradeOrb()
{
    if (GetAnswer(SELF) ^ 1) return;
    if (GetGold(OTHER) >= 50000)
    {
        EnchantOff(OTHER, "ENCHANT_INVULNERABLE");
        EnchantOff(OTHER, "ENCHANT_SHIELD");
        SetUnitHealth(OTHER, 3000);
        ChangeGold(OTHER, -50000);
        OrbClassCreate(OTHER);
        TellStoryUnitName("oAo", "GUITrade.c:TradeVendorAccept", "씹사기 지팡이\n구입 완료!");
        UniPrint(OTHER, "거래가 완료되었습니다! 구입하신 망각의 지팡이는 당신 아래에 있어요");
    }
    else
    {
        if (MapLang)
            UniPrint(OTHER, " * There is not enough money. The purchase of a magic wand is 50,000 won..");
        else
            UniPrint(OTHER, " * 돈이 충분치 않습니다 . 망각의 지팡이 구입은 5만원 입니다 .");
    }
}

void ShopClassDescTradeOrb()
{
    TellStoryUnitName("oAo", "Wiz02:NecroSpiders", "씹사기 지팡이");
    UniPrint(OTHER, "망각의 지팡이를 구입하시겠어요? 1개당 50,000 골드가 요구됩니다");
}

int ShopClassOblivionOrbMarket(int sLocation)
{
    int shop = DummyUnitCreateAt("WizardWhite", GetWaypointX(sLocation), GetWaypointY(sLocation));

    LookWithAngle(shop, 160);
    SetDialog(shop, "YESNO", ShopClassDescTradeOrb, ShopClassTradeOrb);
    StoryPic(shop, "MaleWizardRobe");
    return shop;
}

void goIndoorDunmir() {
    MoveObject(OTHER, GetWaypointX(67), GetWaypointY(67));
}

void outIndoorDunmir() {
    MoveObject(OTHER, GetWaypointX(68), GetWaypointY(68));
}

void LoopManageKeys(int sArg)
{
    int key[5], ptr;

    if (IsLocked(GauntletGate(sArg)) && !GetDirection(key[sArg]))
    {
        key[sArg] = CreateObject(GauntletKeyName(sArg), sArg + 117);
        ptr = GetMemory(0x750710);
        SetMemory(ptr + 0x2c8, ImportAllowAllDrop());
        LookWithAngle(key[sArg], 1);
        if (MapLang)
            UniPrintToAll("Key " + GauntletKeyName(sArg) + " is Respawned.");
        else
            UniPrintToAll("열쇠 " + GauntletKeyName(sArg) + " 를 재 생성 했습니다.");
    }
    SecondTimerWithArg(3, (sArg + 1) % 5, LoopManageKeys);
}

void InitGateset()
{
    GauntletGate(-1);
    SecondTimerWithArg(5, 0, LoopManageKeys);
}

string GauntletKeyName(int sNum)
{
    string keylist[] = {"SilverKey", "GoldKey", "GoldKey", "SapphireKey", "RubyKey"};
    
    return keylist[sNum];
}

int GauntletGate(int num)
{
    int data[5];

    if (num < 0)
    {
        data[0] = Object("GauntletDoorR");
        data[1] = Object("noKeepGate1");
        data[2] = Object("gearGate");
        data[3] = Object("MidCityGate1");
        data[4] = Object("rubyDoor");
        return 0;
    }
    return data[num];
}

void CaptainTakeBreak() {
    if (CurrentHealth(SELF) < 5000 - 400) {
        UniChatMessage(SELF, "Rules.c:Deaths", 90);
        Damage(SELF, OTHER, 5000, 14);
    }
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

void MonsterDeathHandler()
{
    CallFunctionWithArgInt(ItemDropFuncPtr() + Random(0, 9), SELF);
    DeleteObjectTimer(SELF, 60);
}

void hurtEvent() {
    if (HasClass(OTHER, "WEAPON") && HasSubclass(OTHER,"CHAKRAM"))
        MoveObject(OTHER, GetObjectX(SELF), GetObjectY(SELF));
    else if (HasClass(OTHER, "MISSILE") && HasSubclass(OTHER, "MISSILE_COUNTERSPELL"))
    {
        CastSpellObjectObject("SPELL_COUNTERSPELL", SELF, SELF);
        RestoreHealth(SELF, 100);
    }
}
void collisionMonster()
{
    int plr;
    if (CurrentHealth(SELF) && HasClass(OTHER, "WEAPON") && HasSubclass(OTHER, "ARROW"))
    {
        plr = CheckOwner(OTHER);
        if (plr >= 0)
        {
            if (CurrentHealth(player[plr]) && MaxHealth(player[plr]) == 150)
            {
                MoveWaypoint(140, GetObjectX(SELF), GetObjectY(SELF));
                MoveObject(SELF, 4294.0, 4041.0);
                MoveObject(SELF, GetWaypointX(140), GetWaypointY(140));
                if (GetWaypointX(140) > 300.0)
                    DeleteObjectTimer(CreateObject("ForceOfNatureCharge", 140), 28);
                AudioEvent("CharmSuccess", 140);
                Damage(SELF, player[plr], 99, 13);
                Effect("SENTRY_RAY", GetWaypointX(140), GetWaypointY(140), GetObjectX(player[plr]), GetObjectY(player[plr]));
            }
        }
    }
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

void loopIsVisbleTower()
{
    int idx = idx % 25;
    int k;
    int mis;

    if (CurrentHealth(tower[idx]))
    {
        for (k = 9 ; k >= 0 ; k -= 1)
        {
            if (CurrentHealth(player[k]) && IsVisibleTo(tower[idx], player[k]))
            {
                MoveWaypoint(138, GetObjectX(tower[idx]) - UnitRatioX(tower[idx], player[k], 40.0), GetObjectY(tower[idx]) - UnitRatioY(tower[idx], player[k], 40.0));
                mis = CreateObject("DeathBallFragment", 138);
                SetOwner(MasterUnit(), mis);
                PushObject(mis, -47.0, GetObjectX(player[k]), GetObjectY(player[k]));
            }
        }
    }
    idx ++;
    FrameTimer(1, loopIsVisbleTower);
}

void settingFinalBoss()
{
    int boss = Object("FinalBoss");
    Enchant(boss, "ENCHANT_VAMPIRISM", 0.0);
    SetCallback(boss, 5, setDeathFromBoss);
    SetCallback(boss, 7, beAttackedByBoss);
    FrameTimerWithArg(10, boss, absoluteDurabilityFromArmor);
    FrameTimerWithArg(20, boss, absoluteDurabilityFromWeapon);
}

void absoluteDurabilityFromWeapon(int arg_0) {
    int var_0;
    var_0 = GetLastItem(arg_0);
    while(var_0 != 0) {
        if (HasClass(var_0, "WEAPON") && HasEnchant(var_0, "ENCHANT_INVULNERABLE") != true) {
            Enchant(var_0, "ENCHANT_INVULNERABLE", 0.0);
        }
        var_0 = GetPreviousItem(var_0);
    }
}
void absoluteDurabilityFromArmor(int arg_0) {
    int var_0;
    var_0 = GetLastItem(arg_0);
    while(var_0 != 0)
    {
        if (HasClass(var_0, "ARMOR") && !HasEnchant(var_0, "ENCHANT_INVULNERABLE"))
            Enchant(var_0, "ENCHANT_INVULNERABLE", 0.0);
        var_0 = GetPreviousItem(var_0);
    }
}
void beAttackedByBoss()
{
    int r_pic;

    if (CurrentHealth(SELF))
    {
        if (CurrentHealth(OTHER))
        {
            if (!HasEnchant(SELF,"ENCHANT_BURNING"))
            {
                r_pic = Random(0, 2);
                Enchant(SELF, "ENCHANT_BURNING", 6.0);
                if (!r_pic)
                {
                    MoveWaypoint(108, GetObjectX(SELF), GetObjectY(SELF));
                    CastSpellObjectLocation("SPELL_EARTHQUAKE", SELF, GetWaypointX(108), GetWaypointY(108));
                    CastSpellObjectLocation("SPELL_EARTHQUAKE", SELF, GetWaypointX(108), GetWaypointY(108));
                    DeleteObjectTimer(CreateObject("ManaBombCharge", 108), 60);
                    Enchant(SELF, "ENCHANT_INVULNERABLE", 0.0);
                    Enchant(SELF, "ENCHANT_FREEZE", 0.0);
                    AudioEvent("ManaBombCast", 108);
                    FrameTimerWithArg(57, GetTrigger(), manaBombStrike);
                }
                else if (r_pic == 1)
                {
                    straightFist();
                    FrameTimerWithArg(30, GetTrigger(), DisableBossStable);
                }
                else
                {
                    MoveWaypoint(108, GetObjectX(SELF), GetObjectY(SELF));
                    r_pic = CreateObject("InvisibleLightBlueHigh", 108);
                    CreateObject("InvisibleLightBlueHigh", 108);
                    Raise(r_pic, ToFloat(GetTrigger()));
                    Raise(r_pic + 1, ToFloat(GetCaller()));
                    FrameTimerWithArg(Random(1, 90), r_pic, BossCastMeteor);
                    if (!Random(0, 2))
                        FrameTimerWithArg(Random(91, 120), GetTrigger(), teleportToRandomLocation);
                }
            }
        }
        else {
            if (HasClass(OTHER, "FIRE"))
                teleportToRandomLocation(SELF);
            else if (HasClass(OTHER, "MISSILE"))
            {
                if (GetUnitThingID(OTHER) ^ 706) return;
                CastSpellObjectObject("SPELL_COUNTERSPELL", SELF, SELF);
                RestoreHealth(SELF, 30);
            }
        }
    }
}

void DisableBossStable(int unit)
{
    EnchantOff(unit, "ENCHANT_FREEZE");
    EnchantOff(unit, "ENCHANT_INVULNERABLE");
}

void manaBombStrike(int unit)
{
    int k;

    if (CurrentHealth(unit))
    {
        DisableBossStable(unit);
        Effect("WHITE_FLASH", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        CastSpellObjectLocation("SPELL_EARTHQUAKE", unit, GetObjectX(unit), GetObjectY(unit));
        for (k = 9 ; k >= 0 ; k --)
        {
            if (CurrentHealth(player[k]))
            {
                if (Distance(GetObjectX(unit), GetObjectY(unit), GetObjectX(player[k]), GetObjectY(player[k])) < 380.0)
                    Damage(player[k], unit, 150, 17);
            }
        }
    }
}
void straightFist()
{
    int k;
    int unit;
    float pos_x = UnitRatioX(SELF, other, -30.0);
    float pos_y = UnitRatioY(SELF, other, -30.0);
    string name = "Wizard";
    string magic = "SPELL_FIST";

    MoveWaypoint(108, GetObjectX(SELF) + pos_x, GetObjectY(SELF) + pos_y);
    Enchant(SELF, "ENCHANT_FREEZE", 1.0);
    Enchant(SELF, "ENCHANT_INVULNERABLE", 1.0);

    unit = CreateObject("InvisibleLightBlueHigh", 108);
    for (k = 0 ; k < 18 ; k ++)
    {
        MoveWaypoint(108, GetWaypointX(108) + pos_x, GetWaypointY(108) + pos_y);
        if (checkMapBoundary(108))
        {
            CreateObject(name, 108);
            DeleteObjectTimer(unit + (k * 2) + 1, 1);
            SetOwner(SELF, unit + (k * 2) + 1);
            CastSpellObjectLocation(magic, unit + (k * 2) + 1, GetWaypointX(108), GetWaypointY(108));
        }
    }
    Delete(unit);
}

void BossCastMeteor(int ptr)
{
    int boss = ToInt(GetObjectZ(ptr));
    int enemy = ToInt(GetObjectZ(ptr + 1));

    if (CurrentHealth(boss) && CurrentHealth(enemy))
    {
        if (MapLang)
            UniChatMessage(boss, "Meteor SHOT_!!", 90);
        else
            UniChatMessage(boss, "메테오_!!", 90);
        CastSpellObjectLocation("SPELL_METEOR", boss, GetObjectX(enemy), GetObjectY(enemy));
        CastSpellObjectLocation("SPELL_BURN", boss, GetObjectX(enemy), GetObjectY(enemy));
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void setDeathFromBoss()
{
    MoveWaypoint(108, GetObjectX(SELF), GetObjectY(SELF));
    Effect("WHITE_FLASH", GetWaypointX(108), GetWaypointY(108), 0.0, 0.0);
    Effect("JIGGLE", GetWaypointX(108), GetWaypointY(108), 75.0, 0.0);
    AudioEvent("FlagCapture", 108);
    if (MapLang)
        UniPrintToAll(" Victory__!! The final boss monster is dead __!! ");
    else
        UniPrintToAll(" 승리__!! 최종 보스를 죽이셨습니다 __!! ");
    StrVictory();
}

void teleportToRandomLocation(int arg_0) {
    int pic_idx = Random(0,6);
    int arr[7];

    if (!arr[0])
    {
        arr[0] = 13; arr[1] = 1194; arr[2] = 115; arr[3] = 1422; arr[4] = 1429; arr[5] = 1387; arr[6] = 116;
    }
    Effect("TELEPORT", GetObjectX(arg_0), GetObjectY(arg_0), 0.0, 0.0);
    MoveObject(arg_0, GetWaypointX(arr[pic_idx]), GetWaypointY(arr[pic_idx]));
    Effect("TELEPORT", GetObjectX(arg_0), GetObjectY(arg_0), 0.0, 0.0);
    AudioEvent("BlindOff", arr[pic_idx]);
}

void initializeSearchIndex(int sUnit)
{
    FrameTimerWithArg(1, sUnit, SearchAllNpcs);
    InitStack();
    LockDoor(Object("ServiceDoor"));
}

int GetUnitOffset1C(int sUnit)
{
    int ptr = UnitToPtr(sUnit);

    if (ptr)
        return GetMemory(ptr + 0x1c);
    return 0;
}

void NPCRemoveInventory(int sUnit)
{
    int iCount = GetUnitOffset1C(sUnit);
    int cur = sUnit + 2, i;

    for (i = 0 ; i < iCount ; i ++)
        Delete(cur + (i * 2));
}

void NpcArmorProperty(int sUnit)
{
    int aPtr;

    if (IsObjectOn(sUnit))
        SetArmorProperties(sUnit, 5, Random(3, 5), Random(0, 20), Random(0, 20));
}

void NPCSetInventoryCount(int sUnit)
{
    int ptr = UnitToPtr(sUnit), count = 0, cur;

    if (ptr)
    {
        cur = GetLastItem(sUnit);
        while (cur)
        {
            if (HasClass(cur, "ARMOR"))
                NpcArmorProperty(cur);
            count ++;
            cur = GetPreviousItem(cur);
        }
        SetMemory(ptr + 0x1c, count);
    }
}

void NPCDeathHandler()
{
    NPCRemoveInventory(GetTrigger());
    MonsterDeathHandler();
}

void ProcessNPCPrepare(int sUnit)
{
    SetCallback(sUnit, 5, NPCDeathHandler);
    SetCallback(sUnit, 7, hurtEvent);
    SetCallback(sUnit, 9, collisionMonster);
    NPCSetInventoryCount(sUnit);
}

void RewardClassProc(int sUnit)
{
    CallFunctionWithArgInt(ItemDropFuncPtr() + Random(0, 8), sUnit);
    Delete(sUnit);
}

void SearchResultHandler(int sUnit)
{
    int thingID = GetUnitThingID(sUnit);

    if (thingID == 1343)
        ProcessNPCPrepare(sUnit);
    else if (thingID == 2672)
        RewardClassProc(sUnit);
    else if (thingID == 2675)
        ProcessFieldMonsterMarker(sUnit);
    else if (thingID == 2670)
        ProcessAdvanceFieldMobMarker(sUnit);
}

void SearchAllNpcs(int sCur)
{
    int i;

    for (i = 0 ; i < 20 ; i ++)
        SearchResultHandler(sCur + (i * 2));
    if (sCur < MapLastUnit)
        FrameTimerWithArg(1, sCur + (i * 2), SearchAllNpcs);
    else
    {
        UnlockDoor(Object("StartIxDoorL"));
        UnlockDoor(Object("StartIxDoorR"));
        InitGateset();
        HCenterUnit = DrawMagicIcon(14);
        FrameTimer(1, settingFinalBoss);
        FrameTimer(2, PlaceWarSkillMarket);
        FrameTimer(15, InitObstacleCreate);
        if (MapLang)
            UniPrintToAll(" Unit searching Process is over... ");
        else
            UniPrintToAll(" 유닛 검색이 모두 끝났습니다 ... ");
        FrameTimer(30, StartBgmLoop);
        FrameTimer(3, MapSignSetting);
    }
}

void setJoney(int arg_0)
{
    JournalEntry(player[arg_0], "던미르 마을의 최종 보스를 죽이세요 .", 2);
    if (MaxHealth(player[arg_0]) == 150) {
        JournalEntry(player[arg_0], "전사의 스킬: 늑데의 눈, 작살, 조심스럽게 걷기", 1);
    }
}
void StrVictory()
{
	int arr[13];
	string name = "ManaBombOrb";
	int i = 0;
	arr[0] = 2613312; arr[1] = 301998097; arr[2] = 7080064; arr[3] = 1099186194; arr[4] = 35653889; arr[5] = 268762112; arr[6] = 33718242; arr[7] = 16777488; arr[8] = 132155394; arr[9] = 134217985; 
	arr[10] = 570458248; arr[11] = 2086650888; arr[12] = 536999970; 
	while(i < 13)
	{
		drawStrVictory(arr[i], name);
		i ++;
	}
}

void drawStrVictory(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(108);
		pos_y = GetWaypointY(108);
	}
	for (i = 1 ; i > 0 && count < 403 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 108);
		if (count % 38 == 37)
			MoveWaypoint(108, GetWaypointX(108) - 74.000000, GetWaypointY(108) + 2.000000);
		else
			MoveWaypoint(108, GetWaypointX(108) + 2.000000, GetWaypointY(108));
		count ++;
	}
	if (count >= 403)
	{
		count = 0;
		MoveWaypoint(108, pos_x, pos_y);
	}
}

void HorTalkStart()
{
    if (!IsAttackedBy(other, MasterUnit()))
        GoBackHome(other);
}
void WarriorListen() {
}
void WarriorIdle() {
}
void WarriorEngage() {
}

void noKeepHere()
{
    if (IsLocked(Object("noKeepGate1")))
    {
        if (MapLang)
            UniPrintToAll("Administrator: You can't go into an orchard without a key !");
        else
            UniPrintToAll("관리자: 열쇠 없이는 과수원에 들어갈 수 없지 !");
        MoveObject(other, GetWaypointX(116), GetWaypointY(116));
    }
}

void enterBossRoom()
{
    ObjectOff(SELF);
    if (MapLang)
        UniPrintToAll("You entered the final boss room.");
    else
        UniPrintToAll("보스 방에 입장하셨습니다.");
    FrameTimerWithArg(1, Object("FinalBoss"), ShowBossHp);
}

void ShowBossHp(int boss)
{
    if (CurrentHealth(boss))
    {
        if (MapLang)
            UniChatMessage(MasterUnit(), "FinalBossHP: " + IntToString(CurrentHealth(boss)) + "/ " + IntToString(MaxHealth(boss)), 90);
        else
            UniChatMessage(MasterUnit(), "보스 체력현황: " + IntToString(CurrentHealth(boss)) + "/ " + IntToString(MaxHealth(boss)), 90);
        SecondTimerWithArg(4, boss, ShowBossHp);
    }
}

void pickKeyThenTeleportNpcs()
{
    if (Distance(GetObjectX(other), GetObjectY(other), GetWaypointX(120), GetWaypointY(120)) < 200.0)
    {
        MoveObject(Object("dropNpc1"), GetWaypointX(120), GetWaypointY(120));
        MoveObject(Object("dropNpc2"), GetWaypointX(120), GetWaypointY(120));
        if (MapLang)
            UniChatMessage(Object("dropNpc1"), "Key thief! I will kill you", 150);
        else
            UniChatMessage(Object("dropNpc1"), "잡았다 요놈 ...! 흐흐흐 ...", 150);
        Effect("SMOKE_BLAST", GetWaypointX(120), GetWaypointY(120), 0.0, 0.0);
        ObjectOff(SELF);
    }
    else if (Distance(GetObjectX(other), GetObjectY(other), GetWaypointX(121), GetWaypointY(121)) < 200.0)
    {
        MoveObject(Object("dropNpc3"), GetWaypointX(121), GetWaypointY(121));
        MoveObject(Object("dropNpc4"), GetWaypointX(121), GetWaypointY(121));
        Effect("SMOKE_BLAST", GetWaypointX(121), GetWaypointY(121), 0.0, 0.0);
        ObjectOff(SELF);
    }
}

void SetUnitHealth(int unit, int amount)
{
    Damage(unit, 0, CurrentHealth(unit) - 1, -1);
    Pickup(unit, CreateObject("RottenMeat", 1));
    Damage(unit, 0, CurrentHealth(unit) - amount, -1);
}

void MovingSewerRows()
{
    int row, k;

    if (!row)
    {
        row = Object("SewerRowBase");
        AudioEvent("SpikeBlockMove", 73);
        ObjectOff(SELF);
    }
    if (GetObjectX(row) <= 1184.0)
    {
        for (k = 3 ; k >= 0 ; k --)
            MoveObject(row + (k*2), GetObjectX(row + (k* 2)) + 2.0, GetObjectY(row + (k*2)) + 2.0);
        FrameTimer(1, MovingSewerRows);
    }
}

void MapExit()
{
    ResetPlayerHandlerWhenExitMap();
    MusicEvent();
    RemoveCoopTeamMode();
    ResetHostileCritter();
}

void SetGameTypeCoopMode()
{
    MobFieldFunc = FieldMobSwordsman;
    MakeCoopTeam();
    SetHostileCritter();
}

string WeaponList(int sIdx)
{
    string weapons[] = {
        "WarHammer", "GreatSword", "Longsword", "Sword", "MorningStar",
        "OgreAxe", "BattleAxe", "CrossBow", "Bow", "StaffWooden",
        "RoundChakram", "OblivionHalberd", "OblivionHeart", "OblivionWierdling", "OblivionOrb"
    };
    return weapons[sIdx];
}

string PotionList(int sIdx)
{
    string potions[] = {
        "RedPotion", "BluePotion", "CurePoisonPotion", "VampirismPotion", "HastePotion",
        "InvisibilityPotion", "InfravisionPotion", "InvulnerabilityPotion", "ShieldPotion", "ShockProtectPotion",
        "PoisonProtectPotion", "FireProtectPotion", "YellowPotion", "BlackPotion", "WhitePotion"
    };
    return potions[sIdx];
}

string ArmorList(int sIdx)
{
    string armors[] = {
        "OrnateHelm", "Breastplate", "PlateArms", "PlateBoots", "PlateLeggings",
        "ChainCoif", "ChainLeggings", "ChainTunic", "SteelHelm", "SteelShield",
        "WoodenShield", "ConjurerHelm", "LeatherArmbands", "LeatherArmor", "LeatherArmoredBoots",
        "LeatherBoots", "LeatherHelm", "LeatherLeggings", "MedievalCloak", "MedievalPants",
        "MedievalShirt", "WizardRobe", "WizardHelm"
    };
    return armors[sIdx];
}

string OblivionList(int sIdx)
{
    string oblivion[] = {"OblivionHalberd", "OblivionHeart", "OblivionWierdling", "OblivionOrb"};

    return oblivion[sIdx];
}

string StaffList(int sIdx)
{
    string staffs[] = {
        "DeathRayWand", "FireStormWand", "ForceWand", "LesserFireballWand", "InfinitePainWand",
        "SulphorousFlareWand", "SulphorousShowerWand"
    };
    return staffs[sIdx];
}

void OblivionCopies(int sUnit)
{
    int nUnit = CreateObjectAt("OblivionHeart", GetObjectX(sUnit), GetObjectY(sUnit));
    int dPtr = GetMemory(0x750710), sPtr = UnitToPtr(sUnit);

    if (dPtr && sPtr)
    {
        SetMemory(dPtr + 0x1ec, GetMemory(sPtr + 0x1ec));
        SetMemory(dPtr + 0x1f0, GetMemory(sPtr + 0x1f0));
        SetMemory(dPtr + 4, GetMemory(sPtr + 4));
        SetMemory(dPtr + 0x2c4, GetMemory(sPtr + 0x2c4));
        SetMemory(dPtr + 0x2c8, GetMemory(sPtr + 0x2c8));
        SetMemory(dPtr + 0x2f8, GetMemory(sPtr + 0x2f8));
        SetMemory(GetMemory(dPtr + 0x2b4), GetMemory(GetMemory(sPtr + 0x2b4)));
        SetMemory(GetMemory(dPtr + 0x2b4) + 4, GetMemory(GetMemory(sPtr + 0x2b4) + 4));
        SetMemory(GetMemory(dPtr + 0x2b4) + 8, GetMemory(GetMemory(sPtr + 0x2b4) + 8));
        SetMemory(GetMemory(dPtr + 0x2b4) + 0xc, GetMemory(GetMemory(sPtr + 0x2b4) + 0xc));
    }
}

void OblivionDrop()
{
    if (MaxHealth(OTHER))
    {
        MoveObject(SELF, GetObjectX(OTHER), GetObjectY(OTHER));
        OblivionCopies(SELF);
    }
    Delete(SELF);
}

void FlareWandMissile(int sWand)
{
    int ptr = UnitToPtr(sWand), sAddr;

    if (ptr)
    {
        if (GetMemory(ptr + 4) == 227 || GetMemory(ptr + 4) == 228)
        {
            sAddr = GetMemory(ptr + 0x2e0) + 4;
            SetMemory(sAddr, 0x74616544);
            SetMemory(sAddr + 0x04, 0x6c614268);
            SetMemory(sAddr + 0x08, 0x6172466c);
            SetMemory(sAddr + 0x0c, 0x6e656d67);
            SetMemory(sAddr + 0x10, 0x74);
        }
    }
}

int ItemDropFuncPtr()
{
    StopScript(ItemDropHotPotion);
}

int ItemDropHotPotion(int sUnit)
{
    return CreateObjectAt("RedPotion", GetObjectX(sUnit), GetObjectY(sUnit));
}

int ItemDropPotions(int sUnit)
{
    return CheckPotionThingID(CreateObjectAt(PotionList(Random(0, 14)), GetObjectX(sUnit), GetObjectY(sUnit)));
}

int ItemDropWeapon(int sUnit)
{
    int wUnit = CreateObjectAt(WeaponList(Random(0, 13)), GetObjectX(sUnit), GetObjectY(sUnit));

    OblivionSetting(wUnit);
    if (Random(0, 3))
        SetWeaponProperties(wUnit, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    return wUnit;
}

int ItemDropArmor(int sUnit)
{
    int aUnit = CreateObjectAt(ArmorList(Random(0, 22)), GetObjectX(sUnit), GetObjectY(sUnit));

    if (Random(0, 3))
        SetArmorProperties(aUnit, Random(0, 5), Random(0, 5), Random(0, 20), Random(0, 20));
    return aUnit;
}

int ItemDropGold(int sUnit)
{
    string gName[] = {"QuestGoldChest", "QuestGoldPile", "Gold"};
    int gUnit = CreateObjectAt(gName[Random(0, 2)], GetObjectX(sUnit), GetObjectY(sUnit));
    int *ptr = 0x750710;
    int *vptr = ptr[0];
    int *goldValue = vptr[173];

    goldValue[0] = Random(1000, 5000);

    return gUnit;
}

int ItemDropGerms(int sUnit)
{
    string gName[] = {"Ruby", "Emerald", "Diamond", "Emerald", "Ruby"};
    
    return CreateObjectAt(gName[Random(0, 4)], GetObjectX(sUnit), GetObjectY(sUnit));
}

int ItemDropConsumeEquip(int sUnit)
{
    string wName[] = {"FanChakram", "Quiver"};
    int wUnit = CreateObjectAt(wName[Random(0, 1)], GetObjectX(sUnit), GetObjectY(sUnit));
    int *ptr = 0x750710;
    int *vptr = ptr[0];
    int *equipAmount = vptr[184];

    equipAmount[0] = 0xffff;
    if (Random(0, 5))
        SetWeaponProperties(wUnit, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    return wUnit;
}

int ItemMagicalStaff(int sUnit)
{
    int wand = CreateObjectAt(StaffList(Random(0, 6)), GetObjectX(sUnit), GetObjectY(sUnit));
    
    FlareWandMissile(wand);
    return wand;
}

int ItemDropSpecial(int sUnit)
{
    string sName[] = {"Fear", "AmuletofManipulation", "AnkhTradable", "AmuletofCombat", "AmuletofNature", "Candle1", "BottleCandle"};
    int itemUseFunction[] = {UseItemFunc1, UseItemFunc2, UseItemFunc3, UseItemFunc4, UseItemFunc5, UseItemFunc6, UseItemFunc7};
    int pic = Random(0, 6);
    int unit = CreateObjectAt(sName[pic], GetObjectX(sUnit), GetObjectY(sUnit));
    int *ptr = 0x750710;
    int *vptr = ptr[0];

    vptr[177] = 5190112;
    SetUnitCallbackOnUseItem(unit, itemUseFunction[pic]);
    return unit;
}

int ItemDropClassLichKey(int sUnit)
{
    return CreateObjectAt("RedOrbKeyOfTheLich", GetObjectX(sUnit), GetObjectY(sUnit));
}

int ItemOblivion(int sUnit)
{
    int wUnit = CreateObjectAt(OblivionList(Random(0, 2)), GetObjectX(sUnit), GetObjectY(sUnit));
    int *ptr = 0x750710;
    int *wptr = ptr[0];

    if (wptr != NULLPTR)
    {
        wptr[177] = 0x53a720;
        wptr[178] = ImportAllowAllDrop();
        SetWeaponProperties(wUnit, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    }
    return wUnit;
}

void OblivionSetting(int sUnit)
{
    int *wptr = UnitToPtr(sUnit);

    if (wptr != NULLPTR)
    {
        int thingId = wptr[1];
        if (thingId >= 222 && thingId <= 225)
        {
            wptr[177] = 0x53a720;
            wptr[178] = ImportAllowAllDrop();
        }
    }
}

void UnitTeleportToUnitAndHealBuffFx(int sUnit, int dUnit)
{
    int *dUnitPtr = UnitToPtr(dUnit);

    if (dUnitPtr != NULLPTR)
    {
        float xProfile = ToFloat(dUnitPtr[14]);
        float yProfile = ToFloat(dUnitPtr[15]);
        MoveObject(sUnit, xProfile, yProfile);
        Effect("GREATER_HEAL", xProfile, yProfile, xProfile, yProfile - 160.0);
    }
}

void BuffHealingHealth(int sUnit)
{
    int owner, count = GetDirection(sUnit);

    while (IsObjectOn(sUnit))
    {
        owner = GetOwner(sUnit);
        if (CurrentHealth(owner) && count)
        {
            UnitTeleportToUnitAndHealBuffFx(sUnit, owner);
            RestoreHealth(owner, 1);
            LookWithAngle(sUnit, count - 1);
            FrameTimerWithArg(2, sUnit, BuffHealingHealth);
            break;
        }
        Delete(sUnit);
        break;
    }
}

void UseItemFunc1()
{
    Delete(SELF);
    Enchant(other, "ENCHANT_SHOCK", 120.0);
}

void UseItemFunc2()
{
    int home = HCenterUnit;
    Delete(SELF);
    if (IsVisibleTo(other, home) || IsVisibleTo(home, other))
    {
        if (MapLang)
            UniPrint(other, "Space movement failed: already around destination");
        else
            UniPrint(other, "공간이동 실패: 이미 목적지 주변에 있음");
    }
    else
    {
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        MoveObject(other, GetObjectX(home), GetObjectY(home));
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
    }
}

void UseItemFunc3()
{
    if (CurrentHealth(other))
    {
        Delete(SELF);
        SplashDamage(GetCaller(), 200, GetObjectX(other), GetObjectY(other), 180.0);
        Effect("WHITE_FLASH", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        DeleteObjectTimer(CreateObjectAt("ManaBombCharge", GetObjectX(other), GetObjectY(other)), 24);
    }
}

void UseItemFunc4()
{
    int trap;

    if (CurrentHealth(other))
    {
        trap = CreateObjectAt("BearTrap", GetObjectX(other), GetObjectY(other));
        SetOwner(other, trap);
        Delete(SELF);
        if (MapLang)
            UniPrint(other, "A bear trap has been created on the floor. And pick it up and put it in inventory.");
        else
            UniPrint(other, "바닥에 곰덫이 생성되었습니다. 줍기를 통해 인벤토리에 넣을 수 있습니다");
    }
}

void UseItemFunc5()
{
    int trap;

    if (CurrentHealth(other))
    {
        trap = CreateObjectAt("PoisonGasTrap", GetObjectX(other), GetObjectY(other));
        SetOwner(other, trap);
        Delete(SELF);
        if (MapLang)
            UniPrint(other, "A poison gas trap has been created on the floor. And pick it up and put it in inventory");
        else
            UniPrint(other, "바닥에 독가스 함정이 생성되었습니다. 줍기를 통해 인벤토리에 넣을 수 있습니다");
    }
}

void UseItemFunc6()
{
    if (CurrentHealth(OTHER))
    {
        RestoreHealth(other, MaxHealth(other) - CurrentHealth(other));
        if (IsPoisonedUnit(other))
        {
            CastSpellObjectObject("SPELL_CURE_POISON", other, other);
            if (IsPoisonedUnit(other))
                CastSpellObjectObject("SPELL_CURE_POISON", other, other);
        }
        Enchant(other, "ENCHANT_REFLECTIVE_SHIELD", 120.0);
        Delete(SELF);
        if (MapLang)
            UniPrint(other, "With the power of one million Gwanghwamun candles ...! (Full health recovery + Poison treatment, reflex shield for 2 minutes)");
        else
            UniPrint(other, "100만 광화문 촛불의 힘으로...! (체력 풀회복 + 맹독 치료, 2분 간 반사방어막)");
    }
}

void UseItemFunc7()
{
    int sUnit;

    if (CurrentHealth(OTHER))
    {
        Delete(SELF);
        if (HasEnchant(other, "ENCHANT_VILLAIN")) return;
        sUnit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(other), GetObjectY(other));
        SetOwner(other, sUnit);
        LookWithAngle(sUnit, 225);
        FrameTimerWithArg(1, sUnit, BuffHealingHealth);
        Enchant(other, "ENCHANT_VILLAIN", 15.0);
        Enchant(sUnit, "ENCHANT_RUN", 0.0);
        if (MapLang)
            UniPrint(other, "cheer up Mr.Lee  ~ (Increases health recovery rate for 15 seconds)");
        else
            UniPrint(other, "힘을내요 미스터리~ (15 초간 체력회복 속도 증가)");
    }
}

string ObstacleNameTable(int index)
{
    string sType[] = {
        "Barrel", "Barrel2", "BarrelLOTD", "WaterBarrel", "PiledBarrels1",
        "PiledBarrels2", "LargeBarrel1", "LargeBarrel2", "Crate1", "Crate2",
        "DarkCrate1", "DarkCrate2", "Coffin1", "Coffin2", "Coffin3",
        "Coffin4",
        "BarrelBreaking", "BarrelBreaking", "BarrelBreakingLOTD", "WaterBarrelBreaking", "PiledBarrels1Breaking",
        "PiledBarrels2Breaking", "LargeBarrelBreaking1", "LargeBarrelBreaking2", "CrateBreaking1", "CrateBreaking2",
        "DarkCrateBreaking1", "DarkCrateBreaking2", "CoffinBreaking1", "CoffinBreaking2", "CoffinBreaking3",
        "CoffinBreaking4"
    };
    
    return sType[index];
}

void ObstacleBreaking()
{
    int idx = GetDirection(SELF), pic;
    float xProfile = GetObjectX(SELF), yProfile = GetObjectY(SELF);

    Delete(SELF);
    pic = CreateObjectAt(ObstacleNameTable(idx + 16), xProfile, yProfile);
    CallFunctionWithArgInt(ItemDropFuncPtr() + Random(0, 9), pic);
}

int PlacingObstacle(int sUtype, float xProfile, float yProfile, int hitP)
{
    int obst = CreateObjectAt(ObstacleNameTable(sUtype), xProfile, yProfile);

    if (obst)
    {
        LookWithAngle(obst, sUtype);
        SetUnitCallbackOnDeath(obst, ObstacleBreaking);
        SetUnitMaxHealth(obst, hitP);
    }
    return obst;
}

void InitObstacleCreate2()
{
    PlacingObstacle(2, 5036.0, 5472.0, 200);
    PlacingObstacle(2, 5010.0, 5450.0, 200);
    PlacingObstacle(6, 5072.0, 5056.0, 200);
    PlacingObstacle(12, 5061.5, 4604.5, 200);
    PlacingObstacle(0, 472.0, 3177.0, 85);
    PlacingObstacle(0, 285.0, 3422.0, 85);
    PlacingObstacle(1, 252.0, 3441.0, 85);
    PlacingObstacle(3, 239.0, 3406.0, 100);
    PlacingObstacle(0, 216.0, 3432.0, 100);
    PlacingObstacle(2, 1549.0, 1994.0, 100);
    PlacingObstacle(2, 1570.0, 2021.0, 100);
    PlacingObstacle(2, 1523.0, 1958.0, 100);
    PlacingObstacle(6, 923.0, 640.0, 125);
    PlacingObstacle(3, 3115.0, 3790.0, 125);
    PlacingObstacle(1, 2803.0, 3595.5, 125);
    PlacingObstacle(1, 4917.5, 2197.5, 60);
    PlacingObstacle(11, 3542.0, 4943.5, 125);
    PlacingObstacle(0, 3450.5, 4830.5, 100);
    PlacingObstacle(0, 3588.5, 4868.333, 85);
    PlacingObstacle(0, 3584.5, 4901.5, 100);
    PlacingObstacle(1, 3618.667, 4853.333, 100);
    PlacingObstacle(1, 3493.5, 4788.5, 100);
    PlacingObstacle(2, 3321.5, 3814.5, 125);
    PlacingObstacle(0, 3141.5, 3766.5, 200);
    PlacingObstacle(0, 2977.5, 3933.5, 250);
    PlacingObstacle(1, 1504.5, 4428.5, 250);
    PlacingObstacle(11, 2512.5, 3634.5, 225);
}

void InitObstacleCreate()
{
    PlacingObstacle(0, 3504.0, 4921.0, 100);
    PlacingObstacle(11, 3541.0, 4891.0, 100);
    PlacingObstacle(5, 3588.0, 4822.0, 100);
    PlacingObstacle(10, 3542.0, 4776.0, 100);
    PlacingObstacle(1, 3475.0, 4806.0, 100);
    PlacingObstacle(7, 3634.0, 4896.0, 100);
    PlacingObstacle(11, 3505.333, 4874.667, 100);
    PlacingObstacle(6, 3115.0, 4239.5, 100);
    PlacingObstacle(0, 3347.5, 3828.5, 100);
    PlacingObstacle(3, 3301.5, 3843.5, 100);
    PlacingObstacle(1, 4086.5, 1237.5, 100);
    PlacingObstacle(0, 3343.5, 2194.5, 100);
    PlacingObstacle(11, 1718.0, 5017.0, 100);
    PlacingObstacle(11, 1783.0, 5090.0, 100);
    PlacingObstacle(11, 1817.0, 5123.0, 100);
    PlacingObstacle(11, 1750.0, 5054.0, 100);
    PlacingObstacle(1, 1369.0, 4572.0, 100);
    PlacingObstacle(0, 1451.0, 4496.0, 30);
    PlacingObstacle(1, 1646.0, 4176.0, 70);
    PlacingObstacle(1, 1722.0, 4178.0, 70);
    PlacingObstacle(1, 1792.0, 4032.0, 70);
    PlacingObstacle(1, 1671.0, 4197.0, 70);
    PlacingObstacle(1, 1818.0, 4056.0, 70);
    PlacingObstacle(3, 3406.0, 3894.0, 100);
    PlacingObstacle(5, 3474.0, 3952.0, 100);
    PlacingObstacle(0, 3513.0, 4050.0, 100);
    FrameTimer(1, InitObstacleCreate2);
}

void GreenSparkFx(float xProfile, float yProfile)
{
    int fxTarget = CreateObjectAt("MonsterGenerator", xProfile, yProfile);

    Damage(fxTarget, 0, 1, -1);
    Delete(fxTarget);
}

void PlayerPoisonAutoCure(int sUnit)
{
    if (IsPoisonedUnit(sUnit))
    {
        CastSpellObjectObject("SPELL_CURE_POISON", sUnit, sUnit);
        Effect("VIOLET_SPARKS", GetObjectX(sUnit), GetObjectY(sUnit), 0.0, 0.0);
    }
}

void PlayerAllEnchantments(int sPlrUnit)
{
    Enchant(sPlrUnit, "ENCHANT_PROTECT_FROM_ELECTRICITY", 0.0);
    Enchant(sPlrUnit, "ENCHANT_PROTECT_FROM_FIRE", 0.0);
    Enchant(sPlrUnit, "ENCHANT_PROTECT_FROM_POISON", 0.0);
    Enchant(sPlrUnit, "ENCHANT_VAMPIRISM", 0.0);
    Enchant(sPlrUnit, "ENCHANT_REFLECTIVE_SHIELD", 0.0);
}

void CommonAllEncDesc()
{
    TellStoryUnitName("AmuletDrop", "War05A.scr:HoundGreeting", "올엔첸 구입");
    if (MapLang)
        UniChatMessage(self, "Would you like to buy all-enchantment?\nVarious useful buffs always last\nneed gold: 50000", 120);
    else
        UniChatMessage(self, "올엔첸을 구입하겠습니까?\n각종 유용한 버프가 항상 지속됩니다\n5만원 필요!", 120);
}

void CommonAllEncTrade()
{
    int plr;

    if (GetAnswer(self) ^ 1) return;
    plr = CheckPlayer();
    if (plr + 1)
    {
        if (GetGold(other) >= 50000)
        {
            if (CheckPlayerSkillFlag3(plr))
            {
                if (MapLang)
                    UniPrint(other, "You have already learned this ability");
                else
                    UniPrint(other, "이미 이 능력을 배우셨습니다");
            }
            else
            {
                ChangeGold(other, -50000);
                MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
                SetPlayerSkillFlag3(plr);
                GreenSparkFx(GetWaypointX(1), GetWaypointY(1));
                PlayerAllEnchantments(other);
                Effect("WHITE_FLASH", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
                AudioEvent("SpellPopOffBook", 1);
                if (MapLang)
                    UniPrint(other, "I learned the 'All-Enchant' ability. Useful encent is always maintained and not poisoned");
                else
                    UniPrint(other, "'올엔첸트' 능력을 배웠습니다. 유용한 엔첸트가 항상 유지되며 독에 걸리지 않습니다");
            }
        }
        else
        {
            if (MapLang)
                UniChatMessage(self, "Transaction failure: insufficient balance", 120);
            else
                UniChatMessage(self, "거래실패: 잔액이 부족합니다", 120);
        }
    }
}

void WarWindBoosterDesc()
{
    TellStoryUnitName("AmuletDrop", "War05C.scr:SisterSad", "전사 스킬구입");
    if (MapLang)
        UniChatMessage(self, "Purchase the Warrior's new skill 'Wind Booster' \n10,000 won required!", 120);
    else
        UniChatMessage(self, "전사의 새로운 스킬 '윈드 부스터'을 구입합니다\n10000원 필요!", 120);
}

void WarSkill2MarketTrade()
{
    int plr;

    if (GetAnswer(self) ^ 1) return;
    plr = CheckPlayer();
    if (plr + 1)
    {
        if (GetGold(other) >= 10000)
        {
            if (CheckPlayerSkillFlag2(plr))
            {
                if (MapLang)
                    UniPrint(other, "You have already learned this ability");
                else
                    UniPrint(other, "이미 이 능력을 배우셨습니다");
            }
            else
            {
                ChangeGold(other, -10000);
                MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
                SetPlayerSkillFlag2(plr);
                GreenSparkFx(GetWaypointX(1), GetWaypointY(1));
                Effect("WHITE_FLASH", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
                AudioEvent("SpellPopOffBook", 1);
                if (MapLang)
                    UniPrint(other, "I learned the 'Wind Booster' ability. It fires whenever you cast a skill called TreadLightly.");
                else
                    UniPrint(other, "'윈드 부스터' 능력을 배웠습니다. 조심스럽게 걷기 를 시전할 때 마다 발동됩니다");
            }
        }
        else
        {
            if (MapLang)
                UniChatMessage(self, "Transaction failure: insufficient balance", 120);
            else
                UniChatMessage(self, "거래실패: 잔액이 부족합니다", 120);
        }
    }
}

void WarSkillMarketDesc()
{
    TellStoryUnitName("AmuletDrop", "War02A:BartenderTalk1fStart", "레일건 구입");
    if (MapLang)
        UniChatMessage(self, "I will purchase the new 'Rail Gun' skill of the warrior. \n20,000 won required!", 120);
    else
        UniChatMessage(self, "전사의 새로운 스킬 '레일건'을 구입합니다\n20000원 필요!", 120);
}

void WarSkillMarketTrade()
{
    int plr;

    if (GetAnswer(self) ^ 1) return;
    plr = CheckPlayer();
    if (plr + 1)
    {
        if (GetGold(other) >= 20000)
        {
            if (CheckPlayerSkillFlag(plr))
            {
                if (MapLang)
                    UniPrint(other, "You have already learned this ability");
                else
                    UniPrint(other, "이미 이 능력을 배우셨습니다");
            }
            else
            {
                ChangeGold(other, -20000);
                MoveWaypoint(1, GetObjectX(other), GetObjectY(other));
                SetPlayerSkillFlag(plr);
                GreenSparkFx(GetWaypointX(1), GetWaypointY(1));
                Effect("WHITE_FLASH", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
                AudioEvent("SpellPopOffBook", 1);
                if (MapLang)
                    UniPrint(other, "I learned the 'railgun' ability. Fired whenever the J key is pressed.");
                else
                    UniPrint(other, "'레일건' 능력을 배웠습니다. J 키를 누를 때 마다 발동됩니다");
            }
        }
        else
        {
            if (MapLang)
                UniChatMessage(self, "Transaction failure: insufficient balance", 120);
            else
                UniChatMessage(self, "거래실패: 잔액이 부족합니다", 120);
        }
    }
}

void PlaceWarSkillMarket()
{
    int market = DummyUnitCreateAt("Horrendous", 5267.0, 3285.0);

    LookWithAngle(DummyUnitCreateAt("Swordsman", 5238.5, 3311.5), 160);
    LookWithAngle(DummyUnitCreateAt("UrchinShaman", 5152.5, 3325.5), -32);
    LookWithAngle(market, 160);
    SetDialog(market, "YESNO", WarSkillMarketDesc, WarSkillMarketTrade);
    StoryPic(market, "HorrendousPic");
    StoryPic(market + 1, "GearhartPic");
    StoryPic(market + 2, "MalePic12");
    SetDialog(market + 1, "YESNO", WarWindBoosterDesc, WarSkill2MarketTrade);
    SetDialog(market + 2, "YESNO", CommonAllEncDesc, CommonAllEncTrade);
    ShopClassOblivionOrbMarket(26);
}

void ZombieWhenDead()
{
    MoveWaypoint(1, GetObjectX(self), GetObjectY(self));
    if (MaxHealth(self))
        Damage(self, 0, 100, 14);
    DeleteObjectTimer(CreateObject("MediumFlame", 1), 150);
    MonsterDeathHandler();
    Effect("SPARK_EXPLOSION", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
    AudioEvent("BurnCast", 1);
}

int FieldMobSwordsman(int sUnit)
{
    int mob = CreateObjectAt("Swordsman", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 125);
    return mob;
}

int FieldMobBat(int sUnit)
{
    int mob = CreateObjectAt("Bat", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 64);
    return mob;
}

int FieldMobGreenFrog(int sUnit)
{
    int mob = CreateObjectAt("GreenFrog", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 64);
    UnitLinkBinScript(mob, GreenFrogBinTable());
    return mob;
}

int FieldMobArcher(int sUnit)
{
    int mob = CreateObjectAt("Archer", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 98);
    return mob;
}

int FieldMobWasp(int sUnit)
{
    int mob = CreateObjectAt("Wasp", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 64);
    return mob;
}

int FieldMobGiantLeech(int sUnit)
{
    int mob = CreateObjectAt("GiantLeech", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 120);
    return mob;
}

int FieldMobUrchin(int sUnit)
{
    int mob = CreateObjectAt("Urchin", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 98);
    return mob;
}

int FieldMobBomber(int sUnit)
{
    string bombers[] = {"Bomber", "BomberBlue", "BomberGreen", "BomberYellow"};
    int mob = CreateObjectAt(bombers[Random(0, 3)], GetObjectX(sUnit), GetObjectY(sUnit));

    UnitLinkBinScript(mob, BomberGreenBinTable());
    BomberSetMonsterCollide(mob);
    SetUnitMaxHealth(mob, 135);
    return mob;
}

int FieldMobMystic(int sUnit)
{
    int mob = CreateObjectAt("Wizard", GetObjectX(sUnit), GetObjectY(sUnit));

    Enchant(mob, "ENCHANT_ANCHORED", 0.0);
    SetUnitMaxHealth(mob, 175);
    return mob;
}

int FieldMobBlackBear(int sUnit)
{
    int mob = CreateObjectAt("BlackBear", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 275);
    return mob;
}

int FieldMobBlackWolf(int sUnit)
{
    int mob = CreateObjectAt("BlackWolf", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 225);
    return mob;
}

int FieldMobBear(int sUnit)
{
    int mob = CreateObjectAt("Bear", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 325);
    return mob;
}

int FieldMobScopion(int sUnit)
{
    int mob = CreateObjectAt("Scorpion", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 260);
    return mob;
}

int FieldMobShade(int sUnit)
{
    int mob = CreateObjectAt("Shade", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 170);
    return mob;
}

int FieldMobSpider(int sUnit)
{
    int mob = CreateObjectAt("BlackWidow", GetObjectX(sUnit), GetObjectY(sUnit));

    UnitLinkBinScript(mob, BlackWidowBinTable());
    SetUnitMaxHealth(mob, 170);
    SetUnitVoice(mob, 33);
    return mob;
}

int FieldMobSkeleton(int sUnit)
{
    int mob = CreateObjectAt("Skeleton", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 250);
    return mob;
}

int FieldMobSkeletonLord(int sUnit)
{
    int mob = CreateObjectAt("SkeletonLord", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 306);
    return mob;
}

int FieldMobGargoyle(int sUnit)
{
    int mob = CreateObjectAt("EvilCherub", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 108);
    return mob;
}

int FieldMobZombie(int sUnit)
{
    int mob = CreateObjectAt("Zombie", GetObjectX(sUnit), GetObjectY(sUnit));

    ObjectOff(mob);
    SetUnitMaxHealth(mob, 240);
    SetCallback(mob, 5, ZombieWhenDead);
    return mob;
}

int FieldMobVileZombie(int sUnit)
{
    int mob = CreateObjectAt("VileZombie", GetObjectX(sUnit), GetObjectY(sUnit));

    ObjectOff(mob);
    SetUnitMaxHealth(mob, 325);
    SetUnitSpeed(mob, 2.4);
    SetCallback(mob, 5, ZombieWhenDead);
    return mob;
}

int FieldMobGhost(int sUnit)
{
    int mob = CreateObjectAt("Ghost", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 98);
    return mob;
}

int FieldMobLich(int sUnit)
{
    int mob = CreateObjectAt("Lich", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 360);
    SetUnitStatus(mob, GetUnitStatus(mob) ^ 0x20);
    UnitLinkBinScript(mob, LichLordBinTable());
    return mob;
}

int FieldMobLichLord(int sUnit)
{
    int mob = CreateObjectAt("LichLord", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 450);
    SetUnitStatus(mob, GetUnitStatus(mob) ^ 0x20);
    UnitLinkBinScript(mob, LichLord2BinTable());
    return mob;
}

int FieldMobMecaGolem(int sUnit)
{
    int mob = CreateObjectAt("MechanicalGolem", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 800);
    return mob;
}

int FieldMobNecromancer(int sUnit)
{
    int unit = CreateObjectAt("Necromancer", GetObjectX(sUnit), GetObjectY(sUnit));
    int uec = GetMemory(GetMemory(0x750710) + 0x2ec);

    SetUnitMaxHealth(unit, 325);
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

int FieldMobFireFairy(int sUnit)
{
    int mob = CreateObjectAt("FireSprite", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 130);
    SetUnitStatus(mob, GetUnitStatus(mob) ^ 0x10000);
    UnitLinkBinScript(mob, FireSpriteBinTable());
    return mob;
}

int FieldMobJandor(int sUnit)
{
    int mob = CreateObjectAt("AirshipCaptain", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 325);
    UnitLinkBinScript(mob, AirshipCaptainBinTable());
    return mob;
}

int FieldMobEmberDemon(int sUnit)
{
    string mName[] = {"EmberDemon", "MeleeDemon"};
    int mob = CreateObjectAt(mName[Random(0, 1)], GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 160);
    return mob;
}

int FieldMobWhiteWizard(int sUnit)
{
    int mob = CreateObjectAt("WizardWhite", GetObjectX(sUnit), GetObjectY(sUnit));

    Enchant(mob, "ENCHANT_ANCHORED", 0.0);
    SetUnitMaxHealth(mob, 275);
    return mob;
}

int FieldMobHecubahWithOrb(int sUnit)
{
    int mob = CreateObjectAt("HecubahWithOrb", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 375);
    SetUnitStatus(mob, GetUnitStatus(mob) ^ 0x10000);
    UnitLinkBinScript(mob, HecubahWithOrbBinTable());
    return mob;
}

int FieldMobOgre(int sUnit)
{
    int mob = CreateObjectAt("OgreBrute", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 295);
    return mob;
}

int FieldMobOgreAxe(int sUnit)
{
    int mob = CreateObjectAt("GruntAxe", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 260);
    return mob;
}

int FieldMobOgreLord(int sUnit)
{
    int mob = CreateObjectAt("OgreWarlord", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 325);
    return mob;
}

int FieldMobGoon(int sUnit)
{
    int mob = CreateObjectAt("Goon", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 225);
    if (!IsMonsterPoisonImmune(mob))
        SetMonsterPoisonImmune(mob);
    UnitLinkBinScript(mob, GoonBinTable());
    return mob;
}

int FieldMobFemale(int sUnit)
{
    int mob = ColorMaidenAt(Random(0, 255), Random(0, 255), Random(0, 255), GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 325);
    if (!IsMonsterPoisonImmune(mob))
        SetMonsterPoisonImmune(mob);
    SetUnitVoice(mob, 7);
    UnitLinkBinScript(mob, MaidenBinTable());

    return mob;
}

int FieldMobBeast(int sUnit)
{
    int mob = CreateObjectAt("WeirdlingBeast", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 240);
    UnitLinkBinScript(mob, WeirdlingBeastBinTable());
    return mob;
}

int FieldMobStoneGolem(int sUnit)
{
    int mob = CreateObjectAt("StoneGolem", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 700);
    return mob;
}

int FieldMobDryad(int sUnit)
{
    int mob = CreateObjectAt("WizardGreen", GetObjectX(sUnit), GetObjectY(sUnit));

    Enchant(mob, "ENCHANT_ANCHORED", 0.0);
    SetUnitMaxHealth(mob, 225);
    return mob;
}

int FieldMobShaman(int sUnit)
{
    int mob = CreateObjectAt("UrchinShaman", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 128);
    return mob;
}

int FieldMobHorrendous(int sUnit)
{
    int mob = CreateObjectAt("Horrendous", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 420);
    SetUnitSpeed(mob, 1.4);
    return mob;
}

int FieldMobDemon(int sUnit)
{
    int mob = CreateObjectAt("Demon", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 500);
    return mob;
}

int FieldMobMimic(int sUnit)
{
    int mob = CreateObjectAt("Mimic", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 600);
    return mob;
}

int FieldMobBeholder(int sUnit)
{
    int mob = CreateObjectAt("Beholder", GetObjectX(sUnit), GetObjectY(sUnit));

    Enchant(mob, "ENCHANT_ANCHORED", 0.0);
    SetUnitMaxHealth(mob, 400);
    return mob;
}

int FieldMobSmallWhiteSpider(int sUnit)
{
    int mob = CreateObjectAt("SmallAlbinoSpider", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 75);
    SetUnitSpeed(mob, 1.1);
    return mob;
}

int FieldMobWhiteWolf(int sUnit)
{
    int mob = CreateObjectAt("WhiteWolf", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 175);
    return mob;
}

int FieldMobAlbinoSpider(int sUnit)
{
    int mob = CreateObjectAt("AlbinoSpider", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 160);
    return mob;
}

int FieldMobMecaFlier(int sUnit)
{
    int mob = CreateObjectAt("FlyingGolem", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 75);
    return mob;
}

int FieldMobImp(int sUnit)
{
    int mob = CreateObjectAt("SmallSpider", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 50);
    SetUnitSpeed(mob, 1.3);
    return mob;
}

int FieldMobSmallSpider(int sUnit)
{
    int mob = CreateObjectAt("SmallAlbinoSpider", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 75);
    SetUnitSpeed(mob, 1.1);
    return mob;
}

int FieldMobWolf(int sUnit)
{
    int mob = CreateObjectAt("Wolf", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitMaxHealth(mob, 140);
    return mob;
}

int FieldMobHecubah(int sUnit)
{
    int mob = CreateObjectAt("Hecubah", GetObjectX(sUnit), GetObjectY(sUnit));

    SetUnitStatus(mob, GetUnitStatus(mob) ^ 0x20);
    UnitLinkBinScript(mob, HecubahBinTable());
    SetUnitMaxHealth(mob, 550);
    return mob;
}

int FieldMobTalkingSkull(int sUnit)
{
    int mob = CreateObjectAt("TalkingSkull", GetObjectX(sUnit), GetObjectY(sUnit));

    Enchant(mob, "ENCHANT_VAMPIRISM", 0.0);
    SetUnitSpeed(mob, 2.7);
    UnitLinkBinScript(mob, TalkingSkullBinTable());
    SetUnitMaxHealth(mob, 240);
    return mob;
}

int FieldMobFireballWiz(int sUnit)
{
    int mob = CreateObjectAt("StrongWizardWhite", GetObjectX(sUnit), GetObjectY(sUnit));

    UnitLinkBinScript(mob, StrongWizardWhiteBinTable());
    SetUnitMaxHealth(mob, 260);
    return mob;
}

void CommonFieldMonsterProperty(int sMob)
{
    RetreatLevel(sMob, 0.0);
    UnitZeroFleeRange(sMob);
    if (IsObjectOn(sMob))
        SetCallback(sMob, 5, MonsterDeathHandler);
    else
        ObjectOn(sMob);
    SetCallback(sMob, 7, hurtEvent);
    SetCallback(sMob, 9, collisionMonster);
}

int GoldAmount(int sUnit)
{
    int gPtr = UnitToPtr(sUnit);
    if (gPtr)
    {
        if (GetMemory(gPtr + 0x14))
            return GetMemory(GetMemory(gPtr + 0x2b4));
    }
    return 0;
}

int ProcessFieldMonsterMarker(int sUnit)
{
    int sArg;
    int uPtr, sFunc;

    if (IsObjectOn(sUnit))
    {
        uPtr = UnitToPtr(sUnit);
        if (uPtr)
        {
            sArg = GetMemory(uPtr + 0x14);
            sFunc = MobFieldFunc + Random(sArg & 0xff, (sArg >> 8) & 0xff);
            CommonFieldMonsterProperty(CallFunctionWithArgInt(sFunc, sUnit));
        }
        Delete(sUnit);
    }
}

int ProcessAdvanceFieldMobMarker(int sUnit)
{
    int sArg, arr[4];

    if (IsObjectOn(sUnit))
    {
        sArg = GoldAmount(sUnit);
        if (sArg)
        {
            arr[0] = sArg & 0xff;
            arr[1] = (sArg >> 8) & 0xff;
            arr[2] = (sArg >> 16) & 0xff;
            arr[3] = (sArg >> 24) & 0xff;
            CommonFieldMonsterProperty(CallFunctionWithArgInt(MobFieldFunc + arr[Random(0, 3)], sUnit));
        }
        Delete(sUnit);
    }
}

void ResetHostileCritter()
{
	//SetMemory(0x833e64, 0x55b);		//CarnivorousPlant
	//SetMemory(0x833e70, 1329);		//FishBig
	//SetMemory(0x833e74, 1330);		//FishSmall
	//SetMemory(0x833e78, 1359);		//Rat
	SetMemory(0x833e7c, 1313);		//GreenFrog
}

void SetHostileCritter()
{
	//SetMemory(0x833e64, 0x540);		//CarnivorousPlant
	//SetMemory(0x833e70, 0x540);		//FishBig
	//SetMemory(0x833e74, 0x540);		//FishSmall
	//SetMemory(0x833e78, 0x540);		//Rat
	SetMemory(0x833e7c, 0x540);		//GreenFrog
}

void UnitVisibleSplash()
{
    int parent = GetOwner(self);
    int spIdx = ToInt(GetObjectZ(parent + 1));

    if (CurrentHealth(GetOwner(parent)))
    {
        if (Distance(GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other)) <= GetObjectX(parent))
        {
            Damage(other, GetOwner(parent), ToInt(GetObjectZ(parent)), 14);
            Enchant(other, "ENCHANT_CHARMING", 0.08);
        }
    }
}

void SplashDamage(int owner, int dam, float x, float y, float range)
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
        SetCallback(ptr + k, 3, UnitVisibleSplash);
    }
    DeleteObjectTimer(ptr - 2, 2);
}

int BgmTable(int idx)
{
	int misc[30];

	if (idx < 0)
	{
		misc[1] = 33; misc[2] = 33; misc[3] = 33; misc[4] = 32; misc[5] = 60;
		misc[6] = 46; misc[7] = 86; misc[8] = 48; misc[9] = 33; misc[10] = 38;
		misc[11] = 32; misc[12] = 39; misc[13] = 36; misc[14] = 73; misc[15] = 264;
		misc[16] = 236; misc[17] = 216; misc[18] = 265; misc[19] = 254; misc[20] = 239;
		misc[21] = 244; misc[22] = 274; misc[23] = 181; misc[24] = 99; misc[25] = 112;
		misc[26] = 68; misc[27] = 150; misc[28] = 118; misc[29] = 54;
		return 0;
	}
	return misc[idx];
}

void PlayMapBgm(int num)
{
	Music(num, 100);
}

void MapBgmLoop()
{
	int key = Random(1, 29);
	MusicEvent();
	SecondTimerWithArg(3, key, PlayMapBgm);
	SecondTimer(BgmTable(key) + 3, MapBgmLoop);
}

void StartBgmLoop()
{
    int versionInfo = 0x974ac0;

    FrameTimerWithArg(1, ReadStringAddress(&versionInfo), DisplayMapVersion);
	BgmTable(-1);
	SecondTimer(2, MapBgmLoop);
}

//0x974ac0

void DisplayMapVersion(string sArg)
{
    if (MapLang)
        UniPrintToAll("Map Version:::");
    else
        UniPrintToAll("맵 버전:::");
    UniPrintToAll(sArg);
}

void MapSignSetting()
{
    if (MapLang)
    {
        RegistSignMessage(Object("DunmirSign1"), "Sign: There is GS-24 Hours Market");
        RegistSignMessage(Object("DunmirSign2"), "Sign: Sinchon Bar (unlimit support SOJU)");
        RegistSignMessage(Object("DunmirSign3"), "In the front, there is a machine room facing the underground sewer.");
        RegistSignMessage(Object("DunmirSign4"), "Selling Oblivion Staff! You can buy it by clicking on the well and you need 50,000 gold.");
        RegistSignMessage(Object("DunmirSign5"), "If you give 70,000 won to this well, a strong bodyguard will be summoned.");
        RegistSignMessage(Object("DunmirSign6"), "HANARO Mart: Everything except the world has nothing.");
        RegistSignMessage(Object("DunmirSign7"), "DunmirFinalBoss's Entrance: you need ruby key and SapphireKey");
        RegistSignMessage(Object("DunmirSign8"), "You can find even more cool items through secret areas hidden all over the map.");
    }
    else
    {
        RegistSignMessage(Object("DunmirSign1"), "표지판: 이곳은 GS25시 편의점 이다");
        RegistSignMessage(Object("DunmirSign2"), "표지판: 신촌 감성주점 (소주 무한리필)");
        RegistSignMessage(Object("DunmirSign3"), "저 앞은 ,지하 하수도로 향하는 기계실 입니다");
        RegistSignMessage(Object("DunmirSign4"), "망각의 지팡이를 판매하고 있어요! 우물을 클릭하면 구입할 수 있고 5만 골드가 필요해요");
        RegistSignMessage(Object("DunmirSign5"), "이 우물에 7만원을 준다면 잔도가 소환될 것이에요");
        RegistSignMessage(Object("DunmirSign6"), "하나로 마트: 세상 모든 물건 없는 것 빼고 다 있습니다");
        RegistSignMessage(Object("DunmirSign7"), "던미르 최종 보스의 방 입구. 이곳에 들어가기 위해서는 루비, 사파이어 열쇠가 필요");
        RegistSignMessage(Object("DunmirSign8"), "맵 곳곳에 숨겨진 비밀구역을 통해 더욱 멋진 아이템을 만나보실 수 있어요");
    }
}

void GameGuideMessage()
{
    if (MapLang)
        UniPrintToAll("How to play:\nRaid the village of Dunmir and destroy all enemies.\nWin by killing the last boss of Dunmir Village");
    else
        UniPrintToAll("게임방법:\n던미르 마을을 습격하여 모든 적들을 파괴하세요.\n던미르 마을의 마지막 보스를 죽이면 승리합니다");
}

void HostPlayerTeleport(int sUnit)
{
    MoveObject(sUnit, GetWaypointX(14), GetWaypointY(14));
    FrameTimerWithArg(1, Object("MapFirstUnitScan"), initializeSearchIndex);
    FrameTimer(180, GameGuideMessage);
}

void KorPick()
{
    UniPrint(other, "한글 언어모드 선택됨");
    HostPlayerTeleport(other);
}

void EngPick()
{
    UniPrint(other, "English Language");
    MapLang = 1;
    HostPlayerTeleport(other);
}

void HostPlayerSelectLanguage()
{
    MoveObject(GetHost(), GetWaypointX(12), GetWaypointY(12));
    UniPrint(GetHost(), "언어를 선택하세요");
    UniPrint(GetHost(), "Please select map language");
    RegistSignMessage(Object("LangSign1"), "Language: Korea");
    RegistSignMessage(Object("LangSign2"), "Language: US");
}

int ImportChestCollide()
{
	int arr[14], link;

	if (!link)
	{
		arr[0] = 0x50565551; arr[1] = 0x1824748B; arr[2] = 0x14246C8B; arr[3] = 0x02FC858B; arr[4] = 0x10680000; arr[5] = 0x55005073; arr[6] = 0x54FF5056;
		arr[7] = 0x5E580C24; arr[8] = 0xF0858B5D; arr[9] = 0x83000002; arr[10] = 0xC08504C4; arr[11] = 0x595D5E58; arr[12] = 0x40680574; arr[13] = 0xC3004E9C;
		link = &arr;
	}
	return link;
}

int FindRubyKey(int unit)
{
	int inv = GetLastItem(unit);

	while (inv)
	{
		if (GetUnitThingID(inv) == 2182)
			return inv;
		inv = GetPreviousItem(inv);
	}
	return 0;
}

void ChestCollide()
{
	int cFps = GetMemory(0x84ea04);
	int ptr = UnitToPtr(self), key, item;

	if (ptr && HasClass(other, "PLAYER"))
	{
		if (MathAbs(cFps - GetMemory(ptr + 0x2e4)) > 30)
		{
			SetMemory(ptr + 0x2e4, cFps);
			if (!GetMemory(ptr + 0x2f0))
			{
				if (FindRubyKey(other))
				{
					Delete(FindRubyKey(other));
					item = CreateObjectAt("RewardMarker", GetObjectX(self) + UnitRatioX(other, self, 32.0), GetObjectY(self) + UnitRatioY(other, self, 32.0));
                    RewardClassProc(item);
					SetMemory(ptr + 0x2f0, 1);
					UniPrint(other, "리치 열쇠를 사용하여 잠긴 금고를 열었습니다");
				}
				else
				{
					PlaySoundAround(other, 1012);
					UniPrint(other, "이 금고를 열려면 리치 열쇠가 필요합니다");
				}
			}
		}
	}
}

void ChangeChestHandler(int chestObj)
{
	int ptr = UnitToPtr(chestObj);

	if (ptr)
	{
		SetMemory(ptr + 0x2b8, ImportChestCollide());
		SetMemory(ptr + 0x2fc, ChestCollide);
	}
}

void InitGauntletChest(int multiple)
{
    string boxName = "DunmirBox";
    int i;

    multiple *= 10;
    for (i = 0 ; i < 10 ; i ++)
        ChangeChestHandler(Object(boxName + IntToString(multiple + i + 1)));
}


