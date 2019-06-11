#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "sub.h"

void printWarning(char *message) {
	gotoxy(1, 10);   printf("%s", message);   Sleep(1000);
	gotoxy(1, 10);   printf("\t\t\t\t\t\t\t\t\t");
}

//도서 출력 시 메뉴 레이아웃(이거랑 밑에 printBook()이랑 써먹으면 편할것 같아!!)
void printLayout()
{
	printf("%-9s", "청구기호");
	printf("%-21s", "서명");
	printf("%-19s", "저자");
	printf("%-15s", "발행사");
	printf("%-9s", "출판년도");
	printf("%-4s", "상태");
	printf("\n------------------------------------------------------------------------------\n");
}


//문자열 입력 수 예외 처리 함수
void putValue(char *pValue, int size, int x, int y)
{
	char str[100] = { 0, };
	while (1) {
		gotoxy(x, y);   printf("                                            ");   gotoxy(x, y);
		fgets(str, sizeof(str), stdin);
		str[strlen(str) - 1] = '\0';

		if (strlen(str) <= size)
			break;

		gotoxy(1, 10);
		if (size == 6) { printf("%d자 이하로 입력하세요!", size); }
		else { printf("%d자 이하로 입력하세요!", size / 2); }
		Sleep(500);
		gotoxy(1, 10);   printf("                           ");
	}

	strncpy(pValue, str, strlen(str) + 1);
}

//정수인지 확인
int isInteger(char * str, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if ((str[i] & 0x80) != 0 || !isdigit(str[i]))
			return 0;
	}
	return 1;
}

//커서 이동 함수
void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
