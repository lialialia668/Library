// main.cpp —— 图书资源借阅管理系统主程序
#include "Library.h"
using namespace std;

int main() {
    Library lib;

    // 启动时尝试从文件恢复数据
    bool loaded = lib.start();

    // 如果没有历史数据，预置一批初始数据
    if (!loaded) {
        lib.addExisting(new Book("B001", "C++ Primer", "Stanley", 3,
            "机械工业出版社", "9787111557769", 858, "A区1排3架"));
        lib.addExisting(new Book("B002", "三体", "刘慈欣", 5,
            "人民出版社", "9787536692930", 302, "A区2排1架"));
        lib.addExisting(new Magazine("M001", "计算机学报", "编辑部", 2,
            2026, "月刊", "B区期刊架2"));
        lib.addExisting(new EResource("E001", "算法导论电子版", "Cormen", 999,
            "PDF", 52.3, "http://example.com/algo.pdf"));
        cout << ">> 已预置 4 条初始资源。" << endl;
    }

    cout << "\n========== 图书资源借阅管理系统启动 ==========" << endl;
    cout << "提示：借阅/归还/续借前请先『注册新账号(14)』或『登录(15)』。" << endl;

    int choice;
    while (true) {
        lib.showMenu();
        cin >> choice;

        switch (choice) {
        case 1:  lib.addResource();      break;
        case 2:  lib.increaseQty();      break;
        case 3:  lib.displayOne();       break;
        case 4:  lib.displayAll();       break;
        case 5:  lib.decreaseQty();      break;
        case 6:  lib.removeResource();   break;
        case 7:  lib.borrowResource();   break;
        case 8:  lib.returnResource();   break;
        case 9:  lib.renewResource();    break;
        case 10: lib.queryLocation();    break;
        case 11: lib.addBorrower();      break;
        case 12: lib.showBorrowers();    break;
        case 13: lib.modifyBorrower();   break;
        case 14: lib.modifyResource();   break;
        case 15: lib.showRecords();      break;
        case 16: lib.registerUser();     break;
        case 17: lib.login();            break;
        case 18: lib.logout();           break;
        case 0:
            lib.save();
            cout << ">> 感谢使用，再见！" << endl;
            return 0;
        default:
            cout << ">> 无效选项，请重新输入！" << endl;
        }
    }
    return 0;
}
