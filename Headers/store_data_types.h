#pragma once

#define STORE_NAME_LEN_MAX 65				// ���� �̸��� �ִ� ���ڼ�: 64��
#define STORE_COTACT_LEN_MAX 81				// ���� ����ó�� �ִ� ���ڼ�: 81��
#define ITEM_NAME_LEN_MAX 33				// ��ǰ �̸��� �ִ� ���ڼ�: 32��
#define ITEM_COUNT_MAX 24					// ��� ������ ��ǰ�� �ִ� ����: 25��

/* ----------------------------------------------------------------------
*	struct Storeinfo: ���� �⺻���� ������ ����.
*		- (char*) name: ���� �̸�
*		- (char*) contact: ����ó ����
*/
typedef struct _Storeinfo* StoreinfoPtr;
typedef struct _Storeinfo {
	char name[STORE_NAME_LEN_MAX];
	char contact[STORE_COTACT_LEN_MAX];
} Storeinfo;

/* ----------------------------------------------------------------------
*	struct Item: �ǸŻ�ǰ ������ ���� ����.
*		- (char*) name: ��ǰ��
*		- (int) price: �ǸŰ���
*		- (int) quantity: ��� ����
*		- (long) accumulated_quantity: ���� ����
*		- (Bool) available: ��� ���ɼ�(ǥ�ÿ���)
*/
typedef struct _Item* ItemPtr;
typedef struct _Item {
	char name[ITEM_NAME_LEN_MAX];
	int price;
	int quantity;
	long accumulated_quantity;
	BOOL available;
} Item;