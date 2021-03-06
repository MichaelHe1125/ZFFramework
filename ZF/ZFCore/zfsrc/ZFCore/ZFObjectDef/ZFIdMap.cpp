/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFIdMap.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
_ZFP_ZFIdMapHolder::_ZFP_ZFIdMapHolder(ZF_IN const zfchar *moduleName, ZF_IN const zfchar *idName)
: ZFCoreLibDestroyFlag(zffalse)
, moduleName(moduleName)
, idName(idName)
, idValue(_ZFP_ZFIdMapRegister(&ZFCoreLibDestroyFlag, moduleName, idName))
{
}
_ZFP_ZFIdMapHolder::~_ZFP_ZFIdMapHolder(void)
{
    _ZFP_ZFIdMapUnregister(&ZFCoreLibDestroyFlag, moduleName, *idValue);
}

// ============================================================
zfclassLikePOD _ZFP_ZFIdMapData
{
public:
    zfuint refCount;
    zfidentity idValue;
    zfstring idName;
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;

public:
    _ZFP_ZFIdMapData(void)
    : refCount(1)
    , idValue(zfidentityInvalid())
    , idName()
    , ZFCoreLibDestroyFlag()
    {
    }
    ~_ZFP_ZFIdMapData(void)
    {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i)
        {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
    }
};
typedef zfstlmap<zfidentity, _ZFP_ZFIdMapData *> _ZFP_ZFIdMapDataIdMapType;
typedef zfstlmap<const zfchar *, _ZFP_ZFIdMapData *, zfcharConst_zfstlComparer> _ZFP_ZFIdMapDataNameMapType;

zfclassLikePOD _ZFP_ZFIdMapModuleData
{
public:
    _ZFP_ZFIdMapDataIdMapType dataIdMap;
    _ZFP_ZFIdMapDataNameMapType dataNameMap;
    ZFIdentityGenerator idValueGenerator;
};
typedef zfstlmap<zfstlstringZ, _ZFP_ZFIdMapModuleData> _ZFP_ZFIdMapModuleDataMapType;

static _ZFP_ZFIdMapModuleDataMapType &_ZFP_ZFIdMapModuleDataMap(void)
{
    static _ZFP_ZFIdMapModuleDataMapType d;
    return d;
}

const zfidentity *_ZFP_ZFIdMapRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *moduleName, ZF_IN const zfchar *idName)
{
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataMap()[moduleName];
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapData *data = zfnull;
    for(_ZFP_ZFIdMapDataIdMapType::iterator it = dataIdMap.begin(); it != dataIdMap.end(); ++it)
    {
        if(zfscmpTheSame(it->second->idName.cString(), idName))
        {
            data = it->second;
            ++data->refCount;
            break;
        }
    }
    if(data == zfnull)
    {
        data = zfnew(_ZFP_ZFIdMapData);
        data->idValue = moduleData.idValueGenerator.nextMarkUsed();
        data->idName = idName;

        dataIdMap[data->idValue] = data;
        dataNameMap[data->idName.cString()] = data;
    }
    data->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);

    return &(data->idValue);
}
void _ZFP_ZFIdMapUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *moduleName, ZF_IN zfidentity idValue)
{
    if(*ZFCoreLibDestroyFlag)
    {
        return ;
    }
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataMap()[moduleName];
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapDataIdMapType::iterator it = dataIdMap.find(idValue);
    if(it == dataIdMap.end())
    {
        zfCoreCriticalShouldNotGoHere();
        return ;
    }
    _ZFP_ZFIdMapData *data = it->second;
    data->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    --(data->refCount);
    if(data->refCount == 0)
    {
        dataIdMap.erase(data->idValue);
        dataNameMap.erase(data->idName.cString());
        zfdelete(data);
    }
}
const zfchar *ZFIdMapGetName(ZF_IN const zfchar *moduleName, ZF_IN const zfidentity &idValue)
{
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataMap()[moduleName];
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;

    _ZFP_ZFIdMapDataIdMapType::const_iterator it = dataIdMap.find(idValue);
    if(it != dataIdMap.end())
    {
        return it->second->idName.cString();
    }
    return zfnull;
}
zfidentity ZFIdMapGetId(ZF_IN const zfchar *moduleName, ZF_IN const zfchar *idName)
{
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataMap()[moduleName];
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapDataNameMapType::const_iterator it = dataNameMap.find(idName);
    if(it != dataNameMap.end())
    {
        return it->second->idValue;
    }
    return zfidentityInvalid();
}
void ZFIdMapGetAll(ZF_IN const zfchar *moduleName, ZF_OUT ZFCoreArrayPOD<zfidentity> &idValues, ZF_OUT ZFCoreArrayPOD<const zfchar *> &idNames)
{
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataMap()[moduleName];

    idValues.capacitySet(idValues.count() + moduleData.dataIdMap.size());
    idNames.capacitySet(idNames.count() + moduleData.dataIdMap.size());
    for(_ZFP_ZFIdMapDataIdMapType::iterator it = moduleData.dataIdMap.begin(); it != moduleData.dataIdMap.end(); ++it)
    {
        idValues.add(it->second->idValue);
        idNames.add(it->second->idName);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const zfchar *, ZFIdMapGetName, ZFMP_IN(const zfchar *, moduleName), ZFMP_IN(const zfidentity &, idValue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfidentity, ZFIdMapGetId, ZFMP_IN(const zfchar *, moduleName), ZFMP_IN(const zfchar *, idName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFIdMapGetAll, ZFMP_IN(const zfchar *, moduleName), ZFMP_OUT(ZFCoreArrayPOD<zfidentity> &, idValues), ZFMP_OUT(ZFCoreArrayPOD<const zfchar *> &, idNames))

ZF_NAMESPACE_GLOBAL_END
#endif

