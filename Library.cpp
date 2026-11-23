// Library.cpp —— 图书馆管理类实现
#include "Library.h"
#include <iomanip>
#include <algorithm>
using namespace std;

// ---------------- 构造 / 析构 ----------------
Library::Library() {
}

Library::~Library() {
    for (auto* r : resources)
        delete r;
    for (auto* b : borrowers)
        delete b;
}

// 启动：尝试从文件恢复；失败则初始化空库
bool Library::start() {
    if (load()) {
        cout << ">> 已从文件恢复数据（资源 " << resources.size()
             << " 条，借阅者 " << borrowers.size()
             << " 人，记录 " << records.size() << " 条）" << endl;
        return true;
    }
    cout << ">> 无持久化数据，使用空库启动。" << endl;
    return false;
}

// ---------------- 当前登录用户 ----------------
Borrower* Library::getCurrentUser() {
    if (currentUserId.empty()) return nullptr;
    return findBorrowerById(currentUserId);
}

// ---------------- 辅助：日期、查找 ----------------
Data Library::readDate(string prompt) {
    int y, m, d;
    cout << prompt;
    cin >> y >> m >> d;
    return Data(y, m, d);
}

LibraryResource* Library::findById(string id) {
    auto it = std::find_if(resources.begin(), resources.end(),
        [&](LibraryResource* r) { return r != nullptr && r->getId() == id; });
    return it != resources.end() ? *it : nullptr;
}

Borrower* Library::findBorrowerById(string id) {
    auto it = std::find_if(borrowers.begin(), borrowers.end(),
        [&](Borrower* b) { return b != nullptr && b->getId() == id; });
    return it != borrowers.end() ? *it : nullptr;
}

// ==================== 1. 增加新的图书/资源 ====================
void Library::addResource() {
    int type;
    cout << "\n--- 选择资源类型 ---" << endl;
    cout << "  1. 一般图书" << endl;
    cout << "  2. 杂志" << endl;
    cout << "  3. 电子资源" << endl;
    cout << "请选择：";
    cin >> type;

    string id, title, author;
    int qty;
    cout << "请输入编号：";   cin >> id;
    cout << "请输入名称：";   cin >> title;
    cout << "请输入作者：";   cin >> author;
    cout << "请输入数量：";   cin >> qty;

    if (findById(id) != nullptr) {
        cout << ">> 编号已存在，添加失败！" << endl;
        return;
    }

    LibraryResource* res = nullptr;
    if (type == 1) {
        string publisher, isbn, location;
        int pages;
        cout << "请输入出版社：";        cin >> publisher;
        cout << "请输入ISBN：";          cin >> isbn;
        cout << "请输入页数：";          cin >> pages;
        cout << "请输入馆藏位置：";      cin >> location;
        res = new Book(id, title, author, qty, publisher, isbn, pages, location);
    } else if (type == 2) {
        int issueNo;
        string period, location;
        cout << "请输入期号：";          cin >> issueNo;
        cout << "请输入出版周期：";      cin >> period;
        cout << "请输入馆藏位置：";      cin >> location;
        res = new Magazine(id, title, author, qty, issueNo, period, location);
    } else if (type == 3) {
        string format, url;
        double fileSize;
        cout << "请输入格式（PDF/EPUB等）：";  cin >> format;
        cout << "请输入文件大小（MB）：";       cin >> fileSize;
        cout << "请输入下载地址：";             cin >> url;
        res = new EResource(id, title, author, qty, format, fileSize, url);
    } else {
        cout << ">> 类型无效！" << endl;
        return;
    }
    resources.push_back(res);
    cout << ">> 添加成功！" << endl;
    res->display();
}

// ==================== 2. 增加已有资源数量 ====================
void Library::increaseQty() {
    string id; int n;
    cout << "请输入资源编号：";  cin >> id;
    cout << "请输入增加数量：";  cin >> n;
    LibraryResource* res = findById(id);
    if (!res) { cout << ">> 未找到该资源！" << endl; return; }
    res->increaseQty(n);
}

// ==================== 3. 显示某一资源 ====================
void Library::displayOne() {
    string id;
    cout << "请输入资源编号：";  cin >> id;
    LibraryResource* res = findById(id);
    if (!res) { cout << ">> 未找到该资源！" << endl; return; }
    res->display();   // ★ 多态
}

// ==================== 4. 显示所有资源 ====================
void Library::displayAll() {
    if (resources.empty()) { cout << ">> 当前没有任何资源！" << endl; return; }
    cout << "\n===== 所有资源列表（共 " << resources.size() << " 条）=====" << endl;
    for (auto* r : resources)
        if (r) r->display();   // ★ 多态
}

// ==================== 5. 减少已有资源数量 ====================
void Library::decreaseQty() {
    string id; int n;
    cout << "请输入资源编号：";  cin >> id;
    cout << "请输入减少数量：";  cin >> n;
    LibraryResource* res = findById(id);
    if (!res) { cout << ">> 未找到该资源！" << endl; return; }
    res->decreaseQty(n);
}

// ==================== 6. 删除某一资源 ====================
void Library::removeResource() {
    string id;
    cout << "请输入要删除的资源编号：";  cin >> id;
    auto it = std::find_if(resources.begin(), resources.end(),
        [&](LibraryResource* r) { return r != nullptr && r->getId() == id; });//拿到迭代器
    if (it == resources.end()) { cout << ">> 未找到该资源！" << endl; return; }//先判空
    cout << ">> 正在删除：" << (*it)->getTitle() << endl;
    delete *it;//用it解引用成指针值再delete
    resources.erase(it);//需要用迭代器erase
    cout << ">> 删除成功！" << endl;
}

// ==================== 7. 借阅（需登录 + 本人） ====================
void Library::borrowResource() {
    // ---- 权限守卫 ----
    if (!isLoggedIn()) {
        cout << ">> 请先登录后再借阅！（请在菜单 15 登录 / 14 注册）" << endl;
        return;
    }
    Borrower* b = getCurrentUser();
    if (!b) { cout << ">> 当前登录账号无效，请重新登录。" << endl; return; }

    string id;
    cout << "\n当前借阅人：" << b->getName() << "（" << b->getId() << "）" << endl;

    if (!b->canBorrow()) {
        cout << ">> 借阅失败：已达最大可借数量 " << b->getMaxBorrow() << " 本" << endl;
        return;
    }

    cout << "请输入要借阅的资源编号：";  cin >> id;
    LibraryResource* res = findById(id);
    if (!res) { cout << ">> 未找到该资源！" << endl; return; }

    Data borrowDate = readDate("请输入借阅日期（年 月 日）：");
    bool hasDue = !res->isElectronic();
    Data dueDate = borrowDate;
    if (hasDue) dueDate = borrowDate.addDays(b->borrowDays());

    if (!res->borrow()) return;

    b->addBorrow();

    BorrowRecord rec;
    rec.borrowerId = b->getId();
    rec.resourceId = id;
    rec.borrowDate = borrowDate;
    rec.dueDate    = dueDate;
    rec.renewCount = 0;
    rec.hasDue     = hasDue;
    rec.active     = true;
    records.push_back(rec);

    cout << ">> 已记录：" << b->getType() << " " << b->getName()
         << " 借阅【" << res->getTitle() << "】";
    if (hasDue)
        cout << "，应还日期 " << dueDate.toString()
             << "（可借 " << b->borrowDays() << " 天）";
    else
        cout << "（电子资源，无应还日期）";
    cout << endl;
}

// ==================== 8. 归还（需登录 + 本人） ====================
void Library::returnResource() {
    if (!isLoggedIn()) {
        cout << ">> 请先登录后再归还！（菜单 15 登录）" << endl;
        return;
    }
    Borrower* b = getCurrentUser();
    if (!b) { cout << ">> 当前登录账号无效。" << endl; return; }

    string id;
    cout << "\n当前归还人：" << b->getName() << "（" << b->getId() << "）" << endl;
    cout << "请输入要归还的资源编号：";  cin >> id;

    auto recIt = std::find_if(records.begin(), records.end(),
        [&](const BorrowRecord& r) {
            return r.active && r.borrowerId == b->getId() && r.resourceId == id;
        });
    if (recIt == records.end()) {
        cout << ">> 未找到属于您的这条在借记录！" << endl;
        return;
    }

    LibraryResource* res = findById(id);
    if (!res) { cout << ">> 该资源已被删除！" << endl; return; }

    Data returnDate = readDate("请输入归还日期（年 月 日）：");
    if (!res->returnBack()) return;

    b->subBorrow();
    recIt->active = false;

    cout << ">> " << b->getName() << " 归还【" << res->getTitle() << "】" << endl;
    if (recIt->hasDue) {
        int usedDays = returnDate.daysBetween(recIt->borrowDate);
        int overdue  = returnDate.daysBetween(recIt->dueDate);
        cout << "   借阅日期：" << recIt->borrowDate.toString()
             << "  应还日期：" << recIt->dueDate.toString()
             << "  归还日期：" << returnDate.toString()
             << "  共借 " << usedDays << " 天" << endl;
        if (overdue > 0) {
            double fine = overdue * b->finePerDay();
            cout << ">> 逾期 " << overdue << " 天，应缴罚款:"
                 << fixed << setprecision(2) << fine
                 << "（" << b->finePerDay() << " 元/天）" << endl;
        } else {
            cout << ">> 按期归还，无罚款。" << endl;
        }
    } else {
        cout << "   电子资源，无借阅期限，不产生罚款。" << endl;
    }
}

// ==================== 9. 续借（需登录 + 本人） ====================
void Library::renewResource() {
    if (!isLoggedIn()) {
        cout << ">> 请先登录后再续借！（菜单 15 登录）" << endl;
        return;
    }
    Borrower* b = getCurrentUser();
    if (!b) { cout << ">> 当前登录账号无效。" << endl; return; }

    string id;
    cout << "\n当前续借人：" << b->getName() << "（" << b->getId() << "）" << endl;
    cout << "请输入要续借的资源编号：";  cin >> id;

    auto recIt = std::find_if(records.begin(), records.end(),
        [&](const BorrowRecord& r) {
            return r.active && r.borrowerId == b->getId() && r.resourceId == id;
        });
    if (recIt == records.end()) {
        cout << ">> 未找到属于您的在借记录！" << endl;
        return;
    }

    LibraryResource* res = findById(id);
    if (!res) { cout << ">> 该资源已被删除！" << endl; return; }

    if (!recIt->hasDue) {
        cout << ">> 电子资源无固定借期，无需续借。" << endl;
        return;
    }
    if (recIt->renewCount >= MAX_RENEW) {
        cout << ">> 续借失败：已达最大续借次数（" << MAX_RENEW << " 次）。" << endl;
        return;
    }

    Data renewDate = readDate("请输入续借日期（年 月 日）：");
    if (recIt->dueDate < renewDate) {
        cout << ">> 续借失败：已超过应还日期 " << recIt->dueDate.toString()
             << "，逾期后不能续借，请先归还。" << endl;
        return;
    }

    int days = b->borrowDays();
    recIt->dueDate = recIt->dueDate.addDays(days);
    recIt->renewCount++;
    cout << ">> 续借成功：应还日期延长 " << days << " 天，至 "
         << recIt->dueDate.toString()
         << "（第 " << recIt->renewCount << " 次续借）" << endl;
}

// ==================== 10. 查询馆藏位置 ====================
void Library::queryLocation() {
    string id;
    cout << "请输入资源编号：";  cin >> id;
    LibraryResource* res = findById(id);
    if (!res) { cout << ">> 未找到该资源！" << endl; return; }
    cout << ">> 【" << res->getTitle() << "】（" << res->getType() << "）" << endl;
    cout << "   馆藏位置：" << res->getLocation() << endl;
    cout << "   可借数量：" << res->getAvailableQty()
         << " / " << res->getTotalQty() << endl;
}

// ==================== 11. 添加借阅者（无密码版本，兼容旧菜单） ====================
void Library::addBorrower() {
    int type;
    cout << "\n--- 选择借阅者类型 ---" << endl;
    cout << "  1. 老师" << endl;
    cout << "  2. 本科生" << endl;
    cout << "  3. 硕士" << endl;
    cout << "  4. 博士" << endl;
    cout << "请选择：";
    cin >> type;

    string id, name, dept;
    cout << "请输入编号：";          cin >> id;
    if (findBorrowerById(id) != nullptr) {
        cout << ">> 该编号已存在！" << endl;
        return;
    }
    cout << "请输入姓名：";          cin >> name;
    cout << "请输入院系/部门：";     cin >> dept;

    Borrower* b = nullptr;
    if (type == 1) {
        string title; cout << "请输入职称："; cin >> title;
        b = new Teacher(id, name, dept, title);
    } else if (type == 2) {
        int grade; cout << "请输入年级："; cin >> grade;
        b = new Undergraduate(id, name, dept, grade);
    } else if (type == 3) {
        string area; cout << "请输入研究方向："; cin >> area;
        b = new Master(id, name, dept, area);
    } else if (type == 4) {
        string supervisor; cout << "请输入导师："; cin >> supervisor;
        b = new Doctor(id, name, dept, supervisor);
    } else {
        cout << ">> 类型无效！" << endl; return;
    }
    borrowers.push_back(b);
    cout << ">> 添加成功！（注意：此账号尚未设置密码，可直接通过菜单 15 登录）" << endl;
    b->print();   // ★ 多态
}

// ==================== 12. 查看所有借阅者 ====================
void Library::showBorrowers() {
    if (borrowers.empty()) { cout << ">> 暂无借阅者！" << endl; return; }
    cout << "\n===== 借阅者列表（共 " << borrowers.size() << " 人）=====" << endl;
    for (auto* b : borrowers)
        if (b) b->print();   // ★ 多态
}

// ==================== 13. 修改借阅人信息 ====================
void Library::modifyBorrower() {
    string id;
    cout << "请输入要修改的借阅人编号：";  cin >> id;
    Borrower* b = findBorrowerById(id);
    if (!b) { cout << ">> 未找到该借阅人！" << endl; return; }
 
    cout << "\n当前信息：" << endl;
    b->print();
    cout << "\n--- 选择要修改的字段 ---" << endl;
    cout << "  1. 姓名   2. 院系   3. 密码   4. 最大可借数" << endl;
    cout << "  5. 身份特有字段（职称/年级/研究方向/导师）" << endl;
    cout << "  0. 返回" << endl;
    cout << "请选择：";
    int opt; cin >> opt;
 
    switch (opt) {
    case 1: {
        string n; cout << "请输入新姓名："; cin >> n;
        b->setName(n); cout << ">> 姓名已修改为：" << b->getName() << endl;
        break;
    }
    case 2: {
        string d; cout << "请输入新院系："; cin >> d;
        b->setDepartment(d); cout << ">> 院系已修改为：" << b->getDepartment() << endl;
        break;
    }
    case 3: {
        string p; cout << "请输入新密码："; cin >> p;
        b->setPassword(p); cout << ">> 密码已修改。" << endl;
        break;
    }
    case 4: {
        int m; cout << "请输入新的最大可借数："; cin >> m;
        b->setMaxBorrow(m); cout << ">> 最大可借数已修改为：" << b->getMaxBorrow() << endl;
        break;
    }
    case 5: {
        string t = b->getType();
        if (t == "老师") {
            string title; cout << "请输入新职称："; cin >> title;
            dynamic_cast<Teacher*>(b)->setTitle(title);
        } else if (t == "本科生") {
            int g; cout << "请输入新年级："; cin >> g;
            dynamic_cast<Undergraduate*>(b)->setGrade(g);
        } else if (t == "硕士") {
            string area; cout << "请输入新研究方向："; cin >> area;
            dynamic_cast<Master*>(b)->setResearchArea(area);
        } else if (t == "博士") {
            string sup; cout << "请输入新导师："; cin >> sup;
            dynamic_cast<Doctor*>(b)->setSupervisor(sup);
        }
        cout << ">> 身份特有字段已修改。" << endl;
        break;
    }
    default: return;
    }
    cout << "\n修改后信息：";
    b->print();
}
 
// ==================== 14. 修改图书/资源信息 ====================
void Library::modifyResource() {
    string id;
    cout << "请输入要修改的资源编号：";  cin >> id;
    LibraryResource* r = findById(id);
    if (!r) { cout << ">> 未找到该资源！" << endl; return; }
 
    cout << "\n当前信息：" << endl;
    r->display();
    cout << "\n--- 选择要修改的字段 ---" << endl;
    cout << "  1. 名称   2. 作者   3. 总数量" << endl;
    cout << "  4. 类型特有字段（出版社/期号/格式等）" << endl;
    cout << "  0. 返回" << endl;
    cout << "请选择：";
    int opt; cin >> opt;
 
    switch (opt) {
    case 1: {
        string t; cout << "请输入新名称："; cin >> t;
        r->setTitle(t); cout << ">> 名称已修改为：" << r->getTitle() << endl;
        break;
    }
    case 2: {
        string a; cout << "请输入新作者："; cin >> a;
        r->setAuthor(a); cout << ">> 作者已修改为：" << r->getAuthor() << endl;
        break;
    }
    case 3: {
        int total; cout << "请输入新的总数量："; cin >> total;
        int diff = total - r->getTotalQty();
        r->setTotalQty(total);
        r->setAvailableQty(r->getAvailableQty() + diff);  // 同步调整可借数量
        cout << ">> 总数量已修改为 " << total
             << "（可借数量同步调整为 " << r->getAvailableQty() << "）" << endl;
        break;
    }
    case 4: {
        string t = r->getType();
        if (t == "一般图书") {
            Book* b = dynamic_cast<Book*>(r);
            string pub, isbn, loc; int pages;
            cout << "请输入新出版社："; cin >> pub; b->setPublisher(pub);
            cout << "请输入新ISBN：";   cin >> isbn; b->setIsbn(isbn);
            cout << "请输入新页数：";   cin >> pages; b->setPages(pages);
            cout << "请输入新馆藏位置："; cin >> loc; b->setBookLocation(loc);
        } else if (t == "杂志") {
            Magazine* m = dynamic_cast<Magazine*>(r);
            int issue; string period, loc;
            cout << "请输入新期号：";       cin >> issue; m->setIssueNo(issue);
            cout << "请输入新出版周期：";   cin >> period; m->setPeriod(period);
            cout << "请输入新馆藏位置：";   cin >> loc; m->setMagLocation(loc);
        } else if (t == "电子资源") {
            EResource* e = dynamic_cast<EResource*>(r);
            string fmt, url; double size;
            cout << "请输入新格式：";       cin >> fmt; e->setFormat(fmt);
            cout << "请输入新文件大小(MB)："; cin >> size; e->setFileSize(size);
            cout << "请输入新下载地址：";   cin >> url; e->setDownloadURL(url);
        }
        cout << ">> 类型特有字段已修改。" << endl;
        break;
    }
    default: return;
    }
    cout << "\n修改后信息：";
    r->display();
}
 
// ==================== 15. 查看借阅记录（登录者只看自己） ====================
void Library::showRecords() {
    if (records.empty()) { cout << ">> 暂无借阅记录！" << endl; return; }
    cout << "\n===== 借阅记录";
    if (isLoggedIn())
        cout << "（已登录，仅显示【" << currentUserId << "】的记录）";
    else
        cout << "（未登录，显示全部记录）";
    cout << " =====" << endl;

    size_t shown = 0;
    for (const auto& rec : records) {
        if (isLoggedIn() && rec.borrowerId != currentUserId) continue;
        shown++;
        cout << "  " << (rec.active ? "[在借] " : "[已还] ")
             << "资源：" << rec.resourceId
             << "  借阅人：" << rec.borrowerId
             << "  借于 " << rec.borrowDate.toString();
        if (rec.hasDue)
            cout << "  应还 " << rec.dueDate.toString();
        else
            cout << "  （电子资源无应还日期）";
        cout << "  续借 " << rec.renewCount << " 次" << endl;
    }
    if (shown == 0) cout << "  （无匹配记录）" << endl;
}

// ==================== 权限：注册 / 登录 / 登出 ====================

// 16. 注册：创建借阅者 + 设置密码
void Library::registerUser() {
    int type;
    cout << "\n===== 注册新借阅账号 =====" << endl;
    cout << "  1. 老师" << endl;
    cout << "  2. 本科生" << endl;
    cout << "  3. 硕士" << endl;
    cout << "  4. 博士" << endl;
    cout << "请选择身份：";
    cin >> type;

    string id, name, dept, password;
    cout << "请输入账号（编号）：";  cin >> id;
    if (findBorrowerById(id) != nullptr) {
        cout << ">> 账号已存在，请更换！" << endl;
        return;
    }
    cout << "请输入姓名：";          cin >> name;
    cout << "请输入院系/部门：";     cin >> dept;

    Borrower* b = nullptr;
    if (type == 1) {
        string title; cout << "请输入职称："; cin >> title;
        b = new Teacher(id, name, dept, title);
    } else if (type == 2) {
        int grade; cout << "请输入年级："; cin >> grade;
        b = new Undergraduate(id, name, dept, grade);
    } else if (type == 3) {
        string area; cout << "请输入研究方向："; cin >> area;
        b = new Master(id, name, dept, area);
    } else if (type == 4) {
        string supervisor; cout << "请输入导师："; cin >> supervisor;
        b = new Doctor(id, name, dept, supervisor);
    } else {
        cout << ">> 身份类型无效！" << endl; return;
    }
    cout << "请输入密码：";          cin >> password;
    b->setPassword(password);
    borrowers.push_back(b);
    currentUserId = id;   // 注册成功即自动登录
    cout << ">> 注册成功！已自动登录为【" << b->getType() << "  " << b->getName() << "】" << endl;
}

// 17. 登录：校验密码
void Library::login() {
    string id, pwd;
    cout << "请输入账号（编号）：";  cin >> id;
    cout << "请输入密码：";          cin >> pwd;
    Borrower* b = findBorrowerById(id);
    if (!b) { cout << ">> 账号不存在！" << endl; return; }
    if (!b->verifyPassword(pwd)) {
        cout << ">> 密码错误！" << endl; return;
    }
    currentUserId = id;
    cout << ">> 登录成功！欢迎，" << b->getType() << " " << b->getName() << endl;
}

// 18. 登出
void Library::logout() {
    if (currentUserId.empty()) {
        cout << ">> 当前未登录。" << endl; return;
    }
    cout << ">> 已退出登录。" << endl;
    currentUserId.clear();
}

// 返回当前登录状态字符串
string Library::showCurrentUser() const {
    if (currentUserId.empty()) return "未登录";
    return "已登录：" + currentUserId;
}

// ==================== 文件持久化（实验六 fstream） ====================

static const char* RES_FILE  = "resources.dat";
static const char* BOR_FILE  = "borrowers.dat";
static const char* REC_FILE  = "records.dat";

void Library::save() {
    // ---- 1. 资源 ----
    ofstream fr(RES_FILE);
    if (!fr) {
        cout << ">> [保存] 无法打开 " << RES_FILE << endl; return;
    }
    for (auto* r : resources) {
        fr << r->getType() << '\n';
        // 通用基类字段
        fr << r->getId()    << '\t'
           << r->getTitle() << '\t'
           << r->getAuthor()<< '\t'
           << r->getTotalQty() << '\t'
           << r->getAvailableQty() << '\t';
        // 派生类特有字段
        string t = r->getType();
        if (t == "一般图书") {
            Book* b = dynamic_cast<Book*>(r);
            fr << b->getPublisher() << '\t' << b->getIsbn() << '\t'
               << b->getPages() << '\t' << b->getLocation() << '\n';
        } else if (t == "杂志") {
            Magazine* m = dynamic_cast<Magazine*>(r);
            fr << m->getIssueNo() << '\t' << m->getPeriod() << '\t'
               << m->getLocation() << '\n';
        } else if (t == "电子资源") {
            EResource* e = dynamic_cast<EResource*>(r);
            fr << e->getFormat() << '\t' << e->getFileSize() << '\t'
               << e->getDownloadURL() << '\n';
        }
    }
    fr.close();

    // ---- 2. 借阅者 ----
    ofstream fb(BOR_FILE);
    if (!fb) {
        cout << ">> [保存] 无法打开 " << BOR_FILE << endl; return;
    }
    for (auto* b : borrowers) {
        fb << b->getType() << '\n';
        fb << b->getId()    << '\t'
           << b->getName()  << '\t'
           << b->getDepartment() << '\t'
           << b->getMaxBorrow()  << '\t'
           << b->getCurrentBorrow() << '\t'
           << b->getPassword() << '\t';
        string t = b->getType();
        if (t == "老师") {
            Teacher* tp = dynamic_cast<Teacher*>(b);
            fb << tp->getTitle() << '\n';
        } else if (t == "本科生") {
            Undergraduate* u = dynamic_cast<Undergraduate*>(b);
            fb << u->getGrade() << '\n';
        } else if (t == "硕士") {
            Master* m = dynamic_cast<Master*>(b);
            fb << m->getResearchArea() << '\n';
        } else if (t == "博士") {
            Doctor* d = dynamic_cast<Doctor*>(b);
            fb << d->getSupervisor() << '\n';
        }
    }
    fb.close();

    // ---- 3. 借阅记录 ----
    ofstream fre(REC_FILE);
    if (!fre) {
        cout << ">> [保存] 无法打开 " << REC_FILE << endl; return;
    }
    for (const auto& r : records) {
        fre << r.borrowerId << '\t' << r.resourceId << '\t'
            << r.borrowDate.getYear() << ' ' << r.borrowDate.getMonth() << ' ' << r.borrowDate.getDay() << '\t'
            << r.dueDate.getYear()    << ' ' << r.dueDate.getMonth()    << ' ' << r.dueDate.getDay()    << '\t'
            << r.renewCount << '\t'
            << (r.hasDue ? 1 : 0) << '\t'
            << (r.active  ? 1 : 0) << '\n';
    }
    fre.close();
    cout << ">> 数据已保存到文件。" << endl;
}

bool Library::load() {
    return loadResources() && loadBorrowers() && loadRecords();
}

bool Library::loadResources() {
    ifstream fr(RES_FILE);
    if (!fr) return false;   // 文件不存在就当作空库
    string type;
    while (getline(fr, type)) {
        if (type.empty()) continue;
        string id, title, author;
        int total, avail;
        fr >> id >> title >> author >> total >> avail;

        LibraryResource* r = nullptr;
        if (type == "一般图书") {
            string pub, isbn, loc; int pages;
            fr >> pub >> isbn >> pages >> loc;
            r = new Book(id, title, author, total, pub, isbn, pages, loc);
        } else if (type == "杂志") {
            int issue; string period, loc;
            fr >> issue >> period >> loc;
            r = new Magazine(id, title, author, total, issue, period, loc);
        } else if (type == "电子资源") {
            string fmt, url; double size;
            fr >> fmt >> size >> url;
            r = new EResource(id, title, author, total, fmt, size, url);
        }
        if (r) {
            r->setTotalQty(total);
            r->setAvailableQty(avail);   // 恢复借出/归还状态
            resources.push_back(r);
        }
    }
    fr.close();
    return true;
}

bool Library::loadBorrowers() {
    ifstream fb(BOR_FILE);
    if (!fb) return false;
    string type;
    while (getline(fb, type)) {
        if (type.empty()) continue;
        string id, name, dept, pwd;
        int maxB, curB;
        fb >> id >> name >> dept >> maxB >> curB >> pwd;

        Borrower* b = nullptr;
        if (type == "老师") {
            string title; fb >> title;
            b = new Teacher(id, name, dept, title);
        } else if (type == "本科生") {
            int grade; fb >> grade;
            b = new Undergraduate(id, name, dept, grade);
        } else if (type == "硕士") {
            string area; fb >> area;
            b = new Master(id, name, dept, area);
        } else if (type == "博士") {
            string sup; fb >> sup;
            b = new Doctor(id, name, dept, sup);
        }
        if (b) {
            b->setMaxBorrow(maxB);
            b->setCurrentBorrow(curB);    // 恢复借/还状态
            b->setPassword(pwd);
            borrowers.push_back(b);
        }
    }
    fb.close();
    return true;
}

bool Library::loadRecords() {
    ifstream fre(REC_FILE);
    if (!fre) return false;
    string bid, rid;
    int by, bm, bd, dy, dm, dd, renew, hasDue, active;
    while (fre >> bid >> rid >> by >> bm >> bd >> dy >> dm >> dd >> renew >> hasDue >> active) {
        BorrowRecord r;
        r.borrowerId = bid;
        r.resourceId = rid;
        r.borrowDate = Data(by, bm, bd);
        r.dueDate    = Data(dy, dm, dd);
        r.renewCount = renew;
        r.hasDue     = (hasDue != 0);
        r.active     = (active  != 0);
        records.push_back(r);
    }
    fre.close();
    return true;
}

// ==================== 菜单 ====================
void Library::showMenu() {
    cout << "\n========== 图书资源管理系统 ==========" << endl;
    cout << "  当前状态：" << showCurrentUser() << endl;
    cout << "--------------------------------------" << endl;
    cout << "  -- 资源管理 --" << endl;
    cout << "  1. 增加新的图书或资源" << endl;
    cout << "  2. 增加已有资源的数量" << endl;
    cout << "  3. 显示某一资源情况" << endl;
    cout << "  4. 显示所有资源情况" << endl;
    cout << "  5. 减少已有资源的数量" << endl;
    cout << "  6. 删除某一资源" << endl;
    cout << "  14. 修改图书/资源信息" << endl;
    cout << "  10. 查询馆藏位置" << endl;
    cout << "  -- 借阅（需登录） --" << endl;
    cout << "  7. 借阅图书或资源" << endl;
    cout << "  8. 归还图书或资源（含逾期罚款）" << endl;
    cout << "  9. 续借图书或资源" << endl;
    cout << "  15. 查看借阅记录（登录者仅显示本人）" << endl;
    cout << "  -- 借阅者管理 --" << endl;
    cout << "  11. 添加借阅者" << endl;
    cout << "  12. 查看所有借阅者" << endl;
    cout << "  13. 修改借阅人信息" << endl;
    cout << "  -- 权限 / 用户管理 --" << endl;
    cout << "  16. 注册新账号" << endl;
    cout << "  17. 登录" << endl;
    cout << "  18. 登出" << endl;
    cout << "  -- 退出 --" << endl;
    cout << "  0. 退出系统（自动保存）" << endl;
    cout << "======================================" << endl;
    cout << "请选择功能：";
}
