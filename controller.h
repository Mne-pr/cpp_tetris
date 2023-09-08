#pragma once

#include "planeTable.h"
#include <sstream>
#include <chrono>
#include <conio.h>
#include <thread>

#ifndef LEFT
#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'
#define SPACE 32
#endif

// 컨트롤러 클래스
class controller {
public:
	int standby;
	int clearCount;
	planeTable table;
	note* n;
	note* next_n;
	std::mt19937 gen;
	rendering render;
	double runSpeed;
	double last_runSpeed;

public:
	controller();

	void makeNote();

	void makeNoteN();

	void running(int);

	void statPrint();

	void endPrint(int);

	void RunGame();

};
