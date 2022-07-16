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

int RangeToPoint(Guy* me, int x, int y)
{
	return abs(me->x - x) + abs(me->y - y);
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

void AI_MineCart(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte ok[4];
	int diff,dir;
	static byte noiseLoop=0;

	goodguy=GetGuyOfAIType(MONS_BOUAPHA);
	if(!goodguy)
		return;

	me->facing=me->mind2/32;	// mind2 holds the 'true' visual facing 0-255

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
		else if(RangeToTarget(me,goodguy)<32*FIXAMT && player.vehicle==0)
		{
			MakeSound(SND_MINECART,goodguy->x,goodguy->y,SND_CUTOFF,1200);
			noiseLoop=0;
			me->mind=1;
			goodguy->parent=me;
			player.vehicle=VE_MINECART;
			goodguy->x=me->x;
			goodguy->y=me->y+1;
			goodguy->z=FIXAMT*8;
			me->mind1=0;	// acceleration
		}
	}
	else	// being ridden
	{
		if(!editing && !player.cheated && verified)
			profile.progress.cartDistance+=abs(me->dx/FIXAMT)+abs(me->dy/FIXAMT);
		noiseLoop++;
		if(noiseLoop>=28)
		{
			noiseLoop=0;
			MakeSound(SND_MINECART,goodguy->x,goodguy->y,SND_CUTOFF,1200);
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
				goodguy->dx=0;
				goodguy->dy=0;
				player.vehicle=0;
				goodguy->parent=NULL;
				goodguy->x=me->x;
				goodguy->y=me->y;
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
						x=goodguy->facing/2;
						if(ok[x])	// the direction you're facing is ok
							me->mind3=x*64;
						else if(ok[(x+1)&3])	// direction to your left
							me->mind3=((x+1)&3)*64;
						else if(ok[(x+3)&3])	// direction to your right (same as x-1)
							me->mind3=((x+3)&3)*64;
						else
							me->mind3=((x+2)&3)*64;	// behind you as a last resort
					}
				}
			}
		}

		// move forward at that pace
		me->dx=Cosine(me->mind3)*(me->mind1/8);
		me->dy=Sine(me->mind3)*(me->mind1/8);
		if(me->aiType==MONS_MINECARTSLOW)
		{
			Clamp(&me->dx,6*FIXAMT);
			Clamp(&me->dy,6*FIXAMT);
		}

		// confine the player to the cart
		goodguy->x=me->x;
		goodguy->y=me->y+1;
		goodguy->z=FIXAMT*8;
		goodguy->dx=0;
		goodguy->dy=0;
		goodguy->dz=0;

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


void AI_Dummy(Guy* me, Map* map, world_t* world, Guy* goodguy)
{
	if(!me->parent)
		return;

	Guy *g = me->parent;
	int size = monsType[g->type].size,
		tx = g->x + Cosine(g->facing*32)*-size,
		ty = g->y + Sine(g->facing*32)*-size;

	me->x = lerpi(me->x,tx,0.15);
	me->y = lerpi(me->y,ty,0.15);

	FaceGoodguy(me,me->parent);
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

void AI_LunaChick(Guy* me, Map* map, world_t* world, Guy* goodguy)
{
	int x, y, i;
	byte a;
	bullet_t* b;

	BasicAI(me,SND_LUNAOUCH,SND_LUNADIE,map,world,goodguy);
	if (me->action == ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm>3)
		{
			me->frmAdvance=128;
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z-Random(FIXAMT*60),FIXAMT);
		}
		if(me->seq==ANIM_ATTACK)
		{
			Dampen(&me->dx,FIXAMT/2);
			Dampen(&me->dy,FIXAMT/2);
			if(me->frm>3 && me->frm<11 && !me->reload)
			{
				x = me->x + Cosine(me->facing * 32) * 16;
				y = me->y + Sine(me->facing * 32) * 16;
				b = GetFireBullet(x,y,me->facing*32,BLT_BADLUNA,me->friendly);
				MakeSound(SND_LUNASHOOT,me->x,me->y,SND_CUTOFF,1200);
				if(b)
				{
					i=Random(6)+12;
					a=Random(4);
					b->z=30*FIXAMT;
					b->dx=Cosine(me->facing*32 + (a-2)*4)*i;
					b->dy=Sine(me->facing*32 + (a-2)*4)*i;
					me->reload = Random(4) + 4;
					b->flags |= BFL_HOMING;
				}
			}
		}
		if(me->seq==ANIM_A1)
		{
			me->mind4++;
			if(me->mind4<25)
			{
				me->dx+=Cosine(me->mind3);
				me->dy+=Sine(me->mind3);
				Clamp(&me->dx,FIXAMT*12);
				Clamp(&me->dy,FIXAMT*12);
			}
			else
			{
				Dampen(&me->dx,FIXAMT/2);
				Dampen(&me->dy,FIXAMT/2);
			}

			if(Bounce(me,me->mind3));
				me->mind1 = 0;
			me->facing = (me->facing + 1) & 7;	// spin!

			if (me->reload == 0)
			{
				MakeSound(SND_LUNASHOOT,me->x,me->y,SND_CUTOFF|SND_RANDOM,1200);
				a = AngleFrom(me->x,me->y,goodguy->x+Random(16)-8,goodguy->y+Random(16)-8);
				b = GetFireBullet(me->x,me->y,a,BLT_ENERGY2,me->friendly);
				if(b)
				{
					RecolorBullet(b,4,6);
				}
				me->reload = 4;
			}
		}
		return;	// can't do nothin' right now
	}
	if(me->mind == 0)
	{
		me->mind=1;
		me->mind2=30;
		me->facing=Random(8);
		DoMove(me,ANIM_MOVE,128,0,Cosine(me->facing*32)*6,Sine(me->facing*32)*6);
	}
	else if(me->mind == 1)
	{
		if (Bounce(me, me->facing));
			me->mind1 = 0;

		me->dx=lerpi(me->dx,Cosine(me->facing*32)*8,0.1);
		me->dy=lerpi(me->dy,Sine(me->facing*32)*8,0.1);

		if (me->seq != ANIM_MOVE)
			DoMove(me, ANIM_MOVE, 128, 0, me->dx, me->dy);
		if(Random(20) == 1)
			me->facing=Random(8);
		else if(Random(20) == 1)
		{
			FaceGoodguy(me,goodguy);
			me->frm=0;
			me->seq=ANIM_ATTACK;
			me->frmAdvance=64;
			me->action=ACTION_BUSY;
		}
		else if(Random(40) == 1)
		{
			MakeSound(SND_JACKATTACK,me->x,me->y,SND_CUTOFF|SND_RANDOM,1200);
			FaceGoodguy(me,goodguy);
			me->mind4=0;
			me->mind3=me->facing*32;
			me->mind2=30+Random(15);
			me->frm=0;
			me->seq=ANIM_A1;
			me->frmAdvance=32;
			me->action=ACTION_BUSY;
		}
		if(me->ouch&&Random(4)==0)
		{
			FaceGoodguy(me,goodguy);
		}
	}
}

void AI_SpisPumpkin(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	bullet_t *b;
	byte a;
	if(me->reload)
		me->reload--;

	if(!me->mind2)
		me->mind2=10*3;
	me->mind2--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			if(me->aiType==MONS_WACKYPUMP)
			{
				for (int i = 0; i < 4; i++)
				{
					a = (i * 64 + 32) & 255;
					b = GetFireBullet(me->x + Cosine(a) * 32, me->y + Sine(a) + 32, a, BLT_ACID, me->friendly);
					b->fromColor = 1;
					b->toColor = wacky[Random(3)];
				}
				ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 8);
				me->reload=2;
			}
			else if(me->aiType==MONS_ICYPUMPKN)
			{
				for (int i = 0; i < 8; i++)
					SpecialSnow(me->x + ((-16 + Random(33)) << FIXSHIFT),
						me->y + ((-16 + Random(33)) << FIXSHIFT));
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->seq==ANIM_MOVE && me->frm==2 && goodguy)	// hits on this frame
	{

		if (me->aiType == MONS_WACKYPUMP)
		{
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 24, Cosine(me->facing * 32) * 8, Sine(me->facing * 32) * 8, 2, map, world, me->friendly))
			{
				if (!goodguy->confuse)
					goodguy->confuse = 30 * 5;
			}
		}
		else if (me->aiType == MONS_ICYPUMPKN)
		{
			FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 24, Cosine(me->facing * 32) * 2, Sine(me->facing * 32) * 2, 2, map, world, me->friendly);
			FreezeGuy(goodguy);
		}
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}

		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch>0)
			{
				// if the goodguy is near, or he shot me
				me->mind=1;	// start hunting
				if(me->ouch==0)
					me->mind1=60;	// for 2 seconds minimum
				else
					me->mind1=120;	// 4 seconds, because they HATE getting shot
				FaceGoodguy(me,goodguy);
			}
		}
		me->dx=0;
		me->dy=0;
	}
	else
	{
		FaceGoodguy(me,goodguy);
		if(!goodguy)
		{
			me->mind=0;	// go back to being bored, nothing to see here
			me->mind1=0;
			return;
		}

		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>256*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}
		me->dx = Cosine(me->facing * 32) * 4;
		me->dy = Sine(me->facing * 32) * 4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
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

	BasicAI(me, SND_MATTIEOUCH, SND_LOOKEYDIE, map, world, goodguy);

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
			MakeSound(SND_LASERSMALL1,me->x,me->y,SND_CUTOFF,1200);
			for(int i=0;i<3;i++)
			{
				b=GetFireBullet(x,y,2,BLT_BIGYELLOW,me->friendly);
				if(b)
				{
					b->dx=Cosine((i+1+me->facing)*32)*8;
					b->dy=Sine((i+1+me->facing)*32)*8;
				}
			}
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
		me->frmAdvance=64;	//  slow animation
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

void AI_Pusher(Guy *me, Map *map, world_t *world, Guy *goodguy)
{
	if (me->reload)
		me->reload--;

	if(RangeToTarget(me, goodguy)>512*FIXAMT)
		return;

	if (map->CheckLOS(me->mapx, me->mapy, 512, goodguy->mapx, goodguy->mapy))
	{
		if (me->aiType == MONS_PUSHERH && abs(me->mapy-goodguy->mapy)<2)
		{
			if(me->x>goodguy->x) // push to the left
				goodguy->ax -= FIXAMT/2;
			else
				goodguy->ax += FIXAMT/2;
			Clamp(&goodguy->ax, FIXAMT*6);
		}
		else if(abs(me->mapx-goodguy->mapx)<2)
		{
			if(me->y>goodguy->y) // push to the north
				goodguy->ay -= FIXAMT/2;
			else
				goodguy->ay += FIXAMT/2;
			Clamp(&goodguy->ay, FIXAMT*6);
		}
	}
}

void AI_SubClone(Guy *me, Map *map, world_t *world, Guy *goodguy)
{
	int x, y, x2, y2, f;

	if (me->reload)
		me->reload--;

	if (me->ouch == 4)
	{
		if (me->hp > 0)
			MakeSound(SND_ROBOBOUAPHAOUCH, me->x, me->y, SND_CUTOFF, 1200);
	}

	if (me->action == ACTION_BUSY)
	{
		if (me->seq == ANIM_DIE)
		{
			x = me->x >> FIXSHIFT;
			y = me->y >> FIXSHIFT;
			BlowUpGuy(x + me->rectx, y + me->recty, x + me->rectx2, y + me->recty2, me->z, 2);
			BlowSmoke((x + me->rectx + Random(me->rectx2 - me->rectx)) << FIXSHIFT,
				(y + me->recty + Random(me->recty2 - me->recty)) << FIXSHIFT,
				me->z, FIXAMT);
			me->facing = (me->facing+1)&7;
			if(me->frm == 4)
			{
				// become an evil clone...
				FireBullet(me->x, me->y, 0, BLT_BOOM, me->friendly);
				me->type = MONS_EVILCLONE;	// become a lesser clone
				me->aiType = MONS_EVILCLONE;
				strcpy(me->name, monsType[me->aiType].name);
				me->hp = 32;
				me->maxHP = 32;
				me->mind = 0;
				me->mind1 = 0;
				me->facing = 2;
			}
		}
		return; // can't do nothin' right now
	}

	if (me->mind == 0) // when mind=0, singlemindedly lumber towards Bouapha
	{
		if (me->mind2 == 1)
		{
			me->mind3--;
			if (me->reload == 0 && goodguy)
			{
				MakeSound(SND_SUBSHOOT, me->x, me->y, SND_CUTOFF, 1200);
				f = me->facing * 32 - 64;
				x = me->x + Cosine(me->facing * 32) * 24;
				y = me->y + Sine(me->facing * 32) * 24;

				x2 = x + Cosine(f) * 8;
				y2 = y + Sine(f) * 8;
				FireBullet(x2, y2, me->facing, BLT_TORPEDO, me->friendly);
				x2 = x - Cosine(f) * 8;
				y2 = y - Sine(f) * 8;
				FireBullet(x2, y2, me->facing, BLT_TORPEDO, me->friendly);
				me->reload = 4;
			}
			if (!me->mind3)
				me->mind2 = 0;
		}
		else if (me->mind2 == 2)
		{
			me->mind3--;
			if(me->reload == 0 && goodguy)
			{
				MakeSound(SND_SUBMINE, me->x, me->y, SND_CUTOFF, 1200);
				x = me->x + Cosine((me->facing * 32 + 128) & 255) * 24;
				y = me->y + Sine((me->facing * 32 + 128) & 255) * 24;

				FireBullet(x, y, me->facing, BLT_MINE, me->friendly);
				me->reload = 30;
				if(Random(3)==0)
					me->mind1 = 1;
			}
			if (!me->mind3)
				me->mind2 = 0;
		}
		else if(me->mind2 == 0)
		{
			if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(32) == 0) // torpedos
			{
				// spit at him
				me->mind2 = 1;
				me->mind3 = Random(3)*8 + 8;
				me->reload = 0;
				FaceGoodguy(me, goodguy);
				return;
			}
			else if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(64) == 0) // mine
			{
				me->mind2 = 2;
				me->mind3 = Random(6) * 8 + 8;
				me->reload = 0;
				FaceGoodguy(me, goodguy);
				return;
			}
			FaceGoodguy(me, goodguy);

			me->dx = Cosine(me->facing * 32)*4;
			me->dy = Sine(me->facing * 32)*4;
			if (me->seq != ANIM_MOVE)
			{
				me->seq = ANIM_MOVE;
				me->frm = 0;
				me->frmTimer = 0;
				me->frmAdvance = 128;
			}
			if (Random(64) == 0)
			{
				me->mind = 1; // occasionally wander
				me->mind1 = 1;
			}
		}
	}
	else if (me->mind == 1) // random wandering
	{
		if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(32) == 0)
		{
			// spit at him
			MakeSound(SND_HAMMERTOSS, me->x, me->y, SND_CUTOFF, 1200);
			me->seq = ANIM_ATTACK;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 256;
			me->action = ACTION_BUSY;
			me->dx = 0;
			me->dy = 0;
			me->reload = 0;
			FaceGoodguy(me, goodguy);
			return;
		}
		else if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(128) == 0)
		{
			// spit at him
			me->seq = ANIM_A1;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 256;
			me->action = ACTION_BUSY;
			me->dx = 0;
			me->dy = 0;
			me->reload = 0;
			FaceGoodguy(me, goodguy);
			return;
		}

		if (!(me->mind1--)) // time to get a new direction
		{
			if ((goodguy) && Random(3) == 0)
				me->mind = 0; // get back on track
			else
				me->facing = (byte) Random(8);
			me->mind1 = Random(40) + 1;
		}

		me->dx = Cosine(me->facing * 32)*5;
		me->dy = Sine(me->facing * 32)*5;
		if (me->seq != ANIM_MOVE)
		{
			me->seq = ANIM_MOVE;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 128;
		}
	}
}

void AI_CaptainSeal(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte f;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SEALOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SEALDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frmTimer<32 && me->frm==3)
		{
			f=me->facing*32;
			x=me->x+Cosine(f)*16;
			y=me->y+Sine(f)*16;
			FireBullet(x,y,me->facing,BLT_ROCKET,me->friendly);
			MakeSound(SND_FRANKMISSILE,x,y,SND_CUTOFF,1050);
			me->reload=10;
		}
		if(me->seq==ANIM_DIE && me->frm>=4)
		{
			for(f=0;f<8;f++)
				SpecialSnow(me->x+((-32+Random(65))<<FIXSHIFT),
							me->y+((-32+Random(65))<<FIXSHIFT));
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(Random(64)==0 && me->reload==0 && RangeToTarget(me,goodguy)<700*FIXAMT)
			{
				// get him!
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
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(128)==0)
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

	if(me->mind==0)		// when mind=0, singlemindedly roll towards the nearest ally
	{
		if(me->target)
		{
			FaceGoodguy3(me,me->target);
			if(RangeToTarget(me,me->target)<(512*FIXAMT) && Random(72)==0)
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
		else
		{
			me->target = GetGuy(LockOn3(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 512, me->friendly));
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(me->target)
		{
			if(RangeToTarget(me,me->target)<(512*FIXAMT) && Random(256)==0)
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
			if((me->target) && Random(3)==0)
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

void AI_EmpressHead(Guy *me,Map *map,world_t *world,Guy *goodguy) {

	int x,y,tx,i;
	byte f;
	bullet_t *b;

	me->reload--;

	if (goodguy)
		tx=goodguy->x;

	BasicAI(me,SND_LOOKEYOUCH,SND_EMPRESSHEAD,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==12 && me->parent && me->frmTimer < 32)
			{
				me->parent->mind = 1; // Body is now vulnerable.
				me->parent->reload=15;
			}
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
			ShakeScreen(2,2);
			return;
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm==5 && me->frmTimer < 32)
			{
				MakeSound(SND_ALIENSPIT, me->x, me->y, SND_CUTOFF, 1200);
				// spit acid glob
				x=me->x+Cosine(me->facing*32)*20-FIXAMT*4+(Random(9)<<FIXSHIFT);
				y=me->y+Sine(me->facing*32)*20-FIXAMT*4+(Random(9)<<FIXSHIFT);
				f=2*32-16+Random(33);

				b=GetFireBullet(x,y,f,BLT_QUEENACID,me->friendly);
			}
			else if(me->frm==9)
			{
				me->reload = 20 + Random(40);
			}
		}
	}

	if(!me->reload)
	{
		me->action = ACTION_BUSY;
		me->seq = ANIM_ATTACK;
		me->frm = 0;
		me->frmTimer = 0;
		me->frmAdvance = 128;
	}

	if (me->parent)
	{
		int parx = me->parent->x;
		int len = monsType[me->parent->type].size/2;
		if (me->x > parx+len*FIXAMT)
		{
			tx = parx + len * FIXAMT;
		}
		if (me->x < parx-len*FIXAMT)
		{
			tx = parx - len * FIXAMT;
		}
		me->z = me->parent->z + abs(me->x-parx);
	}

	if(tx!=NULL)
	{
		if (tx < me->x)
			me->dx -= FIXAMT / 4;
		else
			me->dx += FIXAMT / 4;
	}

	Clamp(&me->dx, FIXAMT * 2);
	me->x += me->dx;
}

void AI_EmpressBody(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	bullet_t *b;
	Guy *g;

	me->reload--;
	me->mind1--;

	BasicAI(me,SND_LOOKEYOUCH,SND_LOOKEYDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->frm==0 && me->frmTimer<32)
			KillKids(me);
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
			ShakeScreen(2,4);
		}
		return;
	}

	if (me->mind == 1)
	{
		if(me->reload==15)
		{
			MakeSound(SND_MEGABEAM, me->x, me->y, SND_CUTOFF, 1200);
		}
		else if(!me->reload)
		{
			b=GetFireBullet(me->x,me->y+32*FIXAMT,2*32,BLT_LASERBEAM, me->friendly);
			if(b)
				RecolorBullet(b,4,1);
			me->reload=120;
		}
		if(!me->mind1)
		{
			me->mind1=Random(150)+100;
			MakeSound(SND_EMPRESSSPWN,me->x,me->y,SND_CUTOFF,1200);
			if (Random(2) == 0)
			{
				g=AddGuy(me->x+50*FIXAMT,me->y,0,MONS_ALIEN,me->friendly);
				if(g)
				{
					g->facing=0;
					g->seq=ANIM_MOVE;
					ExplodeParticles(PART_SLIME, me->x, me->y, me->z, 24);
				}
			}
			else
			{
				g=AddGuy(me->x-50*FIXAMT,me->y,0,MONS_ALIEN,me->friendly);
				if(g)
				{
					g->facing=4;
					g->seq=ANIM_MOVE;
					ExplodeParticles(PART_SLIME, me->x, me->y, me->z, 24);
				}
			}
		}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_EmpressTail(Guy* me, Map* map, world_t* world, Guy* goodguy)
{
	int x,y,z,i;
	byte f,j;
	bullet_t *b;
	if (me->reload)
		me->reload--;

	if (me->action == ACTION_BUSY)
	{
		if(me->frm==8 && me->frmTimer < 32)
		{
			j=Random(3)==0?2:1;
			for(i=0;i<j;i++)
			{
				x=me->x;
				y=me->y+16*FIXAMT;
				f=48+Random(33);
				b=GetFireBullet(x,y,f,BLT_ALIENEGG,me->friendly);
				if(b)
				{
					b->z = me->z+80*FIXAMT;
				}
			}
			me->reload=60+Random(120);
		}
		return;
	}

	if(!me->reload)
	{
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
	else if(me->seq!=ANIM_MOVE)
	{
		me->seq = ANIM_MOVE;
		me->frm = 0;
		me->frmTimer = 0;
		me->frmAdvance = 128;
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
					b=GetFireBullet(x,y,(me->facing-1+i)&7, BLT_YELWAVE, me->friendly);
					if(b)
					{
						RecolorBullet(b,5,GetBaseColor(me));
					}
				}
			}
			else
			{
				FireBullet(x,y, me->facing, BLT_YELWAVE, me->friendly);
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
