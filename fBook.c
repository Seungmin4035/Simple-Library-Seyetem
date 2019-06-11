#define _CRT_SECURE_NO_WARNINGS
#define MAX 50
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <windows.h>
#include "fBook.h"



//파일 읽기 함수
void readFile(Book *pBooks, int *pNum)
{
	FILE *ofp;
	char *filename = "book.bin";
	int fr;

	if (_access(filename, 0) != 0) {
		return;
	}

	ofp = fopen(filename, "rb");
	if (ofp == NULL) {
		printf("파일 열기 오류!");
		return;
	}

	fseek(ofp, 0, SEEK_END);
	*pNum = ftell(ofp) / sizeof(Book);

	rewind(ofp);
	fr = fread(pBooks, sizeof(Book), *pNum, ofp);
	if (fr != *pNum) {
		printf("도서 정보를 읽지 못했습니다!");
	}
	fclose(ofp);
}

//초기 메뉴 선택
void menu(Book *pBooks, int *pNum)
{
	char sChoice[10] = { 0, };
	int choice = 0;

	while (1) {
		system("cls");
		printf("도서관리 program\n\n");
		printf("1. 도서등록\n");
		printf("2. 도서현황\n");
		printf("3. 도서검색\n");
		printf("4. 도서대출\n");
		printf("5. 도서반납\n");
		printf("6. 종료\n\n");

		printf("번호를 입력하고 Enter > ");
		scanf("%s", sChoice);
		while (getchar() != '\n');

		if (!isInteger(sChoice, strlen(sChoice))) {
			printf("번호를 입력하세요!");      Sleep(500);
			continue;
		}

		choice = atoi(sChoice);

		if (choice == 6) {
			printf("\n도서관리 program을 종료합니다.\n\n\n");
			break;
		}

		switch (choice) {
		case 1:
			addBook(pBooks, pNum);
			break;
		case 2:
			showBooks(pBooks, *pNum);
			break;
		case 3:
			searchBook(pBooks, *pNum);
			break;
		case 4:
			borrowBook(pBooks, *pNum);
			break;
		case 5:
			returnBook(pBooks, *pNum);
			break;
		default:
			printf("없는 번호입니다! 다시 입력하세요.");   Sleep(500);
			break;
		}
	}
}

//도서 등록
void addBook(Book *pBooks, int *pNum)
{
	Book book;
	char sYear[10] = { 0, };
	char choice = 0;
	int i, ck = 0;

	while (1) {
		if (*pNum == MAX) {
			printf("\n도서 저장 공간이 모두 찼습니다.");   Sleep(500);
			break;
		}

		system("cls");
		printf("도서 등록\n\n");
		printf("청구기호 입력  (최대 영문 6자)  >\n");
		printf("도서명 입력    (최대 한글 10자) >\n");
		printf("제1 저자명 입력(최대 한글 9자)  >\n");
		printf("발행사 입력    (최대 한글 7자)  >\n");
		printf("출판년도 입력  (정수 4자)       >\n");

		//위치(35, 3);
		while (1) {
			ck = 0;
			putValue(book.bookid, 6, 35, 3);
			for (i = 0; i < 3; i++) {
				if ((book.bookid[i] & 0x80) != 0 || !isdigit(book.bookid[i])) {
					ck++;
					break;
				}
			}
			if (ck != 0) {
				printWarning("앞 세글자는 한국십진분류포에 따라 작성하세요.");
				continue;
			}

			if (findBookById(pBooks, *pNum, book.bookid) == -1)
				break;

			printWarning("같은 청구번호가 존재합니다! 다시 입력하세요.");
		}

		//위치(35, 4);
		putValue(book.title, 20, 35, 4);

		//위치(35, 5);
		putValue(book.author, 18, 35, 5);

		//위치(35, 6);
		putValue(book.publisher, 14, 35, 6);

		while (1) {
			gotoxy(35, 7);   printf("                           ");   gotoxy(35, 7);
			scanf("%s", sYear);
			while (getchar() != '\n');

			if (isInteger(sYear, strlen(sYear))) {
				book.year = atoi(sYear);
				if (book.year < 10000)
					break;
			}
			printWarning("정수 4자를 입력하세요!");
		}

		strcpy(book.state, "정상");

		(*pNum)++;
		pBooks[*pNum - 1] = book;

		//파일 저장 여부
		addBookFile(book);

		//추가 등록 여부
		while (1) {
			printf("\n\n도서를 추가로 등록하시겠습니까?(예: y  / 아니요: n) > ");
			choice = getchar();
			while (getchar() != '\n');

			if (choice == 'y' || choice == 'n')
				break;

			printf("\ny 또는 n을 입력하세요.\n");
		}

		if (choice == 'n')
			break;
	}
}

//도서 탐색 후 도서의 인덱스값 반환
int findBookById(Book *pBook, int num, char * getId)
{
	int i;
	for (i = 0; i < num; i++) {
		if (strcmp(pBook[i].bookid, getId) == 0) {
			return i;
		}
	}
	return -1;
}

//파일에 도서 저장
void addBookFile(Book book)
{
	FILE *afp;
	int fw;
	char choice;

	afp = fopen("book.bin", "ab");
	if (afp == NULL) {
		printf("파일 열기 실패!\n");
		return;
	}

	while (1) {
		printf("\n파일에 저장하시겠습니까?(예: y / 아니오: n) > ");
		choice = getchar();
		while (getchar() != '\n');

		if (choice == 'y') {
			fw = fwrite(&book, sizeof(Book), 1, afp);
			if (fw != 1) {
				printf("도서 저장 실패!");
			}
			break;
		}
		else if (choice == 'n')
			break;
		else
			printf("\ny 또는 n을 입력하세요.\n");
	}
	fclose(afp);
}


//현재 도서 출력 함수
void showBooks(Book *pBooks, int num)
{
	char sChoice[10] = { 0, };
	int choice = 0;

	while (1) {
		system("cls");
		printf("도서 현황\n\n");
		printf("1. 도서등록\n");
		printf("2. 출판년도\n");
		printf("3. 대출도서\n");
		printf("4. 이전 메뉴로\n\n");

		printf("번호를 입력하고 Enter > ");
		scanf("%s", sChoice);
		while (getchar() != '\n');

		if (!isInteger(sChoice, strlen(sChoice))) {
			printf("번호를 입력하세요!");      Sleep(500);
			continue;
		}

		choice = atoi(sChoice);

		if (choice == 4)
			break;

		switch (choice) {
		case 1:
			printBooksByReg(pBooks, num);
			break;
		case 2:
			printBooksByYear(pBooks, num);
			break;
		case 3:
			printBorrowedBooks(pBooks, num);
			break;
		default:
			printf("없는 번호입니다! 다시 입력하세요.\n");      Sleep(500);
			break;
		}
	}
}

//등록 순으로 현재 도서 출력
void printBooksByReg(Book *pBooks, int num)
{
	int i;
	system("cls");
	printf("등록 도서 현황: 총 %d권\n\n", num);
	printLayout();

	for (i = 0; i < num; i++) {
		printBook(pBooks[i]);
	}

	printf("\nEnter키를 누르면 이전 메뉴로 돌아감...");
	while (getchar() != '\n');
}

//년도 순으로 현재 도서 출력
void printBooksByYear(Book *pBooks, int num)
{
	int i;
	Book *books = (Book *)malloc(sizeof(Book) * num);
	if (books == NULL) {
		printf("메모리 할당 실패!");
		return;
	}

	memcpy(books, pBooks, sizeof(Book)*num);
	sortByYear(books, num);

	system("cls");
	printf("등록 도서 현황(출판년도 기준): 총 %d권\n\n", num);
	printLayout();

	for (i = 0; i < num; i++) {
		printBook(books[i]);
	}

	printf("\nEnter키를 누르면 이전 메뉴로 돌아감...");
	while (getchar() != '\n');
	free(books);
}

//대출 도서 출력
void printBorrowedBooks(Book *pBooks, int num)
{
	int brwBookNum = 0;
	int i;

	system("cls");
	printf("등록 도서 현황: 총 %d권\n\n", num);
	printLayout();

	for (i = 0; i < num; i++) {
		if (strcmp(pBooks[i].state, "대출") == 0) {
			printBook(pBooks[i]);
			brwBookNum++;
		}
	}
	printf("\n총 대출 도서: %d권\n\n", brwBookNum);
	printf("\nEnter키를 누르면 이전 메뉴로 돌아감...");
	while (getchar() != '\n');
}

//년도 순으로 정렬 함수
void sortByYear(Book * list, int num)
{
	int i, j;
	Book tmp;

	for (i = 0; i < num; i++) {
		for (j = 0; j < num - i - 1; j++) {
			if (list[j].year > list[j + 1].year) {
				tmp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = tmp;
			}
		}
	}
}

//도서 검색 함수
void searchBook(Book *pBooks, int num)
{
	char sChoice[10] = { 0, };
	int choice = 0;

	while (1) {
		system("cls");
		printf("도서 검색\n\n");
		printf("검색할 도서의 항목 선택\n\n");
		printf("1. 청구기호 입력(최대 영문 6자리)\n");
		printf("2. 도서명입력(최대 한글 10자리)\n");
		printf("3. 제1 저자명 입력(최대 한글 9자)\n");
		printf("4. 이전 메뉴로\n\n");

		printf("번호를 입력하고 Enter > ");
		scanf("%s", sChoice);
		while (getchar() != '\n');

		if (!isInteger(sChoice, strlen(sChoice))) {
			printf("번호를 입력하세요!");      Sleep(500);
			continue;
		}

		choice = atoi(sChoice);

		if (choice == 4)
			break;

		switch (choice) {
		case 1:
			searchByBookid(pBooks, num, 1);
			break;
		case 2:
			searchByBooktitle(pBooks, num, 1);
			break;
		case 3:
			searchByAuthor(pBooks, num, 1);
			break;
		default:
			printf("없는 번호입니다! 다시 입력하세요.\n");      Sleep(500);
			break;
		}
	}
}

//청구기호로 도서 검색 및 대출할 도서 검색
void searchByBookid(Book *pBooks, int num, int function)
{
	Book result;
	int count = 0;

	system("cls");
	if (function == 1) {
		printf("검색할 도서의 청구기호를 입력하고 Enter\n");
		printf("청구기호 입력 (최대 6자리) >");
		putValue(result.bookid, 6, 29, 2);

		printf("\n");
		printLayout();
		searchByBookid_len(pBooks, num, &result, &count);
	}
	else if (function == 2) {
		printf("대출할 도서의 청구기호를 입력하고 Enter\n");
		printf("청구기호 입력 (최대 6자리) >");
		putValue(result.bookid, 6, 29, 2);

		printf("\n");
		printLayout();

		for (int i = 0; i < num; i++) {
			if (strcmp(result.bookid, pBooks[i].bookid) == 0) {
				count++;
				printBook(pBooks[i]);
				changeState(pBooks, i);
				break;
			}
		}
	}

	if (count == 0) {
		printf("해당 도서가 존재하지 않습니다.\n\n");
	}

	printf("\nEnter키를 누르면 이전 메뉴로 돌아감...");
	while (getchar() != '\n');
}

//청구기호로 책 검색(한국십진표 기준으로도 검색 가능)
void searchByBookid_len(Book *pBooks, int num, Book *book, int *pCount)
{
	if (strlen(book->bookid) == 1) {
		for (int i = 0; i < num; i++) {
			if (pBooks[i].bookid[0] == book->bookid[0]) {
				printBook(pBooks[i]);
				*pCount = *pCount + 1;
			}
		}

	}
	else if (strlen(book->bookid) == 3) {
		for (int i = 0; i < num; i++) {
			if (strncmp(book->bookid, pBooks[i].bookid, 3) == 0) {
				printBook(pBooks[i]);
				*pCount = *pCount + 1;
			}
		}
	}
	else {
		for (int i = 0; i < num; i++) {
			if (strcmp(book->bookid, pBooks[i].bookid) == 0) {
				printBook(pBooks[i]);
				*pCount = *pCount + 1;
			}
		}
	}
}

//제목으로 도서 검색 및 대출할 도서 검색
void searchByBooktitle(Book *pBooks, int num, int function)
{
	Book result;
	int count = 0;

	system("cls");
	if (function == 1) {
		printf("검색할 도서의 도서명을 입력하고 Enter\n");
	}
	else if (function == 2) {
		printf("대출할 도서의 도서명을 입력하고 Enter\n");
	}
	printf("도서명입력 (최대 한글 10자리) >");
	putValue(result.title, 20, 32, 2);

	printf("\n");
	printLayout();

	for (int i = 0; i < num; i++) {
		if (strcmp(result.title, pBooks[i].title) == 0) {
			printBook(pBooks[i]);
			count++;
			if (function == 2) {
				changeState(pBooks, i);
				break;
			}

		}
	}

	if (count == 0) {
		printf("해당 도서가 존재하지 않습니다.\n\n");
	}

	printf("\nEnter키를 누르면 이전 메뉴로 돌아감...");
	while (getchar() != '\n');
}

//저자로 도서 검색 및 대출할 도서 검색
void searchByAuthor(Book *pBooks, int num, int function)
{
	Book result;
	int count = 0;
	int borrow = 0;

	system("cls");
	if (function == 1) {
		printf("검색할 도서의 제1 저자명을 입력하고 Enter\n");
	}
	else if (function == 2) {
		printf("대출할 도서의 제1 저자명을 입력하고 Enter\n");
	}
	printf("제1 저자명 입력 (최대 한글 9자리) >");
	putValue(result.author, 18, 36, 2);

	printf("\n");
	printLayout();

	for (int i = 0; i < num; i++) {
		if (strcmp(result.author, pBooks[i].author) == 0) {
			printBook(pBooks[i]);
			count++;
			if (function == 2) {
				changeState(pBooks, i);
				break;
			}
		}
	}

	if (count == 0) {
		printf("해당 도서가 존재하지 않습니다.\n\n");
	}

	printf("\nEnter키를 누르면 이전 메뉴로 돌아감...");
	while (getchar() != '\n');
}

//도서 대출 함수
void borrowBook(Book *pBooks, int num)
{
	char sChoice[10] = { 0, };
	int choice = 0;

	while (1) {
		system("cls");
		printf("도서 대출\n\n");
		printf("대출할 도서의 항목 선택\n\n");
		printf("1. 청구기호 입력(최대 영문 6자리)\n");
		printf("2. 도서명입력(최대 한글 10자리)\n");
		printf("3. 제1 저자명 입력(최대 한글 9자)\n");
		printf("4. 이전 메뉴로\n\n");

		printf("번호를 입력하고 Enter > ");
		scanf("%s", sChoice);
		while (getchar() != '\n');

		if (!isInteger(sChoice, strlen(sChoice))) {
			printf("번호를 입력하세요!");      Sleep(500);
			continue;
		}

		choice = atoi(sChoice);

		if (choice == 4)
			break;

		switch (choice) {
		case 1:
			searchByBookid(pBooks, num, 2);

			break;
		case 2:
			searchByBooktitle(pBooks, num, 2);

			break;
		case 3:
			searchByAuthor(pBooks, num, 2);

			break;
		default:
			printf("없는 번호입니다! 다시 입력하세요.\n");      Sleep(500);
			break;
		}
	}
}

//도서 상태 변경 함수
void changeState(Book *pBooks, int select) {

	int borrow = 0;
	int check;

	check = (strcmp(pBooks[select].state, "대출") == 0) ? 0 : 1;
	//책이 이미 대출 되어있는 상태인지 확인. 대출이 되어있으면 0, 아니면 1

	if (check == 0) {
		printf("\n\n해당 도서는 이미 대출 상태라 대출이 불가능합니다!\n");
		return;
	}

	while (1) {


		printf("\n위의 도서를 대출하려면 1을, 취소는 0을 입력> ");
		scanf("%d", &borrow);
		while (getchar() != '\n');

		if (borrow == 1) {
			strcpy(pBooks[select].state, "대출");
			writeFile(pBooks, select);
			printf("\n");
			printLayout();
			printBook(pBooks[select]);
			break;
		}
		else if (borrow == 0) {
			printf("대출 취소!\n");
			break;
		}
		else {
			printf("없는 번호입니다. 다시 입력하세요!\n");
		}

	}
}

//책을 반납 시켜주는 함수 
void returnBook(Book *pBooks, int num)
{
	int choice;
	int i = searchBybookid_forReturn(pBooks, num);

	while (1) {

		printf("\n위의 도서를 반납하려면 1을, 취소는 0을 입력>");
		scanf("%d", &choice);
		while (getchar() != '\n');

		if (choice == 1) {
			printf("\n");
			printLayout();
			printBook(pBooks[i]);
			strcpy(pBooks[i].state, "정상");
			writeFile(pBooks, i);
			printBook(pBooks[i]);
			break;
		}
		else if (choice == 0) {
			printf("반납 취소!\n");
			break;
		}
		else {
			printf("없는 번호입니다. 다시 입력하세요!\n");
		}
	}

	printf("\nEnter키를 누르면 이전 메뉴로 돌아감...");
	while (getchar() != '\n');
}

//반납할 도서를 청구기호로 찾아주는 함수
int searchBybookid_forReturn(Book *pBooks, int num)
{
	Book result;
	int count = 0;
	int i = 0;

	while (1) {
		system("cls");
		printf("도서 반납\n\n");
		printf("반납할 도서의 청구기호(6자리)입력 후 Enter>");
		putValue(result.bookid, 6, 45, 3);

		for (i = 0; i < num; i++) {
			if (strcmp(result.bookid, pBooks[i].bookid) == 0 && strcmp(pBooks[i].state, "대출") == 0) {
				printf("\n");
				printLayout();
				printBook(pBooks[i]);
				count++;
				return i;
			}
			else if (strcmp(result.bookid, pBooks[i].bookid) == 0) {
				printf("\n\n해당 도서는 대출이 된 상태가 아니라서 반납이 불가능합니다. 다시 입력해주세요"), Sleep(600);
				count++;
				break;
			}
		}

		if (count == 0) {
			printf("\n\n해당 청구 기호의 도서가 존재하지 않습니다. 다시 입력해주세요!\n"), Sleep(600);
		}

	}

	return 0;
}

//대출과 반납 상태가 바뀌면 파일 안에 바뀐 상태를 저장해준다.
void writeFile(Book *pBooks, int select)
{
	FILE * ofp = fopen("book.bin", "r+");
	if (ofp == NULL) {
		printf("파일 열기 실패!");
		return;
	}

	fseek(ofp, sizeof(Book)*select, SEEK_SET);
	fwrite(&pBooks[select], sizeof(Book), 1, ofp);
	fclose(ofp);


}



//한 권의 도서 출력 레이아웃
void printBook(Book book)
{
	printf("%-9s", book.bookid);
	printf("%-21s", book.title);
	printf("%-19s", book.author);
	printf("%-15s", book.publisher);
	printf("%-9d", book.year);
	printf("%-4s", book.state);
	printf("\n");
}


