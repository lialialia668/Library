// Book.h —— 一般图书（继承自 LibraryResource）
#ifndef BOOK_H
#define BOOK_H

#include "LibraryResource.h"

class Book : public LibraryResource {
private:
    string publisher;    // 出版社
    string isbn;         // ISBN号
    int    pages;        // 页数
    string location;     // 馆藏位置（书架号）

public:
    Book(string id, string title, string author, int qty,
         string publisher, string isbn, int pages, string location);

    // 重写纯虚函数（多态）
    void display() const override;
    string getType() const override     { return "一般图书"; }
    string getLocation() const override { return location; }

    // Book 特有 getter + setter（修改资源信息用）
    string getPublisher()   const { return publisher; }
    string getIsbn()        const { return isbn; }
    int    getPages()       const { return pages; }
    string getBookLocation()const { return location; }

    void setPublisher(string p)   { publisher = p; }
    void setIsbn(string i)        { isbn = i; }
    void setPages(int p)          { pages = p; }
    void setBookLocation(string l){ location = l; }
};

#endif
