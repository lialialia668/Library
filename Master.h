// Master.h —— 硕士（借阅者派生类）
#ifndef MASTER_H
#define MASTER_H
 
#include "Borrower.h"
 
class Master : public Borrower {
private:
    string researchArea;   // 研究方向
 
public:
    // 硕士默认最多可借 15 本
    Master(string id, string name, string dept, string researchArea);
 
    string getResearchArea() const { return researchArea; }
    void setResearchArea(string a) { researchArea = a; }
 
    void   print()   const;   // 覆盖基类纯虚函数
    string getType() const;   // 覆盖基类纯虚函数
    int    borrowDays() const;   // 硕士：可借 45 天
    double finePerDay() const;   // 硕士：逾期 0.15 元/天
};
 
#endif
