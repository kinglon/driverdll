#include "stdafx.h"
#include "WaveFile.h"
#include <memory>
#include <shlwapi.h>

using namespace std;

CWaveFile::CWaveFile()
{

}

CWaveFile::~CWaveFile()
{

}

bool CWaveFile::BindFile(const wstring& strDestFile, const wstring& strBindFile)
{
    if (strDestFile.empty() || strBindFile.empty())
    {
        LOG_ERROR(L"invalid param");
        return false;
    }

    if (!PathFileExists(strBindFile.c_str()))
    {
        LOG_ERROR(L"bind file (%s) is not exist", strBindFile.c_str());
        return true;  //音频文件缺失，暂不认为是一个错误，便于测试
    }

    const wchar_t* pDestFile = strDestFile.c_str();
    const wchar_t* pBindFile = strBindFile.c_str();
    LOG_DEBUG(L"bind file (%s) to (%s)", pBindFile, pDestFile);

    if (!PathFileExists(strDestFile.c_str()))  //第一个文件
    {
        if (!::CopyFile(pBindFile, pDestFile, FALSE))
        {
            LOG_ERROR(L"failed to copy file (%s) to (%s), error=%d", pBindFile, pDestFile, GetLastError());
            return false;
        }
        else
        {
            return true;
        }
    }

    FILE* fpSrc = nullptr; 
    int nResult = _wfopen_s(&fpSrc, pDestFile, L"r+b") != 0;
    if (nResult != 0)
    {
        LOG_ERROR(L"failed to open file (%s), error=%d", pDestFile, nResult);
        return false;
    }

    FILE* fpBind = nullptr;    
    nResult = _wfopen_s(&fpBind, pBindFile, L"rb");
    if (nResult != 0)
    {
        LOG_ERROR(L"failed to open file (%s), error=%d", pBindFile, nResult);
        fclose(fpSrc);
        return false;
    }
    
    if (fseek(fpSrc, 40, SEEK_SET) != 0)
    {
        LOG_ERROR(L"source file size < 40");
        fclose(fpSrc);
        fclose(fpBind);
        return false;
    }

    int nSrcSize = 0;
    if (fread(&nSrcSize, 4, 1, fpSrc) != 1)
    {
        LOG_ERROR(L"source file size < 44");
        fclose(fpSrc);
        fclose(fpBind);
        return false;
    }
    
    if (fseek(fpBind, 40, SEEK_SET) != 0)
    {
        LOG_ERROR(L"bind file size < 40");
        fclose(fpSrc);
        fclose(fpBind);
        return false;
    }

    int nBindSize = 0;
    if (fread(&nBindSize, 4, 1, fpBind) != 1)
    {
        LOG_ERROR(L"bind file size < 44");
        fclose(fpSrc);
        fclose(fpBind);
        return false;
    }

    nSrcSize += nBindSize;
    fseek(fpSrc, 40, SEEK_SET);
    fwrite(&nSrcSize, 4, 1, fpSrc);

    int nFileSize = nSrcSize + 36;
    fseek(fpSrc, 4, SEEK_SET);
    fwrite(&nFileSize, 4, 1, fpSrc);

    int nPos = nFileSize + 8 - nBindSize;
    if (fseek(fpSrc, nPos, SEEK_SET) != 0)
    {
        LOG_ERROR(L"call fseek failed, pos=%d", nPos);
        fclose(fpSrc);
        fclose(fpBind);
        return false;
    }
    fseek(fpBind, 44, SEEK_SET);

    unique_ptr<char[]> pDataBlock(new char[nBindSize]);
    if (fread(pDataBlock.get(), 1, nBindSize, fpBind) != nBindSize)
    {
        LOG_ERROR(L"call fread failed");
        fclose(fpSrc);
        fclose(fpBind);
        return false;
    }

    fwrite(pDataBlock.get(), 1, nBindSize, fpSrc);

    fclose(fpSrc);
    fclose(fpBind);

    return true;
}