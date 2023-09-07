#pragma once

#include "planeTable.h"

// 컨트롤러 클래스
class controller {
public:
	int standby;
	planeTable table;
	note* n;
	note* next_n;
	std::mt19937 gen;
	rendering render;

public:
	controller();

	void makeNote();

	void makeNoteN();

	void running(int);

	void statPrint();

	void endPrint();

	void RunGame();

};
