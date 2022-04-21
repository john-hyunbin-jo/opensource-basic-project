#include "Headers/c_essentials.h" 
#include "Headers/wincore.h" 
#include "Headers/store_core.h" 
#pragma warning (disable: 4996)	// Deprecated, 보안 함수 경고 비활성화.
#pragma warning (disable: 6031) // 반환값 무시 경고 비활성화.

int main(int argc, char** argv)
{
	init();
	// 콘솔 창 지우기: 메뉴 화면 다시 그리기.
	while (1)
	{
		SetConsoleTitle(L"상점 관리 프로그램");
		prompt_menu_header("상점 관리 프로그램", DEFAULT_MENU_COLS, DEFAULT_MENU_LINES);
		printf("   @ %s (☎ %s) ]\n", get_store_name(), get_store_contact());
		char menu_titles[6][MENU_STRING_LEN] = { "상품 판매", "상품 입고", "상품 데이터 수정", "상점 정보 수정", "통계 보기", "종료" };
		prompt_aligned_options(6, menu_titles, 5);
		printf("\n\n");
		printf("   [i] 키를 누르면 메뉴가 열립니다.\n");
		switch (getch() - '0')
		{
		case 1:
			prompt_sell();
			break;
		case 2:
			prompt_import();
			break;
		case 3:
			prompt_update_item();
			break;
		case 4:
			prompt_update_storeinfo();
			break;
		case 5:
			prompt_show_statistics();
			break;
		case 6:
			// 정상 종료 코드 0을 반환해 준다.
			exit(EXIT_SUCCESS);
		default:
			printf("\n\n   [!] 올바르지 않은 키입니다. 다시 입력하세요.");
			Sleep(1000);
			// IMPORTANT: 아래 while문으로 Sleep 동안 들어온 모든 입력버퍼를 비운다. fflush(stdin)으로는 안비워진다, 따라서 수동으로 버퍼 존재하는 동안 다 지워버리기.
			while (kbhit()) { getch(); }
			continue;
		}
	}
	return EXIT_FAILURE;
}