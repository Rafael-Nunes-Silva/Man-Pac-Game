#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "Map.h"

Map map;
int mapID = -1;

Character player;

int main() {
	ConfigureGame();

	InitGame();

	do {
		printf("%c - Player | %c - Enemy | %c - Bomb | Bombs: %i\n", PlayerSymbol, EnemySymbol, BombSymbol, player.bombs);
		printf("Controls:\n");
		printf(" W \nASD\nMovement\nB Bomb\n");

		GetInput();

		EnemiesLogic();

		system("cls");
		PrintTitle();

		DrawMap(&map);
	} while (!ShouldFinish());

	EndGame();

	return 0;
}

void PrintTitle() {
	printf("**********************************\n");
	printf("*          Man-Pac Game          *\n");
	printf("**********************************\n");
}
void InitGame() {
	PrintTitle();

	LoadMap(&map, mapID);
	DrawMap(&map);

	int pows = 0, enems = 0;
	for (int i = 0; i < map.lines; i++) {
		for (int j = 0; j < map.columns; j++) {
			if (map.matrix[i][j] == PlayerSymbol) {
				player.pos.x = j;
				player.pos.y = i;
				player.alive = 1;
			}
		}
	}
}
void ConfigureGame() {
	printf("Insira o numero do mapa que quer jogar (-1 mapa teste): ");
	scanf("%i", &mapID);
}

void GetInput() {
	char input;
	scanf(" %c", &input);

	if (input == Bomb && player.bombs > 0) {
		UseBomb(player.pos);
		player.bombs--;
	}

	if		(input == MoveUP)		Move(UP, &player);
	else if (input == MoveDOWN)		Move(DOWN, &player);
	if		(input == MoveLEFT)		Move(LEFT, &player);
	else if (input == MoveRIGHT)	Move(RIGHT, &player);
}
void Move(MoveDir dir, Position* character) {
	Position nextPos = *character;

	switch (dir) {
	case UP:
		nextPos.y--;
		break;
	case DOWN:
		nextPos.y++;
		break;
	case LEFT:
		nextPos.x--;
		break;
	case RIGHT:
		nextPos.x++;
		break;
	default:
		return;
		break;
	}

	if (!InsideMap(&map, nextPos)) return;
	if (!IsWalkable(&map, nextPos)) return;

	if (map.matrix[character->y][character->x] == PlayerSymbol && map.matrix[nextPos.y][nextPos.x] == BombSymbol) {
		map.matrix[nextPos.y][nextPos.x] = WalkableSymbol;
		player.bombs++;
	}
	else if (map.matrix[character->y][character->x] == EnemySymbol && map.matrix[nextPos.y][nextPos.x] == PlayerSymbol) {
		map.matrix[nextPos.y][nextPos.x] = WalkableSymbol;
		player.alive = 0;
	}

	map.matrix[character->y][character->x] = WalkableSymbol;
	map.matrix[nextPos.y][nextPos.x] = (character == &player.pos) ? PlayerSymbol : EnemySymbol;
	*character = nextPos;
}
void EnemiesLogic() {
	Position playerDir;
	Position temPos;
	Position enemPos;
	int aliveEnemies = CheckEnemies(&map);

	for (int i = 0; i < map.lines; i++) {
		for (int j = 0; j < map.columns; j++) {
			if (aliveEnemies > 0 && map.matrix[i][j] == EnemySymbol) {
				enemPos.x = j;
				enemPos.y = i;

				playerDir.x = (player.pos.x < enemPos.x) ? -1 : 1;
				playerDir.y = (player.pos.y < enemPos.y) ? -1 : 1;

				temPos.x = enemPos.x + playerDir.x;
				temPos.y = enemPos.y + playerDir.y;

				int moveOrder = rand() % 2;

				if (moveOrder == 0) {
					if (playerDir.x > 0) Move(RIGHT, &enemPos);
					else if (playerDir.x < 0) Move(LEFT, &enemPos);
				}
				else {
					if (playerDir.y > 0) Move(DOWN, &enemPos);
					else if (playerDir.y < 0) Move(UP, &enemPos);
				}

				aliveEnemies--;
			}
		}
	}
}
void UseBomb(Position pos) {
	Position p;
	p.x = pos.x + 1;
	p.y = pos.y + 0;
	if (InsideMap(&map, p) && map.matrix[p.y][p.x] == EnemySymbol)
		map.matrix[p.y][p.x] = WalkableSymbol;

	p.x = pos.x + 2;
	p.y = pos.y + 0;
	if (InsideMap(&map, p) && map.matrix[pos.y][pos.x] == EnemySymbol)
		map.matrix[pos.y][pos.x] = WalkableSymbol;

	p.x = pos.x - 1;
	p.y = pos.y + 0;
	if (InsideMap(&map, p) && map.matrix[p.y][p.x] == EnemySymbol)
		map.matrix[p.y][p.x] = WalkableSymbol;

	p.x = pos.x - 2;
	p.y = pos.y + 0;
	if (InsideMap(&map, p) && map.matrix[p.y][p.x ] == EnemySymbol) 
		map.matrix[p.y][p.x] = WalkableSymbol;

	p.x = pos.x + 0;
	p.y = pos.y + 1;
	if (InsideMap(&map, p) && map.matrix[p.y][p.x] == EnemySymbol) 
		map.matrix[p.y][p.x] = WalkableSymbol;

	p.x = pos.x + 0;
	p.y = pos.y + 2;
	if (InsideMap(&map, p) && map.matrix[p.y][p.x] == EnemySymbol) 
		map.matrix[p.y][p.x] = WalkableSymbol;

	p.x = pos.x + 0;
	p.y = pos.y - 1;
	if (InsideMap(&map, p) && map.matrix[p.y][p.x] == EnemySymbol) 
		map.matrix[p.y][p.x] = WalkableSymbol;

	p.x = pos.x + 0;
	p.y = pos.y - 2;
	if (InsideMap(&map, p) && map.matrix[p.y][p.x] == EnemySymbol) 
		map.matrix[p.y][p.x] = WalkableSymbol;
}

int ShouldFinish() {
	if (!player.alive) {
		printf("*****************************\n");
		printf("*         You Died!         *\n");
		printf("*****************************\n");
		return 1;
	}

	if (CheckEnemies(&map) <= 0) {
		printf("*****************************\n");
		printf("*          You Won!         *\n");
		printf("*****************************\n");
		return 1;
	}

	return 0;
}
void EndGame() {
	// free memory from map
	DeleteMap(&map);
}