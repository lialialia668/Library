# 图书资源借阅管理系统

基于 C++ 面向对象思想实现的控制台图书借阅管理系统，支持图书 / 杂志 / 电子资源三类馆藏与教师 / 本科生 / 硕士 / 博士四类借阅者，具备借阅、归还、续借、逾期罚款与文件持久化功能。

> 面向对象程序设计课程设计项目。

## 功能特性

- **资源管理**：新增、入库、出库、删除、修改资源；按编号查询单个资源、显示全部馆藏
- **位置查询**：查询图书 / 杂志的架位，电子资源返回下载地址
- **借阅者管理**：注册、登录、注销、资料修改、列表显示
- **借阅业务**：借阅、归还、续借，自动校验在借数量与借阅上限
- **逾期罚款**：按应还日期计算逾期天数，费率随借阅者身份不同而不同
- **数据持久化**：退出时写入三个 `.dat` 文件，启动时自动恢复

## 技术要点

| 知识点 | 应用位置 |
|---|---|
| 继承与抽象基类 | `LibraryResource` → `Book` / `Magazine` / `EResource`；`Borrower` → `Teacher` / `Undergraduate` / `Master` / `Doctor` |
| 虚函数与多态 | `getType()`、`print()`、`maxDays()`、`finePerDay()` 等通过基类指针动态绑定 |
| 虚析构 | 基类析构声明为 `virtual`，保证 `delete` 基类指针时派生类正确释放 |
| 向下转型 | `dynamic_cast` 访问派生类特有字段（出版社、职称、年级等） |
| STL | `vector` 管理对象集合，`find_if` + lambda 实现按条件查找 |
| 运算符重载 | `Data::operator<` / `operator>=` 支持日期先后比较 |
| 日期算法 | `toDays` / `fromDays` 将日期映射为天数序号，简化加减与间隔计算 |
| 文件流 | `ifstream` / `ofstream` 完成对象序列化与反序列化 |

## 目录结构

```
.
├── main.cpp                    程序入口、菜单循环
├── Library.h / .cpp            系统核心：业务逻辑、菜单、持久化
├── LibraryResource.h / .cpp    资源抽象基类
├── Book.h / .cpp               一般图书
├── Magazine.h / .cpp           杂志
├── EResource.h / .cpp          电子资源
├── Borrower.h / .cpp           借阅者抽象基类
├── Teacher.h / .cpp            教师
├── Undergraduate.h / .cpp      本科生
├── Master.h / .cpp             硕士
├── Doctor.h / .cpp             博士
├── Data.h / .cpp               日期类
├── library_classdiagram.puml   PlantUML 类图源码
├── lib2.0.dev                  Dev-C++ 工程文件
└── 图书馆借阅管理系统_设计报告.docx
```

## 编译与运行

### 方式一：Dev-C++（推荐）

1. 用 Dev-C++ 打开 `lib2.0.dev`；
2. 按 **F11**（编译运行）；
3. 首次运行若无历史数据，会自动预置 4 条初始资源。

### 方式二：命令行（MinGW / g++）

```bash
g++ -std=c++17 -o library main.cpp Library.cpp LibraryResource.cpp \
    Book.cpp Magazine.cpp EResource.cpp \
    Borrower.cpp Teacher.cpp Undergraduate.cpp Master.cpp Doctor.cpp \
    Data.cpp
./library        # Windows 下为 library.exe
```

## 使用说明

1. 菜单 **16** 注册新账号（选择身份：老师 / 本科生 / 硕士 / 博士）；
2. 菜单 **17** 登录；
3. 登录后可执行 **7 借阅 / 8 归还 / 9 续借**；
4. 菜单 **0** 退出，**只有正常退出才会保存数据**，直接关闭窗口会丢失本次操作。

不同身份的借阅规则：

| 身份 | 最大可借 | 借期（实体资源） | 罚款费率 |
|---|---|---|---|
| 老师 | 10 本 | 60 天 | 0.10 元/天 |
| 本科生 | 5 本 | 30 天 | 0.20 元/天 |
| 硕士 | 8 本 | 45 天 | 0.15 元/天 |
| 博士 | 10 本 | 90 天 | 0.10 元/天 |

电子资源无固定借期，不产生罚款。

## 数据文件

程序运行后会在**当前工作目录**生成三个文本文件（已加入 `.gitignore`，不上传）：

| 文件 | 内容 |
|---|---|
| `resources.dat` | 馆藏资源及库存 |
| `borrowers.dat` | 借阅者账号、密码、已借数量 |
| `records.dat` | 借阅记录、应还日期、在借状态 |

删除这三个文件后重新启动，即回到初始空库状态。

## 类图

`library_classdiagram.puml` 为 PlantUML 源码，可用 VS Code 的 PlantUML 插件或 [PlantUML 在线服务](https://www.plantuml.com/plantuml) 渲染。

## 已知不足与改进方向

- 密码以明文保存，后续可改为哈希存储；
- 仅在菜单 0 退出时保存，异常关闭会丢数据，可在每次增删改后立即落盘；
- `cin >>` 对非法输入缺少校验，可能导致输入流进入失败状态；
- 资源字段含空格时文件解析会错位，可改为 `getline` + 分隔符解析。
