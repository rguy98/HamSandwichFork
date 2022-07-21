void AI_Bonehead2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	// Gangrenes glow
	if(me->aiType==MONS_GANGRENE)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,12,16);

	BasicAI(me,SND_SKELOUCH,SND_SKELDIE,map,world,goodguy);

	if(me->aiType==MONS_SKULLY && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==8)
			me->frmAdvance=256;
		if(me->seq==ANIM_ATTACK && me->aiType==MONS_SKULLY)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			me->dx+=Cosine(me->facing*32)*2;
			me->dy+=Sine(me->facing*32)*2;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,5,map,world);
		}
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,5,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->aiType==MONS_REDBONE)
				FireBullet(x,y,me->facing*32,BLT_ENERGY2,me->friendly);
			else if(me->aiType==MONS_GANGRENE)
				FireBullet(x,y,me->facing*32,BLT_GASBLAST,me->friendly);
			else if(me->aiType==MONS_SKULLY)
			{
					FireBullet(x,y,(byte)((me->facing*32-16+Random(32))&255),BLT_FLAMESHOT,me->friendly);
					FireBullet(x,y,(byte)((me->facing*32-16+Random(32))&255),BLT_FLAMESHOT,me->friendly);
					FireBullet(x,y,(byte)((me->facing*32-16+Random(32))&255),BLT_FLAMESHOT,me->friendly);
			}
			me->reload=5;
			me->mind1=1;
		}
		if(me->seq==ANIM_A2 && me->aiType==MONS_NUMBSKULL && me->reload==0 && me->frm>=2)
		{
			i=Random(TILE_WIDTH)+TILE_WIDTH*(me->frm-1);
			x=me->x+Cosine(me->facing*32)*i;
			y=me->y+Sine(me->facing*32)*i;
			FireBullet(x,y,me->facing*32,BLT_FLAMEWALL,me->friendly);
			me->reload=3;
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->reload==0)
		{
			// raise the dying
			me->reload=5;
			RaiseBaddies(me,GRP_SKELETON);
		}
		return;	// can't do nothin' right now
	}

	// randomly decide to point at Bouapha to unnerve him
	// (but only if in pursuit mode, because otherwise you'd point the wrong way)
	// Numbskulls don't do this, since they have a much nastier point they use
	// neither does Agent Skully
	if((!Random(100)) && me->mind==0 && me->aiType!=MONS_NUMBSKULL && me->aiType!=MONS_SKULLY)
	{
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		return;
	}
	// if you aren't wandering and you are a Bonehead or Numbskull, then kick as needed
	if(me->mind==0 && (me->aiType==MONS_NUMBSKULL))
	{
		if(RangeToTarget(me,goodguy)<(48*FIXAMT) && Random(8)==0)
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
	}
	if(me->mind==0 && me->aiType==MONS_SKULLY)
	{
		if(RangeToTarget(me,goodguy)<(200*FIXAMT) && Random(8)==0)
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
	}
	// Redbones and Gangrenes shoot only
	if(me->aiType==MONS_REDBONE || me->aiType==MONS_GANGRENE || me->aiType==MONS_SKULLY)
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
		{
			// spit at him
			MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
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
	// numbskull special attacks
	if(me->aiType==MONS_NUMBSKULL)
	{
		// resurrect any dying skellies
		if(Random(8)==0 && DyingBaddies(me,GRP_SKELETON))
		{
			// resurrect dying skellies
			MakeSound(SND_SKELSUMMON,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A4;
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
		if(Random(32)==0)
		{
			// point to cause flame geyser
			MakeSound(SND_SKELBLAST,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			me->seq=ANIM_A2;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
	}
	WanderAI(me,4,64,40,3,map,world,goodguy);
}

void AI_Zombie2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;

	BasicAI(me,SND_ZOMBIEOUCH,SND_ZOMBIEDIE,map,world,goodguy);
	UpdateColorBright(me);

	if(me->seq==ANIM_DIE && me->aiType==MONS_BOMBIELORD)
	{
		if(Random(5)==0)
		{
			FireBullet(me->x-32*FIXAMT+Random(65)*FIXAMT,me->y-32*FIXAMT+Random(65)*FIXAMT,0,BLT_BIGBOOM,me->friendly);
			FireBullet(me->x-32*FIXAMT+Random(65)*FIXAMT,me->y-32*FIXAMT+Random(65)*FIXAMT,0,BLT_EVILFACE,me->friendly);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			if(me->aiType==MONS_BOMBIE)
				FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
			me->reload=2;
		}
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			if(me->aiType==MONS_FSTZOMBIE){
				me->dx=Cosine(me->facing*32)*16;
				me->dy=Sine(me->facing*32)*16;
			}
			else{
				me->dx=Cosine(me->facing*32)*8;
				me->dy=Sine(me->facing*32)*8;
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm>6)
		{
			Dampen(&me->dx,FIXAMT/2);
			Dampen(&me->dy,FIXAMT/2);
			if(me->frm==11 && me->reload==0 && (me->aiType==MONS_BOMBIE || me->aiType==MONS_BOMBIELORD))	// bombie
			{
				// blow self up
				if(me->aiType==MONS_BOMBIE)
				{
					me->seq=ANIM_DIE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->dx=0;
					me->dy=0;
					me->hp=0;
				}
				return;
			}
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
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && goodguy)
		{
			if(me->aiType==MONS_ZOMBIELORD)
			{
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				FireBullet(x,y,me->facing*32,BLT_EARTHSPIKE,me->friendly);
				FireBullet(x,y,((me->facing+6)*32)&255,BLT_EARTHSPIKE,me->friendly);
				FireBullet(x,y,((me->facing+1)*32)&255,BLT_EARTHSPIKE,me->friendly);
				me->reload=5;
				me->mind1=1;
			}
			else
			{
				for (i = 0; i < 12; i++)
				{
					x = me->x - 48 + Random(97) * FIXAMT;
					y = me->y - 48 + Random(97) * FIXAMT;
					g = AddGuy(x, y, FIXAMT * 40, MONS_BOMBIE, me->friendly);
					if (g && !g->CanWalk(g->x, g->y, map, world))
					{
						g->type = MONS_NONE;
						i++;
						continue;
					}
					else
					{
						FireBullet(x, y, 0, BLT_BIGBOOM, me->friendly);
						break;
					}
				}
				me->reload=5;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// heading for Bouapha
	{
		x=128*FIXAMT;
		y=32;
		if(me->aiType==MONS_BOMBIE)
			y=16;

		if(RangeToTarget(me,goodguy)<(x) && Random(y)==0 &&
			map->CheckLOS(me->mapx,me->mapy,8,goodguy->mapx,goodguy->mapy))
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
	}
	else if(me->aiType==MONS_BOMBIE)	// bombie
	{
		me->dx=me->dx/2;
		me->dy=me->dy/2;
	}

	if((me->aiType==MONS_ZOMBIELORD || me->aiType==MONS_BOMBIELORD) && RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0)
	{
		// stomp earth at him
		MakeSound(SND_ZOMBIESTOMP,me->x,me->y,SND_CUTOFF,1200);
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
	if(me->seq==ANIM_MOVE)
	{
		switch(me->aiType)
		{
			case MONS_BOMBIE:	// bombie
				me->frmAdvance=128;
				break;
			case MONS_ZOMBIELORD:	// zombie lord
			case MONS_BOMBIELORD:
				me->frmAdvance=48;
				break;
		}
	}
	if(me->aiType!=MONS_BOMBIE)
		WanderAI(me,1,64,40,3,map,world,goodguy);
	else
		WanderAI(me,4,128,10,3,map,world,goodguy);
}

void AI_Bat2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	bullet_t *b;

	BasicAI(me,SND_BATOUCH,SND_BATDIE,map,world,goodguy);

	if(me->aiType==MONS_BAT4 && Random(3)==0)
	{
		b=GetFireBullet(me->x,me->y,(byte)Random(256),BLT_GASBLAST,me->friendly);
		if(b)
		{
			b->dx/=2;
			b->dy/=2;
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,3,map,world);
			me->reload=2;

		}
		if(me->seq==ANIM_A1 && me->frm>1 && goodguy && me->hp>0)
		{
			// diving attack hit check
			x=me->x;
			y=me->y;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,3,map,world);
				me->seq=ANIM_A2;	// bounce off
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->dx=-me->dx/4;
				me->dy=-me->dy/4;
			}
		}
		if(me->seq==ANIM_A3 && me->frm==3 && me->reload==0)
		{
			FireBullet(me->x,me->y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=256;

		return;	// can't do nothin' right now
	}

	if(me->aiType!=MONS_BAT4)
	{
		if(RangeToTarget(me,goodguy)<(48*FIXAMT) && Random(4)==0 &&
			map->CheckLOS(me->mapx,me->mapy,6,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			MakeSound(SND_BATEYES,me->x,me->y,SND_CUTOFF,1200);
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

		if(RangeToTarget(me,goodguy)>(75*FIXAMT) && Random(32)==0 && me->aiType==MONS_BAT2 &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			FaceGoodguy(me,goodguy);
			MakeSound(SND_BATDIVE,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;	// diving attack move
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			return;
		}

		if(RangeToTarget(me,goodguy)>(200*FIXAMT) && Random(32)==0 && me->aiType==MONS_BAT3 &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			MakeSound(SND_BATSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A3;	// shoot
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			FaceGoodguy(me,goodguy);
			return;
		}
	}
	WanderAI(me,6,64,40,3,map,world,goodguy);
}

void AI_Villager(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4 && me->hp==0)
		MakeSound(SND_VILLDIE1+(me->aiType>MONS_VILLAGERM),me->x,me->y,SND_CUTOFF,1200);

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(me->reload==0 && Random(64)==0 && me->mind3==0)	// scared girl doesn't yawn
	{
		if(me->aiType==MONS_VILLAGERM)
			MakeSound(SND_VILLAGERSCRATCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_VILLAGERYAWN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=(byte)(30*4+Random(136));
		return;
	}

	if(!(me->mind2--) || (me->mind1>0 && me->mind3==0))	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind2=(byte)Random(40)+20;
		me->mind=(byte)Random(4);
		if(me->mind==1)
			me->mind=2;
		if(me->mind3 && me->mind>0)
			me->mind=(byte)Random(4)+3;

	}

	if(me->mind3==0 && RangeToTarget(me,goodguy)<FIXAMT*60)
	{
		me->mind=0;
		FaceGoodguy(me,goodguy);
	}

	me->dx=(Cosine(me->facing*32)*me->mind*FIXAMT)/FIXAMT;
	me->dy=(Sine(me->facing*32)*me->mind*FIXAMT)/FIXAMT;
	if(me->mind==0)
	{
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	else
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=me->mind*32;
		}
	}
	me->mind1=0;

	if(me->mind3)
	{
		// follow obnoxiously
		FaceGoodguy(me,goodguy);
	}

	// villagers have lanterns in dark places
	if(map->flags&MAP_TORCHLIT)
		map->DimTorch(me->x,me->y,8);
}

void AI_Frog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_FROGOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_FROGDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->aiType==MONS_FROG4 && Random(3)==0)
	{
		x=me->x-10*FIXAMT+Random(21*FIXAMT);
		y=me->y-10*FIXAMT+Random(21*FIXAMT);
		FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && me->aiType==MONS_FROG)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
			}
			else
			{
				x=me->x+Cosine(me->facing*32)*60;
				y=me->y+Sine(me->facing*32)*60;
				if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
				}
			}
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && me->aiType==MONS_FROG2)
		{
			x=me->x+Cosine(me->facing*32)*10;
			y=me->y+Sine(me->facing*32)*10;
			FireBullet(x,y,me->facing*32-10+Random(21),BLT_ENERGY,me->friendly);
			me->reload=40;
		}
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && me->aiType==MONS_FROG2)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_GASBLAST,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && me->aiType==MONS_FROG3)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_ENERGY2,me->friendly);
			me->reload=30;
		}
		if(me->seq==ANIM_DIE && me->aiType==MONS_FROG2 && me->frm==4)
		{
			for(y=0;y<4;y++)
			{
				x=Random(256);
				FireBullet(me->x,me->y,x,BLT_GASBLAST,me->friendly);
			}
		}
		if(me->seq==ANIM_DIE && me->aiType==MONS_FROG3 && me->frm==4 && me->reload==0)
		{
			FireBullet(me->x,me->y,0,BLT_FLAMEWALL,me->friendly);
			me->reload=5;
		}

		return;	// can't do nothin' right now
	}

	if(me->mind==0 && me->aiType!=MONS_FROG4)	// heading for Bouapha
	{
		if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0 && me->aiType==MONS_FROG)
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
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
		if(RangeToTarget(me,goodguy)<(400*FIXAMT) && Random(8)==0 && me->aiType==MONS_FROG2 && me->reload==0)
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
	}

	WanderAI(me,3,64,40,3,map,world,goodguy);
}

void AI_Mummy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,z;
	Guy *g;

	BasicAI(me,SND_ZOMBIEOUCH,SND_ZOMBIEDIE,map,world,goodguy);

	if(me->aiType==MONS_MUMMY5 && me->seq==ANIM_DIE && Random(5)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,12,map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && me->aiType!=MONS_PHARAOHF)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			if(me->aiType!=MONS_MUMMY5)
			{
				FireBullet(x,y,me->facing*32,BLT_ENERGY2,me->friendly);
				if(me->aiType==MONS_PHARAOHG)
				{
					FireBullet(x,y,(me->facing*32+240)&255,BLT_ENERGY2,me->friendly);
					FireBullet(x,y,(me->facing*32+16)&255,BLT_ENERGY2,me->friendly);
				}
			}
			else
				FireBullet(x,y,me->facing*32,BLT_WIND,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm>=3 && me->frm<=5 && me->reload==0 && me->aiType==MONS_PHARAOHF)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			z=me->facing*32-12+Random(25)+256;
			FireBullet(x,y,z&255,BLT_ENERGY2,me->friendly);
		}
		if(me->seq==ANIM_A2 && me->frm==4 && me->reload==0)
		{
			g=AddGuy(me->x-FIXAMT*50,me->y+FIXAMT*10,0,MONS_RAGMAN+2*(me->aiType==MONS_MUMMY5),me->friendly);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
			g=AddGuy(me->x+FIXAMT*50,me->y+FIXAMT*10,0, MONS_RAGMAN+2*(me->aiType==MONS_MUMMY5),me->friendly);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1==0)
		me->mind1=70;
	else
	{
		me->mind1--;
		if(me->mind1==0)
		{
			if((me->aiType==MONS_PHARAOHG || me->aiType==MONS_MUMMY5) && Random(4)==0)
			{
				MakeSound(SND_MUMMYSUMMON,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
			}
		}
	}

	if(RangeToTarget(me,goodguy)<(72*FIXAMT) && Random(8)==0 &&
		map->CheckLOS(me->mapx,me->mapy,4,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_MUMMYSMACK,me->x,me->y,SND_CUTOFF,1200);
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

	x=64;
	if(me->aiType==MONS_PHARAOHF)
		x=32;
	if(RangeToTarget(me,goodguy)<(360*FIXAMT) && Random(x)==0 &&
		map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// shoot
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

	if(me->aiType==MONS_PHARAOHF && me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	if(me->aiType== MONS_PHARAOHG)
		WanderAI(me,2,1024,2,1,map,world,goodguy);
	else
		WanderAI(me,1,1024,2,1,map,world,goodguy);
}

void AI_EvilTree(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	// initially adjust position
	if(me->mind3==0)
	{
		me->x-=FIXAMT*3;
		me->y+=FIXAMT;
		me->mind3=1;
	}

	BasicAI(me,SND_TREEOUCH,SND_TREEDIE,map,world,goodguy);

	if(me->aiType==MONS_EVILTREE3 && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			if(me->aiType==MONS_EVILTREE2)
			{
				// gas attack
				MakeSound(SND_SKELGAS,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,64,BLT_GASBLAST,me->friendly);
				FireBullet(me->x,me->y,64+16,BLT_GASBLAST,me->friendly);
				FireBullet(me->x,me->y,64-16,BLT_GASBLAST,me->friendly);
				me->reload=30*2;
			}
			else if(me->aiType==MONS_EVILTREE)
			{
				// summon frog
				MakeSound(SND_TREESUMMON,me->x,me->y,SND_CUTOFF,1200);
				x=me->x;
				y=me->y+40*FIXAMT;
				g=AddGuy(x,y,FIXAMT*10,MONS_FROG3,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				me->reload=30*3;
			}
			else
			{
				// summon frog
				MakeSound(SND_TREESUMMON,me->x,me->y,SND_CUTOFF,1200);
				x=me->x;
				y=me->y+40*FIXAMT;
				g=AddGuy(x,y,FIXAMT*10,MONS_FROG4,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				me->reload=30*3;
			}
		}
		if(me->seq==ANIM_DIE && me->frm==0 && me->frmTimer<64)
		{
			for(int i = 0; i < 12; i++)
			{
				StinkySteam(me->x - FIXAMT * 20 + Random(FIXAMT * 40), me->y - FIXAMT * 20 + Random(FIXAMT * 40),
					me->z + FIXAMT * 40, FIXAMT * 2);
			}
		}
		return;	// can't do nothin' right now
	}


	if(me->aiType==MONS_EVILTREE)
	{
		// launch frogs when player is very near
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && me->reload==0 && Random(16)==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
	else if(me->aiType==MONS_EVILTREE3)
	{
		// launch frogs when player is near
		if(RangeToTarget(me,goodguy)<(400*FIXAMT) && me->reload==0 && Random(16)==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
	else
	{
		// launch gas at a standard rate
		if(me->reload==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
}

void AI_Wolfman(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	if(me->aiType==MONS_WOLF2 && me->mind2>0)
	{
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,4,2);
	}

	BasicAI(me,SND_WOLFOUCH,SND_WOLFDIE,map,world,goodguy);

	if(me->mind3 && me->mind2==0)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,8,map,world);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_ENERGY2,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,8,map,world);
			}
		}
		if(me->seq==ANIM_A2)
		{
			// constantly turn toward goodguy
			FaceGoodguy2(me,goodguy);
			if(me->frm<15 && me->frm>4)
			{
				x=Cosine(me->facing*32)*12;
				y=Sine(me->facing*32)*12;
			}
			else
			{
				x=0;
				y=0;
			}
			me->dx-=x;
			Dampen(&me->dx,FIXAMT+FIXAMT/2);
			me->dx+=x;
			me->dy-=y;
			Dampen(&me->dy,FIXAMT+FIXAMT/2);
			me->dy+=y;
		}
		if(me->seq==ANIM_A3 && me->frm==6 && me->reload==0)
		{
			// howl effect
			me->reload=10;
			if(me->aiType==MONS_WOLF2)
			{
				// go fast for 3 seconds
				me->mind2=30*3;
			}
			else // dire wolf summons dogboys
			{
				// summon one randomly off each edge of the screen
				// left
				GetCamera(&x,&y);
				x=x-320-80-Random(320);
				y=y-240+Random(480);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLF,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				// right
				GetCamera(&x,&y);
				x=x+320+80+Random(320);
				y=y-240+Random(480);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLF,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				// top
				GetCamera(&x,&y);
				x=x-320+Random(640);
				y=y-240-80-Random(240);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLF,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				// bottom
				GetCamera(&x,&y);
				x=x-320+Random(640);
				y=y+240+80+Random(240);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLF,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
			}
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=190;
		return;	// can't do nothin' right now
	}

	// dire wolves don't pounce
	if(me->aiType!=MONS_WOLF3 && RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(12)==0 &&
		map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_WOLFPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=160;
		me->action=ACTION_BUSY;
		me->reload=0;
		me->dx=0;
		me->dy=0;
		return;
	}
	// dogboys don't spit
	if(RangeToTarget(me,goodguy)<(360*FIXAMT) && Random(128)==0 &&
		map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// shoot
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		MakeSound(SND_WOLFSPIT,me->x,me->y,SND_CUTOFF,1200);
		FaceGoodguy(me,goodguy);
		return;
	}
	// only dire wolves charge
	if(me->aiType==MONS_WOLF3 && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0 &&
		map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// charge!!!
		MakeSound(SND_WOLFCHARGE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A2;
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
	// Dire Wolf and Wolfman howl
	if(me->mind3==0 && me->mind2==0 && Random(128)==0 &&
		map->CheckLOS(me->mapx,me->mapy,20,goodguy->mapx,goodguy->mapy))
	{
		// howl
		me->mind3=30*5;
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A3;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}
	if(me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	if(me->aiType==MONS_WOLF2)
		WanderAI(me,5,1024,2,1,map,world,goodguy);
	else
		WanderAI(me,4,1024,2,1,map,world,goodguy);
}

void AI_Swampdog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,z;

	BasicAI(me,SND_SWAMPOUCH,SND_SWAMPDIE,map,world,goodguy);

	if(me->mind)
		me->mind--;

	if(me->aiType==MONS_SWAMPDOG4 && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*20+Random(FIXAMT*41);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,8,map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A3 && me->frm==6 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;

			if(me->aiType==MONS_SWAMPDOG3 || me->aiType==MONS_SWAMPDOG4)
			{
				if(me->mind3&1)
				{
					z=(me->facing+8)*32-32;
					z+=(me->mind3-1)*8;
					z=z&255;
				}
				else
				{
					z=(me->facing+8)*32+32;
					z-=(me->mind3-1)*8;
					z=z&255;
				}
				if(me->aiType==MONS_SWAMPDOG4)
					FireBullet(x,y,z,BLT_BIGSHOT,me->friendly);
				else
					FireBullet(x,y,z,BLT_ENERGY,me->friendly);
				if(me->mind3>1)
				{
					// repeat fire
					me->mind3-=2;
					me->frm=4;
				}
				me->reload=3;
			}
			else
			{
				FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
				me->reload=5;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind2==0)	// unalert
	{
		if(RangeToTarget(me,goodguy)<128*FIXAMT || me->ouch>0)
		{
			// seen goodguy/got shot - ALERT!!
			me->mind2=1;
			me->action=ACTION_BUSY;
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->mind=30*5;
			MakeSound(SND_SWAMPALERT,me->x,me->y,SND_CUTOFF,1200);
		}
		else
		{
			if(Random(64)==0)
				me->facing=(me->facing+7+(byte)Random(3))&7;

			if(me->mind1==0)	// standing
			{
				me->dx=0;
				me->dy=0;
				if(me->seq!=ANIM_IDLE)
				{
					me->seq=ANIM_IDLE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
				}
				if(me->mind==0)
				{
					// plod around a bit
					me->mind=(byte)Random(30*4);
					me->mind1=1;
				}
			}
			else	// plodding
			{
				me->dx=Cosine(me->facing*32)*1;
				me->dy=Sine(me->facing*32)*1;
				if(me->seq!=ANIM_MOVE)
				{
					me->seq=ANIM_MOVE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
				}
				if(me->mind==0)
				{
					// stand around a bit
					me->mind=(byte)Random(30*2);
					me->mind1=0;
				}
			}
		}
	}
	else // ALERT
	{
		if(me->ouch)
			me->mind=30*5;	// force alertness when injured

		if(me->mind==0)
		{
			// time to be bored again
			DoMove(me,ANIM_A4,64,1,0,0);
			me->mind2=0;
		}
		if(me->seq==ANIM_A5 && me->frm>1)
			me->frm=0;

		if(me->seq==ANIM_IDLE)
		{
			// can't stand still, must charge
			FaceGoodguy(me,goodguy);
			if(me->aiType!=MONS_SWAMPDOG3)
			{
				MakeSound(SND_SWAMPCHARGE,me->x,me->y,SND_CUTOFF,1200);
				DoMove(me,ANIM_A2,128,0,Cosine(me->facing*32)*5,Sine(me->facing*32)*5);
			}
			else	// Shambling Thing does stand still
				DoMove(me,ANIM_A5,128,0,0,0);
		}
		if(RangeToTarget(me,goodguy)<(72*FIXAMT) && Random(8)==0 &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			FaceGoodguy(me,goodguy);
			MakeSound(SND_SWAMPJAB,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_ATTACK,128,1,0,0);
			me->reload=0;
			return;
		}
		if(me->aiType==MONS_SWAMPDOG3)
			x=8;
		else
			x=32;
		if(RangeToTarget(me,goodguy)<(380*FIXAMT) && Random(x)==0 &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			FaceGoodguy(me,goodguy);
			MakeSound(SND_SWAMPSHOOT,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A3,128,1,0,0);
			me->reload=0;
			me->mind3=4*2+(byte)Random(2);

			return;
		}
	}
	if(me->seq==ANIM_A2)
	{
		// charging
		if(me->AttackCheck(32,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
		{
			goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,16,map,world);
		}
	}
}

void AI_Ghost2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	// banshees glow
	if(me->aiType==MONS_GHOST3)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,9);
	// shades make darkness
	else if(me->aiType==MONS_GHOST2)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,-4,8);

	BasicAI(me,SND_GHOSTOUCH,SND_GHOSTDIE,map,world,goodguy);

	if(me->mind3>0)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,8,map,world);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==4 && me->reload==0)
		{
			// banshees teleport randomly
			if(me->aiType==MONS_GHOST3)
			{
				me->x=(me->x-320*FIXAMT)+Random(640*FIXAMT);
				me->y=(me->y-240*FIXAMT)+Random(480*FIXAMT);
				if(me->x<32*FIXAMT)
					me->x=32*FIXAMT;
				if(me->y<32*FIXAMT)
					me->y=32*FIXAMT;
				if(me->x>map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT)
					me->x=map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT;
				if(me->y>map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT)
					me->y=map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT;
			}
			else	// shades teleport to player
			{
				me->x=Random(80*FIXAMT)+40*FIXAMT;
				if(Random(2)==0)
					me->x=-me->x;
				me->x+=goodguy->x;
				me->y=Random(60*FIXAMT)+30*FIXAMT;
				if(Random(2)==0)
					me->y=-me->y;
				me->y+=goodguy->y;
				if(me->x<32*FIXAMT)
					me->x=32*FIXAMT;
				if(me->y<32*FIXAMT)
					me->y=32*FIXAMT;
				if(me->x>map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT)
					me->x=map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT;
				if(me->y>map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT)
					me->y=map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT;
			}
			for(x=0;x<4;x++)
			{
				map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,16,9);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && (me->frm==2 || me->frm==8 || me->frm==14))
		{
			MakeSound(SND_GHOSTSPIN,me->x,me->y,SND_CUTOFF,1200);
			if(me->frm==14)
			{
				me->dx=Cosine(me->facing*32)*6;
				me->dy=Sine(me->facing*32)*6;
			}
		}
		if(me->seq==ANIM_A2 && me->frm==18)
			MakeSound(SND_GHOSTPUNCH,me->x,me->y,SND_CUTOFF,1200);
		if(me->seq==ANIM_A2 && me->frm>=17 && me->frm<=25)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,16,map,world);
			}
		}
		if(me->seq==ANIM_A2 && me->frm>21)
		{
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
		}
		if(me->seq==ANIM_A3 && me->frm==2 && me->reload==0)
		{
			// scream
			me->reload=10;

			MakeColdRingParticle(me->x,me->y,0,180);
			if(RangeToTarget(me,goodguy)<200*FIXAMT)
			{
				goodguy->dx=0;
				goodguy->dy=0;
				Inflict(goodguy,GEF_FROZEN,150);
				MakeSound(SND_FREEZE,x*FIXAMT,y*FIXAMT,SND_CUTOFF,1200);
			}
		}
		return;	// can't do nothin' right now
	}

	// punch
	// Shades only use super punches if player is stone
	if(RangeToTarget(me,goodguy)<(88*FIXAMT) && Random(6)==0 &&
		me->aiType!=MONS_GHOST2)
	{
		// get him!
		MakeSound(SND_GHOSTPUNCH,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,164,1,0,0);
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	// Super punch!!
	if(me->aiType==MONS_GHOST2 && RangeToTarget(me,goodguy)<(160*FIXAMT) &&
		Random(32)==0)
	{
		DoMove(me,ANIM_A2,256,1,0,0);
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	// teleport if non-Ghost
	if(me->aiType!=MONS_GHOST && RangeToTarget(me,goodguy)>(320*FIXAMT) && Random(32)==0)
	{
		MakeSound(SND_GHOSTTPORT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		me->reload=0;
		return;
	}
	// Banshees scream
	if(me->aiType==MONS_GHOST3 && RangeToTarget(me,goodguy)<256*FIXAMT && me->mind3==0 && Random(128)==0)
	{
		// scream
		me->mind3=30*5;
		MakeSound(SND_GHOST2SCREAM,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A3,128,1,0,0);
		me->reload=0;
		return;
	}
	WanderAI(me,4,256,20,3,map,world,goodguy);
}

void AI_Pumpkin2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(RangeToTarget(me,goodguy)>512*FIXAMT)
		return;

	BasicAI(me,SND_PUMPKINOUCH,SND_PUMPKINDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
		return;
	if(RangeToTarget(me,goodguy)<48*FIXAMT)
	{
		// 'acidic'
		goodguy->GetShot(0,0,2,map,world);
	}

	if(me->aiType==MONS_PINKPUMPKN)
	{
		if(Random(10)==0)
			FireBullet(me->x,me->y,(byte)Random(256),BLT_BIGSHOT,me->friendly);
		WanderAI(me, 3, 1024, 5, 3, map, world, goodguy);
	}
	else {
		WanderAI(me, 2, 1024, 5, 3, map, world, goodguy);
	}
}

void AI_Vampire2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	BasicAI(me,SND_VAMPOUCH,SND_VAMPDIE,map,world,goodguy);

	if(me->mind3>0)
		me->mind3--;

	if(me->mind2>0)
		me->mind2--;

	if(me->action==ACTION_BUSY)
	{
		// shove player
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,12,map,world);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==4 && me->reload==0)
		{
			// summon something
			if(me->aiType==MONS_VAMPIRE3)
			{
				// summon Gangrenes, one on either side
				x=me->x-64*FIXAMT;
				y=me->y;
				g=AddGuy(x,y,0,MONS_GANGRENE,me->friendly);
				if(g)
				{
					if(!g->CanWalk(g->x,g->y,map,world))
						g->type=MONS_NONE;	// vanish if this spot is occupied
					else if(!map->CheckLOS(me->mapx,me->mapy,5,g->mapx,g->mapy))
						g->type=MONS_NONE;
					else // do resurrect move
						DoMove(g,ANIM_A3,128,1,0,0);
				}
				x=me->x+64*FIXAMT;
				y=me->y;
				g=AddGuy(x,y,0,MONS_GANGRENE,me->friendly);
				if(g)
				{
					if(!g->CanWalk(g->x,g->y,map,world))
						g->type=MONS_NONE;	// vanish if this spot is occupied
					else if(!map->CheckLOS(me->mapx,me->mapy,5,g->mapx,g->mapy))
						g->type=MONS_NONE;
					else // do resurrect move
						DoMove(g,ANIM_A3,128,1,0,0);
				}
			}
			if(me->aiType==MONS_VAMPIRE4)
			{
				// summon 4 ghosts
				g=AddGuy(me->x,me->y,0,MONS_GHOST,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
				g=AddGuy(me->x,me->y,0,MONS_GHOST,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
				g=AddGuy(me->x,me->y,0,MONS_GHOST,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
				g=AddGuy(me->x,me->y,0,MONS_GHOST,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm==2 && me->reload==0)
		{
			// shoot something
			if(me->aiType==MONS_VAMPIRE2)
			{
				// shoot bullet
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				FireBullet(x,y,me->facing*32,BLT_ENERGY2,me->friendly);
			}
			if(me->aiType==MONS_VAMPIRE3)
			{
				// shoot poison gas
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				FireBullet(x,y,me->facing*32,BLT_GASBLAST,me->friendly);
			}
			if(me->aiType==MONS_VAMPIRE4)
			{
				// shoot a bat
				x=me->x+Cosine(me->facing*32)*32;
				y=me->y+Sine(me->facing*32)*32;
				g=AddGuy(x,y,0,MONS_BAT,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				else if(g)
				{
					g->facing=me->facing;
					DoMove(g,ANIM_A1,128,1,Cosine(me->facing*32)*8,Sine(me->facing*32)*8);
				}
			}
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	// the two lower classes of vampire use shoves
	if(me->type<MONS_VAMPIRE3)
	{
		if(RangeToTarget(me,goodguy)<64*FIXAMT && Random(4)==0 &&
			map->CheckLOS(me->mapx,me->mapy,5,goodguy->mapx,goodguy->mapy))
		{
			FaceGoodguy(me,goodguy);
			if(me->aiType==MONS_VAMPIRE2)
				x=256;
			else
				x=180;
			MakeSound(SND_VAMPSHOVE,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_ATTACK,x,1,0,0);
			return;
		}
	}

	// all non-Junior vampires shoot things
	if(me->type>MONS_VAMPIRE)
	{
		if(RangeToTarget(me,goodguy)<300*FIXAMT && Random(48)==0 && me->mind2==0 &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// Vamp Lords are limited to one shot every second, to reduce the insanity
			if(me->aiType==MONS_VAMPIRE3)
				me->mind2=30;
			else	// 1 second for Draculites
				me->mind2=30;
			FaceGoodguy(me,goodguy);
			MakeSound(SND_VAMPSHOOT,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A2,128,1,0,0);
			return;
		}
	}

	// everything above Bloodsucker has a summon attack
	if(me->type>MONS_VAMPIRE2 && map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		if(Random(320)==0 && me->mind3==0)
		{
			me->mind3=5*30;
			MakeSound(SND_VAMPSUMMON,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A1,128,1,0,0);
			return;
		}
	}

	if(me->aiType==MONS_VAMPIRE2)
		WanderAI(me,7,256,20,3,map,world,goodguy);
	else
		WanderAI(me,5,256,20,3,map,world,goodguy);
}

void AI_Frankenjulie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			if(me->mind<2)
				MakeSound(SND_FRANKOUCH,me->x,me->y,SND_CUTOFF,1200);
			else
				MakeSound(SND_FRANKOUCH2,me->x,me->y,SND_CUTOFF,1200);

			if(me->hp<monsType[me->type].hp/2 && me->mind==0)
			{
				me->mind=1;
				FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
				DoMove(me,ANIM_A1,128,0,0,0);
				ShakeScreen(5);
			}
			if(me->hp<monsType[me->type].hp/4 && me->mind==1)
			{
				me->mind=2;
				FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
				DoMove(me,ANIM_A1,128,0,0,0);
				ShakeScreen(5);
				g=AddGuy(me->x-FIXAMT*150,me->y,0,MONS_HAND,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				g=AddGuy(me->x+FIXAMT*150,me->y,0,MONS_HAND,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
			}
		}
		else
		{
			MakeSound(SND_FRANKDIE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ShakeScreen(120);
			FireBullet(me->x-60*FIXAMT+Random(120*FIXAMT),
						   me->y-60*FIXAMT+Random(120*FIXAMT),0,BLT_MEGABOOM,me->friendly);
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frmTimer<128)
				MakeSound(SND_FRANKHEAD,me->x,me->y,SND_CUTOFF,400);
			if(me->frm<16 && me->frmTimer<128)
			{
				x=(me->x+Cosine(me->frm*16)*80);
				y=(me->y+Sine(me->frm*16)*60);
				FireExactBullet(x,y,FIXAMT*70,Cosine(me->frm*16)*8,Sine(me->frm*16)*8,
					-FIXAMT*2,0,60,me->frm*16,BLT_ENERGY2,me->friendly);
				x=(me->x+Cosine((me->frm+8)*16)*80);
				y=(me->y+Sine((me->frm+8)*16)*60);
				FireExactBullet(x,y,FIXAMT*70,Cosine((me->frm+8)*16)*8,Sine((me->frm+8)*16)*8,
					-FIXAMT*2,0,60,(me->frm+8)*16,BLT_ENERGY2,me->friendly);

			}
		}
		if(me->seq==ANIM_A3)
		{
			if(me->frm>4 && me->frm<20)
			{
				me->dx+=Cosine(me->facing*32);
				me->dy+=Sine(me->facing*32);
				Clamp(&me->dx,6*FIXAMT);
				Clamp(&me->dy,6*FIXAMT);

				x=(me->x+Cosine((me->frm-4)*16)*200)/FIXAMT;
				y=(me->y+Sine((me->frm-4)*16)*160)/FIXAMT;
				if(me->AttackCheck(50,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm-4)*16)*8,Sine((me->frm-4)*16)*8,3,map,world);
				x=(me->x+Cosine((me->frm+4)*16)*200)/FIXAMT;
				y=(me->y+Sine((me->frm+4)*16)*160)/FIXAMT;
				if(me->AttackCheck(50,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm+4)*16)*8,Sine((me->frm+4)*16)*8,3,map,world);
			}
			else if(me->frm>=20)
			{
				me->dx=(me->dx*3)/4;
				me->dy=(me->dy*3)/4;
				if(me->frm==21)
					DoMove(me,ANIM_A1,128,0,0,0);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->frmTimer<128)
		{
			x=me->x-120*FIXAMT;
			y=me->y-20*FIXAMT;
			FireBullet(x,y,128,BLT_ROCKET,me->friendly);
			FireBullet(x,y-30*FIXAMT,148,BLT_ROCKET,me->friendly);
			x=me->x+120*FIXAMT;
			y=me->y-20*FIXAMT;
			FireBullet(x,y,0,BLT_ROCKET,me->friendly);
			FireBullet(x,y-30*FIXAMT,236,BLT_ROCKET,me->friendly);
			MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
		}
		if(me->seq==ANIM_A4 && me->frm==10)
		{
			DoMove(me,ANIM_A2,128,0,0,0);
		}
		return;
	}
	if(RangeToTarget(me,goodguy)<96*FIXAMT)
	{
		// 'acidic'
		goodguy->GetShot(0,0,8,map,world);
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)/4;
	me->dy+=Sine(me->facing*32)/4;
	Clamp(&me->dx,3*FIXAMT);
	Clamp(&me->dy,3*FIXAMT);

	if(me->mind==0)	// have head
	{
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
			}
		}
	}
	if(me->mind==1)	// lost head already
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A1 && me->frm>1))
		{
			DoMove(me,ANIM_A1,128,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				MakeSound(SND_FRANKARMS,me->x,me->y,SND_CUTOFF,1200);
				DoMove(me,ANIM_A3,256,1,me->dx,me->dy);
				FaceGoodguy(me,goodguy);
			}
			if(me->mind1%15==0)
			{
				MakeSound(SND_GRENADE,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,(byte)Random(256),BLT_ORBGRENADE,me->friendly);
			}
		}
	}
	if(me->mind==2)	// lost body too
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A2 && me->frm>1))
		{
			DoMove(me,ANIM_A2,128,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*2;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_A4,128,1,me->dx,me->dy);
			}
		}
	}
}

void AI_BigHand(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	BasicAI(me,SND_PUMPKINOUCH,SND_PUMPKINDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==7 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*40;
			y=me->y+Sine(me->facing*32)*40;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,12,map,world);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(20)==0)
	{
		// get him!
		MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
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

	WanderAI(me,5,512,10,10,map,world,goodguy);
}

void AI_Polterguy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;
	int x,y;
	Guy *g;

	BasicAI(me,SND_POLTEROUCH,SND_POLTERDIE,map,world,goodguy);

	if(me->mind==0)
	{
		me->mind=1;
		for(i=0;i<15;i++)
			LaunchJunk(map,me);
		me->reload=60;
		me->mind3=120+15;
		me->facing=(byte)Random(256);
		me->dx=Cosine(me->facing)*2;
		me->dy=Sine(me->facing)*2;
	}

	if(me->mind1)
	{
		if(me->mind1&1)
			me->dx=-me->dx;
		if(me->mind1&2)
			me->dy=-me->dy;

		if(Random(20)==0)
		{
			me->facing=(byte)Random(256);
			me->dx=Cosine(me->facing)*2;
			me->dy=Sine(me->facing)*2;
		}
		me->mind1=0;
	}

	if(RangeToTarget(me,goodguy)<128*FIXAMT)
		Inflict(goodguy,GEF_POISON,96);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			//LaunchJunk(map,me);
			LaunchJunk(map,me);
			LaunchJunk(map,me);
			me->reload=30*2;

			x=me->x-300*FIXAMT+Random(600)*FIXAMT;
			y=me->y-200*FIXAMT+Random(500)*FIXAMT;
			g=AddGuy(x,y,FIXAMT*50,MONS_GHOST,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
			{
				g->type=MONS_NONE;
			}
			else if(g)
			{
				DoMove(g,ANIM_A1,128,1,0,0);
				g->frm=4;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==15 && me->frmTimer<128)
		{
			Sneeze(me);
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		MakeSound(SND_POLTERGET,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
	}
	if(me->mind3)
	{
		me->mind3--;
		if(me->mind3==0)
		{
			MakeSound(SND_POLTERSNEEZE,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A1,128,1,me->dx,me->dy);
			me->mind3=120;
		}
	}
}

void AI_Junk(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	BasicAI(me,SND_LIGHTSMACK,SND_BOMBBOOM,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ExplodeParticles(PART_DIRT,me->x,me->y,me->z,16);
		}
		return;
	}
	if(me->parent==NULL || me->parent->hp==0)
	{
		me->hp=0;
		DoMove(me,ANIM_DIE,128,1,0,0);
	}

	if(!goodguy || !me->parent)
		return;

	if(RangeToTarget(me,goodguy)<20*FIXAMT)
	{
		goodguy->GetShot(me->dx,me->dy,2,map,world);
	}

	me->frmAdvance=180;
	FaceGoodguy(me,me->parent);
	me->dx+=Cosine(me->facing*32)/2;
	me->dy+=Sine(me->facing*32)/2;
	// circling Polterguy
	if(me->mind==0)
	{
		if(me->x<me->parent->x)
		{
			// on left side
			if(me->y<me->parent->y)
			{
				// upper left
				me->dy+=FIXAMT;
			}
			else
				// lower left
				me->dx+=FIXAMT;
		}
		else
		{
			// on right side
			if(me->y<me->parent->y)
			{
				// upper left
				me->dx-=FIXAMT;
			}
			else
				// lower left
				me->dy-=FIXAMT;
		}
		Dampen(&me->dx,FIXAMT/4);
		Dampen(&me->dy,FIXAMT/4);
		Clamp(&me->dx,FIXAMT*11);
		Clamp(&me->dy,FIXAMT*11);
	}
	else
	{
		// flying outward
		me->mind1--;
		if(me->mind1==0)
			me->mind=0;
	}
}

void AI_WindElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);
	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*6;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm<6)
		{
			me->dx=Cosine(me->facing*32)*(6+me->frm*2);
			me->dy=Sine(me->facing*32)*(6+me->frm*2);
		}
		else if(me->seq==ANIM_ATTACK)
		{
			me->dx/=2;
			me->dy/=2;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*16,Sine(me->facing*32)*16,25,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frmTimer<128)
		{
			x=me->x-400*FIXAMT+Random(800*FIXAMT);
			y=me->y-300*FIXAMT+Random(600*FIXAMT);
			g=AddGuy(x,y,0,MONS_GHOST,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
		}
		if(me->seq==ANIM_A2 && me->frm==2 && me->reload==0)
		{
			FireBullet(me->x,me->y,(me->facing*32-96+256)&255,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32-72+256)&255,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32+96+256)&255,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32+72+256)&255,BLT_WIND,me->friendly);
			me->reload=4;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			FireBullet(x,y,0,BLT_BIGBOOM+(byte)Random(2),me->friendly);
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// summon hordes of ghosts
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		return;
	}

	if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(20)==0)
	{
		// charge at him
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}
	if(Random(50)==0)
	{
		// fire wind missiles
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,3,1024,2,1,map,world,goodguy);
}

void AI_EarthElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			FireBullet(me->x,me->y,(me->facing*32),BLT_EARTHSPIKE2,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32-32+256)&255,BLT_EARTHSPIKE2,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32+32)&255,BLT_EARTHSPIKE2,me->friendly);
			me->reload=5;
		}

		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0)
		{
			for(x=0;x<6;x++)
				FireBullet(me->x,me->y,(byte)Random(256),BLT_ORBGRENADE,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm<9)
		{
			x=me->x+Cosine(me->facing*32)*me->frm*48;
			y=me->y+Sine(me->facing*32)*me->frm*32;
			if(me->frm==8 && me->reload==0)
			{
				me->reload=3;
				FireBullet(x,y,0,BLT_MEGABOOM,me->friendly);
			}
			else if(me->frmTimer<128)
				FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
		}
		if(me->seq==ANIM_DIE)
		{
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
			// doesn't explode
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3)
		me->mind3-=2;
	else
		me->mind3=30*8;

	if(me->mind3==30*5 || me->mind3==30*3)
	{
		// launch grenades
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		return;
	}

	if(me->mind3==30*8 || me->mind3==30*6 || me->mind3==30*2)
	{
		// shoot earth spikes
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}

	FaceGoodguy(me,goodguy);

	me->dx=0;
	me->dy=0;
}

void AI_WaterElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);
	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*6;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			FireBullet(me->x,me->y,me->facing*32+256-8+(byte)Random(16),BLT_ICESHARD,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			g=AddGuy(x,y,FIXAMT*10,MONS_BUBBLE,me->friendly);
			if(g && g->CanWalk(g->x,g->y,map,world))
			{
				g->dx=Cosine(me->facing*32)*4;
				g->dy=Sine(me->facing*32)*4;
			}
			else if(g)
				g->type=MONS_NONE;
			me->reload=4;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(Random(40)==0)
	{
		// fire ice shards
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}
	if(Random(30)==0)
	{
		// fire bubbles
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)*3/2;
	me->dy+=Sine(me->facing*32)*3/2;
	Clamp(&me->dx,FIXAMT*6);
	Clamp(&me->dy,FIXAMT*6);
}

void AI_Bubble(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;

	me->mind3++;
	if(me->mind3==30)
	{
		me->mind3=0;
		me->mind++;
	}

	if(me->seq==ANIM_DIE)
	{
		ExplodeParticles(PART_WATER,me->x,me->y,me->z,4);
		return;
	}
	if(me->mind==15)	// last 15 seconds
	{
		MakeSound(SND_BUBBLEPOP,me->x,me->y,SND_CUTOFF,1200);
		me->hp=0;
		DoMove(me,ANIM_DIE,128,1,0,0);
		me->dx=0;
		me->dy=0;
	}

	me->mind1=0;

	if(RangeToTarget(me,goodguy)<40*FIXAMT)
	{
		goodguy->dx+=me->dx*2;
		goodguy->dy+=me->dy*2;
		FaceGoodguy(me,goodguy);

		me->dx=-Cosine(me->facing*32)*4;
		me->dy=-Sine(me->facing*32)*4;
	}
	if(me->z==0)
		me->dz=FIXAMT*8;
}

void AI_FireElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);

	map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,16,8);

	if(me->ouch && me->action==ACTION_BUSY && me->seq!=ANIM_DIE && me->seq!=ANIM_ATTACK && Random(10)==0)
	{
		// run away from the water
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		me->facing=(byte)Random(8);
		return;
	}
	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*8;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm<6)
		{
			me->dx=Cosine(me->facing*32)*(6+me->frm*2);
			me->dy=Sine(me->facing*32)*(6+me->frm*2);
		}
		else if(me->seq==ANIM_ATTACK)
		{
			me->dx/=2;
			me->dy/=2;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*16,Sine(me->facing*32)*16,20,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==1)
			me->mind2=me->facing*32;
		if(me->seq==ANIM_A1 && me->frm>0 && me->frm<22)
		{
			me->mind2+=8;
			me->facing=me->mind2/32;
			x=me->x+20*Cosine(me->mind2);
			y=me->y+20*Sine(me->mind2);
			FireBullet(x,y,me->mind2,BLT_FLAMESHOT,me->friendly);
		}

		if(me->seq==ANIM_A2 && me->frm>4 && me->frm<9 && me->frmTimer<128)
		{
			x=me->x+Cosine(me->facing*32)*(me->frm-4)*48;
			y=me->y+Sine(me->facing*32)*(me->frm-4)*32;
			if(me->frm==8 && me->reload==0)
			{
				me->reload=3;
				FireBullet(x,y,0,BLT_MEGABOOM,me->friendly);
			}
			else if(me->frmTimer<128)
				FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		return;	// can't do nothin' right now
	}

	x=me->x-40*FIXAMT+Random(80*FIXAMT);
	y=me->y-30*FIXAMT+Random(60*FIXAMT);
	FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);

	if(me->mind3==0 || me->mind3==30*4)
	{
		// unleash flame circle
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		return;
	}

	/*
	if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(20)==0)
	{
		// charge at him
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}
	*/

	if(me->mind3==30*2 || me->mind3==30*6)
	{
		// fire explosion stream
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,5,1024,2,1,map,world,goodguy);
}

void AI_SummonElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,a,b;
	int vampire[4] = { MONS_VAMPIRE,MONS_VAMPIRE2,MONS_VAMPIRE3,MONS_VAMPIRE4 };
	Guy *g;

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);

	if(me->seq==ANIM_DIE && Random(5)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,8,8);

	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*4;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm>0 && me->frm<22 && me->reload==0)
		{
			a=Random(256);
			b=Random(60)+20;
			x=me->x+b*Cosine(a);
			y=me->y+b*Sine(a);

			g=AddGuy(x,y,FIXAMT*40,vampire[me->mind2],me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
			else
			{
				MakeRingParticle(x,y,FIXAMT*40,20,40);
				FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
			}
			me->reload=(me->mind2+1)*4;
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0 || me->mind3==30*4)
	{
		// summon minions
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		me->mind2=(byte)Random(4);
		return;
	}

	WanderAI(me,5,1024,2,1,map,world,goodguy);
}

void AI_Bonkula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	int vampire[4] = { MONS_VAMPIRE,MONS_VAMPIRE2,MONS_VAMPIRE3,MONS_VAMPIRE4 };
	Guy *g;
	dword d;

	if(me->reload)
		me->reload--;

	if(me->ouch==3)
	{
		if(me->hp>0)
			MakeSound(SND_BONKOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BONKDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->ouch)
	{
		x=me->x-40*FIXAMT+Random(80*FIXAMT);
		y=me->y-30*FIXAMT+Random(60*FIXAMT);
		FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);
	}

	if(map->GetTile(me->mapx,me->mapy)->light>0 && me->ouch==0 && me->hp>0)
	{
		d=monsType[me->aiType].flags;
		monsType[me->aiType].flags=0;
		me->GetShot(0,0,map->GetTile(me->mapx,me->mapy)->light,map,world);
		monsType[me->aiType].flags=d;
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
	}

	if (me->reload == 0)
	{
		if (Random(3) == 1)
		{
			x = me->x - 300 * FIXAMT + Random(600) * FIXAMT;
			y = me->y + Random(900) * FIXAMT;
			g = AddGuy(x, y, FIXAMT * 50, MONS_VAMPIRE2 + (byte)Random(3), me->friendly);
			if (g && !g->CanWalk(g->x, g->y, map, world))
				g->type = MONS_NONE;
		}
		me->reload = 2;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==8 && me->reload==0)
		{
			MakeSound(SND_BONKHIT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*60;
			y=me->y+Sine(me->facing*32)*60;

			FindVictim(x>>FIXSHIFT,y>>FIXSHIFT,6*15,Cosine(me->facing*32)*2,
							   Sine(me->facing*32)*2,25,map,world,me->friendly);
			MakeRingParticle(x,y,0,6*15,10*15);
			me->reload=5;
		}

		if(me->seq==ANIM_DIE)
		{
			me->dx=0;
			me->dy=0;
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);
			MakeSound(SND_BONKDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		if(me->seq==ANIM_A1)
		{
			me->facing++;
			if(me->facing>7)
				me->facing=0;
			if(Random(30)==0)
			{
				x=me->x-300*FIXAMT+Random(600)*FIXAMT;
				y=me->y-300*FIXAMT+Random(600)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*50,vampire[(byte)Random(4)],me->type);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// begin spinning
		DoMove(me,ANIM_A1,256,1,0,0);
		me->mind3=60;
	}
	else
	{
		if(me->mind3==60)
		{
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
			me->dx+=-FIXAMT*2+Random(4*FIXAMT);
			me->dy+=-FIXAMT*2+Random(4*FIXAMT);
		}
		me->mind3--;
		if(RangeToTarget(me,goodguy)<200*FIXAMT)
		{
			MakeSound(SND_BONKSWING,me->x,me->y,SND_CUTOFF,2000);
			DoMove(me,ANIM_ATTACK,256,1,0,0);
			me->mind3=0;
		}
	}
}

void AI_Evilizer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int summonTab[] = { MONS_CRAZYBONE,MONS_MUTANT,MONS_CREEPAZOID,MONS_MUMBLE2,MONS_ALIEN,MONS_REDBONE,MONS_WHATSIT,MONS_VAMPIRE,MONS_BAT,
		MONS_EYEGUY,MONS_STAREYBAT,MONS_MADMAN,MONS_WACKO,MONS_GHOST,MONS_SHOCKGHOST,MONS_MUMBLE,MONS_WOLF,MONS_WOLF2,MONS_BOMBIE,MONS_FSTZOMBIE };

	int x,y;
	Guy *g;

	if(me->ouch==4)
		MakeSound(SND_METALSMACK,me->x,me->y,SND_CUTOFF,1200);

	if(!CountMonsters(MONS_STEAMPUMP)&&me->mind==0) // invincible w/ steam pumps
	{
		me->mind=1;
	}

	// mind=whether uncovered or not
	if(me->mind==0)
		me->facing=0;
	else
		me->facing=16;

	// mind1=time to shoot an evilface off
	if(me->mind1==0)
	{
		y=me->y-FIXAMT;
		x=me->x-140*FIXAMT+Random(281*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
		me->mind1=12;
		MakeSound(SND_EVILIZER,me->x,me->y,SND_CUTOFF,300);
	}
	else
	{
		me->mind1--;
		if(!(me->mind1&3))
		{
			y=me->y-FIXAMT;
			x=me->x-140*FIXAMT+Random(281*FIXAMT);
			FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			y=me->y+140*FIXAMT-Random(281*FIXAMT);
			x=me->x-140*FIXAMT+Random(281*FIXAMT);
			FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
		}
	}

	// mind2=grenade timer (faster as it gets hurt more)
	if(me->mind2==0)
		me->mind2=30+me->hp/10;
	else
	{
		me->mind2--;
		if(me->mind2==0)
		{
			x=180*FIXAMT;
			y=180*FIXAMT;
			FireBullet(me->x-x,me->y,64+(byte)Random(128),BLT_ORBGRENADE,me->friendly);
			FireBullet(me->x+x,me->y,192+(byte)Random(128),BLT_ORBGRENADE,me->friendly);
			FireBullet(me->x,me->y-y,128+(byte)Random(128),BLT_ORBGRENADE,me->friendly);
			FireBullet(me->x,me->y+y,(byte)Random(128),BLT_ORBGRENADE,me->friendly);
			MakeSound(SND_GRENADE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	// mind3=missile timer (only works once exposed)
	if(me->mind==1)
	{
		if(me->mind3==0)
			me->mind3=60+me->hp/20;
		else
		{
			me->mind3--;
			if(me->mind3==0)
			{
				x=100*FIXAMT;
				y=40*FIXAMT;
				FireBullet(me->x-x,me->y-y,160,BLT_ROCKET,me->friendly);
				FireBullet(me->x+x,me->y-y,224,BLT_ROCKET,me->friendly);
				FireBullet(me->x-x,me->y+y,96,BLT_ROCKET,me->friendly);
				FireBullet(me->x+x,me->y+y,32,BLT_ROCKET,me->friendly);
				MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
			}
		}

		// reload=summon super duper monsters, only when exposed
		if(me->reload==0)
			me->reload=30*2;
		else
		{
			me->reload--;
			if(me->reload==0)
			{
				x=((Random(map->width-6)+3)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
				y=((Random(map->height-20)+10)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*40,summonTab[Random(20)],me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
					g->type=MONS_NONE;
			}

		}
	}
}

void AI_EvilPump(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	FaceGoodguy3(me,goodguy);

	if(me->ouch==4)
		MakeSound(SND_METALSMACK,me->x,me->y,SND_CUTOFF,1200);

	if(me->seq==ANIM_IDLE && me->frm==8 && me->frmTimer<128)
	{
		FireBullet(me->x, me->y, 0, BLT_SWAMPGAS, me->friendly);
		me->reload=3;
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm==3 || me->frm==5 || me->frm==7) && me->reload==0)
		{
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*48;
			y=me->y+Sine(me->facing*32)*48;
			FireBullet(x,y,(byte)(me->facing*32+240+Random(33)),BLT_BIGSHOT,me->friendly);
			me->reload=3;
			if(me->frm==7)
				me->reload=30*3;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==0)
				me->reload=0;
			x=me->x-20*FIXAMT+Random(40*FIXAMT);
			y=me->y-20*FIXAMT+Random(40*FIXAMT);
			FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
			if(me->frm==4 && me->reload==0)
			{
				me->reload=10;
				FireBullet(me->x,me->y,0,BLT_SWAMPGAS2,me->friendly);
			}
		}
		return;
	}

	if(me->reload==0)
	{
		DoMove(me,ANIM_ATTACK,128,1,0,0);
	}
}

void AI_Larry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;

	//Burn((me->mind2*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT,(me->mind3*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT,0);

	BasicAI(me,SND_WOLFOUCH,SND_WOLFDIE,map,world,goodguy);

	if(me->mind)
		me->mind--;

	if(me->mind<30)
	{
		Burn(me->x,me->y,Random(50*FIXAMT));
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,15,map,world);
			}
			if(me->frm==8 && me->frmTimer<128)
			{
				x=me->x+Cosine(me->facing*32)*48;
				y=me->y+Sine(me->facing*32)*48;
				MakeRingParticle(x,y,0,128,64);
				if(me->AttackCheck(128,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,15,map,world);
				}
				ShakeScreen(15);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && (me->frm==6 || me->frm==7) && me->frmTimer<128)
		{
			FaceGoodguy(me,goodguy);
			x=me->x+Cosine((me->facing+7+2*(7-me->frm))*32)*20;
			y=me->y+Sine((me->facing+7+2*(7-me->frm))*32)*20;
			MakeSound(SND_LARRYDARTS,me->x,me->y,SND_CUTOFF,1200);
			for(i=0;i<10;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*8+256-48+i*(96/10))&255,BLT_CLAW,me->friendly);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,15,map,world);
			}
		}
		if(me->seq==ANIM_DIE)
		{
		}
		return;	// can't do nothin' right now
	}

	if(map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// if goodguy is in sight
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(12)==0)
		{
			// get him!
			MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_ATTACK,128,1,0,0);
			FaceGoodguy2(me,goodguy);
			return;
		}
		if(Random(32)==0)	// fire at any range
		{
			// shoot darts
			MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A1,200,1,0,0);
			FaceGoodguy2(me,goodguy);
			return;
		}
		if(me->mapx==me->mind2 && me->mapy==me->mind3)
		{
		//	me->mind2=goodguy->mapx;
		//	me->mind3=goodguy->mapy;
			SelectDestination(me,goodguy,map,0);
		}
	}
	else
	{
		if(me->mapx==me->mind2 && me->mapy==me->mind3)
			SelectDestination(me,goodguy,map,0);
	}

	if(me->mind1 && (abs(me->dx)>FIXAMT*3 || abs(me->dy)>FIXAMT*3))
	{
		me->mind1=0;
		me->dx=0;
		me->dy=0;
		SelectDestination(me,goodguy,map,1);
	}

	if(me->mind==0)
	{
		me->type=MONS_HUMANLARRY;
		me->aiType=MONS_HUMANLARRY;
		DoMove(me,ANIM_IDLE,128,0,0,0);
		MakeSound(SND_LARRYNORMAL,me->x,me->y,SND_CUTOFF,1800);
		SelectDestination(me,goodguy,map,1);
		me->mind=30*8;
		return;
	}

	if(me->mapx>me->mind2)
		me->dx-=FIXAMT;
	else if(me->mapx<me->mind2)
		me->dx+=FIXAMT;
	if(me->mapy>me->mind3)
		me->dy-=FIXAMT;
	else if(me->mapy<me->mind3)
		me->dy+=FIXAMT;
	Dampen(&me->dx,FIXAMT/4);
	Dampen(&me->dy,FIXAMT/4);
	Clamp(&me->dx,7*FIXAMT);
	Clamp(&me->dy,7*FIXAMT);

	if(me->dx<-FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=5;
		else if(me->dy>FIXAMT)
			me->facing=3;
		else
			me->facing=4;
	}
	else if(me->dx>FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=7;
		else if(me->dy>FIXAMT)
			me->facing=1;
		else
			me->facing=0;
	}
	else
	{
		if(me->dy<-FIXAMT)
			me->facing=6;
		else
			me->facing=2;
	}

	if(me->seq!=ANIM_MOVE)
		DoMove(me,ANIM_MOVE,180,0,me->dx,me->dy);
}

void AI_HumanLarry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;

	if(me->ouch==4)
	{
		if(me->mind>15)
			me->mind-=15;
		else
			me->mind=0;
	}
	if(me->mind)
		me->mind--;

	if(me->mind<30)
	{
		Burn(me->x,me->y,Random(50*FIXAMT));
	}

	BasicAI(me,SND_LARRYOUCH,SND_VILLDIE1,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,10,map,world);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && (me->frm==6 || me->frm==7) && me->frmTimer<128)
		{
			x=me->x+Cosine((me->facing+7)*32)*20;
			y=me->y+Sine((me->facing+7)*32)*20;
			for(i=0;i<3;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*8+256-32+i*(32/6))&255,BLT_CLAW,me->friendly);
			}
			x=me->x+Cosine((me->facing+1)*32)*20;
			y=me->y+Sine((me->facing+1)*32)*20;
			for(i=0;i<3;i++)
			{
				FireBullet(x,y,(me->facing*32-(me->frm-6)*8+256-16+i*(32/6))&255,BLT_CLAW,me->friendly);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,10,map,world);
			}
		}
		if(me->seq==ANIM_DIE)
		{
		}
		return;	// can't do nothin' right now
	}

	if(me->mapx==me->mind2 && me->mapy==me->mind3)
		SelectDestination(me,goodguy,map,0);

	if(me->mind==0)
	{
		me->type=MONS_LARRY;
		me->aiType=MONS_LARRY;
		DoMove(me,ANIM_A2,128,1,0,0);
		MakeSound(SND_LARRYHOWL,me->x,me->y,SND_CUTOFF,1800);
		SelectDestination(me,goodguy,map,1);
		me->mind = 30 * 8;
		return;
	}

	if(me->mapx>me->mind2)
		me->dx-=FIXAMT;
	else if(me->mapx<me->mind2)
		me->dx+=FIXAMT;
	if(me->mapy>me->mind3)
		me->dy-=FIXAMT;
	else if(me->mapy<me->mind3)
		me->dy+=FIXAMT;
	Dampen(&me->dx,FIXAMT/4);
	Dampen(&me->dy,FIXAMT/4);
	Clamp(&me->dx,5*FIXAMT);
	Clamp(&me->dy,5*FIXAMT);

	if(me->dx<-FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=5;
		else if(me->dy>FIXAMT)
			me->facing=3;
		else
			me->facing=4;
	}
	else if(me->dx>FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=7;
		else if(me->dy>FIXAMT)
			me->facing=1;
		else
			me->facing=0;
	}
	else
	{
		if(me->dy<-FIXAMT)
			me->facing=6;
		else
			me->facing=2;
	}

	if(me->seq!=ANIM_MOVE)
		DoMove(me,ANIM_MOVE,256,0,me->dx,me->dy);
}

void AI_Harry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,a,b;
	Guy *g;

	BasicAI(me,SND_WOLFOUCH,SND_WOLFDIE,map,world,goodguy);

	if(me->mind3==0)
		me->mind3=30*4;
	if(me->mind3>1)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,25,map,world);
			}
			if(me->frm==8 && me->frmTimer<128)
			{
				x=me->x+Cosine(me->facing*32)*48;
				y=me->y+Sine(me->facing*32)*48;
				MakeRingParticle(x,y,0,128,64);
				if(me->AttackCheck(128,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,25,map,world);
				}
				ShakeScreen(15);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==6 && me->frmTimer<128)
		{
			x=me->x+Cosine((me->facing+7+2*(7-me->frm))*32)*20;
			y=me->y+Sine((me->facing+7+2*(7-me->frm))*32)*20;
			MakeSound(SND_LARRYDARTS,me->x,me->y,SND_CUTOFF,1200);
			for(i=0;i<10;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*4+256-24+i*(48/10))&255,BLT_CLAW,me->friendly);
			}
			me->reload=5;
		}

		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15 && me->reload==0)
		{
			a=Random(256);
			b=Random(60)+20;
			x=me->x+b*Cosine(a);
			y=me->y+b*Sine(a);

			g=AddGuy(x,y,FIXAMT*40,MONS_WOLF3,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
			else
			{
				MakeRingParticle(x,y,FIXAMT*40,20,40);
				FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
			}
			me->reload=3;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x-20*FIXAMT+Random(40*FIXAMT);
			y=me->y-20*FIXAMT+Random(40*FIXAMT);
			FireBullet(x,y,0,BLT_BIGBOOM,me->friendly);
			if(Random(2)==0)
			{
				y=me->y-FIXAMT*10+Random(FIXAMT*11);
				x=me->x-32*FIXAMT+Random(65*FIXAMT);
				FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<64*FIXAMT && Random(4)==0 &&
		map->CheckLOS(me->mapx,me->mapy,5,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}
	if(Random(32)==0)	// fire at any range
	{
		// shoot darts
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,200,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}

	if(me->mind3==1)
	{
		me->mind3=0;
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1000);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,4,256,20,3,map,world,goodguy);
}

void AI_Ghastly(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;
	int x,y;
	Guy *g;

	BasicAI(me,SND_POLTEROUCH,SND_POLTERDIE,map,world,goodguy);

	if(me->mind==0 && RangeToTarget(me,goodguy)<256*FIXAMT)
	{
		me->mind=1;
		me->reload=60;
		me->mind3=120+15;
		me->facing=(byte)Random(256);
		me->dx=Cosine(me->facing)*2;
		me->dy=Sine(me->facing)*2;
	}

	if(me->mind1)
	{
		if(me->mind1&1)
			me->dx=-me->dx;
		if(me->mind1&2)
			me->dy=-me->dy;

		if(Random(20)==0)
		{
			me->facing=(byte)Random(256);
			me->dx=Cosine(me->facing)*2;
			me->dy=Sine(me->facing)*2;
		}
		me->mind1=0;
	}

	if(Random(4)==0)
		FireBullet(me->x,me->y,(byte)Random(256),BLT_GASBLAST,me->friendly);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			me->reload=30*4;

			for(i=0;i<3;i++)
			{
				x=me->x-30*FIXAMT+Random(61)*FIXAMT;
				y=me->y-20*FIXAMT+Random(51)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*30,MONS_BAT4,me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		if(me->seq==ANIM_A1 && me->frm==15 && me->frmTimer<128)
		{
			for(i=0;i<16;i++)
				FireBullet(me->x,me->y,i*16,BLT_GASBLAST,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		MakeSound(SND_POLTERGET,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
	}
	if(Random(50)==0)
	{
		me->facing=(byte)Random(256);
		me->dx=Cosine(me->facing)*2;
		me->dy=Sine(me->facing)*2;
	}
}


void AI_MiniFrankenjulie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;


	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			MakeSound(SND_FRANKOUCH2,me->x,me->y,SND_CUTOFF,1200);

			if(me->hp<monsType[me->type].hp/2 && me->mind==0)
			{
				me->mind=1;
				FireBullet(me->x,me->y,0,BLT_BIGBOOM,me->friendly);
				DoMove(me,ANIM_A1,256,0,0,0);
			}
			if(me->hp<monsType[me->type].hp/4 && me->mind==1)
			{
				me->mind=2;
				FireBullet(me->x,me->y,0,BLT_BIGBOOM,me->friendly);
				DoMove(me,ANIM_A1,256,0,0,0);
			}
		}
		else
		{
			MakeSound(SND_FRANKDIE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			FireBullet(me->x-60*FIXAMT+Random(120*FIXAMT),
						   me->y-60*FIXAMT+Random(120*FIXAMT),0,BLT_BIGBOOM,me->friendly);
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frmTimer<128)
				MakeSound(SND_FRANKHEAD,me->x,me->y,SND_CUTOFF,400);
			if(me->frm<16 && me->frmTimer<128)
			{
				x=(me->x+Cosine(me->frm*16)*40);
				y=(me->y+Sine(me->frm*16)*30);
				FireExactBullet(x,y,FIXAMT*40,Cosine(me->frm*16)*12,Sine(me->frm*16)*12,
					-FIXAMT*2,0,60,me->frm*16,BLT_ENERGY2,me->friendly);
				x=(me->x+Cosine((me->frm+8)*16)*40);
				y=(me->y+Sine((me->frm+8)*16)*30);
				FireExactBullet(x,y,FIXAMT*40,Cosine((me->frm+8)*16)*12,Sine((me->frm+8)*16)*12,
					-FIXAMT*2,0,60,(me->frm+8)*16,BLT_ENERGY2,me->friendly);
			}
		}
		if(me->seq==ANIM_A3)
		{
			if(me->frm>4 && me->frm<20)
			{
				me->dx+=Cosine(me->facing*32)*3;
				me->dy+=Sine(me->facing*32)*3;
				Clamp(&me->dx,9*FIXAMT);
				Clamp(&me->dy,9*FIXAMT);

				x=(me->x+Cosine((me->frm-4)*16)*100)/FIXAMT;
				y=(me->y+Sine((me->frm-4)*16)*80)/FIXAMT;
				if(me->AttackCheck(25,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm-4)*16)*8,Sine((me->frm-4)*16)*8,3,map,world);
				x=(me->x+Cosine((me->frm+4)*16)*100)/FIXAMT;
				y=(me->y+Sine((me->frm+4)*16)*80)/FIXAMT;
				if(me->AttackCheck(25,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm+4)*16)*8,Sine((me->frm+4)*16)*8,3,map,world);
			}
			else if(me->frm>=20)
			{
				me->dx=(me->dx*3)/4;
				me->dy=(me->dy*3)/4;
				if(me->frm==21)
					DoMove(me,ANIM_A1,256,0,0,0);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->frmTimer<128)
		{
			x=me->x-60*FIXAMT;
			y=me->y-10*FIXAMT;
			FireBullet(x,y,128,BLT_ROCKET,me->friendly);
			x=me->x+60*FIXAMT;
			y=me->y-10*FIXAMT;
			FireBullet(x,y,0,BLT_ROCKET,me->friendly);
			MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
		}
		if(me->seq==ANIM_A4 && me->frm==10)
		{
			DoMove(me,ANIM_A2,256,0,0,0);
		}
		return;
	}
	if(RangeToTarget(me,goodguy)<65*FIXAMT)
	{
		// 'acidic'
		goodguy->GetShot(0,0,10,map,world);
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)/4;
	me->dy+=Sine(me->facing*32)/4;
	Clamp(&me->dx,6*FIXAMT);
	Clamp(&me->dy,6*FIXAMT);

	if(me->mind==0)	// have head
	{
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_ATTACK,256,1,me->dx,me->dy);
			}
		}
	}
	if(me->mind==1)	// lost head already
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A1 && me->frm>1))
		{
			DoMove(me,ANIM_A1,256,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				MakeSound(SND_FRANKARMS,me->x,me->y,SND_CUTOFF,1200);
				DoMove(me,ANIM_A3,160,1,me->dx,me->dy);
				FaceGoodguy(me,goodguy);
			}
			if(me->mind1%15==0)
			{
				MakeSound(SND_GRENADE,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,(byte)Random(256),BLT_ORBGRENADE,me->friendly);
			}
		}
	}
	if(me->mind==2)	// lost body too
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A2 && me->frm>1))
		{
			DoMove(me,ANIM_A2,256,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*2;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_A4,256,1,me->dx,me->dy);
			}
		}
	}
}

void AI_MechaBonkula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;
	int vampire[4] = { MONS_VAMPIRE,MONS_VAMPIRE2,MONS_VAMPIRE3,MONS_VAMPIRE4 };

	

	if(me->reload)
		me->reload--;

	if(me->ouch==3)
	{
		if(me->hp>0)
			MakeSound(SND_BONKOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BONKDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==8 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*60;
			y=me->y+Sine(me->facing*32)*60;

			FindVictim(x>>FIXSHIFT,y>>FIXSHIFT,6*15,Cosine(me->facing*32)*2,
							   Sine(me->facing*32)*2,20,map,world,me->friendly);
			MakeRingParticle(x,y,0,6*15,10*15);
			me->reload=5;
		}

		if(me->seq==ANIM_DIE)
		{
			me->dx=0;
			me->dy=0;
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			//FireBullet(x,y,0,BLT_FLAMEWALL);
			ExplodeParticles(PART_YELLOW,x,y,Random(10*FIXAMT),5);
		}
		if(me->seq==ANIM_A1)
		{
			me->facing++;
			if(me->facing>7)
				me->facing=0;
			if(Random(30)==0)
			{
				x=me->x-300*FIXAMT+Random(600)*FIXAMT;
				y=me->y-300*FIXAMT+Random(600)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*50,vampire[(byte)Random(4)],me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// begin spinning
		DoMove(me,ANIM_A1,256,1,0,0);
		me->mind3=60;
	}
	else
	{
		if(me->mind3==60)
		{
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			me->dx+=-FIXAMT*2+Random(4*FIXAMT);
			me->dy+=-FIXAMT*2+Random(4*FIXAMT);
		}
		me->mind3--;
		if(RangeToTarget(me,goodguy)<150*FIXAMT)
		{
			MakeSound(SND_BONKSWING,me->x,me->y,SND_CUTOFF,2000);
			DoMove(me,ANIM_ATTACK,256,1,0,0);
			me->mind3=0;
		}
	}
}

// summoned monsters
void SummonAI(Guy *me,Guy *target,int spd,int range,Map *map,world_t *world,Guy *goodguy)
{
	int moveFactor;
	moveFactor=128+128*(20-player.hamSpeed)/10;	// a modifier from 50% to 100% (128-256) modifying your movement speed

	if(me->action==ACTION_BUSY)
		return;

	if(me->mind2)	// while mind2 is on, you just head in whichever direction you're facing
		me->mind2--;
	else
	{
		if(me->mind1)	// bumped a wall
		{
			me->mind2=10+(byte)Random(40);
			me->facing=(me->facing+7+(byte)Random(3))&7;
		}
		else
		{
			if(me->mind==0)	// seeking enemies
			{
				if(RangeToTarget(me,goodguy)>range || target==NULL)	// return home to Summony
				{
					me->mind=1;
					return;
				}
				FaceGoodguy2(me,target);
				me->mind2=3+(byte)Random(5);
			}
			else	// coming home
			{
				FaceGoodguy2(me,goodguy);
				me->mind2=3+(byte)Random(5);
				if(RangeToTarget(me,goodguy)<range/2 && target!=NULL)	// go hunting again
				{
					me->mind=0;
					return;
				}
				if(RangeToTarget(me,goodguy)<=range/2 && target==NULL)	// if you are within the okay range, but just have no target, wander
				{
					if(Random(30)==0)
					{
						me->facing=(me->facing+7+(byte)Random(3))&7;
						me->mind2=10+(byte)Random(40);
					}
				}
			}
		}
	}

	me->dx=(Cosine(me->facing*32)*spd*moveFactor/256)/FIXAMT;
	me->dy=(Sine(me->facing*32)*spd*moveFactor/256)/FIXAMT;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=moveFactor;
	}
	if(RangeToTarget(me,goodguy)>600*FIXAMT)
	{
		me->x=goodguy->x;
		me->y=goodguy->y;
		me->bright=32;
		MakeNormalSound(SND_AUTODOOROPEN);
	}
}

void AI_StickWitch(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	bullet_t *b;

	BasicAI(me,SND_STKWITCHOUCH,SND_STKWITCHDIE,map,world,goodguy);

	if(me->mind3<2)
	{
		if(RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch)
		{
			me->mind3=2;
			MakeSound(SND_WITCHTAUNT,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A2,128,1,0,0);
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->reload==0)
		{
			// shoot
			switch(me->mind2)
			{
				case 0:	// blast spell
					MakeSound(SND_CATTAILSMASH,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					b=GetFireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_BIGSHOT,me->friendly);
					if(b)
					{
						b->dx*=2;
						b->dy*=2;
					}
					me->reload=2;
					break;
				case 1:	// gas blast
					MakeSound(SND_SKELGAS,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					b=GetFireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_GASBLAST,me->friendly);
					if(b)
					{
						b->dx*=2;
						b->dy*=2;
					}
					me->reload=1;
					break;
				case 2:	// flames
					MakeSound(SND_STEAM,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					b=GetFireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_FLAMESHOT,me->friendly);
					me->reload=1;
					break;
				case 3:	// wind missiles
					MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					b=GetFireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_WIND,me->friendly);
					me->reload=3;
					break;
				case 4: // earth spikes
					MakeSound(SND_ZOMBIESTOMP,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					b=GetFireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_EARTHSPIKE2,me->friendly);
					me->reload=3;
					break;
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<512*FIXAMT && Random(16)==0)
	{
		x=me->facing;
		FaceGoodguy3(me,goodguy);
		me->mind=me->facing;
		me->facing=x;
		DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
		return;
	}

	if(me->mind3>2)
	{
		me->mind3--;
		if(me->mind3<10)
		{
			MakeRingParticle(me->x,me->y,0,(15-me->mind3)*3,50);
			if(RangeToTarget(me,goodguy)<70*FIXAMT)
				goodguy->GetShot(Cosine(me->facing)*16,Sine(me->facing)*16,8,map,world);
		}
	}
	else
	{
		me->mind2=(byte)Random(5);
		me->mind3=30*2+(byte)Random(30*5);
		MakeSound(SND_STKWITCHATK,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->ouch>0)
	{
		if(me->mind3>10)
			me->mind3-=5;
		DoMove(me,ANIM_A1,128,0,me->dx,me->dy);
		return;
	}

	if(!(me->mind1--))	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind1=(byte)Random(30*3)+1;
	}

	me->dx=Cosine(me->facing*32)*8;
	me->dy=Sine(me->facing*32)*8;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}
