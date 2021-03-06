/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyDeclare.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFProperty *ZFPropertyGet(ZF_IN const zfchar *clsName,
                                ZF_IN const zfchar *propertyName)
{
    const ZFClass *cls = ZFClass::classForName(clsName);
    if(cls != zfnull)
    {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}
const ZFProperty *ZFPropertyGet(ZF_IN const ZFClass *cls,
                                ZF_IN const zfchar *propertyName)
{
    if(cls != zfnull)
    {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyGet, ZFMP_IN(const zfchar *, clsName), ZFMP_IN(const zfchar *, propertyName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyGet, ZFMP_IN(const ZFClass *, cls), ZFMP_IN(const zfchar *, propertyName))

ZF_NAMESPACE_GLOBAL_END
#endif

