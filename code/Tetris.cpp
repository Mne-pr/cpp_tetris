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

// ��Ʈ��ü, �������̺�ü, ��Ʈ�ѷ���ü
// ��Ʈ�ѷ���ü�� �������̺�ü�� ��ϵ� ��Ʈ��ü ������ ��������

// �����̴� ���� - �������� �ð��� �Ǿ��� �� or ������ �Է��� ��
// �ε����� ���� - ��ü�� ��ġ�� �ٲپ��� ��, ��ģ�ٸ� ����ϱ�
// ������� ���� - �������̺��� �� �ϳ��� �ϼ��Ǿ��� ��
// ����ϴ� ��� - ���� ��� 0���� �ʱ�ȭ -> ��ü �ϳ��� �߰��غ�. �� �������� �̹� 1�� �����ϴ� ���� �����̷��� �Ѵٸ�, ��ü�� �������� ���
// �����ϴ� ��� - ������

#define LEFT  (int)'a'
#define RIGHT (int)'d'
#define DOWN  (int)'s'
#define ROT   (int)'w'

#define PLANEW 8
#define PLANEH 20

// ��Ʈ Ŭ����
class note {
public:
	int ltx = 0, lty = 0;
	int** shape = nullptr;
	int sW = 0, sH = 0;
	int command = 0; // 0 - ��, 1 - ��, 2 - ��

	note(int w, int h) : sW(w), sH(h) {
		//shape = (int**)malloc(sizeof(int*) * sH);
		shape = new int* [sH];
		for (int i = 0; i < sH; i++) {
			shape[i] = new int[sW](); // ()�� 0���� �ʱ�ȭ�Ѵٴ� ��
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

	void printTable() {
		// just for debug
		for (int i = 0; i < sH; i++) {
			for (int j = 0; j < sW; j++) {
				if   ((shape[i][j]) == 0) cout << "��";
				else cout << "��";
			}
			cout << endl;
		}
	}

};

// ��Ʈ ���� Ŭ����
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

// ������ Ŭ����
class rendering {
public:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int backScreen = 0;
	int curx = 0, cury = 0;

	rendering() {
		// �ܼ�â ����
		oss << "mode con: cols=100 lines=40";
		system(oss.str().c_str());
		system("title TETRIS");
		screenInit();
	}

	void screenInit() {
		// ���� ����
		screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		// Ŀ�� �Ӽ� ����
		CURSOR.dwSize = 1;
		CURSOR.bVisible = false;
		// ���ۿ� Ŀ�� �Ӽ� ����
		SetConsoleCursorInfo(screen[0], &CURSOR);
		SetConsoleCursorInfo(screen[1], &CURSOR);
	}

	// ���� �׸��� - �ؽ�Ʈ, ��ǥ����
	void screenRender(int x, int y, char* text) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)x,(SHORT)y };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
	}

	// ���� �׸��� - ����, ��ǥ����
	void screenRender(int x, int y, char ch) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)x,(SHORT)y };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
	}

	// ���� �׸��� - �ؽ�Ʈ
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

	// �����ڵ��
	void screenRender(wchar_t ch,char unicode) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
		curx = curx + 2; // �����ڵ��
	}

	void screenRender(int x, int y, wchar_t ch, char unicode) {
		DWORD err;
		COORD backBufferCursor = { (SHORT)x,(SHORT)y };
		SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
		WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
	}

	// ���۸� ����Ʈ�� �ø�
	void screenswitch() {
		SetConsoleActiveScreenBuffer(screen[backScreen]);
		if (backScreen == 0) backScreen = 1;
		else backScreen = 0;
	}

	void screenEndl() {
		cury++; curx = 0;
	}

	// ���� Ŭ���� - ������ǥ, ����ǥ
	void screenClear(int x, int y, int x2, int y2) {
		for (int i = y; i <= y2 - y; i++) {
			COORD Coor = { (SHORT)x,(SHORT)i };
			DWORD dw;
			FillConsoleOutputCharacter(screen[backScreen], ' ', x2 - x, Coor, &dw);
		}
		curx = cury = 0;
	}
};

// �� Ŭ����
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
		// ��ġ���� �˻�. ��ġ�� ���� �� stockTable�� �߰��� �� ��ȣ��ȯ
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

		// ��ġ�� ������ movTable�� �߰�
		movNote(n);
		return 0;
	}

	void tablePrint() {
		for (int i = 0; i < PLANEW + 2; i++) { render->screenRender(L'��', 0); } render->screenEndl();

		for (int i = 4; i < PLANEH; i++) {
			render->screenRender(L'��',0);
			for (int j = 0; j < PLANEW; j++) {
				
				if (movTable[i][j] == 1 || stockTable[i][j] == 1) render->screenRender(L'��',0);
				else render->screenRender(L'��',0);
			}
			render->screenRender(L'��', 0); render->screenEndl();
		}

		for (int i = 0; i < PLANEW + 2; i++) { render->screenRender(L'��', 0); } render->screenEndl();
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
				render->screenRender((PLANEW + j) * 2 + 6, i + 2 , L'��', 0);
			}
		}

	}
};


// ��Ʈ�ѷ� Ŭ����
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

	void makeNoteN() {
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

	void running(int direction) {
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
		// ���ӿ��� ��� ��
		// ���� ������ ��Ȳ ����� �� ����
	}

	void RunGame() {
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

};


int main() {
	controller con;
	con.RunGame();
}


// �ؾ��ϴ� �� 
// �������..
// 
// Ư�� ���ھ� �̻� �������� �ӵ� ����
// ���߿��� ���� ������ ���� ����
// �� �� �����ϱ�
// �����̽� �� �Է� �� �ٷ� ��������
// 
// �� ���� ���ֱ� -ok
// ���ӿ��� ���� -ok
// 