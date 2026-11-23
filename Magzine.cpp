#include "Magazine.h"
using namespace std;
 
Magazine::Magazine(string id, string title, string author, int qty,
                   int issueNo, string period,string location)
    : LibraryResource(id, title, author, qty),
      issueNo(issueNo), period(period),location(location){
}
 
void Magazine::display() const {
    cout << "========================================" << endl;
    cout << "  类型：     " << getType() << endl;
    cout << "  编号：     " << id << endl;
    cout << "  刊名：     " << title << endl;
    cout << "  主编：     " << author << endl;
    cout << "  期号：     第 " << issueNo << " 期" << endl;
    cout << "  出版周期： " << period << endl;
    cout << "  馆藏位置： " << location << endl;
    cout << "  总数量：   " << totalQty << endl;
    cout << "  可借数量： " << availableQty << endl;
    cout << "========================================" << endl;
}
