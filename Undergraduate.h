// Undergraduate.h —— 本科生（借阅者派生类）
#ifndef UNDERGRADUATE_H
#define UNDERGRADUATE_H
 
#include "Borrower.h"
 
class Undergraduate : public Borrower {
private:
    int grade;   // 年级
 
public:
    // 本科生默认最多可借 10 本
    Undergraduate(string id, string name, string dept, int grade);
 
    int  getGrade() const { return grade; }
    void setGrade(int g) { grade = g; }
 
    void   print()   const;   // 覆盖基类纯虚函数
    string getType() const;   // 覆盖基类纯虚函数
    int    borrowDays() const;   // 本科生：可借 30 天
    double finePerDay() const;   // 本科生：逾期 0.20 元/天
};
 
#endif
