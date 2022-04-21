#include "Headers/c_essentials.h" 
#include "Headers/wincore.h" 
#include "Headers/store_core.h" 
#pragma warning (disable: 4996)	// Deprecated, ���� �Լ� ��� ��Ȱ��ȭ.
#pragma warning (disable: 6031) // ��ȯ�� ���� ��� ��Ȱ��ȭ.

int main(int argc, char** argv)
{
	init();
	// �ܼ� â �����: �޴� ȭ�� �ٽ� �׸���.
	while (1)
	{
		SetConsoleTitle(L"���� ���� ���α׷�");
		prompt_menu_header("���� ���� ���α׷�", DEFAULT_MENU_COLS, DEFAULT_MENU_LINES);
		printf("   @ %s (�� %s) ]\n", get_store_name(), get_store_contact());
		char menu_titles[6][MENU_STRING_LEN] = { "��ǰ �Ǹ�", "��ǰ �԰�", "��ǰ ������ ����", "���� ���� ����", "��� ����", "����" };
		prompt_aligned_options(6, menu_titles, 5);
		printf("\n\n");
		printf("   [i] Ű�� ������ �޴��� �����ϴ�.\n");
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
			// ���� ���� �ڵ� 0�� ��ȯ�� �ش�.
			exit(EXIT_SUCCESS);
		default:
			printf("\n\n   [!] �ùٸ��� ���� Ű�Դϴ�. �ٽ� �Է��ϼ���.");
			Sleep(1000);
			// IMPORTANT: �Ʒ� while������ Sleep ���� ���� ��� �Է¹��۸� ����. fflush(stdin)���δ� �Ⱥ������, ���� �������� ���� �����ϴ� ���� �� ����������.
			while (kbhit()) { getch(); }
			continue;
		}
	}
	return EXIT_FAILURE;
}