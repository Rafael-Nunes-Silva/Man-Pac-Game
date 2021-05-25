#ifndef GAME_H
#define GAME_H

#define MoveUP		'w'
#define MoveDOWN	's'
#define MoveLEFT	'a'
#define MoveRIGHT	'd'
#define Bomb		'b'

#define WalkableSymbol  ','
#define PlayerSymbol	'P'
#define EnemySymbol		'&'
#define BombSymbol		'@'

struct PositionStruct {
	int x, y;
};
typedef struct PositionStruct Position;

enum MoveDirEnum {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
typedef enum MoveDirEnum MoveDir;

void PrintTitle();
void InitGame();
void ConfigureGame();
void Restart();

void GetInput();
void Move(MoveDir dir, Position* character, char symbol);
void SetupEnemiesLogic();
void EnemiesLogic();
void UseBomb(Position pos);
void Explode(Position pos, Position* area);

int ShouldFinish();
void EndGame();
#endif