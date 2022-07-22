#ifndef BULLET_H
#define BULLET_H

#include "jamulspr.h"
#include "map.h"
#include "world.h"
#include "display.h"
#include "cossin.h"
#include "sound.h"
#include "particle.h"

#define BLT_NONE	0
#define BLT_HAMMER  1
#define BLT_HAMMER2 2		// this is a hammer with reflection
#define BLT_MISSILE 3
#define BLT_FLAME   4
#define BLT_LASER	5
#define BLT_ACID	6
#define BLT_BOMB	7
#define BLT_BOOM	8
#define BLT_ENERGY  9
#define BLT_MEGABEAM 10		// this is a ball of energy that launches off a megabeam1
#define BLT_MEGABEAM1 11	// this is a huge laser beam (downward)
#define BLT_MEGABEAM2 12	// this is the laser hitting an object (just a visual effect)
#define BLT_FLAME2	13		// just like flame, except it is anti-Bouapha
#define BLT_SPORE	14
#define BLT_SHROOM  15
#define BLT_GRENADE 16		// energy grenade, an enemy weapon
#define BLT_YELBOOM 17		// yellow explosion made by energy grenade
#define BLT_SHOCKWAVE 18	// purple shockwave, for Super Zombie stomp
#define BLT_LILBOOM 19		// explosion made by missile
#define BLT_SNOWBALL 20
#define BLT_BIGSNOW  21
#define BLT_ICESPIKE 22		// spike juts out of the ground
#define BLT_ROCK	 23
#define BLT_SPINE	 24		// cactus spine
#define BLT_EVILHAMMER 25	// a grey hammer that is anti-bouapha
#define BLT_BIGSHELL 26		// Bouapha's power armor shoots these
#define BLT_BIGAXE	 27		// Bouapha weapon
#define BLT_LIGHTNING 28	// Bouapha weapon
#define BLT_SPEAR	 29		// Bouapha's version of the pygmy spear
#define BLT_SLASH	 30		// Bouapha's machete slash
#define BLT_MINE	 31		// Bouapha's mines
#define BLT_BADSPEAR 32		// pygmy-thrown spear
#define BLT_ORBITER  33		// guy that flies around Bouapha shooting
#define BLT_GREEN	 34		// friendly green bullets
#define BLT_BALLLIGHTNING 35
#define BLT_LIGHTNING2 36
#define BLT_MINDWIPE	37
#define BLT_REFLECT	 38
#define BLT_SWAP	 39
#define BLT_SHARK	 40
#define BLT_ORBITER2 41		// orbit bomber
#define BLT_HARPOON	 42		// a spear, underwater - only difference is it sinks much more slowly
#define BLT_SCANNER	 43		// what the scanner scans with
#define BLT_SCANSHOT 44		// the shots the scanner fires after scanning
#define BLT_TORPEDO	 45		// Bouapha's minisub shoots them
#define BLT_DIRTSPIKE 46	// ice spike in brown
#define BLT_PAPER	47
#define BLT_SCANLOCK 48		// the scanner locked onto a guy
#define BLT_BUBBLE	49
#define BLT_FREEZE	50
#define BLT_BUBBLEPOP 51	// a bubble that is popping, just visual effect
#define BLT_LILBOOM2 52		// a harmless lilboom
#define BLT_CHEESEHAMMER 53	// enhanced hammers
#define BLT_FREEZE2	54		// a freeze bullet that drops like acid bullets and splats
#define BLT_LUNA	55		// lunachick's bullets
#define BLT_LUNA2	56		// lunachick's bullets with wall-bounce power

// Kid Mystic
#define BLT_FIREBALL	57	// kid mystic's fireball
#define BLT_FIREBALL2	58	// kid mystic's fireball (bouncy edition)
#define BLT_MAGICMISSILE 59	// kid mystic's missiles
#define BLT_YELWAVE		60	// wavey thing, used by eye guys
#define BLT_BIGYELLOW	61	// big yellow bullet (same as grenade, but not lobbed or explosive)
#define BLT_COIN		62	// oh goody, a coin.
#define BLT_ICECLOUD	63	// ???
#define BLT_MINIFBALL	64	// ???
#define BLT_BIGCOIN		65	// oh goody, a BIG coin.
#define BLT_LIQUIFY		66	// a "mega-flame"
#define BLT_GOODSHOCK	67	// player shockwave.
#define BLT_SKULL		68	// player shockwave.
#define BLT_ICEBEAM		69
#define BLT_ICEBEAM2	70	// Dud part
#define BLT_DEATHBEAM	71
#define BLT_DEATHBEAM2	72	// Dud part
#define BLT_SLIME		73	// slug slime
#define BLT_COMET		74	// Armageddon drops these
#define BLT_COMETBOOM	75	// and they explode like this

// Sleepless Hollow
#define BLT_SITFLAME	76	// sits around for a while
#define BLT_FLAME3		77	// floaty flame, drifts up
#define BLT_IGNITE		78	// flamebringer shot that ignites the target hit
#define BLT_HOLESHOT	79	// black hole shot flying
#define BLT_BLACKHOLE	80	// black hole shot existing
#define BLT_BADGREEN	81
#define BLT_BADSITFLAME	82
#define BLT_LASERBEAM	83

// Loonyland
#define BLT_ENERGY2		84	// Redbone bullet
#define BLT_FLAMEWALL	85	// Flame wall
#define BLT_EARTHSPIKE	86	// more advanced than the regular
#define BLT_GASBLAST	87	// Green gas blast
#define BLT_MEGABOOM	88	// Giant explosion
#define BLT_WHOOPEE		89	// Whoopee cushion (yellow gas)
#define BLT_CACTUS		90	// Potted cactus
#define BLT_BOOMERANG	91	// Player boomerang
#define BLT_ICEWAND		92	// Ice wand blast
#define BLT_BIGBOMB		93	// Loonyland bomb
#define BLT_BIGBOOM		94	// Big bomb blast
#define BLT_HOTPANTS	95	// Big bomb blast
#define BLT_SWAMPGAS	96	// swamp gas
#define BLT_ORBGRENADE	97	// Frankenjulie grenade
#define BLT_ROCKET		98	// non-guided rocket
#define BLT_HEATSEEKER	99	// guided rocket
#define BLT_ROCKETSMOKE	100	// guided rocket
#define BLT_ICEWAND2	101 //  shoots off of ice bolts
#define BLT_BATSHOT		102 // 
#define BLT_WIND		103
#define BLT_EARTHSPIKE2	104
#define BLT_ICESHARD	105
#define BLT_WATER		106
#define BLT_FOUNTAIN	107
#define BLT_FLAMESHOT	108
#define BLT_SWAMPGAS2	109
#define BLT_EVILFACE	110
#define BLT_BIGSHOT		111
#define BLT_WOLFSHOT	112
#define BLT_WOLFSHOCK	113
#define BLT_CLAW		114
#define BLT_CROAKERGAS	115
#define BLT_HOTDOGFIRE	116
#define BLT_ICEWOLFICE	117
#define BLT_FARLEYGAS	118

// Operation SCARE
#define BLT_BADLUNA			119
#define BLT_BADFBALL		120
#define BLT_BADLIGHTNING	121
#define BLT_BADBOOMERANG	122
#define BLT_BADLUNA2		123
#define BLT_QUEENACID		124
#define BLT_ALIENEGG		125
#define BLT_SPOREBALL		126
#define BLT_SQUIRT			127

// the special hammer flags for different powerups - player-specific
#define HMR_REVERSE 1
#define HMR_REFLECT 2
#define HMR_HOMING	4
#define HMR_ATTRACT	8

#define BFL_GRAVITY 1
#define BFL_HOMING 	2
#define BFL_WAVY	4

#define CHT_ULTRA 	1
#define CHT_JESUS 	2
#define CHT_SHIELD	4
#define CHT_SPEED	8
#define CHT_OXYGEN	16
#define CHT_NOSKID	32
#define CHT_LIGHT	64

typedef struct bullet_t
{
	int x,y,z;
	int dx,dy,dz; // destination x/y/z
	int timer; // how many frames left
	word target;
	byte anim;
	byte facing;
	byte type;
	char bright;
	byte friendly;
	byte flags;
	byte fromColor,toColor;
} bullet_t;

void InitBullets(void);
void ExitBullets(void);

void UpdateBullets(Map *map,world_t *world);
void RenderBullets(void);
void RenderSmoke(int x,int y,int z,char bright,byte frm);
void RenderBoom(int x,int y,int z,char bright,byte frm);
void RenderBubble(int x,int y,int z,char bright,byte frm);
void RenderMindControl(int x,int y,int z,char bright,byte frm);
void RenderStinky(int x,int y,int z,char bright,byte frm);

void QuadMissile(int x,int y,byte facing,byte friendly);
void LaunchMegabeam(int x,int y,word owner);
void FireBullet(int x,int y,byte facing,byte type,byte friendly);
void FireBulletAfter(int x,int y,byte facing,byte type,bullet_t *thisone,byte friendly);
void FireExactBullet(int x,int y,int z,int dx,int dy,int dz,byte anim,byte timer,byte facing,byte type,byte friendly);
void HammerLaunch(int x,int y,byte facing,byte count,byte flags);
byte Bulletable(Map *map,int x,int y);
void BulletRanOut(bullet_t *me,Map *map,world_t *world);	// prototyped for interior use
void ReflectShot(void);
void BulletSwap(int sx,int sy,int width,int height,int dx,int dy);
void SpitAcid(int x,int y,byte facing,byte type,byte friendly);
void FireBulletZ(int x,int y,int z,byte facing,byte type,byte friendly);
void HappyLaunch(int x,int y,byte facing,byte count,byte flags);
void ShroomSpew(int x,int y,byte facing,byte count,byte flags);
void ReflectBullets(int x,int y,byte size,byte friendly);
void RemoveOrbiters(int n,byte f,byte t);
void FireScanShots(Guy *victim);
void MakeRadar(int rx,int ry,byte w);
byte GetBulletAttackType(void);
byte HasGravity(bullet_t* me);
int GetGravityAmt(bullet_t* me);

void FireBullet(int x, int y, byte facing, byte type, byte friendly, word target);
bullet_t *GetFireBullet(int x, int y, byte facing, byte type, byte friendly);
void RecolorBullet(bullet_t *me, byte from, byte to);
void SetBulletVars(bullet_t* me, int dx, int dy, int dz, int z, int timer, byte type);

int CountBullets(byte type); // For specials to check orbiters
int CountBulletsInRect(byte type,int x,int y,int x2,int y2); // for specials to check for bullets
void ChangeBullet(byte fx,int x,int y,int type,int newtype,int friendly); // for specials to change bullets
void ChangeBulletColor(byte fx,int x,int y,int type,int colCode); // for specials to change bullet color
byte BulletFacingType(byte type); // To check if a type of bullet uses none, 0-7, or 0-255 facings
void RenderMysticalHammerItem(byte col1, byte col2, char brt, int x, int y);
void RenderSimpleAnimatedItem(byte col1, byte col2, char brt, int x, int y, int z, int a, int b, int c);
void EraseLasers(int mapx, int mapy);
void BurnHay(int x, int y);

void GetBulletDeltas(int *bdx,int *bdy);

void SaveBullets(FILE* f);
void LoadBullets(FILE* f);

#endif
