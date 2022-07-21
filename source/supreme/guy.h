#ifndef GUY_H
#define GUY_H

#include "map.h"
#include "monster.h"
#include "jamulspr.h"
#include "display.h"
#include "pathfinding.h"
#include <vector>

#define ACTION_IDLE	0
#define ACTION_BUSY 1

class Guy
{
	public:
		Guy(void);
		~Guy(void);

		void Update(Map *map,world_t *world);
		void EditorUpdate(Map *map);
		void Render(byte light);
		byte CanWalk(int xx,int yy,Map *map,world_t *world);
		void NextFrame(void);
		void SeqFinished(void);
		void MonsterControl(Map *map,world_t *world);
		byte CoconutBonk(int xx,int yy,Guy *him);
		byte AttackCheck(byte size,int xx,int yy,Guy *him);
		byte AttackCheck2(int xx,int yy,int xx2,int yy2,Guy *him);
		void AttackThem(void);
		void GetShot(int dx,int dy,byte damage,Map *map,world_t *world);
		void CalculateRect(void);
		byte IsAwake(void);

		int x,y,z;
		int oldx,oldy;
		int dx,dy,dz;
		int ax,ay; // acceleration provided by badguys
		byte mapx,mapy;
		byte lastBumpX,lastBumpY;
		byte facing;

		byte mind;		// brain variables for AI
		byte mind1;
		byte mind2;
		byte mind3;
		byte mind4;
		byte mind5;

		byte reload;
		byte poison;

		byte ouch;
		byte action;
		word frmTimer;
		word frmAdvance;
		byte frm;
		byte seq;
		char bright;
		byte friendly;

		word mindControl;
		Guy *target;
		Guy *parent;
		int hp,maxHP;
		int type;
		int rectx,recty,rectx2,recty2;	// for collision checks
		word ID;	// just a copy of the guy's number
		byte item;	// what item you're carrying
		byte frozen;

		//NEW STUFF
		byte ignited;	// burn damage (red)
		byte weak;		// weakness affliction - more damage taken (dark red)
		byte strong;	// steelskin defense - increased resistance (dark grey)
		byte confuse;	// confusion - inverted movement controls (light pink)
		byte garlic;	// now applicable to more enemies - kills vampires quick!
		byte speedy;	// now applicable to more enemies!
		byte specialFlags;	// special flags (rainbow, white flash)

		int aiType;
		byte fromColor,toColor;
		char brtChange;
		char name[32];

		sprite_set_t* customSpr; // custom sprites
		//std::vector<node_t> nodes;
};

#define GSF_NEWAI		1	// New AI - avoids pre-mod conflict
#define GSF_LOONY		2	// Rainbow - like a loonykey!
#define GSF_FASTFWD		4	// 2x speed (stacks with speedy - BEWARE!)
#define NUM_MONSFLAGS	3

#define GEF_POISON	0
#define GEF_FROZEN	1
#define GEF_BURN	2
#define GEF_WEAK	3
#define GEF_STRONG	4
#define GEF_CONFUSE	5
#define GEF_GARLIC	6
#define GEF_SPEEDY	7
#define NUM_CONDITIONS	8

extern Guy *goodguy;

void InitGuys(int max);
void ExitGuys(void);
void UpdateGuys(Map *map,world_t *world);
void EditorUpdateGuys(Map *map);
void RenderGuys(byte light);
Guy *AddGuy(int x,int y,int z,int type,byte friendly);
Guy *AddBaby(int x,int y,int z,int type,Guy *me);
Guy *GetGuy(word w);
void DeleteGuy(int x,int y,int type);
void AddMapGuys(Map *map);
byte FindVictim(int x,int y,byte size,int dx,int dy,byte damage,Map *map,world_t *world,byte friendly);
byte FindVictims(int x,int y,byte size,int dx,int dy,byte damage,Map *map,world_t *world,byte friendly,byte slow);
byte FindNewVictim(int x,int y,word* target,byte size,int dx,int dy,byte damage,Map *map,world_t *world,byte friendly);
word LockOnEvil(int x,int y);
word LockOnEvil2(int x,int y);
word LockOnGood(int x,int y);
word LockOnGood2(int x,int y);
word LockOn2(int x,int y,byte friendly);
word LockOn3(int x,int y,int maxRange, byte friendly);
byte GetGuyPos(word guy,int *x,int *y);
byte MonsterExists(int type);
void HealGoodguy(byte amt);
byte MossCheck(int x,int y);
void KillKids(Guy *g);
byte RaftNearby(void);
void GuySwap(int sx,int sy,int width,int height,int dx,int dy);
void ShiftGuys(char dx,char dy,Map *map);
void AddPygmy(Map *map,world_t *world,byte friendly);
void AddNinja(Map *map,world_t *world,byte friendly);
Guy *GetLastGuyHit(void);
byte ControlMind(Guy *me);
void KillMonster(int x,int y,int type,byte nofx);
void MonsterLife(byte fx,int x,int y,int type,int newLife);
void MonsterMaxLife(byte fx,int x,int y,int type,int newLife);
byte CheckMonsterLife(int x,int y,int type,int life,byte flags);
byte SwapMe(int x,int y,byte size,Map *map);
int CountMonsters(int type);
int CountMonstersInRect(int type,int x,int y,int x2,int y2);
Guy *GetGuyOfType(int type);
void RemoveGuy(Guy *g);
void Telefrag(Guy *g);
byte TryToPushItem(int x,int y,int destx,int desty,Map *map,world_t *world);
void ChangeMonster(byte fx,int x,int y,int type,int newtype);
void ChangeMonsterAI(byte fx,int x,int y,int type,int newtype);
void ChangeTeam(byte fx,int x,int y,int type,byte team);
void ChangeMonsItem(byte fx,int x,int y,int type,int newItem);
byte CheckMonsterAwake(int x,int y,int type,byte flags);
Guy *FindMonster(int x,int y,int type);
byte BadguyRegions(int x,int y,int x2,int y2,int tx,int ty);
void MonsterLifeAmt(byte fx,int x,int y,int type,int amt);
void SetMonsterName(byte fx,int x,int y,int type,char *name);
void SetMonsterGraphics(byte fx,int x,int y,int type,char *name);
void SetMonsterColor(byte fx,int x,int y,int type,int colCode);
void SetMonsterBright(byte fx,int x,int y,int type,int bright);
void SetMonsterTempCondition(byte fx,int x,int y,int type,int cond, int amt);
void SetMonsterPermCondition(byte fx,int x,int y,int type,int cond);
void SetMonsterSpecialFlags(byte fx,int x,int y,int type,int mflags);
byte CheckMonsterColor(int x,int y,int type,byte color);
byte CheckMonsterTempCondition(int x,int y,int type,byte flags, byte trg, int amt);
byte CheckMonsterPermCondition(int x,int y,int type,byte trg);
Guy *GetGuyOfAIType(int type);

byte DyingBaddies(Guy *g,int group);
void RaiseBaddies(Guy *g,int group);
void KillAllMonsters(byte type);
void ResetGuy(Guy *g);

// Camera stuff
byte CheckTopLeftCameraPoint(int x, int y);
byte CheckCameraRect(int x, int y,int v);

// Kid Mystic stuff
byte PeepAtKid(int x,int y,Map *map,byte face);
byte LogNearby(void);

void WhackazoidUpdate(byte init);
void WhackedAZoid(Map* map);
void WhackazoidDisplay(void);

// SH stuff
byte ArrangeBats(byte facing);
byte SmashTrees(Guy* me, byte rad);
byte EatHay(Guy* me, byte rad);
void SuckInEvil(int x, int y, byte friendly);
void SuckInGood(int x, int y, byte friendly);
void PushOutEvil(int x, int y, byte friendly);
void Tornado(int x, int y, byte friendly);
void HealOthers(int x, int y, int f, int h);
void PushOthers(Guy* g, byte h, Map* map);
byte Walkable(Guy *me, int x, int y, Map* map, world_t* world);
void SpreadCharge(Guy* me);
void GetSpook(void);
void PutSpook(void);
byte HealNearbyAllies(Guy *me,int x,int y,byte size,byte amt,Map *map,world_t *world,byte friendly,byte canBeSelf);
byte BuffNearbyAllies(Guy *me,int x,int y,byte size,byte effect,byte amt,Map *map,world_t *world,byte friendly,byte canBeSelf);

// LL stuff
void LaunchJunk(Map* map, Guy* me);
void Sneeze(Guy *me);

// Helpful new functions
byte Inflict(Guy* me,byte effect,byte amt);
byte IsZombie(Guy *g);
byte IsSuperZombie(Guy *g);
byte FocusOnGuy(Guy *g, Map *map);
byte GetBaseColor(Guy *g);
byte AngerYetis(Guy* me);
byte InflictVictims(int x,int y,byte size,int dx,int dy,byte type,byte amt,Map *map,world_t *world,byte friendly,byte slow);
byte FreezeGuy2(Guy *me,byte amt);

void FindMonsterBrain(int myx,int myy);
void FindMonsterCandle(int myx,int myy);
#endif
