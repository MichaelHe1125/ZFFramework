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
 * @file zfnullObject.h
 * @brief zfnullObject
 */

#ifndef _ZFI_zfnullObject_h_
#define _ZFI_zfnullObject_h_

#include "ZFObjectCore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT ZFObject *_ZFP_zfnullObject(void);
/**
 * @brief a global null object
 *
 * useful when adding null to a container,
 * since containers doesn't allow null
 */
#define zfnullObject() _ZFP_zfnullObject()

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfnullObject_h_

