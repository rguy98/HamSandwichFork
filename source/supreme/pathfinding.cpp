#include "winpch.h"
#include "guy.h"
#include "player.h"
#include "editor.h"
#include "config.h"
#include "pathfinding.h"
#include "map.h"
#include "game.h"
#include <vector>
#include <unordered_set>

/*
	typedef struct node_t{
	int x,y;
	int gcost, // distance from starting node
		hcost, // distance from end node - heuristic
		fcost // g cost + h cost
	} node_t;
*/

void SetNode(node_t& node, int x, int y)
{
	node.x = x;
	node.y = y;
}

std::vector<node_t> GetNeighbors(node_t node, Map *map)
{
	std::vector<node_t> neighbors;
	for(int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			if(x == 0 && y == 0)
				continue;

			int chx = node.x + x,
				chy = node.y + y;

			if(chx < 0 || chx >= map->width || chy < 0 || chy >= map->height)
				continue;

			node_t n;
			SetNode(n,x,y);

			neighbors.push_back(n);
		}
	}
	return neighbors;
}

// Finds and removes nodes in vectors
void FindAndRemove(std::vector<node_t> &v, node_t node){

}

void AStar(Guy* g, Guy* t, Map* map)
{
	/*
	if(!t)
		return;

	byte ending=0;
	std::vector<node_t> openSet; // to be evaluated
	std::vector<node_t> closedSet; // already evaluated

	// Create nodes for ME and TARGET
	node_t start, end;
	SetNode(start,g->x,g->y);
	SetNode(end,t->x,t->y);

	openSet.push_back(start);

	while (openSet.size()>0 && ending < 2) // continue as long as working
	{
		int c;
		node_t current = openSet.front(); // get first from open set vector
		for (int i = 1; i < openSet.size(); i++)
		{ // TODO: optimize!!
			if(openSet.at(i).fcost < current.fcost || openSet.at(i).fcost == current.fcost && openSet.at(i).hcost < current.hcost) // compare fcosts
			{
				current = openSet.at(i);
				c = i;
			}
		}
		FindAndRemove(openSet,current);
		ending++; // just here to avoid forever loop!
	}
	*/
}
