#ifndef GUARD_ITEM_EFFECTS_H
#define GUARD_ITEM_EFFECTS_H

const u8 gItemEffect_Potion[]       = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 20};
const u8 gItemEffect_Antidote[]     = {0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
const u8 gItemEffect_BurnHeal[]     = {0x00, 0x00, 0x00, 0x08, 0x00, 0x00};
const u8 gItemEffect_IceHeal[]      = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00};
const u8 gItemEffect_Awakening[]    = {0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
const u8 gItemEffect_ParalyzeHeal[] = {0x00, 0x00, 0x00, 0x02, 0x00, 0x00};
const u8 gItemEffect_FullRestore[]  = {0x00, 0x00, 0x00, 0x3f, 0x04, 0x00, 0xff};
const u8 gItemEffect_MaxPotion[]    = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xff};
const u8 gItemEffect_HyperPotion[]  = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 200};
const u8 gItemEffect_SuperPotion[]  = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 50};
const u8 gItemEffect_FullHeal[]     = {0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};
const u8 gItemEffect_Revive[]       = {0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xfe};
const u8 gItemEffect_MaxRevive[]    = {0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff};
const u8 gItemEffect_FreshWater[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 50};
const u8 gItemEffect_SodaPop[]      = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 60};
const u8 gItemEffect_Lemonade[]     = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 80};
const u8 gItemEffect_MoomooMilk[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 100};
const u8 gItemEffect_EnergyPowder[] = {0x00, 0x00, 0x00, 0x00, 0x04, 0xe0, 50, 0xfb, 0xfb, 0xf6};
const u8 gItemEffect_EnergyRoot[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0xe0, 200, 0xf6, 0xf6, 0xf1};
const u8 gItemEffect_HealPowder[]   = {0x00, 0x00, 0x00, 0x3f, 0x00, 0xe0, 0xfb, 0xfb, 0xf6};
const u8 gItemEffect_RevivalHerb[]  = {0x00, 0x00, 0x00, 0x00, 0x44, 0xe0, 0xfe, 0xf1, 0xf1, 0xec};
const u8 gItemEffect_Ether[]        = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x0a};
const u8 gItemEffect_MaxEther[]     = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x7f};
const u8 gItemEffect_Elixir[]       = {0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0a};
const u8 gItemEffect_MaxElixir[]    = {0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x7f};
const u8 gItemEffect_LavaCookie[]   = {0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};
const u8 gItemEffect_BlueFlute[]    = {0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
const u8 gItemEffect_YellowFlute[]  = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
const u8 gItemEffect_RedFlute[]     = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00};
const u8 gItemEffect_BerryJuice[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 20};
const u8 gItemEffect_SacredAsh[]    = {0x40, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff};
const u8 gItemEffect_VitalTonic[]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04};
const u8 gItemEffect_StrongTonic[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05};
const u8 gItemEffect_GuardTonic[]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06};
const u8 gItemEffect_RapidTonic[]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07};
const u8 gItemEffect_MentalTonic[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08};
const u8 gItemEffect_ShieldTonic[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09};
const u8 gItemEffect_RollTypes[]    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
const u8 gItemEffect_RollAbility[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
const u8 gItemEffect_RollNature[]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
const u8 gItemEffect_HPUp[]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a};
const u8 gItemEffect_Protein[]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b};
const u8 gItemEffect_Iron[]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c};
const u8 gItemEffect_Carbos[]       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d};
const u8 gItemEffect_Calcium[]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e};
const u8 gItemEffect_RareCandy[]    = {0x00, 0x00, 0x00, 0x40, 0x44, 0xe0,  253, 0x05, 0x03, 0x02};
const u8 gItemEffect_PPUp[]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x05, 0x03, 0x02, 0x00, 0x10};
const u8 gItemEffect_Zinc[]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f};
const u8 gItemEffect_PPMax[]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x05, 0x03, 0x02, 0x00, 0x11};
const u8 gItemEffect_GuardSpec[]    = {0x00, 0x00, 0x00, 0x80, 0x00, 0x60, 0x01, 0x01};
const u8 gItemEffect_DireHit[]      = {0x20, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};
const u8 gItemEffect_XAttack[]      = {0x01, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};
const u8 gItemEffect_XDefend[]      = {0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};
const u8 gItemEffect_XSpeed[]       = {0x00, 0x01, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};
const u8 gItemEffect_XAccuracy[]    = {0x00, 0x00, 0x10, 0x00, 0x00, 0x60, 0x01, 0x01};
const u8 gItemEffect_XSpecial[]     = {0x00, 0x00, 0x01, 0x00, 0x00, 0x60, 0x01, 0x01};
const u8 gItemEffect_SunStone[]     = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_MoonStone[]    = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_FireStone[]    = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_ThunderStone[] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_WaterStone[]   = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_LeafStone[]    = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_FrostStone[]   = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_DawnStone[]    = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_DuskStone[]    = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_ShinyStone[]   = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};
const u8 gItemEffect_CheriBerry[]   = {0x00, 0x00, 0x00, 0x02, 0x00, 0x00};
const u8 gItemEffect_ChestoBerry[]  = {0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
const u8 gItemEffect_PechaBerry[]   = {0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
const u8 gItemEffect_RawstBerry[]   = {0x00, 0x00, 0x00, 0x08, 0x00, 0x00};
const u8 gItemEffect_AspearBerry[]  = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00};
const u8 gItemEffect_LeppaBerry[]   = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x0a};
const u8 gItemEffect_OranBerry[]    = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 10};
const u8 gItemEffect_PersimBerry[]  = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
const u8 gItemEffect_LumBerry[]     = {0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};
const u8 gItemEffect_SitrusBerry[]  = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 30};

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
    NULL,
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
    NULL,
    gItemEffect_GuardSpec,
    gItemEffect_DireHit,
    gItemEffect_XAttack,
    gItemEffect_XDefend,
    gItemEffect_XSpeed,
    gItemEffect_XAccuracy,
    gItemEffect_XSpecial,
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
    gItemEffect_ChestoBerry,
    gItemEffect_PechaBerry,
    gItemEffect_RawstBerry,
    gItemEffect_AspearBerry,
    gItemEffect_LeppaBerry,
    gItemEffect_OranBerry,
    gItemEffect_PersimBerry,
    gItemEffect_LumBerry,
    gItemEffect_SitrusBerry,
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
