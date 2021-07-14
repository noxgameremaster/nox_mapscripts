
#include "noxscript\builtins.h"
#include "libs\typecast.h"
#include "libs\callmethod.h"
#include "libs\unitutil.h"
#include "libs\buff.h"
#include "libs\printutil.h"
#include "libs\mathlab.h"
#include "libs\playerupdate.h"
#include "libs\username.h"
#include "libs\unitstruct.h"
#include "libs\cmdline.h"
#include "libs\coopteam.h"
#include "libs\spellutil.h"

#define GUARD_COUNT     40
#define GAP_SCORE       10

int NPCS[40]; //npcs
int player[20], RndTable[40], AIKnight[5];
int HumanScore, BotScore, ggOver;


int Bear2BinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1918985538; arr[1] = 50;
		arr[16] = 20000; arr[17] = 90; arr[18] = 100; arr[19] = 40; 
		arr[21] = 1065353216; arr[23] = 65545; arr[24] = 1067450368; 
		arr[27] = 1; arr[28] = 1106247680; arr[29] = 50; 
		arr[30] = 1103626240; arr[31] = 2; arr[32] = 20; arr[33] = 30;
		arr[58] = 5547856; arr[59] = 5542784; 
        link = &arr;
	}
	return link;
}

void SetSpellRule()
{
    CmdLine("set spell SPELL_FUMBLE off", false);
}

void RegistQueue(int unit)
{
    int k;

    for (k = 4 ; k >= 0 ; k --)
    {
        if (!AIKnight[k])
        {
            AIKnight[k] = unit;
            LookWithAngle(GetPreviousItem(GetLastItem(unit)), k);
            break;
        }
    }
}

static int InitMathSineBaseWaypointNumber() { return 104; }

void InitRTable()
{
    int k;

    for (k = 39 ; k >= 0 ; k -= 1)
        RndTable[k] = k;
    MixRndTable();
}

void MixRndTable()
{
    int k, temp, pic;

    for (k = 39 ; k >= 0 ; k -= 1)
    {
        pic = Random(0, 39);
        temp = RndTable[k];
        RndTable[k] = RndTable[pic];
        RndTable[pic] = temp;
    }
}

int PopRndIndex()
{
    int cur, res;

    while (true)
    {
        res = RndTable[cur];
        cur += 1;
        if (cur == 40)
        {
            MixRndTable();
            cur = 0;
        }
        if (!IsObjectOn(NPCS[res]))
            break;
    }
    return res;
}

void BonusEventDescription()
{
    UniPrintToAll("**이 맵에서 전사는 조심스럽게 걷기를 시전할 경우 대쉬(짧은거리를 순간적으로 이동)가 발동됩니다**");
    UniPrintToAll("**보너스 킬 획득 방법: 맵에 알파고가 단 한마리도 없을 시 인간 팀에게 킬 스코어 +1 을 더 줍니다**");
}

void startMent()
{
    UniPrintToAll("극한의 막장 인공지능과의 전투 in.익스사원___ 인간 대 알파고 Human VS AlphaGo                               -제작: happySoft");
    UniPrintToAll("**게임승리 조건: 자기 팀 점수가 상대팀 점수보다 " + IntToString(GAP_SCORE) + " 점 앞서면 승리합니다**");
    DisplayScore();
    FrameTimer(1, LoopSearchIndex);
    SecondTimer(10, BonusEventDescription);
}

void MapExit()
{
    RemoveCoopTeamMode();
    ResetPlayerHandlerWhenExitMap();
    UniPrintToAll("안녕히 가십시오 by. 알파고");
}

void PlayerDeath()
{
    AddBotScore();
    UniPrintToAll("방금 인간팀 소속 플레이어가 적에게 격추되었습니다. 플레이어가 자살한 경우에도 상대팀 킬에 누적되므로 주의하세요");
}

void InventoryEmptyAll(int unit)
{
    while (IsObjectOn(GetLastItem(unit)))
        Delete(GetLastItem(unit));
}

int PlayerClassOnInit(int plr, int pUnit)
{
    int pResult = plr;

    player[plr] = pUnit;
    player[plr + 10] = 1;

    DiePlayerHandlerEntry(pUnit);

    UniPrintToAll(PlayerIngameNick(pUnit) + " 님께서 게임에 참가하셨습니다");

    return pResult;
}

void PlayerClassFailedJoin()
{ }

void getPlayers()
{
    int i, plr;

    while (true)
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
            if (plr >= 0)
            {
                teleportPlayer(plr);
                break;
            }
        }
        PlayerClassFailedJoin();
        break;
    }
}

int CheckPlayer()
{
    int k;

    for (k = 9 ; k >= 0 ; k -= 1)
    {
        if (IsCaller(player[k]))
            return k;
    }
    return -1;
}

int PlayerClassCheckDeathFlag(int plr)
{
    return player[plr + 10] & 2;
}

void PlayerClassSetDeathFlag(int plr)
{
    player[plr + 10] ^= 2;
}

void PlayerClassOnAlive(int pUnit)
{
    if (UnitCheckEnchant(pUnit, GetLShift(31)))
    {
        Windbooster(pUnit);
        EnchantOff(pUnit, EnchantList(31));
        RemoveTreadLightly(pUnit);
    }
}

void PlayerClassOnFree(int plr)
{
    int ptr = UnitToPtr(player[plr]);

    player[plr] = 0;
    player[plr + 10] = 0;
}

//@brief. 데스 핸들러
void PlayerClassOnDeath(int plr)
{ }

void PlayerClassOnLoop()
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
                    PlayerClassOnAlive(player[i]);
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
                PlayerClassOnFree(i);
            break;
        }
    }
    FrameTimer(1, PlayerClassOnLoop);
}

void Windbooster(int unit)
{
    if (!HasEnchant(unit, "ENCHANT_ANCHORED"))
    {
        Effect("RICOCHET", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        PushObjectTo(unit, UnitAngleCos(unit, 60.0), UnitAngleSin(unit, 60.0));
    }
}

void teleportPlayer(int plr)
{
    int wp = Waypoint("respawnLocation" + IntToString(Random(1, 18)));
    
    MoveObject(player[plr], GetWaypointX(wp), GetWaypointY(wp));
    DeleteObjectTimer(CreateObject("OblivionUp", wp), 30);
    Effect("TELEPORT", GetWaypointX(wp), GetWaypointY(wp), 0.0, 0.0);
    Effect("SMOKE_BLAST", GetWaypointX(wp), GetWaypointY(wp), 0.0, 0.0);
    AudioEvent("BlinkCast", wp);
}

void WarEnemyDetectHandler()
{
    int rnd = Random(0, 4);

    if (!rnd)
    {
        BerserkerInRange(GetTrigger(), GetCaller(), 10);
    }
    else if (rnd == 1)
    {
        WarriorWarcry(GetTrigger());
    }
    else if (rnd == 2)
    {
        WarThrowSword(GetTrigger(), GetCaller());
    }
    else if (rnd == 3)
    {
        WarriorStrikeSubPart(6);
    }
    else if (rnd == 4)
    {
        WarThrowSword(GetTrigger(), GetCaller());
        BerserkerInRange(GetTrigger(), GetCaller(), 10);
    }
}

void LookForEnemy()
{
    if (HasEnchant(self, "ENCHANT_PROTECT_FROM_POISON"))
    {
        if (IsConjurer(self))
        {
            AbsoluteTargetStrike(GetTrigger(), GetCaller(), 85.0, GetShootXBowFunc());
            //Test
            //UniChatMessage(self, "Guardian Number: " + IntToString(GetDirection(GetLastItem(self))), 120);
            //UniPrintToAll(IntToString(GetDirection(GetLastItem(self)))); //TODO: Remove me
            //TestEnd
        }
        else
        {
            WarEnemyDetectHandler();
        }
    }
    CheckResetSight(GetTrigger(), 30 * Random(4, 6));
}

void CheckResetSight(int unit, int delay)
{
    if (!HasEnchant(unit, "ENCHANT_DETECTING"))
    {
        AroundWallDestroy(unit);
        Enchant(unit, "ENCHANT_DETECTING", 0.0);
        FrameTimerWithArg(delay, unit, ResetUnitSight);
    }
}

void ResetUnitSight(int unit)
{
    EnchantOff(unit, "ENCHANT_DETECTING");
    Enchant(unit, "ENCHANT_BLINDED", 0.1);
    AggressionLevel(unit, 1.0);
}

void dontDie()
{
    UniPrintToAll("예외발생! 가디언(npc) 1 개가 손실되었습니다 .");
}

void hurtNPC()
{
    int die_hp = MaxHealth(self) - 150, dead;

    if (HasEnchant(self, "ENCHANT_SNEAK"))
    {
        die_hp += 50;
        if (HasClass(other, "MISSILE") && HasSubclass(other, "IMMUNE_FEAR"))
        {
            teleportRandomWarp(self);
            CastSpellObjectObject("SPELL_COUNTERSPELL", self, self);
            RestoreHealth(self, 200);
        }
    }
    Damage(self, other, 10, 14);
    if (CurrentHealth(self) && CurrentHealth(self) < die_hp)
    {
        AlphagoDeathHandler();
        PlayerKillHandler(CheckPlayer());
    }
}

void PlayerKillHandler(int plr)
{
    if (plr + 1)
    {
        if (CurrentHealth(player[plr]))
            BerserkerNoDelayCore(plr);
    }
}

int CheckOwner(int unit)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (IsOwnedBy(unit, player[i]))
            return i;
    }
    return -1;
}

void AlphagoDeathHandler()
{
    int dead;

    MoveWaypoint(46, GetObjectX(self), GetObjectY(self));
    FrameTimerWithArg(3, CreateObject("InvisibleLightBlueHigh", 46), PutAlphagoDeathText);
    dead = CreateObject("Swordsman", 46);
    DeleteObjectTimer(dead, 150);
    LookAtObject(dead, other);
    Damage(dead, 0, 255, 14);
    RestoreHealth(self, MaxHealth(self));
    CastSpellObjectObject("SPELL_CURE_POISON", self, self);
    EnchantOff(self, "ENCHANT_PROTECT_FROM_POISON");
    EnchantOff(self, "ENCHANT_SNEAK");
    EnchantOff(self, "ENCHANT_INFRAVISION");
    Enchant(self, "ENCHANT_FREEZE", 0.0);
    Enchant(self, "ENCHANT_BURNING", 0.0);
    getAutoPlaced(self);                        //here: NPC[idx + 40] = 0
    ObjectOff(self);
    UniPrintToAll("* 가디언 한마리를 격추 시켰습니다-! (인간팀 킬수 + 1)");
    if (!ggOver)
    {
        AIKnight[GetDirection(GetPreviousItem(GetLastItem(self)))] = 0;
        FrameTimerWithArg(150, PopRndIndex(), releaseKnight);
    }
    AddHumanScore();
}

int CheckGuardianCount(int max)
{
    int i, res = 0;

    for (i = 0 ; i < max ; i ++)
        res += (CurrentHealth(AIKnight[i]) > 0);
    return res;
}

void AllKillsBonus(int max)
{
    if (!CheckGuardianCount(max))
    {
        HumanScore ++;
        UniPrintToAll("지금 맵에 알파고가 전멸되었습니다! (알파고 싹쓸이 보너스 킬 +1)");
    }
}

int AddHumanScore()
{
    HumanScore ++;
    AllKillsBonus(5);
    if (!ggOver)
    {
        if (HumanScore > BotScore + GAP_SCORE)
        {
            ggOver = 1;
            VictoryEvent(48);
            return 1;
        }
    }
    return 0;
}

int AddBotScore()
{
    BotScore ++;
    if (!ggOver)
    {
        if (BotScore > HumanScore + GAP_SCORE)
        {
            ggOver = 1;
            DefeatGame(93);
            return 1;
        }
    }
    return 0;
}

void VictoryEvent(int wp)
{
    UniPrintToAll("인간팀 승리! -- 대단합니다, 인간팀이 알파고 팀의 점수보다 " + IntToString(GAP_SCORE) + "점을 앞서 승리하였습니다...!");
    UniPrintToAll("축하합니다! 승리하셨습니다");
    ObjectOff(Object("mainSwitch"));
    strVictory();
    teleportAllPlayer(wp);
    CureHumanStatus();
    MoveObject(Object("PlayerStartLocation"), GetWaypointX(wp), GetWaypointY(wp));
    DeleteObjectTimer(CreateObject("LevelUp", wp), 600);
}

void DefeatGame(int wp)
{
    UniPrintToAll("인간팀 패배! -- 이번 게임에서는 알파고 팀이 승리했습니다, 알파고 팀이 인간팀 점수보다 " + IntToString(GAP_SCORE) + "점 앞섰습니다");
    UniPrintToAll("패배하셨습니다");
    ObjectOff(Object("mainSwitch"));
    MoveWaypoint(1, GetWaypointX(wp) - 100.0, GetWaypointY(wp) - 100.0);
    StrDefeatTeam();
    MoveObject(Object("PlayerStartLocation"), GetWaypointX(wp), GetWaypointY(wp));
    teleportAllPlayer(wp);
}

void CureHumanStatus()
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (CurrentHealth(player[i]))
        {
            CastSpellObjectObject("SPELL_CURE_POISON", player[i], player[i]);
            RestoreHealth(player[i], MaxHealth(player[i]) - CurrentHealth(player[i]));
        }
    }
}

void teleportAllPlayer(int arg_0)
{
    int i;
    
    for (i = 9 ; i >= 0 ; i --)
    {
        if (CurrentHealth(player[i]))
        {
            MoveObject(player[i], GetWaypointX(arg_0), GetWaypointY(arg_0));
            Enchant(player[i], "ENCHANT_ANCHORED", 0.0);
        }
    }
}

void perceiveToEnemy()
{
    int rnd, ptr;

    if (CurrentHealth(self) && HasEnchant(self, "ENCHANT_SNEAK") && !HasEnchant(self, "ENCHANT_CROWN"))
    {
        rnd = Random(0, 5);
        if (!rnd)
        {
            AbsoluteTargetStrike(GetTrigger(), GetCaller(), 9.0, GetMeteorShotFunc());
            CastSpellObjectLocation("SPELL_BURN", self, GetObjectX(other), GetObjectY(other));
            Enchant(self, "ENCHANT_CROWN", 3.0);
        }
        else if (rnd == 1)
        {
            if (!Random(0, 5))
                CastSpellObjectLocation(randomSummonUnit(), self, GetObjectX(other), GetObjectY(other));
            //HitFarLocation(self, GetObjectX(other), GetObjectY(other));
            AbsoluteTargetStrike(GetTrigger(), GetCaller(), 85.0, GetShootXBowFunc());
            Enchant(self, "ENCHANT_CROWN", 2.0);
        }
        else if (rnd == 2)
        {
            AbsoluteTargetStrike(GetTrigger(), GetCaller(), 10.5, GetFistShotFunc());
            FrameTimerWithArg(35, GetTrigger(), GuardianCastTeleport);
            PauseObject(self, 50);
            Enchant(self, "ENCHANT_CROWN", 5.0);
        }
        else if (rnd == 3)
        {
            MoveWaypoint(68, GetObjectX(self), GetObjectY(self));
            ptr = CreateObject("Wizard", 68);
            SetOwner(self, ptr);
            CastSpellObjectLocation("SPELL_TOXIC_CLOUD", ptr, GetObjectX(other), GetObjectY(other));
            Delete(ptr);
            CastSpellObjectObject("SPELL_PIXIE_SWARM", self, self);
            RunAway(self, other, 900);
            Enchant(self, "ENCHANT_CROWN", 3.0);
        }
        else if (rnd == 4)
        {
            CastSpellObjectObject("SPELL_PIXIE_SWARM", self, self);
            CastSpellObjectObject("SPELL_SLOW", self, other);
            PauseObject(self, 60);
            FrameTimerWithArg(30, GetTrigger(), GuardianCastTeleport);
        }
        else
        {
            //HitFarLocation(self, GetObjectX(other), GetObjectY(other));
            AbsoluteTargetStrike(GetTrigger(), GetCaller(), 85.0, GetShootXBowFunc());
            CastSpellObjectObject("SPELL_SLOW", self, other);
            Enchant(self, "ENCHANT_CROWN", 2.0);
        }
    }
}

void ForceTeleport(int unit)
{
    int wp;
    if (CurrentHealth(unit))
    {
        Effect("TELEPORT", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
        MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
        AudioEvent("BlinkCast", 1);
        if (IsObjectOn(unit))
            wp = Waypoint("respawnLocation" + IntToString(Random(1, 18)));
        else
            wp = 92;
        MoveObject(unit, GetWaypointX(wp), GetWaypointY(wp));
        Effect("TELEPORT", GetWaypointX(wp), GetWaypointY(wp), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetWaypointX(wp), GetWaypointY(wp), 0.0, 0.0);
        AudioEvent("BlinkCast", wp);
    }
}

void teleportRandomWarp(int arg_0)
{
    int var_0;
    
    if (CurrentHealth(arg_0) && !HasEnchant(arg_0, "ENCHANT_ANCHORED") && !HasEnchant(arg_0, "ENCHANT_BURNING"))
    {
        Effect("TELEPORT", GetObjectX(arg_0), GetObjectY(arg_0), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(arg_0), GetObjectY(arg_0), 0.0, 0.0);
        MoveWaypoint(1, GetObjectX(arg_0), GetObjectY(arg_0));
        AudioEvent("BlinkCast", 1);
        var_0 = Waypoint("respawnLocation" + IntToString(Random(1, 18)));
        MoveObject(arg_0, GetWaypointX(var_0), GetWaypointY(var_0));
        Effect("TELEPORT", GetWaypointX(var_0), GetWaypointY(var_0), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetWaypointX(var_0), GetWaypointY(var_0), 0.0, 0.0);
        AudioEvent("BlinkCast", var_0);
    }
}

void GuardianCastTeleport(int unit)
{
    int dest;
    if (CurrentHealth(unit) && IsObjectOn(unit))
    {
        dest = Waypoint("respawnLocation" + IntToString(Random(1, 18)));
        MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
        MoveObject(unit, GetWaypointX(dest), GetWaypointY(dest));
        AudioEvent("BlinkCast", 1);
        SetOwner(unit, TeleportPortalDuration(1, unit, 24) + 1);
    }
}

int TeleportPortalDuration(int srcWp, int destU, int dur)
{
    int unit = CreateObject("Bomber", srcWp);

    DeleteObjectTimer(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(destU), GetObjectY(destU)), dur + 10);
    UnitNoCollide(CreateObjectAt("TeleportWake", GetObjectX(unit), GetObjectY(unit)));
    DeleteObjectTimer(unit + 2, dur);
    DeleteObjectTimer(unit, dur);
    ObjectOff(unit);
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    Frozen(unit, 1);
    SetCallback(unit, 9, UnitPortalEnter);
    Effect("TELEPORT", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
    Effect("TELEPORT", GetObjectX(unit + 1), GetObjectY(unit + 1), 0.0, 0.0);
    Effect("SMOKE_BLAST", GetObjectX(unit), GetObjectY(unit), 0.0, 0.0);
    Effect("SMOKE_BLAST", GetObjectX(unit + 1), GetObjectY(unit + 1), 0.0, 0.0);

    return unit;
}

void UnitPortalEnter()
{
    if (CurrentHealth(other) && IsObjectOn(other))
    {
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        MoveObject(other, GetObjectX(GetTrigger() + 1), GetObjectY(GetTrigger() + 1));
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        UnitPortalEntryHandler();
    }
}

void UnitPortalEntryHandler()
{
    int owner = GetOwner(GetTrigger() + 1), unit;

    if (CurrentHealth(owner) && CurrentHealth(other) && IsAttackedBy(other, owner) && IsObjectOn(owner))
    {
        AbsoluteTargetStrike(owner, GetCaller(), 85.0, GetShootXBowFunc());
        ConjShotFireball(owner, GetCaller());
        /*unit = CreateObjectAt("ArcherBolt", GetObjectX(other), GetObjectY(other));
        LookWithAngle(unit, GetDirection(other));
        SetOwner(owner, unit);*/
        if (!HasEnchant(owner, "ENCHANT_TELEKINESIS"))
        {
            Enchant(owner, "ENCHANT_TELEKINESIS", 2.0);
            if (HasEnchant(owner, "ENCHANT_ANTI_MAGIC"))
                AbsoluteTargetStrike(owner, other, 85.0, GetShootXBowFunc());
            else
            {
                AbsoluteTargetStrike(owner, GetCaller(), 10.5, GetFistShotFunc());
                FrameTimerWithArg(Random(12, 21), owner, GuardianCastTeleport);
            }
        }
    }
}

void ConjShotFireball(int owner, int target)
{
    int unit;

    if (HasEnchant(owner, "ENCHANT_ETHEREAL"))
        return;
    else
    {
        Enchant(owner, "ENCHANT_ETHEREAL", 3.0);
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
        SetOwner(owner, unit);
        Raise(unit, ToFloat(target));
        FrameTimerWithArg(1, unit, ShotFireballHandler);
    }
}

void ShotFireballHandler(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), count = GetDirection(ptr);

    if (CurrentHealth(owner) && CurrentHealth(target) && count < 12)
    {
        if (IsVisibleTo(owner, target))
        {
            MoveWaypoint(1, GetObjectX(owner), GetObjectY(owner));
            AudioEvent("FireballWand", 1);
            LookAtObject(owner, target);
            if (count % 5 == 4)
                AbsoluteTargetStrike(owner, target, 85.0, GetShootXBowFunc());
            else
                AbsoluteTargetStrike(owner, target, 3.2, GetFireballShotFunc());
            LookWithAngle(ptr, count + 1);
        }
        else
            LookWithAngle(ptr, 100);
        FrameTimerWithArg(9, ptr, ShotFireballHandler);
    }
    else
        Delete(ptr);
}

void AlphagoShootXBow(int sUnit)
{
    int owner = GetOwner(sUnit), target = ToInt(GetObjectZ(sUnit)), mis;
    float dt = Distance(GetObjectX(sUnit), GetObjectY(sUnit), GetObjectX(target), GetObjectY(target));
    float thresHold;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        mis = EquipedXBow(owner, target, 19.0);
        thresHold = DistanceUnitToUnit(mis, target) / GetObjectZ(sUnit + 1);
        MoveObject(sUnit, GetObjectX(target) + UnitRatioX(target, sUnit, dt * thresHold), GetObjectY(target) + UnitRatioY(target, sUnit, dt * thresHold));
        if (IsVisibleTo(sUnit, owner))
        {
            PushObject(mis, -42.0, GetObjectX(sUnit), GetObjectY(sUnit));
        }
        else
        {
            PushObject(mis, -42.0, GetObjectX(target), GetObjectY(target));
        }
    }
    Delete(sUnit);
    Delete(sUnit + 1);
}

void PushObjectTarget(int unit, int target, float force)
{
    PushObjectTo(unit, UnitRatioX(target, unit, force), UnitRatioY(target, unit, force));
}

int EquipedXBow(int owner, int target, float gap)
{
    int mis = CreateObjectAt("ArcherBolt", GetObjectX(owner) + UnitRatioX(target, owner, gap), GetObjectY(owner) + UnitRatioY(target, owner, gap));
    int inv = GetQuiverByInventory(owner);

    if (inv)
        CopyQuiverPropertyData(mis, inv);
    LookAtObject(mis, target);
    SetOwner(owner, mis);
    Enchant(mis, "ENCHANT_VILLAIN", 0.0);

    return mis;
}

void CopyQuiverPropertyData(int dst, int src)
{
    int dstPtr = UnitToPtr(dst), srcPtr = UnitToPtr(src), temp, i;

    if (dstPtr && srcPtr)
    {
        temp = GetMemory(dstPtr + 0x2b4);
        if (temp)
        {
            SetMemory(temp, GetMemory(GetMemory(srcPtr + 0x2b4)));
            SetMemory(temp + 4, GetMemory(GetMemory(srcPtr + 0x2b4) + 4));
            SetMemory(temp + 8, GetMemory(GetMemory(srcPtr + 0x2b4) + 8));
            SetMemory(temp + 12, GetMemory(GetMemory(srcPtr + 0x2b4) + 12));
            for (i = 31 ; i >= 0 ; i -= 1)
                SetMemory(dstPtr + 0x230 + (i * 4), 0x200);
        }
    }
}

int GetQuiverByInventory(int unit)
{
    int inv = GetLastItem(unit);

    while (IsObjectOn(inv))
    {
        if (HasSubclass(inv, "QUIVER") && HasClass(inv, "WEAPON"))
            return inv;
        inv = GetPreviousItem(inv);
    }
    return 0;
}

void WarriorStrikeSubPart(int delay)
{
    int ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(self), GetObjectY(self));

    SetOwner(self, ptr);
    Raise(ptr, ToFloat(GetCaller()));
    FrameTimerWithArg(delay, ptr, WarriorTripleMissile);
}

void WarriorTripleMissile(int ptr)
{
    int caster = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), i;

    if (CurrentHealth(caster) && CurrentHealth(target))
    {
        LookWithAngle(ptr, GetDirection(caster) - 30);
        for (i = 0 ; i < 13 ; i += 1)
        {
            SpawnBullet(caster, GetObjectX(caster) + UnitAngleCos(ptr, 17.0), GetObjectY(caster) + UnitAngleSin(ptr, 17.0), 70, 33.0);
            LookWithAngle(ptr, GetDirection(ptr) + 5);
        }
    }
    Delete(ptr);
}

int SpawnBullet(int owner, float x, float y, int dam, float force)
{
    int unit = CreateObjectAt("OgreShuriken", x, y);
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2bc) + 4, dam);
    LookAtObject(unit, owner);
    LookWithAngle(unit, GetDirection(unit) + 128);
    PushObjectTo(unit, UnitRatioX(unit, owner, force), UnitRatioY(unit, owner, force));
    SetOwner(owner, unit);
    return unit;
}

void WarriorWarcry(int unit)
{
    if (!HasEnchant(unit, "ENCHANT_CROWN"))
    {
        MoveWaypoint(1, GetObjectX(unit), GetObjectY(unit));
        SplashHandler(unit, GetWarcryFxFunc(), GetWaypointX(1), GetWaypointY(1), 300.0);
        Enchant(unit, "ENCHANT_FREEZE", 0.8);
        Effect("JIGGLE", GetObjectX(unit), GetObjectY(unit), 23.0, 0.0);
        AudioEvent("WarcryInvoke", 1);
        Enchant(unit, "ENCHANT_CROWN", 5.0);
    }
}

int GetWarcryFxFunc()
{
    StopScript(WarcryEffect);
}

void WarcryEffect()
{
    int owner = GetOwner(GetOwner(self));

    if (CurrentHealth(owner))
    {
        Enchant(other, "ENCHANT_VILLAIN", 0.08);
        Damage(other, owner, 10, 14);
        if (HasClass(other, "PLAYER"))
            Enchant(other, "ENCHANT_ANTI_MAGIC", 5.0);
        else
            Enchant(other, "ENCHANT_HELD", 5.0);
    }
}

void HarpoonInRange(int owner, int target, int wait)
{
    int unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        if (!HasEnchant(owner, "ENCHANT_ETHEREAL"))
        {
            Enchant(owner, "ENCHANT_ETHEREAL", 0.0);
            unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
            SetOwner(owner, unit);
            Raise(unit, ToFloat(target));
            LookWithAngle(unit, wait);
            FrameTimerWithArg(1, unit, HarpoonWaitStrike);
        }
    }
}

void HarpoonWaitStrike(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));

    while (1)
    {
        if (IsObjectOn(ptr) && CurrentHealth(owner) && CurrentHealth(target) && IsObjectOn(owner))
        {
            if (count)
            {
                if (IsVisibleTo(owner, target) && Distance(GetObjectX(owner), GetObjectY(owner), GetObjectX(target), GetObjectY(target)) < 400.0)
                    WarPullEnemy(owner, target);
                else
                {
                    LookWithAngle(ptr, count - 1);
                    FrameTimerWithArg(6, ptr, HarpoonWaitStrike);
                    break;
                }
            }
        }
        if (CurrentHealth(owner))
            EnchantOff(owner, "ENCHANT_ETHEREAL");
        if (IsObjectOn(ptr))
            Delete(ptr);
        break;
    }
}

void BerserkerInRange(int owner, int target, int wait)
{
    int unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        if (!HasEnchant(owner, "ENCHANT_ETHEREAL"))
        {
            Enchant(owner, "ENCHANT_ETHEREAL", 0.0);
            unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
            SetOwner(owner, unit);
            Raise(unit, ToFloat(target));
            LookWithAngle(unit, wait);
            FrameTimerWithArg(1, unit, BerserkerWaitStrike);
        }
    }
}

void BerserkerWaitStrike(int ptr)
{
    int count = GetDirection(ptr), owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));

    while (1)
    {
        if (IsObjectOn(ptr) && CurrentHealth(owner) && CurrentHealth(target) && IsObjectOn(owner))
        {
            if (count)
            {
                if (IsVisibleTo(owner, target) && Distance(GetObjectX(owner), GetObjectY(owner), GetObjectX(target), GetObjectY(target)) < 400.0)
                    BerserkerCharge(owner, target);
                else
                {
                    LookWithAngle(ptr, count - 1);
                    FrameTimerWithArg(6, ptr, BerserkerWaitStrike);
                    break;
                }
            }
        }
        if (CurrentHealth(owner))
            EnchantOff(owner, "ENCHANT_ETHEREAL");
        if (IsObjectOn(ptr))
            Delete(ptr);
        break;
    }
}

void BerserkerCharge(int owner, int target)
{
    int unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        MoveWaypoint(1, GetObjectX(owner), GetObjectY(owner));
        AudioEvent("BerserkerChargeInvoke", 1);
        unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), UnitRatioX(target, owner, 23.0));
        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), UnitRatioY(target, owner, 23.0));
        SetOwner(owner, unit);
        LookWithAngle(GetLastItem(owner), 0);
        SetCallback(owner, 9, BerserkerTouched);
        Raise(unit, ToFloat(target));
        LookAtObject(unit + 1, target);
        FrameTimerWithArg(1, unit, BerserkerLoop);
    }
}

void BerserkerLoop(int ptr)
{
    int owner = GetOwner(ptr), count = GetDirection(ptr);

    if (CurrentHealth(owner) && count < 60 && IsObjectOn(ptr) && IsObjectOn(owner))
    {
        if (CheckUnitFrontSight(owner, GetObjectZ(ptr + 1) * 1.5, GetObjectZ(ptr + 2) * 1.5) && !GetDirection(GetLastItem(owner)))
        {
            MoveObject(owner, GetObjectX(owner) + GetObjectZ(ptr + 1), GetObjectY(owner) + GetObjectZ(ptr + 2));
            LookWithAngle(owner, GetDirection(ptr + 1));
            Walk(owner, GetObjectX(owner), GetObjectY(owner));
        }
        else
            LookWithAngle(ptr, 100);
        FrameTimerWithArg(1, ptr, BerserkerLoop);
    }
    else
    {
        SetCallback(owner, 9, WarNormalCollide);
        Delete(ptr);
        Delete(ptr + 1);
        Delete(ptr + 2);
    }
}

void BerserkerTouched()
{
    if (IsObjectOn(self))
    {
        while (1)
        {
            if (!GetCaller() || (HasClass(other, "IMMOBILE") && !HasClass(other, "DOOR")))
            {
                Enchant(self, "ENCHANT_HELD", 1.0);
                AroundWallDestroy(self);
            }
            else if (CurrentHealth(other))
            {
                GreenSparkFx(GetObjectX(self), GetObjectY(self));
                if (IsAttackedBy(self, other))
                    Damage(other, self, 150, 2);
                else
                    break;
            }
            else
                break;
            LookWithAngle(GetLastItem(self), 1);
            break;
        }
    }
}

void WarNormalCollide()
{
    if (!GetCaller())
        AroundWallDestroy(self);
}

void ConHarpoonCollide()
{
    int target;

    if (GetCaller())
    {
        if (!HasEnchant(self, "ENCHANT_PROTECT_FROM_MAGIC") && IsObjectOn(self))
        {
            if (GetUnitThingID(other) == 526)
            {
                target = GetOwner(other);
                Enchant(self, "ENCHANT_PROTECT_FROM_MAGIC", 2.0);
                if (HasEnchant(self, "ENCHANT_ANTI_MAGIC"))
                {
                    LookAtObject(self, target);
                    AbsoluteTargetStrike(GetTrigger(), target, 9.0, GetMeteorShotFunc());
                    AbsoluteTargetStrike(GetTrigger(), target, 85.0, GetShootXBowFunc());
                }
                else
                {
                    if (DistanceUnitToUnit(target, self) < 300.0)
                    {
                        LookAtObject(self, target);
                        AbsoluteTargetStrike(GetTrigger(), target, 9.0, GetMeteorShotFunc());
                        AbsoluteTargetStrike(GetTrigger(), target, 85.0, GetShootXBowFunc());
                        CastSpellObjectLocation("SPELL_BURN", self, GetObjectX(target), GetObjectY(target));
                    }
                    else
                    {
                        AbsoluteTargetStrike(GetTrigger(), target, 10.5, GetFistShotFunc());
                        FrameTimerWithArg(30, GetTrigger(), GuardianCastTeleport);
                    }
                }
            }
        }
    }
    else
        AroundWallDestroy(self);
}

void releaseKnight(int arg_0)
{
    int wp;
    
    UniPrintToAll("* " + IntToString(arg_0) + " 번 가디언이 맵에 투입됩니다");
    
    wp = Waypoint("respawnLocation" + IntToString(Random(1, 18)));
    MoveObject(NPCS[arg_0], GetWaypointX(wp), GetWaypointY(wp));
    ObjectOn(NPCS[arg_0]);
    if (!HasSubclass(GetLastItem(NPCS[arg_0]), "HAMMER"))
    {
        Enchant(NPCS[arg_0], "ENCHANT_SNEAK", 0.0);
        Enchant(NPCS[arg_0], "ENCHANT_VAMPIRISM", 0.0);
    }
    RegistQueue(NPCS[arg_0]);
    Enchant(NPCS[arg_0], "ENCHANT_PROTECT_FROM_POISON", 0.0);
    Enchant(NPCS[arg_0], "ENCHANT_INFRAVISION", 0.0);
    Effect("TELEPORT", GetWaypointX(wp), GetWaypointY(wp), 0.0, 0.0);
    AudioEvent("TeleportOut", wp);
    EnchantOff(NPCS[arg_0], "ENCHANT_FREEZE");
    EnchantOff(NPCS[arg_0], "ENCHANT_BURNING");
}

int FindOutQuiver(int unit)
{
    int inv = GetLastItem(unit);

    while (IsObjectOn(inv))
    {
        if (HasClass(inv, "WEAPON") && HasSubclass(inv, "QUIVER"))
            return inv;
        inv = GetPreviousItem(inv);
    }
    return 0;
}

void QuiverSetAllImpactPropert(int quiv)
{
    int ptr = UnitToPtr(quiv);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2b4) + 8, GetMemory(0x5BA444));
        SetMemory(GetMemory(ptr + 0x2b4) + 12, GetMemory(0x005BA24C));
    }
}

void CheckImpactQuiver(int unit)
{
    int quiv = FindOutQuiver(unit);

    if (IsObjectOn(quiv))
    {
        QuiverSetAllImpactPropert(quiv);
    }
}

void initializeKnightHammers(int num)
{
    float temp;

    if (num < GUARD_COUNT)
    {
        NPCS[num] = Object("arenaNPC" + IntToString(num + 1));
        ObjectOff(NPCS[num]);
        temp = IntToFloat(num * 23);
        MoveObject(NPCS[num], GetWaypointX(47) - temp, GetWaypointY(47) + temp);
        Enchant(NPCS[num], "ENCHANT_FREEZE", 0.0);
        Enchant(NPCS[num], "ENCHANT_BURNING", 0.0);
        if (num >= 20)
        {
            SetCallback(NPCS[num], 9, ConHarpoonCollide);
            SetUnitScanRange(NPCS[num], 600.0);
            //CheckImpactQuiver(NPCS[num]); //TODO: Remove it. just test
        }
        else
            SetCallback(NPCS[num], 9, WarNormalCollide);
        LookWithAngle(GetLastItem(NPCS[num]), num);
        nonCrashInventoryOfKnight(NPCS[num]);
        FrameTimerWithArg(1, num + 1, initializeKnightHammers);
    }
    else
    {
        FrameTimerWithArg(120, PopRndIndex(), releaseKnight);
        FrameTimerWithArg(140, PopRndIndex(), releaseKnight);
        FrameTimerWithArg(170, PopRndIndex(), releaseKnight);
    }
}

void nonCrashInventoryOfKnight(int unit)
{
    int inv = GetLastItem(unit);

    while (IsObjectOn(inv))
    {
        if (!HasEnchant(inv, "ENCHANT_INVULNERABLE"))
            Enchant(inv, "ENCHANT_INVULNERABLE", 0.0);
        inv = GetPreviousItem(inv);
    }
}

void loopGuardianAi()
{
    int i, k, plr;

    for (i = 0 ; i < 5; i += 1)
    {
        if (CurrentHealth(AIKnight[i]))
        {
            plr = SearchTargetPlayer(AIKnight[i]);
            if (plr >= 0)
            {
                if (Distance(GetObjectX(AIKnight[i]), GetObjectY(AIKnight[i]), GetObjectX(player[plr]), GetObjectY(player[plr])) < 200.0 && HasEnchant(AIKnight[i], "ENCHANT_SNEAK") && !HasEnchant(AIKnight[i], "ENCHANT_VILLAIN"))
                {
                    Enchant(AIKnight[i], "ENCHANT_VILLAIN", 15.0);
                    GuardianCastTeleport(AIKnight[i]);
                }
                AroundWallDestroy(AIKnight[i]);
                CreatureFollow(AIKnight[i], player[plr]);
                AggressionLevel(AIKnight[i], 1.0);
            }
        }
    }
    if (!ggOver)
        FrameTimer(60, loopGuardianAi);
}

int SearchTargetPlayer(int unit)
{
    int k, target = -1;
    float temp = 8192.0, rad;

    for (k = 9 ; k >= 0 ; k -= 1)
    {
        if (CurrentHealth(player[k]))
        {
            rad = Distance(GetObjectX(unit), GetObjectY(unit), GetObjectX(player[k]), GetObjectY(player[k]));
            if (rad < temp)
            {
                temp = rad;
                target = k;
            }
        }
    }
    return target;
}

void loopGuardianStat()
{
    int i;

    for (i = 4 ; i >= 0 ; i -= 1)
    {
        if (CurrentHealth(AIKnight[i]))
        {
            if (GetDirection(GetLastItem(AIKnight[i])) < 20)
            {
                if (!HasSubclass(GetLastItem(AIKnight[i]), "HAMMER"))
                {
                    violateRule();
                    break;
                }
            }
            else
            {
                if (!HasSubclass(ExportQuiver(AIKnight[i]), "QUIVER"))
                {
                    violateRule();
                    break;
                }
            }
        }
    }
    if (!ggOver)
        FrameTimer(2, loopGuardianStat);
}

int ExportQuiver(int unit)
{
    int inv = GetLastItem(unit), i;

    for (i = 0 ; i < 6 && IsObjectOn(inv) ; i += 1)
        inv = GetPreviousItem(inv);
    return inv;
}

void violateRule()
{
    UniPrintToAll("펌블 쓰지말랬지-!!");
    ObjectOff(Object("mainSwitch"));
    teleportAllPlayer(75);
}

void getAutoPlaced(int arg_0)
{
    int idx = GetDirection(GetLastItem(arg_0));
    float place = IntToFloat(idx * 23);

    MoveObject(arg_0, GetWaypointX(47) - place, GetWaypointY(47) + place);
    NPCS[idx + 40] = 0;
}

void DisplayScore()
{
    UniChatMessage(GetMaster(), "스코어 보드판\n인간팀 킬수:\t" + IntToString(HumanScore) + "킬\n알파고팀 킬수:\t" + IntToString(BotScore) + "킬", 60);
    SecondTimer(1, DisplayScore);
}

string randomSummonUnit()
{
    string table[] = {"SUMMON_GHOST", "SUMMON_EMBER_DEMON", "SUMMON_EVIL_CHERUB", "SUMMON_MECHANICAL_FLYER", "FIST"};

    return "SPELL_" + table[Random(0, 4)];
}

int GetMaster()
{
    int unit;

    if (!unit)
    {
        unit = CreateObject("Hecubah", 73);
        Frozen(unit, 1);
    }
    return unit;
}

void strVictory()
{
	int arr[38], i;
	string name = "DrainManaOrb";
	
	arr[0] = 2082988096; arr[1] = 1132584967; arr[2] = 2021689456; arr[3] = 2101523; arr[4] = 131108; arr[5] = 1140867072; arr[6] = 143132800; arr[7] = 2115109080; arr[8] = 670301187; arr[9] = 71320830; 
	arr[10] = 25364616; arr[11] = 805306898; arr[12] = 773070848; arr[13] = 71566400; arr[14] = 2139748352; arr[15] = 283233795; arr[16] = 35660322; arr[17] = 83870788; arr[18] = 8; arr[19] = 286355728; 
	arr[20] = 36569632; arr[21] = 2139358208; arr[22] = 143261568; arr[23] = 17829902; arr[24] = 35619362; arr[25] = 134234116; arr[26] = 142674056; arr[27] = 536936720; arr[28] = 1069679136; arr[29] = 71597952; 
	arr[30] = 8915199; arr[31] = 2131763473; arr[32] = 536870978; arr[33] = 2084602936; arr[34] = 1619574667; arr[35] = 534839311; arr[36] = 1107304448; arr[37] = 1074004095; 
	while (i < 38)
	{
		drawstrVictory(arr[i], name);
		i += 1;
	}
}

void drawstrVictory(int arg_0, string name)
{
	int count, i;
	float pos_x, pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(76);
		pos_y = GetWaypointY(76);
	}
	for (i = 1 ; i > 0 && count < 1178 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 76);
		if (count % 108 == 107)
			MoveWaypoint(76, GetWaypointX(76) - 214.000000, GetWaypointY(76) + 2.000000);
		else
			MoveWaypoint(76, GetWaypointX(76) + 2.000000, GetWaypointY(76));
		count += 1;
	}
	if (count >= 1178)
	{
		count = 0;
		MoveWaypoint(76, pos_x, pos_y);
	}
}

string PlayerName(int ptr)
{
    int addr = GetMemory(0x97bb40), xwis_id = GetMemory(GetMemory(ptr + 0x2ec) + 0x114) + 0x830;

    SetMemory(addr, GetMemory(xwis_id));
    SetMemory(addr + 4, GetMemory(xwis_id + 4));
    SetMemory(addr + 8, GetMemory(xwis_id + 8));
	StopScript(0);
}

void AbsoluteTargetStrike(int owner, int target, float threshold, int func)
{
    int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(target), GetObjectY(target));
    Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(owner), GetObjectY(owner)), threshold);

    SetOwner(owner, unit);
    Raise(unit, ToFloat(target));
    FrameTimerWithArg(1, unit, func);
}

void FistShot(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));
    float dt = Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(target), GetObjectY(target));
    float vectX = UnitRatioX(target, ptr, dt), vectY = UnitRatioY(target, ptr, dt);

    if (CurrentHealth(owner) && CurrentHealth(target) && IsObjectOn(owner))
    {
        MoveObject(ptr, GetObjectX(target) + (vectX * GetObjectZ(ptr + 1)), GetObjectY(target) + (vectY * GetObjectZ(ptr + 1)));
        if (IsVisibleTo(ptr, owner))
            CastSpellObjectLocation("SPELL_FIST", owner, GetObjectX(ptr), GetObjectY(ptr));
        else
            CastSpellObjectObject("SPELL_FIST", owner, target);
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void MeteorShot(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));
    float dt = Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(target), GetObjectY(target));
    float vectX = UnitRatioX(target, ptr, dt), vectY = UnitRatioY(target, ptr, dt);

    if (CurrentHealth(owner) && CurrentHealth(target) && IsObjectOn(owner))
    {
        MoveObject(ptr, GetObjectX(target) + (vectX * GetObjectZ(ptr + 1)), GetObjectY(target) + (vectY * GetObjectZ(ptr + 1)));
        if (IsVisibleTo(ptr, owner))
            CastSpellObjectLocation("SPELL_METEOR", owner, GetObjectX(ptr), GetObjectY(ptr));
        else
            CastSpellObjectObject("SPELL_METEOR", owner, target);
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void ConjFireballShot(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), unit;
    float dt = Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(target), GetObjectY(target));
    float vectX = UnitRatioX(target, ptr, dt), vectY = UnitRatioY(target, ptr, dt);

    if (CurrentHealth(owner) && CurrentHealth(target) && IsObjectOn(owner))
    {
        unit = CreateObjectAt("Fireball", GetObjectX(owner) + UnitRatioX(target, owner, 32.0), GetObjectY(owner) + UnitRatioY(target, owner, 32.0));
        SetOwner(owner, unit);
        MoveObject(ptr, GetObjectX(target) + (vectX * GetObjectZ(ptr + 1)), GetObjectY(target) + (vectY * GetObjectZ(ptr + 1)));
        if (IsVisibleTo(ptr, owner))
        {
            PushObjectTarget(unit, ptr, 35.0);
            LookAtObject(unit, ptr);
        }
        else
        {
            PushObjectTarget(unit, target, 35.0);
            LookAtObject(unit, target);
        }
    }
    Delete(ptr);
    Delete(ptr + 1);
}

void PutBerserkerText(int unit)
{
    if (CurrentHealth(unit))
    {
        if (IsObjectOn(unit))
        {
            MoveWaypoint(1, GetObjectX(unit) - 60.0, GetObjectY(unit) + 30.0);
            FrameTimerWithArg(60, BerserkerText(), RemoveText);
        }
    }
}

int BerserkerText()
{
    int unit = CreateObject("RedPotion", 1) + 1;

    StrBerserkerCharge();
    LookWithAngle(CreateObject("InvisibleLightBlueLow", 1), 1);
    Delete(unit - 1);
    return unit;
}

int CheckDotEnd(int dotUnit)
{
    return GetDirection(dotUnit);
}

void RemoveText(int dotprintStart)
{
    int loop = true, dots = dotprintStart;

    while (loop)
    {
        if (CheckDotEnd(dots)) loop = false;
        Delete(dots);
        dots += 1;
    }
}

void StrBerserkerCharge()
{
	int arr[24], i, count = 0;
	string name = "HealOrb";
	float pos_x = GetWaypointX(1), pos_y = GetWaypointY(1);

	arr[0] = 2117067330; arr[1] = 1174161156; arr[2] = 138449032; arr[3] = 135266592; arr[4] = 270803490; arr[5] = 100419588; arr[6] = 151554180; arr[7] = 135397640; 
	arr[8] = 2116165890; arr[9] = 133986823; arr[10] = 143147140; arr[11] = 269566241; arr[12] = 573104386; arr[13] = 72370244; arr[14] = 168337544; arr[15] = 571609361; 
	arr[16] = 1112023298; arr[17] = 75759688; arr[18] = 554180753; arr[19] = 1058083361; arr[20] = 570828834; arr[21] = 71631936; arr[22] = 524416; arr[23] = 16781313; 
	
	for (i = 0 ; i < 24 ; i += 1)
		count = DrawStrBerserkerCharge(arr[i], name, count);
	MoveWaypoint(1, pos_x, pos_y);
}

int DrawStrBerserkerCharge(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 744 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 1);
		if (count % 68 == 67)
			MoveWaypoint(1, GetWaypointX(1) - 134.0, GetWaypointY(1) + 2.0);
		else
			MoveWaypoint(1, GetWaypointX(1) + 2.0, GetWaypointY(1));
		count ++;
	}
	return count;
}

void PutAlphagoDeathText(int unit)
{
    MoveWaypoint(1, GetObjectX(unit) - 60.0, GetObjectY(unit) + 30.0);
    FrameTimerWithArg(60, AlphagoDeathText(), RemoveText);
    Delete(unit);
}

int AlphagoDeathText()
{
    int unit = CreateObject("RedPotion", 1) + 1;

    StrAlphagoIsDead();
    LookWithAngle(CreateObject("InvisibleLightBlueLow", 1), 1);
    Delete(unit - 1);
    return unit;
}

void StrAlphagoIsDead()
{
	int arr[23], i, count = 0;
	string name = "HealOrb";
	float pos_x = GetWaypointX(1), pos_y = GetWaypointY(1);

	arr[0] = 2083512892; arr[1] = 1338253319; arr[2] = 16781840; arr[3] = 1109458976; arr[4] = 143888516; arr[5] = 285737216; arr[6] = 1149535268; arr[7] = 138414080; 
	arr[8] = 606324513; arr[9] = 1107313156; arr[10] = 553650680; arr[11] = 671224034; arr[12] = 267403292; arr[13] = 1074823442; arr[14] = 1073741858; arr[15] = 4458640; 
	arr[16] = 2081407266; arr[17] = 2114695; arr[18] = 545327376; arr[19] = 2145582592; arr[20] = 67653696; arr[21] = 1049086; arr[22] = 528613376; 
	for (i = 0 ; i < 23 ; i += 1)
		count = DrawStrAlphagoIsDead(arr[i], name, count);
	MoveWaypoint(1, pos_x, pos_y);
}

int DrawStrAlphagoIsDead(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 713 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 1);
		if (count % 65 == 64)
			MoveWaypoint(1, GetWaypointX(1) - 128.0, GetWaypointY(1) + 2.0);
		else
			MoveWaypoint(1, GetWaypointX(1) + 2.0, GetWaypointY(1));
		count ++;
	}
	return count;
}

void StrDefeatTeam()
{
	int arr[16], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(1), pos_y = GetWaypointY(1);

	arr[0] = 2116362942; arr[1] = 609223748; arr[2] = 1351123081; arr[3] = 334571796; arr[4] = 623023138; arr[5] = 1384399876; arr[6] = 142641528; arr[7] = 1911102625; 
	arr[8] = 580133423; arr[9] = 71303242; arr[10] = 2131314853; arr[11] = 689217537; arr[12] = 1042423874; arr[13] = 1141394373; arr[14] = 151035912; arr[15] = 510; 
	
	for (i = 0 ; i < 16 ; i ++)
		count = DrawStrDefeatTeam(arr[i], name, count);
	MoveWaypoint(1, pos_x, pos_y);
}

int DrawStrDefeatTeam(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 496 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 1);
		if (count % 44 == 43)
			MoveWaypoint(1, GetWaypointX(1) - 86.0, GetWaypointY(1) + 2.0);
		else
			MoveWaypoint(1, GetWaypointX(1) + 2.0, GetWaypointY(1));
		count ++;
	}
	return count;
}

void LinkBear2BinScript(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, Bear2BinTable());
}

void BerserkerNoDelayCore(int plr)
{
    int arr[10];

    if (!MaxHealth(arr[plr]))
    {
        arr[plr] = CreateObject("Bear2", 81 + plr);
        LinkBear2BinScript(arr[plr]);
        CreateObjectAt("Rat", GetObjectX(arr[plr]), GetObjectY(arr[plr]) + 23.0);
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

int GetShootXBowFunc()
{
    StopScript(AlphagoShootXBow);
}

int GetFistShotFunc()
{
    StopScript(FistShot);
}

int GetMeteorShotFunc()
{
    StopScript(MeteorShot);
}

int GetFireballShotFunc()
{
    StopScript(ConjFireballShot);
}

void WarThrowSword(int owner, int target)
{
    int unit;

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        MoveWaypoint(1, GetObjectX(owner) + UnitRatioX(target, owner, 20.0), GetObjectY(owner) + UnitRatioY(target, owner, 20.0));
        Effect("SMOKE_BLAST", GetWaypointX(1), GetWaypointY(1), 0.0, 0.0);
        AudioEvent("HarpoonInvoke", 1);
        AudioEvent("PowderBarrelExplode", 1);
        unit = CreateObject("InvisibleLightBlueHigh", 1);
        SetOwner(owner, unit);
        Raise(unit, ToFloat(target));
        FrameTimerWithArg(1, unit, FlyingSword);
    }
}

void FlyingSword(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), count = GetDirection(ptr), unit;

    if (CurrentHealth(owner) && CurrentHealth(target) && count < 19)
    {
        while (true)
        {
            if (CheckSeeUnit(ptr, target))
            {
                MoveObject(ptr, GetObjectX(ptr) + UnitRatioX(target, ptr, 21.0), GetObjectY(ptr) + UnitRatioY(target, ptr, 21.0));
                if (DistanceUnitToUnit(ptr, target) > 50.0)
                {
                    unit = CreateObjectAt("HarpoonBolt", GetObjectX(ptr), GetObjectY(ptr));
                    LookAtObject(unit, target);
                    Frozen(unit, 1);
                    DeleteObjectTimer(unit, 9);
                    LookWithAngle(ptr, count + 1);
                    break;
                }
                else
                {
                    MoveWaypoint(1, GetObjectX(ptr), GetObjectY(ptr));
                    AudioEvent("HarpoonReel", 1);
                    AudioEvent("StoneHitMetal", 1);
                    GreenSparkFx(GetWaypointX(1), GetWaypointY(1));
                    WarPullEnemy(owner, target);
                }
            }
            LookWithAngle(ptr, 100);
            break;
        }
        FrameTimerWithArg(1, ptr, FlyingSword);
    }
    else
    {
        Delete(ptr);
    }
}

void WarPullEnemy(int owner, int target)
{
    int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(target), GetObjectY(target));

    ObjectOff(CreateObjectAt("Maiden", GetObjectX(target), GetObjectY(target)));
    CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner));
    Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(target), GetObjectY(target)), CurrentHealth(owner));
    Damage(unit + 1, 0, MaxHealth(unit + 1) + 1, -1);
    Frozen(unit + 1, 1);
    SetCallback(unit + 1, 9, PullRiskUnit);
    SetOwner(owner, unit);
    Raise(unit, ToFloat(target));
    PullUp(unit);
}

void PullUp(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr)), count = GetDirection(ptr);

    if (CurrentHealth(owner) && CurrentHealth(target) && count < 20 && IsObjectOn(owner))
    {
        if (IsVisibleTo(owner, target) && DistanceUnitToUnit(owner, target) > 60.0)
        {
            MoveObject(ptr + 1, GetObjectX(target) + UnitRatioX(target, owner, 12.0), GetObjectY(target) + UnitRatioY(target, owner, 12.0));
            Effect("SENTRY_RAY", GetObjectX(owner), GetObjectY(owner), GetObjectX(target), GetObjectY(target));
            count += CheckCutPulling(owner, ptr + 3);
            LookWithAngle(ptr, count + 1);
        }
        else
            LookWithAngle(ptr, 100);
        FrameTimerWithArg(1, ptr, PullUp);
    }
    else
    {
        Delete(ptr);
        Frozen(ptr + 1, 0);
        Delete(ptr + 1);
        Delete(ptr + 2);
        Delete(ptr + 3);
    }
}

int CheckCutPulling(int owner, int ptr)
{
    int tempHp = ToInt(GetObjectZ(ptr)), curHp = CurrentHealth(owner);

    if (tempHp ^ curHp)
    {
        if (curHp < tempHp)
            return 100;
        Raise(ptr, ToFloat(curHp));
    }
    return 0;
}

void PullRiskUnit()
{
    int owner = GetOwner(GetTrigger() - 1), target = ToInt(GetObjectZ(GetTrigger() - 1));
    int count = GetDirection(GetTrigger() + 1);

    if (IsCaller(target))
    {
        if (!count)
        {
            if (CurrentHealth(owner) && CurrentHealth(target))
            {
                MoveWaypoint(1, GetObjectX(target), GetObjectY(target));
                AudioEvent("SentryRayHit", 1);
                Effect("VIOLET_SPARKS", GetObjectX(target), GetObjectY(target), 0.0, 0.0);
                Damage(target, owner, 15, 14);
            }
        }
        LookWithAngle(GetTrigger() + 1, (count + 1) % 7);
    }
}

int IsConjurer(int unit)
{
    return HasEnchant(unit, "ENCHANT_SNEAK");
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

void MapInitialize()
{
    MusicEvent();

    RegistSignMessage(Object("VictoryPic"), "명예의 전당:: 고된 승부 끝에 값진 승리를 손에 거머쥔 자만이 이곳에 오게 되어있지");
    RegistSignMessage(Object("DefeatPic"), "패배자의 지옥:: 헤헤.. 뜨겁지? 그러니 어서 빨리 다시 도전해 보라구!");
    FrameTimer(30, PlayerClassOnLoop);
    FrameTimer(3, InitRTable);
    //FrameTimer(60, loopGuardianStat); --remove 25 nov 2019
    FrameTimer(60, loopGuardianAi);
    FrameTimer(190, startMent);
    FrameTimerWithArg(1, 0, initializeKnightHammers);
    FrameTimer(1, MakeCoopTeam);
    FrameTimer(3, SetSpellRule);
}

int CheckUnitFrontSight(int unit, float dtX, float dtY)
{
    int temp = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit) + dtX, GetObjectY(unit) + dtY);
    int res = IsVisibleTo(unit, temp);

    Delete(temp);
    return res;
}

void GreenSparkFx(float x, float y)
{
    int ptr = CreateObjectAt("MonsterGenerator", x, y);

    Damage(ptr, 0, 10, 100);
    Delete(ptr);
}

int CheckSeeUnit(int unit1, int unit2)
{
    return (IsVisibleTo(unit1, unit2) || IsVisibleTo(unit2, unit1));
}

void CheckMagicElement(int cur)
{
    int ptr = UnitToPtr(cur);

    if (ptr)
    {
        if (GetMemory(GetMemory(ptr + 0x2ec) + 0x0c) == 33)
        {
            SetMemory(GetMemory(ptr + 0x2ec) + 0x0c, 0x4a);
            Enchant(cur, "ENCHANT_RUN", 0.0);
        }
    }
}

void DetectedSpecificIndex(int curId)
{
    int thingId = GetUnitThingID(curId);

    if (thingId == 531)
        AIBotShootBowHandler(GetOwner(curId), curId);
    else if (thingId == 955)     //@brief. ThrowingStone
        CheckMagicElement(curId);
    else if (thingId == 618)
        Delete(curId);
}

void LoopSearchIndex()
{
    int curId, tempId;
    
    while (GetMemory(0x750710))
    {
        tempId = GetMemory(GetMemory(0x750710) + 0x2c);
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

void AIBotShootBowHandler(int owner, int cur)
{
    if (HasClass(owner, "MONSTER"))
    {
        if (!HasEnchant(cur, "ENCHANT_VILLAIN") && CurrentHealth(owner))
        {
            AIBotDetectEnemy(owner);
            Delete(cur);
        }
    }
}

void AIBotDetectEnemy(int owner)
{
    int unit = CreateObjectAt("WeirdlingBeast", GetObjectX(owner), GetObjectY(owner));
    SetOwner(owner, CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner)));
    Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner), GetObjectY(owner)), 5000.0);
    LookWithAngle(unit, GetDirection(owner));
    CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit) + UnitAngleCos(unit, 200.0), GetObjectY(unit) + UnitAngleSin(unit, 200.0), 500.0);
    UnitNoCollide(unit);
    SetCallback(unit, 3, GetNealryEnemy);
    DeleteObjectTimer(unit, 1);
    FrameTimerWithArg(1, unit + 1, AIBotShootBow);
}

void GetNealryEnemy()
{
    int ptr = GetTrigger() + 1;
    int target = ToInt(GetObjectZ(ptr));
    float dist = DistanceUnitToUnit(self, other);

    if (dist < GetObjectZ(ptr + 1))
    {
        Raise(ptr + 1, dist);
        Raise(ptr, ToFloat(GetCaller()));
    }
}

void AIBotShootBow(int ptr)
{
    int owner = GetOwner(ptr), target = ToInt(GetObjectZ(ptr));

    if (CurrentHealth(owner) && CurrentHealth(target))
    {
        AbsoluteTargetStrike(owner, target, 85.0, GetShootXBowFunc());
    }
    Delete(ptr);
    Delete(ptr + 1);
}

int CheckWallByCoor(int xPos, int yPos)
{
    int spX, spY, tx, ty, rx;

    if (xPos > yPos) xPos += 23;
    else             yPos += 23;
    spX = (xPos + yPos - 22) / 46;
    spY = (xPos - yPos) / 46;
    tx = spX * 46;
    ty = spY * 46;
    rx = (tx + ty) / 2;
    return Wall(rx / 23, (rx - ty) / 23);
}

void AroundWallDestroy(int sUnit)
{
    int xProfile = FloatToInt(GetObjectX(sUnit)), yProfile = FloatToInt(GetObjectY(sUnit));

    WallBreak(CheckWallByCoor(xProfile, yProfile));
    WallBreak(CheckWallByCoor(xProfile - 23, yProfile - 23));
    WallBreak(CheckWallByCoor(xProfile + 23, yProfile - 23));
    WallBreak(CheckWallByCoor(xProfile - 23, yProfile + 23));
    WallBreak(CheckWallByCoor(xProfile + 23, yProfile + 23));
    WallBreak(CheckWallByCoor(xProfile - 23, yProfile));
    WallBreak(CheckWallByCoor(xProfile + 23, yProfile));
    WallBreak(CheckWallByCoor(xProfile, yProfile + 23));
    WallBreak(CheckWallByCoor(xProfile, yProfile - 23));
}

