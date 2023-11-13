#include "rendering.h"

std::ostringstream oss;
using namespace std;

// ������ Ŭ����

rendering::rendering() {
	// �ܼ�â ����
	backScreen = curx = cury = 0;
	oss << "mode con: cols=100 lines=40" << endl;
	system(oss.str().c_str());
	system("title TETRIS");
	screenInit();
}

rendering::~rendering() {
	CloseHandle(screen[0]);
	CloseHandle(screen[1]);
}

void rendering::screenInit() {
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
void rendering::screenRender(int x, int y, char* text) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
}

// ���� �׸��� - ����, ��ǥ����
void rendering::screenRender(int x, int y, char ch) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
}

// ���� �׸��� - �ؽ�Ʈ
void rendering::screenRender(char* text) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
	curx = curx + (int)strlen(text);
}

void rendering::screenRender(char ch) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
	curx++;
}

// �����ڵ��
void rendering::screenRender(wchar_t ch, char unicode) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
	curx = curx + 2; // �����ڵ��
}

void rendering::screenRender(int x, int y, wchar_t ch, char unicode) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
}

// ���۸� ����Ʈ�� �ø�
void rendering::screenswitch() {
	SetConsoleActiveScreenBuffer(screen[backScreen]);
	if (backScreen == 0) backScreen = 1;
	else backScreen = 0;
}

void rendering::screenEndl() { cury++; curx = 0; }

// ���� Ŭ���� - ������ǥ, ����ǥ
void rendering::screenClear(int x, int y, int x2, int y2) {
	for (int i = y; i <= y2 - y; i++) {
		COORD Coor = { (SHORT)x,(SHORT)i };
		DWORD dw;
		FillConsoleOutputCharacter(screen[backScreen], ' ', x2 - x, Coor, &dw);
	}
	curx = cury = 0;
}

