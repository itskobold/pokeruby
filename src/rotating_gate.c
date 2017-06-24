#include "global.h"
#include "bike.h"
#include "event_data.h"
#include "fieldmap.h"
#include "sound.h"
#include "sprite.h"

struct UnknownStruct
{
    /* 0x0 */ struct Coords16 pos;
    /* 0x4 */ u8 e;
    /* 0x5 */ u8 f;
    /* 0x6 */ u8 g;
    /* 0x7 */ u8 h;
};

struct UnknownStruct2
{
    s8 a;
    s8 b;
    u8 c;
    u8 d;
};

extern const struct SpriteSheet gRotatingGatesGraphicsTable[];
extern /* const */ struct UnknownStruct gUnknown_083D2964[];
extern /* const */ struct UnknownStruct gUnknown_083D299C[];
extern const struct SpriteTemplate gSpriteTemplate_83D60A4;
extern const struct SpriteTemplate gSpriteTemplate_83D608C;

extern u8 gUnknown_020387F0[10]; // puzzle oam ID list

extern struct UnknownStruct *gUnknown_02038800;
extern u8 gUnknown_02038804; // puzzle count

extern struct UnknownStruct2 gUnknown_083D611C[5];
extern struct UnknownStruct2 gUnknown_083D60FC[5];
extern const u8 gUnknown_083D613C[12][8];

extern const u8 gUnknown_083D60BC[4][4];
extern const u8 gUnknown_083D60CC[4][4];
extern const u8 gUnknown_083D60DC[4][4];
extern const u8 gUnknown_083D60EC[4][4];

u8 sub_80C7BAC(u8 i, s16 a, s16 b);
void sub_8060388(s16 x, s16 y, s16 *outX, s16 *outY);
void sub_80C7D14(struct Sprite *sprite);

int GetCurrentMapRotatingGatePuzzleType(void)
{
    if (gSaveBlock1.location.mapGroup == 0xC && gSaveBlock1.location.mapNum == 0x1)
    {
        return 1;
    }

    if (gSaveBlock1.location.mapGroup == 0x1d && gSaveBlock1.location.mapNum == 0x8)
    {
        return 2;
    }

    return 0;
}

// ResetSomething
void sub_80C79CC(void)
{
    int i;
    u8 *ptr;

    ptr = (u8 *)GetVarPointer(0x4000);

    for (i = 0; i < gUnknown_02038804; i++)
    {
        ptr[i] = gUnknown_02038800[i].f;
    }
}

// GetSomething
/* u8 */ int sub_80C7A08(u8 a)
{
    u8 *ptr;
    ptr = (u8 *)GetVarPointer(0x4000);
    return ptr[a];
}

// SetSomething
void sub_80C7A24(u8 a, u8 b)
{
    u8 *ptr;
    ptr = (u8 *)GetVarPointer(0x4000);
    ptr[a] = b;
}

// RotateInDirectionProbably
void sub_80C7A44(u8 a, u32 b)
{
    u8 r1;
    r1 = sub_80C7A08(a);

    if (b == 1)
    {
        if (r1)
        {
            r1--;
        }
        else
        {
            r1 = 3;
        }
    }
    else
    {
        r1 = ++r1 % 4;
    }

    sub_80C7A24(a, r1);
}

// Configure
void sub_80C7A80(void)
{
    int a = GetCurrentMapRotatingGatePuzzleType();
    u32 i;

    switch (a)
    {
    case 1:
        gUnknown_02038800 = gUnknown_083D2964;
        gUnknown_02038804 = 7;
        break;
    case 2:
        gUnknown_02038800 = gUnknown_083D299C;
        gUnknown_02038804 = 0xE;
        break;
    case 0:
    default:
        return;
    }

    for (i = 0; i < 14; i++)
    {
        gUnknown_020387F0[i] = MAX_SPRITES;
    }
}

void sub_80C7ADC(s16 a, s16 b)
{
    u8 i;
    s16 x = gSaveBlock1.pos.x - 2;
    s16 x2 = gSaveBlock1.pos.x + 0x11;
    s16 y = gSaveBlock1.pos.y - 2;
    s16 y2 = gSaveBlock1.pos.y + 0xe;

    s16 x3, y3;

    for (i = 0; i < gUnknown_02038804; i++)
    {
        x3 = gUnknown_02038800[i].pos.x + 7;
        y3 = gUnknown_02038800[i].pos.y + 7;

        if (y <= y3 && y2 >= y3 && x <= x3 && x2 >= x3 && gUnknown_020387F0[i] == MAX_SPRITES)
        {
            gUnknown_020387F0[i] = sub_80C7BAC(i, a, b);
        }
    }
}

// CreateSprite
u8 sub_80C7BAC(u8 i, s16 a, s16 b)
{
    struct Sprite *sprite; // r4
    struct SpriteTemplate template;
    struct UnknownStruct *unk;
    u8 id; //  r5
    s16 x, y;

    unk = &gUnknown_02038800[i];

    if (unk->e == 0 || unk->e == 4)
    {
        template = gSpriteTemplate_83D60A4;
    }
    else
    {
        template = gSpriteTemplate_83D608C;
    }

    template.tileTag = unk->e + 0x1300;

    id = CreateSprite(&template, 0, 0, 0x94);
    if (id == MAX_SPRITES)
    {
        return MAX_SPRITES;
    }

    x = unk->pos.x + 7;
    y = unk->pos.y + 7;

    sprite = &gSprites[id];
    sprite->data0 = i;
    sprite->coordOffsetEnabled = 1;

    sub_8060388(x + a, y + b, &sprite->pos1.x, &sprite->pos1.y);
    sub_80C7D14(sprite);
    StartSpriteAffineAnim(sprite, sub_80C7A08(i));

    return id;
}

// RotateGateWithSE
void sub_80C7C94(struct Sprite *sprite)
{
    u8 a; // r5
    u8 b;
    u8 affineAnimation;

    a = sprite->data1;
    b = sprite->data2;

    sub_80C7D14(sprite);

    if (a == 1)
    {
        affineAnimation = b + 4;

        if (GetPlayerSpeed() != 1)
            affineAnimation += 8;

        PlaySE(48);
        StartSpriteAffineAnim(sprite, affineAnimation);
    }
    else if (a == 2)
    {
        affineAnimation = b + 8;

        if (GetPlayerSpeed() != 1)
            affineAnimation += 8;

        PlaySE(48);
        StartSpriteAffineAnim(sprite, affineAnimation);
    }

    sprite->data1 = 0;
}

// See sub_806487C (field_map_obj_helpers.c)
void sub_80C7D14(struct Sprite *sprite)
{
    u16 x;
    s16 x2;
    u16 y;
    s16 y2;

    sprite->invisible = 0;
    x = sprite->pos1.x + sprite->pos2.x + sprite->centerToCornerVecX + gSpriteCoordOffsetX;
    y = sprite->pos1.y + sprite->pos2.y + sprite->centerToCornerVecY + gSpriteCoordOffsetY;

    x2 = x + 0x40; // Dimensions of the rotating gate
    y2 = y + 0x40;

    // TODO: Make this 0x10 a constant
    if ((s16)x > DISPLAY_WIDTH + 0x10 - 1 || x2 < -0x10)
    {
        sprite->invisible = 1;
    }

    if ((s16)y > DISPLAY_HEIGHT + 0x10 - 1 || y2 < -0x10)
    {
        sprite->invisible = 1;
    }
}

void LoadRotatingGatePics(void)
{
    LoadSpriteSheets(gRotatingGatesGraphicsTable);
}

void sub_80C7DC0(void)
{
    s16 x;
    s16 x2;
    s16 y;
    s16 y2;
    s16 xGate;
    s16 yGate;
    int i;
    struct Sprite *sprite;

    x = gSaveBlock1.pos.x - 2;
    x2 = gSaveBlock1.pos.x + 17;
    y = gSaveBlock1.pos.y - 2;
    y2 = gSaveBlock1.pos.y + 14;

    for (i = 0; i < gUnknown_02038804; i++)
    {
        xGate = gUnknown_02038800[i].pos.x + 7;
        yGate = gUnknown_02038800[i].pos.y + 7;

        if (gUnknown_020387F0[i] == MAX_SPRITES)
            continue;

        if (xGate < x || xGate > x2 || yGate < y || yGate > y2)
        {
            sprite = &gSprites[gUnknown_020387F0[i]];
            FreeSpriteOamMatrix(sprite);
            DestroySprite(sprite);
            gUnknown_020387F0[i] = MAX_SPRITES;
        }
    }
}

#ifdef NONMATCHING
int sub_80C7E8C(u8 a, int puzzleType)
{
    struct UnknownStruct2 *unk1;
    u8 unk2;
    s16 x;
    s16 y;
    int unk3;
    int i;
    int j;
    int unk4;
    const u8 *unk5;
    int unk6;
    u8 unk7;
    int wat;

    if (puzzleType == 1)
        unk1 = gUnknown_083D611C;
    else if (puzzleType == 2)
        unk1 = gUnknown_083D60FC;
    else
        return 0;

    unk2 = sub_80C7A08(a);

    unk3 = gUnknown_02038800[a].e;
    x = gUnknown_02038800[a].pos.x + 7;
    y = gUnknown_02038800[a].pos.y + 7;

    for (i = 0; i <= 3; i++)
    {
        j = 0;
        unk4 = unk2 + i;
        wat = unk2 + i;
        unk5 = &gUnknown_083D613C[unk3][i * 2];

        for (; j <= 1; j++)
        {
            unk6 = wat;
            if (wat < 0)
                unk6 = unk4 + 3;

            unk7 = (2 * (wat - ((unk6 >> 2) << 2)) + j);

            if (*unk5)
            {
                if (MapGridIsImpassableAt(unk1[unk7].a + x, unk1[unk7].b + y) == 1)
                    return 0;
            }
            unk5++;
        }
    }

    return 1;
}
#else
__attribute__((naked)) int sub_80C7E8C(u8 a, int puzzleType)
{
    asm(".syntax unified\n\
	push {r4-r7,lr}\n\
	mov r7, r10\n\
	mov r6, r9\n\
	mov r5, r8\n\
	push {r5-r7}\n\
	sub sp, 0xC\n\
	lsls r0, 24\n\
	lsrs r4, r0, 24\n\
	cmp r1, 0x1\n\
	bne _080C7EAC\n\
	ldr r0, _080C7EA8 @ =gUnknown_083D611C\n\
	mov r10, r0\n\
	b _080C7EB8\n\
	.align 2, 0\n\
_080C7EA8: .4byte gUnknown_083D611C\n\
_080C7EAC:\n\
	cmp r1, 0x2\n\
	beq _080C7EB4\n\
_080C7EB0:\n\
	movs r0, 0\n\
	b _080C7F48\n\
_080C7EB4:\n\
	ldr r1, _080C7F58 @ =gUnknown_083D60FC\n\
	mov r10, r1\n\
_080C7EB8:\n\
	adds r0, r4, 0\n\
	bl sub_80C7A08\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	str r0, [sp]\n\
	ldr r0, _080C7F5C @ =gUnknown_02038800\n\
	ldr r1, [r0]\n\
	lsls r0, r4, 3\n\
	adds r0, r1\n\
	ldrb r2, [r0, 0x4]\n\
	ldrh r1, [r0]\n\
	adds r1, 0x7\n\
	ldrh r0, [r0, 0x2]\n\
	adds r0, 0x7\n\
	movs r3, 0\n\
	lsls r2, 3\n\
	str r2, [sp, 0x4]\n\
	lsls r1, 16\n\
	asrs r1, 16\n\
	mov r9, r1\n\
	lsls r0, 16\n\
	asrs r0, 16\n\
	mov r8, r0\n\
_080C7EE8:\n\
	movs r6, 0\n\
	ldr r2, [sp]\n\
	adds r7, r2, r3\n\
	lsls r0, r3, 1\n\
	adds r5, r7, 0\n\
	ldr r1, [sp, 0x4]\n\
	adds r0, r1\n\
	ldr r2, _080C7F60 @ =gUnknown_083D613C\n\
	adds r4, r0, r2\n\
_080C7EFA:\n\
	adds r0, r5, 0\n\
	cmp r5, 0\n\
	bge _080C7F02\n\
	adds r0, r7, 0x3\n\
_080C7F02:\n\
	asrs r0, 2\n\
	lsls r0, 2\n\
	subs r0, r5, r0\n\
	lsls r0, 1\n\
	adds r0, r6\n\
	lsls r0, 24\n\
	lsrs r1, r0, 24\n\
	ldrb r0, [r4]\n\
	cmp r0, 0\n\
	beq _080C7F38\n\
	lsls r1, 2\n\
	add r1, r10\n\
	movs r0, 0\n\
	ldrsb r0, [r1, r0]\n\
	add r0, r9\n\
	ldrb r1, [r1, 0x1]\n\
	lsls r1, 24\n\
	asrs r1, 24\n\
	add r1, r8\n\
	str r3, [sp, 0x8]\n\
	bl MapGridIsImpassableAt\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	ldr r3, [sp, 0x8]\n\
	cmp r0, 0x1\n\
	beq _080C7EB0\n\
_080C7F38:\n\
	adds r4, 0x1\n\
	adds r6, 0x1\n\
	cmp r6, 0x1\n\
	ble _080C7EFA\n\
	adds r3, 0x1\n\
	cmp r3, 0x3\n\
	ble _080C7EE8\n\
	movs r0, 0x1\n\
_080C7F48:\n\
	add sp, 0xC\n\
	pop {r3-r5}\n\
	mov r8, r3\n\
	mov r9, r4\n\
	mov r10, r5\n\
	pop {r4-r7}\n\
	pop {r1}\n\
	bx r1\n\
	.align 2, 0\n\
_080C7F58: .4byte gUnknown_083D60FC\n\
_080C7F5C: .4byte gUnknown_02038800\n\
_080C7F60: .4byte gUnknown_083D613C\n\
.syntax divided\n");
}
#endif

#ifdef NONMATCHING
int sub_80C7F64(u8 a, u8 b)
{
    int unk1;
    int unk2;
    int unk4;
    int unk5;
    int unk6;
    int unk7;

    unk5 = a >> 1;
    unk1 = b & 1;
    unk2 = unk5 - sub_80C7A08(a) + 4;
    unk7 = unk2 < 0 ? unk2 + 3 : unk2;
    unk4 = (unk7 - ((unk7 >> 2) << 2));
    unk6 = gUnknown_02038800[a].e;
    return gUnknown_083D613C[unk6][(s8)unk4 * 2 + unk1];
}
#else
__attribute__((naked)) int sub_80C7F64(u8 a, u8 b)
{
    asm(".syntax unified\n\
	push {r4-r6,lr}\n\
	adds r4, r1, 0\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	adds r6, r0, 0\n\
	lsls r4, 24\n\
	lsrs r5, r4, 24\n\
	lsrs r4, 25\n\
	movs r0, 0x1\n\
	ands r5, r0\n\
	adds r0, r6, 0\n\
	bl sub_80C7A08\n\
	subs r4, r0\n\
	adds r1, r4, 0x4\n\
	adds r0, r1, 0\n\
	cmp r1, 0\n\
	bge _080C7F8A\n\
	adds r0, r4, 0x7\n\
_080C7F8A:\n\
	asrs r0, 2\n\
	lsls r0, 2\n\
	subs r0, r1, r0\n\
	ldr r1, _080C7FB0 @ =gUnknown_02038800\n\
	ldr r2, [r1]\n\
	lsls r1, r6, 3\n\
	adds r1, r2\n\
	ldrb r1, [r1, 0x4]\n\
	ldr r2, _080C7FB4 @ =gUnknown_083D613C\n\
	lsls r0, 24\n\
	asrs r0, 23\n\
	adds r0, r5\n\
	lsls r1, 3\n\
	adds r0, r1\n\
	adds r0, r2\n\
	ldrb r0, [r0]\n\
	pop {r4-r6}\n\
	pop {r1}\n\
	bx r1\n\
	.align 2, 0\n\
_080C7FB0: .4byte gUnknown_02038800\n\
_080C7FB4: .4byte gUnknown_083D613C\n\
.syntax divided\n");
}
#endif

void sub_80C7FB8(u8 a, int b)
{
    struct Sprite *sprite;

    if (gUnknown_020387F0[a] != MAX_SPRITES)
    {
        sprite = &gSprites[gUnknown_020387F0[a]];
        sprite->data1 = b;
        sprite->data2 = sub_80C7A08(a);
    }
}

#ifdef NONMATCHING
u8 sub_80C7FEC(u8 a, s16 b, s16 c)
{
    register const u8(*ptr)[][4] asm("r3");

    if (a == 2)
        ptr = &gUnknown_083D60BC;
    else if (a == 1)
        ptr = &gUnknown_083D60CC;
    else if (a == 3)
        ptr = &gUnknown_083D60DC;
    else if (a == 4)
        ptr = &gUnknown_083D60EC;
    else
        return 0xFF;

    return (*ptr)[c][b];
}
#else
__attribute__((naked)) u8 sub_80C7FEC(u8 a, s16 b, s16 c)
{
    asm(".syntax unified\n\
    	push {lr}\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	adds r3, r0, 0\n\
	lsls r1, 16\n\
	lsrs r1, 16\n\
	lsls r2, 16\n\
	lsrs r2, 16\n\
	cmp r0, 0x2\n\
	bne _080C8008\n\
	ldr r3, _080C8004 @ =gUnknown_083D60BC\n\
	b _080C802A\n\
	.align 2, 0\n\
_080C8004: .4byte gUnknown_083D60BC\n\
_080C8008:\n\
	cmp r0, 0x1\n\
	bne _080C8014\n\
	ldr r3, _080C8010 @ =gUnknown_083D60CC\n\
	b _080C802A\n\
	.align 2, 0\n\
_080C8010: .4byte gUnknown_083D60CC\n\
_080C8014:\n\
	cmp r0, 0x3\n\
	bne _080C8020\n\
	ldr r3, _080C801C @ =gUnknown_083D60DC\n\
	b _080C802A\n\
	.align 2, 0\n\
_080C801C: .4byte gUnknown_083D60DC\n\
_080C8020:\n\
	cmp r3, 0x4\n\
	beq _080C8028\n\
	movs r0, 0xFF\n\
	b _080C8038\n\
_080C8028:\n\
	ldr r3, _080C803C @ =gUnknown_083D60EC\n\
_080C802A:\n\
	lsls r0, r2, 16\n\
	lsls r1, 16\n\
	asrs r1, 16\n\
	asrs r0, 14\n\
	adds r0, r1\n\
	adds r0, r3, r0\n\
	ldrb r0, [r0]\n\
_080C8038:\n\
	pop {r1}\n\
	bx r1\n\
	.align 2, 0\n\
_080C803C: .4byte gUnknown_083D60EC\n\
.syntax divided\n");
}
#endif

void sub_80C8040(void)
{
    if (GetCurrentMapRotatingGatePuzzleType())
    {
        sub_80C7A80();
        sub_80C79CC();
    }
}

void RotatingGatePuzzleCameraUpdate(u16 a, u16 b)
{
    if (GetCurrentMapRotatingGatePuzzleType())
    {
        sub_80C7ADC(a, b);
        sub_80C7DC0();
    }
}

void sub_80C8080(void)
{
    if (GetCurrentMapRotatingGatePuzzleType())
    {
        LoadRotatingGatePics();
        sub_80C7A80();
        sub_80C7ADC(0, 0);
    }
}

bool8 CheckForRotatingGatePuzzleCollision(u8 a, s16 x, s16 y)
{
    int i;
    s16 gateX;
    s16 gateY;
    register u32 check asm("r0");
    int upperNibble;
    int lowerNibble;
    s16 finalX;
    s16 finalY;

    if (!GetCurrentMapRotatingGatePuzzleType())
    {
        return 0;
    }

    for (i = 0; i < gUnknown_02038804; i++)
    {
        gateX = gUnknown_02038800[i].pos.x + 7;
        gateY = gUnknown_02038800[i].pos.y + 7;

        if (gateX - 2 <= x && x <= gateX + 1 && gateY - 2 <= y && y <= gateY + 1)
        {
            finalX = x - gateX + 2;
            finalY = y - gateY + 2;
            check = sub_80C7FEC(a, finalX, finalY);

            if (check != 255)
            {
                upperNibble = check >> 4;
                lowerNibble = check & 0xF;

                asm("" ::"r"(lowerNibble));

                if (sub_80C7F64(i, lowerNibble))
                {
                    if (sub_80C7E8C(i, upperNibble))
                    {
                        sub_80C7FB8(i, upperNibble);
                        sub_80C7A44(i, upperNibble);
                        return 0;
                    }

                    return 1;
                }
            }
        }
    }

    return 0;
}
