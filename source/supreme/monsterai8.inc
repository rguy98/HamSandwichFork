void AI_PunkBunny2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		MakeSound(SND_BUNNYDIE,me->x,me->y,SND_CUTOFF,800);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm==3 || me->frm==6 || me->frm==9) && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;

			FireBullet(x,y,(me->facing*32-8+Random(17))&255,BLT_ENERGY,me->friendly);
			MakeSound(SND_BULLETFIRE,x,y,SND_CUTOFF,100);
			me->reload=3;
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*10;
			y=me->y+Sine(me->facing*32)*10;

			FireBullet(x,y,me->facing*32,BLT_BOMB,me->friendly);
			me->reload=3;
		}
		if(me->seq==ANIM_DIE)
		{
			for(x=0;x<8;x++)
				SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT),
							me->y+((-16+Random(33))<<FIXSHIFT));
		}
		return;
	}

	if(me->mind==0)	// not activated yet
	{
		if(RangeToTarget(me,goodguy)<72*FIXAMT)
		{
			me->mind=1;
			MakeNormalSound(SND_FOLLOWHI);
		}
	}
	else if(me->mind==1)	// following Bouapha
	{
		Dampen(&me->dx,FIXAMT/8);
		Dampen(&me->dy,FIXAMT/8);

		if(me->x<goodguy->x)
			me->dx+=FIXAMT;
		else
			me->dx-=FIXAMT;
		if(me->y<goodguy->y)
			me->dy+=FIXAMT;
		else
			me->dy-=FIXAMT;

		me->dx+=-FIXAMT/2+Random(FIXAMT);
		me->dy+=-FIXAMT/2+Random(FIXAMT);

		if(RangeToTarget(me,goodguy)<48*FIXAMT)	// got too close
		{
			me->mind=2;
			me->facing=Random(8);
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->mind1=Random(20)+10;
		}
	}
	else if(me->mind==2) // not following, got too close
	{
		me->mind1--;
		if(!me->mind1)
			me->mind=1;
	}
	else	// hunting for fresh meat
	{
		if(goodguy==NULL || goodguy->type==MONS_NOBODY || RangeToTarget(me,goodguy)>320*FIXAMT)	// got too far from papa!!
		{
			me->mind=1;
			return;
		}
		else
		{
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dy,FIXAMT/8);

			if(me->x<goodguy->x)
				me->dx+=FIXAMT;
			else
				me->dx-=FIXAMT;
			if(me->y<goodguy->y)
				me->dy+=FIXAMT;
			else
				me->dy-=FIXAMT;

			me->dx+=-FIXAMT/2+Random(FIXAMT);
			me->dy+=-FIXAMT/2+Random(FIXAMT);

			if(RangeToTarget(me,goodguy)<280*FIXAMT && Random(10)==0)
			{
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->dx=0;
				me->dy=0;
				me->action=ACTION_BUSY;
				FaceGoodguy(me,goodguy);
				return;
			}
			if(Random(50)==0)
			{
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->dx=0;
				me->dy=0;
				me->action=ACTION_BUSY;
				FaceGoodguy(me,goodguy);
				return;
			}
		}
	}

	// if following Bouapha, and an enemy comes within sight, and you aren't already far from Bouapha
	if((me->mind==1 || me->mind==2) && RangeToTarget(me,goodguy)<100*FIXAMT)
	{
		me->mind=3;	// enter assault mode
	}

	Clamp(&me->dx,FIXAMT*4);
	Clamp(&me->dy,FIXAMT*4);

	if(me->dx>FIXAMT)
	{
		if(me->dy>FIXAMT)
			me->facing=1;
		else if(me->dy<-FIXAMT)
			me->facing=7;
		else
			me->facing=0;
	}
	else if(me->dx<-FIXAMT)
	{
		if(me->dy>FIXAMT)
			me->facing=3;
		else if(me->dy<-FIXAMT)
			me->facing=5;
		else
			me->facing=4;
	}
	else
	{
		if(me->dy>FIXAMT)
			me->facing=2;
		else if(me->dy<-FIXAMT)
			me->facing=6;
	}

	if(me->mind!=0)
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_LiveBomb(Guy *me,Map *map,world_t *world,Guy *goodguy)
{;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,16);
			FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
		}
		return;
	}

	if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,0,0,0,map,world,me->friendly))
	{
		me->hp=1;
		me->GetShot(0,0,1,map,world);
		return;
	}
	FaceGoodguy(me,goodguy);
	me->dx+=Cosine(me->facing*32)/2;
	me->dy+=Sine(me->facing*32)/2;

	// circling the player
	if(me->mind==0)
	{
		if(me->x<goodguy->x)
		{
			// on left side
			if(me->y<goodguy->y)
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
			if(me->y<goodguy->y)
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
		Clamp(&me->dx,FIXAMT*8);
		Clamp(&me->dy,FIXAMT*8);
	}

	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;
	me->mind1=0;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=180;
	}
}

void AI_NightTerror(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	byte f;

	if(!goodguy)
		return;
	
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_VAMPOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_VAMPDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{

		if(me->seq==ANIM_DIE)
		{
			if(Random(2))
			{
				x = me->x >> FIXSHIFT;
				y = me->y >> FIXSHIFT;
				BlowSmoke((x + me->rectx + Random(me->rectx2 - me->rectx)) << FIXSHIFT,
					(y + me->recty + Random(me->recty2 - me->recty)) << FIXSHIFT,
					me->z, FIXAMT);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && goodguy)
		{
			FaceGoodguy(me,goodguy);
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(20,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,24,map,world);
			}
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1)
		me->mind1--;
	
	if (me->mind1 % 2 == 1 && me->mind !=2)
	{
		if(me->hp<me->maxHP && me->seq!=ANIM_DIE)
		{
			i=me->hp;
			me->hp+=1;
			if(me->hp>me->maxHP)
			me->hp=me->maxHP;
		}
	}

	if(map->map[me->mapx+me->mapy*map->width].templight>-8)	// if you can see him, RUN!
	{
		me->mind=2;	// dash at you
		me->mind1=3;
	}

	switch(me->mind)
	{
		case 0:	// normal heading towards you
			FaceGoodguy(me,goodguy);
			if(me->mind1==0)
			{
				me->mind=1;
				me->facing=Random(8);
				me->mind1=10+Random(40);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			if(goodguy->x>me->x-FIXAMT*64 && goodguy->x<me->x+FIXAMT*48 && goodguy->y>me->y-FIXAMT*64 && goodguy->y<me->y+FIXAMT*48 && goodguy->ouch==0)
			goodguy->GetShot(0,0,1,map,world);
			break;
		case 1:	// normal wandering
			if(me->mind1==0)
			{
				me->mind=0;
				me->mind1=10+Random(90);
			}
			me->dx=Cosine(me->facing*32)*1;
			me->dy=Sine(me->facing*32)*1;
			me->frmAdvance=128;
			if(goodguy->x>me->x-FIXAMT*64 && goodguy->x<me->x+FIXAMT*48 && goodguy->y>me->y-FIXAMT*64 && goodguy->y<me->y+FIXAMT*48 && goodguy->ouch==0)
			goodguy->GetShot(0,0,2,map,world);
			break;
		case 2:	// dashing at you
			if(me->mind1==0)
			{
				me->mind=1;
				me->mind1=10+Random(40);
			}
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
			me->frmAdvance=200;
			if(goodguy->x>me->x-FIXAMT*64 && goodguy->x<me->x+FIXAMT*48 && goodguy->y>me->y-FIXAMT*64 && goodguy->y<me->y+FIXAMT*48 && goodguy->ouch==0)
			goodguy->GetShot(0,0,1,map,world);
			break;
	}

	if(goodguy)
	{

		x=RangeToTarget(me,goodguy);
		if(x<(128*FIXAMT) && Random(16)==0 && me->reload==0 && me->mind == 2)
		{
			MakeSound(SND_VAMPATTK,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// bite him
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=100;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}
	}

	if(me->mind!=2)	// when dashing, he ignores light, but otherwise wants to avoid it
	{
		x=(me->x+me->dx)/(TILE_WIDTH*FIXAMT);
		y=(me->y+me->dy)/(TILE_HEIGHT*FIXAMT);
		if(map->map[me->mapx+me->mapy*map->width].templight<-5 &&
			map->map[x+y*map->width].templight>=-5)	// you are in darkness now, but wouldn't be on moving
		{
			me->dx=0;
			me->dy=0;
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			return;
		}
		if(map->map[me->mapx+me->mapy*map->width].templight<=-5)	// if you are in light at any time, run
		{
			me->dx=(me->dx*3)/4;
			me->dy=(me->dy*3)/4;
			me->frmAdvance=160;
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

void AI_RevengeClone(Guy *me, Map *map, world_t *world, Guy *goodguy)
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
		if (me->seq == ANIM_ATTACK && me->frm > 3 && me->frm < 12 && me->frm % 2 == 1 && me->reload == 0 && goodguy)
		{
			MakeSound(SND_ARMORSHOOT, me->x, me->y, SND_CUTOFF, 1200);
			f = me->facing * 32 - 64;
			x = me->x + Cosine(me->facing * 32)*20;
			y = me->y + Sine(me->facing * 32)*20;
			x2 = x + Cosine(f)*32;
			y2 = y + Sine(f)*32;
			FireBullet(x2, y2, me->facing * 32, BLT_BIGSHELL, me->friendly);
			x2 = x - Cosine(f)*32;
			y2 = y - Sine(f)*32;
			FireBullet(x2, y2, me->facing * 32, BLT_BIGSHELL, me->friendly);
			me->mind1 = 1;
		}
		if (me->seq == ANIM_A1 && me->frm == 2 && me->reload == 0 && goodguy)
		{
			QuadMissile(me->x, me->y, me->facing*32, me->friendly);
			me->reload = 5;
			me->mind1 = 1;
		}
		if (me->frm >= 3 && me->seq == ANIM_DIE)
			{
				x = me->x >> FIXSHIFT;
				y = me->y >> FIXSHIFT;
				BlowUpGuy(x + me->rectx, y + me->recty, x + me->rectx2, y + me->recty2, me->z, 2);
				BlowSmoke((x + me->rectx + Random(me->rectx2 - me->rectx)) << FIXSHIFT,
						(y + me->recty + Random(me->recty2 - me->recty)) << FIXSHIFT,
						me->z, FIXAMT);
			}
		if (me->frm == 4 && me->seq == ANIM_DIE)
			{
				// become an evil clone...
				FireBullet(me->x, me->y, 0, BLT_BOOM, me->friendly);
				me->type=MONS_EVILCLONE;	// become a lesser clone
				me->aiType=MONS_EVILCLONE;
				strcpy(me->name,monsType[me->aiType].name);
				me->hp=32;
				me->maxHP=32;
				me->mind=0;
				me->mind1=0;
				me->facing=2;
			}
		return; // can't do nothin' right now
	}

	if (me->mind == 0) // when mind=0, singlemindedly lumber towards Bouapha
	{
		if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(32) == 0)
		{
			// spit at him
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

void AI_Missozoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	bullet_t *b;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			// spit stuff
			x=me->x+Cosine(me->facing*32)*50;
			y=me->y+Sine(me->facing*32)*50;
			b=GetFireBullet(x,y,me->facing,BLT_TORPEDO,me->friendly);
			me->reload=15;
			me->mind=0;
		}
		if(me->seq==ANIM_DIE && me->frm>3 && Random(4)==0)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			RenderSmoke(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z);
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_SERPENTSPIT,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// spit on him!
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

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind=Random(30)+1;
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

void AI_Kelvozoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm>=5 && me->frm<=8) && goodguy)
		{
			// spit stuff
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBulletZ(x,y,FIXAMT*12,(me->facing*32-16+(byte)Random(33))&255,BLT_FREEZE2,me->friendly);
			me->reload=15;
			me->mind=0;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_SERPENTSPIT,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// spit on him!
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

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind=Random(60)+1;
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

void AI_PygmyBoomer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PYGMYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PYGMYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			// throw spear
			FireBullet(x,y,me->facing*32,BLT_BOOMERANG,me->friendly);
			me->reload=10;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		me->frmAdvance=32;	// overcome the auto-128 frmAdvance of guy.cpp

		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
		if(me->mind1<101)
			me->mind1++;

		if(me->mind1>100 && Random(200)==0)
		{
			switch(Random(2))
			{
				case 0:
					MakeSound(SND_UNGADANCE,me->x,me->y,SND_CUTOFF,1200);
					me->seq=ANIM_A1;	// unga dance
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 1:
					me->facing=(me->facing+(-1+Random(3)))&7;	//change facing
					me->mind1=0;
					break;
			}
		}
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<320*FIXAMT || me->ouch>0)
			{
				// if the goodguy is near, or he shot me
				me->mind=1;	// start hunting
				if(me->ouch==0)
					me->mind1=90;	// for 2 seconds minimum
				else
					me->mind1=180;	// 4 seconds, because they HATE getting shot
				FaceGoodguy(me,goodguy);

				// do angry animation
				me->action=ACTION_BUSY;
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=255;	// very fast
				MakeSound(SND_PYGMYANGRY,me->x,me->y,SND_CUTOFF,1200);
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
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0 && me->reload==0)
		{
			// spear him!
			MakeSound(SND_PYGMYSTAB,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
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
		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=196;
		}
	}
}

void AI_LunaMecha(Guy* me, Map* map, world_t* world, Guy* goodguy)
{
	int x,y,j;
	byte f;
	bullet_t *b;

	if(me->reload)
		me->reload--;

	BasicAI(me,SND_METALSMACK,SND_ROBOBOUAPHAOUCH,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>4)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);

			}
			if(me->frm==8)
			{
				// become a zombie
				player.enemiesSlain--;
				me->type=MONS_LUNABOSS;
				me->aiType=MONS_LUNABOSS;
				strcpy(me->name,MonsterName(me->type));
				me->mind=0;
				me->mind1=0;
				me->hp=monsType[me->type].hp;
				me->maxHP=monsType[me->type].hp;
				me->seq=ANIM_IDLE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
			}
		}
		else if (me->seq==ANIM_A1)
		{
			FaceGoodguy2(me,goodguy);
			if((me->frm==3 || me->frm==4) && me->reload==0 && goodguy)
			{
				MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1200);
				x=me->x+Cosine(((me->facing+2)&7)*32)*48;
				y=me->y+Sine(((me->facing+2)&7)*32)*32;
				FireBulletZ(x,y,FIXAMT*100,me->facing*32-16+Random(33),BLT_GRENADE,me->friendly);
				if(me->frm==4)
					me->reload=30;
			}
		}
		return;	// can't do nothin' right now
	}
	else {
		if(me->mind==0) // start of cycle
		{
			if (!me->reload)
			{
				me->mind=1; // start bouncing
				me->mind3=TurnToward(me->mind3,AngleFrom(me->x,me->y,goodguy->x,goodguy->y),4)*32;
				me->facing=me->mind3/32;
				me->dx=Cosine(me->mind3)*6;
				me->dy=Sine(me->mind3)*6;
				me->reload=5+Random(5)*Random(5);
			}
		}
		else if(me->mind==1) // bounce
		{
			if(me->mind1>0) {
				FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,48,me->dx,me->dy,20,map,world,me->friendly);
				if(me->mind1&1)
				{
					if(me->dx>0)
						me->mind3=64+Random(16)*8;
					else
						me->mind3=192+Random(16)*8;
				}
				if(me->mind1&2)
				{
					if(me->dy>0)
						me->mind3=128+Random(16)*8;
					else
						me->mind3=Random(16)*8;
				}
				me->mind3=TurnToward(me->mind3,AngleFrom(me->x,me->y,goodguy->x,goodguy->y),4);
				if(Random(4)==0)
				{
					// shoot at him
					me->seq=ANIM_A1;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->action=ACTION_BUSY;
					me->dx=0;
					me->dy=0;
					return;
				}
				me->facing=me->mind3/32;
				me->dx=Cosine(me->mind3)*6;
				me->dy=Sine(me->mind3)*6;
				me->mind1=0;
				me->mind2++; // counter
				MakeSound(SND_PKSTEINBUMP,me->x,me->y,SND_CUTOFF,1200);
			}
			if(!me->reload)
			{
				if(me->mind2>Random(4)+2)
				{
					me->dx=0;
					me->dy=0;
					me->mind3=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
					me->mind2=Random(2);
					me->mind++;
					me->reload=2;
				}
				else
				{
					b=GetFireBullet(me->x,me->y,(me->mind3+128)&255,BLT_ENERGY2,me->friendly);
					if(b)
					{
						RecolorBullet(b,4,6);
						b->dx/=2;
						b->dy/=2;
						MakeSound(SND_BULLETFIRE,me->x,me->y,SND_CUTOFF,1050);
					}
					me->reload=5+Random(5)*Random(5);
				}
			}
		}
		else if(me->mind==2) // charge!!
		{
			if(!me->reload)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
				if(me->facing==me->mind3/32){
					me->mind++;
					me->frmAdvance=128;
					MakeSound(SND_CARSTART,me->x,me->y,SND_CUTOFF,1200);
				}
				else{
					me->reload=2;
					me->facing = (me->facing+1)&7;
					MakeSound(SND_CARGO,me->x,me->y,SND_CUTOFF,1200);
				}
			}
		}
		else if(me->mind==3) // charging
		{
			if(me->mind1)
			{
				if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,48,me->dx,me->dy,40,map,world,me->friendly))
				{
					ShakeScreen(15,7);
					for(int i=0;i<4;i++)
					{
						f=i*2+1;
						x=me->x+Cosine(f*32)*32;
						y=me->y+Sine(f*32)*32;
						b=GetFireBullet(x,y,f*32,BLT_BIGYELLOW,me->friendly);
						if(b)
						{
							b->dx*=2;
							b->dy*=2;
							b->timer/=2;
						}
					}
				}
				else
				{
					ShakeScreen(15,8);
					for(int i=0;i<5;i++)
					{
						f=(me->facing+4)&7;
						x=me->x+Cosine(f*32+(i-2)*16)*32;
						y=me->y+Sine(f*32+(i-2)*16)*32;
						b=GetFireBullet(x,y,f*32+(i-2)*16,BLT_BIGYELLOW,me->friendly);
						if(b)
						{
							b->dx*=2;
							b->dy*=2;
							b->timer/=2;
						}
					}
				}
				MakeSound(SND_ROLYPOLYWALL, me->x, me->y, SND_CUTOFF, 1000);
				me->mind=0;
				me->frmAdvance=128;
				me->mind2=0;
			}
			else
			{
				if(!me->reload)
				{
					FireBullet(me->x,me->y,0,BLT_BADSITFLAME,me->friendly);
					me->reload=8+Random(2)*4;
				}
				me->dx=Cosine(me->mind3)*6;
				me->dy=Sine(me->mind3)*6;
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

void AI_Centi2Head(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==0 && me->frmTimer<32)
				FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
			ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 3);
		}

		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me, goodguy) < 44 * FIXAMT && me->reload == 0)
		{
			goodguy->GetShot(me->dx, me->dy, 6, map, world);
			me->reload = 5;
		}
		if(!me->reload)
		{
			FaceGoodguy2(me,goodguy);
			me->reload=1;
		}
	}

	if(me->mind1)	// bonked a wall or guy
	{
		me->facing=(byte)Random(8);
		if(me->mind1&1)
			me->dx=-me->dx;
		if(me->mind1&2)
			me->dy=-me->dy;
		me->mind1=0;
	}

	if(me->facing==0 || me->facing==1 || me->facing==7)
		me->dx+=FIXAMT;
	else if(me->facing==3 || me->facing==4 || me->facing==5)
		me->dx-=FIXAMT;
	if(me->facing==1 || me->facing==2 || me->facing==3)
		me->dy+=FIXAMT;
	else if(me->facing==5 || me->facing==6 || me->facing==7)
		me->dy-=FIXAMT;

	Dampen(&me->dx,FIXAMT/2);
	Dampen(&me->dy,FIXAMT/2);
	Clamp(&me->dx,FIXAMT*7);
	Clamp(&me->dy,FIXAMT*7);

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}
