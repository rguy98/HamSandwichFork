#include "profmenu.h"
#include "mgldraw.h"
#include "control.h"
#include "display.h"
#include "player.h"
#include "music.h"
#include "dialogbits.h"
#include "textdialog.h"
#include "playlist.h"
#include "nameentry.h"
#include "yesnodialog.h"
#include "recordbook.h"
#include "shop.h"
#include "appdata.h"
#if __linux__ || __EMSCRIPTEN__
#include <unistd.h>
#endif

#define PBTN_HEIGHT	19

#define PROF_NORMAL	0
#define PROF_DELETE	1
#define PROF_KEYCONFIG 2
#define PROF_MODECONFIG 3 // Toggles modes straight from the profile!
#define PROF_MODECONFIRM 4

#define BTN_PLAYLIST	1
#define BTN_DEL_PROF	2
#define BTN_BRAINRADAR	3
#define BTN_CANDLERADAR	4
#define BTN_MOVENSHOOT	5
#define BTN_EXIT		6
#define BTN_KEYCONFIG	7
#define BTN_DIFFICULTY	8
#define BTN_CHARACTER	9
#define BTN_RECORDS		10
#define BTN_MODECONFIG	11

#define MAX_PROFS	20
#define PRFNAME_LEN	32

// keyconfig buttons
#define BTN_K1		10
#define BTN_K2		20
#define BTN_DEFAULT	30
#define BTN_EXITKEYS 31

// modeconfig buttons (1-8)
#define BTN_M1		32
#define BTN_EXITMODES (BTN_M1 + NUM_MODES)

typedef struct profButton_t
{
	int x,y;
	int wid;
	char txt[32];
	byte id;
} profButton_t;

static profButton_t btn[]={
	{20,60,122,"Difficulty",BTN_DIFFICULTY},
	{20,90,122,"Character",BTN_CHARACTER},
	{20,120,122,"Brain Radar",BTN_BRAINRADAR},
	{20,150,122,"Candle Radar",BTN_CANDLERADAR},
	{20,180,122,"Move N' Shoot",BTN_MOVENSHOOT},
	{20,210,122,"Record Book",BTN_RECORDS},
	{20,352,200,"Configure Modes",BTN_MODECONFIG},
	{20,374,200,"Configure Keys",BTN_KEYCONFIG},
	{20,396,200,"Edit Song Playlists",BTN_PLAYLIST},
	{20,418,200,"Delete This Profile",BTN_DEL_PROF},
	{20,440,200,"Save & Exit",BTN_EXIT},
};
#define NUM_PROF_BTNS	10

static profButton_t mBtn[] = {
	{20,60,122,"",BTN_M1},
	{20,90,122,"",BTN_M1+1},
	{20,120,122,"",BTN_M1+2},
	{20,150,122,"",BTN_M1+3},
	{20,180,122,"",BTN_M1+4},
	{20,210,122,"",BTN_M1+5},
	{20,240,122,"",BTN_M1+6},
	{20,440,200,"Exit",BTN_EXITMODES},
};
#define NUM_M_BTNS	8

static profButton_t kcBtn[]={
	{90,90,102,"",BTN_K1+0},
	{90,112,102,"",BTN_K1+1},
	{90,134,102,"",BTN_K1+2},
	{90,156,102,"",BTN_K1+3},
	{90,178,102,"",BTN_K1+4},
	{90,200,102,"",BTN_K1+5},

	{196,90,102,"",BTN_K2+0},
	{196,112,102,"",BTN_K2+1},
	{196,134,102,"",BTN_K2+2},
	{196,156,102,"",BTN_K2+3},
	{196,178,102,"",BTN_K2+4},
	{196,200,102,"",BTN_K2+5},

	{20,300,200,"Default Controls",BTN_DEFAULT},
	{20,350,200,"Exit",BTN_EXITKEYS},
};
#define NUM_KC_BTNS	14

static byte mode,kcMode,kBtnNum,canHitKeys;
static int msx,msy;

static byte *backgd;
static sprite_set_t *plSpr;
static char msBright,msDBright;

static char fileList[PRFNAME_LEN*MAX_PROFS];
static int numFiles;
static byte profChoice;
static byte hoverId = -1;

static char diffName[][16]={"Normal","Hard","Lunatic"};
static char charName[][16]={"Bouapha","Happy Stick Man","Dr. Lunatic","Shtupid Shroom","LunaChick","MechaBouapha"};
static char modeName[][16] = {
	"Disco",
	"TV",
	"Ludicrous",
	"Subliminal",
	"Splatter",
	"Manic",
	"Handheld"
};
char modeDesc[][64] = {
	"All tiles are given FLASHY disco colors!",
	"Applies Atari-esque lines to the screen.",
	"The player dies in one hit, no matter what.",
	"All sounds are played backwards.",
	"Particle effects are amplified for laughs.",
	"Everything runs at 2x speed - sound included.",
	"Shrinks the screen to handheld size."
};

static char modeAvailable[][10] = { "Active","Inactive","???" };

static byte recordBook,candleRadar,brainRadar,moveNShoot;

static char *ShortName(const char *name)
{
	static char result[128];

	strcpy(result,name);

	if(!strcmp(".prf",&name[strlen(name)-4]))
	{
		result[strlen(name)-4]='\0';
	}
	return result;
}

void SortProfiles(void)
{
	byte flip;
	int i;
	char tmp[PRFNAME_LEN];

	flip=1;

	while(flip)
	{
		flip=0;
		for(i=0;i<numFiles-1;i++)
		{
			if(strcmp(&fileList[i*PRFNAME_LEN],&fileList[(i+1)*PRFNAME_LEN])>0)
			{
				strcpy(tmp,&fileList[i*PRFNAME_LEN]);
				strcpy(&fileList[i*PRFNAME_LEN],&fileList[(i+1)*PRFNAME_LEN]);
				strcpy(&fileList[(i+1)*PRFNAME_LEN],tmp);
				flip=1;
			}
		}
	}
}

void ScanProfiles(void)
{
	int i;

	for(i=0;i<MAX_PROFS;i++)
		fileList[i*PRFNAME_LEN]='\0';
	numFiles=0;

	auto files = ListDirectory("profiles", ".prf", PRFNAME_LEN);
	for (const auto& str : files)
	{
		const char* name = str.c_str();
		strcpy(&fileList[numFiles*PRFNAME_LEN], name);
		if (++numFiles >= MAX_PROFS)
			break;
	}
	SortProfiles();

	profChoice=0;
	for(i=0;i<numFiles;i++)
		if(!strcmp(profile.name,ShortName(&fileList[i*PRFNAME_LEN])))
			profChoice=i;
}

void InitProfMenu(MGLDraw *mgl)
{
	int i;

	mode=PROF_NORMAL;

	msBright=0;
	msDBright=1;

	mgl->LoadBMP("graphics/profmenu.bmp");
	backgd=(byte *)malloc(640*480);
	plSpr=new sprite_set_t("graphics/pause.jsp");

	for(i=0;i<480;i++)
		memcpy(&backgd[i*640],&mgl->GetScreen()[i*mgl->GetWidth()],640);

	ScanProfiles();
	recordBook=ItemPurchased(SHOP_MAJOR,MAJOR_RECORDBOOK);
	candleRadar=ItemPurchased(SHOP_ABILITY,ABIL_CANDLE);
	brainRadar=ItemPurchased(SHOP_ABILITY,ABIL_BRAIN);
	moveNShoot=ItemPurchased(SHOP_ABILITY,ABIL_MOVESHOOT);
}

void ExitProfMenu(void)
{
	free(backgd);
	delete plSpr;
}

void InitKeyConfig(void)
{
	int i,j;

	for(j=0;j<2;j++)
	{
		for(i=0;i<6;i++)
		{
			strcpy(kcBtn[i+j*6].txt,ScanCodeText(profile.control[j][i]));
		}
	}
	ApplyControlSettings();
}

#define NUM_MODES 7

void InitModeConfig(void)
{
	//SIF_BOUGHT
	int i;
	for (i = 0; i < NUM_MODES; i++)
	{
		if(profile.progress.purchase[modeShopNum[i]] & SIF_BOUGHT)
			strcpy(mBtn[i].txt, (modeName[i]));
		else
			strcpy(mBtn[i].txt, ("???"));
	}
}

byte UpdateProfMenu(int *lastTime,MGLDraw *mgl)
{
	int i,j;
	char k;

	int KEY_MAX = 0;
	const byte* key = SDL_GetKeyboardState(&KEY_MAX);

	mgl->GetMouse(&msx,&msy);

	if(*lastTime>TIME_PER_FRAME*5)
		*lastTime=TIME_PER_FRAME*5;

	k=mgl->LastKeyPressed();

	while(*lastTime>=TIME_PER_FRAME)
	{
		msBright+=msDBright;
		if(msBright>10)
			msDBright=-1;
		if(msBright<-2)
			msDBright=1;

		mgl->Process();
		*lastTime-=TIME_PER_FRAME;
	}

	switch(mode)
	{
		case PROF_NORMAL:
			if(mgl->MouseTap())
			{
				for(i=0;i<NUM_PROF_BTNS;i++)
				{
					if(PointInRect(msx,msy,btn[i].x,btn[i].y,btn[i].x+btn[i].wid,btn[i].y+PBTN_HEIGHT))
					{
						MakeNormalSound(SND_MENUSELECT);
						switch(btn[i].id)
						{
							case BTN_BRAINRADAR:
								if(ItemPurchased(SHOP_ABILITY,ABIL_BRAIN))
									profile.brainRadar=1-profile.brainRadar;
								break;
							case BTN_CANDLERADAR:
								if(ItemPurchased(SHOP_ABILITY,ABIL_CANDLE))
									profile.candleRadar=1-profile.candleRadar;
								break;
							case BTN_MOVENSHOOT:
								if(ItemPurchased(SHOP_ABILITY,ABIL_MOVESHOOT))
									profile.moveNShoot=1-profile.moveNShoot;
								break;
							case BTN_RECORDS:
								if(recordBook)
									return 4;
								break;
							case BTN_DIFFICULTY:
								profile.difficulty++;
								if(profile.difficulty>2)
									profile.difficulty=0;
								break;
							case BTN_CHARACTER:
								profile.playAs++;
								if(profile.playAs>5)
									profile.playAs=0;
								while(profile.playAs!=PLAY_BOUAPHA && !ItemPurchased(SHOP_PLAYABLE,profile.playAs))
								{
									profile.playAs++;
									if(profile.playAs>5)
										profile.playAs=0;
								}
								break;
							case BTN_PLAYLIST:
								return 2;
								break;
							case BTN_KEYCONFIG:
								InitKeyConfig();
								mode=PROF_KEYCONFIG;
								kcMode=0;
								break;
							case BTN_MODECONFIG:
								InitModeConfig();
								mode=PROF_MODECONFIG;
								break;
							case BTN_DEL_PROF:
								mode=PROF_DELETE;
								InitYesNoDialog("Really delete this profile?","Yes","No");
								break;
							case BTN_EXIT:
								return 1;
								break;
						}
					}
				}
				if(PointInRect(msx,msy,470,39,470+150,38+361))
				{
					// profile list
					i=(msy-39)/18;
					if(i<numFiles)	// selecting a profile
					{
						MakeNormalSound(SND_MENUSELECT);
						SaveProfile();
						FreeProfile();
						profChoice=i;
						LoadProfile(ShortName(&fileList[i*PRFNAME_LEN]));
						recordBook=ItemPurchased(SHOP_MAJOR,MAJOR_RECORDBOOK);
						candleRadar=ItemPurchased(SHOP_ABILITY,ABIL_CANDLE);
						brainRadar=ItemPurchased(SHOP_ABILITY,ABIL_BRAIN);
						moveNShoot=ItemPurchased(SHOP_ABILITY,ABIL_MOVESHOOT);
					}
					else if(i==numFiles && numFiles<MAX_PROFS)
					{
						SaveProfile();
						MakeNormalSound(SND_MENUSELECT);
						return 3;
					}
				}
			}
			if(k==27)
				return 1;	// exit
			break;
		case PROF_DELETE:
			if(mgl->MouseTap())
			{
				YesNoDialogClick(msx,msy);
			}
			if(k!=0)
				YesNoDialogKey(k);
			switch(YesNoDialogCommand())
			{
				case YNM_YES:
					char s[64];

					mode=PROF_NORMAL;
					sprintf(s,"profiles/%s.prf",profile.name);
					FreeProfile();
					AppdataDelete(s);
					if(numFiles==1)	// this was the only profile
					{
						return 3;	// so force them to create a new profile
					}
					else
					{
						if(profChoice>0)
							LoadProfile(ShortName(&fileList[0]));
						else	// load profile #1 if you just deleted #0
							LoadProfile(ShortName(&fileList[PRFNAME_LEN]));
						ScanProfiles();
						recordBook=ItemPurchased(SHOP_MAJOR,MAJOR_RECORDBOOK);
						candleRadar=ItemPurchased(SHOP_ABILITY,ABIL_CANDLE);
						brainRadar=ItemPurchased(SHOP_ABILITY,ABIL_BRAIN);
						moveNShoot=ItemPurchased(SHOP_ABILITY,ABIL_MOVESHOOT);
					}
					mode=PROF_NORMAL;
					break;
				case YNM_NO:
					mode=PROF_NORMAL;
					break;
			}
			break;
		case PROF_KEYCONFIG:
			if(kcMode==0)
			{
				if(mgl->MouseTap())
				{
					for(i=0;i<NUM_KC_BTNS;i++)
					{
						if(PointInRect(msx,msy,kcBtn[i].x,kcBtn[i].y,kcBtn[i].x+kcBtn[i].wid,kcBtn[i].y+PBTN_HEIGHT))
						{
							kBtnNum=i;
							MakeNormalSound(SND_MENUSELECT);
							if(kcBtn[i].id>=BTN_K1 && kcBtn[i].id<BTN_K1+6)
							{
								kcMode=kcBtn[i].id-BTN_K1+1;
								canHitKeys=0;
							}
							else if(kcBtn[i].id>=BTN_K2 && kcBtn[i].id<BTN_K2+6)
							{
								kcMode=kcBtn[i].id-BTN_K2+7;
								canHitKeys=0;
							}
							else if(kcBtn[i].id==BTN_EXITKEYS)
							{
								mode=PROF_NORMAL;
								SaveProfile();
							}
							else if(kcBtn[i].id==BTN_DEFAULT)
							{
								DefaultControls();
								InitKeyConfig();
							}
							if(kcMode!=0)
								mgl->ClearKeys();
						}
					}
				}

				if(k==27)
				{
					mode=PROF_NORMAL;
					SaveProfile();
				}

			}
			else	// entering a key or button (1-6=kb1, 7-12=kb2)
			{
				if(canHitKeys==0)
				{
					j=0;
					for(i=0;i<KEY_MAX;i++)
						if(key[i])
						{
							j++;
							break;
						}
					if(j==0)
						canHitKeys=1;
				}

				if(Random(3)==0)
					strcpy(kcBtn[kBtnNum].txt,"???");
				else
					strcpy(kcBtn[kBtnNum].txt,"? ? ?");

				if(k==27)
				{
					InitKeyConfig();
					kcMode=0;
				}
				else
				{
					if(canHitKeys)
					{
						for(i=0;i<KEY_MAX;i++)
						{
							if(key[i])
							{
								if(kcMode<7)
									profile.control[0][kcMode-1]=(byte)i;
								else
									profile.control[1][kcMode-7]=(byte)i;
								kcMode=0;
								InitKeyConfig();
							}
						}
					}
					/*
					i=mgl->LastRawCode()>>8;
					if(i!=0)
					{
						if(kcMode<7)
							profile.control[0][kcMode-1]=(byte)i;
						else
							profile.control[1][kcMode-7]=(byte)i;
						kcMode=0;
						InitKeyConfig();
					}
					*/
				}
			}
			break;
		case PROF_MODECONFIG:
			if (mgl->MouseTap())
			{
				for (i=0; i< NUM_M_BTNS; i++)
				{
					if (PointInRect(msx, msy, mBtn[i].x, mBtn[i].y, mBtn[i].x + mBtn[i].wid, mBtn[i].y + PBTN_HEIGHT))
					{
						switch (mBtn[i].id)
						{
							default:
								if (i < NUM_MODES) {
									if(profile.progress.purchase[modeShopNum[i]] & SIF_ACTIVE) {
										profile.progress.purchase[modeShopNum[i]] ^= SIF_ACTIVE;
										MakeNormalSound(SND_MENUSELECT);
									}
									else if (profile.progress.purchase[modeShopNum[i]] & SIF_BOUGHT) {
										switch(i){
											case MODE_DISCO:
												mode = PROF_MODECONFIRM;
												InitYesNoDialog("This mode contains flashing lights and is not reccomended "
																"for suffering from epilepsy. Are you sure?","Sure!","Nope");
												break;
											case MODE_LUDICROUS:
												mode = PROF_MODECONFIRM;
												InitYesNoDialog("Are you sure? This mode isn't remotely fair.","Okay","EEK!");
												break;
											default:
												profile.progress.purchase[modeShopNum[i]] ^= SIF_ACTIVE;
												MakeNormalSound(SND_MENUSELECT);
												break;
										}
									}
									else
										MakeNormalSound(SND_KMNOBUY);
								}
								break;
							case BTN_EXITMODES:
								MakeNormalSound(SND_MENUSELECT);
								mode = PROF_NORMAL;
								hoverId = -1;
								SaveProfile();
								break;
						}
					}
				}
			}
			else if (k == 27)
			{
				mode = PROF_NORMAL;
				hoverId = -1;
				SaveProfile();
			}
			else {
				
				
				for (i = 0; i < NUM_M_BTNS; i++)
				{
					if (i < NUM_MODES && PointInRect(msx, msy, mBtn[i].x, mBtn[i].y, mBtn[i].x + mBtn[i].wid, mBtn[i].y + PBTN_HEIGHT)) {
						hoverId = i;
					}
				}
			}
			break;
		case PROF_MODECONFIRM:
			if(mgl->MouseTap())
			{
				YesNoDialogClick(msx,msy);
			}
			if(k!=0)
				YesNoDialogKey(k);
			if(YesNoDialogCommand()==YNM_YES)
			{
				if(hoverId < 0){hoverId = 0;}
				profile.progress.purchase[modeShopNum[hoverId]] ^= SIF_ACTIVE;
				MakeNormalSound(SND_MENUSELECT);
				mode = PROF_MODECONFIG;
				ExitYesNoDialog();
			}
			else if(YesNoDialogCommand()==YNM_NO)
			{
				MakeNormalSound(SND_MENUSELECT);
				mode = PROF_MODECONFIG;
				ExitYesNoDialog();
			}
			break;
	}

	return 0;
}

void RenderProfButton(int x,int y,int wid,const char *txt,MGLDraw *mgl)
{
	if((mode==PROF_NORMAL || mode==PROF_KEYCONFIG || mode==PROF_MODECONFIG) && PointInRect(msx,msy,x,y,x+wid,y+PBTN_HEIGHT))
	{
		mgl->Box(x,y,x+wid,y+PBTN_HEIGHT,32+31);
		mgl->FillBox(x+1,y+1,x+wid-1,y+PBTN_HEIGHT-1,32+8);
	}
	else
		mgl->Box(x,y,x+wid,y+PBTN_HEIGHT,32+16);

	PrintGlow(x+2,y+3,txt,0,2);
}

void RenderProfList(int x,int y,MGLDraw *mgl)
{
	int i;

	mgl->FillBox(x,y,x+150,y+361,32*1+4);
	mgl->Box(x,y,x+150,y+361,32*1+16);

	for(i=0;i<numFiles;i++)
	{
		if(i==profChoice)
			mgl->FillBox(x+1,y+1+i*18,x+149,y+i*18+18,32*1+8);
		if(PointInRect(msx,msy,x,y+i*18,x+150,y+i*18+18))
			mgl->Box(x+1,y+1+i*18,x+149,y+i*18+18,32*1+12);
		PrintGlowLimited(x+2,y+2+i*18,x+150,ShortName(&fileList[i*PRFNAME_LEN]),0,2);
	}

	if(i<MAX_PROFS)
	{
		if(PointInRect(msx,msy,x,y+i*18,x+150,y+i*18+18))
			mgl->Box(x+1,y+1+i*18,x+149,y+i*18+18,32*1+12);
		PrintGlowLimited(x+2,y+2+i*18,x+150,"<New Profile>",0,2);
	}
}

void RenderProfMenu(MGLDraw *mgl)
{
	int i;
	int msx2,msy2;
	char s[64];
	static char yesno[][5]={"Off","On"};

	for(i=0;i<480;i++)
		memcpy(&mgl->GetScreen()[i*mgl->GetWidth()],&backgd[i*640],640);

	for(i=0;i<NUM_PROF_BTNS;i++)
	{
		if(btn[i].id==BTN_RECORDS && !recordBook)
			RenderProfButton(btn[i].x,btn[i].y,btn[i].wid,"?????",mgl);
		else if(btn[i].id==BTN_CANDLERADAR && !candleRadar)
			RenderProfButton(btn[i].x,btn[i].y,btn[i].wid,"?????",mgl);
		else if(btn[i].id==BTN_BRAINRADAR && !brainRadar)
			RenderProfButton(btn[i].x,btn[i].y,btn[i].wid,"?????",mgl);
		else if(btn[i].id==BTN_MOVENSHOOT && !moveNShoot)
			RenderProfButton(btn[i].x,btn[i].y,btn[i].wid,"?????",mgl);
		else
			RenderProfButton(btn[i].x,btn[i].y,btn[i].wid,btn[i].txt,mgl);
	}

	PrintGlow(472,20,"Available Profiles",0,2);
	RenderProfList(470,38,mgl);

	sprintf(s,"Username: %s",profile.name);
	PrintGlow(20,20,s,0,2);

	PrintGlow(146,62,diffName[profile.difficulty],0,2);
	PrintGlow(146,92,charName[profile.playAs],0,2);
	if(brainRadar)
		PrintGlow(146,122,yesno[profile.brainRadar],0,2);
	if(candleRadar)
		PrintGlow(146,152,yesno[profile.candleRadar],0,2);
	if(moveNShoot)
		PrintGlow(146,182,yesno[profile.moveNShoot],0,2);

	if(mode==PROF_DELETE)
	{
		RenderYesNoDialog2(msx,msy,mgl);
	}
	// mouse cursor
	SetSpriteConstraints(13,13,627,467);
	msx2=msx;
	msy2=msy;
	if(msx2<13)
		msx2=13;
	if(msy2<13)
		msy2=13;
	if(msx2>622)
		msx2=622;
	if(msy2>462)
		msy2=462;
	plSpr->GetSprite(0)->DrawBright(msx2,msy2,mgl,msBright/2);
	SetSpriteConstraints(0,0,639,479);
}

void RenderKeyConfigMenu(MGLDraw *mgl)
{
	int i;
	int msx2,msy2;

	for(i=0;i<480;i++)
		memcpy(&mgl->GetScreen()[i*mgl->GetWidth()],&backgd[i*640],640);

	for(i=0;i<NUM_KC_BTNS;i++)
		RenderProfButton(kcBtn[i].x,kcBtn[i].y,kcBtn[i].wid,kcBtn[i].txt,mgl);

	PrintGlow(20,20,"Configure Controls",0,2);

	PrintGlow(20,90,"Up",0,2);
	PrintGlow(20,112,"Down",0,2);
	PrintGlow(20,134,"Left",0,2);
	PrintGlow(20,156,"Right",0,2);
	PrintGlow(20,178,"Hammer",0,2);
	PrintGlow(20,200,"Weapon",0,2);

	PrintGlow(90,68,"Keyboard1",0,2);
	PrintGlow(196,68,"Keyboard2",0,2);

	if(kcMode>0)
	{
		PrintGlow(20,228,"Press a key to assign, or ESC to cancel",0,2);
	}

	// mouse cursor
	SetSpriteConstraints(13,13,627,467);
	msx2=msx;
	msy2=msy;
	if(msx2<13)
		msx2=13;
	if(msy2<13)
		msy2=13;
	if(msx2>622)
		msx2=622;
	if(msy2>462)
		msy2=462;
	plSpr->GetSprite(0)->DrawBright(msx2,msy2,mgl,msBright/2);
	SetSpriteConstraints(0,0,639,479);
}



void RenderModeConfigMenu(MGLDraw* mgl)
{
	int i;
	int msx2, msy2;

	for (i = 0; i < 480; i++)
		memcpy(&mgl->GetScreen()[i * mgl->GetWidth()], &backgd[i * 640], 640);

	for (i = 0; i < NUM_M_BTNS; i++) {
		RenderProfButton(mBtn[i].x, mBtn[i].y, mBtn[i].wid, mBtn[i].txt, mgl);
		if (i < NUM_MODES) {
			int t;
			if (profile.progress.purchase[modeShopNum[i]] & SIF_ACTIVE)
				t = 0;
			else if (profile.progress.purchase[modeShopNum[i]] & SIF_BOUGHT)
				t = 1;
			else
				t = 2;
			PrintGlow(mBtn[i].x + mBtn[i].wid + 32, mBtn[i].y, modeAvailable[t], 0, 2);
		}
	}
	if (hoverId > -1) {
		if (profile.progress.purchase[modeShopNum[hoverId]] & SIF_BOUGHT) {
			PrintGlow(mBtn[0].x + mBtn[i].wid + 128, mBtn[hoverId].y, modeDesc[hoverId], 0, 2);
		}
	}
	
	if(mode==PROF_MODECONFIRM)
	{
		RenderYesNoDialog3(msx,msy,mgl);
	}

	PrintGlow(20, 20, "Configure Modes", 0, 2);

	//PrintGlow(20, 90, "Up", 0, 2);

	// mouse cursor
	SetSpriteConstraints(13, 13, 627, 467);
	msx2 = msx;
	msy2 = msy;
	if (msx2 < 13)
		msx2 = 13;
	if (msy2 < 13)
		msy2 = 13;
	if (msx2 > 622)
		msx2 = 622;
	if (msy2 > 462)
		msy2 = 462;
	plSpr->GetSprite(0)->DrawBright(msx2, msy2, mgl, msBright / 2);
	SetSpriteConstraints(0, 0, 639, 479);
}

//----------------

TASK(void) ProfMenu(MGLDraw *mgl)
{
	byte done=0;
	int lastTime=1;

	InitProfMenu(mgl);
	PlaySongForce("003worldpicker.ogg");

	while(!done)
	{
		lastTime+=TimeLength();
		StartClock();
		done=UpdateProfMenu(&lastTime,mgl);

		if(mode<PROF_KEYCONFIG)
			RenderProfMenu(mgl);
		else if(mode==PROF_KEYCONFIG)
			RenderKeyConfigMenu(mgl);
		else
			RenderModeConfigMenu(mgl);

		AWAIT mgl->Flip();

		if(!mgl->Process())
			done=1;
		EndClock();

		if(done==2)
		{
			AWAIT PlayListMenu(mgl);
			done=0;
		}
		if(done==3)
		{
			AWAIT NameEntry(mgl,1);
			done=0;
			ExitProfMenu();
			InitProfMenu(mgl);
		}
		if(done==4)
		{
			AWAIT RecordBook(mgl);
			done=0;
		}
	}

	SaveProfile();
	ExitProfMenu();
}
