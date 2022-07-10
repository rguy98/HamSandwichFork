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

monsterType_t *GetMonsterType(dword type)
{
	return &monsType[type];
}

void ChangeOffColor(dword type,byte from,byte to)
{
	monsType[type].fromCol=from;
	monsType[type].toCol=to;
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
		shld=PlayerShield();
		if((shld<16) && (shld&2))	// it blinks when there is 1/2 second left
			shld=0;
		curSpr=monsType[MONS_BOUAPHA].spr->GetSprite(464+(shld&7));
		if(curSpr==NULL)
			return;
		if(shld)
			SprDraw(x>>FIXSHIFT,(y>>FIXSHIFT)+1,1+(z>>FIXSHIFT),255,bright,curSpr,DISPLAY_DRAWME|DISPLAY_GLOW);
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
		else if(player.invisibility)
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

	if(!(monsType[type].flags&MF_NOSHADOW))
		SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,0,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW);

	if(g->ouch==0)
	{
		if(g->frozen)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,7,bright+4,curSpr,DISPLAY_DRAWME);
		else if(g->ignited)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,4,bright+16,curSpr,DISPLAY_DRAWME);
		else if(g->poison)
			SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,1,bright,curSpr,DISPLAY_DRAWME);
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
				SprDrawOff(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,monsType[type].fromCol,monsType[type].toCol,
					bright+monsType[type].brtChg,curSpr,DISPLAY_DRAWME);
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

void AI_Frozombie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	bullet_t *b;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ZOMBIEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm==7 && me->reload==0 && goodguy)
		{
			// only fire if the range is good
			if(RangeToTarget(me,goodguy)<800*FIXAMT)
			{
				x=me->x+Cosine(me->facing*32)*32;
				y=me->y+Sine(me->facing*32)*32;
				b=GetFireBullet(x,y,me->facing*32,BLT_BIGSNOW,me->friendly);
				if(b)
				{
					b->dx=Cosine(b->facing)*4;
					b->dy=Sine(b->facing)*4;
					b->z=FIXAMT*30;
				}
			}
			me->reload=5;
			me->mind1=1;
		}
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
		}
		if(me->seq==ANIM_ATTACK && me->frm>6)
		{
			Dampen(&me->dx,FIXAMT/2);
			Dampen(&me->dy,FIXAMT/2);
		}
		if(me->seq==ANIM_ATTACK && me->frm>=7 && me->frm<13 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(8,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
				me->reload=2;
			}
		}
		if(me->seq==ANIM_DIE)
		{
			me->frmAdvance = 128;
			for(x=0;x<8;x++)
				SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT),
							me->y+((-16+Random(33))<<FIXSHIFT));
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(64*FIXAMT) && Random(8)==0)
			{
				// get him!
				MakeSound(SND_ZOMBIELEAP,me->x,me->y,SND_CUTOFF,1200);
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
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
			{
				// throw a snowball
				MakeSound(SND_SNOWTHROW,me->x,me->y,SND_CUTOFF,1100);
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
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

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Sandman(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,s;
	bullet_t *b;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SNOWOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SNOWDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			if(me->AttackCheck(20,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,8,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==11 && me->reload==0 && goodguy)
		{
			// only fire if the range is good
			if(RangeToTarget(me,goodguy)<800*FIXAMT)
			{
				x=me->x+Cosine(me->facing*32)*32;
				y=me->y+Sine(me->facing*32)*32;
				b=GetFireBullet(me->x+x,me->y+y,me->facing*32,BLT_ROCK,me->friendly);
				if(b)
				{
					s=-2+Random(4);
					b->dx=Cosine(b->facing)*s;
					b->dy=Sine(b->facing)*s;
					b->z=FIXAMT*30;
				}
			}
			me->reload=5;
			me->mind1=1;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==1)
			{
				for(int i=0; i<8; i++)
				{
					x=me->x+Cosine(i*32)*32;
					y=me->y+Sine(i*32)*32;
					b=GetFireBullet(me->x+x,me->y+y,i*32,BLT_ROCK,me->friendly);
					if(b)
					{
						s=-1+Random(2);
						b->dx=Cosine(b->facing)*s;
						b->dy=Sine(b->facing)*s;
						b->z=FIXAMT*30;
					}
				}
			}
			me->frmAdvance = 128;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(64*FIXAMT) && Random(8)==0)
			{
				// get him!
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
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
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*1;
			me->dy=Sine(me->facing*32)*1;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
			{
				// throw a snowball
				MakeSound(SND_SNOWTHROW,me->x,me->y,SND_CUTOFF,1100);
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
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

		me->dx=Cosine(me->facing*32)*1;
		me->dy=Sine(me->facing*32)*1;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_MiniZombie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ZOMBIEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
		}
		if(me->seq==ANIM_ATTACK && me->frm>6)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
		}
		if(me->seq==ANIM_ATTACK && me->frm>=7 && me->frm<13 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			if(me->AttackCheck(8,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,1,map,world);
				me->reload=2;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(64*FIXAMT) && Random(32)==0)
			{
				// get him!
				MakeSound(SND_ZOMBIELEAP,me->x,me->y,SND_CUTOFF,1200);
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

			FaceGoodguy(me,goodguy);
			
			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if (Random(64) == 0)
			{
				me->mind = 1;		// occasionally wander
				me->mind1 = 1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}
		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
		}
	}
}

void AI_MiniMattie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte tempface;
	byte faceTable[8]={5,4,0,1,2,3,0,6};
	bullet_t *b;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MATTIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_LOOKEYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
			ShakeScreen(2,1);
		}
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			// shoot laser
			x=me->x;
			y=me->y+FIXAMT*8;
			b=GetFireBullet(x,y,0,BLT_DEATHBEAM,me->friendly);
			me->reload=0;
			me->mind=0;
			me->reload=100;
		}
		me->facing=0;
		return;
	}
	else
	{
		tempface=me->facing;
		FaceGoodguy(me,goodguy);

		if(me->facing>=5)	// can't look straight back, so deal with it
		{
			if(tempface<2)
				me->facing=0;
			else
				me->facing=4;
		}
		me->facing=faceTable[me->facing];
		if(me->facing!=0)
			me->facing+=4;
	}

	if((!me->reload) && Random(100)==0)
	{
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=32;	// very very slow animation
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
	}

	// use the claws to cover your face when scared
	if(me->ouch && me->mind1<32)
		me->mind1+=8;	// mind1 is fear, when it gets high, the claws cover her face

	if(me->mind1)
		me->mind1--;
}

void AI_MiniMattieClaw(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload>0)
		me->reload--;

	if(!me->parent || me->parent->type==MONS_NONE)
	{
		me->type=MONS_NONE;	// can't exist alone
		return;
	}

	if(me->aiType==MONS_MINIMATCLAW1)
	{
		me->x=me->parent->x-26*FIXAMT;
		me->y=me->parent->y+6*FIXAMT;
		me->z=me->parent->z-6*FIXAMT;
	}
	else	// MONS_MATCLAW2
	{
		me->x=me->parent->x+26*FIXAMT;
		me->y=me->parent->y+6*FIXAMT;
		me->z=me->parent->z-6*FIXAMT;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->parent->mind1>32)
			me->frmTimer=0;	// hold them up until she calms down
		if(me->seq==ANIM_ATTACK && me->frm>1)
		{
			if(me->frm<5)
			{
				if(me->aiType==MONS_MINIMATCLAW1)
					x=me->x-FIXAMT*59;
				else
					x=me->x+FIXAMT*59;
				y=me->y+FIXAMT*89;
			}
			else
			{
				if(me->aiType==MONS_MINIMATCLAW1)
					x=me->x-FIXAMT*35;
				else
					x=me->x+FIXAMT*35;
				y=me->y+FIXAMT*80;
			}

			FindVictim(x>>FIXSHIFT,y>>FIXSHIFT,50,0,FIXAMT*8,8,map,world,me->friendly);
		}
		return;
	}

	if(me->reload==0)
	{
		MakeSound(SND_MATTIECLAW,me->x,me->y,SND_CUTOFF,1200);
		me->reload=Random(180)+20;
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		return;
	}

	// make their idle weird and wiggly
	me->frmAdvance=Random(128)+1;

	if(me->parent->mind1>32)	// she is scared, block!!
	{
		me->action=ACTION_BUSY;
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=48;
	}
}

void AI_MiniMattieTail(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte f;
	int x,y;
	short shootXTable[9]={-134,-124,-101,-57,0,67,113,134,143};
	short shootYTable[9]={-51,-43,-24,-7,-11,-15,-29,-41,-46};

	if(!me->parent || me->parent->type==MONS_NONE)
	{
		me->type=MONS_NONE;	// can't exist alone
		return;
	}

	me->x=me->parent->x;
	me->y=me->parent->y-52*FIXAMT;
	me->z=me->parent->z;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			f=(32-me->facing)*4;
			// shoot some bullets
			x=me->x+((shootXTable[(me->facing>>2)])/2<<FIXSHIFT);
			y=me->y+((shootYTable[(me->facing>>2)])/2<<FIXSHIFT);

			if(me->parent->hp<monsType[me->parent->type].hp/2)
			{
				f+=12-Random(25);
				if (Random(2))
				FireExactBullet(x,y,me->z+16*FIXAMT,Cosine(f)*8,Sine(f)*8,-FIXAMT/2,
					0,60,f,BLT_ENERGY,me->friendly);
			}
			else
			{
				f+=12-Random(25);
				if (Random(2))
				FireExactBullet(x,y,me->z+16*FIXAMT,Cosine(f)*6,Sine(f)*6,-FIXAMT/2,
					0,60,f,BLT_ENERGY,me->friendly);
			}
		}
		return;
	}

	if(me->mind)
		me->mind--;

	if(me->mind<10)
	{
		MakeSound(SND_MATTIESHOOT,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=255;
		me->action=ACTION_BUSY;
		if(me->mind==0)
		{
			me->mind = 50;
		}
	}

	if(goodguy)
	{
		// point at Bouapha
		if(goodguy->x<me->x-FIXAMT*128)
		{
			if(goodguy->y<me->y+FIXAMT*64)
				f=0;
			else
				f=8;
		}
		else if(goodguy->x>me->x+FIXAMT*128)
		{
			if(goodguy->y<me->y+FIXAMT*64)
				f=32;
			else
				f=24;
		}
		else if(goodguy->y>me->y+FIXAMT*64)
		{
			f=16;
		}
		else
		{
			// can't point at him when he is behind, but you can try
			if(goodguy->x<me->x)
				f=0;
			else
				f=32;
		}
	}
	else
		f=16;

	// b contains the desired facing
	if(me->mind1<f)
		me->mind1++;
	else if(me->mind1>f)
		me->mind1--;
	me->facing=me->mind1&(~3);	// facing is locked in multiples of 4
}

void AI_MiniMattieBody(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte b;

	if(!me->parent || me->parent->type==MONS_NONE)
	{
		me->type=MONS_NONE;	// can't exist alone
		return;
	}

	// unlike other components, this one tells the head itself where to be
	me->parent->x=me->x;
	me->parent->y=me->y+20*FIXAMT;
	me->parent->z=me->z+16*FIXAMT;

	if(goodguy)
	{
		if (goodguy->x < me->x)
			me->dx -= FIXAMT / 16;	// slow acceleration
		if (goodguy->x > me->x)
			me->dx += FIXAMT / 16;
		if (goodguy->y < me->y)		//CRUSH him if he gets behind you
			me->dy -= FIXAMT / 4;
		if (goodguy->y > me->y)
			me->dy += FIXAMT / 4;
		else
		{
			if (me->mind > 128)
			{
				me->mind--;
				me->dy += FIXAMT / 16;
			}
			else if (me->mind < 128)
			{
				me->mind++;
				me->dy -= FIXAMT / 16;
			}
		}
	}
	b=(byte)Random(128);
	if(b<10)
		me->mind=128-Random(64);
	else if(b<40)
		me->mind=128+Random(64);
	else if(b<50)
		me->mind=128;

	Dampen(&me->dx,FIXAMT/32);
	Dampen(&me->dy,FIXAMT/32);
	Clamp(&me->dx,FIXAMT*1);
	Clamp(&me->dy,FIXAMT*1);

	if(me->seq!=ANIM_MOVE)
	{
		MakeSound(SND_MATTIESTOMP,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
	}
}

void AI_MineCartBad(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte ok[4];
	int diff,dir;
	static byte noiseLoop=0;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ZOMBIEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	me->facing=me->mind2/32;	// mind2 holds the 'true' visual facing 0-255
	
	if(me->confuse)
		me->confuse=0;

	// mind2 is the visual facing, mind3 is the actual facing,
	// so rotate mind2 towards mind3
	if(me->mind3>me->mind2)
	{
		diff=me->mind3-me->mind2;
		if(diff>128)
		{
			dir=-1;
			diff=256-diff;
		}
		else
			dir=1;
	}
	else if(me->mind3<me->mind2)
	{
		diff=me->mind2-me->mind3;
		if(diff>128)
		{
			dir=1;
			diff=256-diff;
		}
		else
			dir=-1;
	}
	else
	{
		diff=0;
		dir=0;
	}

	if(me->mind==0)
	{
		// turn quickly if sitting still
		if(diff>32)
			diff=32;
	}
	else
	{
		if(diff>me->mind1/4)
			diff=me->mind1/4;
	}

	me->mind2=(me->mind2+diff*dir)&255;

	if(me->mind==0)	// waiting to be ridden
	{
		// figure out which directions are valid minecart paths
		if(me->mapx<map->width-1 && GetTerrain(world,map->map[me->mapx+1+me->mapy*map->width].floor)->flags&TF_MINECART)
			ok[0]=1;
		else
			ok[0]=0;
		if(me->mapy<map->height-1 && GetTerrain(world,map->map[me->mapx+(me->mapy+1)*map->width].floor)->flags&TF_MINECART)
			ok[1]=1;
		else
			ok[1]=0;
		if(me->mapx>0 && GetTerrain(world,map->map[me->mapx-1+me->mapy*map->width].floor)->flags&TF_MINECART)
			ok[2]=1;
		else
			ok[2]=0;
		if(me->mapy>0 && GetTerrain(world,map->map[me->mapx+(me->mapy-1)*map->width].floor)->flags&TF_MINECART)
			ok[3]=1;
		else
			ok[3]=0;

		// pick one to face
		for(x=0;x<4;x++)
			if(ok[x])
				me->mind3=x*64;

		if(me->mind1==1)	// wait for goodguy to go away before re-allowing him to board
		{
			if(RangeToTarget(me,goodguy)>32*FIXAMT)
				me->mind1=0;
		}
		else if(map->CheckLOS(me->mapx,me->mapy,12,goodguy->mapx,goodguy->mapy))
		{
			MakeSound(SND_MINECART,me->x,me->y,SND_CUTOFF,1200);
			noiseLoop=0;
			me->mind=1;
			me->mind1=0;	// acceleration
		}
	}
	else	// being ridden
	{
		FindVictims((me->x+me->dx)>>FIXSHIFT,(me->y+me->dy)>>FIXSHIFT,16,-FIXAMT*10+Random(FIXAMT*21),-FIXAMT*10+Random(FIXAMT*21),20,map,world,me->friendly);
		//FindVictims((me->x+me->dx)>>FIXSHIFT,(me->y+me->dy)>>FIXSHIFT,16,-FIXAMT*10+Random(FIXAMT*21),-FIXAMT*10+Random(FIXAMT*21),20,map,world,me->friendly);
		if(me->parent)
		{
			// confine the parent to the cart
			me->parent->x=me->x;
			me->parent->y=me->y+1;
			me->parent->z=FIXAMT*8;
			me->parent->dx=0;
			me->parent->dy=0;
			me->parent->dz=0;
		}
		
		noiseLoop++;
		if(noiseLoop>=28)
		{
			noiseLoop=0;
			MakeSound(SND_MINECART,me->x,me->y,SND_CUTOFF,1200);
		}
		// if you've reached the center of a tile, time to decide where to go
		x=(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		y=(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
		if((me->x<=x && me->oldx>x) || (me->x>=x && me->oldx<x) ||
		   (me->y<=y && me->oldy>y) || (me->y>=y && me->oldy<y))
		{
			me->x=x;
			me->y=y;
			// figure out which directions are valid minecart paths
			if(me->mapx<map->width-1 && GetTerrain(world,map->map[me->mapx+1+me->mapy*map->width].floor)->flags&TF_MINECART)
				ok[0]=1;
			else
				ok[0]=0;
			if(me->mapy<map->height-1 && GetTerrain(world,map->map[me->mapx+(me->mapy+1)*map->width].floor)->flags&TF_MINECART)
				ok[1]=1;
			else
				ok[1]=0;
			if(me->mapx>0 && GetTerrain(world,map->map[me->mapx-1+me->mapy*map->width].floor)->flags&TF_MINECART)
				ok[2]=1;
			else
				ok[2]=0;
			if(me->mapy>0 && GetTerrain(world,map->map[me->mapx+(me->mapy-1)*map->width].floor)->flags&TF_MINECART)
				ok[3]=1;
			else
				ok[3]=0;

			ok[(((me->mind3/32)+4)&7)/2]=0;

			// count how many directions are available
			y=0;
			for(x=0;x<4;x++)
				if(ok[x])
					y++;

			if(y==0)	// no directions to go!
			{
				me->mind1=1;
				me->mind=0;
				me->dx=0;
				me->dy=0;
				me->frm=0;
				me->seq=ANIM_IDLE;
				me->frmTimer=0;
				me->frmAdvance=128;
				return;
			}
			else
			{
				if(y==1)
				{
					// face the only valid direction
					for(x=0;x<4;x++)
						if(ok[x])
							me->mind3=x*64;
				}
				else
				{
					// if you can go straight, then that's what you do
					if(ok[me->facing/2])
					{
						me->mind3=(me->facing/2)*64;
					}
					else
					{
						// in the case of multiple directions, we must decide on one
						// based on the player's facing
						if(ok[0]&&me->x<goodguy->x)	// the direction you're facing is ok
							me->mind3=0;
						else if(ok[1]&&me->y<goodguy->y)	// direction to your left
							me->mind3=64;
						else if(ok[2]&&me->x>goodguy->x)	// direction to your right (same as x-1)
							me->mind3=128;
						else
							me->mind3=192;	// behind you as a last resort
					}
				}
			}
		}

		// move forward at that pace
		me->dx=Cosine(me->mind3)*(me->mind1/8);
		me->dy=Sine(me->mind3)*(me->mind1/8);

		if(me->mind1<96)
			me->mind1++;

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
		me->frmAdvance=me->mind1;
	}
	
	if(me->parent)
	{
		me->parent->x=me->x;
		me->parent->y=me->y;
		me->parent->z=me->z+12*FIXAMT;
	}
}

void AI_Eyeguy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	bullet_t *b;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_EYEGUYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_EYEGUYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			// fire a shot
			if (me->aiType == MONS_EYEGUY2)
			{
				for(int i=0;i<3;i++)
				{
					b=GetFireBullet(me->x, me->y,(me->facing-1+i)&7, BLT_YELWAVE, me->friendly);
					if(b)
					{
						b->fromColor=5;
						b->toColor=GetBaseColor(me);
					}
				}
			}
			else
			{
				FireBullet(me->x, me->y, me->facing, BLT_YELWAVE, me->friendly);
			}
			me->reload=10;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0 && me->reload==0)
		{
			// spit at him
			MakeSound(SND_EYEGUYSHT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy(me,goodguy);

			if(me->aiType==MONS_EYEGUY2)
			{
				me->dx=Cosine(me->facing*32)*2;
				me->dy=Sine(me->facing*32)*2;
			}
			else
			{
				me->dx=Cosine(me->facing*32)*3;
				me->dy=Sine(me->facing*32)*3;
			}
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
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
