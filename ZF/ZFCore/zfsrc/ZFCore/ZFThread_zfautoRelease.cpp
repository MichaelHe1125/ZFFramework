/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFThread_zfautoRelease.h"
#include "ZFAutoReleasePool.h"
#include "protocol/ZFProtocolZFThread.h"
#include "protocol/ZFProtocolZFThreadTaskRequest.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoRelease
static ZFObject *_ZFP_zfautoRelease_poolDrain(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfautoRelease_poolDrainDataHolder, ZFLevelZFFrameworkEssential)
{
    this->drainTask = ZFCallbackForRawFunction(_ZFP_zfautoRelease_poolDrain);
    this->drainTaskRequested = zffalse;
}
public:
    ZFListener drainTask;
    zfbool drainTaskRequested;
ZF_GLOBAL_INITIALIZER_END(zfautoRelease_poolDrainDataHolder)
static ZFObject *_ZFP_zfautoRelease_poolDrain(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(zfautoRelease_poolDrainDataHolder)->drainTaskRequested = zffalse;
    ZFAutoReleasePool::instance()->poolDrain();
    return zfnull;
}

void _ZFP_zfautoReleaseAction(ZF_IN ZFObject *obj)
{
    if(ZFPROTOCOL_IS_AVAILABLE(ZFThread))
    {
        ZFThread *threadCur = ZFThread::currentThread();
        if(threadCur == zfnull)
        {
            zfCoreCriticalMessageTrim(zfTextA("[zfautoRelease] current thread is null, make sure the thread is started or registered by ZFThread"));
            return ;
        }
        threadCur->_ZFP_ZFThreadAutoReleasePoolAdd(obj);
    }
    else
    {
        ZFAutoReleasePool::instance()->poolAdd(obj);

        if(ZFPROTOCOL_IS_AVAILABLE(ZFThreadTaskRequest))
        {
            ZF_GLOBAL_INITIALIZER_CLASS(zfautoRelease_poolDrainDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfautoRelease_poolDrainDataHolder);
            if(!d->drainTaskRequested)
            {
                d->drainTaskRequested = zftrue;
                ZFThreadTaskRequest(d->drainTask);
            }
        }
        else
        {
            zfCoreLogTrim(zfTextA("[zfautoRelease] zfautoRelease called while no auto drain logic support, object %s would not be released normally"),
                zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

