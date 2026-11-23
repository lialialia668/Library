// EResource.h —— 电子资源（继承自 LibraryResource）
#ifndef ERESOURCE_H
#define ERESOURCE_H

#include "LibraryResource.h"

class EResource : public LibraryResource {
private:
    string format;        // 格式（PDF/EPUB等）
    double fileSize;      // 文件大小（MB）
    string downloadURL;   // 下载地址

public:
    EResource(string id, string title, string author, int qty,
              string format, double fileSize, string downloadURL);

    void display() const override;
    string getType() const override     { return "电子资源"; }
    string getLocation() const override { return "电子资源 · 无实体馆藏位置"; }
    virtual bool  isElectronic() const { return true; }

    // getter + setter
    string getFormat()      const { return format; }
    double getFileSize()    const { return fileSize; }
    string getDownloadURL() const { return downloadURL; }

    void setFormat(string f)      { format = f; }
    void setFileSize(double s)    { fileSize = s; }
    void setDownloadURL(string u) { downloadURL = u; }
};

#endif
