/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFProtocolZFCompress.h
 * @brief protocol for ZFCompress
 */

#ifndef _ZFI_ZFProtocolZFCompress_h_
#define _ZFI_ZFProtocolZFCompress_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFCompress.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFCompress
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFCompress)
public:
    /** @brief see #ZFCompressBegin */
    virtual ZFToken compressBegin(ZF_IN_OUT const ZFOutputCallback &outputZip,
                                  ZF_IN ZFCompressLevelEnum compressLevel) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool compressEnd(ZF_IN_OUT ZFToken compressToken) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool compressContent(ZF_IN_OUT ZFToken compressToken,
                                   ZF_IN_OUT const ZFInputCallback &inputRaw,
                                   ZF_IN const zfchar *filePathInZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool compressContentDir(ZF_IN_OUT ZFToken compressToken,
                                      ZF_IN const zfchar *filePathInZip) zfpurevirtual;

    /** @brief see #ZFCompressBegin */
    virtual ZFToken decompressBegin(ZF_IN_OUT const ZFInputCallback &inputZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool decompressEnd(ZF_IN_OUT ZFToken decompressToken) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool decompressContent(ZF_IN_OUT ZFToken decompressToken,
                                     ZF_IN_OUT const ZFOutputCallback &outputRaw,
                                     ZF_IN zfindex fileIndexInZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfindex decompressContentCount(ZF_IN ZFToken decompressToken) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfindex decompressContentIndex(ZF_IN ZFToken decompressToken,
                                           ZF_IN const zfchar *filePathInZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool decompressContentPath(ZF_IN ZFToken decompressToken,
                                         ZF_IN_OUT zfstring &filePathInZip,
                                         ZF_IN zfindex fileIndexInZip) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFCompress)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFCompress_h_

