#include "winpch.h"
#include "monster.h"
#include "player.h"
#include "progress.h"
#include "shop.h"
#include "editor.h"
#include "goal.h"
#include "water.h"
#include "pathfinding.h"

/*
 -MT_GOOD	 -MT_EVIL		 -MT_SPOOKY		-MT_ZOMBIE	 -MT_VAMPIRE	-MT_SPIDER -MT_PYGMY
 -MT_BOSS	 -MT_MINIBOSS	 -MT_WACKY		-MT_PUMPKIN	 -MT_THINGY		-MT_VEGGIE -MT_ARCTIC
 -MT_ZOID	 -MT_DESERT		 -MT_CRAZIES	-MT_VEHICLE	 -MT_GENERATE	-MT_TRAP   -MT_ALIEN
 -MT_ANIMAL	 -MT_HUMAN		 -MT_URBAN		-MT_AQUATIC	 -MT_UNDERSEA
 -MT_FLYING  -MT_HIGHTECH	 -MT_BITS
*/
extern monsterType_t monsType[NUM_MONSTERS]; // see monsterlist.cpp

void InitMonsters(void)
{
	int i,j,k;
	byte done;

	for(i=0;i<NUM_MONSTERS;i++)
	{
		monsType[i].spr=NULL;
		for(j=0;j<NUM_ANIMS;j++)
		{
			done=0;
			for(k=0;k<ANIM_LENGTH;k++)
			{
				if(monsType[i].anim[j][k]==255)
				{
					done=1;
				}
				if(done)
					monsType[i].anim[j][k]=255;	// fill the unused space in animations with 255's
			}
			monsType[i].anim[j][ANIM_LENGTH-1]=255;	// and set the last one to 255 just in case
		}
	}
	// just keep bouapha perma-loaded
	monsType[MONS_BOUAPHA].spr=new sprite_set_t(monsType[MONS_BOUAPHA].sprName);
}

void ExitMonsters(void)
{
	int i;

	for(i=1;i<NUM_MONSTERS;i++)
	{
		if(monsType[i].sprName[0]=='!')
			monsType[i].spr=NULL;
		else if(monsType[i].spr)
			delete monsType[i].spr;
		monsType[i].spr=NULL;
	}
}

void ChangeOffColor(dword type,byte from,byte to)
{
	monsType[type].fromCol=from;
	monsType[type].toCol=to;
}

monsterType_t *GetMonsterType(dword type)
{
	return &monsType[type];
}

void PurgeMonsterSprites(void)
{
	int i;

	// note this starts at 2, skipping bouapha
	for(i=2;i<NUM_MONSTERS;i++)
	{
		// repeat graphics monsters do not delete their sprites
		if(monsType[i].sprName[0]=='!')
			monsType[i].spr=NULL;
		else if(monsType[i].spr)
			delete monsType[i].spr;
		monsType[i].spr=NULL;
	}
}

byte MonsterSize(dword type)
{
	return monsType[type].size;
}

byte *MonsterAnim(dword type,byte anim)
{
	return monsType[type].anim[anim];
}

word MonsterFlags(dword type,byte aiType)
{
	if(aiType==MONS_BOUAPHA && player.wpns[player.curSlot].wpn==WPN_PWRARMOR)
		return monsType[MONS_PWRBOUAPHA].flags;
	if(aiType==MONS_BOUAPHA && player.wpns[player.curSlot].wpn==WPN_MINISUB)
		return monsType[MONS_MINISUB].flags;
	return monsType[type].flags;
}

byte MonsterFrames(dword type,byte aiType)
{
	if(aiType==MONS_BOUAPHA && player.wpns[player.curSlot].wpn==WPN_PWRARMOR)
		return monsType[MONS_PWRBOUAPHA].framesPerDir;
	if(aiType==MONS_BOUAPHA && player.wpns[player.curSlot].wpn==WPN_MINISUB)
		return monsType[MONS_MINISUB].framesPerDir;
	return monsType[type].framesPerDir;
}

word MonsterPoints(dword type)
{
	return monsType[type].points;
}

word MonsterHP(dword type)
{
	return monsType[type].hp;
}

char *MonsterName(short type)
{
	static char tmp[16];

	if(type==MONS_ANYBODY)
		strcpy(tmp,"Anybody");
	else if(type==MONS_GOODGUY)
		strcpy(tmp,"Goodguy");
	else if(type==MONS_BADGUY)
		strcpy(tmp,"Badguy");
	else if(type==MONS_NONPLAYER)
		strcpy(tmp,"Non-player");
	else if(type==MONS_PLAYER)
		strcpy(tmp,"Player");
	else if(type==MONS_TAGGED)
		strcpy(tmp,"Tagged");
	else
		return monsType[type].name;

	return tmp;
}

void SetMonsterFlags(dword type,word flags)
{
	monsType[type].flags=flags;
}

void LoadMySprite(dword type)
{
	int v;

	if(type==0 || type>=NUM_MONSTERS)
		return;

	if(monsType[type].spr==NULL)
	{
		if(monsType[type].sprName[0]=='!')
		{
			// it's a repeat of someone else's sprite
			v=atoi(&monsType[type].sprName[1]);
			if(!monsType[v].spr)
				monsType[v].spr=new sprite_set_t(monsType[v].sprName);

			monsType[type].spr=monsType[v].spr;
		}
		else
			monsType[type].spr=new sprite_set_t(monsType[type].sprName);

		if(monsType[type].spr==NULL || monsType[type].spr->GetSprite(0)==NULL)
			FatalError("Out of memory or sprites missing!");
	}
}

sprite_t *GetMonsterSprite(dword type,byte seq,byte frm,byte facing)
{
	int v;

	if(type==MONS_BOUAPHA)
	{
		if(player.wpns[player.curSlot].wpn==WPN_PWRARMOR)
			type=MONS_PWRBOUAPHA;
		else if(player.wpns[player.curSlot].wpn==WPN_MINISUB)
			type=MONS_PWRBOUAPHA;
		else if(type==MONS_BOUAPHA)
		{
			if(player.playAs==PLAY_LUNATIC)
				type=MONS_DRL;
			else if(player.playAs==PLAY_HAPPY)
				type=MONS_STICKMAN;
			else if(player.playAs==PLAY_MECHA)
				type=MONS_PLAYMECHA;
			else if(player.playAs==PLAY_SHROOM)
				type=MONS_PLAYSHROOM;
			else if(player.playAs==PLAY_LUNACHIK)
				type=MONS_LUNACHICK;
			else if (player.playAs==PLAY_MYSTIC)
				type=MONS_MYSTIC;
		}
	}

	// load if not loaded
	LoadMySprite(type);

	v=monsType[type].anim[seq][frm];

	if(v==254)
		return NULL;	// 254 means no sprite for this frame

	if(!(monsType[type].flags&MF_ONEFACE))
		v+=facing*monsType[type].framesPerDir;

	if(type==MONS_BOUAPHA)
	{
		if(PlayerHasHammer())
			v+=8*monsType[type].framesPerDir;
	}
	if(type==MONS_EVILCLONE)
		v+=8*monsType[type].framesPerDir;

	if(monsType[type].flags&MF_FACECMD)
		v+=facing;

	return monsType[type].spr->GetSprite(v);
}


const byte wacky[] = { 4,5,3 };
void MonsterDraw(int x,int y,int z,dword type,dword aiType,byte seq,byte frm,byte facing,char bright,Guy *g,sprite_set_t* set)
{
	sprite_t *curSpr;
	int v;
	byte shld,isBouapha,b;
	word flgs;

	if(aiType==MONS_BOUAPHA)
	{
		if(player.wpns[player.curSlot].wpn==WPN_PWRARMOR)
			type=MONS_PWRBOUAPHA;
		else if(player.wpns[player.curSlot].wpn==WPN_MINISUB)
			type=MONS_MINISUB;
		else if(type==MONS_BOUAPHA)
		{
			if(player.playAs==PLAY_LUNATIC)
				type=MONS_DRL;
			else if(player.playAs==PLAY_HAPPY)
				type=MONS_STICKMAN;
			else if(player.playAs==PLAY_MECHA)
				type=MONS_PLAYMECHA;
			else if(player.playAs==PLAY_SHROOM)
				type=MONS_PLAYSHROOM;
			else if(player.playAs==PLAY_LUNACHIK)
				type=MONS_LUNACHICK;
			else if(player.playAs==PLAY_MYSTIC)
				type=MONS_MYSTIC;
		}

		isBouapha=1;
	}
	else
		isBouapha=0;

	// load if not loaded
	LoadMySprite(type);

	if (set == NULL)
		set = monsType[type].spr;

	v=monsType[type].anim[seq][frm];

	if(v==254 || v==255)
		return;	// don't draw this frame

	if(!(monsType[type].flags&MF_ONEFACE))
		v+=facing*monsType[type].framesPerDir;

	if(isBouapha)
	{
		if(type==MONS_BOUAPHA && PlayerHasHammer())
			v+=8*monsType[type].framesPerDir;
		if(g->frozen)
		{
			curSpr=set->GetSprite(v);
			if(!curSpr)
				return;
			if(!(monsType[type].flags&MF_NOSHADOW))
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);
			if(g->ouch==0)
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,7,bright+4,curSpr,DISPLAY_DRAWME);	// aqua
			else
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,6,bright+8,curSpr,DISPLAY_DRAWME); // purple
			return;
		}
		else if(g->ignited)
		{
			curSpr=set->GetSprite(v);
			if(!curSpr)
				return;
			if(!(monsType[type].flags&MF_NOSHADOW))
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);
			if(g->ouch==0)
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright+12,curSpr,DISPLAY_DRAWME);	// light red
			else
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright+4,curSpr,DISPLAY_DRAWME); // red
			return;
		}
		else if(g->poison)
		{
			curSpr=set->GetSprite(v);
			if(!curSpr)
				return;
			if(!(monsType[type].flags&MF_NOSHADOW))
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);
			if(g->ouch==0)
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,1,bright-4,curSpr,DISPLAY_DRAWME);	// green
			else
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,5,bright,curSpr,DISPLAY_DRAWME); // yellow
			return;
		}
		else if(g->confuse)
		{
			curSpr=set->GetSprite(v);
			if(!curSpr)
				return;
			if(!(monsType[type].flags&MF_NOSHADOW))
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);
			if(g->ouch==0)
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,6,bright+8,curSpr,DISPLAY_DRAWME);	// purple
			else
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright+16,curSpr,DISPLAY_DRAWME); // red
			return;
		}
		else if(g->strong)
		{
			curSpr=set->GetSprite(v);
			if(!curSpr)
				return;
			if(!(monsType[type].flags&MF_NOSHADOW))
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);
			if(g->ouch==0)
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,0,bright-4,curSpr,DISPLAY_DRAWME);	// dark grey
			else
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,floor(Random(7)),bright+4,curSpr,DISPLAY_DRAWME); // rainbow
			return;
		}
		else if(g->weak)
		{
			curSpr=set->GetSprite(v);
			if(!curSpr)
				return;
			if(!(monsType[type].flags&MF_NOSHADOW))
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);
			if(g->ouch==0)
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright-4,curSpr,DISPLAY_DRAWME);	// dark red
			else
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,2,bright-4,curSpr,DISPLAY_DRAWME); // brown
			return;
		}
		else if(g->invis)
		{
			curSpr=set->GetSprite(v);
			if(!curSpr)
				return;
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,255,bright,curSpr,DISPLAY_DRAWME|DISPLAY_GLOW);
			return;
		}
		else if(g->specialFlags&GSF_LOONY)
		{
			b=abs(16-(player.clock&31));
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,player.clock/32,bright+b,curSpr,DISPLAY_DRAWME);
		}
	}

	if(type==MONS_EVILCLONE)
		v+=8*monsType[type].framesPerDir;

	if(monsType[type].flags&MF_FACECMD)
		v+=facing;

	curSpr=set->GetSprite(v);
	if(!curSpr)
		return;

	if(!(g->invis && !g->friendly)&&!(monsType[type].flags&MF_NOSHADOW))
		SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);

	if(g->ouch==0)
	{
		if(g->frozen)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,7,bright+4,curSpr,DISPLAY_DRAWME);
		else if(g->ignited)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright+16,curSpr,DISPLAY_DRAWME);
		else if(g->poison)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,1,bright,curSpr,DISPLAY_DRAWME);
		else if (g->invis) {
			if (g->friendly)
				SprDraw(x >> FIXSHIFT, y >> FIXSHIFT, z >> FIXSHIFT, 255, bright, curSpr, DISPLAY_DRAWME | DISPLAY_GLOW);
		}
		else if(g->confuse)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,6,bright+4,curSpr,DISPLAY_DRAWME);
		else if (g->strong)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,0,bright-4,curSpr,DISPLAY_DRAWME);
		else if (g->weak)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,2,bright-4,curSpr,DISPLAY_DRAWME);
		else if(g->specialFlags&GSF_LOONY) {// Imitated loony color effect - switches between all eight colors
			b=abs(16-(player.clock&31));
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,player.clock/32,bright+monsType[type].brtChg+b,curSpr,DISPLAY_DRAWME);
		}
		else if(!(monsType[type].flags&(MF_GHOST|MF_GLOW)))
		{
			if(monsType[type].fromCol==255)
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,255,bright+monsType[type].brtChg,curSpr,DISPLAY_DRAWME);
			else
			{
				switch (type)
				{
					case MONS_WACKYPUMP:
						SprDrawOff(x >> FIXSHIFT, y >> FIXSHIFT, z >> FIXSHIFT, monsType[type].fromCol, wacky[(g->mind2/10)%3],
							bright + monsType[type].brtChg, curSpr, DISPLAY_DRAWME);
						break;
					default:
						SprDrawOff(x >> FIXSHIFT, y >> FIXSHIFT, z >> FIXSHIFT, monsType[type].fromCol, monsType[type].toCol,
							bright + monsType[type].brtChg, curSpr, DISPLAY_DRAWME);
						break;
				}
			}
		}
		else if(monsType[type].flags&MF_GHOST)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,255,bright+monsType[type].brtChg,curSpr,DISPLAY_DRAWME|DISPLAY_GHOST);
		else if(monsType[type].flags&MF_GLOW)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,255,bright+monsType[type].brtChg,curSpr,DISPLAY_DRAWME|DISPLAY_GLOW);
	}
	else
	{
		if(g->frozen)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,6,bright+8,curSpr,DISPLAY_DRAWME);
		else if(g->ignited)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright+4,curSpr,DISPLAY_DRAWME);
		else if(g->poison)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,5,bright,curSpr,DISPLAY_DRAWME);
		else if(g->confuse)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright+16,curSpr,DISPLAY_DRAWME);
		else if (g->strong)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,0,bright+12,curSpr,DISPLAY_DRAWME);
		else if (g->weak)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,2,bright-4,curSpr,DISPLAY_DRAWME);
		else
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright,curSpr,DISPLAY_DRAWME);
	}

	shld = g->shield;
	if ((shld < 16) && (shld & 2))	// it blinks when there is 1/2 second left
		shld = 0;
	curSpr = monsType[MONS_BOUAPHA].spr->GetSprite(464 + (shld & 7));
	if (curSpr == NULL)
		return;
	if (shld)
		SprDraw(x >> FIXSHIFT, (y >> FIXSHIFT) + 1, 1 + (z >> FIXSHIFT), 255, bright, curSpr, DISPLAY_DRAWME | DISPLAY_GLOW);
}

void InstaRenderMonster(int x,int y,dword type,char bright,MGLDraw *mgl)
{
	sprite_t *curSpr;
	int v;

	// load if not loaded
	LoadMySprite(type);

	v=monsType[type].anim[ANIM_IDLE][0];
	if(!(monsType[type].flags&MF_ONEFACE))
		v+=2*monsType[type].framesPerDir;

	curSpr=monsType[type].spr->GetSprite(v);
	if(!curSpr)
		return;

	if(monsType[type].fromCol==255)
		curSpr->DrawBright(x,y,mgl,bright+monsType[type].brtChg);
	else
		curSpr->DrawOffColor(x,y,mgl,monsType[type].fromCol,monsType[type].toCol,
				bright+monsType[type].brtChg);
}

int InstaRenderScannedMonster(int x,int y,dword type,char bright,MGLDraw *mgl)
{
	sprite_t *curSpr;
	int v;

	// load if not loaded
	LoadMySprite(type);

	v=monsType[type].anim[ANIM_IDLE][0];
	if(!(monsType[type].flags&MF_ONEFACE))
		v+=2*monsType[type].framesPerDir;

	curSpr=monsType[type].spr->GetSprite(v);
	if(!curSpr)
		return 10;

	curSpr->DrawColored(x,y,mgl,1,bright);

	return curSpr->height;
}

dword MonsterTheme(dword type)
{
	return monsType[type].theme;
}

// AI auxiliary functions to make it simple
//---------------------------------------------

inline void FaceGoodguy(Guy *me,Guy *goodguy)
{
	if(goodguy->x<me->x-FIXAMT*16)
	{
		if(goodguy->y<me->y-FIXAMT*16)
			me->facing=5;
		else if(goodguy->y>me->y+FIXAMT*16)
			me->facing=3;
		else
			me->facing=4;
	}
	else if(goodguy->x>me->x+FIXAMT*16)
	{
		if(goodguy->y<me->y-FIXAMT*16)
			me->facing=7;
		else if(goodguy->y>me->y+FIXAMT*16)
			me->facing=1;
		else
			me->facing=0;
	}
	else
	{
		if(goodguy->y<me->y-FIXAMT*16)
			me->facing=6;
		else if(goodguy->y>me->y+FIXAMT*16)
			me->facing=2;
	}
}

// Not in use, intended for node facing whenever pathfinding gets implemented...
inline void FaceNode(Guy* me, node_t* node)
{
	if (node->x < me->x - FIXAMT * 16)
	{
		if (node->y < me->y - FIXAMT * 16)
			me->facing = 5;
		else if (node->y > me->y + FIXAMT * 16)
			me->facing = 3;
		else
			me->facing = 4;
	}
	else if (goodguy->x > me->x + FIXAMT * 16)
	{
		if (node->y < me->y - FIXAMT * 16)
			me->facing = 7;
		else if (node->y > me->y + FIXAMT * 16)
			me->facing = 1;
		else
			me->facing = 0;
	}
	else
	{
		if (node->y < me->y - FIXAMT * 16)
			me->facing = 6;
		else if (node->y > me->y + FIXAMT * 16)
			me->facing = 2;
	}
}

int RangeToTarget(Guy *me,Guy *goodguy)
{
	return abs(me->x-goodguy->x)+abs(me->y-goodguy->y);
}

int RangeToPoint(Guy* me, int x, int y)
{
	return abs(me->x - x) + abs(me->y - y);
}

void BigBoom(Guy* me)
{
	int x,y;
	x=me->x>>FIXSHIFT;
	y=me->y>>FIXSHIFT;
	BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
	BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
		(y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
		me->z,FIXAMT);
}

// this version doesn't insta-face, it rotates toward the right facing, and it has much
// more leeway than the 16 pixels of the other (it's for bigger creatures)
inline void FaceGoodguy2(Guy *me,Guy *goodguy)
{
	int desired;
	int diff,dir;

	if(goodguy->x<me->x-FIXAMT*32)
	{
		if(goodguy->y<me->y-FIXAMT*32)
			desired=5;
		else if(goodguy->y>me->y+FIXAMT*32)
			desired=3;
		else
			desired=4;
	}
	else if(goodguy->x>me->x+FIXAMT*32)
	{
		if(goodguy->y<me->y-FIXAMT*32)
			desired=7;
		else if(goodguy->y>me->y+FIXAMT*32)
			desired=1;
		else
			desired=0;
	}
	else
	{
		if(goodguy->y<me->y-FIXAMT*32)
			desired=6;
		else
			desired=2;
	}

	if(desired==me->facing)
		return;

	if(desired>me->facing)
	{
		diff=desired-me->facing;
		if(diff>4)
		{
			dir=-1;
			diff=8-diff;
		}
		else
			dir=1;
	}
	else
	{
		diff=me->facing-desired;
		if(diff>4)
		{
			dir=1;
			diff=8-diff;
		}
		else
			dir=-1;
	}
	me->facing=(me->facing+dir)&7;
}

// gets to facing
inline byte GetFacing(Guy *me,Guy *goodguy, int size, byte away)
{
	byte f=0;
	if(goodguy->x<me->x-FIXAMT*size)
	{
		if(goodguy->y<me->y-FIXAMT*size)
			f=5;
		else if(goodguy->y>me->y+FIXAMT*size)
			f=3;
		else
			f=4;
	}
	else if(goodguy->x>me->x+FIXAMT*size)
	{
		if(goodguy->y<me->y-FIXAMT*size)
			f=7;
		else if(goodguy->y>me->y+FIXAMT*size)
			f=1;
		else
			f=0;
	}
	else
	{
		if(goodguy->y<me->y-FIXAMT*size)
			f=6;
		else if(goodguy->y>me->y+FIXAMT*size)
			f=2;
	}
	if(away)
		f+=4;
	return f&7;
}

inline void FaceGoodguy3(Guy *me,Guy *goodguy)
{
	int desired;
	int diff,dir;

	if(abs(me->x-goodguy->x)+abs(me->y-goodguy->y)>FIXAMT*72)
	{
		FaceGoodguy2(me,goodguy);
		return;
	}

	if(goodguy->x<me->x-FIXAMT*16)
	{
		if(goodguy->y<me->y-FIXAMT*16)
			desired=5;
		else if(goodguy->y>me->y+FIXAMT*16)
			desired=3;
		else
			desired=4;
	}
	else if(goodguy->x>me->x+FIXAMT*16)
	{
		if(goodguy->y<me->y-FIXAMT*16)
			desired=7;
		else if(goodguy->y>me->y+FIXAMT*16)
			desired=1;
		else
			desired=0;
	}
	else
	{
		if(goodguy->y<me->y-FIXAMT*16)
			desired=6;
		else
			desired=2;
	}

	if(desired==me->facing)
		return;

	if(desired>me->facing)
	{
		diff=desired-me->facing;
		if(diff>4)
		{
			dir=-1;
			diff=8-diff;
		}
		else
			dir=1;
	}
	else
	{
		diff=me->facing-desired;
		if(diff>4)
		{
			dir=1;
			diff=8-diff;
		}
		else
			dir=-1;
	}
	me->facing=(me->facing+dir)&7;
}

void UpdateColorBright(Guy *me){
	switch(me->type){
		case MONS_BOMBIE:
		case MONS_PINKPUMPKN:
			if(me->bright>7)
				me->bright=-4;
			me->bright++;
			break;
	}
}

// this is only used for The Thing's tentacles, to keep their flailing within quadrants
void FlailLock(Guy *me)
{
	byte parentangle;
	byte diff;
	char dir;

	if(!me->parent || me->parent->type==MONS_NONE)
		return;	// no good

	if(me->parent->aiType!=MONS_THINGTENT)
	{
		switch(me->mind)	// which quadrant of the tentacles is he?
		{
			case 0:	// lower left
				parentangle=6;
				break;
			case 1:	// lower right
				parentangle=2;
				break;
			case 2:	// upper right
				parentangle=14;
				break;
			case 3:	// upper left
			default:
				parentangle=10;
				break;
		}
	}
	else
	{
		parentangle=me->parent->facing;
	}

	if(parentangle>me->facing)
	{
		diff=parentangle-me->facing;
		if(diff>8)
		{
			dir=1;
			diff=16-diff;
		}
		else
			dir=-1;
	}
	else
	{
		diff=me->facing-parentangle;
		if(diff>8)
		{
			dir=-1;
			diff=16-diff;
		}
		else
			dir=1;
	}
	if(diff>2)
	{
		me->facing=(parentangle+dir*2)&15;
	}
}

void BasicAI(Guy *me,int ouchSound, int dieSound,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(ouchSound,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(dieSound,me->x,me->y,SND_CUTOFF,1200);
	}
}

void WanderAI(Guy *me,int spd,int wanderRate,int wanderTime,int unWanderRate,Map *map,world_t *world,Guy *goodguy)
{
	if(me->action==ACTION_BUSY)
		return;

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		FaceGoodguy(me,goodguy);
		me->dx=Cosine(me->facing*32)*spd;
		me->dy=Sine(me->facing*32)*spd;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(Random(wanderRate)==0)
		{
			me->mind=1;		// occasionally wander
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(unWanderRate)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=(byte)Random(wanderTime)+1;
		}

		me->dx=Cosine(me->facing*32)*spd;
		me->dy=Sine(me->facing*32)*spd;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void DoMove(Guy *me,int move,int frmAdv,byte busy,int dx,int dy)
{
	me->seq=move;
	me->frm=0;
	me->frmTimer=0;
	me->frmAdvance=frmAdv;
	if(busy)
		me->action=ACTION_BUSY;
	else
		me->action=ACTION_IDLE;
	me->dx=dx;
	me->dy=dy;
}

void DoMove(Guy *me,byte move,int dx,int dy)
{
	me->seq=move;
	me->frm=0;
	me->frmTimer=0;
	me->frmAdvance=128;
	me->action=ACTION_BUSY;
	me->dx=dx;
	me->dy=dy;
}

#define NUM_DESTS 5
void SelectDestination(Guy *me,Guy *goodguy,Map *map,byte repeat)
{
	int i,x,y;
	byte bestx[NUM_DESTS],besty[NUM_DESTS];

	for(i=0;i<NUM_DESTS;i++)
		bestx[i]=255;

	for(x=0;x<map->width;x++)
		for(y=0;y<map->height;y++)
		{
			if(map->map[x+y*map->width].light>0 && (x!=me->mind2 || y!=me->mind3 || repeat==1))
			{
				// there is a tag here... which is NOT my previous target
				if(map->CheckLOS(me->mapx,me->mapy,7,x,y))
				{
					// and it's in line of sight
					for(i=0;i<NUM_DESTS;i++)
					{
						if(bestx[i]==255)
						{
							bestx[i]=x;
							besty[i]=y;
							i=NUM_DESTS+2;
						}
					}
					if(i!=NUM_DESTS+3)
					{
						// all 3 tags are presently occupied, so let's see
						// if this is closer than one of them
						for(i=0;i<NUM_DESTS;i++)
						{
							if(abs(bestx[i]-me->mapx)+abs(besty[i]-me->mapy)>
								abs(x-me->mapx)+abs(y-me->mapy))
							{
								bestx[i]=x;
								besty[i]=y;
								i=NUM_DESTS+2;
							}
						}
					}
					// now if the tag is one of the closest 3 visible ones, then
					// it is in the best list
				}
			}
		}
	// now we have a list of up to 3 tags within sight
	// which one is closest or farthest from Loony?
	if(me->aiType==MONS_LARRY)
	{
		// want to kill - go TOWARDS loony
		x=255;
		for(i=0;i<NUM_DESTS;i++)
		{
			if(x==255 ||
				(bestx[i]!=255 && abs(bestx[i]-goodguy->mapx)+abs(besty[i]-goodguy->mapy)<
						abs(bestx[x]-goodguy->mapx)+abs(besty[x]-goodguy->mapy)))
				x=i;
		}
		if(x==255)
		{
			// no visible valid tags, so head straight for Loony!
			me->mind2=goodguy->mapx;
			me->mind3=goodguy->mapy;
		}
		else
		{
			me->mind2=bestx[x];
			me->mind3=besty[x];
		}
	}
	else
	{
		// want to run - go AWAY from loony
		x=255;
		for(i=0;i<NUM_DESTS;i++)
		{
			if(x==255 ||
				(bestx[i]!=255 && abs(bestx[i]-goodguy->mapx)+abs(besty[i]-goodguy->mapy)>
						abs(bestx[x]-goodguy->mapx)+abs(besty[x]-goodguy->mapy)))
				x=i;
		}
		if(x==255)
		{
			// no visible valid tags, so flee randomly
			me->mind2=(byte)Random(map->width);
			me->mind3=(byte)Random(map->height);
		}
		else
		{
			me->mind2=bestx[x];
			me->mind3=besty[x];
		}
	}
}

// here be the AIs for each monster type
//--------------------------------------------------------------------------------------

byte DiesForward(Guy *me){
	switch (me->type)
	{
		case MONS_SVEN:
		case MONS_BJORN:
		case MONS_OLAF:
		case MONS_SHOCKTR:
		case MONS_TROOPER2:
		case MONS_LUNAMECHA:
			return 1;
		default:
			return 0;
	}
}

// Returns true if the enemy is meant to face forward.
byte IsRemixed(Guy *me)
{
	return me->specialFlags&GSF_NEWAI;
}

byte IsOneFacing(Guy* me) {
	return (monsType[me->type].flags & MF_ONEFACE || (monsType[me->aiType].flags & MF_ONEFACE && !me->specialFlags & GSF_NEWAI));
}

// Faces the enemy forward if ONEFACE flag is used, or towards goodguy.
void OneFaceGoodguy(Guy *me, Guy *goodguy)
{
	if (IsOneFacing(me))
		me->facing = 2;
	else
		FaceGoodguy(me,goodguy);
}

byte CheckParent(Guy* me, byte check)
{
	return !me->parent ? 0 : check;
}

// Returns parent AI type, or 0 if no parent.
word GetParentAI(Guy* me)
{
	if(!me->parent)
		return 0;
	else
		return me->parent->aiType;
}

byte IsBunnyAble(mapTile_t *m,world_t *world)
{
	if(!(GetTerrain(world,m->floor)->flags&TF_BUNNY))
		return 0;

	if(GetItem(m->item)->flags&IF_SOLID)
		return 0;

	return 1;
}

byte Bounce(Guy *me,byte &chg)
{
	if(me->mind1&1)
	{
		switch (chg)
		{
		case 0:
			chg = 4;
			break;
		case 1:
			chg = 3;
			break;
		case 2:
		case 6:
			break;
		case 3:
			chg = 1;
			break;
		case 4:
			chg = 0;
			break;
		case 5:
			chg = 7;
			break;
		case 7:
			chg = 5;
			break;
		}
		me->dx=-me->dx;
	}
	if(me->mind1&2)
	{
		switch (chg)
		{
		case 0:
		case 4:
			break;
		case 1:
			chg = 7;
			break;
		case 2:
			chg = 6;
			break;
		case 3:
			chg = 5;
			break;
		case 5:
			chg = 3;
			break;
		case 6:
			chg = 2;
			break;
		case 7:
			chg = 1;
			break;
		}
		me->dy=-me->dy;
	}
	return me->mind1>0;
}

void AI_Healer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,j;
	Guy *g;

	BasicAI(me,SND_MUSHOUCH,SND_MUSHDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=8 && me->reload==0)
		{
			if(me->frm==6)
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
			i=(me->facing*32-32+me->frm)&255;
			for(j=-32+me->frm;j<33;j+=me->frm*2)
			{
				x=me->x+Cosine(i)*32;
				y=me->y+Sine(i)*24;
				FireExactBullet(x,y,FIXAMT*20,Cosine(i)*8,Sine(i)*8,0,0,16,i,BLT_GOODSPORE,me->friendly);
				i=(i+me->frm*2)&255;
			}
			me->reload=1;
		}
		if(me->seq==ANIM_DIE && me->frm==1 && me->frmTimer<64)
		{
			for(i=0;i<30;i++)
				BlowSmoke(me->x-32*FIXAMT+Random(64*FIXAMT),
					  	 me->y-32*FIXAMT+Random(64*FIXAMT),
								Random(64)<<FIXSHIFT,Random(FIXAMT*4));
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
			{
				// get him!
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(256)==0)
			{
				me->mind=1;		// occasionally rest
				me->mind1=(byte)Random(100);
			}
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
		{
			// get him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
		FaceGoodguy2(me,goodguy);

		if(!(me->mind1--))	// time to get a new direction
		{
			me->mind=0;	// get back on track
		}

		me->dx=0;
		me->dy=0;
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
	}
}

void AI_BufferBot(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ROBOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ROBODIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			MakeSound(SND_FACTORYGO, me->x, me->y, SND_CUTOFF, 1200);
			MakeColdRingParticle(me->x,me->y,me->z,GetMonsterType(me->type)->size);
			BuffNearbyAllies(me,me->x,me->y,120,4,128,map,world,me->friendly,0);
			me->reload=2;
		}
		if(me->seq==ANIM_DIE && me->frm>3 && me->frmTimer<63)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly roll towards the player
	{
		if(goodguy)
		{
			FaceGoodguy3(me,goodguy);
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(72)==0)
			{
				// shoot at him
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(48)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(144)==0)
			{
				// shoot at him
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=192;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*6;
		me->dy=Sine(me->facing*32)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Tower2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	BasicAI(me,SND_TOWEROUCH,SND_TOWERDIE,map,world,goodguy);

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_TOWEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_TOWERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)	// leaping attack
		{
			if(me->frm<4)
			{
				me->dx=0;
				me->dy=0;
				me->dz=0;
			}
			else if(me->frm==4)
			{
				me->dx=Cosine(me->facing*32)*8;
				me->dy=Sine(me->facing*32)*8;
				me->dz=10*FIXAMT;
			}
			else if(me->frm<8)
			{
				// stay the course
				Dampen(&me->dx,FIXAMT/8);
				Dampen(&me->dy,FIXAMT/8);
			}
			else if(me->frm==12 && me->frmTimer<32)
			{
				for(i=0;i<4;i++)
					FireBullet(me->x,me->y-FIXAMT,(me->facing+i*64)&255,BLT_EARTHSPIKE,me->friendly);
				MakeSound(SND_SDZLPOUND,me->x,me->y,SND_CUTOFF,1500);
				ShakeScreen(10);
			}
			else
			{
				Dampen(&me->dx,FIXAMT/2);
				Dampen(&me->dy,FIXAMT/2);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=4)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<300*FIXAMT)
				me->mind=1;
		}
		if(me->ouch)
			me->mind=1;
	}
	else	// chase down Kid Mystic
	{
		if(goodguy)
		{
			if(me->mind1)
				{
					me->facing=(me->facing+3+Random(3))&7;
					me->mind1=0;
				}
				else
					FaceGoodguy2(me,goodguy);
			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(32)==0)
				DoMove(me,ANIM_ATTACK,128,1,0,0);
		}
		else
			me->mind=0;
	}
}

void AI_Payload(Guy* me, Map* map, world_t* world, Guy* goodguy)
{
	int x,y;
	byte ok[4];
	int diff,dir;
	static byte noiseLoop=0;


	if(RangeToTarget(me,goodguy)<144*FIXAMT)
	{
		if(!me->mind4)
			MakeSound(SND_ROBOBOUAPHAON,me->x,me->y,SND_CUTOFF,900);
		me->mind4 = 30;
	}

	if (!me->mind4) {
		Dampen(&me->dx, FIXAMT / 2);
		Dampen(&me->dy, FIXAMT / 2);
	}

	if (!goodguy || !me->mind4)
		return;

	me->facing = me->mind2 / 32;	// mind2 holds the 'true' visual facing 0-255

	noiseLoop++;
	if (noiseLoop >= 28)
	{
		noiseLoop = 0;
		MakeSound(SND_MINECART, goodguy->x, goodguy->y, SND_CUTOFF, 1200);
	}

	// mind2 is the visual facing, mind3 is the actual facing,
		// so rotate mind2 towards mind3
	if (me->mind3 > me->mind2)
	{
		diff = me->mind3 - me->mind2;
		if (diff > 128)
		{
			dir = -1;
			diff = 256 - diff;
		}
		else
			dir = 1;
	}
	else if (me->mind3 < me->mind2)
	{
		diff = me->mind2 - me->mind3;
		if (diff > 128)
		{
			dir = 1;
			diff = 256 - diff;
		}
		else
			dir = -1;
	}
	else
	{
		diff = 0;
		dir = 0;
	}

	if (me->mind == 0)
	{
		// turn quickly if sitting still
		if (diff > 32)
			diff = 32;
	}
	else
	{
		if (diff > 8)
			diff = 8;
	}

	if (me->mind1)
	{
		me->mind1 = 0;
		if (me->aiType == MONS_FRIENDLY)
			me->mind3 = Random(4) * 64;
		me->mind = 30;	// wait 1 seconds before trying again
		me->dx = 0;
		me->dy = 0;
		me->frm = 0;
		me->seq = ANIM_IDLE;
		me->frmTimer = 0;
		me->frmAdvance = 128;
		return;
	}

	if (me->mind > 0)
	{
		// countdown to try for a path again, in case the map has changed under you
		me->mind--;
		if (me->mind == 0)
		{
			me->oldx = me->x + 1;
		}
		return;
	}
	me->mind2 = (me->mind2 + diff * dir) & 255;

	// if you've reached the center of a tile, time to decide where to go
	x = (me->mapx * TILE_WIDTH + TILE_WIDTH / 2) * FIXAMT;
	y = (me->mapy * TILE_HEIGHT + TILE_HEIGHT / 2) * FIXAMT;
	if ((me->x <= x && me->oldx > x) || (me->x >= x && me->oldx < x) ||
		(me->y <= y && me->oldy > y) || (me->y >= y && me->oldy < y))
	{
		//	me->x=x;
		//	me->y=y;
			// figure out which directions are valid bunny paths
		if (me->mapx < map->width - 1 && IsBunnyAble(&map->map[me->mapx + 1 + me->mapy * map->width], world))
			ok[0] = 1;
		else
			ok[0] = 0;
		if (me->mapy < map->height - 1 && IsBunnyAble(&map->map[me->mapx + (me->mapy + 1) * map->width], world))
			ok[1] = 1;
		else
			ok[1] = 0;
		if (me->mapx > 0 && IsBunnyAble(&map->map[me->mapx - 1 + me->mapy * map->width], world))
			ok[2] = 1;
		else
			ok[2] = 0;
		if (me->mapy > 0 && IsBunnyAble(&map->map[me->mapx + (me->mapy - 1) * map->width], world))
			ok[3] = 1;
		else
			ok[3] = 0;

		ok[(((me->mind3 / 32) + 4) & 7) / 2] = 0;

		// count how many directions are available
		y = 0;
		for (x = 0; x < 4; x++)
			if (ok[x])
				y++;

		if (y == 0)	// no directions to go!
		{
			if (me->aiType == MONS_FRIENDLY)
				me->mind3 = Random(4) * 64;
			me->mind = 30;	// wait 1 second before trying again
			DoMove(me, ANIM_IDLE, 0, 0);
			return;
		}
		else
		{
			if (y == 1)
			{
				// face the only valid direction
				for (x = 0; x < 4; x++)
					if (ok[x])
						me->mind3 = x * 64;
			}
			else
			{
				if (!ok[me->mind3 / 64])
					// if you can continue ahead, then go on and enjoy
				{
					// in the case of multiple directions, we must pick randomly
					x = Random(4);
					if (ok[x])	// the direction you're facing is ok
						me->mind3 = x * 64;
					else if (ok[(x + 1) & 3])	// direction to your left
						me->mind3 = ((x + 1) & 3) * 64;
					else if (ok[(x + 5) & 3])	// direction to your right (same as x-1)
						me->mind3 = ((x + 5) & 3) * 64;
					else
						me->mind3 = ((x + 2) & 3) * 64;	// behind you as a last resort
				}
			}
		}
	}

	// move forward at that pace
	me->dx = Cosine(me->mind3) * 3;
	me->dy = Sine(me->mind3) * 3;
	me->mind4--;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
	}
	me->frmAdvance=128;
}

void AI_Isodope(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i,j,x,y;
	bullet_t *b;

	if(me->reload)
		me->reload--;

	if(me->mind1)
		me->mind1--;

	BasicAI(me,SND_SZOUCH,SND_SZDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1)	// leaping attack
		{
			if(me->frm<3)
			{
				me->dx=0;
				me->dy=0;
				me->dz=0;
			}
			else if(me->frm==3)
			{
				me->dx=Cosine(me->facing*32)*12;
				me->dy=Sine(me->facing*32)*12;
				me->dz=10*FIXAMT;
			}
			else if(me->frm<7)
			{
				// stay the course
				Dampen(&me->dx,FIXAMT/8);
				Dampen(&me->dy,FIXAMT/8);
			}
			else if(me->frm==12 && me->frmTimer<32)
			{
				for(i=0;i<256;i+=16)
				{
					b=FireExactBullet(me->x,me->y,me->z,Cosine(i)*6,Sine(i)*6,0,0,32,i,BLT_SPORE,me->friendly);
					//RecolorBullet(b,5,1);
				}

				for(i=0;i<5;i++)
				{
					j=(me->facing*32+(i-2)*32)&255;
					b=FireExactBullet(me->x,me->y,me->z,Cosine(j)*12,Sine(j)*12,0,0,32,i,BLT_SPORE,me->friendly);
					//RecolorBullet(b,5,1);
				}
				MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1500);
				ShakeScreen(10,8);
			}
			else
			{
				Dampen(&me->dx,FIXAMT/2);
				Dampen(&me->dy,FIXAMT/2);
			}
		}

		if(me->mind3)
		me->mind3--;
		else
		{
			me->mind3=10;
			if(me->hp<me->maxHP && me->hp>0)
				me->hp++;
		}

		if(me->seq==ANIM_A2 && goodguy)
		{
			if(!me->mind2)
			{
				me->mind2 = Random(2);
				me->mind4 = 0;
			}
			if(!me->reload)
			{
				x=me->x+Cosine(me->facing*32)*64;
				y=me->y+Sine(me->facing*32)*48;

				me->reload = 8;
				b=FireExactBullet(x,y,FIXAMT*48,Cosine(me->facing*32)*(6+me->mind4),Sine(me->facing*32)*(6+me->mind4),FIXAMT*2,0,128,me->facing*32,BLT_GRENADE,me->friendly);
				MakeSound(SND_MUSHMISSILE, me->x, me->y, SND_CUTOFF, 700);

				if(me->mind2)
					me->facing = (me->facing+1)&7;
				else
					me->facing = (me->facing-1)&7;
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			x=(me->x+Cosine(me->facing*32)*40)>>FIXSHIFT;
			y=(me->y+Sine(me->facing*32)*40)>>FIXSHIFT;
			if(me->AttackCheck(30,x,y,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*24,Sine(me->facing*32)*24,10,map,world);
				Inflict(goodguy,GEF_POISON,128);
			}
			me->reload=4;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=8)
				BigBoom(me);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind2>0)
		me->mind2=0;

	if(me->mind==0)
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<300*FIXAMT)
				me->mind=1;
		}
		if(me->ouch)
			me->mind=1;
	}
	else if(me->mind==2)		// when mind=2, hold still and go off on Bouapha
	{
		if(goodguy)
		{
			i=RangeToTarget(me,goodguy);
			if(i<(96*FIXAMT) && Random(10)==0 && me->reload==0)
			{
				// get him! (punch)
				DoMove(me,ANIM_ATTACK,128,1,0,0);
				return;
			}
			// spinny poison attack
			if(i<(256*FIXAMT) && Random(24)==0 && me->reload==0)
			{
				DoMove(me,ANIM_A2,192,1,0,0);
				return;
			}
			FaceGoodguy2(me,goodguy);
			me->dx=0;
			me->dy=0;
			if(me->seq!=ANIM_IDLE)
				DoMove(me,ANIM_IDLE,128,0,0,0);
			if(i>(80*FIXAMT) && me->mind1==0)
			{
				// chase him down!
				me->mind=1;
				me->mind1=8;
			}
		}
		else
		{
			// just sit there
		}
	}
	else if(me->mind==1)	// chase down Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(RangeToTarget(me,goodguy)<128*FIXAMT && me->mind1==0)
			{
				me->mind=2;	// in range, start killin'
				me->mind1=8;
			}
			else if(Random(64)==0)
			{
				//leap!
				DoMove(me,ANIM_A1,128,1,0,0);
			}
		}
		else
			me->mind=0;
	}
}

void AI_SuperShroom(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i,x,y;

	if(me->reload)
		me->reload--;
	bullet_t *b;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm>=1 && me->frm<=15 && me->reload==0 && goodguy)
		{
			i=(me->facing*32-16+Random(33))&255;
			x=me->x+Cosine(me->facing*32)*72;
			y=me->y+Sine(me->facing*32)*64;
			x+=Cosine(i)*48;
			y+=Sine(i)*32;
			FireExactBullet(x,y,FIXAMT*64,Cosine(i)*8,Sine(i)*8,0,0,32,i,BLT_ENERGYPSN,me->friendly);
			MakeSound(SND_MUSHSPORES,me->x,me->y,SND_CUTOFF,600);
			me->reload=1;
		}
		if(me->seq==ANIM_A1 && me->frm==16)
		{
			FaceGoodguy3(me,goodguy);

			me->frmAdvance=64;	// slow the animation for the recovery part
		}
		if(me->seq==ANIM_ATTACK && me->frm==1 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*72;
			y=me->y+Sine(me->facing*32)*64;
			i=(me->facing*32+64)&255;
			x+=Cosine(i)*48;
			y+=Sine(i)*32;
			b=GetFireBullet(x,y,me->facing*32,BLT_SPOREBALL,me->friendly);
			if(b){
				b->flags |= BFL_WAVY;
				b->tall = 6;
				b->freq = 8;
				b->speed = 4;
				MakeSound(SND_MUSHMISSILE, me->x, me->y, SND_CUTOFF, 1000);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm==4 && me->reload==0)
		{
			for(i=0;i<8;i++)
			{
				x=me->x+Cosine(i*32)*72;
				y=me->y+Sine(i*32)*64;
				b=GetFireBullet(x,y,i,BLT_YELWAVE,me->friendly);
				if(b){
					b->flags |= BFL_WAVY;
					b->tall = 12;
					b->freq = 16;
					b->speed = 6;
					b->timer *= 2;
				}
			}
			me->reload=5;
		}
		if(me->seq==ANIM_DIE)
		{
			me->facing=2;	// die only works from forward (memory saver)
			if(me->frm>=3)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}

		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// this mode is only used prior to him seeing Bouapha the first time
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch>0)
			{
				me->mind=2;
			}
		}
	}
	else if(me->mind==2)		// when mind=2, hold still and go off on Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(384*FIXAMT) && Random(32)==0)
			{
				// get him! (fire shroom cannon)
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			if(RangeToTarget(me,goodguy)<(384*FIXAMT) && Random(28)==0)
			{
				i=1;
				if(me->hp < me->maxHP/2)
					i++;
				if(me->hp < me->maxHP/4)
					i++;
				me->mind2=Random(2)+i;
			}
			if(RangeToTarget(me,goodguy)<(384*FIXAMT) && Random(128)==0)
			{
				// get mad!
				MakeSound(SND_MUSHMAD,me->x,me->y,SND_CUTOFF,1200);
				me->facing=2;
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			FaceGoodguy2(me,goodguy);
			me->dx=0;
			me->dy=0;
			if(me->seq!=ANIM_IDLE)
			{
				me->seq=ANIM_IDLE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=192;
			}
			if(RangeToTarget(me,goodguy)>(384*FIXAMT))
			{
				// chase him down!
				me->mind=1;
			}
		}
		else
		{
			// just sit there
		}
	}
	else if(me->mind==1)	// chase down Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
			}
			if(RangeToTarget(me,goodguy)<200*FIXAMT)
				me->mind=2;	// in range, start killin'
		}
		else
			me->mind=0;
	}

	if(me->mind2>0)
	{
		// get him! (fire sporegun)
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		me->mind2--;
			return;
	}
}

void UltraSpew(Guy* me)
{
	int i;
	// first a pile of energy bullets
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, -FIXAMT * 4, FIXAMT * 6, -FIXAMT * 2, 0, 60, 2, BLT_ENERGY2, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, -FIXAMT * 2, FIXAMT * 8, -FIXAMT * 2, 0, 60, 2, BLT_ENERGY2, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, 0, FIXAMT * 9, -FIXAMT * 2, 0, 60, 2, BLT_ENERGY2, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, FIXAMT * 2, FIXAMT * 8, -FIXAMT * 2, 0, 60, 2, BLT_ENERGY2, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, FIXAMT * 4, FIXAMT * 6, -FIXAMT * 2, 0, 60, 2, BLT_ENERGY2, me->friendly);
	// now some fire
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, -FIXAMT * 4, FIXAMT * 16, -FIXAMT * 10, 0, 24, 2, BLT_FLAME2, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, 0, FIXAMT * 18, -FIXAMT * 10, 0, 24, 2, BLT_FLAME2, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, FIXAMT * 4, FIXAMT * 16, -FIXAMT * 10, 0, 24, 2, BLT_FLAME2, me->friendly);
	// and some rocks
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, -FIXAMT * 8, FIXAMT * 1, -FIXAMT * 1, 0, 60, 2, BLT_ROCK, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, -FIXAMT * 4, FIXAMT * 4, -FIXAMT * 1, 0, 60, 2, BLT_ROCK, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, FIXAMT * 4, FIXAMT * 4, -FIXAMT * 1, 0, 60, 2, BLT_ROCK, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, FIXAMT * 8, FIXAMT * 1, -FIXAMT * 1, 0, 60, 2, BLT_ROCK, me->friendly);
	// and big snowballs
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, -FIXAMT * 5, FIXAMT * 1, -FIXAMT * 2, 0, 40, 2, BLT_BIGSNOW, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, 0, FIXAMT * 6, -FIXAMT * 2, 0, 40, 2, BLT_BIGSNOW, me->friendly);
	FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, FIXAMT * 5, FIXAMT * 1, -FIXAMT * 2, 0, 40, 2, BLT_BIGSNOW, me->friendly);
	// and cactus spines
	for (i = 108; i > 19; i -= 10)
		FireExactBullet(me->x, me->y + FIXAMT * 30, FIXAMT * 60, Cosine(i) * 8, Sine(i) * 8, -FIXAMT * 2, 0, 40, i / 16, BLT_SPINE, me->friendly);
}

void AI_GreatPumpkin(Guy* me, Map* map, world_t* world, Guy* goodguy)
{
	int x, y, i, j;
	Guy* g;
	byte f;

	int pumpkins[5][2] = { {-64,40},{-32,40},{0,40},{32,40},{64,40} };

	if (me->reload)
		me->reload--;

	if (me->ouch == 4)
	{
		if (me->hp > 0)
			MakeSound(SND_GREATPKOUCH, me->x, me->y, SND_CUTOFF, 1200);
		else
			MakeSound(SND_GREATPKDIE, me->x, me->y, SND_CUTOFF, 1200);
	}

	if (me->action == ACTION_BUSY)
	{
		if (me->seq == ANIM_ATTACK && me->frm == 6 && me->frmTimer < 32)
		{
			if (me->specialFlags & GSF_NEWAI)
				i = Random(10);
			else
				i = 0;

			j = i >= 8 ? 2 : 1;

			for (i = 0; i < 5; i += j)
			{
				g = AddBaby(me->x + FIXAMT * pumpkins[i][0], me->y + FIXAMT * pumpkins[i][1], 0, j == 2 ? MONS_PUMPKIN2 : MONS_PUMPKIN, me);
				if (g && (!g->CanWalk(g->x, g->y, map, world)))
					RemoveGuy(g);
				else if (g)
				{
					g->mind = 1;
					g->mind1 = 120;
				}
			}

			me->reload = 60;
			MakeSound(SND_GREATPKVOMIT, me->x, me->y, SND_CUTOFF, 1100);
		}

		if (me->seq == ANIM_DIE)
		{
			x = me->x >> FIXSHIFT;
			y = me->y >> FIXSHIFT;
			BlowUpGuy(x + me->rectx, y + me->recty, x + me->rectx2, y + me->recty2, me->z, 2);
			BlowSmoke((x + me->rectx + Random(me->rectx2 - me->rectx)) << FIXSHIFT,
				(y + me->recty + Random(me->recty2 - me->recty)) << FIXSHIFT,
				me->z, FIXAMT);
			ShakeScreen(10);
		}
		return;	// can't do nothin' right now
	}

	if (me->mind > 0)
		me->mind--;

	if (goodguy)
		FaceGoodguy(me, goodguy);

	if (me->mind == 8)
	{
		// left eye fires
		f = (me->facing * 32 - 32 + me->mind1) & 255;

		FireExactBullet(me->x - 64 * FIXAMT, me->y + 32 * FIXAMT, me->z + 80 * FIXAMT,
			Cosine(f) * 8, Sine(f) * 8, -3 * FIXAMT, 0, 30, me->facing, BLT_ENERGY2, me->friendly);
	}
	if (!me->mind)
	{
		// right eye fires
		f = (me->facing * 32 + 32 - me->mind1) & 255;
		// can't fire backwards

		if (f > 128)
			f = f < 192 ? 128 : 0;

		FireExactBullet(me->x + 64 * FIXAMT, me->y + 32 * FIXAMT, me->z + 80 * FIXAMT,
			Cosine(f) * 8, Sine(f) * 8, -3 * FIXAMT, 0, 30, me->facing, BLT_ENERGY2, me->friendly);
		me->mind = 16;
		me->mind1 += 8;
		if (me->mind1 >= 64)
			me->mind1 = 0;
	}

	if (me->reload == 0 && Random(20) == 0)
	{
		me->seq = ANIM_ATTACK;
		me->frm = 0;
		me->frmTimer = 0;
		me->frmAdvance = 64;
		me->action = ACTION_BUSY;
	}
}

void AI_Ultrazoid(Guy* me, Map* map, world_t* world, Guy* goodguy)
{
	int i;

	if (me->reload)
		me->reload--;

	if (me->mind1)
		me->mind1--;

	BasicAI(me, SND_SERPENTOUCH, SND_SERPENTDIE, map, world, goodguy);

	if (me->action == ACTION_BUSY)
	{
		if (me->seq == ANIM_ATTACK && (me->frm == 6 || me->frm == 7) && me->frmTimer < 64)
		{
			// dump a wad of zoid projectiles
			UltraSpew(me);
			MakeSound(SND_SERPENTSPIT, me->x, me->y, SND_CUTOFF, 1100);
			me->mind1 = 2;
		}
		if (me->seq == ANIM_DIE)
			ShakeScreen(10);
		return;	// can't do nothin' right now
	}

	if (me->reload == 0)
	{
		me->seq = ANIM_ATTACK;
		me->frm = 0;
		me->frmTimer = 0;
		me->frmAdvance = 72;
		me->action = ACTION_BUSY;
		me->reload = 90;
	}
}

void AI_Voltage(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	bullet_t* b;
	int x,y;

	if(me->dx==0 && me->dy==0)
	{
		me->dx=FIXAMT*4;
		me->dy=FIXAMT*4;
	}

	VoltageWeather(me->x,me->y);

	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;
	me->mind1=0;

	BasicAI(me,SND_EGGOUCH,SND_VOLTAGEDIE,map,world,goodguy);

	if(me->mind2==0)
	{
		me->mind2=10;
		FaceGoodguy(me,goodguy);
		x=me->facing*32+32-Random(65);
		if(x<0)
			x+=256;
		if(x>255)
			x-=256;
		b = GetFireBullet(me->x,me->y,x,BLT_BADLIGHTNING,me->friendly);
		if(b)
		{
			b->flags |= BFL_WAVY;
			b->tall = 8;
			b->freq = 16;
			b->speed = 8;
		}
	}
	else
		me->mind2--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->frmTimer<64 && (goodguy))
		{
			if(me->AttackCheck(100,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				FaceGoodguy(me,goodguy);
				LightningBolt(
						me->x-FIXAMT*32+Random(FIXAMT*64),
						me->y-FIXAMT*82+Random(FIXAMT*64),
						goodguy->x-FIXAMT*32+Random(FIXAMT*64),
						goodguy->y-FIXAMT*52+Random(FIXAMT*64));
				MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,400);
				goodguy->GetShot(Cosine(me->facing*32)*18,
								Sine(me->facing*32)*18,30,map,world);
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			//BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<100*FIXAMT)
		{
			MakeSound(SND_JACKATTACK,me->x,me->y,SND_CUTOFF,2000);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->action=ACTION_BUSY;
			return;
		}
	}
	if(IsOneFacing(me))
	{
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
		}
	}
	else
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
	}
	
}


// here be the AIs for each monster type
//--------------------------------------------------------------------------------------

#include "monsterai1.inc" // Dr. Lunatic - original
#include "monsterai2.inc" // Expando Pak
#include "monsterai3.inc" // Fun Pack
#include "monsterai4.inc" // Supreme
#include "monsterai5.inc" // Kid Mystic
#include "monsterai6.inc" // Sleepless Hollow
#include "monsterai7.inc" // Loonyland
#include "monsterai8.inc" // Operation SCARE
