// Doctor.cpp —— 博士实现
#include "Doctor.h"
using namespace std;
 
Doctor::Doctor(string id, string name, string dept, string supervisor)
    : Borrower(id, name, dept, 20), supervisor(supervisor) {
}
 
void Doctor::print() const {
    cout << "  [博士] 编号：" << id
         << "  姓名：" << name
         << "  院系：" << department
         << "  导师：" << supervisor
         << "  已借/上限：" << currentBorrow << "/" << maxBorrow << endl;
}
 
string Doctor::getType() const { return "博士"; }
int    Doctor::borrowDays() const { return 60; }
double Doctor::finePerDay() const { return 0.10; }
