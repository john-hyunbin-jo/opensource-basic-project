#include <Windows.h>
#include "wincore.h"

BOOL set_cursor_pos(int x, int y)
{
	COORD _ = { x, y };
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _);
}

void pause(char* msg)
{
	if (msg == NULL)
		printf("�ƹ� Ű�� ���� ����ϼ���...");
	else
		printf("%s", msg);
	system("PAUSE > NUL");
}