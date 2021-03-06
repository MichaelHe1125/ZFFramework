/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_deviceUIInfoImpl_default, ZFEnvInfo_deviceUIInfo, ZFProtocolLevel::e_Default)
public:
    virtual ZFEnvDeviceUITypeEnum deviceUIType(ZF_IN_OPT ZFEnvDeviceUITypeEnum defaultValue = ZFEnvDeviceUIType::e_Desktop)
    {
        #if ZF_ENV_sys_Qt
            {
                return ZFEnvDeviceUIType::e_Desktop;
            }
        #else
            {
                return ZFEnvDeviceUIType::e_Handheld;
            }
        #endif
        return defaultValue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_deviceUIInfoImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFEnvInfo_deviceUIInfoImpl_default)

ZF_NAMESPACE_GLOBAL_END

