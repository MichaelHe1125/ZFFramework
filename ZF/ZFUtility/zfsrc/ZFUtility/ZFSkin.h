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
 * @file ZFSkin.h
 * @brief skin util for localization
 */

#ifndef _ZFI_ZFSkin_h_
#define _ZFI_ZFSkin_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * called when skin changed
 */
ZFOBSERVER_EVENT_GLOBAL(SkinOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
/**
 * @brief access localized skin with key,
 *   or return null if not exist
 *
 * a skin is a #ZFStyleable,
 * and skin logic is achieved by ##ZFStyleable::styleableCopyFrom\n
 * \n
 * to use localized skin, you must:
 * -  prepare your own skin file and load it manually,
 *   then store skin datas by #zfSkinSet
 * -  use this method to access localized skin
 *
 * it all depends on app to manage skin files,
 * and decide which file to use
 */
ZFMETHOD_FUNC_DECLARE_2(zfautoObject, zfSkin,
                        ZFMP_IN(const zfchar *, key),
                        ZFMP_IN_OPT(ZFStyleable *, valueDefault, zfnull))
/**
 * @brief notify skin changed
 */
ZFMETHOD_FUNC_DECLARE_0(void, zfSkinNotifyChanged)
/**
 * @brief manually change one skin data, or remove if value is null
 *
 * no event would be fired when set by this method, no thread-safe is supplied\n
 * for implementation that supply load logic, you should:
 * -# load by your own logic, and save by #zfSkinSet
 * -# notify data changed by #zfSkinNotifyChanged
 */
ZFMETHOD_FUNC_DECLARE_2(void, zfSkinSet,
                        ZFMP_IN(const zfchar *, key),
                        ZFMP_IN(ZFStyleable *, value))
/**
 * @brief similar to #zfSkinSet, but applies only if not exist
 */
ZFMETHOD_FUNC_DECLARE_2(void, zfSkinSetDefault,
                        ZFMP_IN(const zfchar *, key),
                        ZFMP_IN(ZFStyleable *, value))
/**
 * @brief unload all data (not recommended)
 */
ZFMETHOD_FUNC_DECLARE_0(void, zfSkinUnload)

/**
 * @brief for debug use only
 */
ZFMETHOD_FUNC_DECLARE_1(void, zfSkinDebug,
                        ZFMP_IN_OPT(const ZFOutputCallback &, output, ZFOutputCallbackDefault()))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSkin_h_

