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
 * @file ZFUIViewTreePrint.h
 * @brief print view tree for ZFUIView
 */

#ifndef _ZFI_ZFUIViewTreePrint_h_
#define _ZFI_ZFUIViewTreePrint_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief debug use only, to output human readable view tree info
 */
ZFMETHOD_FUNC_DECLARE_2(void, ZFUIViewTreePrint,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN_OPT(const ZFOutputCallback &, outputCallback, ZFOutputCallbackDefault()))

/**
 * @brief delay to #ZFUIViewTreePrint using #ZFThreadTaskRequest
 */
ZFMETHOD_FUNC_DECLARE_2(void, ZFUIViewTreePrintDelayed,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN_OPT(const ZFOutputCallback &, outputCallback, ZFOutputCallbackDefault()))
/**
 * @brief delay to #ZFUIViewTreePrint using #ZFThreadExecuteInMainThreadAfterDelay
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFUIViewTreePrintDelayed,
                        ZFMP_IN(zftimet, delay),
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN_OPT(const ZFOutputCallback &, outputCallback, ZFOutputCallbackDefault()))

/**
 * @brief info getter for #ZFUIViewTreePrint, see #ZFUIViewTreePrintInfoGetterSet
 */
typedef void (*ZFUIViewTreePrintInfoGetter)(ZF_IN ZFUIView *view,
                                            ZF_IN_OUT const ZFOutputCallback &outputCallback);
/**
 * @brief register a custom info getter for output info of a view type using #ZFUIViewTreePrint
 *
 * output should contain only infomations without extra new line or tokens,
 * newly set would override old one, set null to remove\n
 * you may add your own at runtime, using #ZF_GLOBAL_INITIALIZER_INIT is recommended\n
 * if a view class not registered, only #ZFObject::objectInfoOnAppend would be outputed
 * \n
 * if both base class and child class registered,
 * child class's one would have higher priority,
 * without parent's one being called
 */
extern ZF_ENV_EXPORT void ZFUIViewTreePrintInfoGetterSet(ZF_IN const ZFClass *viewClass,
                                                         ZF_IN ZFUIViewTreePrintInfoGetter viewInfoGetter);
/**
 * @brief see #ZFUIViewTreePrintInfoGetterSet
 */
extern ZF_ENV_EXPORT ZFUIViewTreePrintInfoGetter ZFUIViewTreePrintInfoGetterGet(ZF_IN const ZFClass *viewClass);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewTreePrint_h_

