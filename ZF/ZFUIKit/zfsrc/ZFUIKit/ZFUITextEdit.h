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
 * @file ZFUITextEdit.h
 * @brief view to supply simple edit logic
 */

#ifndef _ZFI_ZFUITextEdit_h_
#define _ZFI_ZFUITextEdit_h_

#include "ZFUITextView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextEditKeyboardType
/**
 * @brief keyboard type for text input
 *
 * note this is only a hint for native keyboard to show proper input type,
 * the actual behavior depends on native keyboard\n
 * for advanced keyboard management, you should supply your own native code if necessary
 */
ZFENUM_BEGIN(ZFUITextEditKeyboardType)
    ZFENUM_VALUE(Normal) /**< @brief can input any word */
    ZFENUM_VALUE(CharBased) /**< @brief input char only */
    ZFENUM_VALUE(PhonePad) /**< @brief input phone number only */
    ZFENUM_VALUE(NumberPad) /**< @brief input number only */
ZFENUM_SEPARATOR(ZFUITextEditKeyboardType)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(CharBased)
    ZFENUM_VALUE_REGISTER(PhonePad)
    ZFENUM_VALUE_REGISTER(NumberPad)
ZFENUM_END(ZFUITextEditKeyboardType)

// ============================================================
// ZFUITextEditKeyboardReturnType
/**
 * @brief keyboard's return button's type for text input
 *
 * note this is only a hint for native keyboard to show proper input type,
 * the actual behavior depends on native keyboard\n
 * for advanced keyboard management, you should supply your own native code if necessary
 */
ZFENUM_BEGIN(ZFUITextEditKeyboardReturnType)
    ZFENUM_VALUE(Normal) /**< @brief normal return key */
    ZFENUM_VALUE(Next) /**< @brief go to next */
    ZFENUM_VALUE(Search) /**< @brief search */
    ZFENUM_VALUE(Done) /**< @brief done */
    ZFENUM_VALUE(Go) /**< @brief go */
    ZFENUM_VALUE(Send) /**< @brief send */
ZFENUM_SEPARATOR(ZFUITextEditKeyboardReturnType)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Next)
    ZFENUM_VALUE_REGISTER(Search)
    ZFENUM_VALUE_REGISTER(Done)
    ZFENUM_VALUE_REGISTER(Go)
    ZFENUM_VALUE_REGISTER(Send)
ZFENUM_END(ZFUITextEditKeyboardReturnType)

// ============================================================
// ZFUITextEditKeyboardReturnAction
/**
 * @brief action to perform when click return
 */
ZFENUM_BEGIN(ZFUITextEditKeyboardReturnAction)
    ZFENUM_VALUE(None) /**< @brief do nothing */
    ZFENUM_VALUE(Confirm) /**< @brief confirm only, keep focus and keyboard state */
    ZFENUM_VALUE(FocusNext) /**< @brief confirm and move focus to next */
    ZFENUM_VALUE(HideKeyboard) /**< @brief confirm and hide on screen keyboard if showing */
ZFENUM_SEPARATOR(ZFUITextEditKeyboardReturnAction)
    ZFENUM_VALUE_REGISTER(None)
    ZFENUM_VALUE_REGISTER(Confirm)
    ZFENUM_VALUE_REGISTER(FocusNext)
    ZFENUM_VALUE_REGISTER(HideKeyboard)
ZFENUM_END_WITH_DEFAULT(ZFUITextEditKeyboardReturnAction, ZFUITextEditKeyboardReturnAction::e_FocusNext)

zfclassFwd _ZFP_ZFUITextEditPrivate;
// ============================================================
// ZFUITextEdit
/**
 * @brief view to supply simple edit logic
 *
 * @note this view is used for simple text editing only,
 *   for simple text display, use #ZFUITextView,
 *   for rich text edit, you should supply your own implementation
 * @note this view is quite similar to #ZFUITextView,
 *   however, they have no relationship for some implementation's limitation,
 *   also, style and property copy logic not available between #ZFUITextView and #ZFUITextEdit,
 *   you should use #textStyleCopyFrom/#textStyleCopyTo instead
 * @note this view supply quite simple text edit logic,
 *   designed to be lightweighted and able to be embeded to other complex views
 */
zfclass ZF_ENV_EXPORT ZFUITextEdit : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUITextEdit, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextEdit)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     */
    ZFOBSERVER_EVENT(TextOnEditBegin)
    /**
     * @brief see #ZFObject::observerNotify
     */
    ZFOBSERVER_EVENT(TextOnEditEnd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when text is about to change\n
     * param0 is the text (as #ZFString) that about to change\n
     * param1 is a #ZFValueEditable::boolValue
     * shows whether the text should change,
     * set to false to show the text should not be changed\n
     * @note if #ZFUITextEdit::textEditFilter has been set,
     *   it would be checked first
     *   and store result to param1,
     *   you may change the value to modify the filter result
     */
    ZFOBSERVER_EVENT(TextOnChangeCheck)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when text changed,
     * param0 is the old text (as #ZFString)
     */
    ZFOBSERVER_EVENT(TextOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when return clicked
     */
    ZFOBSERVER_EVENT(TextOnReturnClick)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * util event fired when return clicked or focus lost,
     * or, you may manually notify it by #textEditNotifyConfirm
     */
    ZFOBSERVER_EVENT(TextOnEditConfirm)

public:
    // ============================================================
    // properties
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(zfbool, viewFocusable)
    {
        propertyValue = zftrue;
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeControl());
    }

public:
    /**
     * @brief whether the text is editable, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, textEditEnable, ZFPropertyInitValue(zftrue))
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(zfbool, textEditEnable);
    /**
     * @brief whether the text is secured, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, textEditSecured)
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(zfbool, textEditSecured);
    /**
     * @brief keyboard hint, see #ZFUITextEditKeyboardType, #ZFUITextEditKeyboardType::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextEditKeyboardTypeEnum, textEditKeyboardType, ZFUITextEditKeyboardType::EnumDefault())
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFUITextEditKeyboardTypeEnum, textEditKeyboardType);
    /**
     * @brief keyboard hint, see #ZFUITextEditKeyboardReturnType, #ZFUITextEditKeyboardReturnType::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextEditKeyboardReturnTypeEnum, textEditKeyboardReturnType, ZFUITextEditKeyboardReturnType::EnumDefault())
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFUITextEditKeyboardReturnTypeEnum, textEditKeyboardReturnType);
    /**
     * @brief action to perform when click return, see #ZFUITextEditKeyboardReturnAction, #ZFUITextEditKeyboardReturnAction::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextEditKeyboardReturnActionEnum, textEditKeyboardReturnAction, ZFUITextEditKeyboardReturnAction::EnumDefault())
    /**
     * @brief text place holder
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, textPlaceHolder, ZFPropertyNoInitValue)
    ZFPROPERTY_CUSTOM_ON_INIT_DECLARE(ZFUITextView *, textPlaceHolder)
    {
        zfblockedAlloc(ZFUITextView, textPlaceHolder);
        propertyValue = textPlaceHolder;
        textPlaceHolder->textColorSet(ZFUIGlobalStyle::DefaultStyle()->textColorHint());
        textPlaceHolder->textSizeSet(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
    }

    /**
     * @brief text filter, null by default
     *
     * #text must match exactly the filter,
     * otherwise text would not be changed
     * @note if text doesn't match filter while changing filter,
     *   text would be replaced to null
     * @note null or empty text would always treated as match for safe
     */
    ZFPROPERTY_RETAIN(ZFRegExp *, textEditFilter)
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFRegExp *, textEditFilter);

    /**
     * @brief text edit's cursor position
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindexRange, textSelectRange, ZFPropertyInitValue(zfindexRangeZero()))
    ZFPROPERTY_CUSTOM_ON_VERIFY_DECLARE(zfindexRange, textSelectRange);
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(zfindexRange, textSelectRange);

    /**
     * @brief whether #textEditNotifyConfirm when lost focus, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, textEditConfirmWhenLostFocus, ZFPropertyInitValue(zftrue))

public:
    /**
     * @brief text, may be null if not set
     */
    ZFPROPERTY_ASSIGN(zfstring, text)
    ZFPROPERTY_CUSTOM_ON_VERIFY_DECLARE(ZFString *, text);
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFString *, text);

    /**
     * @brief text appearance, #ZFUIGlobalStyle::textAppearance by default
     *
     * note, for some implementations and font settings, italic or bold may or may not be supported
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextAppearanceEnum, textAppearance,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textAppearance()))
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFUITextAppearanceEnum, textAppearance);

    /**
     * @brief text alignment, #ZFUIGlobalStyle::textAlign by default
     *
     * note, for some implementations, some align type may or may not be supported\n
     * usually only LeftInner, Center, RightInner would be supported
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, textAlign,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textAlign()))
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFUIAlignFlags, textAlign);

    /**
     * @brief text color, #ZFUIGlobalStyle::textColorDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColor,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textColorDefault()))
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFUIColor, textColor);

    /**
     * @brief text shadow color, #ZFUIColorTransparent by default, use transparent to disable text shadow
     *
     * note that implementation may have no text shadow support
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textShadowColor,
                                ZFPropertyInitValue(ZFUIColorTransparent()))
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFUIColor, textShadowColor);

    /**
     * @brief text shadow offset, (1, 1) by default
     *
     * note that implementation may have no text shadow support
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, textShadowOffset,
                                ZFPropertyInitValue((ZFUISizeMake(1, 1))))
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(ZFUISize, textShadowOffset);

    /**
     * @brief text size in pixel, #ZFUIGlobalStyle::textSizeNormal by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSize,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textSizeNormal()))
    ZFPROPERTY_CUSTOM_ON_UPDATE_DECLARE(zfint, textSize);

public:
    /**
     * @brief see #ZFUITextEdit
     */
    ZFMETHOD_DECLARE_1(void, textStyleCopyFrom,
                       ZFMP_IN(ZFUITextView *, src))
    {
        if(src == zfnull)
        {
            return ;
        }

        this->textSet(src->text());
        this->textAppearanceSet(src->textAppearance());
        this->textAlignSet(src->textAlign());
        this->textColorSet(src->textColor());
        this->textShadowColorSet(src->textShadowColor());
        this->textShadowOffsetSet(src->textShadowOffset());
        this->textSizeSet(src->textSize());
    }
    /**
     * @brief see #ZFUITextEdit
     */
    ZFMETHOD_DECLARE_1(void, textStyleCopyTo,
                       ZFMP_IN(ZFUITextView *, dst))
    {
        if(dst == zfnull)
        {
            return ;
        }

        dst->textSet(this->text());
        dst->textAppearanceSet(this->textAppearance());
        dst->textAlignSet(this->textAlign());
        dst->textColorSet(this->textColor());
        dst->textShadowColorSet(this->textShadowColor());
        dst->textShadowOffsetSet(this->textShadowOffset());
        dst->textSizeSet(this->textSize());
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief measure text view
     *
     * this is separate from UIView's measure step,
     * usually called by app to measure text's size,
     * and usually have no need to call\n
     * sizeHint shows max size the text view may have, or -1 if no limit
     */
    ZFMETHOD_DECLARE_2(void, measureTextEdit,
                       ZFMP_OUT(ZFUISize &, ret),
                       ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero()));

public:
    zffinal void _ZFP_ZFUITextEdit_textNotifyBeginEdit(void);
    zffinal void _ZFP_ZFUITextEdit_textNotifyEndEdit(void);
    zffinal void _ZFP_ZFUITextEdit_textNotifyChange(ZF_IN const zfchar *newText);
    zffinal void _ZFP_ZFUITextEdit_textSelectRangeNotifyChange(void);
    zffinal void _ZFP_ZFUITextEdit_textNotifyReturnClicked(void);
    /**
     * @brief whether text should change
     *
     * by default, this method would check accorrding to #ZFUITextEdit::textEditFilter,
     * and null or empty string would always treated as allowed for safe
     */
    ZFMETHOD_DECLARE_1(zfbool, textShouldChange,
                       ZFMP_IN(const zfchar *, newText));
    /**
     * @brief manually start edit
     */
    ZFMETHOD_DECLARE_0(void, textEditBegin);
    /**
     * @brief manually start edit
     */
    ZFMETHOD_DECLARE_0(void, textEditEnd);
    /**
     * @brief true if editing text, typically keyboard is showing
     */
    ZFMETHOD_DECLARE_0(zfbool, textEditing);

protected:
    /** @brief see #EventTextOnEditBegin */
    virtual void textOnEditBegin(void);
    /** @brief see #EventTextOnEditEnd */
    virtual void textOnEditEnd(void);
    /** @brief see #EventTextOnChangeCheck */
    virtual void textOnChangeCheck(ZF_IN const zfchar *newText, ZF_IN_OUT zfbool &shouldChange);
    /** @brief see #EventTextOnChange */
    virtual void textOnChange(ZF_IN const zfchar *oldText);
    /** @brief see #EventTextOnReturnClick */
    virtual void textOnReturnClick(void);
    /** @brief see #EventTextOnEditConfirm */
    virtual void textOnEditConfirm(void);
public:
    /** @brief see #EventTextOnEditConfirm */
    ZFMETHOD_DECLARE_0(void, textEditNotifyConfirm)
    {
        this->textOnEditConfirm();
    }

    // ============================================================
    // override
protected:
    /**
     * @brief text edit view would update text settings when scale changed
     */
    zfoverride
    virtual void scaleOnChange(void);
    /**
     * @brief text edit view would measure accorrding text size
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    /**
     * @brief text edit view would resolve all key event by default,
     *   except tab and shift
     */
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent);
    zfoverride
    virtual void viewFocusOnChange(void);

    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);
    zfoverride
    virtual void internalBgViewOnLayout(ZF_IN const ZFUIRect &bounds);

private:
    _ZFP_ZFUITextEditPrivate *d;
};

// ============================================================
ZFLANGAPPLY_DECLARE(ZFUITextEdit, ZFUITextEditPlaceHolder)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextEdit_h_

