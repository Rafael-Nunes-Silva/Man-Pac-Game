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

int IsWalkable(Map* map, Position pos, char character);
int InsideMap(Map* map, Position pos);
int CheckEnemies(Map* map);

int FindInMap(Map* map, Position* character, char c, int index);
void GetEnemy(Map* map, Position* enemies, int index);

void LoadMap(Map* map, int id);
void DrawMap(Map* map);
void DeleteMap(Map* map);
#endif