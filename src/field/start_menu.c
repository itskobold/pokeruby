#include "global.h"
#include "start_menu.h"
#include "event_data.h"
#include "field_map_obj_helpers.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "item_menu.h"
#include "load_save.h"
#include "main.h"
#include "map_obj_lock.h"
#include "menu.h"
#include "option_menu.h"
#include "palette.h"
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

//Menu actions
enum {
    MENU_ACTION_POKEDEX,
    MENU_ACTION_POKEMON,
    MENU_ACTION_BAG,
    MENU_ACTION_POKENAV,
    MENU_ACTION_PLAYER,
    MENU_ACTION_SAVE,
    MENU_ACTION_OPTION,
    MENU_ACTION_EXIT,
    MENU_ACTION_RETIRE,
    MENU_ACTION_PLAYER_LINK
};

#if DEBUG
static u32 _debugStartMenu_0  __attribute__((unused));
static u32 _debugStartMenu_1 __attribute__((unused));
#endif

static u8 (*saveDialogCallback)(void);
static u8 saveDialogTimer;    //Number of frames to keep the window on screen after save was completed
static bool8 savingComplete;

extern bool8 gDifferentSaveFile;
extern u16 gSaveFileStatus;
extern u16 gSpecialVar_Result;

extern u8 gUnknown_03004860;

u8 (*gMenuCallback)(void);

EWRAM_DATA static u8 sStartMenuCursorPos = 0;
EWRAM_DATA static u8 sNumStartMenuActions = 0;
EWRAM_DATA static u8 sCurrentStartMenuActions[10] = {0};

//Text strings
extern u8 gSaveText_PlayerSavedTheGame[];
extern u8 gSaveText_DontTurnOff[];
extern u8 gSaveText_ThereIsAlreadyAFile[];
extern u8 gSaveText_ThereIsADifferentFile[];
extern u8 gSaveText_WouldYouLikeToSave[];

static u8 StartMenu_PokedexCallback(void);
static u8 StartMenu_PokemonCallback(void);
static u8 StartMenu_BagCallback(void);
static u8 StartMenu_PokenavCallback(void);
static u8 StartMenu_PlayerCallback(void);
static u8 StartMenu_SaveCallback(void);
static u8 StartMenu_OptionCallback(void);
static u8 StartMenu_ExitCallback(void);
static u8 StartMenu_RetireCallback(void);
static u8 StartMenu_PlayerLinkCallback(void);

static const struct MenuAction sStartMenuItems[] =
{
    { SystemText_Pokedex, StartMenu_PokedexCallback },
    { SystemText_Pokemon, StartMenu_PokemonCallback },
    { SystemText_BAG, StartMenu_BagCallback },
    { SystemText_Pokenav, StartMenu_PokenavCallback },
    { SystemText_Player, StartMenu_PlayerCallback },
    { SystemText_Save, StartMenu_SaveCallback },
    { SystemText_Option, StartMenu_OptionCallback },
    { SystemText_Exit, StartMenu_ExitCallback },
    { SystemText_Retire, StartMenu_RetireCallback },
    { SystemText_Player, StartMenu_PlayerLinkCallback },
};

//Private functions
static void BuildStartMenuActions(void);
static void AddStartMenuAction(u8 action);
static void BuildStartMenuActions_Normal(void);
static void BuildStartMenuActions_SafariZone(void);
static void BuildStartMenuActions_Link(void);
static void DisplaySafariBallsWindow(void);
static bool32 PrintStartMenuItemsMultistep(s16 *a, u32 b);
static bool32 InitStartMenuMultistep(s16 *a, s16 *b);
static void Task_StartMenu(u8 taskId);
static u8 StartMenu_InputProcessCallback(void);
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

#if DEBUG
__attribute__((naked))
void debug_sub_8075C30()
{
    asm(
        "	push	{lr}\n"
        "	bl	CloseMenu\n"
        "	bl	debug_sub_8075D9C\n"
        "	mov	r0, #0x1\n"
        "	pop	{r1}\n"
        "	bx	r1\n"
        "\n"
    );
}

__attribute__((naked))
void debug_sub_8075C40()
{
    asm(
        "	push	{r4, r5, r6, r7, lr}\n"
        "	mov	r7, sl\n"
        "	mov	r6, r9\n"
        "	mov	r5, r8\n"
        "	push	{r5, r6, r7}\n"
        "	lsl	r0, r0, #0x18\n"
        "	lsr	r4, r0, #0x18\n"
        "	lsl	r0, r4, #0x2\n"
        "	add	r0, r0, r4\n"
        "	lsl	r0, r0, #0x3\n"
        "	ldr	r1, ._5         @ gTasks\n"
        "	add	r5, r0, r1\n"
        "	mov	r1, #0x0\n"
        "	ldsh	r0, [r5, r1]\n"
        "	cmp	r0, #0x1\n"
        "	beq	._1	@cond_branch\n"
        "	cmp	r0, #0x1\n"
        "	bgt	._2	@cond_branch\n"
        "	cmp	r0, #0\n"
        "	beq	._3	@cond_branch\n"
        "	b	._18\n"
        "._6:\n"
        "	.align	2, 0\n"
        "._5:\n"
        "	.word	gTasks+0x8\n"
        "._2:\n"
        "	cmp	r0, #0x2\n"
        "	beq	._7	@cond_branch\n"
        "	cmp	r0, #0x3\n"
        "	beq	._8	@cond_branch\n"
        "	b	._18\n"
        "._3:\n"
        "	bl	m4aSoundVSyncOff\n"
        "	b	._12\n"
        "._1:\n"
        "	ldr	r1, ._13        @ 0x4000208\n"
        "	ldrh	r0, [r1]\n"
        "	mov	sl, r0\n"
        "	mov	r0, #0x0\n"
        "	strh	r0, [r1]\n"
        "	ldr	r1, ._13 + 4    @ 0x4000108\n"
        "	strh	r0, [r1]\n"
        "	mov	r0, #0x83\n"
        "	lsl	r0, r0, #0x10\n"
        "	str	r0, [r1]\n"
        "	ldr	r4, ._13 + 8    @ 0xc34f\n"
        "	mov	r9, r4\n"
        "	ldr	r2, ._13 + 12   @ 0x40000b0\n"
        "	ldr	r0, ._13 + 16   @ gScanlineEffectRegBuffers\n"
        "	mov	ip, r0\n"
        "	ldr	r1, ._13 + 20   @ 0xc5ff\n"
        "	mov	r8, r1\n"
        "	ldr	r7, ._13 + 24   @ 0x7fff\n"
        "	mov	r3, r9\n"
        "	add	r3, r3, #0x1\n"
        "	ldr	r6, ._13 + 28   @ 0x4000040\n"
        "	ldr	r4, ._13 + 32   @ 0xa2600001\n"
        "._11:\n"
        "	mov	r0, ip\n"
        "	str	r0, [r2]\n"
        "	str	r6, [r2, #0x4]\n"
        "	str	r4, [r2, #0x8]\n"
        "	ldr	r0, [r2, #0x8]\n"
        "	ldrh	r1, [r2, #0xa]\n"
        "	mov	r0, r8\n"
        "	and	r0, r0, r1\n"
        "	strh	r0, [r2, #0xa]\n"
        "	ldrh	r1, [r2, #0xa]\n"
        "	add	r0, r7, #0\n"
        "	and	r0, r0, r1\n"
        "	strh	r0, [r2, #0xa]\n"
        "	ldrh	r0, [r2, #0xa]\n"
        "	sub	r3, r3, #0x1\n"
        "	cmp	r3, #0\n"
        "	bne	._11	@cond_branch\n"
        "	mov	r3, r9\n"
        "	add	r3, r3, #0x1\n"
        "	ldr	r0, ._13 + 36   @ 0x400010a\n"
        "	mov	r2, #0x0\n"
        "	strh	r2, [r0]\n"
        "	ldr	r1, ._13 + 4    @ 0x4000108\n"
        "	ldrh	r0, [r1]\n"
        "	ldr	r4, ._13 + 40   @ _debugStartMenu_0\n"
        "	str	r0, [r4]\n"
        "	strh	r2, [r1]\n"
        "	ldr	r0, ._13        @ 0x4000208\n"
        "	mov	r1, sl\n"
        "	strh	r1, [r0]\n"
        "	ldr	r4, ._13 + 44   @ _debugStartMenu_1\n"
        "	str	r3, [r4]\n"
        "	bl	m4aSoundVSyncOn\n"
        "	b	._12\n"
        "._14:\n"
        "	.align	2, 0\n"
        "._13:\n"
        "	.word	0x4000208\n"
        "	.word	0x4000108\n"
        "	.word	0xc34f\n"
        "	.word	0x40000b0\n"
        "	.word	gScanlineEffectRegBuffers\n"
        "	.word	0xc5ff\n"
        "	.word	0x7fff\n"
        "	.word	0x4000040\n"
        "	.word	0xa2600001\n"
        "	.word	0x400010a\n"
        "	.word	_debugStartMenu_0\n"
        "	.word	_debugStartMenu_1\n"
        "._7:\n"
        "	mov	r0, #0x15\n"
        "	bl	PlaySE\n"
        "	ldr	r0, ._16        @ gStringVar1\n"
        "	ldr	r1, ._16 + 4    @ _debugStartMenu_1\n"
        "	ldr	r1, [r1]\n"
        "	mov	r2, #0x1\n"
        "	mov	r3, #0x8\n"
        "	bl	ConvertIntToDecimalStringN\n"
        "	ldr	r0, ._16 + 8    @ gStringVar2\n"
        "	ldr	r1, ._16 + 12   @ _debugStartMenu_0\n"
        "	ldr	r1, [r1]\n"
        "	mov	r2, #0x1\n"
        "	mov	r3, #0x8\n"
        "	bl	ConvertIntToDecimalStringN\n"
        "	bl	Menu_DisplayDialogueFrame\n"
        "	ldr	r0, ._16 + 16   @ gUnknown_Debug_839B6D8\n"
        "	mov	r1, #0x2\n"
        "	mov	r2, #0xf\n"
        "	bl	Menu_PrintText\n"
        "._12:\n"
        "	ldrh	r0, [r5]\n"
        "	add	r0, r0, #0x1\n"
        "	strh	r0, [r5]\n"
        "	b	._18\n"
        "._17:\n"
        "	.align	2, 0\n"
        "._16:\n"
        "	.word	gStringVar1\n"
        "	.word	_debugStartMenu_1\n"
        "	.word	gStringVar2\n"
        "	.word	_debugStartMenu_0\n"
        "	.word	gUnknown_Debug_839B6D8\n"
        "._8:\n"
        "	ldr	r0, ._19        @ gMain\n"
        "	ldrh	r1, [r0, #0x2e]\n"
        "	mov	r0, #0x1\n"
        "	and	r0, r0, r1\n"
        "	cmp	r0, #0\n"
        "	beq	._18	@cond_branch\n"
        "	bl	Menu_EraseScreen\n"
        "	bl	ScriptContext2_Disable\n"
        "	add	r0, r4, #0\n"
        "	bl	DestroyTask\n"
        "._18:\n"
        "	pop	{r3, r4, r5}\n"
        "	mov	r8, r3\n"
        "	mov	r9, r4\n"
        "	mov	sl, r5\n"
        "	pop	{r4, r5, r6, r7}\n"
        "	pop	{r0}\n"
        "	bx	r0\n"
        "._20:\n"
        "	.align	2, 0\n"
        "._19:\n"
        "	.word	gMain\n"
        "\n"
    );
}

__attribute__((naked))
void debug_sub_8075D9C()
{
    asm(
        "	push	{lr}\n"
        "	ldr	r0, ._21        @ debug_sub_8075C40\n"
        "	mov	r1, #0xa\n"
        "	bl	CreateTask\n"
        "	bl	ScriptContext2_Enable\n"
        "	pop	{r0}\n"
        "	bx	r0\n"
        "._22:\n"
        "	.align	2, 0\n"
        "._21:\n"
        "	.word	debug_sub_8075C40+1\n"
        "\n"
    );
}

__attribute__((naked))
void debug_sub_8075DB4()
{
    asm(
        "	push	{r4, r5, r6, lr}\n"
        "	add	r6, r0, #0\n"
        "	add	r5, r1, #0\n"
        "	add	r4, r2, #0\n"
        "	add	r0, r4, #0\n"
        "	mov	r1, #0x4d\n"
        "	bl	__umodsi3\n"
        "	strb	r0, [r6, #0x1]\n"
        "	add	r1, r6, #0\n"
        "	add	r1, r1, #0xc\n"
        "	add	r0, r4, #0\n"
        "	bl	write_word_to_mem\n"
        "	add	r0, r6, #4\n"
        "	add	r1, r5, #0\n"
        "	bl	StringCopy8\n"
        "	mov	r3, #0x7\n"
        "	mov	r4, #0x0\n"
        "	ldr	r0, ._25        @ gSaveBlock1\n"
        "	ldr	r1, ._25 + 4    @ 0x2b28\n"
        "	add	r5, r0, r1\n"
        "	add	r2, r6, #0\n"
        "	add	r2, r2, #0x10\n"
        "	add	r1, r6, #0\n"
        "	add	r1, r1, #0x1c\n"
        "._23:\n"
        "	ldrh	r0, [r5]\n"
        "	strh	r0, [r2]\n"
        "	strh	r3, [r1]\n"
        "	add	r0, r3, #6\n"
        "	strh	r0, [r1, #0xc]\n"
        "	add	r3, r3, #0x1\n"
        "	add	r5, r5, #0x2\n"
        "	add	r2, r2, #0x2\n"
        "	add	r1, r1, #0x2\n"
        "	add	r4, r4, #0x1\n"
        "	cmp	r4, #0x5\n"
        "	ble	._23	@cond_branch\n"
        "	mov	r4, #0x0\n"
        "._24:\n"
        "	mov	r0, #0x64\n"
        "	mul	r0, r0, r4\n"
        "	ldr	r1, ._25 + 8    @ gPlayerParty\n"
        "	add	r0, r0, r1\n"
        "	mov	r1, #0x2c\n"
        "	mul	r1, r1, r4\n"
        "	add	r1, r1, #0x34\n"
        "	add	r1, r6, r1\n"
        "	bl	sub_803AF78\n"
        "	add	r4, r4, #0x1\n"
        "	cmp	r4, #0x2\n"
        "	ble	._24	@cond_branch\n"
        "	add	r0, r6, #0\n"
        "	bl	SetEReaderTrainerChecksum\n"
        "	pop	{r4, r5, r6}\n"
        "	pop	{r0}\n"
        "	bx	r0\n"
        "._26:\n"
        "	.align	2, 0\n"
        "._25:\n"
        "	.word	gSaveBlock1\n"
        "	.word	0x2b28\n"
        "	.word	gPlayerParty\n"
        "\n"
    );
}

__attribute__((naked))
void unref_sub_8070F90()
{
    asm(
        "	push	{lr}\n"
        "	ldr	r0, ._27        @ 0x801\n"
        "	bl	FlagSet\n"
        "	mov	r0, #0x80\n"
        "	lsl	r0, r0, #0x4\n"
        "	bl	FlagSet\n"
        "	ldr	r0, ._27 + 4    @ 0x802\n"
        "	bl	FlagSet\n"
        "	pop	{r0}\n"
        "	bx	r0\n"
        "._28:\n"
        "	.align	2, 0\n"
        "._27:\n"
        "	.word	0x801\n"
        "	.word	0x802\n"
        "\n"
    );
}
#endif

static void BuildStartMenuActions(void)
{
    sNumStartMenuActions = 0;
    if (is_c1_link_related_active() == TRUE)
        BuildStartMenuActions_Link();
    else
    {
        if (GetSafariZoneFlag() == TRUE)
            BuildStartMenuActions_SafariZone();
        else
            BuildStartMenuActions_Normal();
    }
}

static void AddStartMenuAction(u8 action)
{
    AppendToList(sCurrentStartMenuActions, &sNumStartMenuActions, action);
}

static void BuildStartMenuActions_Normal(void)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
        AddStartMenuAction(MENU_ACTION_POKEDEX);
    if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
        AddStartMenuAction(MENU_ACTION_POKEMON);
    AddStartMenuAction(MENU_ACTION_BAG);
    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
        AddStartMenuAction(MENU_ACTION_POKENAV);
    AddStartMenuAction(MENU_ACTION_PLAYER);
    AddStartMenuAction(MENU_ACTION_SAVE);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}

static void BuildStartMenuActions_SafariZone(void)
{
    AddStartMenuAction(MENU_ACTION_RETIRE);
    AddStartMenuAction(MENU_ACTION_POKEDEX);
    AddStartMenuAction(MENU_ACTION_POKEMON);
    AddStartMenuAction(MENU_ACTION_BAG);
    AddStartMenuAction(MENU_ACTION_PLAYER);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}

static void BuildStartMenuActions_Link(void)
{
    AddStartMenuAction(MENU_ACTION_POKEMON);
    AddStartMenuAction(MENU_ACTION_BAG);
    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
        AddStartMenuAction(MENU_ACTION_POKENAV);
    AddStartMenuAction(MENU_ACTION_PLAYER_LINK);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}

//Show number of safari balls left
static void DisplaySafariBallsWindow(void)
{
    sub_8072C44(gStringVar1, gNumSafariBalls, 12, 1);
    Menu_DrawStdWindowFrame(0, 0, 10, 5);
    Menu_PrintText(gOtherText_SafariStock, 1, 1);
}

//Prints n menu items starting at *index
static bool32 PrintStartMenuItemsMultistep(s16 *index, u32 n)
{
    int _index = *index;

    do
    {
        Menu_PrintText(sStartMenuItems[sCurrentStartMenuActions[_index]].text, 23, 2 + _index * 2);
        _index++;
        if (_index >= sNumStartMenuActions)
        {
            *index = _index;
            return TRUE;
        }
    }
    while (--n != 0);
    *index = _index;
    return FALSE;
}

static bool32 InitStartMenuMultistep(s16 *step, s16 *index)
{
    switch (*step)
    {
    case 1:
        BuildStartMenuActions();
        (*step)++;
        break;
    case 2:
        Menu_DrawStdWindowFrame(22, 0, 29, sNumStartMenuActions * 2 + 3);
        *index = 0;
        (*step)++;
        break;
    case 3:
        if (GetSafariZoneFlag())
            DisplaySafariBallsWindow();
        (*step)++;
        break;
    case 4:
        if (PrintStartMenuItemsMultistep(index, 2))
            (*step)++;
        break;
    case 0:
        (*step)++;
        break;
    case 5:
        sStartMenuCursorPos = InitMenu(0, 0x17, 2, sNumStartMenuActions, sStartMenuCursorPos, 6);
        return TRUE;
    }
    return FALSE;
}

static void InitStartMenu(void)
{
    s16 step = 0;
    s16 index = 0;

    while (InitStartMenuMultistep(&step, &index) == FALSE)
        ;
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

    InitMenuWindow(&gWindowTemplate_81E6CE4);
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
        FreezeMapObjects();
        sub_80594C0();
        sub_80597F4();
    }
    CreateStartMenuTask(sub_80712B4);
    ScriptContext2_Enable();
}

static u8 StartMenu_InputProcessCallback(void)
{
    if (gMain.newKeys & DPAD_UP)
    {
        PlaySE(SE_SELECT);
        sStartMenuCursorPos = Menu_MoveCursor(-1);
    }
    if (gMain.newKeys & DPAD_DOWN)
    {
        PlaySE(SE_SELECT);
        sStartMenuCursorPos = Menu_MoveCursor(1);
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
           gMenuCallback != StartMenu_ExitCallback &&
           gMenuCallback != StartMenu_RetireCallback)
            FadeScreen(1, 0);
        return 0;
    }
    if (gMain.newKeys & (START_BUTTON | B_BUTTON))
    {
        CloseMenu();
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
        TrainerCard_ShowPlayerCard(sub_805469C);
        return 1;
    }
    return 0;
}

//When player selects SAVE
static u8 StartMenu_SaveCallback(void)
{
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
        gMain.savedCallback = sub_805469C;
        return 1;
    }
    return 0;
}

//When player selects EXIT
static u8 StartMenu_ExitCallback(void)
{
    CloseMenu();
    return 1;
}

//When player selects RETIRE
static u8 StartMenu_RetireCallback(void)
{
    CloseMenu();
    SafariZoneRetirePrompt();
    return 1;
}

//When player selects their name in multiplayer mode
static u8 StartMenu_PlayerLinkCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        TrainerCard_ShowLinkCard(gUnknown_03004860, sub_805469C);
        return 1;
    }
    return 0;
}

//Save dialog status
enum
{
    SAVE_IN_PROGRESS,
    SAVE_SUCCESS,
    SAVE_CANCELED,
    SAVE_ERROR,
};

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
        sub_8064E2C();
        ScriptContext2_Disable();
        return TRUE;
    }
    return FALSE;
}

static void sub_807160C(void)
{
    save_serialize_map();
    saveDialogCallback = SaveDialogCB_DisplayConfirmMessage;
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
    return saveDialogCallback();
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
    saveDialogCallback = func;
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
    saveDialogCallback = SaveDialogCB_ProcessConfirmYesNoMenu;
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
                saveDialogCallback = SaveDialogCB_SaveFileExists;
                return SAVE_IN_PROGRESS;
            }
            saveDialogCallback = SaveDialogCB_DisplaySavingMessage;
            return SAVE_IN_PROGRESS;
        default:
            saveDialogCallback = SaveDialogCB_SaveFileExists;
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
    saveDialogCallback = SaveDialogCB_ProcessOverwriteYesNoMenu;
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ProcessOverwriteYesNoMenu(void)
{
    switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
        HideSaveDialog();
        saveDialogCallback = SaveDialogCB_DisplaySavingMessage;
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
        saveDialogCallback = SaveDialogCB_ReturnSuccess;
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
        saveDialogCallback = SaveDialogCB_ReturnError;
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
        Text_LoadWindowTemplate(&gWindowTemplate_81E6CE4);
        InitMenuWindow(&gWindowTemplate_81E6CE4);
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_BG0_ON;
        break;
    case 3:
    {
        u32 savedIme;

        BlendPalettes(-1, 0x10, 0);
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
            BeginNormalPaletteFade(-1, 0, 0x10, 0, 0);
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
            BeginNormalPaletteFade(-1, 0, 0, 0x10, 0);
            (*step)++;
            break;
        case 4:
            SetMainCallback2(gMain.savedCallback);
            DestroyTask(taskId);
            break;
        }
    }
}
