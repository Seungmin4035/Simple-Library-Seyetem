#ifndef BOOK_H
#define BOOK_H
#include "sub.h"

typedef struct book {
	char bookid[10];
	char title[25];
	char author[25];
	char publisher[20];
	int year;
	char state[10];
}Book;

void readFile(Book *pBook, int *pNum);
void menu(Book *pBooks, int *pNum);
void addBook(Book *pBooks, int *pNum);
void addBookFile(Book book);

int findBookById(Book *pBook, int num, char *getId);
void showBooks(Book *pBooks, int num);
void printBooksByReg(Book *pBooks, int num);
void printBooksByYear(Book *pBooks, int num);
void printBorrowedBooks(Book *pBooks, int num);
void sortByYear(Book *list, int num);

void searchBook(Book *pBooks, int num);
void searchByBookid(Book *pBooks, int num, int function);
void searchByBookid_len(Book *pBooks, int num, Book *book, int *pCount);
void searchByBooktitle(Book *pBooks, int num, int function);
void searchByAuthor(Book *pBooks, int num, int function);

void borrowBook(Book *pBooks, int num);
void changeState(Book *pBooks, int select);

void returnBook(Book *pBooks, int num);
int searchBybookid_forReturn(Book *pBooks, int num); //new

void writeFile(Book *pBooks, int select);  //new
void printBook(Book book);


#endif
