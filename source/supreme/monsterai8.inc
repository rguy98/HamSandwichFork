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

void AI_EvilClone2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,a;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_CLONEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_CLONEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			// spit at him
			switch(me->aiType){
				case MONS_CLONESHRM:
				for(i=0;i<3;i++) {
					a=(me->facing*32+16-Random(33))&255;
					FireExactBullet(x,y,FIXAMT*16,Cosine(a)*6,Sine(a)*6,0,0,20,a,BLT_SPORE,me->friendly);
				}
				break;
				case MONS_CLONEDRL:
				FireBullet(x,y,me->facing,BLT_BADLIGHTNING,me->friendly);
				break;
				case MONS_CLONELUNA:
				FireBullet(x,y,me->facing,BLT_BADLUNA,me->friendly);
				break;
				case MONS_CLONEKM:
				FireBullet(x,y,me->facing,BLT_BADFBALL,me->friendly);
				break;
			}
			me->reload=5;
			me->mind1=1;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
		{
			// spit at him
			switch(me->aiType){
				case MONS_CLONESHRM:
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
				break;
				case MONS_CLONEDRL:
				MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,1200);
				break;
				case MONS_CLONELUNA:
				MakeSound(SND_LUNASHOOT,me->x,me->y,SND_CUTOFF,1200);
				break;
				case MONS_CLONEKM:
				MakeSound(SND_FIRECAST,me->x,me->y,SND_CUTOFF,1200);
				break;
			}
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
		FaceGoodguy(me,goodguy);

		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
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
	else if(me->mind==1)	// random wandering
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
		{
			// spit at him
			switch(me->aiType){
				case MONS_CLONESHRM:
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
				break;
				case MONS_CLONEDRL:
				MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,1200);
				break;
				case MONS_CLONELUNA:
				MakeSound(SND_LUNASHOOT,me->x,me->y,SND_CUTOFF,1200);
				break;
				case MONS_CLONEKM:
				MakeSound(SND_FIRECAST,me->x,me->y,SND_CUTOFF,1200);
				break;
			}
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
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

void AI_Incadark(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte a;

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
		return;	// can't do nothin' right now
	}

	if(abs(me->mind2-me->facing)==4)	// max distance
	{
		if(Random(2))
			me->facing+=2;
		else
			me->facing-=2;
		me->facing&=7;
	}
	else if(me->mind2==me->facing)
	{
	}
	else	// rotate towards true facing
	{
		int dir,diff;

		if(me->mind2>me->facing)
		{
			diff=me->mind2-me->facing;
			if(diff>4)
			{
				dir=-2;
				diff=8-diff;
			}
			else
				dir=2;
		}
		else
		{
			diff=me->facing-me->mind2;
			if(diff>4)
			{
				dir=2;
				diff=8-diff;
			}
			else
				dir=-2;
		}
		me->facing=(me->facing+dir)&7;
	}
	if(me->mind==0)	// newborn
	{
		me->facing=Random(8);
		if(me->aiType==MONS_INCADARK)
			me->mind2=0;	// desired facing/also direction of movement
		else
			me->mind2=2;	// desired facing/also direction of movement

		me->mind=1;
	}
	else if(me->mind==1)	// pacing
	{
		if(me->mind1)	// bonked a wall
		{
			me->mind1=0;
			if(me->mind2<4)
				me->mind2+=4;
			else
				me->mind2-=4;
		}
		if(me->mind2!=me->facing)
		{
			me->dx=0;
			me->dy=0;
		}
		else
		{
			me->dx=Cosine(me->mind2*32)*3/2;
			me->dy=Sine(me->mind2*32)*3/2;
		}

		if(me->mind2==me->facing && TargetInSight(me,map,goodguy))
		{
			me->frmAdvance=256;
			me->reload=0;
			me->mind=2;	// start clapping!
		}

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
	}
	else	// staring at goodguy
	{
		me->dx=Cosine(me->mind2*32)*12;
		me->dy=Sine(me->mind2*32)*12;
		x=me->x+Cosine(me->facing*32)*16;
		y=me->y+Sine(me->facing*32)*16;
		if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy)){
			goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,128,map,world);
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
		}
		if(!TargetInSight(me,map,goodguy)){
			me->mind=0;
		}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=180;
		}
	}
}
