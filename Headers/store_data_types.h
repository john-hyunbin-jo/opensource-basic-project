#pragma once

#define STORE_NAME_LEN_MAX 65				// 상점 이름의 최대 글자수: 64자
#define STORE_COTACT_LEN_MAX 81				// 상점 연락처의 최대 글자수: 81자
#define ITEM_NAME_LEN_MAX 33				// 상품 이름의 최대 글자수: 32자
#define ITEM_COUNT_MAX 24					// 등록 가능한 상품의 최대 개수: 25개

/* ----------------------------------------------------------------------
*	struct Storeinfo: 상점 기본정보 데이터 저장.
*		- (char*) name: 상점 이름
*		- (char*) contact: 연락처 정보
*/
typedef struct _Storeinfo* StoreinfoPtr;
typedef struct _Storeinfo {
	char name[STORE_NAME_LEN_MAX];
	char contact[STORE_COTACT_LEN_MAX];
} Storeinfo;

/* ----------------------------------------------------------------------
*	struct Item: 판매상품 데이터 정보 저장.
*		- (char*) name: 상품명
*		- (int) price: 판매가격
*		- (int) quantity: 재고 수량
*		- (long) accumulated_quantity: 누적 수량
*		- (Bool) available: 사용 가능성(표시여부)
*/
typedef struct _Item* ItemPtr;
typedef struct _Item {
	char name[ITEM_NAME_LEN_MAX];
	int price;
	int quantity;
	long accumulated_quantity;
	BOOL available;
} Item;