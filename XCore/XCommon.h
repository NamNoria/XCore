#ifndef XCOMMON_H
#define XCOMMON_H

extern "C" {
	extern char* GetLocalTime();

}

#define SIZE VCSIZE  // ����ϵͳ��SIZE

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
    DECLARE_POINTER_PROPERTY(InitName);          // �ļ�����
    DECLARE_POINTER_PROPERTY(Name);              // �ļ�����
    DECLARE_POINTER_PROPERTY(Path);              // �ļ�·��
    DECLARE_POINTER_PROPERTY(Type);              // �ļ�����
    DECLARE_POINTER_PROPERTY(Hash);              // �ļ���ϣ
    DECLARE_POINTER_PROPERTY(Extension);         // �ļ���չ��
    DECLARE_NORMAL_PROPERTY(TIME, tCreateTime);  // �ļ�����ʱ��
    DECLARE_NORMAL_PROPERTY(TIME, tModifyTime);  // �ļ��޸�ʱ��
    DECLARE_NORMAL_PROPERTY(SIZE, sSize);        // �ļ���С
};
#endif