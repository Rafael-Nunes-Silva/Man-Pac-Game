#include "Map.h"
int IsWalkable(Map* map, Position pos) {
	if (map->matrix[pos.y][pos.x] == WalkableSymbol)
		return 1;

	if (map->matrix[pos.y][pos.x] == BombSymbol)
		return 1;

	if (map->matrix[pos.y][pos.x] == PlayerSymbol)
		return 1;

	return 0;
}
int InsideMap(Map* map, Position pos) {
	if (pos.x <= 0 || pos.y <= 0)
		return 0;

	if (pos.x >= map->columns || pos.y >= map->lines)
		return 0;

	return 1;
}
int CheckEnemies(Map* map) {
	int enemies = 0;
	for (int i = 0; i < map->lines; i++) {
		for (int j = 0; j < map->columns; j++) {
			if (map->matrix[i][j] == EnemySymbol)
				enemies++;
		}
	}
	return enemies;
}

void LoadMap(Map* map, int mapid) {
	char mapPath[30] = "Resources/Maps/testMap.txt";
	if (mapid >= 0) sprintf(mapPath, "Resources/Maps/Map_%i.txt", mapid);

	FILE* file = fopen(mapPath, "r");	// open map file
	// read the ammount of lines and columns from file
	fscanf(file, "%i %i", &map->lines, &map->columns);
	// allocate memory for the map matrix
	map->matrix = malloc(sizeof(char*) * (map->lines + 1));
	for (int i = 0; i < map->lines; i++)
		map->matrix[i] = malloc(sizeof(char) * (map->columns + 1));
	// read the map from file
	for (int i = 0; i < map->lines; i++)
		fscanf(file, "%s", map->matrix[i]);
	fclose(file);	// close map file
}
void DrawMap(Map* map) {
	for (int i = 0; i < map->lines; i++) {
		for (int j = 0; j < map->columns; j++) {
			// draw the map on console
			switch (map->matrix[i][j])
			{
			case WalkableSymbol:	// draw a blank space if ","(symbol for white space in the map)
				printf(" ");
				break;
			default:	// draw the symbol found in the map matrix
				printf("%c", map->matrix[i][j]);
				break;
			}
		}
		printf("\n");
	}
}
void DeleteMap(Map* map) {
	// free memory from the map matrix
	for (int i = 0; i < map->lines; i++)
		free(map->matrix[i]);
	free(map->matrix);
}