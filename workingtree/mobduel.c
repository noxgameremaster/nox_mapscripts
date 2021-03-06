
#include "builtins.h"
#include "libs\noxmemlib.h"
#include "libs\username.h"
#include "libs\callmethod.h"
#include "libs\typecast.h"
#include "libs\unitutil.h"
#include "libs\fxeffect.h"
#include "libs\printutil.h"
#include "libs\imageutil.h"
#include "libs\mathlab.h"
#include "libs\mathspiral.h"
#include "libs\cmdline.h"
#include "libs\unitstruct.h"
#include "libs\reaction.h"
#include "libs\waypoint.h"
#include "libs\playerupdate.h"
#include "libs\network.h"
#include "libs\walls.h"

#define NULLPTR         0
#define MATH_PI         3.141592

#define OTHER           -1
#define SELF            -2

int player[20];
int GAME_MODE = 2; //game_mode (0: auto_pick, 1: all_pick)
int MODEL[10]; //character
int FAN_CHAK[10]; //chakrum
// int MAGIC_ORB[80]; //heal_orb, charm_orb, mana_orb , white_orb
int COOLDOWN[40]; //skill_complete_value (0-9: R, 10-19: S, 20-29: D, 30-39: J)
int ORB_NOTIFIER[40];
int USERSKILL_TIME[40];

int DEBUGMODE;


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

int FireSpriteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972;
		arr[17] = 85; arr[18] = 25; arr[19] = 70; 
		arr[21] = 1065353216; arr[23] = 65544; arr[24] = 1065353216; 
		arr[37] = 1801545047; arr[38] = 1701996870; arr[39] = 1819042146; 
		arr[53] = 1128792064;
		arr[55] = 15; arr[56] = 21; arr[58] = 5545472; 
		link = &arr;
	}
	return link;
}

void RedWizardInit(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WizardRedBinTable());
}

void MapExit()
{
    ResetPlayerHandlerWhenExitMap();
}

static int InitMathSineBaseWaypointNumber() { return 44; }

void MapInitialize()
{
    VoiceList(0);
    setGoldResetBeacon(1, 0);
    initializeStruct(-1);
    
    BkRevent(-1);
    initializeSelectBeacon();
    initializeChak();
    initializeCharacters();
    stoneGolemInv();
    //loop
    FrameTimer(20, PreservePlayerLoop);
    FrameTimer(1, InitializeSkillFuncs);
    FrameTimer(30, LoopSearchIndex);
    
    //delay_run
    FrameTimer(10, resetOff);
    SecondTimer(10, SpawnMapPotions);
    FrameTimer(100, selectGameMode);

    MusicEvent();
    // FrameTimer(1, NetClientLoop);
}

void initializeStruct(int arg_0) {
    //characterName(arg_0);
    dotPosX(arg_0);
    getRangeAttackCount(arg_0);
    wispExplosionPos(arg_0, 0.0);
    rhombusXY(arg_0, 23.0);
    demonSkillJwayBuffer(arg_0);
    lightningXY(arg_0);
}

void InitializeSkillFuncs()
{
    PlayerInputTable(-1);
    
    InitRandSelector(4388.0, 4184.0);

    RegistSignMessage(Object("gameModeSign1"), "게임모드: 부활 시 캐릭터 재선택 가능");
    RegistSignMessage(Object("gameModeSign2"), "게임모드: 한번 선택한 캐릭으로 계속 플레이");
    RegistSignMessage(Object("usage1"), "캐릭터를 선택하려면 비콘에 들어갑니다");
    RegistSignMessage(Object("usage2"), "랜덤한 유닛을 고르려면 물음표로 들어가세요");
}

void selectAllPick() {
    GAME_MODE = 1;
    ModeSelectBeacon();
}
void selectAutoPick() {
    GAME_MODE = 0;
    ObjectOn(Object("autoSelect"));
    ModeSelectBeacon();
}
void ModeSelectBeacon()
{
    ObjectOff(Object("selectBeacon1"));
    ObjectOff(Object("selectBeacon2"));
    Frozen(CreateObject("TraderQuiverRack", 109), 1);
    WallGroupOpen(1);
}

void selectGameMode()
{
    UniPrintToAll("게임모드를 선택하세요 .");
    UniPrintToAll("좌측비콘: 오토픽 모드(한번 고른 캐릭으로 계속 플레이), 우측비콘: 셀렉팅 모드(죽을때마다 캐릭터 교체가능)");
}

void initializeSelectBeacon() {
    int var_0 = 0;
    int var_1 = Object("selectBeaconBase");

    while(var_0 < 10) {
        LookWithAngle(var_1 + (var_0 * 2), var_0);
        var_0 += 1;
    }
}
void initializeChak() {
    int var_0 = 0;
    int var_1 = Object("chakramBase");

    while(var_0 < 10) {
        FAN_CHAK[var_0] = var_1 + (var_0 * 2);
        var_0 += 1;
    }
}
void initializeCharacters()
{
    int i;

    for (i = 0; i < 10 ; i ++)
        MODEL[i] = Object("char" + IntToString(i + 1));
    RedWizardInit(Object("char6"));
    characterPut();
}

void characterPut() {
    int i;
    
    for(i = 0 ; i < 10 ; i += 1) {
        Frozen(MODEL[i], 1);
        if (i < 5) {
            LookWithAngle(MODEL[i], 32);
            MoveObject(MODEL[i], GetWaypointX(i + 4) - 32.0, GetWaypointY(i + 4) - 32.0);
        }
        else {
            LookWithAngle(MODEL[i], 160);
            MoveObject(MODEL[i], GetWaypointX(i + 4) + 32.0, GetWaypointY(i + 4) + 32.0);
        }
    }
}
string CharacterName(int index)
{
    string table[] = {"드라이아이드", "데몬", "기계골렘", "스톤골렘", "비홀더", "레드 위자드", "호렌더스", "헤쿠바", "오우거로드", "리치로드"};
        
    return table[index];
}

string StepSound(int num)
{
    string arr[10] = {
        "EmberDemonMove", "DemonMove", "MechGolemMove", "GolemMove", "ShadeMove",
        "ShadeMove", "ShadeMove", "ShadeMove", "OgreBruteMove", "OgreBruteMove"
    };

    return arr[num];
}

float StepSoundDelay(int num)
{
    float arr[10] = {
        0.4, 0.25, 0.4, 0.4, 0.3,
        0.4, 0.4, 0.4, 0.4, 0.4
    };
    return arr[num];
}

void selectMonster()
{
    int plr = GetDirection(self);

    if (plr < 10)
    {
        if (!MaxHealth(player[plr]) && MaxHealth(other) == 150 && GetGold(other) <= 10 && CurrentHealth(other))
        {
            EnchantOff(other, "ENCHANT_INFRAVISION");
            EnchantOff(other, "ENCHANT_SNEAK");
            player[plr] = GetCaller();
            player[plr + 10] = 1;
            if (IsPlayerUnit(other))
                SelfDamageClassEntry(other);
            // player[plr + 20] = GetMemory(0x979720);
            MoveObject(player[plr], GetWaypointX(1), GetWaypointY(1));
            Enchant(player[plr], "ENCHANT_INVULNERABLE", 4.0);
            Enchant(player[plr], "ENCHANT_SLOWED", 0.0);
            Enchant(player[plr], "ENCHANT_HASTED", 0.0);
            Enchant(player[plr], "ENCHANT_AFRAID", 0.0);
            Enchant(player[plr], "ENCHANT_CROWN", 0.0);
            SetOwner(player[plr], MODEL[plr]);
            ChangeGold(player[plr], -10);
            ChangeGold(player[plr], plr + 1);
            UniPrintToAll(PlayerIngameNick(player[plr]) + " 님께서 " + CharacterName(plr) + "을 선택하셨습니다");
            Pickup(player[plr], FAN_CHAK[plr]);
            UniPrint(player[plr], "쿨다운 보는법: 빨간색-L 키 / 초록색-J 키 / 파란색-K 키 / 흰색-점프 키");
        }
    }
}

void PlayerOnAlive(int plr)
{
    int plrUnit = player[plr];

    PlayerInputHandler(plr);
    MoveObject(BkRevent(plr), GetObjectX(plrUnit), GetObjectY(plrUnit));

    float dis = Distance(GetObjectX(plrUnit), GetObjectY(plrUnit) + 4.0, GetObjectX(MODEL[plr]), GetObjectY(MODEL[plr]));

    if (dis <= 1.0 && HasEnchant(MODEL[plr], "ENCHANT_VILLAIN"))  //stop
    {
        PauseObject(MODEL[plr], 1);
        EnchantOff(MODEL[plr], "ENCHANT_VILLAIN");
    }
    else if (dis > 1.0)     //run
    {
        Frozen(MODEL[plr], false);
        Enchant(MODEL[plr], "ENCHANT_VILLAIN", 0.0);
        if (!HasEnchant(MODEL[plr], "ENCHANT_DETECTING"))
        {
            MoveWaypoint(14, GetObjectX(MODEL[plr]), GetObjectY(MODEL[plr]));
            AudioEvent(StepSound(plr), 14);
            Enchant(MODEL[plr], "ENCHANT_DETECTING", StepSoundDelay(plr));
        }
        Frozen(MODEL[plr], true);
        LookWithAngle(MODEL[plr], GetDirection(plrUnit));
        Walk(MODEL[plr], GetObjectX(MODEL[plr]), GetObjectY(MODEL[plr]));
        MoveObject(MODEL[plr], GetObjectX(plrUnit), GetObjectY(plrUnit) + 4.0);
    }
    if (!HasEnchant(plrUnit, "ENCHANT_INVISIBLE"))
        Enchant(plrUnit, "ENCHANT_INVISIBLE", 0.0);

    if (HasEnchant(plrUnit, "ENCHANT_PROTECT_FROM_MAGIC"))
    {
        RestoreHealth(plrUnit, 1);
        Effect("GREATER_HEAL", GetObjectX(plrUnit), GetObjectY(plrUnit), GetObjectX(plrUnit), GetObjectY(plrUnit) - 200.0);
    }
    else if (HasEnchant(GetLastItem(plrUnit), "ENCHANT_VILLAIN"))
    {
        MoveWaypoint(102, GetObjectX(plrUnit), GetObjectY(plrUnit));
        RestoreHealth(plrUnit, 50);
        AudioEvent("RegenerationOn", 102);
        Delete(GetLastItem(plrUnit));
    }
    UserSkillCooldownSystem(plr);
}

void PlayerOnDeath(int plr)
{
    int plrUnit = player[plr];

    UniPrintToAll(CharacterName(plr) + "(" + PlayerIngameNick(plrUnit) + ")이(가) 격추되었습니다");
    player[plr] = 0;
}

void PlayerOnExit(int plr)
{
    if (HasItem(player[plr], FAN_CHAK[plr]))
        Drop(player[plr], FAN_CHAK[plr]);
    MoveObject(FAN_CHAK[plr], GetWaypointX(104), GetWaypointY(104));
    resetMonsterPlaced(plr);
    player[plr + 10] = 0;
}

int CheckPlayerDeathFlag(int sPlr)
{
    return player[sPlr + 10] & 0x80000000;
}

void SetPlayerDeathFlag(int sPlr)
{
    player[sPlr + 10] ^= 0x80000000;
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
                    // g_playerSelect[i];
                    PlayerOnAlive(i);
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
                PlayerOnExit(i);
            break;
        }
    }
    FrameTimer(1, PreservePlayerLoop);
}

void PlayerInputHandler(int plr)
{
    int plrUnit = player[plr];

    if (HasEnchant(plrUnit, "ENCHANT_INFRAVISION"))
        EnchantOff(plrUnit, "ENCHANT_INFRAVISION");
    CallFunctionWithArg(PlayerInputTable(CheckPlayerInput(plrUnit)), plr);
}

object PlayerInputTable(int key)
{
    int arr[100];

    if (key < 0)
    {
        arr[48] = skillSneak; arr[47] = skillEyeofwolf; arr[49] = harpoonEvent; arr[7] = skillOnJump;
        return 0;
    }
    if (arr[key])
        return arr[key];
    return skillNothing;
}

int BkRevent(int num)
{
    int arr[10];
    int i;

    if (num == -1)
    {
        arr[0] = Object("ReventBaseOffset");
        for (i = 1 ; i < 10 ; i ++)
            arr[i] = arr[0] + (i * 2);
        return 0;
    }
    return arr[num];
}

void FixDecreaseCooldown(int plrIndex)
{
    int t, plrUnit = player[plrIndex];

    for (t = 0 ; t < 40 ; t += 10)
    {
        if (USERSKILL_TIME[t + plrIndex] > 0)
        {
            USERSKILL_TIME[t + plrIndex]--;
            if (!USERSKILL_TIME[t + plrIndex])
                UniPrint(plrUnit, "지금 " + SkillName(t / 10) + " 스킬 쿨다운이 완료되었습니다");
        }
        else if (!IsObjectOn(ORB_NOTIFIER[plrIndex + t]) && CurrentHealth(plrUnit))
        {
            ORB_NOTIFIER[t + plrIndex] = CreateObjectAt(CooldownDotType(t / 10), GetObjectX(plrUnit), GetObjectY(plrUnit));
            Frozen(ORB_NOTIFIER[t + plrIndex], true);
            UnitNoCollide(ORB_NOTIFIER[t + plrIndex]);
        }
    }
}

string SkillName(int index)
{
    string table[] = {"L 키(빨간색)", "J 키(초록색)", "K 키(파란색)", "점프(흰색)"};

    return table[index];
}

string CooldownDotType(int index)
{
    string table[] = {"OldDarkWoodenChair3", "OldDarkWoodenChair1", "OldDarkWoodenChair2", "OldDarkWoodenChair4"};

    return table[index];
}
void UserSkillCooldownSystem(int plrIndex)
{
    int t;

    FixDecreaseCooldown(plrIndex);
    for (t = 0 ; t < 40 ; t += 10)
    {
        if (IsObjectOn(ORB_NOTIFIER[plrIndex + t]))
            MoveObject(ORB_NOTIFIER[plrIndex + t], GetObjectX(player[plrIndex]) + dotPosX(t / 10), GetObjectY(player[plrIndex]) - 60.0);
    }
}
float dotPosX(int arg_0) {
    float var_0[4];

    if (arg_0 == -1) {
        var_0[0] = -30.0;
        var_0[1] = -10.0;
        var_0[2] = 10.0;
        var_0[3] = 30.0;
        return var_0[0];
    }
    return var_0[arg_0];
}
int setGoldResetBeacon(int arg_0, int arg_1) { //mode(0: release, 1: insert), value
    int var_0;

    if (var_0 == 0)
        var_0 = Object("goldResetBeacon");
    if (arg_0 == 1)
        LookWithAngle(var_0, arg_1);
    return GetDirection(var_0);
}
void resetYourGold()
{
    if (setGoldResetBeacon(0, 0) == 0 && GetGold(OTHER) > 0) {
        ChangeGold(OTHER, -GetGold(OTHER));
    }
    else {
        if (MaxHealth(OTHER) == 150) {
            Delete(GetLastItem(OTHER));
            Delete(GetLastItem(OTHER));
            MoveObject(OTHER, GetWaypointX(3), GetWaypointY(3));
        }
        else {
            MoveObject(OTHER, 4723.0, 4028.0);
            UniPrint(OTHER, "죄송합니다 , 이 맵은 오직 전사만 입장 가능합니다 .");
        }
    }
    EnchantOff(OTHER, "ENCHANT_AFRAID");
    EnchantOff(OTHER, "ENCHANT_HASTED");
    EnchantOff(OTHER, "ENCHANT_SLOWED");
    EnchantOff(OTHER, "ENCHANT_ANCHORED");
    if (!HasEnchant(OTHER, "ENCHANT_ANTI_MAGIC"))
        Enchant(OTHER, "ENCHANT_ANTI_MAGIC", 0.0);

    //here
    int pUnit = GetCaller();

    if (ValidPlayerCheck(pUnit))
    {
        if (pUnit == GetHost())
            PlayerClassCommonWhenEntry();
        else
            NetworkUtilClientEntry(pUnit);
    }
}

void autoSelectToCharacter()
{
    int plr = 0;

    if (CurrentHealth(other))
    {
        plr = GetGold(other);
        if (plr > 0 && plr <= 10)
            MoveObject(other, GetWaypointX(3 + plr), GetWaypointY(3 + plr));
    }
}

void resetOff()
{
    SetMemory(0x5D5330, 0);
    setGoldResetBeacon(1, 1);
}

void randomRespawnLocation() {
    int var_0 = Random(15, 32);

    MoveObject(other, GetWaypointX(var_0), GetWaypointY(var_0));
    Effect("TELEPORT", GetWaypointX(var_0), GetWaypointY(var_0), 0.0, 0.0);
    Effect("SMOKE_BLAST", GetWaypointX(var_0), GetWaypointY(var_0), 0.0, 0.0);
    AudioEvent("BlindOff", var_0);
}

void resetMonsterPlaced(int plrIndex)
{
    object location = plrIndex + 4;

    if (plrIndex < 5)
    {
        MoveObject(MODEL[plrIndex], GetWaypointX(location) - 32.0, GetWaypointY(location) - 32.0);
        LookWithAngle(MODEL[plrIndex], 32);
    }
    else {
        MoveObject(MODEL[plrIndex], GetWaypointX(location) + 32.0, GetWaypointY(location) + 32.0);
        LookWithAngle(MODEL[plrIndex], 160);
    }

    //Dots devide.
    //array size. 80
    //dots: 0-9, 10-19, 20-29, 30-39
    //value: 40-49, 50-59, 60-69, 70-79

    // for(var_0 = plrIndex ; var_0 < 40 ; var_0 += 10) {
    //     if (IsObjectOn(MAGIC_ORB[var_0]))
    //         Delete(MAGIC_ORB[var_0]);
    //     if (GAME_MODE)
    //         MAGIC_ORB[var_0 + 40] = 0;
    // }
    int t;
    for (t = 0 ; t < 40 ; t += 10)
    {
        if (IsObjectOn(ORB_NOTIFIER[plrIndex + t]))
            Delete(ORB_NOTIFIER[plrIndex + t]);
        if (GAME_MODE)
            USERSKILL_TIME[plrIndex + t] = 0;
    }
    Frozen(MODEL[plrIndex], true);
}

void skillNothing(int plr)
{ }

void skillSneak(int arg_0)
{
    int time[10];

    if (!time[0])
    {
        time[0] = 40; time[1] = 25; time[2] = 20; time[3] = 20; time[4] = 35;
        time[5] = 25; time[6] = 30; time[7] = 20; time[8] = 30; time[9] = 15;
    }
    if (DEBUGMODE == true)
        UniPrint(GetHost(), PlayerIngameNick(player[arg_0]) + " 이 J 키를 눌렀습니다");
    if (!USERSKILL_TIME[arg_0])
    {
        if (HasEnchant(player[arg_0], "ENCHANT_ANCHORED"))
        {
            UniPrint(player[arg_0], "아직 그 능력을 사용할 수 없어요");
            return;
        }
        if (!COOLDOWN[arg_0])
        {
            USERSKILL_TIME[arg_0] = time[arg_0] * 30;
            COOLDOWN[arg_0] = 1;
            CallFunctionWithArg(SkillAFuncPtr() + arg_0, arg_0);
            Delete(ORB_NOTIFIER[arg_0]);
            if (DEBUGMODE)
                UniPrintToAll(PlayerIngameNick(player[arg_0]) + " 스킬발동1");
        }
        else
        {
            UniPrintToAll("!!!쿨다운1!!!");
        }
    }
    else
        NotifyCurrentCooldown(arg_0, USERSKILL_TIME[arg_0]);
}

void skillEyeofwolf(int arg_0)
{
    int time[10];

    if (!time[0])
    {
        time[0] = 40; time[1] = 30; time[2] = 30; time[3] = 30; time[4] = 30;
        time[5] = 40; time[6] = 30; time[7] = 30; time[8] = 50; time[9] = 80;
    }
    if (DEBUGMODE == true)
        UniPrint(GetHost(), PlayerIngameNick(player[arg_0]) + " 이 L 키를 눌렀습니다");
    if (!USERSKILL_TIME[arg_0 + 10])
    {
        if (HasEnchant(player[arg_0], "ENCHANT_ANCHORED"))
        {
            UniPrint(player[arg_0], "아직 그 능력을 사용할 수 없어요");
            return;
        }
        if (!COOLDOWN[arg_0 + 10])
        {
            COOLDOWN[arg_0 + 10] = 1;
            USERSKILL_TIME[arg_0 + 10] = time[arg_0] * 30;
            CallFunctionWithArg(SkillSFuncPtr() + arg_0, arg_0);
            Delete(ORB_NOTIFIER[arg_0 + 10]);
            if (DEBUGMODE)
                UniPrintToAll(PlayerIngameNick(player[arg_0]) + " 스킬발동2");
        }
        else
        {
            UniPrintToAll("!!!쿨다운2!!!");
        }
    }
    else
        NotifyCurrentCooldown(arg_0, USERSKILL_TIME[arg_0 + 10]);
}

void harpoonEvent(int arg_0)
{
    int time[10];

    if (!time[0])
    {
        time[0] = 5; time[1] = 5; time[2] = 5; time[3] = 8; time[4] = 10;
        time[5] = 12; time[6] = 5; time[7] = 5; time[8] = 4; time[9] = 5;
    }
    if (DEBUGMODE == true)
        UniPrint(GetHost(), PlayerIngameNick(player[arg_0]) + " 이 K 키를 눌렀습니다");
    if (!USERSKILL_TIME[arg_0 + 20])
    {
        if (HasEnchant(player[arg_0], "ENCHANT_ANCHORED"))
        {
            UniPrint(player[arg_0], "아직 그 능력을 사용할 수 없어요");
            return;
        }
        if (!COOLDOWN[arg_0 + 20])
        {
            COOLDOWN[arg_0 + 20] = 1;
            USERSKILL_TIME[arg_0 + 20] = time[arg_0] * 30;
            CallFunctionWithArg(SkillDFuncPtr() + arg_0, arg_0);
            Delete(ORB_NOTIFIER[arg_0 + 20]);
            if (DEBUGMODE)
                UniPrintToAll(PlayerIngameNick(player[arg_0]) + " 스킬발동3");
        }
        else
        {
            UniPrintToAll("!!!쿨다운3!!!");
        }
    }
    else
        NotifyCurrentCooldown(arg_0, USERSKILL_TIME[arg_0 + 20]);
}

void skillOnJump(int arg_0)
{
    int time[10];

    if (DEBUGMODE == true)
        UniPrint(GetHost(), PlayerIngameNick(player[arg_0]) + " 이 점프 키를 눌렀습니다");
    if (!time[0])
    {
        time[0] = 30; time[1] = 15; time[2] = 35; time[3] = 30; time[4] = 40;
        time[5] = 3; time[6] = 20; time[7] = 10; time[8] = 70; time[9] = 3;
    }
    if (!USERSKILL_TIME[arg_0 + 30])
    {
        if (HasEnchant(player[arg_0], "ENCHANT_ANCHORED"))
        {
            UniPrint(player[arg_0], "아직 그 능력을 사용할 수 없어요");
            return;
        }
        if (!COOLDOWN[arg_0 + 30])
        {
            USERSKILL_TIME[arg_0 + 30] = time[arg_0] * 30;
            COOLDOWN[arg_0 + 30] = 1;
            CallFunctionWithArg(SkillJFuncPtr() + arg_0, arg_0);
            Delete(ORB_NOTIFIER[arg_0 + 30]);
            if (DEBUGMODE)
                UniPrintToAll(PlayerIngameNick(player[arg_0]) + " 스킬발동4");
        }
        else
        {
            UniPrintToAll("!!!쿨다운4!!!");
        }
    }
    else
        NotifyCurrentCooldown(arg_0, USERSKILL_TIME[arg_0 + 30]);
}

//43 way not used

void DetectedSpecificIndex(int curId)
{
    if (!IsMissileUnit(curId))
        return;
    int thingId = GetUnitThingID(curId);

    if (thingId == 1179)
    {
        if (CheckOwner(curId) >= 0 && !HasEnchant(curId, "ENCHANT_FREEZE"))
            normalAttackEvent(CheckOwner(curId), curId);
    }
    else if (thingId == 526)
    {
        if (!HasEnchant(curId, "ENCHANT_FREEZE"))
            Delete(curId);
    }
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

void normalAttackEvent(int plr, int cur)
{
    int k = 0;
    int mis;

    if (CurrentHealth(player[plr]))
    {
        if (!HasEnchant(player[plr], "ENCHANT_DETECTING"))
        {
            MoveWaypoint(95, GetObjectX(cur), GetObjectY(cur));
            while(k < getRangeAttackCount(plr))
            {
                mis = CreateObject(MissileNameTables(plr), 95);
                Enchant(mis, "ENCHANT_FREEZE", 0.0);
                SetOwner(player[plr], mis);
                LookWithAngle(mis, GetDirection(player[plr]));
                PushObject(mis, 30.0, GetObjectX(player[plr]), GetObjectY(player[plr]));
                k += 1;
            }
            Enchant(player[plr], "ENCHANT_DETECTING", 0.7);
        }
    }
    Delete(cur);
}

int getRangeAttackCount(int num)
{
    int info[10];

    if (num == -1)
    {
        info[0] = 2;
        info[1] = 2;
        info[2] = 2;
        info[3] = 1;
        info[4] = 2;
        info[5] = 2;
        info[6] = 2;
        info[7] = 1;
        info[8] = 1;
        info[9] = 2;
        return info[0];
    }
    return info[num];
}

string MissileNameTables(int index)
{
    string table[] = {
        "YellowStarShot", "PitifulFireball", "MercArcherArrow", "ThrowingStone", "LightningBolt",
        "ImpShot", "MercArcherArrow", "DeathBallFragment", "OgreShuriken", "CherubArrow"
        };
    return table[index];
}

void SpawnMapPotions()
{
    int arr[10], timer;

    if ((--timer) < 0)
    {
        int k;
        for (k = 9 ; k >= 0 ; k -= 1)
        {
            if (!IsObjectOn(arr[k]))
            {
                arr[k] = CreateObject("AmuletOfClarity", k + 85);
                Enchant(arr[k], "ENCHANT_VILLAIN", 0.0);
            }
        }
        timer = 100;
    }
    SecondTimer(1, SpawnMapPotions);
}

int SkillAFuncPtr()
{
    StopScript(GWizSkillA);
}

void GWizSkillA(int arg_0)
{
    int var_0;
    float var_1;
    float var_2;
    int var_3;

    if (CurrentHealth(player[arg_0]) && var_0 < 22) {
        if (var_0 == 0) {
            var_3 = GetDirection(player[arg_0]);
            var_1 = UnitAngleCos(player[arg_0], -10.0); //dx
            var_2 = UnitAngleSin(player[arg_0], -10.0); //dy
            MoveWaypoint(44, GetObjectX(player[arg_0]) + var_1 + (var_2 * 10.0), GetObjectY(player[arg_0]) + var_2 - (var_1 * 10.0));
            MoveWaypoint(45, GetObjectX(player[arg_0]) + var_1 - (var_2 * 10.0), GetObjectY(player[arg_0]) + var_2 + (var_1 * 10.0));
            AudioEvent("GlyphCast", 44);
            var_0 = 1;
        }
        spawnArrows(arg_0, 44, var_3, var_1, var_2);
        spawnArrows(arg_0, 45, var_3, var_1, var_2);
        MoveWaypoint(44, GetWaypointX(44) - var_2, GetWaypointY(44) + var_1);
        MoveWaypoint(45, GetWaypointX(45) + var_2, GetWaypointY(45) - var_1);
        GreenLightningFx(FloatToInt(GetWaypointX(44)), FloatToInt(GetWaypointY(44)), FloatToInt(GetWaypointX(45)), FloatToInt(GetWaypointY(45)), 12);
        FrameTimerWithArg(1, arg_0, GWizSkillA);
        var_0 += 1;
    }
    else {
        COOLDOWN[arg_0] = 0;
        var_0 = 0;
    }
}

void DemonSkillA(int arg_0)
{
    int var_0;
    float var_1;
    float var_2;
    int var_3;

    if (CurrentHealth(player[arg_0]) && var_0 < 30)
    {
        if (var_0 == 0) {
            var_1 = UnitAngleCos(player[arg_0], 23.0);
            var_2 = UnitAngleSin(player[arg_0], 23.0);
            MoveWaypoint(48, GetObjectX(player[arg_0]) + var_1, GetObjectY(player[arg_0]) + var_2);
            var_0 = 1;
        }
        else {
            MoveWaypoint(48, GetWaypointX(48) + var_1, GetWaypointY(48) + var_2);
            var_3 = CreateObject("CarnivorousPlant", 48);
            LookWithAngle(var_3, arg_0);
            Frozen(var_3, 1);
            LookWithAngle(var_3, arg_0);
            DeleteObjectTimer(var_3, 1);
            SetCallback(var_3, 9, demonTouched);
            DeleteObjectTimer(CreateObject("MediumFireBoom", 48), 6);
            AudioEvent("HammerMissing", 48);
            var_0 += 1;
        }
        FrameTimerWithArg(1, arg_0, DemonSkillA);
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0] = 0;
    }
}

void mecaGolemSkillA(int arg_0) {
    int var_0;
    float var_1;
    float var_2;
    int var_3;

    if (CurrentHealth(player[arg_0]) && var_0 < 30)
    {
        if (var_0 == 0) {
            var_1 = UnitAngleCos(player[arg_0], 24.0);
            var_2 = UnitAngleSin(player[arg_0], 24.0);
            MoveWaypoint(51, GetObjectX(player[arg_0]) + var_1, GetObjectY(player[arg_0]) + var_2);
        }
        else {
            MoveWaypoint(51, GetWaypointX(51) + var_1, GetWaypointY(51) + var_2);
            var_3 = CreateObject("CarnivorousPlant", 51);
            LookWithAngle(var_3, arg_0);
            Frozen(var_3, 1);
            SetCallback(var_3, 9, mecaGolemTouch);
            DeleteObjectTimer(var_3, 1);
            Effect("SENTRY_RAY", GetWaypointX(51), GetWaypointY(51), GetWaypointX(51) + var_1, GetWaypointY(51) + var_2);
        }
        var_0 += 1;
        FrameTimerWithArg(1, arg_0, mecaGolemSkillA);
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0] = 0;
    }
}

void stoneGolemSkillA(int arg_0)
{
    float var_0;
    float var_1;
    int var_2;
    int var_3[3];
    int i;

    if (CurrentHealth(player[arg_0]))
    {
        var_0 = UnitAngleCos(player[arg_0], 20.0); //-dx
        var_1 = UnitAngleSin(player[arg_0], 20.0); //-dy
        MoveWaypoint(58, GetObjectX(player[arg_0]) + var_0, GetObjectY(player[arg_0]) + var_1);
        MoveWaypoint(82, GetWaypointX(58) - var_1, GetWaypointY(58) + var_0);
        MoveWaypoint(83, GetWaypointX(58) + var_1, GetWaypointY(58) - var_0);
        var_3[0] = CreateObject("ArcherArrow", 58);
        var_3[1] = CreateObject("ArcherArrow", 82);
        var_3[2] = CreateObject("ArcherArrow", 83);

        for(i = 0 ; i < 3 ; i += 1)
        {
            Enchant(var_3[i], "ENCHANT_HASTED", 0.0);
            Enchant(var_3[i], "ENCHANT_REFLECTIVE_SHIELD", 0.0);
            SetOwner(player[arg_0], var_3[i]);
            LookWithAngle(var_3[i], GetDirection(player[arg_0]));
            PushObject(var_3[i], -20.0, GetObjectX(var_3[i]) + var_0, GetObjectY(var_3[i]) + var_1);
        }
        AudioEvent("AwardSpell", 58);
        DeleteObjectTimer(CreateObject("ManaBombCharge", 58), 20);
    }
    COOLDOWN[arg_0] = 0;
}

void beholderSkillA(int arg_0)
{
    int var_0;
    int var_1;
    
    if (CurrentHealth(player[arg_0]) && var_0 < 2)
    {
        if (var_0 == 0) {
            var_1 = CreateObject("moonglow", 2);
            SetOwner(player[arg_0], var_1);
        }
        else if (var_0 == 1) {
            MoveWaypoint(59, GetObjectX(var_1), GetObjectY(var_1));
            CastSpellObjectObject("SPELL_PULL", var_1, var_1);
            CastSpellObjectObject("SPELL_PULL", var_1, var_1);
            Delete(var_1);
            DeleteObjectTimer(CreateObject("PlayerWaypoint", 59), 25);
            DeleteObjectTimer(CreateObject("PlayerWaypoint", 59), 25);
            FrameTimerWithArg(10, arg_0 + (256 * 59), bombLightningSparks);
        }
        var_0 += 1;
        FrameTimerWithArg(1, arg_0, beholderSkillA);
    }
    else {
        if (IsObjectOn(var_1))
            Delete(var_1);
        COOLDOWN[arg_0] = 0;
        var_0 = 0;
    }
}

void RWizSkillA(int arg_0)
{
    int var_0;
    float pos_x;
    float pos_y;
    string name = "COUNTERSPELL_EXPLOSION";
    int i;
    int unit[8];
    int pointer;

    if (CurrentHealth(player[arg_0]) && var_0 < 100)
    {
        if (!pointer)
        {
            pointer = CreateObject("moonglow", 2);
            SetOwner(player[arg_0], pointer);
            DeleteObjectTimer(pointer, 3);
        }
        else
        {
            pos_x = MathSpiral(var_0 * 2);
            pos_y = MathSpiral(var_0 * 2 + 1);
            if (!var_0)
            {
                MoveWaypoint(62, GetObjectX(pointer), GetObjectY(pointer));
                AudioEvent("HecubahDieFrame0A", 62);
                for (i = 7 ; i >= 0 ; i --)
                {
                    unit[i] = CreateObject("Bomber", 106);
                    LookWithAngle(unit[i], arg_0);
                    Damage(unit[i], 0, 255, -1);
                    ObjectOff(unit[i]);
                    Frozen(unit[i], 1);
                    SetCallback(unit[i], 9, RWizTouched);
                }
            }
            for (i = 0 ; i <= 315 ; i += 45)
            {
                MoveWaypoint(63, GetWaypointX(62) + Rx_ratio(pos_x, pos_y, i), GetWaypointY(62) + Ry_ratio(pos_x, pos_y, i));
                Effect(name, GetWaypointX(63), GetWaypointY(63), 0.0, 0.0);
                MoveObject(unit[i / 45], GetWaypointX(63), GetWaypointY(63));
            }
            var_0 ++;
        }
        FrameTimerWithArg(1, arg_0, RWizSkillA);
    }
    else
    {
        COOLDOWN[arg_0] = 0;
        var_0 = 0;
        for (i = 7 ; i >= 0 ; i --)
            Delete(unit[i]);
        if (pointer > 0)
            pointer = 0;
    }
}

void horrendousSkillA(int plr)
{
    int progress, glowPoint;
    
    if (CurrentHealth(player[plr]) > 0 && progress < 60)
    {
        if (progress == 0)
        {
            glowPoint = CreateObject("moonglow", 2);
            SetOwner(player[plr], glowPoint);
            progress = -1;
        }
        else if (progress == -1)
        {
            if (IsVisibleTo(player[plr], glowPoint))
            {
                controlBlock(plr, glowPoint, 100.0, false);
            }
            else
                UniPrint(player[plr], "커서 지점을 볼 수 없습니다");
            progress = 1;
            Delete(glowPoint);
        }
        else
            progress += 1;
        FrameTimerWithArg(1, plr, horrendousSkillA);
    }
    else {
        controlBlock(plr, 0, 0.0, true);
        COOLDOWN[plr] = 0;
        progress = 0;
    }
}

void hecubahSkillA(int arg_0)
{
    float var_0;
    float var_1;
    int var_2 = 0;
    int var_3;

    if (CurrentHealth(player[arg_0]))
    {
        var_0 = UnitAngleCos(player[arg_0], 30.0);
        var_1 = UnitAngleSin(player[arg_0], 30.0);
        MoveWaypoint(75, GetObjectX(player[arg_0]) + var_0, GetObjectY(player[arg_0]) + var_1);

        while(var_2 < 12)
        {
            var_3 = CreateObject("CarnivorousPlant", 75);
            SetOwner(player[arg_0], var_3);
            Frozen(var_3, 1);
            SetCallback(var_3, 9, deathrayTouched);
            LookWithAngle(var_3, arg_0);
            DeleteObjectTimer(var_3, 1);
            MoveWaypoint(75, GetWaypointX(75) + var_0, GetWaypointY(75) + var_1);
            AudioEvent("DeathRayCast", 75);
            var_2 += 1;
        }
        Effect("DEATH_RAY", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), GetWaypointX(75), GetWaypointY(75));
        AudioEvent("HecubahTaunt", 75);
    }
    COOLDOWN[arg_0] = 0;
}

void ogreSkillA(int arg_0) {
    float var_0;
    float var_1;
    int var_2;
    int var_3;

    if (CurrentHealth(player[arg_0]) > 0 && var_2 < 21) {
        if (var_2 == 0) {
            var_0 = UnitAngleCos(player[arg_0], 26.0);
            var_1 = UnitAngleSin(player[arg_0], 26.0);
            MoveWaypoint(78, GetObjectX(player[arg_0]) + var_0, GetObjectY(player[arg_0]) + var_1);
            AudioEvent("CrossBowShoot", 78);
            Enchant(player[arg_0], "ENCHANT_ANCHORED", 0.0);
        }
        else {
            MoveWaypoint(78, GetWaypointX(78) + var_0, GetWaypointY(78) + var_1);
            makeStar(78, 50.0);
            var_3 = CreateObject("CarnivorousPlant", 78);
            Frozen(var_3, 1);
            LookWithAngle(var_3, arg_0);
            SetCallback(var_3, 9, OgreCollisionEvent);
            DeleteObjectTimer(var_3, 1);
        }
        var_2 += 1;
        FrameTimerWithArg(1, arg_0, ogreSkillA);
    }
    else {
        EnchantOff(player[arg_0], "ENCHANT_ANCHORED");
        var_2 = 0;
        COOLDOWN[arg_0] = 0;
    }
}

void lichSkillA(int arg_0)
{
    int rep;
    int ray_addr;
    int ray_id;
    float math_x;
    float math_y;
    
    if (CurrentHealth(player[arg_0]) && rep < 2)
    {
        if (!rep)
        {
            math_x = UnitAngleCos(player[arg_0], 25.0);
            math_y = UnitAngleSin(player[arg_0], 25.0);
            MoveWaypoint(96, GetObjectX(player[arg_0]) + math_x, GetObjectY(player[arg_0]) + math_y);
            Effect("SENTRY_RAY", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), GetObjectX(player[arg_0]) + (math_x * 8.0), GetObjectY(player[arg_0]) + (math_y * 8.0));
            ray_addr = SentryRay();
            ray_id = GetMemory(ray_addr + 0x2c);
            SetOwner(player[arg_0], ray_id);
            SetRayRadian(player[arg_0], ray_addr);
            Enchant(player[arg_0], "ENCHANT_INVULNERABLE", 0.9);
            FrameTimerWithArg(6, arg_0, lichSkillA);
        }
        else
        {
            MoveObject(ray_id, GetWaypointX(96), GetWaypointY(96));
            FrameTimerWithArg(6, arg_0, lichSkillA);
        }
        rep ++;
    }
    else {
        rep = 0;
        COOLDOWN[arg_0] = 0;
        if (ray_id)
        {
            ObjectOff(ray_id);
            MoveObject(ray_id, GetWaypointX(108), GetWaypointY(108));
        }
    }
}

void spawnArrows(int plr, int wp, int angle, float pos_x, float pos_y) { //player_idx, wp, direction
    int k;
    int arr[4];

    for (k = 3 ; k >= 0 ; k --)
    {
        arr[k] = CreateObject("WeakArcherArrow", wp);
        SetOwner(player[plr], arr[k]);
        LookWithAngle(arr[k], angle);
        PushObject(arr[k], -30.0, GetObjectX(arr[k]) - pos_x, GetObjectY(arr[k]) - pos_y);
    }
}

int SkillSFuncPtr()
{
    StopScript(GWizSkillS);
}

void GWizSkillS(int arg_0) {
    int var_0;
    int var_1;
    int var_2;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 90) {
        if (var_0 == 0) {
            MoveWaypoint(46, GetObjectX(player[arg_0]) - UnitAngleCos(player[arg_0], -23.0), GetObjectY(player[arg_0]) - UnitAngleSin(player[arg_0], -23.0));
            var_1 = CreateObject("willOWisp", 46);
            Enchant(var_1, "ENCHANT_INVULNERABLE", 0.0);
            Enchant(var_1, "ENCHANT_ANTI_MAGIC", 0.0);
            Enchant(var_1, "ENCHANT_BLINDED", 0.0);
            Enchant(var_1, "ENCHANT_RUN", 0.0);
            AggressionLevel(var_1, 0.0);
            var_2 = CreateObject("moonglow", 2);
            SetOwner(player[arg_0], var_1);
            SetOwner(player[arg_0], var_2);
            Enchant(player[arg_0], "ENCHANT_ANCHORED", 0.0);
            var_0 = 1;
            FrameTimerWithArg(1, arg_0, GWizSkillS);
        }
        else {
            MoveWaypoint(105, GetObjectX(var_1), GetObjectY(var_1));
            MoveWaypoint(46, GetObjectX(var_2), GetObjectY(var_2));
            if (var_0 % 3 == 0) {
                LocalRisk(arg_0, 80, 105, 60.0);
            }
            PushObject(var_1, -3.5, GetObjectX(var_2), GetObjectY(var_2));
            rhombusRaiserFX(var_1);
            var_0 += 1;
            FrameTimerWithArg(1, arg_0, GWizSkillS);
        }
    }
    else {
        var_0 = 0;
        EnchantOff(player[arg_0], "ENCHANT_ANCHORED");
        COOLDOWN[arg_0 + 10] = 0;
        Delete(var_1);
        Delete(var_2);
    }
}

void DemonSkillS(int plr)
{
    int plrUnit = player[plr];

    if (CurrentHealth(plrUnit))
    {
        // object splashUnit = CreateObjectAt("AmbBeachBirds", GetObjectX(player[plr]), GetObjectY(player[plr]));

        // LookWithAngle(splashUnit, 9);

        // SetOwner(player[plr], splashUnit);
        // AroundSplashTriggered(splashUnit, 330.0, 137);
        // PlaySoundAround(splashUnit, 485);
        // DemonsBloodFestival(splashUnit);
        // Effect("JIGGLE", GetObjectX(splashUnit), GetObjectY(splashUnit), 33.0, 0.0);
        float vectX = UnitAngleCos(plrUnit, 14.0);
        float vectY = UnitAngleSin(plrUnit, 14.0);
        int watcher = CreateObjectAt("WeirdlingBeast", GetObjectX(plrUnit) + vectX, GetObjectY(plrUnit) + vectY);

        SetOwner(plrUnit, watcher);
        UnitNoCollide(watcher);
        CreatureGuard(watcher, GetObjectX(watcher), GetObjectY(watcher), GetObjectX(watcher) + vectX, GetObjectY(watcher) + vectY, 600.0);
        LookWithAngle(watcher, GetDirection(plrUnit));
        DeleteObjectTimer(watcher, 1);
        DrawGeometryRingAt("HealOrb", GetObjectX(watcher), GetObjectY(watcher), 0);
        Enchant(watcher, "ENCHANT_INFRAVISION", 0.0);
        SetCallback(watcher, 3, DemonWatcherSight);
    }
    COOLDOWN[plr + 10] = 0;
}

void mecaGolemSkillS(int arg_0) {
    int var_0;
    int var_1;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 2) {
        if (var_0 == 0) {
            var_0 = 1;
            var_1 = CreateObject("moonglow", 2);
            SetOwner(player[arg_0], var_1);
            FrameTimerWithArg(1, arg_0, mecaGolemSkillS);
        }
        else if (var_0 == 1) {
            if (IsVisibleTo(player[arg_0], var_1)) {
                MoveWaypoint(52, GetObjectX(var_1), GetObjectY(var_1));
                Delete(var_1);
                Effect("LIGHTNING", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), GetWaypointX(52), GetWaypointY(52));
                MoveObject(player[arg_0], GetWaypointX(52), GetWaypointY(52));
                aroundSpreadMissiles(arg_0);
            }
            else
                UniPrint(player[arg_0], "마우스의 위치가 벽 너머에 있습니다, 현재 마우스 포인터의 위치로 이동이 불가능합니다");
            var_0 = 2;
            FrameTimerWithArg(1, arg_0, mecaGolemSkillS);
        }
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0 + 10] = 0;
        Delete(var_1);
    }
}

void stoneGolemSkillS(int arg_0) {
    float var_0;
    float var_1;
    int var_2;
    int i;

    if (CurrentHealth(player[arg_0]) > 0) {
        Enchant(player[arg_0], "ENCHANT_FREEZE", 2.0);
        var_0 = UnitAngleCos(player[arg_0], 30.0);
        var_1 = UnitAngleSin(player[arg_0], 30.0);
        MoveWaypoint(58, GetObjectX(player[arg_0]) + var_0, GetObjectY(player[arg_0]) + var_1);
        for(i = 0; i < 12; i += 1) {
            MoveWaypoint(58, GetWaypointX(58) + var_0, GetWaypointY(58) + var_1);
            var_2 = CreateObject("wizard", 58);
            SetOwner(player[arg_0], var_2);
            CastSpellObjectLocation("SPELL_FIST", var_2, GetWaypointX(58), GetWaypointY(58));
            DeleteObjectTimer(var_2, 1);
        }
    }
    COOLDOWN[arg_0 + 10] = 0;
}

void beholderSkillS(int arg_0) {
    int var_0;
    float var_1;
    float var_2;
    int var_3;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 120) {
        var_1 = MathSine(Random(0, 359) + 90, RandomFloat(30.0, 200.0));
        var_2 = MathSine(Random(0, 359), RandomFloat(30.0, 200.0));
        MoveWaypoint(60, GetObjectX(player[arg_0]) + var_1, GetObjectY(player[arg_0]) + var_2);
        var_3 = CreateObject("CarnivorousPlant", 60);
        Frozen(var_3, 1);
        LookWithAngle(var_3, arg_0);
        SetOwner(player[arg_0], var_3);
        SetCallback(var_3, 9, lightningTouched2);
        DeleteObjectTimer(var_3, 1);
        Effect("RICOCHET", GetWaypointX(60), GetWaypointY(60), 0.0, 0.0);
        Effect("LIGHTNING", GetWaypointX(60), GetWaypointY(60), GetWaypointX(60), GetWaypointY(60) - 100.0);
        AudioEvent("LightningBolt", 60);
        BeholderAroundDamage(arg_0);
        var_0 += 1;
        FrameTimerWithArg(1, arg_0, beholderSkillS);
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0 + 10] = 0;
    }
}

void RWizSkillS(int arg_0) {
    int var_0;
    int var_1;
    int var_2;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 51) {
        if (var_0 == 0) {
            var_1 = CreateObject("moonglow", 2);
            SetOwner(player[arg_0], var_1);
            // Enchant(player[arg_0], "ENCHANT_INVULNERABLE", 3.0);
            var_0 = -1;
        }
        else if (var_0 == -1) {
            if (IsVisibleTo(player[arg_0], var_1))
            {
                MoveWaypoint(64, GetObjectX(var_1), GetObjectY(var_1));
                var_0 = 1;
            }
            else
            {
                UniPrint(player[arg_0], "커서위치는 볼 수 없는 지역입니다");
                var_0 = 99999;
            }
            Delete(var_1);
        }
        else {
            MoveWaypoint(65, GetWaypointX(64) + RandomFloat(-160.0, 160.0), GetWaypointY(64) + RandomFloat(-160.0, 160.0));
            var_2 = CreateObject("flame", 65);
            Raise(var_2, 200.0);
            SetOwner(player[arg_0], var_2);
            DeleteObjectTimer(var_2, 21);
            var_2 = CreateObject("CaveRocksMedium", 65);
            LookWithAngle(var_2, arg_0);
            Raise(var_2, 200.0);
            var_0 += 1;
            FrameTimerWithArg(22, var_2, explosionDelayFireStorm);
        }
        FrameTimerWithArg(3, arg_0, RWizSkillS);
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0 + 10] = 0;
        Delete(var_1);
    }
}

void horrendousSkillS(int arg_0) {
    int var_0;
    int var_1;
    
    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 60) {
        if (var_0 == 0) {
            var_1 = CreateObject("moonglow", 2);
            SetOwner(player[arg_0], var_1);
            var_0 -= 1;
        }
        else if (var_0 == -1) {
            MoveWaypoint(69, GetObjectX(var_1), GetObjectY(var_1));
            Delete(var_1);
            var_0 = 1;
        }
        else {
            MoveWaypoint(70, GetWaypointX(69) + RandomFloat(-110.0, 110.0), GetWaypointY(69) + RandomFloat(-110.0, 110.0));
            var_1 = CreateObject("CherubArrow", 70);
            SetOwner(player[arg_0], var_1);
            LookWithAngle(var_1, 64);
            Raise(var_1, 200.0);
            DeleteObjectTimer(var_1, 22);
            AudioEvent("CrossBowShoot", 70);
            var_0 += 1;
            LocalRisk(arg_0, 2, 69, 300.0);
        }
        FrameTimerWithArg(1, arg_0, horrendousSkillS);
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0 + 10] = 0;
        Delete(var_1);
    }
}

void hecubahSkillS(int plr)
{
    int progress, fxUnit;

    if (CurrentHealth(player[plr]) > 0 && progress < 2)
    {
        if (progress == 0)
        {
            fxUnit = CreateObjectAt("ManaBombCharge", GetObjectX(player[plr]), GetObjectY(player[plr]));
            PlaySoundAround(CreateObjectAt("AmbBeachBirds", GetObjectX(fxUnit), GetObjectY(fxUnit)), 80);
            SetOwner(player[plr], fxUnit + 1);
            Effect("JIGGLE", GetObjectX(fxUnit), GetObjectY(fxUnit), 50.0, 0.0);
            Enchant(player[plr], "ENCHANT_FREEZE", 0.0);
            Enchant(player[plr], "ENCHANT_ANCHORED", 0.0);
            Enchant(player[plr], "ENCHANT_INVULNERABLE", 2.0);
            stunPlayer(fxUnit + 1, 350.0);
            FrameTimerWithArg(57, plr, hecubahSkillS);
        }
        else if (progress == 1)
        {
            AroundSplashTriggered(fxUnit + 1, 370.0, 149);
            Effect("WHITE_FLASH", GetObjectX(fxUnit), GetObjectY(fxUnit), 0.0, 0.0);
            EnchantOff(player[plr], "ENCHANT_ANCHORED");
            EnchantOff(player[plr], "ENCHANT_FREEZE");
            FrameTimerWithArg(5, plr, hecubahSkillS);
        }
        progress++;
    }
    else {
        progress = 0;
        COOLDOWN[plr + 10] = 0;
        Delete(fxUnit);
        Delete(fxUnit + 1);
    }
}

void ogreSkillS(int arg_0) {
    int var_0;
    int var_1;
    int i;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 37) {
        if (var_0 == 0) {
            MoveWaypoint(79, GetObjectX(player[arg_0]), GetObjectY(player[arg_0]));
            DeleteObjectTimer(CreateObject("ForceOfNatureCharge", 79), 30);
        }
        MoveWaypoint(80, GetWaypointX(79) - MathSine((var_0 % 36) * 10, 30.0), GetWaypointY(79) - MathSine(((var_0 % 36) * 10) + 90, 30.0));
        for(i = 0; i < 5 ; i += 1) {
            var_1 = CreateObject("FanChakramInMotion", 80);
            Enchant(var_1, "ENCHANT_FREEZE", 0.0);
            SetOwner(player[arg_0], var_1);
            PushObject(var_1, 20.0, GetWaypointX(79), GetWaypointY(79));
        }
        var_0 += 1;
        FrameTimerWithArg(1, arg_0, ogreSkillS);
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0 + 10] = 0;
    }
}

void lichSkillS(int plr) {
    int var_0, glow;
    int var_2[18];
    int i;

    if (CurrentHealth(player[plr]) > 0 && var_0 < 50) {
        if (var_0 == 0) {
            glow = CreateObject("moonglow", 2);
            SetOwner(player[plr], glow);
            Enchant(player[plr], "ENCHANT_ANCHORED", 0.0);
        }
        else if (var_0 == 1)
        {
            if (IsVisibleTo(player[plr], glow))
            {
                MoveWaypoint(98, GetObjectX(glow), GetObjectY(glow));
            }
            else
            {
                MoveWaypoint(98, GetObjectX(player[plr]), GetObjectY(player[plr]));
                UniPrint(player[plr], "마우스 커서 지역을 볼수 없어서 자기 주변에 시전됩니다");
            }
            Delete(glow);
            flagMode(plr, 0);
        }
        else {
            flagMode(0, 1);
        }
        var_0 += 1;
        FrameTimerWithArg(1, plr, lichSkillS);
    }
    else {
        var_0 = 0;
        COOLDOWN[plr + 10] = 0;
        Delete(glow);
        EnchantOff(player[plr], "ENCHANT_ANCHORED");
        flagMode(0, 2);
    }
}

float wispExplosionPos(int num, float size) {
    float arr[4];

    if (num == -1)
    {
        arr[0] = 1.0;
        arr[1] = -1.0;
        arr[2] = 0.0;
        arr[3] = 0.0;
        return arr[0];
    }
    return arr[num] * size;
}

float rhombusXY(int num, float size) {
    float arr[12];

    if (num == -1)
    {
        arr[0] = size;
        arr[1] = -size;
        arr[2] = 0.0;
        arr[3] = 0.0;
        arr[4] = 0.0;
        arr[5] = 0.0;
        arr[6] = size;
        arr[7] = -size;
        arr[8] = size;
        arr[9] = -size;
        arr[10] = 0.0;
        arr[11] = 0.0;
        return arr[0];
    }
    return arr[num];
}

void rhombusRaiserFX(int unit)
{
    int k = 0;

    while(k < 4)
    {
        Effect("SENTRY_RAY", GetObjectX(unit) + rhombusXY(k, 0.0), GetObjectY(unit) + rhombusXY(3 - k, 0.0), GetObjectX(unit) + rhombusXY(k + 4, 0.0), GetObjectY(unit) + rhombusXY(k + 8, 0.0));
        k += 1;
    }
}

void GWizTouched()
{
    if (CurrentHealth(other) && IsAttackedBy(other, self))
        Damage(other, self, 100, 14);
}

int SkillDFuncPtr()
{
    StopScript(GWizSkillD);
}

void GWizSkillD(int arg_0)
{
    int var_0;

    if (CurrentHealth(player[arg_0]))
    {
        MoveWaypoint(47, GetObjectX(player[arg_0]) + UnitAngleCos(player[arg_0], 23.0), GetObjectY(player[arg_0]) + UnitAngleSin(player[arg_0], 23.0));
        var_0 = CreateObject("greenFrog", 47);
        SetCallback(var_0, 9, GWizTouched2);
        AudioEvent("GlyphCast", 47);
        SetUnitMaxHealth(var_0, arg_0 + 1);
        DeleteObjectTimer(var_0, 22);
        Enchant(var_0, "ENCHANT_INVULNERABLE", 0.0);
        Enchant(var_0, "ENCHANT_HASTED", 0.0);
        Enchant(var_0, "ENCHANT_RUN", 0.0);
        PushObject(var_0, 200.0, GetObjectX(player[arg_0]), GetObjectY(player[arg_0]));
    }
    COOLDOWN[arg_0 + 20] = 0;
}

void DemonSkillD(int plr)
{
    int progress;
    object glowPoint;

    if (CurrentHealth(player[plr]) && progress < 2)
    {
        if (progress == 0)
        {
            glowPoint = CreateObject("moonglow", 2);
            DeleteObjectTimer(glowPoint, 30);
            SetOwner(player[plr], glowPoint);
            progress = 1;
            FrameTimerWithArg(1, plr, DemonSkillD);
        }
        else if (progress == 1)
        {
            object flame;
            if (IsVisibleTo(player[plr], glowPoint))
                flame = CreateObjectAt("FireSprite", GetObjectX(glowPoint), GetObjectY(glowPoint));
            else
                flame = CreateObjectAt("FireSprite", GetObjectX(player[plr]), GetObjectY(player[plr]));
            Delete(glowPoint);
            LinkFireSpriteBin(flame);
            UnitNoCollide(flame);
            storageTemp(flame, 1);
            Raise(flame, 180.0);
            SetOwner(player[plr], flame);
            Enchant(flame, "ENCHANT_DEATH", 0.1);
            FrameTimerWithArg(3, plr, disableInvisible);
            progress = 2;
            FrameTimerWithArg(3, plr, DemonSkillD);
        }
    }
    else
    {
        progress = 0;
        COOLDOWN[plr + 20] = 0;
    }
}

void MecaGolemDSkill(int plr)
{
    int ptr;

    if (CurrentHealth(player[plr]))
    {
        COOLDOWN[plr + 20] = 0;
        ptr = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(player[plr]) + UnitAngleCos(player[plr], 23.0), GetObjectY(player[plr]) + UnitAngleSin(player[plr], 23.0));
        CreateObjectAt("InvisibleLightBlueLow", GetObjectX(ptr), GetObjectY(ptr));
        SetOwner(player[plr], ptr);
        LookWithAngle(ptr, GetDirection(player[plr]));
        FrameTimerWithArg(1, ptr, FlyingTrackingBullet);
    }
}

void stoneGolemSkillD(int arg_0) {
    float var_0;
    float var_1;
    int var_2;
    
    if (CurrentHealth(player[arg_0]))
    {
        var_0 = UnitAngleCos(player[arg_0], 30.0);
        var_1 = UnitAngleSin(player[arg_0], 30.0);
        MoveWaypoint(58, GetObjectX(player[arg_0]) + var_0, GetObjectY(player[arg_0]) + var_1);
        var_2 = CreateObject("boulder", 58);
        SetOwner(player[arg_0], var_2);
        Enchant(var_2, "ENCHANT_SHOCK", 0.0);
        Enchant(var_2, "ENCHANT_DEATH", 2.0);
        PushObject(var_2, 30.0, GetObjectX(player[arg_0]), GetObjectY(player[arg_0]));
    }
    COOLDOWN[arg_0 + 20] = 0;
}

void beholderSkillD(int arg_0) {
    if (CurrentHealth(player[arg_0]) > 0) {
        Enchant(player[arg_0], "ENCHANT_PROTECT_FROM_MAGIC", 1.0);
        CastSpellObjectLocation("SPELL_MAGIC_MISSILE", player[arg_0], GetObjectX(player[arg_0]) + UnitAngleCos(player[arg_0], 23.0), GetObjectY(player[arg_0]) + UnitAngleSin(player[arg_0], 23.0));
    }
    COOLDOWN[arg_0 + 20] = 0;
}

void RWizSkillD(int plr)
{
    int durate, statue;

    if (CurrentHealth(player[plr]) > 0 && durate < 52)
    {
        if (durate == 0)
        {
            MoveWaypoint(67, GetObjectX(player[plr]) + UnitAngleCos(player[plr], 30.0), GetObjectY(player[plr]) + UnitAngleSin(player[plr], 30.0));
            statue = CreateObject("MovableStatueVictory4SW", 67);
            SetOwner(player[plr], statue);
            // LookWithAngle(var_1, plr);
        }
        else {
            LightningRisk(statue, plr);
        }
        durate += 1;
        FrameTimerWithArg(1, plr, RWizSkillD);
    }
    else {
        durate = 0;
        COOLDOWN[plr + 20] = 0;
        Delete(statue);
    }
}

void horrendousSkillD(int arg_0) {
    int var_0;
    int unit;
    float pos_x;
    float pos_y;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 21) {
        if (!var_0)
        {
            pos_x = UnitAngleCos(player[arg_0], 20.0);
            pos_y = UnitAngleSin(player[arg_0], 20.0);
            MoveWaypoint(71, GetObjectX(player[arg_0]) + pos_x, GetObjectY(player[arg_0]) + pos_y);
            MoveObject(GetMagic(), GetWaypointX(71), GetWaypointY(71));
            unit = CreateObject("Maiden", 71);
            LookWithAngle(unit, arg_0);
            Frozen(unit, 1);
            SetCallback(unit, 9, horrendousTouched);
        }
        else if (IsObjectOn(unit))
        {
            MoveWaypoint(71, GetWaypointX(71) + pos_x, GetWaypointY(71) + pos_y);
            MoveObject(unit, GetWaypointX(71), GetWaypointY(71));
            MoveObject(GetMagic(), GetWaypointX(71), GetWaypointY(71));
        }
        else
            var_0 = 255;
        var_0 ++;
        FrameTimerWithArg(1, arg_0, horrendousSkillD);
    }
    else {
        if (CurrentHealth(unit))
            Delete(unit);
        MoveObject(GetMagic(), GetWaypointX(107), GetWaypointY(107));
        var_0 = 0;
        COOLDOWN[arg_0 + 20] = 0;
    }
}

void hecubahSkillD(int arg_0) {
    int i;
    int var_0;

    if (CurrentHealth(player[arg_0]) > 0) {
        MoveWaypoint(75, GetObjectX(player[arg_0]) + UnitAngleCos(player[arg_0], 20.0), GetObjectY(player[arg_0]) + UnitAngleSin(player[arg_0], 20.0));
        AudioEvent("ForceOfNatureRelease", 75);
        for(i = 0 ; i < 3 ; i += 1) {
            var_0 = CreateObject("DeathBallFragment", 75);
            Enchant(var_0, "ENCHANT_FREEZE", 0.0);
            SetOwner(player[arg_0], var_0);
            PushObject(var_0, 30.0, GetObjectX(player[arg_0]), GetObjectY(player[arg_0]));
            DeleteObjectTimer(var_0, 17);
        }
    }
    COOLDOWN[arg_0 + 20] = 0;
}

void ogreSkillD(int arg_0) {
    int var_0;
    int var_1;
    int i;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 2) {
        if (var_0 == 0) {
            EnchantOff(player[arg_0], "ENCHANT_FREEZE");
            Enchant(player[arg_0], "ENCHANT_INVULNERABLE", 0.0);
            PushObject(player[arg_0], -150.0, GetObjectX(player[arg_0]) + UnitAngleCos(player[arg_0], 20.0), GetObjectY(player[arg_0]) + UnitAngleSin(player[arg_0], 20.0));
            FrameTimerWithArg(7, arg_0, ogreSkillD);
        }
        else {
            EnchantOff(player[arg_0], "ENCHANT_INVULNERABLE");
            Enchant(player[arg_0], "ENCHANT_FREEZE", 1.0);
            for(i = 0 ; i < 12 ; i += 1) {
                MoveWaypoint(81, GetObjectX(player[arg_0]) + MathSine(i * 30, 50.0), GetObjectY(player[arg_0]) + MathSine(i * 30 + 90, 50.0));
                var_1 = CreateObject("CarnivorousPlant", 81);
                Frozen(var_1, 1);
                LookWithAngle(var_1, arg_0);
                SetCallback(var_1, 9, OgreCollisionEvent2);
                DeleteObjectTimer(var_1, 1);
                DeleteObjectTimer(CreateObject("Smoke", 81), 5);
                Effect("SENTRY_RAY", GetWaypointX(81), GetWaypointY(81), GetObjectX(player[arg_0]), GetObjectY(player[arg_0]));
            }
            FrameTimerWithArg(1, arg_0, ogreSkillD);
        }
        var_0 += 1;
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0 + 20] = 0;
    }
}

void lichSkillD(int arg_0)
{
    int var_0;
    int var_1;
    int var_2;
    float var_3;
    float var_4;

    if (CurrentHealth(player[arg_0]) && var_0 < 30)
    {
        if (!var_0)
        {
            var_1 = GetDirection(player[arg_0]);
            Enchant(player[arg_0], "ENCHANT_ANCHORED", 0.0);
            var_3 = UnitAngleCos(player[arg_0], 22.0);
            var_4 = UnitAngleSin(player[arg_0], 22.0);
            MoveWaypoint(100, GetObjectX(player[arg_0]) + var_3, GetObjectY(player[arg_0]) + var_4);
            var_2 = CreateObject("BlackWolf", 100);
            CreateObject("InvisibleLightBlueHigh", 100);
            LookWithAngle(var_2 + 1, arg_0);
            Frozen(var_2, 1);
            SetOwner(player[arg_0], var_2);
            SetCallback(var_2, 9, lichTouched);
        }
        else if (CurrentHealth(var_2))
        {
            LookWithAngle(var_2, var_1);
            Walk(var_2, GetObjectX(var_2), GetObjectY(var_2));
            MoveObject(var_2, GetObjectX(var_2) + var_3, GetObjectY(var_2) + var_4);
        }
        var_0 ++;
        FrameTimerWithArg(1, arg_0, lichSkillD);
    }
    else
    {
        EnchantOff(player[arg_0], "ENCHANT_ANCHORED");
        Delete(var_2);
        Delete(var_2 + 1);
        var_0 = 0;
        COOLDOWN[arg_0 + 20] = 0;
    }
}

void GWizTouched2() {
    int var_0 = CurrentHealth(self) - 1;

    if (CurrentHealth(other) > 0 && IsAttackedBy(other, player[var_0])) {
        Damage(other, player[var_0], 70, 14);
        Effect("THIN_EXPLOSION", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Delete(self);
    }
}

void FlyingTrackingBullet(int ptr)
{
    int owner = GetOwner(ptr), count = ToInt(GetObjectZ(ptr)), target;

    if (CurrentHealth(owner) && count < 25)
    {
        if (GetDirection(ptr + 1))
        {
            target = ToInt(GetObjectZ(ptr + 1));
            if (CurrentHealth(target))
            {
                if (Distance(GetObjectX(ptr), GetObjectY(ptr), GetObjectX(target), GetObjectY(target)) < 37.0)
                {
                    MoveWaypoint(76, GetObjectX(ptr), GetObjectY(ptr));
                    AudioEvent("HammerMissing", 76);
                    GreenExplosion(GetObjectX(ptr), GetObjectY(ptr));
                    Damage(target, owner, 48, 14);
                    count = 200;
                }
                else
                {
                    LookAtObject(ptr, target);
                    MoveObject(ptr, GetObjectX(ptr) + UnitRatioX(target, ptr, 25.0), GetObjectY(ptr) + UnitRatioY(target, ptr, 25.0));
                }
            }
            else
                LookWithAngle(ptr + 1, 0);
        }
        else
        {
            MoveObject(ptr, GetObjectX(ptr) + UnitAngleCos(ptr, 25.0), GetObjectY(ptr) + UnitAngleSin(ptr, 25.0));
            DetectAroundEnemy(ptr);
        }
        LookWithAngle(HarpoonFxAtLocation(ptr, 9), GetDirection(ptr));
        Raise(ptr, ToFloat(count + 1));
        FrameTimerWithArg(1, ptr, FlyingTrackingBullet);
    }
    else
    {
        Delete(ptr);
        Delete(ptr + 1);
    }
}

int HarpoonFxAtLocation(int unit, int lftime)
{
    int unit2 = CreateObjectAt("HarpoonBolt", GetObjectX(unit), GetObjectY(unit));

    Frozen(unit2, 1);
    DeleteObjectTimer(unit2, lftime);
    Effect("CYAN_SPARKS", GetObjectX(unit2), GetObjectY(unit2), 0.0, 0.0);
    return unit2;
}

void DetectAroundEnemy(int ptr)
{
    int unit = CreateObjectAt("InvisibleLightBlueLow", GetObjectX(ptr), GetObjectY(ptr)) + 1, k;

    Raise(unit - 1, 5000.0);
    SetOwner(ptr, unit - 1);
    for (k = 0 ; k < 8 ; k ++)
    {
        UnitNoCollide(CreateObjectAt("WeirdlingBeast", GetObjectX(ptr), GetObjectY(ptr)));
        SetOwner(unit - 1, unit + k);
        LookWithAngle(unit + k, 32 * k);
        Enchant(unit + k, "ENCHANT_INFRAVISION", 0.0);
        SetCallback(unit + k, 3, UnitDetectEvent);
        DeleteObjectTimer(unit + k, 1);
    }
    DeleteObjectTimer(unit - 1, 1);
}

void UnitDetectEvent()
{
    int ptr = GetOwner(GetOwner(self)), unit = GetOwner(self);

    if (HasClass(other, "PLAYER"))
    {
        if (Distance(GetObjectX(other), GetObjectY(other), GetObjectX(self), GetObjectY(self)) < GetObjectZ(unit))
        {
            Raise(unit, Distance(GetObjectX(other), GetObjectY(other), GetObjectX(self), GetObjectY(self)));
            LookWithAngle(ptr + 1, 1);
            Raise(ptr + 1, ToFloat(GetCaller()));
        }
    }
}

int SkillJFuncPtr()
{
    StopScript(GWizSkillJ);
}

void GWizSkillJ(int arg_0) {
    int var_0;
    int var_1;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 2) {
        if (var_0 == 0) {
            var_1 = CreateObject("moonglow", 2);
            SetOwner(player[arg_0], var_1);
            var_0 = 1;
        }
        else {
            MoveWaypoint(47, GetObjectX(var_1), GetObjectY(var_1));
            Delete(var_1);
            CastSpellObjectLocation("SPELL_METEOR_SHOWER", player[arg_0], GetWaypointX(47), GetWaypointY(47));
            Enchant(player[arg_0], "ENCHANT_FREEZE", 1.0);
            Enchant(player[arg_0], "ENCHANT_INVULNERABLE", 1.0);
            var_0 = 2;
        }
        FrameTimerWithArg(1, arg_0, GWizSkillJ);
    }
    else {
        var_0 = 0;
        Delete(var_1);
        COOLDOWN[arg_0 + 30] = 0;
    }
}

void DemonSkillJ(int plr)
{
    if (CurrentHealth(player[plr]))
    {
        float xPos = GetObjectX(player[plr]), yPos = GetObjectY(player[plr]);
        float vectX = UnitAngleCos(player[plr], -23.0), vectY = UnitAngleSin(player[plr], -23.0);
        object missile = DemonsArmageddon(xPos - vectX, yPos - vectY, player[plr]);

        DemonsArmageddon(xPos + (0.2 * vectY) - vectX, yPos - (0.2 * vectX) - vectY, player[plr]);
        DemonsArmageddon(xPos - (0.2 * vectY) - vectX, yPos + (0.2 * vectX) - vectY, player[plr]);

        int u;

        for (u = 0 ; u < 3 ; u ++)
        {
            LookAtObject(missile + u, player[plr]);
            LookWithAngle(missile + u, GetDirection(missile + u) + 128);
            PushObject(missile + u, 30.0, xPos, yPos);
        }
    }
    COOLDOWN[plr + 30] = 0;
}

void MecaGolemSkillJ(int plr)
{
    int x, subUnit;
    float pos_x, pos_y;

    if (CurrentHealth(player[plr]) && x < 30)
    {
        if (!x)
        {
            pos_x = UnitAngleCos(player[plr], 19.0);
            pos_y = UnitAngleSin(player[plr], 19.0);
            Enchant(player[plr], "ENCHANT_FREEZE", 0.0);
        }
        MoveWaypoint(57, GetObjectX(player[plr]) - pos_x, GetObjectY(player[plr]) - pos_y);
        AudioEvent("FirewalkOn", 57);
        subUnit = CreateObject("CarnivorousPlant", 57);
        Frozen(subUnit, true);
        LookWithAngle(subUnit, plr);
        SetCallback(subUnit, 9, MecaBerserkerTouched);
        DeleteObjectTimer(subUnit, 1);
        DeleteObjectTimer(CreateObject("MeteorExplode", 57), 6);
        x ++;
        FrameTimerWithArg(1, plr, MecaGolemSkillJ);
    }
    else
    {
        EnchantOff(player[plr], "ENCHANT_FREEZE");
        x = 0;
        COOLDOWN[plr + 30] = 0;
    }
}

void stoneGolemSkillJ(int arg_0) {
    int var_0;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 80) {
        if (var_0 == 0) {
            Enchant(player[arg_0], "ENCHANT_PROTECT_FROM_MAGIC", 4.0);
            Enchant(player[arg_0], "ENCHANT_BURNING", 0.0);
        }
        if (!HasItem(player[arg_0], stoneGolemInv()))
            Pickup(player[arg_0], stoneGolemInv());
        else {
            Drop(player[arg_0], stoneGolemInv());
            MoveObject(stoneGolemInv(), 100.0, 100.0);
        }
        PushObject(player[arg_0], -5.0, GetObjectX(player[arg_0]) + UnitAngleCos(player[arg_0], 20.0), GetObjectY(player[arg_0]) + UnitAngleSin(player[arg_0], 20.0));
        var_0 += 1;
        FrameTimerWithArg(1, arg_0, stoneGolemSkillJ);
    }
    else {
        EnchantOff(player[arg_0], "ENCHANT_BURNING");
        Drop(player[arg_0], stoneGolemInv());
        MoveObject(stoneGolemInv(), 100.0, 100.0);
        var_0 = 0;
        COOLDOWN[arg_0 + 30] = 0;
    }
}

void beholderSkillJ(int arg_0) {
    int var_0;
    float var_1;
    float var_2;
    int var_3;
    int i;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 2) {
        if (var_0 == 0) {
            var_0 += 1;
            var_1 = UnitAngleCos(player[arg_0], 30.0);
            var_2 = UnitAngleSin(player[arg_0], 30.0);
            Enchant(player[arg_0], "ENCHANT_ANCHORED", 0.0);
            Enchant(player[arg_0], "ENCHANT_FREEZE", 0.0);
            MoveWaypoint(61, GetObjectX(player[arg_0]) + var_1, GetObjectY(player[arg_0]) + var_2);
            for(i = 0 ; i < 12 ; i += 1) {
                MoveWaypoint(61, GetWaypointX(61) + var_1, GetWaypointY(61) + var_2);
                var_3 = CreateObject("CarnivorousPlant", 61);
                SetOwner(player[arg_0], var_3);
                Frozen(var_3, 1);
                LookWithAngle(var_3, arg_0);
                SetCallback(var_3, 9, beholderTouched);
                DeleteObjectTimer(var_3, 1);
            }
            Effect("SENTRY_RAY", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), GetWaypointX(61), GetWaypointY(61));
            MoveObject(player[arg_0], GetWaypointX(61), GetWaypointY(61));
            Effect("TELEPORT", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), 0.0 ,0.0);
            FrameTimerWithArg(20 , arg_0, beholderSkillJ);
        }
        else if (var_0 == 1) {
            var_0 += 1;
            for(i = 0 ; i < 12 ; i += 1) {
                MoveWaypoint(61, GetWaypointX(61) - var_1, GetWaypointY(61) - var_2);
                var_3 = CreateObject("CarnivorousPlant", 61);
                SetOwner(player[arg_0], var_3);
                Frozen(var_3, 1);
                LookWithAngle(var_3, arg_0);
                SetCallback(var_3, 9, beholderTouched);
                DeleteObjectTimer(var_3, 1);
            }
            Effect("SENTRY_RAY", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), GetWaypointX(61), GetWaypointY(61));
            MoveObject(player[arg_0], GetWaypointX(61), GetWaypointY(61));
            Effect("TELEPORT", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), 0.0, 0.0);
            FrameTimerWithArg(1, arg_0, beholderSkillJ);
        }
    }
    else {
        EnchantOff(player[arg_0], "ENCHANT_FREEZE");
        EnchantOff(player[arg_0], "ENCHANT_ANCHORED");
        var_0 = 0;
        COOLDOWN[arg_0 + 30] = 0;
    }
}

void RWizSkillJ(int plr)
{
    int check;
    float pos_x = GetObjectX(player[plr]), pos_y = GetObjectY(player[plr]);

    if (CurrentHealth(player[plr]))
    {
        MoveWaypoint(67, pos_x + UnitAngleCos(player[plr], 100.0), pos_y + UnitAngleSin(player[plr], 100.0));
        check = CreateObject("ghost", 67);
        Enchant(player[plr], "ENCHANT_SHIELD", 1.5);
        if (IsVisibleTo(player[plr], check))
            MoveObject(player[plr], GetWaypointX(67), GetWaypointY(67));
        else
            UniPrint(player[plr], "그곳을 볼 수 없습니다");
        CastSpellObjectObject("SPELL_INVERSION", player[plr], player[plr]);
        Effect("COUNTERSPELL_EXPLOSION", GetWaypointX(67), GetWaypointY(67), 0.0, 0.0);
        GreenLightningFx(FloatToInt(pos_x), FloatToInt(pos_y), FloatToInt(GetWaypointX(67)), FloatToInt(GetWaypointY(67)), 16);
        AudioEvent("BallThrow", 67);
        Delete(check);
    }
    COOLDOWN[plr + 30] = 0;
}

void horrendousSkillJ(int plr)
{
    int progress;
    int glowPoint;
    float var_2;
    float var_3;
    float var_4;
    float var_5;
    float var_6;
    float var_7;
    float var_8;
    float var_9;
    float var_10;

    if (CurrentHealth(player[plr]) && progress < 10)
    {
        if (progress == 0)
        {
            glowPoint = CreateObject("moonglow", 2);
            SetOwner(player[plr], glowPoint);
            Enchant(player[plr], "ENCHANT_FREEZE", 2.0);
            MoveWaypoint(72, GetObjectX(player[plr]), GetObjectY(player[plr]));
            progress = -1;
        }
        else if (progress == -1) {
            var_2 = Distance(GetObjectX(player[plr]), GetObjectY(player[plr]), GetObjectX(glowPoint), GetObjectY(glowPoint));
            var_3 = (GetObjectX(player[plr]) - GetObjectX(glowPoint)) * 15.0 / var_2; //dx
            var_4 = (GetObjectY(player[plr]) - GetObjectY(glowPoint)) * 15.0 / var_2; //dy
            var_5 = var_3;
            var_6 = var_4;
            Delete(glowPoint);
            glowPoint = CreateObject("InvisibleLightBlueHigh", 72);
            progress = 1;
        }
        else
        {
            spawnArrowWithHor(plr, GetObjectX(glowPoint) - var_3, GetObjectY(glowPoint) - var_4);
            spawnArrowWithHor(plr, GetObjectX(glowPoint) - var_5, GetObjectY(glowPoint) - var_6);
            var_7 = GetObjectX(glowPoint) - (0.1 * var_4) - var_3;
            var_8 = GetObjectY(glowPoint) + (0.1 * var_3) - var_4;
            var_9 = GetObjectX(glowPoint) + (0.1 * var_6) - var_5;
            var_10 = GetObjectY(glowPoint) - (0.1 * var_5) - var_6;
            var_2 = Distance(GetObjectX(glowPoint), GetObjectY(glowPoint), var_7, var_8);
            var_3 = (GetObjectX(glowPoint) - var_7) * 20.0 / var_2;
            var_4 = (GetObjectY(glowPoint) - var_8) * 20.0 / var_2;
            var_5 = (GetObjectX(glowPoint) - var_9) * 20.0 / var_2;
            var_6 = (GetObjectY(glowPoint) - var_10) * 20.0 / var_2;
            progress += 1;
        }
        FrameTimerWithArg(1, plr, horrendousSkillJ);
    }
    else {
        progress = 0;
        COOLDOWN[plr + 30] = 0;
        Delete(glowPoint);
    }
}

void hecubahSkillJ(int plr) {
    int progress;
    int glowPoint;
    float vectX, vectY;
    int flames[4];
    
    if (CurrentHealth(player[plr]) > 0 && progress < 20) {
        if (progress == 0) {
            glowPoint = CreateObject("moonglow", 2);
            SetOwner(player[plr], glowPoint);
            Enchant(player[plr], "ENCHANT_ANCHORED", 0.0);
        }
        else if (progress == 1)
        {
            if (IsVisibleTo(player[plr], glowPoint))
            {
                vectX = UnitAngleCos(player[plr], -20.0); //dx
                vectY = UnitAngleSin(player[plr], -20.0); //dy
                MoveWaypoint(75, GetObjectX(glowPoint), GetObjectY(glowPoint));
                MoveWaypoint(76, GetWaypointX(75), GetWaypointY(75));
            }
            else
            {
                progress = 255;
                UniPrint(player[plr], "커서 위치를 볼 수 없습니다");
            }
            Delete(glowPoint);
        }
        else {
            flames[0] = CreateObject("LargeFlame", 75);
            flames[1] = CreateObject("LargeFlame", 76);
            flames[2] = CreateObject("fireball", 75);
            flames[3] = CreateObject("fireball", 76);
            DeleteObjectTimer(flames[0], 120);
            DeleteObjectTimer(flames[1], 120);
            DeleteObjectTimer(CreateObject("barrel", 75), 3);
            DeleteObjectTimer(CreateObject("barrel", 76), 3);
            SetOwner(player[plr], flames[0]);
            SetOwner(player[plr], flames[1]);
            SetOwner(player[plr], flames[2]);
            SetOwner(player[plr], flames[3]);
            MoveWaypoint(75, GetWaypointX(75) + vectY, GetWaypointY(75) - vectX);
            MoveWaypoint(76, GetWaypointX(76) - vectY, GetWaypointY(76) + vectX);
            AudioEvent("FirewalkOff", 75);
            AudioEvent("FirewalkOff", 76);
        }
        progress += 1;
        FrameTimerWithArg(1, plr, hecubahSkillJ);
    }
    else {
        progress = 0;
        EnchantOff(player[plr], "ENCHANT_ANCHORED");
        COOLDOWN[plr + 30] = 0;
        Delete(glowPoint);
    }
}

void ogreSkillJ(int arg_0) { //not_yet
    int var_0;
    float var_1;
    float var_2;

    if (CurrentHealth(player[arg_0]) > 0 && var_0 < 30) {
        if (var_0 == 0) {
            var_1 = UnitAngleCos(player[arg_0], 21.0);
            var_2 = UnitAngleSin(player[arg_0], 21.0);
            MoveWaypoint(84, GetObjectX(player[arg_0]) + var_1, GetObjectY(player[arg_0]) + var_2);
            ogreSpawnBarrel(0, 84);
            LookWithAngle(ogreTrg(), arg_0);
            MoveObject(ogreTrg(), GetObjectX(ogreSpawnBarrel(1, 0)), GetObjectY(ogreSpawnBarrel(1, 0)));
            ObjectOn(ogreTrg());
        }
        else {
            if (ogreSpawnBarrel(1, 0) != 0) {
                MoveObject(ogreSpawnBarrel(1, 0), GetObjectX(ogreSpawnBarrel(1, 0)) + var_1, GetObjectY(ogreSpawnBarrel(1, 0)) + var_2);
                MoveObject(ogreTrg(), GetObjectX(ogreSpawnBarrel(1, 0)), GetObjectY(ogreSpawnBarrel(1, 0)));
            }
            else
                var_0 = 30;
        }
        var_0 += 1;
        FrameTimerWithArg(1, arg_0, ogreSkillJ);
    }
    else {
        var_0 = 0;
        COOLDOWN[arg_0 + 30] = 0;
        ogreSpawnBarrel(2, 0);
        ObjectOff(ogreTrg());
    }
}

void lichSkillJ(int arg_0) {
    int var_0;

    if (CurrentHealth(player[arg_0]) > 0) {
        UniChatMessage(player[arg_0], "순보...!", 109);
        MoveWaypoint(99, GetObjectX(player[arg_0]) + UnitAngleCos(player[arg_0], 140.0), GetObjectY(player[arg_0]) + UnitAngleSin(player[arg_0], 140.0));
        var_0 = CreateObject("redPotion2", 99);
        if (IsVisibleTo(player[arg_0], var_0)) {
            MoveObject(player[arg_0], GetWaypointX(99), GetWaypointY(99));
            Effect("SMOKE_BLAST", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), 0.0, 0.0);
            Effect("TELEPORT", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), 0.0, 0.0);
            AudioEvent("BlindOn", 99);
        }
        Enchant(player[arg_0], "ENCHANT_SHOCK", 3.0);
        Delete(var_0);
    }
    COOLDOWN[arg_0 + 30] = 0;
}

void demonTouched()
{
    int plr = GetDirection(self);

    if (CurrentHealth(other) && IsAttackedBy(other, player[plr]))
    {
        Damage(other, player[plr], 130, 7);
        Enchant(other, "ENCHANT_HELD", 3.0);
        Enchant(other, "ENCHANT_CHARMING", 0.2);
    }
}

void demonTouched2()
{
    int plr = GetDirection(self);

    if (CurrentHealth(other) && IsAttackedBy(other, player[plr]))
    {
        Damage(other, player[plr], 100, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
    }
}

int storageTemp(int arg_0, int arg_1) { //unit, mode (0: release, 1: save)
    int var_0;
    
    if (arg_1 == 1)
        var_0 = arg_0;
    return var_0;
}
void disableInvisible(int arg_0) {
    EnchantOff(player[arg_0], "ENCHANT_INVISIBLE");
    SetOwner(player[arg_0], storageTemp(0, 0));
}

int demonSkillJwayBuffer(int arg_0) {
    int var_0[3];

    if (arg_0 == -1) {
        var_0[0] = 50;
        var_0[1] = 53;
        var_0[2] = 54;
        return var_0[0];
    }
    return var_0[arg_0];
}

void mecaGolemTouch() {
    int var_0 = GetDirection(self);

    if (CurrentHealth(other) > 0 && IsAttackedBy(other, player[var_0])) {
        Damage(other, player[var_0], 75, 14);
        Enchant(other, "ENCHANT_FREEZE", 2.0);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
    }
}

void MecaBerserkerTouched()
{
    int plr = GetDirection(self);

    if (CurrentHealth(other) && IsAttackedBy(other, player[plr]))
    {
        Damage(other, player[plr], 120, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
        Enchant(other, "ENCHANT_HELD", 2.0);
    }
}

void aroundSpreadMissiles(int arg_0) {
    int var_0 = 0;
    int var_1;

    while(var_0 < 20) {
        MoveWaypoint(52, GetObjectX(player[arg_0]) - MathSine(var_0 * 18 + 90, 35.0), GetObjectY(player[arg_0]) - MathSine(var_0 * 18, 35.0));
        var_1 = CreateObject("CarnivorousPlant", 52);
        LookWithAngle(var_1, arg_0);
        Frozen(var_1, 1);
        SetCallback(var_1, 9, mecaGolemTouch2);
        DeleteObjectTimer(var_1, 1);
        DeleteObjectTimer(CreateObject("SmallFireBoom", 52), 5);
        var_0 += 1;
    }
    AudioEvent("HammerMissing", 52);
    Effect("JIGGLE", GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), 50.0 , 0.0);
}
void mecaGolemTouch2() {
    int var_0 = GetDirection(self);

    if (CurrentHealth(other) > 0 && IsAttackedBy(other, player[var_0])) {
        Damage(other, player[var_0], 70, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
    }
}

void explosionMeca(int arg_0) {
    int var_0;
    int i;

    for(i = 0; i < 4 ; i += 1) {
        MoveWaypoint(56, GetWaypointX(55) + wispExplosionPos(i, 30.0), GetWaypointY(55) + wispExplosionPos(3 - i, 30.0));
        var_0 = CreateObject("CarnivorousPlant", 56);
        LookWithAngle(var_0, arg_0);
        Frozen(var_0, 1);
        SetCallback(var_0, 9, mecaGolemTouch3);
        DeleteObjectTimer(var_0, 1);
        Effect("SPARK_EXPLOSION", GetWaypointX(56), GetWaypointY(56), 0.0, 0.0);
    }
    AudioEvent("FireballExplode", 56);
}
void mecaGolemTouch3() {
    int var_0 = GetDirection(self);

    if (CurrentHealth(other) > 0 && IsAttackedBy(other, player[var_0])) {
        Damage(other, player[var_0], 70, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
        Enchant(other, "ENCHANT_FREEZE", 2.0);
    }
}

void bombLightningSparks(int arg_0) {
    int var_0 = arg_0 / 256; //wp_num
    int var_1 = arg_0 % 256; //player_idx
    int var_2[10];
    int i;

    var_2[9] = CreateObject("InvisibleLightBlueHigh", var_0);
    for(i = 0 ; i < 9 ; i += 1) {
        MoveWaypoint(var_0, GetObjectX(var_2[9]) + lightningXY(i / 3), GetObjectY(var_2[9]) + lightningXY(i % 3));
        Effect("LIGHTNING", GetWaypointX(var_0), GetWaypointY(var_0), GetWaypointX(var_0), GetWaypointY(var_0) - 100.0);
        var_2[i] = CreateObject("CarnivorousPlant", var_0);
        SetOwner(player[var_1], var_2[i]);
        SetCallback(var_2[i], 9, lightningTouched);
        Frozen(var_2[i], 1);
        LookWithAngle(var_2[i], var_1);
        DeleteObjectTimer(var_2[i] , 1);
    }
    Delete(var_2[9]);
}
void lightningTouched() {
    int var_0 = GetDirection(self);

    if (CurrentHealth(other) > 0 && IsAttackedBy(other, player[var_0])) {
        Damage(other, player[var_0], 105, 14);
        Enchant(other, "ENCHANT_FREEZE", 2.0);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
    }
}

void lightningTouched2() {
    int var_0 = GetDirection(self);

    if (CurrentHealth(other) > 0 && IsAttackedBy(other, player[var_0])) {
        Damage(other, player[var_0], 50, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.4);
    }
}
void BeholderAroundDamage(int arg_0) {
    int var_0 = 0;
    float var_1;

    while(var_0 < 10) {
        if (CurrentHealth(player[var_0]) > 0 && var_0 != arg_0) {
            if (Distance(GetObjectX(player[arg_0]), GetObjectY(player[arg_0]), GetObjectX(player[var_0]), GetObjectY(player[var_0])) < 200.0) {
                Damage(player[var_0], player[arg_0], 2, 14);
                Effect("RICOCHET", GetObjectX(player[var_0]), GetObjectY(player[var_0]), 0.0 , 0.0);
            }
        }
        var_0 += 1;
    }
}

void beholderTouched() {
    int var_0 = GetDirection(self);

    if (IsAttackedBy(other, player[var_0]) && CurrentHealth(other) > 0) {
        Damage(other, player[var_0], 75, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.5);
    }
}

void RWizTouched()
{
    int own = GetDirection(self);

    if (IsAttackedBy(other, player[own]) && !HasEnchant(other, "ENCHANT_ETHEREAL"))
    {
        Enchant(other, "ENCHANT_ETHEREAL", 1.5);
        Damage(other, player[own], 80, 14);
    }
}

void explosionDelayFireStorm(int arg_0) {
    int var_0 = GetDirection(arg_0);
    int var_1;

    MoveWaypoint(66, GetObjectX(arg_0), GetObjectY(arg_0));
    Delete(arg_0);
    SetOwner(player[var_0], CreateObject("StrongFireball", 66));
    var_1 = CreateObject("barrel", 66);
    Frozen(var_1, 1);
    DeleteObjectTimer(var_1, 1);
}

void LightningRisk(int statue, int currentplrIndex)
{
    int u, me = player[currentplrIndex];

    for (u = 9 ; u >= 0 ; u -= 1)
    {
        int you = player[u];

        if (you == me)
            continue;
        if (!CurrentHealth(you))
            continue;

        if (IsVisibleTo(statue, you) || IsVisibleTo(you, statue))
        {
            if (Distance(GetObjectX(you), GetObjectY(you), GetObjectX(statue), GetObjectY(statue)) < 277.0)
            {
                Damage(you, me, 2, 16);
                PlaySoundAround(you, 78);
                Effect("LIGHTNING", GetObjectX(statue), GetObjectY(statue), GetObjectX(you), GetObjectY(you));
            }
        }
    }
}

void controlBlock(int plr, int glowPoint, float size, int mode)
{
    //player_index, base_unit, range, mode(0: spawn, 1: delete)
    int blocks[40], i;
    float pointX = GetObjectX(glowPoint), pointY = GetObjectY(glowPoint) + 100.0;

    for (i = 0 ; i < 40 ; i += 1)
    {
        if (mode == false)
        {
            float distSize = Distance(GetObjectX(glowPoint), GetObjectY(glowPoint), pointX, pointY);
            float vectX = (GetObjectX(glowPoint) - pointX) * size / distSize; //dx
            float vectY = (GetObjectY(glowPoint) - pointY) * size / distSize; //dy
            blocks[i] = CreateObjectAt("RotatingSpikes", GetObjectX(glowPoint) - vectX, GetObjectY(glowPoint) - vectY);
            SetOwner(player[plr], blocks[i]);
            Enchant(blocks[i], "ENCHANT_SHOCK", 0.0);
            Frozen(blocks[i], 1);
            pointX = GetObjectX(glowPoint) - (0.2 * vectY) - vectX;
            pointY = GetObjectY(glowPoint) + (0.2 * vectX) - vectY;
        }
        else
            Delete(blocks[i]);
    }
}

void LocalRisk(int plr, int amount, object location, float range)
{
    //index, damage_value, area, range
    int i;

    for (i = 0; i < 10 ; i += 1)
    {
        if (CurrentHealth(player[i]) && i != plr)
        {
            if (Distance(GetObjectX(player[i]), GetObjectY(player[i]), GetWaypointX(location), GetWaypointY(location)) < range)
                Damage(player[i], player[plr], amount, 14);
        }
    }
}

void horrendousTouched()
{
    int var_0 = GetDirection(self);

    if (CurrentHealth(other) > 0 && IsAttackedBy(other, player[var_0]) && HasClass(other, "PLAYER"))
    {
        Effect("CYAN_SPARKS", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Damage(other, player[var_0], 70, 14);
        Enchant(other, "ENCHANT_FREEZE", 2.0);
        Enchant(other, "ENCHANT_CHARMING", 1.0);
    }
    else if (!GetCaller())
        Delete(self);
}

int GetMagic()
{
    int unit;

    if (!unit) unit = Object("IceShoot");
    return unit;
}

void WarHarpoonTouched()
{
    int owner = GetOwner(self);

    while (true)
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Enchant(other, "ENCHANT_CHARMING", 0.1);
            DrawGeometryRingAt("CharmOrb", GetObjectX(other), GetObjectY(other), 0);
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

int spawnArrowWithHor(int plr, float xProfile, float yProfile)
{
    int missile = CreateObjectAt("HarpoonBolt", xProfile, yProfile), owner = player[plr];
    int ptr = GetMemory(0x750710);

    SetMemory(ptr + 0x2e8, 5483536); //projectile update
    SetUnitCallbackOnCollide(missile, WarHarpoonTouched);
    LookAtObject(missile, owner);
    SetOwner(owner, missile);
    LookWithAngle(missile, GetDirection(missile) + 128);
    PushObject(missile, 23.0, GetObjectX(owner), GetObjectY(owner));
    Enchant(missile, "ENCHANT_FREEZE", 0.0);

    return missile;
}

void stunPlayer(object splashUnit, float range)
{
    int i, owner = GetOwner(splashUnit);

    for (i = 0 ; i < 10 ; i += 1)
    {
        if (player[i] ^ owner)
        {
            if (CurrentHealth(player[i]))
            {
                if (IsVisibleTo(player[i], splashUnit) || IsVisibleTo(splashUnit, player[i]))
                {
                    float tempDist = Distance(GetObjectX(player[i]), GetObjectY(player[i]), GetObjectX(splashUnit), GetObjectY(splashUnit));
                    if (tempDist < range)
                        Enchant(player[i], "ENCHANT_FREEZE", 1.0);
                }
            }
        }
    }
}

void AroundSplashTriggered(object splashUnit, float range, int amount)
{
    int i, owner = GetOwner(splashUnit);
    
    for (i = 0 ; i < 10 ; i += 1)
    {
        if (player[i] == owner)
            continue;
        if (CurrentHealth(player[i]))
        {
            if (IsVisibleTo(player[i], splashUnit) || IsVisibleTo(splashUnit, player[i]))
            {
                float tempDist = Distance(GetObjectX(player[i]), GetObjectY(player[i]), GetObjectX(splashUnit), GetObjectY(splashUnit));
                if (tempDist < range)
                    Damage(player[i], owner, amount, 0);
            }
        }
    }
}

void deathrayTouched()
{
    int plr = GetDirection(self);

    if (IsAttackedBy(other, player[plr]) && CurrentHealth(player[plr]))
    {
        Damage(other, player[plr], 100, 16);
        Enchant(other, "ENCHANT_FREEZE", 2.0);
        Enchant(other, "ENCHANT_CHARMING", 0.4);
    }
}

void OgreCollisionEvent2()
{
    int plr = GetDirection(self);

    if (CurrentHealth(other) && IsAttackedBy(other, player[plr]) && !HasEnchant(other, "ENCHANT_ETHEREAL"))
    {
        Damage(other, player[plr], 50, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.4);
        Enchant(other, "ENCHANT_ETHEREAL", 0.4);
    }
}

void makeStar(int arg_0, float arg_1) {
    float var_0 = GetWaypointX(arg_0);
    float var_1 = GetWaypointY(arg_0);
    float var_2[10];
    int i;

    for(i = 0; i < 5; i += 1) {
        var_2[i * 2] = var_0 - MathSine(i * 72, arg_1);
        var_2[i * 2 + 1] = var_1 - MathSine(i * 72 + 90, arg_1);
    }

    Effect("SENTRY_RAY", var_2[0], var_2[1], var_2[4], var_2[5]);
    Effect("SENTRY_RAY", var_2[0], var_2[1], var_2[6], var_2[7]);
    Effect("SENTRY_RAY", var_2[2], var_2[3], var_2[6], var_2[7]);
    Effect("SENTRY_RAY", var_2[2], var_2[3], var_2[8], var_2[9]);
    Effect("SENTRY_RAY", var_2[4], var_2[5], var_2[8], var_2[9]);
}
void OgreCollisionEvent()
{
    int plr = GetDirection(self);

    if (IsAttackedBy(other, player[plr]) && CurrentHealth(other) && !HasEnchant(other, "ENCHANT_ETHEREAL"))
    {
        Damage(other, player[plr], 110, 14);
        Enchant(other, "ENCHANT_HELD", 2.0);
        Enchant(other, "ENCHANT_ETHEREAL", 0.5);
    }
}

int ogreSpawnBarrel(int mode, int wp) 
{
     //mode(0: spawn, 1: reference, 2: remove), way
    int unit;
    
    if (!mode)
    {
        unit = CreateObject("Barrel", wp);
        Frozen(unit, 1);
        return 0;
    }
    else if (mode == 1)
    {
        if (IsObjectOn(unit)) return unit;
        else return 0;
    }
    else
    {
        if (IsObjectOn(unit))
        {
            Frozen(unit, 0);
            Damage(unit, 0, 255, 14);
        }
        return 0;
    }
}

string OgreTables(int index)
{
    string table[] = {"Barrellotd", "PiledBarrels1", "Crate1", "Crate2", "Coffin2"};

    return table[index];
}

void catchOgrePlayer()
{
    int mePlr = GetDirection(ogreTrg());
    int count;
    int enemy, trash;
    int progress;

    if (CurrentHealth(player[mePlr]) && count < 48)
    {
        if (!CurrentHealth(enemy) && !IsCaller(player[mePlr]))
        {
            ogreSpawnBarrel(2, 0);
            enemy = GetCaller();
            UniPrintToAll("트랩에 걸렸습니다 .");
            // Effect("CYAN_SPARKS", GetObjectX(var_2), GetObjectY(var_2), 0.0, 0.0);
            Damage(enemy, player[mePlr], 20, 14);
            Enchant(enemy, "ENCHANT_CONFUSED", 5.0);
            DrawGeometryRingAt("DrainManaOrb", GetObjectX(enemy), GetObjectY(enemy), 0);
            ObjectOff(ogreTrg());
            progress = 1;
        }
        if (progress == 1)
        {
            if (CurrentHealth(enemy))
            {
                if (IsObjectOn(trash))
                {
                    DrawGeometryRingAt("DrainManaOrb", GetObjectX(trash), GetObjectY(trash), 0);
                    Damage(trash, 0, 255, 14);
                    Damage(enemy, player[mePlr], 10, 2);
                }
                else
                {
                    trash = CreateObjectAt(OgreTables(Random(0, 4)), GetObjectX(enemy), GetObjectY(enemy));
                    UnitNoCollide(trash);
                    Raise(trash, 200.0);
                }
                count += 1;
                FrameTimer(11, catchOgrePlayer);
            }
            else {
                count = 60;
                FrameTimer(1, catchOgrePlayer);
            }
        }
    }
    else
    {
        if (IsObjectOn(trash))
            Delete(trash);
        count = 0;
        enemy = 0;
        progress = 0;
    }
}
int ogreTrg()
{
    int sw;

    if (!sw)
        sw = Object("catcherOgre");

    return sw;
}

void LichStatueCollide()
{
    int owner = GetOwner(self);

    while (GetTrigger())
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 70, 9);
            Enchant(other, "ENCHANT_CHARMING", 0.3);
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        Delete(self);
        break;
    }
}

int LichStatue(float xProfile, float yProfile, int index)
{
    int statue = CreateObjectAt(SpiritStatues((index % 16) / 2), xProfile, yProfile);

    SetUnitCallbackOnCollide(statue, LichStatueCollide);
    return statue;
}

void flagMode(int plr, int mode)
{
    //owner_index, mode(0: spawn, 1: control, 2: remove)
    int unit[18];
    int i;

    for (i = 0 ; i < 18 ; i += 1)
    {
        if (!mode)
        {
            unit[i] = LichStatue(GetWaypointX(98) + MathSine(i * 20 + 90, 200.0), GetWaypointY(98) + MathSine(i * 20, 200.0), i);
            SetOwner(player[plr], unit[i]);
            Effect("SMOKE_BLAST", GetWaypointX(98), GetWaypointY(98), 0.0, 0.0);
        }
        else if (IsObjectOn(unit[i]))
        {
            if (mode == 1)
            {
                PushObject(unit[i], -2.0, GetWaypointX(98), GetWaypointY(98));
            }
            else if (mode == 2)
                Delete(unit[i]);
        }
    }
}

string SpiritStatues(int index)
{
    string table[] = {
        "MovableStatue2f", "MovableStatue2e", "MovableStatue2d", "MovableStatue2c", "MovableStatue2b", "MovableStatue2a", "MovableStatue2h", "MovableStatue2g"
        };

    return table[index];
}

void lichTouched()
{
    int plr = GetDirection(GetTrigger() + 1);

    if (CurrentHealth(other) && IsAttackedBy(other, player[plr]))
    {
        Damage(other, player[plr], 70, 14);
        Enchant(other, "ENCHANT_FREEZE", 1.0);
        Delete(self);
        Delete(GetTrigger() + 1);
        Effect("SPARK_EXPLOSION", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
    }
}

float lightningXY(int num)
{
    float pos_xy[3];

    if (num == -1) {
        pos_xy[0] = 0.0;
        pos_xy[1] = 35.0;
        pos_xy[2] = -35.0;
        return pos_xy[0];
    }
    return pos_xy[num];
}
int stoneGolemInv()
{
    int sd;

    if (!IsObjectOn(sd))
        sd = CreateObject("sword", 2);
    return sd;
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

int CheckUnitOwner(int unit)
{
    int i;

    for (i = 9 ; i >= 0 ; i --)
    {
        if (IsOwnedBy(unit, player[i]))
            return i;
    }
    return -1;
}

void DummyFunction()
{
    //
}

void setUnitHealth(int arg_0, int arg_1) {
    Damage(arg_0, 0, CurrentHealth(arg_0) - 1, -1);
    Pickup(arg_0, CreateObject("rottenMeat", 2));
    Damage(arg_0, 0, CurrentHealth(arg_0) - arg_1, -1);
}

float Rx_ratio(float x, float y, int angle)
{
    return (MathSine(angle + 90, 1.0) * x) - (MathSine(angle, 1.0) * y);
}

float Ry_ratio(float x, float y, int angle)
{
    return (MathSine(angle, 1.0) * x) + (MathSine(angle + 90, 1.0) * y);
}

int SentryRay()
{
    int ray;

    if (!ray)
    {
        CreateObject("SentryGlobeMovable", 108);
        ray = GetMemory(0x750710);
        ObjectOff(GetMemory(ray + 0x2c));
    }
    return ray;
}

void SetRayRadian(int owner, int ray_ptr)
{
    SetMemory(GetMemory(ray_ptr + 0x2ec), ToInt(MATH_PI / 180.0 * IntToFloat(GetDirection(owner)) * 45.0 / 32.0));
    ObjectOn(GetMemory(ray_ptr + 0x2c));
}

void DisableRay(int ray)
{
    ObjectOff(ray);
}

void SelectRandomUnit()
{
    int arr[10], k, idx = 0;

    for (k = 0 ; k < 10 ; k ++)
    {
        if (!MaxHealth(player[k]))
        {
            arr[idx] = k;
            idx ++;
        }
    }
    if (idx)
        PlayerTeleportToRandBeacon(other, Random(0, idx - 1));
    else
    {
        MoveObject(other, GetWaypointX(3), GetWaypointY(3));
        UniPrint(other, "에러_ 더 이상 선택가능한 캐릭터가 없습니다");
    }
}

void PlayerTeleportToRandBeacon(int unit, int sel)
{
    float x = GetWaypointX(4 + sel), y = GetWaypointY(4 + sel);

    GreenLightningFx(FloatToInt(GetObjectX(unit)), FloatToInt(GetObjectY(unit)), FloatToInt(x), FloatToInt(y), 12);
    MoveObject(unit, x, y);
}

void InitRandSelector(float x, float y)
{
    int unit = CreateObjectAt("NPC", x, y);

    UnitNoCollide(CreateObjectAt("TeleportWake", x, y));
    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    SetCallback(unit, 9, SelectRandomUnit);
    MoveWaypoint(76, x - 40.0, y - 40.0);
    StrRandomMark();
}

void StrRandomMark()
{
	int arr[3];
	string name = "SpinningCrown";
	int i = 0;
	arr[0] = 67670300; arr[1] = 16514; arr[2] = 2064; 
	while(i < 3)
	{
		drawStrRandomMark(arr[i], name);
		i ++;
	}
}

void drawStrRandomMark(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(76);
		pos_y = GetWaypointY(76);
	}
	for (i = 1 ; i > 0 && count < 93 ; i <<= 1)
	{
		if (i & arg_0)
			UnitNoCollide(CreateObject(name, 76));
		if (count % 7 == 6)
			MoveWaypoint(76, GetWaypointX(76) - 54.000000, GetWaypointY(76) + 9.000000);
		else
			MoveWaypoint(76, GetWaypointX(76) + 9.000000, GetWaypointY(76));
		count ++;
	}
	if (count >= 93)
	{
		count = 0;
		MoveWaypoint(76, pos_x, pos_y);
	}
}

void NotifyCurrentCooldown(int plr, int ctime)
{
    if (CurrentHealth(player[plr]))
    {
        UniPrint(player[plr], "사용하시려는 스킬의 남은 쿨다운: " + IntToString(ctime));
    }
}

void LinkFireSpriteBin(int unit)
{
    int ptr = UnitToPtr(unit);

    if (ptr)
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FireSpriteBinTable());
}

void RectangleImageKeyJ()
{ }

void RectangleImageKeyK()
{ }

void RectangleImageKeyL()
{ }

void RectangleImageSpacebar()
{ }

void BitaPowerDrinkImage()
{ }

void BuldakImage()
{ }

void PlayerClassCommonWhenEntry()
{
    int drawF = ImageResourceDrawFunctionFix(0x7510c4);

    ApplyImageResourceFix(579, GetScrCodeField(RectangleImageKeyJ) + 4, drawF);
    ApplyImageResourceFix(580, GetScrCodeField(RectangleImageKeyK) + 4, drawF);
    ApplyImageResourceFix(581, GetScrCodeField(RectangleImageKeyL) + 4, drawF);
    ApplyImageResourceFix(582, GetScrCodeField(RectangleImageSpacebar) + 4, drawF);
    ApplyImageResourceFix(2178, GetScrCodeField(BitaPowerDrinkImage) + 4, drawF);
    ApplyImageResourceFix(842, GetScrCodeField(BuldakImage) + 4, drawF);

    CmdLine("say 게임 참가완료", false);
    // if (!GetMemory(0x654284))   //!RemoveMe
    //     ClientTestPacket();     //!RemoveMe
}

static void NetworkUtilClientMain()
{
    PlayerClassCommonWhenEntry();
}

void NetServerMapped(int pIndex, int plrUnit)   //@brief. 서버 측 처리를 여기서 구성합니다
{
    PlayerClassCommonWhenEntry();
}

void NetClientMapped(int pIndex, int plrUnit)   //@brief. 여기에서 클라이언트 메인을 구성합니다
{
    UniPrintToAll("클라이언트 진입 내부id:" + IntToString(pIndex));
}

int PlayerEntryCondition(int plrUnit)
{
    if (HasEnchant(plrUnit, "ENCHANT_INVULNERABLE"))
    {
        return ValidPlayerCheck(plrUnit);
    }
    return 0;
}

void NetClientRegist(int pIndex, int plrPtr)
{
    int plrUnit = GetMemory(plrPtr + 0x2c);

    if (PlayerEntryCondition(plrUnit))
    {
        UniPrint(GetHost(), IntToString(pIndex) + " 진입" + IntToString(HasEnchant(plrUnit, "ENCHANT_INVULNERABLE")));
        Enchant(plrUnit, "ENCHANT_BLINDED", 0.0);
        UniPrint(plrUnit, "계속하려면 클릭하세요");
    }
    else if (HasEnchant(plrUnit, "ENCHANT_BLINDED"))
    {
        if (CheckPlayerInput(plrUnit) == 6)
        {
            if (plrUnit == GetHost())
                NetServerMapped(pIndex, plrUnit);
            else
                NetClientMapped(pIndex, plrUnit);
            EnchantOff(plrUnit, "ENCHANT_BLINDED");
            MoveObject(plrUnit, GetWaypointX(110), GetWaypointY(110));
        }
    }
}

void NetClientLoop()
{
    int u;

    for (u = 0 ; u < 32 ; u += 1)
    {
        if (GetMemory(0x62f9e0 + (u * 0x12dc)))
            NetClientRegist(u, GetMemory(0x62f9e0 + (u * 0x12dc)));
    }
    FrameTimer(1, NetClientLoop);
}

void DrawGeometryRingAt(string orbName, float xProfile, float yProfile, int angleAdder)
{
    int u;
    float speed = 2.3;

    for ( u = 0 ; u < 60 ; u += 1)
        LinearOrbMove(CreateObjectAt(orbName, xProfile, yProfile), MathSine((u * 6) + 90 + angleAdder, -12.0), MathSine((u * 6) + angleAdder, -12.0), speed, 10);
}

void DrawSpreadOrbFx(string orbName, float xProfile, float yProfile, int angleAdder)
{
    int u;
    float speed = 2.4;

    for (u = 0 ; u < 60 ; u += 1)
        LinearOrbMove(CreateObjectAt(orbName, xProfile, yProfile), MathSine((u * 6) + 90 + angleAdder, -24.0), MathSine((u * 6) + angleAdder, -24.0), speed, 12);
}

void DemonsBloodFestival(int baseUnit)
{
    int count = GetDirection(baseUnit);
    string orbName[] = {"CharmOrb", "HealOrb", "WhiteOrb", "DrainManaOrb"};

    while (true)
    {
        if (count)
        {
            LookWithAngle(baseUnit, count - 1);
            DrawGeometryRingAt("HealOrb", GetObjectX(baseUnit), GetObjectY(baseUnit), count);
            DrawSpreadOrbFx(orbName[count % 4] , GetObjectX(baseUnit), GetObjectY(baseUnit), count);
            FrameTimerWithArg(1, baseUnit, DemonsBloodFestival);
            break;
        }
        Delete(baseUnit);
        break;
    }
}

void ArmageddonExplosion()
{
    object owner = GetOwner(self);
    object splashUnit = CreateObjectAt("AmbBeachBirds", GetObjectX(self), GetObjectY(self));

    DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(splashUnit), GetObjectY(splashUnit)), 11);
    SetOwner(owner, splashUnit);
    DrawGeometryRingAt("HealOrb", GetObjectX(self), GetObjectY(self), 0);
    AroundSplashTriggered(self, 140.0, 100);
    PlaySoundAround(splashUnit, 918);
    DeleteObjectTimer(splashUnit, 3);
}

void ArmageddonCollide()
{
    int owner = GetOwner(self);

    while (GetTrigger())
    {
        if (CurrentHealth(other) && IsAttackedBy(other, owner))
        {
            Damage(other, owner, 10, 0);
        }
        else if (!GetCaller())
            DestroyWallAtObjectPos(self);
        else
            break;
        ArmageddonExplosion();
        Delete(self);
        break;
    }
}

int DemonsArmageddon(float xProfile, float yProfile, int owner)
{
    int mis = CreateObjectAt("TitanFireball", xProfile, yProfile);
    
    SetUnitCallbackOnCollide(mis, ArmageddonCollide);
    SetOwner(owner, mis);
    return mis;
}

int FxBurnningZombie(float xProfile, float yProfile)
{
    int fxU = CreateObjectAt("Zombie", xProfile, yProfile);

    UnitNoCollide(fxU);
    ObjectOff(fxU);
    Damage(fxU, 0, MaxHealth(fxU) + 1, 1);
    return fxU;
}


void DemonBarrelFlying(int barr)
{
    int owner = GetOwner(barr);

    while (CurrentHealth(barr))
    {
        if (CurrentHealth(owner))
        {
            int enemy = ToInt(GetObjectZ(barr + 1)), durate = GetDirection(barr);

            if (CurrentHealth(enemy) && durate)
            {
                if (Distance(GetObjectX(barr), GetObjectY(barr), GetObjectX(enemy), GetObjectY(enemy)) >= 30.0)
                {
                    LookWithAngle(barr, durate - 1);
                    DeleteObjectTimer(FxBurnningZombie(GetObjectX(barr), GetObjectY(barr)), 18);
                    MoveObject(barr, GetObjectX(barr) + UnitRatioX(enemy, barr, 14.0), GetObjectY(barr) + UnitRatioY(enemy, barr, 14.0));
                    FrameTimerWithArg(1, barr, DemonBarrelFlying);
                    break;
                }
                Damage(enemy, owner, 100, 14);
                DrawGeometryRingAt("HealOrb", GetObjectX(enemy), GetObjectY(enemy), 0);
            }
        }
        Delete(barr);
        Delete(barr + 1);
        break;
    }
}

void DemonWatcherSight()
{
    if (GetOwner(self) == 0)
    {
        UniPrintToAll("오류입니다");
        return;
    }

    if (CurrentHealth(other))
    {
        int barr = CreateObjectAt("TargetBarrel2", GetObjectX(self) + UnitRatioX(self, other, 7.0), GetObjectY(self) + UnitRatioY(self, other, 7.0));

        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(barr), GetObjectY(barr)), GetCaller());
        SetOwner(GetOwner(self), barr);
        SetUnitMaxHealth(barr, 120);
        LookWithAngle(barr, 90);
        FrameTimerWithArg(1, barr, DemonBarrelFlying);
    }
}

void DebugModeSetting()
{
    if (IsCaller(GetHost()))
    {
        DEBUGMODE ^= true;
        if (DEBUGMODE)
        {
            UniPrint(other, "디버그 모드 활성화");
            CmdLine("say 디버깅", false);
        }
    }
}
