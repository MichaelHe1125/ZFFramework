/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
package com.ZFFramework.Android.ZFCore;

import android.os.Handler;
import android.os.Message;

public final class ZFThreadTaskRequest {
    public static Handler _taskHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            ZFThreadTaskRequest.native_doExecute((Long)msg.obj);
        }
    };
    public static void native_taskRequest(long zfjniPointerToken) {
        _taskHandler.sendMessage(Message.obtain(_taskHandler, 0, Long.valueOf(zfjniPointerToken)));
    }
    private static native void native_doExecute(long zfjniPointerToken);
}
