/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import java.lang.ref.WeakReference;
import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;
import com.ZFFramework.Android.NativeUtil.ZFAndroidSize;
import com.ZFFramework.Android.NativeUtil.ZFAndroidUI;
import com.ZFFramework.Android.ZF.ZFEnum;
import com.ZFFramework.Android.ZF.ZFMainEntry;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;

@SuppressLint("RtlHardcoded")
public final class ZFUITextEdit extends EditText {
    public static Object native_nativeTextEditCreate(long zfjniPointerOwnerZFUITextEdit) {
        ZFUITextEdit ret = new ZFUITextEdit(ZFMainEntry.mainEntryActivity());
        ret.zfjniPointerOwnerZFUITextEdit = zfjniPointerOwnerZFUITextEdit;
        return ret;
    }
    public static void native_nativeTextEditDestroy(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.removeTextChangedListener(nativeTextEditTmp._textWatcher);
        nativeTextEditTmp._textWatcher = null;
        nativeTextEditTmp.setText(null);
        nativeTextEditTmp.setOnEditorActionListener(null);
        nativeTextEditTmp.zfjniPointerOwnerZFUITextEdit = 0;
    }

    // ============================================================
    public static void native_textEditEnableSet(Object nativeTextEdit,
                                                boolean textEditEnable) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.setEnabled(textEditEnable);
    }
    public static void native_textEditSecuredSet(Object nativeTextEdit,
                                                 boolean textEditSecured) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textEditSecured) {
            nativeTextEditTmp.setTransformationMethod(PasswordTransformationMethod.getInstance());
        }
        else {
            nativeTextEditTmp.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
        }
    }
    public static void native_textEditKeyboardTypeSet(Object nativeTextEdit,
                                                      int textEditKeyboardType) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textEditKeyboardType == ZFEnum.e("ZFUITextEditKeyboardType", "Normal")) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        }
        else if(textEditKeyboardType == ZFEnum.e("ZFUITextEditKeyboardType", "CharBased")) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        }
        else if(textEditKeyboardType == ZFEnum.e("ZFUITextEditKeyboardType", "PhonePad")) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_PHONE);
        }
        else if(textEditKeyboardType == ZFEnum.e("ZFUITextEditKeyboardType", "NumberPad")) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_NUMBER);
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }
    public static void native_textEditKeyboardReturnTypeSet(Object nativeTextEdit,
                                                            int textEditKeyboardReturnType) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textEditKeyboardReturnType == ZFEnum.e("ZFUITextEditKeyboardReturnType", "Normal")) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_NONE);
        }
        else if(textEditKeyboardReturnType == ZFEnum.e("ZFUITextEditKeyboardReturnType", "Next")) {
            nativeTextEditTmp.setImeActionLabel(null, 0);
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_NEXT);
        }
        else if(textEditKeyboardReturnType == ZFEnum.e("ZFUITextEditKeyboardReturnType", "Search")) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
        }
        else if(textEditKeyboardReturnType == ZFEnum.e("ZFUITextEditKeyboardReturnType", "Done")) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_DONE);
        }
        else if(textEditKeyboardReturnType == ZFEnum.e("ZFUITextEditKeyboardReturnType", "Go")) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_GO);
        }
        else if(textEditKeyboardReturnType == ZFEnum.e("ZFUITextEditKeyboardReturnType", "Send")) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_SEND);
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }

    public static int native_textSelectRange_start(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        return nativeTextEditTmp.getSelectionStart();
    }
    public static int native_textSelectRange_count(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        return nativeTextEditTmp.getSelectionEnd() - nativeTextEditTmp.getSelectionStart();
    }

    public static void native_textSelectRangeSet(Object nativeTextEdit, int start, int count) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(count == 0) {
            nativeTextEditTmp.setSelection(start);
        }
        else {
            nativeTextEditTmp.setSelection(start, start + count);
        }
    }

    // ============================================================
    public static void native_textSet(Object nativeTextEdit,
                                      Object nativeText) {
        ((ZFUITextEdit)nativeTextEdit).setText((String)nativeText);
    }
    public static void native_textAppearanceSet(Object nativeTextEdit,
                                                int textAppearance) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textAppearance == ZFEnum.e("ZFUITextAppearance", "Normal")) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
        else if(textAppearance == ZFEnum.e("ZFUITextAppearance", "Bold")) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        }
        else if(textAppearance == ZFEnum.e("ZFUITextAppearance", "Italic")) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.ITALIC));
        }
        else if(textAppearance == ZFEnum.e("ZFUITextAppearance", "BoldItalic")) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD_ITALIC));
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
    }
    public static void native_textAlignSet(Object nativeTextEdit,
                                           int textAlign) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if((textAlign & ZFEnum.e("ZFUIAlign", "LeftInner")) == ZFEnum.e("ZFUIAlign", "LeftInner")) {
            nativeTextEditTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
        else if((textAlign & ZFEnum.e("ZFUIAlign", "RightInner")) == ZFEnum.e("ZFUIAlign", "RightInner")) {
            nativeTextEditTmp.setGravity(Gravity.RIGHT | Gravity.CENTER_VERTICAL);
        }
        else if(textAlign == ZFEnum.e("ZFUIAlign", "Center")) {
            nativeTextEditTmp.setGravity(Gravity.CENTER);
        }
        else {
            nativeTextEditTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
    }
    public static void native_textColorSet(Object nativeTextEdit,
                                           int textColor) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.setTextColor(textColor);
    }
    public static void native_textShadowColorSet(Object nativeTextEdit,
                                                 int textShadowColor) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp._textShadowColor = textShadowColor;
        nativeTextEditTmp.setShadowLayer(
            1,
            nativeTextEditTmp._textShadowOffsetX,
            nativeTextEditTmp._textShadowOffsetY,
            nativeTextEditTmp._textShadowColor);
    }
    public static void native_textShadowOffsetSet(Object nativeTextEdit,
                                                  int textShadowOffsetX,
                                                  int textShadowOffsetY) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp._textShadowOffsetX = textShadowOffsetX;
        nativeTextEditTmp._textShadowOffsetY = textShadowOffsetY;
        nativeTextEditTmp.setShadowLayer(
            1,
            nativeTextEditTmp._textShadowOffsetX,
            nativeTextEditTmp._textShadowOffsetY,
            nativeTextEditTmp._textShadowColor);
    }
    public static void native_textSizeSet(Object nativeTextEdit,
                                          int textSize) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);
    }
    private static ZFAndroidSize _native_measureNativeTextEdit_sizeCache = new ZFAndroidSize();
    public static Object native_measureNativeTextEdit(Object nativeTextEdit,
                                                      int maxWidthOrNegative,
                                                      int maxHeightOrNegative,
                                                      int textSize) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        float savedTextSize = nativeTextEditTmp.getTextSize();
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);

        int widthOld = nativeTextEditTmp.getMeasuredWidth();
        int heightOld = nativeTextEditTmp.getMeasuredHeight();

        nativeTextEditTmp.measure(
            (maxWidthOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxWidthOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED),
            (maxHeightOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxHeightOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED));

        int padding = (int)(2 * ZFAndroidUI.screenDensity(nativeTextEditTmp.getContext()));
        _native_measureNativeTextEdit_sizeCache.set(
            nativeTextEditTmp.getMeasuredWidth() + padding,
            nativeTextEditTmp.getMeasuredHeight() + padding);
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, savedTextSize);

        // measured size must be restored, otherwise text's draw step would cause strange error
        nativeTextEditTmp.setMeasuredDimension(widthOld, heightOld);

        return _native_measureNativeTextEdit_sizeCache;
    }

    // ============================================================
    public static void native_textEditBegin(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.requestFocus();
    }
    public static void native_textEditEnd(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.clearFocus();
    }

    // ============================================================
    private static native boolean native_notifyCheckTextShouldChange(long zfjniPointerOwnerZFUITextEdit,
                                                                     Object newString);
    private static native void native_notifyTextChange(long zfjniPointerOwnerZFUITextEdit,
                                                       Object newString);
    private static native void native_notifyTextSelectRangeChange(long zfjniPointerOwnerZFUITextEdit);
    private static native void native_notifyTextReturnClicked(long zfjniPointerOwnerZFUITextEdit);
    private static native void native_notifyTextEditBegin(long zfjniPointerOwnerZFUITextEdit);
    private static native void native_notifyTextEditEnd(long zfjniPointerOwnerZFUITextEdit);

    // ============================================================
    private int _ZFP_textSelectRangeUpdateFlag = 0;
    @Override
    public void setSelection(int start, int stop) {
        ++_ZFP_textSelectRangeUpdateFlag;
        super.setSelection(start, stop);
        --_ZFP_textSelectRangeUpdateFlag;
        if(_ZFP_textSelectRangeUpdateFlag == 0) {
            ZFUITextEdit.native_notifyTextSelectRangeChange(this.zfjniPointerOwnerZFUITextEdit);
        }
    }
    @Override
    public void setSelection(int index) {
        ++_ZFP_textSelectRangeUpdateFlag;
        super.setSelection(index);
        --_ZFP_textSelectRangeUpdateFlag;
        if(_ZFP_textSelectRangeUpdateFlag == 0) {
            ZFUITextEdit.native_notifyTextSelectRangeChange(this.zfjniPointerOwnerZFUITextEdit);
        }
    }
    @Override
    protected void onFocusChanged(boolean gainFocus, int direction, Rect previouslyFocusedRect) {
        super.onFocusChanged(gainFocus, direction, previouslyFocusedRect);
        if(gainFocus) {
            ZFUITextEdit.native_notifyTextEditBegin(this.zfjniPointerOwnerZFUITextEdit);
        }
        else {
            InputMethodManager imm = (InputMethodManager)this.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(this.getWindowToken(), 0);
            ZFUITextEdit.native_notifyTextEditEnd(this.zfjniPointerOwnerZFUITextEdit);
        }
    }
    private static class _TextWatcher implements TextWatcher {
        private WeakReference<ZFUITextEdit> owner = null;
        public _TextWatcher(ZFUITextEdit owner) {
            this.owner = new WeakReference<ZFUITextEdit>(owner);
        }

        private String savedString = null;
        private boolean overrided = false;
        @Override
        public void beforeTextChanged(CharSequence s,
                                      int start,
                                      int count,
                                      int after) {
            this.savedString = s.toString();
        }
        @Override
        public void onTextChanged(CharSequence s,
                                  int start,
                                  int before,
                                  int count) {
        }
        @Override
        public void afterTextChanged(Editable s) {
            if(this.owner.get() == null || this.overrided) {
                return ;
            }

            String newString = s.toString();
            if(newString.compareTo(this.savedString) == 0) {
                return ;
            }
            if(newString.contains("\n")) {
                this.overrided = true;
                this.owner.get().setText(this.savedString);
                this.overrided = false;
                ZFUITextEdit.native_notifyTextReturnClicked(this.owner.get().zfjniPointerOwnerZFUITextEdit);
                return ;
            }
            if(!ZFUITextEdit.native_notifyCheckTextShouldChange(this.owner.get().zfjniPointerOwnerZFUITextEdit, newString)) {
                this.overrided = true;
                this.owner.get().setText(this.savedString);
                this.overrided = false;
            }
            else {
                ZFUITextEdit.native_notifyTextChange(this.owner.get().zfjniPointerOwnerZFUITextEdit, newString);
            }
        }
    }
    private long zfjniPointerOwnerZFUITextEdit = 0;
    private int _textShadowColor = Color.TRANSPARENT;
    private int _textShadowOffsetX = 1;
    private int _textShadowOffsetY = 1;
    private TextWatcher _textWatcher = null;

    // ============================================================
    @SuppressWarnings("deprecation")
    public ZFUITextEdit(Context context) {
        super(context);

        this.setTextColor(Color.BLACK);
        this.setBackgroundDrawable(null);
        this.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        this.setPadding(0, 0, 0, 0);
        this.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        this.setImeOptions(EditorInfo.IME_ACTION_NONE);
        this.setLines(1);

        _textWatcher = new _TextWatcher(this);
        this.addTextChangedListener(_textWatcher);

        this.setOnKeyListener(new OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if(keyCode == KeyEvent.KEYCODE_ENTER) {
                    if(event.getAction() == KeyEvent.ACTION_UP) {
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                    }
                    return true;
                }
                return false;
            }
        });
        this.setOnEditorActionListener(new OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                switch(actionId) {
                    case EditorInfo.IME_ACTION_NONE:
                    case EditorInfo.IME_ACTION_NEXT:
                    case EditorInfo.IME_ACTION_SEARCH:
                    case EditorInfo.IME_ACTION_DONE:
                    case EditorInfo.IME_ACTION_GO:
                    case EditorInfo.IME_ACTION_SEND:
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                        return true;
                    default:
                        break;
                }
                if(event.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                    if(event.getAction() == KeyEvent.ACTION_UP) {
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                    }
                    return true;
                }
                return false;
            }
        });
    }
    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if(event.getKeyCode() != KeyEvent.KEYCODE_ENTER) {
            return super.dispatchKeyEvent(event);
        }
        if(event.getAction() == KeyEvent.ACTION_UP) {
            ZFUITextEdit.native_notifyTextReturnClicked(this.zfjniPointerOwnerZFUITextEdit);
        }
        return true;
    }
    @Override
    public boolean dispatchKeyEventPreIme(KeyEvent event) {
        if(event.getKeyCode() != KeyEvent.KEYCODE_ENTER) {
            return super.dispatchKeyEventPreIme(event);
        }
        if(event.getAction() == KeyEvent.ACTION_UP) {
            ZFUITextEdit.native_notifyTextReturnClicked(this.zfjniPointerOwnerZFUITextEdit);
        }
        return true;
    }
}
