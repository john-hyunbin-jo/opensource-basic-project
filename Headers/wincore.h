#pragma once
#include <stdio.h>
#include <Windows.h> // 커서위치 이동 및 창 제목 수정 라이브러리
#define DEFAULT_MENU_COLS 120
#define DEFAULT_MENU_LINES 40

BOOL set_cursor_pos(int, int);
void pause(char*);