#include "store_core.h"
#include "c_essentials.h"
#include "store_data_types.h"
#include "wincore.h"
#include <io.h>  //������ �����ϴ��� ���ϴ��� �Ǵ��ϱ� ���� ���̺귯��

#define STOREINFO_FILENAME "xf_storeinfo.bin"
#define ITEMDB_FILENAME "xf_itemdb.bin"

FILE* fpStoreinfo = NULL;
FILE* fpItemstock = NULL;
StoreinfoPtr gStoreinfo = NULL;
ItemPtr gItemstock = NULL;

/* ------------------------------------------------------------
*	void init(): ó�� ���� �ʱ�ȭ �Լ�.
*/
void init()
{
#ifndef __STOREMGR_INIT__ //�ѹ��� �����ϱ� ���� ����
#define __STOREMGR_INIT__
	printf("   [i] ó�� ���� �غ� ��...\n");
	// ���� ���� �޸𸮿� ��ǰ ���� �޸� �Ҵ��ϱ�.
	gStoreinfo = malloc(sizeof(Storeinfo));
	gItemstock = malloc(sizeof(Item) * ITEM_COUNT_MAX);
	if (!gStoreinfo)// Storeinfo, gItemstock ������ �Ҵ� �ƴ��� Ȯ��
	{
		printf("   [!] ���� ���� �޸𸮸� �Ҵ��� �� �����ϴ�. ������ �� �����ϴ�.\n");
		exit(EXIT_FAILURE);
	}
	if (!gItemstock)
	{
		printf("   [!] ��ǰ ���� �޸𸮸� �Ҵ��� �� �����ϴ�. ������ �� �����ϴ�.\n");
		exit(EXIT_FAILURE);
	}
	if (access(STOREINFO_FILENAME, 0) != -1)
	{
		fpStoreinfo = fopen(STOREINFO_FILENAME, "rb+");	// ���� ���� ���� ����, rb+ ���.
		if (!fpStoreinfo)
		{
			printf("   [!] ���� ���� ���Ͽ� �׼��� �Ұ����մϴ�. ������ �� �����ϴ�.\n");
			exit(EXIT_FAILURE);
		}
		fread(gStoreinfo, sizeof(Storeinfo), 1, fpStoreinfo);
	}
	else
	{
		printf("   [!] ���� ���� ������ �������� �ʽ��ϴ�. �⺻ �������� ����մϴ�.\n");
		strcpy(gStoreinfo->name, "StoreManager ����");	// �⺻ ���� �̸� �Է�.
		strcpy(gStoreinfo->contact, "no data");			// �⺻ ����ó ���� �Է�.
		fpStoreinfo = fopen(STOREINFO_FILENAME, "wb+");	// ���� �����ͷ� ����, wb+ ���.
		fwrite(gStoreinfo, sizeof(Storeinfo), 1, fpStoreinfo);	// ���� �����Ϳ� ����.
	}
	if (access(ITEMDB_FILENAME, 0) != -1)
	{
		fpItemstock = fopen(ITEMDB_FILENAME, "rb+");	// ��ǰ ���� ���� ����, rb+ ���.
		if (!fpItemstock)
		{
			printf("   [!] ��ǰ DB ���Ͽ� �׼��� �Ұ����մϴ�. ������ �� �����ϴ�.\n");
			exit(EXIT_FAILURE);
		}
		// ������ �о�ͼ� �޸𸮿� �ø���.
		fread(gItemstock, sizeof(Item), ITEM_COUNT_MAX, fpItemstock);
	}
	else
	{
		printf("   [!] ��ǰ DB ������ �������� �ʽ��ϴ�. �⺻ �������� ����մϴ�.\n");
		// �⺻ ������ �߰�, �ϴ� �ƹ��͵� ���� ���·� ����.
		for (int i = 0; i < ITEM_COUNT_MAX; i++)
		{
			strcpy(gItemstock[i].name, "undefined");
			gItemstock[i].price = 0;
			gItemstock[i].quantity = 0;
			gItemstock[i].accumulated_quantity = 0;
			gItemstock[i].available = true;
		}
		fpItemstock = fopen(ITEMDB_FILENAME, "wb+");	// ���� �����ͷ� ����, wb+ ���.
		fwrite(gItemstock, sizeof(Item), ITEM_COUNT_MAX, fpItemstock);	// ���� �����Ϳ� ����.
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
	printf("  ������������������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("  ��  %s", title); set_cursor_pos(73, 1); printf("��\n");
	printf("  ������������������������������������������������������������������������������������������������������������������������������������������������\n");
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
	// ��ü ��ǰ ����� ��ȸ.
	printf("   %-10s  %-32s  %-10s  %-10s  %-10s  %-10s\n   ", "No.", "��ǰ��", "����", "���", "�����Ǹŷ�", "Ȱ��ȭ");
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
	printf("\n   �� %d�� �׸�\n", count);
}

void prompt_editmode(int index)
{
	while (1)
	{
		SetConsoleTitle(L"���� ���� ���: ���� ���� ���α׷�");
		char buffer[256] = { '\0' };
		sprintf(buffer, "���� ���� ��� �� (%d)%s", index, gItemstock[index].name);
		prompt_menu_header(buffer, 140, DEFAULT_MENU_LINES);
		printf("\n");
		
		printf("   %-32s  %-10s  %-10s  %-10s  %-10s\n   ",  "��ǰ��", "����", "���", "�����Ǹŷ�", "Ȱ��ȭ");
		for (int i = 0; i < 80; i++)
			printf("-");
		printf("\n   %-32s  %-10d  %-10d  %-10d  %-10s\n", gItemstock[index].name, gItemstock[index].price, gItemstock[index].quantity, gItemstock[index].accumulated_quantity, (gItemstock[index].available) ? "O" : "X");
		char menu[6][MENU_STRING_LEN] = { "��ǰ��", "����", "����", "�����Ǹ� �ʱ�ȭ", "Ȱ��ȭ ���", "������" };
		prompt_aligned_options(6, menu, 8);
		printf("\n\n   [i] Ű�� ������ �޴��� �����ϴ�.\n");
		int option = getch() - '0';
		if (option == 6)
			break;
		else if (option == 1)
		{
			// ��ǰ�� ���� â.
			printf("\n   [i] ���ο� ��ǰ���� �Է��Ͻʽÿ�(�ִ� %d��): ", ITEM_NAME_LEN_MAX - 1);
			char new_str[ITEM_NAME_LEN_MAX] = { '\0' };
			gets_s(new_str, ITEM_NAME_LEN_MAX);
			strcpy(gItemstock[index].name, new_str);
			write_itemstock();
			printf("   [i] ����Ǿ����ϴ�. ");
			pause(NULL);
		}
		else if (option == 2)
		{
			printf("\n   [i] ���ο� ������ �Է��Ͻʽÿ�: ");
			int new_price = 0;
			scanf("%d", &new_price);
			if (new_price < 0)
			{
				printf("   [!] 0 �̸��� ������ ������ �� �����ϴ�! ");
				pause(NULL);
				continue;
			}
			gItemstock[index].price = new_price;
			write_itemstock();
			printf("   [i] ����Ǿ����ϴ�. ");
			pause(NULL);
		}
		else if (option == 3)
		{
			printf("\n   [i] ���ο� ��� ������ �Է��Ͻʽÿ�: ");
			int new_quantity = 0;
			scanf("%d", &new_quantity);
			if (new_quantity < 0)
			{
				printf("   [!] 0 �̸��� ������ ������ �� �����ϴ�! ");
				pause(NULL);
				continue;
			}
			gItemstock[index].quantity = new_quantity;
			write_itemstock();
			printf("   [i] ����Ǿ����ϴ�. ");
			pause(NULL);
		}
		else if (option == 4)
		{
			printf("\n   [i] ���� ���� �Ǹŷ��� �ʱ�ȭ�Ͻðڽ��ϱ�?\n");
			printf("       ���� ���� �Ǹŷ��� %d(����: %d) �Դϴ�.\n", gItemstock[index].accumulated_quantity, gItemstock[index].accumulated_quantity * gItemstock[index].price);
			printf("       ����Ϸ��� 'yes'�� ��ҹ��� �����Ͽ� �Է��� �ֽʽÿ�: ");
			char confirm[256] = { '\0' };
			scanf("%s", confirm);
			if (strcmp(confirm, "yes") == 0)
			{
				gItemstock[index].accumulated_quantity = 0;
				write_itemstock();
				printf("   [i] ���� �Ǹŷ��� �ʱ�ȭ�Ǿ����ϴ�. ");
			}
			else
				printf("   [i] ��ҵǾ����ϴ�. ");
			pause(NULL);
		}
		else if (option == 5)
		{
			gItemstock[index].available = !(gItemstock[index].available);
			write_itemstock();
			printf("\n   [i] ");
			printf(gItemstock[index].available ? "Ȱ��ȭ�Ǿ����ϴ�. " : "��Ȱ��ȭ�Ǿ����ϴ�. ");
			pause(NULL);
		}
		else
		{
			printf("   [!] �ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է��ϼ���!\n");
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
		SetConsoleTitle(L"�Ǹ�: ���� ���� ���α׷�");
		prompt_menu_header("��ǰ �Ǹ�", 140, DEFAULT_MENU_LINES);
		printf("\n");
		prompt_itemlist(true);
		printf("   �Ǹ��� ��ǰ�� ��ȣ�� �Է��ϼ���(���: -1): ");
		int no, quantity;
		scanf("%d", &no);
		if (no == -1)
			break;
		else if (no < 0 || no > ITEM_COUNT_MAX - 1)
			continue;
		else if (!(gItemstock[no].available))
		{
			printf("   [!] ������ �׸��� ��Ȱ��ȭ ���¿��� �Ǹ��� �� �����ϴ�. ");
			pause(NULL);
			continue;
		}
		printf("   �Ǹ��� ������ �Է��ϼ���(���: -1): ");
		scanf("%d", &quantity);
		if (quantity == -1)
			break;
		else if (quantity < 1)
		{
			printf("   [!] �ּ� �� �� �̻��� �Ǹ��Ͽ��� �մϴ�! ");
			pause(NULL);
			continue;
		}
		else if (gItemstock[no].quantity - quantity < 0)
		{
			printf("   [!] ������ �׸�(%s)�� ���� ������ %d���̹Ƿ� %d���� �Ǹ��� �� �����ϴ�. ", gItemstock[no].name, gItemstock[no].quantity, quantity);
			pause(NULL);
			continue;
		}
		// �Ǹ�ó��.
		gItemstock[no].quantity -= quantity;
		gItemstock[no].accumulated_quantity += quantity;
		write_itemstock();
		printf("   [i] ������ �׸�(%s)�� %d�� �Ǹ��Ͽ����ϴ�(���� ���: %d��). ", gItemstock[no].name, quantity, gItemstock[no].quantity);
		pause(NULL);
		continue;
	}
}

void prompt_import()
{
	while (1)
	{
		SetConsoleTitle(L"�԰�: ���� ���� ���α׷�");
		prompt_menu_header("��ǰ �԰�", 140, DEFAULT_MENU_LINES);
		printf("\n");
		prompt_itemlist(true);
		printf("   �԰� ó���� ��ǰ�� ��ȣ�� �Է��ϼ���(���: -1): ");
		int no, quantity;
		scanf("%d", &no);
		if (no == -1)
			break;
		else if (no < 0 || no > ITEM_COUNT_MAX - 1)
			continue;
		else if (!(gItemstock[no].available))
		{
			printf("   [!] ������ �׸�(%s)�� ��Ȱ��ȭ ���¿��� �԰�ó���� �� �����ϴ�. ", gItemstock[no].name);
			pause(NULL);
			continue;
		}
		printf("   �԰� ������ �Է��ϼ���(���: -1): ");
		scanf("%d", &quantity);
		if (quantity == -1)
			break;
		else if (quantity < 1)
		{
			printf("   [!] �ּ� �� �� �̻��� �԰��Ͽ��� �մϴ�! ");
			pause(NULL);
			continue;
		}
		// �԰� ó��.
		gItemstock[no].quantity += quantity;
		write_itemstock();
		printf("   [i] ������ �׸�(%s)�� %d�� �԰��Ͽ����ϴ�(���� ���: %d��). ", gItemstock[no].name, quantity, gItemstock[no].quantity);
		pause(NULL);
		continue;
	}
}

void prompt_update_item()
{
	while (1)
	{
		SetConsoleTitle(L"��ǰ ���� ����: ���� ���� ���α׷�");
		prompt_menu_header("��ǰ ���� ����", 140, DEFAULT_MENU_LINES);
		printf("\n");
		prompt_itemlist(false);
		printf("   ������ ��ǰ�� �����ϼ���(���: -1): ");
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
	SetConsoleTitle(L"���� ���� ����: ���� ���� ���α׷�");
	prompt_menu_header("���� ���� ����", 140, DEFAULT_MENU_LINES);
	printf("\n");
	// gStoreinfo ����.
	printf("   ���� ���� �̸�: %s\n", gStoreinfo->name);
	printf("   ���� ���� ����ó ����: %s\n\n", gStoreinfo->contact);
	printf("   ������ ���� �̸��� �Է��� �ּ���: ");
	// �Է� �ޱ�.
	char new_name[STORE_NAME_LEN_MAX] = { '\0' };
	gets_s(new_name, STORE_NAME_LEN_MAX);
	printf("   ������ ���� ����ó ������ �Է��� �ּ���: ");
	char new_contact[STORE_COTACT_LEN_MAX] = { '\0' };
	gets_s(new_contact, STORE_COTACT_LEN_MAX);

	// ���� �� ����.
	// �켱 gStoreinfo �޸𸮿� �����Ѵ�.
	strcpy(gStoreinfo->name, new_name);
	strcpy(gStoreinfo->contact, new_contact);
	// ����� gStoreinfo �޸𸮿��� ���Ͽ��ٰ� �״�� �����Ѵ�.
	write_storeinfo();
	printf("   [i] ����Ǿ����ϴ�. ");
	pause(NULL);
	return;
}

void prompt_show_statistics()
{
	SetConsoleTitle(L"��� ����: ���� ���� ���α׷�");
	prompt_menu_header("���", 140, DEFAULT_MENU_LINES);
	printf("\n");
	prompt_itemlist(false);
	getchar();
}