#include "controller.h"
#include <sstream>
#include <chrono>
#include <conio.h>
#include <thread>

#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'

using namespace std;
using namespace std::chrono;
std::stringstream sss;

// ��Ʈ�ѷ� Ŭ����
controller::controller() : gen(random_device()()) {
	makeNote(); makeNoteN(); standby = 1;
	table = planeTable(&render);
}

void controller::makeNote() {
	// ���� ��, ��� ����
	uniform_int_distribution<int> distribution(1, 100);
	int rd = distribution(gen), mode = distribution(gen);

	switch (rd % 4) {
	case 0:
		n = new fuNote(mode); break;
	case 1:
		n = new nemoNote(); break;
	case 2:
		n = new stairNote(mode); break;
	case 3:
		n = new longNote(mode); break;
	}
	n->setLocation(gen);
}

void controller::makeNoteN() {
	// ���� ��, ��� ����
	uniform_int_distribution<int> distribution(1, 100);
	int rd = distribution(gen), mode = distribution(gen);

	switch (rd % 4) {
	case 0:
		next_n = new fuNote(mode); break;
	case 1:
		next_n = new nemoNote(); break;
	case 2:
		next_n = new stairNote(mode); break;
	case 3:
		next_n = new longNote(mode); break;
	}
	next_n->setLocation(gen);
}

void controller::running(int direction) {
	// ȭ�� Ŭ����

	table.clearMovTable();

	if (standby == 0) {
		n = next_n;
		makeNoteN();
	}
	n->command = direction;

	// �ϴ� ��Ʈ ���̺� �߰� �õ�. �Ϸ�->���� �� �� ���
	int res = table.setMovTable(n);
	if (res == 0) standby = 1;
	else {
		note* t = n; delete t; standby = 0;
	}
	table.checkLine();

	// ���̺� ���
	render.screenClear(0, 0, 100, 100);
	table.tablePrint();
	table.nextPrint(next_n);
	table.scorePrint();
	if (res == 0) statPrint();
	render.screenswitch();
}

void controller::statPrint() {
	sss.str(""); sss << "ltx : " << n->ltx << endl;
	render.screenRender((char*)(sss.str().c_str())); render.screenEndl();

	sss.str(""); sss << "lty : " << n->lty << endl;
	render.screenRender((char*)(sss.str().c_str())); render.screenEndl();

	sss.str(""); sss << "sH : " << n->sH << endl;
	render.screenRender((char*)(sss.str().c_str())); render.screenEndl();

	sss.str(""); sss << "sW : " << n->sW << endl;
	render.screenRender((char*)(sss.str().c_str())); render.screenEndl();

	sss.str(""); sss << "direction : " << n->command << endl;;
	render.screenRender((char*)(sss.str().c_str()));
}

void controller::endPrint() {
	//render.screenClear(0, 0, 100, 100);

	//ss.str(""); ss << "score : " << table.score << endl;
	//render.screenRender((char*)(ss.str().c_str()));

	//render.screenRender();
	// ���ӿ��� ��� ��
	// ���� ������ ��Ȳ ����� �� ����
}

void controller::RunGame() {
	// ���� �ð� ����
	auto T = high_resolution_clock::now();

	while (true) {
		auto cT = high_resolution_clock::now();
		// 1�ʸ���
		if (duration_cast<duration<double>>(cT - T).count() >= 0.5) {
			T = high_resolution_clock::now();
			running(DOWN);
		}

		// �Է¹��� ��
		if (_kbhit()) {
			char userInput = _getch();
			// 97 115 100
			running((int)userInput);
		}

		if (table.gameover == 1) break;
		this_thread::sleep_for(milliseconds(10)); // 0.1�� ���
	}

	endPrint();
}

