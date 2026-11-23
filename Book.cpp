#include"Book.h"
using namespace std;
Book::Book(string id, string title, string author, int qty,
           string publisher, string isbn, int pages,string location)
    : LibraryResource(id, title, author, qty),   // 调用基类构造
      publisher(publisher), isbn(isbn), pages(pages),location(location) {
      }
      void Book::display() const {
    cout << "========================================" << endl;
    cout << "  类型：     " << getType() << endl;
    cout << "  编号：     " << id << endl;
    cout << "  书名：     " << title << endl;
    cout << "  作者：     " << author << endl;
    cout << "  出版社：   " << publisher << endl;
    cout << "  ISBN：     " << isbn << endl;
    cout << "  页数：     " << pages << " 页" << endl;
    cout << "  馆藏位置： " <<location<<endl;
    cout << "  总数量：   " << totalQty << endl;
    cout << "  可借数量： " << availableQty << endl;
    cout << "========================================" << endl;
}
