#ifndef MONSNOTES_H
#define MONSNOTES_H

#include "mgldraw.h"

typedef struct monsnotes_t {
	dword monster;
	const char* notes;
} monsnotes_t;

const char *MonsterNotes(dword type);

#endif
