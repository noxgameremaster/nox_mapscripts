
#include "noxscript\builtins.h"
#include "libs\typecast.h"
#include "libs\callmethod.h"
#include "libs\unitutil.h"
#include "libs\buff.h"
#include "libs\printutil.h"
#include "libs\unitstruct.h"
#include "libs\playerupdate.h"
#include "libs\network.h"
#include "libs\mathlab.h"
#include "libs\memutil.h"
#include "libs\waypoint.h"
#include "libs\fxeffect.h"
#include "libs\fixtellstory.h"
#include "libs\spellutil.h"
#include "libs\username.h"
#include "libs\coopteam.h"
#include "libs\itemproperty.h"
#include "libs\cweaponproperty.h"
#include "libs\clientside.h"
#include "libs\wandpatch.h"

#include "libs\monsteraction.h"
#include "libs\imageutil.h"
#include "libs\magicmissile.h"

int XMonStat = 0;
int XMainFlag = 0;
int player[20];
int XTelpo;
int XItemNode;
int XCreCount = 0;

#define OTHER   -1
#define SELF        -2  

#define NULLPTR         0

/****
def BitShow(num):
	bitArr=[]
	t=1<<31
	for i in range(32):
		if num&t:
			bitArr.append(1)
		else:
			bitArr.append(0)
		t=t>>1
	print(bitArr)
****/

int CreClassGetCurrentCount()
{
    return XCreCount & 0x1f;
}

void CreClassSetCurrentCount(int setTo)
{
    XCreCount = (XCreCount & (~0x1f)) ^ setTo;
}

void CreClassIncreaseCurrentCount()
{
    CreClassSetCurrentCount(CreClassGetCurrentCount() + 1);
}

void CreClassDecreaseCurrentCount()
{
    if (CreClassGetCurrentCount())
        CreClassSetCurrentCount(CreClassGetCurrentCount() - 1);
}

void MoveObjectVector(int target, float xVect, float yVect)
{
    int ptr = UnitToPtr(target);

    if (ptr)
        MoveObject(target, GetMemoryFloat(ptr + 0x38) + xVect, GetMemoryFloat(ptr + 0x3c) + yVect);
}

int LineStatClassMobCountCheck()
{
    return XMonStat & 0xffff;
}

int LineStatClassKillCountCheck()
{
    return XMonStat >> 0x10;
}

void LineStatClassMobCountSet(int amount)
{
    XMonStat = (XMonStat & (~0xffff)) ^ amount;
}

void LineStatClassKillCountSet(int amount)
{
    XMonStat = (XMonStat & 0xffff) ^ (amount << 0x10);
}

int GameClassMobAccessCheck()
{
    return XMainFlag & 0xf;
}

void GameClassMobAccessSet(int setTo)
{
    XMainFlag = (XMainFlag & (~0xf)) ^ (setTo & 0xf);
}

int GameClassLifeCheck()
{
    return XMainFlag >> 0x18;
}

void GameClassLifeSet(int setTo)
{
    XMainFlag = (XMainFlag & 0xffffff) ^ ((setTo & 0xff) << 0x18);
}

int GameClassWaveCheck()
{
    return (XMainFlag >> 0x10) & 0xff;
}

void GameClassWaveSet(int setTo)
{
    XMainFlag = (XMainFlag & 0xff00ffff) ^ ((setTo & 0xfff) << 0x10);
}

int GameClassMainIsShutdown()
{
    return XMainFlag & 0x10;
}

void GameClassMainSetShutdown()
{
    XMainFlag = XMainFlag ^ 0x10;
}

int GameClassLineIsStart()
{
    return XMainFlag & 0x20;
}

void GameClassLineStartSet()
{
    XMainFlag = XMainFlag ^ 0x20;
}

void MapExit()
{
    MusicEvent();
    RemoveCoopTeamMode();
    ResetPlayerHandlerWhenExitMap();
    ResetHostileCritter();
}

void MapInitialize()
{
    FrameTimer(1, DelayInitRun);

    MusicEvent();
    
    ImportMonsterStrikeFunc();
    FrameTimer(2, SetGameTypeCoopMode);
}

void ImpShotCollide()
{
    int owner = GetOwner(self);

    while (owner)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 48, 1);
            Effect("EXPLOSION", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

void LoopCatchImpshot(int curId)
{
    int owner = GetOwner(curId);
    int mis = SummonCustomMagicMissile("DeathBallFragment", owner, 0.0);

    SetUnitCallbackOnCollide(mis, ImpShotCollide);
    Delete(curId);
}

void SpiderWebCollide()
{
    int owner = GetOwner(self);

    while (owner)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 99, 0);
            Enchant(other, EnchantList(4), 3.0);
            if (IsPlayerUnit(other))
                UniPrint(other, "거미줄이 당신의 움직임을 둔화 시키고 있습니다");
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

void DetectedSpecificIndex(int curId)
{
    int thingID = GetUnitThingID(curId);

    if (thingID == 708) //ImpShot
        LoopCatchImpshot(curId);
    else if (thingID == 524)
        SetUnitCallbackOnCollide(curId, ImpShotCollide);
    else if (thingID == 526)
        HarpoonEvent(GetOwner(curId), curId);
    else if (thingID == 1177)
        ChakramOnThrow(curId);
    else if (thingID == 706)
        ShotSmallDeathBall(GetOwner(curId), curId);
    else if (thingID == 709)
        ShotMagicMissile(GetOwner(curId), curId);
}

void LoopSearchIndex()
{
    int curId, tempId;
    int *ptr = GetMemory(0x750710);

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

int CheckPlayerScrIndex(int pUnit)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (!(player[i] ^ pUnit))
            return i;
    }
    return -1;
}

int PlayerClassOnInit(int pUnit, int plr)
{
    int res = plr;

    player[plr] = pUnit;
    player[plr + 10] = 1;
    EmptyAll(pUnit);
    if (ValidPlayerCheck(pUnit))
    {
        if (pUnit ^ GetHost())
            NetworkUtilClientEntry(pUnit);
        else
            PlayerClassCommonWhenEntry();
        FrameTimerWithArg(60, pUnit, NetPlayCustomBgm);
        SelfDamageClassEntry(pUnit);
        DiePlayerHandlerEntry(pUnit);
        res |= (1 << 8);
    }
    ChangeGold(pUnit, -GetGold(pUnit));
    FrameTimerWithArg(90, pUnit, UserShowGameInfo);
    return res;
}

void RespawnPlayer(int location, int pUnit)
{
    MoveObject(pUnit, LocationX(location), LocationY(location));
    PlaySoundAround(pUnit, 6);
    DeleteObjectTimer(CreateObjectAt("BlueRain", LocationX(location), LocationY(location)), 21);
    Effect("TELEPORT", LocationX(location), LocationY(location), 0.0, 0.0);
}

void PlayerClassOnJoin(int pUnit, int plr)
{
    if (PlayerClassDeathFlagCheck(plr))
        PlayerClassDeathFlagSet(plr);
    Enchant(pUnit, EnchantList(25), 2.0);
    Enchant(pUnit, EnchantList(14), 0.0);
    RespawnPlayer(15, pUnit);
}

void PlayerClassOnFailedEntry(int pUnit)
{
    if (GameClassMainIsShutdown())
    {
        MoveObject(pUnit, LocationX(30), LocationY(30));
        UniPrint(pUnit, "게임 오버. ..");
    }
    else
    {
        MoveObject(pUnit, LocationX(13), LocationY(13));
        UniPrint(pUnit, "지도 입장에 실패하였습니다");
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
    MoveObject(pUnit, LocationX(13), LocationY(13));
    FrameTimerWithArg(5, (plr << 10) | 11, TeleportPlayer);
    UniPrint(pUnit, "지도에 입장을 시도하고 있습니다... 잠시만 기다려 주세요");
}

void PlayerClassEntry()
{
    int i, plr;

    while (1)
    {
        if (CurrentHealth(other) && !GameClassMainIsShutdown())
        {
            plr = CheckPlayer();
            for (i = 9 ; i >= 0 && plr < 0 ; i --)
            {
                if (!MaxHealth(player[i]))
                {
                    plr = PlayerClassOnInit(GetCaller(), i);
                    break;
                }
            }
            if (plr + 1)
            {
                if (plr >> 8)
                    PlayerClassFirstJoin(other, plr & 0xff);
                else
                    PlayerClassOnJoin(other, plr);
                break;
            }
        }
        PlayerClassOnFailedEntry(other);
        break;
    }
}

void PlayerClassFastJoin()
{
    int pIndex;

    if (CurrentHealth(other))
    {
        pIndex = CheckPlayer();
        if (pIndex + 1)
            MoveObject(other, LocationX(11), LocationY(11));
        else
            MoveObject(other, LocationX(12), LocationY(12));
    }
}

void AbilityAwardFx(int sUnit)
{
    PlaySoundAround(sUnit, 226);
    GreenSparkFx(GetObjectX(sUnit), GetObjectY(sUnit));
    Effect("WHITE_FLASH", GetObjectX(sUnit), GetObjectY(sUnit), 0.0, 0.0);
}

void WarAbilityAwardWarcry(int owner, int plr)
{
    if (PlayerClassSkillFlagCheck2(plr))
        UniPrint(owner, "이미 이 능력을 배우셨습니다 -향상된 전쟁의 함성");
    else
    {
        PlayerClassSkillFlagSet2(plr);
        AbilityAwardFx(owner);
        UniPrint(owner, "향상된 전쟁의 함성을 배우셨습니다! 주변 유닛 데미지 85");
    }
}

void WarAbilityAwardHarpoon(int owner, int plr)
{
    if (PlayerClassSkillFlagCheck3(plr))
        UniPrint(owner, "이미 이 능력을 배우셨습니다 -향상된 작살");
    else
    {
        PlayerClassSkillFlagSet3(plr);
        AbilityAwardFx(owner);
        Enchant(owner, EnchantList(30), 0.0);
        UniPrint(owner, "향상된 작살을 배우셨습니다! 일직선 범위 데미지 100");
    }
}

void WarAbilityAwardThreadLigthly(int owner, int plr)
{
    if (PlayerClassSkillFlagCheck1(plr))
        UniPrint(owner, "이미 이 능력을 배우셨습니다 -윈드 부스터");
    else
    {
        PlayerClassSkillFlagSet1(plr);
        AbilityAwardFx(owner);
        UniPrint(owner, "윈드 부스터을 배우셨습니다! 짧은 거리를 빠르게 이동합니다");
    }
}

void BookOfAbilityCheck(int curId, int owner)
{
    int ptr = UnitToPtr(curId), type;
    int plr = CheckPlayerFromUnitId(owner);

    if (ptr && plr + 1)
    {
        while (1)
        {
            type = GetMemory(GetMemory(ptr + 0x2e0)) & 0xff;
            if (type == 2)
                WarAbilityAwardWarcry(owner, plr);
            else if (type == 3)
                WarAbilityAwardHarpoon(owner, plr);
            else if (type == 4)
                WarAbilityAwardThreadLigthly(owner, plr);
            else
                break;
            Delete(curId);
            break;
        }
    }
}

void InventoryChecking(int pUnit)
{
    int inv = GetLastItem(pUnit);

    if (GetUnitThingID(inv) == 2676)
        BookOfAbilityCheck(inv, pUnit);
}

void UserPoisonedHandler(int sPlr)
{
    int arr[10], plrUnit = player[sPlr];

    if (UnitCheckEnchant(plrUnit, GetLShift(18)))
    {
        if (IsPoisonedUnit(plrUnit))
        {
            CastSpellObjectObject("SPELL_CURE_POISON", plrUnit, plrUnit);
            Effect("VIOLET_SPARKS", GetObjectX(plrUnit), GetObjectY(plrUnit), 0.0, 0.0);
        }
    }
    else if (IsPoisonedUnit(plrUnit))
    {
        if (arr[sPlr] < 60)
            arr[sPlr] += IsPoisonedUnit(plrUnit);
        else
        {
            Damage(plrUnit, 0, IsPoisonedUnit(plrUnit), 5);
            arr[sPlr] = 0;
        }
    }
    else if (arr[sPlr])
        arr[sPlr] = 0;
}

void PlayerClassStatCheck(int plr)
{
    int pUnit = player[plr];

    InventoryChecking(pUnit);
    UserPoisonedHandler(plr);
    if (UnitCheckEnchant(pUnit, GetLShift(31)))
    {
        EnchantOff(pUnit, EnchantList(31));
        RemoveTreadLightly(pUnit);
        if (PlayerClassSkillFlagCheck1(plr))
            SkillSetWindBooster(pUnit);
    }
    if (PlayerClassSkillFlagCheck2(plr))
    {
        WarAbilityUse(pUnit, 2, SkillSetWarCry);
    }
}

void SkillSetWindBooster(int pUnit)
{
    PushObjectTo(pUnit, UnitAngleCos(pUnit, 80.0), UnitAngleSin(pUnit, 80.0));
    Effect("RICOCHET", GetObjectX(pUnit), GetObjectY(pUnit), 0.0, 0.0);
}

void ManaBombCancelFx(int sUnit)
{
    int caster = CreateObjectAt("ImaginaryCaster", GetObjectX(sUnit), GetObjectY(sUnit));

    CastSpellObjectObject("SPELL_MANA_BOMB", caster, caster);
    Delete(caster);
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

void SkillSetWarCry(int pUnit)
{
    SplashDamageAt(pUnit, 130, GetObjectX(pUnit), GetObjectY(pUnit), 200.0);
    ManaBombCancelFx(pUnit);
    UniChatMessage(pUnit, "프리 홍콩!! 타이완 넘버 원!!", 120);
}

int WarAbilityTable(int aSlot, int pIndex)
{
    return GetMemory(0x753600 + (pIndex * 24) + (aSlot * 4));
}

void WarAbilityUse(int pUnit, int aSlot, int actionFunction)
{
    int chk[160], pIndex = GetPlayerIndex(pUnit), cTime;
    int arrPic;

    if (!(pIndex >> 0x10))
    {
        arrPic = pIndex * 5 + aSlot; //EyeOf=5, harpoon=3, sneak=4, berserker=1
        cTime = WarAbilityTable(aSlot, pIndex);
        if (cTime ^ chk[arrPic])
        {
            if (!chk[arrPic])
            {
                CallFunctionWithArg(actionFunction, pUnit);
            }
            chk[arrPic] = cTime;
        }
    }
}

int PlayerClassDeathFlagCheck(int pIndex)
{
    return player[pIndex + 10] & 0x02;
}

void PlayerClassDeathFlagSet(int pIndex)
{
    player[pIndex + 10] = player[pIndex + 10] ^ 0x02;
}

int PlayerClassSkillFlagCheck1(int pIndex)
{
    return player[pIndex + 10] & 0x04;
}

void PlayerClassSkillFlagSet1(int pIndex)
{
    player[pIndex + 10] = player[pIndex + 10] ^ 4;
}

int PlayerClassSkillFlagCheck2(int pIndex)
{
    return player[pIndex + 10] & 0x08;
}

void PlayerClassSkillFlagSet2(int pIndex)
{
    player[pIndex + 10] = player[pIndex + 10] ^ 8;
}

int PlayerClassSkillFlagCheck3(int pIndex)
{
    return player[pIndex + 10] & 0x10;
}

void PlayerClassSkillFlagSet3(int pIndex)
{
    player[pIndex + 10] = player[pIndex + 10] ^ 0x10;
}

void ProtectItemTryPick()
{
    int cFps, ptr;

    if (IsPlayerUnit(other))
    {
        ptr = UnitToPtr(self);
        while (1)
        {
            if (GetOwner(self) ^ GetCaller())
            {
                cFps = GetMemory(0x84ea04);
                if (!GetMemory(ptr + 0x2f4))
                    SetMemory(ptr + 0x2f4, cFps);
                if (MathAbs(cFps - GetMemory(ptr + 0x2f4)) >= 90)
                    1;
                else
                {
                    UniPrint(other, "아직 이 아이템을 픽업하실 수 없습니다. 소유자가 있는 아이템 입니다");
                    break;
                }
            }
            SetMemory(ptr + 0x2c4, GetMemory(ptr + 0x1c));
            SetMemory(ptr + 0x1c, 0);
            SetMemory(ptr + 0x228, 0);
            SetMemory(ptr + 0x2f4, 0);
            Pickup(other, self);
            break;
        }
    }
}

void LaiserParTouch()
{
    int owner = ToInt(GetObjectZ(GetOwner(self)));

    if (IsAttackedBy(other, owner) && CurrentHealth(other))
    {
        Damage(other, owner, 100, 14);
        Enchant(other, EnchantList(28), 0.5);
    }
}

void HarpoonEvent(int owner, int curId)
{
    float xVect, yVect;
    int i, unit;

    if (UnitCheckEnchant(owner, GetLShift(30)))
    {
        xVect = UnitAngleCos(owner, 30.0);
        yVect = UnitAngleSin(owner, 30.0);

        Delete(curId);
        TeleportLocation(1, GetObjectX(owner) + xVect, GetObjectY(owner) + yVect);
        unit = CreateObjectAt("InvisibleLightBlueLow", LocationX(1), LocationY(1)) + 1;
        Raise(unit - 1, owner);
        for (i = 0 ; i < 15 ; i ++)
        {
            Frozen(CreateObjectAt("Maiden", LocationX(1), LocationY(1)), 1);
            SetCallback(unit + i, 9, LaiserParTouch);
            SetUnitFlags(unit + i, GetUnitFlags(unit + i) ^ 0x2000);
            SetOwner(unit - 1, unit + i);
            DeleteObjectTimer(unit + i, 1);
            PlaySoundAround(unit + i, 299);
            TeleportLocationVector(1, xVect, yVect);
        }
        DeleteObjectTimer(unit - 1, 30);
        Effect("SENTRY_RAY", GetObjectX(unit - 1), GetObjectY(unit - 1), LocationX(1), LocationY(1));
    }
}

void ChakramOnThrow(int curId)
{ }

void ShotSmallDeathBall(int owner, int curId)
{ }

void ShotMagicMissile(int owner, int curId)
{ }

void PlayerClassOnDeath(int pIndex)
{
    UniPrintToAll(PlayerIngameNick(player[pIndex]) + " 님께서 적에게 격추되었습니다");
}

void PlayerClassOnFree(int pIndex)
{
    player[pIndex] = 0;
    player[pIndex + 10] = 0;
}

void PlayerClassLoop()
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        while (1)
        {
            if (MaxHealth(player[i]))
            {
                if (GetUnitFlags(player[i]) & 0x40)
                    1;
                else if (CurrentHealth(player[i]))
                {
                    PlayerClassStatCheck(i);
                    break;
                }
                else
                {
                    if (!PlayerClassDeathFlagCheck(i))
                    {
                        PlayerClassDeathFlagSet(i);
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
    FrameTimer(1, PlayerClassLoop);
}

void EmptyAll(int sUnit)
{
    while (GetLastItem(sUnit))
        Delete(GetLastItem(sUnit));
}

void SetGameTypeCoopMode()
{
    MakeCoopTeam();

    SetMemory(0x5d5330, 0x2000);
    SetMemory(0x5d5394, 1);
}

int CheckPlayerFromUnitId(int unit)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (!(player[i] ^ unit))
            return i;
    }
    return -1;
}

void GreenSparkFx(float xProfile, float yProfile)
{
    int fxTarget = CreateObjectAt("MonsterGenerator", xProfile, yProfile);

    Damage(fxTarget, 0, 1, -1);
    Delete(fxTarget);
}

void TeleportProgress(int sUnit)
{
    int owner = GetOwner(sUnit), dest = ToInt(GetObjectZ(sUnit)), count = GetDirection(sUnit);

    while (1)
    {
        if (CurrentHealth(owner))
        {
            if (count)
            {
                if (DistanceUnitToUnit(sUnit, owner) < 23.0)
                {
                    LookWithAngle(sUnit, count - 1);
                    FrameTimerWithArg(1, sUnit, TeleportProgress);
                    break;
                }
            }
            else
            {
                Effect("TELEPORT", GetObjectX(sUnit), GetObjectY(sUnit), 0.0, 0.0);
                Effect("SMOKE_BLAST", GetObjectX(sUnit), GetObjectY(sUnit), 0.0, 0.0);
                MoveObject(owner, GetObjectX(dest), GetObjectY(dest));
                PlaySoundAround(owner, 6);
                Effect("TELEPORT", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
            }
            EnchantOff(owner, "ENCHANT_BURNING");
        }
        Delete(sUnit);
        Delete(sUnit + 1);
        break;
    }
}

void TeleportPortal()
{
    int unit;

    if (CurrentHealth(other) && IsObjectOn(other))
    {
        if (!UnitCheckEnchant(other, GetLShift(12)))
        {
            Enchant(other, EnchantList(12), 4.0);
            unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(other), GetObjectY(other));
            Raise(unit, GetTrigger() + 1);
            LookWithAngle(unit, 48); //TODO: 1.XX seconds...
            CreateObjectAt("VortexSource", GetObjectX(unit), GetObjectY(unit));
            Effect("YELLOW_SPARKS", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
            SetOwner(other, unit);
            TeleportProgress(unit);
            PlaySoundAround(other, 772);
            UniPrint(other, "공간이동을 준비 중 입니다. 취소하려면 움직이세요");
        }
    }
}

int TeleportSetup(int srcWp, int dstWp)
{
    int unit = CreateObjectAt("WeirdlingBeast", LocationX(srcWp), LocationY(srcWp));

    SetUnitMaxHealth(CreateObjectAt("InvisibleLightBlueHigh", LocationX(dstWp), LocationY(dstWp)) - 1, 10);
    Enchant(CreateObjectAt("InvisibleLightBlueHigh", LocationX(srcWp), LocationY(srcWp)), "ENCHANT_ANCHORED", 0.0);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    SetCallback(unit, 9, TeleportPortal);

    return unit;
}

void ModifySetupTeleportDestination(int sUnit, int modLocation)
{
    if (MaxHealth(sUnit))
    {
        if (IsObjectOn(sUnit + 1))
            MoveObject(sUnit + 1, LocationX(modLocation), LocationY(modLocation));
    }
}

int MasterMonster()
{
    int master;

    if (!master)
    {
        master = CreateObjectAt("Hecubah", LocationX(14), LocationY(14));
        Frozen(master, 1);
    }
    return master;
}

void PlaceImagePreProc()
{
    DrawImageAtLocation(55, 2507);
    DrawImageAtLocation(57, 2510);
    DrawImageAtLocation(56, 2509);
    DrawImageAtLocation(58, 2508);
    DrawImageAtLocation(59, 2508);
    DrawImageAtLocation(60, 2508);
    DrawImageAtLocation(61, 2508);

    DrawImageAtLocation(75, 2518);
    DrawImageAtLocation(76, 2558);
    DrawImageAtLocation(77, 2559);
    DrawImageAtLocation(78, 2560);
}

void Description()
{
    SellGermPlace(50);
    RepairItemPlaceKp(51);
    SellGermPlace(52);
    RepairItemPlaceKp(53);
    RegistSignMessage(Object("MPik4"), "중국인 입국 막기를 시작하려면 오른쪽 스위치를 누르세요");
    RegistSignMessage(Object("MPik3"), "이 선을 넘어가면 라이프가 하나 씩 깎임!!");
    RegistSignMessage(Object("MPik2"), "입국 심사장: 저 너머 문을 통해 중국인들이 입국을 시작한다. 우리는 그들을 막아야만 한다");
    RegistSignMessage(Object("MPik1"), "크리쳐 지우기 비콘: 지우려는 크리쳐를 이 비콘 위에 올리면 지울 수 있습니다. 3천골드를 되돌려 받게 돼요");
}

void DelayInitRun()
{
    ManaTileAll(18);
    FrameTimer(3, PlaceImagePreProc);
    FrameTimer(5, Description);
    MasterMonster();
    XTelpo = TeleportSetup(22, 16);
    XItemNode = CreateObjectAt("ImaginaryCaster", LocationX(1), LocationY(1));

    GameClassLifeSet(20);
    FrameTimerWithArg(1, 20 | (28 << 8), PlaceDefaultItems);
    FrameTimer(1, PlayerClassLoop);
    FrameTimer(1, SetHostileCritter);
    FrameTimer(1, ShopClassInitPlace);
    FrameTimer(1, CreClassMarketPlace);
    FrameTimer(3, LoopSearchIndex);
    FrameTimer(1, LineSetInit);
    //FrameTimer(3, PotionImgTest); //here: removeMe
}

void NoKeepZoneSystem(int sUnit, int backLocation)
{
    if (IsPlayerUnit(sUnit))
    {
        MoveObject(sUnit, LocationX(backLocation), LocationY(backLocation));
        UniPrint(sUnit, "이봐... 당신은 이곳에 접근할 수 없어!");
    }
}

void HumanNoKeepNorth()
{
    NoKeepZoneSystem(other, 16);
}

void HumanNoKeepSouth()
{
    NoKeepZoneSystem(other, 21);
}

int CheckMonsterSection(float xProfile, float yProfile)
{
    int location = 16, i;

    for (i = 0 ; i < 5 ; i ++)
    {
        if (Distance(xProfile, yProfile, LocationX(location + i), LocationY(location + i)) < 990.0)
            return i + 1;
    }
    return 0;
}

void MonsterEntrySection()
{
    int curSection;

    if (IsMonsterUnit(other))
    {
        curSection = CheckMonsterSection(GetObjectX(other), GetObjectY(other));
        if (curSection)
        {
            if (curSection > GameClassMobAccessCheck())
            {
                ModifySetupTeleportDestination(XTelpo, 16 + curSection);
                GameClassMobAccessSet(curSection);
            }
        }
    }
}

void RespawnLocationReset()
{
    ModifySetupTeleportDestination(XTelpo, 16);
}

void GetUnitXYProfile(int sUnit, int dest)
{
    int ptr = UnitToPtr(sUnit);

    if (ptr)
    {
        SetMemory(dest, GetMemory(ptr + 0x38));
        SetMemory(dest + 4, GetMemory(ptr + 0x3c));
    }
}

int AnyargUnitOrLocation(int anyArg)
{
    float xProfile, yProfile;
    int link;

    if (!link)
    {
        link = &xProfile;
    }
    if (anyArg >> 0x18)
        GetUnitXYProfile(anyArg, link);
    else
    {
        xProfile = LocationX(anyArg & 0xfff);
        yProfile = LocationY(anyArg & 0xfff);
    }
    return link;
}

int WillOWispBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1819044183; arr[1] = 1936283471; arr[2] = 112; arr[17] = 500; arr[19] = 50; 
		arr[21] = 1065353216; arr[23] = 32769; arr[24] = 1069547520; arr[25] = 1; arr[26] = 3; 
		arr[28] = 1120403456; arr[29] = 10; arr[31] = 9; arr[32] = 15; arr[33] = 31; 
		arr[53] = 1128792064; arr[54] = 4; arr[59] = 5542784; arr[60] = 1326; arr[61] = 46913280; 
		link = &arr;
	}
	return link;
}

void WillOWispSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1069547520);
		SetMemory(ptr + 0x224, 1069547520);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32769);
		SetMemory(GetMemory(ptr + 0x22c), 500);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 500);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WillOWispBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int ImportMonsterStrikeFunc()
{
	int arr[14], link;

	if (!link) //arr[5] = 0x0830B68B;
	{
		arr[0] = 0x448B5650; arr[1] = 0xC0850C24; arr[2] = 0xB08B2574; arr[3] = 0x000002EC; arr[4] = 0x1B74F685; arr[5] = 0x04C8B68B; arr[6] = 0xFE830000;
		arr[7] = 0x68107C00; arr[8] = 0x00507310; arr[9] = 0x56006A50; arr[10] = 0x0C2454FF; arr[11] = 0x5E10C483; arr[12] = 0x93806858; arr[13] = 0x90C30054;
		link = &arr;
	}
	return link;
}

int GetVictimUnit()
{
	int ptr = GetMemory(0x834a40);

	if (ptr)
		return GetMemory(ptr + 0x2c);
	return 0;
}

int GetVictimUnit2(int cre)
{
    int ptr = UnitToPtr(cre), ecPtr;

    if (ptr)
    {
        ecPtr = GetMemory(ptr + 0x2ec);
        if (GetMemory(ecPtr + 0x46c))
            return GetMemory(GetMemory(ecPtr + 0x46c) + 0x2c);
    }
    return 0;
}

void RegistryUnitStrikeFunction(int sUnit, int sFunc)
{
    int ptr = UnitToPtr(sUnit), ecPtr, binScrPtr;

    if (ptr)
    {
        ecPtr = GetMemory(ptr + 0x2ec);
        binScrPtr = GetMemory(GetMemory(ptr + 0x2ec) + 0x1e4);
        if (binScrPtr)
        {
            SetMemory(binScrPtr + 0xec, ImportMonsterStrikeFunc());
            //SetMemory(ecPtr + 0x830, sFunc);
            SetMemory(ecPtr + 0x4c8, sFunc);
        }
    }
}

void FONCollide()
{
    int owner = GetOwner(self);

    while (owner)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 130, 14);
            Enchant(other, EnchantList(4), 1.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

int FONCreate(int me, float xProfile, float yProfile)
{
    int mis = CreateObjectAt("DeathBall", xProfile, yProfile);
    int ptr = GetMemory(0x750710);

    if (ptr)
    {
        SetMemory(ptr + 0x2e8, 5483536); //projectile update
        SetUnitCallbackOnCollide(mis, FONCollide);
        SetOwner(me, mis);
    }
    return mis;
}

void AbsoluteTargetStrike(int owner, int target, float threshold, int func)
{
    int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(target), GetObjectY(target));

    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner)), threshold);

    SetOwner(owner, unit);
    Raise(unit, target);
    FrameTimerWithArg(1, unit, func);
}

void OrbHecubahOneShot(int sUnit)
{
    int owner = GetOwner(sUnit), target = ToInt(GetObjectZ(sUnit)), mis;
    float dt = Distance(GetObjectX(sUnit), GetObjectY(sUnit), GetObjectX(target), GetObjectY(target));
    float thresHold;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        mis = FONCreate(owner, GetObjectX(owner) + UnitRatioX(target, owner, 19.0), GetObjectY(owner) + UnitRatioY(target, owner, 19.0));
        thresHold = DistanceUnitToUnit(mis, target) / GetObjectZ(sUnit + 1);
        MoveObject(sUnit, GetObjectX(target) + UnitRatioX(target, sUnit, dt * thresHold), GetObjectY(target) + UnitRatioY(target, sUnit, dt * thresHold));
        if (IsVisibleTo(sUnit, owner))
        {
            PushObject(mis, -41.0, GetObjectX(sUnit), GetObjectY(sUnit));
        }
        else
        {
            PushObject(mis, -41.0, GetObjectX(target), GetObjectY(target));
        }
        DeleteObjectTimer(mis, 21);
    }
    Delete(sUnit);
    Delete(sUnit + 1);
}

void RedWizStrikeTrigger()
{
    int victim = GetVictimUnit2(self);

    if (MaxHealth(victim))
    {
        MonsterForceCastSpell(self, 0, GetObjectX(self), GetObjectY(self));
        Damage(other, self, 80, 16);
        Effect("SENTRY_RAY", GetObjectX(self), GetObjectY(self), GetObjectX(victim), GetObjectY(victim));
    }
}

void OrbHecubahStrikeTrigger()
{
    int victim = GetVictimUnit2(self);

    if (MaxHealth(victim))
    {
        MonsterForceCastSpell(self, 0, GetObjectX(self), GetObjectY(self));
        AbsoluteTargetStrike(GetTrigger(), victim, 85.0, OrbHecubahOneShot);
    }
}

void WispStrikeTrigger()
{
    int victim = GetVictimUnit2(self);

    if (CurrentHealth(victim))
    {
        Damage(victim, self, 10, 9);
        MonsterForceCastSpell(self, 0, GetObjectX(self), GetObjectY(self));
        Effect("LIGHTNING", GetObjectX(victim), GetObjectY(victim), GetObjectX(self), GetObjectY(self));
    }
}

void ResetHostileCritter()
{
	SetMemory(0x833e64, 0x55b);		//CarnivorousPlant
	//SetMemory(0x833e70, 1329);		//FishBig
	//SetMemory(0x833e74, 1330);		//FishSmall
	//SetMemory(0x833e78, 1359);		//Rat
	SetMemory(0x833e7c, 1313);		//GreenFrog
    SetMemory(0x833e5c, 0);         //wisp
}

void SetHostileCritter()
{
	SetMemory(0x833e64, 0x540);		//CarnivorousPlant
	//SetMemory(0x833e70, 0x540);		//FishBig
	//SetMemory(0x833e74, 0x540);		//FishSmall
	//SetMemory(0x833e78, 0x540);		//Rat
	SetMemory(0x833e7c, 0x540);		//GreenFrog
    SetMemory(0x833e5c, 0x540);     //
}

int MonTypeFrog(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("GreenFrog", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    GreenFrogSubProcess(unit);
    return unit;
}

int BatBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 7627074; arr[17] = 60; arr[18] = 1; arr[19] = 95; arr[21] = 1056964608; 
		arr[23] = 32768; arr[24] = 1065353216; arr[25] = 1; arr[26] = 5; arr[27] = 5; 
		arr[28] = 1097859072; arr[29] = 4; arr[31] = 8; arr[59] = 5542784; arr[60] = 1357; 
		arr[61] = 46906368; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * BatBinTable + 0x1c));
	}
	return link;
}

void BatSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077306982);
		SetMemory(ptr + 0x224, 1077306982);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 60);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 60);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BatBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1056964608);
	}
}

int MonTypeBat(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Bat", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    BatSubProcess(unit);
    return unit;
}

int SmallAlbinoSpiderBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1818324307; arr[1] = 1651261804; arr[2] = 1399811689; arr[3] = 1701079408; arr[4] = 114; 
		arr[17] = 75; arr[18] = 1; arr[19] = 76; arr[21] = 1065353216; arr[23] = 34816; 
		arr[24] = 1065353216; arr[26] = 4; arr[27] = 5; arr[28] = 1097859072; arr[29] = 6; 
		arr[31] = 8; arr[59] = 5542784; arr[60] = 1356; arr[61] = 46906624; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * SmallAlbinoSpiderBinTable + 0x1c));
	}
	return link;
}

void SmallAlbinoSpiderSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1074916229);
		SetMemory(ptr + 0x224, 1074916229);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 34816);
		SetMemory(GetMemory(ptr + 0x22c), 75);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 75);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, SmallAlbinoSpiderBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeSmallSpider(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("SmallAlbinoSpider", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SmallAlbinoSpiderSubProcess(unit);
    return unit;
}

int MonTypeLeech(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("GiantLeech", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 80);
    return unit;
}

int MonTypeSwordman(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Swordsman", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 130);
    return unit;
}

int FlyingGolemBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1769565254; arr[1] = 1866950510; arr[2] = 7169388; arr[17] = 98; arr[18] = 10; 
		arr[19] = 90; arr[21] = 1065353216; arr[23] = 32769; arr[24] = 1065353216; arr[26] = 4; 
		arr[37] = 1701605191; arr[38] = 1920090477; arr[39] = 30575; arr[53] = 1125515264; arr[54] = 4; 
		arr[55] = 25; arr[56] = 25; arr[60] = 1316; arr[61] = 46903808; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * FlyingGolemBinTable + 0x1c));
	}
	return link;
}

void FlyingGolemSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1076677837);
		SetMemory(ptr + 0x224, 1076677837);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32769);
		SetMemory(GetMemory(ptr + 0x22c), 98);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 98);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FlyingGolemBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeMecaFlying(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("FlyingGolem", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    FlyingGolemSubProcess(unit);
    return unit;
}

int MonTypeUrchin(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Urchin", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 120);
    UnitZeroFleeRange(unit);
    return unit;
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
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * BlackWidowBinTable + 0x1c));
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
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 180);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 180);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BlackWidowBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeSpider(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("BlackWidow", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    BlackWidowSubProcess(unit);
    return unit;
}

int MonTypeArcher(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Archer", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 180);
    return unit;
}

int MonTypeScorpion(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Scorpion", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 225);
    return unit;
}

int MonTypeGargoyle(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("EvilCherub", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 180);
    return unit;
}

int MonTypeSkeleton(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Skeleton", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 260);
    return unit;
}

int MonTypeOgreAxe(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("GruntAxe", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 275);
    return unit;
}

int MonTypeOgre(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("OgreBrute", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 306);
    return unit;
}

int MonTypeBear(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Bear", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 425);
    return unit;
}

int MonTypeSkullLord(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("SkeletonLord", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 325);
    return unit;
}

int MonTypeShade(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Shade", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 275);
    return unit;
}

int MonTypeOgreLord(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("OgreWarlord", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 360);
    return unit;
}

int MonTypeFirebat(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    string fbat[] = {"EmberDemon", "MeleeDemon", "MeleeDemon", "MeleeDemon"};
    int unit = CreateObjectAt(fbat[Random(0, 3)], GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 225);
    return unit;
}

int MonTypeImp(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Imp", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 192);
    return unit;
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
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * FireSpriteBinTable + 0x1c));
	}
	return link;
}

void FireSpriteSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1074287083);
		SetMemory(ptr + 0x224, 1074287083);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 65536);
		SetMemory(GetMemory(ptr + 0x22c), 168);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 168);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FireSpriteBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeFireFairy(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("FireSprite", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    FireSpriteSubProcess(unit);
    return unit;
}

int NecromancerBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1919116622; arr[1] = 1851878767; arr[2] = 7497059; arr[17] = 370; arr[19] = 60; 
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1070302495; arr[25] = 1; arr[26] = 2; 
		arr[28] = 1103626240; arr[29] = 48; arr[30] = 1092616192; arr[31] = 11; arr[32] = 7; 
		arr[33] = 15; arr[34] = 1; arr[35] = 1; arr[36] = 10; arr[59] = 5542784; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * NecromancerBinTable + 0x1c));
	}
	return link;
}

void NecromancerSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1072064102);
		SetMemory(ptr + 0x224, 1072064102);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 370);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 370);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, NecromancerBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeNecromancer(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Necromancer", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    NecromancerSubProcess(unit);
    return unit;
}

int HecubahBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 6840674; arr[17] = 720; arr[19] = 100; arr[21] = 1065353216; 
		arr[23] = 32768; arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; 
		arr[28] = 1108082688; arr[29] = 130; arr[30] = 1092616192; arr[32] = 10; arr[33] = 18; 
		arr[59] = 5542784; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * HecubahBinTable + 0x1c));
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
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 720);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 720);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HecubahBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeHecubah(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Hecubah", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    HecubahSubProcess(unit);
    return unit;
}

int MonTypeMecaGolem(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("MechanicalGolem", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 900);
    return unit;
}

int MonTypeWisp(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("WillOWisp", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    WillOWispSubProcess(unit);
    RegistryUnitStrikeFunction(unit, WispStrikeTrigger);
    return unit;
}

int MonTypeGhost(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Ghost", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 160);
    return unit;
}

int GoonBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1852796743; arr[17] = 295; arr[19] = 97; arr[21] = 1065353216; arr[23] = 32768; 
		arr[24] = 1066192077; arr[26] = 4; arr[28] = 1106247680; arr[29] = 50; arr[30] = 1092616192; 
		arr[31] = 4; arr[32] = 18; arr[33] = 21; arr[34] = 3; arr[35] = 6; 
		arr[36] = 20; arr[57] = 5548176; arr[58] = 5546608; arr[59] = 5543680; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * GoonBinTable + 0x1c));
	}
	return link;
}

void GoonSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077558640);
		SetMemory(ptr + 0x224, 1077558640);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 295);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 295);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, GoonBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeGoon(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int unit = CreateObjectAt("Goon", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    GoonSubProcess(unit);
    SetUnitVoice(unit, 63);
    return unit;
}

int LichLordBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751345484; arr[1] = 1685221196; arr[17] = 425; arr[19] = 126; arr[21] = 1065353216; 
		arr[23] = 34816; arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; 
		arr[28] = 1113325568; arr[29] = 100; arr[30] = 1092616192; arr[32] = 15; arr[33] = 21; 
		arr[57] = 5548288; arr[59] = 5542784; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * LichLordBinTable + 0x1c));
	}
	return link;
}

void LichLordSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1081207685);
		SetMemory(ptr + 0x224, 1081207685);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 34816);
		SetMemory(GetMemory(ptr + 0x22c), 425);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 425);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, LichLordBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeLichlord(int anyarg)
{
    int xyTable = AnyargUnitOrLocation(anyarg);
    int unit = CreateObjectAt("LichLord", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    LichLordSubProcess(unit);
    return unit;
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
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * AirshipCaptainBinTable + 0x1c));
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
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 34817);
		SetMemory(GetMemory(ptr + 0x22c), 325);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 325);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, AirshipCaptainBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeJandor(int anyarg)
{
    int xyTable = AnyargUnitOrLocation(anyarg);
    int unit = CreateObjectAt("AirshipCaptain", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    AirshipCaptainSubProcess(unit);
    return unit;
}

int CarnivorousPlantBinTable2()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1852989763; arr[1] = 1919907433; arr[2] = 1349743983; arr[3] = 1953390956; arr[17] = 600; 
		arr[18] = 200; arr[19] = 100; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1065353216; 
		arr[25] = 1; arr[26] = 4; arr[27] = 5; arr[28] = 1109393408; arr[29] = 100; 
		arr[31] = 8; arr[34] = 3; arr[35] = 3; arr[36] = 20; arr[59] = 5544320; 
		arr[60] = 1371; arr[61] = 46901760; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * CarnivorousPlantBinTable2 + 0x1c));
	}
	return link;
}

void CarnivorousPlantSubProcess2(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077936128);
		SetMemory(ptr + 0x224, 1077936128);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 600);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 600);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, CarnivorousPlantBinTable2());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeCarnPlant(int anyarg)
{
    int xyTable = AnyargUnitOrLocation(anyarg);
    int unit = CreateObjectAt("CarnivorousPlant", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    CarnivorousPlantSubProcess2(unit);
    return unit;
}

int MonTypeDryad(int anyarg)
{
    int xyTable = AnyargUnitOrLocation(anyarg);
    int unit = CreateObjectAt("WizardGreen", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitMaxHealth(unit, 275);
    Enchant(unit, EnchantList(14), 0.0);

    return unit;
}

int StoneGolemBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1852798035; arr[1] = 1819232101; arr[2] = 28005; arr[17] = 750; arr[19] = 70; 
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1067869798; arr[26] = 4; arr[27] = 5; 
		arr[28] = 1117782016; arr[29] = 100; arr[31] = 2; arr[32] = 60; arr[33] = 90; 
		arr[58] = 5545472; arr[59] = 5543904; arr[60] = 1324; arr[61] = 46901248; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * StoneGolemBinTable + 0x1c));
	}
	return link;
}

void StoneGolemSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1074161254);
		SetMemory(ptr + 0x224, 1074161254);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 750);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 750);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, StoneGolemBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeStoneGiant(int anyarg)
{
    int xyTable = AnyargUnitOrLocation(anyarg);
    int golem = CreateObjectAt("StoneGolem", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    StoneGolemSubProcess(golem);
    return golem;
}

int HorrendousBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1920102216; arr[1] = 1868852837; arr[2] = 29557; arr[17] = 600; arr[19] = 100; 
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1065353216; arr[25] = 1; arr[26] = 9; 
		arr[27] = 5; arr[28] = 1109393408; arr[29] = 62; arr[54] = 4; arr[59] = 5542784; 
		arr[60] = 1386; arr[61] = 46907648; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * HorrendousBinTable + 0x1c));
	}
	return link;
}

void HorrendousSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1077936128);
		SetMemory(ptr + 0x224, 1077936128);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 600);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 600);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HorrendousBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeHorrendous(int anyarg)
{
    int xyTable = AnyargUnitOrLocation(anyarg);
    int horr = CreateObjectAt("Horrendous", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    HorrendousSubProcess(horr);
    return horr;
}

int StrongWizardWhiteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1869771859; arr[1] = 1767335790; arr[2] = 1685217658; arr[3] = 1953065047; arr[4] = 101; 
		arr[17] = 325; arr[19] = 65; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1069547520; 
		arr[37] = 1701996870; arr[38] = 1819042146; arr[53] = 1128792064; arr[54] = 4; arr[55] = 20; 
		arr[56] = 30; arr[58] = 5546320; 
		link = GetMemory(GetMemory(0x75ae28) + (0x30 * StrongWizardWhiteBinTable + 0x1c));
	}
	return link;
}

void StrongWizardWhiteSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1073322393);
		SetMemory(ptr + 0x224, 1073322393);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 325);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 325);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, StrongWizardWhiteBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int MonTypeHovath(int anyarg)
{
    int xyTable = AnyargUnitOrLocation(anyarg);
    int hovath = CreateObjectAt("StrongWizardWhite", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    StrongWizardWhiteSubProcess(hovath);
    return hovath;
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

void GodModePotionFeatureSwap(int potion)
{
    SetUnitCallbackOnUseItem(potion, UseGodModePotion);
    Enchant(potion, EnchantList(22), 0.0);
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

int ItemDropClassHotPotion(int sUnit)
{
    return CreateObjectAt("RedPotion", GetObjectX(sUnit), GetObjectY(sUnit));
}

int ItemDropClassMagicalPotions(int sUnit)
{
    string potionList[] = {
        "RedPotion", "BluePotion", "CurePoisonPotion", "VampirismPotion",
        "HastePotion", "InvisibilityPotion", "InfravisionPotion", "WhitePotion",
        "ShieldPotion", "FireProtectPotion", "ShockProtectPotion", "PoisonProtectPotion",
        "YellowPotion", "BlackPotion"
        };
    return CheckPotionThingID(CreateObjectAt(potionList[Random(0, 13)], GetObjectX(sUnit), GetObjectY(sUnit)));
}

int ItemDropClassWeapon(int sUnit)
{
    string weaponList[] = {
        "GreatSword", "Longsword", "Sword", "MorningStar",
        "OgreAxe", "StaffWooden", "BattleAxe", "FanChakram",
        "RoundChakram", "WarHammer", "OblivionHalberd", "OblivionWierdling",
        "OblivionHeart"
    };
    int weaponUnit = CreateObjectAt(weaponList[Random(0, 12)], GetObjectX(sUnit), GetObjectY(sUnit));
    int weaponPtr = GetMemory(0x750710), cWave = GameClassWaveCheck();

    CheckSpecialItem(weaponPtr);
    if (cWave >= 4)
        SetWeaponProperties(weaponPtr, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    return weaponUnit;
}

int ItemDropClassArmor(int sUnit)
{
    string armorList[] = {
        "OrnateHelm", "Breastplate", "PlateArms", "PlateBoots",
        "PlateLeggings", "MedievalCloak", "ChainCoif", "ChainLeggings",
        "ChainTunic", "SteelHelm", "LeatherArmbands", "LeatherArmor",
        "LeatherArmoredBoots", "LeatherBoots", "LeatherHelm", "LeatherLeggings",
        "MedievalPants", "MedievalShirt"
    };
    int armorUnit = CreateObjectAt(armorList[Random(0, 17)], GetObjectX(sUnit), GetObjectY(sUnit));
    int armorPtr = GetMemory(0x750710), cWave = GameClassWaveCheck();

    if (cWave >= 4)
        SetArmorProperties(armorPtr, Random(0, 5), Random(0, 5), Random(0, 20), Random(0, 20));
    return armorUnit;
}

int ItemDropClassMoney(int sUnit)
{
    int ggg = CreateObjectAt("Gold", GetObjectX(sUnit), GetObjectY(sUnit));
    int ptr = GetMemory(0x750710), incRate = GameClassWaveCheck() * 50;

    SetMemory(GetMemory(ptr + 0x2b4), Random(100 + incRate, 400 + (incRate * 4)));
    if (!(GetMemory(ptr + 0x10) & 0x40))
        SetMemory(ptr + 0x10, GetMemory(ptr + 0x10) ^ 0x40);
    return ggg;
}

int ItemDropClassGerm(int sUnit)
{
    string gmList[] = {"Ruby", "Ruby", "Emerald", "Diamond"};
    int germ = CreateObjectAt(gmList[Random(0, GameClassWaveCheck() / 3)], GetObjectX(sUnit), GetObjectY(sUnit));
    int ptr = GetMemory(0x750710);

    if (!(GetMemory(ptr + 0x10) & 0x40))
        SetMemory(ptr + 0x10, GetMemory(ptr + 0x10) ^ 0x40);
    return germ;
}

int RewardItemFunctionTable(int pickIndex)
{
    int functions[6] = {
        ItemDropClassMoney, ItemDropClassGerm, ItemDropClassArmor,
        ItemDropClassHotPotion, ItemDropClassMagicalPotions, ItemDropClassWeapon
    };
    return functions[pickIndex];
}

int RewardClassItemCreateAtUnitPos(int sUnit)
{
    int pickFunction = RewardItemFunctionTable(Random(0, 5));

    if (!pickFunction)
        pickFunction = ItemDropClassHotPotion;

    return CallFunctionWithArgInt(pickFunction, sUnit);
}

void AllWaveClear(int wave)
{
    if (!GameClassMainIsShutdown())
        GameClassMainSetShutdown();
    AllPlayerTeleportTo(74);
    DrawImageAtLocation(74, 2517);
    UniPrintToAll("** *축하합니다! " + IntToString(wave) + "개의 웨이브를 클리어하셨습니다* **");
}

int LineMobFunctionInfo(int index)
{
    int fTb[34];

    if (!fTb[0])
    {
        fTb[0] = MonTypeArcher; fTb[1] = MonTypeBat; fTb[2] = MonTypeBear; fTb[3] = MonTypeFireFairy;
        fTb[4] = MonTypeFirebat; fTb[5] = MonTypeFrog; fTb[6] = MonTypeWisp; fTb[7] = MonTypeUrchin;
        fTb[8] = MonTypeSwordman; fTb[9] = MonTypeSpider; fTb[10] = MonTypeSmallSpider; fTb[11] = MonTypeSkullLord;
        fTb[12] = MonTypeShade; fTb[13] = MonTypeScorpion; fTb[14] = MonTypeOgreLord; fTb[15] = MonTypeOgreAxe;
        fTb[16] = MonTypeOgre; fTb[17] = MonTypeNecromancer; fTb[18] = MonTypeGargoyle; fTb[19] = MonTypeGhost;
        fTb[20] = MonTypeHecubah; fTb[21] = MonTypeImp; fTb[22] = MonTypeLeech; fTb[23] = MonTypeMecaFlying;
        fTb[24] = MonTypeMecaGolem; fTb[25] = MonTypeSkeleton; fTb[26] = MonTypeGoon; fTb[27] = MonTypeLichlord;
        fTb[28] = MonTypeCarnPlant; fTb[29] = MonTypeDryad; fTb[30] = MonTypeStoneGiant; fTb[31] = MonTypeHorrendous;
        fTb[32] = MonTypeHovath; fTb[33] = MonTypeJandor;
    }
    return fTb[index % 34];
}

void LineMobSetOrder(int arrPtr, int set1, int set2, int set3, int set4)
{
    SetMemory(arrPtr, set1 | (set2 << 8) | (set3 << 16) | (set4 << 24));
}

int LineMobOrder(int wave)
{
    int arr[14], link;

    if (wave < 0)
    {
        link = GetScrDataField(LineMobOrder) + 4;
        LineMobSetOrder(link, 5, 5, 1, 1);
        LineMobSetOrder(link + 4, 22, 22, 10, 10);
        LineMobSetOrder(link + 8, 8, 23, 8, 23);
        LineMobSetOrder(link + 12, 7, 9, 9, 7);     //spider, urchin
        LineMobSetOrder(link + 16, 13, 13, 26, 0);
        LineMobSetOrder(link + 20, 25, 9, 18, 19);
        LineMobSetOrder(link + 24, 14, 16, 2, 26);
        LineMobSetOrder(link + 28, 12, 27, 14, 11);
        LineMobSetOrder(link + 32, 17, 21, 3, 4);
        LineMobSetOrder(link + 36, 6, 19, 20, 24);
        LineMobSetOrder(link + 40, 28, 29, 28, 30);
        LineMobSetOrder(link + 44, 31, 32, 31, 33);
        LineMobSetOrder(link + 48, 24, 6, 27, 33);
        return 0;
    }
    return arr[wave];
}

int LineMobSpawnMarker(int index)
{
    int mark;

    if (!mark)
    {
        mark = CreateObject("ImaginaryCaster", 23);
        LookWithAngle(CreateObject("ImaginaryCaster", 24), 24);
        LookWithAngle(CreateObject("ImaginaryCaster", 25), 25);
        LookWithAngle(CreateObject("ImaginaryCaster", 26), 26);
        LookWithAngle(CreateObject("ImaginaryCaster", 27), 27);
        LookWithAngle(mark, 23);
    }
    return mark + index;
}

void LineSetInit()
{
    LineMobFunctionInfo(0);
    LineMobOrder(-1);
    LineMobSpawnMarker(0);
}

void CompleteCurrentWave()
{
    int wave = GameClassWaveCheck();

    UniPrintToAll("현재 " + IntToString(wave) + " 웨이브를 완료하셨습니다. 시작 버튼이 활성화 되었습니다");
}

void RegistItemPickEvent(int sItem, int eventFunction)
{
    int ptr = UnitToPtr(sItem);

    if (ptr)
        SetMemory(ptr + 0x300, eventFunction);
}

void RewardClassPick()
{
    if (IsObjectOn(GetTrigger() + 1))
    {
        LookWithAngle(GetTrigger() + 1, 1);
    }
}

void RewardClassDrop(int sUnit)
{
    int rwd = RewardClassItemCreateAtUnitPos(sUnit);

    ListClassAddNode(XItemNode, rwd);
    RegistItemPickEvent(rwd, RewardClassPick);
}

void LineMobDeathHandler()
{
    if (LineStatClassMobCountCheck())
    {
        LineStatClassMobCountSet(LineStatClassMobCountCheck() - 1);
        if (!LineStatClassMobCountCheck())
            CompleteCurrentWave();
    }
    LineStatClassKillCountSet(LineStatClassKillCountCheck() + 1);
    if (Random(0, 2))
        RewardClassDrop(self);
    DeleteObjectTimer(self, 90);
}

void LineMobHurtHandler()
{
    if (!GetCaller())
    {
        if (IsPoisonedUnit(self))
        {
            Damage(self, 0, IsPoisonedUnit(self) + 1, 5);
            DeleteObjectTimer(CreateObjectAt("GreenPuff", GetObjectX(self), GetObjectY(self)), 15);
        }
    }
}

void LineMobSightHandler()
{
    return;
}

string LineMobGreetMessage()
{
    string greet[] = {
        "니하오?",
        "이곳 한국도 이제 중국의 일부라는 말이!",
        "중국이 대국임을 명심하고 섬겨라!",
        "그 누구든 공산주의 세력에 반대하면 용서하지 않는다!",
        "다시는 중국을 무시하지 말라!",
        "니 지금 방금 무어라 했니?"};

    return greet[Random(0, 5)];
}

void LineMobGreetOnSight()
{
    SetCallback(self, 3, LineMobSightHandler);
    UniChatMessage(self, LineMobGreetMessage(), 180);
}

void LineMobGoToTarget(int mob)
{
    CreatureFollow(mob, MasterMonster());
    AggressionLevel(mob, 1.0);
}

int MobSummonSingle(int mobInfo, int anyUnit)
{
    int mob = CallFunctionWithArgInt(LineMobFunctionInfo((mobInfo >> (8 * Random(0, 3))) & 0xff), anyUnit);

    RetreatLevel(mob, 0.0);
    ResumeLevel(mob, 1.0);
    SetCallback(mob, 5, LineMobDeathHandler);
    SetCallback(mob, 7, LineMobHurtHandler);
    if (!Random(0, 7))
        SetCallback(mob, 3, LineMobGreetOnSight);
    FrameTimerWithArg(1, mob, LineMobGoToTarget);
    return mob;
}

void MobSummonMultiple(int mobInfo)
{
    int mark = LineMobSpawnMarker(0), i;

    for (i = 0 ; i < 5 ; i ++)
    {
        MobSummonSingle(mobInfo, mark + i);
        MoveObjectVector(mark + i, -60.0, 60.0);
    }
}

void MarkResetPosition(int mark)
{
    int i;

    for (i = 0 ; i < 5 ; i ++)
        MoveObject(mark + i, LocationX(GetDirection(mark + i)), LocationY(GetDirection(mark + i)));
}

void MobWaveClassSummon(int arg)
{
    int wave = (arg >> 4) & 0xff, count = arg & 0xf;

    if (count)
    {
        MobSummonMultiple(LineMobOrder(wave));
        count --;
        FrameTimerWithArg(1, count | (wave << 4), MobWaveClassSummon);
    }
    else
        MarkResetPosition(LineMobSpawnMarker(0));
}

void MobWaveSummonControl(int arg)
{
    int wave = (arg >> 0x4) & 0xff, count = arg & 0xf;

    if (count && !GameClassMainIsShutdown())
    {
        MobWaveClassSummon((wave << 4) | 6);
        count --;
        SecondTimerWithArg(20, count | (wave << 4), MobWaveSummonControl);
    }
    else if (GameClassLineIsStart())
        GameClassLineStartSet();
}

void MobWaveClassStart()
{
    int wave = GameClassWaveCheck();

    if (LineStatClassMobCountCheck())
    {
        UniPrintToAll("라인에 아직 남아있는 적군이 있습니다. 모두 제거 후 다시 시도하세요");
        return;
    }
    GameClassLineStartSet();
    if (wave < 12)
    {
        LineStatClassMobCountSet(300);
        RespawnLocationReset();
        GameClassWaveSet(wave + 1);
        ListClassClearCondition(ListClassGetHeadNext());
        FrameTimerWithArg(15, 10 | (wave << 4), MobWaveSummonControl);
        UniPrintToAll("** *현재 웨이브 " + IntToString(wave + 1) + "을 지금 시작합니다* **");
    }
    else
    {
        AllWaveClear(wave);
    }
}

void GGStart()
{
    MobWaveClassStart();
}

void KillLineMonster(int mob)
{
    Damage(mob, 0, MaxHealth(mob) + 1, -1);
}

void AllPlayerTeleportTo(int location)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (CurrentHealth(player[i]))
            MoveObject(player[i], LocationX(location), LocationY(location));
    }
}

void GGOver()
{
    if (!GameClassMainIsShutdown())
    {
        GameClassMainSetShutdown();
        AllPlayerTeleportTo(29);
        UniPrintToAll("나라가 점령당했습니다! 패배. ..");
    }
}

void MonsKeepOver()
{
    if (IsMonsterUnit(other))
    {
        if (GameClassLifeCheck())
        {
            KillLineMonster(other);
            GameClassLifeSet(GameClassLifeCheck() - 1);
            UniPrintToAll("중국인 1명이 국경을 넘었습니다!! 남은 기회: " + IntToString(GameClassLifeCheck()));
        }
        else
            GGOver();
    }
}

void ComeInHome()
{
    if (CurrentHealth(other))
    {
        if (IsPlayerUnit(other))
            MoveObject(other, LocationX(15), LocationY(15));
    }
}

string DefaultItem(int num)
{
    string item[] = {
        "SteelHelm", "Breastplate", "PlateArms", "PlateBoots", "PlateLeggings", "GreatSword", "RedPotion", "RedPotion", "RedPotion", "CurePoisonPotion",
        "CurePoisonPotion"};

    return item[num];
}

void PlaceItemOnLine(int arg)
{
    int index = arg & 0xf, count = (arg >> 4) & 0xf, location = (arg >> 8) & 0xffff, i;
    int sub = CreateObjectAt("ImaginaryCaster", LocationX(location), LocationY(location));
    string item = DefaultItem(index);

    for (i = 0 ; i < count ; i ++)
        CreateObjectAt(item, GetObjectX(sub + i) + 23.0, GetObjectY(sub + i) + 23.0);
    Delete(sub);
}

void PlaceDefaultItems(int arg)
{
    int count = arg & 0xff, location = (arg >> 8) & 0xffff;

    if (count)
    {
        PlaceItemOnLine((11 << 4) | (count % 11) | (location << 8));
        TeleportLocationVector(location, -23.0, 23.0);
        count --;
        FrameTimerWithArg(1, count | (location << 8), PlaceDefaultItems);
    }
}

int ListClassGetNext(int node)
{
    return ToInt(GetObjectZ(node));
}

void ListClassSetNext(int node, int nextNode)
{
    Raise(node, nextNode);
}

int ListClassAddNode(int prevNode, int parent)
{
    int nNode = CreateObjectAt("ImaginaryCaster", GetObjectX(parent), GetObjectY(parent));

    ListClassSetNext(nNode, ListClassGetNext(prevNode));
    ListClassSetNext(prevNode, nNode);
    return nNode;
}

void ListClassClearCondition(int node)
{
    int next;

    if (IsObjectOn(node))
    {
        next = ListClassGetNext(node);
        if (!GetDirection(node) && IsObjectOn(node - 1))
            Delete(node - 1);
        Delete(node);
        ListClassClearCondition(next);
    }
}

int ListClassGetHeadNext()
{
    return ListClassGetNext(XItemNode);
}

void HitFireballHammer()
{
    CastSpellObjectLocation("SPELL_FIREBALL", OTHER, GetObjectX(OTHER) + UnitAngleCos(OTHER, 17.0), UnitAngleSin(OTHER, 17.0));
}

int PowerItemMagicMisHammer(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int weapon = CreateObjectAt("WarHammer", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    RegistWeaponCPropertyExecScript(weapon, 2, HitFireballHammer);
    return weapon;
}

int PowerItemFireSword(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int weapon = CreateObjectAt("GreatSword", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    RegistWeaponCPropertyExecScript(weapon, 2, MagicMissileSword);
    return weapon;
}

void ShopClassItemDesc()
{
    int remArg = ToInt(GetObjectZ(GetTrigger() + 1)), pay = GetDirection(GetTrigger() + 1) * 1000;
    int dtStr = ToInt(GetObjectZ(GetTrigger() + 2));
    string itemName = ToStr((remArg >> 10) & 0x3ff);

    TellStoryUnitName("oAo", ToStr((dtStr >> 10)& 0x3ff), itemName + "구입");
    UniPrint(other, itemName + "을 구입하시겠어요? 이 작업은" + IntToString(pay) + "골드를 요구합니다");
    UniPrint(other, "거래를 계속 하시려면 '예' 버튼을 누르세요");
}

void ShopClassKeeperRegistDialog(int kp)
{
    SetDialog(kp, "YESNO", ShopClassItemDesc, ShopClassItemTrade);
}

void ShopClassTradeResult()
{
    ShopClassKeeperRegistDialog(self);
}

void ShopClassTradeResultD()
{
    return;
}

void ShopClassItemTrade()
{
    int remArg = ToInt(GetObjectZ(GetTrigger() + 1)), pay = GetDirection(GetTrigger() + 1) * 1000;
    string tradeStr = ToStr((ToInt(GetObjectZ(GetTrigger() + 2)) >> 20) & 0x3ff);
    string itemName = ToStr((remArg >> 10) & 0x3ff);

    if (GetAnswer(self) ^ 1) return;
    if (GetGold(other) >= pay)
    {
        ChangeGold(other, -pay);
        CallFunctionWithArgInt((remArg >> 20) & 0x3ff, other);
        SetDialog(self, "NORMAL", ShopClassTradeResultD, ShopClassTradeResultD);
        TellStoryUnitName("oAo", tradeStr, itemName + "구입완료");
        ShopClassKeeperRegistDialog(self);
        UniPrint(other, "거래가 완료되었습니다! 구입하신 " + itemName + "은 당신 발 아래에 있어요");
    }
    else
        UniPrint(other, "거래가 취소되었습니다. 사유: 잔액 부족");
}

int DeadUnitCreateAtUnitPos(int sUnit, string unitName)
{
    int dead = CreateObjectAt(unitName, GetObjectX(sUnit), GetObjectY(sUnit));

    ObjectOff(dead);
    Damage(dead, 0, CurrentHealth(dead) + 1, -1);
    return dead;
}

int DeadUnitCreateAt(float xProfile, float yProfile, string unitName)
{
    int dead = CreateObjectAt(unitName, xProfile, yProfile);

    ObjectOff(dead);
    Damage(dead, 0, CurrentHealth(dead) + 1, -1);
    return dead;
}

int DeadUnitCreate(int location, string unitName)
{
    int dead = CreateObjectAt(unitName, LocationX(location), LocationY(location));

    ObjectOff(dead);
    Damage(dead, 0, CurrentHealth(dead) + 1, -1);
    return dead;
}

int ShopClassPlaceShopkeeper(int loLochiPay, int remArg, int dtStr)
{
    int location = loLochiPay & 0x3ff;
    int kp = DeadUnitCreate(location, ToStr(remArg & 0x3ff));

    Frozen(CreateObjectAt("ImaginaryCaster", LocationX(location), LocationY(location)) - 1, 1);
    Raise(CreateObjectAt("ImaginaryCaster", LocationX(location), LocationY(location)) - 1, remArg);
    Raise(kp + 2, dtStr);
    LookWithAngle(kp + 1, (loLochiPay >> 10) & 0xff);
    ShopClassKeeperRegistDialog(kp);
    StoryPic(kp, ToStr(dtStr & 0x3ff));
    return kp;
}

void ShopClassInitPlace()
{
    ShopClassPlaceShopkeeper(83 | (99 << 10), SToInt("Mimic") | (SToInt("부처님 완드") << 10) | (PlaceBucherStaff << 20),
        SToInt("HorrendousPic") | (SToInt("NoxDemo:BuyNox") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(35 | (43 << 10), SToInt("Horrendous") | (SToInt("오우거로드엑스") << 10) | (PlaceAxeForOgre << 20),
        SToInt("HorrendousPic") | (SToInt("NoxDemo:BuyNox") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(31 | (40 << 10), SToInt("Wizard") | (SToInt("화이어볼 해머\n") << 10) | (PowerItemMagicMisHammer << 20),
        SToInt("AirshipCaptainPic") | (SToInt("NoxDemo:BuyNox") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(32 | (38 << 10), SToInt("EmberDemon") | (SToInt("마법미사일 서드\n") << 10) | (PowerItemFireSword << 20),
        SToInt("DemonPic") | (SToInt("NoxDemo:BuyNox") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(33 | (42 << 10), SToInt("Archer") | (SToInt("화살서드\n") << 10) | (PlaceArrowSword << 20),
        SToInt("GerardPic") | (SToInt("NoxDemo:BuyNox") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(34 | (47 << 10), SToInt("WizardWhite") | (SToInt("드레인서드\n") << 10) | (PlaceDrainSword << 20),
        SToInt("HorvathPic") | (SToInt("NoxDemo:BuyNox") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(36 | (48 << 10), SToInt("WizardGreen") | (SToInt("포오네서드\n") << 10) | (PlaceFonSword << 20),
        SToInt("DryadPic") | (SToInt("War08z:EstimatedGoodies") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(37 | (44 << 10), SToInt("Spider") | (SToInt("유도탄 메이스\n") << 10) | (PlaceAutoMissileMace << 20),
        SToInt("CreatureCageSpider") | (SToInt("thing.db:SPELL_MAGIC_MISSILE_DESC") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(54 | (53 << 10), SToInt("Beholder") | (SToInt("자동타겟\n데스레이서드\n") << 10) | (PlaceAutoTargetSword << 20),
        SToInt("MaleStaffDeathRay") | (SToInt("thing.db:DeathRay") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(62 | (12 << 10), SToInt("Lich") | (SToInt("아마겟돈 목걸이\n1회용\n") << 10) | (PlaceMeteorAmulet << 20),
        SToInt("MaleStaffDeathRay") | (SToInt("thing.db:SPELL_METEOR_DESC") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
    ShopClassPlaceShopkeeper(65 | (11 << 10), SToInt("Demon") | (SToInt("용의숨결지팡이\n") << 10) | (PlaceDemonWand << 20),
        SToInt("MaleStaffDeathRay") | (SToInt("thing.db:LesserFireballWandDescription") << 10) | (SToInt("GUITrade.c:TradeVendorAccept") << 20));
}

int GreenFrogBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701147207; arr[1] = 1869760110; arr[2] = 103; arr[17] = 35; arr[19] = 64; 
		arr[21] = 1065353216; arr[23] = 32769; arr[24] = 1067869798; arr[25] = 1; arr[26] = 1; 
		arr[28] = 1103626240; arr[29] = 8; arr[31] = 10; arr[32] = 14; arr[33] = 24; 
		arr[59] = 5542784; arr[60] = 1313; arr[61] = 46905856; 
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
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32769);
		SetMemory(GetMemory(ptr + 0x22c), 35);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 35);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, GreenFrogBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void WeaponExecScript1()
{
    int arw = CreateObjectAt("ArcherArrow", GetObjectX(other) + UnitAngleCos(other, 8.0), GetObjectY(other) + UnitAngleSin(other, 8.0));

    LookWithAngle(arw, GetDirection(other));
    SetOwner(other, arw);
    PushObject(arw, 34.0, GetObjectX(other), GetObjectY(other));
}

void DrainTouched()
{
    int owner = GetOwner(self);

    while (owner)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 100, 7);
            Effect("RICOCHET", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

void NatureShotCollide()
{
    int owner = GetOwner(self);

    while (owner)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 210, 14);
            Effect("RICOCHET", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

void OgreJinCollide()
{
    int owner = GetOwner(self);

    while (owner)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 130, 14);
            Enchant(other, EnchantList(28), 0.3);
            Effect("RICOCHET", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
            break;
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

void BreakingBarrelFx(int owner, float xProfile, float yProfile)
{
    int subD = DeadUnitCreateAt(xProfile, yProfile, "Necromancer");

    Frozen(subD, 1);
    SetCallback(CreateObjectAt("PiledBarrels3Breaking", xProfile, yProfile) - 1, 9, OgreJinCollide);
    DeleteObjectTimer(subD, 1);
    SetOwner(owner, subD);
}

void AxeJinHandler(int sub)
{
    float xVect = GetObjectZ(sub), yVect = GetObjectZ(sub + 1);
    int amount = GetDirection(sub), owner = GetOwner(sub), i;

    for (i = amount ; i ; i --)
    {
        if (IsVisibleTo(sub, sub + 1))
        {
            MoveObjectVector(sub, xVect, yVect);
            BreakingBarrelFx(owner, GetObjectX(sub), GetObjectY(sub));
        }
        else
        {
            break;
        }
    }
    Delete(sub);
    Delete(sub + 1);
}

void BreakingAxeJin()
{
    float xVect = UnitAngleCos(other, 21.0), yVect = UnitAngleSin(other, 21.0);
    int sub = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(other) + xVect, GetObjectY(other) + yVect);

    FrameTimerWithArg(1, sub, AxeJinHandler);
    Raise(sub, xVect);
    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(sub), GetObjectY(sub)), yVect);
    LookWithAngle(sub, 12);
    SetOwner(other, sub);
}

void DrainSubUnitCreate(int sub, int owner)
{
    int dm = DeadUnitCreateAtUnitPos(sub, "Troll");

    Frozen(dm, 1);
    DeleteObjectTimer(dm, 1);
    SetCallback(dm, 9, DrainTouched);
    SetOwner(owner, dm);
}

void DrainSwordHandler(int sub)
{
    int count = GetDirection(sub), owner = GetOwner(sub);

    while (IsObjectOn(sub))
    {
        if (count && CurrentHealth(owner))
        {
            if (IsVisibleTo(sub, sub + 1))
            {
                FrameTimerWithArg(1, sub, DrainSwordHandler);
                LookWithAngle(sub, count - 1);
                DrainSubUnitCreate(sub, owner);
                MoveObjectVector(sub, GetObjectZ(sub), GetObjectZ(sub + 1));
                Effect("CHARM", GetObjectX(sub), GetObjectY(sub), GetObjectX(sub + 1), GetObjectY(sub + 1));
                Effect("CHARM", GetObjectX(sub + 1), GetObjectY(sub + 1), GetObjectX(sub), GetObjectY(sub));
                break;
            }
        }
        Delete(sub);
        Delete(sub + 1);
        break;
    }
}

void DrainSwordFx()
{
    float xVect = UnitAngleCos(other, 19.0),yVect = UnitAngleSin(other, 19.0);
    int sub = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(other) + xVect, GetObjectY(other) + yVect);

    FrameTimerWithArg(1, sub, DrainSwordHandler);
    LookWithAngle(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(sub), GetObjectY(sub)) - 1, 18);
    SetOwner(other, sub);
    Raise(sub, xVect);
    Raise(sub + 1, yVect);
}

void MagicMissileSword()
{
    float xVect = UnitAngleCos(other, 23.0), yVect = UnitAngleSin(other, 23.0);
    int sub = CreateObjectAt("ImaginaryCaster", GetObjectX(other) + xVect, GetObjectY(other) + yVect);

    SetOwner(other, sub);
    CastSpellObjectLocation("SPELL_MAGIC_MISSILE", sub, GetObjectX(sub) + xVect, GetObjectY(sub) + yVect);
    DeleteObjectTimer(sub, 90);
}

void ForceOfNatureShot()
{
    float xVect = UnitAngleCos(other, 16.0), yVect = UnitAngleSin(other, 16.0);
    int mis = CreateObjectAt("DeathBall", GetObjectX(other) + xVect, GetObjectY(other) + yVect);
    int ptr = GetMemory(0x750710);

    SetOwner(other, mis);
    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(mis, NatureShotCollide);
    PushObject(mis, 30.0, GetObjectX(other), GetObjectY(other));
}

void AutoDetectMissileCollide()
{
    int owner = GetOwner(self);

    while (owner)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 230, 7);
            Effect("LESSER_EXPLOSION", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(self);
        break;
    }
}

void ShotAutoDetectMissile()
{
    // int mis = MakeMagicMissile("Pixie", other);
    int mis = SummonCustomMagicMissile("Pixie", OTHER, 0.0);

    PlaySoundAround(other, 348);
    SetUnitCallbackOnCollide(mis, AutoDetectMissileCollide);
}

int WizardRedBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1635412311; arr[1] = 1699898482; arr[2] = 100; arr[17] = 300; arr[18] = 100; 
		arr[19] = 50; arr[21] = 1065353216; arr[23] = 34816; arr[24] = 1067869798; arr[26] = 4; 
		arr[28] = 1128792064; arr[29] = 20; arr[31] = 16; arr[32] = 8; arr[33] = 15; 
		arr[53] = 1128792064; arr[54] = 4; arr[59] = 5542784; 
		link = &arr;
	}
	return link;
}

void WizardRedSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1069547520);
		SetMemory(ptr + 0x224, 1069547520);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 34816);
		SetMemory(GetMemory(ptr + 0x22c), 700);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 700);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WizardRedBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
        RegistryUnitStrikeFunction(sUnit, RedWizStrikeTrigger);
	}
}

int HecubahWithOrbBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1969448264; arr[1] = 1466458466; arr[2] = 1332245609; arr[3] = 25202; arr[17] = 700; 
		arr[19] = 85; arr[21] = 1065353216; arr[23] = 65536; arr[25] = 1; arr[26] = 2; 
		arr[28] = 1128792064; arr[29] = 20; arr[31] = 16; arr[32] = 5; 
		arr[33] = 11; arr[59] = 5542784; arr[60] = 1384; arr[61] = 46914560; 
		link = &arr;
	}
	return link;
}

void HecubahWithOrbSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1076048691);
		SetMemory(ptr + 0x224, 1076048691);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 65536);
		SetMemory(GetMemory(ptr + 0x22c), 700);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 700);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, HecubahWithOrbBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
        RegistryUnitStrikeFunction(sUnit, OrbHecubahStrikeTrigger);
	}
}

int MechanicalGolemBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751344461; arr[1] = 1667853921; arr[2] = 1866951777; arr[3] = 7169388; arr[17] = 800; 
		arr[19] = 55; arr[21] = 1065353216; arr[23] = 1; arr[24] = 1067869798; arr[26] = 4; 
		arr[27] = 5; arr[28] = 1112014848; arr[29] = 100; arr[30] = 1120403456; arr[31] = 2; 
		arr[32] = 24; arr[33] = 48; arr[58] = 5545616; arr[59] = 5544288; arr[60] = 1318; 
		arr[61] = 46900992; 
		link = &arr;
	}
	return link;
}

void MechanicalGolemSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1070805811);
		SetMemory(ptr + 0x224, 1070805811);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 1);
		SetMemory(GetMemory(ptr + 0x22c), 800);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 800);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, MechanicalGolemBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int CarnivorousPlantBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1852989763; arr[1] = 1919907433; arr[2] = 1349743983; arr[3] = 1953390956; arr[17] = 800; 
		arr[19] = 84; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1065353216; arr[25] = 1; 
		arr[26] = 2; arr[27] = 5; arr[28] = 1109393408; arr[29] = 150; arr[31] = 8; 
		arr[59] = 5542784; arr[60] = 1371; arr[61] = 46901760; 
		link = &arr;
	}
	return link;
}

void CarnivorousPlantSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1075922862);
		SetMemory(ptr + 0x224, 1075922862);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 800);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 800);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, CarnivorousPlantBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

void CreClassRevive(int sub)
{
    int time = GetDirection(sub), nameProcF;
    int cre;

    while (1)
    {
        if (GetDirection(sub))
        {
            FrameTimerWithArg(2, sub, CreClassRevive);
            LookWithAngle(sub, time - 1);
            break;
        }
        else
        {
            nameProcF = ToInt(GetObjectZ(sub));
            cre = CreClassSummon(nameProcF, sub);
            CreClassCommonProperty(cre, GetOwner(sub));
            UniChatMessage(cre, "크리쳐 부활했당!\n한꺼번에 다 덤벼봐랑!", 120);
            Delete(sub);
        }
        Delete(sub);
        break;
    }
}

void CreClassDeath()
{
    int sub = GetTrigger() + 1;

    MoveObject(sub, GetObjectX(self), GetObjectY(self));
    DeleteObjectTimer(self, 120);
    LookWithAngle(sub, 250); //TODO: Revive Timer
    FrameTimerWithArg(3, sub, CreClassRevive);
    UniChatMessage(self, "크리쳐가 적에게 피습당함...\n잠시 후 부활합니다", 150);
}

int CreClassSummon(int nameProcF, int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg), procF = (nameProcF >> 10) & 0x3ff;
    int cre, subF = (nameProcF >> 20) & 0x3ff;

    cre = CreateObjectAt(ToStr(nameProcF & 0x3ff), GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));
    if (cre)
    {
        if (procF)
            CallFunctionWithArg(procF, CreateObjectAt("ImaginaryCaster", GetObjectX(cre), GetObjectY(cre)) - 1);
        if (subF)
            CallFunctionWithArg(subF, cre);
        SetCallback(cre, 5, CreClassDeath);
        Enchant(cre, EnchantList(30), 0.0);
        Raise(cre + 1, nameProcF);
    }
    return cre;
}

int CreClassSummonBuffer(int nameProcF, int anyArg)
{
    if (CreClassGetCurrentCount() < 30)
    {
        CreClassIncreaseCurrentCount();
        return CreClassSummon(nameProcF, anyArg);
    }
    return 0;
}

void CreClassGuardMode(int cre, int caster)
{
    MonsterSetActionGuard(cre);
    UniChatMessage(cre, "홀드모드로 설정되었습니다", 150);
}

void CreClassEscort(int cre, int caster)
{
    CreatureFollow(cre, caster);
    UniChatMessage(cre, "에스코트 모드로 전환되었습니다", 150);
}

void CreClassOnClick()
{
    int act = MonsterGetCurrentAction(SELF), owner = GetOwner(self), cFps = GetMemory(0x84ea04);

    if (MathAbs(cFps - GetUnit1C(GetTrigger() + 1)) < 12)
    {
        if (owner ^ GetCaller())
            SetOwner(other, self);
        LookAtObject(self, other);
        if (act ^ 4) //isNotGuard
            CreClassGuardMode(self, other);
        else
            CreClassEscort(self, other);
        PlaySoundAround(self, 312);
        Effect("BLUE_SPARKS", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    }
    SetUnit1C(GetTrigger() + 1, cFps);
}

void CreClassCommonProperty(int cre, int owner)
{
    SetDialog(cre, "NORMAL", CreClassOnClick, CreClassOnClick);
    if (!MaxHealth(owner))
        owner = GetHost();
    SetOwner(owner, cre);
    SetOwner(owner, cre + 1);
}

void CreClassShowSumError()
{
    UniPrintToAll("**예외발생** 생산 가능한 최대치에 도달했기 때문에 더 이상 유닛을 만들 수 없습니다");
}

void CreSumClassMecaGolem(int anyArg, int owner)
{
    int pay = 57917, cre;
    int function = MechanicalGolemSubProcess;

    if (owner)
    {
        cre = CreClassSummonBuffer(SToInt("MechanicalGolem") | (function << 10), anyArg);
        if (cre)
            CreClassCommonProperty(cre, owner);
        else
            CreClassShowSumError();
    }
}

void CreSumClassPlant(int anyArg, int owner)
{
    int pay = 51008, cre;
    int function = CarnivorousPlantSubProcess;

    if (owner)
    {
        cre = CreClassSummonBuffer(SToInt("CarnivorousPlant") | (function << 10), anyArg);
        if (cre)
            CreClassCommonProperty(cre, owner);
        else
            CreClassShowSumError();
    }
}

void CreSumClassOrbHecubah(int anyArg, int owner)
{
    int pay = 50033, cre;
    int processer = HecubahWithOrbSubProcess;

    if (owner)
    {
        cre = CreClassSummonBuffer(SToInt("HecubahWithOrb") | (processer << 10), anyArg);
        if (cre)
        {
            CreClassCommonProperty(cre, owner);
        }
        else
            CreClassShowSumError();
    }
}

void CreSumClassRedWiz(int anyArg, int owner)
{
    int pay = 52349, cre;
    int processer = WizardRedSubProcess;

    if (owner)
    {
        cre = CreClassSummonBuffer(SToInt("WizardRed") | (processer << 10), anyArg);
        if (cre)
        {
            CreClassCommonProperty(cre, owner);
        }
        else
            CreClassShowSumError();
    }
}

int CreClassGetSumPay(int lFunction)
{
    int link = GetMemory(GetMemory(0x75ae28) + (0x30 * lFunction + 0x1c)) + 8;

    return GetMemory(link);
}

void CalleeTwoArg(int arg1, int arg2)
{
    CalleeTwoArg(arg1, arg2);
}

void CallFunctionWithTwoArg(int func, int arg1, int arg2)
{
    int link;

    if (!link)
        link = GetScrCodeField(CalleeTwoArg);
    else
    {
        SetMemory(link + 0x1c, func);
        CalleeTwoArg(arg1, arg2);
    }
}

void CreClassMarketDesc()
{
    int lFunction = ToInt(GetObjectZ(GetTrigger() + 1));
    int pay = CreClassGetSumPay(lFunction);

    TellStoryUnitName("oAo", "War08b:HenrickSaleSuccessful", "크리쳐 구입?");
    UniPrint(other, "이 크리쳐를 구입하시겠어요? 가격은 " + IntToString(pay) + " 입니다");
    UniPrint(other, "거래를 계속하려면 예를 누르세요");
}

void CreClassMarketTrade()
{
    int lFunction, pay;

    if (GetAnswer(self) ^ 1) return;
    lFunction = ToInt(GetObjectZ(GetTrigger() + 1));
    pay = CreClassGetSumPay(lFunction);
    if (GetGold(other) >= pay)
    {
        CallFunctionWithTwoArg(lFunction, other, GetCaller());
        ChangeGold(other, -pay);
        UniPrint(other, "크리쳐 구입에 성공했습니다");
    }
    else
    {
        UniPrint(other, "잔액이 부족하여 거래가 취소되었습니다");
    }
}

int CreClassPlaceMarket(int args)
{
    int location = args & 0x3ff, loadFunction = (args >> 10) & 0x3ff;
    string kpName = ToStr((args >> 20) & 0x3ff);
    int kp = DeadUnitCreateAt(LocationX(location), LocationY(location), kpName);

    Frozen(CreateObjectAt("ImaginaryCaster", LocationX(location), LocationY(location)) - 1, 1);
    Raise(kp + 1, loadFunction);
    SetDialog(kp, "YESNO", CreClassMarketDesc, CreClassMarketTrade);
    return kp;
}

void CreClassMarketPlace()
{
    CreSumClassMecaGolem(0, 0);
    CreSumClassOrbHecubah(0, 0);
    CreSumClassPlant(0, 0);
    CreSumClassRedWiz(0, 0);
    CreClassPlaceMarket(38 | (CreSumClassMecaGolem << 10) | (SToInt("Maiden") << 20));
    CreClassPlaceMarket(48 | (CreSumClassOrbHecubah << 10) | (SToInt("Hecubah") << 20));
    CreClassPlaceMarket(49 | (CreSumClassPlant << 10) | (SToInt("CarnivorousPlant") << 20));
    CreClassPlaceMarket(64 | (CreSumClassRedWiz << 10) | (SToInt("WizardRed") << 20));
    AliveLifePlaceMarket(73);
}

void SellCre()
{
    int owner;

    if (IsMonsterUnit(other))
    {
        owner = GetOwner(other);
        if (UnitCheckEnchant(other, GetLShift(30)))
        {
            Delete(GetCaller() + 1);
            Delete(other);
            CreClassDecreaseCurrentCount();
            if (CurrentHealth(owner) && IsPlayerUnit(owner))
            {
                ChangeGold(owner, 3000);
                UniPrint(owner, "크리쳐 1개를 지웠습니다 (+3000골드 돌려받음)");
            }
        }
    }
}

void BucherOneShot()
{
    int cFps = GetMemory(0x84ea04), ptr = UnitToPtr(self), stf;

    if (ptr)
    {
        stf = GetMemory(ptr + 0x2e0);
        if (MathAbs(cFps - GetMemory(stf + 0x64)) > 50)
        {
            SetMemory(stf + 0x64, cFps);
            ThrowBucherFx(other);
        }
    }
}

int PlaceArrowSword(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int sd = CreateObjectAt("Sword", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetWeaponProperties(sd, 5, 5, 8, 3);
    RegistWeaponCPropertyExecScript(sd, 2, WeaponExecScript1);
    return sd;
}

int PlaceDrainSword(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int sd = CreateObjectAt("GreatSword", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetWeaponProperties(sd, 5, 5, 8, 3);
    RegistWeaponCPropertyExecScript(sd, 2, DrainSwordFx);
    return sd;
}

int PlaceFonSword(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int sd = CreateObjectAt("GreatSword", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetWeaponProperties(sd, 5, 5, 8, 3);
    RegistWeaponCPropertyExecScript(sd, 2, ForceOfNatureShot);
    return sd;
}

int PlaceAxeForOgre(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int sd = CreateObjectAt("OgreAxe", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetWeaponProperties(sd, 0, 5, 0, 0);
    RegistWeaponCPropertyExecScript(sd, 2, BreakingAxeJin);
    return sd;
}

int PlaceAutoMissileMace(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int sd = CreateObjectAt("MorningStar", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetWeaponProperties(sd, 0, 5, 0, 0);
    RegistWeaponCPropertyExecScript(sd, 2, ShotAutoDetectMissile);
    return sd;
}

int PlaceBucherStaff(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int staff = CreateObjectAt("OblivionOrb", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2c4, 0x53a720);
    SetMemory(ptr + 0x2c8, ImportAllowAllDrop());
    SetUnitCallbackOnUseItem(staff, BucherOneShot);
    return staff;
}

void ResourceBGMdata()
{
    return;
}

int ObelMecaGolemAnim()
{
	int imgCount = 10, arr[10] = {
        ImagePtrFromID(138405), ImagePtrFromID(138404), ImagePtrFromID(138403), ImagePtrFromID(138402),
	    ImagePtrFromID(138401), ImagePtrFromID(138400), ImagePtrFromID(138399), ImagePtrFromID(138398),
	    ImagePtrFromID(138397), ImagePtrFromID(138396) };

	return &imgCount;
}

void AnimateImagePackage(int thingId, int imgTable)
{
    MakeAnimateImage(thingId, imgTable);
    InnerImageZeroHeight(thingId);
    RemoveSpriteFromThingId(thingId);
}

void SetupBottmText(int thingId, int ptr, int colorSet, string desc)
{
    ModifyThingClassDescription(thingId, desc, colorSet);
    ModifyThingClassDrawFunction(thingId, ptr);
    InnerImageZeroHeight(thingId);
    RemoveSpriteFromThingId(thingId);
}

void PlayerClassCommonWhenEntry()
{
    int ptr = BuildTextDrawFunction();

    SetupBottmText(2508, ptr, 0x7f6, "부활터 가는 문");
    SetupBottmText(2509, ptr, 0x7f6, "게임을 시작하지...");
    SetupBottmText(2510, ptr, 0x7f6, "필드로 나가는 포탈");
    AnimateImagePackage(2507, ObelMecaGolemAnim());
    SetupBottmText(2517, ptr, 0x7f6, "임.무.완.료.!.");
    SetupBottmText(2518, ptr, 0xff60, "중국인 입장하는 곳");
    SetupBottmText(2558, ptr, 0xff60, "중국인 입국완료 지점");
    SetupBottmText(2559, ptr, 0xfa02, "우리는 중국인");
    SetupBottmText(2560, ptr, 0xf8bd, "제작 및 기획- 237");
    SetMemory(0x69b974, 0x64);
    SetMemory(0x59dbe4, 1);
    MakeMusicDirectory();
    ExtractMapBgm("chn01.mp3", ResourceBGMdata);
    
    AppendAllDummyStaffsToWeaponList();
    ImageInit();
    ShowMessageBox("중국인 입국막기", "중국인들의 무분별한 국내 입국을 대 학살을 통해 막아보아요");
}

int AnimateRealloc(int ptr, int imgCount)
{
    int alloc = MemAlloc(24);
    int iLinkTable = MemAlloc((imgCount + 1) * 4);

    SetMemory(alloc, 0x10);
    SetMemory(alloc + 4, iLinkTable);
    SetMemory(alloc + 8, imgCount);
    SetMemory(alloc + 12, 2);
    SetMemory(alloc + 16, 0);
    SetMemory(alloc + 20, 0);
    SetMemory(ptr + 92, alloc);
    return iLinkTable;
}

void ImageTableMapping(int dest, int src, int count)
{
    int i;

    for (i = 0 ; i < count ; i ++)
        SetMemory(dest + (i * 4), GetMemory(src + (i * 4)));
    SetMemory(dest + (i * 4), 0);
}

int MakeAnimateImage(int thingId, int imgTable)
{
    int ptr = GetMemory(GetMemory(0x69f224) + (thingId * 4));
    int imgPtr = GetMemory(ptr + 92), mapped, freePtr = 0;
    int imgCount = GetMemory(imgTable);

    while (1)
    {
        if (GetMemory(imgPtr) == 0x08)
            1;
        else if (GetMemory(imgPtr + 0x8) ^ imgCount)
            freePtr = GetMemory(imgPtr + 4);
        else
        {
            mapped = GetMemory(imgPtr + 4);
            break;
        }
        mapped = AnimateRealloc(ptr, imgCount);
        if (freePtr)
            MemFree(freePtr);
        break;
    }
    ImageTableMapping(mapped, imgTable + 4, imgCount);
    SetMemory(ptr + 116, 134719);
    SetMemory(ptr + 88, 0x4bbd60);
}

int DrawImageAt(float x, float y, int thingId)
{
    int unit = CreateObjectAt("AirshipBasketShadow", x, y);
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x04, thingId);
    return unit;
}

int DrawImageAtLocation(int location, int thingId)
{
    return DrawImageAt(LocationX(location), LocationY(location), thingId);
}

int GetImageTableFirstImgPtr(int iTablePtr)
{
    return GetMemory(iTablePtr + 4);
}

int ImagePtrFromID(int imgId)
{
    return GetMemory(0x694864) + (12 * imgId);
}

void InnerImageZeroHeight(int thingId)
{
    int ptr = GetMemory(GetMemory(0x69f224) + (thingId * 4));

    if (GetMemory(ptr + 20) >> 0x10)
        SetMemory(ptr + 20, GetMemory(ptr + 20) & 0xffff);
    if (GetMemory(ptr + 32) & 0x400000)
        SetMemory(ptr + 32, GetMemory(ptr + 32) ^ 0x400000);
}

void InnerImageSwap(int thingId, int imgId)
{
    int ptr = GetMemory(GetMemory(0x69f224) + (thingId * 4));
    int imgPtr = GetMemory(ptr + 92);
    SetMemory(ptr + 116, imgId);

    if (GetMemory(imgPtr) == 0x10)
    {
        SetMemory(imgPtr, 0x08);
        MemFree(GetMemory(imgPtr + 4));
    }
    SetMemory(ptr + 88, 0x4bcc20);
    SetMemory(imgPtr + 4, ImagePtrFromID(imgId));
}

int ImportRemoveSpriteFromThingId()
{
    int arr[16], link;

    if (!link)
    {
        arr[0] = 0x5008EC83; arr[1] = 0x6D3DC0A1; arr[2] = 0x24448900; arr[3] = 0x623EE808; arr[4] = 0x4489FFDB; arr[5] = 0x8B560424;
        arr[6] = 0x850C2474; arr[7] = 0x8B1B74F6; arr[8] = 0xFF082444; arr[9] = 0x000170B6; arr[10] = 0x768B5600; arr[11] = 0x75F0396C;
        arr[12] = 0x94AAE805; arr[13] = 0x5E58FFD0; arr[14] = 0x585EE1EB; arr[15] = 0xC308C483;
        link = &arr;
        FixCallOpcode(link + 0x0d, 0x507250);
        FixCallOpcode(link + 0x31, 0x45a4e0);
    }
    return link;
}

void RemoveSpriteFromThingId(int thingId)
{
    int temp = GetMemory(0x5c3108);

    SetMemory(0x5c3108, ImportRemoveSpriteFromThingId());
    Unused1f(thingId);
    SetMemory(0x5c3108, temp);
}

void MappingTextDrawCode(int ptr)
{
    SetMemory(ptr + 0, 0x8B20EC83); SetMemory(ptr + 4, 0x84EA040D);
    SetMemory(ptr + 8, 0x56555300); SetMemory(ptr + 12, 0x3424748B); 
    SetMemory(ptr + 16, 0xAC3D8B57); SetMemory(ptr + 20, 0x8B00853B);
    SetMemory(ptr + 24, 0x00012086); SetMemory(ptr + 28, 0x72C83900); 
    SetMemory(ptr + 32, 0xE03D8B06); SetMemory(ptr + 36, 0x570084C9);
    SetMemory(ptr + 40, 0xCE336FE8); SetMemory(ptr + 44, 0x988e8bff); //SetMemory(ptr + 44, 0xC00D8BFF); 
    SetMemory(ptr + 48, 0x51000000); //SetMemory(ptr + 48, 0x510083C8);
    SetMemory(ptr + 52, 0xCE3293E8);
    SetMemory(ptr + 56, 0x244C8BFF); SetMemory(ptr + 60, 0x0C468B3C); 
    SetMemory(ptr + 64, 0x8B08C483); SetMemory(ptr + 68, 0x118B1059);
    SetMemory(ptr + 72, 0x2914698B); SetMemory(ptr + 76, 0x105E8BD8); 
    SetMemory(ptr + 80, 0x518BD001); SetMemory(ptr + 84, 0x704E8B04);
    SetMemory(ptr + 88, 0x4489EA29); SetMemory(ptr + 92, 0xDA011024); 
    SetMemory(ptr + 96, 0x14245489); SetMemory(ptr + 100, 0xA16C4E8B);
    SetMemory(ptr + 104, 0x0069F224); SetMemory(ptr + 108, 0x8B88048B); 
    SetMemory(ptr + 112, 0xD2850450); SetMemory(ptr + 116, 0x00BA0575);
    SetMemory(ptr + 120, 0x8B0058D7); SetMemory(ptr + 124, 0x8B142444); 
    SetMemory(ptr + 128, 0x5010244C); SetMemory(ptr + 132, 0x006A5251);
    SetMemory(ptr + 136, 0xCEE58FE8); SetMemory(ptr + 140, 0x10C483FF); 
    SetMemory(ptr + 144, 0x000001B8); SetMemory(ptr + 148, 0x5D5E5F00);
    SetMemory(ptr + 152, 0x20C4835B); SetMemory(ptr + 156, 0x909090C3);
    FixCallOpcode(ptr + 40, 0x434460);
    FixCallOpcode(ptr + 52, 0x434390);
    FixCallOpcode(ptr + 136, 0x43f6e0);
}

void ModifyThingClassDescription(int thingId, string desc, int textColor)
{
    int tPtr = GetMemory(GetMemory(0x69f224) + (thingId * 4));
    int dest = MemAlloc(48), prevPtr = GetMemory(tPtr + 4);
    int src = GetMemory(0x97bb40 + (SToInt(desc) * 4));

    NoxUtf8ToUnicode(src, dest);
    if (textColor)
        SetMemory(tPtr + 48, textColor);
    SetMemory(tPtr + 4, dest);
    if (prevPtr)
        MemFree(prevPtr);
}

void ModifyThingClassDrawFunction(int thingId, int fPtr)
{
    int tPtr = GetMemory(GetMemory(0x69f224) + (thingId * 4));

    SetMemory(tPtr + 88, fPtr);
}

int BuildTextDrawFunction()
{
    int offset = 0x5cebdc;
    
    MappingTextDrawCode(offset);
    return offset;
}

static void NetworkUtilClientMain()
{
    PlayerClassCommonWhenEntry();
}

void UpdateRepairItem(int pIndex, int item)
{
    int arr[9], link, temp = GetMemory(0x5c3108), ptr = UnitToPtr(item);

    if (!link)
    {
        arr[0] = 0x50685056; arr[1] = 0xFF005072; arr[2] = 0x708B2414; arr[3] = 0x04C48304; arr[4] = 0x4D87A068; arr[5] = 0x30FF5600; arr[6] = 0x082454FF;
        arr[7] = 0x580CC483; arr[8] = 0x9090C35E;
		link = &arr;
    }
    if (ptr)
    {
        item = ptr;
        SetMemory(0x5c3108, link + 8);
        Unused1f(link);
        SetMemory(0x5c3108, temp);
    }
}

int SellGerm(int inv)
{
    int thingId = GetUnitThingID(inv), pic;

    if (thingId >= 2795 && thingId <= 2797)
    {
        Delete(inv);

        int pay[] = {1000, 5000, 10000};
        return pay[2797 - thingId];
    }
    else
        return 0;
}

int FindItemGerm(int holder)
{
    int inv = GetLastItem(holder), res = 0, nextInv;

    while (inv)
    {
        nextInv = GetPreviousItem(inv);
        res += SellGerm(inv);
        inv = nextInv;
    }
    return res;
}

void SellGermDesc()
{
    UniPrint(other, "당신이 소유하고 있는 모든 보석을 저에게 파시겠어요? 거래를 계속하려면 '예' 버튼을 누르세요");
    TellStoryUnitName("oAo", "thing.db:DiscDiamond", "보석 감별사");
}

void SellGermTrade()
{
    int trdRes;

    if (GetAnswer(self) ^ 1) return;
    trdRes = FindItemGerm(other);
    if (trdRes)
    {
        ChangeGold(other, trdRes);
        UniPrint(other, "가지고 있던 모든 보석을 팔아서 " + IntToString(trdRes) + " 골드를 추가했습니다");
    }
    else
        UniPrint(other, "당신은 보석을 하나도 가지고 있지 않아요. 거래를 계속할 수 없습니다");
}

int SellGermPlace(int location)
{
    int kp = DeadUnitCreateAt(LocationX(location), LocationY(location), "UrchinShaman");

    Frozen(kp, 1);
    SetDialog(kp, "YESNO", SellGermDesc, SellGermTrade);
    return kp;
}

int RepairSingle(int inv, int pIndex)
{
    if (MaxHealth(inv) ^ CurrentHealth(inv))
    {
        RestoreHealth(inv, 99999);
        UpdateRepairItem(pIndex, inv);
        return 1;
    }
    return 0;
}

int RepairItemOnInventory(int holder)
{
    int count = 0, inv = GetLastItem(holder), pIndex = GetPlayerIndex(holder);

    while (inv)
    {
        count += RepairSingle(inv, pIndex);
        inv = GetPreviousItem(inv);
    }
    return count;
}

void RepairItemDesc()
{
    TellStoryUnitName("oAo", "WarriorHint:Repair", "아이템 수리?");
    UniPrint(other, "인벤토리 내 모든 아이템을 금화 4천에 수리해 드려요~ 계속 거래하시려면 '예' 를 누르세요");
}

void RepairItemTrade()
{
    int iRes = 0;

    if (GetAnswer(self) ^ 1) return;
    if (GetGold(other) >= 4000)
    {
        iRes = RepairItemOnInventory(other);
        if (iRes)
        {
            ChangeGold(other, -4000);
            PlaySoundAround(self, 803);
            UniPrint(other, IntToString(iRes) + "개 아이템이 수리되었습니다 (-4000 골드 차감됨)");
            Effect("LESSER_EXPLOSION", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        }
    }
    else
        UniPrint(other, "금화가 부족하여 처리되지 않았습니다. 금화를 모은 후 다시 시도해 보세요");
}

int RepairItemPlaceKp(int location)
{
    int kp = DeadUnitCreateAt(LocationX(location), LocationY(location), "Urchin");

    Frozen(kp, 1);
    SetDialog(kp, "YESNO", RepairItemDesc, RepairItemTrade);
    return kp;
}

void ExplosionMeteor(int sub)
{
    int owner = GetOwner(sub);

    SplashDamageAt(owner, 300, GetObjectX(sub), GetObjectY(sub), 200.0);
    PlaySoundAround(sub, 87);
    DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(sub), GetObjectY(sub)), 12);
    DeleteObjectTimer(CreateObjectAt("MeteorExplode", GetObjectX(sub), GetObjectY(sub)), 12);
}

void ThrowMeteor(int sub)
{
    int count = GetDirection(sub), owner = GetOwner(sub);

    while (IsObjectOn(sub))
    {
        if (CurrentHealth(owner))
        {
            if (count)
            {
                Raise(sub + 1, GetObjectZ(sub + 1) - 5.0);
                FrameTimerWithArg(1, sub, ThrowMeteor);
                LookWithAngle(sub, count - 1);
                break;
            }
            else
                ExplosionMeteor(sub);
        }
        Delete(sub);
        Delete(sub + 1);
        break;
    }
}

void AmuletUseHitMeteor()
{
    int sub;

    if (IsObjectOn(self) && CurrentHealth(other))
    {
        sub = CreateObjectAt("ImaginaryCaster", GetObjectX(other), GetObjectY(other));
        Raise(CreateObjectAt("BoulderMine", GetObjectX(sub), GetObjectY(sub)), 250.0);
        FrameTimerWithArg(1, sub, ThrowMeteor);
        PlaySoundAround(sub, 85);
        UnitNoCollide(sub + 1);
        SetOwner(other, sub);
        LookWithAngle(sub, 50);
        Delete(self);
    }
}

void AutoTargetDeathraySight()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && CurrentHealth(self))
    {
        Damage(other, owner, 100, 16);
        PlaySoundAround(other, 299);
        Effect("DEATH_RAY", GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other));
    }
}

void AutoDeathRaySword()
{
    int sub = CreateObjectAt("WeirdlingBeast", GetObjectX(other) + UnitAngleCos(other, 6.0), GetObjectY(other) + UnitAngleSin(other, 6.0));

    UnitNoCollide(sub);
    SetOwner(other, sub);
    SetCallback(sub, 3, AutoTargetDeathraySight);
    LookWithAngle(sub, GetDirection(other));
    DeleteObjectTimer(sub, 1);
    PlaySoundAround(sub, 27);
    Effect("VIOLET_SPARKS", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
}

int PlaceAutoTargetSword(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int sd = CreateObjectAt("GreatSword", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetWeaponProperties(sd, 5, 5, 19, 23);
    RegistWeaponCPropertyExecScript(sd, 2, AutoDeathRaySword);
    return sd;
}

int PlaceMeteorAmulet(int sArg)
{
    int xyTable = AnyargUnitOrLocation(sArg);
    int sd = CreateObjectAt("AmuletofManipulation", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));

    SetUnitCallbackOnUseItem(sd, AmuletUseHitMeteor);
    return sd;
}

void UserShowGameInfo(int pUnit)
{
    if (MaxHealth(pUnit))
    {
        UniPrint(pUnit, "우리는 중국인_version. 2                                                         기획 및 제작. 237");
        UniPrint(pUnit, "때는 2025년... 한국 본토 점령 프로젝트 준비를 위해 중국 공산당 간첩들이 대거 국내로 입국하게 되는데... ");
    }
}

void AliveLifeDesc()
{
    int cce = GameClassLifeCheck();

    TellStoryUnitName("oAo", "Con10B.scr:HecubahDialog1", "라이프구입?");
    UniPrint(other, "1 라이프를 구입하시겠어요? 가격은 5만 골드에요. 현재 라이프 수: " + IntToString(cce));
}

void AliveLifeTrade()
{
    int cce;

    if (GetAnswer(self) ^ 1) return;
    cce = GameClassLifeCheck();
    if (cce < 48)
    {
        if (GetGold(other) >= 50000)
        {
            ChangeGold(other, -50000);
            GameClassLifeSet(cce + 1);
            PlaySoundAround(other, 1004);
            UniPrint(other, "라이프 1개를 추가 했어요! 남은 라이프 수: " + IntToString(cce));
            Effect("YELLOW_SPARKS", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        }
        else
            UniPrint(other, "거래 실패! 금화 부족");
    }
    else
        UniPrint(other, "더 이상 라이프를 추가할 수 없어요!");
}

int AliveLifePlaceMarket(int location)
{
    int dm = DeadUnitCreateAt(LocationX(location), LocationY(location), "Maiden");

    Frozen(CreateObjectAt("Ankh", LocationX(location), LocationY(location)) - 1, 1);
    SetDialog(dm, "YESNO", AliveLifeDesc, AliveLifeTrade);
    return dm;
}

int PlaceDemonWand(int anyArg)
{
    int xyTable = AnyargUnitOrLocation(anyArg);
    int sd = CreateObjectAt("DemonsBreathWand", GetMemoryFloat(xyTable), GetMemoryFloat(xyTable + 4));
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 736) + 108, 0xc8c8);
    return sd;
}

void ManaTileSingle(int location, float xVect, float yVect)
{
    CreateObjectAt("InvisibleObelisk", LocationX(location), LocationY(location));
    TeleportLocationVector(location, xVect, yVect);
}

void ManaTileAll(int count)
{
    int i;

    if (count)
    {
        for (i = 0 ; i < 7 ; i += 1)
            ManaTileSingle(i + 66, 23.0, 23.0);
        FrameTimerWithArg(1, count - 1, ManaTileAll);
    }
}

void ImageResourceField1()
{
    return;
}

void ImageResourceKor() {}
void ImageResourceChin() {}

void RedPotionImage() {}
void PoisonPotionImage() {}
void VampPotionImage() {}
void YellowPotionImage() {}
void MagicPotionImage() {}

void ResourceBucherImage() {}

void ResourceDollar()
{ }

void ImageInit()
{
    int drawF = ImageResourceDrawFunctionFix(0x7510c4);

    ApplyImageResourceFix(357, GetScrCodeField(ImageResourceField1) + 4, drawF);
    ApplyImageResourceFix(636, GetScrCodeField(RedPotionImage) + 4, drawF);
    ApplyImageResourceFix(2682, GetScrCodeField(VampPotionImage) + 4, drawF);
    ApplyImageResourceFix(631, GetScrCodeField(PoisonPotionImage) + 4, drawF);
    ApplyImageResourceFix(2686, GetScrCodeField(MagicPotionImage) + 4, drawF);
    ApplyImageResourceFix(639, GetScrCodeField(YellowPotionImage) + 4, drawF);
    ApplyImageResourceFix(358, GetScrCodeField(ResourceBucherImage) + 4, drawF);
    ApplyImageResourceFix(359, GetScrCodeField(ImageResourceChin) + 4, drawF);
    ApplyImageResourceFix(360, GetScrCodeField(ImageResourceKor) + 4, drawF);
    ApplyImageResourceFix(979, GetScrCodeField(ResourceDollar) + 4, drawF);
}

void PotionImgTest()
{
    CreateYellowPotion(144, LocationX(79), LocationY(79));
    CreateObject("VampirismPotion", 80);
    CreateObject("RedPotion", 80);
    PlaceBucherStaff(81);
}

void RepeatDrawSmokeRing(float x, float y, float gap, int rep)
{
    if (rep)
    {
        DeleteObjectTimer(CreateObjectAt("OldSmoke", x + MathSine(rep * 15 + 90, gap), y + MathSine(rep * 15, gap)), 3);
        RepeatDrawSmokeRing(x, y, gap, rep - 1);
    }
}

void SpreadSmokeRing(int sub, int count, float gap)
{
    float xProfile = GetObjectX(sub), yProfile = GetObjectY(sub);

    if (count)
    {
        RepeatDrawSmokeRing(xProfile, yProfile, gap, 24);
        SpreadSmokeRing(sub, count - 1, gap + 20.0);
    }
}

void SmokeRingBlast(int unit, int count, float gap)
{
    int sub = CreateObjectAt("ImaginaryCaster", GetObjectX(unit), GetObjectY(unit));

    SpreadSmokeRing(sub, count, gap);
    Delete(sub);
}

void ThrowBucherDestroyed(int sub)
{
    SmokeRingBlast(sub, 8, 20.0);
    SplashDamageAt(GetOwner(sub), 240, GetObjectX(sub), GetObjectY(sub), 200.0);
    PlaySoundAround(sub, 759);
    Effect("JIGGLE", GetObjectX(sub), GetObjectY(sub), 20.0, 0.0);
}

void ThrowBucherDurate(int sub)
{
    int owner = GetOwner(sub), durate = GetDirection(sub);

    while (IsObjectOn(sub))
    {
        if (CurrentHealth(owner))
        {
            if (durate)
            {
                FrameTimerWithArg(1, sub, ThrowBucherDurate);
                LookWithAngle(sub, durate - 1);
                Raise(sub + 1, GetObjectZ(sub + 1) - 10.0);
                break;
            }
            else
                ThrowBucherDestroyed(sub);
        }
        Delete(sub);
        Delete(sub + 1);
        break;
    }
}

void ThrowBucherFx(int owner)
{
    float xVect = UnitAngleCos(owner, 23.0),yVect = UnitAngleSin(owner, 23.0);
    int sub = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner) + xVect, GetObjectY(owner) + yVect);

    FrameTimerWithArg(1, sub, ThrowBucherDurate);
    LookWithAngle(CreateObjectAt("MovableStatue1b", GetObjectX(sub), GetObjectY(sub)) - 1, 25);
    UnitNoCollide(sub + 1);
    Raise(sub + 1, 250.0);
    SetOwner(owner, sub);
}

void GodModePotionFx(int fxUnit)
{
    int owner = GetOwner(fxUnit);

    while (IsObjectOn(fxUnit))
    {
        if (CurrentHealth(owner))
        {
            int durate = ToInt(GetObjectZ(fxUnit));

            if (durate)
            {
                if (!UnitCheckEnchant(owner, GetLShift(23)))
                    Enchant(owner, EnchantList(23), 0.0);
                FrameTimerWithArg(1, fxUnit, GodModePotionFx);
                Raise(fxUnit, durate - 1);
                MoveObject(fxUnit + 1, GetObjectX(owner), GetObjectY(owner));
            }
            EnchantOff(owner, EnchantList(23));
        }
        Delete(fxUnit);
        Delete(fxUnit + 1);
        break;
    }
}

void UseGodModePotion()
{
    Delete(self);
    if (!UnitCheckEnchant(other, GetLShift(23)))
    {
        UniPrint(other, "무적의 물약을 사용하셨습니다. 15 초간 무적상태가 유지됩니다");
        int fxUnit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(other), GetObjectY(other));

        SetOwner(other, CreateObjectAt("Smoke", GetObjectX(other), GetObjectY(other)) - 1);
        Raise(fxUnit, 450);
        FrameTimerWithArg(1, fxUnit, GodModePotionFx);
    }
}

