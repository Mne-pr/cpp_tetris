#pragma once

#include <random>

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

// ㅗ 노트
class fuNote : public note {
public:
	fuNote(int);
};

// ㅁ 노트
class nemoNote : public note {
public:
	nemoNote();
	void rotate();
};

// ㄱㄴ 노트
class stairNote : public note {
public:
	stairNote(int);
};

// ㅣ 노트
class longNote : public note {
public:
	longNote(int);
};
