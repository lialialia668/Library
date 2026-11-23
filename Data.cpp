// Data.cpp —— 日期类实现
#include "Data.h"
#include <iostream>
using namespace std;

// ---- 类内静态工具函数 ----
bool Data::isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Data::daysInMonth(int y, int m) {
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeap(y)) return 29;
    return days[m];
}

// ---- 构造 / 析构 ----
Data::Data() : year(2026), month(1), day(1) {
}

Data::Data(int y, int m, int d) {
    setDate(y, m, d);
}

Data::~Data() {
}

void Data::setDate(int y, int m, int d) {
    if (m < 1) m = 1;
    if (m > 12) m = 12;
    if (d < 1) d = 1;
    if (d > daysInMonth(y, m)) d = daysInMonth(y, m);
    year = y; month = m; day = d;
}

void Data::print() const {
    cout << toString();
}

string Data::toString() const {
    return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
}

// ---- 文件内部辅助：把日期转成"天数序号"（以公元 1 年 1 月 1 日为起点） ----
static long countLeapBefore(int y) {
    long count = (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400;
    return count;
}

static long toDays(int y, int m, int d) {
    static const int daysInMonthArr[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    long days = (long)(y - 1) * 365 + countLeapBefore(y);
    for (int i = 1; i < m; i++) {
        days += daysInMonthArr[i];
    }
    if (m > 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) {
        days += 1;
    }
    days += d;
    return days;
}

int Data::daysBetween(const Data& other) const {
    return (int)(toDays(year, month, day) - toDays(other.year, other.month, other.day));
}

// ---- 文件内部辅助：把"天数序号"还原为年、月、日 ----
static void fromDays(long total, int& y, int& m, int& d) {
    int year = (int)(total / 366) + 1;
    while (toDays(year + 1, 1, 1) <= total) year++;
    while (toDays(year, 1, 1) > total) year--;
    int month = 1;
    while (month < 12 && toDays(year, month + 1, 1) <= total) month++;
    d = (int)(total - toDays(year, month, 1)) + 1;
    y = year;
    m = month;
}

Data Data::addDays(int n) const {
    long total = toDays(year, month, day) + n;
    int ny, nm, nd;
    fromDays(total, ny, nm, nd);
    return Data(ny, nm, nd);
}

bool Data::operator<(const Data& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Data::operator>=(const Data& other) const {
    return !(*this < other);
}

#include "Data.h"
