#include "global.h"
#include "constants/maps.h"
#include "constants/species.h"
#include "clock.h"
#include "new_game.h"
#include "battle_records.h"
#include "berry.h"
#include "contest.h"
#include "decoration_inventory.h"
#include "dewford_trend.h"
#include "easy_chat.h"
#include "event_data.h"
#include "field_specials.h"
#include "item_menu.h"
#include "lottery_corner.h"
#include "mail_data.h"
#include "mauville_man.h"
#include "play_time.h"
#include "player_pc.h"
#include "pokeblock.h"
#include "pokedex.h"
#include "pokemon_size_record.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "roamer.h"
#include "script_pokemon_80C4.h"
#include "overworld.h"
#include "rtc.h"
#include "script.h"
#include "secret_base.h"
#include "text.h"
#include "tv.h"

#include "item.h" //for starter items. remove if starter items function is removed
#include "constants/items.h"

EWRAM_DATA u8 gDifferentSaveFile = 0;
EWRAM_DATA u8 gUnknown_020297ED = 0;

extern u8 gPlayerPartyCount;
extern u8 gUnknown_03005CE8;
extern u16 gSaveFileStatus;

extern u8 gUnknown_0819FA81[];

static const struct ContestWinner sEmptyMuseumPortrait =
{
    .nickname = {EOS},
    .trainerName = {EOS},
};

#if DEBUG
const s8 gUnknown_Debug_0823C788[][2] =
{
    { MAP_GROUP(INSIDE_OF_TRUCK),                   MAP_NUM(INSIDE_OF_TRUCK) },
    { MAP_GROUP(OLDALE_TOWN_POKEMON_CENTER_2F),     MAP_NUM(OLDALE_TOWN_POKEMON_CENTER_2F) },
    { MAP_GROUP(LITTLEROOT_TOWN),                   MAP_NUM(LITTLEROOT_TOWN) },
    { MAP_GROUP(LITTLEROOT_TOWN_BRENDANS_HOUSE_2F), MAP_NUM(LITTLEROOT_TOWN_BRENDANS_HOUSE_2F) }
};

const u8 gUnknown_Debug_0823C790[] = _("KRÖTE");
#endif

void write_word_to_mem(u32 var, u8 *dataPtr)
{
    dataPtr[0] = var;
    dataPtr[1] = var >> 8;
    dataPtr[2] = var >> 16;
    dataPtr[3] = var >> 24;
}

void copy_word_to_mem(u8 *copyTo, u8 *copyFrom)
{
    s32 i;
    for (i = 0; i < 4; i++)
        copyTo[i] = copyFrom[i];
}

void InitPlayerTrainerId(void)
{
    write_word_to_mem((Random() << 16) | Random(), gSaveBlock2.playerTrainerId);
}

void SetDefaultOptions(void)
{
    gSaveBlock2.optionsWindowFrameType = 0;
    gSaveBlock2.optionsSound = OPTIONS_SOUND_STEREO;
	if (gSaveBlock2.nuzlockeMode == NUZLOCKE_MODE_OFF)
		gSaveBlock2.optionsBattleStyle = OPTIONS_BATTLE_STYLE_SHIFT;
    gSaveBlock2.optionsBattleSceneOff = FALSE;
	gSaveBlock2.optionsQuickFlee = OPTIONS_QUICK_FLEE_OFF;
	gSaveBlock2.optionsLowHPSound = OPTIONS_LOW_HP_SOUND_ON;
	gSaveBlock2.optionsKeypadSound = OPTIONS_KEYPAD_SOUND_ON;
	gSaveBlock2.optionsBikeMode = OPTIONS_BIKE_MODE_HOLD_B;
	gSaveBlock2.optionsFullParty = OPTIONS_FULL_PARTY_SWAP;
	gSaveBlock2.optionsKeyboard = OPTIONS_KEYBOARD_QWERTY;
	gSaveBlock2.optionsFont = OPTIONS_FONT_ROCKET;
}

void SetDefaultData(void)
{
	gSaveBlock2.nuzlockeMode = NUZLOCKE_MODE_OFF;			//defaults to standard, non-nuzlocke mode
	gSaveBlock2.freezeNuzlocke = FALSE;
	gSaveBlock2.gameMode = GAME_MODE_STORY;					//defaults to story mode
	gSaveBlock2.regionMapZoom = FALSE;
	gSaveBlock2.waitStatus = WAIT_UNABLE;
	gSaveBlock2.waitTime = 60;
	gSaveBlock2.registeredMenuItem = 0;
}

void SetDefaultGameTime(void)
{
	gSaveBlock2.timeSeconds = 0;
	gSaveBlock2.timeMinute = 0;
	gSaveBlock2.timeHour = TIME_HOUR_9AM;
	UpdateDayNightStatus();
	gSaveBlock2.timeDay = TIME_DAY_TUESDAY;
	gSaveBlock2.timeWeek = TIME_WEEK_1;
	gSaveBlock2.timeSeason = TIME_SEASON_SPRING;
	gSaveBlock2.timeYear = Random() % 8;
}

void GiveStarterItems(void)
{
	if (gSaveBlock2.gameMode == GAME_MODE_SANDBOX)
	{
		AddBagItem(ITEM_SUPER_ROD, 1);
		AddBagItem(ITEM_COIN_CASE, 1);
	}
	if (gSaveBlock2.nuzlockeMode != NUZLOCKE_MODE_OFF)
	{
		AddBagItem(ITEM_NUZLOCKE_BUFF, 1);
		AddBagItem(ITEM_FULL_RESTORE, 3);
	}
	else
	{
		AddBagItem(ITEM_POTION, 1);
	}
}

void ClearPokedexFlags(void)
{
    gUnknown_03005CE8 = 0;
    memset(&gSaveBlock2.pokedex.owned, 0, sizeof(gSaveBlock2.pokedex.owned));
    memset(&gSaveBlock2.pokedex.seen, 0, sizeof(gSaveBlock2.pokedex.seen));
}

void ResetContestAndMuseumWinners(void)
{
    s32 i;

    Contest_ResetWinners();
    for (i = 0; i < 5; i++)
        gSaveBlock1.museumPortraits[i] = sEmptyMuseumPortrait;
}

void ZeroBattleTowerData(void)
{
    CpuFill32(0, &gSaveBlock2.battleTower, sizeof(gSaveBlock2.battleTower));
}

#if DEBUG
void debug_sub_8052E04()
{
    u8 i;

    for (i = 0; i < 4; i++)
    {
        if (gUnknown_Debug_0823C788[i][0] == gSaveBlock1.location.mapGroup &&
            gUnknown_Debug_0823C788[i][1] == gSaveBlock1.location.mapNum)
        {
            break;
        }
    }

    i++;
    if (i >= 4)
    {
        i = 0;
    }

    Overworld_SetWarpDestination(gUnknown_Debug_0823C788[i][0], gUnknown_Debug_0823C788[i][1], -1, -1, -1);
}
#endif

void WarpToTruck(void)
{
    Overworld_SetWarpDestination(MAP_GROUP(INSIDE_OF_TRUCK), MAP_NUM(INSIDE_OF_TRUCK), -1, -1, -1);
    WarpIntoMap();
}

void ClearSav2(void)
{
    CpuFill16(0, &gSaveBlock2, sizeof(gSaveBlock2));
    SetDefaultOptions(); //doesn't work for some reason. Bug even in vanilla
}

void sub_8052E4C(void)
{
    gDifferentSaveFile = 0;
#if DEBUG
    gUnknown_020297ED = 0;
#endif
    sub_808C0A0();
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagScrollPositions();
}

void NewGameInitData(void)
{
    if (gSaveFileStatus == 0 || gSaveFileStatus == 2)
        RtcReset();

    gDifferentSaveFile = 1;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetPokedex();
    ZeroBattleTowerData();
    memset(&gSaveBlock1, 0, sizeof(gSaveBlock1));
    ClearMailData();
    gSaveBlock2.specialSaveWarp = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ClearTVShowData();
    ResetGabbyAndTy();
    ResetSecretBases();
    ClearBerryTrees();
    gSaveBlock1.money = 3000;
    ResetLinkContestBoolean();
    ResetGameStats();
    ResetContestAndMuseumWinners();
    InitLinkBattleRecords();
    InitShroomishSizeRecord();
    InitBarboachSizeRecord();
    gPlayerPartyCount = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    ClearRoamerData();
    ClearRoamerLocationData();
    gSaveBlock1.registeredItem = 0;
    ClearBag();
    NewGameInitPCItems();
    ClearPokeblocks();
    ClearDecorationInventories();
    InitEasyChatPhrases();
    SetupMauvilleOldMan();
    InitDewfordTrend();
    ResetFanClub();
    ResetLotteryCorner();
	SetDefaultGameTime(); 				//Second/minute 0, 9AM, Tuesday, Week 1, Spring, random year between 0 and 7
	GiveStarterItems(); 				//this will probably be moved somewhere else eventually, but it's here for now
    WarpToTruck();
    ScriptContext2_RunNewScript(gUnknown_0819FA81);
}

#if DEBUG
extern void debug_sub_80A3904(void);
extern void debug_sub_80A3714(void);
extern void debug_sub_8120F98(void);
extern void unref_sub_8070F90(void);

void debug_sub_8057508(bool32 arg0)
{
    gUnknown_020297ED = 1;
    NewGameInitData();
    gSaveBlock1.money = 999999;
    FlagSet(FLAG_SYS_POKEMON_GET);
    FlagSet(FLAG_SYS_POKEDEX_GET);
    FlagSet(FLAG_SYS_POKENAV_GET);
    FlagSet(FLAG_SYS_B_DASH);
    ScriptGiveMon(SPECIES_TREECKO, 99, 0, 0, 0, 0);

    if (arg0 == TRUE)
        SetMonData(&gPlayerParty[0], MON_DATA_NICKNAME, gUnknown_Debug_0823C790);

    debug_sub_80A3904();
    debug_sub_80A3714();
    debug_sub_8120F98();
    FlagSet(FLAG_SYS_HIPSTER_MEET);
    sub_80EB8EC();
    unref_sub_8070F90();
    InitTimeBasedEvents();
}
#endif
