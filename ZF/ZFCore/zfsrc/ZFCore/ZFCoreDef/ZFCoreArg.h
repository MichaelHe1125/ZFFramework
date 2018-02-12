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
 * @file ZFCoreArg.h
 * @brief arg utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreArg_h_
#define _ZFI_ZFCoreArg_h_

#include "ZFCoreArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief split cmd line to separate item
 *
 * result would be appended to result instead of clear and set
 */
extern ZF_ENV_EXPORT void zfCoreArgSplit(ZF_OUT ZFCoreArray<zfstring> &result,
                                         ZF_IN const zfchar *cmdLine);
/**
 * @brief split cmd line to separate item
 */
extern ZF_ENV_EXPORT ZFCoreArray<zfstring> zfCoreArgSplit(ZF_IN const zfchar *cmdLine);
/**
 * @brief see #zfCoreArgSplit
 *
 * result is appended to result instead of clear and set
 */
extern ZF_ENV_EXPORT void zfCoreArgMerge(ZF_OUT zfstring &result,
                                         ZF_IN const ZFCoreArray<zfstring> &argList);
/**
 * @brief see #zfCoreArgMerge
 */
extern ZF_ENV_EXPORT zfstring zfCoreArgMerge(ZF_IN const ZFCoreArray<zfstring> &argList);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreArg_h_

