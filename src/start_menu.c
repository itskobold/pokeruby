#include "global.h"
#include "battle_tower.h"
#include "start_menu.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "item_menu.h"
#include "load_save.h"
#include "m4a.h"
#include "main.h"
#include "event_obj_lock.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_game.h"
#include "option_menu.h"
#include "palette.h"
#include "play_time.h"
#include "pokedex.h"
#include "pokemon_menu.h"
#include "pokenav.h"
#include "overworld.h"
#include "safari_zone.h"
#include "save.h"
#include "save_menu_util.h"
#include "script.h"
#include "constants/songs.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "strings2.h"
#include "task.h"
#include "trainer_card.h"
#include "scanline_effect.h"
#include "script.h"

//Menu actions
enum {
	MENU_ACTION_DUMMY,
	MENU_ACTION_RETIRE,
    MENU_ACTION_POKEDEX,
    MENU_ACTION_POKEMON,
    MENU_ACTION_BAG,
    MENU_ACTION_MAP,
    MENU_ACTION_PLAYER,
	MENU_ACTION_WAIT,
    MENU_ACTION_SAVE,
	MENU_ACTION_TRADE,
	MENU_ACTION_BATTLE,
    MENU_ACTION_OPTIONS,
    MENU_ACTION_EXIT,
	MENU_ACTION_PLAYER_LINK,
};

//Wait dialog status
enum
{
    WAIT_IN_PROGRESS,
	WAIT_CANT,
};

//Save dialog status
enum
{
    SAVE_IN_PROGRESS,
    SAVE_SUCCESS,
    SAVE_CANCELED,
    SAVE_ERROR,
};

//Clock related things
const u8 *gHourLookup[24] = { TimeText_12AM, TimeText_1AM, TimeText_2AM, TimeText_3AM, TimeText_4AM, TimeText_5AM, TimeText_6AM, TimeText_7AM, TimeText_8AM, TimeText_9AM, TimeText_10AM, TimeText_11AM, TimeText_12PM, TimeText_1PM, TimeText_2PM, TimeText_3PM, TimeText_4PM, TimeText_5PM, TimeText_6PM, TimeText_7PM, TimeText_8PM, TimeText_9PM, TimeText_10PM, TimeText_11PM };
const u8 *gDayLookup[7] = { TimeText_Monday, TimeText_Tuesday, TimeText_Wednesday, TimeText_Thursday, TimeText_Friday, TimeText_Saturday, TimeText_Sunday };
const u8 *gSeasonLookup[12] = { TimeText_EarlySpring, TimeText_Spring, TimeText_LateSpring, TimeText_EarlySummer, TimeText_Summer, TimeText_LateSummer, TimeText_EarlyFall, TimeText_Fall, TimeText_LateFall, TimeText_EarlyWinter, TimeText_Winter, TimeText_LateWinter };
const u8 *gDNStatusLookup[4] = { TimeText_Dawn, TimeText_Day, TimeText_Dusk, TimeText_Night };

const u8 gWaitTimeLookup[4] = { 1, 3, 6, 12 };

#if DEBUG
static u32 _debugStartMenu_0  __attribute__((unused));
static u32 _debugStartMenu_1 __attribute__((unused));
#endif

static u8 (*dialogCallback)(void);
static u8 saveDialogTimer;    //Number of frames to keep the window on screen after save was completed
static bool8 savingComplete;

extern bool8 gDifferentSaveFile;
extern u16 gSaveFileStatus;
extern u16 gSpecialVar_Result;

extern u8 gUnknown_03004860;

u8 (*gMenuCallback)(void);

EWRAM_DATA static u8 sStartMenuScroll = 0;
EWRAM_DATA static u8 sStartMenuCursorPos = 0;
EWRAM_DATA static u8 sWaitMenuCursorPos = 0;
EWRAM_DATA static u8 sStartMenuCursorPosBuffer = 0; //stores the last position the cursor was in
EWRAM_DATA static u8 sWaitMenuCursorPosBuffer = 0; //stores the last position the cursor was in
EWRAM_DATA static u8 sNumStartMenuActions = 0;
EWRAM_DATA static u8 sCurrentStartMenuActions[NUM_START_MENU_ACTIONS] = {0};
EWRAM_DATA static u8 sStartMenuActionList[NUM_START_MENU_ACTIONS] = {0}; //different to sCurrentStartMenuActions - this holds the options available to the player before the start menu is built
EWRAM_DATA static bool8 sHasMenuBeenOpenedBefore = FALSE;

//Text strings
extern u8 gSaveText_PlayerSavedTheGame[];
extern u8 gSaveText_DontTurnOff[];
extern u8 gSaveText_ThereIsAlreadyAFile[];
extern u8 gSaveText_ThereIsADifferentFile[];
extern u8 gSaveText_WouldYouLikeToSave[];

extern u8 gWaitText_HowLongToWait[];
extern u8 gWaitText_YouCantWaitNow[];
extern u8 gWaitText_WaitingTooOften[];
extern u8 gWaitText_HoursRemaining[];
extern u8 gWaitText_Hour[];
extern u8 gWaitText_Space[];
extern u8 gWaitText_S[];
extern u8 gWaitText_NotEnoughTime[];

static u8 StartMenu_DummyCallback(void);
static u8 StartMenu_RetireCallback(void);
static u8 StartMenu_PokedexCallback(void);
static u8 StartMenu_PokemonCallback(void);
static u8 StartMenu_BagCallback(void);
static u8 StartMenu_PokenavCallback(void);
static u8 StartMenu_PlayerCallback(void);
static u8 StartMenu_WaitCallback(void);
static u8 StartMenu_SaveCallback(void);
static u8 StartMenu_OptionCallback(void);
static u8 StartMenu_ExitCallback(void);
static u8 StartMenu_PlayerLinkCallback(void);

//Wait functions
static u8 WaitCallback1(void);
static u8 WaitCallback2(void);
static u8 RunWaitDialogCallback(void);
static void DisplayWaitMessageWithCallback(const u8 *ptr, u8 (*func)(void));
static u8 WaitMenu_InputProcessCallback(void);
static u8 WaitMenu_DoWaitCallback(void);
static u8 WaitMenu_CancelCallback(void);
static void WaitMenu_Close(void);
static void WaitMenu_ResetCursor(void);
static u8 WaitDialogCB_DoWaitMenu(void);
static u8 WaitDialogCB_TryToWait(void);
static u8 WaitDialogCB_CantWait(void);
static u8 WaitDialogCB_DisplayConfirmYesNoMenu(void);
static u8 WaitDialogCB_ProcessConfirmYesNoMenu(void);

static const struct MenuAction sStartMenuItems[] =
{
    { SystemText_Dummy, StartMenu_DummyCallback },
    { SystemText_Retire, StartMenu_RetireCallback },
    { SystemText_Pokedex, StartMenu_PokedexCallback },
    { SystemText_Pokemon, StartMenu_PokemonCallback },
    { SystemText_BAG, StartMenu_BagCallback },
    { SystemText_Map, StartMenu_PokenavCallback },
    { SystemText_Player, StartMenu_PlayerCallback },
	{ SystemText_Wait, StartMenu_WaitCallback },
    { SystemText_Save, StartMenu_SaveCallback },
	{ SystemText_Trade, StartMenu_PlayerCallback }, //replace this with new trade function when it's done
	{ SystemText_Battle, StartMenu_PlayerCallback }, //replace this with link battle function when it's done
    { SystemText_Option, StartMenu_OptionCallback },
    { SystemText_Exit, StartMenu_ExitCallback },
    { SystemText_Player, StartMenu_PlayerLinkCallback },
};

static const struct MenuAction sWaitMenuItems[] =
{
    { WaitText_1Hour, WaitMenu_DoWaitCallback },
    { WaitText_3Hours, WaitMenu_DoWaitCallback },
    { WaitText_6Hours, WaitMenu_DoWaitCallback },
    { WaitText_12Hours, WaitMenu_DoWaitCallback },
    { WaitText_Cancel, WaitMenu_CancelCallback },
};

//Private functions
static void CloseStartMenu(void);
static void BuildStartMenuActions(void);
static void UpdateStartMenuOptions(void);
static void AddStartMenuAction(u8 action);
static void DisplayDateAndWeatherWindow(void);
static bool32 PrintStartMenuItemsMultistep(s16 *a);
static bool32 InitStartMenuMultistep(s16 *a, s16 *b);
static void Task_StartMenu(u8 taskId);
static void BuildStartMenuActionList(void);
static void StartMenu_CreateScrollArrows(void);
static void StartMenu_DestroyScrollArrows(void);
static u8 StartMenu_InputProcessCallback(void);
static u8 GetFirstStartMenuAction(void);
static u8 SaveCallback1(void);
static u8 SaveCallback2(void);
static void sub_807160C(void);
static u8 RunSaveDialogCallback(void);
static void DisplaySaveMessageWithCallback(const u8 *ptr, u8 (*func)(void));
static void Task_SaveDialog(u8 taskId);
static void sub_8071700(void);
static void HideSaveDialog(void);
static void SaveDialogStartTimeout(void);
static u8 SaveDialogCheckForTimeoutOrKeypress(void);
static u8 SaveDialogCheckForTimeoutAndKeypress(void);
static u8 SaveDialogCheckForTimeoutAndKeypress(void);
static u8 SaveDialogCB_DisplayConfirmMessage(void);
static u8 SaveDialogCB_DisplayConfirmYesNoMenu(void);
static u8 SaveDialogCB_ProcessConfirmYesNoMenu(void);
static u8 SaveDialogCB_SaveFileExists(void);
static u8 SaveDialogCB_DisplayOverwriteYesNoMenu(void);
static u8 SaveDialogCB_ProcessOverwriteYesNoMenu(void);
static u8 SaveDialogCB_DisplaySavingMessage(void);
static u8 SaveDialogCB_DoSave(void);
static u8 SaveDialogCB_SaveSuccess(void);
static u8 SaveDialogCB_ReturnSuccess(void);
static u8 SaveDialogCB_SaveError(void);
static u8 SaveDialogCB_ReturnError(void);
static void sub_80719F0(void);
static bool32 sub_80719FC(u8 *ptr);
static void sub_8071B54(void);
static void Task_8071B64(u8 taskId);
static void CopyClockStrings(void);
static void ShowTimeAndDate(void);
static void ShowWeatherOrSafariBalls(void);

#if DEBUG

void debug_sub_8075D9C(void);

u8 debug_sub_8075C30(void)
{
	StartMenu_DestroyScrollArrows();
	CloseMenu();
    debug_sub_8075D9C();
    return 1;
}

extern const u8 gUnknown_Debug_839B6D8[];

void debug_sub_8075C40(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 savedIme;
    s32 i;

    switch (data[0])
    {
    case 0:
        m4aSoundVSyncOff();
        data[0]++;
        break;
    case 1:
        savedIme = REG_IME;
        REG_IME = 0;
        REG_TM2CNT_L = 0;
        REG_TM2CNT = 0x830000;
        for (i = 0; i < 0xC350; i++)
        {
            DmaSet(
                0,
                gScanlineEffectRegBuffers,
                &REG_WIN0H,
                ((DMA_ENABLE | DMA_START_HBLANK | DMA_16BIT | DMA_REPEAT | DMA_SRC_INC | DMA_DEST_RELOAD) << 16) | 1);
            DmaStop(0);
        }
        REG_TM2CNT_H = 0;
        _debugStartMenu_0 = REG_TM2CNT_L;
        REG_TM2CNT_L = 0;
        REG_IME = savedIme;
        _debugStartMenu_1 = i;
        m4aSoundVSyncOn();
        data[0]++;
        break;
    case 2:
        PlaySE(0x15);
        ConvertIntToDecimalStringN(gStringVar1, _debugStartMenu_1, 1, 8);
        ConvertIntToDecimalStringN(gStringVar2, _debugStartMenu_0, 1, 8);
        Menu_DisplayDialogueFrame();
        Menu_PrintText(gUnknown_Debug_839B6D8, 2, 15);
        data[0]++;
        break;
    case 3:
        if (gMain.newKeys & A_BUTTON)
        {
            Menu_EraseScreen();
            ScriptContext2_Disable();
            DestroyTask(taskId);
        }
        break;
    }
}

void debug_sub_8075D9C(void)
{
    CreateTask(debug_sub_8075C40, 10);
    ScriptContext2_Enable();
}

void debug_sub_8075DB4(struct BattleTowerEReaderTrainer *ereaderTrainer, const u8 *b, u32 trainerId)
{
    s32 i;
    s32 r3;

    ereaderTrainer->trainerClass = trainerId % 77;
    write_word_to_mem(trainerId, ereaderTrainer->trainerId);
    StringCopy8(ereaderTrainer->name, b);
    r3 = 7;
    for (i = 0; i < 6; i++)
    {
        ereaderTrainer->greeting[i] = gSaveBlock1.easyChats.unk2B28[i];
        ereaderTrainer->farewellPlayerLost[i] = r3;
        ereaderTrainer->farewellPlayerWon[i] = r3 + 6;
        r3++;
    }
    for (i = 0; i < 3; i++)
        sub_803AF78(&gPlayerParty[i], &ereaderTrainer->party[i]);
    SetEReaderTrainerChecksum(ereaderTrainer);
}

void unref_sub_8070F90(void)
{
    FlagSet(FLAG_SYS_POKEDEX_GET);
    FlagSet(FLAG_SYS_POKEMON_GET);
    FlagSet(FLAG_SYS_POKENAV_GET);
}

#endif

static void AddStartMenuAction(u8 action)
{
    AppendToList(sCurrentStartMenuActions, &sNumStartMenuActions, action);
}

static u8 GetFirstStartMenuAction(void) //probably a much less ugly way to do this but idec at this point
{
	if (GetSafariZoneFlag() == TRUE)
		return MENU_ACTION_RETIRE;
	else if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
		return MENU_ACTION_POKEDEX;
	else if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
		return MENU_ACTION_POKEMON;
	else if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
		return MENU_ACTION_MAP;
	else
		return MENU_ACTION_BAG;
}

static void BuildStartMenuActionList(void)
{
	int i, j;
	
	j = 0;
	
	for (i = 0; i < NUM_START_MENU_ACTIONS; i++)
	{
		switch(i)
		{
			case MENU_ACTION_RETIRE:
				if (GetSafariZoneFlag() == TRUE)
				{
					sStartMenuActionList[j] = MENU_ACTION_RETIRE;
					j++;
				}
				break;
			case MENU_ACTION_POKEDEX:
				if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
				{
					sStartMenuActionList[j] = MENU_ACTION_POKEDEX;
					j++;
				}
				break;
			case MENU_ACTION_POKEMON:
				if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
				{
					sStartMenuActionList[j] = MENU_ACTION_POKEMON;
					j++;
				}
				break;
			case MENU_ACTION_MAP:
				if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
				{
					sStartMenuActionList[j] = MENU_ACTION_MAP;
					j++;
				}
				break;
			case MENU_ACTION_PLAYER:
				if (is_c1_link_related_active() == TRUE)
				{
					sStartMenuActionList[j] = MENU_ACTION_PLAYER_LINK;
				}
				else
				{
					sStartMenuActionList[j] = MENU_ACTION_PLAYER;
				}
				j++;
				break;
			case MENU_ACTION_PLAYER_LINK:
				break;
			case MENU_ACTION_SAVE:
				if (gSaveBlock2.nuzlockeMode < NUZLOCKE_MODE_HARDLOCKE) //remove save option if on hardlocke or deadlocke
				{
					sStartMenuActionList[j] = MENU_ACTION_SAVE;
					j++;
				}
				break;
			case MENU_ACTION_TRADE:
				if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE) //give it it's own flag check eventually
				{
					sStartMenuActionList[j] = MENU_ACTION_TRADE;
					j++;
				}
				break;
			case MENU_ACTION_BATTLE:
				if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE) //give it it's own flag check eventually
				{
					sStartMenuActionList[j] = MENU_ACTION_BATTLE;
					j++;
				}
				break;
			default:
				sStartMenuActionList[j] = i;
				j++;
				break;
		}
	}
}

static void BuildStartMenuActions(void)
{
	int i, firstOption, lastOption;
	
	sNumStartMenuActions = 0;
	
	//review this later
	if (sHasMenuBeenOpenedBefore == FALSE)
	{
		sStartMenuScroll = 1; //clears the blank space at the top of the menu if it has never been opened before caused by the retire option
		sHasMenuBeenOpenedBefore = TRUE;
	}
	
	BuildStartMenuActionList();
	
	for (i = 0; i < 5; i++)
	{
		if (sStartMenuActionList[sStartMenuScroll + i] != MENU_ACTION_DUMMY)
		{
			if (i == 0)
				firstOption = sStartMenuActionList[sStartMenuScroll];
			else if (i == 4)
				lastOption = sStartMenuActionList[sStartMenuScroll + i];
			
			AddStartMenuAction(sStartMenuActionList[sStartMenuScroll + i]);
		}
	}
	
	if (firstOption == GetFirstStartMenuAction())
		SetVerticalScrollIndicators(TOP_ARROW, INVISIBLE);
	if (lastOption == MENU_ACTION_EXIT)
		SetVerticalScrollIndicators(BOTTOM_ARROW, INVISIBLE);
}

static void CopyClockStrings(void)
{
	StringCopy(gStringVar1, gHourLookup[gSaveBlock2.timeHour]);
	StringAppend(gStringVar1, TimeText_Spacer);
	StringAppend(gStringVar1, gDNStatusLookup[gSaveBlock2.dayNightStatus]);
	StringCopy(gStringVar2, gDayLookup[gSaveBlock2.timeDay]);
	StringCopy(gStringVar3, gSeasonLookup[CalculateSubSeason()]);
}

//Show hour & date
//Also shows weather forecast or safari ball count (if in safari zone)
static void DisplayDateAndWeatherWindow(void)
{
    Menu_DrawStdWindowFrame(2, 14, 28, 19);
	
	CopyClockStrings();
	ShowTimeAndDate();
	ShowWeatherOrSafariBalls();
}

static void ShowTimeAndDate(void)
{
	Menu_PrintText(TimeText_PrintTimeString, 3, 15);
}

void UpdateTimeAndDate(void) //called every minute from play_time.c
{
	CopyClockStrings();
	ShowTimeAndDate();
}

static void ShowWeatherOrSafariBalls(void)
{
	if (GetSafariZoneFlag()) //print safari ball count if in safari zone
	{
		AlignInt2InMenuWindow(gStringVar1, gNumSafariBalls, 12, 1);
		Menu_PrintText(gOtherText_SafariStock, 3, 17);
	}
	else //display weather forecast if else - this will need to be implemented later
	{
		Menu_PrintText(gOtherText_WeatherForecastPlaceholder, 3, 17);
	}
}

void UpdateWeatherOrSafariBalls(void) //called every minute from play_time.c
{
	CopyClockStrings();
	ShowWeatherOrSafariBalls();
}

//Prints n menu items starting at *index
static bool32 PrintStartMenuItemsMultistep(s16 *index)
{
	int i;
    int _index = *index;
	
	Menu_BlankWindowRect(23, 1, 28, 11); //clears the menu first

	for (i = 0; i < 5; i++)
	{
        Menu_PrintText(sStartMenuItems[sCurrentStartMenuActions[_index]].text, 23, 2 + _index * 2);
        _index++;
        if (_index >= sNumStartMenuActions)
        {
            *index = _index;
            return TRUE;
        }
	}

    *index = _index;
    return FALSE;
}

static void InitStartMenu(void)
{
    s16 step = 0;
    s16 index = 0;
	
	gMain.stopClockUpdating = TRUE;

    while (InitStartMenuMultistep(&step, &index) == FALSE)
        ;
}

static void UpdateStartMenuOptions(void)
{
	s16 step = 4; //start at print menu items
	s16 index = 0;
	
	BuildStartMenuActions();
	InitStartMenuMultistep(&step, &index);
}

static bool32 InitStartMenuMultistep(s16 *step, s16 *index)
{
    switch (*step)
    {
    case 1:
		StartMenu_CreateScrollArrows();
		BuildStartMenuActions();
		(*step)++;
        break;
    case 2:
        DisplayDateAndWeatherWindow();
        (*step)++;
        break;
    case 3:
		Menu_DrawStdWindowFrame(22, 0, 29, 13);
        *index = 0;
        (*step)++;
        break;
    case 4:
        if (PrintStartMenuItemsMultistep(index))
            (*step)++;
        break;
    case 0:
        (*step)++;
        break;
    case 5:
        sStartMenuCursorPos = InitMenu(0, 0x17, 2, 5, sStartMenuCursorPos, 6);
		gMain.stopClockUpdating = TRUE;
        return TRUE;
    }
    return FALSE;
}

static void Task_StartMenu(u8 taskId)
{
    if (InitStartMenuMultistep(gTasks[taskId].data, gTasks[taskId].data + 1) == TRUE)
    {
        *gTasks[taskId].data = 0;
        SwitchTaskToFollowupFunc(taskId);
    }
}

void CreateStartMenuTask(void (*func)(u8))
{
    u8 taskId;

    InitMenuWindow(&gMenuTextWindowTemplate);
    taskId = CreateTask(Task_StartMenu, 0x50);
    SetTaskFuncWithFollowupFunc(taskId, Task_StartMenu, func);
}

void sub_80712B4(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        gMenuCallback = StartMenu_InputProcessCallback;
        task->data[0]++;
        break;
    case 1:
        if (gMenuCallback() == 1)
            DestroyTask(taskId);
        break;
    }
}

void sub_8071310(void)
{
    if (!is_c1_link_related_active())
    {
        FreezeEventObjects();
        sub_80594C0();
        sub_80597F4();
    }
    CreateStartMenuTask(sub_80712B4);
    ScriptContext2_Enable();
}

static void StartMenu_CreateScrollArrows(void)
{
	ClearVerticalScrollIndicatorPalettes();
	LoadScrollIndicatorPalette();
	CreateVerticalScrollIndicators(TOP_ARROW, 208, 12);
	CreateVerticalScrollIndicators(BOTTOM_ARROW, 208, 100);
	SetVerticalScrollIndicatorPriority(TOP_ARROW, 0);
	SetVerticalScrollIndicatorPriority(BOTTOM_ARROW, 0);
}

static void StartMenu_DestroyScrollArrows(void)
{
	DestroyVerticalScrollIndicator(TOP_ARROW);
	DestroyVerticalScrollIndicator(BOTTOM_ARROW);
}

static u8 StartMenu_InputProcessCallback(void)
{
    if (gMain.newKeys & DPAD_UP)
    {
		if (sStartMenuCursorPos == 0 && sStartMenuItems[sCurrentStartMenuActions[sStartMenuCursorPos]].func != sStartMenuItems[GetFirstStartMenuAction()].func) //if cursor is at the top and item isn't first available option, the list scrolls
		{
			sStartMenuScroll--;
			UpdateStartMenuOptions();
			SetVerticalScrollIndicators(BOTTOM_ARROW, VISIBLE);
			PlaySE(SE_SELECT);
		}
		else
		{
			sStartMenuCursorPosBuffer = sStartMenuCursorPos;
			sStartMenuCursorPos = Menu_MoveCursorNoWrap(-1);
			if (sStartMenuCursorPosBuffer != sStartMenuCursorPos)
				PlaySE(SE_SELECT);
		}
    }
    if (gMain.newKeys & DPAD_DOWN)
    {
		if (sStartMenuCursorPos == 4 && sStartMenuItems[sCurrentStartMenuActions[sStartMenuCursorPos]].func != StartMenu_ExitCallback)
		{
			sStartMenuScroll++;
			UpdateStartMenuOptions();
			SetVerticalScrollIndicators(TOP_ARROW, VISIBLE);
			PlaySE(SE_SELECT);
		}
		else
		{
			sStartMenuCursorPosBuffer = sStartMenuCursorPos;
			sStartMenuCursorPos = Menu_MoveCursorNoWrap(1);
			if (sStartMenuCursorPosBuffer != sStartMenuCursorPos)
				PlaySE(SE_SELECT);
		}
    }
    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if (sStartMenuItems[sCurrentStartMenuActions[sStartMenuCursorPos]].func == StartMenu_PokedexCallback)
        {
            if (GetNationalPokedexCount(0) == 0)
                return 0;
        }
        gMenuCallback = sStartMenuItems[sCurrentStartMenuActions[sStartMenuCursorPos]].func;
        if (gMenuCallback != StartMenu_SaveCallback &&
		   gMenuCallback != StartMenu_WaitCallback &&
           gMenuCallback != StartMenu_ExitCallback &&
           gMenuCallback != StartMenu_RetireCallback)
            FadeScreen(1, 0);
        return 0;
    }
    if (gMain.newKeys & (START_BUTTON | B_BUTTON))
    {
		CloseStartMenu();
        return 1;
    }
    return 0;
}

//When player selects POKEDEX
static u8 StartMenu_PokedexCallback(void)
{
    if (!gPaletteFade.active)
    {
        IncrementGameStat(GAME_STAT_CHECKED_POKEDEX);
        PlayRainSoundEffect();
        SetMainCallback2(CB2_InitPokedex);
        return 1;
    }
    return 0;
}

//When player selects POKEMON
static u8 StartMenu_PokemonCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        SetMainCallback2(sub_8089A70);
        return 1;
    }
    return 0;
}

//When player selects BAG
static u8 StartMenu_BagCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        SetMainCallback2(sub_80A53F8);
        return 1;
    }
    return 0;
}

//When player selects POKENAV
static u8 StartMenu_PokenavCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        SetMainCallback2(sub_80EBA5C);
        return 1;
    }
    return 0;
}

//When player selects his/her name
static u8 StartMenu_PlayerCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        TrainerCard_ShowPlayerCard(c2_exit_to_overworld_1_sub_8080DEC);
        return 1;
    }
    return 0;
}

//When player chooses WAIT
static u8 StartMenu_WaitCallback(void)
{
    StartMenu_DestroyScrollArrows();
    Menu_DestroyCursor();
    gMenuCallback = WaitCallback1;
    return 0;
}

//When player selects SAVE
static u8 StartMenu_SaveCallback(void)
{
	StartMenu_DestroyScrollArrows();
    Menu_DestroyCursor();
    gMenuCallback = SaveCallback1;
    return 0;
}

//When player selects OPTION
static u8 StartMenu_OptionCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        SetMainCallback2(CB2_InitOptionMenu);
        gMain.savedCallback = c2_exit_to_overworld_1_sub_8080DEC;
        return 1;
    }
    return 0;
}

static void CloseStartMenu(void)
{
	gMain.stopClockUpdating = FALSE;
	StartMenu_DestroyScrollArrows();
	CloseMenu();
}

//When player selects EXIT
static u8 StartMenu_ExitCallback(void)
{
	CloseStartMenu();
    return 1;
}

//When player selects RETIRE
static u8 StartMenu_RetireCallback(void)
{
	CloseStartMenu();
    SafariZoneRetirePrompt();
    return 1;
}

//Not supposed to be used, functionally the same as Exit
static u8 StartMenu_DummyCallback(void)
{
	CloseStartMenu();
    return 1;
}

//When player selects their name in multiplayer mode
static u8 StartMenu_PlayerLinkCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        TrainerCard_ShowLinkCard(gUnknown_03004860, c2_exit_to_overworld_1_sub_8080DEC);
        return 1;
    }
    return 0;
}

static u8 SaveCallback1(void)
{
    sub_807160C();
    gMenuCallback = SaveCallback2;
    return FALSE;
}

static u8 SaveCallback2(void)
{
    switch (RunSaveDialogCallback())
    {
    case SAVE_IN_PROGRESS:
        return FALSE;
    case SAVE_CANCELED:
        //Go back to start menu
        Menu_EraseScreen();
        InitStartMenu();
        gMenuCallback = StartMenu_InputProcessCallback;
        return FALSE;
    case SAVE_SUCCESS:
    case SAVE_ERROR:
        Menu_EraseScreen();
        ScriptUnfreezeEventObjects();
        ScriptContext2_Disable();
        return TRUE;
    }
    return FALSE;
}

static void sub_807160C(void)
{
    save_serialize_map();
    dialogCallback = SaveDialogCB_DisplayConfirmMessage;
    savingComplete = FALSE;
}

static u8 RunSaveDialogCallback(void)
{
    if (savingComplete)
    {
        if (!Menu_UpdateWindowText())
            return 0;
    }
    savingComplete = FALSE;
    return dialogCallback();
}

void ScrSpecial_DoSaveDialog(void)
{
    sub_807160C();
    CreateTask(Task_SaveDialog, 0x50);
}

static void DisplaySaveMessageWithCallback(const u8 *ptr, u8 (*func)(void))
{
    StringExpandPlaceholders(gStringVar4, ptr);
    Menu_DisplayDialogueFrame();
    MenuPrintMessageDefaultCoords(gStringVar4);
    savingComplete = TRUE;
    dialogCallback = func;
}

static void Task_SaveDialog(u8 taskId)
{
    u8 status = RunSaveDialogCallback();

    switch (status)
    {
    case SAVE_CANCELED:
    case SAVE_ERROR:
        gSpecialVar_Result = 0;
        break;
    case SAVE_SUCCESS:
        gSpecialVar_Result = status;
        break;
    case SAVE_IN_PROGRESS:
        return;
    }
    DestroyTask(taskId);
    EnableBothScriptContexts();
}

static void sub_8071700(void)
{
    HandleCloseSaveWindow(0, 0);
}

static void HideSaveDialog(void)
{
    Menu_EraseWindowRect(20, 8, 26, 13);
}

static void SaveDialogStartTimeout(void)
{
    saveDialogTimer = 60;
}

static bool8 SaveDialogCheckForTimeoutOrKeypress(void)
{
    saveDialogTimer--;
    if (gMain.heldKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        return TRUE;
    }
    else if (saveDialogTimer == 0)
        return TRUE;
    return FALSE;
}

static bool8 SaveDialogCheckForTimeoutAndKeypress(void)
{
    if (saveDialogTimer != 0)
        saveDialogTimer--;
    else if (gMain.heldKeys & A_BUTTON)
        return TRUE;
    return FALSE;
}

static u8 SaveDialogCB_DisplayConfirmMessage(void)
{
    Menu_EraseScreen();
    HandleDrawSaveWindowInfo(0, 0);
    DisplaySaveMessageWithCallback(gSaveText_WouldYouLikeToSave, SaveDialogCB_DisplayConfirmYesNoMenu);
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DisplayConfirmYesNoMenu(void)
{
    DisplayYesNoMenu(20, 8, 1);
    dialogCallback = SaveDialogCB_ProcessConfirmYesNoMenu;
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ProcessConfirmYesNoMenu(void)
{
    switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
        HideSaveDialog();
        switch (gSaveFileStatus)
        {
        case 0:
        case 2:
            if (gDifferentSaveFile == FALSE)
            {
                dialogCallback = SaveDialogCB_SaveFileExists;
                return SAVE_IN_PROGRESS;
            }
            dialogCallback = SaveDialogCB_DisplaySavingMessage;
            return SAVE_IN_PROGRESS;
        default:
            dialogCallback = SaveDialogCB_SaveFileExists;
            return SAVE_IN_PROGRESS;
        }
        break;
    case -1:    //B button
    case 1:     //NO
        HideSaveDialog();
        sub_8071700();
        return SAVE_CANCELED;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_SaveFileExists(void)
{
    DisplaySaveMessageWithCallback(
      gDifferentSaveFile == TRUE ? gSaveText_ThereIsADifferentFile : gSaveText_ThereIsAlreadyAFile,
      SaveDialogCB_DisplayOverwriteYesNoMenu);
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DisplayOverwriteYesNoMenu(void)
{
    DisplayYesNoMenu(20, 8, 1);
    dialogCallback = SaveDialogCB_ProcessOverwriteYesNoMenu;
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ProcessOverwriteYesNoMenu(void)
{
    switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
        HideSaveDialog();
        dialogCallback = SaveDialogCB_DisplaySavingMessage;
        break;
    case -1:    //B button
    case 1:     //NO
        HideSaveDialog();
        sub_8071700();
        return SAVE_CANCELED;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DisplaySavingMessage(void)
{
    //"SAVING... DON'T TURN OFF THE POWER."
    DisplaySaveMessageWithCallback(gSaveText_DontTurnOff, SaveDialogCB_DoSave);
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DoSave(void)
{
    u8 saveStatus;

    IncrementGameStat(GAME_STAT_SAVED_GAME);
    if (gDifferentSaveFile == TRUE)
    {
        saveStatus = Save_WriteData(SAVE_OVERWRITE_DIFFERENT_FILE);
        gDifferentSaveFile = FALSE;
    }
    else
    {
        saveStatus = Save_WriteData(SAVE_NORMAL);
    }

    if (saveStatus == SAVE_STATUS_OK)
    {
        //"(Player) saved the game."
        DisplaySaveMessageWithCallback(gSaveText_PlayerSavedTheGame, SaveDialogCB_SaveSuccess);
    }
    else
    {
        //"Save error. Please exchange the backup memory."
        DisplaySaveMessageWithCallback(gSystemText_SaveErrorExchangeBackup, SaveDialogCB_SaveError);
    }

    SaveDialogStartTimeout();
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_SaveSuccess(void)
{
    if (Menu_UpdateWindowText())
    {
        PlaySE(SE_SAVE);
        dialogCallback = SaveDialogCB_ReturnSuccess;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ReturnSuccess(void)
{
    if (!IsSEPlaying() && SaveDialogCheckForTimeoutOrKeypress())
    {
        sub_8071700();
        return SAVE_SUCCESS;
    }
    else
        return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_SaveError(void)
{
    if (Menu_UpdateWindowText())
    {
        PlaySE(SE_BOO);
        dialogCallback = SaveDialogCB_ReturnError;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ReturnError(void)
{
    if (!SaveDialogCheckForTimeoutAndKeypress())
        return SAVE_IN_PROGRESS;
    else
    {
        sub_8071700();
        return SAVE_ERROR;
    }
}

static void sub_80719F0(void)
{
    TransferPlttBuffer();
}

static bool32 sub_80719FC(u8 *step)
{
    switch (*step)
    {
    case 0:
        REG_DISPCNT = 0;
        SetVBlankCallback(NULL);
        ScanlineEffect_Stop();
        DmaClear16(3, PLTT, PLTT_SIZE);
        DmaFill16Large(3, 0, (void *)(VRAM + 0x0), 0x18000, 0x1000);
        break;
    case 1:
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        ScanlineEffect_Clear();
        break;
    case 2:
        Text_LoadWindowTemplate(&gMenuTextWindowTemplate);
        InitMenuWindow(&gMenuTextWindowTemplate);
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_BG0_ON;
        break;
    case 3:
    {
        u32 savedIme;

        BlendPalettes(0xFFFFFFFF, 16, RGB(0, 0, 0));
        SetVBlankCallback(sub_80719F0);
        savedIme = REG_IME;
        REG_IME = 0;
        REG_IE |= 1;
        REG_IME = savedIme;
        break;
    }
    case 4:
        return TRUE;
    }
    (*step)++;
    return FALSE;
}

void sub_8071B28(void)
{
    if (sub_80719FC(&gMain.state))
    {
        CreateTask(Task_8071B64, 0x50);
        SetMainCallback2(sub_8071B54);
    }
}

static void sub_8071B54(void)
{
    RunTasks();
    UpdatePaletteFade();
}

static void Task_8071B64(u8 taskId)
{
    s16 *step = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        switch (*step)
        {
        case 0:
            Menu_DisplayDialogueFrame();
            Menu_PrintText(gSystemText_Saving, 2, 15);
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
            (*step)++;
            break;
        case 1:
            SetSecretBase2Field_9_AndHideBG();
            sub_8125E2C();
            (*step)++;
            break;
        case 2:
            if (!sub_8125E6C())
                break;
            ClearSecretBase2Field_9_2();
            (*step)++;
            break;
        case 3:
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
            (*step)++;
            break;
        case 4:
            SetMainCallback2(gMain.savedCallback);
            DestroyTask(taskId);
            break;
        }
    }
}

//=================================================================================================
//WAIT FUNCTIONS
//=================================================================================================

static u8 WaitCallback1(void)
{
    dialogCallback = WaitDialogCB_TryToWait;
    gMenuCallback = WaitCallback2;
    return FALSE;
}

static u8 WaitCallback2(void)
{
    switch (RunWaitDialogCallback())
    {
    case WAIT_IN_PROGRESS:
        return FALSE;
    case WAIT_CANT:
        //Go back to start menu
        Menu_EraseScreen();
        InitStartMenu();
        gMenuCallback = StartMenu_InputProcessCallback;
        return FALSE;
    }
    return FALSE;
}

static u8 RunWaitDialogCallback(void)
{
    if (!Menu_UpdateWindowText())
        return 0;
    return dialogCallback();
}

static void DisplayWaitMessageWithCallback(const u8 *ptr, u8 (*func)(void))
{
    StringExpandPlaceholders(gStringVar4, ptr);
    Menu_DisplayDialogueFrame();
    MenuPrintMessageDefaultCoords(gStringVar4);
    dialogCallback = func;
}

static u8 WaitMenu_InputProcessCallback(void)
{
    if (gMain.newKeys & DPAD_UP)
    {
		sWaitMenuCursorPosBuffer = sWaitMenuCursorPos;
		sWaitMenuCursorPos = Menu_MoveCursor(-1);
		if (sWaitMenuCursorPosBuffer != sWaitMenuCursorPos)
			PlaySE(SE_SELECT);
    }
    if (gMain.newKeys & DPAD_DOWN)
    {
		sWaitMenuCursorPosBuffer = sWaitMenuCursorPos;
		sWaitMenuCursorPos = Menu_MoveCursor(1);
		if (sWaitMenuCursorPosBuffer != sWaitMenuCursorPos)
			PlaySE(SE_SELECT);
    }
    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
		if (gWaitTimeLookup[sWaitMenuCursorPos] > gSaveBlock2.waitTime)
		{
			Menu_DestroyCursor();
			Menu_EraseScreen();
			DisplayWaitMessageWithCallback(gWaitText_NotEnoughTime, WaitDialogCB_DisplayConfirmYesNoMenu);
			return WAIT_IN_PROGRESS;
		}
        gMenuCallback = sWaitMenuItems[sWaitMenuCursorPos].func;
        return 0;
    }
    if (gMain.newKeys & B_BUTTON)
    {
		WaitMenu_Close();
        return 0;
    }
    return 0;
}

static u8 WaitMenu_DoWaitCallback(void)
{
	int i;
	
	//palette fade goes here
	
	gSaveBlock2.timeSeconds = 0; //resets seconds to 0
	gSaveBlock2.timeMinute = 0; //resets minutes to 0
	
	for (i = gWaitTimeLookup[sWaitMenuCursorPos]; i != 0; i--)
	{
		gSaveBlock2.waitTime--;
		IncrementClockHour(FALSE);
		
		if (gSaveBlock2.waitTime == 0)
			break;
	}
	
	WaitMenu_ResetCursor();
	gMenuCallback = StartMenu_ExitCallback;
    return;
}

static u8 WaitMenu_CancelCallback(void)
{
	WaitMenu_Close();
    return;
}

static void WaitMenu_Close(void)
{
	Menu_EraseScreen();
	InitStartMenu();
    gMenuCallback = StartMenu_InputProcessCallback;
}

static void WaitMenu_ResetCursor(void)
{
	sWaitMenuCursorPos = 0;
	sWaitMenuCursorPosBuffer = 0;
}

static u8 WaitDialogCB_DoWaitMenu(void)
{
	int i;
	
	StringCopy(gStringVar1, gHourLookup[gSaveBlock2.timeHour]);
	AlignInt2InMenuWindow(gStringVar2, gSaveBlock2.waitTime, 12, 0);
	
	if (gSaveBlock2.waitTime >= 10) //does there need to be a space added?
		StringAppend(gStringVar2, gWaitText_Space); //space
		
	StringAppend(gStringVar2, gWaitText_Hour);
	
	if (gSaveBlock2.waitTime != 1) //does there need to be an S added?
		StringAppend(gStringVar2, gWaitText_S); //S
	
	Menu_DrawStdWindowFrame(22, 0, 29, 11);
	Menu_DrawStdWindowFrame(0, 0, 8, 5);
	Menu_PrintText(gWaitText_HoursRemaining, 1, 1);
	
	for (i = 0; i < 5; i++)
	{
		Menu_PrintText(sWaitMenuItems[i].text, 23, 1 + i * 2);
	}
	
	sWaitMenuCursorPos = InitMenu(0, 0x17, 1, 5, 0, 6);
	
	dialogCallback = WaitMenu_InputProcessCallback;
	return WAIT_IN_PROGRESS;
}

static u8 WaitDialogCB_TryToWait(void)
{
    Menu_EraseScreen();
	switch(gSaveBlock2.waitStatus)
	{
	case WAIT_ABLE:
		if (gSaveBlock2.waitTime == 0) //no wait time left
			DisplayWaitMessageWithCallback(gWaitText_WaitingTooOften, WaitDialogCB_CantWait);
		else
			DisplayWaitMessageWithCallback(gWaitText_HowLongToWait, WaitDialogCB_DoWaitMenu);
		break;
	default:
	case WAIT_UNABLE:
		DisplayWaitMessageWithCallback(gWaitText_YouCantWaitNow, WaitDialogCB_CantWait);
		break;
	}
}

static u8 WaitDialogCB_CantWait(void) //returns to start menu
{
	if (gMain.newKeys & (A_BUTTON | B_BUTTON))
		return WAIT_CANT;
}

static u8 WaitDialogCB_DisplayConfirmYesNoMenu(void)
{
    DisplayYesNoMenu(20, 8, 1);
    dialogCallback = WaitDialogCB_ProcessConfirmYesNoMenu;
    return WAIT_IN_PROGRESS;
}

static u8 WaitDialogCB_ProcessConfirmYesNoMenu(void)
{
    switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
		WaitMenu_DoWaitCallback();
        break;
    case -1:    //B button
    case 1:     //NO
        return WAIT_CANT;
    }
    return WAIT_IN_PROGRESS;
}