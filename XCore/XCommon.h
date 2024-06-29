#ifndef XCOMMON_H
#define XCOMMON_H

extern "C" {
	extern char* GetLocalTime();

}

#define SIZE VCSIZE  // 屏蔽系统的SIZE

typedef uint64_t TIME;
typedef uint64_t SIZE;


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef STRING_VERSION

    std::string calculateMD5(const std::string& input);
#endif

    char* calculateMD5(const char* input);
    void  printAlignedData(const char* label, const char* value);

#ifdef __cplusplus
}
#endif

#define DECLARE_NORMAL_PROPERTY(type, name) \
private:                                    \
    type m_##name;                          \
                                            \
public:                                     \
    type Get##name() const                  \
    {                                       \
        return m_##name;                    \
    }                                       \
    void Set##name(const type name)         \
    {                                       \
        this->m_##name = name;              \
        return;                             \
    }

#define DECLARE_POINTER_PROPERTY(name)                      \
private:                                                    \
    char *m_psz##name;                                      \
                                                            \
public:                                                     \
    const char *Get##name() const                           \
    {                                                       \
        return m_psz##name;                                 \
    }                                                       \
    bool Set##name(const char *name)                        \
    {                                                       \
        bool bSuccess = false;                              \
        int  i##name##Len = strlen(name);                   \
        this->m_psz##name = new char[i##name##Len + 1];     \
        if ( !this->m_psz##name )                           \
        {                                                   \
            return false;                                   \
        }                                                   \
        memset(this->m_psz##name, 0, i##name##Len + 1);     \
        strncpy(this->m_psz##name, name, i##name##Len + 1); \
        return bSuccess;                                    \
    }


class FileInterface
{
public:
    ~FileInterface()
    {
    }

    virtual bool del(const char* filePath) = 0;
    virtual bool copy(const char* dstPath, const char* srcPath) = 0;
    virtual bool rename(const char* newName, const char* oldName) = 0;
    virtual bool modifyCreateTime(const char* newTime, const char* filePath) = 0;
    virtual bool modifyAccessTime(const char* newTime, const char* filePath) = 0;
    virtual bool modifyChangeTime(const char* newTime, const char* filePath) = 0;
};

class FileManager : public FileInterface
{
public:
    bool del(const char* filePath) override;
    bool copy(const char* dstPath, const char* srcPath) override;
    bool rename(const char* newName, const char* oldName) override;
    bool modifyCreateTime(const char* newTime, const char* filePath) override;
    bool modifyAccessTime(const char* newTime, const char* filePath) override;
    bool modifyChangeTime(const char* newTime, const char* filePath) override;
};

class File

{
public:
    File();
    File(const char* name);
    ~File();
    File(const File& file);
    File& operator= (const File& file);
    bool  operator== (const File& file);

public:
    bool Rename(const char* pName);

private:
    DECLARE_POINTER_PROPERTY(InitName);          // 文件名称
    DECLARE_POINTER_PROPERTY(Name);              // 文件名称
    DECLARE_POINTER_PROPERTY(Path);              // 文件路径
    DECLARE_POINTER_PROPERTY(Type);              // 文件类型
    DECLARE_POINTER_PROPERTY(Hash);              // 文件哈希
    DECLARE_POINTER_PROPERTY(Extension);         // 文件扩展名
    DECLARE_NORMAL_PROPERTY(TIME, tCreateTime);  // 文件创建时间
    DECLARE_NORMAL_PROPERTY(TIME, tModifyTime);  // 文件修改时间
    DECLARE_NORMAL_PROPERTY(SIZE, sSize);        // 文件大小
};
#endif