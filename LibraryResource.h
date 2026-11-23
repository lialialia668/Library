// LibraryResource.h —— 图书资源抽象基类
#ifndef LIBRARY_RESOURCE_H
#define LIBRARY_RESOURCE_H

#include <iostream>
#include <string>
using namespace std;

class LibraryResource {
protected:
    string id;           // 资源编号
    string title;        // 资源名称
    string author;       // 作者
    int totalQty;        // 总数量
    int availableQty;    // 可借数量

public:
    LibraryResource(string id, string title, string author, int qty);
    virtual ~LibraryResource();

    virtual void display() const = 0;           // 纯虚
    virtual string getType() const = 0;          // 纯虚

    virtual string getLocation()  const { return "无"; }
    virtual bool   isElectronic() const { return false; }

    bool borrow();
    bool returnBack();
    void increaseQty(int n);
    bool decreaseQty(int n);

    // getter
    string getId()           const { return id; }
    string getTitle()        const { return title; }
    string getAuthor()       const { return author; }
    int    getTotalQty()     const { return totalQty; }
    int    getAvailableQty() const { return availableQty; }

    // setter（从文件恢复 / 修改资源信息用）
    void setTotalQty(int t)      { totalQty = t; }
    void setAvailableQty(int a)  { availableQty = a; }
    void setTitle(string t)      { title = t; }
    void setAuthor(string a)     { author = a; }
};

#endif
