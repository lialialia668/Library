// Teacher.h —— 老师（借阅者派生类）
#ifndef TEACHER_H
#define TEACHER_H
 
#include "Borrower.h"
 
class Teacher : public Borrower {
private:
    string title;   // 职称（教授 / 副教授 / 讲师等）
 
public:
    // 老师默认最多可借 30 本
    Teacher(string id, string name, string dept, string title);
 
    string getTitle() const { return title; }
    void setTitle(string t) { title = t; }
 
    void   print()   const;   // 覆盖基类纯虚函数
    string getType() const;   // 覆盖基类纯虚函数
    int    borrowDays() const;   // 老师：可借 60 天
    double finePerDay() const;   // 老师：逾期 0.10 元/天
};
 
#endif
