#include <iostream>
#include <conio.h>
#include <chrono>
#include <random>
#include <thread>
#include <cstdlib>
#include <string>
#include <Windows.h>
#include <sstream>


using namespace std;
using namespace std::chrono;
std::ostringstream oss;
std::stringstream ss;

// 노트객체, 메인테이블객체, 컨트롤러객체
// 컨트롤러객체가 메인테이블객체와 등록된 노트객체 정보를 갖고있음

// 움직이는 조건 - 떨어지는 시간이 되었을 때 or 유저가 입력할 때
// 부딪히는 조건 - 객체의 위치를 바꾸었을 때, 겹친다면 취소하기
// 사라지는 조건 - 메인테이블에서 줄 하나가 완성되었을 때
// 출력하는 방법 - 판을 모두 0으로 초기화 -> 객체 하나씩 추가해봄. 그 과정에서 이미 1이 존재하는 곳에 움직이려고 한다면, 객체가 움직임을 취소
// 생성하는 방법 - 생각좀

#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'

#define PLANEW 8
#define PLANEH 20

// 노트 클래스
class note {
public:
	int ltx = 0, lty = 0;
	int** shape = nullptr;
	int sW = 0, sH = 0;
	int command = 0; // 0 - 하, 1 - 좌, 2 - 우

	note(int w, int h) : sW(w), sH(h) {
		//shape = (int**)malloc(sizeof(int*) * sH);
		shape = new int* [sH];
		for (int i = 0; i < sH; i++) {
			shape[i] = new int[sW](); // ()는 0으로 초기화한다는 뜻
		}
	}

	void setShapeWH(int w, int h) { note(sH, sW); }

	void setLocation(int x, int y) { ltx = x; lty = y; }

	void setLocation(mt19937& gen) {
		uniform_int_distribution<int> distribution(sW - 1, PLANEW - sW);
		ltx = distribution(gen);
		lty = 4 - sH;
	}

	void go() { 
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

	void ungo() { 
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

	void rotate() {
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

	void printTable() {
		// just for debug
		for (int i = 0; i < sH; i++) {
			for (int j = 0; j < sW; j++) {
				if   ((shape[i][j]) == 0) cout << "■";
				else cout << "□";
			}
			cout << endl;
		}
	}

};

// 노트 종류 클래스
class fuNote : public note {
public:
	fuNote(int mode) : note(3, 2) {
		
		shape[0][1] = shape[1][0] = shape[1][1] = shape[1][2] = 1;
		for (int i = 0; i < mode % 4; i++) rotate();
	}
};
class nemoNote : public note {
public:
	nemoNote() : note(2, 2) {
		shape[0][0] = shape[0][1] = shape[1][0] = shape[1][1] = 1;
	}

	void rotate() {}
};
class stairNote : public note {
public:
	stairNote(int mode) : note(3, 2) {
		if (mode % 2 == 0) shape[0][1] = shape[0][2] = shape[1][0] = shape[1][1] = 1;
		else { setShapeWH(2, 3); shape[0][0] = shape[0][1] = shape[1][1] = shape[1][2] = 1; }
		for (int i = 0; i < mode % 2; i++) rotate();
	}
};
class longNote : public note {
public:
	longNote(int mode) : note(4, 1) {
		shape[0][0] = shape[0][1] = shape[0][2] = shape[0][3] = 1;
		for (int i = 0; i < mode % 2; i++) rotate();
	}
};

// 랜더링 클래스
class rendering {
public:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int backScreen = 0;
	int curx = 0, cury = 0;

	rendering() {
		// 콘솔창 설정
		oss << "mode con: cols=100 lines=40";
		system(oss.str().c_str());
		system("title TETRIS");
		screenInit();
	}

	void screenInit() {
		// 버퍼 생성
		screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		// 커서 속성 설정
		CURSOR.dwSize = 1;
		CURSOR.bVisible = false;
		// 버퍼에 커서 속성 지정
		SetConsoleCursorInfo(screen[0], &CURSOR);
		SetConsoleCursorInfo(screen[1], &CURSOR);
	}

	// 버퍼 그리기 - 텍스트, 좌표지정
	void screenRender(int x, int y, char* text) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)x,(SHORT)y };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
	}

	// 버퍼 그리기 - 문자, 좌표지정
	void screenRender(int x, int y, char ch) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)x,(SHORT)y };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
	}

	// 버퍼 그리기 - 텍스트
	void screenRender(char* text) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
		curx = curx + (int)strlen(text);
	}

	void screenRender(char ch) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
		curx++;
	}

	// 유니코드용
	void screenRender(wchar_t ch,char unicode) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
		curx = curx + 2; // 유니코드라서
	}

	void screenRender(int x, int y, wchar_t ch, char unicode) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)x,(SHORT)y };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
	}

	// 버퍼를 프론트에 올림
	void screenswitch() {
		SetConsoleActiveScreenBuffer(screen[backScreen]);
		if (backScreen == 0) backScreen = 1;
		else backScreen = 0;
	}

	void screenEndl() {
		cury++; curx = 0;
	}

	// 버퍼 클리어 - 시작좌표, 끝좌표
	void screenClear(int x, int y, int x2, int y2) {
		for (int i = y; i <= y2 - y; i++) {
			COORD Coor = { (SHORT)x,(SHORT)i };
			DWORD dw;
			FillConsoleOutputCharacter(screen[backScreen], ' ', x2 - x, Coor, &dw);
		}
		curx = cury = 0;
	}
};

// 판 클래스
class planeTable {
public:
	int movTable[PLANEH][PLANEW] = { 0 };
	int stockTable[PLANEH][PLANEW] = { 0 };
	rendering* render;
	int score = 0;
	int gameover = 0;

	planeTable() {}
	planeTable(rendering* r) { render = r; }

	void clearMovTable() {
		for (int i = 0; i < PLANEH; i++) {
			for (int j = 0; j < PLANEW; j++) {
				movTable[i][j] = 0;
			}
		}
	}

	void stockNote(note* n) {
		for (int i = 0; i < n->sH; i++) {
			for (int j = 0; j < n->sW; j++) {
				if (n->shape[i][j] == 1) { 
					if (n->lty + i <= 3) gameover = 1;
					stockTable[n->lty + i][n->ltx + j] = 1; 

				}
			}
		}
	}

	void movNote(note* n) {
		for (int i = 0; i < n->sH; i++) {
			for (int j = 0; j < n->sW; j++) {
				movTable[n->lty + i][n->ltx + j] = n->shape[i][j];
			}
		}
	}

	void checkLine() {
		for (int tt = 0; tt < PLANEH; tt++) {
			for (int i = PLANEH - 1; i >= 0; i--) {
				int stock = 0;
				for (int j = 0; j < PLANEW; j++) {
					if (stockTable[i][j] == 1) stock++;
				}
				if (stock == PLANEW) {
					for (int k = i - 1; k >= 0; k--) {
						for (int l = 0; l < PLANEW; l++) {
							stockTable[k + 1][l] = stockTable[k][l];
						}
					}
					score += PLANEW;
				}
			}
		}
	}

	int setMovTable(note* n) {
		// 겹치는지 검사. 겹치면 복구 후 stockTable에 추가한 뒤 신호반환
		n->go();
		for (int i = 0; i < n->sH; i++) {
			for (int j = 0; j < n->sW; j++) {
				if (n->shape[i][j] == 1 && stockTable[n->lty + i][n->ltx + j] == 1 || n->sH + n->lty > PLANEH) {
					n->ungo(); stockNote(n); return 1;
				}
				if (n->ltx < 0 || n->ltx + n->sW > PLANEW) {
					n->ungo(); break;
				}

			}
		}

		// 겹치지 않으면 movTable에 추가
		movNote(n);
		return 0;
	}

	void tablePrint() {
		for (int i = 0; i < PLANEW + 2; i++) { render->screenRender(L'▣', 0); } render->screenEndl();

		for (int i = 4; i < PLANEH; i++) {
			render->screenRender(L'▣',0);
			for (int j = 0; j < PLANEW; j++) {
				
				if (movTable[i][j] == 1 || stockTable[i][j] == 1) render->screenRender(L'■',0);
				else render->screenRender(L'′',0);
			}
			render->screenRender(L'▣', 0); render->screenEndl();
		}

		for (int i = 0; i < PLANEW + 2; i++) { render->screenRender(L'▣', 0); } render->screenEndl();
	}
	
	void scorePrint() {
		ss.str(""); ss << "score" << endl;
		render->screenRender(PLANEW * 2 + 5, 10, (char*)(ss.str().c_str()));

		ss.str(""); ss << score << endl;
		render->screenRender(PLANEW * 2 + 5, 11, (char*)(ss.str().c_str()));
	}

	void nextPrint(note* n) {
		ss.str(""); ss << "next :"<< endl;
		render->screenRender(PLANEW * 2 + 5 , 0, (char*)(ss.str().c_str()));

		for (int i = 0; i < n->sH; i++) {
			for (int j = 0; j < n->sW; j++) {
				if (n->shape[i][j] == 0) continue;
				render->screenRender((PLANEW + j) * 2 + 6, i + 2 , L'■', 0);
			}
		}

	}
};


// 컨트롤러 클래스
class controller {
public:
	int standby = 0;
	planeTable table;
	note* n = nullptr;
	note* next_n = nullptr;
	mt19937 gen;
	rendering render;

public:
	controller() : gen(random_device()()) { 
		makeNote(); makeNoteN(); standby = 1; 
		table = planeTable(&render);
	}

	void makeNote() {
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

	void makeNoteN() {
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

	void running(int direction) {
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
	
	void statPrint() {
		ss.str(""); ss << "ltx : " << n->ltx << endl;
		render.screenRender((char*)(ss.str().c_str())); render.screenEndl();

		ss.str(""); ss << "lty : " << n->lty << endl;
		render.screenRender((char*)(ss.str().c_str())); render.screenEndl();

		ss.str(""); ss << "sH : " << n->sH << endl;
		render.screenRender((char*)(ss.str().c_str())); render.screenEndl();

		ss.str(""); ss << "sW : " << n->sW << endl;
		render.screenRender((char*)(ss.str().c_str())); render.screenEndl();

		ss.str(""); ss << "direction : " << n->command << endl;;
		render.screenRender((char*)(ss.str().c_str()));
	}

	void endPrint() {
		//render.screenClear(0, 0, 100, 100);

		//ss.str(""); ss << "score : " << table.score << endl;
		//render.screenRender((char*)(ss.str().c_str()));

		//render.screenRender();
		// 게임오버 출력 후
		// 현재 점수나 상황 출력한 뒤 종료
	}

	void RunGame() {
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

};


int main() {
	controller con;
	con.RunGame();
}


// 해야하는 것 
// 오류잡기..
// 
// 특정 스코어 이상 떨어지는 속도 증가
// 공중에서 많이 돌리면 점수 증가
// 블럭 색 지정하기
// 스페이스 바 입력 시 바로 떨어지게
// 
// 한 번에 없애기 -ok
// 게임오버 조건 -ok
// 