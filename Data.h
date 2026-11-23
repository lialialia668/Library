// Data.h —— 简单日期类，用于借阅日期、应还日期与逾期判断
#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
using namespace std;

class Data {
private:
    int year, month, day;

    // 判断闰年
    static bool isLeap(int y);
    // 返回某月的天数
    static int daysInMonth(int y, int m);

public:
    Data();                              // 默认构造：2026-01-01
    Data(int y, int m, int d);           // 重载构造
    ~Data();                             // 析构

    // 获取
    int getYear()  const { return year; }
    int getMonth() const { return month; }
    int getDay()   const { return day; }

    // 修改
    void setDate(int y, int m, int d);

    // 输出
    void print() const;

    // 转为 "YYYY-MM-DD" 字符串，便于拼接输出
    string toString() const;

    // 计算两个日期相差的天数（this - other），正数表示 this 更晚
    int daysBetween(const Data& other) const;

    // 返回本日期之后 n 天的日期（n 可为负），用于计算应还日期
    Data addDays(int n) const;

    // 比较
    bool operator<(const Data& other)  const;
    bool operator>=(const Data& other) const;
};

#endif
