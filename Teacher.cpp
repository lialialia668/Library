// Teacher.cpp —— 老师实现
#include "Teacher.h"
using namespace std;
 
// 显式调用基类 Borrower 的构造函数
Teacher::Teacher(string id, string name, string dept, string title)
    : Borrower(id, name, dept, 30), title(title) {
}
 
void Teacher::print() const {
    cout << "  [老师] 编号：" << id
         << "  姓名：" << name
         << "  部门：" << department
         << "  职称：" << title
         << "  已借/上限：" << currentBorrow << "/" << maxBorrow << endl;
}
 
string Teacher::getType() const { return "老师"; }
int    Teacher::borrowDays() const { return 60; }
double Teacher::finePerDay() const { return 0.10; }
