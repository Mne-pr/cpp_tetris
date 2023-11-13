#include "rendering.h"

std::ostringstream oss;
using namespace std;

// 랜더링 클래스

rendering::rendering() {
	// 콘솔창 설정
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
void rendering::screenRender(int x, int y, char* text) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
}

// 버퍼 그리기 - 문자, 좌표지정
void rendering::screenRender(int x, int y, char ch) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
}

// 버퍼 그리기 - 텍스트
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

// 유니코드용
void rendering::screenRender(wchar_t ch, char unicode) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
	curx = curx + 2; // 유니코드라서
}

void rendering::screenRender(int x, int y, wchar_t ch, char unicode) {
	DWORD err;
	COORD backBufferCursor = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
	WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
}

// 버퍼를 프론트에 올림
void rendering::screenswitch() {
	SetConsoleActiveScreenBuffer(screen[backScreen]);
	if (backScreen == 0) backScreen = 1;
	else backScreen = 0;
}

void rendering::screenEndl() { cury++; curx = 0; }

// 버퍼 클리어 - 시작좌표, 끝좌표
void rendering::screenClear(int x, int y, int x2, int y2) {
	for (int i = y; i <= y2 - y; i++) {
		COORD Coor = { (SHORT)x,(SHORT)i };
		DWORD dw;
		FillConsoleOutputCharacter(screen[backScreen], ' ', x2 - x, Coor, &dw);
	}
	curx = cury = 0;
}

