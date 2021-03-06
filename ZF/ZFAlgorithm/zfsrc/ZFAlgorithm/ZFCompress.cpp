/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCompress.h"
#include "protocol/ZFProtocolZFCompress.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFCompressLevel)

// ============================================================
// base api
ZFMETHOD_FUNC_DEFINE_2(ZFToken, ZFCompressBegin,
                       ZFMP_IN_OUT(const ZFOutputCallback &, outputZip),
                       ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()))
{
    if(!outputZip.callbackIsValid())
    {
        return ZFTokenInvalid();
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressBegin(outputZip, compressLevel);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFCompressEnd,
                       ZFMP_IN_OUT(ZFToken &, compressToken))
{
    if(compressToken == ZFTokenInvalid())
    {
        return zffalse;
    }
    else
    {
        ZFToken t = compressToken;
        compressToken = ZFTokenInvalid();
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressEnd(t);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFCompressContent,
                       ZFMP_IN_OUT(ZFToken, compressToken),
                       ZFMP_IN_OUT(const ZFInputCallback &, inputRaw),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(compressToken == ZFTokenInvalid() || !inputRaw.callbackIsValid() || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressContent(compressToken, inputRaw, filePathInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFCompressContentDir,
                       ZFMP_IN_OUT(ZFToken, compressToken),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(compressToken == ZFTokenInvalid() || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressContentDir(compressToken, filePathInZip);
    }
}

ZFMETHOD_FUNC_DEFINE_1(ZFToken, ZFDecompressBegin,
                       ZFMP_IN_OUT(const ZFInputCallback &, inputZip))
{
    if(!inputZip.callbackIsValid())
    {
        return ZFTokenInvalid();
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressBegin(inputZip);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFDecompressEnd,
                       ZFMP_IN_OUT(ZFToken &, decompressToken))
{
    if(decompressToken == ZFTokenInvalid())
    {
        return zffalse;
    }
    else
    {
        ZFToken t = decompressToken;
        decompressToken = ZFTokenInvalid();
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressEnd(t);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContentAtIndex,
                       ZFMP_IN_OUT(ZFToken, decompressToken),
                       ZFMP_IN_OUT(const ZFOutputCallback &, outputRaw),
                       ZFMP_IN(zfindex, fileIndexInZip))
{
    if(decompressToken == ZFTokenInvalid() || !outputRaw.callbackIsValid() || fileIndexInZip == zfindexMax())
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContent(decompressToken, outputRaw, fileIndexInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContent,
                       ZFMP_IN_OUT(ZFToken, decompressToken),
                       ZFMP_IN_OUT(const ZFOutputCallback &, outputRaw),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(decompressToken == ZFTokenInvalid() || !outputRaw.callbackIsValid() || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContent(
            decompressToken, outputRaw, ZFDecompressContentIndex(decompressToken, filePathInZip));
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFDecompressContentCount,
                       ZFMP_IN(ZFToken, decompressToken))
{
    if(decompressToken == ZFTokenInvalid())
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentCount(decompressToken);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFDecompressContentIndex,
                       ZFMP_IN(ZFToken, decompressToken),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(decompressToken == ZFTokenInvalid() || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentIndex(decompressToken, filePathInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContentPathT,
                       ZFMP_IN(ZFToken, decompressToken),
                       ZFMP_IN_OUT(zfstring &, filePathInZip),
                       ZFMP_IN(zfindex, fileIndexInZip))
{
    if(decompressToken == ZFTokenInvalid() || fileIndexInZip == zfindexMax())
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentPath(decompressToken, filePathInZip, fileIndexInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFDecompressContentPath,
                       ZFMP_IN(ZFToken, decompressToken),
                       ZFMP_IN(zfindex, fileIndexInZip))
{
    zfstring ret;
    ZFDecompressContentPathT(decompressToken, ret, fileIndexInZip);
    return ret;
}

// ============================================================
// util
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFCompress,
                       ZFMP_IN_OUT(const ZFOutputCallback &, outputZip),
                       ZFMP_IN_OUT(const ZFInputCallback &, inputRaw),
                       ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()),
                       ZFMP_IN_OPT(const zfchar *, filePathInZip, _ZFP_ZFCompressFilePathDefault))
{
    ZFToken compressToken = ZFCompressBegin(outputZip, compressLevel);
    if(compressToken == ZFTokenInvalid()) {return zffalse;}
    zfbool success = zftrue;
    success &= ZFCompressContent(compressToken, inputRaw, filePathInZip);
    success &= ZFCompressEnd(compressToken);
    return success;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompress,
                       ZFMP_IN_OUT(const ZFOutputCallback &, outputRaw),
                       ZFMP_IN_OUT(const ZFInputCallback &, inputZip),
                       ZFMP_IN_OPT(const zfchar *, filePathInZip, _ZFP_ZFCompressFilePathDefault))
{
    ZFToken decompressToken = ZFDecompressBegin(inputZip);
    if(decompressToken == ZFTokenInvalid()) {return zffalse;}
    zfbool success = zftrue;
    success &= ZFDecompressContent(decompressToken, outputRaw, filePathInZip);
    success &= ZFDecompressEnd(decompressToken);
    return success;
}

static zfbool _ZFP_ZFCompressDir(ZF_IN_OUT ZFToken compressToken,
                                 ZF_IN const ZFFilePathInfoData &fileImpl,
                                 ZF_IN const zfchar *pathType,
                                 ZF_IN const zfchar *pathData,
                                 ZF_IN const zfchar *parentPathInZip)
{
    // prepare param
    zfstring inputPath;
    if(!fileImpl.callbackPathGet(inputPath, pathData))
    {
        return zffalse;
    }
    zfstring inputName;
    if(!inputPath.isEmpty() && inputPath.compare(zfText(".")) != 0)
    {
        if(!ZFFileNameOf(inputName, inputPath))
        {
            return zffalse;
        }
    }

    zfstring filePathInZip = parentPathInZip;
    if(!filePathInZip.isEmpty())
    {
        filePathInZip += ZFFileSeparator();
    }
    filePathInZip += inputName;

    if(!filePathInZip.isEmpty())
    {
        // file
        if(fileImpl.callbackIsExist(pathData)
            && !fileImpl.callbackIsDir(pathData))
        {
            ZFInputCallback inputRaw;
            if(!ZFInputCallbackForPathInfoT(inputRaw,
                pathType, pathData,
                ZFFileOpenOption::e_Read, ZFFileBOMListEmpty()))
            {
                return zffalse;
            }
            return ZFCompressContent(compressToken, inputRaw, filePathInZip);
        }
        else
        {
            if(!ZFCompressContentDir(compressToken, filePathInZip))
            {
                return zffalse;
            }
        }
    }

    // dir
    ZFFileFindData fd;
    zfbool success = zftrue;
    if(fileImpl.callbackFindFirst(fd, pathData))
    {
        do
        {
            zfstring pathDataChild = inputPath;
            pathDataChild += ZFFileSeparator();
            pathDataChild += fd.fileName();
            if(!fileImpl.callbackPathSet(pathDataChild, pathDataChild))
            {
                success = zffalse;
                break;
            }
            if(!_ZFP_ZFCompressDir(compressToken, fileImpl, pathType, pathDataChild, filePathInZip))
            {
                success = zffalse;
                break;
            }
        } while(fileImpl.callbackFindNext(fd));
        fileImpl.callbackFindClose(fd);
    }
    return success;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFCompressDir,
                       ZFMP_IN_OUT(const ZFOutputCallback &, outputZip),
                       ZFMP_IN(const ZFPathInfo &, inputPathInfo),
                       ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()))
{
    const ZFFilePathInfoData *fileImpl = ZFFilePathInfoDataGet(inputPathInfo.pathType);
    if(fileImpl == zfnull)
    {
        return zffalse;
    }

    ZFToken compressToken = ZFCompressBegin(outputZip, compressLevel);
    if(compressToken == ZFTokenInvalid())
    {
        return zffalse;
    }

    zfbool success = zftrue;
    success &= _ZFP_ZFCompressDir(compressToken, *fileImpl, inputPathInfo.pathType, inputPathInfo.pathData, zfText(""));
    success &= ZFCompressEnd(compressToken);
    return success;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFDecompressDir,
                       ZFMP_IN(const ZFPathInfo &, outputPathInfo),
                       ZFMP_IN_OUT(const ZFInputCallback &, inputZip))
{
    const ZFFilePathInfoData *fileImpl = ZFFilePathInfoDataGet(outputPathInfo.pathType);
    if(fileImpl == zfnull)
    {
        return zffalse;
    }

    ZFToken decompressToken = ZFDecompressBegin(inputZip);
    if(decompressToken == ZFTokenInvalid())
    {
        return zffalse;
    }

    zfindex fileCountInZip = ZFDecompressContentCount(decompressToken);
    zfbool success = zftrue;
    do
    {
        zfstring outputPathParent;
        if(!fileImpl->callbackPathGet(outputPathParent, outputPathInfo.pathData))
        {
            success = zffalse;
            break;
        }

        for(zfindex fileIndexInZip = 0; fileIndexInZip < fileCountInZip; ++fileIndexInZip)
        {
            zfstring filePathInZip;
            if(!ZFDecompressContentPathT(decompressToken, filePathInZip, fileIndexInZip))
            {
                success = zffalse;
                break;
            }

            zfstring outputPath = outputPathParent;
            if(!outputPath.isEmpty())
            {
                outputPath += ZFFileSeparator();
            }
            outputPath += filePathInZip;

            if(!filePathInZip.isEmpty() && filePathInZip[filePathInZip.length() - 1] == ZFFileSeparator())
            {
                outputPath.remove(outputPath.length() - 1);
                if(!fileImpl->callbackPathSet(outputPath, outputPath)
                    || !fileImpl->callbackPathCreate(outputPath, zftrue, zfnull))
                {
                    success = zffalse;
                    break;
                }
                continue;
            }

            zfstring outputPathData;
            if(!fileImpl->callbackPathSet(outputPathData, outputPath))
            {
                success = zffalse;
                break;
            }

            ZFOutputCallback outputRaw;
            if(!ZFOutputCallbackForPathInfoT(outputRaw, outputPathInfo.pathType, outputPathData))
            {
                success = zffalse;
                break;
            }
            if(!ZFDecompressContentAtIndex(decompressToken, outputRaw, fileIndexInZip))
            {
                success = zffalse;
                break;
            }
        }
    } while(zffalse);
    success &= ZFDecompressEnd(decompressToken);
    return success;
}

ZF_NAMESPACE_GLOBAL_END

