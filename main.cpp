#define _CRT_SECURE_NO_WARNINGS

#include "book.h"

void menu()
{
    cout << "---------图书管理系统------------" << endl;
    cout << "---------1: 采编入库------------" << endl;
    cout << "---------2: 借阅图书------------" << endl;
    cout << "---------3: 归还图书------------" << endl;
    cout << "---------4: 展示图书------------" << endl;
    cout << "---------5: 归还图书------------" << endl;
}


string& trim(string &s)
{
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

int main() {
    menu();
    int res = 0;
    string filePath = "./";
    System* system = new System(filePath);
    while (true)
    {
        char cmd[1];
        cout << "请输入命令:" << endl;
        cin >> cmd;
        if (cmd[0] != '1' && cmd[0] != '2' && cmd[0] != '3' && cmd[0] != '4' && cmd[0] != '5')
        {
            cout << cmd << "请输入正确的命令字" << endl;
            continue;
        }
        if (cmd[0] == '1') {
            string bookName;
            string author;
            string num;
            cout << "请输出书名、作者、购入数量" << endl;
            cin >> bookName >> author >> num;
            bookName = trim(bookName);
            author = trim(author);
            num = trim(num);
            res = system->BooksToLibrary(bookName, author, atoi(num.c_str()));
            if (res == 0)
                cout << bookName << " to library success " << endl;
            else
                cout << bookName << "to library failed " << endl;
        }else if (cmd[0] == '2') {
            string userName;
            string bookId;
            string dayNum;
            cout << "请输入借阅人、书号、借多少天" << endl;
            cin >> userName >> bookId >> dayNum;
            userName = trim(userName);
            bookId = trim(bookId);
            dayNum = trim(dayNum);
            res = system->BorrowBook(userName, atoi(bookId.c_str()), atoi(dayNum.c_str()));
            if (res == 0)
                cout << userName << " borrow book success " << endl;
            else
                cout << userName << " borrow book failed " << endl;
        }else if (cmd[0] == '3') {
            string userName;
            string bookId;
            cout << "请输入借阅人、书号" << endl;
            cin.clear();
            cin >> userName >> bookId;
            userName = trim(userName);
            bookId = trim(bookId);
            res = system->ReturnBook(userName, atoi(bookId.c_str()));
            if (res == 0)
                cout << userName << " return book success " << endl;
            else
                cout << userName << " return book failed " << endl;
        }else if (cmd[0] == '4'){
            system->ShowBookInfo();
        }else if (cmd[0] == '5'){
            system->ShowRecordInfo();
        }else {
            cout << "请输入正确的命令字" << endl;
        }
    }
    return 0;
}
