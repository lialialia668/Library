// Borrower.cpp —— 借阅者抽象基类实现
#include "Borrower.h"
using namespace std;

Borrower::Borrower(string sid, string n, string dept, int max)
    : id(sid), name(n), department(dept),
      maxBorrow(max), currentBorrow(0), password("") {
}

Borrower::~Borrower() {
}
