#include "store_core.h"
#include "c_essentials.h"
#include "store_data_types.h"
#include "wincore.h"
#include <io.h>  //파일이 존재하는지 안하는지 판단하기 위한 라이브러리

#define STOREINFO_FILENAME "xf_storeinfo.bin"
#define ITEMDB_FILENAME "xf_itemdb.bin"

FILE* fpStoreinfo = NULL;
FILE* fpItemstock = NULL;
StoreinfoPtr gStoreinfo = NULL;
ItemPtr gItemstock = NULL;

/* ------------------------------------------------------------
*	void init(): 처음 시작 초기화 함수.
*/
void init()
{
#ifndef __STOREMGR_INIT__ //한번만 실행하기 위해 정의
#define __STOREMGR_INIT__
	printf("   [i] 처음 시작 준비 중...\n");
	// 상점 정보 메모리와 상품 정보 메모리 할당하기.
	gStoreinfo = malloc(sizeof(Storeinfo));
	gItemstock = malloc(sizeof(Item) * ITEM_COUNT_MAX);
	if (!gStoreinfo)// Storeinfo, gItemstock 포인터 할당 됐는지 확인
	{
		printf("   [!] 상점 정보 메모리를 할당할 수 없습니다. 실행할 수 없습니다.\n");
		exit(EXIT_FAILURE);
	}
	if (!gItemstock)
	{
		printf("   [!] 상품 정보 메모리를 할당할 수 없습니다. 실행할 수 없습니다.\n");
		exit(EXIT_FAILURE);
	}
	if (access(STOREINFO_FILENAME, 0) != -1)
	{
		fpStoreinfo = fopen(STOREINFO_FILENAME, "rb+");	// 상점 정보 파일 열기, rb+ 모드.
		if (!fpStoreinfo)
		{
			printf("   [!] 상점 정보 파일에 액세스 불가능합니다. 실행할 수 없습니다.\n");
			exit(EXIT_FAILURE);
		}
		fread(gStoreinfo, sizeof(Storeinfo), 1, fpStoreinfo);
	}
	else
	{
		printf("   [!] 상점 정보 파일이 존재하지 않습니다. 기본 설정으로 계속합니다.\n");
		strcpy(gStoreinfo->name, "StoreManager 상점");	// 기본 상점 이름 입력.
		strcpy(gStoreinfo->contact, "no data");			// 기본 연락처 정보 입력.
		fpStoreinfo = fopen(STOREINFO_FILENAME, "wb+");	// 파일 포인터로 열기, wb+ 모드.
		fwrite(gStoreinfo, sizeof(Storeinfo), 1, fpStoreinfo);	// 파일 포인터에 쓰기.
	}
	if (access(ITEMDB_FILENAME, 0) != -1)
	{
		fpItemstock = fopen(ITEMDB_FILENAME, "rb+");	// 상품 정보 파일 열기, rb+ 모드.
		if (!fpItemstock)
		{
			printf("   [!] 상품 DB 파일에 액세스 불가능합니다. 실행할 수 없습니다.\n");
			exit(EXIT_FAILURE);
		}
		// 데이터 읽어와서 메모리에 올리기.
		fread(gItemstock, sizeof(Item), ITEM_COUNT_MAX, fpItemstock);
	}
	else
	{
		printf("   [!] 상품 DB 파일이 존재하지 않습니다. 기본 설정으로 계속합니다.\n");
		// 기본 설정을 추가, 일단 아무것도 없는 상태로 시작.
		for (int i = 0; i < ITEM_COUNT_MAX; i++)
		{
			strcpy(gItemstock[i].name, "undefined");
			gItemstock[i].price = 0;
			gItemstock[i].quantity = 0;
			gItemstock[i].accumulated_quantity = 0;
			gItemstock[i].available = true;
		}
		fpItemstock = fopen(ITEMDB_FILENAME, "wb+");	// 파일 포인터로 열기, wb+ 모드.
		fwrite(gItemstock, sizeof(Item), ITEM_COUNT_MAX, fpItemstock);	// 파일 포인터에 쓰기.
	}
#endif
}

void write_storeinfo()
{
	fseek(fpStoreinfo, 0, SEEK_SET);
	fwrite(gStoreinfo, sizeof(Storeinfo), 1, fpStoreinfo);
}

void write_itemstock()
{
	fseek(fpItemstock, 0, SEEK_SET);
	fwrite(gItemstock, sizeof(Item), ITEM_COUNT_MAX, fpItemstock);
}

char* get_store_name()
{
	return gStoreinfo->name;
}

char* get_store_contact()
{
	return gStoreinfo->contact;
}

void prompt_menu_header(char* title, int cols, int lines)
{
	system("cls");
	char parsed_str[255] = { '\0' };
	sprintf(parsed_str, "mode con: cols=%d lines=%d", cols, lines);
	system(parsed_str);
	printf("  ┌──────────────────────────────────────────────────────────────────────┐\n");
	printf("  │  %s", title); set_cursor_pos(73, 1); printf("│\n");
	printf("  └──────────────────────────────────────────────────────────────────────┘\n");
}

void prompt_aligned_options(int c, char options[][MENU_STRING_LEN], int line)
{
	for (int i = 0; i < c; i++)
	{
		set_cursor_pos(3 + (i % 3) * 24, line + (i / 3));
		printf("%02d. %s", i + 1, options[i]);
	}
}

void prompt_itemlist(BOOL is_filtered)
{
	// 전체 상품 목록을 조회.
	printf("   %-10s  %-32s  %-10s  %-10s  %-10s  %-10s\n   ", "No.", "상품명", "가격", "재고", "누적판매량", "활성화");
	for (int i = 0; i < 92; i++)
		printf("-");
	printf("\n");
	int count = 0;
	for (int i = 0; i < ITEM_COUNT_MAX; i++)
	{
		if (is_filtered)
		{
			if (!(gItemstock[i].available))
				continue;
			printf("   %-10d  %-32s  %-10d  %-10d  %-10d  %-10s\n", i, gItemstock[i].name, gItemstock[i].price, gItemstock[i].quantity, gItemstock[i].accumulated_quantity, "O");
		}
		else
			printf("   %-10d  %-32s  %-10d  %-10d  %-10d  %-10s\n", i, gItemstock[i].name, gItemstock[i].price, gItemstock[i].quantity, gItemstock[i].accumulated_quantity, (gItemstock[i].available) ? "O" : "X");
		count++;
	}
	printf("\n   총 %d개 항목\n", count);
}

void prompt_editmode(int index)
{
	while (1)
	{
		SetConsoleTitle(L"세부 수정 모드: 상점 관리 프로그램");
		char buffer[256] = { '\0' };
		sprintf(buffer, "세부 수정 모드 → (%d)%s", index, gItemstock[index].name);
		prompt_menu_header(buffer, 140, DEFAULT_MENU_LINES);
		printf("\n");
		
		printf("   %-32s  %-10s  %-10s  %-10s  %-10s\n   ",  "상품명", "가격", "재고", "누적판매량", "활성화");
		for (int i = 0; i < 80; i++)
			printf("-");
		printf("\n   %-32s  %-10d  %-10d  %-10d  %-10s\n", gItemstock[index].name, gItemstock[index].price, gItemstock[index].quantity, gItemstock[index].accumulated_quantity, (gItemstock[index].available) ? "O" : "X");
		char menu[6][MENU_STRING_LEN] = { "상품명", "가격", "수량", "누적판매 초기화", "활성화 토글", "나가기" };
		prompt_aligned_options(6, menu, 8);
		printf("\n\n   [i] 키를 누르면 메뉴가 열립니다.\n");
		int option = getch() - '0';
		if (option == 6)
			break;
		else if (option == 1)
		{
			// 상품명 수정 창.
			printf("\n   [i] 새로운 상품명을 입력하십시오(최대 %d자): ", ITEM_NAME_LEN_MAX - 1);
			char new_str[ITEM_NAME_LEN_MAX] = { '\0' };
			gets_s(new_str, ITEM_NAME_LEN_MAX);
			strcpy(gItemstock[index].name, new_str);
			write_itemstock();
			printf("   [i] 변경되었습니다. ");
			pause(NULL);
		}
		else if (option == 2)
		{
			printf("\n   [i] 새로운 가격을 입력하십시오: ");
			int new_price = 0;
			scanf("%d", &new_price);
			if (new_price < 0)
			{
				printf("   [!] 0 미만의 가격은 설정할 수 없습니다! ");
				pause(NULL);
				continue;
			}
			gItemstock[index].price = new_price;
			write_itemstock();
			printf("   [i] 변경되었습니다. ");
			pause(NULL);
		}
		else if (option == 3)
		{
			printf("\n   [i] 새로운 재고 수량을 입력하십시오: ");
			int new_quantity = 0;
			scanf("%d", &new_quantity);
			if (new_quantity < 0)
			{
				printf("   [!] 0 미만의 수량은 설정할 수 없습니다! ");
				pause(NULL);
				continue;
			}
			gItemstock[index].quantity = new_quantity;
			write_itemstock();
			printf("   [i] 변경되었습니다. ");
			pause(NULL);
		}
		else if (option == 4)
		{
			printf("\n   [i] 정말 누적 판매량을 초기화하시겠습니까?\n");
			printf("       현재 누적 판매량은 %d(수익: %d) 입니다.\n", gItemstock[index].accumulated_quantity, gItemstock[index].accumulated_quantity * gItemstock[index].price);
			printf("       계속하려면 'yes'를 대소문자 구분하여 입력해 주십시오: ");
			char confirm[256] = { '\0' };
			scanf("%s", confirm);
			if (strcmp(confirm, "yes") == 0)
			{
				gItemstock[index].accumulated_quantity = 0;
				write_itemstock();
				printf("   [i] 누적 판매량이 초기화되었습니다. ");
			}
			else
				printf("   [i] 취소되었습니다. ");
			pause(NULL);
		}
		else if (option == 5)
		{
			gItemstock[index].available = !(gItemstock[index].available);
			write_itemstock();
			printf("\n   [i] ");
			printf(gItemstock[index].available ? "활성화되었습니다. " : "비활성화되었습니다. ");
			pause(NULL);
		}
		else
		{
			printf("   [!] 올바르지 않은 입력입니다. 다시 입력하세요!\n");
			Sleep(1000);
			while (kbhit()) { getch(); }
			continue;
		}
	}
}

void prompt_sell()
{
	while (1)
	{
		SetConsoleTitle(L"판매: 상점 관리 프로그램");
		prompt_menu_header("상품 판매", 140, DEFAULT_MENU_LINES);
		printf("\n");
		prompt_itemlist(true);
		printf("   판매할 상품의 번호를 입력하세요(취소: -1): ");
		int no, quantity;
		scanf("%d", &no);
		if (no == -1)
			break;
		else if (no < 0 || no > ITEM_COUNT_MAX - 1)
			continue;
		else if (!(gItemstock[no].available))
		{
			printf("   [!] 선택한 항목은 비활성화 상태여서 판매할 수 없습니다. ");
			pause(NULL);
			continue;
		}
		printf("   판매할 수량을 입력하세요(취소: -1): ");
		scanf("%d", &quantity);
		if (quantity == -1)
			break;
		else if (quantity < 1)
		{
			printf("   [!] 최소 한 개 이상을 판매하여야 합니다! ");
			pause(NULL);
			continue;
		}
		else if (gItemstock[no].quantity - quantity < 0)
		{
			printf("   [!] 선택한 항목(%s)의 현재 수량은 %d개이므로 %d개를 판매할 수 없습니다. ", gItemstock[no].name, gItemstock[no].quantity, quantity);
			pause(NULL);
			continue;
		}
		// 판매처리.
		gItemstock[no].quantity -= quantity;
		gItemstock[no].accumulated_quantity += quantity;
		write_itemstock();
		printf("   [i] 선택한 항목(%s)를 %d개 판매하였습니다(남은 재고: %d개). ", gItemstock[no].name, quantity, gItemstock[no].quantity);
		pause(NULL);
		continue;
	}
}

void prompt_import()
{
	while (1)
	{
		SetConsoleTitle(L"입고: 상점 관리 프로그램");
		prompt_menu_header("상품 입고", 140, DEFAULT_MENU_LINES);
		printf("\n");
		prompt_itemlist(true);
		printf("   입고 처리할 상품의 번호를 입력하세요(취소: -1): ");
		int no, quantity;
		scanf("%d", &no);
		if (no == -1)
			break;
		else if (no < 0 || no > ITEM_COUNT_MAX - 1)
			continue;
		else if (!(gItemstock[no].available))
		{
			printf("   [!] 선택한 항목(%s)은 비활성화 상태여서 입고처리할 수 없습니다. ", gItemstock[no].name);
			pause(NULL);
			continue;
		}
		printf("   입고 수량을 입력하세요(취소: -1): ");
		scanf("%d", &quantity);
		if (quantity == -1)
			break;
		else if (quantity < 1)
		{
			printf("   [!] 최소 한 개 이상을 입고하여야 합니다! ");
			pause(NULL);
			continue;
		}
		// 입고 처리.
		gItemstock[no].quantity += quantity;
		write_itemstock();
		printf("   [i] 선택한 항목(%s)를 %d개 입고하였습니다(남은 재고: %d개). ", gItemstock[no].name, quantity, gItemstock[no].quantity);
		pause(NULL);
		continue;
	}
}

void prompt_update_item()
{
	while (1)
	{
		SetConsoleTitle(L"상품 정보 수정: 상점 관리 프로그램");
		prompt_menu_header("상품 정보 수정", 140, DEFAULT_MENU_LINES);
		printf("\n");
		prompt_itemlist(false);
		printf("   수정할 상품을 선택하세요(취소: -1): ");
		int no = -1;
		scanf("%d", &no);
		if (no == -1)
			break;
		else if (no < 0 || no > ITEM_COUNT_MAX - 1)
			continue;
		else
			prompt_editmode(no);
	}
}

void prompt_update_storeinfo()
{
	SetConsoleTitle(L"상점 정보 수정: 상점 관리 프로그램");
	prompt_menu_header("상점 정보 수정", 140, DEFAULT_MENU_LINES);
	printf("\n");
	// gStoreinfo 수정.
	printf("   현재 상점 이름: %s\n", gStoreinfo->name);
	printf("   현재 상점 연락처 정보: %s\n\n", gStoreinfo->contact);
	printf("   변경할 상점 이름을 입력해 주세요: ");
	// 입력 받기.
	char new_name[STORE_NAME_LEN_MAX] = { '\0' };
	gets_s(new_name, STORE_NAME_LEN_MAX);
	printf("   변경할 상점 연락처 정보를 입력해 주세요: ");
	char new_contact[STORE_COTACT_LEN_MAX] = { '\0' };
	gets_s(new_contact, STORE_COTACT_LEN_MAX);

	// 저장 및 리턴.
	// 우선 gStoreinfo 메모리에 저장한다.
	strcpy(gStoreinfo->name, new_name);
	strcpy(gStoreinfo->contact, new_contact);
	// 저장된 gStoreinfo 메모리에서 파일에다가 그대로 저장한다.
	write_storeinfo();
	printf("   [i] 변경되었습니다. ");
	pause(NULL);
	return;
}

void prompt_show_statistics()
{
	SetConsoleTitle(L"통계 보기: 상점 관리 프로그램");
	prompt_menu_header("통계", 140, DEFAULT_MENU_LINES);
	printf("\n");
	prompt_itemlist(false);
	getchar();
}