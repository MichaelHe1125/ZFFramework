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
 * @file ZFJsonSerializableConverter.h
 * @brief serializable convertion between json and serializalbe
 */

#ifndef _ZFI_ZFJsonSerializableConverter_h_
#define _ZFI_ZFJsonSerializableConverter_h_

#include "ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFOUTPUT_TYPE(ZFJsonItem, {output << v.objectInfo();})

// ============================================================
// json serializable conversion

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <Node refType="json" refData="pathInfo, see ZFPathInfo" />
 * @endcode
 */
#define ZFSerializableDataRefType_json zfText("json")

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data:
 * @code
 *   "pathInfo, see ZFPathInfo"
 * @endcode
 * data is a pathInfo to a json file,
 * which should contain a serializable object
 */
#define ZFObjectCreatorType_json zfText("json")

// ============================================================
/**
 * @brief parse json formated data to serializable
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFSerializableDataFromJson,
                        ZFMP_OUT(ZFSerializableData &, serializableData),
                        ZFMP_IN(const ZFJsonItem &, jsonObject),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFJsonItem *, outErrorPos, zfnull))
/**
 * @brief see #ZFSerializableDataFromJson
 */
ZFMETHOD_FUNC_DECLARE_3(ZFSerializableData, ZFSerializableDataFromJson,
                        ZFMP_IN(const ZFJsonItem &, jsonObject),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFJsonItem *, outErrorPos, zfnull))
/**
 * @brief print serializable to json formated data
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFSerializableDataToJson,
                        ZFMP_OUT(ZFJsonItem &, jsonObject),
                        ZFMP_IN(const ZFSerializableData &, serializableData),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
/**
 * @brief see #ZFSerializableDataToJson
 */
ZFMETHOD_FUNC_DECLARE_3(ZFJsonItem, ZFSerializableDataToJson,
                        ZFMP_IN(const ZFSerializableData &, serializableData),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))

// ============================================================
/**
 * @brief util method to parse serializable data from input with json format
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFSerializableDataFromJson,
                        ZFMP_OUT(ZFSerializableData &, ret),
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/** @brief see #ZFSerializableDataFromJson */
ZFMETHOD_FUNC_DECLARE_2(ZFSerializableData, ZFSerializableDataFromJson,
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/**
 * @brief util method to print serializable data in json format, usually for debug use only
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFSerializableDataToJson,
                        ZFMP_IN(const ZFOutputCallback &, outputCallback),
                        ZFMP_IN(const ZFSerializableData &, serializableData),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_IN_OPT(const ZFJsonOutputFlags &, flags, ZFJsonOutputFlagsDefault()))

// ============================================================
/**
 * @brief util method to parse object from input with json format
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFObjectFromJson,
                        ZFMP_OUT(zfautoObject &, ret),
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/** @brief see #ZFObjectFromJson */
ZFMETHOD_FUNC_DECLARE_2(zfautoObject, ZFObjectFromJson,
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/**
 * @brief util method to convert serializable object to json format
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFObjectToJson,
                        ZFMP_IN(const ZFOutputCallback &, outputCallback),
                        ZFMP_IN(ZFObject *, obj),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_IN_OPT(const ZFJsonOutputFlags &, flags, ZFJsonOutputFlagsDefault()))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFJsonSerializableConverter_h_

