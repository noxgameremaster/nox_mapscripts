
#include "builtins.h"
#include "libs\typecast.h"
#include "libs\unitutil.h"
#include "libs\callmethod.h"
#include "libs\fxeffect.h"
#include "libs\itemproperty.h"
#include "libs\playerupdate.h"
#include "libs\waypoint.h"
#include "libs\printutil.h"
#include "libs\username.h"
#include "libs\fixtellstory.h"
#include "libs\coopteam.h"
#include "libs\mathlab.h"
#include "libs\spellutil.h"
#include "libs\walls.h"
#include "libs\meleeattack.h"
#include "libs\unitstruct.h"
#include "libs\buff.h"

#define NULLPTR             0

int player[20];

#define DROP_ITEM_FUNCTION              7
int g_dropItemFunctions[DROP_ITEM_FUNCTION];
int g_userLastSavedLocation;

int g_blockLeftUp;
int g_blockLeftDown;
int g_blockRightLeft;
int g_blockRightRight;

int g_mixGenPuzzle[10];
int g_genPuzzleBeacon[9];
int g_puzzleGens = 0;

int g_ternnelSwitch;

int g_frontBlocks;
int g_backBlocks;

#define PLAYER_TELEPORT_LOCATION            12

#define SELF        -2
#define OTHER       -1

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
#define  ENCHANT_INVULNERABLE        23
#define  ENCHANT_TELEKINESIS         24
#define ENCHANT_FREEZE              25
#define ENCHANT_SHIELD              26
#define ENCHANT_REFLECTIVE_SHIELD   27
#define ENCHANT_CHARMING            28
#define ENCHANT_ANTI_MAGIC          29
#define ENCHANT_CROWN               30
#define ENCHANT_SNEAK               31


void MoveObjectVector(int unit, float xVect, float yVect)
{
    int *ptr = UnitToPtr(unit);

    if (ptr != NULLPTR)
    {
        float *coor = ptr;
        MoveObject(unit, coor[14] + xVect, coor[15] + yVect);
    }
}

void InitializeDungeonDropItemFunctions()
{
    g_dropItemFunctions[1] = RewardClassDropPotion;
    g_dropItemFunctions[2] = MoneyDrop;
    g_dropItemFunctions[3] = NormalWeaponItemDrop;
    g_dropItemFunctions[4] = NormalArmorItemDrop;
    g_dropItemFunctions[5] = WeaponItemDrop;
    g_dropItemFunctions[6] = ArmorItemDrop;
    g_dropItemFunctions[0] = RewardClassDropHotPotion;
}

void MapInitialize()
{
    MusicEvent();

    InitializeDungeonDropItemFunctions();
    InitFireTraps();
    InitMathSine(1);

    FrameTimer(3, MakeCoopTeam);
    FrameTimer(10, PlayerClassOnLoop);
    FrameTimer(30, DelayMapInit);
}

void MapExit()
{
    MusicEvent();
    RemoveCoopTeamMode();
    // SelfDamageClassMapExit();
    ResetPlayerHandlerWhenExitMap();
    ResetHostileCritter();
}

int CreateObjectAtUnit(string name, int sUnit)
{
    int *ptr = UnitToPtr(sUnit);
    float *coor = ptr;
    
    if (ptr != NULLPTR)
        return CreateObjectAt(name, coor[14], coor[15]);
    return 0;
}

int CheckPlayer()
{
    int i;

    for (i = 9 ; i >= 0 ; i--)
    {
        if (IsCaller(player[i]))
            return i;
    }
    return -1;
}

void PlayerAllEnchantment(int plrUnit)
{
    if (CurrentHealth(plrUnit))
    {
        Enchant(plrUnit, EnchantList(ENCHANT_VAMPIRISM), 0.0);
        Enchant(plrUnit, EnchantList(ENCHANT_PROTECT_FROM_FIRE), 0.0);
        Enchant(plrUnit, EnchantList(ENCHANT_PROTECT_FROM_POISON), 0.0);
        Enchant(plrUnit, EnchantList(ENCHANT_PROTECT_FROM_ELECTRICITY), 0.0);
        Enchant(plrUnit, EnchantList(ENCHANT_REFLECTIVE_SHIELD), 0.0);
    }
}

int PlayerClassOnInit(int plr, int plrUnit)
{
    int pResult = plr;

    player[plr] = plrUnit;
    player[plr + 10] = 1;
    ChangeGold(plrUnit, -GetGold(plrUnit));
    SelfDamageClassEntry(plrUnit);
    DiePlayerHandlerEntry(plrUnit);

    return pResult;
}

void PlayerClassOnJoin(int plr, int plrUnit)
{
    if (PlayerClassCheckAllEnchant(plr))
        PlayerAllEnchantment(plrUnit);
    MoveObject(plrUnit, LocationX(PLAYER_TELEPORT_LOCATION), LocationY(PLAYER_TELEPORT_LOCATION));
    PlaySoundAround(plrUnit, 1008);
}

void PlayerClassFailToJoin()
{
    MoveObject(other, LocationX(11), LocationY(11));
}

void RegistNewPlayer()
{
    int i, plr;

    while (true)
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
            if (plr >= 0)
            {
                PlayerClassOnJoin(plr, other);
                break;
            }
        }
        PlayerClassFailToJoin();
        break;
    }
}

void PlayerClassFastJoin()
{
    if (CurrentHealth(other))
    {
        int plr = CheckPlayer();
        if (plr < 0)
            MoveObject(other, LocationX(14), LocationY(14));
        else
        {
            MoveObject(other, LocationX(13), LocationY(13));
            UniPrint(other, "패스트 조인되었습니다");
        }
    }
}

void PlayerClassProc(int plr)
{
    int plrUnit = player[plr];

    if (UnitCheckEnchant(plrUnit, GetLShift(ENCHANT_SNEAK)))
    {
        EnchantOff(plrUnit, EnchantList(ENCHANT_SNEAK));
        WindBooster(plrUnit);
    }
}

#define PLAYER_FLAG_DEATH       0x80
#define PLAYER_FLAG_ALLBUFF     2

int PlayerClassCheckDeathFlag(int plr)
{
    return player[plr + 10] & PLAYER_FLAG_DEATH;
}

void PlayerClassSetDeathFlag(int plr)
{
    player[plr + 10] = player[plr + 10] ^ PLAYER_FLAG_DEATH;
}

void PlayerClassSetAllEnchant(int plr)
{
    player[plr + 10] = player[plr + 10] ^ PLAYER_FLAG_ALLBUFF;
}

int PlayerClassCheckAllEnchant(int plr)
{
    return player[plr + 10] & PLAYER_FLAG_ALLBUFF;
}

void PlayerClassOnDeath(int plr)
{
    UniPrintToAll(PlayerIngameNick(player[plr]) + " 님께서 적에게 격추되었습니다");
}

void PlayerClassOnExit(int plr)
{
    player[plr] = 0;
    player[plr + 10] = 0;
}

void PlayerClassOnLoop()
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        while (true)
        {
            if (MaxHealth(player[i]))
            {
                if (GetUnitFlags(player[i]) & 0x40)
                    1;
                else if (CurrentHealth(player[i]))
                {
                    PlayerClassProc(i);
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
                PlayerClassOnExit(i);
            break;
        }
    }
    FrameTimer(1, PlayerClassOnLoop);
}

string PotionList(int index)
{
    string list[] = {"RedPotion", "CurePoisonPotion", "YellowPotion", "BlackPotion",
        "VampirismPotion", "Mushroom", "PoisonProtectPotion", "ShockProtectPotion",
        "FireProtectPotion", "HastePotion", "ShieldPotion", "InfravisionPotion",
        "InvisibilityPotion", "AmuletofManipulation", "AmuletofManipulation", "AmuletofNature",
        "Fear", "WhitePotion", "BluePotion", "BottleCandle"};

    return list[index];
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

int RewardClassDropHotPotion(int sUnit)
{
    return CreateObjectAt("RedPotion", GetObjectX(sUnit), GetObjectY(sUnit));
}

void ItemUseClassTeleportAmulet()
{
    if (CurrentHealth(other))
    {
        Delete(self);
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);

        MoveObject(other, LocationX(PLAYER_TELEPORT_LOCATION), LocationY(PLAYER_TELEPORT_LOCATION));
        PlaySoundAround(other, 6);
        Effect("TELEPORT", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Effect("SMOKE_BLAST", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        UniPrint(other, "안전한 곳으로 공간이동 하였습니다");
    }
}

void ItemUseClassElectricAmulet()
{
    if (CurrentHealth(other))
    {
        if (CurrentHealth(other) ^ MaxHealth(other))
            RestoreHealth(other, 65);
        Enchant(other, EnchantList(27), 15.0);
        Enchant(other, EnchantList(22), 15.0);
        Delete(self);
    }
}

void ItemUseClassHealingPotion()
{
    if (CurrentHealth(other))
    {
        if (!UnitCheckEnchant(other, GetLShift(ENCHANT_CROWN)))
        {
            Enchant(other, EnchantList(ENCHANT_CROWN), 10.0);

            int healing = CreateObjectAtUnit("InvisibleLightBlueLow", other);
            SetOwner(other, healing);
            Enchant(healing, EnchantList(ENCHANT_RUN), 0.0);
            FrameTimerWithArg(1, healing, UnitHealingHandler);

            PlaySoundAround(other, 1004);
            UniPrint(other, "자연의 힘이 잠시동안 당신을 지속적으로 치료해 줍니다");
        }
        else
            UniPrint(other, "당신은 이미 이 능력의 영향권에 있습니다");
        Delete(self);
    }
}

void ItemUseClassCandle()
{
    if (CurrentHealth(other))
    {
        Delete(self);
        CastSpellObjectObject("SPELL_FORCE_OF_NATURE", other, other);
        UniPrint(other, "광화문 1000만 촛불의 힘을 보여주겠다!");
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

void TeleportAmulet(int amulet)
{
    int ptr = UnitToPtr(amulet);

    if (ptr)
    {
        SetMemory(ptr + 0x2dc, ImportUseItemFunc());
        SetMemory(ptr + 0x2fc, ItemUseClassTeleportAmulet);
    }
}

void ElectricAmulet(int amulet)
{
    int ptr = UnitToPtr(amulet);

    if (ptr)
    {
        SetMemory(ptr + 0x2dc, ImportUseItemFunc());
        SetMemory(ptr + 0x2fc, ItemUseClassElectricAmulet);
    }
}

void HealingPotion(int amulet)
{
    int ptr = UnitToPtr(amulet);

    if (ptr)
    {
        SetMemory(ptr + 0x2dc, ImportUseItemFunc());
        SetMemory(ptr + 0x2fc, ItemUseClassHealingPotion);
    }
}

void MagicCandle(int magicCandle)
{
    int ptr = UnitToPtr(magicCandle);

    if (ptr)
    {
        SetMemory(ptr + 0x2dc, ImportUseItemFunc());
        SetMemory(ptr + 0x2fc, ItemUseClassCandle);
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
    else if (thingID == 1197)
        MagicCandle(unit);

    if (x ^ unit) Delete(unit);
    return x;
}

int RewardClassDropPotion(int sUnit)
{
    return CheckPotionThingID(CreateObjectAtUnit(PotionList(Random(0, 19)), sUnit));
}

int MoneyDrop(int sUnit)
{
    int money = CreateObjectAt("Gold", GetObjectX(sUnit), GetObjectY(sUnit));
    SetMemory(GetMemory(GetMemory(0x750710) + 0x2b4), Random(200, 2000));
    return money;
}

int SomeGermDrop(int sUnit)
{
    string name[] = {"Ruby", "Ruby", "Ruby", "Emerald", "Diamond"};

    return CreateObjectAtUnit(name[Random(0, 2)], sUnit);
}

int NormalWeaponItemDrop(int sUnit)
{
    int weapon = CreateObjectAtUnit(WeaponList(Random(0, 7)), sUnit);
    int *ptr = 0x750710;

    CheckSpecialItem(ptr[0]);
    return weapon;
}

int NormalArmorItemDrop(int sUnit)
{
    return CreateObjectAtUnit(ArmorList(Random(0, 17)), sUnit);
}

int WeaponItemDrop(int sUnit)
{
    int unit = CreateObjectAtUnit(WeaponList(Random(0, 12)), sUnit);
    int *ptr = 0x750710;

    CheckSpecialItem(ptr[0]);
    SetWeaponProperties(unit, Random(0, 5), Random(0, 5), Random(0, 36), Random(0, 36));
    return unit;
}

int ArmorItemDrop(int sUnit)
{
    int unit = CreateObjectAtUnit(ArmorList(Random(0, 17)), sUnit);

    SetArmorProperties(unit, Random(0, 5), Random(0, 5), Random(0, 20), Random(0, 20));
    return unit;
}

void DungeonMonsterDeath()
{
    int maxCount = DROP_ITEM_FUNCTION - 1;

    CallFunctionWithArgInt(g_dropItemFunctions[Random(0, maxCount)], self);
    DeleteObjectTimer(self, 150);
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

void WindBooster(int plrUnit)
{
    if (UnitCheckEnchant(plrUnit, GetLShift(ENCHANT_SNEAK)))
    {
        EnchantOff(plrUnit, EnchantList(ENCHANT_SNEAK));
        RemoveTreadLightly(plrUnit);
        Enchant(plrUnit, EnchantList(ENCHANT_RUN), 0.3);
        PushObjectTo(plrUnit, UnitAngleCos(plrUnit, 70.0), UnitAngleSin(plrUnit, 70.0));
        Effect("RICOCHET", GetObjectX(plrUnit), GetObjectY(plrUnit), 0.0, 0.0);
    }
}

int PlaceMagicWeapon(int location)
{
    int weapon = CreateObjectAt("OblivionHeart", LocationX(location), LocationY(location));

    SetWeaponProperties(weapon, 5, 5, 8, 3);
    //Todo. 여기에서 마법 세팅을 한다

    return weapon;
}

int SummonMobSwordsman(int sUnit)
{
    int unit = CreateObjectAtUnit("Swordsman", sUnit);

    SetUnitMaxHealth(unit, 325);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    return unit;
}

int SummonMobArcher(int sUnit)
{
    int unit = CreateObjectAtUnit("Archer", sUnit);

    SetUnitMaxHealth(unit, 98);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    return unit;
}

int SummonMobMecaFlying(int sUnit)
{
    int unit = CreateObjectAtUnit("FlyingGolem", sUnit);

    SetUnitMaxHealth(unit, 64);
    return unit;
}

void MobMakerProc(int maker)
{
    int count = GetDirection(maker), mob;

    while (IsObjectOn(maker))
    {
        if (count)
        {
            FrameTimerWithArg(1, maker, MobMakerProc);
            LookWithAngle(maker, count - 1);
            mob = CallFunctionWithArgInt(ToInt(GetObjectZ(maker)), maker);
            if (CurrentHealth(mob))
                SetCallback(mob, 5, MobMakerMobDeath);
            break;
        }
        Delete(maker);
        break;
    }
}

int MobMakerCounter()
{
    int counter, nextFunction;

    return counter;
}

void SetNextFunction(int nextFunction)
{
    int ptr = GetScrDataField(MobMakerCounter);

    SetMemory(ptr + 4, nextFunction);
}

void MobMakerDecraseCounter()
{
    int counterPtr = GetScrDataField(MobMakerCounter);

    SetMemory(counterPtr, MobMakerCounter() - 1);
    if (!MobMakerCounter())
    {
        CallFunction(GetMemory(counterPtr + 4));
        SetNextFunction(EndAreaDefault);
    }
}

void MobMakerMobDeath()
{
    MobMakerDecraseCounter();
    DungeonMonsterDeath();
}

int PlaceMobMaker(int location, int count, object function)
{
    int maker = CreateObjectAt("ImaginaryCaster", LocationX(location), LocationY(location));
    int counter = MobMakerCounter();
    int counterPtr = GetScrDataField(MobMakerCounter);

    LookWithAngle(maker, count);
    Raise(maker, function);
    SetMemory(counterPtr, counter + count);
    return maker;
}

void MobMakerStartSummon(int delay, int maker)
{
    if (IsObjectOn(maker))
    {
        if (!delay)
            delay = 1;
        FrameTimerWithArg(delay, maker, MobMakerProc);
    }
}

void SaveLastLocation(int location)
{
    TeleportLocation(g_userLastSavedLocation, LocationX(location), LocationY(location));
}

void TeleportComplete(int point, int owner)
{
    Effect("SMOKE_BLAST", GetObjectX(point), GetObjectY(point), 0.0, 0.0);
    Effect("TELEPORT", GetObjectX(point), GetObjectY(point), 0.0, 0.0);
    MoveObject(owner, GetObjectX(point), GetObjectY(point));
    PlaySoundAround(owner, 6);
    Effect("TELEPORT", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
}

void TeleportProgress(int point)
{
    int owner = GetOwner(point), durate = GetDirection(point);

    while (IsObjectOn(point))
    {
        if (CurrentHealth(owner))
        {
            if (durate)
            {
                if (DistanceUnitToUnit(point + 1, owner) < 23.0)
                {
                    FrameTimerWithArg(1, point, TeleportProgress);
                    LookWithAngle(point, durate - 1);
                    break;
                }
            }
            else
                TeleportComplete(point, owner);
            EnchantOff(owner, EnchantList(ENCHANT_BURNING));
        }
        Delete(point);
        Delete(point + 1);
        break;
    }
}

void TeleportStart(int target, int destLocation)
{
    if (CurrentHealth(target))
    {
        if (!UnitCheckEnchant(target, GetLShift(ENCHANT_BURNING)))
        {
            Enchant(target, EnchantList(ENCHANT_BURNING), 4.0);
            int point = CreateObjectAt("ImaginaryCaster", LocationX(destLocation), LocationY(destLocation));
            CreateObjectAt("VortexSource", GetObjectX(target), GetObjectY(target));
            FrameTimerWithArg(1, point, TeleportProgress);
            SetOwner(target, point);
            LookWithAngle(point, 48);
            GreenSparkFx(GetObjectX(target), GetObjectY(target));
            PlaySoundAround(point + 1, 772);    //SoundFX: LongBellsUp
            if (IsPlayerUnit(target))
                UniPrint(target, "공간이동을 준비중입니다. 캐릭터를 움직이면 취소됩니다");
        }
    }
}

void EntryTeleportPortal()
{
    TeleportStart(OTHER, 51);
}

int PlaceLastSavedTeleport(object location)
{
    int telpo = CreateObjectAt("WeirdlingBeast", LocationX(location), LocationY(location));

    Enchant(CreateObjectAtUnit("InvisibleLightBlueLow", telpo), EnchantList(14), 0.0);
    Damage(telpo, 0, MaxHealth(telpo) + 1, -1);
    SetCallback(telpo, 9, EntryTeleportPortal);

    return telpo;
}

void DelayMapInit()
{
    g_userLastSavedLocation = 51;

    PlaceLastSavedTeleport(50);
    PlaceDefaultItems("RedPotion", 48);
    PlaceDefaultItems("RedPotion", 47);
    PlaceDefaultItems("GreatSword", 46);
    PlaceDefaultItems("RoundChakram", 45);
    ShopClassMagicalWeaponMarketCreate(49);
    ShopClassAllEnchantMarketCreate(67);
    ShopClassInvulnerabilityItemShopCreate(157);
    RegistSignMessage(Object("MapSign1"), "안녕하시오! 여행자. 이곳은 당신같은 사람이면 위험한 곳이지..");
    RegistSignMessage(Object("MapSign2"), "-이곳은 이마트 편의점의 입구입니다- 거래를 하려면 문을열고 안으로 오세요");
    RegistSignMessage(Object("MapSign3"), "특수 무기를 판매하고 있어요");
    RegistSignMessage(Object("MapSign4"), "이 워프게이트는 사용자가 마지막으로 탐험한 위치로 이동합니다");

    g_ternnelSwitch = Object("TernnelSwitch");
    ObjectOff(g_ternnelSwitch);
    Enchant(g_ternnelSwitch, EnchantList(0), 0.0);
    FrameTimer(30, LoopSearchIndex);
    FrameTimer(60, InitBlocks);
    FrameTimer(60, InitLastPart);
    FrameTimer(20, InitWeaponContainer);
    FrameTimer(1, InitializeGenerators);
    RegistUnitStrikeCallback(MonsterStrikeCallback);

    SetHostileCritter();
}

void EntryArea1()
{
    int once;

    ObjectOff(SELF);
    if (!once)
    {
        once = true;
        MobMakerStartSummon(0, PlaceMobMaker(15, 30, SummonMobSwordsman));
        MobMakerStartSummon(0, PlaceMobMaker(16, 30, SummonMobArcher));
        MobMakerStartSummon(0, PlaceMobMaker(17, 30, SummonMobMecaFlying));
        SetNextFunction(EndArea1);
    }
}

void EndAreaDefault()
{ }

void EndArea1()
{
    int i;

    for (i = 18 ; i <= 29 ; i += 1)
        RemoveWallAtObjectPos(i);
    FrameTimer(30, EntryArea2);
    UniPrintToAll("비밀의 벽이 열립니다");
}

void EntryArea2()
{
    MobMakerStartSummon(0, PlaceMobMaker(30, 60, SummonMobMystic));
    SetNextFunction(EndArea2);
}

int SummonMobMystic(int sUnit)
{
    int unit = CreateObjectAtUnit("Wizard", sUnit);

    SetUnitMaxHealth(unit, 260);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    Enchant(unit, "ENCHANT_ANCHORED", 0.0);
    return unit;
}

void EndArea2()
{
    int i;

    for (i = 0 ; i < 4 ; i += 1)
    {
        RemoveWallAtObjectPos(31);
        RemoveWallAtObjectPos(35);
        TeleportLocationVector(31, -23.0, 23.0);
        TeleportLocationVector(35, -23.0, 23.0);
    }
    FrameTimer(30, EntryArea3);
    UniPrintToAll("비밀의 벽이 열렸습니다!");
}

int SummonMobBlackSpider(int posUnit)
{
    int unit = CreateObjectAtUnit("BlackWidow", posUnit);

    BlackWidowSubProcess(unit);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);

    return unit;
}

int SummonMobGoon(int posUnit)
{
    int unit = CreateObjectAtUnit("Goon", posUnit);

    UnitLinkBinScript(unit, GoonBinTable());
    SetUnitVoice(unit, 63);
    SetUnitMaxHealth(unit, 275);

    return unit;
}

int SummonUrchin(int posUnit)
{
    int unit = CreateObjectAtUnit("Urchin", posUnit);

    UnitZeroFleeRange(unit);
    SetUnitMaxHealth(unit, 130);
    return unit;
}

void RemoveSingleWallWithFx(int location, float vectX, float vectY)
{
    RemoveWallAtObjectPos(location);
    Effect("JIGGLE", LocationX(location), LocationY(location), 11.0, 0.0);
    Effect("SMOKE_BLAST", LocationX(location), LocationY(location), 11.0, 0.0);
    if (ToInt(vectX) || ToInt(vectY))
        TeleportLocationVector(location, vectX, vectY);
}

void ClearMinesWalls()
{
    int i;

    for (i = 0 ; i < 18 ; i ++)
    {
        if (i < 8)
            RemoveSingleWallWithFx(53, -23.0, 23.0);
        RemoveSingleWallWithFx(54, 23.0, 23.0);
    }
}

void EndArea3()
{
    FrameTimer(10, ClearMinesWalls);
    FrameTimer(20, EntryArea4);
}

void EntryArea3()
{
    MobMakerStartSummon(3, PlaceMobMaker(39, 20, SummonMobBlackSpider));
    MobMakerStartSummon(3, PlaceMobMaker(40, 20, SummonMobGoon));
    MobMakerStartSummon(3, PlaceMobMaker(41, 20, SummonUrchin));
    SetNextFunction(EndArea3);
}

int SummonMobStoneGiant(int posUnit)
{
    int giant = CreateObjectAtUnit("StoneGolem", posUnit);

    SetUnitMaxHealth(giant, 750);
    Enchant(giant, EnchantList(9), 0.0);
    return giant;
}

int SummonMobImp(int posUnit)
{
    int fairy = CreateObjectAtUnit("Imp", posUnit);

    ImpSubProcess(fairy);
    return fairy;
}

int SummonMobShade(int posUnit)
{
    int shade = CreateObjectAtUnit("Shade", posUnit);

    SetUnitMaxHealth(shade, 225);
    return shade;
}

void EndArea4()
{
    RemoveWallAtObjectPos(68);
    RemoveWallAtObjectPos(69);
    RemoveWallAtObjectPos(70);
    
    FrameTimer(30, EntryArea5);
}

void EntryArea4()
{
    MobMakerStartSummon(1, PlaceMobMaker(42, 32, SummonMobStoneGiant));
    MobMakerStartSummon(1, PlaceMobMaker(43, 20, SummonMobImp));
    MobMakerStartSummon(1, PlaceMobMaker(44, 32, SummonMobShade));
    SetNextFunction(EndArea4);
}

int SummonMobWasp(int posUnit)
{
    int honey = CreateObjectAtUnit("Wasp", posUnit);

    WaspSubProcess(honey);
    return honey;
}

int SummonMobScorpion(int posUnit)
{
    int mob = CreateObjectAtUnit("Scorpion", posUnit);

    ScorpionSubProcess(mob);
    return mob;
}

int SummonMobHorrendous(int posUnit)
{
    int mob = CreateObjectAtUnit("Horrendous", posUnit);

    SetUnitMaxHealth(mob, 325);
    RegistUnitStrikeHook(mob);
    return mob;
}

int SummonBigFish(int posUnit)
{
    int mob = CreateObjectAtUnit("FishBig", posUnit);

    FishBigSubProcess(mob);
    RegistUnitStrikeHook(mob);
    return mob;
}

int SummonLichMaster(int posUnit)
{
    int master = CreateObjectAtUnit("LichLord", posUnit);

    LichLordSubProcess(master);
    return master;
}

void EndArea5()
{
    UnlockDoor(Object("ExitLocker1"));
    UnlockDoor(Object("ExitLocker2"));
    UniPrintToAll("출입문이 열렸습니다");
    FrameTimer(30, EntryArea6);
}

void EntryArea5()
{
    MobMakerStartSummon(1, PlaceMobMaker(75, 6, SummonMobWasp));
    MobMakerStartSummon(1, PlaceMobMaker(76, 6, SummonMobWasp));

    MobMakerStartSummon(3, PlaceMobMaker(72, 15, SummonMobScorpion));
    MobMakerStartSummon(3, PlaceMobMaker(73, 15, SummonMobScorpion));
    MobMakerStartSummon(3, PlaceMobMaker(74, 23, SummonMobHorrendous));
    SetNextFunction(EndArea5);
}

int SummonMobStrongWizard(int sUnit)
{
    int unit = CreateObjectAtUnit("StrongWizardWhite", sUnit);

    UnitLinkBinScript(unit, StrongWizardWhiteBinTable());
    SetUnitMaxHealth(unit, 320);
    RetreatLevel(unit, 0.0);
    ResumeLevel(unit, 1.0);
    return unit;
}

void WizRunAway()
{
	if (UnitCheckEnchant(SELF, GetLShift(ENCHANT_ANTI_MAGIC)))
		EnchantOff(SELF, EnchantList(ENCHANT_ANTI_MAGIC));
}

int SummonMobRedWizard(int sUnit)
{
    int redWiz = CreateObjectAtUnit("WizardRed", sUnit);
    int uec = GetMemory(GetMemory(0x750710) + 0x2ec);

    UnitLinkBinScript(redWiz, WizardRedBinTable());
    SetUnitMaxHealth(redWiz, 411);
    SetCallback(redWiz, 8, WizRunAway);
    SetUnitStatus(redWiz, GetUnitStatus(redWiz) ^ 0x20);
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
    return redWiz;
}

int SummonMobPlant(int sUnit)
{
    int mob = CreateObjectAtUnit("CarnivorousPlant", sUnit);

    UnitZeroFleeRange(mob);
    SetUnitSpeed(mob, 2.5);
    SetUnitMaxHealth(mob, 325);
    AggressionLevel(mob, 1.0);
    RetreatLevel(mob, 0.0);
    ResumeLevel(mob, 1.0);
    return mob;
}

int SummonBeast(int posUnit)
{
    int mob = CreateObjectAtUnit("WeirdlingBeast", posUnit);

    UnitZeroFleeRange(mob);
    WeirdlingBeastSubProcess(mob);
    return mob;
}

void EndArea6()
{
    RemoveWallAtObjectPos(103);
    RemoveWallAtObjectPos(104);
    RemoveWallAtObjectPos(105);
    FrameTimer(1, EntryArea7);
    UniPrintToAll("좌측상단의 비밀벽 하나가 열렸습니다");
}

void EntryArea6()
{
    //100, 101// (98, 99)
    MobMakerStartSummon(1, PlaceMobMaker(100, 20, SummonMobStrongWizard));
    MobMakerStartSummon(1, PlaceMobMaker(101, 20, SummonBigFish));
    MobMakerStartSummon(10, PlaceMobMaker(98, 20, SummonLichMaster));
    MobMakerStartSummon(11, PlaceMobMaker(99, 20, SummonMobRedWizard));
    // MobMakerStartSummon(1, PlaceMobMaker(101, 20, Summon))
    SetNextFunction(EndArea6);
}

int SummonMobLich(int sUnit)
{
    int lich = CreateObjectAtUnit("Lich", sUnit);

    SetUnitMaxHealth(lich, 325);
    return lich;
}

int SummonMobEmberDemon(int sUnit)
{
    int mob = CreateObjectAtUnit("EmberDemon", sUnit);

    EmberDemonSubProcess(mob);
    RegistUnitStrikeHook(mob);
    return mob;
}

int SummonMobSkullLord(int posunit)
{
    int mob = CreateObjectAtUnit("SkeletonLord", posunit);

    SkeletonLordSubProcess(mob);
    RegistUnitStrikeHook(mob);

    return mob;
}

void EndArea7()
{
    EntryArea8();
}

void EntryArea7()
{
    MobMakerStartSummon(1, PlaceMobMaker(106, 20, SummonBigFish));
    MobMakerStartSummon(3, PlaceMobMaker(107, 20, SummonMobPlant));
    MobMakerStartSummon(5, PlaceMobMaker(108, 30, SummonMobGoon));
    MobMakerStartSummon(9, PlaceMobMaker(109, 30, SummonMobLich));

    SetNextFunction(EndArea7);
}

void WeaponClassCProperty1Entry(int wUnit, int slot, int execFunctionNumber, int tablePtr)
{
    int ptr = UnitToPtr(wUnit);

    if (ptr)
    {
        SetMemory(ptr + 0x2fc, execFunctionNumber);
        SetMemory(GetMemory(ptr + 0x2b4) + (slot * 4), tablePtr);
    }
}

int WeaponClassCProperty2Table()
{
    int dat[36], link;

    if (!link)
    {
        dat[0] = GetMemory(0x97bb40 + (SToInt("Fire4") * 4));
        dat[1] = 0x61; dat[6] = 0xb40000; dat[5] = 0x4b0; dat[7] = 0x1b2ff2; dat[9] = 1;
        //dat[13] = 0x4e04d0;
        dat[13] = WeaponClassCProperty1Code();
        dat[14] = ToInt(100.0); //dat[13] = 0x4e06f0; dat[13] = 0x4e0550;
        link = &dat;
    }
    return link;
}

void WeaponClassCProperty2Entry(int weapon)
{
    int i, *ptr = UnitToPtr(weapon);

    if (ptr != NULLPTR)
    {
        int *proper = ptr[173];

        proper[2] = WeaponClassCProperty2Table();
        SetWeaponProperties(weapon, 5, 4, 1, 22);
    }
}

int WeaponClassPropertyExecScript()
{
    int arr[13], link;

    if (!link)
    {
        arr[0] = 0x24448D50; arr[1] = 0xEC83520C; arr[2] = 0x85108B0C; arr[3] = 0x501C74D2; arr[4] = 0x02FC828B; arr[5] = 0x44890000;
        arr[6] = 0x89580424; arr[7] = 0x8B082454; arr[8] = 0x44890440; arr[9] = 0xE5E80424; arr[10] = 0x83FFDB62; arr[11] = 0x585A0CC4;
        arr[12] = 0x000090C3;
        link = &arr;
        FixCallOpcode(link + 0x26, 0x507310);
    }
    return link;
}

int WeaponClassCProperty1Code()
{
    int code[21], call1 = 0x4e0702, call2 = 0x4e0722, call3 = 0x4e0731, nop;
    int link;

    if (!link)
    {
        link = &code;
        OpcodeCopiesAdvance(link, link + (21 * 4), 0x4e06f0, 0x4e073c);

        SetMemory(link + 46, 0x83);
        SetMemory(link + 61, 97);
    }
    return link;
}

int WeaponClassCProperty3Table()
{
    int dat[36], link;

    if (!link)
    {
        dat[0] = GetMemory(0x97bb40 + (SToInt("Fire4") * 4));
        dat[1] = 0x61; dat[6] = 0xb40000; dat[5] = 0x4b0; dat[7] = 0x1b2ff2; dat[9] = 1;
        dat[10] = WeaponClassPropertyExecScript();
        dat[13] = WeaponClassCProperty1Code();
        dat[14] = ToInt(4.0);
        link = &dat;
    }
    return link;
}

int DummyUnitCreate(string name, float locX, float locY)
{
    int unit = CreateObjectAt(name, locX, locY);

    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    ObjectOff(unit);
    Frozen(unit, 1);
    return unit;
}

void WispDestroyFx(float posX, float posY)
{
    int wispFx = CreateObjectAt("WillOWisp", posX, posY);

    UnitNoCollide(wispFx);
    ObjectOff(wispFx);
    Damage(wispFx, 0, 255, -1);
    DeleteObjectTimer(wispFx, 9);
}

void GreenSparkFx(float posX, float posY)
{
    int genHurt = CreateObjectAt("MonsterGenerator", posX, posY);

    Damage(genHurt, 0, 1, -1);
    Delete(genHurt);
}

void UnitVisibleSplashA()
{
    int parent = GetOwner(SELF);
    int spIdx = ToInt(GetObjectZ(parent + 1));

    if (CurrentHealth(GetOwner(parent)))
    {
        if (GetUnit1C(OTHER) ^ spIdx)
        {
            if (Distance(GetObjectX(SELF), GetObjectY(SELF), GetObjectX(OTHER), GetObjectY(OTHER)) <= GetObjectX(parent))
            {
                Damage(OTHER, GetOwner(parent), ToInt(GetObjectZ(parent)), 14);
                SetUnit1C(OTHER, spIdx);
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

/**
* @brief. strike a weapon handler
* @brief. self - the weapon
* @brief. other - the holder
*/
void XBowShot()     //@brief. WeaponProperty -3-
{
    int xbow = CreateObjectAt("ArcherArrow", GetObjectX(OTHER) + UnitAngleCos(OTHER, 8.0), GetObjectY(OTHER) + UnitAngleSin(OTHER, 8.0));

    SetOwner(OTHER, xbow);
    LookWithAngle(xbow, GetDirection(OTHER));
    Enchant(xbow, "ENCHANT_SHOCK", 0.0);
    PushObject(xbow, 34.0, GetObjectX(OTHER), GetObjectY(OTHER));
}

int PlaceArrowSword(int location)
{
    int arrow = CreateObjectAt("Sword", LocationX(location), LocationY(location));

    SetWeaponProperties(arrow, 5, 5, 8, 3);
    WeaponClassCProperty1Entry(arrow, 2, XBowShot, WeaponClassCProperty3Table());
    return arrow;
}

void EnergyparTouched()
{
    int owner = GetOwner(self);

    if (CurrentHealth(other) && IsAttackedBy(other, owner))
        Damage(other, owner, 100, 14);
}

void EnergyparFx(int helper)
{
    int owner = GetOwner(helper);

    Effect("EXPLOSION", GetObjectX(helper), GetObjectY(helper), 0.0, 0.0);

    int dmHelper = DummyUnitCreate("Demon", GetObjectX(helper), GetObjectY(helper));
    SetUnitFlags(dmHelper, GetUnitFlags(dmHelper) ^ 0x2000);    //NO_PUSH_CHARACTERS
    SetOwner(owner, dmHelper);
    SetCallback(dmHelper, 9, EnergyparTouched);
    DeleteObjectTimer(dmHelper, 1);
}

void EnergyparGoForward(int helper)
{
    int owner, durate;

    while (IsObjectOn(helper))
    {
        owner = GetOwner(helper);
        if (CurrentHealth(owner))
        {
            durate = GetDirection(helper);
            if (durate)
            {
                FrameTimerWithArg(1, helper, EnergyparGoForward);
                MoveObject(helper, GetObjectX(helper) + GetObjectZ(helper), GetObjectY(helper) + GetObjectZ(helper + 1));
                LookWithAngle(helper, durate - 1);
                EnergyparFx(helper);
                break;
            }
        }
        Delete(helper);
        Delete(helper + 1);
        break;
    }
}

void EnergyparShot()    //@brief. WeaponProperty -2-
{
    float vectX = UnitAngleCos(other, 13.0), vectY = UnitAngleSin(other, 13.0);
    int helper = CreateObjectAt("ImaginaryCaster", GetObjectX(other) + vectX, GetObjectY(other) + vectY);

    Raise(helper, vectY);
    Raise(CreateObjectAtUnit("ImaginaryCaster", other), vectX);
    LookWithAngle(helper, 15);
    SetOwner(other, helper);
    FrameTimerWithArg(1, helper, EnergyparGoForward);
}

int PlaceEnergyparAxe(int location)
{
    int par = CreateObjectAt("OgreAxe", LocationX(location), LocationY(location));

    SetWeaponProperties(par, 5, 5, 8, 3);
    WeaponClassCProperty1Entry(par, 2, EnergyparShot, WeaponClassCProperty3Table());
    return par;
}

void AngelCrystalCollide()
{
    int owner = GetOwner(GetTrigger() + 1);

    if (GetTrigger())
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            WispDestroyFx(GetObjectX(self), GetObjectY(self));
            SplashDamageAt(owner, 40, GetObjectX(self), GetObjectY(self), 50.0);
            Delete(self);
        }
    }
}

int AngelCrystal(int owner, float vectSize)
{
    int crystal = CreateObjectAt("GameBall", GetObjectX(owner) + UnitAngleCos(owner, vectSize), GetObjectY(owner) + UnitAngleSin(owner, vectSize));
    int ptr = GetMemory(0x750710);

    SetOwner(owner, CreateObjectAtUnit("InvisibleLightBlueLow", owner));
    SetMemory(ptr + 0x2b8, ImportUnitCollideFunc());
    SetMemory(ptr + 0x2fc, AngelCrystalCollide);
    DeleteObjectTimer(crystal, 90);
    DeleteObjectTimer(crystal + 1, 100);
    return crystal;
}

void ShotAngelCrystal() //@brief. WeaponProperty -1-
{
    PushObject(AngelCrystal(other, 19.0), 24.0, GetObjectX(other), GetObjectY(other));
    PlaySoundAround(other, 204);
    Effect("CYAN_SPARKS", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
}

int PlaceAngelCrystalSword(int location)
{
    int angel = CreateObjectAt("GreatSword", LocationX(location), LocationY(location));

    SetWeaponProperties(angel, 5, 5, 8, 3);
    WeaponClassCProperty1Entry(angel, 2, ShotAngelCrystal, WeaponClassCProperty3Table());
    return angel;
}

void PlaceDefaultItems(string itemname, object location)
{
    int i;
    
    for (i = 0 ; i < 8 ; i ++)
    {
        CreateObjectAt(itemname, LocationX(location), LocationY(location));
        TeleportLocationVector(location, 23.0, 23.0);
    }
}

int SingleShuriken(int owner, float posX, float posY)
{
    int mis = CreateObjectAt("OgreShuriken", posX, posY);
    int ptr = GetMemory(0x750710);

    SetMemory(GetMemory(ptr + 0x2bc) + 4, 60);
    PushObject(mis, -38.0, GetObjectX(owner), GetObjectY(owner));
    SetOwner(owner, mis);
    return mis;
}

void TripleShurikens(int dirUnit)
{
    int owner = GetOwner(dirUnit);

    if (CurrentHealth(dirUnit) && IsObjectOn(dirUnit))
    {
        int i;

        for (i = 0 ; i < 13 ; i ++)
        {
            SingleShuriken(owner, GetObjectX(dirUnit) + UnitAngleCos(dirUnit, 18.0), GetObjectY(dirUnit) + UnitAngleSin(dirUnit, 18.0));
            LookWithAngle(dirUnit, GetDirection(dirUnit) + 5);
        }
    }
    Delete(dirUnit);
}

void CastTripleShurikens()
{
    int dirUnit = CreateObjectAt("ImaginaryCaster", GetObjectX(other), GetObjectY(other));

    SetOwner(other, dirUnit);
    LookWithAngle(dirUnit, GetDirection(other) - 30);
    FrameTimerWithArg(1, dirUnit, TripleShurikens);
}

int PlaceTripleShurikenSword(int location)
{
    int triple = CreateObjectAt("GreatSword", LocationX(location), LocationY(location));

    SetWeaponProperties(triple, 5, 5, 8, 3);
    WeaponClassCProperty1Entry(triple, 2, CastTripleShurikens, WeaponClassCProperty3Table());
    Enchant(triple, EnchantList(4), 0.0);
    return triple;
}

int PlaceSuperPowerSword(int location)
{
    int super = CreateObjectAt("GreatSword", LocationX(location), LocationY(location));

    WeaponClassCProperty2Entry(super);
    Enchant(super, EnchantList(4), 0.0);
    return super;
}

int MagicWeaponContainer()
{
    int count;
    int array[20];

    return GetScrDataField(MagicWeaponContainer);
}

//@brief. 컨테이너가 포함한 노드 수를 반환합니다
int WeaponContainerCount()
{
    return GetMemory(MagicWeaponContainer());
}

//@brief. 인덱스에 해당하는 노드를 컨테이너로 부터 얻습니다
object WeaponContainerNode(int index)
{
    if (index < 20)
    {
        int ptr = MagicWeaponContainer() + 4;
        object functionId = GetMemory(ptr + (index * 4));

        if (functionId)
            return functionId;
    }
    return 0;   //@brief. nullptr;
}

//@brief. 입력된 인덱스에 해당하는 설명 데이터를 가져옵니다
string WeaponContainerDesc(int index)
{
    object nodeId = WeaponContainerNode(index);

    if (nodeId > 0)
    {
        int ptr = GetScrDataField(nodeId);

        return ToStr(GetMemory(ptr));
    }
    return "null";
}

//@brief. 입력된 인덱스에 해당하는 가격 데이터를 가져옵니다
int WeaponContainerPay(int index)
{
    object nodeId = WeaponContainerNode(index);

    if (nodeId > 0)
    {
        int ptr = GetScrDataField(nodeId);

        return GetMemory(ptr + 4);
    }
    return -1;
}

//@brief. 입력된 인덱스에 해당하는 생성함수 객체를 가져옵니다
object WeaponContainerFunction(int index)
{
    object nodeId = WeaponContainerNode(index);

    if (nodeId > 0)
    {
        int ptr = GetScrDataField(nodeId);

        return GetMemory(ptr + 8);
    }
    return -1;
}

void WeaponContainerPushback(object function)
{
    int ptr = MagicWeaponContainer();
    int count = GetMemory(ptr);

    if (count < 20)
    {
        CallFunction(function);
        SetMemory(ptr, count + 1);
        SetMemory(ptr + 4 + (count * 4), function);
    }
}

int MagicWeaponClassArrowSword()
{
    string name = "에로우 서드";
    int pay = 20000;
    object createFunction = PlaceArrowSword;

    return GetScrDataField(MagicWeaponClassArrowSword);
}

int MagicWeaponClassTripleHammer()
{
    string name = "트리플 슈리켄 해머";
    int pay = 25500;
    object createFunction = PlaceTripleShurikenSword;

    return GetScrDataField(MagicWeaponClassTripleHammer);
}

int MagicWeaponClassEnergyparSword()
{
    string name = "에너지파 엑스칼리버";
    int pay = 28350;
    object createFunction = PlaceEnergyparAxe;

    return GetScrDataField(MagicWeaponClassEnergyparSword);
}

int MagicWeaponClassCrystalSword()
{
    string name = "지랄발광 수정구 서드";
    int pay = 26533;
    object createFunction = PlaceAngelCrystalSword;

    return &name;
}

int MagicWeaponClassPowerfulSword()
{
    string name = "데미지 존나 쎈 서드";
    int pay = 24000;
    object createFunction = PlaceSuperPowerSword;

    return GetScrDataField(MagicWeaponClassPowerfulSword);
}

void InitWeaponContainer()
{
    WeaponContainerPushback(MagicWeaponClassArrowSword);
    WeaponContainerPushback(MagicWeaponClassCrystalSword);
    WeaponContainerPushback(MagicWeaponClassEnergyparSword);
    WeaponContainerPushback(MagicWeaponClassPowerfulSword);
    WeaponContainerPushback(MagicWeaponClassTripleHammer);
}

int FirstItem(int unit)
{
    return GetLastItem(unit);
}

int NextItem(int inv)
{
    return GetPreviousItem(inv);
}

int SetInvulnerabilityItem(int unit)
{
    int count = 0, inv = FirstItem(unit);

    while (inv)
    {
        if (!UnitCheckEnchant(inv, GetLShift(ENCHANT_INVULNERABLE)))
        {
            Enchant(inv, EnchantList(ENCHANT_INVULNERABLE), 0.0);
            ++count;
        }
        inv = NextItem(inv);
    }
    return count;
}

void ShopClassSetInvulnerabilityItemDesc()
{
    UniPrint(OTHER, "당신이 소유한 모든 아이템을 무적화 시켜드립니다");
    UniPrint(OTHER, "이 작업은 금화 5천이 필요합니다. 계속하려면 \"예\" 를 누르세요");
}

void ShopClassSetInvulnerabilityItemTrade()
{
    if (GetGold(OTHER) >= 5000)
    {
        if (SetInvulnerabilityItem(OTHER))
        {
            ChangeGold(OTHER, -5000);
            UniPrint(OTHER, "결제가 완료되었습니다");
        }
        else
            UniPrint(OTHER, "처리할 아이템이 없어서 거래되지 않았습니다");
    }
}

void ShopClassMagicWeaponDesc()
{
    int curIndex = GetDirection(SELF);
    string desc = WeaponContainerDesc(curIndex);

    if (desc != "null")
    {
        UniPrint(OTHER, desc + " 을 구입하시겠어요? 가격은 " + IntToString(WeaponContainerPay(curIndex)) + " 골드 입니다");
        UniPrint(OTHER, "구입하시려면 '예' 를 누르시고, 다른 아이템을 보시려면 '아니오'를 누르세요. 거래를 취소하려면 '떠나기'를 누릅니다");
        TellStoryUnitName("AA", "thing.db:IdentifyDescription", desc);
    }
}

void ShopClassMagicWeaponTrade()
{
    int dlgResult = GetAnswer(SELF), curIndex = GetDirection(SELF);

    if (dlgResult == 1)     //@brief. YES
    {
        int pay = WeaponContainerPay(curIndex);

        if (pay < 0)
            return;
        if (GetGold(OTHER) >= pay)
        {
            ChangeGold(OTHER, -pay);
            object resultFunction = WeaponContainerFunction(curIndex);

            if (resultFunction >= 0)
            {
                TeleportLocation(110, GetObjectX(OTHER), GetObjectY(OTHER));
                CallFunctionWithArgInt(resultFunction, 110);
            }
            // FrameTimerWithArg(1, resultItem, DelayForcePickItemToOwner);
            PlaySoundAround(OTHER, 308);
            UniPrint(OTHER, WeaponContainerDesc(curIndex) + " 거래가 완료되었습니다");
        }
        else
            UniPrint(OTHER, "거래가 취소되었습니다. 잔액이 부족합니다");
    }
    else if (dlgResult == 2)    //@brief. NO
    {
        UniPrint(OTHER, "'아니오'를 누르셨습니다. 다음 판매 품목을 보여드립니다");
        LookWithAngle(SELF, (curIndex + 1) % WeaponContainerCount());
        ShopClassMagicWeaponDesc();
    }
}

void ShopClassAllEnchantmentDesc()
{
    UniPrint(OTHER, "올 엔첸트 능력을 구입하시겠어요? 가격은 6만 골드 입니다");
    UniPrint(OTHER, "올 엔첸트 능력은 각종 유용한 버프가 당신에게 항상 지속됩니다");
    UniPrint(OTHER, "계속 거래하기를 원하시면 '예' 을 누르세요");
    TellStoryUnitName("AA", "thing.db:IdentifyDescription", "올엔첸6만원");
}

void ShopClassAllEnchantmentTrade()
{
    if (GetAnswer(SELF) ^ 1)
        return;

    while (true)
    {
        if (GetGold(OTHER) >= 60000)
        {
            int plr = CheckPlayer();

            if (plr < 0)
                break;
            if (PlayerClassCheckAllEnchant(plr))
            {
                UniPrint(OTHER, "당신은 이미 이 능력의 소유자 입니다");
                break;
            }
            ChangeGold(OTHER, -60000);
            PlayerClassSetAllEnchant(plr);
            PlayerAllEnchantment(OTHER);
            UniPrint(OTHER, "거래가 완료되었습니다. 올 엔첸트 능력은 이제 당신의 소유입니다");
            break;
        }
        UniPrint(OTHER, "거래가 취소되었습니다. 사유: 잔액이 부족합니다");
        break;
    }
}

int ShopClassInvulnerabilityItemShopCreate(int location)
{
    int mk = DummyUnitCreate("Swordsman", LocationX(location), LocationY(location));

    SetDialog(mk, "YESNO", ShopClassSetInvulnerabilityItemDesc, ShopClassSetInvulnerabilityItemTrade);
    return mk;
}

int ShopClassMagicalWeaponMarketCreate(int location)
{
    int keeper = DummyUnitCreate("Horrendous", LocationX(location), LocationY(location));

    CreateObjectAtUnit("ImaginaryCaster", keeper);
    LookWithAngle(keeper, 0);
    SetDialog(keeper, "YESNO", ShopClassMagicWeaponDesc, ShopClassMagicWeaponTrade);

    return keeper;
}

int ShopClassAllEnchantMarketCreate(int location)
{
    int allEnchantMarket = DummyUnitCreate("WizardGreen", LocationX(location), LocationY(location));

    SetDialog(allEnchantMarket, "YESNO", ShopClassAllEnchantmentDesc, ShopClassAllEnchantmentTrade);
    return allEnchantMarket;
}

void LightningShotCollide()
{
    if (GetTrigger() && GetCaller())
    {
        Damage(other, 0, 50, 9);
        Enchant(other, EnchantList(25), 2.0);
        Delete(self);
    }
}

int LightningShotSingle(float posX, float posY)
{
    int mis = CreateObjectAt("LightningBolt", posX, posY);
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2b8, ImportUnitCollideFunc());
    SetMemory(ptr + 0x2fc, LightningShotCollide);
    return mis;
}

int LightningShooter(object baseUnit, float shotVectX, float shotVectY, float xVect, float yVect)
{
    int mis = LightningShotSingle(GetObjectX(baseUnit) + xVect, GetObjectY(baseUnit) + yVect);

    PushObjectTo(mis, shotVectX, shotVectY);
    return mis;
}

void LightningShotGunTrapArea1()
{
    int i, trpBase = CreateObjectAt("ImaginaryCaster", LocationX(52), LocationY(52));

    for (i = 0 ; i < 10 ; i ++)
        LightningShooter(trpBase + i, -32.0, 32.0, 13.0, 13.0);
    Delete(trpBase);
}

void UnitHealingHandler(int healing)
{
    int owner = GetOwner(healing);

    while (IsObjectOn(healing))
    {
        if (CurrentHealth(owner) && UnitCheckEnchant(owner, GetLShift(ENCHANT_CROWN)))
        {
            RestoreHealth(owner, 1);
            if (ToInt(DistanceUnitToUnit(owner, healing)))
                MoveObject(healing, GetObjectX(owner), GetObjectY(owner));
            FrameTimerWithArg(1, healing, UnitHealingHandler);
            break;
        }
        Delete(healing);
        break;
    }
}

void WellRefreshing()
{
    if (!UnitCheckEnchant(other, GetLShift(ENCHANT_CROWN)))
    {
        Enchant(other, EnchantList(ENCHANT_CROWN), 10.0);

        int healing = CreateObjectAtUnit("InvisibleLightBlueLow", other);
        SetOwner(other, healing);
        Enchant(healing, EnchantList(ENCHANT_RUN), 0.0);
        FrameTimerWithArg(1, healing, UnitHealingHandler);

        PlaySoundAround(self, 1004);
        UniPrint(other, "우물의 마법이 잠시동안 당신을 지속적으로 치료해 줍니다");
    }
}

/*********
* monsterbin script section
*********/

int WizardRedBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1635412311; arr[1] = 1699898482; arr[2] = 100; arr[16] = 80000; arr[17] = 300; 
		arr[18] = 100; arr[19] = 50; arr[21] = 1065353216; arr[24] = 1067869798; arr[26] = 4; 
		arr[27] = 4; arr[53] = 1128792064; arr[54] = 4;
		link = &arr;
	}
	return link;
}

int LichLordBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1751345484; arr[1] = 1685221196; arr[17] = 473; arr[19] = 100; arr[21] = 1065353216; 
		arr[23] = 2048; arr[24] = 1065353216; arr[25] = 1; arr[26] = 4; arr[27] = 7; 
		arr[28] = 1108082688; arr[29] = 50; arr[30] = 1092616192; arr[32] = 19; arr[33] = 27; 
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
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 2048);
		//SetMemory(GetMemory(ptr + 0x22c), 300);
		//SetMemory(GetMemory(ptr + 0x22c) + 0x4, 300);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, LichLordBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int FishBigBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1752394054; arr[1] = 6777154; arr[17] = 237; arr[18] = 1; arr[19] = 55; 
		arr[21] = 1065353216; arr[23] = 34816; arr[24] = 1073741824; arr[27] = 1; arr[28] = 1112014848; 
		arr[29] = 30; arr[31] = 8; arr[32] = 3; arr[33] = 7; arr[59] = 5542784; 
		arr[60] = 1329; arr[61] = 46905600; 
		link = &arr;
	}
	return link;
}

void FishBigSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1070805811);
		SetMemory(ptr + 0x224, 1070805811);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 34816);
		SetMemory(GetMemory(ptr + 0x22c), 237);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 237);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FishBigBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
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

int ScorpionBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1919902547; arr[1] = 1852795248; arr[17] = 275; arr[18] = 75; arr[19] = 75; 
		arr[21] = 1065353216; arr[24] = 1067869798; arr[25] = 1; arr[26] = 5; arr[27] = 5; 
		arr[28] = 1112014848; arr[29] = 77; arr[30] = 1101004800; arr[31] = 3; arr[32] = 22; 
		arr[33] = 30; arr[34] = 3; arr[35] = 5; arr[36] = 30; arr[59] = 5543344; 
		arr[60] = 1373; arr[61] = 46895952; 
		link = &arr;
	}
	return link;
}

void ScorpionSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1074790400);
		SetMemory(ptr + 0x224, 1074790400);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 0);
		SetMemory(GetMemory(ptr + 0x22c), 275);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 275);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, ScorpionBinTable());
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
		arr[0] = 1919509847; arr[1] = 1852402788; arr[2] = 1634026087; arr[3] = 29811; arr[17] = 470; 
		arr[19] = 55; arr[21] = 1065353216; arr[24] = 1071225242; arr[26] = 4; arr[28] = 1106247680; 
		arr[29] = 44; arr[31] = 3; arr[32] = 4; arr[33] = 5; arr[59] = 5542784; 
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
		SetMemory(GetMemory(ptr + 0x22c), 470);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 470);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WeirdlingBeastBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int WaspBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1886609751; arr[17] = 98; arr[18] = 1; arr[19] = 120; arr[21] = 1065353216; 
		arr[23] = 1; arr[24] = 1067869798; arr[27] = 5; arr[28] = 1097859072; arr[29] = 20; 
		arr[31] = 3; arr[34] = 2; arr[35] = 3; arr[36] = 20; arr[59] = 5544320; 
		arr[60] = 1331; arr[61] = 46900736; 
		link = &arr;
	}
	return link;
}

void WaspSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1080452710);
		SetMemory(ptr + 0x224, 1080452710);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 1);
		SetMemory(GetMemory(ptr + 0x22c), 98);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 98);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WaspBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int ImpBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 7368009; arr[17] = 180; arr[18] = 1; arr[19] = 110; arr[21] = 1065353216; 
		arr[24] = 1065353216; arr[25] = 1; arr[26] = 5; arr[37] = 1769236816; arr[38] = 1181513062; 
		arr[39] = 1650815593; arr[40] = 7105633; arr[53] = 1128792064; arr[54] = 1; arr[55] = 9; 
		arr[56] = 17; arr[60] = 1328; arr[61] = 46904064; 
		link = &arr;
	}
	return link;
}

void ImpSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1079194419);
		SetMemory(ptr + 0x224, 1079194419);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 0);
		SetMemory(GetMemory(ptr + 0x22c), 180);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 180);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, ImpBinTable());
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
		arr[0] = 1667329090; arr[1] = 1684625259; arr[2] = 30575; arr[17] = 180; arr[19] = 85; 
		arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1065353216; arr[26] = 4; arr[27] = 3; 
		arr[28] = 1097859072; arr[29] = 25; arr[31] = 8; arr[32] = 13; arr[33] = 21; 
		arr[34] = 4; arr[35] = 2; arr[36] = 9; arr[37] = 1684631635; arr[38] = 1884516965; 
		arr[39] = 29801; arr[53] = 1128792064; arr[55] = 20; arr[56] = 28; arr[59] = 5544896; 
		arr[61] = 45071360; 
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
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 260);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 260);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, BlackWidowBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}

int EmberDemonBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1700949317; arr[1] = 1835353202; arr[2] = 28271; arr[17] = 225; arr[19] = 85; 
		arr[21] = 1056964608; arr[23] = 32768; arr[24] = 1067869798; arr[26] = 4; arr[27] = 5; 
		arr[28] = 1103626240; arr[29] = 20; arr[31] = 10; arr[32] = 5; arr[33] = 10; 
		arr[37] = 1869771859; arr[38] = 1766221678; arr[39] = 1633838450; arr[40] = 27756; arr[53] = 1128792064; 
		arr[54] = 4; arr[55] = 15; arr[56] = 30; arr[58] = 5545344; arr[59] = 5542784; 
		arr[60] = 1337; arr[61] = 46903552; 
		link = &arr;
	}
	return link;
}

void EmberDemonSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1076048691);
		SetMemory(ptr + 0x224, 1076048691);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 32768);
		SetMemory(GetMemory(ptr + 0x22c), 225);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 225);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, EmberDemonBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1056964608);
	}
}

int SkeletonLordBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1818585939; arr[1] = 1852798053; arr[2] = 1685221196; arr[16] = 50000; arr[17] = 100; 
		arr[18] = 75; arr[19] = 65; arr[21] = 1065353216; arr[23] = 1; arr[24] = 1067869798; 
		arr[26] = 4; arr[27] = 4; arr[28] = 1106247680; arr[29] = 20; arr[32] = 10; 
		arr[33] = 15; arr[58] = 5547856; arr[59] = 5542784; arr[60] = 1315; arr[61] = 46910464; 
		link = &arr;
	}
	return link;
}

void SkeletonLordSubProcess(int sUnit)
{
	int ptr = UnitToPtr(sUnit);

	if (ptr)
	{
		SetMemory(ptr + 0x220, 1073322393);
		SetMemory(ptr + 0x224, 1073322393);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x5a0, 1);
		SetMemory(GetMemory(ptr + 0x22c), 100);
		SetMemory(GetMemory(ptr + 0x22c) + 0x4, 100);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, SkeletonLordBinTable());
		SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x538, 0);
		SetMemory(GetMemory(ptr + 0x2ec) + 0x540, 1065353216);
	}
}


//sectionEnd

int DisposeBlockGroup(int baseLocation)
{
    int blocks = CreateObjectAt("ImaginaryCaster", LocationX(baseLocation), LocationY(baseLocation));

    CreateObjectAtUnit("ImaginaryCaster", blocks);
    Frozen(CreateObjectAt("SpikeBlock", LocationX(baseLocation), LocationY(baseLocation)), true);
    Frozen(CreateObjectAt("SpikeBlock", LocationX(baseLocation + 1), LocationY(baseLocation + 1)), true);
    Frozen(CreateObjectAt("SpikeBlock", LocationX(baseLocation + 2), LocationY(baseLocation + 2)), true);

    return blocks;
}

void SetBlockParams(int blocks, float vectX, float vectY, int moveDist)
{
    if (IsObjectOn(blocks))
    {
        LookWithAngle(blocks + 1, moveDist);
        Raise(blocks, vectX);
        Raise(blocks + 1, vectY);
    }
}

void InitBlocks()
{
    g_blockLeftUp = DisposeBlockGroup(55);
    g_blockLeftDown = DisposeBlockGroup(58);
    g_blockRightLeft = DisposeBlockGroup(61);
    g_blockRightRight = DisposeBlockGroup(64);

    SetBlockParams(g_blockLeftUp, 2.0, 2.0, 57);
    SetBlockParams(g_blockLeftDown, 2.0, -2.0, 57);
    SetBlockParams(g_blockRightLeft, 2.0, -2.0, 57);
    SetBlockParams(g_blockRightRight, -2.0, -2.0, 57);
}

void BlockReset(int blocks)
{
    ObjectOn(blocks);
}

void BlockMovingCommon(int blocks)
{
    int durate = GetDirection(blocks);
    int obstacle = blocks + 2;

    if (durate)
    {
        float vectX = GetObjectZ(blocks);
        float vectY = GetObjectZ(blocks + 1);

        MoveObjectVector(obstacle, vectX, vectY);
        MoveObjectVector(obstacle + 1, vectX, vectY);
        MoveObjectVector(obstacle + 2, vectX, vectY);
        LookWithAngle(blocks, durate - 1);
        FrameTimerWithArg(1, blocks, BlockMovingCommon);
    }
    else
    {
        Raise(blocks, -GetObjectZ(blocks)); //direction swapping
        Raise(blocks + 1, -GetObjectZ(blocks + 1)); //direction swapping
        // LookWithAngle(blocks, GetDirection(blocks) ^ true);
        ObjectToggle(blocks + 1);
        if (!IsObjectOn(blocks + 1))
        {
            FrameTimerWithArg(30, blocks, BlockMovingCommon);
            LookWithAngle(blocks, GetDirection(blocks + 1));
            PlaySoundAround(obstacle, 882);
            Effect("JIGGLE", GetObjectX(obstacle), GetObjectY(obstacle), 17.0, 0.0);
        }
        else
            FrameTimerWithArg(30, blocks, BlockReset);
    }
}

void StartBlockMoving(int blocks)
{
    if (IsObjectOn(blocks))
    {
        ObjectOff(blocks);  //Lock
        LookWithAngle(blocks, GetDirection(blocks + 1));    //setCounter
        FrameTimerWithArg(1, blocks, BlockMovingCommon);
    }
}

void BlockPressLeft()
{
    int upRow = g_blockLeftUp;
    int downRow = g_blockLeftDown;

    if (IsObjectOn(upRow) && IsObjectOn(downRow))
    {
        PlaySoundAround(SELF, 910);
        StartBlockMoving(upRow);
        StartBlockMoving(downRow);
    }
}

void BlockPressRight()
{
    int leftRow = g_blockRightLeft;
    int rightRow = g_blockRightRight;

    if (IsObjectOn(leftRow) && IsObjectOn(rightRow))
    {
        PlaySoundAround(SELF, 910);
        StartBlockMoving(leftRow);
        StartBlockMoving(rightRow);
    }
}

void HorrendousElectricStrike()
{
    Damage(OTHER, SELF, 30, 9);
    Effect("LIGHTNING", GetObjectX(SELF), GetObjectY(SELF), GetObjectX(OTHER), GetObjectY(OTHER));
    Effect("RICOCHET", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
}

void FishBiteStrike()
{
    LinearOrbMove(CreateObjectAt("CharmOrb", GetObjectX(SELF), GetObjectY(SELF)), UnitRatioX(SELF, OTHER, -1.0), UnitRatioY(SELF, OTHER, -1.0), 3.0, 3);
    Damage(OTHER, SELF, 6, 10);
}

void DrawGeometryRingAt(string orbName, float xProfile, float yProfile, int angleAdder)
{
    int u;
    float speed = 2.3;

    for ( u = 0 ; u < 60 ; u += 1)
        LinearOrbMove(CreateObjectAt(orbName, xProfile, yProfile), MathSine((u * 6) + 90 + angleAdder, -12.0), MathSine((u * 6) + angleAdder, -12.0), speed, 10);
}

void EmberDemonStrike()
{
    Damage(OTHER, SELF, 30, 1);
    DrawGeometryRingAt("HealOrb", GetObjectX(OTHER), GetObjectY(OTHER), 0);
    PushObjectTo(OTHER, UnitRatioX(OTHER, SELF, 19.0), UnitRatioY(OTHER, SELF, 19.0));
}

void SkullLordStrike()
{
    int subunit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(OTHER) + UnitRatioX(OTHER, SELF, 3.0), GetObjectY(OTHER) + UnitRatioY(OTHER, SELF, 3.0));

    SetOwner(SELF, subunit);
    Damage(OTHER, subunit, 40, 9);
    Effect("LIGHTNING", GetObjectX(OTHER), GetObjectY(OTHER), GetObjectX(OTHER), GetObjectY(OTHER) - 181.0);
    Delete(subunit);
}

void MonsterStrikeCallback()
{
    int thingId = GetUnitThingID(SELF);

    if (CurrentHealth(OTHER))
    {
        if (thingId == 1386)
            HorrendousElectricStrike();
        else if (thingId == 1329)
            FishBiteStrike();
        else if (thingId == 1337)
            EmberDemonStrike();
        else if (thingId == 1315)
            SkullLordStrike();
    }
}

void UrchinStoneCollide()
{
    int owner = GetOwner(SELF);

    if (GetTrigger())
    {
        if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
        {
            Damage(OTHER, SELF, 30, 9);
            Effect("RICOCHET", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
            Delete(SELF);
        }
    }
}

void SpiderWebMissileCollide()
{
    int owner = GetOwner(SELF);

    if (GetTrigger())
    {
        if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
        {
            Damage(OTHER, SELF, 55, 5);
            DeleteObjectTimer(CreateObjectAt("GreenSmoke", GetObjectX(OTHER), GetObjectY(OTHER)), 9);
            Delete(SELF);
        }
    }
}

void UrchinStone(int missile)
{
    int ptr = UnitToPtr(missile);

    if (ptr)
    {
        SetMemory(ptr + 0x2b8, ImportUnitCollideFunc());
        SetMemory(ptr + 0x2fc, UrchinStoneCollide);
    }
    Enchant(missile, EnchantList(21), 0.0);
}

void SpiderWebMissile(int missile)
{
    int ptr = UnitToPtr(missile);

    if (ptr)
    {
        SetMemory(ptr + 0x2b8, ImportUnitCollideFunc());
        SetMemory(ptr + 0x2fc, SpiderWebMissileCollide);
    }
}

void HarpoonEvent(int missile)
{
    int ptr = UnitToPtr(missile);

    Enchant(missile, EnchantList(21), 0.0);
}

void DetectedSpecificIndex(int curId)
{
    int thingId = GetUnitThingID(curId);

    if (thingId == 524)
        SpiderWebMissile(curId);
    else if (thingId == 525)     //@brief. ThrowingStone
        UrchinStone(curId);
    else if (thingId == 526)
        HarpoonEvent(curId);
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

void RemoveMinesExitWalls()
{
    ObjectOff(self);

    int i;

    for (i = 0 ; i < 6 ; i += 1)
    {
        RemoveWallAtObjectPos(71);
        TeleportLocationVector(71, 23.0, 23.0);
    }
    UniPrint(other, "우측 울타리가 낮아졌습니다");
}

string GeneratorUnitList(int index)
{
    string units[] = {
        "WizardGreen", "Bear", "Imp", "Horrendous",
        "Archer", "Wizard", "Mimic", "UrchinShaman", "Demon"
    };
    return units[index];
}

int CreateGenerator(int index, int location)
{
    string genName = GeneratorUnitList(index % 9) + "Generator";
    int gen = CreateObjectAt(genName, LocationX(location), LocationY(location));

    if (gen)
    {
        SetUnitMass(gen, 25.0);
        ObjectOff(gen);
        Enchant(gen, EnchantList(23), 0.0);
    }
    return gen;
}

string GetGeneratorUnitname(int index)
{
    return GeneratorUnitList(index % 9);
}

void AfterInitPuzzle()
{
    int i;

    for (i = 0 ; i < 9 ; i += 1)
    {
        int index = g_mixGenPuzzle[i + 1];

        DummyUnitCreate(GetGeneratorUnitname(index), LocationX(77 + i), LocationY(77 + i));
        g_genPuzzleBeacon[index] = Object("PuzBeacon" + IntToString(i + 1));
    }
}

void GenPuzzleMix()
{
    int i, pic, max = g_mixGenPuzzle[0], temp;

    for (i = 1 ; i <= max ; i += 1)
    {
        pic = Random(1, max);
        if (i ^ pic)
        {
            temp = g_mixGenPuzzle[i];
            g_mixGenPuzzle[i] = g_mixGenPuzzle[pic];
            g_mixGenPuzzle[pic] = temp;
        }
    }
}

void InitializeGenerators()
{
    int gen = CreateObjectAt("ImaginaryCaster", LocationX(86), LocationY(86));
    int i;

    g_mixGenPuzzle[0] = 9;  //@brief. set pad count
    for (i = 0 ; i < 9 ; i ++)
    {
        CreateGenerator(i, i + 86);
        g_mixGenPuzzle[i + 1] = i;
    }
    GenPuzzleMix();
    GenPuzzleMix();
    GenPuzzleMix();

    g_puzzleGens = gen + 1;
    FrameTimer(1, AfterInitPuzzle);
}

int CheckPuzzleSolved()
{
    int i, result = 0;

    for (i = 0 ; i < 9 ; i ++)
    {
        if (DistanceUnitToUnit(g_puzzleGens + i, g_genPuzzleBeacon[i]) < 46.0)
            result ++;
    }
    return result;
}

void RemoveSouthWalls()
{
    int i;

    for (i = 21 ; i ; i --)
    {
        RemoveWallAtObjectPos(95);
        TeleportLocationVector(95, -23.0, 23.0);
    }
}

void PuzzleGenHolding(int i)
{
    ObjectOff(g_genPuzzleBeacon[i]);
    MoveObject(g_puzzleGens + i, GetObjectX(g_genPuzzleBeacon[i]), GetObjectY(g_genPuzzleBeacon[i]));
    Frozen(g_puzzleGens + i, true);
}

void PuzzleSolved() //21, 95
{
    int i;

    for (i = 0 ; i < 9 ; i ++)
        PuzzleGenHolding(i);
    FrameTimer(15, RemoveSouthWalls);
    Effect("WHITE_FLASH", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
    PlaySoundAround(self, 795);
    UniPrintToAll("모든 비밀이 풀렸습니다. 남쪽 벽이 개방되었습니다");
    ObjectOn(g_ternnelSwitch);
    EnchantOff(g_ternnelSwitch, EnchantList(0));
}

void PressedGenPuzzleBeacon()
{
    if (HasClass(other, "MONSTERGENERATOR"))
    {
        if (CheckPuzzleSolved() ^ g_mixGenPuzzle[0])
            Effect("YELLOW_SPARKS", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
        else
            PuzzleSolved();
    }
}

void ClearTernnelWalls()
{
    ObjectOff(self);

    int i;

    for (i = 11 ; i ; i --)
    {
        RemoveWallAtObjectPos(96);
        RemoveWallAtObjectPos(97);
        TeleportLocationVector(96, -23.0, 23.0);
        TeleportLocationVector(97, -23.0, 23.0);
    }
}

void ResetHostileCritter()
{
	SetMemory(0x833e64, 0x55b);		//CarnivorousPlant
	SetMemory(0x833e70, 1329);		//FishBig
	SetMemory(0x833e74, 1330);		//FishSmall
	SetMemory(0x833e78, 1359);		//Rat
	SetMemory(0x833e7c, 1313);		//GreenFrog
}

void SetHostileCritter()
{
	SetMemory(0x833e64, 0x540);		//CarnivorousPlant
	SetMemory(0x833e70, 0x540);		//FishBig
	SetMemory(0x833e74, 0x540);		//FishSmall
	SetMemory(0x833e78, 0x540);		//Rat
	SetMemory(0x833e7c, 0x540);		//GreenFrog
}

void TestFunction()
{
    ObjectOff(self);
    // FrameTimer(30, EntryArea6);
}

void FrontBlockMovingToRight(int block)
{
    int remain = GetDirection(block);

    if (remain)
    {
        FrameTimerWithArg(1, block, FrontBlockMovingToRight);
        MoveObjectVector(block, 1.0, -1.0);
        LookWithAngle(block, --remain);
    }
}

void FrontBlockMovingToLeft(int block)
{
    int remain = GetDirection(block);

    if (remain)
    {
        FrameTimerWithArg(1, block, FrontBlockMovingToLeft);
        MoveObjectVector(block, -1.0, 1.0);
        LookWithAngle(block, --remain);
    }
}

void RemoveFrontBlockWalls()
{
    int i;

    for (i = 0 ; i < 5 ; i ++)
    {
        RemoveWallAtObjectPos(118);
        TeleportLocationVector(118, -23.0, 23.0);
    }
}

void StartFrontBlocks()
{
    int blocks = g_frontBlocks;

    if (!IsObjectOn(blocks))
    {
        ObjectOn(blocks);
        FrameTimerWithArg(1, blocks, FrontBlockMovingToRight);
        FrameTimerWithArg(1, blocks + 1, FrontBlockMovingToRight);
        FrameTimerWithArg(1, blocks + 2, FrontBlockMovingToLeft);
        FrameTimerWithArg(1, blocks + 3, FrontBlockMovingToLeft);
        SecondTimer(3, RemoveFrontBlockWalls);
    }
}

void FrontBlockLever()
{
    int count;

    ObjectOff(self);
    if (++count == 2)
        StartFrontBlocks();
}

int InitSecondBlocks()
{
    int blocks = CreateObjectAt("IronBlock", LocationX(111), LocationY(111));

    Frozen(CreateObjectAt("IronBlock", LocationX(112), LocationY(112)), true);
    Frozen(CreateObjectAt("IronBlock", LocationX(113), LocationY(113)), true);
    Frozen(CreateObjectAt("IronBlock", LocationX(114), LocationY(114)), true);
    Frozen(blocks, true);
    int u;

    for (u = 0 ; u < 4 ; u += 1)
        LookWithAngle(blocks + u, 99);
    
    ObjectOff(blocks);

    return blocks;
}

void BackBlockMovingToRight(int block)
{
    int remain = GetDirection(block);

    if (remain)
    {
        FrameTimerWithArg(1, block, BackBlockMovingToRight);
        MoveObjectVector(block, 2.0, 2.0);
        LookWithAngle(block, --remain);
    }
}

void BackBlockMovingToLeft(int block)
{
    int remain = GetDirection(block);

    if (remain)
    {
        FrameTimerWithArg(1, block, BackBlockMovingToLeft);
        MoveObjectVector(block, -2.0, 2.0);
        LookWithAngle(block, --remain);
    }
}

void StartBackBlocks()
{
    int blocks = g_backBlocks;

    ObjectOff(self);
    if (!IsObjectOn(blocks))
    {
        FrameTimerWithArg(3, blocks, BackBlockMovingToRight);
        FrameTimerWithArg(3, blocks + 1, BackBlockMovingToRight);
        FrameTimerWithArg(3, blocks + 2, BackBlockMovingToRight);
    }
}

void StartBackBlocks2()
{
    int blocks = g_backBlocks + 3;

    ObjectOff(self);
    if (!IsObjectOn(blocks))
    {
        FrameTimerWithArg(3, blocks, BackBlockMovingToLeft);
        FrameTimerWithArg(3, blocks + 1, BackBlockMovingToLeft);
        FrameTimerWithArg(3, blocks + 2, BackBlockMovingToLeft);
    }
}

int InitBackBlocks()
{
    int blocks = CreateObjectAt("SpikeBlock", LocationX(115), LocationY(115));

    CreateObjectAt("SpikeBlock", LocationX(116), LocationY(116));
    CreateObjectAt("SpikeBlock", LocationX(117), LocationY(117));

    ObjectOff(CreateObjectAt("SpikeBlock", LocationX(119), LocationY(119)));
    CreateObjectAt("SpikeBlock", LocationX(120), LocationY(120));
    CreateObjectAt("SpikeBlock", LocationX(121), LocationY(121));
    ObjectOff(blocks);

    int u;
    for (u = 0 ; u < 6 ; u += 1)
        LookWithAngle(blocks + u, 162);

    return blocks;
}

void InitLastPart()
{
    g_frontBlocks = InitSecondBlocks();
    g_backBlocks = InitBackBlocks();
}

void EntryArea9()
{
    //Todo.
    MobMakerStartSummon(1, PlaceMobMaker(122, 20, SummonBeast));
    MobMakerStartSummon(1, PlaceMobMaker(129, 20, SummonBeast));
    MobMakerStartSummon(1, PlaceMobMaker(130, 20, SummonBeast));
    MobMakerStartSummon(1, PlaceMobMaker(123, 20, SummonMobWasp));
    MobMakerStartSummon(1, PlaceMobMaker(124, 20, SummonMobWasp));
    MobMakerStartSummon(1, PlaceMobMaker(131, 20, SummonMobBlackSpider));
}

void EndArea9()
{
    FrameTimer(1, EntryAreaA);

    UnlockDoor(Object("Area9Gate"));
    UnlockDoor(Object("Area9Gate1"));

    InitPlacingGenerator();
}

void EndArea8()
{
    FrameTimer(1, EntryArea9);

    int u;

    for (u = 0 ; u < 3 ; u += 1)
    {
        UnlockDoor(Object("A6Gate" + IntToString(u + 1)));
        UnlockDoor(Object("A6Gate" + IntToString(u + 1) + "1"));
    }
    UniPrintToAll("출입문의 잠금이 해제되었습니다");
}

void EntryArea8()
{
    MobMakerStartSummon(1, PlaceMobMaker(109, 30, SummonMobStrongWizard));
    MobMakerStartSummon(1, PlaceMobMaker(108, 30, SummonMobPlant));
    MobMakerStartSummon(1, PlaceMobMaker(128, 10, SummonMobLich));

    SetNextFunction(EndArea8);
}

void EndAreaA()
{ }

void EntryAreaA()
{
    SetNextFunction(EndAreaA);

    MobMakerStartSummon(1, PlaceMobMaker(137, 14, SummonMobEmberDemon));
    MobMakerStartSummon(1, PlaceMobMaker(138, 14, SummonMobSkullLord));
    MobMakerStartSummon(1, PlaceMobMaker(139, 8, SummonMobLich));
}

void RemoveArea9EntryWalls()
{
    ObjectOff(self);

    RemoveWallAtObjectPos(133);
    RemoveWallAtObjectPos(134);
    int u;

    for (u = 0 ; u < 6 ; u += 1)
    {
        RemoveWallAtObjectPos(132);
        TeleportLocationVector(132, -23.0, 23.0);
    }
}

void RemoveBackBlockWalls()
{
    int u;

    for (u = 0 ; u < 6 ; u += 1)
    {
        if (u < 4)
        {
            RemoveWallAtObjectPos(125);
            TeleportLocationVector(125, -23.0, 23.0);
            if (u < 3)
            {
                RemoveWallAtObjectPos(127);
                TeleportLocationVector(127, 23.0, -23.0);
            }
        }
        RemoveWallAtObjectPos(126);
        TeleportLocationVector(126, 23.0, 23.0);
    }
}

void BackBlockClearWallTriggered()
{
    int alreadyExec;

    ObjectOff(SELF);
    if (alreadyExec == false)
    {
        RemoveBackBlockWalls();
        alreadyExec = true;
    }
}

int FireTrapReference(int *st, int *trap, int assign)
{
    int *tst, *ttrap;

    if (assign)
    {
        tst = st;
        ttrap = trap;
    }
    else
    {
        if (tst != NULLPTR && ttrap != NULLPTR)
        {
            st[0] = tst;
            trap[0] = ttrap;
            return true;
        }
    }
    return false;
}

void InitFireTraps()
{
    int statues[] = {
        CreateObjectAt("MovableStatueVictory3SW", LocationX(135), LocationY(135)), CreateObjectAt("MovableStatueVictory3NE", LocationX(136), LocationY(136))};
    int traps[] = {CreateObjectAtUnit("Skull1", statues[0]), CreateObjectAtUnit("Skull1", statues[1])};

    UnitNoCollide(statues[0]);
    UnitNoCollide(statues[1]);
    ObjectOff(traps[0]);
    ObjectOff(traps[1]);

    FireTrapReference(&statues, &traps, true);
}

void DisableObject(int unit)
{
    if (IsObjectOn(unit))
        ObjectOff(unit);
}

void FireballStatueFx(int *pack)
{
    int *time = pack[1];

    if (time[0] > 0)
    {
        time[0] -= 1;
        FrameTimerWithArg(3, pack, FireballStatueFx);
    }
    else
    {
        int *statue = pack[0];

        EnchantOff(statue[0], EnchantList(ENCHANT_PROTECT_FROM_FIRE));
        EnchantOff(statue[1], EnchantList(ENCHANT_PROTECT_FROM_FIRE));
    }
}

void LaunchFireballFromTrap()
{
    int *statue = NULLPTR, *trap = NULLPTR;
    
    if (!FireTrapReference(&statue, &trap, false))
        return;

    int u, time;

    for (u = 0 ; u < 2 ; u += 1)
    {
        ObjectOff(trap[u]);
        LookAtObject(trap[u], OTHER);
        ObjectOn(trap[u]);
        FrameTimerWithArg(1, trap[u], DisableObject);
    }

    int pack[] = {statue, &time};

    if (!time)
    {
        Enchant(statue[0], EnchantList(ENCHANT_PROTECT_FROM_FIRE), 0.0);
        Enchant(statue[1], EnchantList(ENCHANT_PROTECT_FROM_FIRE), 0.0);
        FrameTimerWithArg(1, &pack, FireballStatueFx);
    }
    time = 30;
}

int GeneratorUnitCreate(string unitType, int hpAmount, int subunit)
{
    int mob = CreateObjectAt(unitType, GetObjectX(subunit), GetObjectY(subunit));

    SetUnitMaxHealth(mob, hpAmount);
    Enchant(mob, EnchantList(ENCHANT_ANCHORED), 0.0);
    return mob;
}

void MonsterObeliskOnDestroy()
{
    int subunit = GetTrigger() + 1;

    if (IsObjectOn(subunit))
    {
        int gentype = GetDirection(subunit), u;
        int amount = ObeliskTypeAmount(gentype);
        int hpamount = ObeliskTypeHitPoint(gentype);
        string unitType = ObeliskTypes(gentype);

        for (u = 0 ; u < amount ; u += 1)
            GeneratorUnitCreate(unitType, hpamount, subunit);
        DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(subunit), GetObjectY(subunit)), 9);
        Delete(subunit);
    }
    Delete(SELF);
}

#define MON_GEN_MAX         16

string ObeliskTypes(int index)
{
    string types[MON_GEN_MAX] =
    {
        "Horrendous", "Skeleton", "Wizard", "WizardGreen",
        "EmberDemon", "Troll", "Mimic", "Beholder",
        "Swordsman", "EvilCherub", "Bear", "UrchinShaman",
        "StoneGolem", "MechanicalGolem", "Demon", "OgreBrute"
    };
    return types[index];
}

int ObeliskTypeHitPoint(int index)
{
    int hptable[MON_GEN_MAX] =
    {
        600, 225, 275, 225,
        225, 360, 800, 325,
        325, 192, 421, 198,
        1200, 1300, 1080, 295
    };
    return hptable[index];
}

int ObeliskTypeAmount(int index)
{
    int amountTable[MON_GEN_MAX] =
    {
        2, 8, 4, 4,
        4, 4, 2, 4,
        16, 32, 4, 4,
        2, 2, 2, 8
    };
    return amountTable[index];
}

int PlaceMonsterObelisk(int location, int type)
{
    int oblisk = CreateObjectAt(ObeliskTypes(type) + "Generator", LocationX(location), LocationY(location));
    int *ptr = 0x750710;

    LookWithAngle(CreateObjectAt("AmbBeachBirds", LocationX(location), LocationY(location)), type);
    ptr = ptr[0];
    SetUnitMaxHealth(oblisk, 744);
    ObjectOff(oblisk);
    ptr[181] = ImportUnitDieFunc();
    ptr[191] = MonsterObeliskOnDestroy;

    return oblisk;
}

void InitPlacingGenerator()
{
    int u;

    for (u = 0 ; u < MON_GEN_MAX ; u += 1)
        PlaceMonsterObelisk(140 + u, u);
}

void OnClearGen()
{
    int count, complete;

    ObjectOff(SELF);
    if ((++count) >= MON_GEN_MAX)
    {
        if (complete)
            return;
        complete = true;
        int u;

        for (u = 0 ; u < 5 ; u += 1)
        {
            RemoveWallAtObjectPos(156);
            TeleportLocationVector(156, 23.0, 23.0);
        }
        UniPrintToAll("다음 구간으로 향하는 벽이 열렸습니다");
    }
}