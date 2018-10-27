#include "global.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "battle.h"
#include "battle_message.h"
#include "data2.h"
#include "event_data.h"
#include "item.h"
#include "link.h"
#include "m4a.h"
#include "main.h"
#include "move_tutor_menu.h"
#include "pokemon.h"
#include "pokedex.h"
#include "random.h"
#include "overworld.h"
#include "rom_8077ABC.h"
#include "rom_8094928.h"
#include "rtc.h"
#include "constants/songs.h"
#include "sound.h"
#include "constants/species.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "trainer.h"
#include "util.h"
#include "ewram.h"
#include "play_time.h"

extern u8 gPlayerPartyCount;
extern u8 gEnemyPartyCount;
extern struct BattlePokemon gBattleMons[4];
extern u8 gActiveBattler;
extern struct BattleEnigmaBerry gEnigmaBerries[];
extern u16 gSpeciesToHoennPokedexNum[];
extern u16 gSpeciesToNationalPokedexNum[];
extern u16 gHoennToNationalOrder[];
extern u16 gSpeciesIdToCryId[];
extern u8 gBattleTextBuff1[];
extern u8 gBattleTextBuff2[];
extern u8 gDisplayedStringBattle[];
extern u8 gBankAttacker;
extern u8 gBankTarget;
extern u8 gStringBank;
extern u8 gBankInMenu;
extern struct SpindaSpot gSpindaSpotGraphics[];
extern s8 gNatureStatTable[][5];
extern u16 gTrainerBattleOpponent;
extern u16 gBattleTypeFlags;
extern u32 gTMHMLearnsets[][2];
extern u8 gBattleMonForms[];
extern const u8 BattleText_Wally[];
extern s8 gPokeblockFlavorCompatibilityTable[];
extern u8 gLastUsedAbility;
extern const u8 BattleText_PreventedSwitch[];
extern u16 gBattlerPartyIndexes[];

extern u8 BattleText_Rose[];
extern u8 BattleText_UnknownString3[];
extern u8 BattleText_MistShroud[];
extern u8 BattleText_GetPumped[];
extern u8 *gUnknown_08400F58[];

bool8 HealStatusConditions(struct Pokemon *mon, u32 unused, u32 healMask, u8 battleId)
{
    u32 status = GetMonData(mon, MON_DATA_STATUS, 0);

    if (status & healMask)
    {
        status &= ~healMask;
        SetMonData(mon, MON_DATA_STATUS, &status);
        if (gMain.inBattle && battleId != 4)
            gBattleMons[battleId].status1 &= ~healMask;
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

u8 GetItemEffectParamOffset(u16 itemId, u8 effectByte, u8 effectBit)
{
    const u8 *temp;
    const u8 *itemEffect;
    u8 offset;
    int i;
    u8 j;
    u8 val;

    offset = 6;

    temp = gItemEffectTable[itemId - 13];

    if (!temp && itemId != ITEM_ENIGMA_BERRY)
        return 0;

    if (itemId == ITEM_ENIGMA_BERRY)
    {
        temp = gEnigmaBerries[gActiveBattler].itemEffect;
    }

    itemEffect = temp;

    for (i = 0; i < 6; i++)
    {
        switch (i)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            if (i == effectByte)
                return 0;
            break;
        case 4:
            val = itemEffect[4];
            if (val & 0x20)
                val &= 0xDF;
            j = 0;
            while (val)
            {
                if (val & 1)
                {
                    switch (j)
                    {
                    case 2:
                        if (val & 0x10)
                            val &= 0xEF;
                    case 0:
                        if (i == effectByte && (val & effectBit))
                            return offset;
                        offset++;
                        break;
                    case 1:
                        if (i == effectByte && (val & effectBit))
                            return offset;
                        offset++;
                        break;
                    case 3:
                        if (i == effectByte && (val & effectBit))
                            return offset;
                        offset++;
                        break;
                    case 7:
                        if (i == effectByte)
                            return 0;
                        break;
                    }
                }
                j++;
                val >>= 1;
                if (i == effectByte)
                    effectBit >>= 1;
            }
            break;
        case 5:
            val = itemEffect[5];
            j = 0;
            while (val)
            {
                if (val & 1)
                {
                    switch (j)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if (i == effectByte && (val & effectBit))
                            return offset;
                        offset++;
                        break;
                    case 7:
                        if (i == effectByte)
                            return 0;
                        break;
                    }
                }
                j++;
                val >>= 1;
                if (i == effectByte)
                    effectBit >>= 1;
            }
            break;
        }
    }

    return offset;
}

const u8 gUnknown_082082F8[] = {1, 1, 3, 2, 4, 6};

void sub_803F324(int stat)
{
    gBankTarget = gBankInMenu;
    StringCopy(gBattleTextBuff1, gUnknown_08400F58[gUnknown_082082F8[stat]]);
    StringCopy(gBattleTextBuff2, BattleText_Rose);
    StrCpyDecodeToDisplayedStringBattle(BattleText_UnknownString3);
}

u8 *sub_803F378(u16 itemId)
{
    int i;
    const u8 *itemEffect;

    if (itemId == ITEM_ENIGMA_BERRY)
    {
        if (gMain.inBattle)
        {
            itemEffect = gEnigmaBerries[gBankInMenu].itemEffect;
        }
        else
        {
            itemEffect = gSaveBlock1.enigmaBerry.itemEffect;
        }
    }
    else
    {
        itemEffect = gItemEffectTable[itemId - 13];
    }

    gStringBank = gBankInMenu;

    for (i = 0; i < 3; i++)
    {
        if (itemEffect[i] & 0xF)
            sub_803F324(i * 2);
        if (itemEffect[i] & 0xF0)
        {
            if (i)
            {
                sub_803F324(i * 2 + 1);
            }
            else
            {
                gBankAttacker = gBankInMenu;
                StrCpyDecodeToDisplayedStringBattle(BattleText_GetPumped);
            }
        }
    }

    if (itemEffect[3] & 0x80)
    {
        gBankAttacker = gBankInMenu;
        StrCpyDecodeToDisplayedStringBattle(BattleText_MistShroud);
    }

    return gDisplayedStringBattle;
}

u8 GetNature(struct Pokemon *mon)
{
	return GetMonData(mon, MON_DATA_NATURE, 0); //lmao
    //return GetMonData(mon, MON_DATA_PERSONALITY, 0) % 25;
}

u8 GetNatureFromPersonality(u32 personality)
{
    return personality % 25;
}

u16 GetEvolutionTargetSpecies(struct Pokemon *mon, u8 type, u16 evolutionItem)
{
    int i;
	int moveCount;
    u16 targetSpecies = 0;
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u16 heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, 0);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, 0);
    u8 level;
    u16 friendship;
    u16 upperPersonality = personality >> 16;
    u8 holdEffect;
	u8 gender = GetMonGender(mon);
	u16 move;
	u16 mapGroup = gSaveBlock1.location.mapGroup;
	u16 mapNum = gSaveBlock1.location.mapNum;
	u8 hpEV = GetMonData(mon, MON_DATA_HP_EV, 0);
	u8 atkEV = GetMonData(mon, MON_DATA_ATK_EV, 0);
	u8 defEV = GetMonData(mon, MON_DATA_DEF_EV, 0);
	u8 speedEV = GetMonData(mon, MON_DATA_SPEED_EV, 0);
	u8 spAtkEV = GetMonData(mon, MON_DATA_SPATK_EV, 0);
	u8 spDefEV = GetMonData(mon, MON_DATA_SPDEF_EV, 0);

    if (heldItem == ITEM_ENIGMA_BERRY)
        holdEffect = gSaveBlock1.enigmaBerry.holdEffect;
    else
        holdEffect = ItemId_GetHoldEffect(heldItem);

    if (holdEffect == HOLD_EFFECT_PREVENT_EVOLVE && type != 3)
        return 0;

    switch (type)
    {
    case 0:
        level = GetMonData(mon, MON_DATA_LEVEL, 0);
        friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, 0);

        for (i = 0; i < 5; i++)
        {
            switch (gEvolutionTable[species][i].method)
            {
            case EVO_LEVEL_FRIENDSHIP:
                if (gEvolutionTable[species][i].param <= level && friendship >= gEvolutionTable[species][i].param2)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_MALE:
                if (gEvolutionTable[species][i].param <= level)
					if (gender == MON_MALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_FEMALE:
                if (gEvolutionTable[species][i].param <= level)
					if (gender == MON_FEMALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL:
                if (gEvolutionTable[species][i].param <= level)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_MOVE:
                if (gEvolutionTable[species][i].param <= level)
				{
					for (moveCount = 0; moveCount < 4; moveCount++)
					{
						move = GetMonData(mon, MON_DATA_MOVE1 + moveCount, 0);
						if (gEvolutionTable[species][i].param2 == move) 
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
							break;
					}
				}
                break;
            case EVO_MAP:
                if (gEvolutionTable[species][i].param == mapGroup && gEvolutionTable[species][i].param2 == mapNum)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_ATK_GT_DEF:
                if (gEvolutionTable[species][i].param <= level)
                    if (GetMonData(mon, MON_DATA_ATK, 0) > GetMonData(mon, MON_DATA_DEF, 0))
                        targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_ATK_EQ_DEF:
                if (gEvolutionTable[species][i].param <= level)
                    if (GetMonData(mon, MON_DATA_ATK, 0) == GetMonData(mon, MON_DATA_DEF, 0))
                        targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_ATK_LT_DEF:
                if (gEvolutionTable[species][i].param <= level)
                    if (GetMonData(mon, MON_DATA_ATK, 0) < GetMonData(mon, MON_DATA_DEF, 0))
                        targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_SILCOON:
                if (gEvolutionTable[species][i].param <= level && (upperPersonality % 10) <= 4)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_CASCOON:
                if (gEvolutionTable[species][i].param <= level && (upperPersonality % 10) > 4)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_NINJASK:
                if (gEvolutionTable[species][i].param <= level)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_DAY_ONLY:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.dayNightStatus == TIME_DAY)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_NIGHT_ONLY:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.dayNightStatus == TIME_NIGHT)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_DAY:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.dayNightStatus == TIME_DAWN || TIME_DAY)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_NIGHT:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.dayNightStatus == TIME_DUSK || TIME_NIGHT)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_TWILIGHT:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.dayNightStatus == TIME_DAWN || TIME_DUSK)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_SPATK_GT_SPDEF:
                if (gEvolutionTable[species][i].param <= level)
                    if (GetMonData(mon, MON_DATA_SPATK, 0) > GetMonData(mon, MON_DATA_SPDEF, 0))
                        targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_SPATK_EQ_SPDEF:
                if (gEvolutionTable[species][i].param <= level)
                    if (GetMonData(mon, MON_DATA_SPATK, 0) == GetMonData(mon, MON_DATA_SPDEF, 0))
                        targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_SPATK_LT_SPDEF:
                if (gEvolutionTable[species][i].param <= level)
                    if (GetMonData(mon, MON_DATA_SPATK, 0) < GetMonData(mon, MON_DATA_SPDEF, 0))
                        targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_SPRING:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.timeSeason == TIME_SEASON_SPRING)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_SUMMER:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.timeSeason == TIME_SEASON_SUMMER)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_FALL:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.timeSeason == TIME_SEASON_FALL)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_WINTER:
                if (gEvolutionTable[species][i].param <= level)
					if (gSaveBlock2.timeSeason == TIME_SEASON_WINTER)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_HELD_ITEM:
                if (gEvolutionTable[species][i].param <= level && gEvolutionTable[species][i].param2 == heldItem)
                {
                    heldItem = 0;
                    SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                }
                break;
			case EVO_LEVEL_HP_EV:
                if (gEvolutionTable[species][i].param <= level && gEvolutionTable[species][i].param2 <= hpEV)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_ATK_EV:
                if (gEvolutionTable[species][i].param <= level && gEvolutionTable[species][i].param2 <= atkEV)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_DEF_EV:
                if (gEvolutionTable[species][i].param <= level && gEvolutionTable[species][i].param2 <= defEV)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_SPEED_EV:
                if (gEvolutionTable[species][i].param <= level && gEvolutionTable[species][i].param2 <= speedEV)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_SPATK_EV:
                if (gEvolutionTable[species][i].param <= level && gEvolutionTable[species][i].param2 <= spAtkEV)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_SPDEF_EV:
                if (gEvolutionTable[species][i].param <= level && gEvolutionTable[species][i].param2 <= spDefEV)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            }
        }
        break;
    case 1:
        break;
    case 2:
    case 3:
        for (i = 0; i < 5; i++)
        {
            if (gEvolutionTable[species][i].method == EVO_ITEM
             && gEvolutionTable[species][i].param == evolutionItem)
            {
                targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            }
        }
        break;
    }

    return targetSpecies;
}

u16 HoennPokedexNumToSpecies(u16 hoennNum)
{
    u16 species;

    if (!hoennNum)
        return 0;

    species = 0;

    while (species < POKEMON_SLOTS_NUMBER - 1 && gSpeciesToHoennPokedexNum[species] != hoennNum)
        species++;

    if (species == POKEMON_SLOTS_NUMBER - 1)
        return 0;

    return species + 1;
}

u16 NationalPokedexNumToSpecies(u16 nationalNum)
{
    u16 species;

    if (!nationalNum)
        return 0;

    species = 0;

    while (species < POKEMON_SLOTS_NUMBER - 1 && gSpeciesToNationalPokedexNum[species] != nationalNum)
        species++;

    if (species == POKEMON_SLOTS_NUMBER - 1)
        return 0;

    return species + 1;
}

u16 NationalToHoennOrder(u16 nationalNum)
{
    u16 hoennNum;

    if (!nationalNum)
        return 0;

    hoennNum = 0;

    while (hoennNum < POKEMON_SLOTS_NUMBER - 1 && gHoennToNationalOrder[hoennNum] != nationalNum)
        hoennNum++;

    if (hoennNum == POKEMON_SLOTS_NUMBER - 1)
        return 0;

    return hoennNum + 1;
}

u16 SpeciesToNationalPokedexNum(u16 species)
{
    if (!species)
        return 0;

    return gSpeciesToNationalPokedexNum[species - 1];
}

u16 SpeciesToHoennPokedexNum(u16 species)
{
    if (!species)
        return 0;

    return gSpeciesToHoennPokedexNum[species - 1];
}

u16 HoennToNationalOrder(u16 hoennNum)
{
    if (!hoennNum)
        return 0;

    return gHoennToNationalOrder[hoennNum - 1];
}

u16 SpeciesToCryId(u16 species)
{
    if (species < SPECIES_OLD_UNOWN_B - 1)
        return species;

    if (species <= SPECIES_OLD_UNOWN_Z - 1)
        return SPECIES_UNOWN - 1;

    return gSpeciesIdToCryId[species - ((SPECIES_OLD_UNOWN_Z + 1) - 1)];
}

void unref_sub_803F938(u16 species, u32 personality, u8 *dest)
{
    if (species == SPECIES_SPINDA && dest != gUnknown_081FAF4C[0] && dest != gUnknown_081FAF4C[2])
    {
        int i;
        for (i = 0; i < 4; i++)
        {
            int j;
            u8 x = gSpindaSpotGraphics[i].x + ((personality & 0x0F) - 8);
            u8 y = gSpindaSpotGraphics[i].y + (((personality & 0xF0) >> 4) - 8);

            for (j = 0; j < 16; j++)
            {
                int k;
                s32 row = gSpindaSpotGraphics[i].image[j];

                for (k = x; k < x + 16; k++)
                {
                    u8 *val = dest + ((k / 8) * 32) + ((k % 8) / 2) + ((y >> 3) << 8) + ((y & 7) << 2);

                    if (row & 1)
                    {
                        if (k & 1)
                        {
                            if ((u8)((*val & 0xF0) - 0x10) <= 0x20)
                                *val += 0x40;
                        }
                        else
                        {
                            if ((u8)((*val & 0xF) - 0x01) <= 0x02)
                                *val += 0x04;
                        }
                    }

                    row >>= 1;
                }

                y++;
            }

            personality >>= 8;
        }
    }
}

void DrawSpindaSpots(u16 species, u32 personality, u8 *dest, u8 a4)
{
    if (species == SPECIES_SPINDA && a4)
    {
        int i;
        for (i = 0; i < 4; i++)
        {
            int j;
            u8 x = gSpindaSpotGraphics[i].x + ((personality & 0x0F) - 8);
            u8 y = gSpindaSpotGraphics[i].y + (((personality & 0xF0) >> 4) - 8);

            for (j = 0; j < 16; j++)
            {
                int k;
                s32 row = gSpindaSpotGraphics[i].image[j];

                for (k = x; k < x + 16; k++)
                {
                    u8 *val = dest + ((k / 8) * 32) + ((k % 8) / 2) + ((y >> 3) << 8) + ((y & 7) << 2);

                    if (row & 1)
                    {
                        if (k & 1)
                        {
                            if ((u8)((*val & 0xF0) - 0x10) <= 0x20)
                                *val += 0x40;
                        }
                        else
                        {
                            if ((u8)((*val & 0xF) - 0x01) <= 0x02)
                                *val += 0x04;
                        }
                    }

                    row >>= 1;
                }

                y++;
            }

            personality >>= 8;
        }
    }
}

void EvolutionRenameMon(struct Pokemon *mon, u16 oldSpecies, u16 newSpecies)
{
#ifdef BUGFIX_EVO_NAME
    u8 language;
    GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);
    language = GetMonData(mon, MON_DATA_LANGUAGE, &language);
    if (language == GAME_LANGUAGE && !StringCompareWithoutExtCtrlCodes(gSpeciesNames[oldSpecies], gStringVar1))
        SetMonData(mon, MON_DATA_NICKNAME, gSpeciesNames[newSpecies]);
#else
    GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);
    if (!StringCompareWithoutExtCtrlCodes(gSpeciesNames[oldSpecies], gStringVar1))
        SetMonData(mon, MON_DATA_NICKNAME, gSpeciesNames[newSpecies]);
#endif
}

bool8 sub_803FBBC(void)
{
    bool8 retVal = FALSE;
    switch (gLinkPlayers[GetMultiplayerId()].lp_field_18)
    {
    case 0:
    case 3:
        retVal = FALSE;
        break;
    case 1:
    case 2:
        retVal = TRUE;
        break;
    }
    return retVal;
}

bool8 sub_803FBFC(u8 id)
{
    bool8 retVal = FALSE;
    switch (gLinkPlayers[id].lp_field_18)
    {
    case 0:
    case 3:
        retVal = FALSE;
        break;
    case 1:
    case 2:
        retVal = TRUE;
        break;
    }
    return retVal;
}

s32 sub_803FC34(u16 a1)
{
    s32 id;
    for (id = 0; id < MAX_LINK_PLAYERS; id++)
        if (gLinkPlayers[id].lp_field_18 == a1)
            break;
    return id;
}

u8 sub_803FC58(u16 trainer)
{
    return gTrainers[trainer].encounterMusic_gender & 0x7F;
}

u16 nature_stat_mod(u8 nature, u16 n, u8 statIndex)
{
    if (statIndex < 1 || statIndex > 5)
    {
        // should just be "return n", but it wouldn't match without this
        u16 retVal = n;
        retVal++;
        retVal--;
        return retVal;
    }

    switch (gNatureStatTable[nature][statIndex - 1])
    {
    case 1:
        return (u16)(n * 110) / 100;
    case -1:
        return (u16)(n * 90) / 100;
    }

    return n;
}

// Friendship deltas. Each event has 3 separate values, depending on the mon's
// current friendship value. In general, a mon's friendship grows faster if
// its current friendship is lower. The 3 tiers are 0-99, 100-199, and 200-255.
static const s8 sFriendshipEventDeltas[][3] = {
    { 5,  3,   2}, // FRIENDSHIP_EVENT_GROW_LEVEL
    { 5,  3,   2}, // FRIENDSHIP_EVENT_VITAMIN
    { 1,  1,   0}, // FRIENDSHIP_EVENT_BATTLE_ITEM
    { 3,  2,   1}, // FRIENDSHIP_EVENT_LEAGUE_BATTLE
    { 1,  1,   0}, // FRIENDSHIP_EVENT_LEARN_TMHM
    { 1,  1,   1}, // FRIENDSHIP_EVENT_WALKING
    {-1, -1,  -1}, // FRIENDSHIP_EVENT_FAINT_SMALL
    {-5, -5, -10}, // FRIENDSHIP_EVENT_FAINT_OUTSIDE_BATTLE
    {-5, -5, -10}, // FRIENDSHIP_EVENT_FAINT_LARGE
};

void AdjustFriendship(struct Pokemon *mon, u8 event)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, 0);
    u16 heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, 0);
    u8 holdEffect;

    if (heldItem == ITEM_ENIGMA_BERRY)
    {
        if (gMain.inBattle)
            holdEffect = gEnigmaBerries[0].holdEffect;
        else
            holdEffect = gSaveBlock1.enigmaBerry.holdEffect;
    }
    else
    {
        holdEffect = ItemId_GetHoldEffect(heldItem);
    }

    if (species && species != SPECIES_EGG)
    {
        u8 friendshipLevel = 0;
        s16 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, 0);
        if (friendship > 99)
            friendshipLevel++;
        if (friendship > 199)
            friendshipLevel++;

        if ((event != FRIENDSHIP_EVENT_WALKING || !(Random() & 1))
         && (event != FRIENDSHIP_EVENT_LEAGUE_BATTLE
          || ((gBattleTypeFlags & BATTLE_TYPE_TRAINER)
           && (gTrainers[gTrainerBattleOpponent].trainerClass == TRAINER_CLASS_ELITE_FOUR
            || gTrainers[gTrainerBattleOpponent].trainerClass == TRAINER_CLASS_LEADER
            || gTrainers[gTrainerBattleOpponent].trainerClass == TRAINER_CLASS_CHAMPION))))
        {
            s8 delta = sFriendshipEventDeltas[event][friendshipLevel];
            if (delta > 0 && holdEffect == HOLD_EFFECT_HAPPINESS_UP)
                delta = (150 * delta) / 100;

            friendship += delta;
            if (delta > 0)
            {
                if (GetMonData(mon, MON_DATA_POKEBALL, 0) == ITEM_LUXURY_BALL)
                    friendship++;
                if (GetMonData(mon, MON_DATA_MET_LOCATION, 0) == sav1_map_get_name())
                    friendship++;
            }

            if (friendship < 0)
                friendship = 0;
            if (friendship > 255)
                friendship = 255;

            SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
        }
    }
}

void MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies)
{
    u8 evs[NUM_STATS];
    u16 evIncrease = 0;
    u16 totalEVs = 0;
    u16 heldItem;
    u8 holdEffect;
    int i;

    for (i = 0; i < NUM_STATS; i++)
    {
        evs[i] = GetMonData(mon, MON_DATA_HP_EV + i, 0);
        totalEVs += evs[i];
    }

    for (i = 0; i < NUM_STATS; i++)
    {
        u8 hasHadPokerus;
        int multiplier;

        if (totalEVs >= MAX_TOTAL_EVS)
            break;

        hasHadPokerus = CheckPartyHasHadPokerus(mon, 0);

        if (hasHadPokerus)
            multiplier = 2;
        else
            multiplier = 1;

        switch (i)
        {
        case 0:
            evIncrease = gBaseStats[defeatedSpecies].evYield_HP * multiplier;
            break;
        case 1:
            evIncrease = gBaseStats[defeatedSpecies].evYield_Attack * multiplier;
            break;
        case 2:
            evIncrease = gBaseStats[defeatedSpecies].evYield_Defense * multiplier;
            break;
        case 3:
            evIncrease = gBaseStats[defeatedSpecies].evYield_Speed * multiplier;
            break;
        case 4:
            evIncrease = gBaseStats[defeatedSpecies].evYield_SpAttack * multiplier;
            break;
        case 5:
            evIncrease = gBaseStats[defeatedSpecies].evYield_SpDefense * multiplier;
            break;
        }

        heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, 0);

        if (heldItem == ITEM_ENIGMA_BERRY)
        {
            if (gMain.inBattle)
                holdEffect = gEnigmaBerries[0].holdEffect;
            else
                holdEffect = gSaveBlock1.enigmaBerry.holdEffect;
        }
        else
        {
            holdEffect = ItemId_GetHoldEffect(heldItem);
        }

        if (holdEffect == HOLD_EFFECT_MACHO_BRACE)
            evIncrease *= 2;

        if (totalEVs + (s16)evIncrease > MAX_TOTAL_EVS)
            evIncrease = ((s16)evIncrease + MAX_TOTAL_EVS) - (totalEVs + evIncrease);

        if (evs[i] + (s16)evIncrease > 252)
        {
            int val1 = (s16)evIncrease + 252;
            int val2 = evs[i] + evIncrease;
            evIncrease = val1 - val2;
        }

        evs[i] += evIncrease;
        totalEVs += evIncrease;
        SetMonData(mon, MON_DATA_HP_EV + i, &evs[i]);
    }
}

u16 GetMonEVCount(struct Pokemon *mon)
{
    int i;
    u16 count = 0;

    for (i = 0; i < NUM_STATS; i++)
        count += GetMonData(mon, MON_DATA_HP_EV + i, 0);

    return count;
}

void RandomlyGivePartyPokerus(struct Pokemon *party)
{
    u16 rnd = Random();
    if (rnd == 0x4000 || rnd == 0x8000 || rnd == 0xC000)
    {
        struct Pokemon *mon;

        do
        {
            do
            {
                rnd = Random() % PARTY_SIZE;
                mon = &party[rnd];
            }
            while (!GetMonData(mon, MON_DATA_SPECIES, 0));
        }
        while (GetMonData(mon, MON_DATA_IS_EGG, 0));

        if (!(CheckPartyHasHadPokerus(party, gBitTable[rnd])))
        {
            u8 rnd2;

            do
            {
                rnd2 = Random();
            }
            while (rnd2 == 0);

            if (rnd2 & 0xF0)
                rnd2 &= 0x07;

            rnd2 |= (rnd2 << 4);
            rnd2 &= 0xF3;
            rnd2++;

            SetMonData(&party[rnd], MON_DATA_POKERUS, &rnd2);
        }
    }
}

u8 CheckPartyPokerus(struct Pokemon *party, u8 selection)
{
    u8 retVal;

    int partyIndex = 0;
    unsigned curBit = 1;
    retVal = 0;

    if (selection)
    {
        do
        {
            if ((selection & 1) && (GetMonData(&party[partyIndex], MON_DATA_POKERUS, 0) & 0xF))
                retVal |= curBit;
            partyIndex++;
            curBit <<= 1;
            selection >>= 1;
        }
        while (selection);
    }
    else if (GetMonData(&party[0], MON_DATA_POKERUS, 0) & 0xF)
    {
        retVal = 1;
    }

    return retVal;
}

u8 CheckPartyHasHadPokerus(struct Pokemon *party, u8 selection)
{
    u8 retVal;

    int partyIndex = 0;
    unsigned curBit = 1;
    retVal = 0;

    if (selection)
    {
        do
        {
            if ((selection & 1) && GetMonData(&party[partyIndex], MON_DATA_POKERUS, 0))
                retVal |= curBit;
            partyIndex++;
            curBit <<= 1;
            selection >>= 1;
        }
        while (selection);
    }
    else if (GetMonData(&party[0], MON_DATA_POKERUS, 0))
    {
        retVal = 1;
    }

    return retVal;
}

void UpdatePartyPokerusTime(u16 days)
{
    int i;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0))
        {
            u8 pokerus = GetMonData(&gPlayerParty[i], MON_DATA_POKERUS, 0);
            if (pokerus & 0xF)
            {
                if ((pokerus & 0xF) < days || days > 4)
                    pokerus &= 0xF0;
                else
                    pokerus -= days;

                SetMonData(&gPlayerParty[i], MON_DATA_POKERUS, &pokerus);
            }
        }
    }
}

void PartySpreadPokerus(struct Pokemon *party)
{
    if ((Random() % 3) == 0)
    {
        int i;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&party[i], MON_DATA_SPECIES, 0))
            {
                u8 pokerus = GetMonData(&party[i], MON_DATA_POKERUS, 0);
                u8 curPokerus = pokerus;
                if (pokerus)
                {
                    if (pokerus & 0xF)
                    {
                        // spread to adjacent party members
                        if (i != 0 && !(GetMonData(&party[i - 1], MON_DATA_POKERUS, 0) & 0xF0))
                            SetMonData(&party[i - 1], MON_DATA_POKERUS, &curPokerus);
                        if (i != (PARTY_SIZE - 1) && !(GetMonData(&party[i + 1], MON_DATA_POKERUS, 0) & 0xF0))
                        {
                            SetMonData(&party[i + 1], MON_DATA_POKERUS, &curPokerus);
                            i++;
                        }
                    }
                }
            }
        }
    }
}

bool8 TryIncrementMonLevel(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u8 nextLevel = GetMonData(mon, MON_DATA_LEVEL, 0) + 1;
    if (GetMonData(mon, MON_DATA_EXP, 0) > gExperienceTables[gBaseStats[species].growthRate][nextLevel])
    {
        SetMonData(mon, MON_DATA_LEVEL, &nextLevel);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

u32 CanMonLearnTMHM(struct Pokemon *mon, u8 tm)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, 0);
    if (species == SPECIES_EGG)
    {
        return 0;
    }
    else if (tm < 32)
    {
        u32 mask = 1 << tm;
        return gTMHMLearnsets[species][0] & mask;
    }
    else
    {
        u32 mask = 1 << (tm - 32);
        return gTMHMLearnsets[species][1] & mask;
    }
}

u8 GetMoveTutorMoves(struct Pokemon *mon, u16 *moves)
{
    u16 knownMoves[4];
    u8 numMoves = 0;
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u8 level = GetMonData(mon, MON_DATA_LEVEL, 0);
    int i, j, k;

    for (i = 0; i < 4; i++)
        knownMoves[i] = GetMonData(mon, MON_DATA_MOVE1 + i, 0);

    for (i = 0; i < MAX_MOVE_TUTOR_MOVES; i++)
    {
        u16 moveLevel;

        if (gLevelUpLearnsets[species][i] == 0xFFFF)
            break;

        moveLevel = gLevelUpLearnsets[species][i] & 0xFE00;
        if (moveLevel <= (level << 9))
        {
            for (j = 0; j < 4 && knownMoves[j] != (gLevelUpLearnsets[species][i] & 0x1FF); j++)
                ;

            if (j == 4)
            {
                for (k = 0; k < numMoves && moves[k] != (gLevelUpLearnsets[species][i] & 0x1FF); k++)
                    ;

                if (k == numMoves)
                    moves[numMoves++] = gLevelUpLearnsets[species][i] & 0x1FF;
            }
        }
    }

    return numMoves;
}

u8 GetLevelUpMovesBySpecies(u16 species, u16 *moves)
{
    u8 numMoves = 0;
    int i;

    for (i = 0; i < 20 && gLevelUpLearnsets[species][i] != 0xFFFF; i++)
         moves[numMoves++] = gLevelUpLearnsets[species][i] & 0x1FF;

     return numMoves;
}

u8 sub_8040574(struct Pokemon *mon)
{
    u16 learnedMoves[4];
    u16 moves[20];
    u8 numMoves = 0;
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, 0);
    u8 level = GetMonData(mon, MON_DATA_LEVEL, 0);
    int i, j, k;

    if (species == SPECIES_EGG)
        return 0;

    for (i = 0; i < 4; i++)
        learnedMoves[i] = GetMonData(mon, MON_DATA_MOVE1 + i, 0);

    for (i = 0; i < 20; i++)
    {
        u16 moveLevel;

        if (gLevelUpLearnsets[species][i] == 0xFFFF)
            break;

        moveLevel = gLevelUpLearnsets[species][i] & 0xFE00;

        if (moveLevel <= (level << 9))
        {
            for (j = 0; j < 4 && learnedMoves[j] != (gLevelUpLearnsets[species][i] & 0x1FF); j++)
                ;

            if (j == 4)
            {
                for (k = 0; k < numMoves && moves[k] != (gLevelUpLearnsets[species][i] & 0x1FF); k++)
                    ;

                if (k == numMoves)
                    moves[numMoves++] = gLevelUpLearnsets[species][i] & 0x1FF;
            }
        }
    }

    return numMoves;
}

u16 SpeciesToPokedexNum(u16 species)
{
    if (IsNationalPokedexEnabled())
    {
        return SpeciesToNationalPokedexNum(species);
    }
    else
    {
        species = SpeciesToHoennPokedexNum(species);
        if (species <= HOENN_DEX_COUNT)
            return species;
        return 0xFFFF;
    }
}

void ClearBattleMonForms(void)
{
    int i;
    for (i = 0; i < 4; i++)
        gBattleMonForms[i] = 0;
}

u16 GetMUS_ForBattle(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_KYOGRE_GROUDON)
        return MUS_BATTLE34;
    if (gBattleTypeFlags & BATTLE_TYPE_REGI)
        return MUS_BATTLE36;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        return MUS_BATTLE20;
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        switch (gTrainers[gTrainerBattleOpponent].trainerClass)
        {
        case 2:
        case 0x31:
            return MUS_BATTLE30;
        case 3:
        case 4:
        case 0x32:
        case 0x33:
            return MUS_BATTLE31;
        case 0x19:
            return MUS_BATTLE32;
        case 0x20:
            return MUS_BATTLE33;
        case 0x2E:
            if (!StringCompare(gTrainers[gTrainerBattleOpponent].trainerName, BattleText_Wally))
                return MUS_BATTLE20;
            return MUS_BATTLE35;
        case 0x18:
            return MUS_BATTLE38;
        default:
            return MUS_BATTLE20;
        }
    }
    return MUS_BATTLE27;
}

void sub_80408BC(void)
{
    ResetMapMusic();
    m4aMPlayAllStop();
    PlayBGM(GetMUS_ForBattle());
}

void current_map_music_set__default_for_battle(u16 song)
{
    ResetMapMusic();
    m4aMPlayAllStop();
    if (song)
        PlayNewMapMusic(song);
    else
        PlayNewMapMusic(GetMUS_ForBattle());
}

const u8 *GetMonSpritePal(struct Pokemon *mon)
{
	u8 rarity = GetMonData(mon, MON_DATA_RARITY, 0);
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, 0);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, 0);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, 0);
	
	if (species > SPECIES_EGG)
        return gMonPaletteTable[0].data;
	
	if (rarity != 0) //use rarity value if it's been set
	{
		switch (rarity)
		{
		default:
		case COSMETIC_RARITY_TYPICAL:
			return gMonPaletteTable[species].data;
		case COSMETIC_RARITY_COMMON:
			return gMonCommonPaletteTable[species].data;
		case COSMETIC_RARITY_UNCOMMON:
			return gMonUncommonPaletteTable[species].data;
		case COSMETIC_RARITY_LESSER:
			return gMonLesserPaletteTable[species].data;
		case COSMETIC_RARITY_RARE:
			return gMonRarePaletteTable[species].data;
		case COSMETIC_RARITY_ELITE:
			return gMonElitePaletteTable[species].data;
		case COSMETIC_RARITY_EXOTIC:
			return gMonExoticPaletteTable[species].data;
		case COSMETIC_RARITY_MYTHICAL:
			return gMonShinyPaletteTable[species].data;
		case COSMETIC_RARITY_GOLD:
			return gMonGoldPaletteTable[0].data;
		}
	}
	else //if not use PID & OtId
		return GetMonSpritePalFromOtIdPersonality(species, otId, personality);
}

//Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

//Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

const u8 *GetMonSpritePalFromOtIdPersonality(u16 species, u32 otId, u32 personality)
{
    switch (GetRarityOtIdPersonality(otId, personality))
	{
		case RARITY_TYPICAL:
			return gMonPaletteTable[species].data;
		case RARITY_COMMON:
			return gMonCommonPaletteTable[species].data;
		case RARITY_UNCOMMON:
			return gMonUncommonPaletteTable[species].data;
		case RARITY_LESSER:
			return gMonLesserPaletteTable[species].data;
		case RARITY_RARE:
			return gMonRarePaletteTable[species].data;
		case RARITY_ELITE:
			return gMonElitePaletteTable[species].data;
		case RARITY_EXOTIC:
			return gMonExoticPaletteTable[species].data;
		case RARITY_MYTHICAL:
			return gMonShinyPaletteTable[species].data;
		case RARITY_GOLD:
			return gMonGoldPaletteTable[0].data;
	}  
}

const struct CompressedSpritePalette *GetMonSpritePalStruct(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, 0);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, 0);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, 0);
    return GetMonSpritePalStructFromOtIdPersonality(species, otId, personality);
}

const struct CompressedSpritePalette *GetMonSpritePalStructFromOtIdPersonality(u16 species, u32 otId , u32 personality)
{
    u32 rarityValue = HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality);

	if (rarityValue == RARITY_GOLD)
		return &gMonGoldPaletteTable[0];
	else if (rarityValue < RARITY_MYTHICAL)
		return &gMonShinyPaletteTable[species];
	else if (rarityValue < RARITY_EXOTIC)
		return &gMonExoticPaletteTable[species];
	else if (rarityValue < RARITY_ELITE)
		return &gMonElitePaletteTable[species];
	else if (rarityValue < RARITY_RARE)
		return &gMonRarePaletteTable[species];
	else if (rarityValue < RARITY_LESSER)
		return &gMonLesserPaletteTable[species];
	else if (rarityValue < RARITY_UNCOMMON)
		return &gMonUncommonPaletteTable[species];
	else if (rarityValue < RARITY_COMMON)
		return &gMonCommonPaletteTable[species];
	else
		return &gMonPaletteTable[species];
}

const u16 gHMMoves[] =
{
    MOVE_CUT,
    MOVE_FLY,
    MOVE_SURF,
    MOVE_STRENGTH,
    MOVE_FLASH,
    MOVE_ROCK_SMASH,
    MOVE_WATERFALL,
    MOVE_DIVE,
    0xffff
};

bool32 IsHMMove2(u16 move)
{
    int i = 0;
    while (gHMMoves[i] != 0xFFFF)
    {
        if (gHMMoves[i++] == move)
            return TRUE;
    }
    return FALSE;
}

bool8 IsPokeSpriteNotFlipped(u16 species)
{
    return gBaseStats[species].noFlip;
}

s8 sub_8040A54(struct Pokemon *mon, u8 a2)
{
    u8 nature = GetNature(mon);
    return gPokeblockFlavorCompatibilityTable[nature * 5 + a2];
}

s8 GetPokeFlavourRelation(u32 personality, u8 a2) //being removed, don't worry about fixing the faulty nature retrieval
{
    u8 nature = GetNatureFromPersonality(personality);
    return gPokeblockFlavorCompatibilityTable[nature * 5 + a2];
}

bool8 IsTradedMon(struct Pokemon *mon)
{
    u8 otName[8];
    u32 otId;
    GetMonData(mon, MON_DATA_OT_NAME, otName);
    otId = GetMonData(mon, MON_DATA_OT_ID, 0);
    return IsOtherTrainer(otId, otName);
}

bool8 IsOtherTrainer(u32 otId, u8 *otName)
{
    if (otId == (gSaveBlock2.playerTrainerId[0] | (gSaveBlock2.playerTrainerId[1] << 8) | (gSaveBlock2.playerTrainerId[2] << 16) | (gSaveBlock2.playerTrainerId[3] << 24)))
    {
        int i;

        for (i = 0; otName[i] != EOS; i++)
            if (otName[i] != gSaveBlock2.playerName[i])
                return TRUE;
        return FALSE;
    }

    return TRUE;
}

void BoxMonRestorePP(struct BoxPokemon *);

void MonRestorePP(struct Pokemon *mon)
{
    BoxMonRestorePP(&mon->box);
}

void BoxMonRestorePP(struct BoxPokemon *boxMon)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        if (GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, 0))
        {
            u16 move = GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, 0);
            u16 bonus = GetBoxMonData(boxMon, MON_DATA_PP_BONUSES, 0);
            u8 pp = CalculatePPWithBonus(move, bonus, i);
            SetBoxMonData(boxMon, MON_DATA_PP1 + i, &pp);
        }
    }
}

void sub_8040B8C(void)
{
    gLastUsedAbility = gBattleStruct->unk160C0;;
    gBattleTextBuff1[0] = 0xFD;
    gBattleTextBuff1[1] = 4;
    gBattleTextBuff1[2] = gBattleStruct->unk16054;
    gBattleTextBuff1[4] = EOS;
    if (!GetBattlerSide(gBattleStruct->unk16054))
        gBattleTextBuff1[3] = pokemon_order_func(gBattlerPartyIndexes[gBattleStruct->unk16054]);
    else
        gBattleTextBuff1[3] = gBattlerPartyIndexes[gBattleStruct->unk16054];
    gBattleTextBuff2[0] = 0xFD;
    gBattleTextBuff2[1] = 4;
    gBattleTextBuff2[2] = gBankInMenu;
    gBattleTextBuff2[3] = pokemon_order_func(gBattlerPartyIndexes[gBankInMenu]);
    gBattleTextBuff2[4] = EOS;
    StrCpyDecodeBattle(BattleText_PreventedSwitch, gStringVar4);
}

static const u16 sWildTreasureMap[] =
{
	ITEM_TREASURE_MAP
};

void SetWildMonHeldItem(void)
{
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LEGENDARY | BATTLE_TYPE_TRAINER)))
    {
        u16 rnd = Random() % 100;
		u16 rnd2 = Random() % 1024;	//1 in 1,024 chance of holding Treasure Map
        u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, 0);
		if (rnd2 == 0)
		{
			SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &sWildTreasureMap[0]);
			return;
		}

        if (gBaseStats[species].item1 == gBaseStats[species].item2)
        {
            SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &gBaseStats[species].item1);
            return;
        }

        if (rnd > 44)
        {
            if (rnd <= 94)
                SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &gBaseStats[species].item1);
            else
                SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &gBaseStats[species].item2);
        }
    }
}

u16 GetRarity(struct Pokemon *mon)
{
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, 0);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, 0);
    return GetRarityOtIdPersonality(otId, personality);
}

u16 GetRarityOtIdPersonality(u32 otId, u32 personality)
{
    u16 retVal = RARITY_TYPICAL;
    u32 rarityValue = HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality);
	
	if (rarityValue == RARITY_GOLD)
		retVal = RARITY_GOLD;
	else if (rarityValue < RARITY_MYTHICAL)
		retVal = RARITY_MYTHICAL;
	else if (rarityValue < RARITY_EXOTIC)
		retVal = RARITY_EXOTIC;
	else if (rarityValue < RARITY_ELITE)
		retVal = RARITY_ELITE;
	else if (rarityValue < RARITY_RARE)
		retVal = RARITY_RARE;
	else if (rarityValue < RARITY_LESSER)
		retVal = RARITY_LESSER;
	else if (rarityValue < RARITY_UNCOMMON)
		retVal = RARITY_UNCOMMON;
	else if (rarityValue < RARITY_COMMON)
		retVal = RARITY_COMMON;
	
    return retVal;
}

u8 *sub_8040D08(void)
{
    u8 id = GetMultiplayerId();
    return gLinkPlayers[sub_803FC34(gLinkPlayers[id].lp_field_18 ^ 2)].name;
}

const u8 gJapaneseNidoranNames[][11] = {_("ニドラン♂"), _("ニドラン♀")};

bool32 ShouldHideGenderIconForLanguage(u16 species, u8 *name, u8 language)
{
    bool32 retVal = FALSE;
    if (species == SPECIES_NIDORAN_M || species == SPECIES_NIDORAN_F)
    {
        const u8 *speciesName;
        if (language == GAME_LANGUAGE)
        {
            speciesName = gSpeciesNames[species];
        }
        else
        {
            if (species == SPECIES_NIDORAN_M)
                speciesName = gJapaneseNidoranNames[0];
            else
                speciesName = gJapaneseNidoranNames[1];
        }
        if (!StringCompareWithoutExtCtrlCodes(name, speciesName))
            retVal = TRUE;
        else
            retVal = FALSE;
    }
    return retVal;
}

bool32 ShouldHideGenderIcon(u16 species, u8 *name)
{
    u8 language = GAME_LANGUAGE;
    if (name[0] == 0xFC && name[1] == 21)
        language = LANGUAGE_JAPANESE;
    return ShouldHideGenderIconForLanguage(species, name, language);
}

bool32 unref_sub_8040DAC(struct Pokemon *mon)
{
    u8 name[12];
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u8 language = GetMonData(mon, MON_DATA_LANGUAGE, 0);
    GetMonData(mon, MON_DATA_NICKNAME, name);
    return ShouldHideGenderIconForLanguage(species, name, language);
}
