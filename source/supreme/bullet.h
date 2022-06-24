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

// Sleepless
#define BLT_SITFLAME	76	// sits around for a while
#define BLT_FLAME3		77	// floaty flame, drifts up
#define BLT_IGNITE		78	// flamebringer shot that ignites the target hit
#define BLT_HOLESHOT	79	// black hole shot flying
#define BLT_BLACKHOLE	80	// black hole shot existing
#define BLT_BADGREEN	81
#define BLT_BADSITFLAME	82
#define BLT_LASERBEAM	83

// Operation SCARE
#define BLT_BADLUNA			84
#define BLT_BADFBALL		85
#define BLT_BADLIGHTNING	86

// the special hammer flags for different powerups
#define HMR_REVERSE 1
#define HMR_REFLECT 2
#define HMR_WATERWALK 4		// not really a hammer powerup, it's a cheat code, but this is a good spot for it
#define HMR_SHIELD	8		// also cheat code
#define HMR_SPEED	16		// another cheat
#define HMR_OXYGEN	32		// another cheat
#define HMR_NOSKID	64		// another cheat
#define HMR_LIGHT	128		// another cheat

typedef struct bullet_t
{
	int x,y,z;
	int dx,dy,dz;
	int timer;
	word target;
	byte anim;
	byte facing;
	byte type;
	char bright;
	byte friendly;
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

int CountBullets(byte type); // For specials to check orbiters
int CountBulletsInRect(byte type,int x,int y,int x2,int y2); // for specials to check for bullets
void ChangeBullet(byte fx,int x,int y,int type,int newtype); // for specials to change bullets
byte BulletFacingType(byte type); // To check if a type of bullet uses none, 0-7, or 0-255 facings
void RenderMysticalHammerItem(byte col, int x, int y);
void EraseLasers(int mapx, int mapy);
void BurnHay(int x, int y);

void GetBulletDeltas(int *bdx,int *bdy);

void SaveBullets(FILE* f);
void LoadBullets(FILE* f);

#endif
