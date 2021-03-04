#ifndef BOOKSYSTEM_BOOK_H
#define BOOKSYSTEM_BOOK_H
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <sstream>
using namespace std;

//书的结构
struct Book
{
    int        id;          // 书号
    string     bookName;    // 书名
    string     author;      // 作者
    int        numNow;      // 现存
    int        repertory;   // 库存
    Book*      next;        // 图书信息链表形式
} ;

//借阅信息
struct Record
{
    string     userName;      // 借阅人
    int        bookId;        // 借阅的书号
    string     bookName;      // 借阅书名
    string     nowDay;        // 借阅日期
    int        dayNum;        // 借多少天
    Record*    next;
};


class System {
private:
    Book* book;
    Record* record;
    string filePath;
    int id;

private:
    string GetNowTime();
    string CalcDate(const char* cDateIn,int n);
    string QuerybookNameById(int id);
    void InsertRecord(string userName,int id,int day);
    void OperationFile();
    void ReadFile();
    void InsertBook(int id, string bookName,string author,int numNow,int repertory);
    void InsertRecord2(string userName,int bookId, string bookName,string nowDay,int dayNum);
public:
    System(string filePath)
        :book(NULL)
        ,record(NULL)
    {
        id = 1;
        this->filePath = filePath;
        ReadFile();
    }

    // 图书入库
    // 参数：书名、作者
    // 返回值：0成功 -1失败
    int BooksToLibrary(string bookName, string author, int num);

    // 借阅
    // 参数：借阅人、书号、借多少天
    // 返回值：0成功 -1 失败
    int BorrowBook(string userName, int id, int dayNum);

    // 归还图书
    // 参数：借阅人、书号
    // 返回值：0成功 -1 失败
    int ReturnBook(string username, int id);

    // 展示书信息
    void ShowBookInfo();
    // 展示借阅信息
    void ShowRecordInfo();

};

#endif //BOOKSYSTEM_BOOK_H
