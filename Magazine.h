// Magazine.h —— 杂志（继承自 LibraryResource）
#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "LibraryResource.h"

class Magazine : public LibraryResource {
private:
    int    issueNo;      // 期号
    string period;       // 出版周期
    string location;     // 馆藏位置

public:
    Magazine(string id, string title, string author, int qty,
             int issueNo, string period, string location);

    void display() const override;
    string getType() const override     { return "杂志"; }
    string getLocation() const override { return location; }

    // getter + setter
    int    getIssueNo()    const { return issueNo; }
    string getPeriod()     const { return period; }
    string getMagLocation()const { return location; }

    void setIssueNo(int n)        { issueNo = n; }
    void setPeriod(string p)      { period = p; }
    void setMagLocation(string l) { location = l; }
};

#endif
