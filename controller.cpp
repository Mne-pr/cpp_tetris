#include "controller.h"

#include <iostream>

using namespace std;
using namespace std::chrono;
std::stringstream sss;

// 컨트롤러 클래스
controller::controller() : gen(random_device()()) {
	makeNote(); makeNoteN(); standby = 1;
	table = planeTable(&render);
	runSpeed = last_runSpeed = 1.0;
	clearCount = 0;
}

void controller::makeNote() {
	// 랜덤 값, 모드 생성
	uniform_int_distribution<int> distribution(1, 100);
	int rd = distribution(gen), mode = distribution(gen);

	switch (rd % 5) {
	case 0:
		n = new fuNote(mode); break;
	case 1:
		n = new nemoNote(); break;
	case 2:
		n = new stairNote(mode); break;
	case 3:
		n = new longNote(mode); break;
	case 4:
		n = new nieunNote(mode); break;
	}
	n->setLocation(gen);
}

void controller::makeNoteN() {
	// 랜덤 값, 모드 생성
	uniform_int_distribution<int> distribution(1, 100);
	int rd = distribution(gen), mode = distribution(gen);

	switch (rd % 5) {
	case 0:
		next_n = new fuNote(mode); break;
	case 1:
		next_n = new nemoNote(); break;
	case 2:
		next_n = new stairNote(mode); break;
	case 3:
		next_n = new longNote(mode); break;
	case 4:
		next_n = new nieunNote(mode); break;
	}
	next_n->setLocation(gen);
}

void controller::running(int direction) {
	// 화면 클리어
	int res = 0;

	if (standby == 0) {
		n = next_n;
		makeNoteN();
	}
	n->command = direction;

	// 일단 노트 테이블에 추가 시도. 완료->삭제 후 새 노드
	while (1) {
		table.clearMovTable();
		if (direction == SPACE) n->command = DOWN;

		res = table.setMovTable(n);
		if (res == 0) standby = 1;
		else {
			note* t = n; delete t; standby = 0;
		}

		if (direction == SPACE && standby == 1) continue;
		else break;
	}

	if (table.checkLine() == 1) {
		clearCount++;
		if (clearCount % 5 == 4) { last_runSpeed = runSpeed; runSpeed -= 0.01; }
	}

	// 테이블 출력
	render.screenClear(0, 0, 100, 100);
	table.tablePrint(); table.nextPrint(next_n); table.scorePrint();
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

	sss.str(""); sss << "direction : " << n->command << endl;
	render.screenRender((char*)(sss.str().c_str())); render.screenEndl();

	sss.str(""); sss << "runspeed : " << runSpeed;
	if (runSpeed != last_runSpeed) { sss << " speed up!"; ast_runSpeed = runSpeed; } sss << endl;
	render.screenRender((char*)(sss.str().c_str()));
}

void controller::endPrint(int m) {
	render.screenClear(0, 0, 100, 100);

	render.screenRender(0, (PLANEH - 4) / 2 + 1, (char*)"game over!");
	sss.str(""); sss << "score : " << table.score << endl;
	render.screenRender(0, (PLANEH - 4) / 2 + 2,(char*)(sss.str().c_str()));
	if (m == 0) render.screenRender(0, (PLANEH - 4) / 2 + 3, (char*)"press any key to exit");

	render.screenswitch();
}

void controller::RunGame() {
	// 현재 시간 측정
	auto T = high_resolution_clock::now();

	while (true) {
		auto cT = high_resolution_clock::now();
		// 1초마다
		if (duration_cast<duration<double>>(cT - T).count() >= runSpeed) {
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

	while (true) {
		auto cT = high_resolution_clock::now();
		if (duration_cast<duration<double>>(cT - T).count() >= 0.5) {
			T = high_resolution_clock::now();
			endPrint(standby); // 그냥 변수 하나 더 만들기 싫어서 재활용
			if (standby == 0) standby = 1;
			else standby = 0;
		}

		if (_kbhit()) break;
		// 입력값이 r인지 확인 후 해당 값을 메인으로 리턴, 그럼 controller::RunGame()은 int형이라던가
		this_thread::sleep_for(milliseconds(10)); // 0.1초 대기
	}

}

