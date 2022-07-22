#ifndef COSSIN_H
#define COSSIN_H
//hello
#include "jamultypes.h"
#define MATHPI 3.14159265358979323846

void InitCosSin(void);
int Cosine(int angle);
int Sine(int angle);
void Dampen(int* value, int amt);
void Clamp(int* value, int amt);
void RevClamp(int* value, int amt);
int CirclePi(int i);

byte AngleFrom(int fromx, int fromy, int tox, int toy);
byte TurnToward(byte faceNow, byte newFace, byte spd);
int Distance(int fromx, int fromy, int tox, int toy);

#endif
