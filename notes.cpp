#include "notes.h"
#include <iostream>

#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'

#define PLANEW 8
#define PLANEH 20

using namespace std;

// 노트 클래스
note::note(int w, int h) : sW(w), sH(h) {
	//shape = (int**)malloc(sizeof(int*) * sH);
	shape = new int* [sH];
	for (int i = 0; i < sH; i++) {
		shape[i] = new int[sW](); // ()는 0으로 초기화한다는 뜻
	}
	ltx = lty = command = 0;
}

void note::setShapeWH(int w, int h) { note(sH, sW); }

void note::setLocation(int x, int y) { ltx = x; lty = y; }

void note::setLocation(mt19937& gen) {
	uniform_int_distribution<int> distribution(sW - 1, PLANEW - sW);
	ltx = distribution(gen);
	lty = 4 - sH;
}

void note::go() {
	switch (command) {
	case DOWN:
		++lty; break;
	case LEFT:
		--ltx; break;
	case RIGHT:
		++ltx; break;
	case ROT:
		rotate(); break;
	}
}

void note::ungo() {
	switch (command) {
	case DOWN:
		--lty; break;
	case LEFT:
		++ltx; break;
	case RIGHT:
		--ltx; break;
	case ROT:
		rotate(); rotate(); rotate(); break;

	}
}

void note::rotate() {
	// 바꿀 공간(전치) 새로 선언
	int** newShape = new int* [sW];
	for (int i = 0; i < sW; i++) {
		newShape[i] = new int[sH]();
	}

	// 새 공간에 옮기고
	for (int i = 0; i < sH; i++) {
		for (int j = 0; j < sW; j++) {
			newShape[j][i] = shape[i][j];
		}
	}

	int** tempShape = shape; shape = newShape;
	for (int i = 0; i < sH; i++) free(tempShape[i]);
	free(tempShape);

	int temp = sW; sW = sH; sH = temp;

	if (sW == 1 || sH == 1) return;

	// 각 행을 뒤집음
	for (int i = 0; i < sH; i++) {
		for (int j = 0; j < sW / 2; j++) {
			int temp = shape[i][j];
			shape[i][j] = shape[i][sW - 1 - j];
			shape[i][sW - 1 - j] = temp;
		}
	}
}

void note::printTable() {
	// just for debug
	for (int i = 0; i < sH; i++) {
		for (int j = 0; j < sW; j++) {
			if ((shape[i][j]) == 0) cout << "■";
			else cout << "□";
		}
		cout << endl;
	}
}



// 노트 종류 클래스

fuNote::fuNote(int mode) : note(3, 2) {
	shape[0][1] = shape[1][0] = shape[1][1] = shape[1][2] = 1;
	for (int i = 0; i < mode % 4; i++) rotate();
}

nemoNote::nemoNote() : note(2, 2) {
	shape[0][0] = shape[0][1] = shape[1][0] = shape[1][1] = 1;
}
void nemoNote::rotate() {}

stairNote::stairNote(int mode) : note(3, 2) {
	if (mode % 2 == 0) shape[0][1] = shape[0][2] = shape[1][0] = shape[1][1] = 1;
	else { setShapeWH(2, 3); shape[0][0] = shape[0][1] = shape[1][1] = shape[1][2] = 1; }
	for (int i = 0; i < mode % 2; i++) rotate();
}

longNote::longNote(int mode) : note(4, 1) {
	shape[0][0] = shape[0][1] = shape[0][2] = shape[0][3] = 1;
	for (int i = 0; i < mode % 2; i++) rotate();
}
