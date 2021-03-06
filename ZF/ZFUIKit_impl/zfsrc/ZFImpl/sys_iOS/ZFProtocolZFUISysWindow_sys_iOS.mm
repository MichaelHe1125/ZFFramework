/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFImpl/sys_iOS/ZFMainEntry_sys_iOS.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow : UIViewController
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUISysWindow) *impl;
@property (nonatomic, assign) ZFUISysWindow *ownerZFUISysWindow;
@property (nonatomic, assign) ZFUIOrientationEnum sysWindowOrientation;
@property (nonatomic, assign) ZFUIOrientationFlags sysWindowOrientationFlags;
@property (nonatomic, assign) BOOL _ZFP_windowResumeFlag;
- (void)_ZFP_updateLayout;
@end
@implementation _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow
// ============================================================
- (void)_ZFP_updateLayout
{
    if(self.ownerZFUISysWindow != zfnull)
    {
        ZFUIMargin sysWindowMargin = ZFUIMarginZero();

        UIView *tmpView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
        if([tmpView respondsToSelector:@selector(safeAreaInsets)])
        {
            NSMethodSignature *methodSig = [tmpView methodSignatureForSelector:@selector(safeAreaInsets)];
            NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSig];
            invocation.target = tmpView;
            invocation.selector = @selector(safeAreaInsets);
            [invocation invoke];
            UIEdgeInsets t = UIEdgeInsetsZero;
            [invocation getReturnValue:&t];

            sysWindowMargin.left = t.left;
            sysWindowMargin.top = t.top;
            sysWindowMargin.right = t.right;
            sysWindowMargin.bottom = t.bottom;
        }
        else
        {
            sysWindowMargin.top = [UIApplication sharedApplication].statusBarFrame.size.height;
        }

        UIView *nativeRootView = (__bridge UIView *)self.ownerZFUISysWindow->rootView()->nativeView();
        nativeRootView.frame = ZFImpl_sys_iOS_ZFUIKit_ZFUIRectToCGRect(self.impl->notifyMeasureWindow(
            self.ownerZFUISysWindow,
            ZFImpl_sys_iOS_ZFUIKit_ZFUIRectFromCGRect(self.view.bounds),
            sysWindowMargin));
    }
}
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];
    if(self)
    {
        self.sysWindowOrientation = ZFUIOrientation::e_Top;
        self.sysWindowOrientationFlags = ZFUIOrientation::e_Top;
    }
    return self;
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [super willAnimateRotationToInterfaceOrientation:toInterfaceOrientation duration:duration];
    switch(toInterfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
            self.sysWindowOrientation = ZFUIOrientation::e_Top;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            self.sysWindowOrientation = ZFUIOrientation::e_Bottom;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            self.sysWindowOrientation = ZFUIOrientation::e_Left;
            break;
        case UIInterfaceOrientationLandscapeRight:
            self.sysWindowOrientation = ZFUIOrientation::e_Right;
            break;
        default:
            self.sysWindowOrientation = ZFUIOrientation::e_Top;
            break;
    }
    if(self.ownerZFUISysWindow != zfnull)
    {
        self.impl->notifyOnRotate(self.ownerZFUISysWindow);
    }
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    switch(toInterfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
            return ZFBitTest(self.sysWindowOrientation, ZFUIOrientation::e_Top);
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            return ZFBitTest(self.sysWindowOrientation, ZFUIOrientation::e_Bottom);
            break;
        case UIInterfaceOrientationLandscapeLeft:
            return ZFBitTest(self.sysWindowOrientation, ZFUIOrientation::e_Left);
            break;
        case UIInterfaceOrientationLandscapeRight:
            return ZFBitTest(self.sysWindowOrientation, ZFUIOrientation::e_Right);
        default:
            return NO;
    }
}

- (void)viewWillLayoutSubviews
{
    [super viewWillLayoutSubviews];
    [self _ZFP_updateLayout];
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    if(self.ownerZFUISysWindow != zfnull)
    {
        if(!self._ZFP_windowResumeFlag)
        {
            self._ZFP_windowResumeFlag = YES;
            self.impl->notifyOnResume(self.ownerZFUISysWindow);
        }
    }
}
- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    if(self.ownerZFUISysWindow != zfnull)
    {
        if(self._ZFP_windowResumeFlag)
        {
            self._ZFP_windowResumeFlag = NO;
            self.impl->notifyOnPause(self.ownerZFUISysWindow);
        }
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_iOS, ZFUISysWindow, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIViewController"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        this->mainWindowOnCleanup();
        zfsuper::protocolOnDeallocPrepare();
    }
public:
    virtual ZFUISysWindow *mainWindow(void)
    {
        if(this->_mainWindow == zfnull)
        {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());
            _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = [_ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow new];
            nativeWindow.ownerZFUISysWindow = this->_mainWindow;

            this->notifyOnCreate(this->_mainWindow, (__bridge_retained void *)nativeWindow);

            ZFImpl_sys_iOS_rootWindow().rootViewController = nativeWindow;
            [ZFImpl_sys_iOS_rootWindow() makeKeyAndVisible];
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void)
    {
        if(this->_mainWindow != zfnull)
        {
            _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge_transfer _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)this->_mainWindow->nativeWindow();
            nativeWindow.ownerZFUISysWindow = zfnull;
            ZFImpl_sys_iOS_rootWindow().rootViewController = nil;

            if(this->_mainWindow->nativeWindowIsResumed())
            {
                this->notifyOnPause(this->_mainWindow);
            }
            zfblockedRelease(this->_mainWindow);
            this->notifyOnDestroy(this->_mainWindow);
            nativeWindow = nil;
        }
    }
    virtual void mainWindowOnDestroy(void)
    {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView)
    {
        _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow();

        UIView *nativeRootView = (__bridge UIView *)sysWindow->rootView()->nativeView();
        [nativeWindow.view addSubview:nativeRootView];
        [nativeWindow _ZFP_updateLayout];
        nativeParentView = (__bridge void *)nativeWindow.view;
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
    {
        UIView *nativeRootView = (__bridge UIView *)sysWindow->rootView()->nativeView();
        [nativeRootView removeFromSuperview];
    }

    virtual ZFUISysWindow *modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
    {
        ZFUISysWindow *modalWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());
        _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeModalWindow = [_ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow new];
        nativeModalWindow.ownerZFUISysWindow = modalWindow;

        [(__bridge UIViewController *)sysWindowOwner->nativeWindow() presentViewController:nativeModalWindow animated:YES completion:nil];
        this->notifyOnCreate(modalWindow, (__bridge_retained void *)nativeModalWindow);

        return modalWindow;
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish)
    {
        this->notifyOnDestroy(sysWindowToFinish);
        [(__bridge UIViewController *)sysWindowOwner->nativeWindow() dismissViewControllerAnimated:YES completion:nil];
    }

    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow)
    {
    }
    virtual void sysWindowLayoutParamOnChange(ZF_IN ZFUISysWindow *sysWindow)
    {
        [(__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow() _ZFP_updateLayout];
    }

    virtual ZFUIOrientationEnum sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow)
    {
        return ((__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow()).sysWindowOrientation;
    }
    virtual void sysWindowOrientationFlagsSet(ZF_IN ZFUISysWindow *sysWindow,
                                              ZF_IN const ZFUIOrientationFlags &flags)
    {
        ((__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow()).sysWindowOrientationFlags = flags;
    }

private:
    ZFUISysWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

