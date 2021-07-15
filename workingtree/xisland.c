
#include "noxscript\builtins.h"
#include "libs\typecast.h"
#include "libs\callmethod.h"
#include "libs\unitutil.h"
#include "libs\printutil.h"
#include "libs\unitstruct.h"
#include "libs\reaction.h"
#include "libs\spellutil.h"
#include "libs\buff.h"
#include "libs\mathlab.h"
#include "libs\fxeffect.h"
#include "libs\waypoint.h"
#include "libs\username.h"
#include "libs\playerupdate.h"
#include "libs\coopteam.h"
#include "libs\itemproperty.h"

int player[20];
int INDEX;
int MOBS[90];
int DEATHS = 90;
int UPGRADE[10];

#define OTHER -1
#define SELF -2

#define NULLPTR         0

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
#define ENCHANT_INVULNERABLE        23
#define ENCHANT_TELEKINESIS         24
#define ENCHANT_FREEZE              25
#define ENCHANT_SHIELD              26
#define ENCHANT_REFLECTIVE_SHIELD   27
#define ENCHANT_CHARMING            28
#define ENCHANT_ANTI_MAGIC          29
#define ENCHANT_CROWN               30
#define ENCHANT_SNEAK               31

#define TRUE 1
#define FALSE 0

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

int CheckMonsterThing(int unit)
{
    int arr[97], init;
    int thingID = GetUnitThingID(unit);
    int key = thingID % 97;

    if (!init)
    {
        init = 1;
        arr[5] = MonsterGoonProcess; arr[72] = MonsterStrongWhiteWizProcess; arr[30] = MonsterWeirdlingBeastProcess; arr[34] = MonsterBlackWidowProcess; arr[6] = MonsterBear2Process;
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
        SetUnitMaxHealth(unit, 275);
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
        //SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(34));       //HorvathVoice
        SetUnitMaxHealth(unit, 200);
        UnitZeroFleeRange(unit);
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

void InitPickets()
{
    RegistSignMessage(Object("PicketHorr"), "호렌더스의 체력을 회복시켜줍니다. 가격은 2만 골드입니다");
    RegistSignMessage(Object("PicketStartGame"), "게임 시작버튼입니다. 최대 20 라운드 까지 있습니다");
    RegistSignMessage(Object("PicketUpgrade"), "창고 업그레이드 입니다. 가격은 만 골드입니다");
    RegistSignMessage(Object("PicketStaff"), "망각 지팡이 구입입니다. 가격은 15000 골드입니다");
}

void MapExit()
{
    ResetPlayerHandlerWhenExitMap();
    RemoveCoopTeamMode();
    MusicEvent();
}

static int InitMathSineBaseWaypointNumber() { return 42; }

void MapInitialize()
{
    MusicEvent();
    InitPickets();
    GetCaptain();
    ParentNode();

    //loop_run
    FrameTimer(10, LoopPreservePlayers);
    FrameTimer(2, loopReventCharm);
    FrameTimer(1, MakeCoopTeam);
    FrameTimer(3, loopHorrendousStatus);

    //delay_run
    // FrameTimer(10, strCureHealth);
    // FrameTimer(11, strStartButton);
    // FrameTimer(12, strUpgradeHome);
    // FrameTimer(13, strPlasmaStaff);
    FrameTimerWithArg(30, 13, callTurboTrigger);
    // FrameTimer(150, PutShopInfoStamp);
    FrameTimer(160, ShowHorrendousHP);
}

int PlayerClassOnInit(int plr, int pUnit)
{
    player[plr] = pUnit;
    player[plr + 10] = 1;
    UPGRADE[plr] = 0;

    SelfDamageClassEntry(pUnit);
    DiePlayerHandlerEntry(pUnit);

    ChangeGold(pUnit, -GetGold(pUnit));

    return plr;
}

void PlayerClassOnEntry()
{
    int plr, i;

    if (CurrentHealth(OTHER))
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
            EntryPlayer(plr);
        else
        {
            CantPlayerEntry();
        }
        
    }
}

void EntryPlayer(int plr)
{
    int location = Random(5, 8), pUnit = player[plr];

    if (PlayerClassCheckDeathFlag(plr))
        PlayerClassSetDeathFlag(plr);
    Enchant(pUnit, "ENCHANT_ANCHORED", 0.0);
    MoveObject(pUnit, LocationX(location), LocationY(location));
    DeleteObjectTimer(CreateObject("BlueRain", location), 10);
    AudioEvent("BlindOff", location);
}

void CantPlayerEntry()
{
    UniPrintToAll("맵이 수용할 수 있는 플레이어 최대 수를 넘었기 때문에 더 이상 입장하실 수 없습니다.");
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    MoveObject(other, GetWaypointX(40), GetWaypointY(40));
}

void PlayerClassOnAlive(int plr, int pUnit)
{
    if (UnitCheckEnchant(pUnit, GetLShift(ENCHANT_SNEAK)))
    {
        RemoveTreadLightly(pUnit);
        EnchantOff(pUnit, EnchantList(ENCHANT_SNEAK));
        CastWindbooster(pUnit);
    }
    if (CheckPlayerInput(pUnit) == 7)   //jump
    {
        CastDeathCharging(pUnit);
    }
}

void PlayerClassOnExit(int plr)
{
    int *ptr = UnitToPtr(player[plr]);

    if (ptr != NULLPTR)
        CancelPlayerDialogWithPTR(ptr);
    player[plr] = 0;
    player[plr + 10] = 0;
}

void PlayerOnDeath(int plr)
{
    int pUnit = player[plr];

    UniPrintToAll(PlayerIngameNick(pUnit) + " 님께서 적에게 격추당하셨습니다");
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
                        PlayerOnDeath(i);
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

void CastWindbooster(int plrUnit)
{
    Effect("RICOCHET", GetObjectX(plrUnit), GetObjectY(plrUnit), 0.0, 0.0);
    PushObjectTo(plrUnit, UnitAngleCos(plrUnit, 75.0), UnitAngleSin(plrUnit, 75.0));
}

void CastTripleArrowShot(int plrUnit)
{
    float xVect = UnitAngleCos(plrUnit, -20.0), yVect = UnitAngleSin(plrUnit, -20.0);
    int k, missile;

    TeleportLocation(11, GetObjectX(plrUnit) + (1.0 / 1.6 * yVect) - xVect, GetObjectY(plrUnit) - (1.0 / 1.6 * xVect) - yVect);
    
    for (k = 0 ; k < 9 ; k += 1)
    {
        xVect = GetRatioUnitWpXY(plrUnit, 11, 0, 23.0);
        yVect = GetRatioUnitWpXY(plrUnit, 11, 1, 23.0);

        TeleportLocation(11, GetObjectX(plrUnit) - (1.0 / 8.0 * yVect) - xVect, GetObjectY(plrUnit) + (1.0 / 8.0 * xVect) - yVect);
        missile = CreateObject("ArcherArrow", 11);
        LookAtObject(missile, plrUnit);
        LookWithAngle(missile, GetDirection(missile) + 128);
        SetOwner(plrUnit, missile);
        PushObject(missile, 35.0, GetObjectX(plrUnit), GetObjectY(plrUnit));
    }
}

void PowerChargingCollide()
{
    int owner = GetOwner(SELF);

    while (TRUE)
    {
        if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
        {
            Damage(OTHER, owner, 75, 14);
            Enchant(OTHER, EnchantList(ENCHANT_CHARMING), 0.2);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(SELF);
        break;
    }
}

int PowerChargingCreateSubUnit(int owner, float xProfile, float yProfile)
{
    int subUnit = CreateObjectAt("CarnivorousPlant", xProfile, yProfile);

    ObjectOff(subUnit);
    Damage(subUnit, 0, MaxHealth(subUnit) + 1, -1);
    Frozen(subUnit, TRUE);
    SetOwner(owner, subUnit);
    DeleteObjectTimer(subUnit, 1);

    return subUnit;
}

void PowerChargingRelay(int subUnit)
{
    int owner = GetOwner(subUnit), count = GetDirection(subUnit);

    while (IsObjectOn(subUnit))
    {
        if (CurrentHealth(owner) && count)
        {
            FrameTimerWithArg(1, subUnit, PowerChargingRelay);
            LookWithAngle(subUnit, --count);
            SetCallback(PowerChargingCreateSubUnit(owner, GetObjectX(owner) - UnitAngleCos(owner, 20.0), GetObjectY(owner) - UnitAngleSin(owner, 20.0)), 9, PowerChargingCollide);
            PlaySoundAround(owner, 297);
            Effect("YELLOW_SPARKS", GetObjectX(owner), GetObjectY(owner), 0.0, 0.0);
            break;
        }
        Delete(subUnit);
        break;
    }
}

void CastDeathCharging(int plrUnit)
{
    if (MaxHealth(plrUnit) == 150 && !UnitCheckEnchant(plrUnit, GetLShift(ENCHANT_AFRAID)))
    {
        Enchant(plrUnit, EnchantList(ENCHANT_AFRAID), 15.0);
        PlaySoundAround(plrUnit, 900);
        int subUnit = CreateObjectAt("AmbBeachBirds", GetObjectX(plrUnit), GetObjectY(plrUnit));
        SetOwner(plrUnit, subUnit);
        LookWithAngle(subUnit, 16);
        FrameTimerWithArg(1, subUnit, PowerChargingRelay);
    }
}

void TeleportPlayerAt(int wp)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (CurrentHealth(player[i]))
            MoveObject(player[i], GetWaypointX(wp), GetWaypointY(wp));
    }
}

void ChangeUnitAffiliatedTeam(int unit, int teamId)
{
    int *ptr = UnitToPtr(unit);

    if (ptr != NULLPTR)
        ptr[13] = teamId;
}

int GetCaptain()
{
    int unit;
    
    if (!unit)
    {
        unit = CreateObject("Horrendous", 3);
        SetUnitHealth(unit, 30000);
        SetCallback(unit, 3, GiveWeaponToHorrendous);
        SetCallback(unit, 5, HorrendousDie);
        SetDialog(unit, "NORMAL", TeleportPlayerHome, NullPointer);
        // SetOwner(GetHost(), unit);
        ChangeUnitAffiliatedTeam(unit, 1);
    }

    return unit;
}

void NullPointer()
{
    //
}

void TeleportPlayerHome()
{
    int i;

    if (!HasEnchant(other, "ENCHANT_DETECTING"))
    {
        UniChatMessage(self, "저를 한번 더 클릭하면 창고로 이동됩니다.", 150);
        Enchant(other, "ENCHANT_DETECTING", 1.0);
    }
    else
    {
        i = CheckPlayer();
        if (i >= 0)
        {
            Enchant(other, "ENCHANT_RUN", 0.1);
            MoveObject(other, GetWaypointX(HomeTeleportNum(UPGRADE[i])), GetWaypointY(HomeTeleportNum(UPGRADE[i])));
            DeleteObjectTimer(CreateObject("BlueRain", HomeTeleportNum(UPGRADE[i])), 10);
            AudioEvent("BlindOff", HomeTeleportNum(UPGRADE[i]));
            UniChatMessage(other, HomeName(UPGRADE[i]) + " 창고로 이동했습니다.", 150);
        }
    }
}

int HomeTeleportNum(int num)
{
    int arr[] = {25, 26, 27, 22};

    return arr[num % 4];
}

string HomeName(int num)
{
    string arr[] = {"초급", "중급", "상급", "최고급"};
        
    return arr[num % 4];
}

void UpgradeHome()
{
    int i;

    if (GetGold(other) >= 10000)
    {
        i = CheckPlayer();
        if (UPGRADE[i] < 3 && i >= 0)
        {
            UPGRADE[i] ++;
            MoveWaypoint(37, GetObjectX(other), GetObjectY(other));
            ChangeGold(other, -10000);
            AudioEvent("AwardGuide", 37);
            Effect("WHITE_FLASH", GetObjectX(self), GetObjectY(self), 0.0, 0.0);
            UniPrintToAll("창고가 업그레이드 되었습니다_!");
        }
    }
}

int DummyUnitCreateAt(string name, float xProfile, float yProfile)
{
    int unit = CreateObjectAt(name, xProfile, yProfile);

    if (CurrentHealth(unit))
    {
        ObjectOff(unit);
        Damage(unit, 0, MaxHealth(unit) + 1, -1);
        Frozen(unit, 1);
    }
    return unit;
}

void BurnningZombieCollide()
{
    int owner = GetOwner(SELF);

    while (TRUE)
    {
        if (CurrentHealth(OTHER) && IsAttackedBy(OTHER, owner))
        {
            Damage(OTHER, owner, 170, 14);
            Enchant(OTHER, "ENCHANT_FREEZE", 1.0);
            Enchant(OTHER, EnchantList(ENCHANT_CHARMING), 0.2);
        }
        else if (!GetCaller())
            1;
        else
            break;
        Delete(SELF);
        break;
    }
}

int FxBurnningZombie(float xProfile, float yProfile)
{
    int fxU = CreateObjectAt("Zombie", xProfile, yProfile);

    UnitNoCollide(fxU);
    ObjectOff(fxU);
    Damage(fxU, 0, MaxHealth(fxU) + 1, 1);
    return fxU;
}

void BurnningSunFlying(int subunit)
{
    int durate = GetDirection(subunit);

    while (IsObjectOn(subunit))
    {
        int owner = GetOwner(subunit);

        if (CurrentHealth(owner) && durate)
        {
            FrameTimerWithArg(1, subunit, BurnningSunFlying);
            LookWithAngle(subunit, durate - 1);
            int coll = DummyUnitCreateAt("Troll", GetObjectX(subunit), GetObjectY(subunit));
            SetCallback(coll, 9, BurnningZombieCollide);
            SetOwner(owner, coll);
            DeleteObjectTimer(coll, 1);
            DeleteObjectTimer(FxBurnningZombie(GetObjectX(subunit), GetObjectY(subunit)), 18);
            MoveObject(subunit, GetObjectX(subunit) + GetObjectZ(subunit), GetObjectY(subunit) + GetObjectZ(subunit + 1));
            MoveObject(subunit + 1, GetObjectX(subunit + 1) + GetObjectZ(subunit), GetObjectY(subunit + 1) + GetObjectZ(subunit + 1));
            if (IsVisibleTo(subunit + 1, subunit))
                break;
        }
        Delete(subunit);
        break;
    }
}

void OblivionUseHandler()
{
    int cFps = GetMemory(0x84ea04);
    int cTime = GetUnit1C(SELF);

    if (MathAbs(cFps - cTime) < 20)
        return;
    if (CurrentHealth(OTHER))
    {
        SetUnit1C(SELF, cFps);
        float xVect = UnitAngleCos(OTHER, 19.0), yVect = UnitAngleSin(OTHER, 19.0);
        int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(OTHER) + xVect, GetObjectY(OTHER) + yVect);

        Raise(unit, xVect);
        Raise(CreateObjectAt("InvisibleLightBlueLow", GetObjectX(unit) - xVect, GetObjectY(unit) - yVect), yVect);
        SetOwner(OTHER, unit);
        LookWithAngle(unit, 32);
        FrameTimerWithArg(1, unit, BurnningSunFlying);
        PlaySoundAround(OTHER, 221);
    }
}

int SummonOblivionStaff(float sX, float sY)
{
    int unit = CreateObjectAt("OblivionOrb", sX, sY);
    int ptr = GetMemory(0x750710);

    if (ptr)
    {
        int thingId = GetUnitThingID(unit);

        if (thingId >= 222 && thingId <= 225)
        {
            SetMemory(ptr + 0x2c4, 0x53a720);
            SetMemory(ptr + 0x2c8, ImportAllowAllDrop());   //here
        }
        else
        {
            return 0;
        }
        
        SetUnitCallbackOnUseItem(unit, OblivionUseHandler);
    }
    return unit;
}

void BuyObilivStaff()
{
    if (GetGold(OTHER) >= 30000)
    {
        ChangeGold(OTHER, -30000);

        int staff = SummonOblivionStaff(GetObjectX(SELF), GetObjectY(SELF));

        if (staff)
            PlaySoundAround(staff, 226);
        else
        {
            UniPrint(OTHER, "something happen wrong");
        }
        
        Effect("WHITE_FLASH", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
        Effect("CYAN_SPARKS", GetObjectX(SELF), GetObjectY(SELF), 0.0, 0.0);
        UniPrint(OTHER, "망각의 지팡이를 구입하셨습니다.");
    }
    else
        UniPrint(OTHER, "거래실패! 금액이 모자랍니다. 망각의 지팡이는 3만 골드입니다");
}

void SetInfinite()
{
    int cur = GetLastItem(other);
    int count = 0;

    while (IsObjectOn(cur))
    {
        if ((HasClass(cur, "WEAPON") || HasClass(cur, "ARMOR")) && !HasEnchant(cur, "ENCHANT_INVULNERABLE"))
        {
            count ++;
            Enchant(cur, "ENCHANT_INVULNERABLE", 0.0);
        }
        cur = GetPreviousItem(cur);
    }
    if (count > 0)
    {
        UniPrint(OTHER, "처리결과, 총 " + IntToString(count) + "개의 아이템을 무적화 했습니다.");
        Enchant(other, "ENCHANT_FREEZE", 1.0);
    }
}

void outItemRoom()
{
    Enchant(other, "ENCHANT_FREEZE", 1.5);
    MoveObject(other, GetWaypointX(3), GetWaypointY(3));
    AudioEvent("BlindOff", 3);
}

void goSelectRoom()
{
    if (DEATHS == 90)
    {
        Enchant(other, "ENCHANT_FREEZE", 2.0);
        MoveObject(other, GetWaypointX(30), GetWaypointY(30));
    }
    else
    {
        UniPrint(OTHER, "Debug::" + IntToString(DEATHS));
    }
    
}

void setHorrendousHealthAdd()
{
    if (GetGold(other) >= 20000 && CurrentHealth(GetCaptain()) < 25000)
    {
        ChangeGold(other, -20000);
        SetUnitHealth(GetCaptain(), CurrentHealth(GetCaptain()) + 5000);
        UniPrintToAll("호렌더스의 현재체력을 5000 만큼 회복시켰습니다. 현재체력: " + IntToString(CurrentHealth(GetCaptain())));
    }
}

void StartCurrentStage()
{
    if (DEATHS == 90)
    {
        DEATHS = 0;
        Enchant(other, "ENCHANT_RUN", 0.1);
        TeleportPlayerAt(3);
        UniPrintToAll("시작버튼을 누르셨습니다, 잠시 후 스테이지" + IntToString(GetStage(1)) + " 이 시작됩니다.");
        FrameTimer(150, ControlStage);
    }
}

void ControlStage()
{
    if (GetStage(0) == 20)
    {
        UniPrintToAll("승리__ 20 개의 모든 스테이지를 클리어 하셨습니다_!!");
        TeleportPlayerAt(3);
        AudioEvent("FlagCapture", 3);
        Effect("WHITE_FLASH", GetWaypointX(3), GetWaypointY(3), 0.0, 0.0);
        MoveWaypoint(3, 2465.0, 2321.0);
        FrameTimer(3, StrVictory);
    }
    else
        FrameTimer(150, SpawnMonster);
}

void loopHorrendousStatus()
{
    if (CurrentHealth(GetCaptain()))
    {
        if (Distance(GetObjectX(GetCaptain()), GetObjectY(GetCaptain()), GetWaypointX(3), GetWaypointY(3)) > 300.0)
        {
            Effect("TELEPORT", GetObjectX(GetCaptain()), GetObjectY(GetCaptain()), 0.0, 0.0);
            MoveObject(GetCaptain(), GetWaypointX(3), GetWaypointY(3));
            Effect("TELEPORT", GetObjectX(GetCaptain()), GetObjectY(GetCaptain()), 0.0, 0.0);
        }
        FrameTimer(10, loopHorrendousStatus);
    }
}

void HorrendousDie()
{
    UniPrintToAll("임무실패__!! 방금 호렌더스가 적에게 격추되었습니다.");
    UniPrintToAll("생존자들은 호렌더스를 지켜내지 못했습니다");
    MoveObject(Object("PlayerStartPic"), GetWaypointX(36), GetWaypointY(36));
    TeleportPlayerAt(36);
    AudioEvent("ManaBombEffect", 36);
    FrameTimer(3, strMissionFail);
}

int WaveClassCreate(int location)
{
    return CreateObject(MonsterName(GetStage(0) - 1), location);
}

void SpawnMonster()
{
    int i;

    for (i = 0 ; i < 90 ; i +=1)
    {
        TeleportLocation(9, LocationX(3) + MathSine(i * 4, 1000.0), LocationY(3) + MathSine(i * 4 + 90, 1000.0));
        MOBS[i] = WaveClassCreate(9);
        CheckMonsterThing(MOBS[i]);
        // SetUnitMaxHealth(MOBS[i], 1000 + MonsterHealth(GetStage(0) - 1));
        SetUnitMaxHealth(MOBS[i], MonsterHealth(GetStage(0) - 1));
        SetCallback(MOBS[i], 5, SetDeaths);
        // SetCallback(MOBS[i], 7, RiskMonster);
        SetCallback(MOBS[i], 9, CollisionEvent);
        SetOwner(ParentNode(), MOBS[i]);
        RetreatLevel(MOBS[i], 0.0);
        ResumeLevel(MOBS[i], 1.0);
    }
    FrameTimer(1, GoAttackToCenter);
}

void ExceptionProperty(int unit)
{
    if (GetStage(0) - 1 == 15)
    {
        CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit) - 36.0, GetObjectY(unit) - 36.0, 400.0);
        SetCallback(unit, 3, GiveWeaponToRedWiz);
    }
}

void GiveWeaponToRedWiz()
{
    if (CurrentHealth(self) > 0)
    {
        if (CurrentHealth(other) > 0)
        {
            Effect("SENTRY_RAY", GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other));
            Damage(other, self, 15, 17);
        }
        Enchant(self, "ENCHANT_BLINDED", 0.2);
        AggressionLevel(self, 1.0);
    }
}

void GoAttackToCenter()
{
    int i;

    for (i = 0 ; i < 90 ; i +=1)
    {
        if (CurrentHealth(MOBS[i]))
        {
            CreatureFollow(MOBS[i], GetCaptain());
            AggressionLevel(MOBS[i], 1.0);
        }
    }
}

void loopReventCharm()
{
    int i, uIndex;

    if (uIndex == 90)
        uIndex = 0;

    for (i = 9 ; i >= 0 ; i -=1)
    {
        if (CurrentHealth(MOBS[uIndex]))
        {
            if (HasEnchant(MOBS[uIndex], "ENCHANT_CHARMING"))
            {
                MoveObject(CharmLight(), GetObjectX(MOBS[uIndex]), GetObjectY(MOBS[uIndex]));
                EnchantOff(MOBS[uIndex], "ENCHANT_CHARMING");
                CastSpellObjectObject("SPELL_COUNTERSPELL", CharmLight(), MOBS[uIndex]);
            }
        }
        uIndex += 1;
    }
    FrameTimer(1, loopReventCharm);
}

void RiskMonster()
{
    int owner;

    if (CurrentHealth(self) <= 1000)
    {
        owner = CheckOwner(other);
        if (owner >= 0 && !IsCaller(GetCaptain()))
            ChangeGold(player[owner], Random(100, 400));
        Damage(self, other, 1000, 14);
    }
}

void CollisionEvent()
{
    int owner;

    if (IsMissileUnit(OTHER))
    {
        if (GetUnitThingID(OTHER) == 526)
        {
            owner = GetOwner(OTHER);
            if (owner)
            {
                DeleteObjectTimer(CreateObjectAt("ForceOfNatureCharge", GetObjectX(SELF), GetObjectY(SELF)), 10);
                PlaySoundAround(SELF, 226);
                Delete(OTHER);
                Damage(SELF, owner, 250, 14);
            }
        }
    }
}

int GetUnitTopParent(int unit)
{
    int next;

    while (TRUE)
    {
        next = GetOwner(unit);
        if (next)
            unit = next;
        else
            break;
    }
    return unit;
}

int GetKillCreditTopParent()
{
    int *victim = GetMemory(0x979724);

    if (victim != NULLPTR)
    {
        int *attacker = victim[130];

        if (attacker != NULLPTR)
            return GetUnitTopParent(attacker[11]);
    }
    return NULLPTR;
}

void KillEvent(int me)
{
    int kill = GetKillCreditTopParent();

    if (CurrentHealth(kill))
    {
        if (IsPlayerUnit(kill))
            ChangeGold(kill, Random(100, 400));
    }
}

void SetDeaths()
{
    if (++DEATHS == 90)
    {
        UniPrintToAll("이번 스테이지 완료!");
        PlaySoundAround(GetCaptain(), 777);
    }
    KillEvent(GetTrigger());
    DeleteObjectTimer(self, 30);
}

void GiveWeaponToHorrendous()
{
    if (CurrentHealth(self))
    {
        if (CurrentHealth(other))
        {
            Effect("LIGHTNING", GetObjectX(self), GetObjectY(self), GetObjectX(other), GetObjectY(other));
            Damage(other, self, 128, 17);
        }
        Enchant(self, "ENCHANT_BLINDED", 0.3);
        AggressionLevel(self, 1.0);
    }
}

int GetStage(int mode)
{
    int stage;

    if (mode == 1)
        stage ++;
    return stage;
}

int CharmLight()
{
    int unit;

    if (!unit)
        unit = CreateObject("InvisibleLightBlueHigh", 3);
    return unit;
}

string MonsterName(int num)
{
    string name[] = {
        "Bat", "SmallAlbinoSpider", "GiantLeech", "Imp", "Swordsman",
        "WhiteWolf", "Scorpion", "Skeleton", "OgreBrute", "Bear",
        "SkeletonLord", "OgreWarlord", "MeleeDemon", "Mimic", "Shade",
        "WeirdlingBeast", "Goon", "Bear2", "MechanicalGolem", "BlackWidow"
        };
    return name[num];
}

int MonsterHealth(int num)
{
    int hpTable[20] = {
        30, 60, 90, 120, 150,
        180, 200, 220, 250, 275,
        290, 310, 320, 340, 360,
        350, 370, 400, 420, 450
    };

    return hpTable[num];
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

int CheckPlayer()
{
    int i;

    for (i = 9 ; i >= 0 ; i -=1)
    {
        if (IsCaller(player[i]))
            return i;
    }
    return -1;
}

void SetUnitHealth(int unit, int amount)
{
    Damage(unit, 0, CurrentHealth(unit) - 1, -1);
    Pickup(unit, CreateObject("RottenMeat", 4));
    Damage(unit, 0, CurrentHealth(unit) - amount, -1);
}

void PutShopInfoStamp()
{
    int i;

    if (i < 4)
    {
        if (i == 1)
            MoveWaypoint(38, 2541.0, 4323.0);
        else if (i == 2)
            MoveWaypoint(38, 4416.0, 2450.0);
        else if (i == 3)
            MoveWaypoint(38, 2679.0, 688.0);
        i ++;
        FrameTimer(1, strEntryShop);
        FrameTimer(3, PutShopInfoStamp);
    }
}

void StrVictory()
{
	int arr[13];
	string name = "HealOrb";
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
		pos_x = GetWaypointX(3);
		pos_y = GetWaypointY(3);
	}
	for (i = 1 ; i > 0 && count < 403 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 3);
		if (count % 38 == 37)
			MoveWaypoint(3, GetWaypointX(3) - 74.000000, GetWaypointY(3) + 2.000000);
		else
			MoveWaypoint(3, GetWaypointX(3) + 2.000000, GetWaypointY(3));
		count ++;
	}
	if (count >= 403)
	{
		count = 0;
		MoveWaypoint(3, pos_x, pos_y);
	}
}

void strMissionFail()
{
	int arr[17];
	string name = "HealOrb";
	int i = 0;
	arr[0] = 272613948; arr[1] = 75782084; arr[2] = 537411713; arr[3] = 1090654737; arr[4] = 606245922; arr[5] = 675873800; arr[6] = 267423498; arr[7] = 1352002; arr[8] = 1073872898; arr[9] = 114; 
	arr[10] = 1889865712; arr[11] = 268697375; arr[12] = 1075856020; arr[13] = 86540160; arr[14] = 16842881; arr[15] = 33428976; arr[16] = 671349764; 
	while(i < 17)
	{
		drawstrMissionFail(arr[i], name);
		i ++;
	}
}

void drawstrMissionFail(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(34);
		pos_y = GetWaypointY(34);
	}
	for (i = 1 ; i > 0 && count < 527 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 34);
		if (count % 48 == 47)
			MoveWaypoint(34, GetWaypointX(34) - 94.000000, GetWaypointY(34) + 2.000000);
		else
			MoveWaypoint(34, GetWaypointX(34) + 2.000000, GetWaypointY(34));
		count ++;
	}
	if (count >= 527)
	{
		count = 0;
		MoveWaypoint(34, pos_x, pos_y);
	}
}

void strEntryShop()
{
	int arr[18];
	string name = "ManaBombOrb";
	int i = 0;
	arr[0] = 1009246736; arr[1] = 138436548; arr[2] = 135401732; arr[3] = 1200637186; arr[4] = 604520592; arr[5] = 37888398; arr[6] = 151654853; arr[7] = 555880929; arr[8] = 296000; arr[9] = 2568; 
	arr[10] = 132096; arr[11] = 285149152; arr[12] = 541129744; arr[13] = 142574600; arr[14] = 270598664; arr[15] = 1881162244; arr[16] = 267452167; arr[17] = 252; 
	while(i < 18)
	{
		drawstrEntryShop(arr[i], name);
		i ++;
	}
}

void drawstrEntryShop(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(38);
		pos_y = GetWaypointY(38);
	}
	for (i = 1 ; i > 0 && count < 558 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 38);
		if (count % 49 == 48)
			MoveWaypoint(38, GetWaypointX(38) - 96.000000, GetWaypointY(38) + 2.000000);
		else
			MoveWaypoint(38, GetWaypointX(38) + 2.000000, GetWaypointY(38));
		count ++;
	}
	if (count >= 558)
	{
		count = 0;
		MoveWaypoint(38, pos_x, pos_y);
	}
}

int ParentNode()
{
    int node;

    if (!node)
    {
        node = CreateObject("Hecubah", 28);
        Frozen(node, 1);
    }
    return node;
}

float GetRatioUnitWpXY(int unit, int wp, int mode, float size)
{
    if (!mode)
        return (GetObjectX(unit) - GetWaypointX(wp)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetWaypointX(wp), GetWaypointY(wp));
    else
        return (GetObjectY(unit) - GetWaypointY(wp)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetWaypointX(wp), GetWaypointY(wp));
}

void callTurboTrigger(int wp)
{
    int var_0 = CreateObject("FishSmall", wp);

    SetCallback(var_0, 9, turboTrigger);
    CreateObject("FishSmall", wp);
    Frozen(var_0, 1);
    Frozen(var_0 + 1, 1);
}

void turboTrigger()
{
    MoveObject(self, GetObjectX(self), GetObjectY(self));
}

int GetMaster()
{
    int unit;

    if (!unit)
    {
        unit = CreateObject("Hecubah", 41);
        Frozen(unit, 1);
    }
    return unit;
}

void ShowHorrendousHP()
{
    if (IsObjectOn(GetCaptain()))
    {
        UniChatMessage(GetMaster(), "현재 스테이지: " + IntToString(GetStage(0)) + "\n호렌더스 현재체력: " + IntToString(CurrentHealth(GetCaptain())), 35);
        SecondTimer(1, ShowHorrendousHP);
    }
}