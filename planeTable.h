#pragma once

#include "notes.h"
#include "rendering.h"
#include <sstream>

#ifndef PLANEW
#define PLANEW 8
#define PLANEH 20
#endif

#ifndef LEFT
#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'
#define SPACE 32
#endif

// 판 클래스
class planeTable {
public:
	int movTable[PLANEH][PLANEW];
	int stockTable[PLANEH][PLANEW];
	rendering* render;
	int score;
	int gameover;

	planeTable();
	planeTable(rendering*);
	~planeTable();

	void init();

	void clearMovTable();

	void stockNote(note*);

	void movNote(note*);

	int checkLine();

	int setMovTable(note*);

	void tablePrint();

	void scorePrint();

	void nextPrint(note*);
};
