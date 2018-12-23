#include "global.h"
#include "option_menu.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_game.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "constants/songs.h"
#include "sprite.h"
#include "strings2.h"
#include "task.h"

extern void SetPokemonCryStereo(u32 val);

// Menu items
enum
{
	MENUITEM_BIKEMODE,
	MENUITEM_FULLPARTY,
	MENUITEM_BATTLESTYLE,
	MENUITEM_BATTLESCENE,
	MENUITEM_QUICKFLEE,
	MENUITEM_KEYBOARD,
	MENUITEM_FONT,
    MENUITEM_FRAME,
	MENUITEM_LOWHPSOUND,
	MENUITEM_KEYPADSOUND,
	MENUITEM_SOUNDOUTPUT,
	MENUITEM_RESETDEFAULTS,
	MENUITEM_LOWERNUZLOCKE,
	MENUITEM_FREEZENUZLOCKE,
    MENUITEM_CLOSEMENU,
};

const u8 *gMainOptionsList[] = { gSystemText_BikeMode, gSystemText_FullParty, gSystemText_BattleStyle, gSystemText_BattleScene, gSystemText_QuickFlee, gSystemText_Keyboard, gSystemText_Font, gSystemText_Frame, gSystemText_LowHPSound, gSystemText_KeypadSound, gSystemText_Sound, gSystemText_ResetDefaults, gSystemText_DisableNuzlockeMode, gSystemText_FreezeNuzlockeMode, gSystemText_Cancel };
const u8 *gOptionsDescriptionList[] = { gSystemText_BikeModeDescription, gSystemText_FullPartyDescription, gSystemText_BattleStyleDescription, gSystemText_BattleSceneDescription, gSystemText_QuickFleeDescription, gSystemText_KeyboardDescription, gSystemText_FontDescription, gSystemText_FrameDescription, gSystemText_LowHPSoundDescription, gSystemText_KeypadSoundDescription, gSystemText_SoundOutputDescription, gSystemText_ResetDefaultsDescription, gSystemText_DisableNuzlockeModeDescription, gSystemText_FreezeNuzlockeModeDescription, gSystemText_CancelDescription  };

extern const u8 gSystemText_DowngradeToHardlockeMode[];
extern const u8 gSystemText_DowngradeToNuzlockeMode[];
extern const u8 gSystemText_DowngradeToHardlockeModeDescription[];
extern const u8 gSystemText_DowngradeToNuzlockeModeDescription[];
extern const u8 gSystemText_AreYouSure[];

const u8 *gKeyboardNameList[] = { gSystemText_QWERTY, gSystemText_QWERTYPlus, gSystemText_ABC, gSystemText_ABCPlus, gSystemText_AZERTY, gSystemText_AZERTYPlus, gSystemText_Dvorak, gSystemText_DvorakPlus, gSystemText_Colemak, gSystemText_ColemakPlus, gSystemText_Vanilla };
const u8 *gFontNameList[] = { gSystemText_Rocket, gSystemText_Magma, gSystemText_Aqua, gSystemText_Galactic };

// Task data
#define tMenuSelection  data[0]
#define tMenuScroll     data[1]
#define tMenuInput      data[2] //blocks option menu input if true (for handling yes/no boxes)
#define tOptBikeMode    data[3]
#define tOptFullParty   data[4]
#define tOptBattleStyle data[5]
#define tOptBattleScene	data[6]
#define tOptQuickFlee	data[7]
#define tOptKeyboard    data[8]
#define tOptFont        data[9]
#define tOptFrameType  	data[10]
#define tOptLowHPSound	data[11]
#define tOptKeypadSound	data[12]
#define tOptSoundOutput data[13]

const u16 gUnknown_0839F5FC[] = INCBIN_U16("graphics/misc/option_menu_text.gbapal");
// note: this is only used in the Japanese release
const u8 gUnknown_0839F63C[] = INCBIN_U8("graphics/misc/option_menu_equals_sign.4bpp");

static void RedrawDescriptionAndHighlight(u8 taskId);
static void DisplayConfirmMessage(void);
static void Task_ProcessYesNoMenuNuzlocke(u8 taskId);
static void Task_ProcessYesNoMenuFreeze(u8 taskId);
static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void CopyOptionsToTask(u8 taskId);
static u8 CalcMenuShift(u8 taskId);
static void PrintOptions(u8 taskId);
static void BlankChoice(u8 taskId);
static void HighlightOptionMenuItem(u8 selection);
static void CreateScrollArrows(void);
static void DestroyScrollArrows(void);

static void OnOff_DrawChoices(u8 selection, int yPos);
static u8   OnOff_ProcessInput(u8 selection, u8 taskId);
static void BattleStyle_DrawChoices(u8 selection, int yPos);
static u8   BattleStyle_ProcessInput(u8 selection, u8 taskId);
static void Sound_DrawChoices(u8 selection, int yPos);
static u8   Sound_ProcessInput(u8 selection, u8 taskId);
static void BikeMode_DrawChoices(u8 selection, int yPos);
static u8   BikeMode_ProcessInput(u8 selection, u8 taskId);
static void FullParty_DrawChoices(u8 selection, int yPos);
static u8   FullParty_ProcessInput(u8 selection, u8 taskId);
static void Keyboard_DrawChoices(u8 selection, int yPos);
static u8   Keyboard_ProcessInput(u8 selection, u8 taskId);
static void Font_DrawChoices(u8 selection, int yPos);
static u8   Font_ProcessInput(u8 selection, u8 taskId);
static void FrameType_DrawChoices(u8 selection, int yPos);
static u8   FrameType_ProcessInput(u8 selection, u8 taskId);

static void MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitOptionMenu(void)
{
	switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        REG_DISPCNT = 0;
        REG_BG2CNT = 0;
        REG_BG1CNT = 0;
        REG_BG0CNT = 0;
        REG_BG2HOFS = 0;
        REG_BG2VOFS = 0;
        REG_BG1HOFS = 0;
        REG_BG1VOFS = 0;
        REG_BG0HOFS = 0;
        REG_BG0VOFS = 0;
        DmaFill16Large(3, 0, (u8 *)VRAM, 0x18000, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        gMain.state++;
        break;
    case 1:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 2:
        Text_LoadWindowTemplate(&gWindowTemplate_81E71B4);
        gMain.state++;
        break;
    case 3:
        MultistepInitMenuWindowBegin(&gWindowTemplate_81E71B4);
        gMain.state++;
        break;
    case 4:
        if (!MultistepInitMenuWindowContinue())
            return;
        gMain.state++;
        break;
    case 5:
        LoadPalette(gUnknown_0839F5FC, 0x80, 0x40);
        CpuCopy16(gUnknown_0839F63C, (void *)0x0600BEE0, 0x40);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
        gMain.state++;
        break;
    case 7:
    {
        u16 savedIme;

        REG_WIN0H = 0;
        REG_WIN0V = 0;
        REG_WIN1H = 0;
        REG_WIN1V = 0;
        REG_WININ = 0x1111;
        REG_WINOUT = 0x31;
        REG_BLDCNT = 0xE1;
        REG_BLDALPHA = 0;
        REG_BLDY = 7;
        savedIme = REG_IME;
        REG_IME = 0;
        REG_IE |= INTR_FLAG_VBLANK;
        REG_IME = savedIme;
        REG_DISPSTAT |= DISPSTAT_VBLANK_INTR;
        SetVBlankCallback(VBlankCB);
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON |
          DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
        gMain.state++;
        break;
    }
    case 8:
    {
        u8 taskId = CreateTask(Task_OptionMenuFadeIn, 0);

        gTasks[taskId].tMenuSelection = 0;
		gTasks[taskId].tMenuScroll = 0;
		CopyOptionsToTask(taskId);

        Menu_DrawStdWindowFrame(1, 0, 28, 13);  // options list box
		Menu_DrawStdWindowFrame(1, 14, 28, 19); // description box
		
		Menu_PrintText(gSystemText_BikeModeDescription,  2,  15); //always opens on bike mode

		PrintOptions(taskId);
		
        REG_WIN0H = WIN_RANGE(9, 231);
        REG_WIN0V = WIN_RANGE(113, 159);

        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
        gMain.state++;
        break;
    }
    case 9:
        SetMainCallback2(MainCB);
        return;
    }
}

static void CopyOptionsToTask(u8 taskId)
{
	gTasks[taskId].tOptBikeMode	   = gSaveBlock2.optionsBikeMode;
	gTasks[taskId].tOptFullParty   = gSaveBlock2.optionsFullParty;
	gTasks[taskId].tOptBattleStyle = gSaveBlock2.optionsBattleStyle;
	gTasks[taskId].tOptBattleScene = gSaveBlock2.optionsBattleSceneOff;
	gTasks[taskId].tOptQuickFlee   = gSaveBlock2.optionsQuickFlee;
	gTasks[taskId].tOptKeyboard    = gSaveBlock2.optionsKeyboard;
	gTasks[taskId].tOptFont		   = gSaveBlock2.optionsFont;
	gTasks[taskId].tOptFrameType   = gSaveBlock2.optionsWindowFrameType;
	gTasks[taskId].tOptLowHPSound  = gSaveBlock2.optionsLowHPSound;
	gTasks[taskId].tOptKeypadSound = gSaveBlock2.optionsKeypadSound;
	gTasks[taskId].tOptSoundOutput = gSaveBlock2.optionsSound;
}

static u8 CalcMenuShift(u8 taskId)
{
	if (gTasks[taskId].tMenuSelection + gTasks[taskId].tMenuScroll > MENUITEM_RESETDEFAULTS)
	{
		if (gSaveBlock2.nuzlockeMode == NUZLOCKE_MODE_OFF || gSaveBlock2.freezeNuzlocke == TRUE)
			return 2; //no nuzlocke options
		else
			return 0;
	}
	else
		return 0;
}

static void PrintOptions(u8 taskId)
{
	int i, j;
	
	Menu_BlankWindowRect(2, 1, 27, 12); //clears all options on screen
	
	for (i = 0; i < 6; i++) // 6 options on screen at once
	{
		//different from CalcMenuShift
		if (gTasks[taskId].tMenuScroll + i > MENUITEM_RESETDEFAULTS)
		{
			if (gSaveBlock2.nuzlockeMode == NUZLOCKE_MODE_OFF || gSaveBlock2.freezeNuzlocke == TRUE)
				j = 2;
			else
				j = 0;
		}
		else
			j = 0;
		
		if (gMainOptionsList[i + j + gTasks[taskId].tMenuScroll] == gSystemText_DisableNuzlockeMode)
		{
			switch (gSaveBlock2.nuzlockeMode)
			{
				case NUZLOCKE_MODE_DEADLOCKE:
					Menu_PrintText(gSystemText_DowngradeToHardlockeMode,   2, (i * 2) +  1);
					break;
				case NUZLOCKE_MODE_HARDLOCKE:
					Menu_PrintText(gSystemText_DowngradeToNuzlockeMode,   2, (i * 2) +  1);
					break;
				case NUZLOCKE_MODE_NUZLOCKE:
					Menu_PrintText(gSystemText_DisableNuzlockeMode,   2, (i * 2) +  1);
					break;
			}
		}
		else
			Menu_PrintText(gMainOptionsList[i + j + gTasks[taskId].tMenuScroll],   2, (i * 2) +  1);
		
		switch (i + gTasks[taskId].tMenuScroll)
		{
			case 0: //BIKE MODE
				BikeMode_DrawChoices(gTasks[taskId].tOptBikeMode, i * 16);
				break;
				
			case 1: //FULL PARTY
				FullParty_DrawChoices(gTasks[taskId].tOptFullParty, i * 16);
				break;
				
			case 2: //BATTLE STYLE
				BattleStyle_DrawChoices(gTasks[taskId].tOptBattleStyle, i * 16);
				break;
				
			case 3: //BATTLE SCENE
				OnOff_DrawChoices(gTasks[taskId].tOptBattleScene, i * 16);
				break;
				
			case 4: //QUICK FLEE
				OnOff_DrawChoices(gTasks[taskId].tOptQuickFlee, i * 16);
				break;	
				
			case 5: //KEYBOARD
				Keyboard_DrawChoices(gTasks[taskId].tOptKeyboard, (i * 2) + 1);
				break;
				
			case 6: //FONT
				Font_DrawChoices(gTasks[taskId].tOptFont, (i * 2) + 1);
				break;
				
			case 7: //FRAME
				FrameType_DrawChoices(gTasks[taskId].tOptFrameType, (i * 2) + 1);
				break;
				
			case 8: //LOW HP SOUND
				OnOff_DrawChoices(gTasks[taskId].tOptLowHPSound, i * 16);
				break;
				
			case 9: //KEYPAD SOUND
				OnOff_DrawChoices(gTasks[taskId].tOptKeypadSound, i * 16);
				break;
				
			case 10: //SOUND OUTPUT
				Sound_DrawChoices(gTasks[taskId].tOptSoundOutput, i * 16);
				break;
		}
	}
}

static void RedrawDescriptionAndHighlight(u8 taskId)
{
	HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
	Menu_BlankWindowRect(2, 15, 27, 18);
	
	if (gOptionsDescriptionList[gTasks[taskId].tMenuSelection + gTasks[taskId].tMenuScroll + CalcMenuShift(taskId)] == gSystemText_DisableNuzlockeModeDescription)
	{
		switch (gSaveBlock2.nuzlockeMode)
		{
			case NUZLOCKE_MODE_DEADLOCKE:
				Menu_PrintText(gSystemText_DowngradeToHardlockeModeDescription,  2,  15);
				break;
			case NUZLOCKE_MODE_HARDLOCKE:
				Menu_PrintText(gSystemText_DowngradeToNuzlockeModeDescription,  2,  15);
				break;
			case NUZLOCKE_MODE_NUZLOCKE:
				Menu_PrintText(gSystemText_DisableNuzlockeModeDescription,  2,  15);
				break;
		}
	}
	else
		Menu_PrintText(gOptionsDescriptionList[gTasks[taskId].tMenuSelection + gTasks[taskId].tMenuScroll + CalcMenuShift(taskId)],  2,  15);
}

static void DisplayConfirmMessage(void)
{
	Menu_BlankWindowRect(2, 15, 27, 18);
	Menu_DrawStdWindowFrame(1, 14, 21, 19);
	Menu_PrintText(gSystemText_AreYouSure,  2,  15);
	
	DisplayYesNoMenu(22, 14, 1);
}

static void Task_ProcessYesNoMenuNuzlocke(u8 taskId)
{
	switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
		if (gSaveBlock2.nuzlockeMode != NUZLOCKE_MODE_OFF)
			gSaveBlock2.nuzlockeMode--;
		if (gSaveBlock2.nuzlockeMode == NUZLOCKE_MODE_OFF)
		{
			gTasks[taskId].tMenuScroll -= 2;
			gTasks[taskId].tMenuSelection = 5;
		}
    case -1:    //B button
    case 1:     //NO
		PrintOptions(taskId);
		Menu_DrawStdWindowFrame(1, 14, 28, 19);
		RedrawDescriptionAndHighlight(taskId);
		PlaySE(SE_SELECT);
		gTasks[taskId].tMenuInput = FALSE;
		gTasks[taskId].func = Task_OptionMenuProcessInput;
    }
}

static void Task_ProcessYesNoMenuFreeze(u8 taskId)
{
	switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
		gSaveBlock2.freezeNuzlocke = TRUE;
		gTasks[taskId].tMenuScroll -= 2;
		gTasks[taskId].tMenuSelection = 5;
    case -1:    //B button
    case 1:     //NO
		PrintOptions(taskId);
		Menu_DrawStdWindowFrame(1, 14, 28, 19);
		RedrawDescriptionAndHighlight(taskId);
		PlaySE(SE_SELECT);
		gTasks[taskId].tMenuInput = FALSE;
		gTasks[taskId].func = Task_OptionMenuProcessInput;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
	{
        gTasks[taskId].func = Task_OptionMenuProcessInput;
		CreateScrollArrows();
	}
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
	int i;
	
	if (gTasks[taskId].tMenuInput)
		return;
	
    if (gMain.newKeys & A_BUTTON)
    {
		switch (gTasks[taskId].tMenuSelection + gTasks[taskId].tMenuScroll + CalcMenuShift(taskId))
		{
			case MENUITEM_RESETDEFAULTS:
				SetDefaultOptions();
				CopyOptionsToTask(taskId);
				Menu_LoadStdFrameGraphicsOverrideStyle(gTasks[taskId].tOptFrameType);
				PrintOptions(taskId);
				RedrawDescriptionAndHighlight(taskId);
				PlaySE(SE_SELECT);			
				break;
			case MENUITEM_LOWERNUZLOCKE:
				gTasks[taskId].tMenuInput = TRUE;
				DisplayConfirmMessage();
				gTasks[taskId].func = Task_ProcessYesNoMenuNuzlocke;
				break;
			case MENUITEM_FREEZENUZLOCKE:
				gTasks[taskId].tMenuInput = TRUE;
				DisplayConfirmMessage();
				gTasks[taskId].func = Task_ProcessYesNoMenuFreeze;
				break;
			case MENUITEM_CLOSEMENU:
				PlaySE(SE_SELECT);
				gTasks[taskId].func = Task_OptionMenuSave;
				break;
		}
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (gMain.newKeys & DPAD_UP)
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else if (gTasks[taskId].tMenuSelection + gTasks[taskId].tMenuScroll != MENUITEM_BIKEMODE)
		{
            gTasks[taskId].tMenuScroll--;
			PrintOptions(taskId);
		}
        RedrawDescriptionAndHighlight(taskId);
    }
    else if (gMain.newKeys & DPAD_DOWN)
    {		
        if (gTasks[taskId].tMenuSelection < 5)
            gTasks[taskId].tMenuSelection++;
        else if (gTasks[taskId].tMenuSelection + gTasks[taskId].tMenuScroll + CalcMenuShift(taskId) != MENUITEM_CLOSEMENU)
		{
            gTasks[taskId].tMenuScroll++;
			PrintOptions(taskId);
		}
        RedrawDescriptionAndHighlight(taskId);
    }
    else
    {
		if (gSaveBlock2.nuzlockeMode == NUZLOCKE_MODE_OFF || gSaveBlock2.freezeNuzlocke == TRUE)
			i = 2; //no nuzlocke options
		else
			i = 0;
		
		if (gTasks[taskId].tMenuScroll == 0)
			SetVerticalScrollIndicators(TOP_ARROW, INVISIBLE);
		else
			SetVerticalScrollIndicators(TOP_ARROW, VISIBLE);
		
		if (gTasks[taskId].tMenuScroll + i + 5 == MENUITEM_CLOSEMENU) //if last option visible is close menu
			SetVerticalScrollIndicators(BOTTOM_ARROW, INVISIBLE);
		else
			SetVerticalScrollIndicators(BOTTOM_ARROW, VISIBLE);
		
        switch (gTasks[taskId].tMenuSelection + gTasks[taskId].tMenuScroll)
        {
		case MENUITEM_BIKEMODE:
            gTasks[taskId].tOptBikeMode = BikeMode_ProcessInput(gTasks[taskId].tOptBikeMode, taskId);
            BikeMode_DrawChoices(gTasks[taskId].tOptBikeMode, gTasks[taskId].tMenuSelection * 16);
            break;
		case MENUITEM_FULLPARTY:
            gTasks[taskId].tOptFullParty = FullParty_ProcessInput(gTasks[taskId].tOptFullParty, taskId);
            FullParty_DrawChoices(gTasks[taskId].tOptFullParty, gTasks[taskId].tMenuSelection * 16);
            break;
		case MENUITEM_BATTLESTYLE:
            gTasks[taskId].tOptBattleStyle = BattleStyle_ProcessInput(gTasks[taskId].tOptBattleStyle, taskId);
            BattleStyle_DrawChoices(gTasks[taskId].tOptBattleStyle, gTasks[taskId].tMenuSelection * 16);
            break;
        case MENUITEM_BATTLESCENE:
            gTasks[taskId].tOptBattleScene = OnOff_ProcessInput(gTasks[taskId].tOptBattleScene, taskId);
            OnOff_DrawChoices(gTasks[taskId].tOptBattleScene, gTasks[taskId].tMenuSelection * 16);
            break;
        case MENUITEM_QUICKFLEE:
            gTasks[taskId].tOptQuickFlee = OnOff_ProcessInput(gTasks[taskId].tOptQuickFlee, taskId);
            OnOff_DrawChoices(gTasks[taskId].tOptQuickFlee, gTasks[taskId].tMenuSelection * 16);
            break;
		case MENUITEM_KEYBOARD:
            gTasks[taskId].tOptKeyboard = Keyboard_ProcessInput(gTasks[taskId].tOptKeyboard, taskId);
            Keyboard_DrawChoices(gTasks[taskId].tOptKeyboard, (gTasks[taskId].tMenuSelection * 2) + 1);
            break;
		case MENUITEM_FONT:
            gTasks[taskId].tOptFont = Font_ProcessInput(gTasks[taskId].tOptFont, taskId);
            Font_DrawChoices(gTasks[taskId].tOptFont, (gTasks[taskId].tMenuSelection * 2) + 1);
            break;
        case MENUITEM_FRAME:
            gTasks[taskId].tOptFrameType = FrameType_ProcessInput(gTasks[taskId].tOptFrameType, taskId);
            FrameType_DrawChoices(gTasks[taskId].tOptFrameType, (gTasks[taskId].tMenuSelection * 2) + 1);
            break;
        case MENUITEM_LOWHPSOUND:
            gTasks[taskId].tOptLowHPSound = OnOff_ProcessInput(gTasks[taskId].tOptLowHPSound, taskId);
            OnOff_DrawChoices(gTasks[taskId].tOptLowHPSound, gTasks[taskId].tMenuSelection * 16);
            break;
        case MENUITEM_KEYPADSOUND:
            gTasks[taskId].tOptKeypadSound = OnOff_ProcessInput(gTasks[taskId].tOptKeypadSound, taskId);
            OnOff_DrawChoices(gTasks[taskId].tOptKeypadSound, gTasks[taskId].tMenuSelection * 16);
            break;
		case MENUITEM_SOUNDOUTPUT:
            gTasks[taskId].tOptSoundOutput = Sound_ProcessInput(gTasks[taskId].tOptSoundOutput, taskId);
            Sound_DrawChoices(gTasks[taskId].tOptSoundOutput, gTasks[taskId].tMenuSelection * 16);
            break;
        }
    }
}

static void Task_OptionMenuSave(u8 taskId)
{
    gSaveBlock2.optionsBikeMode        = gTasks[taskId].tOptBikeMode;
	gSaveBlock2.optionsFullParty       = gTasks[taskId].tOptFullParty;
	gSaveBlock2.optionsBattleStyle     = gTasks[taskId].tOptBattleStyle;
    gSaveBlock2.optionsBattleSceneOff  = gTasks[taskId].tOptBattleScene;
	gSaveBlock2.optionsQuickFlee	   = gTasks[taskId].tOptQuickFlee;
	gSaveBlock2.optionsKeyboard        = gTasks[taskId].tOptKeyboard;
	gSaveBlock2.optionsFont            = gTasks[taskId].tOptFont;
    gSaveBlock2.optionsWindowFrameType = gTasks[taskId].tOptFrameType;
	gSaveBlock2.optionsLowHPSound	   = gTasks[taskId].tOptLowHPSound;
	gSaveBlock2.optionsKeypadSound	   = gTasks[taskId].tOptKeypadSound;	
    gSaveBlock2.optionsSound           = gTasks[taskId].tOptSoundOutput;

    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
		DestroyScrollArrows();
        DestroyTask(taskId);
        SetMainCallback2(gMain.savedCallback);
    }
}

//This version uses addition '+' instead of OR '|'.
#define WIN_RANGE_(a, b) (((a) << 8) + (b))

static void HighlightOptionMenuItem(u8 index)
{
    REG_WIN1H = WIN_RANGE(9, 231);
    REG_WIN1V = WIN_RANGE_(index * 16 + 8, index * 16 + 24);
}

static void CreateScrollArrows(void)
{
	ClearVerticalScrollIndicatorPalettes();
	LoadScrollIndicatorPalette();
	CreateVerticalScrollIndicators(TOP_ARROW, 110, 12);
	SetVerticalScrollIndicators(TOP_ARROW, INVISIBLE); //top arrow is always invisible on loading
	CreateVerticalScrollIndicators(BOTTOM_ARROW, 110, 108);
	SetVerticalScrollIndicatorPriority(TOP_ARROW, 0);
	SetVerticalScrollIndicatorPriority(BOTTOM_ARROW, 0);
}

static void DestroyScrollArrows(void)
{
	DestroyVerticalScrollIndicator(TOP_ARROW);
	DestroyVerticalScrollIndicator(BOTTOM_ARROW);
}

static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[15];
    u16 i;

    for (i = 0; *text != EOS && i < 15; i++)
        dst[i] = *(text++);

    dst[2] = style;
    dst[i] = EOS;
    Menu_PrintTextPixelCoords(dst, x, y, 1);
}

static void BlankChoice(u8 taskId)
{
	Menu_BlankWindowRect(16, gTasks[taskId].tMenuSelection * 2 + 1, 27, gTasks[taskId].tMenuSelection * 2 + 2);
}

static void OnOff_DrawChoices(u8 selection, int yPos)
{
    u8 styles[2];

    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;

    DrawOptionMenuChoice(gSystemText_On,  136, 8 + yPos, styles[0]);
    DrawOptionMenuChoice(gSystemText_Off, 206, 8 + yPos, styles[1]);
}

static u8 OnOff_ProcessInput(u8 selection, u8 taskId)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
	{
        selection ^= 1;
		BlankChoice(taskId);
	}
    return selection;
}

static u8 BattleStyle_ProcessInput(u8 selection, u8 taskId)
{
	if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
	{
		if (gSaveBlock2.nuzlockeMode == NUZLOCKE_MODE_OFF)
		{
			selection ^= 1;
			BlankChoice(taskId);
			return selection;
		}
		else
		{
			Menu_BlankWindowRect(2, 15, 27, 18);
			Menu_PrintText(gSystemText_BattleStyleNuzlockeDescription,  2,  15);
			PlaySE(SE_BOO);
			return 1;
		}
	}
}

#if ENGLISH
#define BATTLESTYLE_SHIFT (136)
#define BATTLESTYLE_SET (206)
#elif GERMAN
#define BATTLESTYLE_SHIFT (120)
#define BATTLESTYLE_SET (178)
#endif

static void BattleStyle_DrawChoices(u8 selection, int yPos)
{
    u8 styles[2];

    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;

    DrawOptionMenuChoice(gSystemText_Shift, BATTLESTYLE_SHIFT, 8 + yPos, styles[0]);
    DrawOptionMenuChoice(gSystemText_Set,   BATTLESTYLE_SET,   8 + yPos, styles[1]);
}

static u8 Sound_ProcessInput(u8 selection, u8 taskId)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
		BlankChoice(taskId);
        SetPokemonCryStereo(selection);
    }
    return selection;
}

static void Sound_DrawChoices(u8 selection, int yPos)
{
    u8 styles[3];

    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;

    DrawOptionMenuChoice(gSystemText_Mono,   136, 8 + yPos, styles[0]);
    DrawOptionMenuChoice(gSystemText_Stereo, 188, 8 + yPos, styles[1]);
}

static u8 BikeMode_ProcessInput(u8 selection, u8 taskId)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        BlankChoice(taskId);
    }
    return selection;
}

static void BikeMode_DrawChoices(u8 selection, int yPos)
{
    u8 styles[3];

    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;

    DrawOptionMenuChoice(gSystemText_HoldB, 136, 8 + yPos, styles[0]);
    DrawOptionMenuChoice(gSystemText_Auto,  200, 8 + yPos, styles[1]);
}

static u8 FullParty_ProcessInput(u8 selection, u8 taskId)
{
	if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
	{
		if (gSaveBlock2.nuzlockeMode != NUZLOCKE_MODE_DEADLOCKE)
		{
			selection ^= 1;
			BlankChoice(taskId);
			return selection;
		}
		else
		{
			Menu_BlankWindowRect(2, 15, 27, 18);
			Menu_PrintText(gSystemText_FullPartyDeadlockeDescription,  2,  15);
			PlaySE(SE_BOO);
			return 0;
		}
	}
}

static void FullParty_DrawChoices(u8 selection, int yPos)
{
    u8 styles[3];
	
    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;

    DrawOptionMenuChoice(gSystemText_Swap,     136, 8 + yPos, styles[0]);
    DrawOptionMenuChoice(gSystemText_SendToPC, 170, 8 + yPos, styles[1]);
}

static u8 Keyboard_ProcessInput(u8 selection, u8 taskId)
{
	if ((gMain.newKeys & DPAD_RIGHT) || (gMain.newKeys & DPAD_LEFT))
	{
		if (gMain.newKeys & DPAD_RIGHT)
		{
			if (selection < 10)
				selection++;
			else
				selection = 0;
		}
		if (gMain.newKeys & DPAD_LEFT)
		{
			if (selection > 0)
				selection--;
			else
				selection = 10;
		}
		BlankChoice(taskId);
	}
    return selection;
}

static void Keyboard_DrawChoices(u8 selection, int yPos)
{
    Menu_PrintText(gKeyboardNameList[selection], 17, yPos);
}

static u8 Font_ProcessInput(u8 selection, u8 taskId)
{
	if ((gMain.newKeys & DPAD_RIGHT) || (gMain.newKeys & DPAD_LEFT))
	{
		if (gMain.newKeys & DPAD_RIGHT)
		{
			if (selection < 3) //4 types of keyboard
				selection++;
			else
				selection = 0;
			//functionToReloadFont(selection);
		}
		if (gMain.newKeys & DPAD_LEFT)
		{
			if (selection > 0)
				selection--;
			else
				selection = 3;
			//functionToReloadFont(selection);
		}
		BlankChoice(taskId);
	}
    return selection;
}

static void Font_DrawChoices(u8 selection, int yPos)
{
    Menu_PrintText(gFontNameList[selection], 17, yPos);
}

static u8 FrameType_ProcessInput(u8 selection, u8 taskId)
{
	if ((gMain.newKeys & DPAD_RIGHT) || (gMain.newKeys & DPAD_LEFT))
	{
		if (gMain.newKeys & DPAD_RIGHT)
		{
			if (selection < 19)
				selection++;
			else
				selection = 0;
			Menu_LoadStdFrameGraphicsOverrideStyle(selection);
		}
		if (gMain.newKeys & DPAD_LEFT)
		{
			if (selection > 0)
				selection--;
			else
				selection = 19;
			Menu_LoadStdFrameGraphicsOverrideStyle(selection);
		}
		BlankChoice(taskId);
	}
    return selection;
}

#define CHAR_0 0xA1 //Character code of '0' character
#define CHAR_LEFT_ARROW 0x7B //Character code of right arrow character
#define CHAR_RIGHT_ARROW 0x7C //Character code of right arrow character

#if ENGLISH
static void FrameType_DrawChoices(u8 selection, int yPos)
{
    u8 text[8];
    u8 n = selection + 1;
    u16 i;

    for (i = 0; gSystemText_Terminator[i] != EOS && i < 6; i++)
        text[i] = gSystemText_Terminator[i];
	
	text[i] = CHAR_LEFT_ARROW;
	i++;
	text[i] = CHAR_SPACE;
	i++;

    //Convert number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
		text[i] = CHAR_SPACE;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = CHAR_SPACE;
        i++;
    }
	
	text[i] = CHAR_RIGHT_ARROW;
	i++;
    text[i] = EOS;
    Menu_PrintText(gSystemText_Type, 17, yPos);
    Menu_PrintText(text, 21, yPos);
}
#elif GERMAN
NAKED
static void FrameType_DrawChoices(u8 taskId)
{
    asm(".syntax unified\n\
    push {r4-r6,lr}\n\
    sub sp, 0x10\n\
    lsls r0, 24\n\
    movs r1, 0x80\n\
    lsls r1, 17\n\
    adds r0, r1\n\
    lsrs r5, r0, 24\n\
    ldr r1, _0808C368 @ =gSystemText_Type\n\
    mov r0, sp\n\
    bl StringCopy\n\
    ldr r1, _0808C36C @ =gSystemText_Terminator\n\
    mov r0, sp\n\
    bl StringAppend\n\
    adds r4, r0, 0\n\
    adds r0, r5, 0\n\
    movs r1, 0xA\n\
    bl __udivsi3\n\
    adds r1, r0, 0\n\
    lsls r0, r1, 24\n\
    lsrs r6, r0, 24\n\
    cmp r6, 0\n\
    beq _0808C370\n\
    adds r0, r1, 0\n\
    adds r0, 0xA1\n\
    strb r0, [r4]\n\
    adds r4, 0x1\n\
    adds r0, r5, 0\n\
    movs r1, 0xA\n\
    bl __umodsi3\n\
    adds r0, 0xA1\n\
    strb r0, [r4]\n\
    b _0808C380\n\
    .align 2, 0\n\
_0808C368: .4byte gSystemText_Type\n\
_0808C36C: .4byte gSystemText_Terminator\n\
_0808C370:\n\
    adds r0, r5, 0\n\
    movs r1, 0xA\n\
    bl __umodsi3\n\
    adds r0, 0xA1\n\
    strb r0, [r4]\n\
    adds r4, 0x1\n\
    strb r6, [r4]\n\
_0808C380:\n\
    adds r4, 0x1\n\
    movs r0, 0xFF\n\
    strb r0, [r4]\n\
    mov r0, sp\n\
    movs r1, 0xF\n\
    movs r2, 0xF\n\
    bl Menu_PrintText\n\
    add sp, 0x10\n\
    pop {r4-r6}\n\
    pop {r0}\n\
    bx r0\n\
    .syntax divided\n");
}
#endif
