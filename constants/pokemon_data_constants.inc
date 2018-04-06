	.set NATURE_HARDY,   0
	.set NATURE_LONELY,  1
	.set NATURE_BRAVE,   2
	.set NATURE_ADAMANT, 3
	.set NATURE_NAUGHTY, 4
	.set NATURE_BOLD,    5
	.set NATURE_DOCILE,  6
	.set NATURE_RELAXED, 7
	.set NATURE_IMPISH,  8
	.set NATURE_LAX,     9
	.set NATURE_TIMID,   10
	.set NATURE_HASTY,   11
	.set NATURE_SERIOUS, 12
	.set NATURE_JOLLY,   13
	.set NATURE_NAIVE,   14
	.set NATURE_MODEST,  15
	.set NATURE_MILD,    16
	.set NATURE_QUIET,   17
	.set NATURE_BASHFUL, 18
	.set NATURE_RASH,    19
	.set NATURE_CALM,    20
	.set NATURE_GENTLE,  21
	.set NATURE_SASSY,   22
	.set NATURE_CAREFUL, 23
	.set NATURE_QUIRKY,  24

	.set GROWTH_MEDIUM_FAST, 0x00
	.set GROWTH_ERRATIC,     0x01
	.set GROWTH_FLUCTUATING, 0x02
	.set GROWTH_MEDIUM_SLOW, 0x03
	.set GROWTH_FAST,        0x04
	.set GROWTH_SLOW,        0x05

	.set EGG_GROUP_NONE,         0x00
	.set EGG_GROUP_MONSTER,      0x01
	.set EGG_GROUP_WATER_1,      0x02
	.set EGG_GROUP_BUG,          0x03
	.set EGG_GROUP_FLYING,       0x04
	.set EGG_GROUP_FIELD,        0x05
	.set EGG_GROUP_FAIRY,        0x06
	.set EGG_GROUP_GRASS,        0x07
	.set EGG_GROUP_HUMAN_LIKE,   0x08
	.set EGG_GROUP_WATER_3,      0x09
	.set EGG_GROUP_MINERAL,      0x0a
	.set EGG_GROUP_AMORPHOUS,    0x0b
	.set EGG_GROUP_WATER_2,      0x0c
	.set EGG_GROUP_DITTO,        0x0d
	.set EGG_GROUP_DRAGON,       0x0e
	.set EGG_GROUP_UNDISCOVERED, 0x0f

	.set BODY_COLOR_RED,    0x00
	.set BODY_COLOR_BLUE,   0x01
	.set BODY_COLOR_YELLOW, 0x02
	.set BODY_COLOR_GREEN,  0x03
	.set BODY_COLOR_BLACK,  0x04
	.set BODY_COLOR_BROWN,  0x05
	.set BODY_COLOR_PURPLE, 0x06
	.set BODY_COLOR_GRAY,   0x07
	.set BODY_COLOR_WHITE,  0x08
	.set BODY_COLOR_PINK,   0x09

	.set F_SUMMARY_SCREEN_FLIP_SPRITE, 0x80

	.set EVO_LEVEL_FRIENDSHIP,      0x0001 @ Pokémon levels up with friendship at a certain value
	.set EVO_LEVEL_MALE,       		0x0002 @ Pokémon reaches a certain level and is male
	.set EVO_LEVEL_FEMALE,     		0x0003 @ Pokémon reaches a certain level and is female
	.set EVO_LEVEL,            		0x0004 @ Pokémon reaches the specified level
	.set EVO_LEVEL_MOVE,        	0x0005 @ Pokémon reaches a certain level knowing a move
	.set EVO_MAP,	      	 		0x0006 @ Pokémon levels up in a map
	.set EVO_ITEM,             		0x0007 @ specified item is used on Pokémon
	.set EVO_LEVEL_ATK_GT_DEF, 		0x0008 @ Pokémon reaches the specified level with attack > defense
	.set EVO_LEVEL_ATK_EQ_DEF, 		0x0009 @ Pokémon reaches the specified level with attack = defense
	.set EVO_LEVEL_ATK_LT_DEF, 		0x000a @ Pokémon reaches the specified level with attack < defense
	.set EVO_LEVEL_SILCOON,    		0x000b @ Pokémon reaches the specified level with a Silcoon personality value
	.set EVO_LEVEL_CASCOON,    		0x000c @ Pokémon reaches the specified level with a Cascoon personality value
	.set EVO_LEVEL_NINJASK,    		0x000d @ Pokémon reaches the specified level (special value for Ninjask)
	.set EVO_LEVEL_SHEDINJA,   		0x000e @ Pokémon reaches the specified level (special value for Shedinja)
	.set EVO_LEVEL_DAY_ONLY,   		0x000f @ Pokémon reaches a certain level during the day. Use when mon can evolve in twilight as well
	.set EVO_LEVEL_NIGHT_ONLY, 		0x0010 @ Pokémon reaches a certain level at night. Use when mon can evolve in twilight as well
	.set EVO_LEVEL_DAY,   	   		0x0011 @ Pokémon reaches a certain level during dawn/day
	.set EVO_LEVEL_NIGHT, 	   		0x0012 @ Pokémon reaches a certain level at dusk/night
	.set EVO_LEVEL_TWILIGHT,   		0x0013 @ Pokémon reaches a certain level at dawn/dusk
	.set EVO_LEVEL_SPATK_GT_SPDEF,  0x0014 @ Pokémon reaches the specified level with special attack > special defense
	.set EVO_LEVEL_SPATK_EQ_SPDEF,  0x0015 @ Pokémon reaches the specified level with special attack = special defense
	.set EVO_LEVEL_SPATK_LT_SPDEF,  0x0016 @ Pokémon reaches the specified level with special attack < special defense
	.set EVO_LEVEL_SPRING,			0x0017 @ Pokémon reaches a certain level in Spring
	.set EVO_LEVEL_SUMMER,			0x0018 @ Pokémon reaches a certain level in Summer
	.set EVO_LEVEL_FALL,			0x0019 @ Pokémon reaches a certain level in Fall
	.set EVO_LEVEL_WINTER,			0x001a @ Pokémon reaches a certain level in Winter
	.set EVO_LEVEL_HELD_ITEM,		0x001b @ Pokémon reaches a certain level holding an item
	.set EVO_LEVEL_HP_EV,			0x001c @ Pokémon reaches a certain level with a certain EV value
	.set EVO_LEVEL_ATK_EV,			0x001d @ Pokémon reaches a certain level with a certain EV value
	.set EVO_LEVEL_DEF_EV,			0x001e @ Pokémon reaches a certain level with a certain EV value
	.set EVO_LEVEL_SPEED_EV,		0x001f @ Pokémon reaches a certain level with a certain EV value
	.set EVO_LEVEL_SPATK_EV,		0x0020 @ Pokémon reaches a certain level with a certain EV value
	.set EVO_LEVEL_SPDEF_EV,		0x0021 @ Pokémon reaches a certain level with a certain EV value
