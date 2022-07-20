#include "winpch.h"
#include "bullet.h"
#include "guy.h"
#include "player.h"
#include "scanner.h"
#include "editor.h"
#include "shop.h"
#include "config.h"

#define SPR_FLAME		0
#define SPR_LASER		5
#define SPR_HAMMER		21
#define SPR_MISSILE		149
#define SPR_SMOKE		165
#define SPR_ACID		172
#define SPR_BOMB		228
#define SPR_ENERGY		236
#define SPR_BOOM		238
#define SPR_MEGABEAM	246
#define SPR_SPORE		254
#define SPR_SHROOM		258
#define SPR_GRENADE		266
#define SPR_YELBOOM		268
#define SPR_SHOCKWAVE	273
#define SPR_LILBOOM		277
#define SPR_SNOWBALL	282
#define SPR_BIGSNOW		283
#define SPR_ICESPIKE	286
#define SPR_ROCK		290
#define SPR_SPINE		294
#define SPR_BIGAXE		310
#define SPR_SPEAR		318
#define SPR_SLASH		326
#define SPR_MINE		350
#define SPR_STINKY		355
#define SPR_GREEN		358
#define SPR_ORBITER		359
#define SPR_PAPER		367
#define SPR_BUBBLE		375
#define SPR_SCANSHOT	384
#define SPR_SCANLOCK	387

// Kid Mystic additions
#define SPR_MAGICMISSILE 388
#define SPR_FIREBALL	404
#define SPR_WAVE		444
#define SPR_LIQUIFY		452
#define SPR_ICEBEAM		459
#define SPR_SKULL		464
#define SPR_SLIME		480
#define SPR_COMET		492
#define SPR_COMETBOOM	500
#define SPR_COIN		509
#define SPR_BIGCOIN		517

// Loonyland additions
#define SPR_FLAMEWALL	549
#define SPR_GASCLOUD	553
#define SPR_REDBONE		559
#define SPR_EARTHSPIKE	561
#define SPR_MEGABOOM	569
#define SPR_MEGABOOM	569
#define SPR_CACTUS		579
#define SPR_BOOMERANG	595
#define SPR_SLVRBULLET	611
#define SPR_BIGBOMBBOOM	627
#define SPR_BIGBOMB		638
#define SPR_ROCKET		645
#define SPR_BARREL		661
#define SPR_CHAIR		669
#define SPR_TABLE		677
#define SPR_DROPLET		685
#define SPR_DROPLET		685
#define SPR_CLAW		692
#define SPR_WHOOPEE		708
#define SPR_ORB			714

bullet_t *bullet;
sprite_set_t *bulletSpr;
byte reflect=0;
byte attackType;
int activeBulDX,activeBulDY;

void GetBulletDeltas(int *bdx,int *bdy)
{
	*bdx=activeBulDX;
	*bdy=activeBulDY;
}

void InitBullets(void)
{
	bulletSpr=new sprite_set_t("graphics/bullets.jsp");

	bullet=(bullet_t *)malloc(sizeof(bullet_t)*config.numBullets);
	memset(bullet,0,config.numBullets*sizeof(bullet_t));
}

void ExitBullets(void)
{
	free(bullet);
	delete bulletSpr;
}

byte Bulletable(byte type,Map *map,int x,int y)
{
	mapTile_t *tile;

	tile=map->GetTile(x,y);
	if(tile->wall==65535)
		return 0;

	if(tile->wall ||
		!BulletHitItem(type,tile,x,y))
	{
		if(tile->wall)
		{
			if(curWorld.terrain[tile->wall].flags&TF_DESTRUCT)
				tile->wall=curWorld.terrain[tile->wall].next;
		}
		if(curWorld.terrain[tile->floor].flags&TF_DESTRUCT)
			tile->floor=curWorld.terrain[tile->floor].next;
		return 0;
	}

	return 1;
}

void OffScreenBulletDie(bullet_t *me,Map *map)
{
	if(me->x<0 || me->y<0 || me->x>=(map->width*TILE_WIDTH*FIXAMT) ||
		me->y>=(map->height*TILE_HEIGHT*FIXAMT))
		me->type=BLT_NONE;
}

byte BulletCanGo(byte type,int xx,int yy,Map *map,byte size,byte friendly)
{
	byte result;
	int mapx,mapy,mapx1,mapx2,mapy1,mapy2;

	if(xx<0 || yy<0)
		return 0;

	xx>>=FIXSHIFT;
	yy>>=FIXSHIFT;

	mapx=xx/TILE_WIDTH;
	mapy=yy/TILE_HEIGHT;
	mapx1=(xx-size)/TILE_WIDTH;
	mapy1=(yy-size)/TILE_HEIGHT;
	mapx2=(xx+size)/TILE_WIDTH;
	mapy2=(yy+size)/TILE_HEIGHT;

	result=(mapx1>=0 && mapy1>=0 && mapx2<map->width && mapy2<map->height &&
		(Bulletable(type,map,mapx,mapy1)) &&
		(Bulletable(type,map,mapx,mapy2)) &&
		(Bulletable(type,map,mapx1,mapy)) &&
		(Bulletable(type,map,mapx2,mapy)) &&
		(Bulletable(type,map,mapx,mapy)) &&
		(Bulletable(type,map,mapx1,mapy1)) &&
		(Bulletable(type,map,mapx2,mapy1)) &&
		(Bulletable(type,map,mapx2,mapy2)) &&
		(Bulletable(type,map,mapx1,mapy2)));

	if(!result)
		EventOccur(EVT_SHOOT,friendly,mapx,mapy,NULL);

	return result;
}

void LaserMirrorHit(bullet_t* me, Map* map, world_t* world)
{
	short mapx, mapy;
	char n[2];

	mapx = me->x / (TILE_WIDTH * FIXAMT);
	mapy = me->y / (TILE_HEIGHT * FIXAMT);

	if (mapx == (me->target % 256) && mapy == (me->target / 256))	// you've already hit/not hit this space
		return;

	if (mapx < 0 || mapy < 0 || mapx >= map->width || mapy >= map->height)
	{
		me->type = BLT_NONE;
		return;
	}

	if (me->dx < 0)
	{
		me->dx = me->dx;
	}
	n[0] = GetItem(map->map[mapx + mapy * map->width].item)->name[0];
	n[1] = GetItem(map->map[mapx + mapy * map->width].item)->name[1];
	if (n[0] == '@')	// mirror items start with @
	{
		if (me->dx > 0)	// heading right
		{
			if (n[1] == '1' || n[1] == '3')
			{
				me->type = BLT_NONE;
				ExplodeParticlesColor(5, me->x, me->y, me->z, 1, 2);
				return;	// blocked
			}
			else if (n[1] == '0')
			{
				me->dx = 0;
				me->dy = -FIXAMT * 24;
				me->facing = 6 * 32;
			}
			else if (n[1] == '2')
			{
				me->dx = 0;
				me->dy = FIXAMT * 24;
				me->facing = 2 * 32;
			}
		}
		else if (me->dx < 0)	// heading left
		{
			if (n[1] == '0' || n[1] == '2')
			{
				me->type = BLT_NONE;
				ExplodeParticlesColor(4, me->x, me->y, me->z, 1, 2);
				return;	// blocked
			}
			else if (n[1] == '1')
			{
				me->dx = 0;
				me->dy = -FIXAMT * 24;
				me->facing = 6 * 32;
			}
			else if (n[1] == '3')
			{
				me->dx = 0;
				me->dy = FIXAMT * 24;
				me->facing = 2 * 32;
			}
		}
		else if (me->dy < 0)	// heading up
		{
			if (n[1] == '0' || n[1] == '1')
			{
				me->type = BLT_NONE;
				ExplodeParticlesColor(4, me->x, me->y, me->z, 1, 2);
				return;	// blocked
			}
			else if (n[1] == '2')
			{
				me->dx = -FIXAMT * 32;
				me->dy = 0;
				me->facing = 4 * 32;
			}
			else if (n[1] == '3')
			{
				me->dx = FIXAMT * 32;
				me->dy = 0;
				me->facing = 0 * 32;
			}
		}
		else if (me->dy > 0)	// heading down
		{
			if (n[1] == '2' || n[1] == '3')
			{
				me->type = BLT_NONE;
				ExplodeParticlesColor(4, me->x, me->y, me->z, 1, 2);
				return;	// blocked
			}
			else if (n[1] == '0')
			{
				me->dx = -FIXAMT * 32;
				me->dy = 0;
				me->facing = 4 * 32;
			}
			else if (n[1] == '1')
			{
				me->dx = FIXAMT * 32;
				me->dy = 0;
				me->facing = 0 * 32;
			}
		}
		me->target = (mapx + mapy * 256);
		me->x = (mapx * TILE_WIDTH + TILE_WIDTH / 2) * FIXAMT;
		me->y = (mapy * TILE_HEIGHT + TILE_HEIGHT / 2) * FIXAMT;
	}
	else
	{
		ExplodeParticlesColor(4, me->x, me->y, me->z, 1, 2);
		me->type = BLT_NONE;
	}
}

// Just a nice litle way to set bullets to type 0
void Remove(bullet_t *me)
{
	me->type = BLT_NONE;
}

// Do this upon "death"
void Splat(bullet_t* me)
{
	switch (me->type)
	{
		case BLT_HAMMER:
		case BLT_BIGSHELL:
		case BLT_LUNA:
			ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 8);
			MakeSound(SND_HAMMERBONK, me->x, me->y, SND_CUTOFF, 1000);
			Remove(me);
			break;
		case BLT_FIREBALL:
			ExplodeParticles(PART_YELLOW, me->x, me->y, me->z, 8);
			MakeSound(SND_HAMMERBONK, me->x, me->y, SND_CUTOFF, 1000);
			Remove(me);
			break;
		case BLT_HAMMER2:
		case BLT_EVILHAMMER:
		case BLT_LUNA2:
		case BLT_FIREBALL2:
		case BLT_SKULL:
		case BLT_BADLUNA:
		case BLT_BADFBALL:
		case BLT_BADLUNA2:
			MakeSound(SND_HAMMERBONK, me->x, me->y, SND_CUTOFF, 1000);
			Remove(me);
			break;
		case BLT_MINIFBALL:
		case BLT_YELWAVE:
			ExplodeParticles(PART_YELLOW, me->x, me->y, me->z, 8);
			Remove(me);
			break;
		case BLT_ACID:
		case BLT_QUEENACID:
		case BLT_SWAP:
			MakeSound(SND_ACIDSPLAT, me->x, me->y, SND_CUTOFF, 950);
			ExplodeParticles(PART_SLIME, me->x, me->y, me->z, 6);
			Remove(me);
			break;
		case BLT_SHARK:
			MakeSound(SND_ACIDSPLAT, me->x, me->y, SND_CUTOFF, 950);
			ExplodeParticles(PART_WATER, me->x, me->y, me->z, 6);
			Remove(me);
			break;
		case BLT_ENERGY:
			MakeSound(SND_ENERGYBONK, me->x, me->y, SND_CUTOFF, 950);
			ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 4);
			Remove(me);
			break;
		case BLT_ENERGY2:
			MakeSound(SND_ENERGYBONK, me->x, me->y, SND_CUTOFF, 950);
			ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 6);
			Remove(me);
			break;
		case BLT_GREEN:
		case BLT_BADGREEN:
			MakeSound(SND_ENERGYBONK, me->x, me->y, SND_CUTOFF, 950);
			ExplodeParticles(PART_SLIME, me->x, me->y, me->z, 4);
			Remove(me);
			break;
		case BLT_SLASH:
			ExplodeParticles2(PART_SNOW2, me->x, me->y, me->z, 4, 8);
			Remove(me);
			break;
		case BLT_BIGSHOT:
		case BLT_FLAMEWALL:
		case BLT_FLAMESHOT:
		case BLT_BATSHOT:
		case BLT_WOLFSHOT:
			ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 4);
			Remove(me);
			break;
		case BLT_CLAW:
			ExplodeParticles(PART_SNOW2, me->x, me->y, me->z, 7);
			Remove(me);
			break;
		case BLT_EARTHSPIKE:
		case BLT_EARTHSPIKE2:
			ExplodeParticles(PART_DIRT, me->x, me->y, me->z, 8);
			Remove(me);
			break;
		case BLT_PAPER:
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,6,12);
			Remove(me);
			break;
		case BLT_BIGAXE:
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,10,8);
			Remove(me);
			break;
		case BLT_IGNITE:
		case BLT_WIND:
			ExplodeParticles2(PART_SNOW2, me->x, me->y, me->z, 10, 8);
			Remove(me);
			break;
		case BLT_SNOWBALL:
		case BLT_FREEZE:
		case BLT_FREEZE2:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			Remove(me);
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,6,12);
			break;
		case BLT_BUBBLE:
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			me->type=BLT_BUBBLEPOP;
			me->anim=0;
			me->timer=10;
			me->dx=0;
			me->dy=0;
			MakeSound(SND_BUBBLEPOP,me->x,me->y,SND_CUTOFF,1);
			break;
		case BLT_SCANNER:
		case BLT_SCANSHOT:
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			Remove(me);
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
			Remove(me);
			BlowSmoke(me->x, me->y, me->z, FIXAMT);
			break;
		case BLT_BALLLIGHTNING:
		case BLT_BADLIGHTNING:
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
			Remove(me);
			break;
		case BLT_BOOMERANG:
			ExplodeParticles(PART_DIRT,me->x,me->y,me->z,8);
			Remove(me);
			break;
		case BLT_ICEWAND:
		case BLT_ICEWAND2:
		case BLT_ICESHARD:
		case BLT_ICEWOLFICE:
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
			Remove(me);
			break;
		case BLT_WATER:
			if(Random(3)==0)
				MakeSound(SND_WATERSPLASH,me->x,me->y,SND_CUTOFF,200);
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,8);
			Remove(me);
			break;
		default:
			Remove(me);
			break;
	}
}

// Used for both wall hit functions.
void InteractWithWall(bullet_t *me, Map* map, world_t* world)
{
	switch (me->type)
	{
		case BLT_HAMMER:
		case BLT_BIGSHELL:
		case BLT_LUNA:
		case BLT_FIREBALL:
		case BLT_SKULL:
		case BLT_MINIFBALL:
		case BLT_YELWAVE:
		case BLT_ACID:
		case BLT_QUEENACID:
		case BLT_SHARK:
		case BLT_ENERGY:
		case BLT_ENERGY2:
		case BLT_GREEN:
		case BLT_BADGREEN:
		case BLT_SLASH:
		case BLT_BIGSHOT:
		case BLT_FLAMEWALL:
		case BLT_FLAMESHOT:
		case BLT_BATSHOT:
		case BLT_WOLFSHOT:
		case BLT_CLAW:
		case BLT_EARTHSPIKE:
		case BLT_EARTHSPIKE2:
			Splat(me);
			break;
		case BLT_MEGABEAM:
		case BLT_MEGABEAM1:
			me->type=BLT_MEGABEAM2;
			me->dy=0;
			me->timer=2;
			break;
		case BLT_LASER:	// reflects off walls
			MakeSound(SND_BULLETREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
		case BLT_LASERBEAM:	// reflects off mirrors
			LaserMirrorHit(me, map, world);
			break;
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
			me->dx = me->dy = 0;
			break;
		case BLT_HAMMER2:
		case BLT_LUNA2:
		case BLT_FIREBALL2:
			MakeSound(SND_HAMMERREFLECT, me->x, me->y, SND_CUTOFF, 600);
			break;
		case BLT_ROCK:
			MakeSound(SND_ROCKBOUNCE,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_ICEBEAM:
			me->timer = 8;
			me->type = BLT_ICEBEAM2;
			break;
		case BLT_DEATHBEAM:
			me->timer = 8;
			if (Random(3) == 0)
				me->type = BLT_DEATHBEAM2;
			else
				LaserMirrorHit(me,map,world);
			break;
		case BLT_GASBLAST:
			if(me->timer>6)
				me->timer=6;
			break;
		case BLT_CACTUS:
			me->target=0;
			me->dx=Cosine(me->facing)*16;
			me->dy=Sine(me->facing)*16;
			break;
		case BLT_BALLLIGHTNING:
		case BLT_BADLIGHTNING:
			me->dx=Cosine(me->facing)*11;
			me->dy=Sine(me->facing)*11;
			break;
		case BLT_CHEESEHAMMER:
			me->dx=Cosine(me->facing)*14;
			me->dy=Sine(me->facing)*14;
			MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
	}
}

// How does the bullet react to hitting a wall?
byte OnHitWall(bullet_t* me)
{
	switch(me->type)
	{
		case BLT_HAMMER2:
		case BLT_LASER:
		case BLT_BOMB:
		case BLT_EVILHAMMER:
		case BLT_ORBITER:
		case BLT_ORBITER2:
		case BLT_SWAP:
		case BLT_SCANSHOT:
		case BLT_CHEESEHAMMER:
		case BLT_LUNA2:
		case BLT_FIREBALL2:
		case BLT_COIN:
		case BLT_BIGCOIN:
		case BLT_BIGYELLOW:
		case BLT_SKULL:
		case BLT_BIGBOMB:
		case BLT_BOOMERANG:
		case BLT_BADLUNA:
		case BLT_BADLUNA2:
		case BLT_BADFBALL:
		case BLT_BADLIGHTNING:
		case BLT_QUEENACID:
		case BLT_ALIENEGG:
			return 1; // reflects
		case BLT_HAMMER:
		case BLT_BIGSHELL:
		case BLT_LUNA:
		case BLT_FIREBALL:
		case BLT_MINIFBALL:
		case BLT_YELWAVE:
		case BLT_ACID:
		case BLT_SHARK:
		case BLT_ENERGY:
		case BLT_ENERGY2:
		case BLT_GREEN:
		case BLT_BADGREEN:
		case BLT_SPORE:
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
		case BLT_BIGSHOT:
		case BLT_FLAMEWALL:
		case BLT_FLAMESHOT:
		case BLT_BATSHOT:
		case BLT_WOLFSHOT:
		case BLT_CLAW:
		case BLT_EARTHSPIKE:
		case BLT_EARTHSPIKE2:
		case BLT_SLASH:
			return 2; // breaks w/o running out
		case BLT_MISSILE:
		case BLT_MAGICMISSILE:
		case BLT_SPINE:
		case BLT_BIGAXE:
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_MINDWIPE:
		case BLT_REFLECT:
		case BLT_HARPOON:
		case BLT_SCANNER:
		case BLT_TORPEDO:
		case BLT_PAPER:
		case BLT_BUBBLE:
		case BLT_FREEZE:
		case BLT_FREEZE2:
		case BLT_SNOWBALL:
		case BLT_BIGSNOW:
		case BLT_ICEWAND:
		case BLT_ICEWAND2:
		case BLT_ICESHARD:
		case BLT_ICEWOLFICE:
		case BLT_WIND:
		case BLT_SHROOM:
			return 3; // runs out
		case BLT_GRENADE:
		case BLT_ICECLOUD:
		case BLT_HOLESHOT:
		case BLT_BLACKHOLE:
		case BLT_WHOOPEE:
		case BLT_HOTPANTS:
		case BLT_ORBGRENADE:
		case BLT_WATER:
		case BLT_FOUNTAIN:
		case BLT_CROAKERGAS:
		case BLT_FARLEYGAS:
		case BLT_CACTUS:
		case BLT_GASBLAST:
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
			return 4; // pushes back, and that's all
		default:
			return 0; // does nothing
		case BLT_BALLLIGHTNING:
			return (player.hammerFlags&HMR_REFLECT) ? 1 : 3; // conditional - must have hammer reflect
	}
}

void BulletHitWallX(bullet_t *me,Map *map,world_t *world)
{

	if(OnHitWall(me)==1)
	{
		me->x -= me->dx;
		me->dx = -me->dx;
		// Special facing sets.
		switch (me->type)
		{
			case BLT_SKULL:
				me->facing = ((byte)(8 - me->facing)) & 15; // 16 directions
				break;
			case BLT_LASER:
			case BLT_BOMB:
				me->dy+=-FIXAMT/4+Random(FIXAMT/2);
				me->facing = ((byte)(8 - me->facing)) & 15; // 16 directions
				break;
			case BLT_CHEESEHAMMER:
				if(me->facing<128)
					me->facing=128-me->facing;
				else
					me->facing=128+(256-me->facing);
				break;
			case BLT_HAMMER2:
			case BLT_FIREBALL2:
			case BLT_LUNA2:
			case BLT_EVILHAMMER:
			case BLT_BADLUNA:
			case BLT_BADFBALL:
			case BLT_ROCK:
			case BLT_COIN:
			case BLT_BIGCOIN:
			case BLT_BIGYELLOW:
			case BLT_ALIENEGG:
			case BLT_QUEENACID:
				me->facing = ((byte)(4 - me->facing)) & 7; // 8 directions
				break;
			case BLT_BALLLIGHTNING: // Lightning balls
			case BLT_BADLIGHTNING:
				me->facing = 84 + Random(89);
				if (me->dx < 0)
					me->facing = (me->facing + 128) & 255;
				break;
			case BLT_FLAME:
			case BLT_FLAME2:
			case BLT_FLAME3:
				me->dy=((3-Random(7))<<FIXSHIFT);
				Dampen(&me->dx,FIXAMT/2);
				break;
			case BLT_CACTUS:
				me->facing=(byte)(84+Random(89));
				if(me->dx<0)
					me->facing=(me->facing+128)&255;
				break;
		}
	}
	else if(OnHitWall(me)==4)
		me->x-=me->dx;
		
	InteractWithWall(me,map,world);

	if(OnHitWall(me)==2)
		Remove(me);
	else if(OnHitWall(me)==3)
		BulletRanOut(me,map,world);
}

void BulletHitWallY(bullet_t *me,Map *map,world_t *world)
{
	if(OnHitWall(me)==1)
	{
		me->y -= me->dy;
		me->dy = -me->dy;
		// Special facing sets.
		switch (me->type)
		{
			case BLT_SKULL:
				me->facing = (16 - me->facing) & 15;
				break;
			case BLT_LASER:
			case BLT_BOMB:
				me->dx+=-FIXAMT/4+Random(FIXAMT/2);
				me->facing = (16 - me->facing) & 15; // 16 directions
				break;
			case BLT_CHEESEHAMMER:
				if(me->facing<128)
					me->facing=128-me->facing;
				else
					me->facing=128+(256-me->facing);
				break;
			case BLT_HAMMER2:
			case BLT_FIREBALL2:
			case BLT_LUNA2:
			case BLT_EVILHAMMER:
			case BLT_BADLUNA:
			case BLT_BADFBALL:
			case BLT_ROCK:
			case BLT_COIN:
			case BLT_BIGCOIN:
			case BLT_BIGYELLOW:
			case BLT_ALIENEGG:
			case BLT_QUEENACID:
				me->facing = (8 - me->facing)&7; // 8 directions
				break;
			case BLT_BALLLIGHTNING: // Lightning balls
			case BLT_BADLIGHTNING:
				me->facing = 20 + Random(89);
				if (me->dy > 0)
					me->facing += 128;
				break;
			case BLT_FLAME:
			case BLT_FLAME2:
			case BLT_FLAME3:
				me->dx=((3-Random(7))<<FIXSHIFT);
				Dampen(&me->dy,FIXAMT/2);
				break;
			case BLT_CACTUS:
				me->facing=(byte)(20+Random(89));
				if(me->dy>0)
					me->facing+=128;
				break;
		}
	}
	else if(OnHitWall(me)==4)
		me->y-=me->dy;

	InteractWithWall(me,map,world);

	if(OnHitWall(me)==2)
		Remove(me);
	else if(OnHitWall(me)==3)
		BulletRanOut(me,map,world);
}

void CheckForWater(bullet_t *me,Map *map,world_t *world)
{
	int x,y;
	x = (me->x >> FIXSHIFT) / TILE_WIDTH;
	y = (me->y >> FIXSHIFT) / TILE_HEIGHT;
	if (GetTerrain(world, map->GetTile(x, y)->floor)->flags & TF_WATER)
	{
		ExplodeParticles(PART_WATER, me->x, me->y, 0, 8);
		switch (me->type)
		{
			case BLT_COIN:
			case BLT_BIGCOIN:
				Remove(me);
				break;
		}
	}
	else if (GetTerrain(world, map->GetTile(x, y)->floor)->flags & TF_LAVA)
	{
		switch(me->type)
		{
			case BLT_FIREBALL:
			case BLT_FIREBALL2:
			case BLT_BADFBALL:	// reflects off walls
				ExplodeParticles(PART_YELLOW, me->x, me->y, 0, 8);
				break;
			case BLT_COIN:
			case BLT_BIGCOIN:
				for(int i=0;i<3;i++)
					BlowWigglySmoke(me->x, me->y, 0, FIXAMT * 2+i*2);
				Remove(me);
				break;
			default:
				ExplodeParticles(PART_HAMMER, me->x, me->y, 0, 8);
				break;
		}
	}
}

void Dribble(bullet_t* me)
{
	me->z=0;
	switch (me->type)
	{
		default:
			me->dz=-me->dz*3/4;
			break;
		case BLT_COIN:
		case BLT_BIGCOIN:
			me->dz=-me->dz*7/8;
			break;
	}
	if (me->dz < -FIXAMT) // Check if the sound needs doing!
	{
		switch(me->type)
		{
			case BLT_HAMMER:
			case BLT_HAMMER2:
			case BLT_EVILHAMMER:
			case BLT_LUNA:
			case BLT_LUNA2:
			case BLT_BADLUNA:
			case BLT_BADLUNA2:
			case BLT_FIREBALL:
			case BLT_FIREBALL2:
			case BLT_BADFBALL:
				MakeSound(SND_HAMMERREFLECT, me->x, me->y, SND_CUTOFF, 850);
				break;
			case BLT_BOMB:
				MakeSound(SND_BOMBREFLECT, me->x, me->y, SND_CUTOFF, 600);
				break;
			case BLT_ROCK:
				MakeSound(SND_ROCKBOUNCE, me->x, me->y, SND_CUTOFF, 600);
				break;
		}
	}
}

void BulletHitFloor(bullet_t *me,Map *map,world_t *world)
{
	int x,y;

	switch(me->type)
	{
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dx,FIXAMT/8);
			me->z=0;
			me->dz=0;
			break;
		case BLT_COMET:
			me->type=BLT_COMETBOOM;
			me->anim=0;
			me->timer=18;	// 9 frames of animation
			me->dz=0;
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,200);
			break;
		case BLT_COMETBOOM:
			break;
		case BLT_BIGSHELL:
			BulletRanOut(me,map,world);
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_EVILHAMMER:
		case BLT_LUNA:
		case BLT_LUNA2:
		case BLT_BADLUNA:
		case BLT_BADLUNA2:
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
		case BLT_BADFBALL:
		case BLT_BOMB:
		case BLT_MINDWIPE:
		case BLT_BUBBLE:
		case BLT_ROCK:
			MakeSound(SND_HAMMERREFLECT, me->x, me->y, SND_CUTOFF, 850);
			Dribble(me);
			CheckForWater(me, map, world);
			break;
		case BLT_COIN:
		case BLT_BIGCOIN:
			Dribble(me);
			CheckForWater(me, map, world);
			break;
		case BLT_CHEESEHAMMER:
		case BLT_MISSILE: // this really really should never happen
		case BLT_MAGICMISSILE:
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
		case BLT_LASER:
		case BLT_LASERBEAM:
		case BLT_MEGABEAM:
		case BLT_MEGABEAM1:
		case BLT_MEGABEAM2:
		case BLT_SPORE:
		case BLT_YELBOOM:
		case BLT_LILBOOM:
		case BLT_LILBOOM2:
		case BLT_SHOCKWAVE:
		case BLT_GOODSHOCK:
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
		case BLT_SPINE:
		case BLT_BIGAXE:
		case BLT_SLASH:
		case BLT_MINE:
		case BLT_ORBITER:
		case BLT_ORBITER2:
		case BLT_BALLLIGHTNING:
		case BLT_BADLIGHTNING:
		case BLT_REFLECT:
		case BLT_SWAP:
		case BLT_SCANNER:
		case BLT_SCANSHOT:
		case BLT_TORPEDO:
		case BLT_PAPER:
		case BLT_FREEZE:
		case BLT_IGNITE:
		case BLT_HOLESHOT:
		case BLT_BLACKHOLE:
		case BLT_BIGBOMB:
			me->z=0;
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_HARPOON:
		case BLT_FREEZE2:
			BulletRanOut(me,map,world);
			break;
		case BLT_ENERGY:
		case BLT_ENERGY2:
		case BLT_BIGSHOT:
		case BLT_GREEN:
		case BLT_BADGREEN:
		case BLT_ACID:
		case BLT_QUEENACID:
		case BLT_SHARK:
			Splat(me);
			break;
		case BLT_SHROOM:
		case BLT_ALIENEGG:
			// detonate
			BulletRanOut(me,map,world);
			break;
		case BLT_GRENADE:
			me->type=BLT_YELBOOM;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			me->anim=0;
			me->timer=9;
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,950);
			break;
		case BLT_SNOWBALL:
		case BLT_BIGSNOW:
			BulletRanOut(me,map,world);
			break;
		case BLT_FLAMEWALL:
		case BLT_EARTHSPIKE:
		case BLT_EARTHSPIKE2:
		case BLT_GASBLAST:
		case BLT_WHOOPEE:
		case BLT_MEGABOOM:
		case BLT_CACTUS:
		case BLT_ICEWAND:
		case BLT_ICEWAND2:
		case BLT_BOOMERANG:
		case BLT_HOTPANTS:
		case BLT_BATSHOT:
		case BLT_WIND:
		case BLT_ICESHARD:
		case BLT_FOUNTAIN:
		case BLT_FLAMESHOT:
		case BLT_WOLFSHOT:
		case BLT_CLAW:
		case BLT_CROAKERGAS:
		case BLT_HOTDOGFIRE:
		case BLT_ICEWOLFICE:
		case BLT_FARLEYGAS:
			// don't care about the floor
			break;
		case BLT_ORBGRENADE:
		case BLT_WATER:
			BulletRanOut(me, map, world);
			break;
		case BLT_EVILFACE:
			// can go through anything
			break;
	}
}

void BulletRanOut(bullet_t *me,Map *map,world_t *world)
{
	int i;
	byte a;
	bullet_t b;
	Guy *g;

	switch(me->type)
	{
		default: // most bullets use this
			Splat(me);
			break;
		case BLT_HOLESHOT:
			me->type = BLT_BLACKHOLE;
			me->dx = 0;
			me->dy = 0;
			me->timer = 30 * 2 + 50;
			me->anim = 0;
			break;
		case BLT_MINIFBALL:
		case BLT_YELWAVE:
			me->type = BLT_NONE;
			ExplodeParticles(PART_YELLOW, me->x, me->y, me->z, 2);
			break;
		case BLT_HAMMER2:
		case BLT_EVILHAMMER:
		case BLT_LUNA2:
		case BLT_BADLUNA2:
			ExplodeParticles2(PART_HAMMER, me->x, me->y, me->z, 6, 3);
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
		case BLT_BADFBALL:
		case BLT_BIGYELLOW:
		case BLT_SKULL:
			ExplodeParticles2(PART_YELLOW, me->x, me->y, me->z, 6, 3);
			me->type = 0;
			break;
		case BLT_BIGSNOW:
			// poof into snowballs
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			me->type=0;
			for(i=0;i<8;i++)
				FireBullet(me->x,me->y,(byte)i*32,BLT_SNOWBALL,me->friendly);
			break;
		case BLT_MISSILE:
		case BLT_MAGICMISSILE:
		case BLT_CHEESEHAMMER:
			MakeSound(SND_MISSILEBOOM,me->x,me->y,SND_CUTOFF,1500);
			me->type=BLT_LILBOOM;
			me->timer=9;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_TORPEDO:
			MakeSound(SND_MISSILEBOOM,me->x,me->y,SND_CUTOFF,1500);
			me->type=BLT_LILBOOM2;
			me->timer=9;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_ACID:
		case BLT_SWAP:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_QUEENACID:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			for(i=0;i<8;i++)
				FireBullet(me->x,me->y,(byte)i*32,BLT_ACID,me->friendly);
			break;
		case BLT_SCANNER:
		case BLT_SCANSHOT:
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_BOMB:
		case BLT_MINE:
		case BLT_ORBITER:
		case BLT_ORBITER2:
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,2000);
			me->dx=0;
			me->dy=0;
			me->dz=0;
			me->type=BLT_BOOM;
			me->timer=7;
			me->anim=0;
			break;
		case BLT_SHROOM:
			for(i=0;i<256;i+=8)
			{
				FireExactBullet(me->x,me->y,me->z,Cosine(i)*12,Sine(i)*12,0,0,16,i,BLT_SPORE,me->friendly);
				FireExactBullet(me->x,me->y,me->z,Cosine(i)*6,Sine(i)*6,0,0,16,i,BLT_SPORE,me->friendly);
			}
			g=AddGuy(me->x-me->dx*2,me->y-me->dy*2,me->z,MONS_SHROOM,me->friendly);	// become a living shroom
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);

			me->type=BLT_NONE;	// all gone
			MakeSound(SND_MISSILEBOOM,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_ALIENEGG:
			for(i=0;i<256;i+=8)
			{
				FireExactBullet(me->x,me->y,me->z,Cosine(i)*6,Sine(i)*6,0,0,16,i,BLT_ACID,me->friendly);
			}
			g=AddGuy(me->x-me->dx*2,me->y-me->dy*2,me->z,MONS_ALIENEGG,me->friendly);	// become a living shroom
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);

			me->type=BLT_NONE;	// all gone
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_ORBGRENADE:
			FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
			me->type=0;
			break;
		case BLT_ROCKET:
			FireBullet(me->x,me->y,0,BLT_BIGBOOM,me->friendly);
			me->type=0;
			break;
		case BLT_BIGBOMB:
			me->type = BLT_BIGBOOM;
			me->timer = 7;
			MakeSound(SND_BIGBOOM, me->x, me->y, SND_CUTOFF, 1200);
			break;
		case BLT_BADLUNA:
			if(Random(3)==0)
			{
				MakeSound(SND_BIGBOOM, me->x, me->y, SND_CUTOFF, 1200);
				ExplodeParticles(PART_LUNA, me->x, me->y, me->z, 8);
				a=5+Random(4);
				for(int i=0;i<a;i++)
				{
					FireBullet(me->x,me->y,256/a*i,BLT_ENERGY,me->friendly);
				}
			}
			me->type=BLT_NONE;
			break;
	}
}

void HitBadguys(bullet_t *me,Map *map,world_t *world)
{
	int i;
	byte b;
	bullet_t* you;

	attackType=me->type;
	switch(me->type)
	{
		case BLT_COMETBOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,64,(8-Random(17))<<FIXSHIFT,
				(8-Random(16))<<FIXSHIFT,10,map,world, me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_BLACKHOLE:
			if (FindVictims2(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 32, 0, 0, 1, map, world, me->friendly))
			{

			}
			break;
		case BLT_SCANNER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,0,map,world,me->friendly))
			{
				if(GetLastGuyHit()->aiType!=MONS_BOUAPHA)
				{
					ScanGuy(GetLastGuyHit());
					me->type=BLT_SCANLOCK;
					me->timer=60;
					me->target=GetLastGuyHit()->ID;
				}
			}
			else if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,0,map,world,1-me->friendly))
			{
				if(GetLastGuyHit()->aiType!=MONS_BOUAPHA)
				{
					ScanGuy(GetLastGuyHit());
					me->type=BLT_SCANLOCK;
					me->timer=60;
					me->target=GetLastGuyHit()->ID;
				}
			}
			break;
		case BLT_LASER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,1,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				MakeSound(SND_BULLETHIT,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_LASERBEAM:
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 8, 0, 0, 80, map, world, me->friendly))
			{
				ExplodeParticlesColor(7, me->x, me->y, me->z, 2, 5);
				MakeSound(SND_LIGHTNING, me->x, me->y, SND_CUTOFF, 900);
			}
			if (me->timer < 30 * 10 - 2 && FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 8, 0, 0, 80, map, world, 1 - me->friendly))
			{
				ExplodeParticlesColor(7, me->x, me->y, me->z, 2, 5);
				MakeSound(SND_LIGHTNING, me->x, me->y, SND_CUTOFF, 900);
			}
			break;
		case BLT_ICECLOUD:
			if (FindVictims(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 20, 0, 0, -1000, map, world, me->friendly))
			{
				ExplodeParticles(PART_SNOW2, me->x, me->y, me->z, 4);
				MakeSound(SND_FREEZE, me->x, me->y, SND_CUTOFF, 900);
			}
			break;
		case BLT_ICEBEAM:
			if (FindVictims(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 20, 0, 0, -1000, map, world, me->friendly))
			{
				ExplodeParticles(PART_SNOW2, me->x, me->y, me->z, 4);
				MakeSound(SND_FREEZE, me->x, me->y, SND_CUTOFF, 900);
			}
			break;
		case BLT_DEATHBEAM:
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 12, 0, 0, 5, map, world, me->friendly))
			{
				ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 4);
				MakeSound(SND_MEGABEAMHIT, me->x, me->y, SND_CUTOFF, 900);
				me->type = BLT_DEATHBEAM2;
				me->timer = 8;
			}
			break;
		case BLT_GREEN:
		case BLT_BADGREEN:
		case BLT_SCANSHOT:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,3,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_SLIME,me->x,me->y,me->z,8);
				MakeSound(SND_BULLETHIT,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_BALLLIGHTNING:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
				MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,900);
				for(i=0;i<player.hammers-1;i++)
					FireBullet(me->x,me->y,0,BLT_LIGHTNING2,me->friendly);
				me->type=BLT_NONE;
			}
			break;
		case BLT_BADLIGHTNING:
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 12, me->dx, me->dy, 5, map, world, me->friendly))
			{
				me->type = BLT_NONE;
				ExplodeParticles(PART_SNOW2, me->x, me->y, me->z, 8);
				MakeSound(SND_LIGHTNING, me->x, me->y, SND_CUTOFF, 900);
				FireBullet(me->x, me->y, 0, BLT_LIGHTNING2, me->friendly);
				me->type = BLT_NONE;
			}
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
		case BLT_SKULL:
		case BLT_LUNA:
		case BLT_LUNA2:
		case BLT_BADLUNA:
		case BLT_BADLUNA2:
		case BLT_BIGSHELL:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,5,map,world,me->friendly))
			{
				if(!reflect)
				{
					me->type=BLT_NONE;
					ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
					MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,900);
				}
				else
				{
					me->friendly=1-me->friendly;
					switch (me->type) {
						case BLT_HAMMER:
						case BLT_HAMMER2:
						default:
							me->type = BLT_EVILHAMMER;
							break;
						case BLT_LUNA:
						case BLT_LUNA2:
							me->type = BLT_BADLUNA;
							break;
						case BLT_FIREBALL:
						case BLT_FIREBALL2:
							me->type = BLT_BADFBALL;
							break;
						case BLT_BALLLIGHTNING:
							me->type = BLT_BADLIGHTNING;
							break;
					}
					me->facing=(me->facing+4)&7;
					me->dx=Cosine(me->facing*32)*12;
					me->dy=Sine(me->facing*32)*12;
					me->timer+=10;
				}
			}
			break;
		case BLT_CHEESEHAMMER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,5,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_PAPER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,5,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_SPEAR:
		case BLT_HARPOON:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,5,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_BADSPEAR:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,3,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_IGNITE:
			BurnHay(me->x, me->y);
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 16, 0, 0, 0, map, world, me->friendly))
			{
				if (!reflect)
				{
					GetLastGuyHit()->ignited = 330;
					me->type = BLT_NONE;
					ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 8);
					MakeSound(SND_FLAMEGO, me->x, me->y, SND_CUTOFF, 900);
				}
				else
				{
					me->friendly = 1 - me->friendly;
					me->target = 65535;
					me->dx = -me->dx;
					me->dy = -me->dy;
				}
			}
			break;
		case BLT_MINIFBALL:
			if (FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,6,me->dx,me->dy,3,map,world,me->friendly))
			{
				me->type = BLT_NONE;
				ExplodeParticles(PART_YELLOW, me->x, me->y, me->z, 4);
				MakeSound(SND_HAMMERBONK, me->x, me->y, SND_CUTOFF, 900);
			}
			break;
		case BLT_MISSILE:
		case BLT_MAGICMISSILE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,8,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_COIN:
		case BLT_BIGCOIN: // Work in progress - adds 1/5 to coins when obtained
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 8, 0, 0, 0, map, world, me->friendly))
			{
				if (me->type == BLT_COIN)
				{
					//FloaterParticles(me->x, me->y, 5, 24, 0, 4);
					player.coins++;
				}
				else
				{
					//FloaterParticles(me->x, me->y, 5, 24, 0, 4);
					//FloaterParticles(me->x, me->y, 5, 32, -1, 4);
					// double noise
					MakeSound(SND_MONEY, me->x, me->y, SND_CUTOFF, 500);
					player.coins+=5;
				}
				Remove(me);
				MakeSound(SND_MONEY, me->x, me->y, SND_CUTOFF, 500);
			}
			break;
		case BLT_TORPEDO:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,2,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_LILBOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,(8-Random(17))<<FIXSHIFT,
				(8-Random(16))<<FIXSHIFT,2,map,world,me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_SLASH:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,Cosine(me->facing*32)*4,
				Sine(me->facing*32)*4,6,map,world,me->friendly))
			{
				// nothing needs to be done
			}
			break;
		case BLT_FLAME:
			BurnHay(me->x, me->y);
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,1,map,world,me->friendly))
			{
				// no noise, just let them scream
			}
			break;
		case BLT_LIQUIFY:
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 12, me->dx, me->dy, 1, map, world,me->friendly))
			{
				// no noise, just let them scream
			}
		case BLT_FLAME2:
			BurnHay(me->x, me->y);
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,1,map,world,me->friendly))
			{
				// no noise, just let him scream
			}
			break;
		case BLT_BIGYELLOW:
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 16, me->dx, me->dy, 30, map, world, me->friendly))
			{
				// hit noise
				BulletRanOut(me, map, world);
			}
			break;
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
			BurnHay(me->x, me->y);
			if (FindVictims2(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 12, me->dx, me->dy, 1, map, world, me->friendly))
			{
				BlowSmoke(me->x, me->y, me->z, FIXAMT);
				// no noise, just let them scream
			}
			break;
		case BLT_YELWAVE:
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 12, me->dx, me->dy, 8, map, world, me->friendly))
			{
				// hit noise
				BulletRanOut(me, map, world);
			}
		case BLT_SPORE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,4,me->dx/2,me->dy/2,1,map,world,me->friendly))
			{

				PoisonVictim(GetLastGuyHit(),30);
				me->type=BLT_NONE;	// go away
			}
			break;
		case BLT_SPINE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,4,me->dx/4,me->dy/4,4,map,world,me->friendly))
			{
				me->type=BLT_NONE;
			}
			break;
		case BLT_SHROOM:
		//case BLT_ALIENEGG:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,10,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate, not to mention the 10 damage you already did
			}
			break;
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,Cosine(me->facing*32)*8,Sine(me->facing*32)*8,
				6,map,world,me->friendly))
			{
				// nothing special happens
			}
			break;
		case BLT_BOMB:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,0,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_MINE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,0,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_BOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,64,(8-Random(17))<<FIXSHIFT,
				(8-Random(16))<<FIXSHIFT,4,map,world,me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_BIGAXE:
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,(4-Random(9))<<FIXSHIFT,
				(4-Random(9))<<FIXSHIFT,5,map,world,me->friendly))
			{
				ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,10,8);
			}
			break;
		case BLT_YELBOOM:
			i=20*(5-(me->timer/2));	// size expands as boom expands
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,i,(8-Random(17))<<FIXSHIFT,
				(8-Random(16))<<FIXSHIFT,2,map,world,me->friendly))
			{
				// don't disappear because Bouapha needs to get multipounded
			}
			break;
		case BLT_SHOCKWAVE:
		case BLT_GOODSHOCK:
			i=30*(3-(me->timer/2))+30;	// size expands as wave expands
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,i,0,0,4,map,world,me->friendly))
			{
				// don't disappear because Bouapha needs to get multipounded
			}
			break;
		case BLT_ACID:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,1000);
				ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			}
			break;
		case BLT_QUEENACID:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,10,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,1000);
				ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			}
			break;
		case BLT_SHARK:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,1000);
				ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			}
			break;
		case BLT_BUBBLE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_BUBBLEPOP;
				me->anim=0;
				me->timer=10;
				me->dx=0;
				me->dy=0;
				MakeSound(SND_BUBBLEPOP,me->x,me->y,SND_CUTOFF,1000);
				ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			}
			break;
		case BLT_MEGABEAM1:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,40,0,12*FIXAMT,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_MEGABEAMHIT,me->x,me->y,SND_CUTOFF,1000);
				me->type=BLT_MEGABEAM2;
				me->dy=0;
				me->timer=2;
			}
			break;
		case BLT_ENERGY:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,3,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,6);
			}
			break;
		case BLT_ENERGY2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,6,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,6);
			}
			break;
		case BLT_BADFBALL:
			BurnHay(me->x, me->y);
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,10,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,950);
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
			}
			break;
		case BLT_EVILHAMMER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,10,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,950);
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
			}
			break;
		case BLT_ROCK:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,8,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ROCKBOUNCE,me->x,me->y,SND_CUTOFF,600);
				ExplodeParticles2(PART_DIRT,me->x,me->y,me->z,12,6);
			}
			break;
		case BLT_SNOWBALL:
		case BLT_BIGSNOW:
			if(me->z<FIXAMT*40)
			{
				if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,6,map,world,me->friendly))
				{
					BulletRanOut(me,map,world);
				}
			}
			break;
		case BLT_FREEZE:
		case BLT_FREEZE2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,0,map,world,me->friendly))
			{
				if(FreezeGuy(GetLastGuyHit()))
				{
					MakeSound(SND_FREEZE,me->x,me->y,SND_CUTOFF,1400);
				}
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_MINDWIPE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,0,map,world,me->friendly))
			{
				if(ControlMind(GetLastGuyHit()))
				{
					MakeSound(SND_ROBOBOUAPHAON,me->x,me->y,SND_CUTOFF,1400);
				}
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_REFLECT:
			ReflectBullets(me->x,me->y,128,me->friendly);
			break;
		case BLT_SWAP:
			if(SwapMe(me->x,me->y,10,map))
				BulletRanOut(me,map,world);
			break;
		case BLT_FARLEYGAS:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,me->dx,me->dy,1,map,world,me->friendly))
			{
				// nothing, just ouch
			}
			break;
		case BLT_WOLFSHOT:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,8,map,world,me->friendly))
			{
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				me->type=BLT_NONE;
			}
			break;
		case BLT_BATSHOT:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,1,map,world,me->friendly))
			{
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				me->type=BLT_NONE;
			}
			break;
		case BLT_BIGSHOT:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,12,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,12);
			}
			break;
		case BLT_FLAMEWALL:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,1,map,world,me->friendly))
			{
				// make a noise
			}
			break;
		case BLT_HOTDOGFIRE:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,1,map,world,me->friendly))
			{
				// ouch
			}
			break;
		case BLT_FLAMESHOT:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,3,map,world,me->friendly))
			{
				// make a noise
			}
			break;
		case BLT_EARTHSPIKE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,3,map,world,me->friendly))
			{
				// ouch
			}
			break;
		case BLT_EARTHSPIKE2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,1,map,world,me->friendly))
			{
				// ouch
			}
			break;
		case BLT_GASBLAST:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,me->dx,me->dy,0,map,world,me->friendly))
			{
				PoisonVictim(GetLastGuyHit(), 96);
			}
			break;
		case BLT_MEGABOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,100,(16-Random(33))<<FIXSHIFT,
				(16-Random(33))<<FIXSHIFT,4,map,world,me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_WHOOPEE:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,me->dx,me->dy,1,map,world,me->friendly))
			{
				// nothing, just ouch
			}
			break;
		case BLT_CROAKERGAS:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,me->dx,me->dy,1,map,world,me->friendly))
			{
				// nothing, just ouch
			}
			break;
		case BLT_CACTUS:
			if (FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 4, me->dx, me->dy, 2, map, world,me->friendly))
			{
				me->type = BLT_NONE;
				ExplodeParticles(PART_YELLOW, me->x, me->y, me->z, 3);
			}
			break;
		case BLT_BOOMERANG:
			if(FindNewVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,&me->target,12,me->dx,me->dy,7,map,world,me->friendly))
			{
				ExplodeParticles(PART_DIRT,me->x,me->y,me->z,3);
			}
			break;
		case BLT_ICEWAND:
		case BLT_ICEWAND2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,me->target,map,world,me->friendly))
			{
				if(FreezeGuy(GetLastGuyHit()))
				{
					MakeSound(SND_FREEZE,me->x,me->y,SND_CUTOFF,1400);
				}
				me->target = GetLastGuyHit()->ID;

				if(me->type!=BLT_ICEWAND2)
				{
					for (i = 0; i < 16; i++)
					{
						FireBullet(me->x, me->y, (byte)Random(256), BLT_ICEWAND2,me->friendly,me->target);
					}
				}
				me->type=BLT_NONE;
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,10);
			}
			break;
		case BLT_ICEWOLFICE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,12,map,world,me->friendly))
			{
				if(FreezeGuy(GetLastGuyHit()))
				{
					MakeSound(SND_FREEZE,me->x,me->y,SND_CUTOFF,1400);
				}
				me->type=BLT_NONE;
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,10);
			}
			break;
		case BLT_BIGBOMB:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,5,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_BIGBOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,96,(16-Random(33))<<FIXSHIFT,
				(16-Random(33))<<FIXSHIFT,4,map,world,me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_SWAMPGAS:
		case BLT_SWAMPGAS2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,10,me->dx,me->dy,0,map,world,me->friendly))
			{
				PoisonVictim(GetLastGuyHit(),32);
			}
			break;
		case BLT_ROCKET:
		case BLT_WIND:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,20,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_ICESHARD:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,4,me->dx/2,me->dy/2,0,map,world,me->friendly))
			{
				goodguy->dx+=me->dx/4;
				goodguy->dy+=me->dy/4;
				MakeSound(SND_FREEZE,me->x,me->y,SND_CUTOFF,1200);
			}
			break;
		case BLT_WATER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,1,map,world,me->friendly))
			{
				FireBullet(me->x-10*FIXAMT+Random(20*FIXAMT),me->y-10*FIXAMT+Random(20*FIXAMT),0,BLT_ROCKETSMOKE,me->friendly);
				MakeSound(SND_STEAM,me->x,me->y,SND_CUTOFF,600);
				me->type=BLT_NONE;
			}
			break;
		case BLT_CLAW:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,3,map,world,me->friendly))
			{
				ExplodeParticles(PART_SNOW2,me->x,me->y,0,10);
				me->type=BLT_NONE;
			}
			break;
	}
	attackType=BLT_NONE;
}

inline void BulletFaceGuy(bullet_t *me,Guy *goodguy)
{
	int desired;
	int diff,dir;

	if(goodguy==NULL)
		return;

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

inline void BulletFaceGuy2(bullet_t *me,Guy *goodguy)
{
	int f;
	int diff,dir,desired,amt;

	f=me->facing;
	me->facing/=32;
	BulletFaceGuy(me,goodguy);
	desired=me->facing*32;
	me->facing=f;

	if(desired>me->facing)
	{
		diff=desired-me->facing;
		if(diff>128)
		{
			dir=-1;
			diff=256-diff;
		}
		else
			dir=1;
		amt=4;
		if(amt>diff)
			amt=diff;
	}
	else
	{
		diff=me->facing-desired;
		if(diff>128)
		{
			dir=1;
			diff=256-diff;
		}
		else
			dir=-1;
		amt=4;
		if(amt>diff)
			amt=diff;
	}
	me->facing+=dir*amt;
}

void RepositionMissile(bullet_t* me)
{
	if (me->dx > 0)
	{
		if (me->facing > 8)
			me->facing++;
		else
			me->facing--;
	}
	if (me->dx < 0)
	{
		if (me->facing > 8)
			me->facing--;
		else
			me->facing++;
	}
	if (me->dy > 0)
	{
		if (me->facing > 11 || me->facing < 4)
			me->facing++;
		else
			me->facing--;
	}
	if (me->dy < 0)
	{
		if (me->facing > 11 || me->facing < 4)
			me->facing--;
		else
			me->facing++;
	}
	if (me->facing > 200)
		me->facing += 16;
	if (me->facing > 15)
		me->facing -= 16;
}

void UpdateBullet(bullet_t *me,Map *map,world_t *world)
{
	int mapx,mapy;
	int x,y,x2,y2,v,v2;
	byte b;
	word w;

	OffScreenBulletDie(me,map);
	if(!me->type)
		return;

	reflect=0;

	mapx=(me->x/TILE_WIDTH)>>FIXSHIFT;
	mapy=(me->y/TILE_HEIGHT)>>FIXSHIFT;
	me->bright=map->GetTile(mapx,mapy)->templight;

	activeBulDX=me->dx;
	activeBulDY=me->dy;
	b=0;


	if (me->type == BLT_LASERBEAM)
	{
		me->x += me->dx;
		me->y += me->dy;
		me->z = FIXAMT * 5;
		if (!BulletCanGo(me->type, me->x, me->y, map, 1, me->friendly))
			LaserMirrorHit(me, map, world);
	}
	else
	{
		if (map->flags & MAP_UNDERWATER)
		{
			me->x += me->dx * 3 / 4;
			if (!BulletCanGo(me->type, me->x, me->y, map, 8, me->friendly))
				BulletHitWallX(me, map, world);
			else
			{
				me->y += me->dy * 3 / 4;
				if (!BulletCanGo(me->type, me->x, me->y, map, 8, me->friendly))
					BulletHitWallY(me, map, world);
			}
			me->z += me->dz * 3 / 4;
		}
		else
		{
			me->x += me->dx;
			if (!BulletCanGo(me->type, me->x, me->y, map, 8, me->friendly))
				BulletHitWallX(me, map, world);
			else
			{
				me->y += me->dy;
				if (!BulletCanGo(me->type, me->x, me->y, map, 8, me->friendly))
					BulletHitWallY(me, map, world);
			}
			me->z += me->dz;
		}
	}

	if(me->z<0)
		BulletHitFloor(me,map,world);

	// all gravity-affected bullets, get gravitized
	if(me->flags&BFL_GRAVITY || HasGravity(me))
		me->dz-=FIXAMT;

	if(me->flags&BFL_WAVY)
	{
		me->dx+=Cosine(me->timer/2)/2;
		me->dy+=Sine(me->timer/2)/2;
	}

	me->timer--;
	if(!me->timer)
		BulletRanOut(me,map,world);

	if(!me->type)
		return;

	// special things like animation
	switch(me->type)
	{
		case BLT_COMET:
			me->anim++;
			if(me->anim==8)
				me->anim=0;
			break;
		case BLT_COMETBOOM:
			me->anim++;
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BOMBED)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me,map,world);
			break;
		case BLT_HOLESHOT:
			me->anim = 1 - me->anim;
			me->dx = (me->dx * 19) / 20;
			me->dy = (me->dy * 19) / 20;
			break;
		case BLT_BLACKHOLE:
			SuckParticle(me->x, me->y, FIXAMT * 20);
			map->BrightTorch(mapx, mapy, -10, 2);
			SuckInEvil(me->x, me->y, me->friendly);
			if (Random(2) == 0)
				HitBadguys(me, map, world);
			break;
		case BLT_SLIME:	// frame 259-270
			me->anim++;
			if(me->anim&1)
				HitBadguys(me,map,world);
			break;
		case BLT_LUNA:
		case BLT_LUNA2:
			me->dz=0;
			//ExplodeParticlesColor(4,me->x,me->y,me->z,1,1);
			ColorDrop(4,me->x,me->y,me->z);
			HitBadguys(me,map,world);
			break;
		case BLT_BADLUNA:
		case BLT_BADLUNA2:
			me->dz = 0;
			//ExplodeParticlesColor(0,me->x,me->y,me->z,1,1);
			ColorDrop(6, me->x, me->y, me->z);
			HitBadguys(me, map, world);
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_EVILHAMMER:
			me->anim++;
			if(me->anim>15)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
		case BLT_BADFBALL:
			me->anim++;
			if(me->anim>4)
			{
				me->anim=0;
				AddParticle(me->x,me->y,me->z,-FIXAMT/2+Random(FIXAMT),-FIXAMT/2+Random(FIXAMT),0,
							10,PART_SHORTYELLOW,191);
			}
			HitBadguys(me,map,world);
			break;
		case BLT_MINIFBALL:
			HitBadguys(me,map,world);
			break;
		case BLT_COIN:
		case BLT_BIGCOIN:
			Dampen(&me->dx,FIXAMT/64);
			Dampen(&me->dy,FIXAMT/64);
			me->anim++;
			if(me->anim>=8*2)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_CHEESEHAMMER:
			me->anim++;
			if(me->anim>7)
				me->anim=0;
			me->facing+=6-Random(13);
			me->dx=Cosine(me->facing)*16;
			me->dy=Sine(me->facing)*16;
			HitBadguys(me,map,world);
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
			me->dz+=FIXAMT/2;
			HitBadguys(me,map,world);
			break;
		case BLT_HARPOON:
			me->dz-=FIXAMT/8;
			HitBadguys(me,map,world);
			break;
		case BLT_BUBBLE:
			me->dz+=FIXAMT/2;	// less gravity
			HitBadguys(me,map,world);
			me->anim++;
			if(me->anim>=7*4)
				me->anim=7*4-1;
			if(me->timer==1)
			{
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_BUBBLEPOP:
			me->dz=0;
			me->anim++;
			if(me->anim==4)
				me->type=BLT_NONE;
			break;
		case BLT_PAPER:
			me->anim++;
			if(me->anim==16)
				me->anim=0;
			HitBadguys(me,map,world);
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dy,FIXAMT/8);
			break;
		case BLT_SLASH:
			me->anim++;
			if(me->anim==4)
				BulletRanOut(me,map,world);
			else if(me->anim==2)
			{
				HitBadguys(me,map,world);
				if(GetItem(map->GetTile((me->x>>FIXSHIFT)/TILE_WIDTH,((me->y>>FIXSHIFT)/TILE_HEIGHT))->item)->trigger&ITR_CHOP)
				{
					if(!editing && !player.cheated && verified)
						profile.progress.grassChopped++;
					TriggerItem(NULL,map->GetTile((me->x>>FIXSHIFT)/TILE_WIDTH,((me->y>>FIXSHIFT)/TILE_HEIGHT)),(me->x>>FIXSHIFT)/TILE_WIDTH,(me->y>>FIXSHIFT)/TILE_HEIGHT);
				}
			}
			break;
		case BLT_BIGAXE:
			me->anim++;
			if(me->anim>7)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_BIGSHELL:
		case BLT_SPINE:
			HitBadguys(me,map,world);
			break;
		case BLT_LIQUIFY:
			if(me->anim&1)
				HitBadguys(me,map,world);
			me->anim++;
			if(me->anim==14)
			{
				me->type=BLT_NONE;
				BlowSmoke(me->x,me->y,me->z,FIXAMT);
			}
			if(me->anim==4 && me->timer>13)
			{
				b=(me->facing-8+MGL_random(17))&255;
				FireExactBullet(me->x+Cosine(b)*16,me->y+Sine(b)*16,0,0,0,0,0,me->timer,b,BLT_LIQUIFY,me->friendly);
			}
			mapx=(me->x>>FIXSHIFT)/TILE_WIDTH;
			mapy=(me->y>>FIXSHIFT)/TILE_HEIGHT;

			map->BrightTorch(mapx,mapy,2,4);
			if(world->terrain[map->map[mapx+mapy*map->width].floor].flags&TF_WATER)
				BulletRanOut(me,map,world);
			break;
		case BLT_ICEBEAM2:
		case BLT_DEATHBEAM2:
			break;
		case BLT_DEATHBEAM:
			if (!BulletCanGo(me->type, me->x, me->y, map, 8, me->friendly))
			{
				me->type = BLT_DEATHBEAM2;
				me->timer = 8;
				break;
			}
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,32,4);
			HitBadguys(me,map,world);
			if(me->timer==4)
				FireBulletAfter(me->x+Cosine(me->facing)*16,me->y+Sine(me->facing)*16,me->facing,BLT_DEATHBEAM,me,me->friendly);
			me->anim=1-me->anim;
			break;
		case BLT_ICEBEAM:
			if (!BulletCanGo(me->type, me->x, me->y, map, 8, me->friendly))
			{
				me->type = BLT_ICEBEAM2;
				me->timer = 8;
				break;
			}
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_FROZEN)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me,map,world);
			if(me->timer==4)
				FireBulletAfter(me->x+Cosine(me->facing)*16*FIXAMT,me->y+Sine(me->facing)*16*FIXAMT,me->facing,BLT_ICEBEAM,me,me->friendly);
			me->anim=1-me->anim;
			if(me->anim==0)
				BlowWigglySmoke(me->x,me->y,me->z-Random(FIXAMT*4),0);
			break;
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			if(me->timer>4)
				me->anim=(8-me->timer);
			else
				me->anim=me->timer-1;
			if(me->timer==5)
				HitBadguys(me,map,world);
			else if(me->timer==4)
			{
				b=(me->facing*32-32+Random(65))&255;
				mapx=(me->x+Cosine(b)*32);
				mapy=(me->y+Sine(b)*32);
				FireBullet(mapx,mapy,me->facing,me->type,me->friendly);
			}
			break;
		case BLT_ROCK:
			me->anim++;
			if(me->anim>3)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_ENERGY:
		case BLT_ENERGY2:
			me->anim=1-me->anim;	// flip-flop animation
			HitBadguys(me,map,world);
			break;
		case BLT_GREEN:
		case BLT_BADGREEN:
			HitBadguys(me,map,world);
			break;
		case BLT_LILBOOM:
			HitBadguys(me,map,world);
			break;
		case BLT_LILBOOM2:
			break;
		case BLT_ACID:
		case BLT_SHARK:
			me->dz-=FIXAMT/2;	// less gravity than normal things
			me->anim++;
			if(me->anim>6)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_QUEENACID:
			me->dz-=FIXAMT/2;	// less gravity than normal things
			me->anim++;
			if(me->anim>6)
				me->anim=0;
			HitBadguys(me,map,world);
			
			if(me->timer==40)
				me->target = LockOn3(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 1200, me->friendly);

			if(!GetGuyPos(me->target,&mapx,&mapy))
				me->target=65535;
			else
			{
				if(me->x>mapx)
					me->dx-=FIXAMT;
				else
					me->dx+=FIXAMT;
				if(me->y>mapy)
					me->dy-=FIXAMT;
				else
					me->dy+=FIXAMT;

				Clamp(&me->dx,FIXAMT*8);
				Clamp(&me->dy,FIXAMT*8);
				RepositionMissile(me);
			}
			break;
		case BLT_FREEZE2:
			me->dz-=FIXAMT/2;	// less gravity than normal things
			me->anim++;
			if(me->anim>6)
				me->anim=0;
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_FROZEN)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me,map,world);
			break;
		case BLT_FREEZE:
			me->dz -= FIXAMT / 2;	// less gravity than normal things
			me->anim++;
			if(me->anim>6)
				me->anim=0;
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_FROZEN)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me,map,world);
			break;
		case BLT_SPORE:
			me->anim++;
			if(me->anim>3*4+3)
				me->anim=3*4+3;
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
			HitBadguys(me,map,world);
			break;
		case BLT_SNOWBALL:
			me->dz-=FIXAMT/2;	// less gravity than normal things
			HitBadguys(me,map,world);
			break;
		case BLT_BIGSNOW:
			me->anim++;
			if(me->anim>5)
				me->anim=0;
			me->dz-=FIXAMT/2;	// less gravity than normal things
			if(me->z<64*FIXAMT)
				HitBadguys(me,map,world);
			break;
		case BLT_GRENADE:
			me->anim=1-me->anim;
			break;
		case BLT_BIGYELLOW:
			me->anim = 1 - me->anim;
			HitBadguys(me, map, world);
			break;
		case BLT_SHOCKWAVE:
		case BLT_GOODSHOCK:
			me->anim++;
			HitBadguys(me,map,world);
			break;
		case BLT_YELBOOM:
			me->anim++;
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BOMBED)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me,map,world);
			break;
		case BLT_SHROOM:
		//case BLT_ALIENEGG:
			HitBadguys(me,map,world);
			break;
		case BLT_MEGABEAM:
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BOMBED)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			if(me->anim<(4*5))
				me->anim++;
			else	// FIRE!
				FireBulletAfter(me->x,me->y,0,BLT_MEGABEAM1,me,me->friendly);

			// keep this beam locked onto its launcher
			if(GetGuyPos(me->target,&me->x,&me->y))
			{
				if(GetGuy(me->target) && GetGuy(me->target)->aiType==MONS_DOZER)
					me->y+=FIXAMT*120;
				else
					me->y+=FIXAMT*8;
			}

			break;
		case BLT_MEGABEAM1:
		case BLT_YELWAVE:
			HitBadguys(me,map,world);
			break;
		case BLT_MEGABEAM2:
			// nothin
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BURNT)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			if(me->timer&1)	// every other frame
				HitBadguys(me,map,world);
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
			me->dz+=Random(FIXAMT/8);		//anti gravity
			me->dx+=Random(FIXAMT)-FIXAMT/2;
			me->dy+=Random(FIXAMT)-FIXAMT/2;
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
			Clamp(&me->dx,FIXAMT*10);
			Clamp(&me->dy,FIXAMT*10);
			me->anim=((32-me->timer)/8)+1;
			if(me->anim>4)
				me->anim=4;
			break;
		case BLT_FLAME3:
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BURNT)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			map->BrightTorch((me->x / TILE_WIDTH) >> FIXSHIFT,
				(me->y / TILE_HEIGHT) >> FIXSHIFT, 8, 4);
			me->dz += Random(FIXAMT / 8);		//anti gravity
			me->dx += Random(FIXAMT) - FIXAMT / 2;
			me->dy += Random(FIXAMT) - FIXAMT / 2;
			Dampen(&me->dx, FIXAMT / 4);
			Dampen(&me->dy, FIXAMT / 4);
			Clamp(&me->dx, FIXAMT * 10);
			Clamp(&me->dy, FIXAMT * 10);
			me->anim = ((me->timer) / 8) + 1;
			if (me->anim > 4)
				me->anim = 4;
			break;
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BURNT)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			if (me->timer & 1)	// every other frame
			{
				HitBadguys(me, map, world);
				map->DimTorch((me->x / TILE_WIDTH) >> FIXSHIFT, (me->y / TILE_HEIGHT) >> FIXSHIFT, 10);
			}
			map->BrightTorch((me->x / TILE_WIDTH) >> FIXSHIFT,
				(me->y / TILE_HEIGHT) >> FIXSHIFT, 8, 4);
			me->dz += FIXAMT / 2;		//anti gravity
			Dampen(&me->dx, FIXAMT / 8);
			Dampen(&me->dy, FIXAMT / 8);
			Clamp(&me->dx, FIXAMT * 10);
			Clamp(&me->dy, FIXAMT * 10);
			me->anim = Random(5);
			break;
		case BLT_IGNITE:
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BURNT)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me, map, world);
			BlowSmoke(me->x, me->y, me->z, FIXAMT);
			break;
		case BLT_LASER:
		case BLT_LASERBEAM:
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BURNT)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me,map,world);
			break;
		case BLT_BOMB:
			me->anim++;
			if(me->anim>15)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_MINE:
			HitBadguys(me,map,world);
			me->anim+=me->facing-1;
			me->timer++;
			if(me->anim==0)
				me->facing=2;
			else if(me->anim==5*4-1)
			{
				me->timer--;
				me->facing=0;
			}
			if(map->flags&MAP_UNDERWATER)	// floating mines
			{
				if(me->z<FIXAMT*20)
					me->dz+=FIXAMT/4;
				else
					me->dz-=FIXAMT/4;
			}
			break;
		case BLT_BOOM:
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
			if (GetItem(map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT))->item)->trigger & ITR_BOMBED)
				TriggerItem(NULL, map->GetTile((me->x >> FIXSHIFT) / TILE_WIDTH, ((me->y >> FIXSHIFT) / TILE_HEIGHT)), (me->x >> FIXSHIFT) / TILE_WIDTH, (me->y >> FIXSHIFT) / TILE_HEIGHT);
			HitBadguys(me,map,world);
			break;
		case BLT_ICECLOUD:
			me->anim = 1 - me->anim;
			if (me->anim == 0)
				BlowWigglySmoke(me->x, me->y, me->z - MGL_randoml(FIXAMT * 4), 0);
			HitBadguys(me, map, world);
			break;
		case BLT_SCANSHOT:
			HitBadguys(me,map,world);
			me->anim++;
			if(me->anim==3)
				me->anim=0;
			if(!GetGuyPos(me->target,&mapx,&mapy))
				me->target=65535;
			else
			{
				if(me->x>mapx)
					me->dx-=FIXAMT;
				else
					me->dx+=FIXAMT;
				if(me->y>mapy)
					me->dy-=FIXAMT;
				else
					me->dy+=FIXAMT;

				Clamp(&me->dx,FIXAMT*12);
				Clamp(&me->dy,FIXAMT*12);
			}
			break;
		case BLT_MISSILE:
		case BLT_SKULL:
		case BLT_MAGICMISSILE:
			if(me->timer==40)
			{
				if(me->friendly)
					me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
				else
					me->target=LockOnGood(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
			}

			me->anim++;
			if(((me->anim>0) && (me->target!=65535)) ||
				((me->anim>2) && (me->target==65535)))
			{
				if(map->flags&MAP_UNDERWATER)
					BlowBubble(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				else
					BlowSmoke(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				me->anim=0;
			}
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);

			HitBadguys(me,map,world);
			if(!GetGuyPos(me->target,&mapx,&mapy))
				me->target=65535;
			else
			{
				if(me->x>mapx)
					me->dx-=FIXAMT;
				else
					me->dx+=FIXAMT;
				if(me->y>mapy)
					me->dy-=FIXAMT;
				else
					me->dy+=FIXAMT;

				Clamp(&me->dx,FIXAMT*12);
				Clamp(&me->dy,FIXAMT*12);
				RepositionMissile(me);
			}
			break;
		case BLT_TORPEDO:
			me->anim++;
			if(me->anim==9)
			{
				BlowBubble(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				me->anim=0;
			}
			me->z+=me->dz;	// don't actually move by dz, it is just used to tell it which way to veer
			HitBadguys(me,map,world);
			me->dx+=Cosine(me->target/16)/2;
			me->dy+=Sine(me->target/16)/2;
			me->target+=me->dz;
			if(me->target>65536-256)
				me->target+=256*16;
			if(me->target>=256*16)
				me->target-=256*16;
			me->facing=me->target/256;
			Clamp(&me->dx,FIXAMT*16);
			Clamp(&me->dy,FIXAMT*16);
			break;
		case BLT_LIGHTNING:
			w=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
			if(w!=65535)
			{
				GetGuy(w)->GetShot(0,0,2,map,world);
				LightningBolt(goodguy->x,goodguy->y-FIXAMT*20,GetGuy(w)->x,
					GetGuy(w)->y-FIXAMT*10-GetGuy(w)->z);
				MakeSound(SND_ZAP,GetGuy(w)->x,GetGuy(w)->y,SND_CUTOFF,1000);
			}
			else
			{
				// make sizzle around player if there was no target
				LightningBolt(
					goodguy->x-FIXAMT*32+Random(FIXAMT*64),
					goodguy->y-FIXAMT*52+Random(FIXAMT*64),
					goodguy->x-FIXAMT*32+Random(FIXAMT*64),
					goodguy->y-FIXAMT*52+Random(FIXAMT*64));
			}
			me->type=BLT_NONE;	// begone immediately
			break;
		case BLT_ORBITER:
			me->timer++;
			if(--me->anim==0)
			{
				me->timer--;
				me->anim=15;
				me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
				if(me->target!=65535)
				{
					FireBullet(me->x,me->y,me->facing*32,BLT_GREEN,me->friendly);
				}
			}
			if(me->target!=65535)
			{
				BulletFaceGuy(me,GetGuy(me->target));
			}
			if(me->x<goodguy->x)
				me->dx+=FIXAMT;
			if(me->y<goodguy->y)
				me->dy+=FIXAMT;
			if(me->x>goodguy->x)
				me->dx-=FIXAMT;
			if(me->y>goodguy->y)
				me->dy-=FIXAMT;
			Clamp(&me->dx,FIXAMT*9);
			Clamp(&me->dy,FIXAMT*9);
			break;
		case BLT_ORBITER2:
			me->timer++;
			if(--me->anim==0)
			{
				me->timer--;
				me->anim=45;
				me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
				if(me->target!=65535)
				{
					FireBullet(me->x,me->y,me->facing*32,BLT_BOMB,me->friendly);
				}
			}
			if(me->target!=65535)
			{
				BulletFaceGuy(me,GetGuy(me->target));
			}
			if(me->x<goodguy->x)
				me->dx+=FIXAMT;
			if(me->y<goodguy->y)
				me->dy+=FIXAMT;
			if(me->x>goodguy->x)
				me->dx-=FIXAMT;
			if(me->y>goodguy->y)
				me->dy-=FIXAMT;
			Clamp(&me->dx,FIXAMT*9);
			Clamp(&me->dy,FIXAMT*9);
			break;
		case BLT_BALLLIGHTNING:
		case BLT_BADLIGHTNING:
			x=me->x;
			y=me->y-me->z;
			v=Random(256);
			v2=Random(16)+2;
			x2=(x+Cosine(v)*v2);
			y2=(y+Sine(v)*v2);
			v=Random(256);
			v2=Random(16)+2;
			x=(x+Cosine(v)*v2);
			y=(y+Sine(v)*v2);
			LightningBolt(x,y,x2,y2);
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,1);
			HitBadguys(me,map,world);
			break;
		case BLT_LIGHTNING2:
			w=LockOn3(me->x>>FIXSHIFT,me->y>>FIXSHIFT,1200,me->friendly);
			if(w!=65535)
			{
				GetGuy(w)->GetShot(0,0,5,map,world);
				LightningBolt(me->x,me->y-FIXAMT*20,GetGuy(w)->x,
					GetGuy(w)->y-FIXAMT*10-GetGuy(w)->z);
				MakeSound(SND_ZAP,GetGuy(w)->x,GetGuy(w)->y,SND_CUTOFF,1000);
			}
			else
			{
				// zap to nowhere
				LightningBolt(
					me->x,
					me->y,
					me->x-FIXAMT*64+Random(FIXAMT*128),
					me->y-FIXAMT*64+Random(FIXAMT*128));
			}
			me->type=BLT_NONE;	// begone immediately
			break;
		case BLT_MINDWIPE:
			me->anim++;
			if(me->anim>2)
				me->anim=0;

			HitBadguys(me,map,world);
			me->bright=(char)Random(16);
			break;
		case BLT_REFLECT:
			me->anim++;
			//map->BrightTorch(me->x/(TILE_WIDTH*FIXAMT),me->y/(TILE_HEIGHT*FIXAMT),me->timer,4);
			if(me->anim<3)
				HitBadguys(me,map,world);
			break;
		case BLT_SWAP:
			me->anim+=8;
			HitBadguys(me,map,world);
			break;
		case BLT_SCANNER:
			me->anim++;
			if(me->anim==3)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_SCANLOCK:
			if(me->target!=65535 && GetGuy(me->target)->type!=MONS_NONE)
			{
				me->x=GetGuy(me->target)->x;
				me->y=GetGuy(me->target)->y;
				me->z=GetGuy(me->target)->z;
				if(me->z==0)
					me->z=FIXAMT*20;
			}
			else
			{
				me->target=65535;
				if(me->timer>3)
					me->timer=3;	// go away if not on a target
			}
			me->dx=me->dy=me->dz=0;
			break;
		case BLT_BIGSHOT:
			me->anim=1-me->anim;	// flip-flop animation
			me->facing+=16;			// spin
			HitBadguys(me,map,world);
			break;
		case BLT_FLAMEWALL:
			if(me->timer>6)
				me->anim=(me->timer-6);
			else
				me->anim=(6-me->timer);
			if(me->anim>3)
			HitBadguys(me,map,world);
			break;
		case BLT_FLAMESHOT:
			if(me->anim>1)
				me->anim--;
			else
				me->anim=1-me->anim;
			HitBadguys(me,map,world);
			break;
		case BLT_EARTHSPIKE:
		case BLT_EARTHSPIKE2:
			if(me->timer>22 || me->timer<10)
			{
				me->anim++;
				if(me->anim==15)
					me->type=BLT_NONE;
			}
			if(me->anim==4)
			{
				FireBullet(me->x+Cosine(me->facing)*TILE_WIDTH,me->y+Sine(me->facing)*TILE_HEIGHT,
						((me->facing+(256-16))+(byte)Random(33))&255,BLT_EARTHSPIKE,me->friendly);
			}
			if(me->anim>4 && me->anim<10)
				HitBadguys(me,map,world);
			break;
		case BLT_GASBLAST:
			if(me->timer>=10 && me->timer<=50)
			{
				me->anim++;
				if(me->anim>5)
					me->anim=3;
				Dampen(&me->dx,FIXAMT/16);
				Dampen(&me->dy,FIXAMT/16);
				HitBadguys(me,map,world);
			}
			else if(me->timer<6)
			{
				if(me->anim>0)
					me->anim--;
			}
			break;
		case BLT_MEGABOOM:
			me->anim++;
			if(me->anim<2)
				HitBadguys(me,map,world);
			break;
		case BLT_WHOOPEE:
			if(me->timer>=10 && me->timer<=110)
			{
				me->anim++;
				if(me->anim>5*2)
				{
					me->anim=3*2;
					HitBadguys(me,map,world);
				}
				Dampen(&me->dx,FIXAMT/2);
				Dampen(&me->dy,FIXAMT/2);
			}
			else if(me->timer<6)
			{
				if(me->anim>0)
					me->anim--;
			}
			break;
		case BLT_CROAKERGAS:
			if(me->timer>=10 && me->timer<=28)
			{
				me->anim++;
				if(me->anim>5*2)
					me->anim=3*2;
				if((me->timer%8)==0)
					HitBadguys(me,map,world);

				Dampen(&me->dx,FIXAMT/32);
				Dampen(&me->dy,FIXAMT/32);
			}
			else if(me->timer<6)
			{
				if(me->anim>0)
					me->anim--;
			}
			break;
		case BLT_FARLEYGAS:
			if(me->timer>=10 && me->timer<=50)
			{
				me->anim++;
				if(me->anim>5*2)
					me->anim=3*2;
				if((me->timer&3)==0)
					HitBadguys(me,map,world);

				Dampen(&me->dx,FIXAMT/32);
				Dampen(&me->dy,FIXAMT/32);
			}
			else if(me->timer<6)
			{
				if(me->anim>0)
					me->anim--;
			}
			break;
		case BLT_CACTUS:
		case BLT_ICESHARD:
			HitBadguys(me,map,world);
			break;
		case BLT_BOOMERANG:
			me->anim++;
			if(me->anim>15)
				me->anim=0;
			if(me->timer<60)
			{
				// home in on player
				BulletFaceGuy(me,goodguy);
				me->dx+=Cosine(me->facing*32);
				me->dy+=Sine(me->facing*32);
				Clamp(&me->dx,FIXAMT*8);
				Clamp(&me->dy,FIXAMT*8);
			}
			HitBadguys(me,map,world);
			break;
		case BLT_ICEWAND:
		case BLT_ICEWAND2:
			HitBadguys(me,map,world);
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,1,1);
			break;
		case BLT_ICEWOLFICE:
			HitBadguys(me,map,world);
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,1,1);
			break;
		case BLT_BATSHOT:
		case BLT_WOLFSHOT:
			HitBadguys(me,map,world);
			break;
		case BLT_BIGBOMB:
			me->anim=me->timer/10;
			HitBadguys(me,map,world);
			break;
		case BLT_BIGBOOM:
			me->anim++;
			if(me->anim<2)
				HitBadguys(me,map,world);
			break;
		case BLT_ROCKETSMOKE:
			me->anim++;
			break;
		case BLT_SWAMPGAS:
		case BLT_SWAMPGAS2:
			if(me->anim>0)
			{
				me->anim++;
				HitBadguys(me,map,world);
				return;	// don't let the timer reset!
			}
			if(me->timer==1 && Random(10)!=1)
			{
				me->timer=3;
			}
			break;
		case BLT_HOTPANTS:
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->anim>0)
				me->anim--;
			else
				me->anim=1;
			if(me->timer<6)
				me->anim=(6-me->timer)/2;

			if(me->target>0 && me->timer==16)
			{
				for(b=0;b<4;b++)
				{
					if(me->target<2)
					FireBullet(me->x,me->y,(byte)Random(256),BLT_HOTPANTS,me->friendly,me->target+1);
				}
			}
			if(me->anim==0)
				HitBadguys(me,map,world);
			break;
		case BLT_HOTDOGFIRE:
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dy,FIXAMT/8);
			if(me->anim>0)
				me->anim--;
			else
				me->anim=1;
			if(me->timer<6)
				me->anim=(6-me->timer)/2;

			if(me->anim==0)
				HitBadguys(me,map,world);
			break;
		case BLT_ORBGRENADE:
			if(me->anim>16)
				me->anim=0;
			else
				me->anim++;
			break;
		case BLT_ROCKET:
			BulletFaceGuy2(me,goodguy);
			me->dx+=Cosine(me->facing)*2;
			me->dy+=Sine(me->facing)*2;
			Dampen(&me->dx,FIXAMT/2);
			Dampen(&me->dy,FIXAMT/2);
			Clamp(&me->dx,FIXAMT*8);
			Clamp(&me->dy,FIXAMT*8);
			HitBadguys(me,map,world);
			me->anim=1-me->anim;
			if(me->anim)
				FireBullet(me->x,me->y-FIXAMT*20,0,BLT_ROCKETSMOKE,me->friendly);
			break;
		case BLT_WIND:
			BulletFaceGuy2(me,goodguy);
			me->dx+=Cosine(me->facing)*2;
			me->dy+=Sine(me->facing)*2;
			Dampen(&me->dx,FIXAMT/2);
			Dampen(&me->dy,FIXAMT/2);
			Clamp(&me->dx,FIXAMT*6);
			Clamp(&me->dy,FIXAMT*6);
			HitBadguys(me,map,world);
			FireBullet(me->x,me->y-FIXAMT*20,0,BLT_ROCKETSMOKE,me->friendly);
			me->anim++;
			if(((me->anim>0) && (me->target!=65535)) ||
				((me->anim>2) && (me->target==65535)))
			{
				if(map->flags&MAP_UNDERWATER)
					BlowBubble(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				else
					BlowSmoke(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				me->anim=0;
			}
			break;
		case BLT_WATER:
			if(me->anim==0)
			{
				for(w=0;w<3;w++)
				{
					bullet_t *you = GetFireBullet(me->x-me->dx/2,me->y-me->dy/2,me->facing,BLT_WATER,me->friendly);
					if (you) {
						you->x += -FIXAMT * 8 + Random(FIXAMT * 16);
						you->y += -FIXAMT * 8 + Random(FIXAMT * 16);
						you->z = me->z - me->dz - FIXAMT * 4 + Random(FIXAMT * 8);
						you->anim = 1;
					}
				}
				me->anim=1;
			}
			HitBadguys(me,map,world);
			me->dz-=FIXAMT/2;
			break;
		case BLT_FOUNTAIN:
			FireBullet(me->x,me->y,me->facing,BLT_WATER,me->friendly);
			break;
		case BLT_EVILFACE:
			me->anim+=(byte)Random(3);
			if(me->anim>=6*16)
				me->anim=6*16-1;
			me->dx+=-FIXAMT/16+Random(FIXAMT/8);
			me->dy-=Random(FIXAMT/16);
			break;
		case BLT_WOLFSHOCK:
			w=LockOn3(me->x>>FIXSHIFT,me->y>>FIXSHIFT,6*TILE_HEIGHT,me->friendly);
			if(w!=65535)
			{
				GetGuy(w)->GetShot(0,0,2,map,world);
				LightningBolt(goodguy->x,goodguy->y-FIXAMT*20,GetGuy(w)->x,
					GetGuy(w)->y-FIXAMT*10-GetGuy(w)->z);
				MakeSound(SND_ZAP,GetGuy(w)->x,GetGuy(w)->y,SND_CUTOFF,1000);
			}
			else
			{
				// make sizzle around player if there was no target
				LightningBolt(
					goodguy->x-FIXAMT*32+Random(FIXAMT*64),
					goodguy->y-FIXAMT*52+Random(FIXAMT*64),
					goodguy->x-FIXAMT*32+Random(FIXAMT*64),
					goodguy->y-FIXAMT*52+Random(FIXAMT*64));
			}
			me->type=BLT_NONE;	// begone immediately
			break;
		case BLT_CLAW:
			HitBadguys(me,map,world);
			break;
	}
	if (me->flags & BFL_HOMING)
	{
		me->target=LockOn3(me->x >> FIXSHIFT, me->y >> FIXSHIFT,1200,me->friendly);
		if (!GetGuyPos(me->target, &mapx, &mapy))
			me->target = 65535;
		else
		{
			if (me->x > mapx)
				me->dx -= FIXAMT;
			else
				me->dx += FIXAMT;
			if (me->y > mapy)
				me->dy -= FIXAMT;
			else
				me->dy += FIXAMT;

			Clamp(&me->dx, FIXAMT * 12);
			Clamp(&me->dy, FIXAMT * 12);
		}
	}
}

void RenderSmoke(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_SMOKE+frm),DISPLAY_DRAWME|DISPLAY_GHOST);
}

void RenderBubble(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_BUBBLE+frm),DISPLAY_DRAWME|DISPLAY_GLOW);
}

void RenderMindControl(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_SCANSHOT+frm),DISPLAY_DRAWME|DISPLAY_GLOW);
}

void RenderBoom(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_BOOM+frm),DISPLAY_DRAWME|DISPLAY_GLOW);
}

void RenderStinky(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_STINKY+frm),DISPLAY_DRAWME|DISPLAY_GHOST);
}

void RenderMysticalHammerItem(byte col1, byte col2, char brt, int x, int y)
{
	int v;
	sprite_t* curSpr;
	byte f;

	f = ((player.clock / 2) % 8);
	x -= Cosine(f * 32) * 8;
	y -= Sine(f * 32) * 6;

	v = f * 16 + 12 + SPR_HAMMER;
	curSpr = bulletSpr->GetSprite(v);
	SprDraw(x >> FIXSHIFT, y >> FIXSHIFT, 0, 255, 0, curSpr,
		DISPLAY_DRAWME | DISPLAY_SHADOW);
	SprDrawOff(x >> FIXSHIFT, y >> FIXSHIFT, 10, col1, col2, brt, curSpr,
		DISPLAY_DRAWME | DISPLAY_OFFCOLOR);
}

void RenderSimpleAnimatedItem(byte col1, byte col2, char brt, int x, int y, int z, int a, int b, int c) {

	int v;
	sprite_t* curSpr;
	byte f;

	f = ((player.clock / c) % b);
	v = a + f;
	curSpr = bulletSpr->GetSprite(v);
	SprDraw(x >> FIXSHIFT, y >> FIXSHIFT, 0, 255, 0, curSpr,
		DISPLAY_DRAWME | DISPLAY_SHADOW);
	SprDrawOff(x >> FIXSHIFT, y >> FIXSHIFT, z, col1, col2, brt, curSpr,
		DISPLAY_DRAWME | DISPLAY_OFFCOLOR);
}

void SprDrawBullet(bullet_t *me,sprite_t *curSpr,word flags,char brt){

	if(flags&DISPLAY_SHADOW)
		SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright+brt,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW); // Display shadow

	flags&=~DISPLAY_SHADOW;
	if (me->fromColor == 255) // Display sprite minus w/o shadow flag
		SprDrawOff(me->x >>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,255,me->bright+brt,curSpr,flags|DISPLAY_DRAWME);
	else // Ditto, but w/ off-color
		SprDrawOff(me->x >>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,me->fromColor,me->toColor,me->bright+brt,curSpr,flags|DISPLAY_DRAWME);
}

void SprDrawBulletOffXY(int x, int y,bullet_t *me,sprite_t *curSpr,word flags,char brt){

	if(flags&DISPLAY_SHADOW)
		SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,me->bright+brt,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW); // Display shadow

	flags&=~DISPLAY_SHADOW;
	if (me->fromColor == 255) // Display sprite minus w/o shadow flag
		SprDrawOff(x>>FIXSHIFT,y>>FIXSHIFT,me->z>>FIXSHIFT,255,255,me->bright+brt,curSpr,flags|DISPLAY_DRAWME);
	else // Ditto, but w/ off-color
		SprDrawOff(x>>FIXSHIFT,y>>FIXSHIFT,me->z>>FIXSHIFT,me->fromColor,me->toColor,me->bright+brt,curSpr,flags|DISPLAY_DRAWME);
}

void SprDrawBulletOffXYZ(int x, int y, int z,bullet_t *me,sprite_t *curSpr,word flags,char brt){

	if(flags&DISPLAY_SHADOW)
		SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,me->bright+brt,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW); // Display shadow

	flags&=~DISPLAY_SHADOW;
	if (me->fromColor == 255) // Display sprite minus w/o shadow flag
		SprDrawOff(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,255,255,me->bright+brt,curSpr,flags|DISPLAY_DRAWME);
	else // Ditto, but w/ off-color
		SprDrawOff(x>>FIXSHIFT,y>>FIXSHIFT,z>>FIXSHIFT,me->fromColor,me->toColor,me->bright+brt,curSpr,flags|DISPLAY_DRAWME);
}

void RenderBullet(bullet_t *me)
{
	int v,x,y,z;
	sprite_t *curSpr;

	switch(me->type)
	{
		case BLT_COMET:
			curSpr=bulletSpr->GetSprite(SPR_COMET+me->anim);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_COMETBOOM:
			curSpr=bulletSpr->GetSprite(SPR_COMETBOOM+me->anim/2);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_SLIME:
			curSpr=bulletSpr->GetSprite(SPR_SLIME+(me->anim/16));
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,-4);
			break;
		case BLT_ICECLOUD:
			// invisible... it just gives off steam
			break;
		case BLT_ICEBEAM:
			curSpr=bulletSpr->GetSprite(((me->facing/32)&3)+SPR_ICEBEAM+1);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_ICEBEAM2:
			curSpr=bulletSpr->GetSprite(SPR_ICEBEAM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_DEATHBEAM:
			curSpr=bulletSpr->GetSprite(((me->facing/32)&3)+SPR_ICEBEAM+1);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_DEATHBEAM2:
			curSpr=bulletSpr->GetSprite(SPR_ICEBEAM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_LIQUIFY:
			curSpr=bulletSpr->GetSprite(231+me->anim/2);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_COIN:
		case BLT_BIGCOIN:
			if(me->timer<30 && (me->timer&1)==0)
				return;
			if(me->type==BLT_COIN)
				curSpr=bulletSpr->GetSprite(SPR_COIN+me->anim/2);
			else
				curSpr=bulletSpr->GetSprite(SPR_BIGCOIN+me->anim/2);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
			v=me->facing*16+(me->anim)+SPR_HAMMER;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
			v = me->facing * 5 + (me->anim) + SPR_FIREBALL;
			curSpr = bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_BADFBALL:
			v = me->facing * 5 + (me->anim) + SPR_FIREBALL;
			curSpr = bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_CHEESEHAMMER:
			v=(((me->facing/32)+me->anim)&7)*16+SPR_HAMMER;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_EVILHAMMER:
			v=me->facing*16+(me->anim)+SPR_HAMMER;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_ACID:
			v=me->facing*7+me->anim+SPR_ACID;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_SHARK:
			v=me->facing*7+me->anim+SPR_ACID;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_FREEZE:
		case BLT_FREEZE2:
			v=me->facing*7+me->anim+SPR_ACID;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,0,0);
			break;
		case BLT_MISSILE:
			v=me->facing+SPR_MISSILE;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_MAGICMISSILE:
			v = me->facing + SPR_MAGICMISSILE;
			curSpr = bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_SKULL:
			curSpr = bulletSpr->GetSprite(SPR_SKULL + me->facing);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_TORPEDO:
			v=(me->facing)+SPR_MISSILE;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawBullet(me,curSpr,0,0);
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
			curSpr = bulletSpr->GetSprite(me->anim + SPR_FLAME);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_LASER:
			curSpr=bulletSpr->GetSprite(me->facing+SPR_LASER);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_LASERBEAM:
			curSpr = bulletSpr->GetSprite(((me->facing + 8) / 16) + SPR_LASER);
			for (v = 0; v < 3; v++)
			{
				x = me->x - FIXAMT * 10 + Random(FIXAMT * 20 + 1);
				y = me->y - FIXAMT * 10 + Random(FIXAMT * 20 + 1);
				SprDrawBulletOffXY(x,y,me,curSpr,DISPLAY_GLOW,0);
			}
			break;
		case BLT_PAPER:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_PAPER);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_BUBBLE:
			curSpr=bulletSpr->GetSprite(me->anim/4+SPR_BUBBLE);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW|DISPLAY_GLOW,0);
			break;
		case BLT_BUBBLEPOP:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_BUBBLE+5);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW|DISPLAY_GLOW,0);
			break;
		case BLT_BOMB:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_BOMB);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_BOOM:
			curSpr=bulletSpr->GetSprite(7-me->timer+SPR_BOOM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_ENERGY:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_ENERGY);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_MEGABEAM:
			curSpr=bulletSpr->GetSprite((me->anim>>2)+SPR_MEGABEAM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_MEGABEAM1:
			curSpr=bulletSpr->GetSprite(6+SPR_MEGABEAM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_MEGABEAM2:
			curSpr=bulletSpr->GetSprite(7+SPR_MEGABEAM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_SPORE:
			curSpr=bulletSpr->GetSprite(me->anim/4+SPR_SPORE);
			SprDrawBullet(me,curSpr,0,0);
			break;
		case BLT_SHROOM:
			curSpr=bulletSpr->GetSprite(me->facing+SPR_SHROOM);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_GRENADE:
		case BLT_BIGYELLOW:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_GRENADE);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW|DISPLAY_GLOW,0);
			break;
		case BLT_HOLESHOT:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_GRENADE);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,-12);
			break;
		case BLT_YELBOOM:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_YELBOOM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_SHOCKWAVE:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_SHOCKWAVE);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_LILBOOM:
		case BLT_LILBOOM2:
			curSpr=bulletSpr->GetSprite(4-(me->timer/2)+SPR_LILBOOM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_SNOWBALL:
			curSpr=bulletSpr->GetSprite(SPR_SNOWBALL);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_BIGSNOW:
			curSpr=bulletSpr->GetSprite(SPR_BIGSNOW+(me->anim/2));
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_ICESPIKE:
			curSpr=bulletSpr->GetSprite(SPR_ICESPIKE+(me->anim));
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_DIRTSPIKE:
			curSpr=bulletSpr->GetSprite(SPR_ICESPIKE+(me->anim));
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_ROCK:
			if((me->timer<8) && (me->timer&1))
				return;	// flicker when almost gone
			curSpr=bulletSpr->GetSprite(SPR_ROCK+(me->anim));
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_SPINE:
			curSpr=bulletSpr->GetSprite(SPR_SPINE+(me->facing));
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_MINIFBALL:
			curSpr = bulletSpr->GetSprite(SPR_SPINE + (me->facing));
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_YELWAVE:
			curSpr = bulletSpr->GetSprite(SPR_WAVE + (me->facing));
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_BIGSHELL:
			curSpr=bulletSpr->GetSprite(SPR_BOOM);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_BIGAXE:
			curSpr=bulletSpr->GetSprite(SPR_BIGAXE+me->anim);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_HARPOON:
			curSpr=bulletSpr->GetSprite(SPR_SPEAR+me->facing);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_SLASH:
			if(me->anim>0)
			{
				curSpr=bulletSpr->GetSprite(SPR_SLASH+me->facing*3+me->anim-1);
				SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			}
			break;
		case BLT_MINE:
			curSpr=bulletSpr->GetSprite(SPR_MINE+me->anim/4);
			SprDrawBullet(me,curSpr,curMap->flags&MAP_UNDERWATER?DISPLAY_SHADOW:0,0);
			break;
		case BLT_ORBITER:
			curSpr=bulletSpr->GetSprite(SPR_ORBITER+me->facing);
				SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_ORBITER2:
			curSpr=bulletSpr->GetSprite(SPR_ORBITER+me->facing);
				SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_GREEN:
		case BLT_BADGREEN:
			curSpr = bulletSpr->GetSprite(SPR_GREEN);
				SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_SCANSHOT:
		case BLT_SCANNER:
			curSpr=bulletSpr->GetSprite(SPR_SCANSHOT+me->anim);
			SprDrawBullet(me, curSpr, DISPLAY_GLOW, 0);
			break;
		case BLT_SCANLOCK:
			if(me->timer>10 || (me->timer&1))
			{
				curSpr=bulletSpr->GetSprite(SPR_SCANLOCK);
				SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			}
			break;
		case BLT_BALLLIGHTNING:
		case BLT_BADLIGHTNING:
			break;
		case BLT_MINDWIPE:
			curSpr=bulletSpr->GetSprite(SPR_BOOM+me->anim);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_REFLECT:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_YELBOOM);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_SWAP:
			int x,y;
			curSpr=bulletSpr->GetSprite(SPR_GREEN);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			for(v=0;v<4;v++)
			{
				x=me->x+Cosine((me->anim+v*64)&255)*6;
				y=me->y+Sine((me->anim+v*64)&255)*6;
				SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			}
			break;
		case BLT_ENERGY2:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_REDBONE);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_BIGSHOT:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_REDBONE);
			SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			SprDrawBulletOffXY((me->x + Cosine(me->facing) * 10) >> FIXSHIFT,
				(me->y + Sine(me->facing) * 10) >> FIXSHIFT, me, curSpr, DISPLAY_SHADOW, 0);
			SprDrawBulletOffXY((me->x + Cosine(me->facing) * 10) >> FIXSHIFT,
				(me->y + Sine(me->facing+86) * 10) >> FIXSHIFT, me, curSpr, DISPLAY_SHADOW, 0);
			SprDrawBulletOffXY((me->x + Cosine(me->facing) * 10) >> FIXSHIFT,
				(me->y + Sine(me->facing+172) * 10) >> FIXSHIFT, me, curSpr, DISPLAY_SHADOW, 0);
			break;
		case BLT_FLAMEWALL:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_FLAMEWALL);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_FLAMESHOT:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_FLAME);
			SprDrawBullet(me,curSpr,DISPLAY_GLOW,0);
			break;
		case BLT_EARTHSPIKE:
		case BLT_EARTHSPIKE2:
			curSpr=bulletSpr->GetSprite(568-me->anim/2);
			SprDrawBullet(me,curSpr,DISPLAY_SHADOW,0);
			break;
		case BLT_GASBLAST:
			curSpr=bulletSpr->GetSprite(558-me->anim);
			SprDrawBullet(me, curSpr, DISPLAY_GLOW, 0);
			break;
		case BLT_MEGABOOM:
			curSpr=bulletSpr->GetSprite(SPR_MEGABOOM+me->anim);
			SprDrawBullet(me, curSpr, DISPLAY_GLOW, 0);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_WHOOPEE:
		case BLT_CROAKERGAS:
		case BLT_FARLEYGAS:
			curSpr=bulletSpr->GetSprite(713-(me->anim/2));
			SprDrawBullet(me, curSpr, DISPLAY_GLOW, 0);
			break;
		case BLT_CACTUS:
			curSpr=bulletSpr->GetSprite((me->facing/16)+SPR_CACTUS);
			SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			break;
		case BLT_BOOMERANG:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_BOOMERANG);
			SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			break;
		case BLT_BIGBOMB:
			curSpr=bulletSpr->GetSprite(644-me->anim);
			SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			break;
		case BLT_BIGBOOM:
			curSpr=bulletSpr->GetSprite(SPR_BIGBOMBBOOM+me->anim);
			SprDrawBullet(me, curSpr, DISPLAY_GLOW, 0);
			break;
		case BLT_SWAMPGAS:
		case BLT_SWAMPGAS2:
		case BLT_ROCKETSMOKE:
			if(me->anim>0)
			{
				curSpr=bulletSpr->GetSprite(634+me->anim/2);
				SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			}
			break;
		case BLT_HOTPANTS:
		case BLT_HOTDOGFIRE:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_FLAMEWALL);
			SprDrawBullet(me, curSpr, DISPLAY_GLOW, 0);
			break;
		case BLT_ORBGRENADE:
			curSpr = bulletSpr->GetSprite(me->anim+SPR_ORB);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_ROCKET:
			curSpr=bulletSpr->GetSprite(me->facing/16+SPR_ROCKET);
			SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			break;
		case BLT_WIND:
			// no image
			break;
		case BLT_ICESHARD:
			curSpr=bulletSpr->GetSprite((me->facing/16)+SPR_CACTUS);
			RecolorBullet(me,0,7);
			SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			break;
		case BLT_WATER:
			curSpr=bulletSpr->GetSprite(SPR_DROPLET);
			SprDrawBullet(me, curSpr, DISPLAY_GLOW | DISPLAY_SHADOW, 0);
			break;
		case BLT_CLAW:
			curSpr=bulletSpr->GetSprite((me->facing/16)+SPR_CLAW);
			SprDrawBullet(me, curSpr, DISPLAY_SHADOW, 0);
			break;
		case BLT_EVILFACE:
			curSpr=bulletSpr->GetSprite(me->anim/16+686);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GHOST);
			break;
		case BLT_QUEENACID:
			for (int i = 0; i < 8; i++)
			{
				byte v2 = (me->timer * 2 + i * 32) & 255;
				v = SPR_ACID + v2 / 32 * 7 + me->anim;
				curSpr = bulletSpr->GetSprite(v);
				x = me->x + Cosine(v2)*16;
				y = me->y + Sine(v2)*12;
				SprDrawBulletOffXYZ(x,y,me->z,me,curSpr, DISPLAY_SHADOW, 0);
			}
			break;
		case BLT_ALIENEGG:
			curSpr = GetMonsterSprite(MONS_ALIENEGG,ANIM_IDLE,0,0);
			SprDraw(me->x>>FIXSHIFT,me->y>> FIXSHIFT,0,255,me->bright,curSpr,DISPLAY_DRAWME|DISPLAY_SHADOW); // Display shadow
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,DISPLAY_DRAWME);
			break;
	}
}

void UpdateBullets(Map *map,world_t *world)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(bullet[i].type)
			UpdateBullet(&bullet[i],map,world);
}

void RenderBullets(void)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(bullet[i].type)
			RenderBullet(&bullet[i]);
}

byte HasGravity(bullet_t* me) {
	switch(me->type)
	{
		default:
			return 0;
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_BOMB:
		case BLT_GRENADE:
		case BLT_ROCK:
		case BLT_EVILHAMMER:
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_BUBBLE:
		case BLT_FREEZE:
		case BLT_FREEZE2:
		case BLT_ACID:
		case BLT_QUEENACID:
		case BLT_ALIENEGG:
		case BLT_COIN:
		case BLT_BIGCOIN:
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
		case BLT_FLAME3:
		case BLT_ORBGRENADE:
			return 1;
	}
}

byte HasHoming(bullet_t* me) {
	return (me->type == BLT_PAPER);
}

#define BD_NONE		0
#define BD_EIGHT	1
#define BD_SIXTEEN	2

// No recolor bullet!
void UncolorBullet(bullet_t* me) {
	me->fromColor = 255;
	me->toColor = 0;
}

// Recolor bullet
void RecolorBullet(bullet_t* me, byte from, byte to) {
	me->fromColor = from;
	me->toColor = to;
}

void CheckRecolor(bullet_t *me){
	switch(me->type){
		case BLT_TORPEDO:
			RecolorBullet(me, 0, 4);
			break;
		case BLT_CHEESEHAMMER:
			RecolorBullet(me, 4, 5);
			break;
		case BLT_EVILHAMMER:
			RecolorBullet(me, 4, 5);
			break;
		case BLT_SHARK:
			RecolorBullet(me, 1, 4);
			break;
		case BLT_FREEZE:
		case BLT_FREEZE2:
			RecolorBullet(me, 1, 7);
			break;
		default:
			UncolorBullet(me);
			break;
	}
}

// Easy way to set initial bullet variables
void SetBulletVars(bullet_t* me, int dx, int dy, int dz, int z, int timer, byte type) {
	byte f;
	switch (type) {
	default:
		f = me->facing;
		break;
	case BD_EIGHT:
		f = me->facing * 32;
		break;
	case BD_SIXTEEN:
		f = me->facing * 16;
		break;
	}
	CheckRecolor(me);
	me->anim = 0;
	me->dx = Cosine(f) * dx;
	me->dy = Sine(f) * dy;
	me->dz = dz;
	me->z = z;
	me->timer = timer;
}

void SetMissileFacing(byte &facing){
	byte f;
	f = facing*2;
	f += Random(5) - 2;
	if (f < 0)
		f += 16;
	facing = (byte)(f & 15);
}

void SetTorpedoFacing(byte &facing, word &target){
	byte f;
	f=facing*32*16;
	f+=Random(33)-16;
	if(f<0)
		f+=256*16;
	target=f%(256*16);
	facing=target/256;
}

void SetSplatFacing(byte &facing){
	facing=((facing+16)&255)/32;
}

void SetMissileOffset(int &x, int &y){
	x += ((Random(17) - 8) << FIXSHIFT);
	y += ((Random(17) - 8) << FIXSHIFT);
}

void SetFlameOffset(int &x, int &y, byte facing){
	x += ((Random(3) - 1) << FIXSHIFT) + Cosine(facing * 32) * 5;
	y += ((Random(3) - 1) << FIXSHIFT) + Sine(facing * 32) * 5;
}

void SetLaserOffset(int &x, int &y){
	x+=((Random(3)-1)<<FIXSHIFT)+FIXAMT/2-Random(FIXAMT);
	y+=((Random(3)-1)<<FIXSHIFT)+FIXAMT/2-Random(FIXAMT);
}

void FireMe(bullet_t *me,int x,int y,byte facing,byte type,byte friendly)
{
	int i,f,x2,y2,z;

	me->friendly=friendly;
	me->type=type;
	me->x=x;
	me->y=y;
	me->facing=facing;
	me->bright=0;
	me->fromColor=255;
	me->toColor=0;
	me->flags=0;

	switch(me->type)
	{
		case BLT_HOLESHOT:
			SetBulletVars(me,8,8,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_COMET:
			SetBulletVars(me,0,0,-FIXAMT*50,400*FIXAMT+Random(300*FIXAMT),255,BD_NONE);
			me->anim=(byte)Random(8);
			break;
		case BLT_SLIME:
			SetBulletVars(me,0,0,0,0,12*16,BD_NONE);
			break;
		case BLT_LIQUIFY:
			SetBulletVars(me,0,0,0,0,6,BD_NONE);
			break;
		case BLT_ICEBEAM:
			SetBulletVars(me,0,0,0,FIXAMT*20,5,BD_NONE);
			break;
		case BLT_DEATHBEAM:
			z=FIXAMT*45;
			if(me->facing==32*0 || me->facing==32*4)
				z=FIXAMT*50;
			SetBulletVars(me,0,0,0,z,5,BD_NONE);
			break;
		case BLT_DEATHBEAM2:
			SetBulletVars(me,0,0,0,FIXAMT*40,8,BD_NONE);
			break;
		case BLT_BIGYELLOW:
			SetBulletVars(me,4,4,0,FIXAMT*30,30*10,BD_NONE);
			break;
		case BLT_YELWAVE:
			SetBulletVars(me,10,10,0,FIXAMT*2,15,BD_EIGHT);
			break;
		case BLT_COIN:
		case BLT_BIGCOIN:
			me->facing = (byte)Random(256);
			f = Random(3) + 1;
			me->dx = -FIXAMT * 4 + Random(FIXAMT * 8);
			me->dy = -FIXAMT * 4 + Random(FIXAMT * 8);
			me->dz = Random(FIXAMT * 6) + FIXAMT * 4;
			me->z = FIXAMT / 2;
			me->timer = 30 * 9 + Random(30 * 4);
			break;
		case BLT_SCANSHOT:
			me->facing=Random(256);
			SetBulletVars(me,4,4,0,me->z,60,BD_NONE);
			break;
		case BLT_SCANNER:
			//me->dx=Cosine(facing*32)*16;
			//me->dy=Sine(facing*32)*16;
			me->facing=facing*32;
			SetBulletVars(me,16,16,0,me->z,60,BD_NONE);
			break;
		case BLT_SWAP:
			me->facing=facing*32;
			SetBulletVars(me,10,10,0,me->z,120,BD_NONE);
			break;
		case BLT_REFLECT:
			SetBulletVars(me,0,0,0,0,9,BD_NONE);
			break;
		case BLT_BOOM:
			SetBulletVars(me,0,0,0,0,7,BD_NONE);
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1800);
			break;
		case BLT_BALLLIGHTNING:
		case BLT_BADLIGHTNING:
			SetBulletVars(me,11,11,0,FIXAMT*20,30,BD_EIGHT);
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
			SetBulletVars(me,12,12,me->dz,FIXAMT*20,30,BD_EIGHT);
			break;
		case BLT_LUNA:
		case BLT_LUNA2:
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
			SetBulletVars(me,12,12,0,FIXAMT*20,30,BD_EIGHT);
			break;
		case BLT_CHEESEHAMMER:
			SetBulletVars(me,14,14,0,FIXAMT*15,50,BD_EIGHT);
			break;
		case BLT_EVILHAMMER:
			SetBulletVars(me,12,12,FIXAMT*10,FIXAMT*20,30,BD_EIGHT);
			break;
		case BLT_BADLUNA:
		case BLT_BADLUNA2:
			SetBulletVars(me,12,12,0,FIXAMT*20,30,BD_EIGHT);
			break;
		case BLT_BADFBALL:
			RecolorBullet(me,5,0);
			SetBulletVars(me,12,12,0,FIXAMT*20,30,BD_EIGHT);
			break;
		case BLT_BOMB:
			SetBulletVars(me,8,8,FIXAMT*5,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_IGNITE:
			SetBulletVars(me,12,12,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_MISSILE:
		case BLT_MAGICMISSILE:
		case BLT_SKULL:
			me->target=65535;
			SetMissileFacing(me->facing);
			SetMissileOffset(me->x,me->y);
			SetBulletVars(me,4,4,0,FIXAMT*20,60,BD_SIXTEEN);
			MakeSound(SND_MISSILELAUNCH,me->x,me->y,SND_CUTOFF,1100);
			break;
		case BLT_TORPEDO:
			me->anim=Random(8);
			me->timer=60;
			me->z=FIXAMT*20;
			me->dz=-6+Random(13);
			f=facing*32*16;
			f+=Random(33)-16;
			if(f<0)
				f+=256*16;
			me->target=f%(256*16);
			me->facing=me->target/256;
			me->x+=((Random(17)-8)<<FIXSHIFT);
			me->y+=((Random(17)-8)<<FIXSHIFT);
			me->dx=0;
			me->dy=0;
			CheckRecolor(me);
			break;
		case BLT_ACID:
		case BLT_SHARK:
		case BLT_FREEZE2:
			SetBulletVars(me,10,10,FIXAMT*4,FIXAMT*10,30,BD_NONE);
			SetSplatFacing(me->facing);
			break;
		case BLT_FREEZE:
			SetBulletVars(me,8,8,FIXAMT*4,FIXAMT*10,30,BD_NONE);
			SetSplatFacing(me->facing);
			break;
		case BLT_QUEENACID:
			SetBulletVars(me,10,10,FIXAMT*8,FIXAMT*10,60,BD_NONE);
			SetSplatFacing(me->facing);
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
			SetFlameOffset(me->x,me->y,me->facing);
			SetBulletVars(me,10,10,-FIXAMT/2,FIXAMT*20,24-Random(4),BD_EIGHT);
			if(Random(5)==0)
				MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,1100);
			break;
		case BLT_FLAME3:
			me->anim = 4;
			me->facing = Random(256);
			SetFlameOffset(me->x,me->y,me->facing);
			SetBulletVars(me,1,1,FIXAMT/2,FIXAMT*20,24-Random(4),BD_NONE);
			break;
		case BLT_SITFLAME:
		case BLT_BADSITFLAME:
			me->facing = Random(256);
			SetFlameOffset(me->x,me->y,me->facing);
			f=Random(FIXAMT*6)+FIXAMT;
			SetBulletVars(me,f/FIXAMT,f/FIXAMT,Random(FIXAMT*4)+FIXAMT,FIXAMT*20,30+Random(30*15),BD_NONE);
			MakeSound(SND_FLAMEGO, me->x, me->y, SND_CUTOFF, 1100);
			break;
		case BLT_LASER:
			me->anim = 0;
			me->timer = 30;
			me->z = FIXAMT * 20 - Random(FIXAMT);
			SetLaserOffset(me->x, me->y);
			me->facing = me->facing * 32 + 4 - Random(9);
			me->dx = Cosine(me->facing) * 24;
			me->dy = Sine(me->facing) * 24;
			me->facing /= 16;
			me->dz = 0;
			MakeSound(SND_BULLETFIRE, me->x, me->y, SND_CUTOFF, 1050);
			break;
		case BLT_LASERBEAM: // man i don't know what to do here
			me->anim = 0;
			me->timer = 30 * 10;
			me->facing = me->facing * 32;
			me->dx = Cosine(me->facing) * 32;
			me->dy = Sine(me->facing) * 24;
			me->x += Cosine(me->facing) * (Random(10));
			me->y += Sine(me->facing) * (Random(8));
			me->dz = 0;
			me->target = 255 + 255 * 256;
			break;
		case BLT_ENERGY:
			SetBulletVars(me,8,8,0,FIXAMT*20,30,BD_NONE);
			break;
		case BLT_MEGABEAM:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*32;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			MakeSound(SND_MEGABEAM,me->x,me->y,SND_CUTOFF,5000);
			break;
		case BLT_MEGABEAM1:
			me->anim=0;
			me->timer=40;
			me->z=FIXAMT*32;
			me->dx=0;
			me->dy=32*FIXAMT;	// always 32 pixels to make a continuous not-too-overlapping beam
			me->dz=0;
			break;
		case BLT_SPORE:
			SetBulletVars(me,4,4,0,FIXAMT*16,16,BD_NONE);
			break;
		case BLT_SHROOM:
			SetBulletVars(me,8,8,0,FIXAMT*32,60,BD_EIGHT);
			break;
		case BLT_GRENADE:
			f=Random(12)+1;
			SetBulletVars(me,f,f,FIXAMT*20,FIXAMT*80,255,BD_NONE);
			break;
		case BLT_ALIENEGG:
			f=Random(12)+1;
			SetBulletVars(me,f,f,FIXAMT*10,FIXAMT*80,60,BD_NONE);
			break;
		case BLT_SHOCKWAVE:
		case BLT_GOODSHOCK:
			SetBulletVars(me,0,0,0,0,7,BD_NONE);
			break;
		case BLT_SNOWBALL:
			SetBulletVars(me,8,8,FIXAMT*4,FIXAMT*30,7,BD_NONE);
			break;
		case BLT_BIGSNOW:
			SetBulletVars(me,8,8,FIXAMT*4,FIXAMT*30,7,BD_NONE);
			break;
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			SetBulletVars(me,0,0,0,0,9,BD_NONE);
			break;
		case BLT_ROCK:
			SetBulletVars(me,6,6,FIXAMT*8,FIXAMT*20,60,BD_EIGHT);
			break;
		case BLT_SPINE:
			SetBulletVars(me,8,8,0,FIXAMT*20,60,BD_NONE);
			me->facing/=16;
			break;
		case BLT_BIGSHELL:
			SetBulletVars(me,12,12,-FIXAMT*3,FIXAMT*50,30,BD_NONE);
			break;
		case BLT_MINIFBALL:
			SetBulletVars(me,12,12,0,FIXAMT*20,30,BD_NONE);
			me->facing/=16;
			break;
		case BLT_ICECLOUD:
			SetBulletVars(me,1,1,-FIXAMT/2,FIXAMT*10,16,BD_NONE);
			MakeSound(SND_FLAMEGO, me->x, me->y, SND_CUTOFF, 1100);
			break;
		case BLT_BIGAXE:
			SetBulletVars(me,11,11,0,FIXAMT*20,90,BD_EIGHT);
			break;
		case BLT_LIGHTNING:
		case BLT_LIGHTNING2:
			me->timer=2;
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
			SetBulletVars(me,9,9,FIXAMT*6,FIXAMT*20,60,BD_EIGHT);
			break;
		case BLT_HARPOON:
			SetBulletVars(me,12,12,FIXAMT,FIXAMT*10,90,BD_EIGHT);
			break;
		case BLT_SLASH:
			SetBulletVars(me,0,0,0,FIXAMT*20,10,BD_NONE);
			break;
		case BLT_MINE:
			me->facing=2;
			SetBulletVars(me,0,0,0,0,50,BD_NONE);
			if(curMap->flags&MAP_UNDERWATER)
				me->z=FIXAMT*10;
			break;
		case BLT_GREEN:
		case BLT_BADGREEN:
			SetBulletVars(me,8,8,0,FIXAMT*20,30,BD_NONE);
			break;
		case BLT_ORBITER:
			SetBulletVars(me,0,0,0,FIXAMT*20,40,BD_EIGHT);
			me->anim=15;
			me->dx=Cosine(me->facing*32)*8+Sine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*8+Cosine(me->facing*32)*4;
			me->target=65535;
			break;
		case BLT_ORBITER2:
			SetBulletVars(me,0,0,0,FIXAMT*20,10,BD_EIGHT);
			me->anim=15;
			me->dx=Cosine(me->facing*32)*8+Sine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*8+Cosine(me->facing*32)*4;
			me->target=65535;
			break;
		case BLT_MINDWIPE:
			SetBulletVars(me,10,10,0,FIXAMT*20,40,BD_NONE);
			break;
		case BLT_PAPER:
			SetBulletVars(me,9,9,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_BUBBLE:
			SetBulletVars(me,5,5,FIXAMT*4,FIXAMT*20,40+Random(30),BD_NONE);
			break;
		case BLT_LILBOOM:
		case BLT_LILBOOM2:
			// Fix the little booms to show proper sprites when fired manually
			me->timer=9;
			break;
		case BLT_BUBBLEPOP:
			// Likewise with bubble pops
			me->timer=10;
			break;
		case BLT_EVILFACE:
			SetBulletVars(me, 0, 0, 0, FIXAMT * 40, 300, BD_NONE);
			me->dx=-FIXAMT+Random(FIXAMT*2);
			me->dy=-FIXAMT-Random(FIXAMT*2);
			break;
		case BLT_ENERGY2:
			SetBulletVars(me,8,8,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_BIGSHOT:
			SetBulletVars(me,6,6,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_FLAMEWALL:
			SetBulletVars(me,0,0,0,0,12,BD_NONE);
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_FLAMESHOT:
			SetBulletVars(me,7,7,0,0,60,BD_NONE);
			me->anim=3;
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_EARTHSPIKE:
		case BLT_EARTHSPIKE2:
			SetBulletVars(me,0,0,0,0,30,BD_NONE);
			break;
		case BLT_GASBLAST:
			SetBulletVars(me,4,4,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_MEGABOOM:
			MakeSound(SND_BIGBOOM,me->x,me->y,SND_CUTOFF,1200);
			SetBulletVars(me,0,0,0,FIXAMT*20,10,BD_NONE);
			break;
		case BLT_WHOOPEE:
			SetBulletVars(me,2,2,0,FIXAMT*20,120,BD_NONE);
			break;
		case BLT_FARLEYGAS: // unsure how to condense this
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->facing=(byte)Random(256);
			me->dx=Cosine(me->facing)*2;
			me->dy=Sine(me->facing)*2;
			me->dz=0;
			break;
		case BLT_CROAKERGAS:
			SetBulletVars(me,4,4,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_CACTUS:
			SetBulletVars(me,16,16,0,FIXAMT*20,30,BD_NONE);
			break;
		case BLT_ICESHARD:
			SetBulletVars(me,12,12,0,FIXAMT*20,30,BD_NONE);
			break;
		case BLT_BOOMERANG:
			me->target=-1;
			SetBulletVars(me,8,8,0,FIXAMT*20,100,BD_NONE);
			break;
		case BLT_ICEWAND:
		case BLT_ICEWAND2:
			me->target=0;
			SetBulletVars(me,12,12,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_ICEWOLFICE:
			me->target=0;
			SetBulletVars(me,10,10,0,FIXAMT*20,70,BD_NONE);
			break;
		case BLT_BATSHOT:
			SetBulletVars(me,12,12,0,FIXAMT*20,30,BD_NONE);
			break;
		case BLT_WOLFSHOT:
			SetBulletVars(me,16,16,0,FIXAMT*20,30,BD_NONE);
			break;
		case BLT_BIGBOOM:
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1200);
			SetBulletVars(me,0,0,0,FIXAMT*20,5,BD_NONE);
			break;
		case BLT_SWAMPGAS:
		case BLT_SWAMPGAS2:
			SetBulletVars(me,0,0,0,FIXAMT*2,3,BD_NONE);
			break;
		case BLT_ROCKETSMOKE:
			SetBulletVars(me,0,0,0,FIXAMT*2,7,BD_NONE);
			break;
		case BLT_HOTPANTS:
			SetBulletVars(me,8,8,0,FIXAMT*2,35,BD_NONE);
			me->target=LockOn3(me->x,me->y,600,me->friendly);
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_HOTDOGFIRE:
			SetBulletVars(me,7,7,0,0,25,BD_NONE);
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_ROCKET:
		case BLT_WIND:
			SetBulletVars(me,0,0,0,FIXAMT*20,60,BD_NONE);
			break;
		case BLT_ORBGRENADE:
			me->anim=0;
			me->timer=120;
			me->z=FIXAMT*60;
			i=Random(8)+1;
			me->dx=Cosine(me->facing)*i;
			me->dy=Sine(me->facing)*i;
			me->dz=20*FIXAMT;
			break;
		case BLT_WOLFSHOCK:
			me->timer=5;
			break;
		case BLT_FOUNTAIN:
			MakeSound(SND_WATERSPURT,me->x,me->y,SND_CUTOFF,200);
			SetBulletVars(me,0,0,0,0,15,BD_NONE);
			break;
		case BLT_WATER:
			me->z=0;
			me->timer=60;
			me->anim=0;
			switch(me->facing)
			{
				case 0:
					me->dx=FIXAMT*19;
					me->dy=0;
					me->dz=FIXAMT*8;
					break;
				case 1:
					me->dx=0;
					me->dy=FIXAMT*17;
					me->dz=FIXAMT*6;
					break;
				case 2:
					me->dx=-FIXAMT*19;
					me->dy=0;
					me->dz=FIXAMT*8;
					break;
				case 3:
					me->dx=0;
					me->dy=- FIXAMT*17;
					me->dz=FIXAMT*6;
					break;
			}
			break;
		case BLT_CLAW:
			SetBulletVars(me,12,12,0,FIXAMT*20,150,BD_NONE);
			break;
	}
}

void QuadMissile(int x,int y,byte facing,byte friendly)
{
	int i;
	byte f;
	bullet_t *me;

	f=(facing*2-2)&15;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			me=&bullet[i];
			me->type=BLT_MISSILE;
			me->friendly=friendly;
			me->x=x;
			me->y=y;
			me->facing=f;
			me->bright=0;
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dz=0;
			me->target=65535;
			me->flags=0;
			me->x+=Cosine(me->facing*16)*10;
			me->y+=Sine(me->facing*16)*10;
			me->dx=Cosine(me->facing*16)*4;
			me->dy=Sine(me->facing*16)*4;
			f=(f+1)&15;
			if(f==facing*2)
				f=(f+1)&15;
			if(f==((facing*2+3)&15))
				break;
		}
	MakeSound(SND_MISSILELAUNCH,x,y,SND_CUTOFF,1100);
}


void LaunchMegabeam(int x,int y,word owner)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,0,BLT_MEGABEAM,GetGuy(owner)->friendly);
			bullet[i].target=owner;
			break;
		}
}

void SpitAcid(int x,int y,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			bullet[i].friendly=friendly;
			bullet[i].type=type;
			bullet[i].x=x;
			bullet[i].y=y;
			bullet[i].facing=facing;
			bullet[i].bright=0;
			bullet[i].anim=0;
			bullet[i].timer=30;
			bullet[i].z=FIXAMT*30;
			bullet[i].dz=FIXAMT*3;
			bullet[i].dx=Cosine(bullet[i].facing)*10;
			bullet[i].dy=Sine(bullet[i].facing)*10;
			bullet[i].facing=((bullet[i].facing+16)&255)/32;
			bullet[i].flags=0;
			break;
		}
}

void FireBullet(int x,int y,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,facing,type,friendly);
			break;
		}
}

bullet_t *GetFireBullet(int x,int y,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,facing,type,friendly);
			return &bullet[i];
		}
	return 0;
}

// Overload that chooses target.
void FireBullet(int x, int y, byte facing, byte type, byte friendly, word target)
{
	int i;

	for (i = 0; i < config.numBullets; i++)
		if (!bullet[i].type)
		{
			FireMe(&bullet[i], x, y, facing, type, friendly);
			bullet[i].target = target;
			break;
		}
}

void FireScanShots(Guy *victim)
{
	int i;
	byte count=0;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],goodguy->x,goodguy->y,(byte)Random(256),BLT_SCANSHOT,goodguy->friendly);
			bullet[i].target=victim->ID;
			count++;
			if(count==8)
				break;
		}
}

void FireBulletZ(int x,int y,int z,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,facing,type,friendly);
			bullet[i].z=z;
			break;
		}
}

// this only fires if there is room in the bullet list PAST a specific point
// this is used for the Megabeam to ensure that all the laser bits stay lined up nicely
void FireBulletAfter(int x,int y,byte facing,byte type,bullet_t *thisone,byte friendly)
{
	int i,j,start;

	start=-1;

	for(j=0;j<config.numBullets;j++)
		if(&bullet[j]==thisone)
		{
			start=j+1;
			break;
		}

	if(start==-1)
		return;

	for(i=start;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,facing,type,friendly);
			if(thisone->fromColor!=255)
				RecolorBullet(&bullet[i],thisone->fromColor,thisone->toColor);
			break;
		}
}

void FireExactBullet(int x,int y,int z,int dx,int dy,int dz,byte anim,byte timer,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			bullet[i].friendly=friendly;
			bullet[i].x=x;
			bullet[i].y=y;
			bullet[i].z=z;
			bullet[i].bright=0;
			bullet[i].dx=dx;
			bullet[i].dy=dy;
			bullet[i].dz=dz;
			bullet[i].anim=anim;
			bullet[i].timer=timer;
			bullet[i].facing=facing;
			bullet[i].type=type;
			bullet[i].target=65535;
			bullet[i].flags=0;
			CheckRecolor(&bullet[i]);
			break;
		}
}

void HammerLaunch(int x,int y,byte facing,byte count,byte flags)
{
	byte angle,newfacing;
	byte type,spd,face,timer;
	int height,dz;

	if(player.cheesePower)
	{
		type=BLT_CHEESEHAMMER;
		face=facing*32;
		spd=14;
		height=FIXAMT*15;
		dz=0;
		timer=50;
	}
	else if(flags&HMR_REFLECT)
	{
		type=BLT_HAMMER2;
		face=facing;
		spd=12;
		height=FIXAMT*20;
		dz=FIXAMT*6;
		timer=30;
	}
	else
	{
		type=BLT_HAMMER;
		face=facing;
		spd=12;
		height=FIXAMT*20;
		dz=FIXAMT*6;
		timer=30;
	}
	if(player.playAs==PLAY_LUNACHIK)
	{
		dz=0;
		if(type==BLT_HAMMER)
			type=BLT_LUNA;
		else if(type==BLT_HAMMER2)
			type=BLT_LUNA2;
		MakeSound(SND_LUNASHOOT,x,y,SND_CUTOFF,1200);
	}
	else if (player.playAs==PLAY_MYSTIC)
	{
		dz=0;
		if (type == BLT_HAMMER)
			type = BLT_FIREBALL;
		else if (type == BLT_HAMMER2)
			type = BLT_FIREBALL2;
		MakeSound(SND_LUNASHOOT, x, y, SND_CUTOFF, 1200);
	}
	else
		MakeSound(SND_HAMMERTOSS,x,y,SND_CUTOFF,1200);
	if(count==1 || count==3 || count==5)	// 1,3,5 have direct forward fire
	{
		angle=facing*32;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(count==2 || count==4)	// these have slight off-angle double forward fire
	{
		angle=facing*32-8;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
		angle=facing*32+8;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(count==3 || count==5)	// these have 45 degree angle fire
	{
		angle=facing*32-32;
		newfacing=((byte)(facing-1))%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
		angle=facing*32+32;
		newfacing=(facing+1)%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(count==4 || count==5)	// these add almost 90 degree off fire
	{
		angle=facing*32-56;
		newfacing=((byte)(facing-2))%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
		angle=facing*32+56;
		newfacing=(facing+2)%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(flags&HMR_REVERSE)
	{
		newfacing=((byte)(facing-4))%8;
		HammerLaunch(x,y,newfacing,count,flags&(~HMR_REVERSE));
	}
}

void HappyFire(int x,int y,byte facing)
{
	byte happyList[]={BLT_HAMMER,BLT_HAMMER2,BLT_MISSILE,BLT_FLAME,BLT_LASER,BLT_BOMB,
					  BLT_BALLLIGHTNING,BLT_BIGSHELL,BLT_BIGAXE,BLT_LIGHTNING,BLT_SPEAR,BLT_SLASH,
					  BLT_GREEN,BLT_TORPEDO,BLT_BUBBLE,BLT_LUNA,BLT_FIREBALL,BLT_ROCKET};
	byte b;

	b=happyList[Random(16)];
	if(b==BLT_BOMB || b==BLT_GREEN || b==BLT_BIGSHELL || b==BLT_BUBBLE)
		facing*=32;
	FireBullet(x,y,facing,b,1);
}

void HappyLaunch(int x,int y,byte facing,byte count,byte flags)
{
	MakeSound(SND_HAMMERTOSS,x,y,SND_CUTOFF,1200);
	if(count==1 || count==3 || count==5)	// 1,3,5 have direct forward fire
	{
		HappyFire(x,y,facing);
	}
	if(count==2 || count==4)	// these have slight off-angle double forward fire
	{
		HappyFire(x,y,facing);
		if(Random(2)==0)
			HappyFire(x,y,(facing-1)&7);
		else
			HappyFire(x,y,(facing+1)&7);
	}
	if(count==3 || count==5)	// these have 45 degree angle fire
	{
		HappyFire(x,y,(facing-1)&7);
		HappyFire(x,y,(facing+1)&7);
	}
	if(count==4 || count==5)	// these add almost 90 degree off fire
	{
		HappyFire(x,y,(facing-2)&7);
		HappyFire(x,y,(facing+2)&7);
	}
	if(flags&HMR_REVERSE)
	{
		HappyLaunch(x,y,(facing+4)&7,count,(flags&(~HMR_REVERSE)));
	}
}

void ShroomFire(int x,int y,byte facing)
{
	byte a;
	int i;

	for(i=0;i<3;i++)
	{
		a=(facing*32+16-Random(33))&255;
		FireExactBullet(x,y,FIXAMT*16,Cosine(a)*6,Sine(a)*6,0,0,20,a,BLT_SPORE,1);
	}
}

void ShroomSpew(int x,int y,byte facing,byte count,byte flags)
{
	MakeSound(SND_MUSHSPORES,x,y,SND_CUTOFF,1200);

	if(count==1 || count==3 || count==5)	// 1,3,5 have direct forward fire
	{
		ShroomFire(x,y,facing);
	}
	if(count==2 || count==4)	// these have slight off-angle double forward fire
	{
		ShroomFire(x,y,facing);
		if(Random(2)==0)
			ShroomFire(x,y,(facing-1)&7);
		else
			ShroomFire(x,y,(facing+1)&7);
	}
	if(count==3 || count==5)	// these have 45 degree angle fire
	{
		ShroomFire(x,y,(facing-1)&7);
		ShroomFire(x,y,(facing+1)&7);
	}
	if(count==4 || count==5)	// these add almost 90 degree off fire
	{
		ShroomFire(x,y,(facing-2)&7);
		ShroomFire(x,y,(facing+2)&7);
	}
	if(flags&HMR_REVERSE)
	{
		ShroomSpew(x,y,(facing+4)&7,count,(flags&(~HMR_REVERSE)));
	}
}

void ReflectShot(void)
{
	reflect=1;
}

void BulletSwap(int sx,int sy,int width,int height,int dx,int dy)
{
	int i;

	sx*=(TILE_WIDTH*FIXAMT);
	sy*=(TILE_HEIGHT*FIXAMT);
	dx*=(TILE_WIDTH*FIXAMT);
	dy*=(TILE_HEIGHT*FIXAMT);
	width*=(TILE_WIDTH*FIXAMT);
	height*=(TILE_HEIGHT*FIXAMT);

	for(i=0;i<config.numBullets;i++)
	{
		if(bullet[i].type)
		{
			if(bullet[i].x>=sx && bullet[i].y>=sy && bullet[i].x<=(sx+width) && bullet[i].y<=(sy+height))
			{
				// in target area, swap
				bullet[i].x+=(-sx+dx);
				bullet[i].y+=(-sy+dy);
			}
			else if(bullet[i].x>=dx && bullet[i].y>=dy && bullet[i].x<=(dx+width) && bullet[i].y<=(dy+height))
			{
				// in other target area, swap
				bullet[i].x+=(-dx+sx);
				bullet[i].y+=(-dy+sy);
			}
		}
	}
}

void ReflectBullets(int x,int y,byte size,byte friendly)
{
	int i;
	int rx,ry,rx2,ry2;

	rx=x-size*FIXAMT;
	rx2=x+size*FIXAMT;
	ry=y-size*FIXAMT;
	ry2=y+size*FIXAMT;

	for(i=0;i<config.numBullets;i++)
	{
		if(bullet[i].type && bullet[i].friendly!=friendly && bullet[i].x>rx &&
			bullet[i].y>ry && bullet[i].x<rx2 && bullet[i].y<ry2)
		{
			if (bullet[i].type == BLT_BADSITFLAME || bullet[i].type == BLT_SITFLAME)
				continue;	// no reflecting those
			bullet[i].friendly=friendly;
			bullet[i].dx=-bullet[i].dx;
			bullet[i].dy=-bullet[i].dy;
			if(bullet[i].type==BLT_ICESPIKE || bullet[i].type==BLT_DIRTSPIKE)
				bullet[i].facing=(bullet[i].facing+4)&7;
		}
	}
}

void RemoveOrbiters(int n,byte f,byte t)
{
	int i;

	if(n==0)
		return;

	for(i=0;i<config.numBullets;i++)
	{
		if(bullet[i].type==BLT_ORBITER && bullet[i].friendly==f && bullet[i].type==t)
		{
			MakeSound(SND_BOMBBOOM,bullet[i].x,bullet[i].y,SND_CUTOFF,2000);
			bullet[i].dx=0;
			bullet[i].dy=0;
			bullet[i].dz=0;
			bullet[i].type=BLT_BOOM;
			bullet[i].timer=7;
			bullet[i].anim=0;
			n--;
			if(n==0)
				return;
		}
	}

}

void MakeRadar(int rx,int ry,byte w)
{
	if(rx==255)
		return;

	rx=(rx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
	ry=(ry*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;

	TrackParticle(w,goodguy->x,goodguy->y,rx,ry);
/*	int camX,camY;

	if(rx==255)
		return;

	GetCamera(&camX,&camY);
	rx=rx*TILE_WIDTH-(camX-320);
	ry=ry*TILE_HEIGHT-(camY-240);

	if(rx<TILE_WIDTH/2)
		rx=TILE_WIDTH/2;

	if(ry<TILE_HEIGHT/2)
		ry=TILE_HEIGHT/2;

	if(rx>640-TILE_WIDTH/2)
		rx=640-TILE_WIDTH/2;

	if(ry>480-TILE_HEIGHT/2)
		ry=480-TILE_HEIGHT/2;

	rx+=(camX-320)+TILE_WIDTH/2;
	ry+=(camY-240)+TILE_HEIGHT/2;
	FireExactBullet(rx*FIXAMT,ry*FIXAMT,FIXAMT,0,0,0,0,20,0,BLT_SCANLOCK,0);
	*/
}

void EraseLasers(int mapx, int mapy)
{
	int i;

	mapx = (mapx * TILE_WIDTH + TILE_WIDTH / 2) * FIXAMT;
	mapy = (mapy * TILE_HEIGHT + TILE_HEIGHT / 2) * FIXAMT;

	for (i = 0; i < config.numBullets; i++)
	{
		if (bullet[i].type == BLT_LASER && abs(bullet[i].x - mapx) < FIXAMT * 64 && abs(bullet[i].y - mapy) < FIXAMT * 48)
			bullet[i].type = BLT_NONE;
	}
}

int CountBullets(byte type)
{
	int count = 0;

	for(int i=0;i<config.numBullets;i++)
		if(bullet[i].type == type)
			++count;

	return count;
}

int CountBulletsInRect(byte type,int x,int y,int x2,int y2)
{
	int count = 0;

	for(int i=0;i<config.numBullets;i++) {
		int tx = bullet[i].x / TILE_WIDTH / FIXAMT;
		int ty = bullet[i].y / TILE_HEIGHT / FIXAMT;
		if(bullet[i].type==type && tx>=x && ty>=y && tx<=x2 && ty<=y2)
			++count;
	}

	return count;
}

void ChangeBullet(byte fx,int x,int y,int type,int newtype)
{
	for(int i=0;i<config.numBullets;i++)
		if(bullet[i].type != newtype && ((type != 0 && bullet[i].type == type) || (type == 0 && bullet[i].type != 0)))
			if (x == 255 || ((bullet[i].x >> FIXSHIFT)/TILE_WIDTH == x && (bullet[i].y >> FIXSHIFT)/TILE_HEIGHT == y))
			{
				int dx = bullet[i].dx, dy = bullet[i].dy, f = bullet[i].facing;

				if (type == BLT_LASER)
					f /= 2;

				if (BulletFacingType(newtype) == 0)
					f = 0;
				else if (BulletFacingType(newtype) == 7 && BulletFacingType(type) == 255)
					f /= 32;
				else if (BulletFacingType(newtype) == 255 && BulletFacingType(type) == 7)
					f *= 32;

				FireMe(&bullet[i],bullet[i].x,bullet[i].y,f,newtype,bullet[i].friendly);
				bullet[i].dx = dx;
				bullet[i].dy = dy;
				if (fx)
					BlowSmoke(bullet[i].x, bullet[i].y, 0, FIXAMT/8);
			}
}

void ChangeBulletColor(byte fx,int x,int y,int type,int colCode)
{
	byte fromCol, toCol;

	fromCol = colCode % 256;
	toCol = colCode / 256;

	for(int i=0;i<config.numBullets;i++){
		if( type==0 || bullet[i].type==0 || bullet[i].type!=type)
			continue;
		if (x == 255 || ((bullet[i].x >> FIXSHIFT)/TILE_WIDTH == x && (bullet[i].y >> FIXSHIFT)/TILE_HEIGHT == y))
		{
			bullet[i].fromColor = fromCol;
			bullet[i].toColor = toCol;
		}
	}
}

// TORPEDO, LASER
static const byte bulletFacingType[] = {
	0,  	// BLT_NONE    0
	7,  	// BLT_HAMMER  1
	7,  	// BLT_HAMMER2 2	// this is a hammer with reflection
	7,  	// BLT_MISSILE 3
	7,  	// BLT_FLAME   4
	7,  	// BLT_LASER	5
	255,  	// BLT_ACID	6
	255,  	// BLT_BOMB	7
	0,  	// BLT_BOOM	8
	255,  	// BLT_ENERGY  9
	0,  	// BLT_MEGABEAM 10		// this is a ball of energy that launches off a megabeam1
	0,  	// BLT_MEGABEAM1 11	// this is a huge laser beam (downward)
	0,  	// BLT_MEGABEAM2 12	// this is the laser hitting an object (just a visual effect)
	7,  	// BLT_FLAME2	13	// just like flame, except it is anti-Bouapha
	255,  	// BLT_SPORE	14
	7,  	// BLT_SHROOM  15
	255,  	// BLT_GRENADE 16	// energy grenade, an enemy weapon
	0,  	// BLT_YELBOOM 17	// yellow explosion made by energy grenade
	0,  	// BLT_SHOCKWAVE 18	// purple shockwave, for Super Zombie stomp
	0,  	// BLT_LILBOOM 19	// explosion made by missile
	255,  	// BLT_SNOWBALL 20
	255,  	// BLT_BIGSNOW  21
	7,  	// BLT_ICESPIKE 22	// spike juts out of the ground
	7,  	// BLT_ROCK	23
	255,  	// BLT_SPINE	24 // cactus spine
	7,  	// BLT_EVILHAMMER 25 // a grey hammer that is anti-bouapha
	255,  	// BLT_BIGSHELL 26	// Bouapha's power armor shoots these
	7,  	// BLT_BIGAXE	27	// Bouapha weapon
	0,  	// BLT_LIGHTNING 28 // Bouapha weapon
	7,  	// BLT_SPEAR	29	// Bouapha's version of the pygmy spear
	0,  	// BLT_SLASH	30	// Bouapha's machete slash
	0,  	// BLT_MINE	31	// Bouapha's mines
	7,  	// BLT_BADSPEAR 32	// pygmy-thrown spear
	7,  	// BLT_ORBITER  33	// guy that flies around Bouapha shooting
	255,  	// BLT_GREEN	34	// friendly green bullets
	7,  	// BLT_BALLLIGHTNING 35
	0,  	// BLT_LIGHTNING2 36
	7,  	// BLT_MINDWIPE	37
	0,  	// BLT_REFLECT	38
	7,  	// BLT_SWAP	39
	255,  	// BLT_SHARK	40
	7,  	// BLT_ORBITER2 41	// orbit bomber
	7,  	// BLT_HARPOON	42	// a spear, underwater - only difference is it sinks much more slowly
	7,  	// BLT_SCANNER	43	// what the scanner scans with
	255,  	// BLT_SCANSHOT 44	// the shots the scanner fires after scanning
	7,  	// BLT_TORPEDO	45	// Bouapha's minisub shoots them
	7,  	// BLT_DIRTSPIKE 46	// ice spike in brown
	255,  	// BLT_PAPER	47
	0,  	// BLT_SCANLOCK 48	// the scanner locked onto a guy
	255,  	// BLT_BUBBLE	49
	255,  	// BLT_FREEZE	50
	0,  	// BLT_BUBBLEPOP 51	// a bubble that is popping, just visual effect
	0,  	// BLT_LILBOOM2 52		// a harmless lilboom
	7,  	// BLT_CHEESEHAMMER 53	// enhanced hammers
	255,  	// BLT_FREEZE2	54		// a freeze bullet that drops like acid bullets and splats
	7,  	// BLT_LUNA	55		// lunachick's bullets
	7,  	// BLT_LUNA2	56		// lunachick's bullets with wall-bounce power,
	7,		// fireball
	7,		// fireball 2
	7,		// magic missile
	7,		// yellow wave
	255,	// big yellow
	255,	// coin
	0,		// ice cloud
	7,		// mini fireball
	255,	// big coin
	0,		// liquify
	0,		// good shockwave
	7,		// skull missile
	7,		// ice beam
	0,		// ice beam 2
	7,		// death beam
	0,		// death beam 2
	0,		// slime
	0,		// comet
	0,		// comet boom
	255,	// BLT_SITFLAME	59
	255,	// BLT_FLAME3	60	// floaty flame, drifts up
	255,	// BLT_IGNITE	61	// flamebringer shot that ignites the target hit
	255,	// BLT_HOLESHOT 62	// black hole shot flying
	0,		// BLT_BLACKHOLE 63	// black hole existing
	255,	// BLT_BADGREEN 64
	255,	// BLT_BADSITFLAME 65
	7,		// BLT_LASERBEAM
	255,	// BLT_ENERGY2
	255,
	255,
	255,
	0,		// megaboom
	255,
	255,
	255,
	255,
	255,
	255,
	0,		// big bomb boom
	255,	// hot pants
	0,		// swamp gas
	255,
	255,
	255,
	0,		// rocket smoke
	255,
	255,	// "bat shot"
	255,	
	255,	
	255,	
	255,	
	0,		// fountain	
	255,	
	0,	
	255,	// evil face	
	255,	
	255,	// wolf shot	
	255,	// wolf claw	
	255,	
	0,	
	255,	
	255,	
	255,	
	255,	
	7,  	// BLT_LUNA	55
	7,		// fireball
	7,  	
	255,  	
	7,  	
};

byte BulletFacingType(byte type)
{
	return bulletFacingType[type];
}

byte GetBulletAttackType(void)
{
	return attackType;
}

void SaveBullets(FILE *f)
{
	int i;

	fwrite(&config.numBullets,1,sizeof(int),f);
	for(i=0;i<config.numBullets;i++)
	{
		fwrite(&bullet[i].type,1,sizeof(byte),f);
		if(bullet[i].type!=0)
		{
			fwrite(&bullet[i],sizeof(bullet_t),1,f);
		}
	}
}

void LoadBullets(FILE *f)
{
	int i;

	fread(&config.numBullets,1,sizeof(int),f);
	for(i=0;i<config.numBullets;i++)
	{
		fread(&bullet[i].type,1,sizeof(byte),f);
		if(bullet[i].type!=0)
		{
			fread(&bullet[i],sizeof(bullet_t),1,f);
		}
	}
}

void BurnHay(int x, int y)
{
	int i, j, k;

	x /= (TILE_WIDTH * FIXAMT);
	y /= (TILE_HEIGHT * FIXAMT);

	for (i = x - 1; i <= x + 1; i++)
		for (j = y - 1; j <= y + 1; j++)
		{
			if (i < 0 || j < 0 || i >= curMap->width || j >= curMap->height)
				return;

			if (curMap->map[i + j * curMap->width].item == ITM_HAYBALE)	// hay or minecart blocks
			{
				if (curMap->map[i+j*curMap->width].item == ITM_HAYBALE)
				{
					FireBullet((i * TILE_WIDTH + TILE_WIDTH / 2) * FIXAMT, (j * TILE_HEIGHT + TILE_HEIGHT / 2) * FIXAMT, 0, BLT_BADSITFLAME, 0);
				}
				curMap->map[i + j * curMap->width].item = 0;
				for (k = 0; k < 4; k++)
					BlowSmoke((i * TILE_WIDTH + TILE_WIDTH / 2) * FIXAMT, (j * TILE_HEIGHT + TILE_HEIGHT / 2) * FIXAMT, 5, FIXAMT);
				MakeSound(SND_FLAMEGO, (i * TILE_WIDTH + TILE_WIDTH / 2) * FIXAMT, (j * TILE_HEIGHT + TILE_HEIGHT / 2) * FIXAMT, SND_CUTOFF, 500);
			}
		}
}
