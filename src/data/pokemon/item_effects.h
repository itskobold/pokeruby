#ifndef GUARD_ITEM_EFFECTS_H
#define GUARD_ITEM_EFFECTS_H

const u8 gItemEffect_Potion[]         = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 20};
const u8 gItemEffect_Antidote[]       = {0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
const u8 gItemEffect_BurnHeal[]       = {0x00, 0x00, 0x00, 0x08, 0x00, 0x00};
const u8 gItemEffect_IceHeal[]        = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00};
const u8 gItemEffect_Awakening[]      = {0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
const u8 gItemEffect_ParalyzeHeal[]   = {0x00, 0x00, 0x00, 0x02, 0x00, 0x00};
const u8 gItemEffect_FullRestore[]    = {0x00, 0x00, 0x00, 0x3f, 0x04, 0x00, 0xff};
const u8 gItemEffect_MaxPotion[]      = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xff};
const u8 gItemEffect_HyperPotion[]    = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 200};
const u8 gItemEffect_SuperPotion[]    = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 50};
const u8 gItemEffect_FullHeal[]       = {0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};
const u8 gItemEffect_Revive[]         = {0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xfe};
const u8 gItemEffect_MaxRevive[]      = {0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff};
const u8 gItemEffect_FreshWater[]     = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 50};
const u8 gItemEffect_SodaPop[]        = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 60};
const u8 gItemEffect_Lemonade[]       = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 80};
const u8 gItemEffect_MoomooMilk[]     = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 100};
const u8 gItemEffect_EnergyPowder[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0xe0, 50, 0xfb, 0xfb, 0xf6};
const u8 gItemEffect_EnergyRoot[]     = {0x00, 0x00, 0x00, 0x00, 0x04, 0xe0, 200, 0xf6, 0xf6, 0xf1};
const u8 gItemEffect_HealPowder[]     = {0x00, 0x00, 0x00, 0x3f, 0x00, 0xe0, 0xfb, 0xfb, 0xf6};
const u8 gItemEffect_RevivalHerb[]    = {0x00, 0x00, 0x00, 0x00, 0x44, 0xe0, 0xfe, 0xf1, 0xf1, 0xec};
const u8 gItemEffect_Ether[]          = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x0a};
const u8 gItemEffect_MaxEther[]       = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x7f};
const u8 gItemEffect_Elixir[]         = {0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0a};
const u8 gItemEffect_MaxElixir[]      = {0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x7f};
const u8 gItemEffect_LavaCookie[]     = {0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};
const u8 gItemEffect_BlueFlute[]      = {0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
const u8 gItemEffect_YellowFlute[]    = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
const u8 gItemEffect_RedFlute[]       = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00};
const u8 gItemEffect_BerryJuice[]     = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 20};
const u8 gItemEffect_SacredAsh[]      = {0x40, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff};
const u8 gItemEffect_Candy[]          = {MEDICINE_GROUP_EXP_BOOSTER, ITEM_CANDY};
const u8 gItemEffect_VitalTonic[]     = {MEDICINE_GROUP_IV_TONIC, ITEM_VITAL_TONIC};
const u8 gItemEffect_StrongTonic[]    = {MEDICINE_GROUP_IV_TONIC, ITEM_STRONG_TONIC};
const u8 gItemEffect_GuardTonic[]     = {MEDICINE_GROUP_IV_TONIC, ITEM_GUARD_TONIC};
const u8 gItemEffect_RapidTonic[]     = {MEDICINE_GROUP_IV_TONIC, ITEM_RAPID_TONIC};
const u8 gItemEffect_MentalTonic[]    = {MEDICINE_GROUP_IV_TONIC, ITEM_MENTAL_TONIC};
const u8 gItemEffect_ShieldTonic[]    = {MEDICINE_GROUP_IV_TONIC, ITEM_SHIELD_TONIC};
const u8 gItemEffect_RollTypes[]      = {MEDICINE_GROUP_TYPE_MODIFIER, ITEM_ROLL_TYPES};
const u8 gItemEffect_RollAbility[]    = {MEDICINE_GROUP_ABILITY_MODIFIER, ITEM_ROLL_ABILITY};
const u8 gItemEffect_RollNature[]     = {MEDICINE_GROUP_NATURE_MODIFIER, ITEM_ROLL_NATURE};
const u8 gItemEffect_HPUp[]           = {MEDICINE_GROUP_EV_VITAMIN, ITEM_HP_UP};
const u8 gItemEffect_Protein[]        = {MEDICINE_GROUP_EV_VITAMIN, ITEM_PROTEIN};
const u8 gItemEffect_Iron[]           = {MEDICINE_GROUP_EV_VITAMIN, ITEM_IRON};
const u8 gItemEffect_Carbos[]         = {MEDICINE_GROUP_EV_VITAMIN, ITEM_CARBOS};
const u8 gItemEffect_Calcium[]        = {MEDICINE_GROUP_EV_VITAMIN, ITEM_CALCIUM};
const u8 gItemEffect_RareCandy[]      = {MEDICINE_GROUP_EXP_BOOSTER, ITEM_RARE_CANDY};
const u8 gItemEffect_PPUp[]           = {MEDICINE_GROUP_PP_BOOSTER, ITEM_PP_UP};
const u8 gItemEffect_Zinc[]           = {MEDICINE_GROUP_EV_VITAMIN, ITEM_ZINC};
const u8 gItemEffect_PPMax[]          = {MEDICINE_GROUP_PP_BOOSTER, ITEM_PP_MAX};
const u8 gItemEffect_AbilityCapsule[] = {MEDICINE_GROUP_ABILITY_MODIFIER, ITEM_ABILITY_CAPSULE};
const u8 gItemEffect_GuardSpec[]      = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_GUARD_SPEC};
const u8 gItemEffect_DireHit[]        = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_DIRE_HIT};
const u8 gItemEffect_XAttack[]        = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_X_ATTACK};
const u8 gItemEffect_MaxAttack[]      = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_MAX_ATTACK};
const u8 gItemEffect_XDefend[]        = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_X_DEFEND};
const u8 gItemEffect_MaxDefend[]      = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_MAX_DEFEND};
const u8 gItemEffect_XSpeed[]         = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_X_SPEED};
const u8 gItemEffect_MaxSpeed[]       = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_MAX_SPEED};
const u8 gItemEffect_XSpAtk[]         = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_X_SP_ATK};
const u8 gItemEffect_MaxSpAtk[]       = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_MAX_SP_ATK};
const u8 gItemEffect_XSpDef[]         = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_X_SP_DEF};
const u8 gItemEffect_MaxSpDef[]       = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_MAX_SP_DEF};
const u8 gItemEffect_XAccuracy[]      = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_X_ACCURACY};
const u8 gItemEffect_MaxAccuracy[]    = {MEDICINE_GROUP_BATTLE_ITEM, ITEM_MAX_ACCURACY};
const u8 gItemEffect_SunStone[]       = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_MoonStone[]      = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_FireStone[]      = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_ThunderStone[]   = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_WaterStone[]     = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_LeafStone[]      = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_FrostStone[]     = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_DawnStone[]      = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_DuskStone[]      = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_ShinyStone[]     = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_CheriBerry[]     = {0x00, 0x00, 0x00, 0x02, 0x00, 0x00};
const u8 gItemEffect_ChestoBerry[]    = {0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
const u8 gItemEffect_PechaBerry[]     = {0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
const u8 gItemEffect_RawstBerry[]     = {0x00, 0x00, 0x00, 0x08, 0x00, 0x00};
const u8 gItemEffect_AspearBerry[]    = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00};
const u8 gItemEffect_LeppaBerry[]     = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x0a};
const u8 gItemEffect_OranBerry[]      = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 10};
const u8 gItemEffect_PersimBerry[]    = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
const u8 gItemEffect_LumBerry[]       = {0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};
const u8 gItemEffect_SitrusBerry[]    = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 30};

const u8 *const gItemEffectTable[] =
{
    gItemEffect_Potion,
    gItemEffect_Antidote,
    gItemEffect_BurnHeal,
    gItemEffect_IceHeal,
    gItemEffect_Awakening,
    gItemEffect_ParalyzeHeal,
    gItemEffect_FullRestore,
    gItemEffect_MaxPotion,
    gItemEffect_HyperPotion,
    gItemEffect_SuperPotion,
    gItemEffect_FullHeal,
    gItemEffect_Revive,
    gItemEffect_MaxRevive,
    gItemEffect_FreshWater,
    gItemEffect_SodaPop,
    gItemEffect_Lemonade,
    gItemEffect_MoomooMilk,
    gItemEffect_EnergyPowder,
    gItemEffect_EnergyRoot,
    gItemEffect_HealPowder,
    gItemEffect_RevivalHerb,
    gItemEffect_Ether,
    gItemEffect_MaxEther,
    gItemEffect_Elixir,
    gItemEffect_MaxElixir,
    gItemEffect_LavaCookie,
    gItemEffect_BlueFlute,
    gItemEffect_YellowFlute,
    gItemEffect_RedFlute,
    NULL,
    NULL,
    gItemEffect_BerryJuice,
    gItemEffect_SacredAsh,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    gItemEffect_Candy,
    gItemEffect_VitalTonic,
    gItemEffect_StrongTonic,
    gItemEffect_GuardTonic,
    gItemEffect_MentalTonic,
    gItemEffect_ShieldTonic,
    gItemEffect_RapidTonic,
    gItemEffect_RollTypes,
    gItemEffect_RollAbility,
    gItemEffect_RollNature,
    gItemEffect_HPUp,
    gItemEffect_Protein,
    gItemEffect_Iron,
    gItemEffect_Carbos,
    gItemEffect_Calcium,
    gItemEffect_RareCandy,
    gItemEffect_PPUp,
    gItemEffect_Zinc,
    gItemEffect_PPMax,
    gItemEffect_AbilityCapsule,
    gItemEffect_GuardSpec,
    gItemEffect_DireHit,
    gItemEffect_XAttack,
	gItemEffect_MaxAttack,
    gItemEffect_XDefend,
	gItemEffect_MaxDefend,
    gItemEffect_XSpeed,
	gItemEffect_MaxSpeed,
	gItemEffect_XSpAtk,
	gItemEffect_MaxSpAtk,
	gItemEffect_XSpDef,
	gItemEffect_MaxSpDef,
    gItemEffect_XAccuracy,
    gItemEffect_MaxAccuracy,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    gItemEffect_SunStone,
    gItemEffect_MoonStone,
    gItemEffect_FireStone,
    gItemEffect_ThunderStone,
    gItemEffect_WaterStone,
    gItemEffect_LeafStone,
    gItemEffect_FrostStone,
    gItemEffect_DawnStone,
    gItemEffect_DuskStone,
    gItemEffect_ShinyStone,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    gItemEffect_CheriBerry,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

#endif //GUARD_ITEM_EFFECTS_H
