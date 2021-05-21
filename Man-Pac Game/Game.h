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

struct CharacterStruct {
	Position pos;
	int alive;
	int bombs;
};
typedef struct CharacterStruct Character;

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

void GetInput();
void Move(MoveDir dir, Position* character);
void EnemiesLogic();
void UseBomb(Position pos);

int ShouldFinish();
void EndGame();
#endif