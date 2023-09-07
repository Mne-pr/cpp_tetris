#pragma once

#include <Windows.h>

// 랜더링 클래스
class rendering {
public:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int backScreen;
	int curx, cury;

	// 생성자
	rendering();

	// 버퍼 생성
	void screenInit();

	// 버퍼 그리기 - 일반문자
	void screenRender(char);
	// 버퍼 그리기 - 일반텍스트
	void screenRender(char*);
	// 버퍼 그리기 - x좌표, y좌표, 일반문자
	void screenRender(int, int, char);
	// 버퍼 그리기 - x좌표, y좌표,일반텍스트
	void screenRender(int, int, char*);

	// 버퍼 그리기 - 유니코드
	void screenRender(wchar_t, char);
	// 버퍼 그리기 - x좌표, y좌표, 유니코드
	void screenRender(int, int, wchar_t, char);

	// 버퍼와 프론트를 교체
	void screenswitch();

	// 엔터 - 좌표없이 srcrreRender()하는 경우 사용
	void screenEndl();

	// 버퍼 클리어 - 시작좌표, 끝좌표
	void screenClear(int, int, int, int);
};
