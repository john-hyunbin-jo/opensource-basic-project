#pragma once
#include <stdio.h>
#include <Windows.h> // Ŀ����ġ �̵� �� â ���� ���� ���̺귯��
#define DEFAULT_MENU_COLS 120
#define DEFAULT_MENU_LINES 40

BOOL set_cursor_pos(int, int);
void pause(char*);