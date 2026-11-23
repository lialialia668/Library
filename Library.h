#include "Data.h"
#include <vector>
#include <string>
#include <fstream>

// 借阅记录：一笔借阅 = 一条记录
struct BorrowRecord {
    string borrowerId;   // 借阅人编号
    string resourceId;   // 资源编号
    Data   borrowDate;   // 借阅日期
    Data   dueDate;      // 应还日期（电子资源无意义）
    int    renewCount;   // 已续借次数
    bool   hasDue;       // true=有固定应还日期（非电子资源）
    bool   active;       // true=在借，false=已归还
};

class Library {
private:
    static const int MAX_RENEW = 1;     // 最多续借次数

    std::vector<LibraryResource*> resources;   // 资源：基类指针数组（多态）
    std::vector<Borrower*>        borrowers;   // 借阅者：基类指针数组（多态）
    std::vector<BorrowRecord>     records;     // 借阅记录

    // ---- 权限控制 ----
    string currentUserId;   // 当前登录用户编号；空串表示未登录

    // ---- 辅助函数 ----
    Data readDate(string prompt);
    bool isLoggedIn() const { return !currentUserId.empty(); }
    Borrower* getCurrentUser();   // 返回登录用户的 Borrower*；未登录返回 nullptr

    // ---- 文件持久化（实验六 fstream） ----
    bool load();                   // 从文件恢复数据，成功返回 true
    bool loadResources();
    bool loadBorrowers();
    bool loadRecords();

public:
    Library();
    ~Library();  // 析构时 delete 所有动态对象

    // ---- 文件持久化（实验六 fstream，main.cpp 退出前调用） ----
    void save();

    // 1. 增加新的图书/资源（动态创建）
    void addResource();

    // 辅助：直接添加已创建的资源指针（用于预置初始数据）
    void addExisting(LibraryResource* res) { resources.push_back(res); }

    // 2. 增加已有资源的数量
    void increaseQty();

    // 3. 显示某一资源情况（多态：根据类型调用不同 display）
    void displayOne();

    // 4. 显示所有资源（多态：遍历调用各自的 display）
    void displayAll();

    // 5. 减少已有资源的数量
    void decreaseQty();

    // 6. 删除某一资源（delete 动态对象）
    void removeResource();

    // 7. 借阅（权限：必须登录，且借阅人 = 当前用户）
    void borrowResource();

    // 8. 归还（权限：必须登录，且归还人 = 当前用户）
    void returnResource();

    // 9. 续借（权限：必须登录，且续借人 = 当前用户）
    void renewResource();

    // 10. 查询馆藏位置（非电子资源位置；电子资源给出提示）
    void queryLocation();

    // 11. 添加借阅者（老师 / 本科生 / 硕士 / 博士）
    void addBorrower();

    // 辅助：直接添加已创建的借阅者（用于预置初始数据）
    void addExistingBorrower(Borrower* b) { borrowers.push_back(b); }

    // 12. 查看所有借阅者（多态：遍历调用各自的 print）
    void showBorrowers();

    // 13. 修改借阅人信息
    void modifyBorrower();

    // 14. 修改图书/资源信息
    void modifyResource();

    // 15. 查看借阅记录（未登录看全部；已登录看自己的）
    void showRecords();

    // ---- 权限相关 ----
    void registerUser();          // 16. 注册（带密码）
    void login();                  // 17. 登录
    void logout();                 // 18. 登出
    string showCurrentUser() const;

    // 辅助：按编号查找资源
    LibraryResource* findById(string id);

    // 辅助：按编号查找借阅者
    Borrower* findBorrowerById(string id);

    // 辅助：显示菜单
    void showMenu();

    // 辅助：启动时调用 load()
    bool start();
};

#endif
