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
 * @file ZFLangLoad.h
 * @brief language util for localization
 */

#ifndef _ZFI_ZFLangLoad_h_
#define _ZFI_ZFLangLoad_h_

#include "ZFLang.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load language data for #zfLang, see #zfLang
 *
 * can load from more than one file,
 * old one would be kept, and new one would be merged to current\n
 * the file's format should be like:
 * @code
 *   <lang>
 *       <l k="YourLangKey1" v="YourLangValue1" />
 *       <l k="YourLangKey2" v="YourLangValue2" />
 *   </lang>
 * @endcode
 * equivalent json format:
 * @code
 *   {
 *     "@lang" : [
 *       {
 *         "@l" : [],
 *         "k" : "YourLangKey1",
 *         "v" : "YourLangValue1"
 *       },
 *       {
 *         "@l" : [],
 *         "k" : "YourLangKey2",
 *         "v" : "YourLangValue2"
 *       }
 *     ]
 *   }
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, zfLangLoad,
                        ZFMP_IN(const ZFSerializableData &, serializableData),
                        ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLangLoad_h_

