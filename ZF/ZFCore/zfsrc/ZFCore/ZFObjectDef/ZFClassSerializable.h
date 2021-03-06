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
 * @file ZFClassSerializable.h
 * @brief serialization for ZFClass
 */

#ifndef _ZFI_ZFClassSerializable_h_
#define _ZFI_ZFClassSerializable_h_

#include "ZFClass.h"
#include "ZFSerializable.h"
#include "ZFPropertyType.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 */
ZFPROPERTY_TYPE_ACCESS_ONLY_DECLARE(ZFClassInstanceObserverAddParam, ZFClassInstanceObserverAddParam)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFClass value="ClassName" />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFClass, const ZFClass *)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClassSerializable_h_

