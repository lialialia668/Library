// Borrower.h —— 借阅者抽象基类
#ifndef BORROWER_H
#define BORROWER_H

#include <string>
#include <iostream>
using namespace std;

class Borrower {
protected:
    string id;
    string name;
    string department;
    int    maxBorrow;
    int    currentBorrow;
    string password;       // ★ 权限控制

public:
    Borrower(string id, string name, string dept, int max);
    virtual ~Borrower();

    // 获取
    string getId()            const { return id; }
    string getName()          const { return name; }
    string getDepartment()    const { return department; }
    int    getMaxBorrow()     const { return maxBorrow; }
    int    getCurrentBorrow() const { return currentBorrow; }
    string getPassword()      const { return password; }

    // 密码（权限控制）
    void   setPassword(string p) { password = p; }
    bool   verifyPassword(string p) const { return password == p; }

    // 修改
    void setId(string v)         { id = v; }
    void setName(string v)       { name = v; }
    void setDepartment(string v) { department = v; }
    void setMaxBorrow(int m)     { maxBorrow = m; }
    void setCurrentBorrow(int c) { currentBorrow = c; }   // 从文件恢复用

    bool canBorrow() const { return currentBorrow < maxBorrow; }
    void addBorrow()  { currentBorrow++; }
    void subBorrow()  { if (currentBorrow > 0) currentBorrow--; }

    // 纯虚
    virtual void   print()     const = 0;
    virtual string getType()   const = 0;
    virtual int    borrowDays()const = 0;
    virtual double finePerDay()const = 0;
};

#endif
