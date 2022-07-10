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
	int gcost, // distance from starting node
		hcost, // distance from end node
		fcost; // g cost + h cost
} node_t;

void AStar(Guy* me, Guy* target, Map* map);

#endif
