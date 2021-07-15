
#include "noxscript\builtins.h"
#include "libs\callmethod.h"
#include "libs\typecast.h"
#include "libs\unitutil.h"
#include "libs\unitstruct.h"
#include "libs\printutil.h"
#include "libs\mathlab.h"
#include "libs\waypoint.h"

#include "libs\spellutil.h"
#include "libs\fxeffect.h"
#include "libs\buff.h"
#include "libs\reaction.h"
#include "libs\username.h"

#include "libs\network.h"
#include "libs\playerupdate.h"

#define DEFAULT_TRADE_MSG       "계속하려면 클릭하세요"

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

int LIFE = 10;
int START_GAME = 0;
int STAGE_LV = 0;
int MAX_MONSTERS = 200;
int PLR_LV[10];
int PLR_WEAPON[10];
int PLR_MARK[10];
int player[30];
int MAX_STAGE = 14;

#define TRUE        1
#define FALSE       0

#define OTHER   -1
#define SELF    -2




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
		arr[0] = 1635412311; arr[1] = 1699898482; arr[2] = 100; arr[16] = 80000; arr[17] = 300; 
		arr[18] = 100; arr[19] = 50; arr[21] = 1065353216; arr[23] = 32; arr[24] = 1067869798; 
		arr[26] = 4; arr[27] = 4; arr[53] = 1128792064; arr[54] = 4; 
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

int FireSpriteBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1701996870; arr[1] = 1769107539; arr[2] = 25972; arr[17] = 135; arr[19] = 65; 
		arr[21] = 1065353216; arr[23] = 65536; arr[24] = 1067450368; arr[37] = 1769236816; arr[38] = 1181513062; 
		arr[39] = 1650815593; arr[40] = 7105633; arr[53] = 1133903872; arr[55] = 13; arr[56] = 21; 
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
		arr[18] = 100; arr[19] = 60; arr[21] = 1065353216; arr[23] = 32768; arr[24] = 1070386381; 
		arr[26] = 4; arr[28] = 1077936128; arr[29] = 150; arr[31] = 8; arr[32] = 12; 
		arr[33] = 20; arr[57] = 5547984; arr[58] = 5546320; arr[59] = 5542432; 
		link = &arr;
	}
	return link;
}

int WoundedApprenticeBinTable()
{
	int arr[62], link;
	if (!link)
	{
		arr[0] = 1853189975; arr[1] = 1097098596; arr[2] = 1701998704; arr[3] = 1667855470; arr[4] = 101; 
		arr[17] = 50; arr[18] = 10; arr[19] = 100; arr[21] = 1065353216; arr[24] = 1065353216; 
		arr[28] = 1112014848; arr[29] = 40; arr[30] = 1106247680; arr[31] = 2; arr[32] = 12; 
		arr[33] = 20; arr[57] = 5548112; arr[59] = 5542784; 
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
        arr[12] = MonsterFireSpriteProcess; arr[73] = MonsterWizardRedProcess; arr[29] = MonsterAirshipCaptainProcess; arr[39] = MonsterWoundedApprentice;
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
        SetUnitMaxHealth(unit, 130);
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
        SetMemory(GetMemory(ptr + 0x2ec) + 0x54c, 0); //Flee Range set to 0
        //SetMemory(GetMemory(ptr + 0x2ec) + 0x1e8, VoiceList(34));       //HorvathVoice
        SetUnitMaxHealth(unit, 150);
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

void MonsterFireSpriteProcess(int unit)
{
    //TODO: Index. 6, ThingName= FireSprite
    int ptr = UnitToPtr(unit);

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, FireSpriteBinTable());
        SetUnitMaxHealth(unit, 135);
        SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000);
    }
}

void MonsterWizardRedProcess(int unit)
{
    //TODO: Index. 7, ThingName= WizardRed
    int ptr = UnitToPtr(unit); //, uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WizardRedBinTable());
        SetUnitMaxHealth(unit, 225);
    }
}

void MonsterAirshipCaptainProcess(int unit)
{
    //TODO: Index. 8, ThingName= AirshipCaptain
    int ptr = UnitToPtr(unit), uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, AirshipCaptainBinTable());
        SetUnitMaxHealth(unit, 250);
        RetreatLevel(unit, 0.0);
        ResumeLevel(unit, 1.0);
    }
}

void MonsterWoundedApprentice(int unit)
{
    //TODO: Index. 9, ThingName= WoundedApprentice
    int ptr = UnitToPtr(unit), uec;

    if (ptr)
    {
        SetMemory(GetMemory(ptr + 0x2ec) + 0x1e4, WoundedApprenticeBinTable());
        SetUnitMaxHealth(unit, 230);
        RetreatLevel(unit, 0.0);
        ResumeLevel(unit, 1.0);
        SetUnitStatus(unit, GetUnitStatus(unit) ^ 0x10000);
    }
}

int UserDamageArrowCreate(int owner, float x, float y, int dam, int thingID)
{
    int unit = CreateObjectAt("MercArcherArrow", x, y);
    int ptr = GetMemory(0x750710);

    SetOwner(owner, unit);
    SetMemory(ptr + 0x04, thingID);
    SetMemory(GetMemory(ptr + 0x2bc) + 4, dam);
    return unit;
}

void MapExit()
{
    MusicEvent();
    ResetPlayerHandlerWhenExitMap();
}

void MapInitialize()
{
    MusicEvent();
    
    GetMaster();
    
    PutMapDecorations();
    FrameTimer(1, DelayRun);
    FrameTimer(10, LoopPreservePlayers);
    FrameTimer(20, LoopSearchIndex);
    FrameTimer(90, PutMapMaker);
    FrameTimerWithArg(240, 25, SpawnStartButton);
    FrameTimer(245, StrStartButton);
    FuncPtr();
}

void DelayRun()
{
    InitShopPtr();
    MoveWaypoint(7, 2680.0, 2834.0);
    StrShopHere();
    FrameTimer(1, PutShopItems);
}

void DetectedSpecficIndex(int curId)
{
    if (IsMissileUnit(curId))
    {
        int thingID = GetUnitThingID(curId);

        if (thingID == 526)
            HarpoonEvent(curId);
        else if (thingID == 1179)
            ShurikenEvent(curId);
    }
}

void LoopSearchIndex()
{
    int curId, tempId;

    if (GetMemory(0x750710))
    {
        tempId = GetMemory(GetMemory(0x750710) + 0x2c);
        if (curId)
        {
            while (curId < tempId)
            {
                curId += 1;
                DetectedSpecficIndex(curId);
            }
        }
        else
            curId = tempId;
    }
    FrameTimer(1, LoopSearchIndex);
}

void PutMapMaker()
{
    int unit = CreateObject("WoundedApprentice", 31);

    Chat(unit, "맵 제작자: 저에게 오시면 이 게임에 대해 설명해 드립니다");
    LookWithAngle(unit, 0);
    Frozen(unit, 1);
    SetDialog(unit, "NORMAL", MakerDialogPic, DummyFunction);
    FrameTimer(1, StrHelpAboutGame);
}

void MakerDialogPic()
{
    if (!GetDirection(self))
    {
        AudioEvent("MaleNPC1Retreat", 31);
        TalkMapMapker(GetTrigger());
    }
}

void TalkMapMapker(int unit)
{
    int count = GetDirection(unit);

    if (count < 10)
    {
        if (count)
        {
            if (Random(0, 1))
                AudioEvent("MaleNPC1Talkable", 31); 
            else
                AudioEvent("MaleNPC2Talkable", 31);
        }
        Chat(unit, "맵 제작자: " + CommentTable(count));
        LookWithAngle(unit, count + 1);
        FrameTimerWithArg(75, unit, TalkMapMapker);
    }
    else
    {
        LookWithAngle(unit, 0);
    }
}

string CommentTable(int num)
{
    string table = 
    "게임에 대한 설명을 시작하겠습니다";
    "제 위에 있는 시작버튼을 누르게 되면 몬스터가 우르르 몰려올 것입니다.";
    "몬스터가 사선에 두번 도달하게 되면 목숨이 하나씩 깎입니다, 이미 사선에 도달했었던 몬스터는 흡혈 엔첸이 걸려있습니다.";
    "즉, 흡혈 엔첸이 걸려있는 몬스터가 사선에 도달하면 목숨이 감소됩니다";
    "상점에서 탄창을 구입하셔야 사격을 하실 수 있습니다, 탄창 말고도 다양한 물품을 판매하고 있으니 많은 이용바랍니다";
    "조심스럽게 걷기 사용을 통해 상점으로 더 빨리 이동하고 제자리로 돌아올 수 있습니다";
    "특수한 기술은 작살을 시전함으로서 발동됩니다, 탄창을 업그레이드 하면 발사되는 탄이 강해집니다.";
    "스테이지는 최대 14 까지 존재하며 첫판은 연습게임 입니다.";
    "돈을 빠르게 모으는 팁!- 상점에 있는 골드 도박 시스템을 이용해 보세요, 이것은 여러분의 운을 시험합니다";
    "이제 모든 설명이 끝났습니다, 게임을 시작하십시오";
    "맵 제작: Happy Soft ltd.";

    return ToStr(SToInt(table) + num);
}

void SpawnStartButton(int wp)
{
    int unit = CreateObject("Fear", wp);
    CreateObject("InvisibleLightBlueHigh", wp);

    Chat(unit, "시작버튼 이에욤..('x') ");
    LookWithAngle(unit + 1, wp);
    Frozen(unit, 1);
    RegistItemPickupCallback(unit, PressStartButton);
}

void PressStartButton()
{
    FrameTimerWithArg(1, GetDirection(GetTrigger() + 1), SpawnStartButton);
    Delete(self);
    Delete(GetTrigger() + 1);
    if (!START_GAME)
    {
        START_GAME = 1;
        if (STAGE_LV < MAX_STAGE)
        {
            UniPrint(OTHER, "시작버튼을 누르셨습니다.");
            if (!STAGE_LV)
                UniPrintToAll("스테이지 1 은 연습게임 입니다, 유닛이 사선을 넘어와도 라이프가 소모되지 않아요");
            else
                UniPrintToAll("시작버튼을 눌렀습니다, 스테이지" + IntToString(STAGE_LV + 1) + " 가 시작됩니다.");
            FrameTimerWithArg(120, MAX_MONSTERS, ControlSpawnMonsters);
        }
        else
        {
            UniPrintToAll("축하합니다-! 스테이지를 모두 클리어 하셨습니다-!");
            FrameTimer(1, StrAllClear);
            Effect("WHITE_FLASH", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
            MoveWaypoint(4, GetObjectX(other), GetObjectY(other));
            AudioEvent("StaffOblivionAchieve1", 4);
        }
    }
}

void ControlSpawnMonsters(int max)
{
    int count;

    if (count < max)
    {
        rhombusPut(24, 1027.0, 1666.0, 1091.0, 1137.0);
        SpawnMonster(24, STAGE_LV);
        count ++;
        FrameTimerWithArg(42, max, ControlSpawnMonsters);
    }
    else
        count = 0;
}

void SpawnMonster(int wp, int stage)
{
    int pic = Random(0, 2), unit = CreateObject(MonsterName(stage + pic), wp);
    int ptr = GetMemory(0x750710);

    CheckMonsterThing(unit);
    CreateObject("InvisibleLightBlueHigh", wp);
    Enchant(unit, "ENCHANT_BLINDED", 0.0);
    AggressionLevel(unit, 0.0);
    MonsterSpecialFlag(unit, MonsterFlags(pic));
    SetUnitHealth(unit, 1000 + MonsterHP(stage + pic));
    SetOwner(GetMaster(), unit);
    SetCallback(unit, 7, MonsterHurt);
    SetCallback(unit, 5, SetDeaths);
    FrameTimerWithArg(1, unit, DelayWalkToDown);
}

void MonsterTeleportToStart()
{
    if (CurrentHealth(other) && IsOwnedBy(other, GetMaster()))
    {
        if (!HasEnchant(other, "ENCHANT_VAMPIRISM"))
        {
            rhombusPut(24, 1027.0, 1666.0, 1091.0, 1137.0);
            MoveObject(other, GetWaypointX(24), GetWaypointY(24));
            Enchant(other, "ENCHANT_VAMPIRISM", 0.0);
            Enchant(other, "ENCHANT_PROTECT_FROM_ELECTRICITY", 5.0);
            FrameTimerWithArg(3, GetCaller(), DelayWalkToDown);
        }
        else if (LIFE && !HasEnchant(other, "ENCHANT_PROTECT_FROM_ELECTRICITY"))
        {
            if (!STAGE_LV)
            {
                PrintToAll("보호모드가 작동되고 있습니다, 1 단계에서는 목숨이 소모되지 않습니다.");
            }
            else
            {
                LIFE --;
                PrintToAll("적군이 선을 넘었습니다, 라이프 1개가 소모됩니다. 남은 라이프: " + IntToString(LIFE));
            }
            Damage(other, 0, CurrentHealth(other) + 1, 14);
            if (!LIFE)
            {
                MoveObject(Object("StartLocation"), GetWaypointX(27), GetWaypointY(27));
                TeleportAllPlayers(27);
                FrameTimer(60, EndMissionGameOver());
            }
        }
    }
}

void TeleportAllPlayers(int wp)
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (CurrentHealth(player[k]))
        {
            MoveObject(player[k], GetWaypointX(wp), GetWaypointY(wp));
            player[k] = 0;
        }
    }
}

void EndMissionGameOver()
{
    PrintToAll("미션실패-! 너무 많은 적군들이 사선으로 넘어왔습니다");
    FrameTimer(10, StrGGOver);
    AudioEvent("HorrendousIsKilled", 27);
}

void DelayWalkToDown(int unit)
{
    Walk(unit, GetObjectX(unit) + 800.0, GetObjectY(unit) + 800.0);
}

void MonsterSpecialFlag(int unit, int flag)
{
    int k;

    for (k = 0x10 ; k ; k >>= 1)
    {
        if (flag & k)
        {
            if (k == 0x1)
                Enchant(unit, "ENCHANT_SLOWED", 0.0);
            else if (k == 0x2)
                SetUnitSpeed(unit, 1.2);
            else if (k == 0x4)
                LookWithAngle(unit + 1, 1);
            else if (k == 0x8)
                SetUnitSubclass(unit, GetUnitSubclass(unit) ^ 0x20000);
            else if (k == 0x10)
                LookWithAngle(unit + 1, 2);
        }
    }
}

void MonsterHurt()
{
    int plr, addon;

    if (CurrentHealth(self) <= 1000)
    {
        plr = CheckOwner(other);
        if (plr >= 0)
        {
            addon = STAGE_LV * 2;
            ChangeGold(player[plr], Random(20 + addon, 110 + addon));
        }
        Damage(self, other, CurrentHealth(self) + 1, 14);
    }
}

void SetDeaths()
{
    int count, ptr = GetTrigger() + 1, flags = GetDirection(ptr);

    if (flags)
    {
        if (flags == 1)
            DeleteObjectTimer(self, 1);
        else
        {
            MoveWaypoint(4, GetObjectX(self), GetObjectY(self));
            DeleteObjectTimer(CreateObject("Flame", 4), 6);
        }
    }
    else
        DeleteObjectTimer(self, 60);
    Delete(ptr);
    count ++;
    if (count >= MAX_MONSTERS)
    {
        PrintToAll("이번 스테이지를 클리어 하셨습니다");
        AudioEvent("GameOver", 1);
        START_GAME = 0;
        STAGE_LV ++;
        count = 0;
        GoldGiveToAllPlayers(200);
        if (STAGE_LV == 1)
        {
            PrintToAll("지금, 샵에 새로운 품목이 공급되었습니다.");
            FrameTimer(1, Stage2ShopItemCreated);
        }
    }
}

void GoldGiveToAllPlayers(int amount)
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (CurrentHealth(player[k]))
            ChangeGold(player[k], amount);
    }
}

void PutMapDecorations()
{
    int k;

    for (k = 0 ; k < 7 ; k ++)
    {
        Frozen(CreateObject("DunMirScaleTorch1", 2), 1);
        MoveWaypoint(2, GetWaypointX(2) + 90.0, GetWaypointY(2) - 90.0);
    }
    Enchant(CreateObject("MovableStatueVictory3NE", 6), "ENCHANT_FREEZE", 0.0);
    Enchant(CreateObject("MovableStatueVictory3NW", 7), "ENCHANT_FREEZE", 0.0);
    Enchant(CreateObject("MovableStatueVictory3SW", 8), "ENCHANT_FREEZE", 0.0);
    Enchant(CreateObject("MovableStatueVictory3SE", 9), "ENCHANT_FREEZE", 0.0);
}

int PlayerClassOnInit(int plr, int pUnit)
{
    player[plr] = pUnit;
    player[plr + 10] = 1;

    ChangeGold(pUnit, -GetGold(pUnit));
    SelfDamageClassEntry(pUnit);

    UniPrintToAll(PlayerIngameNick(pUnit) + " 님께서 지도에 입장했습니다");
    return plr;
}

void PlayerRegist()
{
    int plr, k;

    while (1)
    {
        if (CurrentHealth(other))
        {
            if (MaxHealth(other) == 150)
            {
                plr = CheckPlayer();
                for (k = 9 ; plr < 0 && k >= 0 ; k --)
                {
                    if (!MaxHealth(player[k]))
                    {
                        plr = PlayerClassOnInit(k, GetCaller());
                        break;
                    }
                }
                if (plr >= 0)
                {
                    PlayerHasJoin(plr);
                    break;
                }
            }
        }
        PlayerCantJoin();
        break;
    }
}

void PlayerHasJoin(int plr)
{
    if (IsObjectOn(PLR_MARK[plr]))
        Delete(PLR_MARK[plr]);
    if (PlayerClassCheckDeathFlag(plr))
        PlayerClassSetDeathFlag(plr);
    InventoryEmpty(other);
    MoveObject(player[plr], GetWaypointX(1), GetWaypointY(1));
    ChangeGold(player[plr], -GetGold(player[plr]));
    ChangeGold(player[plr], 1000);
    DeleteObjectTimer(CreateObject("OblivionUp", 1), 10);
    PLR_LV[plr] = 0;
    PLR_WEAPON[plr] = 0;
    UniPrint(OTHER, "사선에 입장했습니다, 1000 골드가 지급되었습니다.");
}

void PlayerCantJoin()
{
    Enchant(other, "ENCHANT_FREEZE", 0.0);
    Enchant(other, "ENCHANT_ANTI_MAGIC", 0.0);
    UniPrintToAll("맵에 문제가 있거나 이 맵은 전사만 입장가능 합니다.");
    MoveObject(other, GetWaypointX(3), GetWaypointY(3));
}

int PlayerClassCheckDeathFlag(int plr)
{
    return player[plr + 10] & 2;
}

void PlayerClassSetDeathFlag(int plr)
{
    player[plr + 10] ^= 2;
}

void PlayerClassOnAlive(int plr, int pUnit)
{
    if (CheckPlayerInput(pUnit) == 6)
        PlayerShotEvent(plr, pUnit);
    else if (UnitCheckEnchant(pUnit, GetLShift(ENCHANT_SNEAK)))
    {
        RemoveTreadLightly(pUnit);
        PlayerTeleport(pUnit);
    }
}

void PlayerClassOnExit(int plr)
{
    player[plr] = 0;
    player[plr + 10] = 0;
}

void PlayerClassOnDeath(int plr)
{
    UniPrintToAll(PlayerIngameNick(player[plr]) + " 님께서 적에게 격추되었습니다");
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

void PlayerTeleport(int plr)
{
    EnchantOff(player[plr], "ENCHANT_SNEAK");
    Enchant(player[plr], "ENCHANT_RUN", 0.08);
    if (!HasEnchant(player[plr], "ENCHANT_ANTI_MAGIC"))
    {
        Enchant(player[plr], "ENCHANT_ANTI_MAGIC", 0.0);
        MoveWaypoint(4, GetObjectX(player[plr]), GetObjectY(player[plr]));
        if (IsObjectOn(PLR_MARK[plr]))
            MoveObject(PLR_MARK[plr], GetWaypointX(4), GetWaypointY(4));
        else
            PLR_MARK[plr] = CreateObject("TeleportGlyph4", 4);
        SetOwner(player[plr], PLR_MARK[plr]);
        MoveObject(player[plr], GetWaypointX(34), GetWaypointY(34));
        Effect("TELEPORT", GetWaypointX(34), GetWaypointY(34), 0.0, 0.0);
        AudioEvent("BlindOff", 34);
        UniPrint(player[plr], "상점으로 이동했습니다, 기존 위치로 돌아가려면 한번 더 조심스럽게 걷기를 시전하십시오");
    }
    else
    {
        EnchantOff(player[plr], "ENCHANT_ANTI_MAGIC");
        if (IsObjectOn(PLR_MARK[plr]))
        {
            MoveObject(player[plr], GetObjectX(PLR_MARK[plr]), GetObjectY(PLR_MARK[plr]));
            Effect("TELEPORT", GetObjectX(player[plr]), GetObjectY(player[plr]), 0.0, 0.0);
            Delete(PLR_MARK[plr]);
        }
    }
}

int FuncPtr()
{
    int unit;

    if (!unit)
        unit = CreateObject("InvisibleLightBlueHigh", 35);
    return unit;
}

int PlayerShotActions(int index)
{
    int actions[] = {NormalBullet, StoneBullet, HardBullet, GreenballBullet, ShiningBullet, WispBullet, FireballBullet, ArrowBullet };

    return actions[index];
}

void PlayerShotEvent(int plr, int pUnit)
{
    if (CheckHasBullet(pUnit))
        FrameTimerWithArg(1, pUnit, PlayerShotActions(PLR_LV[plr]));
}

int CheckHasBullet(int unit)
{
    int inven = GetLastItem(unit), res = 0;

    while (IsObjectOn(inven))
    {
        if (GetDirection(inven) == 1)
        {
            Delete(inven);
            res = 1;
            break;
        }
        inven = GetPreviousItem(inven);
    }
    return res;
}

void NormalBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        int mis = CreateObjectAt("WeakArcherArrow", GetObjectX(shooter) + UnitAngleCos(shooter, 15.0), GetObjectY(shooter) + UnitAngleSin(shooter, 15.0));

        LookWithAngle(mis, GetDirection(shooter));
        SetOwner(shooter, mis);
        PushObject(mis, -40.0, GetObjectX(shooter), GetObjectY(shooter));
        PlaySoundAround(mis, 886);
    }
}

void StoneBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        int mis = CreateObjectAt("ThrowingStone", GetObjectX(shooter) + UnitAngleCos(shooter, 17.0), GetObjectY(shooter) + UnitAngleSin(shooter, 17.0));
        
        SetOwner(shooter, mis);
        PushObject(mis, -40.0, GetObjectX(shooter), GetObjectY(shooter));
        PlaySoundAround(mis, 498);
    }
}

void HardBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        int mis = CreateObjectAt("WeakFireball", GetObjectX(shooter) + UnitAngleCos(shooter, 18.0), GetObjectY(shooter) + UnitAngleSin(shooter, 18.0));

        SetOwner(shooter, mis);
        PushObject(mis, -40.0, GetObjectX(shooter), GetObjectY(shooter));
        PlaySoundAround(mis, 219);
    }
}

void GreenballBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        int mis = CreateObjectAt("DeathBallFragment", GetObjectX(shooter) + UnitAngleCos(shooter, 20.0), GetObjectY(shooter) + UnitAngleSin(shooter, 20.0));

        SetOwner(shooter, mis);
        PushObject(mis, -40.0, GetObjectX(shooter), GetObjectY(shooter));
        PlaySoundAround(mis, 83);
    }
}

void ShiningBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        float pos_x = UnitAngleCos(shooter, 21.0), pos_y = UnitAngleSin(shooter, 21.0);
        int unit = CreateObjectAt("GreenFrog", GetObjectX(shooter) + pos_x, GetObjectY(shooter) + pos_y);

        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), pos_x);
        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), pos_y);
        UnitSetEnchantTime(unit, ENCHANT_RUN, 0);
        Frozen(unit, true);
        LookWithAngle(unit, GetDirection(shooter));
        LookWithAngle(unit + 1, 20);
        SetOwner(shooter, unit);
        SetCallback(unit, 9, FrogShooterTouch);
        FrameTimerWithArg(1, unit, FrogShooter);
    }
}

void WispBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        float pos_x = UnitAngleCos(shooter, 22.0), pos_y = UnitAngleSin(shooter, 22.0);
        int magic = CreateObjectAt("WillOWisp", GetObjectX(shooter) + pos_x, GetObjectY(shooter) + pos_y);

        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(magic), GetObjectY(magic)), pos_x);
        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(magic), GetObjectY(magic)), pos_y);
        UnitSetEnchantTime(magic, ENCHANT_HASTED, 0);
        Frozen(magic, true);
        SetOwner(shooter, magic);
        LookWithAngle(magic + 1, 23);
        FrameTimerWithArg(1, magic, FlyingWisp);
        SetCallback(magic, 9, WispShockShield);
    }
}

void FireballBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        int mis = CreateObjectAt("StrongFireball", GetObjectX(shooter) + UnitAngleCos(shooter, 20.0), GetObjectY(shooter) + UnitAngleSin(shooter, 20.0));

        LookWithAngle(mis, GetDirection(shooter));
        SetOwner(shooter, mis);
        PushObject(mis, -50.0, GetObjectX(shooter), GetObjectY(shooter));
        PlaySoundAround(mis, 41);
    }
}

void ArrowBullet(int shooter)
{
    if (CurrentHealth(shooter))
    {
        int mis = CreateObjectAt("ArcherArrow", GetObjectX(shooter) + UnitAngleCos(shooter, 20.0), GetObjectY(shooter) + UnitAngleSin(shooter, 20.0));

        LookWithAngle(mis, GetDirection(shooter));
        SetOwner(shooter, mis);
        PushObject(mis, -50.0, GetObjectX(shooter), GetObjectY(shooter));
        PlaySoundAround(mis, 886);
    }
}

void FrogShooter(int subUnit)
{
    while (TRUE)
    {
        if (MaxHealth(subUnit))
        {
            int owner = GetOwner(subUnit), durate = GetDirection(subUnit + 1);
            float xVect = GetObjectZ(subUnit + 1), yVect = GetObjectZ(subUnit + 2);

            if (CurrentHealth(owner) && durate)
            {
                FrameTimerWithArg(1, subUnit, FrogShooter);
                LookWithAngle(subUnit + 1, --durate);
                MoveObject(subUnit, GetObjectX(subUnit) + xVect, GetObjectY(subUnit) + yVect);
                break;
            }
            Delete(subUnit);
        }
        Delete(++subUnit);
        Delete(++subUnit);
        break;
    }
}

void FrogShooterTouch()
{
    if (!CurrentHealth(OTHER))
        return;

    int owner = GetOwner(SELF);

    if (CurrentHealth(owner) && IsAttackedBy(other, owner))
    {
        Effect("EXPLOSION", GetObjectX(other), GetObjectX(other), 0.0, 0.0);
        Damage(other, owner, 60, 14);
        Delete(self);
    }
}

void FlyingWisp(int subUnit)
{
    while (true)
    {
        if (MaxHealth(subUnit))
        {
            int owner = GetOwner(subUnit), durate = GetDirection(subUnit + 1);
            float xVect = GetObjectZ(subUnit + 1), yVect = GetObjectZ(subUnit + 2);

            if (CurrentHealth(owner) && durate)
            {
                FrameTimerWithArg(1, subUnit, FlyingWisp);
                LookWithAngle(subUnit + 1, --durate);
                MoveObject(subUnit, GetObjectX(subUnit) + xVect, GetObjectY(subUnit) + yVect);
                break;
            }
            Delete(subUnit);
        }
        Delete(++subUnit);
        Delete(++subUnit);
        break;
    }
}

void WispShockShield()
{
    if (!CurrentHealth(OTHER))
        return;
    int owner = GetOwner(self);

    if (CurrentHealth(owner) && IsAttackedBy(other, owner))
    {
        Effect("RICOCHET", GetObjectX(other), GetObjectY(other), 0.0, 0.0);
        Damage(other, owner, 90, 9);
        Delete(self);
    }
}

void ShurikenEvent(int curId)
{
    int owner = GetOwner(curId);

    if (CurrentHealth(owner))
    {
        TeleportLocation(4, GetObjectX(owner) + UnitAngleCos(owner, 23.0), GetObjectY(owner) + UnitAngleSin(owner, 23.0));
        int mis = UserDamageArrowCreate(owner, LocationX(4), LocationY(4), 45, 710);
        Enchant(CreateObjectAt("SpiderSpit", LocationX(4), LocationY(4)) - 1, "ENCHANT_FREEZE", 0.0);
        SetOwner(owner, mis + 1);
        LookWithAngle(mis, GetDirection(owner));
        LookWithAngle(mis + 1, GetDirection(owner));
        PushObject(mis, 50.0, GetObjectX(owner), GetObjectY(owner));
        PushObject(mis + 1, 50.0, GetObjectX(owner), GetObjectY(owner));
    }
    Delete(curId);
}

int GetPlayerScrIndex(int pUnit)
{
    int u;

    if (pUnit)
    {
        for (u = 0 ; u < 9 ; u += 1)
        {
            if (pUnit == player[u])
                return u;
        }
    }
    return -1;
}

void HarpoonEvent(int curId)
{
    int owner = GetOwner(curId);

    if (CurrentHealth(owner))
    {
        int scrIndex = GetPlayerScrIndex(owner);

        if (scrIndex >= 0)
            FrameTimerWithArg(1, owner, HarpoonFunctionTable(PLR_WEAPON[scrIndex]));
    }
    Delete(curId);
}

void DummyFunction()
{
    return;
}

int HarpoonFunctionTable(int index)
{
    int actions[] = {Nothing, LaserRifleBullet, StormGun, StraightPar, TripleArrowShot, FireHole, DeathrayShot, PurpleRaySpreay, 
        MonoFistShot, FistLine, SpawnBomber, CastForceOfNatures};

    return actions[index];
}

void Nothing(int plr)
{
    return;
}

int DummyUnitCreate(string name, float locX, float locY)
{
    int unit = CreateObjectAt(name, locX, locY);

    Damage(unit, 0, MaxHealth(unit) + 1, -1);
    ObjectOff(unit);
    Frozen(unit, 1);
    return unit;
}

void LaserRifleBullet(int owner)
{
    if (CurrentHealth(owner))
    {
        float pos_x = UnitAngleCos(owner, 23.0), pos_y = UnitAngleSin(owner, 23.0);
        int subUnit = DummyUnitCreate("Troll", GetObjectX(owner) + pos_x, GetObjectY(owner) + pos_y);

        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(subUnit), GetObjectY(subUnit)), pos_x);
        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(subUnit), GetObjectY(subUnit)), pos_y);
        SetOwner(owner, subUnit);
        LookWithAngle(subUnit, 30);
        SetCallback(subUnit, 9, LaserTouched);
        FrameTimerWithArg(1, subUnit, FlyingLaser);
    }
}

void StormGun(int owner)
{
    int ptr;

    if (CurrentHealth(owner))
    {
        float pos_x = UnitAngleCos(owner, 23.0), pos_y = UnitAngleSin(owner, 23.0);
        int subUnit = DummyUnitCreate("Necromancer", GetObjectX(owner) + pos_x, GetObjectY(owner) + pos_y);

        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(subUnit), GetObjectY(subUnit)), pos_x);
        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(subUnit), GetObjectY(subUnit)), pos_y);
        CreateObjectAt("WhirlWind", GetObjectX(subUnit), GetObjectY(subUnit));
        LookWithAngle(subUnit, 30);
        SetCallback(subUnit, 9, TouchedStorm);
        SetOwner(owner, subUnit);
        FrameTimerWithArg(1, subUnit, AbroadStorm);
    }
}

void StraightPar(int owner)
{
    if (CurrentHealth(owner))
    {
        float pos_x = UnitAngleCos(owner, 20.0), pos_y = UnitAngleSin(owner, 20.0);
        int subUnit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner) + pos_x, GetObjectY(owner) + pos_y);
        CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(subUnit), GetObjectY(subUnit));

        Raise(subUnit, pos_x);
        Raise(subUnit + 1, pos_y);
        LookWithAngle(subUnit, 30);
        SetOwner(owner, subUnit);
        FrameTimerWithArg(1, subUnit, StraightParLoop);
    }
}

void TripleArrowShot(int owner)
{
    float pos_x = UnitAngleCos(owner, -20.0), pos_y = UnitAngleSin(owner, -20.0);

    if (CurrentHealth(owner))
    {
        // MoveWaypoint(4, GetObjectX(player[plr]) + (1.0 / 1.6 * pos_y) - pos_x, GetObjectY(player[plr]) - (1.0 / 1.6 * pos_x) - pos_y);
        int base = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner) + (1.0 / 1.6 * pos_y) - pos_x, GetObjectY(owner) - (1.0 / 1.6 * pos_x) - pos_y), k;

        for (k = 0 ; k < 9 ; k += 1)
        {
            pos_x = UnitRatioX(owner, base, 23.0);
            pos_y = UnitRatioY(owner, base, 23.0);
            // MoveWaypoint(4, GetObjectX(player[plr]) - (1.0 / 8.0 * pos_y) - pos_x, GetObjectY(player[plr]) + (1.0 / 8.0 * pos_x) - pos_y);
            CreateObjectAt("ArcherArrow", GetObjectX(owner) - (1.0 / 8.0 * pos_y) - pos_x, GetObjectY(owner) + (1.0 / 8.0 * pos_x) - pos_y);
            LookAtObject(base + k + 1, owner);
            LookWithAngle(base + k + 1, GetDirection(base + k + 1) + 128);
            SetOwner(owner, base + k + 1);
            PushObject(base + k + 1, 40.0, GetObjectX(owner), GetObjectY(owner));
        }
        Delete(base);
    }
}

void FireHole(int owner)
{
    if (CurrentHealth(owner))
    {
        int glow = CreateObjectAt("Moonglow", GetObjectX(owner), GetObjectY(owner));

        SetOwner(owner, glow);
        FrameTimerWithArg(1, glow, SelectFireHolePosition);
    }
}

void DeathrayShot(int owner)
{
    if (CurrentHealth(owner))
    {
        int glow = CreateObjectAt("Moonglow", GetObjectX(owner), GetObjectY(owner));
        
        SetOwner(owner, glow);
        FrameTimerWithArg(1, glow, DeathrayTargetLocation);
    }
}

void PurpleRaySpreay(int owner)
{
    if (CurrentHealth(owner))
    {
        float pos_x = UnitAngleCos(owner, 20.0), pos_y = UnitAngleSin(owner, 20.0);

        int unit = CreateObjectAt("WeirdlingBeast", GetObjectX(owner) + pos_x, GetObjectY(owner) + pos_y);
        CheckMonsterThing(unit);
        DeleteObjectTimer(unit, 1);
        SetOwner(owner, unit);
        CreatureGuard(unit, GetObjectX(unit), GetObjectY(unit), GetObjectX(unit) + pos_x, GetObjectY(unit) + pos_y, 450.0);
        LookWithAngle(unit, GetDirection(owner));
        AggressionLevel(unit, 1.0);
        SetCallback(unit, 3, UnitDetectedRaySpreay);
    }
}

void MonoFistShot(int owner)
{
    if (CurrentHealth(owner))
    {
        int glow = CreateObjectAt("Moonglow", GetObjectX(owner), GetObjectY(owner));
        
        SetOwner(owner, glow);
        FrameTimerWithArg(1, glow, SingleFistHit);
    }
}

void FistLine(int owner)
{
    if (CurrentHealth(owner))
    {
        float pos_x = UnitAngleCos(owner, 40.0), pos_y = UnitAngleSin(owner, 40.0);

        int unit = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(owner) + pos_x, GetObjectY(owner) + pos_y);

        Raise(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(unit), GetObjectY(unit)), pos_y);
        Raise(unit, pos_x);
        SetOwner(owner, unit);
        FrameTimerWithArg(1, unit, FistTrailer);
    }
}

void SpawnBomber(int owner)
{
    if (CurrentHealth(owner))
    {
        int glow = CreateObjectAt("Moonglow", GetObjectX(owner), GetObjectY(owner));

        SetOwner(owner, glow);
        FrameTimerWithArg(1, glow, BomberSummonAtTargetLocation);
    }
}

void CastForceOfNatures(int owner)
{
    if (CurrentHealth(owner))
    {
        int mis = CreateObjectAt("DeathBall", GetObjectX(owner) + UnitAngleCos(owner, 19.0), GetObjectY(owner) + UnitAngleSin(owner, 19.0));

        Enchant(mis, "ENCHANT_FREEZE", 0.0);
        Enchant(mis, "ENCHANT_HASTED", 0.0);
        Enchant(mis, "ENCHANT_RUN", 0.0);
        SetOwner(owner, mis);
        DeleteObjectTimer(mis, 16);
        PushObject(mis, 30.0, GetObjectX(owner), GetObjectY(owner));
        PlaySoundAround(owner, 38);
    }
}

void FlyingLaser(int subUnit)
{
    int owner = GetOwner(subUnit), durate = GetDirection(subUnit);

    while (TRUE)
    {
        if (MaxHealth(subUnit))
        {
            if (CurrentHealth(owner) && durate)
            {
                FrameTimerWithArg(1, subUnit, FlyingLaser);
                MoveObject(subUnit, GetObjectX(subUnit) + GetObjectZ(subUnit + 1), GetObjectY(subUnit) + GetObjectZ(subUnit + 2));
                Effect("SENTRY_RAY", GetObjectX(subUnit + 1), GetObjectY(subUnit + 1), GetObjectX(subUnit), GetObjectY(subUnit));
                LookWithAngle(subUnit, --durate);
                break;
            }
            Delete(subUnit);
        }
        Delete(subUnit + 1);
        Delete(subUnit + 2);
        break;
    }
}

void LaserTouched()
{
    int owner = GetOwner(SELF);

    if (CurrentHealth(OTHER))
    {
        if (CurrentHealth(owner) && IsAttackedBy(OTHER, owner))
        {
            PlaySoundAround(OTHER, 298);
            Effect("VIOLET_SPARKS", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
            Enchant(OTHER, "ENCHANT_FREEZE", 1.0);
            Damage(OTHER, owner, 120, 14);
            Delete(SELF);
        }
    }
}

void TouchedStorm()
{
    if (!CurrentHealth(OTHER))
        return;

    int owner = GetOwner(SELF);

    if (CurrentHealth(owner) && IsAttackedBy(OTHER, owner))
    {
        Damage(other, owner, 175, 14);
        Enchant(other, "ENCHANT_CHARMING", 0.7);
        Enchant(other, "ENCHANT_CONFUSED", 1.5);
    }
}

void AbroadStorm(int subUnit)
{
    while (TRUE)
    {
        if (MaxHealth(subUnit))
        {
            int owner = GetOwner(subUnit), durate = GetDirection(subUnit);

            if (CurrentHealth(owner) && durate)
            {
                FrameTimerWithArg(1, subUnit, AbroadStorm);
                LookWithAngle(subUnit, --durate);
                MoveObject(subUnit, GetObjectX(subUnit) + GetObjectZ(subUnit + 1), GetObjectY(subUnit) + GetObjectZ(subUnit + 2));
                MoveObject(subUnit + 3, GetObjectX(subUnit), GetObjectY(subUnit));
                break;
            }
            Delete(subUnit);
        }
        Delete(subUnit + 1);
        Delete(subUnit + 2);
        Delete(subUnit + 3);
        break;
    }
}

void ExplosionWhetTouched()
{
    int owner = GetOwner(SELF);

    if (!CurrentHealth(OTHER))
        return;

    if (IsAttackedBy(OTHER, owner))
    {
        Effect("EXPLOSION", GetObjectX(OTHER), GetObjectY(OTHER), 0.0, 0.0);
        Damage(OTHER, owner, 90, 14);
        Enchant(OTHER, "ENCHANT_CHARMING", 0.1);
    }
}

void StraightParLoop(int subUnit)
{
    while (IsObjectOn(subUnit))
    {
        int owner = GetOwner(subUnit), durate = GetDirection(subUnit);

        if (CurrentHealth(owner) && durate)
        {
            FrameTimerWithArg(1, subUnit, StraightParLoop);

            LookWithAngle(subUnit, --durate);
            MoveObject(subUnit, GetObjectX(subUnit) + GetObjectZ(subUnit), GetObjectY(subUnit) + GetObjectZ(subUnit + 1));
            int unit = DummyUnitCreate("CarnivorousPlant", GetObjectX(subUnit), GetObjectY(subUnit));

            SetOwner(owner, unit);
            DeleteObjectTimer(unit, 1);
            SetCallback(unit, 9, ExplosionWhetTouched);
            DeleteObjectTimer(CreateObjectAt("Explosion", GetObjectX(subUnit), GetObjectY(subUnit)), 3);
            break;
        }
        Delete(subUnit);
        Delete(++subUnit);
        break;
    }
}

void FireHoleWhenTouched()
{
    int plr = GetDirection(self);

    if (CurrentHealth(other) && IsAttackedBy(other, player[plr]) && !HasEnchant(other, "ENCHANT_VILLAIN"))
    {
        Damage(other, player[plr], 90, 0);
        Enchant(other, "ENCHANT_VILLAIN", 0.3);
    }
}

void SpawnFireHole(string name, string fx_name, int posUnit, int owner)
{
    int k;
    float pos_x, pos_y;

    for (k = 0 ; k < 8 ; k += 1)
    {
        pos_x = MathSine(k * 45 + 90, 46.0);
        pos_y = MathSine(k * 45, 46.0);
        TeleportLocation(5, GetObjectX(posUnit) + pos_x, GetObjectY(posUnit) + pos_y);

        int j;
        for (j = 0 ; j < 4 ; j += 1)
        {
            Effect(fx_name, LocationX(5), LocationY(5), 0.0, 0.0);
            int unit = DummyUnitCreate(name, LocationX(5), LocationY(5));

            DeleteObjectTimer(unit, 1);
            SetOwner(owner, unit);
            SetCallback(unit, 9, FireHoleWhenTouched);
            TeleportLocationVector(5, pos_x, pos_y);
        }
    }
}

void SelectFireHolePosition(int glow)
{
    int owner = GetOwner(glow);

    if (CurrentHealth(owner))
    {
        Effect("JIGGLE", GetObjectX(glow), GetObjectY(glow), 40.0, 0.0);
        SpawnFireHole("CarnivorousPlant", "EXPLOSION", glow, owner);
        PlaySoundAround(glow, 485);
    }
    Delete(glow);
}

void DeathrayTargetLocation(int glow)
{
    int owner = GetOwner(glow);

    if (CurrentHealth(owner))
    {
        CastSpellObjectLocation("SPELL_DEATH_RAY", owner, GetObjectX(glow), GetObjectY(glow));
        CastSpellObjectLocation("SPELL_BURN", owner, GetObjectX(glow), GetObjectY(glow));
    }
    Delete(glow);
}

void UnitDetectedRaySpreay()
{
    int owner = GetOwner(SELF);

    if (CurrentHealth(owner))
    {
        CastSpellObjectObject("SPELL_DEATH_RAY", self, other);
    }
}

void SingleFistHit(int glow)
{
    int owner = GetOwner(glow);

    if (CurrentHealth(owner))
    {
        CastSpellObjectLocation("SPELL_FIST", owner, GetObjectX(glow), GetObjectY(glow));
    }
    Delete(glow);
}

void FistTrailer(int subUnit)
{
    int owner = GetOwner(subUnit);

    if (CurrentHealth(owner))
    {
        int base = CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(subUnit), GetObjectY(subUnit)), k;
        float xVect = GetObjectZ(subUnit), yVect = GetObjectZ(subUnit + 1);

        for (k = 0 ; k < 10 ; k += 1)
        {
            int unit = CreateObjectAt("Wizard", GetObjectX(base), GetObjectY(base));

            DeleteObjectTimer(unit, 1);
            SetOwner(owner, unit);
            CastSpellObjectObject("SPELL_FIST", unit, unit);
            MoveObject(base, GetObjectX(base) + xVect, GetObjectY(base) + yVect);
        }
        Delete(base);
    }
    Delete(subUnit);
    Delete(++subUnit);
}

void BomberSummonAtTargetLocation(int glow)
{
    int owner = GetOwner(glow);

    while (IsPlayerUnit(owner))
    {
        if (!CurrentHealth(owner))
            break;
        int pIndex = GetPlayerIndex(owner), bomb[32];

        MoveWaypoint(5, GetObjectX(glow), GetObjectY(glow));
        if (IsVisibleTo(owner, glow) && GetObjectX(glow) + GetObjectY(glow) < 4024.0)
        {
            Effect("TELEPORT", GetObjectX(glow), GetObjectY(glow), 0.0, 0.0);
            if (!CurrentHealth(bomb[pIndex]))
            {
                bomb[pIndex] = CreateObjectAt("BomberGreen", GetObjectX(glow), GetObjectY(glow));
                Enchant(CreateObjectAt("InvisibleLightBlueHigh", GetObjectX(glow), GetObjectY(glow)), "ENCHANT_ANCHORED", 0.0);
                DeleteObjectTimer(bomb[pIndex] + 1, 3);
                TrapSpells(bomb[pIndex], "SPELL_FORCE_OF_NATURE", "SPELL_PULL", "NULL");
            }
            else
            {
                Effect("LIGHTNING", GetObjectX(bomb[pIndex]), GetObjectY(bomb[pIndex]), GetObjectX(glow), GetObjectY(glow));
                MoveObject(bomb[pIndex], GetObjectX(glow), GetObjectY(glow));
            }
            SetOwner(owner, bomb[pIndex]);
        }
        else
            UniPrint(owner, "타겟형 스킬입니다, 사선 내 올바른 위치를 선택하십시오");
        break;
    }
    Delete(glow);
}

void PutShopItems()
{
    int ptr = InitShopPtr();
    
    ShopItem(ptr, 10);
    ShopItem(ptr + 7, 17);
    ShopItem(ptr + 12, 22);
}

void Stage2ShopItemCreated()
{
    int ptr = InitShopPtr();

    ShopItem(ptr + 1, 11);
    ShopItem(ptr + 2, 12);
    ShopItem(ptr + 3, 13);
    ShopItem(ptr + 4, 14);
    ShopItem(ptr + 5, 15);
    ShopItem(ptr + 6, 16);
    ShopItem(ptr + 8, 18);
    ShopItem(ptr + 9, 19);
    ShopItem(ptr + 10, 20);
    ShopItem(ptr + 11, 21);
    ShopItem(ptr + 12, 22);
}

int InitShopPtr()
{
    int ptr, k;

    if (!ptr)
    {
        ptr = CreateObject("InvisibleLightBlueHigh", 36) + 1;
        Delete(ptr - 1);
        for (k = 0 ; k < 13 ; k ++)
        {
            LookWithAngle(CreateObject("InvisibleLightBlueHigh", 36), k);
            MoveWaypoint(36, GetWaypointX(36) + 10.0, GetWaypointY(36) + 10.0);
        }
        Raise(ptr, BuyBullet);
        Raise(ptr + 1, UpgradeGun);
        Raise(ptr + 2, BuyLaserRifle);
        Raise(ptr + 3, BuyStormGun);
        Raise(ptr + 4, BuyStraightParGun);
        Raise(ptr + 5, BuyTripleGun);
        Raise(ptr + 6, BuyFireHole);
        Raise(ptr + 7, IncreaseYourLife);
        Raise(ptr + 8, AwardDeathRayShoot);
        Raise(ptr + 9, AwardFistShot);
        Raise(ptr + 10, AwardSummonBomber);
        Raise(ptr + 11, BuyShuriken);
        Raise(ptr + 12, TryGoldBetting);
    }
    return ptr;
}

string ShopItemType(int num)
{
    string table[] = { "HarpoonBolt", "BlackBook1", "AmuletOfClarity", "RewardMarker",
        "AmuletofManipulation", "AmuletofCombat", "ProtectionFire", "Ankh",
        "RewardMarkerPlus", "ProtectionEnchantments", "FieldGuide", "FanChakramInMotion", "SpinningCrown"};
    return table[num];
}

void ShopItem(int ptr, int wp)
{
    int unit = CreateObject("Maiden", wp), idx = GetDirection(ptr), func = ToInt(GetObjectZ(ptr));
    Enchant(CreateObject(ShopItemType(idx), wp), "ENCHANT_ANCHORED", 0.0);
    Frozen(unit, 1);
    Frozen(unit + 1, 1);
    SetDialog(unit, "NORMAL", func, DummyFunction);
}

void BuyBullet()
{
    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(other, "탄창을 25개 구입합니다, 가격은 450 골드 입니다");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 450)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -450);
        SpawnBullet("BraceletofChanneling", GetCaller(), 25);
    }
}

void SpawnBullet(string name, int owner, int max)
{
    int ptr = CreateObject("InvisibleLightBlueHigh", 1), k;

    LookWithAngle(ptr, max);
    ptr ++;
    for (k = 0 ; k < max ; k ++)
    {
        CreateObject(name, 23);
        LookWithAngle(ptr + k, 1);
    }
    FrameTimerWithArg(3, owner, DelayBulletPickup);
    FrameTimerWithArg(3, ptr, DelayBulletPickup);
}

void UpgradeGun()
{
    int plr = CheckPlayer(), pay = 2000;

    if (plr < 0)
        return;
    pay += (PLR_LV[plr] * 500);
    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "총기를 업그레이드 합니다, " + IntToString(pay) + " 골드가 요구됩니다.");
        UniPrint(OTHER, "최대 7레밸 까지 증가할 수 있으며 현재 레밸은 " + IntToString(PLR_LV[plr]) + " 입니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= pay)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        pay += (PLR_LV[plr] * 500);
        if (PLR_LV[plr] < 7)
        {
            PLR_LV[plr] ++;
            ChangeGold(other, -pay);
        }
        else
            UniPrint(OTHER, "최대 레밸에 도달했기 때문에 더 이상 증가시킬 수 없습니다.");
    }
}

void BuyLaserRifle()
{
    int plr;

    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "레이저 라이플 속성을 구입합니다, 가격은 3600 골드 입니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 3600)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -3600);
        plr = CheckPlayer();
        if (plr >= 0)
        {
            PLR_WEAPON[plr] = 1;
        }
    }
}

void BuyStormGun()
{
    int plr;

    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "스톰건 속성을 구입합니다, 가격은 3700 골드 입니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 3700)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -3700);
        plr = CheckPlayer();
        if (plr >= 0)
        {
            PLR_WEAPON[plr] = 2;
        }
    }
}

void BuyStraightParGun()
{
    int plr;

    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "에너지파 건을 구입합니다, 가격은 4000골드 입니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 4000)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -4000);
        plr = CheckPlayer();
        if (plr >= 0)
        {
            PLR_WEAPON[plr] = 3;
        }
    }
}

void BuyTripleGun()
{
    int plr;

    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "트리플 샷건을 구입합니다, 가격은 4200골드 입니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 4200)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -4200);
        plr = CheckPlayer();
        if (plr >= 0)
        {
            PLR_WEAPON[plr] = 4;
        }
    }
}

void BuyFireHole()
{
    int plr;

    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "불구덩이 폭탄을 구입합니다, 가격은 3800 골드 입니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 3800)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -3800);
        plr = CheckPlayer();
        if (plr >= 0)
            PLR_WEAPON[plr] = 5;
    }
}

void IncreaseYourLife()
{
    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "목숨 +1 구입을 위해서는 3000 골드가 요구됩니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 3000)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (STAGE_LV)
        {
            ChangeGold(other, -3000);
            LifePickEvent();
        }
        else
            UniPrint(OTHER, "스테이지 2부터 사용하실 수 있습니다.");
    }
}

void AwardDeathRayShoot()
{
    int plr;

    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "데스레이 사격을 합니다, 3400골드가 요구되며, 두번째 구입 시 다중 데스레이 사격으로 전환됩니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 3400)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -3400);
        plr = CheckPlayer();
        if (plr >= 0)
        {
            if (PLR_WEAPON[plr] == 6)
            {
                PLR_WEAPON[plr] = 7;
                UniPrint(OTHER, "다중 데스레이 사격으로 전환되었습니다.");
            }
            else
                PLR_WEAPON[plr] = 6;
        }
    }
}

void AwardFistShot()
{
    int plr;

    if (!HasEnchant(OTHER, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "피스트 사격은 4400 골드가 요구되고, 두번째 구입 시 다중 피스트 사격으로 전환됩니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 4400)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -4400);
        plr = CheckPlayer();
        if (plr >= 0)
        {
            if (PLR_WEAPON[plr] == 8)
            {
                PLR_WEAPON[plr] = 9;
                UniPrint(OTHER, "다중 피스트 사격으로 전환되었습니다");
            }
            else
                PLR_WEAPON[plr] = 8;
        }
    }
}

void AwardSummonBomber()
{
    int plr;

    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "포스오브 네이처의 힘을 담고있는 봄버를 소환할 수 있게 됩니다, 가격은 3350 골드이며,");
        UniPrint(OTHER, "두 번째 구입 시 포스오브 네이처 스킬을 시전하게 될 것입니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 3350)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -3350);
        plr = CheckPlayer();
        if (plr >= 0)
        {
            if (PLR_WEAPON[plr] == 10)
            {
                PLR_WEAPON[plr] = 11;
                UniPrint(OTHER, "포스오브 네이쳐 스킬시전으로 전환되었습니다.");
            }
            else
                PLR_WEAPON[plr] = 10;
        }
    }
}

void BuyShuriken()
{
    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "슈리켄 20개를 구입하려면 480 골드가 요구됩니다.");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else if (GetGold(other) >= 480)
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        ChangeGold(other, -480);
        FrameTimerWithArg(1, GetCaller(), DelayGiveTo);
        FrameTimerWithArg(1, CreateObject("FanChakram", 23), DelayGiveTo);
    }
}

void TryGoldBetting()
{
    if (!HasEnchant(other, "ENCHANT_AFRAID"))
    {
        Enchant(other, "ENCHANT_AFRAID", 1.0);
        UniPrint(OTHER, "골드 도박입니다, 성공시: 현재금화+현재금화의 반, 실패시: 현재금화-현재금화의 반");
        UniPrint(OTHER, DEFAULT_TRADE_MSG);
    }
    else
    {
        EnchantOff(other, "ENCHANT_AFRAID");
        if (!HasEnchant(other, "ENCHANT_PROTECT_FROM_FIRE"))
        {
            Enchant(other, "ENCHANT_PROTECT_FROM_FIRE", 70.0);
            if (Random(0, 2))
                ChangeGold(other, -(GetGold(other) / 2));
            else
                ChangeGold(other, GetGold(other) / 2);
        }
        else
            UniPrint(OTHER, "골드 도박 쿨다운 입니다 (70 초)");
    }
}

void DelayBulletPickup(int args)
{
    int unit;
    int k;

    if (!unit) unit = args;
    else
    {
        if (CurrentHealth(unit))
        {
            for (k = GetDirection(args - 1) - 1 ; k >= 0 ; k -= 1)
                Pickup(unit, args + k);
        }
        else
        {
            for (k = GetDirection(args - 1) - 1 ; k >= 0 ; k -= 1)
                Delete(args + k);
        }
        Delete(args - 1);
        unit = 0;
    }
}

void DelayGiveTo(int ptr)
{
    int unit;

    if (!unit) unit = ptr;
    else
    {
        Pickup(unit, ptr);
        unit = 0;
    }
}

void LifePickEvent()
{
    string sound = "AwardLife";
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (CurrentHealth(player[k]))
        {
            MoveWaypoint(4, GetObjectX(player[k]), GetObjectY(player[k]));
            AudioEvent(sound, 4);
        }
    }
    LIFE ++;
    PrintToAll("라이프가 추가되었습니다.");
}

int CheckPlayer()
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (IsCaller(player[k])) return k;
    }
    return -1;
}

int CheckOwner(int unit)
{
    int k;

    for (k = 9 ; k >= 0 ; k --)
    {
        if (IsOwnedBy(unit, player[k])) return k;
    }
    return -1;
}

void InventoryEmpty(int unit)
{
    while (GetLastItem(unit))
        Delete(GetLastItem(unit));
}

void rhombusPut(int wp, float x_low, float x_high, float y_low, float y_high)
{
    float pos_x= RandomFloat(y_low, y_high), pos_y = RandomFloat(0.0, x_high - x_low);

    MoveWaypoint(wp, x_high - y_high + pos_x - pos_y, pos_x + pos_y);
}

int MonsterHP(int num)
{
    int hp[16] = {
        10, 10, 15, 25,
        42, 65, 82, 96,
        116, 140, 155, 180,
        200, 220, 240, 260
    };
    
    return hp[num];
}

string MonsterName(int num)
{
    string table[16] = {
    "SmallAlbinoSpider", "GiantLeech", "Archer", "Swordsman",
    "Imp", "WhiteWolf", "BlackBear", "GruntAxe",
    "Zombie", "Bear", "Goon", "CarnivorousPlant",
    "FlyingGolem", "FireSprite", "MeleeDemon", "Horrendous"};

    return table[num];
}

int MonsterFlags(int num)
{
    //DESCRIPTION_ FLAGS
    //0: none, 1: slowly, 2: faster, 4: remove dead, 8: cant_has_stun, 16: when_dead_spawn_flame
    int flags[16] = {1 | 8, 8, 0, 1,
    1 | 2|8, 0, 0, 1,
    2|8|0x10, 0, 4|8, 2,
    1|8, 1|4, 1|4, 1|4};

    return flags[num];
}

int GetMaster()
{
    int unit;

    if (!unit)
    {
        unit = CreateObject("Hecubah", 24);
        Frozen(unit, 1);
        MoveObject(unit, 5020.0, 100.0);
    }
    return unit;
}

void SetUnitHealth(int unit, int amount)
{
    Damage(unit, 0, CurrentHealth(unit) - 1, -1);
    Pickup(unit, CreateObject("RottenMeat", 26));
    Damage(unit, 0, CurrentHealth(unit) - amount, -1);
}

void StrGGOver()
{
	int arr[34];
	string name = "HealOrb";
	int i = 0;
	arr[0] = 120; arr[1] = 469762048; arr[2] = 0; arr[3] = 1056; arr[4] = 268435456; arr[5] = 2; arr[6] = 128; arr[7] = 1073741824; arr[8] = 32; arr[9] = 117441536; 
	arr[10] = 3671928; arr[11] = 470319364; arr[12] = 1141842080; arr[13] = 35669056; arr[14] = 272893984; arr[15] = 4260610; arr[16] = 285352452; arr[17] = 35668224; arr[18] = 34080785; arr[19] = 2014384188; 
	arr[20] = 167905281; arr[21] = 272646392; arr[22] = 1082687760; arr[23] = 1343242240; arr[24] = 1140981824; arr[25] = 71568513; arr[26] = 4456516; arr[27] = 1074799106; arr[28] = 572676171; arr[29] = 566235586; 
	arr[30] = 143188240; arr[31] = 1049088; arr[32] = 32784; arr[33] = 1077938178; 
	while(i < 34)
	{
		drawStrGGOver(arr[i], name);
		i ++;
	}
}

void drawStrGGOver(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(28);
		pos_y = GetWaypointY(28);
	}
	for (i = 1 ; i > 0 && count < 1054 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 28);
		if (count % 96 == 95)
			MoveWaypoint(28, GetWaypointX(28) - 190.000000, GetWaypointY(28) + 2.000000);
		else
			MoveWaypoint(28, GetWaypointX(28) + 2.000000, GetWaypointY(28));
		count ++;
	}
	if (count >= 1054)
	{
		count = 0;
		MoveWaypoint(28, pos_x, pos_y);
	}
}

void StrStartButton()
{
	int arr[26], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(30), pos_y = GetWaypointY(30);

	arr[0] = 83757056; arr[1] = 66978816; arr[2] = 553910792; arr[3] = 16656; arr[4] = 138510466; arr[5] = 1077932100; arr[6] = 40935456; arr[7] = 268436497; 
	arr[8] = 545530888; arr[9] = 67109124; arr[10] = 2145456258; arr[11] = 1115750273; arr[12] = 570968080; arr[13] = 278921216; arr[14] = 142741504; arr[15] = 71573502; 
	arr[16] = 35684352; arr[17] = 1091633152; arr[18] = 545792255; arr[19] = 4726784; arr[20] = 136448064; arr[21] = 1180672; arr[22] = 34110480; arr[23] = 262144; 
	arr[24] = 2130837508; arr[25] = 3; 
	for (i = 0 ; i < 26 ; i ++)
		count = DrawStrStartButton(arr[i], name, count);
	MoveWaypoint(30, pos_x, pos_y);
}

int DrawStrStartButton(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 806 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 30);
		if (count % 60 == 59)
			MoveWaypoint(30, GetWaypointX(30) - 118.0, GetWaypointY(30) + 2.5);
		else
			MoveWaypoint(30, GetWaypointX(30) + 2.0, GetWaypointY(30));
		count ++;
	}
	return count;
}

void StrHelpAboutGame()
{
	int arr[26], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(32), pos_y = GetWaypointY(32);

	arr[0] = 71239712; arr[1] = 33554432; arr[2] = 1105740296; arr[3] = 8904960; arr[4] = 272765171; arr[5] = 539099200; arr[6] = 2081457185; arr[7] = 1141407775; 
	arr[8] = 17277192; arr[9] = 545398788; arr[10] = 4261954; arr[11] = 1083393; arr[12] = 537936880; arr[13] = 267919872; arr[14] = 133697536; arr[15] = 67110976; 
	arr[16] = 2097152; arr[17] = 16712208; arr[18] = 524543; arr[19] = 268443778; arr[20] = 805437504; arr[21] = 67110944; arr[22] = 52428304; arr[23] = 2114452488; 
	arr[24] = 3; arr[25] = 2; 
	for (i = 0 ; i < 26 ; i ++)
		count = DrawStrHelpAboutGame(arr[i], name, count);
	MoveWaypoint(32, pos_x, pos_y);
}

int DrawStrHelpAboutGame(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 806 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 32);
		if (count % 60 == 59)
			MoveWaypoint(32, GetWaypointX(32) - 118.0, GetWaypointY(32) + 2.5);
		else
			MoveWaypoint(32, GetWaypointX(32) + 2.0, GetWaypointY(32));
		count ++;
	}
	return count;
}


void StrAllClear()
{
	int arr[30];
	string name = "ManaBombOrb";
	int i = 0;
	arr[0] = 36167744; arr[1] = 2088731632; arr[2] = 268996361; arr[3] = 33624576; arr[4] = 574627842; arr[5] = 3540040; arr[6] = 1887305865; arr[7] = 303112223; arr[8] = 574644418; arr[9] = 33556480; 
	arr[10] = 8980624; arr[11] = 1880178432; arr[12] = 669515391; arr[13] = 1341933444; arr[14] = 2176; arr[15] = 554700800; arr[16] = 538968098; arr[17] = 2139617284; arr[18] = 143689736; arr[19] = 1052924; 
	arr[20] = 35652096; arr[21] = 1082148932; arr[22] = 2130741256; arr[23] = 285286369; arr[24] = 2116043300; arr[25] = 134250565; arr[26] = 149995392; arr[27] = 16781249; arr[28] = 66863040; arr[29] = 262208; 
	
	while(i < 30)
	{
		drawStrAllClear(arr[i], name);
		i ++;
	}
}

void drawStrAllClear(int arg_0, string name)
{
	int count;
	int i;
	float pos_x;
	float pos_y;

	if (!count)
	{
		pos_x = GetWaypointX(33);
		pos_y = GetWaypointY(33);
	}
	for (i = 1 ; i > 0 && count < 930 ; i <<= 1)
	{
		if (i & arg_0)
			CreateObject(name, 33);
		if (count % 84 == 83)
			MoveWaypoint(33, GetWaypointX(33) - 166.000000, GetWaypointY(33) + 2.000000);
		else
			MoveWaypoint(33, GetWaypointX(33) + 2.000000, GetWaypointY(33));
		count ++;
	}
	if (count >= 930)
	{
		count = 0;
		MoveWaypoint(33, pos_x, pos_y);
	}
}

void StrShopHere()
{
	int arr[19], i, count = 0;
	string name = "ManaBombOrb";
	float pos_x = GetWaypointX(7), pos_y = GetWaypointY(7);

	arr[0] = 505659964; arr[1] = 277094405; arr[2] = 21041681; arr[3] = 71847440; arr[4] = 167820169; arr[5] = 574759969; arr[6] = 139001936; arr[7] = 336666898; 
	arr[8] = 1552214080; arr[9] = 134936720; arr[10] = 606224912; arr[11] = 0; arr[12] = 2115113249; arr[13] = 1212414979; arr[14] = 8456456; arr[15] = 1008975944; 
	arr[16] = 302006404; arr[17] = 1627394048; arr[18] = 4177983; 
	for (i = 0 ; i < 19 ; i ++)
		count = DrawStrShopHere(arr[i], name, count);
	MoveWaypoint(7, pos_x, pos_y);
}

int DrawStrShopHere(int arg, string name, int count)
{
	int i;

	for (i = 1 ; i > 0 && count < 589 ; i <<= 1)
	{
		if (i & arg)
			CreateObject(name, 7);
		if (count % 53 == 52)
			MoveWaypoint(7, GetWaypointX(7) - 104.0, GetWaypointY(7) + 2.0);
		else
			MoveWaypoint(7, GetWaypointX(7) + 2.0, GetWaypointY(7));
		count ++;
	}
	return count;
}
