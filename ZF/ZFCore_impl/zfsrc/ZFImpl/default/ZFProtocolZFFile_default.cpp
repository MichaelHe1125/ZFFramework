/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFile.h"
#include "ZFCore/ZFString.h"
#include "ZFCore/ZFFile.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    #include <unistd.h>
    #include <dirent.h>
    #include <errno.h>
    #include <sys/stat.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileImpl_default, ZFFile, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("nativeAPI"))
public:
    virtual zfbool fileIsExist(ZF_IN const zfchar *path)
    {
        if(path == zfnull) {return zffalse;}
        #if ZF_ENV_sys_Windows
            return (GetFileAttributesW(ZFStringZ2W(path)) != 0xFFFFFFFF);
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            return (access(ZFStringZ2A(path), F_OK) != -1);
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    }
    virtual zfbool fileIsDir(ZF_IN const zfchar *path)
    {
        #if ZF_ENV_sys_Windows
            return ((GetFileAttributesW(ZFStringZ2W(path)) & FILE_ATTRIBUTE_DIRECTORY) != 0);
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            zfstring tmp = path;
            tmp += ZFFileSeparator();
            struct stat statbuf;
            if(lstat(ZFStringZ2A(path), &statbuf) <0) {return zffalse;}
            return S_ISDIR(statbuf.st_mode);
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    }

    virtual zfbool filePathCreate(ZF_IN const zfchar *path,
                                  ZF_IN_OPT zfbool autoMakeParent = zffalse,
                                  ZF_IN_OPT zfstring *errPos = zfnull)
    {
        if(autoMakeParent)
        {
            return this->makePath(path, errPos);
        }
        else
        {
            return this->makeDir(path, errPos);
        }
    }
private:
    zfbool cp_or_mv(ZF_IN zfbool isCopy,
                    ZF_IN const zfchar *srcPath,
                    ZF_IN const zfchar *dstPath,
                    ZF_IN_OPT zfbool isRecursive,
                    ZF_IN_OPT zfbool isForce,
                    ZF_IN_OPT zfstring *errPos)
    {
        if(srcPath == zfnull || dstPath == zfnull)
        {
            zfself::SetErrPos(errPos, zfnull);
            return zffalse;
        }
        if(!this->fileIsExist(srcPath))
        {
            zfself::SetErrPos(errPos, srcPath);
            return zffalse;
        }
        zfbool srcIsDir = this->fileIsDir(srcPath);
        if(srcIsDir && !isRecursive)
        {
            zfself::SetErrPos(errPos, srcPath);
            return zffalse;
        }
        zfbool dstExist = this->fileIsExist(dstPath);
        zfbool dstIsDir = this->fileIsDir(dstPath);
        if(dstExist && srcIsDir != dstIsDir)
        {
            zfself::SetErrPos(errPos, dstPath);
            return zffalse;
        }

        if(!this->makePath(dstPath, errPos, !srcIsDir))
        {
            return zffalse;
        }
        if(srcIsDir)
        {
            return this->copyOrMoveDir(isCopy, srcPath, dstPath, isForce, errPos);
        }
        else
        {
            if(isCopy)
            {
                return this->copyFile(srcPath, dstPath, isForce, errPos);
            }
            else
            {
                return this->moveFile(srcPath, dstPath, isForce, errPos);
            }
        }
    }
public:
    virtual zfbool fileCopy(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull)
    {
        zfstring dstPathParent;
        if(ZFFilePathParentOf(dstPathParent, dstPath))
        {
            ZFFileFilePathCreate(dstPathParent);
        }
        return this->cp_or_mv(zftrue, srcPath, dstPath, isRecursive, isForce, errPos);
    }
    virtual zfbool fileMove(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull)
    {
        return this->cp_or_mv(zffalse, srcPath, dstPath, isRecursive, isForce, errPos);
    }
    virtual zfbool fileRemove(ZF_IN const zfchar *path,
                              ZF_IN_OPT zfbool isRecursive = zftrue,
                              ZF_IN_OPT zfbool isForce = zffalse,
                              ZF_IN_OPT zfstring *errPos = zfnull)
    {
        if(!this->fileIsExist(path)) {return zftrue;}
        if(this->fileIsDir(path))
        {
            if(!isRecursive)
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
            return this->removeDir(path, isForce, errPos);
        }
        else
        {
            return this->removeFile(path, isForce, errPos);
        }
    }

    zfclassNotPOD _ZFP_ZFFileNativeFd
    {
    public:
        zfstring parentPath;
    #if ZF_ENV_sys_Windows
        _ZFP_ZFFileNativeFd(void)
        {
            zfmemset(&this->fd, 0, sizeof(WIN32_FIND_DATAW));
            this->hFind = zfnull;
        }
        WIN32_FIND_DATAW fd;
        HANDLE hFind;
        void setup(ZFFileFindData::Impl &zfd)
        {
            zfd.filePath = this->parentPath;
            zfd.filePath += ZFFileSeparator();
            zfindex t = zfd.filePath.length();
            zfd.filePath += ZFStringW2Z(fd.cFileName);
            zfd.fileName = zfd.filePath.cString() + t;

            zfd.fileIsDir = ZFFileFileIsDir(zfd.filePath.cString());
        }
    #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
        _ZFP_ZFFileNativeFd(void)
        {
            this->pDir = zfnull;
            this->pDirent = zfnull;
            zfmemset(&this->fStat, 0, sizeof(struct stat));
        }
        DIR *pDir;
        struct dirent *pDirent;
        struct stat fStat;
        void setup(ZFFileFindData::Impl &zfd)
        {
            zfd.filePath = this->parentPath;
            zfd.filePath += ZFFileSeparator();
            zfindex t = zfd.filePath.length();
            zfd.filePath += ZFStringA2Z(pDirent->d_name);
            zfd.fileName = zfd.filePath.cString() + t;

            zfd.fileIsDir = ZFFileFileIsDir(zfd.filePath.cString());
        }
    #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    };

    virtual zfbool fileFindFirst(ZF_IN_OUT ZFFileFindData::Impl &fd,
                                 ZF_IN const zfchar *path)
    {
        if(path == zfnull) {return zffalse;}
        _ZFP_ZFFileNativeFd *nativeFd = zfnew(_ZFP_ZFFileNativeFd);
        nativeFd->parentPath = path;
        fd.nativeFd = nativeFd;

        zfbool success = zffalse;
        do
        {
        #if ZF_ENV_sys_Windows
            zfstring tmp;
            tmp += path;
            tmp += ZFFileSeparator();
            tmp += '*';
            nativeFd->hFind = FindFirstFileW(
                &(nativeFd->fd),
                ZFStringZ2W(tmp.cString()));
            if(nativeFd->hFind == INVALID_HANDLE_VALUE) {break;}

            nativeFd->setup(fd);
            success = zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            nativeFd->pDir = opendir(ZFStringZ2A(path));
            if(nativeFd->pDir == zfnull) {break;}
            nativeFd->pDirent = readdir(nativeFd->pDir);
            if(nativeFd->pDirent == zfnull) {break;}

            nativeFd->setup(fd);
            success = zftrue;
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        } while(zffalse);
        if(success)
        {
            while(zfscmpTheSame(fd.fileName, zfText("."))
                || zfscmpTheSame(fd.fileName, zfText("..")))
            {
                if(!this->fileFindNext(fd))
                {
                    this->fileFindClose(fd);
                    return zffalse;
                }
            }
            return zftrue;
        }
        else
        {
            this->fileFindClose(fd);
            return zffalse;
        }
    }
    virtual zfbool fileFindNext(ZF_IN_OUT ZFFileFindData::Impl &fd)
    {
        _ZFP_ZFFileNativeFd *nativeFd = (_ZFP_ZFFileNativeFd *)fd.nativeFd;
        #if ZF_ENV_sys_Windows
            if(!FindNextFileW(nativeFd->hFind, &(nativeFd->fd))) {return zffalse;}
            nativeFd->setup(fd);
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            nativeFd->pDirent = readdir(nativeFd->pDir);
            if(nativeFd->pDirent == zfnull) {return zffalse;}
            nativeFd->setup(fd);
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        if(zfscmpTheSame(fd.fileName, zfText("."))
            || zfscmpTheSame(fd.fileName, zfText("..")))
        {
            return this->fileFindNext(fd);
        }
        return zftrue;
    }
    virtual void fileFindClose(ZF_IN_OUT ZFFileFindData::Impl &fd)
    {
        _ZFP_ZFFileNativeFd *nativeFd = (_ZFP_ZFFileNativeFd *)fd.nativeFd;

        #if ZF_ENV_sys_Windows
            if(nativeFd->hFind != zfnull)
            {
                FindClose(nativeFd->hFind);
            }
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            if(nativeFd->pDir != zfnull)
            {
                closedir(nativeFd->pDir);
            }
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

        zfdelete(nativeFd);
        fd.nativeFd = zfnull;
    }

private:
    static void SetErrPos(ZF_IN zfstring *errPos, ZF_IN const zfchar *s)
    {
        if(errPos != zfnull)
        {
            *errPos = ((s != zfnull) ? s : zfText("<null>"));
        }
    }
    zfbool makeDir(ZF_IN const zfchar *path,
                   ZF_IN_OPT zfstring *errPos)
    {
        if(this->fileIsExist(path))
        {
            if(!this->fileIsDir(path))
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
            return zftrue;
        }
        #if ZF_ENV_sys_Windows
            if(!CreateDirectoryW(ZFStringZ2W(path), zfnull))
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            if(mkdir(ZFStringZ2A(path), 0777) != 0)
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        return zftrue;
    }
    zfbool makePath(ZF_IN const zfchar *path,
                    ZF_IN_OPT zfstring *errPos,
                    ZF_IN_OPT zfbool excludeLastLevel = zffalse)
    {
        zfstring pathTmp = path;
        zfindex indexL = zfstringFindReversely(pathTmp, pathTmp.length(), ZFFileSeparatorString());
        if(indexL == zfindexMax())
        {
            if(!excludeLastLevel)
            {
                return this->makeDir(pathTmp, errPos);
            }
            return zftrue;
        }
        ZFCoreArray<zfstring> dirToMake;
        if(!excludeLastLevel)
        {
            if(this->fileIsExist(pathTmp))
            {
                if(!this->fileIsDir(pathTmp))
                {
                    zfself::SetErrPos(errPos, pathTmp);
                    return zffalse;
                }
                return zftrue;
            }
            dirToMake.add(pathTmp);
        }
        pathTmp.remove(indexL);
        do
        {
            if(this->fileIsExist(pathTmp))
            {
                if(!this->fileIsDir(pathTmp))
                {
                    zfself::SetErrPos(errPos, pathTmp);
                    return zffalse;
                }
                break;
            }
            dirToMake.add(pathTmp);
            if(indexL == zfindexMax())
            {
                break;
            }
            indexL = zfstringFindReversely(pathTmp, pathTmp.length(), ZFFileSeparatorString());
            if(indexL != zfindexMax())
            {
                pathTmp.remove(indexL);
            }
        } while(zftrue);
        for(zfindex i = dirToMake.count() - 1; i != zfindexMax(); --i)
        {
            if(!this->makeDir(dirToMake[i], errPos))
            {
                return zffalse;
            }
        }
        return zftrue;
    }
    zfbool copyFile(ZF_IN const zfchar *srcPath,
                    ZF_IN const zfchar *dstPath,
                    ZF_IN zfbool isForce,
                    ZF_IN_OPT zfstring *errPos)
    {
        if(isForce)
        {
            this->removeFile(dstPath, isForce, zfnull);
        }
        #if ZF_ENV_sys_Windows
            if(CopyFileW(ZFStringZ2W(srcPath), ZFStringZ2W(dstPath), !isForce) != TRUE)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }
            return zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
            if(this->fileIsExist(dstPath) && !isForce)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }

            FILE *fpSrc = fopen(ZFStringZ2A(srcPath), "rb");
            if(fpSrc == zfnull)
            {
                zfself::SetErrPos(errPos, srcPath);
                return zffalse;
            }
            FILE *fpDst = fopen(ZFStringZ2A(dstPath), "wb");
            if(fpDst == zfnull)
            {
                fclose(fpSrc);
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }

            size_t readSize = 0;
            #define _copyFile_bufSize 4096
            zfbyte readBuf[_copyFile_bufSize];
            while((readSize = fread(readBuf, 1, _copyFile_bufSize, fpSrc)) > 0)
            {
                fwrite(readBuf, 1, readSize, fpDst);
            }
            fclose(fpSrc);
            fclose(fpDst);
            #undef _copyFile_bufSize
            return zftrue;
        #endif // #if ZF_ENV_sys_Windows
    }
    zfbool moveFile(ZF_IN const zfchar *srcPath,
                    ZF_IN const zfchar *dstPath,
                    ZF_IN zfbool isForce,
                    ZF_IN_OPT zfstring *errPos)
    {
        if(isForce)
        {
            this->removeFile(dstPath, isForce, zfnull);
        }
        #if ZF_ENV_sys_Windows
            if(MoveFileW(ZFStringZ2W(srcPath), ZFStringZ2W(dstPath)) != TRUE)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }
            return zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            if(rename(ZFStringZ2A(srcPath), ZFStringZ2A(dstPath)) != 0)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }
            return zftrue;
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    }
    zfbool copyOrMoveDir(ZF_IN zfbool isCopy,
                         ZF_IN const zfchar *srcPath,
                         ZF_IN const zfchar *dstPath,
                         ZF_IN zfbool isForce,
                         ZF_IN_OPT zfstring *errPos)
    {
        ZFCoreArray<zfstring> stacksDirSrc;
        ZFCoreArray<zfstring> stacksDirDst;
        stacksDirSrc.add(srcPath);
        stacksDirDst.add(dstPath);

        while(stacksDirSrc.count() > 0)
        {
            zfstring srcDir = stacksDirSrc.getLast();
            zfstring dstDir = stacksDirDst.getLast();
            stacksDirSrc.removeLast();
            stacksDirDst.removeLast();

            if(!this->makePath(dstDir, errPos))
            {
                return zffalse;
            }

            ZFFileFindData::Impl fd;
            if(this->fileFindFirst(fd, srcDir.cString()))
            {
                do
                {
                    zfstring dstTmp;
                    dstTmp += dstDir;
                    dstTmp += ZFFileSeparator();
                    dstTmp += fd.fileName;
                    if(this->fileIsDir(fd.filePath))
                    {
                        stacksDirSrc.add(fd.filePath);
                        stacksDirDst.add(dstTmp.cString());
                    }
                    else
                    {
                        if(isCopy)
                        {
                            if(!this->copyFile(fd.filePath, dstTmp.cString(), isForce, errPos))
                            {
                                this->fileFindClose(fd);
                                return zffalse;
                            }
                        }
                        else
                        {
                            if(!this->moveFile(fd.filePath, dstTmp.cString(), isForce, errPos))
                            {
                                this->fileFindClose(fd);
                                return zffalse;
                            }
                        }
                    }
                } while(this->fileFindNext(fd));
                this->fileFindClose(fd);
            } // if(this->fileFindFirst(fd, srcDir.cString()))
        } // while(!stacksDirSrc.empty())

        if(!isCopy)
        {
            if(!this->removeDir(srcPath, zffalse, errPos))
            {
                return zffalse;
            }
        }

        return zftrue;
    }
    zfbool removeFile(ZF_IN const zfchar *srcPath,
                      ZF_IN zfbool isForce,
                      ZF_IN_OPT zfstring *errPos)
    {
        #if ZF_ENV_sys_Windows
            if(isForce)
            {
                SetFileAttributesW(ZFStringZ2W(srcPath), FILE_ATTRIBUTE_NORMAL);
            }
            if(DeleteFileW(ZFStringZ2W(srcPath)) != TRUE)
            {
                zfself::SetErrPos(errPos, srcPath);
                return zffalse;
            }
            return zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
            if(isForce)
            {
                chmod(ZFStringZ2A(srcPath), 0777);
            }
            if(remove(ZFStringZ2A(srcPath)) != 0)
            {
                zfself::SetErrPos(errPos, srcPath);
                return zffalse;
            }
            return zftrue;
        #endif
    }
    zfbool removeDir(ZF_IN const zfchar *srcPath,
                     ZF_IN zfbool isForce,
                     ZF_IN_OPT zfstring *errPos)
    {
        ZFCoreArray<zfstring> dirsToCheck;
        dirsToCheck.add(srcPath);
        ZFCoreArray<zfstring> emptyDirsToDel;

        // delete all child files
        while(dirsToCheck.count() > 0)
        {
            zfstring dirPath = dirsToCheck.getLast();
            dirsToCheck.removeLast();
            emptyDirsToDel.add(dirPath);

            if(isForce)
            {
                #if ZF_ENV_sys_Windows
                    SetFileAttributesW(ZFStringZ2W(dirPath.cString()), FILE_ATTRIBUTE_NORMAL);
                #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
                    chmod(ZFStringZ2A(dirPath.cString()), 0777);
                #endif
            }

            ZFFileFindData::Impl fd;
            if(this->fileFindFirst(fd, dirPath.cString()))
            {
                do
                {
                    if(this->fileIsDir(fd.filePath))
                    {
                        dirsToCheck.add(fd.filePath);
                    }
                    else
                    {
                        if(!this->removeFile(fd.filePath, isForce, errPos))
                        {
                            return zffalse;
                        }
                    }
                } while(this->fileFindNext(fd));
                this->fileFindClose(fd);
            }
        } // while(!dirsToCheck.empty())

        // delete all empty dir
        while(emptyDirsToDel.count() > 0)
        {
            zfstring pathTmp = emptyDirsToDel.getLast();
            emptyDirsToDel.removeLast();

            #if ZF_ENV_sys_Windows
                if(RemoveDirectoryW(ZFStringZ2W(pathTmp.cString())) == 0)
                {
                    zfself::SetErrPos(errPos, pathTmp.cString());
                    return zffalse;
                }
            #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
                if(rmdir(ZFStringZ2A(pathTmp.cString())) != 0)
                {
                    zfself::SetErrPos(errPos, pathTmp.cString());
                    return zffalse;
                }
            #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        } // while(!stacksDirToDel.empty())

        return zftrue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileImpl_default)

ZF_NAMESPACE_GLOBAL_END

