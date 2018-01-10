#include "global.h"
#include "ewram.h"
#include "field_fadetransition.h"
#include "constants/game_stat.h"
#include "m4a.h"
#include "main.h"
#include "menu.h"
#include "menu_cursor.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "roulette.h"
#include "roulette_util.h"
#include "rtc.h"
#include "constants/songs.h"
#include "sound.h"
#include "script.h"
#include "constants/species.h"
#include "sprite.h"
#include "strings2.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "trig.h"
#include "unknown_task.h"

asm(".include \"constants/gba_constants.inc\"");

struct Roulette /* ewram + 0x19000 */
{
    u8 var00;
    u8 var01;
    u8 var02;
    u8 var03_0:5;
    u8 var03_5:1;
    u8 var03_6:1;
    u8 var03_7:1;
    u8 var04_0:2;
    u8 var04_2:5;
    u8 var04_7:1;
    u32 var08;
    u8 var0C[0x6];
    u8 var12[0x4];
    u8 var16[0x3];
    u8 var19;
    u8 var1A_0:4;
    u8 var1A_4:4;
    u8 var1B[0x6];
    u8 var21;
    u8 var22;
    u8 var23;
    s16 var24;
    s16 var26;
    s16 var28;
    s16 var2A;
    struct OamMatrix var2C;
    u16 var34;
    struct Sprite *var38;
    u8 var3C[0x40]; // Sprite IDs
    u8 var7C;
    u8 var7D;
    u8 var7E;
    u8 var7F;
    s16 var80;
    s16 var82;
    s16 var84;
    s16 var86;
    float var88;
    float var8C;
    float var90;
    float var94;
    float var98;
    float var9C;
    float varA0;
    u8 varA4;
    u8 varA5;
    u8 v51[0x2];
    u16 varA8;
    u16 varAA;
    TaskFunc varAC;
    u8 v46[0x4];
    TaskFunc varB4;
    struct UnkStruct0 varB8;
};

struct StructgUnknown_083F8DF4
{
    u8 var00;
    u8 var01;
    u8 var02;
    u8 var03;
    u8 var04;
    u8 v[0x3];
    u16 var08;
    u16 var0A;
    u16 var0C;
    u16 v13[0x1];
    u16 var10;
    u16 var12;
    u16 var14;
    u8 v1[0x2];
    u16 var18;
    u16 var1A;
    float var1C;
};

struct StructgUnknown_083F8C00
{
    u8 var00;
    u8 var01_0:4;
    u8 var01_4:4;
    u8 var02;
    u8 var03;
    u8 var04;
    u8 var05;
    u8 var06;
    u8 var07;
    u32 var08;
    u32 var0C;
    u16 var10;
    u16 var12;
};

struct StructgUnknown_083F8D90
{
    u8 var00;
    u8 var01;
    u8 var02;
    u8 var03;
    u32 var04;
};

static void sub_81150FC(void);
static void sub_8115124(void);
static void sub_8115238(void);
static void sub_8115384(void);
static void sub_8115634(u8);
static void sub_81156BC(u8);
static void sub_8115734(u8);
void sub_811577C(u8);
void sub_81157AC(u8);
static void sub_81157D0(u8);
static void sub_8115928(u8, u8);
static void sub_811597C(u8);
static void sub_81159BC(u8);
static u8 sub_8115A94(s16 *, u8);
void sub_8115B58(u8);
static void sub_8115DA0(u8);
static void sub_8115E14(u8);
static void sub_8115ECC(u8);
u8 sub_8115F58(u16, u16);
static void sub_8116100(u8);
void sub_8116308(u8);
static void sub_811637C(u8);
static void sub_8116474(u8);
static void sub_8116514(u8);
static void sub_811659C(u8);
static void sub_8116638(u8);
static void sub_81166E8(u8);
static void sub_811677C(u8);
static void sub_81167F4(u8);
static void sub_8116880(u8);
static void dp01t_12_3_battle_menu(u8);
static void sub_8116AB0(u8);
static void sub_8116B40(u8);
static void sub_8116BC0(u8);
static void sub_8116C34(u8, TaskFunc, u16, u16);
static void sub_8116CAC(u8);
static void sub_8116CF8(void);
static u8 sub_8116D54(u8, u8);
static u8 sub_8116E5C(u8, u8);
static void sub_8116EF8(u8);
static void sub_8117158(u8);
static u8 sub_8117380(u8);
static void sub_8117434(void);
static void sub_81174F8(u8);
static void sub_8117528(u8);
void sub_811755C(u8);
void sub_81175C0(u8);
static void sub_81175DC(u8);
static void sub_8117630(u8);
static void Task_Roulette_0(u8);
static void sub_8117838(u8);
static u8 sub_8117890(const struct SpriteTemplate *, u8, u16 *);
static void sub_8117900(void);
void unref_sub_8117A74(void);
static void sub_8117AA8(u8, u8);
static void sub_8117BBC(void);
static void sub_8117C60(u8, u8);
static void sub_8117D68(u8);
static void sub_8117DF4(void);
void sub_8117E98(struct Sprite *);
static void sub_8117F2C(void);
static void sub_81180F4(u16);
static u8 sub_81181E8(u8);
static void sub_811829C(u8);
static void sub_81182F8(u8);
void sub_81184CC(struct Sprite *);
static void sub_81184D8(void);
void sub_8118554(struct Sprite *);
static void sub_811857C(void);
static void sub_81185E8(void);
static s16 sub_811866C(struct Sprite *);
static u8 sub_81186B8(struct Sprite *);
static s16 sub_81186E8(struct Sprite *);
static void sub_8118724(struct Sprite *);
static void sub_8118834(struct Sprite *);
static void sub_811889C(struct Sprite *);
static void sub_81189A8(struct Sprite *);
static void sub_8118B30(struct Sprite *);
static void sub_8118BD8(struct Sprite *);
static void sub_8118CAC(struct Sprite *);
void sub_8118CEC(struct Sprite *);
void sub_8118D2C(struct Sprite *);
static void sub_8118DE4(struct Sprite *);
static void sub_8118F8C(struct Sprite *);
static void sub_8119088(struct Sprite *);
static void sub_8119134(struct Sprite *);
static void sub_81191F4(struct Sprite *);
static void sub_8119224(struct Sprite *);
static void sub_81193D4(struct Sprite *);
static void sub_811952C(struct Sprite *);
static void sub_8119780(struct Sprite *);
static void sub_81197D8(struct Sprite *);
static void sub_8119898(struct Sprite *);
void sub_8119964(struct Sprite *);
static void sub_8119A90(struct Sprite *);
static void sub_8119AAC(struct Sprite *);
static void sub_8119B24(struct Sprite *);
static void sub_8119BCC(struct Sprite *);
static void sub_8119D08(struct Sprite *);
void sub_8119D80(struct Sprite *);

extern u8 gUnknown_02019000[];
extern u16 gSpecialVar_0x8004;
extern struct StructgUnknown_083F8DF4 gUnknown_083F8DF4[];
extern u8 gUnknown_083F8EC4;
extern u8 gUnknown_083F8DF0[];
extern u8 gUnknown_083F8E34[];
extern const u8 gUnknown_08E8096C[];
extern const u8 gRouletteWheelTiles[];
extern const u8 gRouletteCenter_Gfx[];
extern const u8 gRouletteHeadersTiles[];
extern const u8 gRouletteCreditTiles[];
extern const u8 gRouletteNumbersTiles[];
extern const u8 gRouletteMultiplierTiles[];
extern const u16 gUnknown_083F86BC[];
extern const u8 gUnknown_083F88BC[];
extern const u8 gUnknown_083F8A60[];
extern u16 gPlttBufferFaded[];
extern u16 gPlttBufferUnfaded[];
extern const u8 gUnknown_081C4157[];
extern const struct YesNoFuncTable gUnknown_083F8EBC;
extern const u8 gUnknown_081C41E3;
extern const u16 gUnknown_083F8ECE;
extern const struct StructgUnknown_083F8C00 gUnknown_083F8C00[];
const extern u8 gUnknown_083F8ECA[];
const extern u8 gUnknown_081C41A5;
const extern u8 gUnknown_081C4199;
const extern u8 gUnknown_081C41AE;
const extern u8 gUnknown_081C41BD;
const extern u8 gUnknown_081C41F1;
const extern u8 gUnknown_081C4231;
const extern u8 gUnknown_081C41D2;
extern const struct StructgUnknown_083F8D90 gUnknown_083F8D90[];
extern const u32 gUnknown_083F8ED8[];
extern const u32 gUnknown_083F8EE8[];
extern const struct UnkStruct1 gUnknown_083F8E9C[];
extern const u8 gUnknown_083F8EF4[];
extern const struct YesNoFuncTable gUnknown_083F8EB4;
extern const u8 gUnknown_081C40DF;
extern const u8 gUnknown_081C4139;
extern const u8 gUnknown_081C411C;
extern const u8 gUnknown_083F92A8[];
extern const u8 gUnknown_083F90FC[];
extern const u8 gUnknown_083F9D3C[];
extern const struct SpritePalette gUnknown_083F9E30;
extern const struct SpriteSheet gUnknown_083F9F54;
extern const struct SpriteSheet gUnknown_083F9F5C;
extern const struct SpriteTemplate gSpriteTemplate_83FA07C[];
extern const struct SpriteTemplate gSpriteTemplate_83F9FD4[];
extern const struct SpriteTemplate gSpriteTemplate_83FA034[];
extern const struct SpriteTemplate gSpriteTemplate_83FA40C;
extern const struct SpriteSheet gUnknown_083F9EE8;
extern const struct SpriteTemplate gSpriteTemplate_83FA0DC[];
extern struct OamMatrix gOamMatrices[];
extern const struct SpriteSheet gUnknown_083FA21C[];
extern const struct SpriteTemplate gSpriteTemplate_83FA2B0;
extern const struct SpriteTemplate gSpriteTemplate_83FA2C8;
extern const struct SpriteTemplate gSpriteTemplate_83FA2E0;
extern const struct SpriteTemplate gSpriteTemplate_83FA2F8;
extern const struct SpriteTemplate gSpriteTemplate_83FA310;
extern const u8 gUnknown_083FA608[0x5];
extern const struct SpriteSheet gUnknown_083FA42C;
extern const struct SpriteTemplate gSpriteTemplate_83FA434;
extern void (*gFieldCallback)(void);
extern struct MusicPlayerInfo gMPlay_SE1;
extern struct MusicPlayerInfo gMPlay_SE2;
extern const u16 gUnknown_083FA60E[0x2][0x2];
extern const struct SpriteTemplate gSpriteTemplate_83FA50C;
extern const struct SpriteTemplate gSpriteTemplate_83FA5C0[];
extern const u16 gUnknown_083FA616[0x2][0x2];
extern const struct SpriteTemplate gSpriteTemplate_83FA524;
extern const struct SpriteTemplate gSpriteTemplate_83FA5F0;
extern const u16 gUnknown_083FA61E[0xA];
extern const u16 gUnknown_083FA632[];
extern const s8 gUnknown_083FA64A[0x2];
extern const s8 gUnknown_083FA64C[0x8][0x2];




void sub_81150FC(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	if (eRoulette->varB8.var00)
	   task_tutorial_controls_fadein(&eRoulette->varB8);
}

void sub_8115124(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    sub_8117434();
    REG_BG1HOFS = 0x200 - eRoulette->var26;
    if (eRoulette->var01)
        REG_BLDALPHA = eRoulette->var34;
    if (eRoulette->var2A != 0)
    {
        DmaCopy16(3, &gBGTilemapBuffers[1][0xE0], (void *)(VRAM + 0x21C0), 0x340);
        eRoulette->var2A = 0x0;
    }
    switch (eRoulette->var28)
    {
    case 0x1:
        REG_BG0CNT = (0x20 * 0xF8);
        DmaCopy16(3, &gBGTilemapBuffers[2][0xE0], (void *)(VRAM + 0xF9C0), 0x340);
        eRoulette->var28 = 0x2;
    break;
    case 0x2:
        DmaCopy16(3, &gBGTilemapBuffers[2][0xE0], (void *)(VRAM + 0xF9C0), 0x340);
    break;
    case 0xFF:
        REG_BG0CNT = 0x1F08;
        DmaFill16(3, 0x0, (void *)(VRAM + 0xF9C0), 0x340);
        eRoulette->var28 = 0x0;
    break;
    case 0x0:
    }
}
#ifdef NONMATCHING

void sub_8115238(void)
{
    u8 i;
    u32 temp;
    struct PlttData t;
    struct PlttData *unfaded;
    struct PlttData *faded;
    struct StructgUnknown_083F8DF4 *s0;
    struct PlttData arr[0x3]; // the third is never used ?
    memcpy(&arr, &gUnknown_083F8EC4, 0x6);
    memset(eRoulette, 0x0, 0x17C);
    eRoulette->var04_0 = (gSpecialVar_0x8004 & 0x1);
    if (gSpecialVar_0x8004 & 0x80)
        eRoulette->var04_7 = 0x1;
    s0 = &gUnknown_083F8DF4[0];
    eRoulette->var22   = s0[eRoulette->var04_0].var03;
    eRoulette->var23   = s0[eRoulette->var04_0].var04;
    temp = gUnknown_083F8DF0[eRoulette->var04_0 + eRoulette->var04_7 * 2];
    eRoulette->var19   = temp;
    eRoulette->var1A_4 = 0x1;
    if (temp == 0x1)
    {
        unfaded = (struct PlttData *)&gPlttBufferUnfaded[0];
        faded = (struct PlttData *)&gPlttBufferFaded[0];
        t = arr[0];
    }
    else
    {
        unfaded = (struct PlttData *)&gPlttBufferUnfaded[0];
        faded = (struct PlttData *)&gPlttBufferFaded[0];
        t = arr[1];
    }
    faded[0x51]   = t;
    faded[0x0]    = faded[0x51];
    unfaded[0x51] = t;
    unfaded[0x0]  = t;
    sub_8124918((&eRoulette->varB8));
    for (i = 0; i < 0xD; i++)
        sub_812492C((&eRoulette->varB8), i, (struct UnkStruct1 *)&gUnknown_083F8E34[i * 8]);
    for (i = 0; i < 0x6; i++)
    {
        switch (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2))
        {
        case SPECIES_TAILLOW:
        eRoulette->var02 |= 0x2;
        break;
        case SPECIES_SHROOMISH:
        eRoulette->var02 |= 0x1;
        break;
        }
    }
    RtcCalcLocalTime();
}
#else
__attribute__((naked))
void sub_8115238(void)
{
    	asm(".syntax unified\n\
        push {r4-r6,lr}\n\
    	sub sp, 0x8\n\
    	ldr r1, _081152C0 @ =gUnknown_083F8EC4\n\
    	mov r0, sp\n\
    	movs r2, 0x6\n\
    	bl memcpy\n\
    	ldr r5, _081152C4 @ =0x02019000\n\
    	movs r2, 0xBE\n\
    	lsls r2, 1\n\
    	adds r0, r5, 0\n\
    	movs r1, 0\n\
    	bl memset\n\
    	ldr r0, _081152C8 @ =gSpecialVar_0x8004\n\
    	ldrh r2, [r0]\n\
    	movs r0, 0x1\n\
    	ands r0, r2\n\
    	ldrb r1, [r5, 0x4]\n\
    	movs r3, 0x4\n\
    	negs r3, r3\n\
    	ands r3, r1\n\
    	orrs r3, r0\n\
    	strb r3, [r5, 0x4]\n\
    	movs r0, 0x80\n\
    	ands r0, r2\n\
    	cmp r0, 0\n\
    	beq _08115276\n\
    	movs r0, 0x80\n\
    	orrs r3, r0\n\
    	strb r3, [r5, 0x4]\n\
    _08115276:\n\
    	ldr r3, _081152CC @ =gUnknown_083F8DF4\n\
    	ldrb r2, [r5, 0x4]\n\
    	lsls r1, r2, 30\n\
    	lsrs r0, r1, 25\n\
    	adds r0, r3\n\
    	ldrb r0, [r0, 0x3]\n\
    	adds r4, r5, 0\n\
    	adds r4, 0x22\n\
    	strb r0, [r4]\n\
    	lsrs r0, r1, 25\n\
    	adds r0, r3\n\
    	ldrb r0, [r0, 0x4]\n\
    	adds r3, r5, 0\n\
    	adds r3, 0x23\n\
    	strb r0, [r3]\n\
    	ldr r0, _081152D0 @ =gUnknown_083F8DF0\n\
    	lsrs r1, 30\n\
    	lsls r2, 24\n\
    	lsrs r2, 31\n\
    	lsls r2, 1\n\
    	adds r1, r2\n\
    	adds r1, r0\n\
    	ldrb r2, [r1]\n\
    	strb r2, [r5, 0x19]\n\
    	ldrb r1, [r5, 0x1A]\n\
    	movs r0, 0xF\n\
    	ands r0, r1\n\
    	movs r1, 0x10\n\
    	orrs r0, r1\n\
    	strb r0, [r5, 0x1A]\n\
    	cmp r2, 0x1\n\
    	bne _081152DC\n\
    	ldr r4, _081152D4 @ =gPlttBufferUnfaded\n\
    	ldr r3, _081152D8 @ =gPlttBufferFaded\n\
    	mov r0, sp\n\
    	ldrh r2, [r0]\n\
    	b _081152E4\n\
    	.align 2, 0\n\
    _081152C0: .4byte gUnknown_083F8EC4\n\
    _081152C4: .4byte 0x02019000\n\
    _081152C8: .4byte gSpecialVar_0x8004\n\
    _081152CC: .4byte gUnknown_083F8DF4\n\
    _081152D0: .4byte gUnknown_083F8DF0\n\
    _081152D4: .4byte gPlttBufferUnfaded\n\
    _081152D8: .4byte gPlttBufferFaded\n\
    _081152DC:\n\
    	ldr r4, _08115348 @ =gPlttBufferUnfaded\n\
    	ldr r3, _0811534C @ =gPlttBufferFaded\n\
    	mov r0, sp\n\
    	ldrh r2, [r0, 0x2]\n\
    _081152E4:\n\
    	adds r0, r3, 0\n\
    	adds r0, 0xA2\n\
    	strh r2, [r0]\n\
    	ldr r1, _08115350 @ =0x0000ffff\n\
    	adds r0, r1, 0\n\
    	ands r0, r2\n\
    	strh r0, [r3]\n\
    	ands r0, r1\n\
    	adds r2, r4, 0\n\
    	adds r2, 0xA2\n\
    	strh r0, [r2]\n\
    	ands r0, r1\n\
    	strh r0, [r4]\n\
    	ldr r0, _08115354 @ =0x020190b8\n\
    	bl sub_8124918\n\
    	movs r4, 0\n\
    	ldr r5, _08115358 @ =gUnknown_083F8E34\n\
    _08115308:\n\
    	lsls r2, r4, 3\n\
    	adds r2, r5\n\
    	ldr r0, _08115354 @ =0x020190b8\n\
    	adds r1, r4, 0\n\
    	bl sub_812492C\n\
    	adds r0, r4, 0x1\n\
    	lsls r0, 24\n\
    	lsrs r4, r0, 24\n\
    	cmp r4, 0xC\n\
    	bls _08115308\n\
    	movs r4, 0\n\
    	ldr r5, _0811535C @ =0x02019000\n\
    _08115322:\n\
    	movs r0, 0x64\n\
    	muls r0, r4\n\
    	ldr r1, _08115360 @ =gPlayerParty\n\
    	adds r0, r1\n\
    	movs r1, 0x41\n\
    	bl GetMonData\n\
    	adds r1, r0, 0\n\
    	movs r0, 0x98\n\
    	lsls r0, 1\n\
    	cmp r1, r0\n\
    	beq _08115364\n\
    	adds r0, 0x2\n\
    	cmp r1, r0\n\
    	bne _0811536C\n\
    	ldrb r0, [r5, 0x2]\n\
    	movs r1, 0x1\n\
    	b _08115368\n\
    	.align 2, 0\n\
    _08115348: .4byte gPlttBufferUnfaded\n\
    _0811534C: .4byte gPlttBufferFaded\n\
    _08115350: .4byte 0x0000ffff\n\
    _08115354: .4byte 0x020190b8\n\
    _08115358: .4byte gUnknown_083F8E34\n\
    _0811535C: .4byte 0x02019000\n\
    _08115360: .4byte gPlayerParty\n\
    _08115364:\n\
    	ldrb r0, [r5, 0x2]\n\
    	movs r1, 0x2\n\
    _08115368:\n\
    	orrs r0, r1\n\
    	strb r0, [r5, 0x2]\n\
    _0811536C:\n\
    	adds r0, r4, 0x1\n\
    	lsls r0, 24\n\
    	lsrs r4, r0, 24\n\
    	cmp r4, 0x5\n\
    	bls _08115322\n\
    	bl RtcCalcLocalTime\n\
    	add sp, 0x8\n\
    	pop {r4-r6}\n\
    	pop {r0}\n\
    	bx r0\n\
        .syntax divided\n");
}
#endif

void sub_8115384(void)
{
    u32 temp_IME;
    u8 taskid;
    switch (gMain.state)
    {
    case 0x0:
        SetVBlankCallback(NULL);
        remove_some_task();
        sub_80F9438();
        sub_80F9368();
        REG_BG2CNT   = 0x4686;
        REG_BG1CNT   = 0x4401;
        REG_BLDCNT   = 0x2400;
        REG_BLDALPHA = 0x060A;
        LZ77UnCompVram(&gUnknown_08E8096C, (void *)VRAM);
        LZ77UnCompVram(&gRouletteWheelTiles, (void *)(VRAM + 0x4000));
        gMain.state++;
        break;
    case 0x1:
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        gMain.state++;
        break;
    case 0x2:
        Text_LoadWindowTemplate(&gWindowTemplate_81E6C3C);
        InitMenuWindow(&gWindowTemplate_81E6CE4);
        LoadPalette(&gUnknown_083F86BC, 0x0, 0x1C0);
        gMain.state++;
        break;
    case 0x3:
        sub_8115238();
        sub_80F9020();
        LZ77UnCompWram(&gUnknown_083F88BC, (void *)(ewram18800));
        LZ77UnCompVram(&gUnknown_083F8A60, (void *)(VRAM + 0x3000));
        gMain.state++;
        break;
    case 0x4:
        sub_8117838(0x0);
        sub_811857C();
        sub_81184D8();
        sub_8117F2C();
        sub_8117900();
        sub_8117BBC();
        sub_8117DF4();
        gMain.state++;
        break;
    case 0x5:
        AnimateSprites();
        BuildOamBuffer();
        sub_81180F4(gSaveBlock1.coins);
        sub_81182F8(0x6);
        sub_811829C(0x0);
        sub_8117158(0x0);
        Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
        Menu_PrintText(&gUnknown_081C4157[0], 0x1, 0xF);
        gSpriteCoordOffsetX = -0x3C;
        gSpriteCoordOffsetY = 0x0;
        gMain.state++;
        break;
    case 0x6:
        REG_DISPCNT = 0x1741;
        gMain.state++;
        break;
    case 0x7:
        temp_IME      = REG_IME;
        REG_IME       = 0x0;      //disable interrupts
        REG_IE       |= 0x1;      //enable VBlank interrupt
        REG_IME       = temp_IME; //reenable interrupts
        REG_DISPSTAT |= 0x8;
        SetVBlankCallback(&sub_8115124);
        BeginHardwarePaletteFade(0xFF, 0x0, 0x10, 0x0 , 0x1);
        taskid = eRoulette->varA4 = CreateTask(&sub_81156BC, 0x0);
        gTasks[taskid].data[0x6] = 0x6;
        gTasks[taskid].data[0xD] = gSaveBlock1.coins;
        eRoulette->varA5 = CreateTask(&sub_8115634, 0x1);
        SetMainCallback2(&sub_81150FC);
        break;
    }
}

void sub_8115634(u8 unused)
{
    s16 sin;
    s16 cos;
    s32 cos32;
    if (eRoulette->var21++ == eRoulette->var23)
    {
        eRoulette->var21 = 0x0;
        if ((eRoulette->var24 -= eRoulette->var22) < 0)
            eRoulette->var24 = 0x168 - eRoulette->var22;
    }
    sin = Sin2(eRoulette->var24);
    cos = Cos2(eRoulette->var24);
    sin = sin / 16;
    cos32 = cos / 16;
    eRoulette->var2C.d =  cos32;
    eRoulette->var2C.a =  cos32;
    eRoulette->var2C.b =  sin;
    eRoulette->var2C.c = -sin;
}

void sub_81156BC(u8 taskid)
{
    if (UpdatePaletteFade() == 0)
    {
        REG_BLDCNT   |= 0x2400;
        REG_BLDALPHA  = 0x0808;
        gTasks[taskid].data[0x6] = 0;
        sub_8116CAC(taskid);
        sub_8116CF8();
        sub_81185E8();
        sub_8117158(0);
        sub_81182F8(0x6);
        sub_8116C34(taskid, &sub_81159BC, 0xFFFF, 0x3);
    }
}

void sub_8115734(u8 taskid)
{
    DisplayYesNoMenu(0x14, 0x8, 0x1);
    sub_814AAF8(0x2D9E);
    Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
    Menu_PrintText(&gUnknown_081C41E3, 0x1, 0xF);
    DoYesNoFuncWithChoice(taskid, &gUnknown_083F8EBC);
}

void sub_811577C(u8 taskid)
{
    Menu_EraseWindowRect(0x14, 0x8, 0x1A, 0xD);
    gTasks[taskid].func = &sub_81159BC;
}

void sub_81157AC(u8 taskid)
{
    DestroyTask(eRoulette->varA5);
    sub_8116AB0(taskid);
}

void sub_81157D0(u8 r0)
{
    u8 temp0, temp1;
    switch (r0)
    {
    case 0x0:
        sub_8124DDC(&gBGTilemapBuffers[2][0], 0x0, 0xE, 0x7, 0x10, 0xD);
        break;
    case 0x1:
    case 0x2:
    case 0x3:
    case 0x4:
        temp0 = (r0 * 3 + 0xE);
        sub_8124DDC(&gBGTilemapBuffers[2][0], 0x0, 0xE, 0x7, 0x10, 0xD);
        sub_8124E2C(&gBGTilemapBuffers[2][0], ewram18a32, temp0, 0x7, 0x3, 0xD);
        break;
    case 0x5:
    case 0xA:
    case 0xF:
        temp1 = ((r0 - 1) / 5 * 3 + 0xA);
        sub_8124DDC(&gBGTilemapBuffers[2][0], 0x0, 0xE, 0x7, 0x10, 0xD);
        sub_8124E2C(&gBGTilemapBuffers[2][0], ewram18a80, 0xE, temp1, 0x10, 0x3);
        break;
    default:
        temp0 = ((r0 % 5) * 3 + 0xE);
        temp1 = ((r0 - 1) / 5 * 3 + 0x7);
        sub_8124DDC(&gBGTilemapBuffers[2][0], 0x0, 0xE, 0x7, 0x10, 0xD);
        sub_8124E2C(&gBGTilemapBuffers[2][0], ewram18a20, temp0, temp1, 0x3, 0x3);
    }
}

void sub_8115928(u8 taskid, u8 r1)
{
    if (!r1)
        sub_811829C(gTasks[taskid].data[0x4]);
    else
        sub_811829C(eRoulette->var1B[eRoulette->var1A_0]);
    sub_81157D0(gTasks[taskid].data[0x4]);
}

void sub_811597C(u8 taskid)
{
    eRoulette->var28 = 0x1;
    sub_81157D0(gTasks[taskid].data[0x4]);
    eRoulette->var23 = 0x2;
    eRoulette->var21 = 0x0;
    gTasks[taskid].func = &sub_8115E14;
}

void sub_81159BC(u8 taskid)
{
    s16 i;
    if(eRoulette->var08 & 0x20)
    {
        for (i = 0xB; (i < 0xE); i++)
            if ((eRoulette->var08 & gUnknown_083F8C00[i].var08) == 0)
                break;
    }
    else
    {
        for (i = 0x6; (i < 0xA); i++)
            if ((eRoulette->var08 & gUnknown_083F8C00[i].var08) == 0)
                break;
    }
    gTasks[taskid].data[0x4] = i;
    sub_8116CAC(taskid);
    sub_8117158(gTasks[taskid].data[0x4]);
    sub_811829C(gTasks[taskid].data[0x4]);
    sub_8116EF8(gTasks[taskid].data[0x4]);
    gTasks[taskid].data[0x1] = 0x0;
    gTasks[taskid].func = &sub_811597C;
}

u8 sub_8115A94(s16 *r0, u8 r1)
{
    s8 temp1 = 0;
    s8 temp = 0;
    s8 arr[0x4];
    s8 t;
    memcpy(&arr, gUnknown_083F8ECA, 0x4);
    t = (u8)*r0;
    switch (r1)
    {
    case 0x0:
    case 0x1:
        temp1 = (*r0 % 5);
        temp = temp1 + 0xF;
        if (temp1 == 0)
            temp1 = 0x5;
        break;
    case 0x2:
    case 0x3:
        temp1 = (s16)(*r0 / 5) * 5;
        temp = temp1 + 0x4;
        if (temp1 == 0)
            temp1 = 0x1;
        break;
    }
    *r0 += arr[r1];
    if (*r0 < temp1)
        *r0 = temp;
    if (*r0 > temp)
        *r0 = temp1;
    if (*r0 != t)
        return TRUE;
    return FALSE;
}

void sub_8115B58(u8 r0)
{
    u8 z = 0x0;
    bool8 var0 = FALSE;
    if (!(gMain.newKeys & DPAD_UP) || ((var0 = TRUE), sub_8115A94(&gTasks[r0].data[0x4], 0x0)))
        if (!(gMain.newKeys & DPAD_DOWN) || ((var0 = TRUE), sub_8115A94(&gTasks[r0].data[0x4], 0x1)))
            if (!(gMain.newKeys & DPAD_LEFT) || ((var0 = TRUE), sub_8115A94(&gTasks[r0].data[0x4], 0x2)))
                if (!(gMain.newKeys & DPAD_RIGHT) || ((var0 = TRUE), sub_8115A94(&gTasks[r0].data[0x4], 0x3)))
                    if (var0)
                    {
                        u8 i;
                        sub_8117158(gTasks[r0].data[0x4]);
                        sub_8115928(r0, FALSE);
                        gTasks[r0].data[0x1] = z;
                        PlaySE(SE_SELECT);
                        sub_8124D3C((&eRoulette->varB8), 0xFFFF);
                        (&eRoulette->varB8)->var04[0xF].var00_7 = 0x0;
                        (&eRoulette->varB8)->var04[0xE].var00_7 = 0x0;
                        (&eRoulette->varB8)->var04[0xD].var00_7 = 0x0;
                        sub_8116EF8(gTasks[r0].data[0x4]);
                        for (i = 0; i < 0x4; i++)
                        {
                            gSprites[eRoulette->var3C[i + 0x29]].oam.tileNum =
                            gSprites[eRoulette->var3C[i + 0x29]].sheetTileStart
                            + (*gSprites[eRoulette->var3C[i + 0x29]].anims)->type;
                        }
                        if ((u16)(gTasks[r0].data[0x4] - 1) < 0x4 && !(eRoulette->var08 & gUnknown_083F8C00[gTasks[r0].data[0x4]].var08) )
                        {
                            z = gTasks[r0].data[0x4] - 1;
                            gSprites[eRoulette->var3C[z + 0x29]].oam.tileNum =
                            gSprites[eRoulette->var3C[z + 0x29]].sheetTileStart
                            + (*gSprites[eRoulette->var3C[z + 0x29]].anims + 1)->type;
                        }
                    }
}

void sub_8115D58(u8 r0)
{
    eRoulette->var28 = 0xFF;
    if (eRoulette->var19 == 1)
        eRoulette->var23 = 1;
    else
        eRoulette->var23 = 0;
    eRoulette->var21 = 0;
    gTasks[r0].data[0x1] = 0x20;
    gTasks[r0].func = &sub_8115ECC;
}

void sub_8115DA0(u8 taskid)
{
    eRoulette->var1B[eRoulette->var1A_0] = gTasks[taskid].data[0x4];
    gTasks[taskid].data[0x2] = sub_8117380(eRoulette->var1B[eRoulette->var1A_0]);
    sub_811829C(eRoulette->var1B[eRoulette->var1A_0]);
    if ((gTasks[taskid].data[0xD] -= eRoulette->var19) < 0)
        gTasks[taskid].data[0xD] = 0;
    sub_81180F4(gTasks[taskid].data[0xD]);
    gTasks[taskid].func = &sub_8115D58;
}

void sub_8115E14(u8 taskid)
{
    sub_8115B58(taskid);
    switch (gTasks[taskid].data[0x1])
    {
    case 0x0:
        sub_81157D0(gTasks[taskid].data[0x4]);
        gTasks[taskid].data[0x1]+= 1;
    break;
    case 0x1E:
        sub_81157D0(0);
        gTasks[taskid].data[0x1]+= 1;
    break;
    case 0x3B:
        gTasks[taskid].data[0x1] = 0;
    break;
    default:
        gTasks[taskid].data[0x1]++;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        if ((eRoulette->var08 & gUnknown_083F8C00[gTasks[taskid].data[0x4]].var08))
            PlaySE(SE_BOO);
        else
        {
            m4aSongNumStart(SE_REGI);
            gTasks[taskid].func = &sub_8115DA0;
        }
    }
}

void sub_8115ECC(u8 taskid)
{
    if (gTasks[taskid].data[0x1]-- > 0)
    {
        if (gTasks[taskid].data[0x1] > 0x2)
            gSpriteCoordOffsetX += 0x2;
        if ((eRoulette->var26 += 0x4) == 0x68)
            gSprites[eRoulette->var3C[0x19]].callback = &SpriteCallbackDummy;
    }
    else
    {
        sub_8117AA8(0x1, 0xFF);
        sub_8117C60(0x1, 0xFF);
        gTasks[taskid].func = &sub_8116100;
        gTasks[taskid].data[0x1] = 0;
    }
}

u8 sub_8115F58(u16 r0, u16 r1)
{
    u8 timeh;
    switch (eRoulette->var02)
    {
    case 0x1: // SHROOMISH
    case 0x2: // TAILLOW
        // one of the two is in party
        timeh = gLocalTime.hours - 0x4;
        if (!(timeh > 0x5))
        {
            if (!(r0 < 0xC || (r1 & 0x1)))
                return 0x1;
            else
            {
                struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
                return p[eRoulette->var04_0].var02 / 2;
            }
        }
        else if (!(r1 & 0x3))
        {
            struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
            return p[eRoulette->var04_0].var02 / 2;
        }
        else
        {
            struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
            return p[eRoulette->var04_0].var02;
        }
        break;
    case 0x3:
        // both are in party
        timeh = gLocalTime.hours - 0x4;
        if (!(timeh > 0x6))
        {
            if (r0 < 0x6 || (r1 & 0x1))
            {
                struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
                return p[eRoulette->var04_0].var02 / 2;
            }
            else
            {
                return 0x1;
            }
        }
        else if ((r1 & 0x1) && !(r0 < 0x7))
        {
            struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
            return p[eRoulette->var04_0].var02 / 4;
        }
        else
        {
            struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
            return p[eRoulette->var04_0].var02 / 2;
        }
        break;
    case 0:
    default:
        // neither is in party
        timeh = gLocalTime.hours - 0x4;
        if (!(timeh > 5))
        {
            if (!(r1 & 0x3))
            {
                return 0x1;
            }
            else
            {
                struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
                return p[eRoulette->var04_0].var02 / 2;
            }
        }
        else if (!(r1 & 0x3))
        {
            if (!(r0 < 0xD))
            {
                struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
                return p[eRoulette->var04_0].var02 / 2;
            }
            else
            {
                struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
                return p[eRoulette->var04_0].var02;
            }
        }
        else if (r1 & (0x80 << 8))
        {
            if (!(r0 < 0xD))
            {
                struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
                return p[eRoulette->var04_0].var02;
            }
            else
            {
                struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
                return p[eRoulette->var04_0].var01;
            }
        }
        else
        {
            struct StructgUnknown_083F8DF4 *p = &gUnknown_083F8DF4[0];
            return p[eRoulette->var04_0].var01 * 2;
        }
    }
}

#ifdef NONMATCHING
void sub_8116100(u8 taskid)
{
    u8 randf;
    s8 randfinal;
    s8 r5;
    u16 g;
    u16 rand;
    u16 randmod;
    u16 angles[0x4]; // angles in 90 degree steps
    u8 zero = 0x0;
    memcpy(angles, &gUnknown_083F8ECE, 0x8);
    rand = Random();
    randmod = rand % 0x64;
    eRoulette->var7C = gTasks[taskid].data[0x6];
    eRoulette->var7F = zero;
    eRoulette->var7E = zero;
    eRoulette->var7D = zero;
    randf = sub_8115F58(gTasks[taskid].data[0x8], rand);
    randfinal = (rand % randf) - (randf / 2);
    if (gLocalTime.hours < 0xD)
        r5 = 0x0;
    else
        r5 = 0x1;
    if (randmod < 0x50)
        r5 *= 2;
    else
        r5 = (1 - r5) * 2;
    g = (&gUnknown_083F8DF4[eRoulette->var04_0])->var1A;
    eRoulette->var80 = (g + randfinal);
    //
    g = ((float)(u16)(g + randfinal)) / 5.0f;
    eRoulette->var82 = g * 3;
    eRoulette->var84 = g;
    eRoulette->var86 = g;
    //
    eRoulette->var88 = (float)(angles[(rand & 0x1) + r5]);
    eRoulette->var8C = (float)((&gUnknown_083F8DF4[eRoulette->var04_0])->var18);
    eRoulette->var90 = ((eRoulette->var8C * 0.5f) - eRoulette->var8C) / (float)(u16)(g * 3);
    eRoulette->var94 = 68.0f;
    eRoulette->var9C = 0.0f;
    eRoulette->var98 = -(8.0f / (float)(u16)(g * 3));
    eRoulette->varA0 = 36.0f;
    gTasks[taskid].func = &sub_8116308;
}
#else
__attribute__((naked))
void sub_8116100(u8 taskid)
{
asm(".syntax unified\n\
push {r4-r7,lr}\n\
mov r7, r10\n\
mov r6, r9\n\
mov r5, r8\n\
push {r5-r7}\n\
sub sp, 0x8\n\
lsls r0, 24\n\
lsrs r0, 24\n\
mov r9, r0\n\
movs r4, 0\n\
ldr r1, _08116188 @ =gUnknown_083F8ECE\n\
mov r0, sp\n\
movs r2, 0x8\n\
bl memcpy\n\
bl Random\n\
lsls r0, 16\n\
lsrs r7, r0, 16\n\
adds r0, r7, 0\n\
movs r1, 0x64\n\
bl __umodsi3\n\
lsls r0, 16\n\
lsrs r6, r0, 16\n\
ldr r3, _0811618C @ =0x02019000\n\
ldr r1, _08116190 @ =gTasks\n\
mov r2, r9\n\
lsls r0, r2, 2\n\
add r0, r9\n\
lsls r0, 3\n\
adds r0, r1\n\
ldrh r2, [r0, 0x14]\n\
adds r1, r3, 0\n\
adds r1, 0x7C\n\
strb r2, [r1]\n\
adds r1, 0x3\n\
strb r4, [r1]\n\
subs r1, 0x1\n\
strb r4, [r1]\n\
subs r1, 0x1\n\
strb r4, [r1]\n\
ldrh r0, [r0, 0x18]\n\
adds r1, r7, 0\n\
bl sub_8115F58\n\
adds r4, r0, 0\n\
lsls r4, 24\n\
lsrs r1, r4, 24\n\
adds r0, r7, 0\n\
bl __modsi3\n\
lsrs r4, 25\n\
subs r0, r4\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
ldr r0, _08116194 @ =gLocalTime\n\
ldrb r0, [r0, 0x2]\n\
lsls r0, 24\n\
asrs r0, 24\n\
movs r5, 0x1\n\
cmp r0, 0xC\n\
bgt _08116180\n\
movs r5, 0\n\
_08116180:\n\
cmp r6, 0x4F\n\
bhi _08116198\n\
lsls r0, r5, 25\n\
b _081161A2\n\
.align 2, 0\n\
_08116188: .4byte gUnknown_083F8ECE\n\
_0811618C: .4byte 0x02019000\n\
_08116190: .4byte gTasks\n\
_08116194: .4byte gLocalTime\n\
_08116198:\n\
lsls r1, r5, 24\n\
asrs r1, 24\n\
movs r0, 0x1\n\
subs r0, r1\n\
lsls r0, 25\n\
_081161A2:\n\
lsrs r5, r0, 24\n\
ldr r6, _081162D8 @ =0x02019000\n\
ldrb r0, [r6, 0x4]\n\
lsls r0, 30\n\
mov r10, r0\n\
lsrs r0, 25\n\
ldr r1, _081162DC @ =gUnknown_083F8DF4\n\
adds r0, r1\n\
lsls r4, 24\n\
asrs r4, 24\n\
ldrh r0, [r0, 0x1A]\n\
adds r4, r0\n\
adds r0, r6, 0\n\
adds r0, 0x80\n\
strh r4, [r0]\n\
lsls r4, 16\n\
asrs r4, 16\n\
adds r0, r4, 0\n\
bl __floatsisf\n\
cmp r4, 0\n\
bge _081161D4\n\
ldr r1, _081162E0 @ =0x47800000\n\
bl __addsf3\n\
_081161D4:\n\
ldr r1, _081162E4 @ =0x40a00000\n\
bl __divsf3\n\
bl __fixunssfsi\n\
lsls r0, 16\n\
lsrs r4, r0, 16\n\
lsls r0, r4, 1\n\
adds r0, r4\n\
mov r8, r0\n\
adds r0, r6, 0\n\
adds r0, 0x82\n\
mov r2, r8\n\
strh r2, [r0]\n\
adds r0, 0x2\n\
strh r4, [r0]\n\
adds r0, 0x2\n\
strh r4, [r0]\n\
movs r0, 0x1\n\
ands r0, r7\n\
lsls r1, r5, 24\n\
asrs r1, 24\n\
adds r0, r1\n\
lsls r0, 1\n\
add r0, sp\n\
movs r1, 0\n\
ldrsh r4, [r0, r1]\n\
adds r0, r4, 0\n\
bl __floatsisf\n\
cmp r4, 0\n\
bge _0811621A\n\
ldr r1, _081162E0 @ =0x47800000\n\
bl __addsf3\n\
_0811621A:\n\
ldr r2, _081162E8 @ =0x02019088\n\
str r0, [r2]\n\
adds r7, r6, 0\n\
adds r7, 0x8C\n\
mov r1, r10\n\
lsrs r0, r1, 25\n\
ldr r2, _081162DC @ =gUnknown_083F8DF4\n\
adds r0, r2\n\
movs r1, 0x18\n\
ldrsh r4, [r0, r1]\n\
adds r0, r4, 0\n\
bl __floatsisf\n\
adds r5, r0, 0\n\
cmp r4, 0\n\
bge _08116242\n\
ldr r1, _081162E0 @ =0x47800000\n\
bl __addsf3\n\
adds r5, r0, 0\n\
_08116242:\n\
str r5, [r7]\n\
adds r7, r6, 0\n\
adds r7, 0x90\n\
ldr r1, _081162EC @ =0x3f000000\n\
adds r0, r5, 0\n\
bl __mulsf3\n\
adds r1, r5, 0\n\
bl __subsf3\n\
adds r5, r0, 0\n\
mov r2, r8\n\
lsls r0, r2, 16\n\
asrs r4, r0, 16\n\
adds r0, r4, 0\n\
bl __floatsisf\n\
adds r2, r0, 0\n\
cmp r4, 0\n\
bge _08116272\n\
ldr r1, _081162E0 @ =0x47800000\n\
bl __addsf3\n\
adds r2, r0, 0\n\
_08116272:\n\
adds r0, r5, 0\n\
adds r1, r2, 0\n\
bl __divsf3\n\
str r0, [r7]\n\
adds r1, r6, 0\n\
adds r1, 0x94\n\
ldr r0, _081162F0 @ =0x42880000\n\
str r0, [r1]\n\
adds r1, 0x8\n\
ldr r0, _081162F4 @ =0x00000000\n\
str r0, [r1]\n\
adds r5, r6, 0\n\
adds r5, 0x98\n\
adds r0, r4, 0\n\
bl __floatsisf\n\
adds r2, r0, 0\n\
cmp r4, 0\n\
bge _081162A2\n\
ldr r1, _081162E0 @ =0x47800000\n\
bl __addsf3\n\
adds r2, r0, 0\n\
_081162A2:\n\
ldr r0, _081162F8 @ =0x41000000\n\
adds r1, r2, 0\n\
bl __divsf3\n\
bl __negsf2\n\
str r0, [r5]\n\
adds r1, r6, 0\n\
adds r1, 0xA0\n\
ldr r0, _081162FC @ =0x42100000\n\
str r0, [r1]\n\
ldr r1, _08116300 @ =gTasks\n\
mov r2, r9\n\
lsls r0, r2, 2\n\
add r0, r9\n\
lsls r0, 3\n\
adds r0, r1\n\
ldr r1, _08116304 @ =sub_8116308\n\
str r1, [r0]\n\
add sp, 0x8\n\
pop {r3-r5}\n\
mov r8, r3\n\
mov r9, r4\n\
mov r10, r5\n\
pop {r4-r7}\n\
pop {r0}\n\
bx r0\n\
.align 2, 0\n\
_081162D8: .4byte 0x02019000\n\
_081162DC: .4byte gUnknown_083F8DF4\n\
_081162E0: .4byte 0x47800000\n\
_081162E4: .4byte 0x40a00000\n\
_081162E8: .4byte 0x02019088\n\
_081162EC: .4byte 0x3f000000\n\
_081162F0: .4byte 0x42880000\n\
_081162F4: .4byte 0x00000000\n\
_081162F8: .4byte 0x41000000\n\
_081162FC: .4byte 0x42100000\n\
_08116300: .4byte gTasks\n\
_08116304: .4byte sub_8116308\n\
.syntax divided\n");
}
#endif

void sub_8116308(u8 taskid)
{
    u8 index;
    eRoulette->var03_7 = 1;
    index = eRoulette->var3C[eRoulette->var7C];
    eRoulette->var38 = &gSprites[index];
    (&gSprites[index])->callback = &sub_81191F4;
    gTasks[taskid].data[0x6]++;
    gTasks[taskid].data[0x8]++;
    sub_81182F8(0x6 - gTasks[taskid].data[0x6]);
    m4aSongNumStart(SE_TAMAKORO);
    gTasks[taskid].func = &sub_811637C;
}

void sub_811637C(u8 taskid)
{
    if (eRoulette->var7D)
    {
        if (eRoulette->var03_5)
        {
            if (eRoulette->var03_6)
            {
                eRoulette->var03_6 = FALSE;
                eRoulette->var03_5 = FALSE;
            }
        }
        else
        {
            if (!gTasks[taskid].data[0x1])
            {
                u8 temp = sub_8116E5C(sub_8116D54(taskid, eRoulette->var7E), eRoulette->var1B[eRoulette->var1A_0]);
                gTasks[taskid].data[0x5] = temp;
                if (temp == 1)
                    sub_8124CE8((&eRoulette->varB8), 0x1000);
            }
            if (gTasks[taskid].data[0x1] < 0x3D)
            {
                if (gMain.newKeys & A_BUTTON)
                    gTasks[taskid].data[0x1] = 0x3C;
                gTasks[taskid].data[0x1]++;
            }
            else
            {
                sub_8117158(eRoulette->var1B[eRoulette->var1A_0]);
                sub_8117AA8(0x0, (u8)gTasks[taskid].data[0xC]);
                sub_8117C60(0x0, (u8)gTasks[taskid].data[0x6] - 0x1);
                gTasks[taskid].data[0x1] = 0x20;
                gTasks[taskid].func = &sub_8116474;
            }
        }
    }
}

void sub_8116474(u8 taskid)
{
    if (gTasks[taskid].data[0x1]-- > 0x0)
    {
        if(gTasks[taskid].data[0x1] > 0x2)
            gSpriteCoordOffsetX -= 0x2;
        if((eRoulette->var26 -= 0x4) == 0x68)
            gSprites[eRoulette->var3C[0x19]].callback = &sub_81184CC;
    }
    else
    {
        sub_8117D68(gTasks[taskid].data[0xC]);
        if (gTasks[taskid].data[0x5] == 1)
            gTasks[taskid].data[0x1] = 0x79;
        else
            gTasks[taskid].data[0x1] = 0x3D;
        gTasks[taskid].func = &sub_8116514;
    }
}

void sub_8116514(u8 taskid)
{
    if (gTasks[taskid].data[0x1]-- > 0x1)
    {
        switch(gTasks[taskid].data[0x1] % 0x10)
        {
        case 0x8:
            sub_8117AA8(0x0, 0xFF);
            sub_8117C60(0x0, 0xFF);
            break;
        case 0x0:
            sub_8117AA8(0x0, (u8)gTasks[taskid].data[0xC]);
            sub_8117C60(0x0, (u8)gTasks[taskid].data[0x6] - 0x1);
            break;
        }
    }
    else
    {
        sub_8116C34(taskid, &sub_8116638, 0x1E, 0x0);
    }
}

void sub_811659C(u8 taskid)
{
    switch(gTasks[taskid].data[0x5])
    {
    case 0x1:
    case 0x2:
        if (IsFanfareTaskInactive())
        {
            u32 wins = GetGameStat(GAME_STAT_CONSECUTIVE_ROULETTE_WINS);
            if(wins < ++gTasks[taskid].data[0xB])
                SetGameStat(GAME_STAT_CONSECUTIVE_ROULETTE_WINS, gTasks[taskid].data[0xB]);
            sub_8116C34(taskid, &sub_811677C, 0xFFFF, 0x3);
        }
        break;
    case 0:
    default:
        if (!IsSEPlaying())
        {
            gTasks[taskid].data[0xB] = FALSE;
            sub_8116C34(taskid, &sub_81167F4, 0xFFFF, 0x3);
        }
    }
}

void sub_8116638(u8 taskid)
{
    switch(gTasks[taskid].data[0x5])
    {
    case 0x1:
    case 0x2:
        if (gTasks[taskid].data[0x2] == 0xC)
        {
            PlayFanfare(BGM_ME_B_BIG);
            Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
            Menu_PrintText(&gUnknown_081C41A5, 0x1, 0xF);
        }
        else
        {
            PlayFanfare(BGM_ME_B_SMALL);
            Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
            Menu_PrintText(&gUnknown_081C4199, 0x1, 0xF);
        }
        break;
    case 0:
    default:
        m4aSongNumStart(SE_HAZURE);
        Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
        Menu_PrintText(&gUnknown_081C41AE, 0x1, 0xF);
    }
    gTasks[taskid].data[0x1] = 0x0;
    gTasks[taskid].func = &sub_811659C;
}

void sub_81166E8(u8 taskid)
{
    s32 r0 = gTasks[taskid].data[0x7];
    switch(r0)
    {
    case 0x0:
        gTasks[taskid].data[0xD]++;
        m4aSongNumStart(SE_PIN);
        sub_81180F4(gTasks[taskid].data[0xD]);
        if (gTasks[taskid].data[0xD] > 0x270E)
        {
            gTasks[taskid].data[0x1] = r0;
        }
        else
        {
            gTasks[taskid].data[0x1]--;
            gTasks[taskid].data[0x7]++;
        }
        break;
    case 0x3:
        m4aSongNumStop(SE_PIN);
        gTasks[taskid].data[0x7] = 0x0;
        break;
    default:
        gTasks[taskid].data[0x7]++;
    }
    if (gTasks[taskid].data[0x1] == 0x0)
        sub_8116C34(taskid, &sub_81167F4, 0xFFFF, 0x3);
}

void sub_811677C(u8 taskid)
{
    ConvertIntToDecimalStringN((u8 *)&gStringVar1, (eRoulette->var19 * gTasks[taskid].data[0x2]), STR_CONV_MODE_LEFT_ALIGN, 0x2);
    StringExpandPlaceholders((u8 *)&gStringVar4, &gUnknown_081C41BD);
    Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
    Menu_PrintText((u8 *)&gStringVar4, 0x1, 0xF);
    gTasks[taskid].data[0x1] = (eRoulette->var19 * gTasks[taskid].data[0x2]);
    gTasks[taskid].data[0x7] = 0x0;
    gTasks[taskid].func = &sub_81166E8;
}

void sub_81167F4(u8 taskid)
{
    sub_8124D3C((&eRoulette->varB8), 0xFFFF);
    (&eRoulette->varB8)->var04[0xF].var00_7 = 0x0;
    (&eRoulette->varB8)->var04[0xE].var00_7 = 0x0;
    (&eRoulette->varB8)->var04[0xD].var00_7 = 0x0;
    gSprites[eRoulette->var3C[0x7 + gUnknown_083F8C00[gTasks[taskid].data[0xC]].var00]].invisible = TRUE;
    gTasks[taskid].func = &sub_8116880;
}

void sub_8116880(u8 taskid)
{
    u8 i = 0;
    gTasks[taskid].data[0x4] = i;
    eRoulette->var1B[eRoulette->var1A_0] = 0x0;
    sub_8117158(0x0);
    gSprites[eRoulette->var3C[0x30]].invisible = TRUE;
    for (i = 0; i < 0x4; i++)
    {
        gSprites[eRoulette->var3C[i + 0x29]].oam.tileNum =
        gSprites[eRoulette->var3C[i + 0x29]].sheetTileStart
        + (*gSprites[eRoulette->var3C[i + 0x29]].anims)->type;
    }
    if (!(gTasks[taskid].data[0xD] < eRoulette->var19))
    {
        if (gTasks[taskid].data[0x6] == 0x6)
        {
            Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
            Menu_PrintText(&gUnknown_081C41F1, 0x1, 0xF);
            sub_8116C34(taskid, &dp01t_12_3_battle_menu, 0xFFFF, 0x3);
        }
        else if (gTasks[taskid].data[0xD] == 0x270F)
        {
            Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
            Menu_PrintText(&gUnknown_081C4231, 0x1, 0xF);
            sub_8116C34(taskid, &sub_8115734, 0xFFFF, 0x3);
        }
        else
        {
            gTasks[taskid].func = &sub_8115734;
        }
    }
    else
    {
        Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
        Menu_PrintText(&gUnknown_081C41D2, 0x1, 0xF);
        sub_8116C34(taskid, &sub_81157AC, 0x3C, 0x3);
    }
}

void dp01t_12_3_battle_menu(u8 taskid)
{
    u8 i = 0x0;
    gTasks[taskid].data[0x6] = 0x0;
    sub_8116CAC(taskid);
    sub_8116CF8();
    sub_81185E8();
    sub_8117158(0x0);
    sub_81182F8(0x6);
    for (i = 0; i < 0xC; i++)
        gSprites[eRoulette->var3C[i + 0x7]].invisible = FALSE;
    if (gTasks[taskid].data[0xD] == 0x270F)
    {
        Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
        Menu_PrintText(&gUnknown_081C4231, 0x1, 0xF);
        sub_8116C34(taskid, &sub_8115734, 0xFFFF, 0x3);
    }
    else
    {
        gTasks[taskid].func = &sub_8115734;
    }
}

void sub_8116AB0(u8 taskid)
{
    sub_8124D3C((&eRoulette->varB8), 0xFFFF);
    sub_8124918((&eRoulette->varB8));
    gSaveBlock1.coins = gTasks[taskid].data[0xD];
    if (gSaveBlock1.coins < eRoulette->var19)
        gSpecialVar_0x8004 = TRUE;
    else
        gSpecialVar_0x8004 = FALSE;
    BeginHardwarePaletteFade(0xFF, 0x0, 0x0, 0x10, 0x0);
    gTasks[taskid].func = &sub_8116B40;
}

void sub_8116B40(u8 taskid) // end oulette ?
{
    if (UpdatePaletteFade() == 0)
    {
        SetVBlankCallback(0x0);
        memset(eRoulette, 0x0, 0x17C);
        gSpriteCoordOffsetX = gSpriteCoordOffsetY = 0x0;
        sub_80F9368();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetSpriteData();
        sub_80F9020();
        REG_BLDCNT = 0x0;
        REG_BLDALPHA = 0x0;
        REG_BLDY = 0x0;
        gFieldCallback = &sub_8080990;
        SetMainCallback2(&c2_exit_to_overworld_2_switch);
        DestroyTask(taskid);
    }
}

void sub_8116BC0(u8 taskid)
{
    if(eRoulette->varA8 == 0 || gMain.newKeys & eRoulette->varAA)
    {
        gTasks[taskid].func = eRoulette->varAC;
        if (eRoulette->varAA > 0)
            PlaySE(SE_SELECT);
        eRoulette->varAC = NULL;
        eRoulette->varAA = 0x0;
        eRoulette->varA8 = 0x0;
    }
    if (eRoulette->varA8 != 0xFFFF)
        eRoulette->varA8--;
}

void sub_8116C34(u8 taskid, TaskFunc r1, u16 r2, u16 r3)
{
    eRoulette->varB4 = gTasks[taskid].func;
    if(r1 == NULL)
        r1 = eRoulette->varB4;
    eRoulette->varAC = r1;
    eRoulette->varA8 = r2;
    if (r2 == 0xFFFF && r3 == 0)
        eRoulette->varAA = 0xFFFF;
    else
        eRoulette->varAA = r3;
    gTasks[taskid].func = &sub_8116BC0;
}

void sub_8116CAC(u8 taskid)
{
    u8 i = 0;
    eRoulette->var00 = i;
    eRoulette->var03_0 = 0x0;
    eRoulette->var03_5 = 0x0;
    eRoulette->var03_6 = 0x0;
    eRoulette->var03_7 = 0x0;
    for (i = 0; i < 0x6; i++)
        eRoulette->var1B[i] = 0x0;
    eRoulette->var1A_0 = 0x0;
    gTasks[taskid].data[0x1] = 0x0;
}

void sub_8116CF8(void)
{
    u8 i;
    eRoulette->var08 = 0x0;
    for (i = 0; i < 0x6; i++)
        eRoulette->var0C[i] = 0x0;
    for (i = 0; i < 0x4; i++)
        eRoulette->var12[i] = 0x0;
    for (i = 0; i < 0x3; i++)
        eRoulette->var16[i] = 0x0;
    sub_8117C60(0x1, 0xFF);
}

u8 sub_8116D54(u8 taskid, u8 r1)
{
    u32 t0[0x4];
    u32 t1[0x3];
    memcpy(t0, gUnknown_083F8ED8, 0x10);
    memcpy(t1, gUnknown_083F8EE8, 0xC);
    if (r1 > 0xB)
    {
        return 0x0;
    }
    else
    {
        u8 i;
        u8 z;
        eRoulette->var0C[gTasks[taskid].data[0x6] - 1] = gUnknown_083F8D90[r1].var02;
        gTasks[taskid].data[0xC] = gUnknown_083F8D90[r1].var02;
        eRoulette->var08 |= gUnknown_083F8D90[r1].var04;
        for (i = 0; i < 0x4; i++)
        {
            if (gUnknown_083F8D90[r1].var04 & t0[i])
                eRoulette->var12[i]++;
            if (eRoulette->var12[i] > 0x2)
                eRoulette->var08 |= t0[i];
        }
        for (z = 0; z < 0x3; z++)
        {
            if (gUnknown_083F8D90[r1].var04 & t1[z])
                eRoulette->var16[z]++;
            if (eRoulette->var16[z] > 0x3)
                eRoulette->var08 |= t1[z];
        }
        return gUnknown_083F8D90[r1].var02;
    }
}

u8 sub_8116E5C(u8 r0, u8 r1)
{
    u8 t = r0;
    if (--r0 < 0x13)
    {
        switch(r1)
        {
        case 0x0:
            return 0x3;
            break;
        case 0x1 ... 0x4:
            if (t == r1 + 5 || t == r1 + 10 || t == r1 + 15)
                return 0x1;
            break;
        case 0x5:
        case 0xA:
        case 0xF:
            if (!(t < r1 + 1) && !(t > r1 + 4))
                return 0x1;
            break;
        default:
            if (t == r1)
                return 0x1;
        }
    }
    return 0x0;
}

#ifdef NONMATCHING // stack variable switched with a register variable
void sub_8116EF8(u8 r0)
{

    u32 var0 = 0x0;
    struct UnkStruct1 var1[0x3];
    u32 var2;
    u16 var3;
    u32 var4;
    u8 i;
    switch(r0)
    {
    case 0x5:
    case 0xA:
    case 0xF:
        for (i = (r0 + 0x1); i < (r0 + 0x5); i++)
            if (!(eRoulette->var08 & gUnknown_083F8C00[i].var08))
                var0 |= gUnknown_083F8C00[i].var10;
        var0 &= 0xDFFF;
        sub_8124CE8(&eRoulette->varB8, var0);
        break;
    default:
        memcpy(var1, gUnknown_083F8E9C, 0x18);
        if ((u8)(r0 - 0x1) < 0x4)
            var2 = 0x3;
        else
            var2 = 0x1;
        var3 = ((r0 / 0x5) - 0x1);
        switch((u8)r0 % 0x5)
        {
        case 0x1:
            var3 = gSprites[eRoulette->var3C[0x7 + 0x0]].oam.paletteNum * 0x10;
            break;
        case 0x2:
            var3 = gSprites[eRoulette->var3C[0x7 + 0x1]].oam.paletteNum * 0x10;
            break;
        case 0x3:
            var3 = gSprites[eRoulette->var3C[0x7 + 0x2]].oam.paletteNum * 0x10;
            break;
        case 0x4:
            var3 = gSprites[eRoulette->var3C[0x7 + 0x3]].oam.paletteNum * 0x10;
            break;
        }
        if (var2 == 0x1)
        {
            if (!(eRoulette->var08 & gUnknown_083F8C00[r0].var08))
            {
                var4 = (r0 / 0x5);
                var1[var4 - 0x1].var02 += var3;
                sub_812492C(&eRoulette->varB8, 0xD, &var1[var4 - 0x2]);
                sub_8124CE8(&eRoulette->varB8, var0 |= gUnknown_083F8C00[r0].var10);
            }
        }
        else
        {
            for (i = 0; i < 0x3; i++)
            {
                u8 var4 = i * 0x5 + r0 + 0x5;
                if (!(eRoulette->var08 & gUnknown_083F8C00[var4].var08))
                {
                    u8 var5 = (var4 / 0x5);
                    var1[var5 - 0x1].var02 += var3;
                    sub_812492C(&eRoulette->varB8, (u8)(0xD + i), &var1[var5 - 0x2]);
                    if (var2 == 0x3)
                        var0 = gUnknown_083F8C00[var4].var10;
                    var2--;
                }
            }
            if (var2 != 0x2)
                var0 = 0x0;
            sub_8124CE8(&eRoulette->varB8, var0 |= gUnknown_083F8C00[r0].var10);
        }
    }
}

#else
__attribute__((naked))
void sub_8116EF8(u8 r0)
{
asm(".syntax unified\n\
push {r4-r7,lr}\n\
mov r7, r10\n\
mov r6, r9\n\
mov r5, r8\n\
push {r5-r7}\n\
sub sp, 0x20\n\
lsls r0, 24\n\
lsrs r6, r0, 24\n\
movs r0, 0\n\
str r0, [sp, 0x18]\n\
cmp r6, 0xA\n\
beq _08116F1E\n\
cmp r6, 0xA\n\
bgt _08116F1A\n\
cmp r6, 0x5\n\
beq _08116F1E\n\
b _08116F7C\n\
_08116F1A:\n\
cmp r6, 0xF\n\
bne _08116F7C\n\
_08116F1E:\n\
adds r0, r6, 0x1\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
adds r0, r6, 0x5\n\
ldr r7, _08116F70 @ =0x020190b8\n\
cmp r4, r0\n\
bge _08116F5E\n\
adds r1, r7, 0\n\
subs r1, 0xB8\n\
ldr r3, _08116F74 @ =gUnknown_083F8C00\n\
ldr r5, [r1, 0x8]\n\
adds r6, r3, 0\n\
adds r6, 0x8\n\
adds r2, r0, 0\n\
_08116F3A:\n\
lsls r0, r4, 2\n\
adds r0, r4\n\
lsls r1, r0, 2\n\
adds r0, r1, r6\n\
ldr r0, [r0]\n\
ands r0, r5\n\
cmp r0, 0\n\
bne _08116F54\n\
adds r0, r1, r3\n\
ldrh r0, [r0, 0x10]\n\
ldr r1, [sp, 0x18]\n\
orrs r1, r0\n\
str r1, [sp, 0x18]\n\
_08116F54:\n\
adds r0, r4, 0x1\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
cmp r4, r2\n\
blt _08116F3A\n\
_08116F5E:\n\
ldr r0, _08116F78 @ =0x0000dfff\n\
ldr r2, [sp, 0x18]\n\
ands r2, r0\n\
str r2, [sp, 0x18]\n\
adds r0, r7, 0\n\
adds r1, r2, 0\n\
bl sub_8124CE8\n\
b _0811713C\n\
.align 2, 0\n\
_08116F70: .4byte 0x020190b8\n\
_08116F74: .4byte gUnknown_083F8C00\n\
_08116F78: .4byte 0x0000dfff\n\
_08116F7C:\n\
mov r0, sp\n\
ldr r1, _08116FC8 @ =gUnknown_083F8E9C\n\
ldm r1!, {r2-r4}\n\
stm r0!, {r2-r4}\n\
ldm r1!, {r2-r4}\n\
stm r0!, {r2-r4}\n\
subs r0, r6, 0x1\n\
lsls r0, 24\n\
lsrs r0, 24\n\
movs r3, 0x1\n\
mov r10, r3\n\
cmp r0, 0x3\n\
bhi _08116F9A\n\
movs r4, 0x3\n\
mov r10, r4\n\
_08116F9A:\n\
adds r0, r6, 0\n\
movs r1, 0x5\n\
bl __udivsi3\n\
lsls r0, 24\n\
lsrs r0, 8\n\
ldr r1, _08116FCC @ =0xffff0000\n\
adds r0, r1\n\
lsrs r7, r0, 16\n\
adds r0, r6, 0\n\
movs r1, 0x5\n\
bl __umodsi3\n\
lsls r0, 24\n\
lsrs r0, 24\n\
cmp r0, 0x2\n\
beq _08116FF8\n\
cmp r0, 0x2\n\
bgt _08116FD4\n\
cmp r0, 0x1\n\
beq _08116FE4\n\
ldr r4, _08116FD0 @ =0x02019000\n\
b _0811703A\n\
.align 2, 0\n\
_08116FC8: .4byte gUnknown_083F8E9C\n\
_08116FCC: .4byte 0xffff0000\n\
_08116FD0: .4byte 0x02019000\n\
_08116FD4:\n\
cmp r0, 0x3\n\
beq _0811700C\n\
cmp r0, 0x4\n\
beq _08117020\n\
ldr r4, _08116FE0 @ =0x02019000\n\
b _0811703A\n\
.align 2, 0\n\
_08116FE0: .4byte 0x02019000\n\
_08116FE4:\n\
ldr r3, _08116FF0 @ =gSprites\n\
ldr r2, _08116FF4 @ =0x02019000\n\
adds r0, r2, 0\n\
adds r0, 0x43\n\
b _08117028\n\
.align 2, 0\n\
_08116FF0: .4byte gSprites\n\
_08116FF4: .4byte 0x02019000\n\
_08116FF8:\n\
ldr r3, _08117004 @ =gSprites\n\
ldr r2, _08117008 @ =0x02019000\n\
adds r0, r2, 0\n\
adds r0, 0x44\n\
b _08117028\n\
.align 2, 0\n\
_08117004: .4byte gSprites\n\
_08117008: .4byte 0x02019000\n\
_0811700C:\n\
ldr r3, _08117018 @ =gSprites\n\
ldr r2, _0811701C @ =0x02019000\n\
adds r0, r2, 0\n\
adds r0, 0x45\n\
b _08117028\n\
.align 2, 0\n\
_08117018: .4byte gSprites\n\
_0811701C: .4byte 0x02019000\n\
_08117020:\n\
ldr r3, _08117088 @ =gSprites\n\
ldr r2, _0811708C @ =0x02019000\n\
adds r0, r2, 0\n\
adds r0, 0x46\n\
_08117028:\n\
ldrb r1, [r0]\n\
lsls r0, r1, 4\n\
adds r0, r1\n\
lsls r0, 2\n\
adds r0, r3\n\
ldrb r0, [r0, 0x5]\n\
lsrs r0, 4\n\
lsls r7, r0, 4\n\
adds r4, r2, 0\n\
_0811703A:\n\
mov r2, r10\n\
cmp r2, 0x1\n\
bne _08117094\n\
ldr r1, _08117090 @ =gUnknown_083F8C00\n\
lsls r2, r6, 2\n\
adds r0, r2, r6\n\
lsls r0, 2\n\
adds r1, 0x8\n\
adds r0, r1\n\
ldr r1, [r4, 0x8]\n\
ldr r0, [r0]\n\
ands r1, r0\n\
str r2, [sp, 0x1C]\n\
cmp r1, 0\n\
bne _0811713C\n\
adds r0, r6, 0\n\
movs r1, 0x5\n\
bl __udivsi3\n\
lsls r0, 24\n\
lsrs r0, 24\n\
subs r1, r0, 0x1\n\
lsls r1, 3\n\
mov r3, sp\n\
adds r2, r3, r1\n\
ldrh r1, [r2, 0x2]\n\
adds r1, r7, r1\n\
strh r1, [r2, 0x2]\n\
adds r1, r4, 0\n\
adds r1, 0xB8\n\
lsls r0, 3\n\
subs r0, 0x8\n\
adds r2, r3, r0\n\
adds r0, r1, 0\n\
movs r1, 0xD\n\
bl sub_812492C\n\
b _08117122\n\
.align 2, 0\n\
_08117088: .4byte gSprites\n\
_0811708C: .4byte 0x02019000\n\
_08117090: .4byte gUnknown_083F8C00\n\
_08117094:\n\
movs r4, 0\n\
lsls r0, r6, 2\n\
str r0, [sp, 0x1C]\n\
ldr r1, _0811714C @ =0x02019000\n\
mov r8, r1\n\
ldr r2, _08117150 @ =gUnknown_083F8C00\n\
mov r9, r2\n\
_081170A2:\n\
lsls r0, r4, 2\n\
adds r0, r4\n\
adds r0, r6, r0\n\
adds r0, 0x5\n\
lsls r0, 24\n\
lsrs r2, r0, 24\n\
lsls r0, r2, 2\n\
adds r0, r2\n\
lsls r5, r0, 2\n\
mov r0, r9\n\
adds r0, 0x8\n\
adds r0, r5, r0\n\
mov r3, r8\n\
ldr r1, [r3, 0x8]\n\
ldr r0, [r0]\n\
ands r1, r0\n\
cmp r1, 0\n\
bne _0811710E\n\
adds r0, r2, 0\n\
movs r1, 0x5\n\
bl __udivsi3\n\
lsls r0, 24\n\
lsrs r0, 24\n\
subs r1, r0, 0x1\n\
lsls r1, 3\n\
mov r3, sp\n\
adds r2, r3, r1\n\
ldrh r1, [r2, 0x2]\n\
adds r1, r7, r1\n\
strh r1, [r2, 0x2]\n\
adds r1, r4, 0\n\
adds r1, 0xD\n\
lsls r1, 24\n\
lsrs r1, 24\n\
lsls r0, 3\n\
subs r0, 0x8\n\
adds r2, r3, r0\n\
mov r0, r8\n\
adds r0, 0xB8\n\
bl sub_812492C\n\
mov r0, r10\n\
cmp r0, 0x3\n\
bne _08117104\n\
mov r1, r9\n\
adds r0, r5, r1\n\
ldrh r0, [r0, 0x10]\n\
str r0, [sp, 0x18]\n\
_08117104:\n\
mov r0, r10\n\
subs r0, 0x1\n\
lsls r0, 24\n\
lsrs r0, 24\n\
mov r10, r0\n\
_0811710E:\n\
adds r0, r4, 0x1\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
cmp r4, 0x2\n\
bls _081170A2\n\
mov r2, r10\n\
cmp r2, 0x2\n\
beq _08117122\n\
movs r3, 0\n\
str r3, [sp, 0x18]\n\
_08117122:\n\
ldr r0, _08117154 @ =0x020190b8\n\
ldr r2, _08117150 @ =gUnknown_083F8C00\n\
ldr r4, [sp, 0x1C]\n\
adds r1, r4, r6\n\
lsls r1, 2\n\
adds r1, r2\n\
ldrh r1, [r1, 0x10]\n\
ldr r2, [sp, 0x18]\n\
orrs r2, r1\n\
str r2, [sp, 0x18]\n\
adds r1, r2, 0\n\
bl sub_8124CE8\n\
_0811713C:\n\
add sp, 0x20\n\
pop {r3-r5}\n\
mov r8, r3\n\
mov r9, r4\n\
mov r10, r5\n\
pop {r4-r7}\n\
pop {r0}\n\
bx r0\n\
.align 2, 0\n\
_0811714C: .4byte 0x02019000\n\
_08117150: .4byte gUnknown_083F8C00\n\
_08117154: .4byte 0x020190b8\n\
.syntax divided\n");
}
#endif

#ifdef NONMATCHING //not enough stack usage
void sub_8117158(u8 r0)
{
    u8 i;
    u8 l;
    u8 v[0x5];
    u8 z;
    u8 var0;
    u8 var1;
    u16 var2;
    eRoulette->var2A = 0x1;
    sub_8117AA8(0x0, 0x0);
    sub_8124E2C(&gBGTilemapBuffers[2][0], ewram18800, 0xE, 0x7, 0x10, 0xD);
    switch(r0)
    {
    case 0x0:
        return;
    case 0x1 ... 0x4:
        l = 0x4;
        for (i = 0x0; i < l; i++)
            v[i] = i * 0x5 + r0;
        break;
    case 0x5:
    case 0xA:
    case 0xF:
        l = 0x5;
        for (i = 0x0; i < l; i++)
            v[i] = i + r0;
        break;
    default:
        l = 0x1;
        v[0] = r0;
    }
    for (i = 0x0; i < l; i++)
    {
        //_0811727C
        var0 = gUnknown_083F8C00[v[i]].var06;
        var1 = gUnknown_083F8C00[v[i]].var03;
        for (z = 0; z < 0x3; z++)
        {
            var2 = (gUnknown_083F8C00[v[i]].var04 + z) * 0x20;
            gBGTilemapBuffers[2][var2 + var1 + 0x0] = ewram189a0[(var0 + z) * 0x3 + 0x0];
            gBGTilemapBuffers[2][var2 + var1 + 0x1] = ewram189a0[(var0 + z) * 0x3 + 0x1];
            gBGTilemapBuffers[2][var2 + var1 + 0x2] = ewram189a0[(var0 + z) * 0x3 + 0x2];
        }
        //_08117350
    }
}
#else
__attribute__((naked))
void sub_8117158(u8 r0)
{
asm(".syntax unified\n\
push {r4-r7,lr}\n\
mov r7, r10\n\
mov r6, r9\n\
mov r5, r8\n\
push {r5-r7}\n\
sub sp, 0x24\n\
lsls r0, 24\n\
lsrs r5, r0, 24\n\
ldr r4, _0811719C @ =0x02019000\n\
movs r0, 0x1\n\
strh r0, [r4, 0x2A]\n\
movs r0, 0\n\
movs r1, 0\n\
bl sub_8117AA8\n\
ldr r0, _081171A0 @ =gBGTilemapBuffers + 0x800\n\
ldr r1, _081171A4 @ =0xfffff800\n\
adds r4, r1\n\
movs r1, 0x10\n\
str r1, [sp]\n\
movs r1, 0xD\n\
str r1, [sp, 0x4]\n\
adds r1, r4, 0\n\
movs r2, 0xE\n\
movs r3, 0x7\n\
bl sub_8124E2C\n\
cmp r5, 0xF\n\
bhi _08117250\n\
lsls r0, r5, 2\n\
ldr r1, _081171A8 @ =_081171AC\n\
adds r0, r1\n\
ldr r0, [r0]\n\
mov pc, r0\n\
.align 2, 0\n\
_0811719C: .4byte 0x02019000\n\
_081171A0: .4byte gBGTilemapBuffers + 0x800\n\
_081171A4: .4byte 0xfffff800\n\
_081171A8: .4byte _081171AC\n\
.align 2, 0\n\
_081171AC:\n\
.4byte _08117360\n\
.4byte _081171EC\n\
.4byte _081171EC\n\
.4byte _081171EC\n\
.4byte _081171EC\n\
.4byte _08117220\n\
.4byte _08117250\n\
.4byte _08117250\n\
.4byte _08117250\n\
.4byte _08117250\n\
.4byte _08117220\n\
.4byte _08117250\n\
.4byte _08117250\n\
.4byte _08117250\n\
.4byte _08117250\n\
.4byte _08117220\n\
_081171EC:\n\
movs r0, 0x4\n\
str r0, [sp, 0x18]\n\
add r1, sp, 0x8\n\
movs r0, 0\n\
strb r0, [r1]\n\
adds r0, r1, 0\n\
ldrb r0, [r0]\n\
cmp r0, 0x3\n\
bhi _08117258\n\
add r4, sp, 0x10\n\
adds r3, r1, 0\n\
_08117202:\n\
ldrb r2, [r3]\n\
adds r2, r4\n\
ldrb r1, [r3]\n\
lsls r0, r1, 2\n\
adds r0, r1\n\
adds r0, r5, r0\n\
strb r0, [r2]\n\
ldrb r0, [r3]\n\
adds r0, 0x1\n\
strb r0, [r3]\n\
ldrb r0, [r3]\n\
ldr r1, [sp, 0x18]\n\
cmp r0, r1\n\
bcc _08117202\n\
b _08117258\n\
_08117220:\n\
movs r0, 0x5\n\
str r0, [sp, 0x18]\n\
add r1, sp, 0x8\n\
movs r0, 0\n\
strb r0, [r1]\n\
adds r0, r1, 0\n\
ldrb r0, [r0]\n\
cmp r0, 0x4\n\
bhi _08117258\n\
add r3, sp, 0x10\n\
adds r2, r1, 0\n\
_08117236:\n\
ldrb r1, [r2]\n\
adds r1, r3\n\
ldrb r0, [r2]\n\
adds r0, r5, r0\n\
strb r0, [r1]\n\
ldrb r0, [r2]\n\
adds r0, 0x1\n\
strb r0, [r2]\n\
ldrb r0, [r2]\n\
ldr r1, [sp, 0x18]\n\
cmp r0, r1\n\
bcc _08117236\n\
b _08117258\n\
_08117250:\n\
movs r0, 0x1\n\
str r0, [sp, 0x18]\n\
add r0, sp, 0x10\n\
strb r5, [r0]\n\
_08117258:\n\
add r1, sp, 0x8\n\
movs r0, 0\n\
strb r0, [r1]\n\
adds r0, r1, 0\n\
ldrb r0, [r0]\n\
ldr r1, [sp, 0x18]\n\
cmp r0, r1\n\
bcs _08117360\n\
mov r0, sp\n\
adds r0, 0xE\n\
str r0, [sp, 0x1C]\n\
add r1, sp, 0x10\n\
mov r9, r1\n\
add r0, sp, 0x8\n\
mov r12, r0\n\
mov r1, sp\n\
adds r1, 0xA\n\
str r1, [sp, 0x20]\n\
_0811727C:\n\
mov r1, r12\n\
ldrb r0, [r1]\n\
add r0, r9\n\
ldrb r1, [r0]\n\
lsls r0, r1, 2\n\
adds r0, r1\n\
lsls r0, 2\n\
ldr r1, _08117370 @ =gUnknown_083F8C00\n\
adds r0, r1\n\
ldrb r0, [r0, 0x6]\n\
ldr r1, [sp, 0x1C]\n\
strb r0, [r1]\n\
mov r1, r12\n\
ldrb r0, [r1]\n\
add r0, r9\n\
ldrb r1, [r0]\n\
lsls r0, r1, 2\n\
adds r0, r1\n\
lsls r0, 2\n\
ldr r1, _08117370 @ =gUnknown_083F8C00\n\
adds r0, r1\n\
ldrb r0, [r0, 0x3]\n\
ldr r1, [sp, 0x20]\n\
strh r0, [r1]\n\
mov r4, sp\n\
adds r4, 0x9\n\
movs r0, 0\n\
strb r0, [r4]\n\
ldrb r0, [r4]\n\
cmp r0, 0x2\n\
bhi _08117350\n\
add r5, sp, 0xC\n\
ldr r7, [sp, 0x20]\n\
ldr r0, _08117374 @ =gBGTilemapBuffers + 0x800\n\
mov r8, r0\n\
ldr r6, [sp, 0x1C]\n\
ldr r1, _08117378 @ =0x020189a0\n\
mov r10, r1\n\
_081172C8:\n\
mov r1, r12\n\
ldrb r0, [r1]\n\
add r0, r9\n\
ldrb r0, [r0]\n\
lsls r1, r0, 2\n\
adds r1, r0\n\
lsls r1, 2\n\
ldr r0, _08117370 @ =gUnknown_083F8C00\n\
adds r1, r0\n\
adds r3, r4, 0\n\
ldrb r0, [r4]\n\
ldrb r1, [r1, 0x4]\n\
adds r0, r1\n\
lsls r0, 5\n\
strh r0, [r5]\n\
ldrh r0, [r7]\n\
ldrh r2, [r5]\n\
adds r2, r0\n\
lsls r2, 1\n\
add r2, r8\n\
ldrb r0, [r6]\n\
ldrb r1, [r4]\n\
adds r1, r0\n\
lsls r0, r1, 1\n\
adds r0, r1\n\
lsls r0, 1\n\
add r0, r10\n\
ldrh r0, [r0]\n\
strh r0, [r2]\n\
ldrh r0, [r7]\n\
ldrh r2, [r5]\n\
adds r2, r0\n\
adds r2, 0x1\n\
lsls r2, 1\n\
add r2, r8\n\
ldrb r0, [r6]\n\
ldrb r1, [r4]\n\
adds r1, r0\n\
lsls r0, r1, 1\n\
adds r0, r1\n\
lsls r0, 1\n\
mov r1, r10\n\
adds r1, 0x2\n\
adds r0, r1\n\
ldrh r0, [r0]\n\
strh r0, [r2]\n\
ldrh r0, [r7]\n\
ldrh r2, [r5]\n\
adds r2, r0\n\
adds r2, 0x2\n\
lsls r2, 1\n\
add r2, r8\n\
ldrb r0, [r6]\n\
ldrb r1, [r4]\n\
adds r1, r0\n\
lsls r0, r1, 1\n\
adds r0, r1\n\
lsls r0, 1\n\
ldr r1, _0811737C @ =0x020189a4\n\
adds r0, r1\n\
ldrh r0, [r0]\n\
strh r0, [r2]\n\
ldrb r0, [r4]\n\
adds r0, 0x1\n\
strb r0, [r4]\n\
ldrb r0, [r3]\n\
cmp r0, 0x2\n\
bls _081172C8\n\
_08117350:\n\
mov r1, r12\n\
ldrb r0, [r1]\n\
adds r0, 0x1\n\
strb r0, [r1]\n\
ldrb r0, [r1]\n\
ldr r1, [sp, 0x18]\n\
cmp r0, r1\n\
bcc _0811727C\n\
_08117360:\n\
add sp, 0x24\n\
pop {r3-r5}\n\
mov r8, r3\n\
mov r9, r4\n\
mov r10, r5\n\
pop {r4-r7}\n\
pop {r0}\n\
bx r0\n\
.align 2, 0\n\
_08117370: .4byte gUnknown_083F8C00\n\
_08117374: .4byte gBGTilemapBuffers + 0x800\n\
_08117378: .4byte 0x020189a0\n\
_0811737C: .4byte 0x020189a4\n\
.syntax divided\n");
}
#endif

#ifdef NONMATCHING
u8 sub_8117380(u8 r0)
{
    u8 var0[0x5];
    u8 t;
    u8 z;
    memcpy(var0, gUnknown_083F8EF4, 0x5);
    if (r0 > 0x13)
        r0 = 0x0;
    switch (gUnknown_083F8C00[r0].var01_0)
    {
    case 0x3:
        z = r0 / 0x5 - 0x1;
        if (eRoulette->var16[z] > 0x3)
            return 0x0;
        return var0[eRoulette->var16[z] + 0x1];
    case 0x4:
        t = r0 - 0x1;
        if (eRoulette->var12[t] > 0x2)
            return 0x0;
        return var0[eRoulette->var12[t] + 0x2];
    case 0xC:
        if (eRoulette->var08 & gUnknown_083F8C00[r0].var08)
            return 0x0;
        return var0[0x4];
    default:
    }
    return 0x0;
}
#else
__attribute__((naked))
u8 sub_8117380(u8 r0)
{
asm(".syntax unified\n\
push {r4,lr}\n\
sub sp, 0x8\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
ldr r1, _081173B8 @ =gUnknown_083F8EF4\n\
mov r0, sp\n\
movs r2, 0x5\n\
bl memcpy\n\
cmp r4, 0x13\n\
bls _08117398\n\
movs r4, 0\n\
_08117398:\n\
ldr r3, _081173BC @ =gUnknown_083F8C00\n\
lsls r0, r4, 2\n\
adds r0, r4\n\
lsls r2, r0, 2\n\
adds r0, r2, r3\n\
ldrb r0, [r0, 0x1]\n\
lsls r0, 28\n\
lsrs r0, 28\n\
cmp r0, 0x4\n\
beq _081173EC\n\
cmp r0, 0x4\n\
bgt _081173C0\n\
cmp r0, 0x3\n\
beq _081173C6\n\
b _08117428\n\
.align 2, 0\n\
_081173B8: .4byte gUnknown_083F8EF4\n\
_081173BC: .4byte gUnknown_083F8C00\n\
_081173C0:\n\
cmp r0, 0xC\n\
beq _0811740C\n\
b _08117428\n\
_081173C6:\n\
adds r0, r4, 0\n\
movs r1, 0x5\n\
bl __udivsi3\n\
subs r0, 0x1\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
ldr r0, _081173E8 @ =0x02019000\n\
adds r0, 0x16\n\
adds r1, r4, r0\n\
ldrb r0, [r1]\n\
cmp r0, 0x3\n\
bhi _08117428\n\
ldrb r0, [r1]\n\
adds r0, 0x1\n\
b _08117402\n\
.align 2, 0\n\
_081173E8: .4byte 0x02019000\n\
_081173EC:\n\
subs r0, r4, 0x1\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
ldr r0, _08117408 @ =0x02019000\n\
adds r0, 0x12\n\
adds r1, r4, r0\n\
ldrb r0, [r1]\n\
cmp r0, 0x2\n\
bhi _08117428\n\
ldrb r0, [r1]\n\
adds r0, 0x2\n\
_08117402:\n\
add r0, sp\n\
ldrb r0, [r0]\n\
b _0811742A\n\
.align 2, 0\n\
_08117408: .4byte 0x02019000\n\
_0811740C:\n\
ldr r1, _08117424 @ =0x02019000\n\
adds r0, r3, 0\n\
adds r0, 0x8\n\
adds r0, r2, r0\n\
ldr r1, [r1, 0x8]\n\
ldr r0, [r0]\n\
ands r1, r0\n\
cmp r1, 0\n\
bne _08117428\n\
mov r0, sp\n\
ldrb r0, [r0, 0x4]\n\
b _0811742A\n\
.align 2, 0\n\
_08117424: .4byte 0x02019000\n\
_08117428:\n\
movs r0, 0\n\
_0811742A:\n\
add sp, 0x8\n\
pop {r4}\n\
pop {r1}\n\
bx r1\n\
.syntax divided\n");
}
#endif

void sub_8117434(void)
{
    s32 x1;
    s32 x2;
    REG_BG2PA = eRoulette->var2C.a;
    REG_BG2PB = eRoulette->var2C.b;
    REG_BG2PC = eRoulette->var2C.c;
    REG_BG2PD = eRoulette->var2C.d;
    x1 = 0x7400 - eRoulette->var2C.a * (gSpriteCoordOffsetX + 0x74)
                - eRoulette->var2C.b * (gSpriteCoordOffsetY + 0x50);
    x2 = 0x5400 - eRoulette->var2C.c * (gSpriteCoordOffsetX + 0x74)
                - eRoulette->var2C.d * (gSpriteCoordOffsetY + 0x50);
    REG_BG2X_L = x1;
    REG_BG2X_H = (x1 & 0x0fff0000) >> 16;
    REG_BG2Y_L = x2;
    REG_BG2Y_H = (x2 & 0x0fff0000) >> 16;
}

s16 sub_81174C4(s16 r0, s16 r1)
{
    s32 t = r0 * r1;
    t /= 0x100;
    return t;
}

s16 sub_81174E0(s16 r0)
{
    s32 t = 0x10000;
    return t / r0;
}

void sub_81174F8(u8 r0)
{
    DisplayYesNoMenu(0x14, 0x8, 0x1);
    sub_814AAF8(0x2D9E);
    DoYesNoFuncWithChoice(r0, &gUnknown_083F8EB4);
}

void sub_8117528(u8 taskid)
{
    if (!gPaletteFade.active)
    {
        SetVBlankCallback(NULL);
        SetMainCallback2(&sub_8115384);
        DestroyTask(taskid);
    }
}

void sub_811755C(u8 taskid)
{
    Menu_EraseWindowRect(0x14, 0x8, 0x1A, 0xD);
    Menu_EraseScreen();
    BeginNormalPaletteFade(-0x1, 0x0, 0x0, 0x10, 0x0);
    gPaletteFade.delayCounter = gPaletteFade.multipurpose2;
    UpdatePaletteFade();
    gTasks[taskid].func = &sub_8117528;
}

void sub_81175C0(u8 taskid)
{
    Menu_EraseScreen();
    ScriptContext2_Disable();
    DestroyTask(taskid);
}

void sub_81175DC(u8 taskid)
{
    gTasks[taskid].data[0]++;
    if (!(gMain.newKeys & (A_BUTTON | B_BUTTON)) && gTasks[taskid].data[0] < 0x3D)
        return;
    gSpecialVar_0x8004 = 0x1;
    Menu_EraseScreen();
    ScriptContext2_Disable();
    DestroyTask(taskid);
}

void sub_8117630(u8 taskid)
{
    u32 temp = gUnknown_083F8DF0[(gSpecialVar_0x8004 & 0x1) + (gSpecialVar_0x8004 >> 0x7 << 0x1)];
    ConvertIntToDecimalStringN(gStringVar1, temp, 0x2, 0x1);
    StringExpandPlaceholders(gStringVar4, &gUnknown_081C40DF);
    Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
    Menu_PrintText(gStringVar4, 0x1, 0xF);
    gTasks[taskid].func = &sub_81174F8;
}

void Task_Roulette_0(u8 taskid)
{
    s32 temp;
    ConvertIntToDecimalStringN(gStringVar1, gTasks[taskid].data[0xD], 0x1, 0x4);
    StringExpandPlaceholders(gStringVar4, gOtherText_Coins);
    Menu_DrawStdWindowFrame(0x0, 0x0, 0x9, 0x3);
    MenuPrint_RightAligned(gStringVar4, 0x9, 0x1);
    temp = gUnknown_083F8DF0[(gSpecialVar_0x8004 & 0x1) + (gSpecialVar_0x8004 >> 0x7 << 0x1)];
    ConvertIntToDecimalStringN(gStringVar1, temp, 0x2, 0x1);
    if (gTasks[taskid].data[0xD] >= temp)
    {
        if ((gSpecialVar_0x8004 & 0x80) && (gSpecialVar_0x8004 & 0x1))
        {
            Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
            Menu_PrintText(&gUnknown_081C4139, 0x1, 0xF);
            sub_8116C34(taskid , &sub_8117630, 0xFFFF, 0x3);
        }
        else
        {
            StringExpandPlaceholders(gStringVar4, &gUnknown_081C40DF);
            Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
            Menu_PrintText(gStringVar4, 0x1, 0xF);
            gTasks[taskid].func = &sub_81174F8;
        }
    }
    else
    {
        StringExpandPlaceholders(gStringVar4, &gUnknown_081C411C);
        Menu_DrawStdWindowFrame(0x0, 0xE, 0x1D, 0x13);
        Menu_PrintText(gStringVar4, 0x1, 0xF);
        gTasks[taskid].func = &sub_81175DC;
        gTasks[taskid].data[0xD] = 0x0;
        gTasks[taskid].data[0x0] = 0x0;
    }
}

void PlayRoulette(void)
{
    u8 taskid;
    ScriptContext2_Enable();
    taskid = CreateTask(&Task_Roulette_0, 0x0);
    gTasks[taskid].data[0xD] = gSaveBlock1.coins;
}

void sub_8117838(u8 r0)
{
    if (!r0)
    {
        FreeAllSpritePalettes();
        LoadSpritePalettes(&gUnknown_083F9E30);
        LZ77UnCompWram(gUnknown_083F92A8, ewram17000);
        LZ77UnCompWram(gUnknown_083F90FC, ewram17E00);
        LZ77UnCompWram(gUnknown_083F9D3C, ewram18000);
    }
    else
    {
        FreeAllSpritePalettes();
    }
}

u8 sub_8117890(const struct SpriteTemplate *r0, u8 r1, u16 *r2)
{
    u16 temp;
    u8 spriteid = CreateSprite(r0, 0x74, 0x50, r0->oam->y);
    gSprites[spriteid].data[0]            = *r2;
    gSprites[spriteid].data[1]            = r1;
    gSprites[spriteid].coordOffsetEnabled = TRUE;
    gSprites[spriteid].animPaused         = TRUE;
    gSprites[spriteid].affineAnimPaused   = TRUE;
    temp = *r2;
    *r2 += 0x1E;
    if (*r2 > 0x167)
        *r2 = temp - 0x14A;
    return spriteid;
}

void sub_8117900(void)
{
    u8 i, j;
    u8 spriteid;
    struct SpriteSheet s;
    LZ77UnCompWram(gUnknown_083F9F54.data, gSharedMem);
    s.data = gSharedMem;
    s.size = gUnknown_083F9F54.size;
    s.tag  = gUnknown_083F9F54.tag;
    LoadSpriteSheet(&s);
    LZ77UnCompWram(gUnknown_083F9F5C.data, gSharedMem);
    s.data = gSharedMem;
    s.size = gUnknown_083F9F5C.size;
    s.tag  = gUnknown_083F9F5C.tag;
    LoadSpriteSheet(&s);
    for (i = 0; i < 0x3; i++)
    {
        u8 o = i * 0x18;
        for (j = 0; j < 0x4; j++)
        {
            spriteid = eRoulette->var3C[0x1D + i * 0x4 + j] =
                CreateSprite(&gSpriteTemplate_83FA07C[j], j * 0x18 + 0x94, o + 0x5C, 0x1E);
            gSprites[spriteid].animPaused = TRUE;
            o += 0x18;
            if (o > 0x47)
                o = 0x0;
        }
    }
    for (i = 0x0; i < 0x4; i++)
    {
        spriteid = eRoulette->var3C[0x29 + i] =
            CreateSprite(&gSpriteTemplate_83F9FD4[i], (s16)(i * 0x18 + 0x94), 0x46, 0x1E);
        gSprites[spriteid].animPaused = TRUE;
    }
    for (i = 0x0; i < 0x3; i++)
    {
        spriteid = eRoulette->var3C[0x2D + i] =
            CreateSprite(&gSpriteTemplate_83FA034[i], 0x7E, (s16)(i * 0x18 + 0x5C), 0x1E);
        gSprites[spriteid].animPaused = TRUE;
    }
}

void unref_sub_8117A74(void) //destroy all sprites at 0x1D
{
    u8 i;
    for (i = 0x0; i < 0xC; i++)
        DestroySprite(&gSprites[eRoulette->var3C[0x1D + i]]);
}

void sub_8117AA8(u8 r0, u8 r1)
{
    u8 i;
    switch(r0)
    {
    case 0x1:
        for (i = 0x0; i < 0x13; i++)
            gSprites[eRoulette->var3C[0x1D + i]].invisible = TRUE;
        break;
    case 0x0:
        for (i = 0x0; i < 0xC; i++)
        {
            if (!(eRoulette->var08 & gUnknown_083F8D90[i].var04))
                gSprites[eRoulette->var3C[0x1D + i]].invisible = FALSE;
            else if(!(gUnknown_083F8D90[i].var02 == r1))
                gSprites[eRoulette->var3C[0x1D + i]].invisible = TRUE;
            else
                gSprites[eRoulette->var3C[0x1D + i]].invisible = FALSE;
        }
        for ( ; i < 0x13; i++)
            gSprites[eRoulette->var3C[0x1D + i]].invisible = FALSE;
        break;
    }
}

void sub_8117BBC(void)
{
    u8 i;
    for (i = 0x0; i < 0x6; i++)
    {
        eRoulette->var3C[0x31 + i] = CreateSprite(&gSpriteTemplate_83FA40C, 0x74, 0x14, 0xA);
        gSprites[eRoulette->var3C[0x31 + i]].invisible    = TRUE;
        gSprites[eRoulette->var3C[0x31 + i]].data[0]        = 0x1;
        gSprites[eRoulette->var3C[0x31 + i]].callback     = &sub_81184CC;
        gSprites[eRoulette->var3C[0x31 + i]].oam.priority = 0x1;
        StartSpriteAnim(&gSprites[eRoulette->var3C[0x31 + i]], 0x8);
    }
}

void sub_8117C60(u8 r0, u8 r1)
{
    u8 i = 0x0;
    if (r0)
    {
        for ( ; i < 0x6; i++)
            gSprites[eRoulette->var3C[0x31 + i]].invisible = TRUE;
    }
    else
    {
        for ( ; i < 0x6; i++)
        {
            if (!(eRoulette->var0C[i]) || (i == r1))
            {
                gSprites[eRoulette->var3C[0x31 + i]].invisible = TRUE;
            }
            else
            {
                gSprites[eRoulette->var3C[0x31 + i]].invisible = FALSE;
                gSprites[eRoulette->var3C[0x31 + i]].pos1.x    = (gUnknown_083F8C00[eRoulette->var0C[i]].var03 + 0x1) * 0x8 + 0x4;
                gSprites[eRoulette->var3C[0x31 + i]].pos1.y    = (gUnknown_083F8C00[eRoulette->var0C[i]].var04 + 0x1) * 0x8 + 0x3;
            }
        }
    }
}

void sub_8117D68(u8 r0)
{
    if (!r0)
    {
        gSprites[eRoulette->var3C[0x30]].invisible = TRUE;
    }
    else
    {
        gSprites[eRoulette->var3C[0x30]].invisible = FALSE;
        gSprites[eRoulette->var3C[0x30]].pos1.x    = (gUnknown_083F8C00[r0].var03 + 0x2) * 0x8;
        gSprites[eRoulette->var3C[0x30]].pos1.y    = (gUnknown_083F8C00[r0].var04 + 0x2) * 0x8;
    }
}

void sub_8117DF4(void)
{
    u8 i, j;
    u16 k;
    struct SpriteSheet s;
    LZ77UnCompWram(gUnknown_083F9EE8.data, gSharedMem);
    s.data = gSharedMem;
    s.size = gUnknown_083F9EE8.size;
    s.tag  = gUnknown_083F9EE8.tag;
    LoadSpriteSheet(&s);
    k = 0xF;
    for (i = 0x0; i < 0x3; i++)
    {
        for (j = 0x0; j < 0x4; j++)
        {
            u8 spriteid;
            spriteid =
                eRoulette->var3C[0x7 + i * 0x4 + j] = sub_8117890(&gSpriteTemplate_83FA0DC[i * 0x4 + j], 0x28, &k);
            gSprites[spriteid].animPaused       = TRUE;
            gSprites[spriteid].affineAnimPaused = TRUE;
        }
    }
}

#ifdef NONMATCHING
void sub_8117E98(struct Sprite *sprite)
{
    struct OamMatrix *m;
    u8 p;
    u16 angle;
    s16 cos, sin, z;
    u32 cos32;
    angle = eRoulette->var24 + sprite->data[0x0];
    z = angle;
    if (z > 359)
        angle = z - 360;
    sin = Sin2(angle);
    cos = Cos2(angle);
    sprite->pos2.x =  sin * sprite->data[0x1] >> 0xC;
    sprite->pos2.y = -cos * sprite->data[0x1] >> 0xC;
    p = sprite->oam.matrixNum;
    sin = sin / 0x10;
    m = &gOamMatrices[p];
    cos32 = cos / 0x10;

    m->d =  cos32;
    m->a =  cos32;
    m->b =  sin;
    m->c = -sin;
}
#else
__attribute__((naked))
void sub_8117E98(struct Sprite *r0)
{
asm(".syntax unified\n\
push {r4-r6,lr}\n\
adds r6, r0, 0\n\
ldr r0, _08117F1C @ =0x02019000\n\
ldrh r1, [r6, 0x2E]\n\
ldrh r0, [r0, 0x24]\n\
adds r1, r0\n\
lsls r1, 16\n\
lsrs r4, r1, 16\n\
asrs r1, 16\n\
ldr r0, _08117F20 @ =0x00000167\n\
cmp r1, r0\n\
ble _08117EB8\n\
ldr r2, _08117F24 @ =0xfffffe98\n\
adds r0, r1, r2\n\
lsls r0, 16\n\
lsrs r4, r0, 16\n\
_08117EB8:\n\
adds r0, r4, 0\n\
bl Sin2\n\
lsls r0, 16\n\
lsrs r5, r0, 16\n\
adds r0, r4, 0\n\
bl Cos2\n\
lsls r2, r5, 16\n\
asrs r2, 16\n\
movs r3, 0x30\n\
ldrsh r1, [r6, r3]\n\
muls r1, r2\n\
asrs r1, 12\n\
strh r1, [r6, 0x24]\n\
lsls r0, 16\n\
asrs r4, r0, 16\n\
negs r1, r4\n\
movs r3, 0x30\n\
ldrsh r0, [r6, r3]\n\
muls r0, r1\n\
asrs r0, 12\n\
strh r0, [r6, 0x26]\n\
ldrb r0, [r6, 0x3]\n\
lsls r0, 26\n\
lsrs r3, r0, 27\n\
cmp r2, 0\n\
bge _08117EF2\n\
adds r2, 0xF\n\
_08117EF2:\n\
lsls r0, r2, 12\n\
lsrs r5, r0, 16\n\
ldr r1, _08117F28 @ =gOamMatrices\n\
lsls r0, r3, 3\n\
adds r1, r0, r1\n\
adds r0, r4, 0\n\
cmp r0, 0\n\
bge _08117F04\n\
adds r0, 0xF\n\
_08117F04:\n\
asrs r0, 4\n\
strh r0, [r1, 0x6]\n\
strh r0, [r1]\n\
strh r5, [r1, 0x2]\n\
lsls r0, r5, 16\n\
asrs r0, 16\n\
negs r0, r0\n\
strh r0, [r1, 0x4]\n\
pop {r4-r6}\n\
pop {r0}\n\
bx r0\n\
.align 2, 0\n\
_08117F1C: .4byte 0x02019000\n\
_08117F20: .4byte 0x00000167\n\
_08117F24: .4byte 0xfffffe98\n\
_08117F28: .4byte gOamMatrices\n\
.syntax divided\n");
}
#endif

void sub_8117F2C(void)
{
    u8 i;
    for (i = 0x0; i < 0x5; i++)
    {
        struct SpriteSheet s;
        LZ77UnCompWram(gUnknown_083FA21C[i].data, gSharedMem);
        s.data = gSharedMem;
        s.size = gUnknown_083FA21C[i].size;
        s.tag  = gUnknown_083FA21C[i].tag;
        LoadSpriteSheet(&s);
    }
    eRoulette->var3C[0x14] = CreateSprite(&gSpriteTemplate_83FA2B0, 0xD0, 0x10, 0x4);
    gSprites[eRoulette->var3C[0x14]].animPaused = TRUE;
    for (i = 0x0; i < 0x4; i++)
    {
        eRoulette->var3C[0x15 + i] = CreateSprite(&gSpriteTemplate_83FA2C8, (s16)(i * 0x8 + 0xC4), 0x18, 0x0);
        gSprites[eRoulette->var3C[0x15 + i]].invisible = TRUE;
        gSprites[eRoulette->var3C[0x15 + i]].animPaused = TRUE;
    }
    eRoulette->var3C[0x19] = CreateSprite(&gSpriteTemplate_83FA2E0, 0x78, 0x44, 0x4);
    gSprites[eRoulette->var3C[0x19]].animPaused = TRUE;
    for (i = 0x0; i < 0x3; i++)
    {
        eRoulette->var3C[0x1A + i] = CreateSprite(&gSpriteTemplate_83FA2F8, (s16)(i * 0x10 + 0xC0), 0x24, 0x4);
        gSprites[eRoulette->var3C[0x1A + i]].invisible = TRUE;
        gSprites[eRoulette->var3C[0x1A + i]].animPaused = TRUE;
    }
    eRoulette->var3C[0x30] = CreateSprite(&gSpriteTemplate_83FA310, 0x98, 0x60, 0x9);
    gSprites[eRoulette->var3C[0x30]].oam.priority = 0x1;
    gSprites[eRoulette->var3C[0x30]].animPaused = TRUE;
    gSprites[eRoulette->var3C[0x30]].invisible = TRUE;
}

void sub_81180F4(u16 r0)
{
    u8 i;
    u16 d = 1000;
    bool8 v = FALSE;
    for (i = 0x0; i < 0x4; i++)
    {
        u8 t = r0 / d;
        gSprites[eRoulette->var3C[0x15 + i]].invisible = TRUE;
        if (t > 0x0 || v || i == 0x3)
        {
            gSprites[eRoulette->var3C[0x15 + i]].invisible = FALSE;
            gSprites[eRoulette->var3C[0x15 + i]].oam.tileNum =
                gSprites[eRoulette->var3C[0x15 + i]].sheetTileStart
                + (*gSprites[eRoulette->var3C[0x15 + i]].anims + t)->type;
            v = TRUE;
        }
        r0 = r0 % d;
        d = d / 10;
    }
}

#ifdef NONMATCHING
u8 sub_81181E8(u8 r0)
{
    u8 t[0x5];
    memcpy(&t, &gUnknown_083FA608, 0x5);
    if (r0 > 0x13)
        r0 = 0x0;
    switch(gUnknown_083F8C00[r0].var01_0)
    {
    case 0x3:
        r0 = r0 / 0x5 - 0x1;
        if ((u32)eRoulette->var16[r0] < 0x4)
            return t[0x1 + eRoulette->var16[r0]]; // couldn't recreate redundant loads
        break;
    case 0x4:
        r0 = r0 - 0x1;
        if (eRoulette->var12[r0] < 0x3)
            return t[0x2 + eRoulette->var12[r0]];
        break;
    case 0xC:
        if (!(eRoulette->var08 & gUnknown_083F8C00[r0].var08))
            return t[0x4];
        break;
    }
    return 0x0;
}
#else
__attribute__((naked))
u8 sub_81181E8(u8 r0)
{
asm(".syntax unified\n\
push {r4,lr}\n\
sub sp, 0x8\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
ldr r1, _08118220 @ =gUnknown_083FA608\n\
mov r0, sp\n\
movs r2, 0x5\n\
bl memcpy\n\
cmp r4, 0x13\n\
bls _08118200\n\
movs r4, 0\n\
_08118200:\n\
ldr r3, _08118224 @ =gUnknown_083F8C00\n\
lsls r0, r4, 2\n\
adds r0, r4\n\
lsls r2, r0, 2\n\
adds r0, r2, r3\n\
ldrb r0, [r0, 0x1]\n\
lsls r0, 28\n\
lsrs r0, 28\n\
cmp r0, 0x4\n\
beq _08118254\n\
cmp r0, 0x4\n\
bgt _08118228\n\
cmp r0, 0x3\n\
beq _0811822E\n\
b _08118290\n\
.align 2, 0\n\
_08118220: .4byte gUnknown_083FA608\n\
_08118224: .4byte gUnknown_083F8C00\n\
_08118228:\n\
cmp r0, 0xC\n\
beq _08118274\n\
b _08118290\n\
_0811822E:\n\
adds r0, r4, 0\n\
movs r1, 0x5\n\
bl __udivsi3\n\
subs r0, 0x1\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
ldr r0, _08118250 @ =0x02019000\n\
adds r0, 0x16\n\
adds r1, r4, r0\n\
ldrb r0, [r1]\n\
cmp r0, 0x3\n\
bhi _08118290\n\
ldrb r0, [r1]\n\
adds r0, 0x1\n\
b _0811826A\n\
.align 2, 0\n\
_08118250: .4byte 0x02019000\n\
_08118254:\n\
subs r0, r4, 0x1\n\
lsls r0, 24\n\
lsrs r4, r0, 24\n\
ldr r0, _08118270 @ =0x02019000\n\
adds r0, 0x12\n\
adds r1, r4, r0\n\
ldrb r0, [r1]\n\
cmp r0, 0x2\n\
bhi _08118290\n\
ldrb r0, [r1]\n\
adds r0, 0x2\n\
_0811826A:\n\
add r0, sp\n\
ldrb r0, [r0]\n\
b _08118292\n\
.align 2, 0\n\
_08118270: .4byte 0x02019000\n\
_08118274:\n\
ldr r1, _0811828C @ =0x02019000\n\
adds r0, r3, 0\n\
adds r0, 0x8\n\
adds r0, r2, r0\n\
ldr r1, [r1, 0x8]\n\
ldr r0, [r0]\n\
ands r1, r0\n\
cmp r1, 0\n\
bne _08118290\n\
mov r0, sp\n\
ldrb r0, [r0, 0x4]\n\
b _08118292\n\
.align 2, 0\n\
_0811828C: .4byte 0x02019000\n\
_08118290:\n\
movs r0, 0\n\
_08118292:\n\
add sp, 0x8\n\
pop {r4}\n\
pop {r1}\n\
bx r1\n\
.syntax divided\n");
}
#endif

void sub_811829C(u8 r0)
{
    struct Sprite *s = &gSprites[eRoulette->var3C[0x19]];
    s->animCmdIndex  = sub_81181E8(r0);
    s->oam.tileNum   =
        s->sheetTileStart
        + (*s->anims + s->animCmdIndex)->type;
}

void sub_81182F8(u8 r0)
{
    u8 i;
    u8 t = 0x0;
    if (eRoulette->var19 == 0x1)
        t = 0x2;
    switch(r0)
    {
    case 0x6:
        for (i = 0x0; i < 0x3; i++)
        {
            gSprites[eRoulette->var3C[0x1A + i]].invisible = FALSE;
            gSprites[eRoulette->var3C[0x1A + i]].oam.tileNum =
                gSprites[eRoulette->var3C[0x1A + i]].sheetTileStart
                + (*gSprites[eRoulette->var3C[0x1A + i]].anims)->type;
        }
        break;
    case 0x5:
        gSprites[eRoulette->var3C[0x1C]].oam.tileNum =
            gSprites[eRoulette->var3C[0x1C]].sheetTileStart
            + (*gSprites[eRoulette->var3C[0x1C]].anims + t + 0x1)->type;
        break;
    case 0x4:
        gSprites[eRoulette->var3C[0x1C]].oam.tileNum =
            gSprites[eRoulette->var3C[0x1C]].sheetTileStart
            + (*gSprites[eRoulette->var3C[0x1C]].anims + t + 0x2)->type;
        break;
    case 0x3:
        gSprites[eRoulette->var3C[0x1B]].oam.tileNum =
            gSprites[eRoulette->var3C[0x1B]].sheetTileStart
            + (*gSprites[eRoulette->var3C[0x1B]].anims + t + 0x1)->type;
        break;
    case 0x2:
        gSprites[eRoulette->var3C[0x1B]].oam.tileNum =
            gSprites[eRoulette->var3C[0x1B]].sheetTileStart
            + (*gSprites[eRoulette->var3C[0x1B]].anims + t + 0x2)->type;
        break;
    case 0x1:
        gSprites[eRoulette->var3C[0x1A]].oam.tileNum =
            gSprites[eRoulette->var3C[0x1A]].sheetTileStart
            + (*gSprites[eRoulette->var3C[0x1A]].anims + t + 0x1)->type;
        break;
    case 0x0:
    default:
        for (i = 0x0; i < 0x3; i++)
        {
            gSprites[eRoulette->var3C[0x1A + i]].oam.tileNum =
                gSprites[eRoulette->var3C[0x1A + i]].sheetTileStart
                + (*gSprites[eRoulette->var3C[0x1A + i]].anims + t + 2)->type;
        }
    }
}

void sub_81184CC(struct Sprite *sprite)
{
    sprite->pos2.x = eRoulette->var26;
}

void sub_81184D8(void)
{
    u8 spriteid;
    struct SpriteSheet s;
    LZ77UnCompWram(gUnknown_083FA42C.data, gSharedMem);
    s.data = gSharedMem;
    s.size = gUnknown_083FA42C.size;
    s.tag  = gUnknown_083FA42C.tag;
    LoadSpriteSheet(&s);
    spriteid = CreateSprite(&gSpriteTemplate_83FA434, 0x74, 0x50, 0x51);
    gSprites[spriteid].data[0]            = eRoulette->var24;
    gSprites[spriteid].data[1]            = 0x0;
    gSprites[spriteid].animPaused         = TRUE;
    gSprites[spriteid].affineAnimPaused   = TRUE;
    gSprites[spriteid].coordOffsetEnabled = TRUE;
}

void sub_8118554(struct Sprite *sprite)
{
    u32 t = sprite->oam.matrixNum;
    struct OamMatrix *m = &gOamMatrices[0];
    m[t].d = eRoulette->var2C.a;
    m[t].a = eRoulette->var2C.a;
    m[t].b = eRoulette->var2C.b;
    m[t].c = eRoulette->var2C.c;
}

void sub_811857C(void)
{
    u8 i;
    for (i = 0x0; i < 0x6; i++)
    {
        u8 spriteid =
            eRoulette->var3C[i] = CreateSprite(&gSpriteTemplate_83FA40C, 0x74, 0x50, 0x39 - i);
        if (spriteid != 0x40)
        {
            gSprites[eRoulette->var3C[i]].invisible          = TRUE;
            gSprites[eRoulette->var3C[i]].coordOffsetEnabled = TRUE;
        }
    }
}

void sub_81185E8(void)
{
    u8 t = eRoulette->var3C[0x0];
    u8 i;
    for (i = 0x0; i < 0x6; i++)
    {
        u8 j;
        gSprites[t].invisible = TRUE;
        gSprites[t].callback = &SpriteCallbackDummy;
        StartSpriteAnim(&gSprites[t], 0x0);
        for (j = 0x0; j < 0x8; j++)
            gSprites[t].data[j] = 0x0;
        t++;
    }
}

s16 sub_811866C(struct Sprite *sprite)
{
    if (eRoulette->var24 > sprite->data[0x3])
    {
        sprite->data[0x6] = 360 - eRoulette->var24 + sprite->data[0x3];
        if (sprite->data[0x6] > 359)
            sprite->data[0x6] -=360;
    }
    else
        sprite->data[0x6] = sprite->data[0x3] - eRoulette->var24;
    return sprite->data[0x6];
}

u8 sub_81186B8(struct Sprite *sprite)
{
    eRoulette->var7E = (u8)(((float)(s16)sub_811866C(sprite)) / 30.0f);
    return eRoulette->var7E;
}

s16 sub_81186E8(struct Sprite *sprite)
{
    s16 t = sub_811866C(sprite) % 30;
    u16 z;
    if (t == 0xE)
    {
        z = 0x0;
        return sprite->data[0x2] = z;
    }
    else if (t > 0xD)
    {
        z = (u16)(0x2B - t);
        return sprite->data[0x2] = z;
    }
    else
    {
        z = (u16)(0xE - t);
        return sprite->data[0x2] = z;
    }
}

void sub_8118724(struct Sprite *sprite)
{
    s16 sin, cos;
    eRoulette->var8C += eRoulette->var90;
    eRoulette->var88 += eRoulette->var8C;
    if (eRoulette->var88 >= 360)
        eRoulette->var88 -= 360.0f;
    else
        if (eRoulette->var88 < 0.0f)
            eRoulette->var88 += 360.0f;
    sprite->data[0x3] = eRoulette->var88;
    eRoulette->var98 += eRoulette->var9C;
    eRoulette->var94 += eRoulette->var98;
    sprite->data[0x4] = eRoulette->var94;
    sin = Sin2(sprite->data[0x3]);
    cos = Cos2(sprite->data[0x3]);
    sprite->pos2.x =  sin * sprite->data[0x4] >> 0xC;
    sprite->pos2.y = -cos * sprite->data[0x4] >> 0xC;
    if (IsSEPlaying())
    {
        m4aMPlayPanpotControl(&gMPlay_SE1, 0xFFFF, sprite->pos2.x);
        m4aMPlayPanpotControl(&gMPlay_SE2, 0xFFFF, sprite->pos2.x);
    }
}

void sub_8118834(struct Sprite *sprite)
{
    s16 sin, cos;
    sprite->data[0x3] = eRoulette->var24 + sprite->data[0x6];
    if (sprite->data[0x3] > 359)
        sprite->data[0x3] -= 360;
    sin = Sin2(sprite->data[0x3]);
    cos = Cos2(sprite->data[0x3]);
    sprite->pos2.x =  sin * sprite->data[0x4] >> 0xC;
    sprite->pos2.y = -cos * sprite->data[0x4] >> 0xC;
    sprite->pos2.y += gSpriteCoordOffsetY;
}

void sub_811889C(struct Sprite *sprite)
{
    sub_8118724(sprite);
    sprite->data[0x2]++;
    if ((u16)(sprite->data[0x4] + 0x84) > 0xD4)
        sprite->invisible = TRUE;
    else
        sprite->invisible = FALSE;
    if (!(sprite->data[0x2] < 30))
    {
        if (!sprite->data[0x0])
        {
            if (eRoulette->var94 <= eRoulette->varA0 - 2.0f)
            {
                eRoulette->var7D = 0xFF;
                eRoulette->var03_7 = 0x0;
                StartSpriteAnim(sprite, sprite->animCmdIndex + 0x3);
                sub_81186B8(sprite);
                sprite->data[0x4] = 30;
                sub_811866C(sprite);
                sprite->data[0x6] = (sprite->data[0x6] / 30) * 30 + 0xF;
                sprite->callback = &sub_8118834;
                m4aSongNumStartOrChange(0x47);
                eRoulette->var9C = eRoulette->var98 = 0.0f;
                eRoulette->var8C = -1.0f;
            }
        }
        else
        {
            if (eRoulette->var94 >= eRoulette->varA0 - 2.0f)
            {
                eRoulette->var7D = 0xFF;
                eRoulette->var03_7 = 0x0;
                StartSpriteAnim(sprite, sprite->animCmdIndex + 0x3);
                sub_81186B8(sprite);
                sprite->data[0x4] = 30;
                sub_811866C(sprite);
                sprite->data[0x6] = (sprite->data[0x6] / 30) * 30 + 0xF;
                sprite->callback = &sub_8118834;
                m4aSongNumStartOrChange(0x47);
                eRoulette->var9C = eRoulette->var98 = 0.0f;
                eRoulette->var8C = -1.0f;
            }
        }
    }
}

void sub_81189A8(struct Sprite *sprite)
{
    float f0, f1, f2;
    struct StructgUnknown_083F8DF4 *p;
    sub_8118724(sprite);
    switch(sprite->data[0x3])
    {
    case 0:
        if (sprite->data[0x0] != 0x1)
        {
            f0 = ((float)sprite->data[0x7]);
            p = &gUnknown_083F8DF4[0];
            f1 = (f0 * ((float)(s32)p[eRoulette->var04_0].var01) + (float)((s32)p[eRoulette->var04_0].var02 - 0x1));
            f2 = (f0 / ((float)(s32)p[eRoulette->var04_0].var0C));
        }
        else
        {
            return;
        }
        break;
    case 180:
        if (sprite->data[0x0] != 0x0)
        {
            f0 = ((float)sprite->data[0x7]);
            p = &gUnknown_083F8DF4[0];
            f1 = (f0 * ((float)(s32)p[eRoulette->var04_0].var01) + (float)((s32)p[eRoulette->var04_0].var02 - 0x1));
            f2 = -(f0 / ((float)(s32)p[eRoulette->var04_0].var0C));
        }
        else
        {
            return;
        }
        break;
        default: return;
    }
    eRoulette->varA0 = eRoulette->var94;
    eRoulette->var98 = f2;
    eRoulette->var9C = -((f2 + f2) / f1 + (2.0f / (f1 * f1)));
    eRoulette->var8C = 0.0f;
    sprite->animPaused    = FALSE;
    sprite->animNum       = 0x0;
    sprite->animBeginning = TRUE;
    sprite->animEnded     = FALSE;
    sprite->callback      = &sub_811889C;
    sprite->data[0x2]     = 0x0;
}

void sub_8118B30(struct Sprite *sprite)
{
    sprite->pos2.y = (s32)(((float)sprite->data[0x2]) * 0.05f * ((float)sprite->data[0x2])) - 45;
    sprite->data[0x2]++;
    if (sprite->data[0x2] > 29 && sprite->pos2.y >= 0)
    {
        eRoulette->var7D   = 0xFF;
        eRoulette->var03_7 = FALSE;
        StartSpriteAnim(sprite, sprite->animCmdIndex + 0x3);
        sub_81186B8(sprite);
        sprite->data[0x4] = 30;
        sub_811866C(sprite);
        sprite->data[0x6] = (sprite->data[0x6] / 30) * 30 + 0xF;
        sprite->callback  = &sub_8118834;
        m4aSongNumStartOrChange(0x47);
        eRoulette->var03_6 = TRUE;
    }
}

void sub_8118BD8(struct Sprite *sprite)
{
    if (sprite->data[0x2]++ < 45)
    {
        sprite->pos2.y--;
        if(sprite->data[0x2] == 45)
        {
            if (gSprites[eRoulette->var3C[0x37]].animCmdIndex == 0x1)
                sprite->pos2.y++;
        }
    }
    else
    {
        if (sprite->data[0x2] < sprite->data[0x7])
        {
            if (gSprites[eRoulette->var3C[0x37]].animDelayCounter == 0x0)
            {
                if (gSprites[eRoulette->var3C[0x37]].animCmdIndex == 0x1)
                    sprite->pos2.y++;
                else
                    sprite->pos2.y--;
            }
        }
        else
        {
            sprite->animPaused    = FALSE;
            sprite->animNum       = 0x1;
            sprite->animBeginning = TRUE;
            sprite->animEnded     = FALSE;
            sprite->data[0x2]     = 0x0;
            sprite->callback      = &sub_8118B30;
            m4aSongNumStart(0x3D);
        }
    }
}

void sub_8118CAC(struct Sprite *sprite)
{
    sub_8118724(sprite);
    switch(sprite->data[0x3])
    {
    case 90:
        if (sprite->data[0x0] != 0x1)
        {
            sprite->callback  = &sub_8118BD8;
            sprite->data[0x2] = 0x0;
        }
        break;
    case 270:
        if (sprite->data[0x0] != 0x0)
        {
            sprite->callback  = &sub_8118BD8;
            sprite->data[0x2] = 0x0;
        }
        break;
    }
}

void sub_8118CEC(struct Sprite *sprite)
{
    sub_8118724(sprite);
    switch(eRoulette->var03_0)
    {
    default:
    case 0x0:
        sub_8119224(sprite);
        sprite->callback = &sub_81189A8;
        break;
    case 0x1:
        sub_81193D4(sprite);
        sprite->callback = &sub_8118CAC;
        break;
    }
}

void sub_8118D2C(struct Sprite *sprite)
{
    sub_8118724(sprite);
    if (sprite->data[0x2]-- == 0x10)
        eRoulette->var98 *= -1.0f;
    if (sprite->data[0x2] == 0x0)
    {
        if (!sprite->data[0x0])
        {
            eRoulette->var7D   = 0xFF;
            eRoulette->var03_7 = 0x0;
            StartSpriteAnim(sprite, sprite->animCmdIndex + 0x3);
            sub_81186B8(sprite);
            sprite->data[0x4] = 30;
            sub_811866C(sprite);
            sprite->data[0x6] = (sprite->data[0x6] / 30) * 30 + 15;
            sprite->callback  = &sub_8118834;
            m4aSongNumStartOrChange(0x47);
        }
        else
        {
            sprite->animPaused = TRUE;
            m4aSongNumStart(0x38);
            sub_811952C(sprite);
        }
    }
}

#ifdef NONMATCHING
void sub_8118DE4(struct Sprite *sprite)
{
    sub_8118724(sprite);
    sprite->data[0x2] = 0x0;
    sub_81186B8(sprite);
    if (!(gUnknown_083F8D90[eRoulette->var7E].var04 & eRoulette->var08))
    {
        eRoulette->var7D   = 0xFF;
        eRoulette->var03_7 = 0x0;
        StartSpriteAnim(sprite, sprite->animCmdIndex + 0x3);
        sub_81186B8(sprite);
        sprite->data[0x4] = 30;
        sub_811866C(sprite);
        sprite->data[0x6] = (sprite->data[0x6] / 30) * 30 + 15;
        sprite->callback  = &sub_8118834;
        m4aSongNumStartOrChange(0x47);
    }
    else
    {
        u8 t;
        u32 z;
        m4aSongNumStart(0x38);
        if ((z = (Random() & 0x1)))
        {
            u32 o;
            eRoulette->var8C = 0.0f;
            o = (eRoulette->var7E + 0x1) % 0xC;
            t = o;
            eRoulette->var7F = o;
        }
        else
        {
            eRoulette->var8C = gUnknown_083F8DF4[eRoulette->var04_0].var1C * 2; // couldn't replicate loads
            t = (eRoulette->var7E + 0xB) % 0xC;
            eRoulette->var7F = eRoulette->var7E;
        }
        if (gUnknown_083F8D90[t].var04 & eRoulette->var08)
        {
            sprite->data[0x0] = 0x1;
            sprite->data[0x2] = (&gUnknown_083F8DF4[eRoulette->var04_0])->var02;
        }
        else
        {
            sprite->data[0x0] = gUnknown_083F8D90[t].var04 & eRoulette->var08;
            if (eRoulette->var04_0)
            {
                sprite->data[0x2] = (&gUnknown_083F8DF4[eRoulette->var04_0])->var01;
            }
            else
            {
                sprite->data[0x2] = (&gUnknown_083F8DF4[eRoulette->var04_0])->var02;
                if (z)
                {
                    eRoulette->var8C = 1.5f;
                }
                else
                {
                    eRoulette->var8C = -1.5f;
                }
            }
        }
        eRoulette->var98 = 0.085f;
        sprite->callback = &sub_8118D2C;
        sprite->data[0x1] = 0x5;
    }
}
#else
__attribute__((naked))
void sub_8118DE4(struct Sprite *sprite)
{
asm(".syntax unified\n\
push {r4-r7,lr}\n\
mov r7, r9\n\
mov r6, r8\n\
push {r6,r7}\n\
adds r7, r0, 0\n\
bl sub_8118724\n\
movs r0, 0\n\
strh r0, [r7, 0x32]\n\
adds r0, r7, 0\n\
bl sub_81186B8\n\
ldr r1, _08118E70 @ =gUnknown_083F8D90\n\
ldr r6, _08118E74 @ =gSharedMem + 0x19000\n\
movs r0, 0x7E\n\
adds r0, r6\n\
mov r8, r0\n\
ldrb r0, [r0]\n\
lsls r0, 3\n\
adds r1, 0x4\n\
adds r0, r1\n\
ldr r0, [r0]\n\
ldr r1, [r6, 0x8]\n\
ands r0, r1\n\
cmp r0, 0\n\
bne _08118E7C\n\
adds r1, r6, 0\n\
adds r1, 0x7D\n\
movs r0, 0xFF\n\
strb r0, [r1]\n\
ldrb r1, [r6, 0x3]\n\
movs r0, 0x7F\n\
ands r0, r1\n\
strb r0, [r6, 0x3]\n\
adds r0, r7, 0\n\
adds r0, 0x2B\n\
ldrb r1, [r0]\n\
adds r1, 0x3\n\
lsls r1, 24\n\
lsrs r1, 24\n\
adds r0, r7, 0\n\
bl StartSpriteAnim\n\
adds r0, r7, 0\n\
bl sub_81186B8\n\
movs r0, 0x1E\n\
strh r0, [r7, 0x36]\n\
adds r0, r7, 0\n\
bl sub_811866C\n\
movs r1, 0x3A\n\
ldrsh r0, [r7, r1]\n\
movs r1, 0x1E\n\
bl __divsi3\n\
lsls r0, 16\n\
asrs r0, 16\n\
lsls r1, r0, 4\n\
subs r1, r0\n\
lsls r1, 1\n\
adds r1, 0xF\n\
strh r1, [r7, 0x3A]\n\
ldr r0, _08118E78 @ =sub_8118834\n\
str r0, [r7, 0x1C]\n\
movs r0, 0x47\n\
bl m4aSongNumStartOrChange\n\
b _08118F74\n\
.align 2, 0\n\
_08118E70: .4byte gUnknown_083F8D90\n\
_08118E74: .4byte gSharedMem + 0x19000\n\
_08118E78: .4byte sub_8118834\n\
_08118E7C:\n\
movs r0, 0x38\n\
bl m4aSongNumStart\n\
bl Random\n\
movs r2, 0x1\n\
mov r9, r2\n\
mov r1, r9\n\
ands r1, r0\n\
mov r9, r1\n\
cmp r1, 0\n\
beq _08118EC0\n\
adds r1, r6, 0\n\
adds r1, 0x8C\n\
ldr r0, _08118EB8 @ =0x00000000\n\
str r0, [r1]\n\
mov r2, r8\n\
ldrb r0, [r2]\n\
adds r0, 0x1\n\
movs r1, 0xC\n\
bl __modsi3\n\
lsls r1, r0, 24\n\
lsrs r2, r1, 24\n\
adds r1, r6, 0\n\
adds r1, 0x7F\n\
strb r0, [r1]\n\
ldr r5, _08118EBC @ =gUnknown_083F8DF4\n\
b _08118EF4\n\
.align 2, 0\n\
_08118EB8: .4byte 0x00000000\n\
_08118EBC: .4byte gUnknown_083F8DF4\n\
_08118EC0:\n\
adds r4, r6, 0\n\
adds r4, 0x8C\n\
ldr r5, _08118F1C @ =gUnknown_083F8DF4\n\
ldrb r0, [r6, 0x4]\n\
lsls r0, 30\n\
lsrs r0, 25\n\
adds r1, r5, 0\n\
adds r1, 0x1C\n\
adds r0, r1\n\
ldr r1, [r0]\n\
adds r0, r1, 0\n\
bl __addsf3\n\
str r0, [r4]\n\
mov r0, r8\n\
ldrb r4, [r0]\n\
adds r0, r4, 0\n\
adds r0, 0xB\n\
movs r1, 0xC\n\
bl __modsi3\n\
lsls r0, 24\n\
lsrs r2, r0, 24\n\
adds r0, r6, 0\n\
adds r0, 0x7F\n\
strb r4, [r0]\n\
_08118EF4:\n\
ldr r0, _08118F20 @ =gUnknown_083F8D90\n\
lsls r1, r2, 3\n\
adds r0, 0x4\n\
adds r1, r0\n\
ldr r2, _08118F24 @ =gSharedMem + 0x19000\n\
ldr r1, [r1]\n\
ldr r0, [r2, 0x8]\n\
ands r1, r0\n\
cmp r1, 0\n\
beq _08118F28\n\
movs r0, 0x1\n\
strh r0, [r7, 0x2E]\n\
ldrb r0, [r2, 0x4]\n\
lsls r0, 30\n\
lsrs r0, 25\n\
adds r0, r5\n\
ldrb r0, [r0, 0x2]\n\
strh r0, [r7, 0x32]\n\
b _08118F64\n\
.align 2, 0\n\
_08118F1C: .4byte gUnknown_083F8DF4\n\
_08118F20: .4byte gUnknown_083F8D90\n\
_08118F24: .4byte gSharedMem + 0x19000\n\
_08118F28:\n\
strh r1, [r7, 0x2E]\n\
ldrb r1, [r2, 0x4]\n\
movs r0, 0x3\n\
ands r0, r1\n\
cmp r0, 0\n\
beq _08118F40\n\
lsls r0, r1, 30\n\
lsrs r0, 25\n\
adds r0, r5\n\
ldrb r0, [r0, 0x1]\n\
strh r0, [r7, 0x32]\n\
b _08118F64\n\
_08118F40:\n\
lsls r0, r1, 30\n\
lsrs r0, 25\n\
adds r0, r5\n\
ldrb r0, [r0, 0x2]\n\
strh r0, [r7, 0x32]\n\
mov r1, r9\n\
cmp r1, 0\n\
beq _08118F5C\n\
adds r1, r2, 0\n\
adds r1, 0x8C\n\
ldr r0, _08118F58 @ =0x3f000000\n\
b _08118F62\n\
.align 2, 0\n\
_08118F58: .4byte 0x3f000000\n\
_08118F5C:\n\
adds r1, r2, 0\n\
adds r1, 0x8C\n\
ldr r0, _08118F80 @ =0xbfc00000\n\
_08118F62:\n\
str r0, [r1]\n\
_08118F64:\n\
adds r1, r2, 0\n\
adds r1, 0x98\n\
ldr r0, _08118F84 @ =0x3dae147b\n\
str r0, [r1]\n\
ldr r0, _08118F88 @ =sub_8118D2C\n\
str r0, [r7, 0x1C]\n\
movs r0, 0x5\n\
strh r0, [r7, 0x30]\n\
_08118F74:\n\
pop {r3,r4}\n\
mov r8, r3\n\
mov r9, r4\n\
pop {r4-r7}\n\
pop {r0}\n\
bx r0\n\
.align 2, 0\n\
_08118F80: .4byte 0xbfc00000\n\
_08118F84: .4byte 0x3dae147b\n\
_08118F88: .4byte sub_8118D2C\n\
.syntax divided\n");
}
#endif

void sub_8118F8C(struct Sprite *sprite)
{
    sub_8118724(sprite);
    if (!(eRoulette->var8C > 0.5f))
    {
        sub_81186B8(sprite);
        if (sub_81186E8(sprite) == 0x0)
        {
            struct StructgUnknown_083F8DF4 *p;
            eRoulette->var90 = 0.0f;
            p = &gUnknown_083F8DF4[0];
            eRoulette->var8C -= ((float)p[eRoulette->var04_0].var03)
                / ((float)(s16)((&p[eRoulette->var04_0])->var04 + 0x1));
            sprite->data[0x1] = 0x4;
            sprite->callback = &sub_8118DE4;
        }
        else
        {
            if (eRoulette->var90 != 0.0f)
            {
                if (eRoulette->var8C < 0.0f)
                {
                    eRoulette->var90 = 0.0f;
                    eRoulette->var8C = 0.0f;
                    eRoulette->var98 /= 1.2;
                }
            }
        }
    }
}

void sub_8119088(struct Sprite *sprite)
{
    sub_8118724(sprite);
    if (!(eRoulette->var94 > 40.f))
    {
        eRoulette->var98 = - (        4.0f / (float)(u16)eRoulette->var86);
        eRoulette->var90 = - (eRoulette->var8C / (float)(u16)eRoulette->var86);
        sprite->animNum       = 0x2;
        sprite->animBeginning = TRUE;
        sprite->animEnded     = FALSE;
        sprite->data[0x1]     = 0x3;
        sprite->callback      = &sub_8118F8C;
    }
}

void sub_8119134(struct Sprite *sprite)
{
    sub_8118724(sprite);
    if (!(eRoulette->var94 > 60.0f))
    {
        m4aSongNumStartOrChange(0x5D);
        eRoulette->var98 = - (                20.0f / (float)(u16)eRoulette->var84);
        eRoulette->var90 =   ((1.0f - eRoulette->var8C) / (float)(u16)eRoulette->var84);
        sprite->animNum       = 0x1;
        sprite->animBeginning = TRUE;
        sprite->animEnded     = FALSE;
        sprite->data[0x1]     = 0x2;
        sprite->callback      = &sub_8119088;
    }
}

void sub_81191F4(struct Sprite *sprite)
{
    sprite->data[0x1] = 0x1;
    sprite->data[0x2] = 0x0;
    sub_8118724(sprite);
    sprite->invisible = FALSE;
    sprite->callback  = &sub_8119134;
}

void sub_8119224(struct Sprite *sprite)
{
    u16 t;
    u8 i;
    s16 s[0x2][0x2];
    struct Roulette *p;
    memcpy(s, &gUnknown_083FA60E, 0x8);
    t = sprite->data[0x7] - 0x2;
    p = eRoulette;
    p->var3C[0x37] = CreateSprite(&gSpriteTemplate_83FA50C, 0x24, -0xC, 0x32);
    p->var3C[0x38] = CreateSprite(&gSpriteTemplate_83FA5C0[0x0], s[sprite->data[0x0]][0x0], s[sprite->data[0x0]][0x1], 0x3B);
    p->var3C[0x39] = CreateSprite(&gSpriteTemplate_83FA5C0[0x1], 0x24, 0x8C, 0x33);
    gSprites[eRoulette->var3C[0x39]].oam.objMode = 0x1;
    for (i = 0x0; i < 0x3; i++)
    {
        gSprites[eRoulette->var3C[0x37 + i]].coordOffsetEnabled = FALSE;
        gSprites[eRoulette->var3C[0x37 + i]].invisible          = TRUE;
        gSprites[eRoulette->var3C[0x37 + i]].animPaused         = TRUE;
        gSprites[eRoulette->var3C[0x37 + i]].affineAnimPaused   = TRUE;
        gSprites[eRoulette->var3C[0x37 + i]].data[0x4]          = eRoulette->var3C[0x37];
        gSprites[eRoulette->var3C[0x37 + i]].data[0x5]          = eRoulette->var3C[0x38];
        gSprites[eRoulette->var3C[0x37 + i]].data[0x6]          = eRoulette->var3C[0x39];
        gSprites[eRoulette->var3C[0x37 + i]].data[0x2]          = t;
        gSprites[eRoulette->var3C[0x37 + i]].data[0x3]          = (sprite->data[0x7] * (&gUnknown_083F8DF4[eRoulette->var04_0])->var01) +
                                                                ((&gUnknown_083F8DF4[eRoulette->var04_0])->var02 + 0xFFFF);
    }
    gSprites[eRoulette->var3C[0x38]].coordOffsetEnabled = TRUE;
    eRoulette->var38 = sprite;
}

void sub_81193D4(struct Sprite *sprite)
{
    u8 i = 0;
    s16 t;
    s16 s[0x2][0x2];
    struct StructgUnknown_083F8DF4 *p;
    memcpy(s, &gUnknown_083FA616, 0x8);
    t = sprite->data[0x7] - 0x2;
    eRoulette->var3C[0x37] = CreateSprite(&gSpriteTemplate_83FA524, s[sprite->data[0x0]][0x0], s[sprite->data[0x0]][0x1], 0x32);
    StartSpriteAnim(&gSprites[eRoulette->var3C[0x37]], sprite->data[0x0]);
    eRoulette->var3C[0x38] = CreateSprite(&gSpriteTemplate_83FA5F0, s[sprite->data[0x0]][0x0], s[sprite->data[0x0]][0x1], 0x33);
    gSprites[eRoulette->var3C[0x38]].affineAnimPaused = TRUE;
    gSprites[eRoulette->var3C[0x38]].animPaused       = TRUE;
    sprite->data[0x7] = (t * (p = &gUnknown_083F8DF4[0])[eRoulette->var04_0].var01) + (p[eRoulette->var04_0].var10 + 0x2D);
    for (; i < 0x2; i++)
    {
        gSprites[eRoulette->var3C[0x37 + i]].data[0x4] = eRoulette->var3C[0x37];
        gSprites[eRoulette->var3C[0x37 + i]].data[0x5] = eRoulette->var3C[0x38];
        gSprites[eRoulette->var3C[0x37 + i]].data[0x6] = eRoulette->var3C[0x38];
        gSprites[eRoulette->var3C[0x37 + i]].data[0x2] = t;
        gSprites[eRoulette->var3C[0x37 + i]].data[0x3] = sprite->data[0x7] - 0x2D;
    }
    eRoulette->var38 = sprite;
}

#ifdef NONMATCHING
void sub_811952C(struct Sprite *sprite)
{
    u8 z;
    register u8 h asm("r10") = 0x0;
    u8 j = 0x5;
    u32 p = 0x0;
    u16 o;
    u8 i;
    u8 s[0xA] = (u8[]){0,0,0,0,0,0,0,0,0,0};
    u16 t = Random();
    eRoulette->var7D   = 0x1;
    eRoulette->var03_5 = TRUE;
    eRoulette->var03_6 = FALSE;
    eRoulette->var7E   = 0xFF;
    eRoulette->var88   = sprite->data[0x3];
    eRoulette->var98   = 0.0f;
    eRoulette->var8C   = (&gUnknown_083F8DF4[eRoulette->var04_0])->var1C; //couldn't replicate load, same as sub_8118DE4
    o = (eRoulette->var04_0 * 30 + 33) + (0x1 - eRoulette->var03_0) * 15;
    for (i = 0x0; i < 0x4; i++)
    {
        if (o < sprite->data[0x3] && !(sprite->data[0x3] > o + 90))
        {
            sprite->data[0x0] = i * 0x2;
            eRoulette->var03_0 = 0x1 & i;
            break;
        }
        if (i == 0x3)
        {
            sprite->data[0x0] = 0x1;
            eRoulette->var03_0 = 0x1;
            break;
        }
        o += 90;
    }
    if (eRoulette->var03_0)
    {
        if (sprite->data[0x0])
        {
            PlayCry1(0x130, -0x3F);
        }
        else
        {
            PlayCry1(0x130, 0x3F);
        }
    }
    else
    {
        PlayCry1(0x132, -0x3F);
    }
    i = 0x2;
    z = (eRoulette->var7F + 0x2) % 0xC;
    if (eRoulette->var03_0 == 0x1 && eRoulette->var04_0 == 0x1)
        j += 0x6;
    else
        j += i;
    for (; i < j; i++)
    {
        if (!(eRoulette->var08 & gUnknown_083F8D90[z].var04))
        {
            s[h++] = i;
            if (!p && (gUnknown_083F8D90[z].var04 & gUnknown_083F8C00[eRoulette->var1B[eRoulette->var1A_0]].var00))
            {
                p = i;
            }
        }
        z = (z + 0x1) % 0xC;
    }
    if ((eRoulette->var03_0 + 0x1) & eRoulette->var02)
    {
        if (p && (t & 0xFF) <= 0xBF)
        {
            sprite->data[0x7] = p;
        }
        else
        {
            sprite->data[0x7] = s[t % h];
        }
    }
    else
    {
        sprite->data[0x7] = s[t % h];
    }
    sprite->callback = &sub_8118CEC;
}
#else
__attribute__((naked))
void sub_811952C(struct Sprite *sprite)
{
asm(".syntax unified\n\
push {r4-r7,lr}\n\
mov r7, r10\n\
mov r6, r9\n\
mov r5, r8\n\
push {r5-r7}\n\
sub sp, 0x14\n\
mov r8, r0\n\
movs r0, 0\n\
mov r10, r0\n\
movs r1, 0x5\n\
mov r9, r1\n\
movs r2, 0\n\
str r2, [sp, 0xC]\n\
mov r0, sp\n\
movs r1, 0\n\
movs r2, 0xA\n\
bl memset\n\
bl Random\n\
lsls r0, 16\n\
lsrs r0, 16\n\
str r0, [sp, 0x10]\n\
ldr r7, _08119610 @ =gSharedMem + 0x19000\n\
adds r0, r7, 0\n\
adds r0, 0x7D\n\
movs r6, 0x1\n\
strb r6, [r0]\n\
ldrb r4, [r7, 0x3]\n\
movs r0, 0x20\n\
orrs r4, r0\n\
movs r0, 0x41\n\
negs r0, r0\n\
ands r4, r0\n\
strb r4, [r7, 0x3]\n\
adds r1, r7, 0\n\
adds r1, 0x7E\n\
movs r0, 0xFF\n\
strb r0, [r1]\n\
adds r5, r7, 0\n\
adds r5, 0x88\n\
mov r3, r8\n\
movs r1, 0x34\n\
ldrsh r0, [r3, r1]\n\
bl __floatsisf\n\
str r0, [r5]\n\
adds r1, r7, 0\n\
adds r1, 0x98\n\
ldr r0, _08119614 @ =0x00000000\n\
str r0, [r1]\n\
adds r3, r7, 0\n\
adds r3, 0x8C\n\
ldr r2, _08119618 @ =gUnknown_083F8DF4\n\
ldrb r0, [r7, 0x4]\n\
lsls r0, 30\n\
lsrs r1, r0, 25\n\
adds r2, 0x1C\n\
adds r1, r2\n\
ldr r1, [r1]\n\
str r1, [r3]\n\
lsrs r0, 30\n\
lsls r1, r0, 4\n\
subs r1, r0\n\
lsls r1, 1\n\
adds r1, 0x21\n\
lsls r4, 27\n\
lsrs r4, 27\n\
subs r6, r4\n\
lsls r0, r6, 4\n\
subs r0, r6\n\
adds r1, r0\n\
lsls r1, 16\n\
lsrs r1, 16\n\
mov r5, r10\n\
mov r3, r8\n\
movs r0, 0x34\n\
ldrsh r2, [r3, r0]\n\
_081195C8:\n\
cmp r1, r2\n\
bge _081195D4\n\
adds r0, r1, 0\n\
adds r0, 0x5A\n\
cmp r2, r0\n\
ble _08119638\n\
_081195D4:\n\
cmp r5, 0x3\n\
beq _0811961C\n\
adds r0, r1, 0\n\
adds r0, 0x5A\n\
lsls r0, 16\n\
lsrs r1, r0, 16\n\
adds r0, r5, 0x1\n\
lsls r0, 24\n\
lsrs r5, r0, 24\n\
cmp r5, 0x3\n\
bls _081195C8\n\
_081195EA:\n\
ldr r0, _08119610 @ =gSharedMem + 0x19000\n\
ldrb r1, [r0, 0x3]\n\
movs r0, 0x1F\n\
ands r0, r1\n\
cmp r0, 0\n\
beq _08119664\n\
mov r1, r8\n\
movs r2, 0x2E\n\
ldrsh r0, [r1, r2]\n\
cmp r0, 0\n\
beq _08119658\n\
movs r0, 0x98\n\
lsls r0, 1\n\
movs r1, 0x3F\n\
negs r1, r1\n\
bl PlayCry1\n\
b _08119670\n\
.align 2, 0\n\
_08119610: .4byte gSharedMem + 0x19000\n\
_08119614: .4byte 0x00000000\n\
_08119618: .4byte gUnknown_083F8DF4\n\
_0811961C:\n\
movs r0, 0x1\n\
mov r3, r8\n\
strh r0, [r3, 0x2E]\n\
ldr r2, _08119634 @ =gSharedMem + 0x19000\n\
ldrb r1, [r2, 0x3]\n\
subs r0, 0x21\n\
ands r0, r1\n\
movs r1, 0x1\n\
orrs r0, r1\n\
strb r0, [r2, 0x3]\n\
b _081195EA\n\
.align 2, 0\n\
_08119634: .4byte gSharedMem + 0x19000\n\
_08119638:\n\
lsrs r0, r5, 1\n\
mov r1, r8\n\
strh r0, [r1, 0x2E]\n\
ldr r3, _08119654 @ =gSharedMem + 0x19000\n\
movs r1, 0x1\n\
ands r1, r5\n\
ldrb r2, [r3, 0x3]\n\
movs r0, 0x20\n\
negs r0, r0\n\
ands r0, r2\n\
orrs r0, r1\n\
strb r0, [r3, 0x3]\n\
b _081195EA\n\
.align 2, 0\n\
_08119654: .4byte gSharedMem + 0x19000\n\
_08119658:\n\
movs r0, 0x98\n\
lsls r0, 1\n\
movs r1, 0x3F\n\
bl PlayCry1\n\
b _08119670\n\
_08119664:\n\
movs r0, 0x99\n\
lsls r0, 1\n\
movs r1, 0x3F\n\
negs r1, r1\n\
bl PlayCry1\n\
_08119670:\n\
movs r5, 0x2\n\
ldr r4, _081196A0 @ =gSharedMem + 0x19000\n\
adds r0, r4, 0\n\
adds r0, 0x7F\n\
ldrb r0, [r0]\n\
adds r0, 0x2\n\
movs r1, 0xC\n\
bl __modsi3\n\
lsls r0, 24\n\
lsrs r3, r0, 24\n\
ldrb r1, [r4, 0x3]\n\
movs r0, 0x1F\n\
ands r0, r1\n\
cmp r0, 0x1\n\
bne _081196A4\n\
ldrb r1, [r4, 0x4]\n\
movs r0, 0x3\n\
ands r0, r1\n\
cmp r0, 0x1\n\
bne _081196A4\n\
mov r0, r9\n\
adds r0, 0x6\n\
b _081196A8\n\
.align 2, 0\n\
_081196A0: .4byte gSharedMem + 0x19000\n\
_081196A4:\n\
mov r2, r9\n\
adds r0, r2, r5\n\
_081196A8:\n\
lsls r0, 24\n\
lsrs r0, 24\n\
mov r9, r0\n\
cmp r5, r9\n\
bcs _0811970E\n\
ldr r6, _08119734 @ =gSharedMem + 0x19000\n\
ldr r7, _08119738 @ =gUnknown_083F8C00 + 0xC\n\
_081196B6:\n\
lsls r0, r3, 3\n\
ldr r1, _0811973C @ =gUnknown_083F8D90 + 0x4\n\
adds r0, r1\n\
ldr r1, [r6, 0x8]\n\
ldr r2, [r0]\n\
ands r1, r2\n\
cmp r1, 0\n\
bne _081196F8\n\
mov r0, r10\n\
adds r1, r0, 0x1\n\
lsls r1, 24\n\
lsrs r1, 24\n\
mov r10, r1\n\
add r0, sp\n\
strb r5, [r0]\n\
ldr r0, [sp, 0xC]\n\
cmp r0, 0\n\
bne _081196F8\n\
ldrb r0, [r6, 0x1A]\n\
lsls r0, 28\n\
lsrs r0, 28\n\
ldr r1, _08119740 @ =gSharedMem + 0x1901B\n\
adds r0, r1\n\
ldrb r1, [r0]\n\
lsls r0, r1, 2\n\
adds r0, r1\n\
lsls r0, 2\n\
adds r0, r7\n\
ldr r0, [r0]\n\
ands r2, r0\n\
cmp r2, 0\n\
beq _081196F8\n\
str r5, [sp, 0xC]\n\
_081196F8:\n\
adds r0, r3, 0x1\n\
movs r1, 0xC\n\
bl __modsi3\n\
lsls r0, 24\n\
lsrs r3, r0, 24\n\
adds r0, r5, 0x1\n\
lsls r0, 24\n\
lsrs r5, r0, 24\n\
cmp r5, r9\n\
bcc _081196B6\n\
_0811970E:\n\
ldrb r0, [r4, 0x3]\n\
lsls r0, 27\n\
lsrs r0, 27\n\
adds r0, 0x1\n\
ldrb r1, [r4, 0x2]\n\
ands r0, r1\n\
cmp r0, 0\n\
beq _08119756\n\
ldr r2, [sp, 0xC]\n\
cmp r2, 0\n\
beq _08119744\n\
movs r0, 0xFF\n\
ldr r3, [sp, 0x10]\n\
ands r0, r3\n\
cmp r0, 0xBF\n\
bhi _08119744\n\
mov r0, r8\n\
strh r2, [r0, 0x3C]\n\
b _08119766\n\
.align 2, 0\n\
_08119734: .4byte gSharedMem + 0x19000\n\
_08119738: .4byte gUnknown_083F8C00 + 0xC\n\
_0811973C: .4byte gUnknown_083F8D90 + 0x4\n\
_08119740: .4byte gSharedMem + 0x1901B\n\
_08119744:\n\
ldr r0, [sp, 0x10]\n\
mov r1, r10\n\
bl __modsi3\n\
add r0, sp\n\
ldrb r0, [r0]\n\
mov r1, r8\n\
strh r0, [r1, 0x3C]\n\
b _08119766\n\
_08119756:\n\
ldr r0, [sp, 0x10]\n\
mov r1, r10\n\
bl __modsi3\n\
add r0, sp\n\
ldrb r0, [r0]\n\
mov r2, r8\n\
strh r0, [r2, 0x3C]\n\
_08119766:\n\
ldr r3, _0811977C @ =sub_8118CEC\n\
mov r0, r8\n\
str r3, [r0, 0x1C]\n\
add sp, 0x14\n\
pop {r3-r5}\n\
mov r8, r3\n\
mov r9, r4\n\
mov r10, r5\n\
pop {r4-r7}\n\
pop {r0}\n\
bx r0\n\
.align 2, 0\n\
_0811977C: .4byte sub_8118CEC\n\
.syntax divided\n");
}
#endif

void sub_8119780(struct Sprite *sprite)
{
    if (!(sprite->data[0x1]++ < sprite->data[0x3]))
    {
        if ((sprite->pos1.x -= 0x2) < -0x10)
        {
            if (!eRoulette->var03_6)
            {
                eRoulette->var03_6 = TRUE;
            }
            DestroySprite(sprite);
            eRoulette->var01 = 0x0;
            eRoulette->var34 = gUnknown_083FA61E[0x0];
        }
    }
}

void sub_81197D8(struct Sprite *sprite)
{
    u16 t[0x3][0x4];
    s32 p, z;
    memcpy(t, &gUnknown_083FA632, 0x18);
    if (sprite->data[0x1]++ < sprite->data[0x3])
    {
        if(sprite->data[0x1] & 0x1)
        {
            gSpriteCoordOffsetY = t[sprite->data[0x2] / 0x2][sprite->data[0x7]];
            p = z = sprite->data[0x7] + 0x1;
            if (z < 0)
                p += 0x3;
            sprite->data[0x7] = z - ((p >> 2) * 4);
        }
        sprite->invisible ^= 0x1;
    }
    else
    {
        gSpriteCoordOffsetY = 0x0;
        gSprites[eRoulette->var3C[0x37]].animPaused = FALSE;
        DestroySprite(sprite);
    }
}

void sub_8119898(struct Sprite *sprite)
{
    float t;
    sprite->data[0x1]++;
    t = sprite->data[0x1];
    sprite->pos2.y = t * 0.039f * t;
    eRoulette->var34 = gUnknown_083FA61E[(eRoulette->var01 - 0x1) / 2];
    if (eRoulette->var01 < 0x13)
        eRoulette->var01++;
    if (sprite->data[0x1] > 0x3C)
    {
        sprite->data[0x1] = 0x0;
        sprite->callback = &sub_8119780;
        gSprites[sprite->data[0x6]].callback  = &sub_8119780;
        gSprites[sprite->data[0x6]].data[0x1] = -0x2;
        gSprites[sprite->data[0x5]].invisible = FALSE;
        gSprites[sprite->data[0x5]].callback  = &sub_81197D8;
        m4aSongNumStart(0xD6);
    }
}

void sub_8119964(struct Sprite *sprite)
{
    if(sprite->data[0x7] == 0x0)
    {
        register u32 t asm("r2");
        u32 z ;
        if (eRoulette->var38->data[0x0] == 0x0)
        {
            t = eRoulette->var38->data[0x3];
            z = gUnknown_083F8DF4[eRoulette->var04_0].var08;
        }
        else
        {
            t = eRoulette->var38->data[0x3];
            z = gUnknown_083F8DF4[eRoulette->var04_0].var08;
            z += 0xB4;
        }
        if (t == z)
        {
            sprite->invisible = FALSE;
            sprite->data[0x7]++;
            m4aSongNumStart(0x2B);
            eRoulette->var01 = 0x1;
            eRoulette->var34 = gUnknown_083FA61E[0x0];
        }
    }
    else
    {
        u32 t, z;
        eRoulette->var34 = gUnknown_083FA61E[(eRoulette->var01 - 0x1) / 2];
        if (eRoulette->var01 < 0x13)
            eRoulette->var01++;
        if (eRoulette->var38->data[0x0] == 0x0)
        {
            t = eRoulette->var38->data[0x3];
            z = gUnknown_083F8DF4[eRoulette->var04_0].var0A;
        }
        else
        {
            t = eRoulette->var38->data[0x3];
            z = gUnknown_083F8DF4[eRoulette->var04_0].var0A ;
            z += 0xB4;
        }
        if (t == z)
        {
            gSprites[sprite->data[0x4]].callback  = &sub_8119898;
            gSprites[sprite->data[0x4]].invisible = FALSE;
            sprite->callback  = &SpriteCallbackDummy;
            sprite->data[0x7] = 0x0;
        }
    }
}

void sub_8119A90(struct Sprite *sprite)
{
    sprite->invisible ^= 0x1;
}

void sub_8119AAC(struct Sprite *sprite)
{
    if (sprite->pos1.y > -0x10)
    {
        sprite->pos1.y--;
    }
    else
    {
        sprite->callback   = &SpriteCallbackDummy;
        sprite->invisible  = TRUE;
        sprite->animPaused = TRUE;
        m4aSongNumStop(0x5E);
        DestroySprite(sprite);
        FreeOamMatrix(gSprites[eRoulette->var3C[0x38]].oam.matrixNum);
        DestroySprite(&gSprites[eRoulette->var3C[0x38]]);
    }
}

void sub_8119B24(struct Sprite *sprite)
{
    if (!(sprite->data[0x1] < 0x0))
    {
        sprite->data[0x1]--;
        sprite->pos1.y--;
        if (sprite->data[0x1] == 0x0 && sprite->animCmdIndex == 0x1)
            sprite->pos2.y++;
    }
    else
    {
        if (!(sprite->data[0x3] < 0x0))
        {
            sprite->data[0x3]--;
            if (sprite->animDelayCounter == 0x0)
            {
                if (sprite->animCmdIndex == 0x1)
                    sprite->pos2.y++;
                else
                    sprite->pos2.y--;
            }
        }
        else
        {
            m4aSongNumStart(0x2B);
            StartSpriteAnim(sprite, eRoulette->var38->data[0x0] + 0x4);
            sprite->callback = &sub_8119AAC;
            gSprites[sprite->data[0x6]].affineAnimPaused = FALSE;
        }
    }
}

void sub_8119BCC(struct Sprite *sprite)
{
    s8 t[0x2]; //sign
    s8 z[0x8][0x2];
    memcpy(t, &gUnknown_083FA64A, 0x2);
    memcpy(z, &gUnknown_083FA64C, 0x10);
    if (sprite->data[0x1]-- > 0x7)
    {
        sprite->pos1.x += t[eRoulette->var38->data[0x0]] * 0x2;
        if (IsSEPlaying())
        {
            s8 u =  -((0x74 - sprite->pos1.x) / 0x2);
            m4aMPlayPanpotControl(&gMPlay_SE1, 0xFFFF,u);
            m4aMPlayPanpotControl(&gMPlay_SE2, 0xFFFF, u);
        }
    }
    else
    {
        if (!(sprite->data[0x1] < 0x0))
        {
            sprite->pos1.x += t[eRoulette->var38->data[0x0]] * z[0x7 - sprite->data[0x1]][0x0];
            sprite->pos1.y += z[0x7 - sprite->data[0x1]][0x1];
        }
        else
        {
            m4aSongNumStartOrChange(0x5E);
            if(eRoulette->var38->data[0x0] == 0x0)
                PlayCry1(0x130, 0x3F);
            else
                PlayCry1(0x130, -0x3F);
            StartSpriteAnim(sprite, eRoulette->var38->data[0x0] + 0x2);
            sprite->data[0x1] = 45;
            sprite->callback = &sub_8119B24;
        }
    }
}

void sub_8119D08(struct Sprite *sprite)
{
    s8 t[0x2]; //sign
    memcpy(t, &gUnknown_083FA64A, 0x2);
    if (!(sprite->data[0x1]-- < 0x0))
    {
        sprite->pos1.x += t[eRoulette->var38->data[0x0]] * 0x2;
        gSprites[sprite->data[0x6]].invisible ^= 0x1;
    }
    else
    {
        sprite->callback = &sub_8119A90;
    }
}

void sub_8119D80(struct Sprite *sprite)
{
    if (eRoulette->var38->data[0x0] == 0x0)
    {
        if (eRoulette->var38->data[0x3] == gUnknown_083F8DF4[eRoulette->var04_0].var12 + 90)
        {
            gSprites[sprite->data[0x6]].data[0x1] = 0x34;
            gSprites[sprite->data[0x4]].data[0x1] = 0x34;
        }
        else
        {
            return;
        }
    }
    else
    {
        if (eRoulette->var38->data[0x3] == gUnknown_083F8DF4[eRoulette->var04_0].var14 + 270)
        {
            gSprites[sprite->data[0x6]].data[0x1] = 0x2E;
            gSprites[sprite->data[0x4]].data[0x1] = 0x2E;
        }
        else
        {
            return;
        }
    }
    gSprites[sprite->data[0x6]].callback = &sub_8119D08;
    gSprites[sprite->data[0x4]].callback = &sub_8119BCC;
    m4aSongNumStart(0x2B);
}
