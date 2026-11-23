// Undergraduate.cpp —— 本科生实现
#include "Undergraduate.h"
using namespace std;
 
Undergraduate::Undergraduate(string id, string name, string dept, int grade)
    : Borrower(id, name, dept, 10), grade(grade) {
}
 
void Undergraduate::print() const {
    cout << "  [本科生] 编号：" << id
         << "  姓名：" << name
         << "  院系：" << department
         << "  年级：" << grade
         << "  已借/上限：" << currentBorrow << "/" << maxBorrow << endl;
}
 
string Undergraduate::getType() const { return "本科生"; }
int    Undergraduate::borrowDays() const { return 30; }
double Undergraduate::finePerDay() const { return 0.20; }
