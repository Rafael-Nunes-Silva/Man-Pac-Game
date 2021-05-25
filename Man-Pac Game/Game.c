#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "Map.h"

Map map;
int mapID = -1;

Position player;
int playerBombs = 0;

Position enemies[10];
int aliveEnemies = 0;
Position posAround[4] = {
						{0, -1},
				{-1, 0},		{1, 0},
						{0,  1},
};
Position usablePos[4] = {
	{0,0}, {0,0}, {0,0}, {0,0}
};
Position usedPos[10];
int F[4];

int main() {
	ConfigureGame();

	InitGame();

	do {
		GetInput();
		
		system("cls");
		PrintTitle();

		DrawMap(&map);
		printf("%c - Player | %c - Enemy | %c - Bomb | Bombs: %i\n", PlayerSymbol, EnemySymbol, BombSymbol, playerBombs);
		printf("Controls:\n");
		printf(" W \nASD\nMovement\nB Bomb\n");
	} while (!ShouldFinish());

	EndGame();

	Restart();

	return 0;
}

void PrintTitle() {
	printf("**********************************\n");
	printf("*          Man-Pac Game          *\n");
	printf("**********************************\n");
}
void InitGame() {
	system("cls");

	PrintTitle();

	LoadMap(&map, mapID);
	DrawMap(&map);

	FindInMap(&map, &player, PlayerSymbol, 0);

	SetupEnemiesLogic();
}
void ConfigureGame() {
	printf("Insira o numero do mapa que quer jogar (-1 mapa teste): ");
	scanf("%i", &mapID);
}
void Restart() {
	printf("Wanna play again? (Y) (N)\n");
	char restart = ' ';
	scanf(" %c", &restart);
	restart = toupper(restart);
	if (toupper(restart) == 'Y') {
		system("cls");
		main();
	}
}

void GetInput() {
	char input;
	scanf(" %c", &input);

	int playerMoved = 0;
	switch (input) {
	case MoveUP:
		playerMoved=1;
		Move(UP, &player, PlayerSymbol);
		break;
	case MoveDOWN:
		playerMoved=1;
		Move(DOWN, &player, PlayerSymbol);
		break;
	case MoveRIGHT:
		playerMoved=1;
		Move(RIGHT, &player, PlayerSymbol);
		break;
	case MoveLEFT:
		playerMoved=1;
		Move(LEFT, &player, PlayerSymbol);
		break;
	case Bomb:
		if (playerBombs > 0) {
			UseBomb(player);
			playerBombs--;
		}
		break;
	default:
		return;
		break;
	}

	if (playerMoved) {
		EnemiesLogic();
	}
}
void Move(MoveDir dir, Position* character, char symbol) {
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
	if (!IsWalkable(&map, nextPos, symbol)) return;

	if (map.matrix[character->y][character->x] == PlayerSymbol && map.matrix[nextPos.y][nextPos.x] == BombSymbol) {
		map.matrix[nextPos.y][nextPos.x] = WalkableSymbol;
		playerBombs++;
	}
	else if (map.matrix[character->y][character->x] == EnemySymbol && map.matrix[nextPos.y][nextPos.x] == PlayerSymbol) {
		map.matrix[nextPos.y][nextPos.x] = WalkableSymbol;
	}

	map.matrix[character->y][character->x] = WalkableSymbol;
	map.matrix[nextPos.y][nextPos.x] = (character == &player) ? PlayerSymbol : EnemySymbol;
	*character = nextPos;
}
void SetupEnemiesLogic() {
	aliveEnemies = CheckEnemies(&map);

	for (int i = 0; i < aliveEnemies; i++) {
		GetEnemy(&map, &enemies[i], i);
		usedPos[i].x = -1;
		usedPos[i].y = -1;
	}
}
void EnemiesLogic() {
	Position betterPos;
	betterPos.x = 0;
	betterPos.y = 0;

	for (int e = 0; e < aliveEnemies; e++) {
		for (int i = 0; i < 4; i++) {
			Position pos;
			pos.x = enemies[e].x + posAround[i].x;
			pos.y = enemies[e].y + posAround[i].y;

			if (!InsideMap(&map, pos) || !IsWalkable(&map, pos, EnemySymbol)
				|| (pos.x == usedPos[e].x && pos.y == usedPos[e].y)) {
				F[i] = -1;
				if (i == aliveEnemies)
					SetupEnemiesLogic();
				continue;
			}

			usablePos[i] = pos;
			F[i] = ((pos.x - player.x) < 0) ? -(pos.x - player.x) : (pos.x - player.x);
			F[i] += ((pos.y - player.y) < 0) ? -(pos.y - player.y) : (pos.y - player.y);
		}

		int lowerF = 1000;
		for (int i = 0; i < 4; i++) {
			if (F[i] >= 0 && F[i] < lowerF) {
				lowerF = F[i];
				betterPos = usablePos[i];
			}
		}

		usedPos[e] = enemies[e];

		if		(betterPos.y < enemies[e].y)		Move(UP, &enemies[e], EnemySymbol);
		else if (betterPos.y > enemies[e].y)		Move(DOWN, &enemies[e], EnemySymbol);
		else {
			if		(betterPos.x > enemies[e].x)	Move(RIGHT, &enemies[e], EnemySymbol);
			else if (betterPos.x < enemies[e].x)	Move(LEFT, &enemies[e], EnemySymbol);
		}
	}
}
void UseBomb(Position pos)
{
	Position bombAreaUp[2]	  = { {0, -1}, {0, -2} };
	Position bombAreaDown[2]  = { {0,  1}, {0,  2} };
	Position bombAreaRight[2] = { {1,  0}, {2,  0} };
	Position bombAreaLeft[2]  = { {-1, 0}, {-2, 0} };
	/*
	Position bombArea[8] = {
		{0, -1}, {0, -2},
		{0,  1}, {0,  2},
		{-1, 0}, {-2, 0},
		{1,  0}, {2,  0},
	};
	*/

	Explode(pos, &bombAreaUp);
	Explode(pos, &bombAreaDown);
	Explode(pos, &bombAreaRight);
	Explode(pos, &bombAreaLeft);

	/*
	while (1) {
		Position p;
		p.x = pos.x + bombArea[0].x;
		p.y = pos.y + bombArea[0].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		else break;

		p.x = pos.x + bombArea[1].x;
		p.y = pos.y + bombArea[1].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		break;
	}
	while (1) {
		Position p;
		p.x = pos.x + bombArea[2].x;
		p.y = pos.y + bombArea[2].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		else break;

		p.x = pos.x + bombArea[3].x;
		p.y = pos.y + bombArea[3].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		break;
	}
	while (1) {
		Position p;
		p.x = pos.x + bombArea[4].x;
		p.y = pos.y + bombArea[4].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		else break;

		p.x = pos.x + bombArea[5].x;
		p.y = pos.y + bombArea[5].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		break;
	}
	while (1) {
		Position p;
		p.x = pos.x + bombArea[6].x;
		p.y = pos.y + bombArea[6].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		else break;

		p.x = pos.x + bombArea[7].x;
		p.y = pos.y + bombArea[7].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		break;
	}
	*/
	/*
	for (int i = 0; i < 8; i++) {
		Position p;
		p.x = pos.x + bombArea[i].x;
		p.y = pos.y + bombArea[i].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol) {
				map.matrix[p.y][p.x] = WalkableSymbol;
				printf("Killed Enemy!\n");
			}
		}
	}
	*/
	SetupEnemiesLogic();
}
void Explode(Position pos, Position* area) {
	while (1) {
		Position p;
		p.x = pos.x + area[0].x;
		p.y = pos.y + area[0].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		else break;

		p.x = pos.x + area[1].x;
		p.y = pos.y + area[1].y;

		if (InsideMap(&map, p) && IsWalkable(&map, p, PlayerSymbol)) {
			if (map.matrix[p.y][p.x] == EnemySymbol)
				map.matrix[p.y][p.x] = WalkableSymbol;
		}
		break;
	}
}

int ShouldFinish() {
	if (!FindInMap(&map, &player, PlayerSymbol, 0)) {
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