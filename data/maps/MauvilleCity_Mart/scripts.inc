MauvilleCity_Mart_MapScripts:: @ 81573BD
	.byte 0

MauvilleCity_Mart_EventScript_1573BE:: @ 81573BE
	lock
	faceplayer
	message MauvilleCity_Mart_Text_1A0BE4
	waitmessage
	pokemart MauvilleCity_Mart_Items
	msgbox MauvilleCity_Mart_Text_1A0C02, 4
	release
	end

	.align 2
MauvilleCity_Mart_Items:: @ 81573D8
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFEND
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_X_ACCURACY
	.2byte ITEM_OCCA_BERRY
	.2byte ITEM_PASSHO_BERRY
	.2byte ITEM_WACAN_BERRY
	.2byte ITEM_RINDO_BERRY
	.2byte ITEM_YACHE_BERRY
	.2byte ITEM_CHOPLE_BERRY
	.2byte ITEM_KEBIA_BERRY
	.2byte ITEM_SHUCA_BERRY
	.2byte ITEM_COBA_BERRY
	.2byte ITEM_PAYAPA_BERRY
	.2byte ITEM_TANGA_BERRY
	.2byte ITEM_CHARTI_BERRY
	.2byte ITEM_KASIB_BERRY
	.2byte ITEM_HABAN_BERRY
	.2byte ITEM_COLBUR_BERRY
	.2byte ITEM_BABIRI_BERRY
	.2byte ITEM_CHILAN_BERRY
	.2byte ITEM_ROSELI_BERRY
	.2byte ITEM_PINAP_BERRY
	.2byte ITEM_ENIGMA_BERRY
	.2byte ITEM_NONE
	release
	end

MauvilleCity_Mart_EventScript_1573F4:: @ 81573F4
	msgbox MauvilleCity_Mart_Text_1827D3, 2
	end

MauvilleCity_Mart_EventScript_1573FD:: @ 81573FD
	msgbox MauvilleCity_Mart_Text_18287F, 2
	end
