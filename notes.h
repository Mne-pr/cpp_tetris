#pragma once

#include <random>

#ifndef LEFT
#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'
#define SPACE 32
#endif

#ifndef PLANEW
#define PLANEW 8
#define PLANEH 20
#endif

class note {
public:
	int ltx, lty;
	int** shape;
	int sW, sH;
	int command;

	note(int, int);

	void setShapeWH(int, int);

	void setLocation(int, int);
	void setLocation(std::mt19937&);

	void go();
	void ungo();

	void rotate();

	void printTable();

};

// �� ��Ʈ
class fuNote : public note {
public:
	fuNote(int);
};

// �� ��Ʈ
class nemoNote : public note {
public:
	nemoNote();
	void rotate();
};

// ���� ��Ʈ
class stairNote : public note {
public:
	stairNote(int);
};

// �� ��Ʈ
class longNote : public note {
public:
	longNote(int);
};

// �� ��Ʈ
class nieunNote : public note {
public:
	nieunNote(int);
};