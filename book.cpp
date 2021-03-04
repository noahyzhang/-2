#include "book.h"

void System::InsertBook(int id, string bookName,string author,int numNow,int repertory)
{
    Book* one = new Book;
    one->id = id;
    one->bookName = bookName;
    one->author = author;
    one->numNow = numNow;
    one->repertory = repertory;
    one->next = NULL;
    if (this->book == NULL)
    {
        this->book = one;
        this->id ++;
        return;
    }
    Book* tmp = this->book;
    Book* tmpPre = tmp;
    while(tmp != NULL)
    {
        if (tmp->bookName == bookName)
        {
            tmp->numNow += numNow;
            tmp->repertory += repertory;
            return;
        }
        if (tmp->next == NULL)
            tmpPre = tmp;
        tmp = tmp->next;
    }
    tmpPre->next = one;
    this->id++;
    return;
}

int System::BooksToLibrary(string bookName, string author,int num)
{
    Book* one = new Book;
    one->id = this->id;
    one->bookName = bookName;
    one->author = author;
    one->numNow = num;
    one->repertory = num;
    one->next = NULL;
    if (this->book == NULL)
    {
        this->book = one;
        this->id ++;
        this->OperationFile();
        return 0;
    }
    Book* tmp = this->book;
    Book* tmpPre = tmp;
    while(tmp != NULL)
    {
        if (tmp->bookName == bookName)
        {
            tmp->numNow += num;
            tmp->repertory += num;
            this->OperationFile();
            return 0;
        }
        if (tmp->next == NULL)
            tmpPre = tmp;
        tmp = tmp->next;
    }
    tmpPre->next = one;
    this->id++;
    this->OperationFile();
    return 0;
}

string System::QuerybookNameById(int id)
{
    Book* tmp = this->book;
    while (tmp != NULL)
    {
        if (tmp->id == id)
            return tmp->bookName;
        tmp = tmp->next;
    }
    return "";
}

string System::GetNowTime()
{
    time_t t = time(0);
    char tmp[32];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d",localtime(&t));
    return (string)tmp;
}

string System::CalcDate(const char *cDateIn, int n)
{
    struct tm tmin;
    struct tm *pTmIn = &tmin;
    struct tm tmout;
    struct tm *pTmOut = &tmout;

    //由于windows下没有strptime函数，所以可以使用scanf来格式化
    int year, month, day;
    sscanf(cDateIn, "%d-%d-%d", &year, &month, &day);
    pTmIn->tm_year = year - 1900;
    pTmIn->tm_mon = month - 1;
    pTmIn->tm_mday = day;
    pTmIn->tm_isdst = -1;

    /*将tm结构数据转换成1970年1月1日开始计算的秒数*/
    time_t _tm = mktime(pTmIn);

    /*计算需要增加或者减少天数对应的秒数，结果是最终日期对应1970年1月1日开始计算的秒数*/
    _tm += n * 60 * 60 * 24;

    /*将time_t的信息转化真实世界的时间日期表示，结果由结构tm返回*/
    pTmOut = localtime(&_tm);

    /*tm类型的时间转换。将tm按照%Y%m%d格式转化赋值到输出中*/
    char buf[32];
    memset(buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "%Y-%m-%d", pTmOut);
    char* cDateOut = NULL;
    strcpy(cDateOut, buf);
    return cDateOut;
}

void System::InsertRecord(string userName,int id,int day)
{
    Record* one = new Record;
    one->userName = userName;
    one->bookName = this->QuerybookNameById(id);
    one->bookId = id;
    one->dayNum = day;
    one->nowDay = this->GetNowTime();
    one->next = NULL;
    if (this->record == NULL)
    {
        this->record = one;
        return ;
    }
    Record* tmp = this->record;
    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = one;
}

void System::InsertRecord2(string userName,int bookId, string bookName,string nowDay, int dayNum)
{
//    cout << userName << "\t" << bookId << "\t" << bookName << "\t" << nowDay << "\t" << dayNum << endl;
    Record* one = new Record;
    one->userName = userName;
    one->bookName = bookName;
    one->bookId = bookId;
    one->dayNum = dayNum;
    one->nowDay = nowDay;
    one->next = NULL;
    if (this->record == NULL)
    {
        this->record = one;
        return ;
    }
    Record* tmp = this->record;
    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = one;
}

int System::BorrowBook(string userName, int id, int dayNum)
{
    // 判断此书是否存在
    Book* tmp = this->book;
    bool flag = false;
    while(tmp != NULL)
    {
        if (tmp->id == id)
        {
            flag = true;
            break;
        }
        tmp = tmp->next;
    }
    if (!flag)
    {
        cout << "this book not exist" << endl;
        return -1;
    }
    if (tmp->numNow > 0)
    {
        InsertRecord(userName,id,dayNum);
        tmp->numNow--;

    }else{
        cout << "this book numNow is 0, no borrow this book" << endl;
        return -2;
    }
    this->OperationFile();
    return 0;
}

int System::ReturnBook(string username, int id)
{
    Record* recordTmp = this->record;
    Record* recTmpPre = recordTmp;
    bool flag = false;
    while (recordTmp != NULL)
    {
        if (recordTmp->userName == username && recordTmp->bookId == id)
        {
            flag = true;
            break;
        }
        if (recordTmp->next == NULL)
            recTmpPre = recordTmp;
        recordTmp = recordTmp->next;
    }
    if (flag == true )
    {
        if (this->record == recordTmp)
        {
            this->record = recordTmp->next;
        }else if(recordTmp->next == NULL)
        {
            recTmpPre->next = NULL;
        }else {
            recTmpPre->next = recordTmp->next;
        }
        Book* booktmp = this->book;
        while (booktmp != NULL)
        {
            if (booktmp->id == id)
            {
                booktmp->numNow += 1;
                break;
            }
            booktmp = booktmp->next;
        }
        if (booktmp == NULL)
            return -1;
        this->OperationFile();
        return 0;
    }
    return -1;
}

void System::OperationFile()
{
    ofstream bookInfoFile;
    bookInfoFile.open(filePath+"book_info.txt", ios::out | ios::trunc);
    Book* booktmp = this->book;
    while (booktmp != NULL)
    {
        bookInfoFile << booktmp->id << "\t" << booktmp->bookName << "\t" << booktmp->author << "\t"
        << booktmp->numNow << "\t" << booktmp->repertory << endl;
        booktmp = booktmp->next;
    }
    bookInfoFile.close();
    ofstream recordFile;
    recordFile.open(filePath+"borrow.txt",ios::out | ios::trunc);
    Record* recordtmp = this->record;
    while(recordtmp != NULL)
    {
        recordFile << recordtmp->userName << "\t" << recordtmp->bookId << "\t" << recordtmp->bookName
        << "\t" << recordtmp->nowDay << "\t" << "\t" << recordtmp->dayNum << endl;
        recordtmp = recordtmp->next;
    }
    recordFile.close();
}

void System::ShowBookInfo()
{
    Book* tmp = this->book;
    while(tmp != NULL)
    {
        cout << tmp->id << "\t" << tmp->bookName << "\t" << tmp->author << "\t" << tmp->numNow << "\t" << tmp->repertory << endl;
        tmp = tmp->next;
    }
    cout << endl;
}

void System::ShowRecordInfo()
{
    Record* tmp = this->record;
    while(tmp != NULL)
    {
        cout << tmp->userName << "\t" <<  tmp->bookId << "\t" << tmp->bookName << "\t" << tmp->nowDay << "\t" << tmp->dayNum << endl;
        tmp = tmp->next;
    }
    cout << endl;
}

//vector<string> split(const string &str, const char pattern)
//{
//    vector<string> res;
//    stringstream input(str);   //读取str到字符串流中
//    string temp;
//    //使用getline函数从字符串流中读取,遇到分隔符时停止,和从cin中读取类似
//    //注意,getline默认是可以读取空格的
//    while(getline(input, temp, pattern))
//    {
//        res.push_back(temp);
//    }
//    return res;
//}

vector<string> split(string& str)
{
    vector<string> res;
    string tmp;
    for (int i = 0;i < str.size();i++)
    {
        if (str[i] != '\t')
        {
            tmp += str[i];
        }else if (tmp.size() != 0){
            res.push_back(tmp);
            tmp.clear();
        }
    }
    if (tmp.size() != 0)
    {
        res.push_back(tmp);
    }
    return res;
}

void System::ReadFile()
{
    ifstream read_file;
    read_file.open(filePath+"book_info.txt", ios::binary);

    string line;
    vector<string> vec;
    while(getline(read_file, line))
    {
        vec = split(line);
        this->InsertBook(atoi(vec[0].c_str()),vec[1],vec[2],atoi(vec[3].c_str()),atoi(vec[4].c_str()));
    }
    read_file.close();
    ifstream read_file2;
    read_file2.open(filePath+"borrow.txt",ios::binary);
    string line2;
    vector<string> vec2;
    while(getline(read_file2,line2))
    {
//        cout << "line2:" << line2 << endl;
        vec2 = split(line2);
//        for (int i = 0; i< vec.size();i++)
//        {
//            cout << vec2[i] << endl;
//        }
//        cout << vec2[0] << " " << atoi(vec2[1].c_str()) << " " << vec2[2] << " " << vec2[3] << " " << atoi(vec2[4].c_str()) << endl;
        this->InsertRecord2(vec2[0],atoi(vec2[1].c_str()),vec2[2],vec2[3],atoi(vec2[4].c_str()));
    }
}