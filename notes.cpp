#include "notes.h"
#include <iostream>

#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'

#define PLANEW 8
#define PLANEH 20

using namespace std;

// ��Ʈ Ŭ����
note::note(int w, int h) : sW(w), sH(h) {
	//shape = (int**)malloc(sizeof(int*) * sH);
	shape = new int* [sH];
	for (int i = 0; i < sH; i++) {
		shape[i] = new int[sW](); // ()�� 0���� �ʱ�ȭ�Ѵٴ� ��
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
	// �ٲ� ����(��ġ) ���� ����
	int** newShape = new int* [sW];
	for (int i = 0; i < sW; i++) {
		newShape[i] = new int[sH]();
	}

	// �� ������ �ű��
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

	// �� ���� ������
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
			if ((shape[i][j]) == 0) cout << "��";
			else cout << "��";
		}
		cout << endl;
	}
}



// ��Ʈ ���� Ŭ����

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
