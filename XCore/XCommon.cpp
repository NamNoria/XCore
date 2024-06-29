#include <iostream>
#include <fstream>
#include <ctime>
#include <fcntl.h>

#include "XCommon.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#define O_WRONLY _O_WRONLY
#define O_CREAT _O_CREAT
#define O_TRUNC _O_TRUNC
#define open _open
#define close _close
#else
#include <unistd.h>
#include <fcntl.h>
#endif

char* GetLocalTime()
{
	std::time_t now = std::time(nullptr);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	char buffer[0x100] = {0};
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	char* pTime = (char*)new char[strlen(buffer) + 1];
	if (!pTime)
	{
		return nullptr;
	}
	memset(pTime, 0, strlen(buffer) + 1);
	strncpy(pTime, buffer, strlen(buffer));
	return pTime;
}


#ifdef MEMORY_LEAK
std::map< void *, size_t > allocatedMemory;

void *operator new (size_t size)
{
    void *pMemory = std::malloc(size);

    if ( !pMemory )
    {
        throw std::bad_alloc();
    }

    allocatedMemory[pMemory] = size;

    // LOG(DEBUG) << "alloc memory" << size << "at address" << pMemory;
    std::cout << "alloc memory " << size << " Bit at address 0x" << pMemory << std::endl;
    return pMemory;
}

void operator delete (void *pMemory)
{
    if ( !pMemory )
    {
        return;
    }

    std::map< void *, size_t >::iterator it = allocatedMemory.find(pMemory);

    if ( it != allocatedMemory.end() )
    {
        std::cout << "Deallocating " << it->second << " bytes at address " << pMemory << std::endl;
        allocatedMemory.erase(it);
        std::free(pMemory);
        pMemory = NULL;
    }
    else
    {
        std::cerr << "Attempted to delete non-allocated memory at address " << pMemory << std::endl;
    }

    return;
}

#endif

bool FileManager::del(const char* filePath)
{
    bool bSuccess = false;

    if (!filePath)
    {
        return bSuccess;
    }

    if (std::remove(filePath) != 0)
    {
        //LOG(ERROR) << "delete file:" << filePath << " failed, err = " << strerror(errno);
        bSuccess = false;
    }
    else
    {
#ifdef TEST
        LOG(DEBUG) << "delete file:" << filePath << " succeed";
#endif
        bSuccess = true;
    }

    return bSuccess;
}

bool FileManager::copy(const char* dstPath, const char* srcPath)
{
    bool bSuccess = false;

    if ((!dstPath) || (!srcPath))
    {
        return bSuccess;
    }

    std::ifstream srcFile(srcPath, std::ios::binary);
    if (!srcFile.is_open())
    {
        //LOG(ERROR) << "open src file: " << srcPath << " failed, err = " << strerror(errno);
        return bSuccess;
    }

    std::ofstream dstFile(dstPath, std::ios::binary);
    if (!dstFile.is_open())
    {
        //LOG(ERROR) << "open dst file: " << srcPath << " failed, err = " << strerror(errno);
        srcFile.close();
        return bSuccess;
    }

    dstFile << srcFile.rdbuf();

    if (dstFile.fail())
    {
        //LOG(ERROR) << "copy file:" << srcPath << " to " << dstPath << " failed, err = " << strerror(errno);
        bSuccess = false;
    }
    else
    {
#ifdef TEST
        LOG(DEBUG) << "copy file:" << srcPath << " to " << dstPath << " succeed";
#endif
        bSuccess = true;
    }

    srcFile.close();
    dstFile.close();

    return bSuccess;
}

bool FileManager::rename(const char* newPath, const char* oldPath)
{
    bool bSuccess = false;

    if ((!newPath) || (!oldPath))
    {
        return bSuccess;
    }

    if (rename(newPath, oldPath) != 0)
    {
        //LOG(ERROR) << "rename file:" << oldPath << "to" << newPath << " failed, err = " << strerror(errno);
        bSuccess = false;
    }
    else
    {
#ifdef TEST
        LOG(DEBUG) << "rename file:" << oldPath << "to" << newPath << " success";
#endif
        bSuccess = true;
    }

    return bSuccess;
}

bool FileManager::modifyCreateTime(const char* newTime, const char* filePath)
{
    bool bSuccess = false;

    if (!filePath)
    {
        return bSuccess;
    }

    int fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        //LOG(ERROR) << "open:" << filePath << " failed, err = " << strerror(errno);
    }

    if (futimens(fd, nullptr) != 0)
    {
        std::cerr << "修改文件创建时间失败: " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }

    return bSuccess;
}

bool FileManager::modifyAccessTime(const char* newTime, const char* filePath)
{
    bool bSuccess = false;



    return bSuccess;
}

bool FileManager::modifyChangeTime(const char* newTime, const char* filePath)
{
    bool bSuccess = false;



    return bSuccess;
}

File::File()
{
    m_pszName = NULL;
    m_pszPath = NULL;
    m_pszType = NULL;
    m_pszHash = NULL;
    m_pszExtension = NULL;
    m_tCreateTime = 0;
    m_tModifyTime = 0;
    m_sSize = 0;
}

File::File(const char* name)
{
    if (!name)
    {
        throw std::invalid_argument("null pointer parameter");
    }

    m_pszName = NULL;
    m_pszPath = NULL;
    m_pszType = NULL;
    m_pszHash = NULL;
    m_pszExtension = NULL;
    m_tCreateTime = 0;
    m_tModifyTime = 0;
    m_sSize = 0;

    const char* pName = strrchr(name, '/');
    if (!pName)
    {
        throw std::invalid_argument("invalid path");
    }

    size_t pPathLen = (pName - name) + 1;  // 包含行末的'/'
    size_t pNameLen = strlen(pName) - 1;   // 因为多了一个/在开头

    m_pszName = new char[pNameLen + 1];
    m_pszPath = new char[pPathLen + 1];
    if ((!m_pszPath) || (!m_pszName))
    {
        goto FREE;
    }
    goto NORMAL;

FREE:
    if (m_pszPath)
    {
        delete[] m_pszPath;
    }

    if (m_pszName)
    {
        delete[] m_pszName;
    }

    if (m_pszName)
    {
        delete[] m_pszHash;
    }
    throw std::bad_alloc();

NORMAL:
    memset(m_pszName, 0, pNameLen + 1);
    memset(m_pszPath, 0, pPathLen + 1);

    strncpy(m_pszPath, name, pPathLen);
    m_pszPath[pPathLen] = '\0';

    strncpy(m_pszName, pName + 1, pNameLen);
    m_pszName[pNameLen] = '\0';

    struct stat file;

    if (0 == stat(name, &file))
    {
        const char* pszMD5 = calculateMD5(name);
        m_pszHash = new char[strlen(pszMD5) + 1];
        memset(m_pszHash, 0, strlen(pszMD5) + 1);
        strncpy(m_pszHash, pszMD5, strlen(pszMD5));
        m_pszHash[strlen(pszMD5)] = '\0';

#ifdef TEST
        LOG(DEBUG) << "\n"
            << "FILEINFO:\t" << name << "\n"
            << "Path:\t" << m_pszPath << "\n"
            << "Name:\t" << m_pszName << "\n"
            << "Md5:\t" << m_pszHash;
#endif
    }
}

File::~File()
{
    if (m_pszName)
    {
        delete[] m_pszName;
        m_pszName = nullptr;
    }

    if (m_pszPath)
    {
        delete[] m_pszPath;
        m_pszPath = nullptr;
    }

    if (m_pszType)
    {
        delete[] m_pszType;
        m_pszType = nullptr;
    }

    if (m_pszHash)
    {
        delete[] m_pszHash;
        m_pszHash = nullptr;
    }

    if (m_pszExtension)
    {
        delete[] m_pszExtension;
        m_pszExtension = nullptr;
    }
}

File::File(const File& file)
{
    m_pszName = new char[strlen(file.m_pszName) + 1];
    m_pszPath = new char[strlen(file.m_pszPath) + 1];
    m_pszType = new char[strlen(file.m_pszType) + 1];
    m_pszHash = new char[strlen(file.m_pszHash) + 1];
    m_pszExtension = new char[strlen(file.m_pszExtension) + 1];

    if ((!m_pszName) || (!m_pszPath) || (!m_pszType) || (!m_pszHash) || (!m_pszExtension))
    {
        goto FREE;
    }
    goto NORMAL;

FREE:
    if (m_pszName)
    {
        delete[] m_pszName;
        m_pszName = nullptr;
    }

    if (m_pszPath)
    {
        delete[] m_pszPath;
        m_pszPath = nullptr;
    }

    if (m_pszType)
    {
        delete[] m_pszType;
        m_pszType = nullptr;
    }

    if (m_pszHash)
    {
        delete[] m_pszHash;
        m_pszHash = nullptr;
    }

    if (m_pszExtension)
    {
        delete[] m_pszExtension;
        m_pszExtension = nullptr;
    }

    throw std::bad_alloc();
NORMAL:
    memset(m_pszName, 0, strlen(file.m_pszName) + 1);
    memset(m_pszPath, 0, strlen(file.m_pszPath) + 1);
    memset(m_pszType, 0, strlen(file.m_pszType) + 1);
    memset(m_pszHash, 0, strlen(file.m_pszHash) + 1);
    memset(m_pszExtension, 0, strlen(file.m_pszExtension) + 1);

    memcpy(m_pszName, file.m_pszName, strlen(file.m_pszName));
    memcpy(m_pszPath, file.m_pszPath, strlen(file.m_pszPath));
    memcpy(m_pszType, file.m_pszType, strlen(file.m_pszType));
    memcpy(m_pszHash, file.m_pszHash, strlen(file.m_pszHash));
    memcpy(m_pszExtension, file.m_pszExtension, strlen(file.m_pszExtension));

    m_tCreateTime = file.m_tCreateTime;
    m_tModifyTime = file.m_tModifyTime;
    m_sSize = file.m_sSize;
}

File& File::operator= (const File& file)
{
    // this == &file
    // 这种方式比较的是两个指针是否指向同一个对象，而*this
    // == file 是比较两个对象本身是否相等
    // 如果没有类型定义的运算符重载函数，编译器会使用默认的比较运算符（operator==）来比较两个对象的成员变啊零是否相等
    // 如果没有为类定义相应的相等性运算符重载函数，编译器将会报错
    if (this == &file)
    {
        return *this;
    }

    delete[] m_pszName;
    delete[] m_pszPath;
    delete[] m_pszType;
    delete[] m_pszHash;
    delete[] m_pszExtension;

    SIZE pNameLen = strlen(file.m_pszName);
    SIZE pPathLen = strlen(file.m_pszPath);
    SIZE pTypeLen = strlen(file.m_pszType);
    SIZE pHASHLen = strlen(file.m_pszHash);
    SIZE pExtensionLen = strlen(file.m_pszExtension);

    m_pszName = new char[pNameLen + 1];
    m_pszPath = new char[pPathLen + 1];
    m_pszType = new char[pTypeLen + 1];
    m_pszHash = new char[pHASHLen + 1];
    m_pszExtension = new char[pExtensionLen + 1];

    if ((!m_pszName) || (!m_pszPath) || (!m_pszType) || (!m_pszHash) || (!m_pszExtension))
    {
        goto FREE;
    }
    goto NORMAL;

FREE:
    if (m_pszName)
    {
        delete[] m_pszName;
        m_pszName = nullptr;
    }

    if (m_pszPath)
    {
        delete[] m_pszPath;
        m_pszPath = nullptr;
    }

    if (m_pszType)
    {
        delete[] m_pszType;
        m_pszType = nullptr;
    }

    if (m_pszHash)
    {
        delete[] m_pszHash;
        m_pszHash = nullptr;
    }

    if (m_pszExtension)
    {
        delete[] m_pszExtension;
        m_pszExtension = nullptr;
    }
    throw std::bad_alloc();

NORMAL:
    strncpy(m_pszName, file.m_pszName, pNameLen);
    m_pszName[pNameLen] = '\0';

    strncpy(m_pszPath, file.m_pszPath, pPathLen);
    m_pszPath[pPathLen] = '\0';

    strncpy(m_pszType, file.m_pszType, pTypeLen);
    m_pszType[pTypeLen] = '\0';

    strncpy(m_pszHash, file.m_pszHash, pHASHLen);
    m_pszHash[pHASHLen] = '\0';

    strncpy(m_pszExtension, file.m_pszExtension, pExtensionLen);
    m_pszExtension[pExtensionLen] = '\0';

    m_tCreateTime = file.m_tCreateTime;
    m_tModifyTime = file.m_tModifyTime;
    m_sSize = file.m_sSize;

    return *this;
}

bool File::operator== (const File& file)
{
    if (this == &file)
    {
        return true;
    }

    if ((!file.m_pszHash) || (!this->m_pszHash))
    {
        return false;
    }

    if (0 != strcmp(file.m_pszHash, this->m_pszHash))
    {
        return false;
    }

    return true;
}

bool File::Rename(const char* pName)
{
    bool bReturnValue = false;

    if (!pName)
    {
        //LOG(ERROR) << "parameter is null";
    }

    char* szSrcPath = (char*)malloc(strlen(this->m_pszPath) + strlen(this->m_pszName) + 1);

    sprintf(szSrcPath, "%s%s", this->m_pszPath, this->m_pszName);

    struct stat file;

    if (stat(szSrcPath, &file) != 0)
    {
        //LOG(ERROR) << "%s not exist", szSrcPath;
        return false;
    }

    char* szDstPath = (char*)malloc(strlen(this->m_pszPath) + strlen(pName) + 1);

    sprintf(szDstPath, "%s%s", this->m_pszPath, pName);

    if (rename(szSrcPath, szDstPath) != 0)
    {
        //LOG(ERROR) << "rename %s -> %s failed, errno = %d", szSrcPath, szDstPath, errno;
        return false;
    }

    //LOG(DEBUG) << "rename %s -> %s success", szSrcPath, szDstPath;
    bReturnValue = true;

    return bReturnValue;
}