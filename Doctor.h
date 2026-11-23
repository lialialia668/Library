// Doctor.h —— 博士（借阅者派生类）
#ifndef DOCTOR_H
#define DOCTOR_H
 
#include "Borrower.h"
 
class Doctor : public Borrower {
private:
    string supervisor;   // 导师
 
public:
    // 博士默认最多可借 20 本
    Doctor(string id, string name, string dept, string supervisor);
 
    string getSupervisor() const { return supervisor; }
    void setSupervisor(string s) { supervisor = s; }
 
    void   print()   const;   // 覆盖基类纯虚函数
    string getType() const;   // 覆盖基类纯虚函数
    int    borrowDays() const;   // 博士：可借 60 天
    double finePerDay() const;   // 博士：逾期 0.10 元/天
};
 
#endif
