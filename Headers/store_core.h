#pragma once
#define MENU_STRING_LEN	19					// �޴��� �̸��� �ִ� 18����.
char* get_store_name();
char* get_store_contact();
void init();
void prompt_menu_header(char*, int, int);
void prompt_aligned_options(int, char**, int);
void prompt_sell();
void prompt_import();
void prompt_update_item();
void prompt_update_storeinfo();
void prompt_show_statistics();