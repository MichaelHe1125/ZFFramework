/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLangLoad.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(zfbool, zfLangLoad,
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))
{
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &item = serializableData.elementAtIndex(i);
        if(zfscmpTheSame(item.itemClass(), zfText("l")))
        {
            const zfchar *k = item.attributeForName(zfText("k"));
            const zfchar *v = item.attributeForName(zfText("v"));
            if(k != zfnull && v != zfnull)
            {
                zfLangSet(k, v);
            }
        }
    }

    zfLangNotifyChanged();
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

