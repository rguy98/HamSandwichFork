#include "winpch.h"
#include "shop.h"
#include "cheat.h"
#include "items.h"
#include "dialogbits.h"
#include "scanner.h"
#include "theater.h"
#include "arcade.h"
#include "moron.h"
#include "gallery.h"
#include "goal.h"

#define NUMSHOPITEMS		(136)
#define NUMBUILTINWORLDS	(79)

word moneyAmts[11]={25,75,1500,5000,2222,250,17,300,2,716,42};

lootTable_t rarityTables[5]{
	{{50,30,18,2},3}, // regular
	{{1,97,1,1},1}, // first key
	{{38,36,24,2},2}, // every five keys
	{{20,45,30,5},1}, // every ten keys
	{{5,50,30,15},1}, // every twenty keys
};

// Money values - unlimited.
lootCoin_t moneyValues[4] = {
	{300,750},
	{751,1500},
	{1501,3000},
	{3001,5000},
};

// Limited loot items - one per player.
lootItem_t loot[4][13] = {
	{
		{SHOP_DISCOUNT, 0},
		{SHOP_DISCOUNT, 2},
		{SHOP_DISCOUNT, 4},
		{SHOP_DISCOUNT, 5},
		{SHOP_DISCOUNT, 7},
		{SHOP_DISCOUNT, 8},
		{SHOP_MODE, MODE_RASTER},
		{SHOP_MODE, MODE_SPLATTER},
		{SHOP_MODE, MODE_TEENY},
		{SHOP_GOAL, 29},
		{SHOP_WORLD, 70},
		{SHOP_WORLD, 73},
		{255, 255},
	},
	{
		{SHOP_DISCOUNT, 1},
		{SHOP_DISCOUNT, 3},
		{SHOP_DISCOUNT, 9},
		{SHOP_MODE, MODE_DISCO},
		{SHOP_MODE, MODE_REVERSE},
		{SHOP_GOAL, 59},
		{SHOP_PLAYABLE, PLAY_MYSTIC},
		{SHOP_WORLD, 17},
		{SHOP_WORLD, 65},
		{255, 255},
	},
	{
		{SHOP_DISCOUNT, 6},
		{SHOP_MODE, MODE_MANIC},
		{SHOP_GOAL, 68},
		{SHOP_PLAYABLE, PLAY_LUNATIC},
		{SHOP_WORLD, 66},
		{255, 255},
	},
	{
		{SHOP_MODE, MODE_LUDICROUS},
		{SHOP_GOAL, 69},
		{SHOP_PLAYABLE, PLAY_HAPPY},
		{255, 255},
	}
};

shopItem_t shop[NUMSHOPITEMS]={

	// bones, bats, and beyond
	{SHOP_SPOOKY,
	 400,
	 SHOP_WORLD,
	 0},	// mansion
	{SHOP_SPOOKY,
	 300,
	 SHOP_WORLD,
	 1},	// haunted house
	{SHOP_SPOOKY,
	 600,
	 SHOP_WORLD,
	 2},	// loony halloween
	{SHOP_SPOOKY,
	 200,
	 SHOP_WORLD,
	 3},	// halloween horror 1 BP
	{SHOP_SPOOKY,
	 200,
	 SHOP_WORLD,
	 4},	// halloween horror 1 MS
	{SHOP_SPOOKY,
	 200,
	 SHOP_WORLD,
	 5},	// halloween horror 1 POP
	{SHOP_SPOOKY,
	 200,
	 SHOP_WORLD,
	 6},	// halloween horror 1 MH
	{SHOP_SPOOKY,
	 400,
	 SHOP_WORLD,
	 7},	// halloween horror 2 TDM
	{SHOP_SPOOKY,
	 400,
	 SHOP_WORLD,
	 8},	// halloween horror 2 BP
	{SHOP_SPOOKY,
	 400,
	 SHOP_WORLD,
	 9},	// halloween horror 2 LK
	{SHOP_SPOOKY,
	 400,
	 SHOP_WORLD,
	 10},	// halloween horror 2 RZ
	{SHOP_SPOOKY,
	 400,
	 SHOP_WORLD,
	 11},	// halloween horror 2 POP
	{SHOP_SPOOKY,
	 400,
	 SHOP_WORLD,
	 12},	// halloween horror 2 MH
	{SHOP_SPOOKY,
	 800,
	 SHOP_WORLD,
	 13},	// halloween horror 3 BP
	{SHOP_SPOOKY,
	 800,
	 SHOP_WORLD,
	 14},	// halloween horror 3 LK
	{SHOP_SPOOKY,
	 800,
	 SHOP_WORLD,
	 15},	// halloween horror 3 POP
	{SHOP_SPOOKY,
	 800,
	 SHOP_WORLD,
	 16},	// halloween horror 3 TDM
	{SHOP_SPOOKY,
	 500,
	 SHOP_CHEAT,
	 CHEAT_BRAINS},	// get all brains cheat

	 // sputnik's
	{SHOP_SPACE,
	 600,
	 SHOP_WORLD,
	 18},	// star ramble
	{SHOP_SPACE,
	 100,
	 SHOP_WORLD,
	 19},	// ez-8675
	{SHOP_SPACE,
	 400,
	 SHOP_WORLD,
	 20},	// spaced out
	{SHOP_SPACE,
	 500,
	 SHOP_WORLD,
	 21},	// intergalactic lunacy
	{SHOP_SPACE,
	 300,
	 SHOP_WORLD,
	 22},	// spaced station
	{SHOP_SPACE,
	 200,
	 SHOP_WORLD,
	 23},	// space station loony
	{SHOP_SPACE,
	 250,
	 SHOP_WORLD,
	 24},	// space battle
	{SHOP_SPACE,
	 500,
	 SHOP_CHEAT,
	 CHEAT_SHIELD},	// energy barrier cheat
	{SHOP_SPACE,
	 2500,
	 SHOP_ABILITY,
	 ABIL_BRAIN},	// brain radar ability
	{SHOP_SPACE,
	 600,
	 SHOP_MAJOR,
	 MAJOR_ARCADE},	// arcade game fun
	{SHOP_SPACE,
	 1000,
	 SHOP_PLAYABLE,
	 PLAY_MECHA},	// play as mechabouapha

	 // pier 2
	{SHOP_WATER,
	 300,
	 SHOP_WORLD,
	 25},	// island adventure
	{SHOP_WATER,
	 100,
	 SHOP_WORLD,
	 26},	// pygmy island
	{SHOP_WATER,
	 400,
	 SHOP_WORLD,
	 27},	// waterworld
	{SHOP_WATER,
	 500,
	 SHOP_WORLD,
	 28},	// citanul island
	{SHOP_WATER,
	 300,
	 SHOP_WORLD,
	 72},	// sea quest
	{SHOP_WATER,
	 100,
	 SHOP_WORLD,
	 74},	// sea hunt
	{SHOP_WATER,
	 400,
	 SHOP_WORLD,
	 77},	// underwater antics

	{SHOP_WATER,
	 500,
	 SHOP_CHEAT,
	 CHEAT_WATER},	// waterwalk cheat
	{SHOP_WATER,
	 500,
	 SHOP_CHEAT,
	 CHEAT_OXYGEN},	// oxygen cheat
	{SHOP_WATER,
	 1000,
	 SHOP_MAJOR,
	 MAJOR_GALLERY},	// picture gallery

	 // caves
	{SHOP_CAVE,
	 400,
	 SHOP_WORLD,
	 29},	// cavern havoc
	{SHOP_CAVE,
	 300,
	 SHOP_WORLD,
	 30},	// rocky cliffs
	{SHOP_CAVE,
	 250,
	 SHOP_WORLD,
	 67},	// tunnel vision
	{SHOP_CAVE,
	 400,
	 SHOP_WORLD,
	 71},	// middle earth
	{SHOP_CAVE,
	 500,
	 SHOP_CHEAT,
	 CHEAT_LIGHT},	// light cheat
	{SHOP_CAVE,
	 500,
	 SHOP_CHEAT,
	 CHEAT_SCANNER},	// scanner cheat
	{SHOP_CAVE,
	 500,
	 SHOP_MAJOR,
	 MAJOR_CHEATMENU},	// cheat menu
	{SHOP_CAVE,
	 300,
	 SHOP_MAJOR,
	 MAJOR_RECORDBOOK},	// record book
	{SHOP_CAVE,
	 200,
	 SHOP_ABILITY,
	 ABIL_RAGE},	// rage ability
	{SHOP_CAVE,
	 1000,
	 SHOP_PLAYABLE,
	 PLAY_LUNACHIK},	// luna chick mystery character

	 // forest
	{SHOP_FOREST,
	 200,
	 SHOP_WORLD,
	 31},	// spooky woods
	{SHOP_FOREST,
	 300,
	 SHOP_WORLD,
	 32},	// creepy meadows
	{SHOP_FOREST,
	 100,
	 SHOP_WORLD,
	 33},	// wacky woods
	{SHOP_FOREST,
	 400,
	 SHOP_WORLD,
	 34},	// dead or alive
	{SHOP_FOREST,
	 500,
	 SHOP_WORLD,
	 35},	// loonies, goonies, baboonies
	{SHOP_FOREST,
	 500,
	 SHOP_CHEAT,
	 CHEAT_LIFE},	// life cheat
	{SHOP_FOREST,
	 500,
	 SHOP_CHEAT,
	 CHEAT_AMMO},	// ammo crate cheat

	 // weird
	{SHOP_WEIRD,
	 300,
	 SHOP_WORLD,
	 36},	// new world disorder
	{SHOP_WEIRD,
	 200,
	 SHOP_WORLD,
	 37},	// dimension x
	{SHOP_WEIRD,
	 300,
	 SHOP_WORLD,
	 38},	// nuclear zone
	{SHOP_WEIRD,
	 500,
	 SHOP_WORLD,
	 39},	// loony stew
	{SHOP_WEIRD,
	 500,
	 SHOP_WORLD,
	 40},	// split level lunacy
	{SHOP_WEIRD,
	 400,
	 SHOP_WORLD,
	 41},	// double trouble
	{SHOP_WEIRD,
	 500,
	 SHOP_CHEAT,
	 CHEAT_BOOM},	// kablooie cheat
	{SHOP_WEIRD,
	 1000,
	 SHOP_PLAYABLE,
	 PLAY_SHROOM},	// play as Stupid Shroom
	{SHOP_WEIRD,
	 500,
	 SHOP_CHEAT,
	 CHEAT_RAGE},	// max rage cheat

	 // adventure
	{SHOP_ADVENTURE,
	 400,
	 SHOP_WORLD,
	 42},	// bouapha's quest
	{SHOP_ADVENTURE,
	 300,
	 SHOP_WORLD,
	 43},	// bouapha's excellent expedition
	{SHOP_ADVENTURE,
	 500,
	 SHOP_WORLD,
	 44},	// jungle quest
	{SHOP_ADVENTURE,
	 200,
	 SHOP_WORLD,
	 45},	// halloween hill
	{SHOP_ADVENTURE,
	 100,
	 SHOP_WORLD,
	 46},	// field trip
	{SHOP_ADVENTURE,
	 250,
	 SHOP_WORLD,
	 47},	// the castle
	{SHOP_ADVENTURE,
	 500,
	 SHOP_CHEAT,
	 CHEAT_KEYS},	// all keys cheat
	{SHOP_ADVENTURE,
	 3000,
	 SHOP_ABILITY,
	 ABIL_CANDLE},	// candle radar ability
	{SHOP_ADVENTURE,
	 150,
	 SHOP_MAJOR,
	 MAJOR_EDITOR},	// level editor!
	{SHOP_ADVENTURE,
	 800,
	 SHOP_MAJOR,
	 MAJOR_ARCADE2},	// dig man!

	 // yeti bauer
	{SHOP_ICE,
	 200,
	 SHOP_WORLD,
	 49},	// winter wacky 1 LK
	{SHOP_ICE,
	 200,
	 SHOP_WORLD,
	 50},	// winter wacky 1 RK
	{SHOP_ICE,
	 200,
	 SHOP_WORLD,
	 51},	// winter wacky 1 BP
	{SHOP_ICE,
	 400,
	 SHOP_WORLD,
	 52},	// winter wacky 2 BP
	{SHOP_ICE,
	 400,
	 SHOP_WORLD,
	 53},	// winter wacky 2 LK
	{SHOP_ICE,
	 400,
	 SHOP_WORLD,
	 54},	// winter wacky 2 TDM
	{SHOP_ICE,
	 300,
	 SHOP_WORLD,
	 55},	// seasons beatings
	{SHOP_ICE,
	 250,
	 SHOP_WORLD,
	 56},	// icy adventures
	{SHOP_ICE,
	 500,
	 SHOP_CHEAT,
	 CHEAT_FREEZE},	// freeze enemies cheat
	{SHOP_ICE,
	 1000,
	 SHOP_MAJOR,
	 MAJOR_QUIZ},	// quiz game

	 // kids we be
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 57},	// small world
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 58},	// blast off
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 59},	// loonies all around
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 60},	// cool but hard
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 61},	// cookies
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 48},	// haunted castle
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 68},	// spooky castle 3
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 75},	// horror lord
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 76},	// space mainia
	{SHOP_KIDS,
	 150,
	 SHOP_WORLD,
	 78},	// channel: champions

	{SHOP_KIDS,
	 500,
	 SHOP_CHEAT,
	 CHEAT_HAMMERUP},	// max hammer cheat
	{SHOP_KIDS,
	 350,
	 SHOP_MAJOR,
	 MAJOR_BESTIARY},	// bestiary
	{SHOP_KIDS,
	 500,
	 SHOP_CHEAT,
	 CHEAT_NOSKID},	// no skid boots cheat
	{SHOP_KIDS,
	 2000,
	 SHOP_ABILITY,
	 ABIL_KEYCHAIN},	// keychain help ability

	 // desert
	{SHOP_DESERT,
	 200,
	 SHOP_WORLD,
	 62},	// just deserts
	{SHOP_DESERT,
	 300,
	 SHOP_WORLD,
	 63},	// desert rats
	{SHOP_DESERT,
	 100,
	 SHOP_WORLD,
	 64},	// desert arena
	{SHOP_DESERT,
	 400,
	 SHOP_WORLD,
	 69},	// desert toils

	{SHOP_DESERT,
	 500,
	 SHOP_CHEAT,
	 CHEAT_WINLEVEL},	// win level cheat
	{SHOP_DESERT,
	 500,
	 SHOP_CHEAT,
	 CHEAT_SPEED},	// super speed cheat
	{SHOP_DESERT,
	 700,
	 SHOP_MAJOR,
	 MAJOR_THEATER},	// movie theater
	{SHOP_DESERT,
	 1000,
	 SHOP_ABILITY,
	 ABIL_MOVESHOOT},	// move and shoot ability

	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 0},		// #1 - discount card #0
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 1},		// #3 - discount card #1
	{SHOP_LOCKERS,
	 0,
	 SHOP_MODE,
	 MODE_DISCO}, // #5 - disco mode
	{SHOP_LOCKERS,
	 0,
	 SHOP_GOAL,
	 68},			// #7 - goal #68
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 2},		// #8 - discount card #2
	{SHOP_LOCKERS,
	 0,
	 SHOP_PLAYABLE,
	 PLAY_HAPPY}, // #10 - playas happy stick
	{SHOP_LOCKERS,
	 0,
	 SHOP_MODE,
	 MODE_RASTER}, // #11 - TV mode
	{SHOP_LOCKERS,
	 0,
	 SHOP_WORLD,
	 70},			// #13 - world: Bouapha: HUD Inspector
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 3},			// #17 - discount card #3
	{SHOP_LOCKERS,
	 0,
	 SHOP_WORLD,
	 65},			// #18 - world: TV Lunacy
	{SHOP_LOCKERS,
	 0,
	 SHOP_WORLD,
	 66},			// #22 - world: Blockbusters
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 4},			// #24 - discount card #4
	{SHOP_LOCKERS,
	 0,
	 SHOP_WORLD,
	 73},			// #25 - world: Sprungfield
	{SHOP_LOCKERS,
	 0,
	 SHOP_GOAL,
	 69},			// #26 - goal #69
	{SHOP_LOCKERS,
	 0,
	 SHOP_MODE,
	 MODE_LUDICROUS}, // #27 - ludicrous mode
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 5},			// #28 - discount card #5
	{SHOP_LOCKERS,
	 0,
	 SHOP_GOAL,
	 29},			// #29 - gallery goal #29
	{SHOP_LOCKERS,
	 0,
	 SHOP_GOAL,
	 59},			// #31 - gallery goal #59
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 6},			// #33 - discount card #6
	{SHOP_LOCKERS,
	 0,
	 SHOP_MODE,
	 MODE_REVERSE}, // #34 - reverse mode
	{SHOP_LOCKERS,
	 0,
	 SHOP_MODE,
	 MODE_SPLATTER},// #36 - splatter mode
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 7},			// #38 - discount card #7
	{SHOP_LOCKERS,
	 0,
	 SHOP_WORLD,
	 17},			// #39 - world: Treasure Hunting
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 8},			// #40 - discount card #8

	{SHOP_LOCKERS,
	 0,
	 SHOP_MODE,
	 MODE_MANIC},	// #41 - manic mode
	{SHOP_LOCKERS,
	 0,
	 SHOP_PLAYABLE,
	 PLAY_LUNATIC},	// #44 - Dr. Lunatic
	{SHOP_LOCKERS,
	 0,
	 SHOP_DISCOUNT,
	 9},			// #45 - discount card #9
	{SHOP_LOCKERS,
	 0,
	 SHOP_MODE,
	 MODE_TEENY},	// #50 - teeny mode

	 // the hunger sign
	{SHOP_HUNGER,
	 10000,
	 SHOP_DONATION,
	 0},
};

char worldFName[NUMBUILTINWORLDS][32]={
	"mansion.dlw",			// 0
	"hauntedh.dlw",
	"loonyhalloween.dlw",

	"halloweenBP.dlw",
	"halloweenMS.dlw",
	"halloweenPOP.dlw",
	"halloweenMH.dlw",

	"halloween2TDM.dlw",
	"halloween2BP.dlw",
	"halloween2LK.dlw",
	"halloween2RZ.dlw",		// 10
	"halloween2POP.dlw",
	"halloween2MH.dlw",

	"halloween3BP.dlw",
	"halloween3LK.dlw",
	"halloween3POP.dlw",
	"halloween3TDM.dlw",

	"treasurehunting.dlw",
	"starramble.dlw",
	"ez8675.dlw",
	"spacedout.dlw",		// 20
	"interluna.dlw",
	"spaced.dlw",
	"space.dlw",
	"spacebattle.dlw",
	"islandadventure.dlw",
	"pygmy.dlw",
	"waterworld.dlw",
	"citanul.dlw",
	"cavernhavoc.dlw",
	"cliffs.dlw",			// 30
	"woods.dlw",
	"creepymeadows.dlw",
	"wackywoods.dlw",
	"dead.dlw",
	"Looniesgooniesbaboonies.dlw",
	"disorder.dlw",
	"dimensionx.dlw",
	"nuclear.dlw",
	"loonystew.dlw",
	"splitlevellunacy.dlw",		// 40
	"doubletrouble.dlw",
	"bouaphaq.dlw",
	"excellent.dlw",
	"junglequest.dlw",
	"halloweenhill.dlw",
	"fieldtrip.dlw",
	"thecastle.dlw",
	"hauntedcastle.dlw",
	"winterwackinessLK.dlw",
	"winterwackinessRK.dlw",	// 50
	"winterwackinessBP.dlw",
	"winterwackiness2BP.dlw",
	"winterwackiness2LK.dlw",
	"winterwackiness2TDM.dlw",
	"seasonsbeatings.dlw",
	"icyadv.dlw",
	"smallworld.dlw",
	"blastoff.dlw",
	"loonies.dlw",
	"coolbuthard.dlw",		// 60
	"cookies.dlw",
	"justdeserts.dlw",
	"desertrats.dlw",
	"desertarena.dlw",
	"tvlunacy.dlw",
	"blockbuster.dlw",
	"tunnelvision.dlw",
	"castlerevisited.dlw",
	"deserttoils.dlw",
	"HUD.dlw",				// 70
	"middleearth.dlw",
	"seaquest.dlw",
	"sprungfield.dlw",
	"underwater.dlw",
	"horrorlord.dlw",
	"spmainia.dlw",
	"underwaterantics.dlw",
	"champ.dlw",
};

static char shopName[][32]={
	"Bones, Bats & Beyond",
	"Sputnik's",
	"Pier 2",
	"Cave&Peril",
	"JCPiney",
	"Oddness Depot",
	"G.A.C.",
	"Yeti Bauer",
	"Kids 'We' Be",
	"Cleopatra's Secret"
};

static byte shopContents[11][4];

static byte itemCoords[]={
		0,0,0,0,		// lockers (not handled here)
		78,130,82,133,	// bonesbats&beyond
		150,131,154,135, // sputnik's
		125,131,131,134, // pier 2
		173,62,177,66,	// cave&peril
		172,14,175,16,	// JCPiney
		174,107,179,110, // oddness depot
		173,130,177,134, // GAC
		103,131,106,134, // yeti bauer
		173,84,179,89,	// kids we be
		171,37,176,40,	// cleopatra
	};

static byte modeToToggle;

byte CanPlayWorld(const char *fname)
{
	int i;

	for(i=0;i<NUMBUILTINWORLDS;i++)
	{
		if(!strcasecmp(fname,worldFName[i]))
		{
			// this is one of the built in worlds
			// so find which purchase item relates to it, and see if it's been bought
			return ItemPurchased(SHOP_WORLD,i);
		}
	}

	// if it didn't match any of the built-in names, it's an add-on, so it's okay
	return 1;
}

byte AllOfTypePurchased(byte type)
{
	int j;

	for(j=0;j<NUMSHOPITEMS;j++)
	{
		if(shop[j].type==type && !(profile.progress.purchase[j]&SIF_BOUGHT))
		{
			return 0;
		}
	}
	return 1;	// they've all been bought!
}

/*
byte AllLockersOpen(void)
{
	int j;

	for(j=0;j<NUMSHOPITEMS;j++)
	{
		if(shop[j].shop==SHOP_LOCKERS && !(profile.progress.purchase[j]&SIF_BOUGHT))
		{
			return 0;
		}
	}
	return 1;	// they've all been opened
}
*/

byte AllPurchased(void)
{
	int j;

	for(j=0;j<NUMSHOPITEMS;j++)
	{
		if(!(profile.progress.purchase[j]&SIF_BOUGHT))
		{
			return 0;
		}
	}
	return 1;	// they've all been bought!
}

float ShopPercent(void)
{
	int j;
	int bought;

	bought=0;
	for(j=0;j<NUMSHOPITEMS;j++)
		if(profile.progress.purchase[j]&SIF_BOUGHT)
			bought++;

	return ((float)bought*100.0f/(float)NUMSHOPITEMS);
}

byte ItemPurchased(byte type,byte num)
{
	return ((profile.progress.purchase[ShopItemNumber(type,num)]&SIF_BOUGHT)>0);	// true if you've bought it
}

byte ShopItemNumber(byte type,byte num)
{
	int j;

	for(j=0;j<NUMSHOPITEMS;j++)
	{
		if(shop[j].type==type && shop[j].item==num)
		{
			// here it is!
			return j;
		}
	}
	return 255;	// if you couldn't find that item at all... then how about #255
}

byte ItemsInShop(byte shopNum)
{
	int j,num;

	num=0;
	for(j=0;j<NUMSHOPITEMS;j++)
	{
		if(shop[j].shop==shopNum && (profile.progress.purchase[j]&SIF_AVAILABLE))
		{
			num++;
		}
	}

	return num;
}

byte GetRandomItemFromShop(byte shopNum,byte invalid1,byte invalid2,byte invalid3)
{
	int i;

	while(1)
	{
		i=Random(NUMSHOPITEMS);
		if(shop[i].shop==shopNum && (profile.progress.purchase[i]&SIF_AVAILABLE) && i!=invalid1 && i!=invalid2 && i!=invalid3)
			return i;
	}
	return 0;	// will never occur, just here to prevent warning
}

void AddMapShopItem(Map *map,byte item,byte x,byte y,byte *itmnum)
{
	if(item!=255)
		map->map[x+y*map->width].item=itmnum[shop[item].type];
	else
		map->map[x+y*map->width].item=ITM_NONE;
}

void SetupShops(Map *map)
{
	int i,count,x,y;
	byte itm[4];
	byte itmnum[10];
	byte lockStart;
	char statname[16];

	i=profile.progress.purchase[ShopItemNumber(SHOP_DONATION,0)];
	if(i>0)
		sprintf(statname,"bstatue%d",i+1);
	else
		strcpy(statname,"bstatue");

	map->map[87+49*map->width].item=FindItemByName(statname);

	itmnum[SHOP_WORLD]=FindItemByName("shopworld");
	itmnum[SHOP_CHEAT]=FindItemByName("shopcheat");
	itmnum[SHOP_ABILITY]=FindItemByName("shopabil");
	itmnum[SHOP_MAJOR]=FindItemByName("shopmajor");
	itmnum[SHOP_GOAL]=FindItemByName("shoppic");
	itmnum[SHOP_PLAYABLE]=FindItemByName("shopplay");

	for(i=1;i<11;i++)	// go through each shop, not counting the lockers
	{
		count=ItemsInShop(i);
		if(count>3)
		{
			itm[0]=GetRandomItemFromShop(i,255,255,255);
			itm[1]=GetRandomItemFromShop(i,itm[0],255,255);
			itm[2]=GetRandomItemFromShop(i,itm[0],itm[1],255);
			itm[3]=GetRandomItemFromShop(i,itm[0],itm[1],itm[2]);
		}
		else
		{
			itm[0]=itm[1]=itm[2]=itm[3]=255;
			if(count>=1)
				itm[0]=GetRandomItemFromShop(i,255,255,255);
			if(count>=2)
				itm[1]=GetRandomItemFromShop(i,itm[0],255,255);
			if(count>=3)
				itm[2]=GetRandomItemFromShop(i,itm[0],itm[1],255);
		}

		// now we have our list of items to install
		AddMapShopItem(map,itm[0],itemCoords[i*4+0],itemCoords[i*4+1],itmnum);
		AddMapShopItem(map,itm[1],itemCoords[i*4+0],itemCoords[i*4+3],itmnum);
		AddMapShopItem(map,itm[2],itemCoords[i*4+2],itemCoords[i*4+1],itmnum);
		AddMapShopItem(map,itm[3],itemCoords[i*4+2],itemCoords[i*4+3],itmnum);
		shopContents[i][0]=itm[0];
		shopContents[i][1]=itm[1];
		shopContents[i][2]=itm[2];
		shopContents[i][3]=itm[3];
	}

	// now open/mess with the lockers
	/*lockStart=ShopItemNumber(SHOP_DISCOUNT,0);	// this is what's in locker #0, so this is where the locker list starts
	x=124;
	y=9;
	for(i=0;i<50;i++)
	{
		if(profile.progress.purchase[i+lockStart]&SIF_BOUGHT)
		{
			// you own this one
			if(shop[i+lockStart].type==SHOP_MODE)
			{
				if(profile.progress.purchase[i+lockStart]&SIF_ACTIVE)
				{
					map->map[x+y*map->width].floor=369;
					map->map[x+y*map->width].wall=371;
				}
				else
				{
					map->map[x+y*map->width].floor=370;
					map->map[x+y*map->width].wall=372;
				}
			}
			else
			{
				map->map[x+y*map->width].floor=373;
				map->map[x+y*map->width].wall=374;
			}
		}
		x+=3;
		if((i%10)==9)
		{
			x=124;
			y+=4;
		}
	}
	*/

	if(ItemPurchased(SHOP_MAJOR,MAJOR_BESTIARY))
	{
		map->ContiguousItemChange(71,8,ITM_NONE,0);
	}
	if(ItemPurchased(SHOP_MAJOR,MAJOR_THEATER))
	{
		map->ContiguousItemChange(121,33,ITM_NONE,0);
	}
	if(ItemPurchased(SHOP_MAJOR,MAJOR_GALLERY))
	{
		map->ContiguousItemChange(120,40,ITM_NONE,0);
	}
	if(ItemPurchased(SHOP_MAJOR,MAJOR_ARCADE))
	{
		map->ContiguousItemChange(56,35,ITM_NONE,0);
	}
	if(ItemPurchased(SHOP_MAJOR,MAJOR_ARCADE2))
	{
		map->ContiguousItemChange(61,44,ITM_NONE,0);
	}
	if(ItemPurchased(SHOP_MAJOR,MAJOR_QUIZ))
	{
		map->ContiguousItemChange(55,40,ITM_NONE,0);
	}

	if(profile.progress.goal[86])
		map->map[11+20*map->width].item=0;
	if(profile.progress.goal[87])
		map->map[13+113*map->width].item=0;
}

void DefaultShopAvailability(void)
{
	int i;

	for(i=0;i<NUMSHOPITEMS;i++)
	{
		if(shop[i].type==SHOP_CHEAT || shop[i].type==SHOP_DONATION)
			profile.progress.purchase[i]=0;	// not available!
		else
			profile.progress.purchase[i]=SIF_AVAILABLE;

		//if(shop[i].shop==SHOP_LOCKERS)	// all locker items are available, deal with it!
		//	profile.progress.purchase[i]=SIF_AVAILABLE;
	}
}

const char *GetAbilityShortDesc(byte w)
{
	switch(w)
	{
		case ABIL_RAGE:
			return "Rage Power";
		case ABIL_BRAIN:
			return "Brain Radar";
		case ABIL_CANDLE:
			return "Candle Radar";
		case ABIL_MOVESHOOT:
			return "Move n' Shoot";
		case ABIL_KEYCHAIN:
			return "Keychain Helper";
	}
	return "Unknown!?";
}

///------------------------------------------

static char shopTxt[256];
static byte buying,buyMode;
static byte shopSize;
static byte cursor;

/*
void OpenLocker(int x,int y)
{
	char modeName[][16]={"Disco","TV","Ludicrous","Subliminal","Splatter","Manic","Handheld"};
	char modeDesc[][64]={
		"Enjoy the hideous discoloration of disco lighting!",
		"Like playing on your old Atari!",
		"This makes one hit all it takes to finish you off!",
		"!!yebo dna raeH",
		"Make things much messier in combat!",
		"No time for a long game of Dr. L?  Then speed things up!",
		"No, you can't take it with you!"
	};

	x-=124;
	y-=9;
	x/=3;
	y/=4;

	x=x+y*10;	// x=locker #

	x+=ShopItemNumber(SHOP_DISCOUNT,0);

	if(!(profile.progress.purchase[x]&SIF_BOUGHT))
	{
		if(profile.progress.loonyKeys-profile.progress.loonyKeysUsed<=0)
		{
			MakeNormalSound(SND_TURRETBZZT);
			return;
		}
		profile.progress.loonyKeysUsed++;
	}

	if(shop[x].type==SHOP_MODE)
	{
		if(profile.progress.purchase[x]&SIF_BOUGHT)
		{
			if(profile.progress.purchase[x]&SIF_ACTIVE)
				sprintf(shopTxt,"You close the locker, deactivating %s Mode.",modeName[shop[x].item]);
			else
				sprintf(shopTxt,"You open the locker, activating %s Mode. (%s)",modeName[shop[x].item],modeDesc[shop[x].item]);
		}
		else
		{
			sprintf(shopTxt,"You open the locker to reveal..... %s Mode! %s Bump the locker again to deactivate it.",modeName[shop[x].item],modeDesc[shop[x].item]);
			MakeNormalSound(SND_LOCKER);
		}
		modeToToggle=x;
	}
	else
	{
		if(profile.progress.purchase[x]&SIF_BOUGHT)
			strcpy(shopTxt,"You've already opened this locker, and found: ");
		else
		{
			strcpy(shopTxt,"You open the locker to reveal..... ");
			MakeNormalSound(SND_LOCKER);
		}

		switch(shop[x].type)
		{
			case SHOP_DISCOUNT:
				strcat(shopTxt,"A Discount Card for 10% off of all purchases at ");
				strcat(shopTxt,shopName[shop[x].item]);
				strcat(shopTxt,"!  Go shop 'til you drop!");
				break;
			case SHOP_EMPTY:
				strcat(shopTxt,"Nothing!! ABSOLUTELY NOTHING!!!");
				break;
			case SHOP_MONEY:
				sprintf(&shopTxt[strlen(shopTxt)],"%d coins!  Don't spend 'em all in one place!",moneyAmts[shop[x].item]);
				if(!(profile.progress.purchase[x]&SIF_BOUGHT))
					profile.progress.totalCoins+=moneyAmts[shop[x].item];
				break;
			case SHOP_PLAYABLE:
				if(shop[x].item==PLAY_HAPPY)
					strcat(shopTxt,"The playable character Happy Stick Man!  You can choose to play as him on the Profile menu.");
				else
					strcat(shopTxt,"The playable character Dr. Lunatic!  You can choose to play as him on the Profile menu.");
				break;
			case SHOP_GOAL:
				strcat(shopTxt,"A lovely piece of artwork to view in the Mall Gallery!");
				break;
			case SHOP_WORLD:
				char tmp[32],tmp2[32],tmp3[64];

				sprintf(tmp3,"worlds/%s",worldFName[shop[x].item]);
				GetWorldName(tmp3,tmp,tmp2);

				sprintf(&shopTxt[strlen(shopTxt)],"The secret bonus world \"%s\" by %s!  Play it from the World Select screen!",tmp,tmp2);
				break;
		}
	}
	profile.progress.purchase[x]|=SIF_BOUGHT;
	SetupShops(curMap);
	SendMessageToGame(MSG_SHOPNOW,0);
	shopSize=0;
	buying=255;
	cursor=0;
	buyMode=1;
	GetTaps();
	GetArrowTaps();
	if(shop[x].type==SHOP_GOAL)
		CompleteGoal(shop[x].item);
	GoalPurchase();
}
*/

void HungerSign(void)
{
	SendMessageToGame(MSG_SHOPNOW,0);
	shopSize=0;
	buying=ShopItemNumber(SHOP_DONATION,0);
	if(profile.progress.purchase[buying]==7)
	{
		strcpy(shopTxt,"Thank you for your donations!  With your help, we've completely "
					   "fixed world hunger!  On the minus side, this is only a video game.");
		buying=255;
		buyMode=1;
	}
	else
	{
		strcpy(shopTxt,"Would you care to donate 10,000 coins to world hunger relief?");
		buyMode=0;
	}
	modeToToggle=0;
	cursor=0;
	GetTaps();
	GetArrowTaps();
}

void LootCrateRequest(void)
{
	SendMessageToGame(MSG_SHOPNOW,0);
	shopSize=0;
	buying=ShopItemNumber(SHOP_UNBOX,0);
	if(profile.progress.loonyKeys-profile.progress.loonyKeysUsed<=0)
	{
		strcpy(shopTxt,"Hey buddy, don't have any LoonyKeys to trade in! "
					   "Come back when you're a little richer, hmm?");
		buying=255;
		buyMode=3;
	}
	else
	{
		strcpy(shopTxt,"Looks like you've got a LoonyKey or two on hand! "
					   "Are you in the mood to roll the dice?");
		buyMode=2;
	}
	modeToToggle=0;
	cursor=0;
	GetTaps();
	GetArrowTaps();
}

TASK(void) InitShopping(int x,int y)
{
	int i;
	char tmp[32],tmp2[32],tmp3[64];

	modeToToggle=0;

	if(x==11 && y==20)
	{
		CompleteGoal(86);
		SetupShops(curMap);
	}
	if(x==13 && y==113)
	{
		CompleteGoal(87);
		SetupShops(curMap);
	}
	if(x==59 && y==6)
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		AWAIT Bestiary(GetDisplayMGL());
		RestoreGameplayGfx();
		CO_RETURN;
	}
	if(x==135 && y==33)
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		AWAIT Theater(GetDisplayMGL());
		RestoreGameplayGfx();
		CO_RETURN;
	}
	if(y==33 && (x==57 || x==59 || x==61))
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		if(x==59 && y==33)
			AWAIT Arcade(GetDisplayMGL(),2);
		else
			AWAIT Arcade(GetDisplayMGL(),0);
		RestoreGameplayGfx();
		CO_RETURN;
	}
	if(x>=62 && x<=65 && y>=41 && y<=44)
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		if(x==62 && y==44)
			AWAIT Arcade(GetDisplayMGL(),2);
		else
			AWAIT Arcade(GetDisplayMGL(),1);
		RestoreGameplayGfx();
		CO_RETURN;
	}
	if(x==39 && y==60)
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		AWAIT Moron(GetDisplayMGL());
		player.var[7]=1;	// to clear the mumbles and change the sign
		RestoreGameplayGfx();
		CO_RETURN;
	}
	/*
		if(x>=124 && y>=9 && x<=154 && y<=30)
		{
			goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
			goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
			goodguy->dx=0;
			goodguy->dy=0;
			OpenLocker(x,y);
			RestoreGameplayGfx();
			CO_RETURN;
		}
	*/
	if(x==87 && y==54)
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		HungerSign();
		CO_RETURN;
	}
	if(x==128 && y==23)
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		LootCrateRequest();
		CO_RETURN;
	}

	if(GalleryBump(x,y))
	{
		goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		goodguy->dx=0;
		goodguy->dy=0;
		AWAIT ShowGalleryPic();
		RestoreGameplayGfx();
		CO_RETURN;
	}
	shopSize=0;
	buying=255;
	cursor=0;
	buyMode=0;

	for(i=1;i<11;i++)
	{
		if(itemCoords[i*4]==x)
		{
			if(itemCoords[i*4+1]==y)
			{
				// upper left
				buying=shopContents[i][0];
			}
			else if(itemCoords[i*4+3]==y)
			{
				// lower left
				buying=shopContents[i][1];
			}
		}
		else if(itemCoords[i*4+2]==x)
		{
			if(itemCoords[i*4+1]==y)
			{
				// upper right
				buying=shopContents[i][2];
			}
			else if(itemCoords[i*4+3]==y)
			{
				// lower right
				buying=shopContents[i][3];
			}
		}
	}

	if(buying!=255)
	{
		SendMessageToGame(MSG_SHOPNOW,0);

		strcpy(shopTxt,"Would you like to buy ");
		switch(shop[buying].type)
		{
			case SHOP_WORLD:
				sprintf(tmp3,"worlds/%s",worldFName[shop[buying].item]);
				GetWorldName(tmp3,tmp,tmp2);
				sprintf(&shopTxt[strlen(shopTxt)],"Access to the world \"%s\" by %s",tmp,tmp2);
				break;
			case SHOP_CHEAT:
				sprintf(&shopTxt[strlen(shopTxt)],"the  \"%s\" Cheat",CheatName(shop[buying].item));
				break;
			case SHOP_PLAYABLE:
				strcpy(&shopTxt[strlen(shopTxt)],"a Mysterious Playable Character");
				break;
			case SHOP_ABILITY:
				sprintf(&shopTxt[strlen(shopTxt)],"a Frozen Yogurt Of Fury (%s Flavor)",GetAbilityShortDesc(shop[buying].item));
				break;
			case SHOP_MAJOR:
				switch(shop[buying].item)
				{
					case MAJOR_THEATER:
						strcpy(&shopTxt[strlen(shopTxt)],"Magic Theater Tickets");
						break;
					case MAJOR_GALLERY:
						strcpy(&shopTxt[strlen(shopTxt)],"An Art Gallery Season Pass");
						break;
					case MAJOR_CHEATMENU:
						strcpy(&shopTxt[strlen(shopTxt)],"The Cheat Menu");
						break;
					case MAJOR_RECORDBOOK:
						strcpy(&shopTxt[strlen(shopTxt)],"A Handy Record Book");
						break;
					case MAJOR_EDITOR:
						strcpy(&shopTxt[strlen(shopTxt)],"The World Editor");
						break;
					case MAJOR_QUIZ:
						strcpy(&shopTxt[strlen(shopTxt)],"The Game Show \"Who Doesn't Want To Be A Moron?\"");
						break;
					case MAJOR_ARCADE:
						strcpy(&shopTxt[strlen(shopTxt)],"The Arcade Game \"Zappo Raiders\"");
						break;
					case MAJOR_ARCADE2:
						strcpy(&shopTxt[strlen(shopTxt)],"The Arcade Game \"Dig Man IV\"");
						break;
					case MAJOR_BESTIARY:
						strcpy(&shopTxt[strlen(shopTxt)],"A Zoo Pass");
						break;
				}
				break;
		}
		if(ItemPurchased(SHOP_DISCOUNT,shop[buying].shop-1))
			sprintf(&shopTxt[strlen(shopTxt)]," for %d Coins? (10%% off!)",shop[buying].cost*9/10);
		else
			sprintf(&shopTxt[strlen(shopTxt)]," for %d Coins?",shop[buying].cost);
	}
	GetTaps();
	GetArrowTaps();
}

void SetObtainText(void)
{
	char tmp[32],tmp2[32],tmp3[64];

	switch(shop[buying].type)
	{
		case SHOP_DONATION:
			strcpy(shopTxt,"Thank you so very much for your generous donation!");
			break;
		case SHOP_WORLD:
			sprintf(tmp3,"worlds/%s",worldFName[shop[buying].item]);
			GetWorldName(tmp3,tmp,tmp2);
			sprintf(shopTxt,"%s by %s has now been added to your available World list!",tmp,tmp2);
			break;
		case SHOP_CHEAT:
			sprintf(shopTxt,"\"%s\" has been added to your cheat list!",CheatName(shop[buying].item));
			break;
		case SHOP_ABILITY:
			switch(shop[buying].item)
			{
				case ABIL_BRAIN:
					strcpy(shopTxt,"If you fail to get a brain for 30 seconds, Brain Radar will point you to the nearest one!  If it is too obnoxious,"
								   " turn it off in the Profile menu.");
					profile.brainRadar=1;
					break;
				case ABIL_CANDLE:
					strcpy(shopTxt,"If you fail to get a candle for 30 seconds, Candle Radar will point you to the nearest one!  You can turn this off "
								   "in the Profile menu if your brain can't take it anymore.");
					profile.candleRadar=1;
					break;
				case ABIL_RAGE:
					strcpy(shopTxt,"Your Rage Gauge fills up as you smash badguys. Whenever it's higher than your current health, press both "
								   "fire buttons at the same time to unleash your rage! Effects vary depending on your "
								   "current Special Weapon.");
					break;
				case ABIL_MOVESHOOT:
					strcpy(shopTxt,"You can now throw Hammers while moving!  You move much slower while throwing Hammers, but at least it doesn't stop you "
								   "anymore.  You can turn this off in the Profile menu if it drives you mad.");
					profile.moveNShoot=1;
					break;
				case ABIL_KEYCHAIN:
					strcpy(shopTxt,"Any level you have passed, but not collected a keychain from, will show a special symbol in the hub level.  Watch for it!");
					break;
			}
			break;
		case SHOP_PLAYABLE:
			switch(shop[buying].item)
			{
				case PLAY_MECHA:
					strcpy(shopTxt,"You can now choose to play as MechaBouapha from the Profile menu!");
					break;
				case PLAY_SHROOM:
					strcpy(shopTxt,"You can now choose to play as Stupid Shroom from the Profile menu! ... If you really want to.");
					break;
				case PLAY_LUNACHIK:
					strcpy(shopTxt,"You can now choose to play as LunaChick from the Profile menu!");
					break;
			}
			break;
		case SHOP_MAJOR:
			switch(shop[buying].item)
			{
				case MAJOR_THEATER:
					strcpy(shopTxt,"Please enjoy our Unoplex theater over on the east end of the mall!");
					break;
				case MAJOR_GALLERY:
					strcpy(shopTxt,"Come visit the Mall Gallery any time you like - it's in the southeast corner of the mall.");
					break;
				case MAJOR_CHEATMENU:
					strcpy(shopTxt,"Haha cheater! You got an empty cheat menu! Now you need to buy cheats from our stores.  If "
						           "you cheat on a level, you get zero points and zero coins for it. The Cheat Menu is available when you pause the game.");
					break;
				case MAJOR_RECORDBOOK:
					strcpy(shopTxt,"Check out your new detailed statistics from the Profile menu!");
					break;
				case MAJOR_EDITOR:
					strcpy(shopTxt,"Unleash your creative juices with the Dr. Lunatic World Editor! You'll find it on the game's main menu.");
					break;
				case MAJOR_QUIZ:
					strcpy(shopTxt,"Test your Dr. Lunatic knowledge in our fabulous quiz game - and win prizes!  It's located in our arcade, on the west end of the mall.");
					break;
				case MAJOR_ARCADE:
					strcpy(shopTxt,"Test your arcade skills in this fantastic shooter game!  Located in our arcade, on the west end of the mall.");
					break;
				case MAJOR_ARCADE2:
					strcpy(shopTxt,"Dig out diamonds and battle vicious ants armed only with a water cannon!  Try it in our arcade, on the west end of the mall.");
					break;
				case MAJOR_BESTIARY:
					strcpy(shopTxt,"Please enjoy our conveniently located Mall Zoo, in the northwest corner of our mall!");
					break;
			}
			break;
	}
}

static char rarityNames[][16] = { "common", "uncommon", "rare", "epic" };

void SetUnboxText(int rarity, int value1, int value2)
{
	char tmp[32], tmp2[32], tmp3[64];

	strcpy(shopTxt, "You unboxed a ");
	strcat(shopTxt, rarityNames[rarity]);
	strcat(shopTxt, " LoonyCrate, and recieved... ");

	if (buying == 255) {
		sprintf(&shopTxt[strlen(shopTxt)], "%d coins!  Don't spend 'em all in one place!", value1);
	}
	else {
		switch (value1)
		{
			case SHOP_WORLD:
				sprintf(tmp3, "worlds/%s", worldFName[value2]);
				GetWorldName(tmp3, tmp, tmp2);
				strcat(shopTxt, " the world \"");
				strcat(shopTxt, tmp);
				strcat(shopTxt, "\" by ");
				strcat(shopTxt, tmp2);
				strcat(shopTxt, " - now available to play!");
				break;
			case SHOP_PLAYABLE:
				switch (value2)
				{
				case PLAY_LUNATIC:
					strcat(shopTxt, "the Mad Doctor, Dr. Lunatic - now selectable from the profile menu!");
					break;
				case PLAY_HAPPY:
					strcat(shopTxt, "the agent of chaos, Happy Stick Man - now selectable from the profile menu!!");
					break;
				case PLAY_MYSTIC:
					strcat(shopTxt, "the hero of Tulipton, Kid Mystic - now selectable from the profile menu!");
					break;
				}
				break;
			case SHOP_MODE:
				switch (value2)
				{
				case MODE_DISCO:
					strcat(shopTxt, " Disco Mode - enjoy the hideous discoloration of disco lighting!");
					break;
				case MODE_RASTER:
					strcat(shopTxt, " TV Mode - like playing on your old Atari!");
					break;
				case MODE_LUDICROUS:
					strcat(shopTxt, " !!yebo dna raeH - edoM esreveR");
					break;
				case MODE_REVERSE:
					strcat(shopTxt, " Disco Mode - enjoy the hideous discoloration of disco lighting!");
					break;
				case MODE_SPLATTER:
					strcat(shopTxt, " Splatter Mode - make things much messier in combat!");
					break;
				case MODE_MANIC:
					strcat(shopTxt, " Manic Mode - no time for a long game of Dr. L?  Then speed things up!");
					break;
				case MODE_TEENY:
					strcat(shopTxt, " Teeny Mode - fits right in your pocket!");
					break;
				}
				break;
			case SHOP_DISCOUNT:
				strcat(shopTxt, "A Discount Card for 10% off of all purchases at ");
				strcat(shopTxt, shopName[value2]);
				strcat(shopTxt, "!  Go shop 'til you drop!");
				break;
			case SHOP_EMPTY:
				strcat(shopTxt, "Nothing!! ABSOLUTELY NOTHING!!!");
				break;
		}
		//sprintf(&shopTxt[strlen(shopTxt)], " (buying #%d - type %d, item %d)", buying,value1,value2);
	}
}

TASK(void) UnboxCrate(void){
	byte chanceTable, rarityChance, epic, rare, uncommon, rarity, isItem=1;
	buyMode = 1;
	MakeNormalSound(SND_LOCKER);

	profile.progress.loonyKeysUsed++;

	// Decide which table to use
	/*if (profile.progress.loonyKeysUsed == 1)
		chanceTable = RAR_FIRST;
	else if (profile.progress.loonyKeysUsed % 20 == 0)
		chanceTable = RAR_EVTWENTY;
	else if (profile.progress.loonyKeysUsed % 10 == 0)
		chanceTable = RAR_EVTEN;
	else if (profile.progress.loonyKeysUsed % 5 == 0)
		chanceTable = RAR_EVTEN;
	else*/
		chanceTable = RAR_REGULAR;

	rarityChance = Random(100); // Percent chance
	//isItem = Random(rarityTables[chanceTable].itemChance) == 1 ? 1 : 0;

	epic = 100 - rarityTables[chanceTable].percentages[RNK_EPIC];
	rare = epic - rarityTables[chanceTable].percentages[RNK_RARE];
	uncommon = rare - rarityTables[chanceTable].percentages[RNK_UNCOMMON];

	// Decide rarity
	if (rarityChance < uncommon)
		rarity = RNK_COMMON;
	else if (rarityChance < rare)
		rarity = RNK_UNCOMMON;
	else if (rarityChance < epic) 
		rarity = RNK_RARE;
	else
		rarity = RNK_EPIC;

	if (isItem) { // get an item
		byte lockedIn=0, tries=0;
		int select = 0, listSize = 0, i = 0;

		while (loot[rarity][i].item != 255 && i < 13) // Get the list size, provide a failsafe to prevent endless loop
			i++;
		listSize = i+1;

		select = Random(listSize);
		while (!lockedIn && tries < listSize + 1) {
			buying = ShopItemNumber(loot[rarity][select].type, loot[rarity][select].item);

			if (!(profile.progress.purchase[buying] & SIF_BOUGHT)) {
				lockedIn = 1;
			}
			else {
				select = (select + 1) % listSize; // Move to the next available item, I guess
			}
			tries++;
		}
		if (lockedIn) { // We got an item - unlock the item!
			profile.progress.purchase[buying]++;
			SetUnboxText(rarity, loot[rarity][select].type, loot[rarity][select].item);
		}
		else { // If no item is available, default to coins
			isItem = 0;
		}
	}
	if (!isItem) { // get a coin value
		word coins = 1000;
		profile.progress.totalCoins += coins;
		buying = 255;
		SetUnboxText(rarity,coins,0);
	}
	
	SetupShops(curMap);
}


TASK(void) BuyItem(void)
{
	int i;
	word cost;

	if(shop[buying].shop<11 && shop[buying].shop>0 && ItemPurchased(SHOP_DISCOUNT,shop[buying].shop-1))
		cost=shop[buying].cost*9/10;
	else
		cost=shop[buying].cost;

	buyMode=1;
	if(profile.progress.totalCoins-profile.progress.coinsSpent>=cost)
	{
		profile.progress.coinsSpent+=cost;
		SetObtainText();
		if(shop[buying].type==SHOP_DONATION)
		{
			profile.progress.purchase[buying]++;
			if(profile.progress.purchase[buying]==7)	// solved world hunger!
			{
				CompleteGoal(88);
				strcpy(shopTxt,"With your incredible contributions, we've actually managed to wipe out world hunger!  "
					"Thanks, dude!  Let's celebrate with a packet of our new product, Soylent Orange!");
				MakeNormalSound(SND_VICTORY);
				char statue[] = "statue.flc";
				AWAIT ShowImageOrFlic(statue,1,0);
			}
		}
		else
		{
			profile.progress.purchase[buying]&=(~SIF_AVAILABLE);
			profile.progress.purchase[buying]|=SIF_BOUGHT;
			MakeNormalSound(SND_BUYITEM);
			if(shop[buying].type==SHOP_MAJOR && shop[buying].item==MAJOR_CHEATMENU)
			{
				// if you bought the cheat menu, cheats are now buyable
				for(i=0;i<NUMSHOPITEMS;i++)
				{
					if(shop[i].type==SHOP_CHEAT)
						profile.progress.purchase[i]|=SIF_AVAILABLE;
				}
			}
		}

		SetupShops(curMap);
	}
	else
	{
		strcpy(shopTxt,"Hey buddy, you don't have enough coins to buy that!");
	}
	GoalPurchase();
}

TASK(byte) UpdateShopping(MGLDraw *mgl)
{
	byte c;

	goodguy->x=(goodguy->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
	goodguy->y=(goodguy->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
	goodguy->dx=0;
	goodguy->dy=0;

	if(shopSize<150)
		shopSize+=8;

	if(mgl->LastKeyPressed()==27)
		CO_RETURN 0;	// ESC to cancel shopping

	c=GetTaps()|GetArrowTaps();

	if(buyMode==0)
	{
		if(c&CONTROL_LF)
			cursor=0;
		if(c&CONTROL_RT)
			cursor=1;

		if(c&CONTROL_B1)
		{
			if(cursor==1)
				AWAIT BuyItem();
			else
				CO_RETURN 0;
		}
	}
	else if(buyMode==2) // unbox crate time!
	{
		if(c&CONTROL_LF)
			cursor=0;
		if(c&CONTROL_RT)
			cursor=1;

		if(c&CONTROL_B1)
		{
			if(cursor==1)
				AWAIT UnboxCrate();
			else
				CO_RETURN 0;
		}
	}
	else
	{
		if(c&CONTROL_B1)
		{
			if(modeToToggle)
			{
				profile.progress.purchase[modeToToggle]^=SIF_ACTIVE;
				if(shop[modeToToggle].item==MODE_TEENY && (profile.progress.purchase[modeToToggle]&SIF_ACTIVE))
				{
					GetDisplayMGL()->LoadBMP("graphics/gamepal.bmp");
					AWAIT GetDisplayMGL()->Flip();
				}
				SetupShops(curMap);
			}
			CO_RETURN 0;
		}
	}

	CO_RETURN 1;
}

void RenderShopButton(byte on,int x,int y,int wid,const char *txt,MGLDraw *mgl)
{
	if(on)
	{
		mgl->Box(x,y,x+wid,y+19,32+31);
		mgl->FillBox(x+1,y+1,x+wid-1,y+19-1,32+8);
	}
	else
		mgl->Box(x,y,x+wid,y+19,32+16);

	PrintGlow(x+2,y+3,txt,0,2);
}

void RenderShopping(MGLDraw *mgl)
{
	mgl->FillBox(320-shopSize,240-shopSize/2,320+shopSize,240+shopSize/2,32*1+4);

	if(shopSize>2)
		mgl->Box(320-shopSize+1,240-shopSize/2+1,320+shopSize-1,240+shopSize/2-1,32*1+16);

	switch(buyMode){
		case 0:
		default:
			if(shopSize>=150)
			{
				PrintGlowRect(320-shopSize+4,240-shopSize/2+4,320+shopSize-4,240+shopSize/2-4,18,shopTxt,2);

				RenderShopButton((cursor==0),320-shopSize+10,240+shopSize/2-27,95,"No Thanks!",mgl);
				RenderShopButton((cursor==1),320+shopSize-105,240+shopSize/2-27,95,"Yes Please!",mgl);
			}
			break;
		case 1:
			if(shopSize>=150)
			{
				PrintGlowRect(320-shopSize+4,240-shopSize/2+4,320+shopSize-4,240+shopSize/2-4,18,shopTxt,2);
				RenderShopButton(1,320-48,240+shopSize/2-27,95,"Thank you!",mgl);
			}
			break;
		case 2: // roll the dice?
			if (shopSize >= 150)
			{
				PrintGlowRect(320 - shopSize + 4, 240 - shopSize / 2 + 4, 320 + shopSize - 4, 240 + shopSize / 2 - 4, 18, shopTxt, 2);

				RenderShopButton((cursor == 0), 320 - shopSize + 10, 240 + shopSize / 2 - 27, 95, "Maybe later", mgl);
				RenderShopButton((cursor == 1), 320 + shopSize - 105, 240 + shopSize / 2 - 27, 95, "Sure thing!", mgl);
			}
			break;
		case 3: // not enough loonykeys
			if(shopSize>=150)
			{
				PrintGlowRect(320-shopSize+4,240-shopSize/2+4,320+shopSize-4,240+shopSize/2-4,18,shopTxt,2);
				RenderShopButton(1,320-48,240+shopSize/2-27,95,"Okay",mgl);
			}
			break;
	}
}
