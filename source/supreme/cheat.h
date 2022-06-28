#ifndef CHEAT_H
#define CHEAT_H

#include "sound.h"
#include "display.h"
#include "player.h"
#include "message.h"
#include "bullet.h"

#define CHEAT_WINLEVEL	0	// Wins the level
#define CHEAT_HAMMERUP	1	// spring+reverse+squeezy
#define CHEAT_LIFE		2	// Max health
#define CHEAT_SHIELD	3	// Energy shield
#define CHEAT_BOOM		4	// Explode screen
#define CHEAT_FREEZE	5	// Stops time
#define CHEAT_BRAINS	6	// All brains
#define CHEAT_KEYS		7	// Fill key inventory
#define CHEAT_SCANNER	8	// Free scanner
#define CHEAT_AMMO		9	// Unlimited ammo
#define CHEAT_LIGHT		10	// Removes light/dark
#define CHEAT_WATER		11	// Walking on water!!
#define CHEAT_OXYGEN	12	// Unlimited breathing
#define CHEAT_NOSKID	13	// No skidding on ice
#define CHEAT_SPEED		14	// Unlimited super speed
#define CHEAT_RAGE		15	// Fills rage gauge
#define CHEAT_STRENGTH	16	// Gives super strength
#define CHEAT_QUICKFIX	17	// Removes negative afflictions
#define CHEAT_PRESTO	18	// Randomizes character
#define CHEAT_AURA		19	// Hurts enemies within short range
#define CHEAT_RADARB	20	// Consistent brain radar
#define CHEAT_RADARC	21	// Consistent candle radar

#define NUM_CHEATS		22
#define IGF				1

void InitCheater(void);
void CheatKey(char c);
void DoCheat(byte w);
char *CheatName(byte w);

#endif
