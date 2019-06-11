#define MAX 50

#include <stdio.h>
#include "fBook.h"


int main(void)
{
	int bookNum = 0;
	Book books[MAX];
	memset(books, 0, sizeof(books));

	readFile(books, &bookNum);
	menu(books, &bookNum);

	return 0;
}
