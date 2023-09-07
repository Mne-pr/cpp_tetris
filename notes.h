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
