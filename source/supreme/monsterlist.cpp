#include "monster.h"

#define MT_WEIRD MT_BITS
monsterType_t monsType[NUM_MONSTERS]=
	{
		{"Null",255,255,0,NULL,
		 0,0,0,0,"",0,0,0},
		{"Bouapha",255,255,0,NULL,
		 11,29,128,0,"graphics/bouapha.jsp",0,MF_WATERWALK,
		 MT_GOOD|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{0,0,0,0,0,0,0,26,27,28,255},		// die
				{23,24,25,24,23,255},	// use item
				{17,18,19,20,21,22,21,20,21,22,21,20,19,18,17,255},	// bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,26,27,28,255},	// watery death
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{0,255},
			}},
		{"Bonehead",255,255,0,AI_Bonehead,
		 8,38,10,50,"graphics/skeleton.jsp",0,0,
		 MT_SPOOKY|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{20,21,22,23,24,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},		// die
				{25,26,27,28,29,30,255},	// chomp/spit projectile
				{31,32,32,32,31,255},	// point at bouapha
				{16,15,14,13,12,11,10,9,8,7,255},	// resurrect=A3
				{0,255},
				{0,255},
			}},
		{"Scary Bat",255,255,0,AI_Bat,
		 8,27,5,25,"graphics/bat.jsp",0,MF_FLYING,
		 MT_SPOOKY|MT_FLYING|MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,8,7,255},	// attack
				{17,18,19,20,21,22,23,24,25,26,255},		// die
				{10,11,12,12,12,12,12,11,10,255},	// diving attack
				{13,14,15,15,16,255},	// bounce off during dive
				{7,8,9,8,7,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Eensy Weensy",255,255,0,AI_Spider,
		 4,22,1,10,"graphics/spider.jsp",0,0,
		 MT_EVIL|MT_SPIDER|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,0,255},	// move
				{12,13,14,13,12,255},	// attack
				{15,16,17,18,19,18,19,18,19,20,20,21,21,255},	// die
				{12,13,14,13,12,255},	// attack
				{12,13,14,13,12,255},	// attack
				{12,13,14,13,12,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Spitter",255,255,0,AI_BigSpider,
		 8,21,10,50,"graphics/bigspdr.jsp",0,0,
		 MT_EVIL|MT_SPIDER|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{14,15,16,17,18,17,18,17,18,19,19,20,20,255},	// die
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Zombie",255,255,0,AI_Zombie,
		 11,32,20,150,"graphics/zombie.jsp",0,0,
		 MT_SPOOKY|MT_ZOMBIE|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{20,21,22,23,24,25,26,27,28,29,30,31,255},	// die
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Egg Sac",255,255,0,AI_EggSac,
		 20,19,150,1000,"graphics/eggsac.jsp",0,MF_ONEFACE|MF_ENEMYWALK|MF_NOMOVE,
		 MT_EVIL|MT_GENERATE|MT_SPIDER,
			{
				{0,0,0,0,0,0,0,0,1,2,1,0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0,255},	// attack
				{19,20,21,22,23,24,25,255},	// die
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Mama Spider",255,255,0,AI_MamaSpider,
		 30,30,200,1500,"graphics/mamaspdr.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_MINIBOSS|MT_SPIDER|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,13,13,255},	// spit (attack)
				{19,20,21,22,23,24,25,25,25,26,27,28,29,255},	// die
				{14,15,16,17,18,255},	// bite (A1)
				{7,8,9,10,11,12,12,13,13,255},	// spit (attack)
				{7,8,9,10,11,12,12,13,13,255},	// spit (attack)
				{0,255},
				{0,255},
			}},
		{"Pygmy",255,255,0,AI_Pygmy,
		 10,50,15,100,"graphics/pygmy.jsp",0,0,
		 MT_EVIL|MT_PYGMY|MT_HUMAN,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{8,9,10,9,8,7,11,12,13,12,11,7,255},	// move
				{29,30,31,31,32,33,34,35,36,255},	// attack
				{37,38,39,40,41,42,43,44,45,46,255},	// die
				{14,15,16,17,18,19,20,16,17,18,19,15,14,255},	// (A1) bored (spin spear)
				{21,22,23,24,25,26,26,26,27,28,27,26,27,28,27,26,255}, // (A2) bored (yawn)
				{47,48,49,48,47,0,47,48,49,48,47,255},	// (A3) angry, spotted Bouapha
				{0,255},
				{0,255},
			}},
		{"Aquazoid",255,255,0,AI_Serpent,
		 15,20,15,75,"graphics/serpent.jsp",0,MF_AQUATIC|MF_WATERWALK,
		 MT_AQUATIC|MT_EVIL|MT_ZOID,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{13,14,15,16,17,18,19,255},	// die
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{0,255},
				{0,255},
			}},
		{"Matilda",255,255,0,AI_MattieSkullOrHead,	// the head of matilda
		 40,11,300,1000,"graphics/mathead.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_SPIDER|MT_BOSS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{0,255},
				{0,255},
			}},
		{"Matilda's Skull",255,255,0,AI_MattieSkullOrHead,	// the skull of matilda (head becomes this when killed)
		 40,11,200,1000,"graphics/matskull.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{0,255},
				{0,255},
			}},
		{"Matilda's Brain",255,255,0,AI_MattieBrain,	// the brain of matilda (skull becomes this when killed)
		 40,7,100,5000,"graphics/matbrain.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// no form of attack
				{1,2,3,2,1,0,4,5,6,5,4,0,1,2,3,2,1,0,4,5,6,5,255},	// die
				{0,255},
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Matilda's Body",255,255,0,AI_MattieBody,	// the body of matilda
		 50,7,1,1,"graphics/matbody.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_INVINCIBLE|MF_NOGRAV|MF_ENEMYWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{0,255},	// fire
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{0,255},
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Matilda's Claw",255,255,0,AI_MattieClaw,	// the left claw (her right)
		 20,16,1000,1000,"graphics/matclaw1.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_SPRITEBOX|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,1,2,3,2,1,255},	// idle
				{0,255},	// move
				{7,8,9,10,11,12,13,14,15,255},	// claw slash
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{4,5,6,6,6,6,6,6,6,5,4,255},	// block (A1)
				{7,8,9,10,11,12,13,14,15,255},	// claw slash
				{7,8,9,10,11,12,13,14,15,255},	// claw slash
				{0,255},
				{0,255},
			}},
		{"Matilda's Claw",255,255,0,AI_MattieClaw,	// the right claw (her left)
		 20,16,1000,1000,"graphics/matclaw2.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_SPRITEBOX|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,1,2,3,2,1,255},	// idle
				{0,255},	// move
				{7,8,9,10,11,12,13,14,15,255},	// claw slash
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{4,5,6,6,6,6,6,6,6,5,4,255},	// block (A1)
				{7,8,9,10,11,12,13,14,15,255},	// claw slash
				{7,8,9,10,11,12,13,14,15,255},	// claw slash
				{0,255},
				{0,255},
			}},
		{"Matilda's Tail",255,255,0,AI_MattieTail,	// her tail
		 80,16,200,1000,"graphics/mattail.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_INVINCIBLE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,1,2,3,2,1,255},	// fire
				{1,2,0,3,2,0,3,3,0,1,2,3,1,2,255},	// die
				{0,1,2,3,2,1,255},	// fire
				{0,1,2,3,2,1,255},	// fire
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Ninjabread Man",255,255,0,AI_Ginger,
		 12,33,50,300,"graphics/ginger.jsp",0,0,
		 MT_EVIL|MT_WACKY,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,255},	// move
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,19,20,19,20,21,22,23,255},	// triple punch
				{27,28,29,30,31,32,32,32,32,255},	// die
				{24,25,26,26,26,26,26,255},	// jumpkick (A1)
				{25,24,255},	// unjumpkick (A2)
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,19,20,19,20,21,22,23,255},	// triple punch
				{0,255},
				{0,255},
			}},
		{"PUMPKIN!",255,255,0,AI_Pumpkin,
		 11,6,5,500,"graphics/pumpkin.jsp",0,0,
		 MT_EVIL|MT_PUMPKIN|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{0,0,255},	// die (just long enough to spew shrapnel)
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
			}},
		{"Thingie",255,255,0,AI_BabyThing,
		 12,44,10,50,"graphics/babything.jsp",0,0,
		 MT_EVIL|MT_THINGY,
			{
				{12,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,255},	// move
				{20,21,22,23,24,25,26,27,255},	// attack
				{34,35,36,37,38,39,40,41,42,43,255},	// die
				{13,14,15,16,17,16,15,16,17,16,15,16,17,16,15,255},	// fall asleep (A1)
				{18,19,18,255},	// blink (A2)
				{28,29,30,29,28,255}, // look right (A3)
				{31,32,33,32,31,255}, // look left (A4)
				{0,255},
			}},
		{"Mucho Moss",255,255,0,AI_Moss,
		 15,17,10,25,"graphics/moss.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FREEWALK|MF_ENEMYWALK|MF_WALLWALK|MF_NOSHADOW,
		 MT_EVIL|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,255},	// move=grow vertically
				{4,5,6,5,4,255},	// attack=grow horizontally
				{7,8,9,10,11,12,13,14,15,16,255},	// die
				{4,5,6,5,4,255},	// attack=grow horizontally
				{4,5,6,5,4,255},	// attack=grow horizontally
				{4,5,6,5,4,255},	// attack=grow horizontally
				{0,255},
				{0,255},
			}},
		{"Moss Grande",255,255,0,AI_MossGrande,
		 18,17,100,250,"graphics/mossgrnd.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_ENEMYWALK,
		 MT_EVIL|MT_VEGGIE,
			{
				{0,1,2,3,4,5,6,255},	// idle
				{0,255},	// move=unused
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{22,23,24,25,26,27,28,255},	// die
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Magmazoid",255,255,0,AI_Magmazoid,
		 15,20,20,80,"graphics/magmazoid.jsp",0,MF_AQUATIC|MF_WATERWALK,
		 MT_EVIL|MT_ZOID|MT_AQUATIC,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move
				{4,5,6,7,8,9,10,10,10,10,10,10,10,11,12,255},	// attack
				{13,14,15,16,17,18,19,255},	// die
				{4,5,6,7,8,9,10,10,10,10,10,10,10,11,12,255},	// attack
				{4,5,6,7,8,9,10,10,10,10,10,10,10,11,12,255},	// attack
				{4,5,6,7,8,9,10,10,10,10,10,10,10,11,12,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Shroom",255,255,0,AI_Shroom,
		 15,23,10,50,"graphics/shroom.jsp",0,0,
		 MT_EVIL|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// die
				{7,8,9,10,11,12,13,14,255},	// a1
				{7,8,9,10,11,12,13,14,255},	// attack
				{7,8,9,10,11,12,13,14,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Mush the Shroom",255,255,0,AI_Mush,
		 32,27,200,800,"graphics/bgshroom.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_MINIBOSS|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,2,1,0,3,4,4,3,0,255},	// move
				{5,6,6,7,8,9,10,255},	// attack=cannon fire
				{21,22,23,24,25,26,26,26,26,255},	// die
				{11,12,13,12,13,12,13,12,13,12,13,12,13,12,13,12,13,14,15,16,255},	// A1=sporegun fire
				{17,18,19,20,19,18,19,20,19,18,17,255},	// A2=angry
				{5,6,6,7,8,9,10,255},	// attack=cannon fire
				{0,255},
				{0,255},
			}},
		{"The Thing",255,255,0,AI_TheThing,
		 64,11,800,5000,"graphics/thething.jsp",0,MF_NOMOVE|MF_AQUATIC|MF_WATERWALK|MF_ENEMYWALK|MF_ONEFACE,
		 MT_EVIL|MT_THINGY|MT_BOSS,
			{
				{0,255},	// idle
				{0,255},	// move
				{3,4,5,6,7,8,9,10,10,10,9,8,7,6,5,4,3,255},	// attack=tongue
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255},	// die
				{1,2,1,255},	// A1=blink
				{3,4,5,6,7,8,9,10,10,10,9,8,7,6,5,4,3,255},	// attack=tongue
				{3,4,5,6,7,8,9,10,10,10,9,8,7,6,5,4,3,255},	// attack=tongue
				{0,255},
				{0,255},
			}},
		{"Tentacle",255,255,0,AI_ThingTentacle,
		 32,1,500,100,"graphics/thingtent.jsp",0,MF_NOMOVE|MF_FREEWALK|MF_NOGRAV|MF_ENEMYWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,0,0,255},	// die
				{0,255},
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Tentacle",255,255,0,AI_ThingTentacle,	// tentacle tip
		 32,1,500,100,"graphics/thingtent2.jsp",0,MF_NOMOVE|MF_FREEWALK|MF_NOGRAV|MF_ENEMYWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,0,0,255},	// die
				{0,255},
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Super Zombie",255,255,0,AI_SuperZombie,
		 36,38,300,2500,"graphics/suprzmbe.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_MINIBOSS|MT_SPOOKY|MT_ZOMBIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{17,18,19,20,21,22,23,24,25,255},	// attack=punch
				{9,10,11,12,13,14,15,15,15,15,15,15,255},	// die
				{7,8,9,10,11,11,11,12,12,13,13,13,14,14,14,15,16,16,255},	// A1=leap
				{26,27,28,29,30,255},	// A2=attempted grab
				{27,31,32,33,34,35,34,36,37,36,34,35,34,36,37,36,34,35,34,33,31,27,255},	//A3= grab n' pound
				{0,255},
				{0,255},
			}},
		{"Happy Stick Man",255,255,0,AI_StickMan,
		 16,12,100,1000,"graphics/stickman.jsp",0,MF_ONEFACE,
		 MT_EVIL|MT_WACKY|MT_MINIBOSS|MT_HUMAN,
			{
				{0,255},	// idle
				{1,0,2,0,1,0,2,0,1,0,2,0,255},	// move
				{10,11,11,10,10,10,10,10,10,10,255},	// attack=chomp
				{3,3,3,3,3,3,4,4,5,5,255},	// die
				{6,7,7,7,6,0,8,9,9,9,8,255},	// A1=show off muscles
				{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,255},	// A2=sadness
				{1,0,2,0,1,0,2,0,1,0,2,0,0,0,3,4,5,255},	// watery death
				{0,255},
				{0,255},
			}},
		{"Baby SEAL",255,255,0,AI_BabySeal,
		 22,20,20,125,"graphics/babyseal.jsp",0,0,
		 MT_EVIL|MT_ANIMAL|MT_ARCTIC,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move
				{6,6,7,8,7,8,9,10,9,10,11,12,11,12,13,13,13,14,14,14,255},	// attack=full auto
				{15,16,17,18,19,19,19,255},	// die
				{6,6,7,8,7,8,9,10,9,10,11,12,11,12,13,13,13,14,14,14,255},	// attack=full auto
				{6,6,7,8,7,8,9,10,9,10,11,12,11,12,13,13,13,14,14,14,255},	// attack=full auto
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Cryozoid",255,255,0,AI_Isozoid,
		 15,23,20,75,"graphics/isozoid.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_ZOID|MT_ARCTIC|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move
				{10,11,12,13,14,15,16,255},	// attack=spit snow
				{17,18,19,20,21,22,255},	// die
				{4,5,6,7,8,9,255},	//A1=rise out of the snow
				{254,255},			//A2=idle under the ground
				{10,11,12,13,14,15,16,255},	// attack=spit snow
				{0,255},
				{0,255},
			}},
		{"Snowguy",255,255,0,AI_Snowguy,
		 26,36,40,200,"graphics/snowguy.jsp",0,0,
		 MT_EVIL|MT_ARCTIC,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,0,255},	// move
				{24,25,26,27,28,29,255},	// attack=slash
				{30,31,32,33,34,35,254,35,254,35,254,35,254,254,35,255},	// die
				{8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,255},	//A1=throw snowball
				{24,25,26,27,28,29,255},	// attack=slash
				{24,25,26,27,28,29,255},	// attack=slash
				{0,255},
				{0,255},
			}},
		{"Pengulon",255,255,0,AI_Penguin,
		 10,8,5,50,"graphics/penguin.jsp",0,MF_FLYING,
		 MT_EVIL|MT_FLYING|MT_ARCTIC|MT_ANIMAL,
			{
				{0,1,2,3,4,5,4,3,2,1,255},	// idle
				{0,1,2,3,4,5,4,3,2,1,255},	// move
				{6,7,7,7,6,6,3,3,3,3,3,255},	// attack=dive
				{0,254,3,254,5,254,1,254,4,254,2,254,3,255},	// die
				{6,7,7,7,6,6,3,3,3,3,3,255},	// attack=dive
				{6,7,7,7,6,6,3,3,3,3,3,255},	// attack=dive
				{6,7,7,7,6,6,3,3,3,3,3,255},	// attack=dive
				{0,255},
				{0,255},
			}},
		{"Zomboni",255,255,0,AI_Zomboni,
		 48,9,100,500,"graphics/zomboni.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_VEHICLE|MT_ARCTIC|MT_ZOMBIE|MT_SPOOKY,
			{
				{0,255},	// idle
				{0,1,2,0,1,2,255},	// move
				{0,255},	// attack=unused
				{3,4,5,5,5,6,7,8,8,255},	// die
				{0,255},
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Sven",255,255,0,AI_Yeti,
		 48,40,500,5000,"graphics/yeti.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_ARCTIC|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,1,0,3,4,3,255},	// move
				{14,15,16,17,18,19,20,21,22,23,255},	// attack=snowball
				{35,36,37,38,39,39,39,39,39,39,39,39,39,39,39,39,39,39,255},	// die
				{28,29,30,31,32,33,33,33,34,34,255},	// A1=ground pound
				{5,6,7,8,9,10,11,12,13,255},	//A2=death yodel
				{24,25,26,27,26,25,26,27,26,25,26,27,26,25,24,255},	// A3=wave hello
				{0,255},
				{0,255},
			}},
		{"Bjorn",1,4,2,AI_Yeti,
		 48,40,500,5000,"!36",0,MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_ARCTIC|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,1,0,3,4,3,255},	// move
				{14,15,16,17,18,19,20,21,22,23,255},	// attack=snowball
				{35,36,37,38,39,39,39,39,39,39,39,39,39,39,39,39,39,39,255},	// die
				{28,29,30,31,32,33,33,33,34,34,255},	// A1=ground pound
				{5,6,7,8,9,10,11,12,13,255},	//A2=death yodel
				{24,25,26,27,26,25,26,27,26,25,26,27,26,25,24,255},	// A3=wave hello
				{0,255},
				{0,255},
			}},
		{"Geozoid",255,255,0,AI_Geozoid,
		 15,20,20,75,"graphics/geozoid.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_ZOID|MT_DESERT,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit rock
				{13,14,15,16,17,18,19,255},	// die
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit rock
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit rock
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit rock
				{0,255},
				{0,255},
			}},
		{"Mumble",255,255,0,AI_Mumble,
		 15,28,30,100,"graphics/mumble.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_DESERT|MT_SPOOKY,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,6,7,8,9,10,0,255},	// move
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{19,20,21,22,23,24,25,26,27,255},	// die
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{0,255},
				{0,255},
			}},
		{"Djinni",255,255,0,AI_Djinni,
		 12,24,10,50,"graphics/djinni.jsp",0,MF_WALLWALK|MF_NOSHADOW|MF_FLYING|MF_GHOST|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_SPOOKY|MT_DESERT|MT_FLYING,
			{
				{0,1,2,3,2,1,255},	// idle
				{0,1,2,3,2,1,255},	// move
				{12,13,14,15,16,17,18,19,255},	// attack=punch
				{20,21,22,23,255},	// die
				{4,5,6,7,8,9,8,7,6,7,8,9,8,7,6,7,8,9,10,11,255},	// A1=scare
				{23,22,21,20,255},	// A2=materialize
				{12,13,14,15,16,17,18,19,255},	// attack=punch
				{0,255},
				{0,255},
			}},
		{"Magic Lamp",255,255,0,AI_MagicLamp,
		 15,10,150,500,"graphics/lamp.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_ENEMYWALK,
		 MT_EVIL|MT_GENERATE|MT_DESERT|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,255},	// attack=summon Djinni
				{1,5,8,3,7,2,0,8,254,1,254,7,254,5,254,8,254,2,255},	// die
				{1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,255},	// attack=summon Djinni
				{1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,255},	// attack=summon Djinni
				{1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,255},	// attack=summon Djinni
				{0,255},
				{0,255},
			}},
		{"Cacton",255,255,0,AI_Cactus,
		 20,25,25,100,"graphics/cactus.jsp",0,0,
		 MT_EVIL|MT_VEGGIE|MT_DESERT,
			{
				{0,0,0,13,14,15,15,14,13,0,0,0,16,17,18,18,17,16,0,255},	// idle
				{3,4,5,4,3,255},	// move
				{6,7,8,8,9,10,11,12,255},	// attack=shoot spines
				{19,20,21,21,22,23,24,24,254,24,254,24,254,24,254,24,255},	// die
				{1,2,3,255},	// A1=begin moving
				{3,2,1,255},	// A2=stop moving
				{6,7,8,8,9,10,11,12,255},	// attack=shoot spines
				{0,255},
				{0,255},
			}},
		{"Roly Poly",255,255,0,AI_Roller,
		 35,11,2000,1000,"graphics/roller.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_DESERT|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,255},
				{0,255},
			}},
		{"Richie Lich",255,255,0,AI_Lich,
		 50,8,500,1500,"graphics/lich.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_GHOST|MF_NOSHADOW|MF_FLYING,
		 MT_EVIL|MT_SPOOKY|MT_MINIBOSS|MT_FLYING,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,2,1,255},	// attack=open mouth
				{0,5,6,7,8,255},	// die
				{1,2,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,2,1,255},	// attack=open mouth
				{1,2,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,2,1,255},	// attack=open mouth
				{1,2,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,2,1,255},	// attack=open mouth
				{0,255},
				{0,255},
			}},
		{"Dust Devil",255,255,0,AI_DustDevil,
		 50,23,600,2000,"graphics/dustdevil.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_GHOST|MF_NOSHADOW,
		 MT_EVIL|MT_WACKY|MT_DESERT|MT_MINIBOSS,
			{
				{0,1,2,3,4,5,255},	// idle
				{0,1,2,3,4,5,255},	// move
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{17,18,19,20,21,22,255},	// die
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{0,255},
				{0,255},
			}},
		{"MechaBouapha",255,255,0,AI_MechaBouapha,
		 20,24,250,1000,"graphics/mechabouapha.jsp",0,0,
		 MT_EVIL|MT_HIGHTECH|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,255},	// attack=hammer toss
				{15,16,17,18,19,20,21,22,23,255},	// die
				{15,16,17,18,19,20,21,22,255},	// A1=melt
				{23,255},	// A2=ball form
				{22,21,20,19,18,17,16,15,255},	// A3=unmelt
				{0,255},
				{0,255},
			}},
		{"Sphinx Arm",255,255,0,AI_SphinxArm,	// arm1
		 30,10,600,2000,"graphics/sphinxarm1.jsp",0,MF_ONEFACE|MF_NOMOVE|MF_SPRITEBOX,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{0,1,2,3,2,3,4,3,2,3,4,3,254,4,254,3,254,4,255},	// die
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
			}},
		{"Sphinx Arm",255,255,0,AI_SphinxArm,	// arm2
		 30,10,600,2000,"graphics/sphinxarm2.jsp",0,MF_ONEFACE|MF_NOMOVE|MF_SPRITEBOX,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{0,1,2,3,2,3,4,3,2,3,4,3,254,4,254,3,254,4,255},	// die
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
				{1,2,3,4,5,6,7,7,8,9,255},	// attack=swipe
			}},
		{"Sphinxter",255,255,0,AI_Sphinx,
		 80,11,1000,5000,"graphics/sphinx.jsp",0,MF_ONEFACE|MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_DESERT,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,255},	// attack=summon mummy
				{10,10,10,10,10,10,10,10,10,255},	// die
				{4,5,6,7,7,6,7,7,7,6,7,8,9,10,255}, // A1=sneeze
				{1,2,3,255},	// attack=summon mummy
				{1,2,3,255},	// attack=summon mummy
				{1,2,3,255},	// attack=summon mummy
			}},
		{"Freakazoid",255,255,0,AI_Freakazoid,
		 15,18,15,75,"graphics/freakazoid.jsp",0,MF_AQUATIC|MF_WATERWALK,
		 MT_EVIL|MT_ZOID|MT_WACKY|MT_AQUATIC,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move
				{4,5,6,7,8,9,9,9,9,10,255},	// attack=spit spines
				{11,12,13,14,15,16,17,255},	// die
				{4,5,6,7,8,9,9,9,9,10,255},	// attack=spit spines
				{4,5,6,7,8,9,9,9,9,10,255},	// attack=spit spines
				{4,5,6,7,8,9,9,9,9,10,255},	// attack=spit spines
				{4,5,6,7,8,9,9,9,9,10,255},	// attack=spit spines
			}},
		{"Centipumpkin",255,255,0,AI_CentiBody,	// body
		 14,5,20,200,"graphics/cpbody.jsp",0,MF_ENEMYWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Centipumpkin",255,255,0,AI_CentiHead,	// head
		 14,1,40,500,"graphics/cphead.jsp",0,MF_ENEMYWALK,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN|MT_MINIBOSS,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Wacko",255,255,0,AI_Wacko,
		 12,21,20,100,"graphics/wacko.jsp",0,0,
		 MT_EVIL|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{15,16,17,18,19,20,254,20,254,20,254,20,254,20,255},	// die
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
			}},
		{"The Boiler",255,255,0,AI_Boiler,
		 100,22,1400,2000,"graphics/boiler.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_TRAP|MT_MINIBOSS|MT_URBAN,
			{
				{0,1,2,3,4,5,6,7,255},	// idle
				{0,255},	// move
				{8,9,10,11,12,13,14,15,14,13,12,11,10,9,8,255},	// attack=flames
				{16,17,18,19,20,21,255},	// die
				{8,9,10,11,12,13,14,15,14,13,12,11,10,9,8,255},	// attack=flames
				{8,9,10,11,12,13,14,15,14,13,12,11,10,9,8,255},	// attack=flames
				{8,9,10,11,12,13,14,15,14,13,12,11,10,9,8,255},	// attack=flames
				{8,9,10,11,12,13,14,15,14,13,12,11,10,9,8,255},	// attack=flames
			}},
		{"Great Pumpkin",255,255,0,AI_GreatPumpkin,
		 120,13,1000,4000,"graphics/greatpk.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_ENEMYWALK,
		 MT_EVIL|MT_PUMPKIN|MT_VEGGIE|MT_MINIBOSS,
			{
				{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,8,255},	// idle
				{0,255},	// move
				{1,2,3,3,4,5,6,6,7,8,255},	// attack=vomit babies
				{9,9,10,10,11,11,12,12,255},	// die
				{1,2,3,3,4,5,6,6,7,8,255},	// attack=vomit babies
				{1,2,3,3,4,5,6,6,7,8,255},	// attack=vomit babies
				{1,2,3,3,4,5,6,6,7,8,255},	// attack=vomit babies
				{1,2,3,3,4,5,6,6,7,8,255},	// attack=vomit babies
			}},
		{"The Ultrazoid",255,255,0,AI_Ultrazoid,
		 40,17,750,2500,"graphics/ultrazoid.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_ZOID|MT_MINIBOSS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,255},	// attack=breathe
				{10,11,12,13,14,15,16,255},	// die
				{1,2,3,4,5,6,7,8,9,255},	// attack=breathe
				{1,2,3,4,5,6,7,8,9,255},	// attack=breathe
				{1,2,3,4,5,6,7,8,9,255},	// attack=breathe
				{1,2,3,4,5,6,7,8,9,255},	// attack=breathe
			}},
		{"Dr. Lunatic",255,255,0,AI_DrLunatic,
		 40,21,50,10000,"graphics/dr-l.jsp",0,0,
		 MT_EVIL|MT_HUMAN|MT_BOSS,
			{
				{0,255},	// idle
				{7,8,9,8,7,0,10,11,12,11,10,0,255},	// move
				{1,2,3,4,5,6,5,4,5,6,5,6,5,4,3,4,3,4,5,4,3,2,1,255},	// attack=laugh
				{13,14,15,16,17,17,17,17,17,17,17,18,19,20,20,20,20,255},	// die
				// these following are only used when you play as Dr. L
				{1,2,3,2,1,255},	// use item
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255},	// bored
				{7,9,7,0,10,12,10,0,7,9,7,0,0,0,0,0,0,255},	// watery death
				{1,2,3,4,5,6,5,4,5,6,5,6,5,4,3,4,3,4,5,4,3,2,1,255},	// attack=laugh
			}},
		{"Super Duper Zombie",255,255,0,AI_SDZL,
		 40,48,1000,25000,"graphics/sdzl.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_BOSS|MT_ZOMBIE|MT_SPOOKY,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move (to the right)
				{28,29,30,31,31,31,31,31,32,33,34,35,36,37,38,39,255},	// attack=slash
				{40,41,42,43,43,43,43,43,44,45,46,47,47,47,47,47,47,255},	// die
				{5,6,5,0,7,8,7,0,255},	// A1=move left
				{9,10,11,12,12,12,12,12,12,13,14,15,16,16,17,18,255},	// A2=breathe
				{19,20,21,21,22,23,24,25,25,25,25,25,25,26,27,255},	// A3=ground pound
				{28,29,30,31,31,31,31,31,32,33,34,35,36,37,38,39,255},	// attack=slash
				{28,29,30,31,31,31,31,31,32,33,34,35,36,37,38,39,255},	// attack=slash
			}},
		{"Santa Claus",255,255,0,AI_Santa,
		 18,25,100,500,"graphics/santa.jsp",0,0,
		 MT_EVIL|MT_HUMAN|MT_WACKY|MT_ARCTIC,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{20,21,22,23,24,24,24,254,24,254,24,254,24,254,24,255},	// die
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
			}},
		// EXPANSION MONSTERS
		{"Mine Cart",255,255,0,AI_MineCart,
		 11,8,100,500,"graphics/minecar.jsp",0,MF_NOMOVE|MF_INVINCIBLE|MF_FREEWALK|MF_NOHIT,
		 MT_VEHICLE|MT_GOOD,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
			}},
		{"Raft",255,255,0,AI_Raft,
		 8,8,100,500,"graphics/raft.jsp",0,MF_NOMOVE|MF_INVINCIBLE|MF_FREEWALK|MF_NOHIT|MF_ONEFACE|MF_AQUATIC|MF_WATERWALK,
		 MT_VEHICLE|MT_GOOD|MT_AQUATIC,
			{
				{1,2,3,4,5,6,0,255},	// idle
				{1,2,3,4,5,6,0,255},	// move
				{1,2,3,4,5,6,0,255},	// move
				{1,2,3,4,5,6,0,255},	// move
				{1,2,3,4,5,6,0,255},	// move
				{1,2,3,4,5,6,0,255},	// move
				{1,2,3,4,5,6,0,255},	// move
				{1,2,3,4,5,6,0,255},	// move
				{1,2,3,4,5,6,0,255},	// move
			}},
		{"Power Armor",255,255,0,NULL,	// in power armor
		 20,14,128,0,"graphics/pwrarmor.jsp",0,0,
		 MT_WEIRD,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,9,9,9,8,7,255},	// attack
				{10,11,12,13,255},		// die
				{7,7,7,255},		// fire missiles
				{13,12,11,10,255},	// activate
				{7,8,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,8,7,255},	// attack
			}},
		{"Vampire",255,255,0,AI_Vampire,
		 12,13,60,500,"graphics/vampire.jsp",0,0,
		 MT_EVIL|MT_VAMPIRE|MT_SPOOKY,
			{
				{1,2,1,0,255},	// idle
				{1,2,1,0,255},	// move
				{3,4,5,6,7,8,9,255},	// attack
				{12,11,10,255},		// die
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
			}},
		{"Coffin",255,255,0,AI_Coffin,
		 25,20,8000,500,"graphics/coffin.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_TRAP|MT_VAMPIRE|MT_SPOOKY,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{17,18,19,255},		// die
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
			}},
		{"Spooky Ghost",255,255,0,AI_Ghost,
		 18,7,10,300,"graphics/ghost.jsp",0,MF_WALLWALK|MF_NOSHADOW|MF_FLYING|MF_GHOST|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_SPOOKY|MT_FLYING,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,3,3,2,1,255},	// attack (scream)
				{4,4,5,5,6,6,255},		// die
				{4,5,6,6,5,4,255},		// A1=teleport
				{254,255},				// A2=idle invisible
				{6,5,4,255},			// A3=materialize
				{1,2,3,3,3,2,1,255},	// attack (scream)
				{1,2,3,3,3,2,1,255},	// attack (scream)
			}},
		{"Burner",255,255,0,AI_Burner,
		 18,2,30,300,"graphics/burner.jsp",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_INVINCIBLE,
		 MT_EVIL|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,255},		// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Lefty",255,255,0,AI_Lefty,
		 9,28,10,100,"graphics/lefty.jsp",0,0,
		 MT_EVIL|MT_SPOOKY|MT_WACKY|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{14,15,16,17,18,19,20,21,255},	// attack (punch)
				{22,23,24,25,26,27,26,25,24,26,254,25,254,27,254,26,255},		// die
				{7,8,9,10,11,12,13,12,11,10,11,12,13,12,11,10,9,8,7,255}, // A1=wave
				{14,15,16,17,18,19,20,21,255},	// attack (punch)
				{14,15,16,17,18,19,20,21,255},	// attack (punch)
				{14,15,16,17,18,19,20,21,255},	// attack (punch)
				{14,15,16,17,18,19,20,21,255},	// attack (punch)
			}},
		{"Pygmy Hunter",255,255,0,AI_Pygmy2,
		 10,41,15,100,"graphics/pygmy2.jsp",0,0,
		 MT_EVIL|MT_PYGMY|MT_HUMAN,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{8,9,10,9,8,7,11,12,13,12,11,7,255},	// move
				{20,21,22,23,24,25,26,27,28,255},	// attack
				{32,33,34,35,36,37,38,39,40,255},	// die
				{14,15,16,15,14,0,17,18,19,18,17,0,255},	// (A1) bored (unga dance)
				{29,30,31,30,29,0,29,30,31,30,29,255},	// (A2) angry, spotted Bouapha
				{20,21,22,23,24,25,26,27,28,255},	// attack
				{20,21,22,23,24,25,26,27,28,255},	// attack
				{20,21,22,23,24,25,26,27,28,255},	// attack
			}},
		{"Pygmy Shaman",255,255,0,AI_Pygmy3,
		 10,35,15,100,"graphics/pygmy3.jsp",0,0,
		 MT_EVIL|MT_PYGMY|MT_HUMAN,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{8,9,10,9,8,7,11,12,13,12,11,7,255},	// move
				{20,21,22,22,22,21,20,255},	// attack
				{26,27,28,29,30,31,32,33,34,255},	// die
				{14,15,16,15,14,0,17,18,19,18,17,0,255},	// (A1) bored (unga dance)
				{23,24,25,24,23,0,23,24,25,24,23,255},	// (A2) angry, spotted Bouapha
				{20,21,22,22,22,21,20,255},	// attack
				{20,21,22,22,22,21,20,255},	// attack
				{20,21,22,22,22,21,20,255},	// attack
			}},
		{"Pumpkinstein",255,255,0,AI_Pumpkinstein,
		 18,13,80,1000,"graphics/pkstein.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_SPOOKY|MT_MINIBOSS|MT_PUMPKIN|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{0,255},	// attack
				{7,8,9,10,11,12,12,12,12,12,12,12,255},	// die
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
			}},
		{"Stan Glass",255,255,0,AI_Knight,
		 25,47,150,1000,"graphics/knight.jsp",0,MF_GLOW|MF_ONEFACE,
		 MT_EVIL|MT_MINIBOSS,
			{
				{14,255},	// idle
				{15,16,17,18,19,20,21,22,14,255},	// move
				{26,27,28,29,30,31,32,33,255},	// attack
				{34,35,36,37,38,39,40,41,42,43,44,45,46,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,255}, // A1 = arrive
				{23,24,25,25,25,25,25,25,25,24,23,255}, // A2 = block
				{26,27,28,29,30,31,32,33,255},	// attack
				{26,27,28,29,30,31,32,33,255},	// attack
				{26,27,28,29,30,31,32,33,255},	// attack
			}},
		{"Triceroid",255,255,0,AI_Triceroid,
		 30,18,60,250,"graphics/triceroid.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_ANIMAL,
			{
				{6,255},	// idle
				{5,4,3,2,1,0,1,2,3,4,5,6,255},	// move
				{7,8,9,9,9,8,7,255},	// attack
				{10,11,12,13,14,15,16,17,255},	// die
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
			}},
		{"Countess",255,255,0,AI_Countess,
		 25,28,220,1000,"graphics/countess.jsp",0,MF_ONEFACE,
		 MT_EVIL|MT_BOSS|MT_SPOOKY|MT_VAMPIRE,
			{
				{0,1,2,3,4,5,6,7,8,9,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
				{20,21,22,22,22,22,23,24,25,26,27,255},	// die
				{10,11,12,12,12,12,11,10,255},	// A1=summon boneheads
				{3,3,3,255},	// A2=charge up to rush
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
			}},
		{"Egg",255,255,0,AI_AlienEgg,
		 12,14,100,200,"graphics/alienegg.jsp",0,MF_ONEFACE|MF_ENEMYWALK|MF_NOMOVE,
		 MT_EVIL|MT_GENERATE|MT_ALIEN,
			{
				{0,1,2,3,2,1,255},	// idle
				{0,255},	// move
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,255},	// die
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
			}},
		{"Xenoid",255,255,0,AI_BabyAlien,
		 8,10,1,20,"graphics/babyalien.jsp",0,0,
		 MT_EVIL|MT_SPIDER|MT_ALIEN,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,255},	// move
				{7,8,9,9,9,8,7,255},	// attack
				{0,255},	// die
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,8,7,255},	// attack
			}},
		{"Xeno Hunter",255,255,0,AI_Alien,
		 13,29,30,350,"graphics/alien.jsp",0,0,
		 MT_EVIL|MT_ALIEN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{15,16,17,18,19,20,21,22,255},	// attack
				{23,24,25,26,27,28,255},	// die
				{7,8,9,10,11,12,13,14,255},	// A1=spit
				{15,16,17,18,19,20,21,22,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// attack
			}},
		{"Robopumpkin",255,255,0,AI_Robopumpkin,
		 26,7,200,1500,"graphics/robopk.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_HIGHTECH|MT_PUMPKIN|MT_VEGGIE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,2,1,255},	// attack
				{0,0,0,0,255},	// die
				{4,5,6,0,4,5,6,255},	// A1=chaingun
				{1,2,3,2,1,255},	// attack
				{1,2,3,2,1,255},	// attack
				{1,2,3,2,1,255},	// attack
				{1,2,3,2,1,255},	// attack
			}},
		{"Shock Trooper",255,255,0,AI_ShockTrooper,
		 40,23,200,1500,"graphics/shocktr.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_HIGHTECH,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{16,17,18,19,20,21,22,22,22,255},	// die
				{5,6,7,7,8,9,255},	// A1=lob grenade
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{10,11,11,11,12,13,14,14,15,255},	// attack
			}},
		{"MiniBot",255,255,0,AI_Minibot,
		 10,6,5,50,"graphics/robot1.jsp",0,0,
		 MT_EVIL|MT_HIGHTECH,
			{
				{0,255},	// idle
				{1,2,1,0,255},	// move
				{0,0,255},	// attack
				{3,4,5,5,5,255},	// die
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
			}},
		{"MeanieBot",255,255,0,AI_Meaniebot,
		 13,10,30,100,"graphics/robot2.jsp",0,0,
		 MT_EVIL|MT_HIGHTECH,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{5,6,6,6,6,6,5,255},	// attack
				{7,8,9,9,9,9,255},	// die
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
			}},
		{"RoboFactory",255,255,0,AI_Robofactory,
		 20,28,150,500,"graphics/robofcty.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_ENEMYWALK,
		 MT_EVIL|MT_HIGHTECH|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack (summon)
				{0,0,0,255},	// die
				{17,18,19,20,21,22,23,24,25,26,27,0,255},	// A1=finish the summon
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack (summon)
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack (summon)
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack (summon)
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack (summon)
			}},
		{"Turret",255,255,0,AI_Turret,
		 10,1,80,200,"graphics/turret.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Doom Bunny",255,255,0,AI_Bunny,
		 10,8,30,1000,"graphics/doombnny.jsp",0,0,
		 MT_EVIL|MT_WACKY|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,0,255},	// move
				{0,255},	// attack
				{0,0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"KONGOR!!",255,255,0,AI_Kongor,
		 80,32,400,2000,"graphics/kongor.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_ANIMAL|MT_BOSS,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move (left)
				{7,8,9,10,10,9,8,7,255},	// attack (left pound)
				{15,16,17,24,25,26,27,28,29,30,31,255},	// die
				{4,5,6,5,4,0,255}, // A1=move (right)
				{11,12,13,14,14,13,12,11,255}, // A2=right pound
				{15,16,17,18,19,20,19,18,21,22,23,22,21,18,19,20,19,18,255}, // A3=chest pound
				{21,22,23,22,21,17,16,15,255}, // A4=finish chest pound
				{7,8,9,10,10,9,8,7,255},	// attack (left pound)
			}},
		{"Squash",255,255,0,AI_Squash,
		 8,14,10,100,"graphics/squash.jsp",0,0,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,9,8,7,255},	// attack
				{11,12,13,255},	// die
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
			}},
		{"UltraPygmy",255,255,0,AI_UltraPygmy,
		 5,50,5,100,"graphics/pygmy4.jsp",0,0,
		 MT_EVIL|MT_PYGMY|MT_HUMAN|MT_WACKY,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{8,9,10,9,8,7,11,12,13,12,11,7,255},	// move
				{29,30,31,31,32,33,34,35,36,255},	// attack
				{37,38,39,40,41,42,43,44,45,46,255},	// die
				{14,15,16,17,18,19,20,16,17,18,19,15,14,255},	// (A1) bored (spin spear)
				{21,22,23,24,25,26,26,26,27,28,27,26,27,28,27,26,255}, // (A2) bored (yawn)
				{47,48,49,48,47,0,47,48,49,48,47,255},	// (A3) angry, spotted Bouapha
				{29,30,31,31,32,33,34,35,36,255},	// attack
				{29,30,31,31,32,33,34,35,36,255},	// attack
			}},
		{"LoonyBot 5000",255,255,0,AI_LoonyBot,
		 120,15,500,3000,"graphics/loonybot.jsp",0,MF_ONEFACE|MF_NOMOVE|MF_INVINCIBLE,
		 MT_EVIL|MT_BOSS|MT_HIGHTECH|MT_VEHICLE,
			{
				{0,0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,7,255},	// attack
				{10,11,12,13,14,14,14,14,14,255},	// die
				{7,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,7,255},	// attack
			}},
		{"LoonyBot 5000",255,255,0,AI_LoonyCore,	// the core object, used to allow it to get hit when it's open
		 40,16,500,3000,"graphics/loonygun.jsp",0,MF_ONEFACE|MF_NOMOVE|MF_NOSHADOW|MF_FREEWALK|MF_ENEMYWALK,
		 MT_BITS,
			{
				{254,254,255},	// idle
				{254,254,255},	// move
				{254,254,255},	// attack
				{254,254,255},	// die
				{254,254,255},	// attack
				{254,254,255},	// attack
				{254,254,255},	// attack
				{254,254,255},	// attack
				{254,254,255},	// attack
			}},
		{"LoonyGun",255,255,0,AI_LoonyGun,
		 40,16,500,3000,"graphics/loonygun.jsp",0,MF_ONEFACE|MF_NOMOVE|MF_FACECMD|MF_NOSHADOW|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,0,255},	// idle
				{0,0,255},	// move
				{0,0,255},	// attack
				{0,0,255},	// die
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
			}},
		{"Loony Zoomer",255,255,0,AI_LoonyShip,
		 30,1,100,3000,"graphics/loonyshp.jsp",0,MF_FLYING,
		 MT_EVIL|MT_MINIBOSS|MT_HIGHTECH|MT_FLYING|MT_VEHICLE,
			{
				{0,0,255},	// idle
				{0,0,255},	// move
				{0,0,255},	// attack
				{0,0,0,0,255},	// die
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
			}},

		// ------------------------------------------
		// FUN PACK MONSTERS!

		{"Buddy Bunny",2,0,3,AI_BuddyBunny,
		 10,8,30,1000,"!83",0,0,
		 MT_GOOD|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,0,255},	// move
				{0,255},	// attack
				{0,0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Good Turret",4,1,5,AI_GoodTurret,
		 10,1,30,200,"!82",0,MF_NOMOVE,
		 MT_GOOD|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Roly Poly",255,255,0,AI_Roller,
		 35,11,2000,1000,"!43",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_TRAP|MT_DESERT,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
			}},
		{"Crazy Egg",0,1,3,AI_AlienEgg,
		 12,14,100,200,"!74",0,MF_ONEFACE|MF_ENEMYWALK|MF_NOMOVE,
		 MT_EVIL|MT_ALIEN|MT_GENERATE|MT_WACKY,
			{
				{0,1,2,3,2,1,255},	// idle
				{0,255},	// move
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,255},	// die
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
			}},
		{"Matilda-X",4,1,0,AI_Mattie2SkullOrHead, // the head of matilda
		 40,11,500,1000,"!11",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_BOSS|MT_SPIDER,
			{
				{0,0,255},	// idle
				{0,0,255},	// move
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
			}},
		{"Matilda-X Skull",4,1,0,AI_Mattie2SkullOrHead,	// the skull of matilda (head becomes this when killed)
		 40,11,400,1000,"!12",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,0,255},	// idle
				{0,0,255},	// move
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
				{1,2,3,4,4,4,4,4,4,4,3,2,1,255},	// fire
			}},
		{"Matilda-X Brain",4,1,0,AI_Mattie2Brain,	// the brain of matilda (skull becomes this when killed)
		 40,7,100,5000,"!13",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,0,255},	// idle
				{0,0,255},	// move
				{0,0,255},	// no form of attack
				{1,2,3,2,1,0,4,5,6,5,4,0,1,2,3,2,1,0,4,5,6,5,255},	// die
				{0,0,255},	// no form of attack
				{0,0,255},	// no form of attack
				{0,0,255},	// no form of attack
				{0,0,255},	// no form of attack
				{0,0,255},	// no form of attack
			}},
		{"Matilda-X Body",4,1,0,AI_Mattie2Body,	// the body of matilda
		 50,7,1,1,"!14",0,MF_NOMOVE|MF_ONEFACE|MF_INVINCIBLE|MF_NOGRAV|MF_ENEMYWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{0,255},	// fire
				{1,2,0,3,4,3,2,0,3,4,3,0,1,4,2,4,3,1,2,255},	// die
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
			}},
		{"Matilda-X Tail",4,1,0,AI_Mattie2Tail,	// her tail
		 80,16,200,1000,"!17",0,MF_NOMOVE|MF_ONEFACE|MF_INVINCIBLE|MF_FACECMD|MF_NOGRAV|MF_ENEMYWALK|MF_FREEWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,1,2,3,2,1,255},	// fire
				{1,2,0,3,2,0,3,3,0,1,2,3,1,2,255},	// die
				{0,1,2,3,2,1,255},	// fire
				{0,1,2,3,2,1,255},	// fire
				{0,1,2,3,2,1,255},	// fire
				{0,1,2,3,2,1,255},	// fire
				{0,1,2,3,2,1,255},	// fire
			}},

		{"Pygmy Queen",4,7,3,AI_PygmyQueen,
		 10,35,50,200,"!69",0,0,
		 MT_EVIL|MT_PYGMY|MT_GENERATE|MT_HUMAN,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{8,9,10,9,8,7,11,12,13,12,11,7,255},	// move
				{20,21,22,22,22,21,20,255},	// attack
				{26,27,28,29,30,31,32,33,34,255},	// die
				{14,15,16,15,14,0,17,18,19,18,17,0,255},	// (A1) bored (unga dance)
				{23,24,25,24,23,0,23,24,25,24,23,255},	// (A2) angry, spotted Bouapha
				{20,21,22,22,22,21,20,255},	// attack
				{20,21,22,22,22,21,20,255},	// attack
				{20,21,22,22,22,21,20,255},	// attack
			}},
		{"Jalapeno",1,4,4,AI_Jalapeno,
		 8,14,5,100,"!85",0,0,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,9,8,7,255},	// attack
				{11,12,13,255},	// die
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
				{7,8,9,10,9,8,7,255},	// attack
			}},
		{"Generator",255,255,0,AI_Generator,		// once every second
		 30,10,100,1000,"graphics/genrtr.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_ENEMYWALK,
		 MT_EVIL|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,0,0,255}, //die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Generator",1,6,0,AI_Generator,		// once every 5 seconds
		 30,10,200,1000,"graphics/genrtr.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_ENEMYWALK,
		 MT_EVIL|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,0,0,255}, //die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Generator",1,3,0,AI_Generator,		// once every 15 seconds
		 30,10,400,1000,"graphics/genrtr.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_ENEMYWALK,
		 MT_EVIL|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,0,0,255}, //die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Generator",1,5,0,AI_Generator,		// once every 30 seconds
		 30,10,800,1000,"graphics/genrtr.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_ENEMYWALK,
		 MT_EVIL|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,0,0,255}, //die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Sneaky Shark",255,255,0,AI_Shark,
		 15,13,50,100,"graphics/shark.jsp",0,MF_ONEFACE|MF_AQUATIC|MF_WATERWALK,
		 MT_EVIL|MT_WACKY|MT_AQUATIC|MT_TRAP|MT_ANIMAL,
			{
				{0,1,2,3,4,5,6,255},	// idle
				{7,255},				// move
				{7,8,9,9,9,9,9,9,8,7,255},		// attack
				{10,11,12,255},			// die
				{7,8,9,9,8,7,255},		// A1=surprise!!!
				{7,8,9,9,9,9,9,9,8,7,255},		// attack
				{7,8,9,9,9,9,9,9,8,7,255},		// attack
				{7,8,9,9,9,9,9,9,8,7,255},		// attack
				{7,8,9,9,9,9,9,9,8,7,255},		// attack
			}},
		{"Mad Millennium Bug",3,4,8,AI_MadBug,
		 4,22,100,500,"!4",0,0,
		 MT_EVIL|MT_ANIMAL|MT_SPIDER|MT_MINIBOSS|MT_WACKY,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,0,255},	// move
				{12,13,14,13,12,255},	// attack
				{15,16,17,18,19,18,19,18,19,20,20,21,21,255},	// die
				{12,13,14,13,12,255},	// A1=shoot
				{12,13,14,13,12,255},	// attack
				{12,13,14,13,12,255},	// attack
				{12,13,14,13,12,255},	// attack
				{12,13,14,13,12,255},	// attack
			}},
		{"Wacky Wizard",4,3,0,AI_Wizard,
		 18,25,200,500,"!59",0,0,
		 MT_WACKY|MT_GOOD|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{20,21,22,23,24,24,24,254,24,254,24,254,24,254,24,255},	// die
				{7,9,11,12,12,12,12,12,12,11,9,7,255},	// A1=cast spell
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,255},	// attack=smack
			}},
		{"Evil Clone",1,3,0,AI_EvilClone,
		 11,29,128,0,"!1",0,0,
		 MT_EVIL|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{0,0,0,0,0,0,0,26,27,28,255},		// die
				{23,24,25,24,23,255},	// use item
				{17,18,19,20,21,22,21,20,21,22,21,20,19,18,17,255},	// bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,26,27,28,255},	// watery death
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
			}},
		{"Bob The Biscuit",255,255,-7,AI_Bob,
		 12,33,70,300,"!18",0,0,
		 MT_EVIL|MT_WACKY,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,255},	// move
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,19,20,19,20,21,22,23,255},	// triple punch
				{27,28,29,30,31,32,32,32,32,255},	// die
				{24,25,26,26,26,26,26,255},	// jumpkick (A1)
				{25,24,255},	// unjumpkick (A2)
				{23,22,21,20,19,20,19,20,21,22,23,255}, // summon help (A3)
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,19,20,19,20,21,22,23,255},	// triple punch
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,19,20,19,20,21,22,23,255},	// triple punch
			}},
		{"MultiMoss",1,6,0,AI_MultiMoss,
		 18,17,10,250,"!22",0,MF_ONEFACE|MF_ENEMYWALK|MF_FREEWALK|MF_NOSHADOW,
		 MT_EVIL|MT_TRAP|MT_VEGGIE,
			{
				{0,1,2,3,4,5,6,255},	// idle
				{0,1,2,3,4,5,6,255},	// move
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{22,23,24,25,26,27,28,255},	// die
				{28,27,26,25,24,23,22,255},	// A1=reborn!
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,255},	// attack
			}},
		{"Moss Rapido",1,5,0,AI_MossRapido,
		 15,17,1,25,"!21",0,MF_NOMOVE|MF_ONEFACE|MF_FREEWALK|MF_ENEMYWALK|MF_WALLWALK|MF_NOSHADOW,
		 MT_EVIL|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,255},	// move=grow vertically
				{4,5,6,5,4,255},	// attack=grow horizontally
				{7,8,9,10,11,12,13,14,15,16,255},	// die
				{4,5,6,5,4,255},	// attack=grow horizontally
				{4,5,6,5,4,255},	// attack=grow horizontally
				{4,5,6,5,4,255},	// attack=grow horizontally
				{4,5,6,5,4,255},	// attack=grow horizontally
				{4,5,6,5,4,255},	// attack=grow horizontally
			}},
		{"Snowball",2,0,10,AI_Snowball,
		 35,11,2000,1000,"!43",0,MF_NOMOVE|MF_ONEFACE|MF_WATERWALK,
		 MT_EVIL|MT_TRAP|MT_ARCTIC,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
			}},
		{"Snowball",2,0,10,AI_Snowball,
		 35,11,2000,1000,"!43",0,MF_NOMOVE|MF_ONEFACE|MF_WATERWALK,
		 MT_EVIL|MT_TRAP|MT_ARCTIC,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
			}},
		{"Snow Blower",255,255,10,AI_Snowblower,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_INVINCIBLE,
		 MT_EVIL|MT_TRAP|MT_ARCTIC,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Boomkin",2,4,5,AI_Pumpkin,
		 11,6,1,500,"!19",0,0,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move
				{0,255},	// attack unused
				{0,0,255},	// die (just long enough to spew shrapnel)
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
			}},
		{"Manic Mumble",0,7,4,AI_Mumble,
		 15,28,20,100,"!39",0,MF_NOMOVE,
		 MT_EVIL|MT_SPOOKY|MT_DESERT,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,6,7,8,9,10,0,255},	// move
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{19,20,21,22,23,24,25,26,27,255},	// die
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
			}},
		{"BuddyBot",0,7,-2,AI_Minibot,
		 10,6,5,50,"!79",0,0,
		 MT_GOOD|MT_HIGHTECH,
			{
				{0,255},	// idle
				{1,2,1,0,255},	// move
				{0,0,255},	// attack
				{3,4,5,5,5,255},	// die
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
				{0,0,255},	// attack
			}},
		{"HelpyBot",0,7,-2,AI_Meaniebot,
		 13,10,30,100,"!80",0,0,
		 MT_GOOD|MT_HIGHTECH,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{5,6,6,6,6,6,5,255},	// attack
				{7,8,9,9,9,9,255},	// die
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
			}},
		{"Xeno Queen",0,6,0,AI_XenoMama,
		 13,29,200,350,"!76",0,MF_ENEMYWALK,
		 MT_EVIL|MT_GENERATE|MT_MINIBOSS|MT_ALIEN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{15,16,17,18,19,20,21,22,255},	// attack
				{23,24,25,26,27,28,255},	// die
				{7,8,9,10,11,12,13,14,255},	// A1=spit
				{15,16,17,18,19,20,21,22,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// attack
			}},
		{"Rumbly Tumbly",255,255,4,AI_Roller,
		 35,11,50,100,"!43",0,MF_NOMOVE|MF_ONEFACE|MF_WATERWALK,
		 MT_EVIL|MT_TRAP|MT_DESERT,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
			}},
		{"Rumbly Tumbly",255,255,4,AI_Roller,
		 35,11,50,100,"!43",0,MF_NOMOVE|MF_ONEFACE|MF_WATERWALK,
		 MT_EVIL|MT_TRAP|MT_DESERT,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
			}},
		{"Dark Vampire",6,0,-6,AI_Vampire,
		 12,13,150,500,"!63",0,MF_INVINCIBLE,
		 MT_EVIL|MT_VAMPIRE|MT_SPOOKY,
			{
				{1,2,1,0,255},	// idle
				{1,2,1,0,255},	// move
				{3,4,5,6,7,8,9,255},	// attack
				{12,11,10,255},		// die
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
				{3,4,5,6,7,8,9,255},	// attack
			}},
		{"Grabby Gnome",2,1,0,AI_Gnome,
		 5,50,15,100,"!86",0,0,
		 MT_EVIL|MT_WACKY|MT_PYGMY|MT_HUMAN,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{8,9,10,9,8,7,11,12,13,12,11,7,255},	// move
				{29,30,31,31,32,33,34,35,36,255},	// attack
				{37,38,39,40,41,42,43,44,45,46,255},	// die
				{14,15,16,17,18,19,20,16,17,18,19,15,14,255},	// (A1) bored (spin spear)
				{21,22,23,24,25,26,26,26,27,28,27,26,27,28,27,26,255}, // (A2) bored (yawn)
				{47,48,49,48,47,0,47,48,49,48,47,255},	// (A3) angry, spotted Bouapha
				{29,30,31,31,32,33,34,35,36,255},	// attack
				{29,30,31,31,32,33,34,35,36,255},	// attack
			}},
		{"Nobody",255,255,0,AI_Nobody,
		 1,1,1,1,"!1",0,MF_INVINCIBLE|MF_NOHIT|MF_FREEWALK|MF_ENEMYWALK|MF_WALLWALK,
		 MT_NONE,
			{
				{254,255},	// idle
				{254,255},	// move
				{254,255},	// attack
				{254,255},	// die
				{254,255},	// (A1) bored (spin spear)
				{254,255}, // (A2) bored (yawn)
				{254,255},	// (A3) angry, spotted Bouapha
				{254,255},	// attack
				{254,255},	// attack
			}},
		{"Buddy Bunny",2,0,3,AI_BuddyBunny,
		 10,8,30,1000,"!83",0,MF_NOMOVE,
		 MT_GOOD|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,0,255},	// move
				{0,255},	// attack
				{0,0,0,0,255},	// die
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
			}},
		{"Rock Trooper",6,2,0,AI_RockTrooper,
		 40,23,100,1000,"!78",0,MF_NOMOVE,
		 MT_EVIL|MT_HIGHTECH,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{16,17,18,19,20,21,22,22,22,255},	// die
				{5,6,7,7,8,9,255},	// A1=lob grenade
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{10,11,11,11,12,13,14,14,15,255},	// attack
				{10,11,11,11,12,13,14,14,15,255},	// attack
			}},
		{"Jacko",255,255,-2,AI_BigPumpkin,	// big pumpkin
		 14,1,30,500,"!52",0,0,
		 MT_EVIL|MT_PUMPKIN|MT_VEGGIE,
			{
				{0,255},	// idle
				{0,0,0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,0,0,255},	// move
				{0,0,0,255},	// move
				{0,0,0,255},	// move
				{0,0,0,255},	// move
				{0,0,0,255},	// move
			}},
		{"Crazybone",0,3,0,AI_CrazyBone,
		 8,38,10,100,"!2",0,0,
		 MT_EVIL|MT_SPOOKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{20,21,22,23,24,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},		// die
				{25,26,27,28,29,30,255},	// chomp/spit projectile
				{31,32,32,32,31,255},	// point at bouapha
				{16,15,14,13,12,11,10,9,8,7,255},	// resurrect=A3
				{20,21,22,23,24,255},	// attack
				{20,21,22,23,24,255},	// attack
			}},
		{"Creepazoid",255,255,10,AI_Creepazoid,
		 15,20,15,75,"!10",0,MF_WATERWALK|MF_WALLWALK|MF_FREEWALK|MF_ENEMYWALK|MF_NOSHADOW|MF_GLOW|MF_FLYING,
		 MT_EVIL|MT_ZOID|MT_SPOOKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{13,14,15,16,17,18,19,255},	// die
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{4,5,6,7,8,9,10,11,12,255},	// spit
				{4,5,6,7,8,9,10,11,12,255},	// spit
			}},
		{"LazyBones",0,2,-4,AI_Lazybones,
		 8,38,30,50,"!2",0,MF_NOMOVE,
		 MT_SPOOKY|MT_EVIL,
			{
				{13,13,13,13,13,13,13,13,13,13,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{25,26,27,28,28,28,28,28,28,28,28,28,28,28,28,28,29,30,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},		// die
				{13,12,11,10,9,8,7,255},	// wake up=A1
				{7,8,9,10,11,12,13,255},		// go back to sleep=A2
				{25,26,27,28,28,28,28,28,28,28,28,28,28,28,28,28,29,30,255},	// attack
				{25,26,27,28,28,28,28,28,28,28,28,28,28,28,28,28,29,30,255},	// attack
				{25,26,27,28,28,28,28,28,28,28,28,28,28,28,28,28,29,30,255},	// attack
			}},
		{"Starey Bat",2,1,-1,AI_StareyBat,
		 8,27,15,25,"!3",0,MF_FLYING|MF_NOMOVE,
		 MT_SPOOKY|MT_FLYING|MT_EVIL|MT_ANIMAL|MT_TRAP,
			{
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// idle
				{0,255},	// move
				{7,8,9,9,9,9,9,8,7,255},	// attack
				{17,18,19,20,21,22,23,24,25,26,255},		// die
				{7,8,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,8,7,255},	// attack
				{7,8,9,9,9,9,9,8,7,255},	// attack
			}},
		{"Madman",5,2,1,AI_Madman,
		 12,21,10,100,"!53",0,0,
		 MT_EVIL|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,255},	// attack=stab
				{15,16,17,18,19,20,254,20,254,20,254,20,254,20,255},	// die
				{7,8,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,13,14,255},	// attack=stab
			}},
		{"Pygmy Accountant",255,255,0,AI_PygmyAcct,
		 10,41,25,100,"graphics/pygacct.jsp",0,0,
		 MT_EVIL|MT_PYGMY|MT_HUMAN|MT_URBAN,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{7,8,9,8,7,0,10,11,12,11,10,0,255},	// move
				{24,25,26,27,28,29,30,31,255},	// attack
				{32,33,34,35,36,37,38,39,40,255},	// die
				{13,14,15,16,17,18,18,18,18,18,18,17,16,15,14,13,255},	// (A1) bored (read paper)
				{19,20,21,22,23,22,21,22,23,22,21,20,19,255},	// (A2) angry, spotted Bouapha
				{24,25,26,27,28,29,30,31,255},	// attack
				{24,25,26,27,28,29,30,31,255},	// attack
				{24,25,26,27,28,29,30,31,255},	// attack
			}},
		{"Mr. Crazypants",255,255,0,AI_Crazypants,
		 12,35,80,100,"graphics/crazypants.jsp",0,0,
		 MT_EVIL|MT_HUMAN|MT_MINIBOSS,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,15,16,17,18,19,8,7,255},	// attack=dash
				{29,30,31,31,32,32,33,33,34,255},	// die
				{20,21,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,21,20,255},	// A1=spin attack
				{23,24,25,26,27,28,27,26,25,26,27,28,27,26,25,24,23,255},	// A2=angry
				{7,8,9,10,11,12,13,14,15,16,17,18,19,8,7,255},	// attack=dash
				{7,8,9,10,11,12,13,14,15,16,17,18,19,8,7,255},	// attack=dash
				{7,8,9,10,11,12,13,14,15,16,17,18,19,8,7,255},	// attack=dash
			}},
		{"Yerfdog",0,0,0,AI_Yerfdog,
		 10,3,30,1000,"graphics/yerfdog.jsp",0,MF_NOHIT|MF_FREEWALK|MF_ENEMYWALK|MF_WATERWALK|MF_ONEFACE|MF_FACECMD,
		 MT_GOOD|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,0,255},	// move
				{0,255},	// attack
				{0,0,0,0,255},	// die
				{1,2,0,255},	// move
				{1,2,0,255},	// move
				{1,2,0,255},	// move
				{1,2,0,255},	// move
				{1,2,0,255},	// move
			}},
		{"Elite SEAL",1,4,0,AI_EliteSeal,
		 22,20,50,125,"!31",0,0,
		 MT_EVIL|MT_ANIMAL|MT_ARCTIC,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move
				{6,6,9,10,13,14,255},	// attack=full auto
				{15,16,17,18,19,19,19,255},	// die
				{6,6,9,10,13,14,255},	// attack=full auto
				{6,6,9,10,13,14,255},	// attack=full auto
				{6,6,9,10,13,14,255},	// attack=full auto
				{6,6,9,10,13,14,255},	// attack=full auto
				{6,6,9,10,13,14,255},	// attack=full auto
			}},
		{"Creeping Cone",255,255,0,AI_Cone,
		 8,21,30,25,"graphics/cone.jsp",0,0,
		 MT_EVIL|MT_URBAN|MT_WACKY|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,0,255},	// move
				{8,9,10,9,8,255},	// attack=poke
				{17,18,19,20,255},	// die
				{11,12,13,14,14,15,16,15,14,13,12,11,255}, // A1=shoot
				{8,9,10,9,8,255},	// attack=poke
				{8,9,10,9,8,255},	// attack=poke
				{8,9,10,9,8,255},	// attack=poke
				{8,9,10,9,8,255},	// attack=poke
			}},
		{"Pygmy Diver",255,255,0,AI_PygmyDiver,
		 10,54,15,25,"graphics/pygmydiver.jsp",0,MF_WATERWALK,
		 MT_EVIL|MT_UNDERSEA|MT_PYGMY|MT_HUMAN,
			{
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,255},	// idle
				{17,18,19,20,21,22,23,24,25,26,27,28,17,255},	// move
				{29,30,31,32,33,34,35,36,37,255},	// attack=throw spear
				{38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,255},	// die
				{15,16,16,255}, // A1=wake up
				{16,15,255},	// A2=go back to waiting
				{29,30,31,32,33,34,35,36,37,255},	// attack=throw spear
				{29,30,31,32,33,34,35,36,37,255},	// attack=throw spear
				{29,30,31,32,33,34,35,36,37,255},	// attack=throw spear
			}},
		{"Puffyfish",255,255,0,AI_Puffyfish,
		 8,23,300,25,"graphics/puffyfish.jsp",0,MF_FLYING|MF_WATERWALK,
		 MT_UNDERSEA|MT_ANIMAL|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,1,255},	// move
				{0,255},	// attack=unused
				{20,21,22,22,22,255},	// die=POP!
				{6,7,6,5,8,9,8,5,6,255}, // A1=move, fatter
				{11,12,11,10,13,14,13,10,11,255},	// A2=move, fatter yet
				{16,17,16,15,18,19,18,15,16,255},	// A3=move, fattest of all
				{20,21,22,22,22,255},	// die=POP!
				{20,21,22,22,22,255},	// die=POP!
			}},
		{"Puffyfish",255,255,0,AI_Puffyfish,
		 8,23,300,25,"!140",0,MF_FLYING|MF_WATERWALK,
		 MT_UNDERSEA|MT_ANIMAL|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,1,255},	// move
				{0,255},	// attack=unused
				{20,21,22,22,22,255},	// die=POP!
				{6,7,6,5,8,9,8,5,6,255}, // A1=move, fatter
				{11,12,11,10,13,14,13,10,11,255},	// A2=move, fatter yet
				{16,17,16,15,18,19,18,15,16,255},	// A3=move, fattest of all
				{20,21,22,22,22,255},	// die=POP!
				{20,21,22,22,22,255},	// die=POP!
			}},
		{"Olaf",1,6,-2,AI_Yeti2,
		 48,40,250,5000,"!36",0,MF_NOMOVE|MF_ENEMYWALK,
		 MT_EVIL|MT_BOSS|MT_ARCTIC|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,1,0,3,4,3,255},	// move
				{14,15,16,17,18,19,20,21,22,23,255},	// attack=snowball
				{35,36,37,38,39,39,39,39,39,39,39,39,39,39,39,39,39,39,255},	// die
				{28,29,30,31,32,33,33,33,34,34,255},	// A1=ground pound
				{5,6,7,8,9,10,11,12,13,255},	//A2=death yodel
				{24,25,26,27,26,25,26,27,26,25,26,27,26,25,24,255},	// A3=wave hello
				{14,15,16,17,18,19,20,21,22,23,255},	// attack=snowball
				{14,15,16,17,18,19,20,21,22,23,255},	// attack=snowball
			}},
		{"Spikey",6,4,5,AI_Spikey,
		 12,13,200,500,"!63",0,0,
		 MT_EVIL|MT_VAMPIRE|MT_SPOOKY,
			{
				{1,2,1,0,255},	// idle
				{1,2,1,0,255},	// move
				{3,4,5,5,6,6,7,7,8,9,255},	// attack
				{12,11,10,255},		// die
				{3,4,5,5,6,6,7,7,8,9,255},	// attack
				{3,4,5,5,6,6,7,7,8,9,255},	// attack
				{3,4,5,5,6,6,7,7,8,9,255},	// attack
				{3,4,5,5,6,6,7,7,8,9,255},	// attack
				{3,4,5,5,6,6,7,7,8,9,255},	// attack
			}},
		{"BomBot",0,4,-4,AI_Bombot,
		 13,10,50,100,"!80",0,0,
		 MT_EVIL|MT_HIGHTECH,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{5,6,6,6,6,6,5,255},	// attack
				{7,8,9,9,9,9,255},	// die
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
				{5,6,6,6,6,6,5,255},	// attack
			}},
		{"Buddy Bunny",2,0,3,AI_FollowBunny,
		 10,8,30,1000,"!83",0,0,
		 MT_GOOD|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,0,255},	// move
				{0,255},	// attack
				{0,0,0,0,255},	// die
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
				{1,2,3,4,5,6,7,0,255},	// move
			}},
		{"Autozoid",255,255,0,AI_Autozoid,
		 15,16,150,1000,"graphics/autozoid.jsp",0,0,
		 MT_EVIL|MT_ZOID|MT_URBAN|MT_VEHICLE,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
			}},
		{"Treasure Chest",255,255,0,AI_Chest,
		 16,5,50,1000,"graphics/chest.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{1,2,3,4,255},	// die
				{1,2,3,4,255},	// die
				{1,2,3,4,255},	// die
				{1,2,3,4,255},	// die
				{1,2,3,4,255},	// die
				{1,2,3,4,255},	// die
			}},
		{"Jellofish",255,255,0,AI_Jellofish,
		 8,22,10,1000,"graphics/jello.jsp",0,MF_FLYING,
		 MT_EVIL|MT_UNDERSEA|MT_ANIMAL,
			{
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack
				{16,17,18,19,20,21,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
			}},
		{"King Cone",255,255,0,AI_KingCone,
		 64,27,500,1000,"graphics/kingcone.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD|MF_ENEMYWALK,
		 MT_EVIL|MT_WACKY|MT_URBAN|MT_MINIBOSS,
			{
				{0,255},	// idle
				{5,6,7,7,8,8,9,0,255},	// move (hop to summon cones)
				{10,11,12,13,14,15,16,17,18,255},	// attack (crown spin)
				{19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,255},	// die
				{10,11,12,13,14,15,16,17,18,255},	// attack (crown spin)
				{10,11,12,13,14,15,16,17,18,255},	// attack (crown spin)
				{10,11,12,13,14,15,16,17,18,255},	// attack (crown spin)
				{10,11,12,13,14,15,16,17,18,255},	// attack (crown spin)
				{10,11,12,13,14,15,16,17,18,255},	// attack (crown spin)
			}},
		{"Mailboxer",255,255,0,AI_Mailboxer,
		 12,32,40,1000,"graphics/mboxer.jsp",0,0,
		 MT_EVIL|MT_WACKY|MT_URBAN,
			{
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// idle
				{21,22,21,0,23,24,23,0,255},	// move
				{7,8,9,10,11,12,13,255},	// attack
				{25,26,27,28,29,30,31,254,31,254,31,254,31,254,31,254,31,255},	// die
				{14,14,15,15,16,16,17,17,18,19,20,255},	// A1=punch2
				{7,8,9,10,11,12,13,255},	// attack
				{7,8,9,10,11,12,13,255},	// attack
				{7,8,9,10,11,12,13,255},	// attack
				{7,8,9,10,11,12,13,255},	// attack
			}},
		{"Piranha",255,255,0,AI_Piranha,
		 7,14,5,1000,"graphics/piranha.jsp",0,MF_FLYING,
		 MT_EVIL|MT_UNDERSEA|MT_ANIMAL,
			{
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// idle
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// move
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// attack
				{7,8,9,10,11,12,13,255},	// die
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// attack
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// attack
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// attack
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// attack
				{0,1,2,3,2,1,0,4,5,6,5,4,255},	// attack
			}},
		{"You-Go",255,255,0,AI_Yugo,
		 14,16,5,1000,"graphics/yugo.jsp",0,MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_FREEWALK|MF_WATERWALK,
		 MT_VEHICLE|MT_GOOD,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
			}},
		{"Mini-Sub",255,255,0,NULL,	// in mini-sub
		 11,1,128,0,"graphics/minisub.jsp",0,MF_FLYING,
		 MT_WEIRD,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255}, // die
				{0,255},	// fire missiles
				{0,255},	// activate
				{0,0,0,255}, // die
				{0,0,0,255}, // die
				{0,0,0,255}, // die
			}},
		{"Punkin",255,255,0,AI_Punkin,
		 9,10,10,1000,"graphics/punkin.jsp",0,0,
		 MT_EVIL|MT_PUMPKIN|MT_VEGGIE|MT_URBAN,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{0,255},	// attack
				{0,0,0,0,255},	// die
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{0,1,2,3,4,5,6,7,8,9,255},	// move
			}},
		{"Sea Cucumber",255,255,0,AI_SeaCuke,
		 11,32,100,1000,"graphics/seacuke.jsp",0,0,
		 MT_EVIL|MT_VEGGIE|MT_UNDERSEA|MT_MINIBOSS,
			{
				{0,255},	// idle
				{9,10,11,12,11,10,9,13,14,15,14,13,9,255},	// move
				{1,2,3,4,5,6,6,7,7,8,255},	// attack
				{25,26,27,28,29,30,31,254,31,254,31,254,31,254,31,255},	// die
				{16,17,18,19,20,21,22,23,24,255}, // A1=dance
				{1,2,3,4,5,6,6,7,7,8,255},	// attack
				{1,2,3,4,5,6,6,7,7,8,255},	// attack
				{1,2,3,4,5,6,6,7,7,8,255},	// attack
				{1,2,3,4,5,6,6,7,7,8,255},	// attack
			}},
		{"Sneaky Shark",255,255,0,AI_Sneaky2,
		 11,32,50,1000,"graphics/snkyshrk2.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_UNDERSEA|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,6,5,4,3,2,1,255},	// attack
				{0,0,0,255},	// die
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,6,5,4,3,2,1,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,6,5,4,3,2,1,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,6,5,4,3,2,1,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,6,5,4,3,2,1,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,6,5,4,3,2,1,255},	// attack
			}},
		{"Starfish",255,255,0,AI_Starfish,
		 11,31,15,1000,"graphics/starfish.jsp",0,0,
		 MT_EVIL|MT_UNDERSEA|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,10,9,8,7,6,5,4,2,1,0,255},	// move
				{11,12,13,14,15,16,17,18,19,12,11,255},	// attack
				{20,21,22,23,24,25,26,27,28,29,30,255},	// die
				{12,11,255},	// A1=stop buzzsawing
				{2,1,0,255},	// A2=stop walking
				{11,12,13,14,15,16,17,18,19,12,11,255},	// attack
				{11,12,13,14,15,16,17,18,19,12,11,255},	// attack
				{11,12,13,14,15,16,17,18,19,12,11,255},	// attack
			}},
		{"Aquazoid",255,255,0,AI_Underzoid,
		 17,29,15,1000,"graphics/underzoid.jsp",0,MF_FLYING,
		 MT_EVIL|MT_UNDERSEA|MT_ZOID,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,255},	// move
				{12,13,14,15,16,17,18,19,20,255},	// attack
				{21,22,23,24,25,26,27,28,254,28,254,28,254,28,254,255},	// die
				{12,13,14,15,16,17,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,19,20,255},	// attack
			}},
		{"Wet Seal",255,255,0,AI_WetSeal,
		 22,22,20,1000,"graphics/wetseal.jsp",0,MF_FLYING,
		 MT_EVIL|MT_UNDERSEA|MT_ANIMAL|MT_ARCTIC,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,7,8,9,8,9,10,11,10,11,12,13,12,13,14,14,14,15,15,16,255},	// attack=full auto
				{17,18,19,20,21,255},	// die
				{7,7,8,9,8,9,10,11,10,11,12,13,12,13,14,14,14,15,15,16,255},	// attack=full auto
				{7,7,8,9,8,9,10,11,10,11,12,13,12,13,14,14,14,15,15,16,255},	// attack=full auto
				{7,7,8,9,8,9,10,11,10,11,12,13,12,13,14,14,14,15,15,16,255},	// attack=full auto
				{7,7,8,9,8,9,10,11,10,11,12,13,12,13,14,14,14,15,15,16,255},	// attack=full auto
				{7,7,8,9,8,9,10,11,10,11,12,13,12,13,14,14,14,15,15,16,255},	// attack=full auto
			}},
		{"Wet Willy",255,255,0,AI_WetWilly,
		 12,20,30,1000,"graphics/willy.jsp",0,0,
		 MT_EVIL|MT_URBAN|MT_WACKY,
			{
				{0,1,2,3,4,5,6,7,8,9,10,11,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,255},	// move
				{12,13,14,15,15,15,15,15,14,13,12,255},	// attack
				{16,17,18,19,17,18,19,18,19,255},	// die
				{12,13,14,15,15,15,15,15,14,13,12,255},	// attack
				{12,13,14,15,15,15,15,15,14,13,12,255},	// attack
				{12,13,14,15,15,15,15,15,14,13,12,255},	// attack
				{12,13,14,15,15,15,15,15,14,13,12,255},	// attack
				{12,13,14,15,15,15,15,15,14,13,12,255},	// attack
			}},
		{"Crab Puff",255,255,0,AI_CrabPuff,
		 7,5,1,1000,"graphics/puff.jsp",0,0,
		 MT_EVIL|MT_UNDERSEA|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,3,4,3,2,1,255},	// move
				{0,255},	// attack
				{0,0,255},	// die
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
			}},
		{"Pumpkin Patch",255,255,0,AI_Patch,
		 16,17,150,1000,"graphics/patch.jsp",0,MF_NOMOVE|MF_ENEMYWALK|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
			}},
		{"Pumpkin Patch",255,255,2,AI_Patch,
		 16,17,150,1000,"!162",0,MF_NOMOVE|MF_ENEMYWALK|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
			}},
		{"Boomkin Patch",1,4,0,AI_Patch,
		 16,17,150,1000,"!162",0,MF_NOMOVE|MF_ENEMYWALK|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
			}},
		{"Squash Patch",255,255,4,AI_Patch,
		 16,17,150,1000,"!162",0,MF_NOMOVE|MF_ENEMYWALK|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_VEGGIE|MT_PUMPKIN|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{3,4,5,6,7,8,9,10,255},	// A1=summon
			}},
		{"Crabby Patty",255,255,0,AI_Patty,
		 34,17,300,1000,"graphics/patty.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_UNDERSEA|MT_BOSS|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{8,9,10,10,10,10,10,9,8,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{5,6,6,6,6,6,6,6,7,255},	// A1=summon
				{8,9,10,10,10,10,10,9,8,255},	// attack
				{8,9,10,10,10,10,10,9,8,255},	// attack
				{8,9,10,10,10,10,10,9,8,255},	// attack
				{8,9,10,10,10,10,10,9,8,255},	// attack
			}},
		{"Dozer The Dozerian",255,255,0,AI_Dozer,
		 140,17,1000,1000,"graphics/dozer.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_FACECMD,
		 MT_EVIL|MT_VEHICLE|MT_BOSS|MT_URBAN,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,4,5,6,4,5,6,4,5,6,3,2,1,255},	// attack (left spin)
				{29,29,30,30,31,31,255},	// die
				{7,8,9,10,11,12,10,11,12,10,11,12,10,11,12,9,8,7,255},	// A1=right spin
				{13,14,15,16,17,18,18,255},	// A2=lose arms
				{0,0,0,0,0,0,255},	// A3=grill explode
				{1,2,3,4,5,6,4,5,6,4,5,6,4,5,6,3,2,1,255},	// attack (left spin)
				{1,2,3,4,5,6,4,5,6,4,5,6,4,5,6,3,2,1,255},	// attack (left spin)
			}},
		{"Dark Coffin",6,0,-6,AI_Coffin,
		 25,20,8000,500,"!64",0,MF_NOMOVE|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_TRAP|MT_VAMPIRE|MT_SPOOKY,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{17,18,19,255},		// die
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
				{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,255},	// attack
			}},
		{"Mutant Zombie",0,1,0,AI_MutantZombie,
		 11,32,30,150,"!6",0,0,
		 MT_SPOOKY|MT_ZOMBIE|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{20,21,22,23,24,25,26,27,28,29,30,31,255},	// die
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,9,10,11,12,13,13,14,15,16,17,18,19,255},	// attack
			}},
		{"Whatsit",1,4,-4,AI_Whatsit,
		 12,44,40,50,"!20",0,0,
		 MT_THINGY|MT_EVIL,
			{
				{12,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,255},	// move
				{20,21,22,23,24,25,26,27,255},	// attack
				{34,35,36,37,38,39,40,41,42,43,255},	// die
				{13,14,15,16,17,16,15,16,17,16,15,16,17,16,15,255},	// fall asleep (A1)
				{18,19,18,255},	// blink (A2)
				{28,29,30,29,28,255}, // look right (A3)
				{31,32,33,32,31,255}, // look left (A4)
				{20,21,22,23,24,25,26,27,255},	// attack
		}},
		{"Shocking Horror",255,255,3,AI_ShockGhost,
		 18,7,15,300,"graphics/ghost.jsp",0,MF_WALLWALK|MF_NOSHADOW|MF_FLYING|MF_GHOST|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_SPOOKY|MT_FLYING,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,3,3,2,1,255},	// attack (scream)
				{4,4,5,5,6,6,255},		// die
				{4,5,6,6,5,4,255},		// A1=teleport
				{254,255},				// A2=idle invisible
				{6,5,4,255},			// A3=materialize
				{1,2,3,3,3,2,1,255},	// attack (scream)
				{1,2,3,3,3,2,1,255},	// attack (scream)
			}},
		{"Mechazoid",1,0,5,AI_Mechazoid,
		 17,29,20,1000,"!158",0,MF_FLYING,
		 MT_EVIL|MT_HIGHTECH|MT_ZOID|MT_FLYING,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,255},	// move
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{21,22,23,24,25,26,27,28,254,28,254,28,254,28,254,255},	// die
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
			}},
		{"Sucker",0,1,-5,AI_Sucker,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"Sucker: Strong",0,1,0,AI_Sucker,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"Sucker: Mini",0,1,5,AI_Sucker,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"Sucker: Strong Mini",0,1,10,AI_Sucker,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"Blower",0,4,-5,AI_Blower,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"Blower: Strong",0,4,0,AI_Blower,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"Blower: Mini",0,4,5,AI_Blower,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"Blower: Strong Mini",0,4,10,AI_Blower,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_INVINCIBLE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP,
			{
				{254,254,254,255},	// idle
				{254,254,254,255},	// move
				{254,254,254,255},	// attack
				{254,255},		// die
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
				{254,254,254,255},	// move
			}},
		{"The Beetles",7,0,-8,AI_Beetle,
		 7,5,1,1000,"!161",0,0,
		 MT_EVIL|MT_DESERT|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,3,4,3,2,1,255},	// move
				{0,255},	// attack
				{0,0,255},	// die
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
				{0,1,2,3,4,3,2,1,255},	// move
			}},
		{"Scary Scarab",6,4,-3,AI_Scarab,
		 34,17,150,1000,"!166",0,MF_NOMOVE,
		 MT_EVIL|MT_DESERT|MT_MINIBOSS|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{8,9,10,10,10,10,10,10,10,10,9,8,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{5,6,6,6,6,6,6,6,7,255},	// A1=summon
				{8,9,10,10,10,10,10,10,10,10,9,8,255},	// attack
				{8,9,10,10,10,10,10,10,10,10,9,8,255},	// attack
				{8,9,10,10,10,10,10,10,10,10,9,8,255},	// attack
				{8,9,10,10,10,10,10,10,10,10,9,8,255},	// attack
			}},
		{"Frostbiter",1,7,3,AI_Frostbiter,
		 8,21,10,50,"!5",0,0,
		 MT_EVIL|MT_ARCTIC|MT_SPIDER|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{14,15,16,17,18,17,18,17,18,19,19,20,20,255},	// die
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
			}},
		{"Missile Turret",0,3,4,AI_Turret,
		 10,1,80,200,"!82",0,MF_NOMOVE,
		 MT_EVIL|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Death Turret",0,7,-4,AI_Turret,
		 10,1,80,200,"!82",0,MF_NOMOVE,
		 MT_EVIL|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Magmazoid",1,4,0,AI_UnderMagmazoid,
		 17,29,20,1000,"!158",0,MF_FLYING,
		 MT_EVIL|MT_ZOID|MT_UNDERSEA,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,255},	// move
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{21,22,23,24,25,26,27,28,254,28,254,28,254,28,254,255},	// die
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
				{12,13,14,15,16,17,18,18,18,18,18,19,20,255},	// attack
			}},
		{"Parked Car",4,3,0,AI_ParkedCar,
		 15,16,15,1000,"!152",0,MF_NOMOVE,
		 MT_VEHICLE|MT_TRAP|MT_URBAN,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
			}},
		{"Parked Car",4,5,0,AI_ParkedCar,
		 15,16,15,1000,"!152",0,MF_NOMOVE,
		 MT_VEHICLE|MT_TRAP|MT_URBAN,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
			}},
		{"Traffic",4,6,0,AI_Traffic,
		 11,16,200,1000,"!152",0,MF_NOMOVE|MF_WALLWALK,
		 MT_VEHICLE|MT_TRAP|MT_EVIL|MT_URBAN,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
			}},
		{"Traffic",4,1,0,AI_Traffic,
		 11,16,200,1000,"!152",0,MF_NOMOVE|MF_WALLWALK,
		 MT_VEHICLE|MT_TRAP|MT_EVIL|MT_URBAN,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
			}},
		{"MechaBouapha",0,4,0,NULL,	// playable version
		 11,24,250,1000,"graphics/mechabouapha.jsp",0,MF_WATERWALK,
		 MT_WEIRD,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,0,0,255},	// attack=hammer toss
				{15,16,17,18,19,20,21,22,23,255},	// die
				{9,11,13,14,13,255},	// A1=use weapon
				{15,16,17,18,19,20,21,22,23,21,19,17,15,255},	// A2=bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,17,19,22,255},	// A3=watery death
				{7,8,9,10,11,12,13,14,0,0,255},	// attack=hammer toss
				{7,8,9,10,11,12,13,14,0,0,255},	// attack=hammer toss
			}},
		{"Shroomlord",255,255,0,AI_ShroomLord,
		 15,14,40,300,"graphics/shrmlord.jsp",0,0,
		 MT_EVIL|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,7,8,8,8,9,10,11,12,12,13,13,255},	// spit spores
				{0,0,255},	// die
				{7,7,8,8,8,9,10,11,12,12,13,13,255},	// spit spores
				{7,7,8,8,8,9,10,11,12,12,13,13,255},	// spit spores
				{7,7,8,8,8,9,10,11,12,12,13,13,255},	// spit spores
				{7,7,8,8,8,9,10,11,12,12,13,13,255},	// spit spores
				{7,7,8,8,8,9,10,11,12,12,13,13,255},	// spit spores
			}},
		{"Scarier Bat",255,255,0,AI_BigBat,
		 26,15,200,500,"graphics/bigbat.jsp",0,MF_FLYING|MF_NOMOVE|MF_ENEMYWALK,
		 MT_EVIL|MT_ANIMAL|MT_SPOOKY|MT_FLYING,
			{
				{0,1,2,1,0,3,4,5,4,3,255},	// idle
				{0,1,2,1,0,3,4,5,4,3,255},	// move
				{6,7,8,8,7,7,6,255},	// attack=spit bat
				{9,10,11,12,13,14,255},	// die
			}},
		{"Die Uberbat",255,255,0,AI_HugeBat,
		 64,7,1000,5000,"graphics/hugebat.jsp",0,MF_NOMOVE|MF_ENEMYWALK|MF_ONEFACE,
		 MT_EVIL|MT_ANIMAL|MT_SPOOKY|MT_MINIBOSS,
			{
				{0,0,255},	// idle
				{0,255},	// move
				{1,2,3,2,1,255},	// attack=spit bats
				{4,4,4,5,5,5,6,6,6,6,6,6,255},	// die
				{1,2,3,2,1,255},	// attack=spit bats
				{1,2,3,2,1,255},	// attack=spit bats
				{1,2,3,2,1,255},	// attack=spit bats
				{1,2,3,2,1,255},	// attack=spit bats
				{1,2,3,2,1,255},	// attack=spit bats
			}},
		{"Werepuppy",255,255,0,AI_Werewolf,
		 12,55,15,100,"graphics/wolfman.jsp",0,0,
		 MT_EVIL|MT_SPOOKY|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{44,45,46,47,48,49,50,51,51,51,52,53,54,254,54,254,54,254,54,255},		// die
				{7,8,9,9,8,7,255},	// spit projectile = A1
				{10,11,12,12,12,13,14,15,16,17,17,18,19,255},	// charging slash = A2
				{20,21,22,23,24,25,26,26,26,26,26,26,26,26,27,28,29,255},		// howl = A3
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
			}},
		{"Weremutt",2,4,0,AI_Werewolf,
		 12,55,25,100,"!195",0,0,
		 MT_EVIL|MT_SPOOKY|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{44,45,46,47,48,49,50,51,51,51,52,53,54,254,54,254,54,254,54,255},		// die
				{7,8,9,9,8,7,255},	// spit projectile = A1
				{10,11,12,12,12,13,14,15,16,17,17,18,19,255},	// charging slash = A2
				{20,21,22,23,24,25,26,26,26,26,26,26,26,26,27,28,29,255},		// howl = A3
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
			}},
		{"Patrol Mumble",0,2,0,AI_Patrol,
		 15,28,100,100,"!39",0,MF_NOMOVE,
		 MT_EVIL|MT_SPOOKY|MT_DESERT|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,6,7,8,9,10,0,255},	// move
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{19,20,21,22,23,24,25,26,27,255},	// die
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
			}},
		{"Patrol Mumble",0,2,0,AI_Patrol,
		 15,28,100,100,"!39",0,MF_NOMOVE,
		 MT_EVIL|MT_SPOOKY|MT_DESERT|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,6,7,8,9,10,0,255},	// move
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{19,20,21,22,23,24,25,26,27,255},	// die
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
			}},
		{"Death Patrol",0,4,0,AI_Patrol,
		 15,28,200,100,"!39",0,MF_NOMOVE,
		 MT_EVIL|MT_SPOOKY|MT_DESERT|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,6,7,8,9,10,0,255},	// move
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{19,20,21,22,23,24,25,26,27,255},	// die
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
			}},
		{"Death Patrol",0,4,0,AI_Patrol,
		 15,28,200,100,"!39",0,MF_NOMOVE,
		 MT_EVIL|MT_SPOOKY|MT_DESERT|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,6,7,8,9,10,0,255},	// move
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{19,20,21,22,23,24,25,26,27,255},	// die
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
				{11,12,13,14,15,16,17,18,255},	// attack=head crush
			}},
		{"Pygmy Weatherman",6,3,5,AI_Weatherman,
		 10,50,100,100,"!9",0,0,
		 MT_EVIL|MT_PYGMY|MT_HUMAN|MT_MINIBOSS,
			{
				{0,0,0,0,0,0,0,0,0,0,1,2,3,2,1,0,0,0,4,5,6,5,4,255},	// idle
				{8,9,10,9,8,7,11,12,13,12,11,7,255},	// move
				{29,30,31,31,32,33,33,34,34,35,35,36,36,255},	// attack
				{37,38,39,40,41,42,43,44,45,46,255},	// die
				{14,15,16,17,18,19,20,16,17,18,19,15,14,255},	// (A1) bored (spin spear)
				{47,48,49,48,47,0,47,48,49,48,47,255},	// (A2) angry, spotted Bouapha
				{29,30,31,31,32,33,33,34,34,35,35,36,36,255},	// attack
				{29,30,31,31,32,33,33,34,34,35,35,36,36,255},	// attack
				{29,30,31,31,32,33,33,34,34,35,35,36,36,255},	// attack
			}},
		{"Hunter Egg",0,6,0,AI_HunterEgg,
		 12,14,1000,200,"!74",0,MF_ONEFACE|MF_ENEMYWALK|MF_NOMOVE,
		 MT_EVIL|MT_ALIEN|MT_GENERATE,
			{
				{0,1,2,3,2,1,255},	// idle
				{0,255},	// move
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,255},	// die
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
				{4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,255},	// attack
			}},
		{"Stupid Shroom",1,4,0,NULL,
		 11,14,40,300,"!192",0,MF_WATERWALK,
		 MT_WEIRD,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,7,8,8,9,10,11,12,12,13,255},	// attack
				{0,0,0,0,0,0,0,0,0,0,255},	// die
				{7,8,10,11,13,255},	// use item
				{7,8,9,9,9,9,9,9,8,7,0,0,0,0,0,255},	// bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,0,0,0,255},	// watery death
				{7,7,8,8,9,10,11,12,12,13,255},	// attack
				{7,7,8,8,9,10,11,12,12,13,255},	// attack
			}},
		{"Punk Bunny",255,255,0,AI_PunkBunny,
		 8,16,40,100,"graphics/punkbunny.jsp",0,0,
		 MT_ANIMAL|MT_URBAN|MT_GOOD,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// walk
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{0,0,254,254,255},	// die
				{11,12,13,14,15,255},	// A1 = bomb throw
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
			}},
		{"LunaChick",255,255,0,NULL,	// playable char
		 11,27,250,1000,"graphics/lunachick.jsp",0,MF_WATERWALK,
		 MT_WEIRD,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,13,0,0,255},	// attack=fire gun
				{14,15,16,17,18,19,20,21,22,23,24,25,26,255},	// die
				{7,9,10,12,13,255},	// A1=use weapon
				{14,15,16,17,18,17,16,15,14,15,17,15,14,0,0,255},	// A2=bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,24,25,26,255},	// A3=watery death
				{7,8,9,10,11,12,13,13,0,0,255},	// attack=fire gun
				{7,8,9,10,11,12,13,13,0,0,255},	// attack=fire gun
			}},
		{"Skele-TON",4,1,-8,AI_Skeleton,
		 8,38,70,50,"!2",0,MF_NOMOVE,
		 MT_SPOOKY|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,9,9,9,9,9,9,8,8,8,7,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},		// die
				{25,26,27,28,29,30,255},	// chomp/spit projectile
				{31,32,32,32,31,255},	// point at bouapha
				{16,15,14,13,12,11,10,9,8,7,255},	// resurrect=A3
				{7,8,9,9,9,9,9,9,9,8,8,8,7,255},	// attack
				{7,8,9,9,9,9,9,9,9,8,8,8,7,255},	// attack
			}},
		{"Hot N' Sour",1,4,0,AI_HotShroom,
		 15,23,10,50,"!24",0,0,
		 MT_EVIL|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,255},	// attack
				{15,16,17,18,19,20,21,22,255},	// die
				{7,8,9,10,11,12,13,14,255},	// attack
				{7,8,9,10,11,12,13,14,255},	// attack
				{7,8,9,10,11,12,13,14,255},	// attack
				{7,8,9,10,11,12,13,14,255},	// attack
				{7,8,9,10,11,12,13,14,255},	// attack
			}},
		{"Microzoid",2,5,0,AI_Microzoid,
		 6,20,1,75,"graphics/minizoid.jsp",0,MF_WATERWALK|MF_AQUATIC,
		 MT_EVIL|MT_ZOID|MT_AQUATIC|MT_WACKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,255},	// move
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit something small
				{13,14,15,16,17,18,19,255},	// die
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit something small
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit something small
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit something small
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit something small
				{4,5,6,7,8,9,10,11,12,255},	// attack=spit something small
			}},
		{"A Little Crazy",5,1,0,AI_MiniWacko,
		 6,21,5,100,"graphics/miniwacko.jsp",0,0,
		 MT_EVIL|MT_HUMAN|MT_WACKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{15,16,17,18,19,20,254,20,254,20,254,20,254,20,255},	// die
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
				{7,8,9,10,11,12,11,10,9,10,11,12,11,10,9,10,11,12,13,14,255},	// attack=stab
			}},
		{"Jack Frost",4,7,2,AI_JackFrost,
		 50,23,350,2000,"!45",0,MF_NOMOVE|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_ARCTIC|MT_MINIBOSS,
			{
				{0,1,2,3,4,5,255},	// idle
				{0,1,2,3,4,5,255},	// move
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{17,18,19,20,21,22,255},	// die
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
				{6,7,8,9,10,11,12,13,14,15,16,255},	// attack=swipe pitchfork
			}},
		{"Kid Mystic",255,255,0,NULL,
		 11,25,128,0,"mystic/graphics/sylabulus.jsp",0,MF_WATERWALK,
		 MT_BITS,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,10,11,11,12,12,12,255},	// attack
				{15,16,17,18,19,20,21,22,23,24,255},		// die
				{13,13,14,14,13,13,255},	// cast non-pointy spell
				{7,8,9,10,11,12,12,255},	// cast pointy spell
				{1,3,1,0,4,6,4,0,1,3,1,0,0,254,254,254,254,255},	// watery death
				{24,23,22,21,20,19,18,17,16,15,255},		// A4=undie
				{0,255},
			}},
		{"Pterodactyl",255,255,0,AI_Ptero,
		 10,11,15,5,"mystic/graphics/ptero.jsp",0,MF_FLYING | MF_ENEMYWALK | MF_FREEWALK,
		 MT_GOOD|MT_ANIMAL,
			{
				{0,1,2,6,3,4,5,4,3,6,2,1,255},	// idle
				{0,1,2,6,3,4,5,4,3,6,2,1,255},	// move
				{7,8,9,10,9,8,7,255},	// fire
				{0,0,0,255},	// die
				{7,8,9,10,9,8,7,255},	// unused
				{7,8,9,10,9,8,7,255},	// unused
				{7,8,9,10,9,8,7,255},	// unused
				{7,8,9,10,9,8,7,255},	// unused
				{7,8,9,10,9,8,7,255},	// unused
			}},
		{"Eye Guy",255,255,0,AI_Eyeguy,
		 14,17,20,75,"mystic/graphics/eyeguy.jsp",0,0,
		 MT_EVIL|MT_THINGY,
			{
				{0,0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,255},	// attack
				{13,14,15,16,255},	// die
				{7,8,9,10,11,12,255},	// a1
				{7,8,9,10,11,12,255},	// a2
				{7,8,9,10,11,12,255},	// a3
				{7,8,9,10,11,12,255},	// a4
				{7,8,9,10,11,12,255},	// a5
			}},
		{"Oculoid",255,255,0,AI_Peeper, //good to go!
		 26,8,40,150,"mystic/graphics/peeper.jsp",0,MF_FLYING|MF_NOMOVE,
		 MT_EVIL|MT_THINGY|MT_FLYING,
			{
				{4,4,255},	// idle
				{4,4,255},	// move
				{5,6,7,6,5,255},	// fire
				{3,2,1,1,1,0,0,0,0,255},	// die
				{3,2,1,0,1,2,3,255},	// A1 = blink
				{0,0,1,1,2,2,3,3,255},	// A2 = wake up
				{5,6,7,6,5,255},	// a3
				{5,6,7,6,5,255},	// a4
				{5,6,7,6,5,255},	// a5
			}},
		{"The Rook",255,255,0,AI_Tower, //good to go
		 60,17,500,500,"mystic/graphics/tower.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_MINIBOSS,
			{
				{0,0,0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{5,5,6,6,7,7,7,8,9,9,9,9,9,10,10,10,11,11,11,255},	// attack (leap)
				{12,13,14,15,16,16,16,16,16,255},	// die
				{1,2,2,2,2,2,2,1,255},	// A1
				{1,2,2,2,2,2,2,1,255},	// A2
				{1,2,2,2,2,2,2,1,255},	// A3
				{1,2,2,2,2,2,2,1,255},	// A4
				{1,2,2,2,2,2,2,1,255},	// A5
			}},
		{"The Lookey-Loo",255,255,0,AI_Lookeyloo, //mind4
		 64,7,1000,2000,"mystic/graphics/lookyloo.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_SPRITEBOX,
		 MT_EVIL|MT_BOSS|MT_THINGY,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,6,6,6,6,6,6,6,6,6,5,4,3,2,1,255},	// eye-beam (frm 9 is shoot)
				{1,3,5,6,6,6,6,255},	// die
				{0,255}, //unused
				{0,255}, //unused
				{0,255}, //unused
				{0,255}, //unused
				{0,255}, //unused
			}},
		{"Pinkeye",255,255,0,AI_Pinkeye, //good to go
		 11,13,20,1,"mystic/graphics/pinkeye.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_THINGY,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,255},	// fire
				{9,10,11,12,12,11,10,9,9,255},	// die & resurrect (freezes on frm 3 until ready)
				{0,255},
				{1,2,3,4,5,6,7,8,255},	// unused
				{1,2,3,4,5,6,7,8,255},	// unused
				{1,2,3,4,5,6,7,8,255},	// unused
				{1,2,3,4,5,6,7,8,255},	// unused
			}},
		{"Microfish",255,255,0,AI_Microfish, //good to go
		 1,1,1,1,"mystic/graphics/micro.jsp",0,MF_AQUATIC|MF_NOSHADOW|MF_WATERWALK,
		 MT_EVIL|MT_ANIMAL|MT_AQUATIC|MT_WACKY,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack unused
				{0,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Grumpy Golem",255,255,0,AI_Golem, //good to go
		 32,26,100,0,"mystic/graphics/golem.jsp",0,MF_NOMOVE|MF_FREEWALK,
		 MT_GOOD,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// attack
				{15,16,17,18,19,20,21,21,21,254,21,254,21,254,21,255},	// die
				{22,23,24,25,255},	// form out of nothing (A1)
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
			}},
		{"Bobby Khan",255,255,0,AI_BobbyKhan, //good to go
		 48,46,1500,5000,"mystic/graphics/bobby.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_BOSS,
			{
				{0,0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,255},	// attack=sword slash
				{42,42,42,43,43,43,44,44,44,44,44,44,255},	// die
				{15,16,17,17,17,17,17,17,17,17,17,16,15,255},	//A1=block and reflect
				{18,19,19,19,20,21,22,23,24,25,26,27,28,28,29,29,255},			//A2=spin of doom
				{30,31,31,31,32,33,34,35,36,37,38,39,40,41,255},	// A3=stomp
				{45,45,45,17,17,255},	// A4=blocked a shot
				{7,8,9,10,11,12,13,14,255},	// unused attack
			}},
		{"Crazy Bush",255,255,0,AI_Crazybush, //good to go
		 10,8,20,5,"mystic/graphics/crazybush.jsp",0,0,
		 MT_EVIL|MT_WACKY|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{0,255},	// attack
				{5,6,7,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Octon",255,255,0,AI_Octopus, //good to go
		 19,21,20,40,"mystic/graphics/octopus.jsp",0,MF_AQUATIC|MF_WATERWALK,
		 MT_EVIL|MT_ZOID|MT_AQUATIC,
			{
				{5,6,7,6,255},	// idle
				{5,6,6,7,6,6,255},	// move
				{16,17,18,19,20,19,18,17,18,19,20,19,18,17,18,19,20,19,18,17,16,255},	// attack=spit from arisen postion
				{8,9,10,11,12,13,14,15,255},	// die
				{5,4,3,2,1,0,0,255},	// A1=arise
				{1,2,3,4,5,5,255},	// A2=descend
				{16,17,18,19,20,19,18,17,16,255},	// unused
				{16,17,18,19,20,19,18,17,16,255},	// unused
				{16,17,18,19,20,19,18,17,16,255},	// unused
			}},
		{"Blockhead",255,255,0,AI_Incabot, //good to go
		 12,23,5,2,"mystic/graphics/incabot.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_HIGHTECH|MT_TRAP,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack=unused
				{16,17,18,19,20,21,22,255},	// die
				{0,255},
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Blockhead Kiln",255,255,0,AI_Incagen, //good to go
		 40,16,100,100,"mystic/graphics/incagen.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_ENEMYWALK,
		 MT_EVIL|MT_HIGHTECH|MT_GENERATE|MT_TRAP,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move=summon
				{0,255},	// attack=unused
				{0,0,0,255},	// die=explode
				{0,255},	// idle
				{0,255},	// idle
				{0,255},	// idle
				{0,255},	// idle
				{0,255},	// idle
			}},
		{"Megablockhead",255,255,0,AI_Incagold, //good to go
		 11,16,2000,75,"mystic/graphics/incagold.jsp",0,MF_NOMOVE,
		 MT_EVIL | MT_HIGHTECH | MT_TRAP,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack=unused
				{0,255},	// die=doesn't!
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Megablockhead",255,255,0,AI_Incagold, //good to go
		 11,16,2000,75,"mystic/graphics/incagold.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_HIGHTECH|MT_TRAP,
			{
				{0,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255},	// move
				{0,255},	// attack=unused
				{0,255},	// die=doesn't!
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Log",255,255,0,AI_Raft, //good to go
		 15,10,1,50,"mystic/graphics/log.jsp",0,MF_WATERWALK|MF_AQUATIC|MF_FREEWALK|MF_NOHIT|MF_ONEFACE|MF_FACECMD|MF_ENEMYWALK,
		 MT_GOOD|MT_AQUATIC,
			{
				{0,1,2,3,4,5,6,7,8,9,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{0,255},	// attack=unused
				{0,1,2,255},	// die
				{0,255},	// still
				{0,1,2,3,4,5,6,7,8,9,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,255},	// idle
			}},
		{"Blocko the Great",255,255,0,AI_Incaboss, //fix lasers again
		 65,20,1,500,"mystic/graphics/incaboss.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_ENEMYWALK|MF_INVINCIBLE,
		 MT_EVIL|MT_MINIBOSS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,5,5,5,5,5,5,5,5,5,5,5,4,3,2,1,255},	// attack=open mouth flames
				{16,16,17,17,18,18,19,19,19,19,255},	// die (loop the end forever to leave the corpse)
				{1,2,3,4,5,6,7,8,9,10,11,12,8,9,10,11,12,7,6,5,4,3,2,255}, // A1=tongue
				{13,13,13,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,14,13,255},	// A2=eyebeam
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Blocko the Great",255,255,0,AI_Incatongue, //make invisible? - DONE!
		 45,1,400,100,"mystic/graphics/log.jsp",0,MF_NOMOVE|MF_ONEFACE|MF_ENEMYWALK,
		 0,
			{
				{254,255},	// idle
				{254,255},	// move
				{254,255},	// attack=unused
				{254,255},	// die
				{254,255},	// unused
				{254,255},	// unused
				{254,255},	// unused
				{254,255},	// unused
				{254,255},	// unused
			}},
		{"Peeping Bomb",255,255,0,AI_Peepbomb, //good to go
		 13,41,20,50,"mystic/graphics/peepbomb.jsp",0,MF_FLYING|MF_ONEFACE|MF_FACECMD,
		 MT_EVIL|MT_THINGY|MT_TRAP,
			{
				{0,0,0,0,255},	// idle
				{0,0,0,255},	// move
				{0,255},	// attack=rotate to next facing
				{32,33,34,35,36,37,38,39,40,255},	// die=explode
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Peeping Bomb",255,255,0,AI_Peepbomb, //good to go
		 13,41,20,50,"mystic/graphics/peepbomb.jsp",0,MF_FLYING|MF_ONEFACE|MF_FACECMD,
		 MT_EVIL|MT_THINGY|MT_TRAP,
			{
				{0,0,0,0,255},	// idle
				{0,0,0,255},	// move
				{0,255},	// attack=rotate to next facing
				{32,33,34,35,36,37,38,39,40,255},	// die=explode
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Trapazoid",255,255,0,AI_Trapazoid, //good to go
		 14,10,30,70,"mystic/graphics/trapazoid.jsp",0,MF_NOMOVE|MF_NOSHADOW,
		 MT_EVIL|MT_ZOID|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,9,255},	// attack=torch in a circle
				{6,5,4,3,2,1,0,255},	// die (turn into dud trapazoid)
				{0,255},	// hiding time!
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
			}},
		{"Trapazoid II",7,2,-2,AI_Trapazoid, //try something with the bullets
		 14,10,30,70,"mystic/graphics/trapazoid.jsp",0,MF_NOMOVE|MF_NOSHADOW,
		 MT_EVIL|MT_ZOID|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,9,255},	// attack=torch in a circle
				{6,5,4,3,2,1,0,255},	// die (turn into dud trapazoid)
				{0,255},	// hiding time!
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
				{1,2,3,4,5,6,7,8,9,9,255},	// unused
			}},
		{"Whackazoid",7,6,0,AI_Whackazoid, //check original
		 14,10,1,5,"mystic/graphics/trapazoid.jsp",0,MF_NOMOVE|MF_NOSHADOW,
		 MT_EVIL|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,7,7,7,7,7,7,7,7,7,6,5,4,3,2,1,255},	// attack=torch in a circle
				{6,5,4,3,2,1,0,255},	// die (turn into dud trapazoid)
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Slimy Slug",255,255,0,AI_Slug, //good to go
		 12,19,30,50,"mystic/graphics/slug.jsp",0,0,
		 MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,3,4,3,2,1,255},	// move
				{1,2,3,255},	// attack=unused
				{5,6,7,8,9,10,11,12,13,14,15,16,17,18,255},	// die
				{4,2,3,255},	// unused
				{1,2,3,255},	// unused
				{1,2,3,255},	// unused
				{1,2,3,255},	// unused
				{1,2,3,255},	// unused
			}},
		{"Sniper Snail",255,255,0,AI_Snail, //just fix the animation
		 12,19,45,60,"mystic/graphics/snail.jsp",0,0,
		 MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,3,4,3,2,1,255},	// move
				{4,5,6,5,4,3,2,1,255},	// attack=spit goop
				{5,6,7,8,9,10,11,12,13,14,15,16,17,18,255},	// die
				{5,6,7,8,9,10,11,12,13,14,14,14,255},	// A1=go into shell
				{13,12,11,10,9,8,7,6,5,5,255},	// A2=get out of shell
				{14,255},	// hiding in shell
				{4,5,6,5,4,3,2,1,255},	// unused
				{4,5,6,5,4,3,2,1,255},	// unused
			}},
		{"Smashball",255,255,0,AI_Ball, //good to go
		 14,1,40,500,"mystic/graphics/ball.jsp",0,MF_INVINCIBLE|MF_ENEMYWALK|MF_NOMOVE|MF_ONEFACE|MF_WATERWALK|MF_FREEWALK,
		 MT_GOOD|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,0,0,0,0,0,0,0,0,0,255},	// attack
				{0,255},	// die
				{0,0,0,0,0,0,0,0,0,0,255},	// attack
				{0,0,0,0,0,0,0,0,0,0,255},	// attack
				{0,0,0,0,0,0,0,0,0,0,255},	// attack
				{0,0,0,0,0,0,0,0,0,0,255},	// attack
				{0,0,0,0,0,0,0,0,0,0,255},	// attack
			}},
		{"Optimum Octon",255,255,0,AI_Octoboss, //good to go
		 90,15,450,300,"mystic/graphics/octoboss.jsp",0,MF_ONEFACE|MF_NOMOVE|MF_AQUATIC|MF_WATERWALK|MF_ENEMYWALK,
		 MT_EVIL|MT_ZOID|MT_BOSS|MT_AQUATIC,
			{
				{0,255},	// idle
				{0,255},	// move
				{10,11,12,13,14,14,14,14,14,14,14,14,14,13,12,11,10,255},	// attack=spit
				{1,2,3,4,5,6,7,8,9,255},	// die
				{10,11,12,13,14,14,14,14,14,14,14,14,14,13,12,11,10,255},	// unused
				{10,11,12,13,14,14,14,14,14,14,14,14,14,13,12,11,10,255},	// unused
				{10,11,12,13,14,14,14,14,14,14,14,14,14,13,12,11,10,255},	// unused
				{10,11,12,13,14,14,14,14,14,14,14,14,14,13,12,11,10,255},	// unused
				{10,11,12,13,14,14,14,14,14,14,14,14,14,13,12,11,10,255},	// unused
			}},
		{"Octentacle",255,255,0,AI_Octotentacle, //good to go
		 20,11,80,50,"mystic/graphics/octotent.jsp",0,MF_NOMOVE|MF_AQUATIC|MF_WATERWALK|MF_ENEMYWALK,
		 MT_BITS,
			{
				{1,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// attack=slap
				{6,5,4,3,2,1,0,255},	// die
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
			}},
		{"Octentacle",255,255,0,AI_Octotentacle, //good to go
		 20,11,80,50,"mystic/graphics/octotent.jsp",0,MF_NOMOVE|MF_AQUATIC|MF_WATERWALK|MF_ENEMYWALK,
		 MT_BITS,
			{
				{1,255},	// idle
				{0,255},	// move
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// attack=slap
				{6,5,4,3,2,1,0,255},	// die
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
				{1,2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2,1,255},	// unused
			}},
		{"Rammy Gruff",255,255,0,AI_Rammy, //check ramming?
		 20,52,275,300,"mystic/graphics/goat.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,12,255},	// move=charge
				{1,2,3,2,1,255},	// attack=shoot
				{32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,255},	// die
				{13,14,15,16,17,18,19,20,21,22,23,15,16,17,18,19,20,21,22,23,15,14,13,255},	// stunned
				{24,25,26,26,255},	//begin hop
				{27,28,29,29,29,29,29,29,29,30,31,32,255},	//squish onto the ground after a hop
				{0,255},	// none
				{0,255},	// unused
			}},
		{"Hoppy Gruff",255,255,-3,AI_Hoppy, //good to go
		 20,52,325,300,"mystic/graphics/goat.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,12,255},	// move=charge
				{1,2,3,2,1,255},	// attack=shoot
				{32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,255},	// die
				{13,14,15,16,17,18,19,20,21,22,23,15,16,17,18,19,20,21,22,23,15,14,13,255},	// stunned
				{24,25,26,26,255},	//begin hop
				{27,28,29,29,29,29,29,29,29,30,31,32,255},	//squish onto the ground after a hop
				{0,255},	// none
				{0,255},	// unused
			}},
		{"Gruffy Gruff",255,255,3,AI_Gruffy, //good to go
		 20,52,400,300,"mystic/graphics/goat.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,12,255},	// move=charge
				{1,2,3,2,1,255},	// attack=shoot
				{32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,255},	// die
				{13,14,15,16,17,18,19,20,21,22,23,15,16,17,18,19,20,21,22,23,15,14,13,255},	// stunned
				{24,25,26,26,255},	//begin hop
				{27,28,29,29,29,29,29,29,29,30,31,32,255},	//squish onto the ground after a hop
				{0,255},	// none
				{0,255},	// unused
			}},
		{"Happy Stick Shroom",255,255,0,AI_Stickshroom, //good to go
		 9,6,15,50,"mystic/graphics/stkshr.jsp",0,MF_ONEFACE,
		 MT_EVIL|MT_WACKY|MT_VEGGIE,
			{
				{0,1,255},	// idle
				{0,1,255},	// move=walk
				{2,2,2,2,2,2,255},	// attack=shoot
				{3,3,4,4,5,5,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Happy Stick Spider",255,255,0,AI_Stickspider, //good to go
		 9,6,25,70,"mystic/graphics/stkspd.jsp",0,MF_ONEFACE,
		 MT_EVIL|MT_WACKY|MT_SPIDER|MT_ANIMAL,
			{
				{0,1,255},	// idle
				{0,1,255},	// move=walk
				{2,2,2,2,2,2,255},	// attack=shoot
				{3,4,3,4,3,4,254,3,254,4,254,3,254,254,4,254,254,3,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Happy Stick Corpse",255,255,0,AI_Stickcorpse, //good to go
		 11,10,40,100,"mystic/graphics/stkskl.jsp",0,MF_ONEFACE,
		 MT_EVIL|MT_WACKY|MT_SPOOKY,
			{
				{0,255},	// idle
				{0,1,0,2,255},	// move=walk
				{3,3,3,3,3,3,255},	// attack=shoot
				{4,5,6,7,8,9,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Happy Stick Bat",255,255,0,AI_Stickbat, //good to go
		 11,6,20,60,"mystic/graphics/stkbat.jsp",0,MF_ONEFACE|MF_FLYING,
		 MT_EVIL|MT_WACKY|MT_FLYING|MT_SPOOKY|MT_ANIMAL,
			{
				{0,1,255},	// idle
				{0,1,255},	// move=walk
				{2,2,2,2,2,2,255},	// attack=shoot
				{3,3,4,4,5,5,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Happy Stick Dancer",7,6,8,AI_Dancer, //good to go
		 11,10,400,5000,"mystic/graphics/stkskl.jsp",0,MF_ONEFACE|MF_FREEWALK|MF_ENEMYWALK|MF_NOMOVE,
		 MT_EVIL|MT_WACKY|MT_MINIBOSS,
			{
				{0,255},	// idle
				{0,1,0,2,0,1,0,2,255},	// move=walk
				{3,3,3,3,255},	// attack=shoot
				{4,5,6,7,8,9,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Creepy Crawly",1,0,-3,AI_BigSpider,
		 8,21,10,50,"graphics/bigspdr.jsp",0,0,
		 MT_EVIL|MT_SPIDER,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{14,15,16,17,18,17,18,17,18,19,19,20,20,255},	// die
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Froggy",255,255,0,AI_Frog,
		 6,22,4,100,"sleepless/graphics/frog.jsp",0,0,
		 MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,8,0,255},	// move
				{9,10,11,12,13,14,15,16,255},	// attack
				{17,18,19,20,21,255},		// die
			}},
		{"Flying Pumpkin",2,4,0,AI_PumpkinFly,
		 11,6,5,500,"!19",0,MF_FLYING,
		 MT_EVIL|MT_PUMPKIN|MT_FLYING,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{0,0,255},	// die (just long enough to spew shrapnel)
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
			}},
		{"Dumble",1,6,0,AI_BabyThing,
		 12,44,20,50,"graphics/babything.jsp",0,0,
		 MT_EVIL|MT_THINGY,
			{
				{12,255},	// idle
				{1,2,3,4,5,6,7,8,9,10,11,255},	// move
				{20,21,22,23,24,25,26,27,255},	// attack
				{34,35,36,37,38,39,40,41,42,43,255},	// die
				{13,14,15,16,17,16,15,16,17,16,15,16,17,16,15,255},	// fall asleep (A1)
				{18,19,18,255},	// blink (A2)
				{28,29,30,29,28,255}, // look right (A3)
				{31,32,33,32,31,255}, // look left (A4)
				{0,255},
			}},
		{"Glass Jaw",0,7,0,AI_Bonehead,
		 8,32,10,50,"!2",0,0,
		 MT_EVIL|MT_SPOOKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{12,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,10,11,11,11,11,254,11,254,11,254,11,254,11,254,11,254,11,254,11,255},		// die
				{20,21,22,23,24,25,255},	// chomp/spit projectile
				{26,27,27,27,26,255},	// point at bouapha
				{11,10,9,8,7,255},	// resurrect=A3
				{28,29,30,31,30,29,28,255},	// leap skyward
			}},
		{"Toadie",1,2,0,AI_Frog,
		 6,22,8,100,"!9",0,0,
		 MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,4,5,6,7,8,0,255},	// move
				{9,10,11,12,13,14,15,16,255},	// attack
				{17,18,19,20,21,255},		// die
			}},
		{"Firebug",1,4,0,AI_BigSpider,
		 8,21,10,50,"!5",0,0,
		 MT_EVIL|MT_SPIDER,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{14,15,16,17,18,17,18,17,18,19,19,20,20,255},	// die
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{7,8,9,10,11,12,12,13,13,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Firebat",2,4,0,AI_Bat,
		 8,27,12,25,"!3",0,MF_FLYING,
		 MT_FLYING|MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,8,7,255},	// attack
				{17,18,19,20,21,22,23,24,25,26,255},		// die
				{10,11,12,12,12,12,12,11,10,255},	// diving attack
				{13,14,15,15,16,255},	// bounce off during dive
				{7,8,9,8,7,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Happy Stick Tree",255,255,0,AI_Tree,
		 20,5,100,5000,"sleepless/graphics/tree.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_WACKY|MT_MINIBOSS,
			{
				{0,255},	// idle
				{0,0,0,0,255},	// move
				{0,0,0,0,255},	// attack=snowball
				{1,2,3,4,254,4,254,4,254,4,255},	// die
				{1,1,255},	// A1=sad
			}},
		{"Funnybone",4,1,6,AI_Goodbone,
		 8,32,6,50,"sleepless/graphics/skeleton.jsp",0,0,
		 MT_GOOD|MT_SPOOKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{12,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,10,11,11,11,11,254,11,254,11,254,11,254,11,254,11,254,11,254,11,255},		// die
				{20,21,22,23,24,25,255},	// chomp/spit projectile
				{26,27,27,27,26,255},	// point at bouapha
				{7,8,9,10,11,11,11,11,254,11,254,11,254,11,254,11,254,11,254,11,254,11,255},		// drown=A3
				{11,10,9,8,7,255},	// resurrect=A4
		}},
		{"Royal Guard",2,6,-4,AI_BatGuard,
		 8,27,50,25,"!3",0,MF_FLYING|MF_NOMOVE|MF_ENEMYWALK|MF_FREEWALK|MF_WALLWALK,
		 MT_ANIMAL|MT_BITS,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,8,7,255},	// attack
				{17,18,19,20,21,22,23,24,25,26,255},		// die
				{10,11,12,12,12,12,12,11,10,255},	// diving attack
				{13,14,15,15,16,255},	// bounce off during dive
				{7,8,9,8,7,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Headless Horseradish",255,255,0,AI_Horseradish,
		 11,10,25000,100,"sleepless/graphics/radish.jsp",0,MF_NOMOVE|MF_ENEMYWALK|MF_FREEWALK,
		 MT_EVIL|MT_VEGGIE|MT_BOSS,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{5,6,7,8,9,255},	// attack=shoot spines
				{0,0,0,0,255},	// die
			}},
		{"Rolling Stone",2,3,0,AI_Roller,
		 35,11,100,1000,"graphics/roller.jsp",0,MF_ONEFACE,
		 MT_EVIL|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,255},
				{0,255},
			}},
		{"Big Head Todd",4,5,0,AI_BigHead,
		 50,8,300,1500,"graphics/lich.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_SPOOKY|MT_MINIBOSS|MT_WACKY,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,5,6,7,8,255},	// die
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Sir Spookley Hauntmore",255,255,0,AI_Spook,
		 30,9,300,1,"sleepless/graphics/bigghost.jsp",0,MF_ONEFACE|MF_FLYING|MF_FREEWALK|MF_ENEMYWALK,
		 MT_EVIL|MT_BOSS|MT_SPOOKY,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,2,1,255},	// attack
				{4,4,5,5,6,6,7,7,8,8,255},	// die
				{1,2,3,3,3,2,1,2,3,2,1,0,0,0,0,1,2,3,2,1,255},	// A1=sneeze
			}},
		{"Spookley",255,255,0,AI_Spook,
		 30,9,300,1,"!51",0,MF_ONEFACE|MF_FLYING|MF_GHOST|MF_FREEWALK|MF_ENEMYWALK|MF_NOSHADOW|MF_NOHIT,
		 MT_EVIL|MT_BOSS|MT_SPOOKY|MT_BITS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,2,1,255},	// attack
				{4,4,5,5,6,6,7,7,8,8,255},	// die
				{1,2,3,3,3,2,1,2,3,2,1,0,0,0,0,1,2,3,2,1,255},	// A1=sneeze
			}},
		{"Big Head Bod",255,255,0,AI_BigHeadBod,
		 8,32,6,50,"sleepless/graphics/bonehead.jsp",0,MF_NOHIT|MF_ENEMYWALK|MF_FREEWALK|MF_NOMOVE,
		 MT_BITS,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{12,13,14,15,16,17,18,19,255},	// attack
				{7,8,9,10,11,11,11,11,254,11,254,11,254,11,254,11,254,11,254,11,254,11,255},		// die
				{20,21,22,23,24,25,255},	// chomp/spit projectile
				{26,27,27,27,26,255},	// point at bouapha
				{11,10,9,8,7,255},	// resurrect=A3
				{28,29,30,31,30,29,28,255},	// leap skyward
			}},
		{"Spark",0,7,7,AI_Spark,
		 11,2,6,300,"!66",0,MF_GHOST|MF_ONEFACE|MF_NOHIT|MF_FREEWALK|MF_ENEMYWALK|MF_INVINCIBLE|MF_NOMOVE|MF_WALLWALK|MF_FLYING|MF_WATERWALK,
		 MT_EVIL|MT_TRAP|MT_FLYING,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,255},	// die
			}},
		{"Sparky",3,7,2,AI_Sparky,
		 12,55,100,100,"!137",0,MF_FREEWALK|MF_ENEMYWALK|MF_NOMOVE,
		 MT_EVIL|MT_MINIBOSS|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{44,45,46,47,48,49,50,51,51,51,52,53,54,254,54,254,54,254,54,255},		// die
				{7,8,9,9,8,7,255},	// spit projectile = A1
				{10,11,12,12,12,13,14,15,16,17,17,18,19,255},	// charging slash = A2
				{20,21,22,23,24,25,26,26,26,26,26,26,26,26,27,28,29,255},		// howl = A3
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
			}},
		{"Megaburner",255,255,0,AI_Burner,
		 18,2,30,300,"!66",0,MF_NOSHADOW|MF_ONEFACE|MF_NOMOVE|MF_INVINCIBLE,
		 MT_EVIL|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,255},		// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Iron Skull",4,0,-8,AI_IronSkull,
		 50,8,20000,1500,"!44",0,MF_NOMOVE|MF_ONEFACE|MF_FLYING|MF_ENEMYWALK,
		 MT_EVIL|MT_SPOOKY|MT_BOSS|MT_FLYING,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,5,6,7,8,255},	// die
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Lightswitch",255,255,0,AI_LightSwitch,
		 10,8,3000,1,"sleepless/graphics/lghtball.jsp",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_TRAP,
			{
				{7,255},	// idle
				{7,255},	// move
				{7,6,5,4,3,2,1,0,0,0,0,255},	// attack (shove)
				{0,255},	// die
			}},
		{"Unbreakable Glass",255,255,0,AI_Knight,
		 50,47,1000,1000,"graphics/knight.jsp",0,MF_GLOW|MF_ONEFACE|MF_NOMOVE,
		 MT_EVIL|MT_MINIBOSS,
			{
				{14,255},	// idle
				{15,16,17,18,19,20,21,22,14,255},	// move
				{26,27,28,29,30,31,32,33,255},	// attack
				{34,35,36,37,38,39,40,41,42,43,44,45,46,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,255}, // A1 = arrive
				{23,24,25,25,25,25,25,25,25,24,23,255}, // A2 = block
				{26,27,28,29,30,31,32,33,255},	// attack
				{26,27,28,29,30,31,32,33,255},	// attack
				{26,27,28,29,30,31,32,33,255},	// attack
			}},
		{"Mad Cow",255,255,0,AI_MadCow,
		 22,15,20,250,"graphics/cow.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,255},	// move
				{5,5,6,6,6,5,5,255},	// attack
				{7,8,9,10,11,12,13,14,255},	// die
				{5,5,6,6,6,5,5,255},	// attack
			}},
		{"Lady Von Spatula",255,255,0,AI_Spatula,
		 25,28,300,1000,"graphics/countess.jsp",0,MF_ONEFACE|MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_SPOOKY,
			{
				{0,1,2,3,4,5,6,7,8,9,255},	// idle
				{0,1,2,3,4,5,6,7,8,9,255},	// move
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
				{20,21,22,22,22,22,23,24,25,26,27,255},	// die
				{10,11,12,12,12,12,11,10,255},	// A1=summon boneheads
				{3,3,3,255},	// A2=charge up to rush
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
				{13,14,15,16,17,18,18,18,18,18,19,255},	// attack
			}},
		{"Extremely Mad Cow",2,4,0,AI_MadCow2,
		 32,15,1000,250,"!72",0,MF_NOMOVE,
		 MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,255},	// move
				{5,5,6,6,6,5,5,255},	// attack
				{7,8,9,10,11,12,13,14,255},	// die
				{5,5,6,6,6,5,5,255},	// attack
			}},
		{"Rafe Lector",255,255,0,AI_Rafe,
		 50,47,200,1000,"!71",0,MF_ONEFACE|MF_NOMOVE|MF_ENEMYWALK,
		 MT_EVIL|MT_BOSS,
			{
				{14,255},	// idle
				{15,16,17,18,19,20,21,22,14,255},	// move
				{26,27,28,29,30,31,32,33,255},	// attack
				{34,35,36,37,38,39,40,41,42,43,44,45,46,255},	// die
				{0,1,2,3,4,5,6,7,8,9,10,11,12,13,255}, // A1 = arrive
				{23,24,25,25,25,25,25,25,25,24,23,255}, // A2 = block
				{26,27,28,29,30,31,32,33,255},	// attack
				{26,27,28,29,30,31,32,33,255},	// attack
				{26,27,28,29,30,31,32,33,255},	// attack
			}},
		{"Laser",4,7,0,AI_Turret,
		 10,1,80,200,"graphics/turret.jsp",0,MF_NOMOVE,
		 MT_EVIL|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Laser",4,7,0,AI_Turret,
		 10,1,80,200,"!79",0,MF_NOMOVE,
		 MT_EVIL|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Laser",4,7,0,AI_Turret,
		 10,1,80,200,"!79",0,MF_NOMOVE,
		 MT_EVIL|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Laser",4,7,0,AI_Turret,
		 10,1,80,200,"!79",0,MF_NOMOVE,
		 MT_EVIL|MT_TRAP|MT_HIGHTECH,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,0,0,255},	// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Bobber",0,4,10,AI_Bobber,
		 18,2,30,300,"!66",0,MF_ONEFACE|MF_NOMOVE|MF_NOHIT|MF_WALLWALK|MF_ENEMYWALK|MF_FREEWALK,
		 MT_GOOD|MT_TRAP,
			{
				{0,255},	// idle
				{0,255},	// move
				{0,255},	// attack
				{0,255},		// die
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
				{0,255},	// attack
			}},
		{"Rock n' Roll",2,6,0,AI_Roller,
		 35,11,100,1000,"!43",0,MF_ONEFACE,
		 MT_EVIL|MT_TRAP,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move (roll down the screen)
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{0,0,255},	// die
				{6,7,8,9,10,0,255},	// A1=roll to the right
				{10,9,8,7,6,0,255},	// A2=roll to the left
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
				{5,4,3,2,1,0,255},	// attack=roll up the screen
			}},
		{"Reflection",255,255,0,AI_Reflection,
		 4,29,128,0,"!1",0,MF_NOMOVE|MF_INVINCIBLE|MF_WATERWALK,
		 MT_GOOD|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{0,0,0,0,0,0,0,26,27,28,255},		// die
				{23,24,25,24,23,255},	// use item
				{17,18,19,20,21,22,21,20,21,22,21,20,19,18,17,255},	// bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,26,27,28,255},	// watery death
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
			}},
		{"Your Bad Side",1,3,0,AI_EvilClone,
		 11,29,128,0,"!1",0,0,
		 MT_EVIL|MT_HUMAN|MT_WACKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{0,0,0,0,0,0,0,26,27,28,255},		// die
				{23,24,25,24,23,255},	// use item
				{17,18,19,20,21,22,21,20,21,22,21,20,19,18,17,255},	// bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,26,27,28,255},	// watery death
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},	// attack
			}},
		{"The Darkness",255,255,-7,AI_Darkness,
		 11,29,6,300,"!1",0,MF_WATERWALK|MF_FREEWALK,
		 MT_EVIL|MT_TRAP|MT_WACKY,
			{
				{254,255},	// idle
				{254,255},	// move
				{254,255},	// attack
				{254,255},	// die
			}},
		{"Scaredy Bat",2,5,0,AI_ScaredyBat,
		 8,27,20,25,"graphics/bat.jsp",0,MF_FLYING,
		 MT_FLYING|MT_ANIMAL|MT_GOOD,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,8,7,255},	// attack
				{17,18,19,20,21,22,23,24,25,26,255},		// die
				{10,11,12,12,12,12,12,11,10,255},	// diving attack
				{13,14,15,15,16,255},	// bounce off during dive
				{7,8,9,8,7,255},	// attack
				{0,255},
				{0,255},
			}},
		{"Blaster",5,4,0,AI_LightBlaster,
		 10,8,60,1,"!70",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_TRAP,
			{
				{7,255},	// idle
				{7,255},	// move
				{7,6,5,4,3,2,1,0,255},	// attack
				{0,255},	// die
			}},
		{"Deadhead",0,6,-16,AI_CrazyBone,
		 8,32,10,100,"!2",0,0,
		 MT_EVIL|MT_SPOOKY,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{20,21,22,23,24,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},		// die
				{25,26,27,28,29,30,255},	// chomp/spit projectile
				{31,32,32,32,31,255},	// point at bouapha
				{16,15,14,13,12,11,10,9,8,7,255},	// resurrect=A3
				{7,8,9,9,8,8,7,255},	// skyward leap
				{7,8,9,9,9,9,9,9,9,8,8,8,7,255},	// attack
			}},
		{"Speedy Hernandez",3,5,0,AI_Speedy,
		 12,55,250,100,"graphics/wolfman.jsp",0,MF_FREEWALK|MF_ENEMYWALK|MF_NOMOVE,
		 MT_EVIL|MT_BOSS|MT_ANIMAL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{44,45,46,47,48,49,50,51,51,51,52,53,54,254,54,254,54,254,54,255},		// die
				{7,8,9,9,8,7,255},	// spit projectile = A1
				{10,11,12,12,12,13,14,15,16,17,17,18,19,255},	// charging slash = A2
				{20,21,22,23,24,25,26,26,26,26,26,26,26,26,27,28,29,255},		// howl = A3
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
				{30,31,32,32,32,33,34,35,36,37,38,39,40,41,42,43,255},	// attack (pounce)
			}},
		{"The Grue",6,0,-13,AI_Grue,
		 30,30,500,1500,"graphics/mamaspdr.jsp",0,MF_NOMOVE|MF_ENEMYWALK,
		 MT_EVIL|MT_BOSS|MT_SPIDER,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,13,13,255},	// spit (attack)
				{19,20,21,22,23,24,25,25,25,26,27,28,29,255},	// die
				{14,15,16,17,18,255},	// bite (A1)
				{7,8,9,10,11,12,12,13,13,255},	// spit (attack)
				{7,8,9,10,11,12,12,13,13,255},	// spit (attack)
				{0,255},
				{0,255},
			}},
		{"Big Head Rod",255,255,0,AI_BigHead2,
		 50,8,300,1500,"!44",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_SPOOKY|MT_MINIBOSS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,5,6,7,8,255},	// die
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Big Head Maude",4,1,0,AI_BigHead,
		 50,8,300,1500,"!44",0,MF_NOMOVE|MF_ONEFACE,
		 MT_EVIL|MT_SPOOKY|MT_MINIBOSS,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,5,6,7,8,255},	// die
				{1,2,3,4,4,3,2,1,255},	// attack=open mouth
				{0,255},
				{0,255},
				{0,255},
				{0,255},
			}},
		{"Charge Coil",5,6,3,AI_LightSlide,
		 10,8,3000,1,"!70",0,MF_NOMOVE|MF_ONEFACE|MF_INVINCIBLE,
		 MT_TRAP|MT_GOOD,
			{
				{5,255},	// idle
				{5,255},	// move
				{5,4,3,2,1,0,0,0,0,0,0,0,255},	// attack (charge up)
				{0,255},	// die
			}},
		{"Marsh Marrow",0,1,-4,AI_Bonehead,
		 8,32,40,50,"!2",0,0,
		 MT_SPOOKY|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{20,21,22,23,24,255},	// attack
				{7,8,9,10,11,12,13,14,15,16,255},		// die
				{25,26,27,28,29,30,255},	// chomp/spit projectile
				{31,32,32,32,31,255},	// point at bouapha
				{16,15,14,13,12,11,10,9,8,7,255},	// resurrect=A3
				{7,8,9,9,8,8,7,255},	// skyward leap
				{7,8,9,9,9,9,9,9,9,8,8,8,7,255},	// attack
			}},
		{"Beetle Nest",1,5,6,AI_Patch,
		 16,17,100,1000,"graphics/patch.jsp",0,MF_NOMOVE|MF_ENEMYWALK|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_TRAP|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
			}},
		{"Simulated Head",255,255,0,AI_PumpkinFly,
		 11,6,25,500,"!19",0,MF_FLYING,
		 MT_EVIL|MT_PUMPKIN|MT_BITS,
			{
				{0,255},	// idle
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{0,0,255},	// die (just long enough to spew shrapnel)
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
				{1,2,3,4,5,0,255},	// move
			}},
		{"Toad Hole",1,2,6,AI_Patch,
		 16,17,50,1000,"!162",0,MF_NOMOVE|MF_ENEMYWALK|MF_ONEFACE|MF_NOSHADOW,
		 MT_EVIL|MT_TRAP|MT_GENERATE,
			{
				{0,255},	// idle
				{0,255},	// move
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{11,12,13,14,15,16,255},	// die
				{3,4,5,6,7,8,9,10,255},	// A1=summon
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
				{1,2,0,2,0,1,2,1,0,1,0,2,1,2,1,0,2,1,2,255},	// attack
			}},
		// Sleepless hollow ends here
		{"Third Eye",5,6,-3,AI_Eyeguy,
		 14,17,20,75,"mystic/graphics/eyeguy.jsp",0,0,
		 MT_EVIL|MT_THINGY,
			{
				{0,0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,255},	// attack
				{13,14,15,16,255},	// die
				{7,8,9,10,11,12,255},	// a1
				{7,8,9,10,11,12,255},	// a2
				{7,8,9,10,11,12,255},	// a3
				{7,8,9,10,11,12,255},	// a4
				{7,8,9,10,11,12,255},	// a5
			}},
		{"Burning Bush",1,4,4,AI_Crazybush, //good to go
		 10,8,15,5,"mystic/graphics/crazybush.jsp",0,0,
		 MT_EVIL|MT_WACKY|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// move
				{0,255},	// attack
				{5,6,7,255},	// die
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
				{0,255},	// unused
			}},
		{"Calamari",6,4,2,AI_Octopus, //good to go
		 19,21,15,40,"!222",0,MF_AQUATIC|MF_WATERWALK,
		 MT_EVIL|MT_ZOID|MT_AQUATIC,
			{
				{5,6,7,6,255},	// idle
				{5,6,6,7,6,6,255},	// move
				{16,17,18,19,20,19,18,17,18,19,20,19,18,17,18,19,20,19,18,17,16,255},	// attack=spit from arisen postion
				{8,9,10,11,12,13,14,15,255},	// die
				{5,4,3,2,1,0,0,255},	// A1=arise
				{1,2,3,4,5,5,255},	// A2=descend
				{16,17,18,19,20,19,18,17,16,255},	// unused
				{16,17,18,19,20,19,18,17,16,255},	// unused
				{16,17,18,19,20,19,18,17,16,255},	// unused
			}},
		{"Banana Slug",7,5,0,AI_Slug, //good to go
		 12,19,60,50,"!235",0,0,
		 MT_EVIL|MT_ANIMAL,
			{
				{0,255},	// idle
				{0,1,2,3,4,3,2,1,255},	// move
				{1,2,3,255},	// attack=unused
				{5,6,7,8,9,10,11,12,13,14,15,16,17,18,255},	// die
				{4,2,3,255},	// unused
				{1,2,3,255},	// unused
				{1,2,3,255},	// unused
				{1,2,3,255},	// unused
				{1,2,3,255},	// unused
			}},
		{"Mad Golem",255,255,-4,AI_Golem, //good to go
		 32,26,100,0,"!219",0,MF_NOMOVE|MF_FREEWALK,
		 MT_EVIL,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// attack
				{15,16,17,18,19,20,21,21,21,254,21,254,21,254,21,255},	// die
				{22,23,24,25,255},	// form out of nothing (A1)
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
				{7,8,9,10,11,12,12,12,13,13,14,14,255},	// unused
			}},
		{"Danger Bunny",0,4,-4,AI_PunkBunny2,
		 8,16,40,100,"!204",0,0,
		 MT_ANIMAL|MT_URBAN|MT_EVIL,
			{
				{0,255},	// idle
				{1,2,1,0,3,4,3,0,255},	// walk
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{0,0,254,254,255},	// die
				{11,12,13,14,15,255},	// A1 = bomb throw
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
				{5,6,7,8,9,7,8,9,7,8,9,10,7,6,5,255},	// attack (machine gun)
			}},
		{"Evil Clone",1,5,-4,AI_EvilClone2,
		 11,14,128,0,"!192",0,0,
		 MT_EVIL|MT_VEGGIE,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,7,8,8,9,10,11,12,12,13,255},	// attack
				{0,0,0,0,0,0,0,0,0,0,255},	// die
				{7,8,10,11,13,255},	// use item
				{7,8,9,9,9,9,9,9,8,7,0,0,0,0,0,255},	// bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,0,0,0,255},	// watery death
				{7,7,8,8,9,10,11,12,12,13,255},	// attack
				{7,7,8,8,9,10,11,12,12,13,255},	// attack
			}},
		{"Evil Clone",6,4,-4,AI_EvilClone2,
		 40,21,128,0,"!57",0,0,
		 MT_EVIL|MT_HUMAN,
			{
				{0,255},	// idle
				{7,8,9,8,7,0,10,11,12,11,10,0,255},	// move
				{1,2,3,4,5,6,5,4,5,6,5,6,5,4,3,4,3,4,5,4,3,2,1,255},	// attack=laugh
				{13,14,15,16,17,17,17,17,17,17,17,18,19,20,20,20,20,255},	// die
				// these following are only used when you play as Dr. L
				{1,2,3,2,1,255},	// use item
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255},	// bored
				{7,9,7,0,10,12,10,0,7,9,7,0,0,0,0,0,0,255},	// watery death
				{1,2,3,4,5,6,5,4,5,6,5,6,5,4,3,4,3,4,5,4,3,2,1,255},	// attack=laugh
			}},
		{"Evil Clone",1,255,-4,AI_EvilClone2,	// playable char
		 11,27,128,0,"!205",0,0,
		 MT_EVIL|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,11,12,13,13,0,0,255},	// attack=fire gun
				{14,15,16,17,18,19,20,21,22,23,24,25,26,255},	// die
				{7,9,10,12,13,255},	// A1=use weapon
				{14,15,16,17,18,17,16,15,14,15,17,15,14,0,0,255},	// A2=bored
				{1,3,1,0,4,6,4,0,1,3,1,0,0,0,24,25,26,255},	// A3=watery death
				{7,8,9,10,11,12,13,13,0,0,255},	// attack=fire gun
				{7,8,9,10,11,12,13,13,0,0,255},	// attack=fire gun
			}},
		{"Evil Clone",3,4,0,AI_EvilClone2,
		 11,25,128,0,"!211",0,0,
		 MT_EVIL|MT_HUMAN,
			{
				{0,255},	// idle
				{1,2,3,2,1,0,4,5,6,5,4,0,255},	// move
				{7,8,9,10,10,11,11,12,12,255},	// attack
				{15,16,17,18,19,20,21,22,23,24,255},		// die
				{13,13,14,14,13,13,255},	// cast non-pointy spell
				{7,8,9,10,11,12,12,255},	// cast pointy spell
				{1,3,1,0,4,6,4,0,1,3,1,0,0,254,254,254,254,255},	// watery death
				{24,23,22,21,20,19,18,17,16,15,255},		// A4=undie
				{0,255},
			}},
	};
