#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "winpch.h"
#include "guy.h"
#include "player.h"
#include "editor.h"
#include "config.h"
#include "pathfinding.h"
#include "map.h"
#include "game.h"

typedef struct node_t{
	int x,y;
	int g,h,f;
} node_t;

#endif
