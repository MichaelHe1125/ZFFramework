/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFIOCallback_util.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfindex ZFInputCallbackReadToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                                    ZF_IN_OUT const ZFInputCallback &input)
{
    zfindex size = 0;
    if(input.callbackIsValid() && output.callbackIsValid())
    {
        #define _ZFP_ZFInputCallbackReadToOutput_blockSize 256
        zfchar buf[_ZFP_ZFInputCallbackReadToOutput_blockSize] = {0};
        zfindex readCount = 0;
        zfindex writeCount = 0;
        do
        {
            readCount = input.execute(buf, _ZFP_ZFInputCallbackReadToOutput_blockSize);
            writeCount = output.execute(buf, readCount);
            size += writeCount;
            if(readCount < _ZFP_ZFInputCallbackReadToOutput_blockSize || writeCount < readCount)
            {
                break;
            }
        } while(zftrue);
        #undef _ZFP_ZFInputCallbackReadToOutput_blockSize
    }
    return size;
}

// ============================================================
// ZFIOBufferedCallbackUsingBuffer
zfclass _ZFP_ZFIOBufferedCallbackUsingBufferOutputIOOwner : zfextends ZFObject
{
public:
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferedCallbackUsingBufferOutputIOOwner, ZFObject)
public:
    _ZFP_ZFIOBufferedCallbackUsingBufferPrivate *d;

    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos));
    ZFMETHOD_DECLARE_0(zfindex, ioTell);
    ZFMETHOD_DECLARE_0(zfindex, ioSize);
};
zfclass _ZFP_ZFIOBufferedCallbackUsingBufferPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferedCallbackUsingBufferPrivate, ZFObject)

public:
    zfstringA ioBuf;
    zfindex inputIndex;
    zfindex outputIndex;
    _ZFP_ZFIOBufferedCallbackUsingBufferOutputIOOwner *outputIOOwner;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->inputIndex = 0;
        this->outputIndex = 0;
        this->outputIOOwner = zfAlloc(_ZFP_ZFIOBufferedCallbackUsingBufferOutputIOOwner);
        this->outputIOOwner->d = this;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->outputIOOwner->d = zfnull;
        zfRelease(this->outputIOOwner);
        zfsuper::objectOnDealloc();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput,
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, count))
    {
        if(buf == zfnull)
        {
            return this->ioBuf.length() - this->inputIndex;
        }
        else
        {
            count = zfmMin(count, this->ioBuf.length() - this->inputIndex);
            zfmemcpy(buf, this->ioBuf.cString() + this->inputIndex, count);
            this->inputIndex += count;
            return count;
        }
    }
    ZFMETHOD_DECLARE_2(zfindex, onOutput,
                       ZFMP_IN(const void *, buf),
                       ZFMP_IN(zfindex, count))
    {
        if(count == zfindexMax())
        {
            count = zfslen((const zfchar *)buf) * sizeof(zfchar);
        }
        this->ioBuf.replace(this->outputIndex, zfmMin(count, this->ioBuf.length() - this->outputIndex), (const zfcharA *)buf, count);
        this->outputIndex += count;
        return count;
    }

    // input IO
    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    {
        this->inputIndex = ZFIOCallbackCalcFSeek(0, this->ioBuf.length(), this->inputIndex, byteSize, pos);
        return zftrue;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    {
        return this->inputIndex;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
    {
        return this->ioBuf.length();
    }
};
// output IO
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferedCallbackUsingBufferOutputIOOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    if(d == zfnull)
    {
        return zffalse;
    }
    else
    {
        d->outputIndex = ZFIOCallbackCalcFSeek(0, d->ioBuf.length(), d->outputIndex, byteSize, pos);
        return zftrue;
    }
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferedCallbackUsingBufferOutputIOOwner, zfindex, ioTell)
{
    if(d == zfnull)
    {
        return zfindexMax();
    }
    else
    {
        return d->outputIndex;
    }
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferedCallbackUsingBufferOutputIOOwner, zfindex, ioSize)
{
    if(d == zfnull)
    {
        return zfindexMax();
    }
    else
    {
        return d->ioBuf.length();
    }
}

/** @cond ZFPrivateDoc */
ZFIOBufferedCallbackUsingBuffer::ZFIOBufferedCallbackUsingBuffer(void)
: ZFIOBufferedCallback()
{
    d = zfAlloc(_ZFP_ZFIOBufferedCallbackUsingBufferPrivate);
}
ZFIOBufferedCallbackUsingBuffer::ZFIOBufferedCallbackUsingBuffer(ZF_IN const ZFIOBufferedCallbackUsingBuffer &ref)
: ZFIOBufferedCallback(ref)
{
    d = zfnull;
    this->operator=(ref);
}
ZFIOBufferedCallbackUsingBuffer &ZFIOBufferedCallbackUsingBuffer::operator =(ZF_IN const ZFIOBufferedCallbackUsingBuffer &ref)
{
    zfRetain(ref.d);
    zfRelease(d);
    d = ref.d;
    return *this;
}
/** @endcond */
ZFIOBufferedCallbackUsingBuffer::~ZFIOBufferedCallbackUsingBuffer(void)
{
    zfRelease(d);
    d = zfnull;
}

ZFInputCallback ZFIOBufferedCallbackUsingBuffer::inputCallback(void)
{
    ZFInputCallback ret = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBufferedCallbackUsingBufferPrivate, onInput));
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, d);
    return ret;
}
ZFOutputCallback ZFIOBufferedCallbackUsingBuffer::outputCallback(void)
{
    ZFOutputCallback ret = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBufferedCallbackUsingBufferPrivate, onOutput));
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, d->outputIOOwner);
    return ret;
}

void ZFIOBufferedCallbackUsingBuffer::resetInput(void)
{
    d->inputIndex = 0;
}
void ZFIOBufferedCallbackUsingBuffer::resetOutput(void)
{
    d->inputIndex = 0;
    d->outputIndex = 0;
    d->ioBuf.removeAll();
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, ZFInputCallbackReadToOutput, ZFMP_IN_OUT(const ZFOutputCallback &, output), ZFMP_IN_OUT(const ZFInputCallback &, input))

ZF_NAMESPACE_GLOBAL_END
#endif

