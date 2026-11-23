#include "EResource.h"
#include <iomanip>
using namespace std;
 
EResource::EResource(string id, string title, string author, int qty,
                     string format, double fileSize, string downloadURL)
    : LibraryResource(id, title, author, qty),
      format(format), fileSize(fileSize), downloadURL(downloadURL) {
}
 
void EResource::display() const {
    cout << "========================================" << endl;
    cout << "  类型：     " << getType() << endl;
    cout << "  编号：     " << id << endl;
    cout << "  名称：     " << title << endl;
    cout << "  作者：     " << author << endl;
    cout << "  格式：     " << format << endl;
    cout << fixed << setprecision(1);
    cout << "  文件大小： " << fileSize << " MB" << endl;
    cout << "  下载地址： " << downloadURL << endl;
    cout << "  馆藏位置： " << getLocation() << endl;
    cout << "  总数量：   " << totalQty << endl;
    cout << "  可借数量： " << availableQty << endl;
    cout << "========================================" << endl;
}
