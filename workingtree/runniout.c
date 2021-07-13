
#include "noxscript\builtins.h"

#include "libs\typecast.h"
#include "libs\unitstruct.h"
#include "libs\unitutil.h"
#include "libs\printutil.h"
#include "libs\observer.h"
#include "libs\username.h"
#include "libs\mathlab.h"
#include "libs\waypoint.h"
#include "libs\network.h"
#include "libs\imageutil.h"
#include "libs\clientside.h"

#define NULLPTR             0

#define OTHER   -1
#define SELF    -2
#define FALSE   0
#define TRUE    1

float EndXProfile = 5760.0;
float LimitArea = 320.0;
int VaildFlag = 1;
int OnGameAlive = 0;
int OnSafeCenter;
int DestMarker;
int PlayerFlagPtr;
int player[20];
int PlrCam[10];

int Area2BombTrp1, Area2BombTrp2;
int FireWayTrp;
int WispBlckTrp;


void CommonGuideMessage()
{
    UniPrint(OTHER, "이 구간 통과방법은 다음과 같습니다");
}

void ResourceBGMdata()
{ }

void ImageResourceExitIndicator()
{ }

void PlayerCommonEntryEvent()
{
    int drawF = ImageResourceDrawFunctionFix(0x7510c4);

    ApplyImageResourceFix(1620, GetScrCodeField(ImageResourceExitIndicator) + 4, drawF);

    ExtractMapBgm("mazerunner.mp3", ResourceBGMdata);
}

static void NetworkUtilClientMain()
{
    PlayerCommonEntryEvent();
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

void PlayerOnInit(int plr, int pUnit)
{
    player[plr] = pUnit;
    player[plr + 10] = 1;

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
        FrameTimerWithArg(60, pUnit, NetPlayCustomBgm);
    }
}

int PlayerCameraSetup(int plr)
{
    int unit = CreateObjectAt("Maiden", GetObjectX(player[plr]), GetObjectY(player[plr]));

    Enchant(unit, "ENCHANT_INVULNERABLE", 0.0);
    Frozen(unit, 1);
    UnitNoCollide(unit);
    return unit;
}

void ObserverModeGuideMessage(int playerUnit)
{
    if (CurrentHealth(playerUnit))
    {
        UniPrint(playerUnit, "이미 게임이 진행중이라서 옵저버 모드상태로 전환됩니다");
        UniPrint(playerUnit, "게임이 재시작 되면 참가 가능하니 기다려 주세요");
    }
}

void RemoveShield(int unit)
{
    int cur = GetLastItem(unit), del = 0;

    while (cur)
    {
        if (HasSubclass(cur, "SHIELD"))
            del = cur;
        else
            del = 0;
        cur = GetPreviousItem(cur);
        if (del)
            Delete(del);
    }
}

void PlayerOnFailJoin()
{
    MoveObject(OTHER, GetWaypointX(12), GetWaypointY(12));
}

void PlayerOnJoin(int plr)
{
    if (CheckPlayerDeathFlag(plr))
        SetPlayerDeathFlag(plr);
    if (OnGameAlive)
    {
        MoveObject(player[plr], GetWaypointX(29), GetWaypointY(29));
        PlrCam[plr] = PlayerCameraSetup(plr);
        FrameTimerWithArg(30, player[plr], ObserverModeGuideMessage);
    }
    else
    {
        RemoveShield(player[plr]);
        MoveObject(player[plr], GetWaypointX(11), GetWaypointY(11));
    }
}

void PlayerEntryHandler()
{
    int plr, i;

    if (CurrentHealth(OTHER))
    {
        plr = CheckPlayer();
        for (i = 9 ; i >= 0 && plr < 0 ; i --)
        {
            if (!MaxHealth(player[i]))
            {
                PlayerOnInit(i, GetCaller());
                plr = i;
                break;
            }
        }
        if (plr + 1)
            PlayerOnJoin(plr);
        else
            PlayerOnFailJoin();
    }
}

int CheckPlayerDeathFlag(int plr)
{
    return player[plr + 10] & 0x02;
}

void SetPlayerDeathFlag(int plr)
{
    player[plr + 10] = player[plr + 10] ^ 0x02;
}

int CheckPlayerOnGameFlag(int plr)
{
    return player[plr + 10] & 0x04;
}

void SetPlayerOnGameFlag(int plr)
{
    player[plr + 10] = player[plr + 10] ^ 0x04;
}

void PlayerOnDeath(int plrIndex)
{
    int playerUnit = player[plrIndex];

    if (CheckPlayerOnGameFlag(plrIndex))
    {
        DecreaseAlivePlayerCount();
        SetPlayerOnGameFlag(plrIndex);
    }
    UniPrintToAll("* " + PlayerIngameNick(playerUnit) + " 님이 격추되셨습니다");
}

void PlayerOnShutdown(int plrIndex)
{
    MoveObject(GetPlayerColorFlag(plrIndex), GetWaypointX(17 + plrIndex), GetWaypointY(17 + plrIndex));
    if (CheckPlayerOnGameFlag(plrIndex))
        DecreaseAlivePlayerCount();
    if (MaxHealth(PlrCam[plrIndex]))
    {
        Frozen(PlrCam[plrIndex], 0);
        Delete(PlrCam[plrIndex]);
    }
    if (CurrentHealth(player[plrIndex]))
    {
        if (HasEnchant(player[plrIndex], "ENCHANT_SLOWED"))
            EnchantOff(player[plrIndex], "ENCHANT_SLOWED");
        if (HasEnchant(player[plrIndex], "ENCHANT_HASTED"))
            EnchantOff(player[plrIndex], "ENCHANT_HASTED");
    }
    player[plrIndex] = 0;
    player[plrIndex + 10] = 0;
}

void CheckPlayerOutLimit(int plrIndex)
{
    if (GetObjectX(player[plrIndex]) < GetObjectX(OnSafeCenter))
    {
        if (HasEnchant(player[plrIndex], "ENCHANT_INVULNERABLE"))
            EnchantOff(player[plrIndex], "ENCHANT_INVULNERABLE");
        Damage(player[plrIndex], 0, MaxHealth(player[plrIndex]) + 1, 16);
    }
}

void PlayerCameraHandler(int plr)
{
    int pCenter = OnSafeCenter, cUnit = PlrCam[plr];

    MoveObject(cUnit, GetObjectX(pCenter), GetObjectY(pCenter));
    if (CheckWatchFocus(player[plr]))
        PlayerLook(player[plr], cUnit);
}

void PreservePlayerLoop()
{
    int i, limitOn = IsObjectOn(OnSafeCenter);

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
                    MoveObject(GetPlayerColorFlag(i), GetObjectX(player[i]), GetObjectY(player[i]));
                    if (limitOn)
                    {
                        if (CheckPlayerOnGameFlag(i))
                            CheckPlayerOutLimit(i);
                        else if (MaxHealth(PlrCam[i]))
                            PlayerCameraHandler(i);
                    }
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

int PlacingStartButton(int location)
{
    int unit = CreateObject("WeirdlingBeast", location);

    UnitNoCollide(CreateObject("TeleportWake", location));
    Frozen(unit + 1, 1);
    SetUnitMaxHealth(unit, 30);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    return unit;
}

int GetPlayerColorFlag(int plrIndex)
{
    return PlayerFlagPtr + (plrIndex * 2);
}

void MovingSafeZoneToRight(int ptr)
{
    if (IsObjectOn(ptr))
    {
        if (GetObjectX(ptr) < EndXProfile)
        {
            MoveObject(ptr, GetObjectX(ptr) + 2.0, GetObjectY(ptr));
            Effect("SENTRY_RAY", GetObjectX(ptr), GetObjectY(ptr) - 240.0, GetObjectX(ptr), GetObjectY(ptr) + 240.0);
            FrameTimerWithArg(1, ptr, MovingSafeZoneToRight);
        }
        else
            Delete(ptr);
    }
}

int PlacingSafeZoneCenter(int pUnit, int alivePlayer)
{
    int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(pUnit), GetObjectY(pUnit));

    LookWithAngle(unit, alivePlayer);
    FrameTimerWithArg(30, unit, MovingSafeZoneToRight);
    return unit;
}

void AllPlayerOut(int location)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (MaxHealth(player[i]))
        {
            if (CurrentHealth(player[i]))
                MoveObject(player[i], GetWaypointX(location), GetWaypointY(location));
            PlayerOnShutdown(i);
        }
    }
}

void GGOver(int pCenter, int over)
{
    Delete(pCenter);
    AllPlayerOut(13);
    OnGameAlive = 0;
    OnSafeCenter = 0;
    AudioEvent("ShortBellsUp", 13);
    if (over)
        UniPrintToAll("게임 초기화: 플레이어가 전멸 되었습니다");
}

void DecreaseAlivePlayerCount()
{
    int pCenter = OnSafeCenter, alivePlayer;

    if (IsObjectOn(pCenter))
    {
        alivePlayer = GetDirection(pCenter);
        if (alivePlayer)
        {
            alivePlayer --;
            LookWithAngle(pCenter, alivePlayer);
            if (!alivePlayer)
                GGOver(pCenter, 1);
        }
    }
}

void PlayerTrackinSuccessCheckMessage(int plrUnit)
{
    if (CurrentHealth(plrUnit))
    {
        UniChatMessage(plrUnit, "어이... 그 뒤는 [죽음] 이다", 150);
        UniPrint(plrUnit, "살고싶으면... 멈추거나 뒤를 돌아보지 말고 무조건 뛰어!!");
        UniPrint(plrUnit, "왼쪽에서 튀어나오는 자색광선에 닿으면 즉사합니다");
    }
}

int PlayerOnTrackin(int destUnit, int plr)
{
    if (CurrentHealth(player[plr]))
    {
        Enchant(player[plr], "ENCHANT_ANCHORED", 0.0);
        Enchant(player[plr], "ENCHANT_SLOWED", 0.0);
        Enchant(player[plr], "ENCHANT_HASTED", 0.0);
        Enchant(player[plr], "ENCHANT_FREEZE", 2.0);
        MoveObject(player[plr], GetObjectX(destUnit), GetObjectY(destUnit));
        FrameTimerWithArg(60, player[plr], PlayerTrackinSuccessCheckMessage);
        if (!CheckPlayerOnGameFlag(plr))
            SetPlayerOnGameFlag(plr);
        return 1;
    }
    return 0;
}

void TeleportToTrack(int ptr)
{
    int i, alivePlayer = 0;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (PlayerOnTrackin(ptr + 1, i))
        {
            MoveObject(ptr + 1, GetObjectX(ptr + 1), GetObjectY(ptr + 1) + 19.0);
            alivePlayer ++;
        }
    }
    if (alivePlayer)
    {
        OnSafeCenter = PlacingSafeZoneCenter(ptr + 2, alivePlayer);
        OnGameAlive = 1;
    }
    else
    {
        OnSafeCenter = 0;
        OnGameAlive = 0;
    }
}

void GameReady(int ptr)
{
    int count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (count)
        {
            LookWithAngle(ptr, count - 1);
            FrameTimerWithArg(1, ptr, GameReady);
            break;
        }
        TeleportToTrack(ptr);
        Delete(ptr);
        Delete(ptr + 1);
        Delete(ptr + 2);
        break;
    }
}

void EnableObject(int unit)
{
    ObjectOn(unit);
}

void StartTheGame()
{
    int unit;

    if (VaildFlag)
    {
        if (IsObjectOn(unit))
            return;
        else if (!OnGameAlive)
        {
            unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(SELF), GetObjectY(SELF));
            CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(DestMarker), GetObjectY(DestMarker));
            CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(DestMarker + 1), GetObjectY(DestMarker + 1));
            LookWithAngle(unit, 150);
            FrameTimerWithArg(3, unit, GameReady);
            UniPrintToAll("5 초 후에 출발합니다, 얼른 참가하십시오!");
        }
    }
    ObjectOff(SELF);
    FrameTimerWithArg(180, GetTrigger(), EnableObject);
}

int LightObelisk(int location)
{
    int unit = CreateObject("BomberGenerator", location);

    SetUnitMass(unit, 60.0);
    Enchant(unit, "ENCHANT_INVULNERABLE", 0.0);
    ObjectOff(CreateObject("PlayerWaypoint", location) - 1);

    return unit;
}

void TouchTeleportPad()
{
    if (CurrentHealth(OTHER))
    {
        if (IsPlayerUnit(OTHER))
        {
            MoveObject(OTHER, GetObjectX(GetTrigger() + 2), GetObjectY(GetTrigger() + 2));
        }
    }
}

void ResetObeliskPadHandler(int ptr)
{
    int count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (count)
        {
            LookWithAngle(ptr, count - 1);
            FrameTimerWithArg(1, ptr, ResetObeliskPadHandler);
            break;
        }
        SetCallback(ToInt(GetObjectZ(ptr)), 9, TouchObeliskPortalPad);
        Delete(ptr);
        Delete(ptr + 1);
        break;
    }
}

void TouchObeliskPortalPad()
{
    int unit;

    if (CurrentHealth(OTHER))
    {
        if (HasClass(OTHER, "MONSTERGENERATOR"))
        {
            MoveObject(OTHER, GetObjectX(GetCaller() + 1), GetObjectY(GetCaller() + 1));
            unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(SELF), GetObjectY(SELF));
            Enchant(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), "ENCHANT_RUN", 0.0);
            LookWithAngle(unit, 120);
            Raise(unit, GetTrigger());
            FrameTimerWithArg(1, unit, ResetObeliskPadHandler);
            SetCallback(SELF, 9, TouchTeleportPad);
        }
    }
}

int ObeliskPortalPad(int location, int destLocation)
{
    int unit = CreateObject("WeirdlingBeast", location);

    SetUnitMaxHealth(CreateObject("BlueSummons", location) - 1, 30);
    CreateObject("PlayerWaypoint", destLocation);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    SetCallback(unit, 9, TouchObeliskPortalPad);
    return unit;
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

void MissileShooterHurt()
{
    if (CurrentHealth(SELF) ^ MaxHealth(SELF))
    {
        RestoreHealth(SELF, MaxHealth(SELF) - CurrentHealth(SELF));
    }
}

void MissileShooterDetectEnemy()
{
    int mis = CreateObjectAt("ImpShot", GetObjectX(SELF) + UnitRatioX(OTHER, SELF, 13.0), GetObjectY(SELF) + UnitRatioY(OTHER, SELF, 13.0));

    Enchant(SELF, "ENCHANT_BLINDED", 0.0);
    SetOwner(SELF, mis);
    if (ToInt(GetObjectZ(GetTrigger() + 1)) ^ GetCaller())
        Raise(GetTrigger() + 1, GetCaller());
    PushObject(mis, -28.0, GetObjectX(OTHER), GetObjectY(OTHER));
}

void MissileShooterLostEnemy()
{
    int enemy = ToInt(GetObjectZ(GetTrigger() + 1));

	EnchantOff(SELF, "ENCHANT_BLINDED");
	if (CurrentHealth(enemy) && IsVisibleTo(SELF, enemy))
	{
		Attack(SELF, enemy);
	}
    LookWithAngle(SELF, GetDirection(GetTrigger() + 1));
}

int MissileShooter(int location, int dirAngle)
{
    int unit = CreateObject("FireSprite", location);
    int ptr = GetMemory(0x750710);

    LookWithAngle(CreateObject("InvisibleLightBlueHigh", location), dirAngle);
    SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FireSpriteBinTable());
    SetUnitMaxHealth(unit, 500);
    Enchant(unit, "ENCHANT_FREEZE", 0.0);
    SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000);
    SetUnitScanRange(unit, 600.0);
    SetUnitMass(unit, 9999.0);
    SetCallback(unit, 3, MissileShooterDetectEnemy);
    SetCallback(unit, 7, MissileShooterHurt);
    SetCallback(unit, 13, MissileShooterLostEnemy);
    return unit;
}

int FlameShooterStatue(int upLoc, int downLoc)
{
    int unit = CreateObject("InvisibleLightBlueHigh", upLoc);

    UnitNoCollide(CreateObject("MovableStatueVictory3N", downLoc));
    UnitNoCollide(CreateObject("MovableStatueVictory3S", upLoc));
    return unit;
}

void TakeFlameShot(int trapPtr)
{
    int target = ToInt(GetObjectZ(trapPtr)), mUnit;

    if (IsObjectOn(trapPtr))
    {
        mUnit = CreateObjectAt("Fireball", GetObjectX(trapPtr + 1), GetObjectY(trapPtr + 1));
        LookAtObject(CreateObjectAt("Fireball", GetObjectX(trapPtr + 2), GetObjectY(trapPtr + 2)), target);
        LookAtObject(mUnit, target);
        PushObject(mUnit, -24.0, GetObjectX(target), GetObjectY(target));
        PushObject(mUnit + 1, -24.0, GetObjectX(target), GetObjectY(target));
    }
}

void FlameTrapResetCountdown(int trapPtr)
{
    int count = GetDirection(trapPtr);

    if (count)
    {
        LookWithAngle(trapPtr, count - 1);
        FrameTimerWithArg(1, trapPtr, FlameTrapResetCountdown);
    }
}

void PressFlameShooterPad()
{
    int trapPtr;

    if (CurrentHealth(OTHER))
    {
        trapPtr = GetOwner(GetTrigger() + 1);
        if (IsObjectOn(trapPtr))
        {
            if (!GetDirection(trapPtr))
            {
                LookWithAngle(trapPtr, 18);
                Raise(trapPtr, GetTrigger());
                FrameTimerWithArg(2, trapPtr, TakeFlameShot);
                FrameTimerWithArg(1, trapPtr, FlameTrapResetCountdown);
            }
        }
    }
}

int PlacingFlameShootPad(int location, int trapPtr)
{
    int unit = CreateObject("CarnivorousPlant", location);

    SetCallback(CreateObject("MagicEnergy", location) - 1, 9, PressFlameShooterPad);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    SetOwner(trapPtr, unit + 1);
    return unit;
}

void SetupFlameTrap(int count)
{
    int trapPtr = FlameShooterStatue(38, 39), i;

    MoveWaypoint(1, GetObjectX(trapPtr), GetObjectY(trapPtr) + 92.0);
    for (i = 0 ; i < 8 ; i ++)
    {
        PlacingFlameShootPad(1, trapPtr);
        MoveWaypoint(1, GetWaypointX(1), GetWaypointY(1) + 46.0);
    }
    TeleportLocationVector(38, 46.0, 0.0);
    MoveWaypoint(39, GetWaypointX(38), GetWaypointY(39));
    if (count)
        FrameTimerWithArg(1, count - 1, SetupFlameTrap);
}

void RespawnPolyp(int respawn)
{
    int durate = GetDirection(respawn);

    while (IsObjectOn(respawn))
    {
        if (durate)
        {
            FrameTimerWithArg(15, respawn, RespawnPolyp);
            LookWithAngle(respawn, durate - 1);
            break;
        }
        SummonPolyp(respawn);
        Delete(respawn);
        break;
    }
}

void PolypCollide()
{
    if (IsPlayerUnit(OTHER))
    {
        if (CurrentHealth(SELF))
        {
            int respawn = CreateObjectAt("AmbBeachBirds", GetObjectX(SELF), GetObjectY(SELF));
            LookWithAngle(respawn, 60);
            FrameTimerWithArg(15, respawn, RespawnPolyp);
            Frozen(SELF, FALSE);
            Damage(OTHER, SELF, 20, 5);
            Damage(SELF, 0, 99, 14);
        }
    }
}

int SummonPolyp(int anyunit)
{
    int polyp;

    if (anyunit < GetHost())
        polyp = CreateObjectAt("Polyp", LocationX(anyunit), LocationY(anyunit));
    else
        polyp = CreateObjectAt("Polyp", GetObjectX(anyunit), GetObjectY(anyunit));
    int *ptr = UnitToPtr(polyp);

    if (ptr != NULLPTR)
    {
        ptr[174] = ImportUnitCollideFunc();
        ptr[191] = PolypCollide;
        Frozen(polyp, TRUE);
    }
    return polyp;
}

void PlacingArea1()
{
    MissileShooter(36, 64);
    MissileShooter(37, 192);
    FrameTimerWithArg(1, 10, SetupFlameTrap);
    int u;

    for (u = 79 ; u <= 108 ; u += 1)
        SummonPolyp(u);
}

int PlacingDummy(int location, string unitName)
{
    int unit = CreateObject(unitName, location);

    if (CurrentHealth(unit))
    {
        ObjectOff(unit);
        Damage(unit, 0, MaxHealth(unit) + 1, -1);
        Frozen(unit, 1);
    }
    return unit;
}

int PlacingDummyAtLocation(float xProfile, float yProfile, string unitName)
{
    int unit = CreateObjectAt(unitName, xProfile, yProfile);

    ObjectOff(unit);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    Frozen(unit, 1);
    return unit;
}

int CommonTouchedExit()
{
    int pCenter;

    if (CurrentHealth(OTHER) && IsObjectOn(SELF))
    {
        if (!IsPlayerUnit(OTHER))
            return 0;
        Delete(SELF);
        pCenter = OnSafeCenter;
        if (IsObjectOn(pCenter))
        {
            GGOver(pCenter, 0);
            return 1;
        }
    }
    return 0;
}

void VictoryEvent()
{
    MoveWaypoint(1, LocationX(78), LocationY(78));
    FrameTimerWithArg(240, CreateObject("BlueRain", 1), UnitDelayRemove);
    Effect("WHITE_FLASH", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
    AudioEvent("BigGong", 1);
    UniPrintToAll("여러분!! 이겼습니다!!");
}

void GameEndAll()
{
    VaildFlag = 0;
}

void Area2Clear()
{
    if (CommonTouchedExit())
    {
        Delete(WispBlckTrp);
        Delete(FireWayTrp);
        Delete(Area2BombTrp2);
        Delete(Area2BombTrp1);
        GameEndAll();
        FrameTimer(30, VictoryEvent);
        UniPrintToAll("에리아 2을 클리어 하셨습니다!");
    }
}

void StartArea2()
{
    int padUnit = CreateObject("InvisibleLightBlueLow", 64);

    Raise(padUnit, 12);
    LookWithAngle(padUnit, 8);
    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(padUnit), GetObjectY(padUnit)), 46.0);
    FrameTimerWithArg(100, padUnit, PlaceIcePadAll);
    FrameTimer(120, SpecificPlaceIcePad);
    FrameTimerWithArg(3, 0, MakeGeneratorFactory);
    FrameTimer(30, PlaceBomberTraps);
    FrameTimerWithArg(60, 61, StartFireboomTrap);
    MoveObject(DestMarker, LocationX(28), LocationY(28));
    MoveObject(DestMarker + 1, LocationX(27), LocationY(27));
    WispBlckTrp = WispBlockingTrap(62, 11);

    PlacingExit(75, Area2Clear);
}

void Area1Clear()
{
    if (CommonTouchedExit())
    {
        FrameTimer(1, StartArea2);
        UniPrintToAll("에리아 1을 클리어 하셨습니다!");
    }
}

int PlacingExit(int location, int collideFunction)
{
    int out = CreateObjectAt("TraderAppleCrate", LocationX(location), LocationY(location));
    int *ptr = UnitToPtr(out);

    if (ptr != NULLPTR && collideFunction)
    {
        ptr[174] = ImportUnitCollideFunc();
        ptr[191] = collideFunction;
        Frozen(out, TRUE);
    }
    return out;
}

void CollideExplosion()
{
    if (CurrentHealth(OTHER))
    {
        Damage(OTHER, 0, 100, 14);
        Effect("SPARK_EXPLOSION", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
    }
}

void ExplosionTargetLocation(float xProfile, float yProfile)
{
    int eUnit = PlacingDummyAtLocation(xProfile, yProfile, "CarnivorousPlant");

    DeleteObjectTimer(CreateObjectAt("MeteorExplode", xProfile, yProfile), 9);
    DeleteObjectTimer(eUnit, 1);
    SetCallback(eUnit, 9, CollideExplosion);
}

void ExplosionToRight(int ptr)
{
    int count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (count)
        {
            ExplosionTargetLocation(GetObjectX(ptr), GetObjectY(ptr));
            MoveObject(ptr, GetObjectX(ptr) + 46.0, GetObjectY(ptr));
            LookWithAngle(ptr, count - 1);
            FrameTimerWithArg(4, ptr, ExplosionToRight);
            break;
        }
        Delete(ptr);
        break;
    }
}

void PlaceExplosionVector(int ptr)
{
    int count = GetDirection(ptr), unit;

    while (IsObjectOn(ptr))
    {
        if (count)
        {
            unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(ptr), GetObjectY(ptr));
            LookWithAngle(unit, 14);
            ExplosionToRight(unit);
            MoveObject(ptr, GetObjectX(ptr), GetObjectY(ptr) + GetObjectZ(ptr));
            LookWithAngle(ptr, count - 1);
            FrameTimerWithArg(18, ptr, PlaceExplosionVector);
            break;
        }
        MoveObject(ptr, GetObjectX(ptr + 1), GetObjectY(ptr + 1));
        LookWithAngle(ptr, GetDirection(ptr + 1));
        FrameTimerWithArg(29, ptr, PlaceExplosionVector);
        break;
    }
}

int PutBomberTrap(int location, float vector, int bombCount)
{
    int unit = CreateObject("InvisibleLightBlueLow", location);
    Raise(CreateObject("InvisibleLightBlueLow", location) - 1, vector);
    LookWithAngle(unit + 1, bombCount);
    LookWithAngle(unit, bombCount);
    FrameTimerWithArg(3, unit, PlaceExplosionVector);
    return unit;
}

void PlaceBomberTraps()
{
    Area2BombTrp1 = PutBomberTrap(53, 46.0, 8);
    Area2BombTrp2 = PutBomberTrap(54, -46.0, 8);
}

void GeneratorCrashEvent()
{
    SecondTimerWithArg(20, CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(GetTrigger() + 1), GetObjectY(GetTrigger() + 1)), GeneratorRespawn);
    DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(SELF), GetObjectY(SELF)), 12);
    Delete(SELF);
    Delete(GetTrigger() + 1);
}

void HittingGenerator()
{
    if (CurrentHealth(SELF) <= 1000)
    {
        GeneratorCrashEvent();
    }
}

void GeneratorDetailProperty(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x34, HittingGenerator); //hitting event
    }
}

int PlaceRespawnGenerator(int location)
{
    int unit = CreateObject("DemonGenerator", location);

    CreateObject("InvisibleLightBlueHigh", location);
    Enchant(unit, "ENCHANT_FREEZE", 0.0);
    ObjectOff(unit);
    SetUnitMaxHealth(unit, 1400);
    GeneratorDetailProperty(unit);

    return unit;
}

void GeneratorRespawn(int respTarget)
{
    if (IsObjectOn(respTarget))
    {
        MoveWaypoint(1, GetObjectX(respTarget), GetObjectY(respTarget));
        PlaceRespawnGenerator(1);
        Delete(respTarget);
    }
}

void BuildGeneratorField(int ptr)
{
    int count = GetDirection(ptr);

    while (IsObjectOn(ptr))
    {
        if (count)
        {
            MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
            PlaceRespawnGenerator(1);
            MoveObject(ptr, GetObjectX(ptr) + 92.0, GetObjectY(ptr));
            LookWithAngle(ptr, count - 1);
            FrameTimerWithArg(1, ptr, BuildGeneratorField);
            break;
        }
        Delete(ptr);
        break;
    }
}

void MakeGeneratorFactory(int count)
{
    int unit;

    if (count < 6)
    {
        unit = CreateObject("InvisibleLightBlueLow", 55 + count);
        LookWithAngle(unit, 10);
        BuildGeneratorField(unit);
        FrameTimerWithArg(1, count + 1, MakeGeneratorFactory);
    }
}

void MovingFireboom(int fboom)
{
    int count = GetDirection(fboom);

    if (IsObjectOn(fboom))
    {
        if (count)
        {
            ExplosionTargetLocation(GetObjectX(fboom), GetObjectY(fboom));
            MoveObject(fboom, GetObjectX(fboom) + GetObjectZ(fboom), GetObjectY(fboom));
            LookWithAngle(fboom, count - 1);
            FrameTimerWithArg(4, fboom, MovingFireboom);
        }
        else
        {
            LookWithAngle(fboom, GetDirection(fboom + 1));
            Raise(fboom, GetObjectZ(fboom) * -1.0);
            FrameTimerWithArg(5, fboom, MovingFireboom);
        }
    }
}

void StartFireboomTrap(int location)
{
    int fboom = CreateObject("InvisibleLightBlueLow", location);

    LookWithAngle(CreateObject("InvisibleLightBlueLow", location), 17);
    LookWithAngle(fboom, 17);
    Raise(fboom, 46.0);
    FrameTimerWithArg(30, fboom, MovingFireboom);

    FireWayTrp = fboom;
}

void BlockingWispCollide()
{
    if (CurrentHealth(OTHER))
    {
        Damage(OTHER, 0, 150, 14);
    }
}

int WispBlockingCreateSingle(float xProfile, float yProfile)
{
    int unit = CreateObjectAt("WillOWisp", xProfile, yProfile);

    Frozen(unit, 1);
    SetCallback(unit, 9, BlockingWispCollide);
    return unit;
}

void ShakeWispBlockingWall(int ptr)
{
    int count = GetDirection(ptr), i;
    float xProfile = GetObjectX(ptr);

    if (IsObjectOn(ptr))
    {
        if (count)
        {
            for (i = GetDirection(ptr + 2) ; i ; i -= 1)
                MoveObject(ptr + i, xProfile, GetObjectY(ptr + i) + GetObjectZ(ptr));
            LookWithAngle(ptr, count - 1);
        }
        else
        {
            Raise(ptr, GetObjectZ(ptr) * -1.0);
            LookWithAngle(ptr, GetDirection(ptr + 1));
        }
        FrameTimerWithArg(1, ptr, ShakeWispBlockingWall);
    }
}

int WispBlockingTrap(int location, int count)
{
    int unit = CreateObject("InvisibleLightBlueLow", location), i;
    float xProfile = GetWaypointX(location);

    Raise(unit, 2.0);
    LookWithAngle(unit, 57);
    for (i = 0 ; i < count ; i += 1)
        WispBlockingCreateSingle(xProfile, GetObjectY(unit + i) + 46.0);
    LookWithAngle(unit + 1, 57);
    LookWithAngle(unit + 2, count);
    FrameTimerWithArg(1, unit, ShakeWispBlockingWall);

    return unit;
}

void UnitDelayRemove(int unit)
{
    Delete(unit);
}

void IcePadCollide()
{
    int bUnit;

    if (CurrentHealth(OTHER))
    {
        if (HasEnchant(OTHER, "ENCHANT_BURNING"))
            return;
        bUnit = CreateObjectAt("BoulderCave", GetObjectX(OTHER) + UnitAngleCos(OTHER, 10.0), GetObjectY(OTHER) + UnitAngleSin(OTHER, 10.0));
        Frozen(bUnit, 1);
        DeleteObjectTimer(bUnit, 1);
        FrameTimerWithArg(6, CreateObjectAt("DamagePoof", GetObjectX(SELF), GetObjectY(SELF)), UnitDelayRemove);
        Enchant(bUnit + 1, "ENCHANT_FREEZE", 0.0);
        Enchant(OTHER, "ENCHANT_BURNING", 0.1);
    }
}

int PlaceIcePad(float xProfile, float yProfile)
{
    int unit = CreateObjectAt("WizardWhite", xProfile, yProfile);

    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    SetCallback(unit, 9, IcePadCollide);
    return unit;
}

void PlaceIcePadAll(int posUnit)
{
    int count = GetDirection(posUnit), ptr, xProfile = ToInt(GetObjectZ(posUnit)), i;
    float xVect = GetObjectZ(posUnit + 1);

    if (count)
    {
        ptr = CreateObjectAt("RedPotion", GetObjectX(posUnit) - xVect, GetObjectY(posUnit));
        for (i = 0 ; i < xProfile ; i ++)
            PlaceIcePad(GetObjectX(ptr + i) + xVect, GetObjectY(ptr + i));
        Delete(ptr);
        LookWithAngle(posUnit, count - 1);
        FrameTimerWithArg(3, posUnit, PlaceIcePadAll);
        MoveObject(posUnit, GetObjectX(posUnit), GetObjectY(posUnit) + xVect);
    }
    else
    {
        Delete(posUnit);
        Delete(posUnit + 1);
    }
}

void SpecificPlaceIcePad()
{
    int i;
    for (i = 65 ; i <= 74 ; i ++)
        PlaceIcePad(GetWaypointX(i), GetWaypointY(i));
}

void InitFunnyObeliskPart()
{
    LightObelisk(30);
    LightObelisk(31);
    ObeliskPortalPad(32, 34);
    ObeliskPortalPad(33, 35);
    PlacingArea1();
    PlacingExit(40, Area1Clear);
}

void DelayInitRun(int arg0)
{
    if (!IsObjectOn(arg0))
        return;
    DestMarker = CreateObject("InvisibleLightBlueHigh", 15);
    CreateObject("InvisibleLightBlueHigh", 16);
    SetCallback(PlacingStartButton(14), 9, PlayerEntryHandler);
    FrameTimer(2, InitFunnyObeliskPart);
    FrameTimer(3, PreservePlayerLoop);
    FrameTimer(30, GameGuideObjects);
}

void MapInitialize()
{
    PlayerFlagPtr = Object("PlayerFlagBase");
    
    InitMathSine(1);
    MusicEvent();
    FrameTimerWithArg(1, PlayerFlagPtr, DelayInitRun);
}

int PlaceInvisiblePad(int location)
{
    int unit = CreateObject("WeirdlingBeast", location);

    SetUnitMaxHealth(unit, 100);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    return unit;
}

void GuideMessageWhenCollideA11()
{
    if (CurrentHealth(OTHER))
    {
        if (HasEnchant(OTHER, "ENCHANT_BURNING"))
            return;
        Enchant(OTHER, "ENCHANT_BURNING", 3.0);
        CommonGuideMessage();
        UniPrint(OTHER, "오벨리스크를 푸른색 비콘위에 올리면 비콘에 화학반응이 일어납니다. 이때 비콘을 밟으시면 옆구간으로 이동됩니다");
    }
}

void GameGuideObjects()
{
    string guide = "오벨리스크를 푸른색 비콘위에 올리면 비콘에 화학반응이 일어납니다. 이때 비콘을 밟으시면 옆구간으로 이동됩니다";

    RegistSignMessage(Object("PicketA1"), guide);
    RegistSignMessage(Object("PicketA11"), guide);

    RegistSignMessage(Object("PicketA2"), "벽에 기대어 가려는 곳 반대방향을 바라보세요");
    RegistSignMessage(Object("PicketA21"), "벽에 기대어 가려는 곳 반대방향을 바라보세요");

    RegistSignMessage(Object("Picket0"), "게임을 시작하려면 파란 가루에 다가가세요. 생존유저가 없을 시 구경하기 모드로 접속됩니다");
    RegistSignMessage(Object("Picket01"), "긴말 안한다. 살기위해 앞만 보고 달려라. 뒤 돌아보는 순간 넌 죽는거다");
}

void MapExit()
{
    MusicEvent();
}
