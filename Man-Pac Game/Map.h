#ifndef MAP_H
#define MAP_H
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
typedef struct MapStruct {
	char** matrix;
	int lines, columns;
};
typedef struct MapStruct Map;

int IsWalkable(Map* map, Position pos);
int InsideMap(Map* map, Position pos);
int CheckEnemies(Map* map);

void LoadMap(Map* map, int id);
void DrawMap(Map* map);
void DeleteMap(Map* map);
#endif