#pragma once

#include <Windows.h>

// ������ Ŭ����
class rendering {
public:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int backScreen;
	int curx, cury;

	// ������
	rendering();

	// ���� ����
	void screenInit();

	// ���� �׸��� - �Ϲݹ���
	void screenRender(char);
	// ���� �׸��� - �Ϲ��ؽ�Ʈ
	void screenRender(char*);
	// ���� �׸��� - x��ǥ, y��ǥ, �Ϲݹ���
	void screenRender(int, int, char);
	// ���� �׸��� - x��ǥ, y��ǥ,�Ϲ��ؽ�Ʈ
	void screenRender(int, int, char*);

	// ���� �׸��� - �����ڵ�
	void screenRender(wchar_t, char);
	// ���� �׸��� - x��ǥ, y��ǥ, �����ڵ�
	void screenRender(int, int, wchar_t, char);

	// ���ۿ� ����Ʈ�� ��ü
	void screenswitch();

	// ���� - ��ǥ���� srcrreRender()�ϴ� ��� ���
	void screenEndl();

	// ���� Ŭ���� - ������ǥ, ����ǥ
	void screenClear(int, int, int, int);
};
