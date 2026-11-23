// Master.cpp —— 硕士实现
#include "Master.h"
using namespace std;
 
Master::Master(string id, string name, string dept, string researchArea)
    : Borrower(id, name, dept, 15), researchArea(researchArea) {
}
 
void Master::print() const {
    cout << "  [硕士] 编号：" << id
         << "  姓名：" << name
         << "  院系：" << department
         << "  研究方向：" << researchArea
         << "  已借/上限：" << currentBorrow << "/" << maxBorrow << endl;
}
 
string Master::getType() const { return "硕士"; }
int    Master::borrowDays() const { return 45; }
double Master::finePerDay() const { return 0.15; }
