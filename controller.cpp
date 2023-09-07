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

// 컨트롤러 클래스
controller::controller() : gen(random_device()()) {
	makeNote(); makeNoteN(); standby = 1;
	table = planeTable(&render);
}

void controller::makeNote() {
	// 랜덤 값, 모드 생성
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
	// 랜덤 값, 모드 생성
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
	// 화면 클리어

	table.clearMovTable();

	if (standby == 0) {
		n = next_n;
		makeNoteN();
	}
	n->command = direction;

	// 일단 노트 테이블에 추가 시도. 완료->삭제 후 새 노드
	int res = table.setMovTable(n);
	if (res == 0) standby = 1;
	else {
		note* t = n; delete t; standby = 0;
	}
	table.checkLine();

	// 테이블 출력
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
	// 게임오버 출력 후
	// 현재 점수나 상황 출력한 뒤 종료
}

void controller::RunGame() {
	// 현재 시간 측정
	auto T = high_resolution_clock::now();

	while (true) {
		auto cT = high_resolution_clock::now();
		// 1초마다
		if (duration_cast<duration<double>>(cT - T).count() >= 0.5) {
			T = high_resolution_clock::now();
			running(DOWN);
		}

		// 입력받을 때
		if (_kbhit()) {
			char userInput = _getch();
			// 97 115 100
			running((int)userInput);
		}

		if (table.gameover == 1) break;
		this_thread::sleep_for(milliseconds(10)); // 0.1초 대기
	}

	endPrint();
}

