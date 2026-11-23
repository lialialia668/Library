#include "LibraryResource.h"
using namespace std;
 
LibraryResource::LibraryResource(string id, string title, string author, int qty)
    : id(id), title(title), author(author), totalQty(qty), availableQty(qty) {
}
 
LibraryResource::~LibraryResource() {
}
 
// 借出：可借数量-1
bool LibraryResource::borrow() {
    if (availableQty <= 0) {
        cout << ">> 借阅失败：【" << title << "】已无可借数量" << endl;
        return false;
    }
    availableQty--;
    cout << ">> 借阅成功：【" << title << "】剩余可借 " << availableQty << " 本" << endl;
    return true;
}
 
// 归还：可借数量+1
bool LibraryResource::returnBack() {
    if (availableQty >= totalQty) {
        cout << ">> 归还失败：【" << title << "】可借数量已达上限" << endl;
        return false;
    }
    availableQty++;
    cout << ">> 归还成功：【" << title << "】剩余可借 " << availableQty << " 本" << endl;
    return true;
}
 
// 增加数量
void LibraryResource::increaseQty(int n) {
    totalQty += n;
    availableQty += n;
    cout << ">> 【" << title << "】增加 " << n << " 本，当前总量 " << totalQty << endl;
}
 
// 减少数量
bool LibraryResource::decreaseQty(int n) {
    if (n > availableQty) {
        cout << ">> 减少失败：可借数量不足（当前可借 " << availableQty << "）" << endl;
        return false;
    }
    totalQty -= n;
    availableQty -= n;
    cout << ">> 【" << title << "】减少 " << n << " 本，当前总量 " << totalQty << endl;
    return true;
}
