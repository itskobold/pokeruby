#include "global.h"
#include "constants/battle_constants.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/species.h"
#include "battle.h"
#include "evolution_scene.h"
#include "ewram.h"
#include "item.h"
#include "main.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_item_effect.h"
#include "random.h"
#include "rom_8077ABC.h"
#include "rom_8094928.h"
#include "util.h"

extern s32 gBattleMoveDamage;
extern u8 gAbsentBattlerFlags;
extern u8 gBankInMenu;
extern u8 gBattlersCount;
extern u16 gBattlerPartyIndexes[];
extern u8 gActiveBattler;
extern u8 gStringBank;
extern u32 gStatuses3[4];
extern u8 gStatBoosted;
extern struct BattlePokemon gBattleMons[];
extern struct BattleEnigmaBerry gEnigmaBerries[];

enum
{
	STAT_BOOSTER_BERRY,	  // 0
    STAT_BOOSTER_X,       // 1
    STAT_BOOSTER_MAX      // 2
};

static const u8 sGetMonDataEVConstants[] =
{
    MON_DATA_HP_EV,
    MON_DATA_ATK_EV,
    MON_DATA_DEF_EV,
    MON_DATA_SPEED_EV,
    MON_DATA_SPDEF_EV,
    MON_DATA_SPATK_EV
};

extern u8 gPPUpReadMasks[];
extern u8 gPPUpWriteMasks[];
extern u8 gPPUpValues[];

static bool8 DoCureStatusCheck(struct Pokemon *pkmn, u8 partyIndex, u8 status, u8 sp34);
static bool8 DoCureStatus(struct Pokemon *pkmn, u8 partyIndex, u8 status, u8 sp34);

static bool8 DoCureStatusCheck(struct Pokemon *pkmn, u8 partyIndex, u8 status, u8 sp34)
{
	bool8 retVal;
	
	if (status == CURE_ALL)
	{
		for (status = CURE_SLP; status <= CURE_SECONDARY; status++)
		{
			retVal = DoCureStatus(pkmn, partyIndex, status, sp34);
			if (retVal == FALSE)
				break;
		}
	}
	else
		retVal = DoCureStatus(pkmn, partyIndex, status, sp34);
	
	return retVal;
}

static bool8 DoCureStatus(struct Pokemon *pkmn, u8 partyIndex, u8 status, u8 sp34)
{
	bool8 retVal = TRUE;
	
	if (status == CURE_SLP && HealStatusConditions(pkmn, partyIndex, 7, sp34) == 0)
	{
		if (sp34 != 4)
			gBattleMons[sp34].status2 &= ~STATUS2_NIGHTMARE;
		retVal = FALSE;
	}
	if (status == CURE_PSN && HealStatusConditions(pkmn, partyIndex, 0xF88, sp34) == 0)
		retVal = FALSE;
	if (status == CURE_BRN && HealStatusConditions(pkmn, partyIndex, 16, sp34) == 0)
		retVal = FALSE;
	if (status == CURE_FRZ && HealStatusConditions(pkmn, partyIndex, 32, sp34) == 0)
		retVal = FALSE;
	if (status == CURE_PAR && HealStatusConditions(pkmn, partyIndex, 64, sp34) == 0)
		retVal = FALSE;
	if (status == CURE_SECONDARY && gMain.inBattle && sp34 != 4 && (gBattleMons[sp34].status2 & STATUS2_CURABLE || gStatuses3[sp34] & STATUS3_CURABLE))
	{
		gBattleMons[sp34].status2 &= ~(STATUS2_CONFUSION);
		gBattleMons[sp34].status2 &= ~(STATUS2_INFATUATION);
		gBattleMons[sp34].status2 &= ~(STATUS2_NIGHTMARE);
		gBattleMons[sp34].status2 &= ~(STATUS2_CURSED);
		gBattleMons[sp34].status2 &= ~(STATUS2_TORMENT);
		gStatuses3[sp34] &= ~(STATUS3_YAWN);
		retVal = FALSE;
	}
	
	return retVal;
}

static bool8 RestorePPAllMoves(struct Pokemon *pkmn, u8 sp34, bool8 restoreAll)
{
	int i;
	u8 pp;
	u8 move;
	bool8 retVal = TRUE;
	
	for (i = 0; i < 4; i++)
	{
		pp = GetMonData(pkmn, MON_DATA_PP1 + i, NULL);
		move = GetMonData(pkmn, MON_DATA_MOVE1 + i, NULL);
		if (pp != CalculatePPWithBonus(move, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), i))
		{
			move = GetMonData(pkmn, MON_DATA_MOVE1 + i, NULL);
			
			if (restoreAll)
				pp = CalculatePPWithBonus(move, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), i);
			else
			{
				pp += 10;
			
				if (pp > CalculatePPWithBonus(move, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), i))
				{
					move = GetMonData(pkmn, MON_DATA_MOVE1 + i, NULL);
					pp = CalculatePPWithBonus(move, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), i);
				}
			}
			
			SetMonData(pkmn, MON_DATA_PP1 + i, &pp);
			if (gMain.inBattle
			 && sp34 != 4 && !(gBattleMons[sp34].status2 & 0x200000)
			 && !(gDisableStructs[sp34].unk18_b & gBitTable[i]))
				gBattleMons[sp34].pp[i] = pp;
				
			retVal = FALSE;
		}
	}
	
	return retVal;
}

bool8 PokemonUseItemEffects(struct Pokemon *pkmn, u16 item, u8 partyIndex, u8 moveIndex, u8 e);

bool8 ExecuteTableBasedItemEffect_(struct Pokemon *pkmn, u16 item, u8 partyIndex, u8 moveIndex)
{
    return PokemonUseItemEffects(pkmn, item, partyIndex, moveIndex, 0);
}

bool8 PokemonUseItemEffects(struct Pokemon *pkmn, u16 item, u8 partyIndex, u8 moveIndex, u8 e)
{
    u32 data;
	u32 data2;
	s32 cmdIndex;
	u8 sp34 = 4;
    bool8 retVal = TRUE;
	u8 medicineGroup = ItemId_GetMedicineGroup(item);
	u16 species = GetMonData(pkmn, MON_DATA_SPECIES);

    gStringBank = gBankInMenu;
    if (gMain.inBattle)
	{
        gActiveBattler = gBankInMenu;
		cmdIndex = (GetBattlerSide(gActiveBattler) != 0);
        while (cmdIndex < gBattlersCount)
        {
            if (gBattlerPartyIndexes[cmdIndex] == partyIndex)
            {
                sp34 = cmdIndex;
                break;
            }
            cmdIndex += 2;
        }
	}
    else
        gActiveBattler = 0;
		
    if (!IS_POKEMON_ITEM(item))
        return TRUE;
    if (medicineGroup == 0)
        return TRUE;
	
	switch (medicineGroup)
	{
		case MEDICINE_GROUP_HP_RESTORE:
		{
			s32 restoreAmt;
			s16 friendshipAmt = 0;
			bool8 cureStatus = FALSE;
			bool8 revive = FALSE;
			bool8 restorePP = FALSE;
			
			if (GetMonData(pkmn, MON_DATA_STATUS, NULL) != 0 || 
				(gMain.inBattle && sp34 != 4 && (gBattleMons[sp34].status2 & STATUS2_CURABLE || gStatuses3[sp34] & STATUS3_CURABLE)) ||  
				(GetMonData(pkmn, MON_DATA_MAX_HP, NULL) != GetMonData(pkmn, MON_DATA_HP, NULL)))
			{
				if (e == 0)
				{
					switch (item)
					{
						case ITEM_ORAN_BERRY:
						case ITEM_POTION:
							restoreAmt = 25;
							break;
						case ITEM_SITRUS_BERRY:
						case ITEM_SUPER_POTION:
							restoreAmt = 75;
							break;
						case ITEM_RAZZ_BERRY:
						case ITEM_ULTRA_POTION:
							restoreAmt = 150;
							break;
						case ITEM_BELUE_BERRY:
						case ITEM_HYPER_POTION:
							restoreAmt = 300;
							break;
						case ITEM_WATMEL_BERRY:
						case ITEM_MAX_POTION:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP);
							break;
						case ITEM_FULL_RESTORE:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP);
							cureStatus = TRUE;
							break;
						case ITEM_REVIVE:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP) / 2;
							revive = TRUE;
							break;
						case ITEM_MAX_REVIVE:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP);
							revive = TRUE;
							break;
						case ITEM_FULL_REVIVE:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP);
							revive = TRUE;
							restorePP = TRUE;
							break;
						case ITEM_FRESH_WATER:
						case ITEM_LEMONADE:
						case ITEM_SHALOUR_SABLE:
						case ITEM_LAVA_COOKIE:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP) / 4;
							friendshipAmt = 3;
							break;
						case ITEM_ORAN_TEA:
						case ITEM_SITRUS_JUICE:
						case ITEM_BIG_MALASADA:
						case ITEM_CASTELIACONE:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP) / 2;
							friendshipAmt = 6;
							break;
						case ITEM_MOOMOO_MILK:
						case ITEM_RAZZ_SHAKE:
						case ITEM_BELUE_KEBAB:
						case ITEM_LEAFY_SALAD:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP) * 0.75;
							friendshipAmt = 9;
							break;
						case ITEM_WATMEL_SLUSH:
						case ITEM_RARE_BERRYADE:
						case ITEM_RAGE_CANDY_BAR:
						case ITEM_CREAM_POFFIN:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP);
							friendshipAmt = 12;
							break;
						case ITEM_ENERGY_PULP:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP) / 2;
							friendshipAmt = -3;
							break;
						case ITEM_ENERGY_ROOT:
							restoreAmt = GetMonData(pkmn, MON_DATA_MAX_HP);
							friendshipAmt = -3;
							break;
						case ITEM_REVIVAL_HERB:
							restoreAmt = 1;
							revive = TRUE;
							friendshipAmt = -3;
							break;
					}
					
					data = GetMonData(pkmn, MON_DATA_HP, NULL);
					
					if ((!revive && data == 0) || (revive && data != 0))
						return TRUE;
					
					if (restoreAmt == 0)
						restoreAmt = 1;
					
					if (cureStatus)
						retVal = DoCureStatusCheck(pkmn, partyIndex, CURE_ALL, sp34);
					
					if (restorePP)
						retVal = RestorePPAllMoves(pkmn, sp34, TRUE);
					
					if (!friendshipAmt)
					{
						s16 friendship = GetMonData(pkmn, MON_DATA_HP, NULL);
						
						if (friendship + friendshipAmt < 0)
							friendship = 0;
						else if (friendship + friendshipAmt > 255)
							friendship = 255;
						SetMonData(pkmn, MON_DATA_FRIENDSHIP, &friendship);
					}
					
					data += restoreAmt;
					if (data > GetMonData(pkmn, MON_DATA_MAX_HP, NULL))
						data = GetMonData(pkmn, MON_DATA_MAX_HP, NULL);
					SetMonData(pkmn, MON_DATA_HP, &data);
					
					if (!revive)
					{
						if (GetMonData(pkmn, MON_DATA_HP, NULL) == 0)
							return TRUE;
						
						if (gMain.inBattle && sp34 != 4)
						{
							gBattleMons[sp34].hp = data;
							if (GetBattlerSide(gActiveBattler) == 0)
							{
								if (gBattleResults.unk3 < 255)
									gBattleResults.unk3++;
								data = gActiveBattler;
								gActiveBattler = sp34;
								EmitGetAttributes(0, 0, 0);
								MarkBufferBankForExecution(gActiveBattler);
								gActiveBattler = data;
							}
						}
					}
					else
					{
						if (gMain.inBattle)
						{
							if (sp34 != 4)
							{
								gAbsentBattlerFlags &= ~gBitTable[sp34];
								CopyPlayerPartyMonToBattleData(sp34, pokemon_order_func(gBattlerPartyIndexes[sp34]));
								if (GetBattlerSide(gActiveBattler) == 0 && gBattleResults.unk4 < 255)
									gBattleResults.unk4++;
							}
							else
							{
								gAbsentBattlerFlags &= ~gBitTable[gActiveBattler ^ 2];
								if (GetBattlerSide(gActiveBattler) == 0 && gBattleResults.unk4 < 255)
									gBattleResults.unk4++;
							}
						}
					}
				}
				else
					gBattleMoveDamage = -data;

				retVal = FALSE;
			}
			break;
		}
		case MEDICINE_GROUP_STATUS_RESTORE:
		{
			u8 status = ItemId_GetSecondaryId(item);
			
			retVal = DoCureStatusCheck(pkmn, partyIndex, status, sp34);
			
			if (item == ITEM_HEALING_SEEDS || item == ITEM_HEALING_DUST)
			{
				s16 friendship = GetMonData(pkmn, MON_DATA_HP, NULL);
						
				if (friendship - 3 < 0)
					friendship = 0;
				else 
					friendship -= 3;
				SetMonData(pkmn, MON_DATA_FRIENDSHIP, &friendship);
			}
			
			break;
		}
		case MEDICINE_GROUP_PP_RESTORE:
			switch (item)
			{
				case ITEM_ELIXIR:
					retVal = RestorePPAllMoves(pkmn, sp34, FALSE);
					break;
				case ITEM_MAX_ELIXIR:
					retVal = RestorePPAllMoves(pkmn, sp34, TRUE);
					break;
				default:
					data = GetMonData(pkmn, MON_DATA_PP1 + moveIndex, NULL);
					data2 = GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL);
					if (data != CalculatePPWithBonus(data2, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex))
					{
						if (item == ITEM_ETHER || item == ITEM_LEPPA_BERRY)
							data += 10;
						else //max ether/bluk berry
							data = CalculatePPWithBonus(data2, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex);
						data2 = GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL);
						if (data > CalculatePPWithBonus(data2, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex))
						{
							data2 = GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL);
							data = CalculatePPWithBonus(data2, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex);
						}
						SetMonData(pkmn, MON_DATA_PP1 + moveIndex, &data);
						if (gMain.inBattle
						 && sp34 != 4 && !(gBattleMons[sp34].status2 & 0x200000)
						 && !(gDisableStructs[sp34].unk18_b & gBitTable[moveIndex]))
							gBattleMons[sp34].pp[moveIndex] = data;
						retVal = FALSE;
					}
					break;
			}
			break;
		case MEDICINE_GROUP_PP_BOOSTER:
			data = (GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL) & gPPUpReadMasks[moveIndex]) >> (moveIndex * 2);
			
			switch (item)
			{
				case ITEM_PP_UP:
					data2 = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex);
					if (data < 3 && data2 > 4)
					{
						data = GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL) + gPPUpValues[moveIndex];
						
						SetMonData(pkmn, MON_DATA_PP_BONUSES, &data);
						data = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), data, moveIndex) - data2;
						data = GetMonData(pkmn, MON_DATA_PP1 + moveIndex, NULL) + data;
						SetMonData(pkmn, MON_DATA_PP1 + moveIndex, &data);
						retVal = FALSE;
					}
					break;
				case ITEM_PP_MAX:
					if (data < 3)
					{
						data2 = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex);
						data = GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL);
						data &= gPPUpWriteMasks[moveIndex];
						data += gPPUpValues[moveIndex] * 3;

						SetMonData(pkmn, MON_DATA_PP_BONUSES, &data);
						data = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), data, moveIndex) - data2;
						data = GetMonData(pkmn, MON_DATA_PP1 + moveIndex, NULL) + data;
						SetMonData(pkmn, MON_DATA_PP1 + moveIndex, &data);
						retVal = FALSE;
					}
					break;
			}
			break;
		case MEDICINE_GROUP_EV_VITAMIN:
		{
			int i;
			u16 evCount = GetMonEVCount(pkmn);
			u8 evGrowth = 5;
			
			if (evCount >= MAX_TOTAL_EVS)
				return TRUE; //has hit max EV count across all stats
			
			data = ItemId_GetSecondaryId(item);
			
			if (item < ITEM_POMEG_BERRY)
				evGrowth = 20;
			
			for (i = 0; i < evGrowth; i++) //EV is raised by a maximum of 20 (vitamin item) or a minimum of 4 (berry)
			{
				data2 = GetMonData(pkmn, MON_DATA_HP_EV + data);
				
				if (data2 >= 252) //EV is maxed out
				{
					if (i == 0) //hasn't looped - EV has been maxed out before vitamin was used
						return TRUE;
					else //EV was maxed during loop
						break;
				}
				else
					data2++;
				
				SetMonData(pkmn, MON_DATA_HP_EV + data, &data2);
			}
			CalculateMonStats(pkmn);
			RedrawPokemonInfoInMenu(partyIndex, pkmn);
			retVal = FALSE;
			break;
		}
		case MEDICINE_GROUP_IV_TONIC:
		{
			int i;
			data = ItemId_GetSecondaryId(item);
			
			for (i = 0; i < 4; i++) //IV is raised by a maximum of 4
			{
				data2 = GetMonData(pkmn, MON_DATA_HP_IV + data);
				
				if (data2 >= 31) //IV is maxed out
				{
					if (i == 0) //hasn't looped - IV has been maxed out before tonic was used
						return TRUE;
					else //IV was maxed during loop
						break;
				}
				else
					data2++;
				
				SetMonData(pkmn, MON_DATA_HP_IV + data, &data2);
			}
			CalculateMonStats(pkmn);
			RedrawPokemonInfoInMenu(partyIndex, pkmn);
			retVal = FALSE;
			break;
		}
		case MEDICINE_GROUP_BATTLE_ITEM:
		{
			int i;

			switch (item)
			{
				case ITEM_DIRE_HIT:
				case ITEM_WIKI_BERRY:
					if (!(gBattleMons[gActiveBattler].status2 & STATUS2_FOCUS_ENERGY))
					{
						gBattleMons[gActiveBattler].status2 |= STATUS2_FOCUS_ENERGY;
						retVal = FALSE;
					}
					break;
				case ITEM_CUSTAP_BERRY:
					if (!(gStatuses3[gActiveBattler] & STATUS3_CUSTAP_BERRY))
					{
						gStatuses3[gActiveBattler] |= STATUS3_CUSTAP_BERRY;
						retVal = FALSE;
					}
					break;
				case ITEM_GUARD_SPEC:
					if (gSideTimers[GetBattlerSide(gActiveBattler)].mistTimer == 0)
					{
						gSideTimers[GetBattlerSide(gActiveBattler)].mistTimer = 5;
						retVal = FALSE;
					}
					break;
				case ITEM_BITTER_HERB:
				{
					bool8 statReset;
					statReset = FALSE;
					
					for (i = 1; i < STAT_STAGE_EVASION + 1; i++)
					{
						if (gBattleMons[gActiveBattler].statStages[i] != 6)
						{
							gBattleMons[gActiveBattler].statStages[i] = 6;
							statReset = TRUE;
						}
						
						if (!statReset)
							return TRUE;
						else
							retVal = FALSE;
					}
					break;
				}
				default:
					data = ItemId_GetSecondaryId(item);
				
					if (data == 0xFF) //nanab berry
					{
						data = 0;
						data2 = STAT_BOOSTER_X;
					}
					else if (data > 19)
					{
						data -= 20;
						data2 = STAT_BOOSTER_MAX;
					}
					else if (data > 9)
					{
						data -= 10;
						data2 = STAT_BOOSTER_X;
					}
					else
						data2 = STAT_BOOSTER_BERRY;
					
					if (data == 0) //random stat
					{
						for (i = 1; i <= STAT_STAGE_SPDEF; i++)
						{
							if (gBattleMons[gActiveBattler].statStages[i] < 12)
								break;
							if (i == STAT_STAGE_SPDEF)
								return TRUE;
						}
						
						do
						{
							data = Random() % 5;
							data++;
						} while (!(gBattleMons[gActiveBattler].statStages[data] < 12));
						gStatBoosted = data; 
					}
						
					if (gBattleMons[gActiveBattler].statStages[data] < 12)
					{
						if (data2 == STAT_BOOSTER_BERRY)										//it's a basic stat booster berry, boost stat by 1 stage
							gBattleMons[gActiveBattler].statStages[data]++;
						else if (data2 == STAT_BOOSTER_X)										//it's a regular X item or higher tier berry, boost stat by 2 stages
							gBattleMons[gActiveBattler].statStages[data] += 2;
						else																	//it's a MAX item, maximise stat
							gBattleMons[gActiveBattler].statStages[data] = 12;
							
						if (gBattleMons[gActiveBattler].statStages[data] > 12)
							gBattleMons[gActiveBattler].statStages[data] = 12;
						
						retVal = FALSE;
					}
			}
			break;
		}
		case MEDICINE_GROUP_EXP_BOOSTER:
		{
			u8 level = GetMonData(pkmn, MON_DATA_LEVEL);
			u32 exp = GetMonData(pkmn, MON_DATA_EXP);
			
			if (item == ITEM_RARE_CANDY)
			{
				data = exp - gExperienceTables[gBaseStats[species].growthRate][level];
				data += gExperienceTables[gBaseStats[species].growthRate][level + 1];
			}
			else //candy
			{
				data = gExperienceTables[gBaseStats[species].growthRate][level + 1]
					 - gExperienceTables[gBaseStats[species].growthRate][level];
				data /= 2;
				data += exp;
			}
			
			SetMonData(pkmn, MON_DATA_EXP, &data);
			CalculateMonStats(pkmn);
			
			if (GetMonData(pkmn, MON_DATA_LEVEL) >= MAX_LEVEL) //if mon hits max level, set exp to cap
			{
				data = gExperienceTables[gBaseStats[species].growthRate][MAX_LEVEL];
				SetMonData(pkmn, MON_DATA_EXP, &data);
			}
			
			retVal = FALSE;
			break;
		}
		case MEDICINE_GROUP_HIDDEN_TYPE:
		{
			u8 hiddenType = ItemId_GetSecondaryId(item);
			
			if (GetMonData(pkmn, MON_DATA_HIDDEN_TYPE) == hiddenType)
				return TRUE;
			else
				SetMonData(pkmn, MON_DATA_HIDDEN_TYPE, &hiddenType);
			
			retVal = FALSE;
			break;
		}
		case MEDICINE_GROUP_TYPE_MODIFIER:
		{
			u8 type1 = GetMonData(pkmn, MON_DATA_TYPE_1), type2 = GetMonData(pkmn, MON_DATA_TYPE_2);
			
			switch (item)
			{
				case ITEM_ROLL_TYPES:
					if (gSaveBlock2.gameMode != GAME_MODE_SUPER_RANDOM) //only works on super random
						return TRUE;
					
					do
					{
						GenerateRandomTypes(&pkmn->box);
					} while (type1 == GetMonData(pkmn, MON_DATA_TYPE_1) && type2 == GetMonData(pkmn, MON_DATA_TYPE_2));
					retVal = FALSE;
					break;
			}
			break;
		}
		case MEDICINE_GROUP_ABILITY_MODIFIER:
		{
			u16 ability = GetMonData(pkmn, MON_DATA_ABILITY);
			
			switch (item)
			{
				case ITEM_ROLL_ABILITY:
					if (gSaveBlock2.gameMode != GAME_MODE_SUPER_RANDOM || species == SPECIES_SHEDINJA) //only works on super random & if mon is not Shedinja
						return TRUE;
					
					do
					{
						SetRandomAbility(&pkmn->box);
					} while (GetMonData(pkmn, MON_DATA_ABILITY) == ability);
					retVal = FALSE;
					break;
				case ITEM_ABILITY_CAPSULE:
					ability = 0;
				
					if (GetMonData(pkmn, MON_DATA_ABILITY) != 0) //set mon custom ability & ability bit to 0 if mon has custom ability
					{
						SetMonData(pkmn, MON_DATA_ABILITY, &ability);
						SetMonData(pkmn, MON_DATA_ALT_ABILITY, &ability);
					}
					else
					{
						if (gBaseStats[species].ability2 == 0) //if mon only has 1 ability return no effect
							return TRUE;
						
						ability = ~(GetMonData(pkmn, MON_DATA_ALT_ABILITY));
						SetMonData(pkmn, MON_DATA_ALT_ABILITY, &ability);
					}
					retVal = FALSE;
					break;
			}
			break;
		}
		case MEDICINE_GROUP_NATURE_MODIFIER:
		{
			u8 nature = GetMonData(pkmn, MON_DATA_NATURE);
		
			switch (item)
			{
				case ITEM_ROLL_NATURE:
					if (gSaveBlock2.gameMode != GAME_MODE_SUPER_RANDOM) //only works on super random
						return TRUE;
					
					do
					{
						GenerateRandomNature(&pkmn->box);
					} while (GetMonData(pkmn, MON_DATA_NATURE) == nature);
					
					CalculateMonStats(pkmn);
					retVal = FALSE;
					break;
			}
			break;
		}
	}
	
    return retVal;
}
